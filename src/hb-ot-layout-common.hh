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

using hb_ot_layout_द्विआधारी_cache_t = hb_cache_t<14, 1, 8>;
static_assert (परिमाण (hb_ot_layout_द्विआधारी_cache_t) == 256, "");

#अन्तर्भाव "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


नामाकाश OT {

ढाँचा<typename Iterator>
स्थिर अन्तःस्थ तार्किक ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

स्थिर तार्किक ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    अचर hb_set_t &klasses,
    तार्किक use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

संरचना ItemVarStoreInstancer;

संरचना hb_collect_feature_substitutes_with_var_context_t
{
  अचर hb_map_t *axes_index_tag_map;
  अचर hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<अचिन्हित, अचर Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  तार्किक apply;
  तार्किक variation_applied;
  तार्किक universal;
  अचिन्हित cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, अचिन्हित> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  तार्किक all_axes_pinned;
};

संरचना hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (अचर शून्य         *table_,
                              hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              अचर hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_गन्ती (0),langsys_feature_गन्ती (0) {}

  तार्किक visitScript ()
  { फर्काऊ script_गन्ती++ < HB_MAX_SCRIPTS; }

  तार्किक visitLangsys (अचिन्हित feature_गन्ती_2)
  {
    langsys_feature_गन्ती += feature_गन्ती_2;
    फर्काऊ langsys_feature_गन्ती < HB_MAX_LANGSYS_FEATURE_गन्ती;
  }

  सार्वजनिक:
  अचर शून्य *table;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  अचर hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  गुप्त:
  अचिन्हित script_गन्ती;
  अचिन्हित langsys_feature_गन्ती;
};

संरचना hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  अचर वर्ण *get_name () { फर्काऊ "SUBSET_LAYOUT"; }
  स्थिर return_t default_return_मान () { फर्काऊ hb_empty_t (); }

  तार्किक visitScript ()
  {
    फर्काऊ script_गन्ती++ < HB_MAX_SCRIPTS;
  }

  तार्किक visitLangSys ()
  {
    फर्काऊ langsys_गन्ती++ < HB_MAX_LANGSYS;
  }

  तार्किक visitFeatureIndex (पूर्णाङ्क गन्ती)
  {
    feature_index_गन्ती += गन्ती;
    फर्काऊ feature_index_गन्ती < HB_MAX_FEATURE_INDICES;
  }

  तार्किक visitLookupIndex()
  {
    lookup_index_गन्ती++;
    फर्काऊ lookup_index_गन्ती < HB_MAX_LOOKUP_VISIT_गन्ती;
  }

  hb_subset_context_t *subset_context;
  अचर hb_tag_t table_tag;
  अचर hb_map_t *lookup_index_map;
  अचर hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  अचर hb_map_t *feature_index_map;
  अचर hb_map_t *feature_map_w_duplicates;
  अचर hb_hashmap_t<अचिन्हित, अचर Feature*> *feature_substitutes_map;
  अचर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  अचर hb_set_t *catch_all_record_feature_idxes;
  अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचर शून्य*, अचर शून्य*>> *feature_idx_tag_map;

  अचिन्हित cur_script_index;
  अचिन्हित cur_feature_index;
  अचिन्हित cur_feature_var_record_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_tag_t tag_) :
				subset_context (c_),
				table_tag (tag_),
				cur_script_index (0xFFFFu),
				cur_feature_index (Index::NOT_FOUND_INDEX),
				cur_feature_var_record_idx (0u),
				script_गन्ती (0),
				langsys_गन्ती (0),
				feature_index_गन्ती (0),
				lookup_index_गन्ती (0)
  {
    यदि (tag_ == HB_OT_TAG_GSUB)
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
    अन्यथा
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
  अचिन्हित script_गन्ती;
  अचिन्हित langsys_गन्ती;
  अचिन्हित feature_index_गन्ती;
  अचिन्हित lookup_index_गन्ती;
};

संरचना ItemVariationStore;
संरचना hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (अचर T &obj) { obj.collect_variation_indices (एतत्); फर्काऊ hb_empty_t (); }
  स्थिर return_t default_return_मान () { फर्काऊ hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  अचर hb_set_t *glyph_set;
  अचर hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *condition_मानहरू = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  अचर hb_set_t *glyph_set_,
					  अचर hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

ढाँचा<typename OutputArray>
संरचना subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 अचर शून्य *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  ढाँचा <typename T>
  तार्किक चालक () (T&& offset)
  {
    स्वतः snap = subset_context->serializer->snapshot ();
    स्वतः *o = out.serialize_append (subset_context->serializer);
    यदि (unlikely (!o)) फर्काऊ असत्य;
    तार्किक ret = o->serialize_subset (subset_context, offset, base);
    यदि (!ret)
    {
      out.झिक ();
      subset_context->serializer->revert (snap);
    }
    फर्काऊ ret;
  }

  गुप्त:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  अचर शून्य *base;
};


ढाँचा<typename OutputArray, typename Arg>
संरचना subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     अचर शून्य *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  ढाँचा <typename T>
  तार्किक चालक () (T&& offset)
  {
    स्वतः snap = subset_context->serializer->snapshot ();
    स्वतः *o = out.serialize_append (subset_context->serializer);
    यदि (unlikely (!o)) फर्काऊ असत्य;
    तार्किक ret = o->serialize_subset (subset_context, offset, base, arg);
    यदि (!ret)
    {
      out.झिक ();
      subset_context->serializer->revert (snap);
    }
    फर्काऊ ret;
  }

  गुप्त:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  अचर शून्य *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
संरचना
{
  ढाँचा<typename OutputArray>
  subset_offset_array_t<OutputArray>
  चालक () (hb_subset_context_t *subset_context, OutputArray& out,
	       अचर शून्य *base) अचर
  { फर्काऊ subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  ढाँचा<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  चालक () (hb_subset_context_t *subset_context, OutputArray& out,
	       अचर शून्य *base, Arg &&arg) अचर
  { फर्काऊ subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

ढाँचा<typename OutputArray>
संरचना subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 अचर शून्य *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  ढाँचा <typename T>
  शून्य
  चालक () (T&& record)
  {
    स्वतः snap = subset_layout_context->subset_context->serializer->snapshot ();
    तार्किक ret = record.subset (subset_layout_context, base);
    यदि (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अन्यथा out->len++;
  }

  गुप्त:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  अचर शून्य *base;
};

ढाँचा<typename OutputArray, typename Arg>
संरचना subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     अचर शून्य *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  ढाँचा <typename T>
  शून्य
  चालक () (T&& record)
  {
    स्वतः snap = subset_layout_context->subset_context->serializer->snapshot ();
    तार्किक ret = record.subset (subset_layout_context, base, arg);
    यदि (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अन्यथा out->len++;
  }

  गुप्त:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  अचर शून्य *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
संरचना
{
  ढाँचा<typename OutputArray>
  subset_record_array_t<OutputArray>
  चालक () (hb_subset_layout_context_t *c, OutputArray* out,
	       अचर शून्य *base) अचर
  { फर्काऊ subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  ढाँचा<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  चालक () (hb_subset_layout_context_t *c, OutputArray* out,
               अचर शून्य *base, Arg &&arg) अचर
  { फर्काऊ subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


ढाँचा<typename OutputArray>
संरचना serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         अचर शून्य *base_,
                         अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  ढाँचा <typename T>
  तार्किक चालक () (T&& record)
  {
    यदि (!serialize_context->copy (record, base, variation_map)) फर्काऊ असत्य;
    out.len++;
    फर्काऊ सत्य;
  }

  गुप्त:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  अचर शून्य *base;
  अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
संरचना
{
  ढाँचा<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  चालक () (hb_serialize_context_t *serialize_context, OutputArray& out,
               अचर शून्य *base,
               अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *variation_map) अचर
  { फर्काऊ serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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
  तार्किक intersects (अचर hb_map_t *indexes) अचर
  { फर्काऊ hb_any (*एतत्, indexes); }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    यदि (!it) फर्काऊ;
    यदि (unlikely (!c->extend_min ((*एतत्)))) फर्काऊ;

    लागि (अचर स्वतः _ : it)
    {
      यदि (!l->visitLookupIndex()) रोक;

      Index i;
      i = _;
      c->copy (i);
      एतत्->len++;
    }
  }

  अचिन्हित पूर्णाङ्क get_indexes (अचिन्हित पूर्णाङ्क start_offset,
			    अचिन्हित पूर्णाङ्क *गन्ती_2 /* IN/OUT */,
			    अचिन्हित पूर्णाङ्क *_indexes /* OUT */) अचर
  {
    यदि (गन्ती_2 && _indexes)
    {
      + एतत्->as_array ().sub_array (start_offset, गन्ती_2)
      | hb_sink (hb_array (_indexes, *गन्ती_2))
      ;
    }
    फर्काऊ एतत्->len;
  }

  शून्य add_indexes_to (hb_set_t* output /* OUT */) अचर
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
संरचना FeatureParamsSize
{
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्))) return_trace (असत्य);
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

    यदि (!designSize)
      return_trace (असत्य);
    अन्यथा यदि (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (सत्य);
    अन्यथा यदि (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (असत्य);
    अन्यथा
      return_trace (सत्य);
  }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) अचर
  { nameids_to_retain->add (subfamilyNameID); }

  तार्किक subset (hb_subset_context_t *c) अचर
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
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (एतत्));
  }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) अचर
  { nameids_to_retain->add (uiNameID); }

  तार्किक subset (hb_subset_context_t *c) अचर
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
  अचिन्हित
  get_characters (अचिन्हित start_offset, अचिन्हित *char_गन्ती, hb_codepoint_t *chars) अचर
  {
    यदि (char_गन्ती && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_गन्ती)
      | hb_sink (hb_array (chars, *char_गन्ती))
      ;
    }
    फर्काऊ characters.len;
  }

  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) अचर
  {
    यदि (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    यदि (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    यदि (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    यदि (!firstParamUILabelNameID || !अङ्क_Named_Parameters || अङ्क_Named_Parameters >= 0x7FFF)
      फर्काऊ;

    अचिन्हित last_name_id = (अचिन्हित) firstParamUILabelNameID + (अचिन्हित) अङ्क_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    return_trace ((तार्किक) c->serializer->embed (*एतत्));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  HBUINT16	अङ्क_Named_Parameters;	/* Number of named parameters. (May
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
  तार्किक sanitize (hb_sanitize_context_t *c, hb_tag_t tag) अचर
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    फर्काऊ सत्य;
#endif
    TRACE_SANITIZE (एतत्);
    यदि (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    यदि ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    यदि ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (सत्य);
  }

  शून्य collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) अचर
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    फर्काऊ;
#endif
    यदि (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      फर्काऊ (u.size.collect_name_ids (nameids_to_retain));
    यदि ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      फर्काऊ (u.stylisticSet.collect_name_ids (nameids_to_retain));
    यदि ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      फर्काऊ (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  तार्किक subset (hb_subset_context_t *c, अचर Tag* tag) अचर
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    फर्काऊ असत्य;
#endif
    TRACE_SUBSET (एतत्);
    यदि (!tag) return_trace (असत्य);
    यदि (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    यदि ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    यदि ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (असत्य);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  अचर FeatureParamsSize& get_size_params (hb_tag_t tag) अचर
  {
    यदि (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      फर्काऊ u.size;
    फर्काऊ Null (FeatureParamsSize);
  }
  अचर FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) अचर
  {
    यदि ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      फर्काऊ u.stylisticSet;
    फर्काऊ Null (FeatureParamsStylisticSet);
  }
  अचर FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) अचर
  {
    यदि ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      फर्काऊ u.characterVariants;
    फर्काऊ Null (FeatureParamsCharacterVariants);
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
  अचर शून्य *list_base;
};

संरचना Feature
{
  अचिन्हित पूर्णाङ्क get_lookup_गन्ती () अचर
  { फर्काऊ lookupIndex.len; }
  hb_tag_t get_lookup_index (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ lookupIndex[i]; }
  अचिन्हित पूर्णाङ्क get_lookup_indexes (अचिन्हित पूर्णाङ्क start_index,
				   अचिन्हित पूर्णाङ्क *lookup_गन्ती /* IN/OUT */,
				   अचिन्हित पूर्णाङ्क *lookup_tags /* OUT */) अचर
  { फर्काऊ lookupIndex.get_indexes (start_index, lookup_गन्ती, lookup_tags); }
  शून्य add_lookup_indexes_to (hb_set_t *lookup_indexes) अचर
  { lookupIndex.add_indexes_to (lookup_indexes); }

  अचर FeatureParams &get_feature_params () अचर
  { फर्काऊ एतत्+featureParams; }

  तार्किक intersects_lookup_indexes (अचर hb_map_t *lookup_indexes) अचर
  { फर्काऊ lookupIndex.intersects (lookup_indexes); }

  शून्य collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) अचर
  {
    यदि (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  तार्किक subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       अचर Tag                   *tag = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->featureParams.serialize_subset (c, featureParams, एतत्, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    यदि (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      स्वतः it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    अन्यथा
    {
      स्वतः it =
      + hb_iter (lookupIndex)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचर Record_sanitize_closure_t *closure = nullptr) अचर
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

ढाँचा <typename Type>
संरचना Record
{
  पूर्णाङ्क cmp (hb_tag_t a) अचर { फर्काऊ tag.cmp (a); }

  तार्किक subset (hb_subset_layout_context_t *c, अचर शून्य *base, अचर शून्य *f_sub = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->subset_context->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    यदि (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    अचर Feature& f = *reinterpret_cast<अचर Feature *> (f_sub);
    स्वतः *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    तार्किक ret = f.subset (c->subset_context, c, &tag);
    यदि (ret)
      s->add_link (out->offset, s->झिक_pack ());
    अन्यथा
      s->झिक_discard ();

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SANITIZE (एतत्);
    अचर Record_sanitize_closure_t closure = {tag, base};
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

ढाँचा <typename Type>
संरचना RecordArrayOf : SortedArray16Of<Record<Type>>
{
  अचर Offset16To<Type>& get_offset (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ (*एतत्)[i].offset; }
  Offset16To<Type>& get_offset (अचिन्हित पूर्णाङ्क i)
  { फर्काऊ (*एतत्)[i].offset; }
  अचर Tag& get_tag (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ (*एतत्)[i].tag; }
  अचिन्हित पूर्णाङ्क get_tags (अचिन्हित पूर्णाङ्क start_offset,
                         अचिन्हित पूर्णाङ्क *record_गन्ती /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) अचर
  {
    यदि (record_गन्ती && record_tags)
    {
      + एतत्->as_array ().sub_array (start_offset, record_गन्ती)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_गन्ती))
      ;
    }
    फर्काऊ एतत्->len;
  }
  तार्किक find_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) अचर
  {
    फर्काऊ एतत्->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

ढाँचा <typename Type>
संरचना RecordListOf : RecordArrayOf<Type>
{
  अचर Type& चालक [] (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ एतत्+एतत्->get_offset (i); }

  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + एतत्->iter ()
    | hb_apply (subset_record_array (l, out, एतत्))
    ;
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (RecordArrayOf<Type>::sanitize (c, एतत्));
  }
};

संरचना RecordListOfFeature : RecordListOf<Feature>
{
  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + hb_enumerate (*एतत्)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, एतत्] (अचर hb_pair_t<अचिन्हित, अचर Record<Feature>&>& _)
                {
                  अचर Feature *f_sub = nullptr;
                  अचर Feature **f = nullptr;
                  यदि (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, एतत्, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (सत्य);
  }
};

प्रकारनिर्देश RecordListOf<Feature> FeatureList;


संरचना LangSys
{
  अचिन्हित पूर्णाङ्क get_feature_गन्ती () अचर
  { फर्काऊ featureIndex.len; }
  hb_tag_t get_feature_index (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ featureIndex[i]; }
  अचिन्हित पूर्णाङ्क get_feature_indexes (अचिन्हित पूर्णाङ्क start_offset,
				    अचिन्हित पूर्णाङ्क *feature_गन्ती_2 /* IN/OUT */,
				    अचिन्हित पूर्णाङ्क *feature_indexes /* OUT */) अचर
  { फर्काऊ featureIndex.get_indexes (start_offset, feature_गन्ती_2, feature_indexes); }
  शून्य add_feature_indexes_to (hb_set_t *feature_indexes) अचर
  { featureIndex.add_indexes_to (feature_indexes); }

  तार्किक has_required_feature () अचर { फर्काऊ reqFeatureIndex != 0xFFFFu; }
  अचिन्हित पूर्णाङ्क get_required_feature_index () अचर
  {
    यदि (reqFeatureIndex == 0xFFFFu)
      फर्काऊ Index::NOT_FOUND_INDEX;
   फर्काऊ reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (*एतत्));
  }

  तार्किक compare (अचर LangSys& o, अचर hb_map_t *feature_index_map) अचर
  {
    यदि (reqFeatureIndex != o.reqFeatureIndex)
      फर्काऊ असत्य;

    स्वतः iter =
    + hb_iter (featureIndex)
    | hb_filter (feature_index_map)
    | hb_map (feature_index_map)
    ;

    स्वतः o_iter =
    + hb_iter (o.featureIndex)
    | hb_filter (feature_index_map)
    | hb_map (feature_index_map)
    ;

    लागि (; iter && o_iter; iter++, o_iter++)
    {
      अचिन्हित a = *iter;
      अचिन्हित b = *o_iter;
      यदि (a != b) फर्काऊ असत्य;
    }

    यदि (iter || o_iter) फर्काऊ असत्य;

    फर्काऊ सत्य;
  }

  शून्य collect_features (hb_prune_langsys_context_t *c) अचर
  {
    यदि (!has_required_feature () && !get_feature_गन्ती ()) फर्काऊ;
    यदि (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       अचर Tag                  *tag = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    अचर uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    यदि (!l->visitFeatureIndex (featureIndex.len))
      return_trace (असत्य);

    स्वतः it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    तार्किक ret = तार्किक (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचर Record_sanitize_closure_t * = nullptr) अचर
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
  अचिन्हित पूर्णाङ्क get_lang_sys_गन्ती () अचर
  { फर्काऊ langSys.len; }
  अचर Tag& get_lang_sys_tag (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ langSys.get_tag (i); }
  अचिन्हित पूर्णाङ्क get_lang_sys_tags (अचिन्हित पूर्णाङ्क start_offset,
				  अचिन्हित पूर्णाङ्क *lang_sys_गन्ती /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) अचर
  { फर्काऊ langSys.get_tags (start_offset, lang_sys_गन्ती, lang_sys_tags); }
  अचर LangSys& get_lang_sys (अचिन्हित पूर्णाङ्क i) अचर
  {
    यदि (i == Index::NOT_FOUND_INDEX) फर्काऊ get_default_lang_sys ();
    फर्काऊ एतत्+langSys[i].offset;
  }
  तार्किक find_lang_sys_index (hb_tag_t tag, अचिन्हित पूर्णाङ्क *index) अचर
  { फर्काऊ langSys.find_index (tag, index); }

  तार्किक has_default_lang_sys () अचर           { फर्काऊ defaultLangSys != 0; }
  अचर LangSys& get_default_lang_sys () अचर { फर्काऊ एतत्+defaultLangSys; }

  शून्य prune_langsys (hb_prune_langsys_context_t *c,
                      अचिन्हित script_index) अचर
  {
    यदि (!has_default_lang_sys () && !get_lang_sys_गन्ती ()) फर्काऊ;
    यदि (!c->visitScript ()) फर्काऊ;

    यदि (!c->script_langsys_map->has (script_index))
    {
      यदि (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	फर्काऊ;
    }

    यदि (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      अचर LangSys& d = get_default_lang_sys ();
      यदि (c->visitLangsys (d.get_feature_गन्ती ())) {
        d.collect_features (c);
      }

      लागि (स्वतः _ : + hb_enumerate (langSys))
      {
        अचर LangSys& l = एतत्+_.second.offset;
        यदि (!c->visitLangsys (l.get_feature_गन्ती ())) जारी;
        यदि (l.compare (d, c->duplicate_feature_map)) जारी;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    अन्यथा
    {
      लागि (स्वतः _ : + hb_enumerate (langSys))
      {
        अचर LangSys& l = एतत्+_.second.offset;
        यदि (!c->visitLangsys (l.get_feature_गन्ती ())) जारी;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  तार्किक subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       अचर Tag                   *tag) अचर
  {
    TRACE_SUBSET (एतत्);
    यदि (!l->visitScript ()) return_trace (असत्य);
    यदि (tag && !c->plan->layout_scripts.has (*tag))
      फर्काऊ असत्य;

    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    तार्किक defaultLang = असत्य;
    यदि (has_default_lang_sys ())
    {
      c->serializer->push ();
      अचर LangSys& ls = एतत्+defaultLangSys;
      तार्किक ret = ls.subset (c, l);
      यदि (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->झिक_discard ();
	out->defaultLangSys = 0;
      }
      अन्यथा
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->झिक_pack ());
	defaultLang = सत्य;
      }
    }

    अचर hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    यदि (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (अचर Record<LangSys>& record) {फर्काऊ l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), एतत्))
      ;
    }

    return_trace (तार्किक (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 अचर Record_sanitize_closure_t * = nullptr) अचर
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
               hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    लागि (स्वतः _ : + hb_enumerate (*एतत्))
    {
      स्वतः snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      तार्किक ret = _.second.subset (l, एतत्);
      यदि (!ret) c->serializer->revert (snap);
      अन्यथा out->len++;
    }

    return_trace (सत्य);
  }
};

प्रकारनिर्देश RecordListOfScript ScriptList;



संरचना LookupFlag : HBUINT16
{
  गणना Flags {
    दायाँ_To_बायाँ		= 0x0001u,
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
  अचिन्हित पूर्णाङ्क get_subtable_गन्ती () अचर { फर्काऊ subTable.len; }

  ढाँचा <typename TSubTable>
  अचर Array16OfOffset16To<TSubTable>& get_subtables () अचर
  { फर्काऊ reinterpret_cast<अचर Array16OfOffset16To<TSubTable> &> (subTable); }
  ढाँचा <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { फर्काऊ reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  ढाँचा <typename TSubTable>
  अचर TSubTable& get_subtable (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ एतत्+get_subtables<TSubTable> ()[i]; }
  ढाँचा <typename TSubTable>
  TSubTable& get_subtable (अचिन्हित पूर्णाङ्क i)
  { फर्काऊ एतत्+get_subtables<TSubTable> ()[i]; }

  माप_प्रकार get_size () अचर
  {
    अचर HBUINT16 &markFilteringSet = StructAfter<अचर HBUINT16> (subTable);
    यदि (lookupFlag & LookupFlag::UseMarkFilteringSet)
      फर्काऊ (अचर वर्ण *) &StructAfter<अचर वर्ण> (markFilteringSet) - (अचर वर्ण *) एतत्;
    फर्काऊ (अचर वर्ण *) &markFilteringSet - (अचर वर्ण *) एतत्;
  }

  अचिन्हित पूर्णाङ्क get_type () अचर { फर्काऊ lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () अचर
  {
    अचिन्हित पूर्णाङ्क flag = lookupFlag;
    यदि (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      अचर HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    फर्काऊ flag;
  }

  ढाँचा <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    अचिन्हित पूर्णाङ्क lookup_type = get_type ();
    TRACE_DISPATCH (एतत्, lookup_type);
    अचिन्हित पूर्णाङ्क गन्ती = get_subtable_गन्ती ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      यदि (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_मान ());
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचिन्हित पूर्णाङ्क lookup_type,
		  uint32_t lookup_props,
		  अचिन्हित पूर्णाङ्क अङ्क_subtables)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    यदि (unlikely (!subTable.serialize (c, अङ्क_subtables))) return_trace (असत्य);
    यदि (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      यदि (unlikely (!c->extend (एतत्))) return_trace (असत्य);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (सत्य);
  }

  ढाँचा <typename TSubTable>
  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    अचर hb_set_t *glyphset = c->plan->glyphset_gsub ();
    अचिन्हित पूर्णाङ्क lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([एतत्, glyphset, lookup_type] (अचर Offset16To<TSubTable> &_) { फर्काऊ (एतत्+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), एतत्, lookup_type))
    ;

    यदि (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      अचर HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      यदि (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
      {
        अचिन्हित new_flag = lookupFlag;
        new_flag &= ~LookupFlag::UseMarkFilteringSet;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        out->lookupFlag = new_flag;
      }
      अन्यथा
      {
        यदि (unlikely (!c->serializer->extend (out))) return_trace (असत्य);
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
    return_trace (सत्य);
  }

  ढाँचा <typename TSubTable>
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!(c->check_struct (एतत्) && subTable.sanitize (c))) return_trace (असत्य);
    hb_barrier ();

    अचिन्हित subtables = get_subtable_गन्ती ();
    यदि (unlikely (!c->visit_subtables (subtables))) return_trace (असत्य);

    यदि (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      अचर HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      यदि (!markFilteringSet.sanitize (c)) return_trace (असत्य);
    }

    यदि (unlikely (!get_subtables<TSubTable> ().sanitize (c, एतत्, get_type ())))
      return_trace (असत्य);

    यदि (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      अचिन्हित पूर्णाङ्क type = get_subtable<TSubTable> (0).u.extension.get_type ();
      लागि (अचिन्हित पूर्णाङ्क i = 1; i < subtables; i++)
	यदि (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (असत्य);
    }
    return_trace (सत्य);
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

ढाँचा <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

ढाँचा <typename TLookup, typename OffsetType>
संरचना LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + hb_enumerate (*एतत्)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, एतत्))
    ;
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, एतत्)));
  }
};


/*
 * Coverage Table
 */


स्थिर तार्किक ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  अचर hb_set_t &klasses,
                                          तार्किक use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  यदि (!klass_map)
    फर्काऊ ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  यदि (!use_class_zero)
    klass_map->set (0, 0);

  अचिन्हित idx = klass_map->has (0) ? 1 : 0;
  लागि (अचर अचिन्हित k: klasses)
  {
    यदि (klass_map->has (k)) जारी;
    klass_map->set (k, idx);
    idx++;
  }


  यदि (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    फर्काऊ असत्य;

  लागि (अचिन्हित i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  फर्काऊ ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

ढाँचा <typename Types>
संरचना ClassDefFormat1_3
{
  मित्र संरचना ClassDef;

  गुप्त:
  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id) अचर
  {
    फर्काऊ class_मान[(अचिन्हित पूर्णाङ्क) (glyph_id - startGlyph)];
  }

  अचिन्हित get_population () अचर
  {
    फर्काऊ class_मान.len;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    यदि (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_मान.len = 0;
      return_trace (सत्य);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    अचिन्हित glyph_गन्ती = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    यदि (unlikely (!class_मान.serialize (c, glyph_गन्ती))) return_trace (असत्य);
    लागि (अचर hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      अचिन्हित idx = gid_klass_pair.first - glyph_min;
      class_मान[idx] = gid_klass_pair.second;
    }
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सत्य,
               तार्किक use_class_zero = सत्य,
               अचर Coverage* glyph_filter = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_मान.len;

    लागि (अचर hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      यदि (new_gid == HB_MAP_मान_INVALID) जारी;
      यदि (glyph_filter && !glyph_filter->has(gid)) जारी;

      अचिन्हित klass = class_मान[gid - start];
      यदि (!klass) जारी;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    यदि (use_class_zero)
    {
      अचर hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      अचिन्हित glyph_गन्ती = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_गन्ती <= glyph_and_klass.length;
    }
    यदि (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (असत्य);
    return_trace (keep_empty_table || (तार्किक) glyph_and_klass);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && class_मान.sanitize (c));
  }

  अचिन्हित cost () अचर { फर्काऊ 1; }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) अचर
  {
    अचिन्हित पूर्णाङ्क start = 0;
    अचिन्हित पूर्णाङ्क गन्ती = class_मान.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (class_मान[i])
	जारी;

      यदि (start != i)
	यदि (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  फर्काऊ असत्य;

      start = i + 1;
    }
    यदि (start != गन्ती)
      यदि (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + गन्ती)))
	फर्काऊ असत्य;

    फर्काऊ सत्य;
  }

  ढाँचा <typename set_t>
  तार्किक collect_class (set_t *glyphs, अचिन्हित klass) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = class_मान.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (class_मान[i] == klass) glyphs->add (startGlyph + i);
    फर्काऊ सत्य;
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_मान.len;
    लागि (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      यदि (class_मान[iter - start]) फर्काऊ सत्य;
    फर्काऊ असत्य;
  }
  तार्किक intersects_class (अचर hb_set_t *glyphs, अचिन्हित klass) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = class_मान.len;
    यदि (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मान_INVALID;
      यदि (!glyphs->next (&g)) फर्काऊ असत्य;
      यदि (g < startGlyph) फर्काऊ सत्य;
      g = startGlyph + गन्ती - 1;
      यदि (glyphs->next (&g)) फर्काऊ सत्य;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    अचर typename Types::HBUINT *arr = class_मान.arrayZ;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (arr[i] == klass && glyphs->has (startGlyph + i))
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  शून्य intersected_class_glyphs (अचर hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) अचर
  {
    अचिन्हित गन्ती = class_मान.len;
    यदि (klass == 0)
    {
      अचिन्हित start_glyph = startGlyph;
      लागि (uint32_t g = HB_SET_मान_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      लागि (uint32_t g = startGlyph + गन्ती - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      फर्काऊ;
    }

    लागि (अचिन्हित i = 0; i < गन्ती; i++)
      यदि (class_मान[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#यदि 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    अचिन्हित start_glyph = startGlyph;
    अचिन्हित end_glyph = start_glyph + गन्ती;
    लागि (अचिन्हित g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      यदि (class_मान.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  शून्य intersected_classes (अचर hb_set_t *glyphs, hb_set_t *intersect_classes) अचर
  {
    यदि (glyphs->is_empty ()) फर्काऊ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    यदि (!class_मान.len)
    {
      intersect_classes->add (0);
      फर्काऊ;
    }

    hb_codepoint_t end_glyph = startGlyph + class_मान.len - 1;
    यदि (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    लागि (अचर स्वतः& _ : + hb_enumerate (class_मान))
    {
      hb_codepoint_t g = startGlyph + _.first;
      यदि (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  रक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::ढाँचा ArrayOf<typename Types::HBUINT>
		class_मान;	/* Array of Class Values--one per GlyphID */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_मान);
};

ढाँचा <typename Types>
संरचना ClassDefFormat2_4
{
  मित्र संरचना ClassDef;

  गुप्त:
  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id) अचर
  {
    फर्काऊ rangeRecord.द्वा_खोज (glyph_id).value;
  }

  अचिन्हित get_population () अचर
  {
    typename Types::large_int ret = 0;
    लागि (अचर स्वतः &r : rangeRecord)
      ret += r.get_population ();
    फर्काऊ ret > अच_पूर्णांक_उच्च ? अच_पूर्णांक_उच्च : (अचिन्हित) ret;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    यदि (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (सत्य);
    }

    अचिन्हित unsorted = असत्य;
    अचिन्हित अङ्क_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    अचिन्हित prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    स्वतः *record = c->copy (range_rec);
    यदि (unlikely (!record)) return_trace (असत्य);

    लागि (अचर स्वतः gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      अचिन्हित cur_klass = gid_klass_pair.second;

      यदि (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	यदि (unlikely (cur_gid < prev_gid))
	  unsorted = सत्य;

	यदि (unlikely (!record)) रोक;
	record->last = prev_gid;
	अङ्क_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    यदि (unlikely (c->in_error ())) return_trace (असत्य);

    यदि (likely (record)) record->last = prev_gid;
    rangeRecord.len = अङ्क_ranges;

    यदि (unlikely (unsorted))
      rangeRecord.as_array ().क्विक (RangeRecord<Types>::cmp_range);

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सत्य,
               तार्किक use_class_zero = सत्य,
               अचर Coverage* glyph_filter = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    यदि (glyph_set.get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len)
	< get_population ())
    {
      लागि (hb_codepoint_t g : glyph_set)
      {
	अचिन्हित klass = get_class (g);
	यदि (!klass) जारी;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	यदि (new_gid == HB_MAP_मान_INVALID) जारी;
	यदि (glyph_filter && !glyph_filter->has (g)) जारी;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    अन्यथा
    {
      अचिन्हित अङ्क_source_glyphs = c->plan->source->get_अङ्क_glyphs ();
      लागि (स्वतः &range : rangeRecord)
      {
	अचिन्हित klass = range.value;
	यदि (!klass) जारी;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, अङ्क_source_glyphs);
	लागि (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  यदि (new_gid == HB_MAP_मान_INVALID) जारी;
	  यदि (glyph_filter && !glyph_filter->has (g)) जारी;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    अचर hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    अचिन्हित glyph_गन्ती = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_गन्ती <= glyph_and_klass.length;
    यदि (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (असत्य);
    return_trace (keep_empty_table || (तार्किक) glyph_and_klass);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (rangeRecord.sanitize (c));
  }

  अचिन्हित cost () अचर { फर्काऊ hb_bit_storage ((अचिन्हित) rangeRecord.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) अचर
  {
    लागि (स्वतः &range : rangeRecord)
      यदि (range.value)
	यदि (unlikely (!range.collect_coverage (glyphs)))
	  फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  ढाँचा <typename set_t>
  तार्किक collect_class (set_t *glyphs, अचिन्हित पूर्णाङ्क klass) अचर
  {
    लागि (स्वतः &range : rangeRecord)
    {
      यदि (range.value == klass)
	यदि (unlikely (!range.collect_coverage (glyphs)))
	  फर्काऊ असत्य;
    }
    फर्काऊ सत्य;
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  {
    यदि (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len))
    {
      लागि (स्वतः g : *glyphs)
        यदि (get_class (g))
	  फर्काऊ सत्य;
      फर्काऊ असत्य;
    }

    फर्काऊ hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (अचर RangeRecord<Types> &range) { फर्काऊ range.intersects (*glyphs) && range.value; }));
  }
  तार्किक intersects_class (अचर hb_set_t *glyphs, अचिन्हित klass) अचर
  {
    यदि (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      यदि (!rangeRecord.len)
        फर्काऊ !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मान_INVALID;
      hb_codepoint_t last = HB_SET_मान_INVALID;
      स्वतः it = hb_iter (rangeRecord);
      लागि (स्वतः &range : it)
      {
        यदि (it->first == last + 1)
	{
	  it++;
	  जारी;
	}

	यदि (!glyphs->next (&g))
	  रोक;
	यदि (g < range.first)
	  फर्काऊ सत्य;
	g = range.last;
	last = g;
      }
      यदि (g != HB_SET_मान_INVALID && glyphs->next (&g))
	फर्काऊ सत्य;
      /* Fall through. */
    }
    लागि (अचर स्वतः &range : rangeRecord)
      यदि (range.value == klass && range.intersects (*glyphs))
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  शून्य intersected_class_glyphs (अचर hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) अचर
  {
    यदि (klass == 0)
    {
      hb_codepoint_t g = HB_SET_मान_INVALID;
      लागि (स्वतः &range : rangeRecord)
      {
	यदि (!glyphs->next (&g))
	  गच्छ done;
	जबसम्म (g < range.first)
	{
	  intersect_glyphs->add (g);
	  यदि (!glyphs->next (&g))
	    गच्छ done;
        }
        g = range.last;
      }
      जबसम्म (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      फर्काऊ;
    }

    अचिन्हित गन्ती = rangeRecord.len;
    यदि (गन्ती > glyphs->get_population () * hb_bit_storage (गन्ती))
    {
      लागि (स्वतः g : *glyphs)
      {
        अचिन्हित i;
        यदि (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      फर्काऊ;
    }

    लागि (स्वतः &range : rangeRecord)
    {
      यदि (range.value != klass) जारी;

      अचिन्हित end = range.last + 1;
      लागि (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  शून्य intersected_classes (अचर hb_set_t *glyphs, hb_set_t *intersect_classes) अचर
  {
    यदि (glyphs->is_empty ()) फर्काऊ;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    यदि (!rangeRecord.len)
    {
      intersect_classes->add (0);
      फर्काऊ;
    }

    hb_codepoint_t g = HB_SET_मान_INVALID;
    लागि (स्वतः &range : rangeRecord)
    {
      यदि (!glyphs->next (&g))
        रोक;
      यदि (g < range.first)
      {
        intersect_classes->add (0);
        रोक;
      }
      g = range.last;
    }
    यदि (g != HB_SET_मान_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    लागि (अचर स्वतः& range : rangeRecord)
      यदि (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  रक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::ढाँचा SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

संरचना ClassDef
{
  /* Has interface. */
  अचिन्हित चालक [] (hb_codepoint_t k) अचर { फर्काऊ get (k); }
  तार्किक has (hb_codepoint_t k) अचर { फर्काऊ (*एतत्)[k]; }
  /* Projection. */
  hb_codepoint_t चालक () (hb_codepoint_t k) अचर { फर्काऊ get (k); }

  अचिन्हित पूर्णाङ्क get (hb_codepoint_t k) अचर { फर्काऊ get_class (k); }
  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_class (glyph_id);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.get_class (glyph_id);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.get_class (glyph_id);
#endif
    स्वभाव:फर्काऊ 0;
    }
  }
  अचिन्हित पूर्णाङ्क get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) अचर
  {
    अचिन्हित klass;
    यदि (cache && cache->get (glyph_id, &klass)) फर्काऊ klass;
    klass = get_class (glyph_id);
    यदि (cache) cache->set (glyph_id, klass);
    फर्काऊ klass;
  }

  अचिन्हित get_population () अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_population ();
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.get_population ();
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.get_population ();
#endif
    स्वभाव:फर्काऊ NOT_COVERED;
    }
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    स्वतः glyphs = + it_with_class_zero | hb_filter (hb_second);

    अचिन्हित format = 2;
    hb_codepoint_t glyph_max = 0;
    अचिन्हित class_max = 0;
    यदि (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      अचिन्हित अङ्क_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      अचिन्हित prev_klass = (*glyphs).second;

      लागि (अचर स्वतः gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	अचिन्हित cur_klass = gid_klass_pair.second;
	यदि (cur_klass > class_max) class_max = cur_klass;
	यदि (cur_gid == glyph_min) जारी;
        यदि (cur_gid > glyph_max) glyph_max = cur_gid;
	यदि (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  अङ्क_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      अचिन्हित glyph_span = glyph_max - glyph_min + 1;
      स्वतः consider = [&] (अचिन्हित candidate, uint64_t size)
      {
	यदि (size >= best_size) फर्काऊ;
	best_size = size;
	format = candidate;
      };

      यदि (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      यदि (glyph_max <= 0xFFFFu && अङ्क_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) अङ्क_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      यदि (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      यदि (glyph_max <= 0xFFFFFFu && अङ्क_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) अङ्क_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      यदि (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (असत्य, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (असत्य);
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
    स्वभाव:return_trace (असत्य);
    }
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सत्य,
               तार्किक use_class_zero = सत्य,
               अचर Coverage* glyph_filter = nullptr) अचर
  {
    TRACE_SUBSET (एतत्);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    पक्ष 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    पक्ष 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    स्वभाव:return_trace (असत्य);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पक्ष 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    पक्ष 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    स्वभाव:return_trace (सत्य);
    }
  }

  अचिन्हित cost () अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.cost ();
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.cost ();
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.cost ();
#endif
    स्वभाव:फर्काऊ 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.collect_coverage (glyphs);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.collect_coverage (glyphs);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.collect_coverage (glyphs);
#endif
    स्वभाव:फर्काऊ असत्य;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  तार्किक collect_class (set_t *glyphs, अचिन्हित पूर्णाङ्क klass) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.collect_class (glyphs, klass);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.collect_class (glyphs, klass);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.collect_class (glyphs, klass);
#endif
    स्वभाव:फर्काऊ असत्य;
    }
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.intersects (glyphs);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.intersects (glyphs);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.intersects (glyphs);
#endif
    स्वभाव:फर्काऊ असत्य;
    }
  }
  तार्किक intersects_class (अचर hb_set_t *glyphs, अचिन्हित पूर्णाङ्क klass) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.intersects_class (glyphs, klass);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.intersects_class (glyphs, klass);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.intersects_class (glyphs, klass);
#endif
    स्वभाव:फर्काऊ असत्य;
    }
  }

  शून्य intersected_class_glyphs (अचर hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    स्वभाव:फर्काऊ;
    }
  }

  शून्य intersected_classes (अचर hb_set_t *glyphs, hb_set_t *intersect_classes) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.intersected_classes (glyphs, intersect_classes);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.intersected_classes (glyphs, intersect_classes);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    स्वभाव:फर्काऊ;
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

ढाँचा<typename Iterator>
स्थिर अन्तःस्थ तार्किक ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ फर्काऊ (c->start_embed<ClassDef> ()->serialize (c, it)); }


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
    पूर्णाङ्क cmp (अचर chars_t& other) अचर
    {
      फर्काऊ as_array ().cmp (other.as_array ());
    }

    hb_pair_t<अचिन्हित, अचिन्हित> get_width ()
    {
      अचिन्हित width = 0;
      अचिन्हित columns = 0;
      लागि (अचिन्हित i = 0; i < length; i++)
      {
	अचिन्हित v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      फर्काऊ hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<अचिन्हित, अचिन्हित> combine_width (अचर chars_t& other) अचर
    {
      अचिन्हित combined_width = 0;
      अचिन्हित combined_columns = 0;
      लागि (अचिन्हित i = 0; i < length; i++)
      {
        अचिन्हित v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      फर्काऊ hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<अचिन्हित, अचिन्हित> combine_width (अचर delta_row_encoding_t& other_encoding) अचर { फर्काऊ chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  अचिन्हित width = 0;
  अचिन्हित overhead = 0;
  hb_vector_t<अचर hb_vector_t<पूर्णाङ्क>*> वस्तुहरू;

  delta_row_encoding_t () = स्वभाव;
  delta_row_encoding_t (hb_vector_t<अचर hb_vector_t<पूर्णाङ्क>*> &&rows, अचिन्हित अङ्क_cols)
  {
    प्रतिज्ञा (rows);

    वस्तुहरू = std::move (rows);

    यदि (unlikely (!chars.resize (अङ्क_cols)))
      फर्काऊ;

    calculate_chars ();
  }

  शून्य merge (अचर delta_row_encoding_t& other)
  {
    वस्तुहरू.alloc (वस्तुहरू.length + other.वस्तुहरू.length);
    लागि (स्वतः &row : other.वस्तुहरू)
      add_row (row);

    // Merge chars
    प्रतिज्ञा (chars.length == other.chars.length);
    लागि (अचिन्हित i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  शून्य chars_changed ()
  {
    स्वतः _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  शून्य calculate_chars ()
  {
    प्रतिज्ञा (वस्तुहरू);

    तार्किक long_शब्दहरू = असत्य;

    लागि (स्वतः &row : वस्तुहरू)
    {
      प्रतिज्ञा (row->length == chars.length);

      /* 0/1/2 byte encoding */
      लागि (अचिन्हित i = 0; i < row->length; i++)
      {
	पूर्णाङ्क v =  row->arrayZ[i];
	यदि (v == 0)
	  जारी;
	अन्यथा यदि (v > 32767 || v < -32768)
	{
	  long_शब्दहरू = सत्य;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	अन्यथा यदि (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	अन्यथा
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    यदि (long_शब्दहरू)
    {
      // Convert 1s to 2s
      लागि (स्वतः &v : chars)
	यदि (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  तार्किक is_empty () अचर
  { फर्काऊ !वस्तुहरू; }

  तार्किक in_error () अचर
  { फर्काऊ chars.in_error () || वस्तुहरू.in_error (); }

  स्थिर अन्तःस्थ अचिन्हित get_chars_overhead (अचिन्हित अङ्क_columns)
  {
    अचिन्हित c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    फर्काऊ c + अङ्क_columns * 2;
  }

  अचिन्हित get_gain (अचिन्हित additional_bytes_per_rows = 1) अचर
  {
    पूर्णाङ्क गन्ती = वस्तुहरू.length;
    फर्काऊ hb_max (0, (पूर्णाङ्क) overhead - गन्ती * (पूर्णाङ्क) additional_bytes_per_rows);
  }

  पूर्णाङ्क gain_from_merging (अचर delta_row_encoding_t& other_encoding) अचर
  {
    // Back of the envelope calculations to reject early.
    चिन्हित additional_bytes_per_rows = other_encoding.width - width;
    यदि (additional_bytes_per_rows > 0)
    {
      यदि (get_gain (additional_bytes_per_rows) == 0)
        फर्काऊ 0;
    }
    अन्यथा
    {
      यदि (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	फर्काऊ 0;
    }

    स्वतः pair = combine_width (other_encoding);
    अचिन्हित combined_width = pair.first;
    अचिन्हित combined_columns = pair.second;

    पूर्णाङ्क combined_gain = (पूर्णाङ्क) overhead + (पूर्णाङ्क) other_encoding.overhead;
    combined_gain -= (combined_width - (पूर्णाङ्क) width) * वस्तुहरू.length;
    combined_gain -= (combined_width - (पूर्णाङ्क) other_encoding.width) * other_encoding.वस्तुहरू.length;
    combined_gain -= get_chars_overhead (combined_columns);

    फर्काऊ combined_gain;
  }

  तार्किक add_row (अचर hb_vector_t<पूर्णाङ्क>* row)
  { फर्काऊ वस्तुहरू.push (row); }

  स्थिर पूर्णाङ्क cmp (अचर शून्य *pa, अचर शून्य *pb)
  {
    अचर delta_row_encoding_t *a = (अचर delta_row_encoding_t *)pa;
    अचर delta_row_encoding_t *b = (अचर delta_row_encoding_t *)pb;

    यदि (a->width != b->width)
      फर्काऊ (पूर्णाङ्क) a->width - (पूर्णाङ्क) b->width;

    फर्काऊ b->chars.cmp (a->chars);
  }
};

संरचना VarRegionAxis
{
  भिन्न evaluate (पूर्णाङ्क coord) अचर
  {
    पूर्णाङ्क peak = peakCoord.to_int ();
    यदि (peak == 0 || coord == peak)
      फर्काऊ 1.f;
    अन्यथा यदि (coord == 0) // Faster
      फर्काऊ 0.f;

    पूर्णाङ्क start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    यदि (unlikely (start > peak || peak > end))
      फर्काऊ 1.f;
    यदि (unlikely (start < 0 && end > 0))
      फर्काऊ 1.f;

    यदि (coord <= start || end <= coord)
      फर्काऊ 0.f;

    /* Interpolate */
    यदि (coord < peak)
      फर्काऊ भिन्न (coord - start) / (peak - start);
    अन्यथा
      फर्काऊ भिन्न (end - coord) / (end - peak);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c) अचर
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
  भिन्न evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len) अचर
  {
    अचिन्हित i = axisIndex;
    पूर्णाङ्क coord = i < coord_len ? coords[i] : 0;
    फर्काऊ axis.evaluate (coord);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर SparseVarRegionAxis *src,
		  अचर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->embed (src);
    यदि (unlikely (!out)) return_trace (असत्य);
    यदि (axis_map)
    {
      यदि (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (असत्य);
      }
      यदि (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (असत्य);
    }
    return_trace (सत्य);
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
  स्थिर constexpr अचिन्हित STATIC_LENGTH = 128;
  स्थिर constexpr भिन्न INVALID = न_अंक;

  सार्वजनिक:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (अचर hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& चालक= (अचर hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& चालक= (hb_scalar_cache_t&&) = delete;

  स्थिर hb_scalar_cache_t *create (अचिन्हित पूर्णाङ्क गन्ती,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    यदि (!गन्ती) फर्काऊ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    यदि (scratch_cache && गन्ती <= STATIC_LENGTH)
    {
      scratch_cache->length = गन्ती;
      scratch_cache->clear ();
      फर्काऊ scratch_cache;
    }

    स्वतः *cache = (hb_scalar_cache_t *) hb_malloc (परिमाण (hb_scalar_cache_t) - परिमाण (static_मानहरू) + परिमाण (static_मानहरू[0]) * गन्ती);
    यदि (unlikely (!cache)) फर्काऊ (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = गन्ती;
    cache->clear ();

    फर्काऊ cache;
  }

  स्थिर शून्य destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    यदि (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  शून्य clear ()
  {
    स्वतः *values = &static_मानहरू[0];
    अचिन्हित i = 0;
#ifndef HB_OPTIMIZE_SIZE
    लागि (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    लागि (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  तार्किक get (अचिन्हित i, भिन्न *value) अचर
  {
    यदि (unlikely (i >= length))
    {
      *value = 0.f;
      फर्काऊ सत्य;
    }
    स्वतः *values = &static_मानहरू[0];
    स्वतः *cached_मान = &values[i];
    भिन्न v = *cached_मान;
    यदि (std::isnan (v))
      फर्काऊ असत्य;
    *value = v;
    फर्काऊ सत्य;
  }

  HB_ALWAYS_INLINE
  शून्य set (अचिन्हित i, भिन्न value)
  {
    यदि (unlikely (i >= length)) फर्काऊ;
    स्वतः *values = &static_मानहरू[0];
    स्वतः *cached_मान = &values[i];
    *cached_मान = value;
  }

  सार्वजनिक:
  अचिन्हित length;
  गुप्त:
  mutable hb_atomic_t<भिन्न> static_मानहरू[STATIC_LENGTH];
};

संरचना VarRegionList
{
  गुप्त:
  भिन्न evaluate_impl (अचिन्हित पूर्णाङ्क region_index,
		       अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len) अचर
  {
    अचर VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_गन्ती_2);
    भिन्न v = 1.f;

    अचिन्हित पूर्णाङ्क गन्ती = axis_गन्ती_2;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      पूर्णाङ्क coord = i < coord_len ? coords[i] : 0;
      भिन्न factor = axes[i].evaluate (coord);
      यदि (factor == 0.f)
      {
	v = 0.f;
	रोक;
      }
      v *= factor;
    }

    फर्काऊ v;
  }

  सार्वजनिक:
  HB_ALWAYS_INLINE
  भिन्न evaluate (अचिन्हित पूर्णाङ्क region_index,
		  अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		  hb_scalar_cache_t *cache = nullptr) अचर
  {
    यदि (unlikely (region_index >= region_गन्ती))
      फर्काऊ 0.;

    भिन्न v;
    यदि (cache && cache->get (region_index, &v))
      फर्काऊ v;

    v = evaluate_impl (region_index, coords, coord_len);

    यदि (cache)
      cache->set (region_index, v);
    फर्काऊ v;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_गन्ती_2 * region_गन्ती));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  अचर hb_vector_t<hb_tag_t>& axis_tags,
                  अचर hb_vector_t<अचर hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित axis_गन्ती = axis_tags.length;
    अचिन्हित region_गन्ती_2 = regions.length;
    यदि (!axis_गन्ती || !region_गन्ती_2) return_trace (असत्य);
    यदि (unlikely (hb_unsigned_mul_overflows (axis_गन्ती * region_गन्ती_2,
                                             VarRegionAxis::static_size))) return_trace (असत्य);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    axis_गन्ती_2 = axis_गन्ती;
    region_गन्ती = region_गन्ती_2;

    लागि (अचिन्हित r = 0; r < region_गन्ती_2; r++)
    {
      अचर स्वतः& region = regions[r];
      लागि (अचिन्हित i = 0; i < axis_गन्ती; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        यदि (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->बीच);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        अन्यथा
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        यदि (!var_region_rec.serialize (c))
          return_trace (असत्य);
      }
    }
    return_trace (सत्य);
  }

  तार्किक serialize (hb_serialize_context_t *c, अचर VarRegionList *src, अचर hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    axis_गन्ती_2 = src->axis_गन्ती_2;
    region_गन्ती = region_map.get_population ();
    यदि (unlikely (hb_unsigned_mul_overflows (axis_गन्ती_2 * region_गन्ती,
					     VarRegionAxis::static_size))) return_trace (असत्य);
    यदि (unlikely (!c->extend (एतत्))) return_trace (असत्य);
    अचिन्हित पूर्णाङ्क region_गन्ती_2 = src->region_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क r = 0; r < region_गन्ती; r++)
    {
      अचिन्हित पूर्णाङ्क backward = region_map.backward (r);
      यदि (backward >= region_गन्ती_2) return_trace (असत्य);
      hb_memcpy (&axesZ[axis_गन्ती_2 * r], &src->axesZ[axis_गन्ती_2 * backward], VarRegionAxis::static_size * axis_गन्ती_2);
    }

    return_trace (सत्य);
  }

  तार्किक get_var_region (अचिन्हित region_index,
                       अचर hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) अचर
  {
    यदि (region_index >= region_गन्ती) फर्काऊ असत्य;
    अचर VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_गन्ती_2);
    लागि (अचिन्हित i = 0; i < axis_गन्ती_2; i++)
    {
      hb_tag_t *axis_tag;
      यदि (!axes_old_index_tag_map.has (i, &axis_tag))
        फर्काऊ असत्य;

      भिन्न min_val = axis_region->startCoord.to_float ();
      भिन्न def_val = axis_region->peakCoord.to_float ();
      भिन्न max_val = axis_region->endCoord.to_float ();

      यदि (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((द्विगुण) min_val, (द्विगुण) def_val, (द्विगुण) max_val));
      axis_region++;
    }
    फर्काऊ !axis_tuples.in_error ();
  }

  तार्किक get_var_regions (अचर hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) अचर
  {
    यदि (!regions.alloc (region_गन्ती))
      फर्काऊ असत्य;

    लागि (अचिन्हित i = 0; i < region_गन्ती; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      यदि (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        फर्काऊ असत्य;
      regions.push (std::move (axis_tuples));
    }
    फर्काऊ !regions.in_error ();
  }

  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_गन्ती_2, region_गन्ती)); }

  सार्वजनिक:
  HBUINT16	axis_गन्ती_2;
  HBUINT15	region_गन्ती;
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
_hb_avar2_region_is_dead (अचर hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  अचर hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  लागि (स्वतः _ : axis_tuples)
  {
    Triple *range;
    यदि (!reachable_ranges.has (_.first, &range)) जारी;
    अचर Triple &tent = _.second;
    पूर्णाङ्क start = (पूर्णाङ्क) roundf ((भिन्न) tent.minimum * 16384.f);
    पूर्णाङ्क peak  = (पूर्णाङ्क) roundf ((भिन्न) tent.बीच * 16384.f);
    पूर्णाङ्क end   = (पूर्णाङ्क) roundf ((भिन्न) tent.maximum * 16384.f);
    यदि (!peak) जारी;
    यदि (start > peak || peak > end) जारी;
    यदि (start < 0 && end > 0) जारी;
    पूर्णाङ्क lo = (पूर्णाङ्क) roundf ((भिन्न) range->minimum * 16384.f);
    पूर्णाङ्क hi = (पूर्णाङ्क) roundf ((भिन्न) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    यदि (hi < start || lo > end)
      फर्काऊ सत्य;
  }
  फर्काऊ असत्य;
}

संरचना SparseVariationRegion
{
  भिन्न evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len) अचर
  {
    भिन्न v = 1.f;
    अचिन्हित पूर्णाङ्क गन्ती = axes.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      भिन्न factor = (एतत्+axes[i]).evaluate (coords, coord_len);
      यदि (factor == 0.f)
	फर्काऊ 0.;
      v *= factor;
    }
    फर्काऊ v;
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर SparseVariationRegion *src,
		  अचर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!axes.serialize (c, (अचिन्हित) src->axes.len))) return_trace (असत्य);
    लागि (अचिन्हित i = 0; i < axes.len; i++)
      यदि (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  भिन्न evaluate (अचिन्हित पूर्णाङ्क region_index,
		  अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		  hb_scalar_cache_t *cache = nullptr) अचर
  {
    यदि (unlikely (region_index >= regions.len))
      फर्काऊ 0.;

    भिन्न v;
    यदि (cache && cache->get (region_index, &v))
      फर्काऊ v;

    अचर SparseVariationRegion &region = एतत्+regions[region_index];

    v = region.evaluate (coords, coord_len);
    यदि (cache)
      cache->set (region_index, v);

    फर्काऊ v;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (regions.sanitize (c, एतत्));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर SparseVarRegionList *src,
		  अचर hb_inc_bimap_t &region_map,
		  अचर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    regions.len = region_map.get_population ();
    यदि (unlikely (!c->extend (regions))) return_trace (असत्य);

    लागि (अचिन्हित i = 0; i < regions.len; i++)
    {
      अचिन्हित old_index = region_map.backward (i);
      यदि (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  सार्वजनिक:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, regions);
};


संरचना VarData
{
  अचिन्हित पूर्णाङ्क get_item_गन्ती () अचर
  { फर्काऊ item_गन्ती; }

  अचिन्हित पूर्णाङ्क get_region_index_गन्ती () अचर
  { फर्काऊ regionIndices.len; }

  अचिन्हित get_region_index (अचिन्हित i) अचर
  { फर्काऊ i >= regionIndices.len ? -1 : regionIndices[i]; }

  माप_प्रकार get_row_size () अचर
  { फर्काऊ (word_गन्ती () + regionIndices.len) * (long_शब्दहरू_2 () ? 2 : 1); }

  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_गन्ती, get_row_size ())); }

  ढाँचा <typename Float = भिन्न>
  Float _get_delta (अचिन्हित पूर्णाङ्क inner,
		    अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		    अचर VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) अचर
  {
    यदि (unlikely (inner >= item_गन्ती))
      फर्काऊ 0.;
    तार्किक is_long = long_शब्दहरू_2 ();
    अचिन्हित पूर्णाङ्क गन्ती = regionIndices.len;
    अचिन्हित word_गन्ती_2 = word_गन्ती ();
    अचिन्हित पूर्णाङ्क scount = is_long ? गन्ती : word_गन्ती_2;
    अचिन्हित पूर्णाङ्क lcount = is_long ? word_गन्ती_2 : 0;

    अचर HBUINT8 *bytes = get_delta_bytes ();
    अचर HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    अचिन्हित पूर्णाङ्क i = 0;

    अचर HBINT32 *lcursor = reinterpret_cast<अचर HBINT32 *> (row);
    लागि (; i < lcount; i++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गन्ती, cache);
      यदि (scalar)
        delta += (Float) scalar * (पूर्णाङ्क) *lcursor;
      lcursor++;
    }
    अचर HBINT16 *scursor = reinterpret_cast<अचर HBINT16 *> (lcursor);
    लागि (; i < scount; i++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गन्ती, cache);
      यदि (scalar)
       delta += (Float) scalar * (पूर्णाङ्क) *scursor;
      scursor++;
    }
    अचर HBINT8 *bcursor = reinterpret_cast<अचर HBINT8 *> (scursor);
    लागि (; i < गन्ती; i++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गन्ती, cache);
      यदि (scalar)
        delta += (Float) scalar * (पूर्णाङ्क) *bcursor;
      bcursor++;
    }

    फर्काऊ delta;
  }

  ढाँचा <typename Float = भिन्न>
  HB_ALWAYS_INLINE
  Float get_delta (अचिन्हित पूर्णाङ्क inner,
		   अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		   अचर VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = regionIndices.len;
    यदि (!गन्ती) फर्काऊ Float (0); // This is quite common, so optimize it.
    फर्काऊ _get_delta<Float> (inner, coords, coord_गन्ती, regions, cache);
  }

  शून्य get_region_scalars (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
			   अचर VarRegionList &regions,
			   भिन्न *scalars /*OUT */,
			   अचिन्हित पूर्णाङ्क अङ्क_scalars) अचर
  {
    अचिन्हित गन्ती = hb_min (अङ्क_scalars, regionIndices.len);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गन्ती);
    लागि (अचिन्हित पूर्णाङ्क i = गन्ती; i < अङ्क_scalars; i++)
      scalars[i] = 0.f;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_गन्ती () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_गन्ती,
				  get_row_size ()));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक has_long,
                  अचर hb_vector_t<अचर hb_vector_t<पूर्णाङ्क>*>& rows)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित row_गन्ती_2 = rows.length;
    यदि (!row_गन्ती_2) {
      // Nothing to serialize, will be empty.
      फर्काऊ असत्य;
    }

    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    यदि (unlikely (!c->check_assign (item_गन्ती, row_गन्ती_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);

    पूर्णाङ्क min_threshold = has_long ? -32768 : -128;
    पूर्णाङ्क max_threshold = has_long ? +32767 : +127;
    गणना delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    अचिन्हित अङ्क_regions = rows[0]->length;
    यदि (!delta_sz.resize (अङ्क_regions))
      return_trace (असत्य);

    अचिन्हित word_गन्ती_2 = 0;
    लागि (अचिन्हित r = 0; r < अङ्क_regions; r++)
    {
      लागि (अचिन्हित i = 0; i < row_गन्ती_2; i++)
      {
        पूर्णाङ्क delta = rows[i]->arrayZ[r];
        यदि (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_गन्ती_2++;
          रोक;
        }
        अन्यथा यदि (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    अचिन्हित word_index = 0;
    अचिन्हित non_word_index = word_गन्ती_2;
    hb_map_t ri_map;
    लागि (अचिन्हित r = 0; r < अङ्क_regions; r++)
    {
      यदि (!delta_sz[r]) जारी;
      अचिन्हित new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      यदि (!ri_map.set (new_r, r))
        return_trace (असत्य);
    }

    word_Size_गन्ती = word_गन्ती_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    अचिन्हित ri_गन्ती = ri_map.get_population ();
    regionIndices.len = ri_गन्ती;
    यदि (unlikely (!c->extend (एतत्))) return_trace (असत्य);

    लागि (अचिन्हित r = 0; r < ri_गन्ती; r++)
    {
      hb_codepoint_t *idx;
      यदि (!ri_map.has (r, &idx))
        return_trace (असत्य);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_size = get_row_size ();
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < row_गन्ती_2; i++)
    {
      लागि (अचिन्हित पूर्णाङ्क r = 0; r < ri_गन्ती; r++)
      {
        पूर्णाङ्क delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (सत्य);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर VarData *src,
		  अचर hb_inc_bimap_t &inner_map,
		  अचर hb_inc_bimap_t &region_map,
		  अचर hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    item_गन्ती = inner_map.get_next_मान ();

    /* Optimize word count */
    अचिन्हित ri_गन्ती = src->regionIndices.len;
    गणना delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<अचिन्हित पूर्णाङ्क> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_गन्ती);
    ri_map.resize (ri_गन्ती);
    अचिन्हित पूर्णाङ्क new_word_गन्ती = 0;
    अचिन्हित पूर्णाङ्क r;

    अचर HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    अचिन्हित src_row_size = src->get_row_size ();
    अचिन्हित src_word_गन्ती = src->word_गन्ती ();
    तार्किक     src_long_शब्दहरू = src->long_शब्दहरू_2 ();

    तार्किक has_long = असत्य;
    यदि (src_long_शब्दहरू)
    {
      लागि (r = 0; r < src_word_गन्ती; r++)
      {
        लागि (अचिन्हित old_gid : inner_map.साँचोहरू())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  यदि (delta < -32768 || 32767 < delta)
	  {
	    has_long = सत्य;
	    रोक;
	  }
        }
      }
    }

    चिन्हित min_threshold = has_long ? -32768 : -128;
    चिन्हित max_threshold = has_long ? +32767 : +127;
    लागि (r = 0; r < ri_गन्ती; r++)
    {
      तार्किक short_circuit = src_long_शब्दहरू == has_long && src_word_गन्ती <= r;

      delta_sz[r] = kZero;
      यदि (culled_regions && culled_regions->has (src->regionIndices[r]))
	जारी; /* unreachable region: drop its column */
      लागि (अचिन्हित old_gid : inner_map.साँचोहरू())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	यदि (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_गन्ती++;
	  रोक;
	}
	अन्यथा यदि (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  यदि (short_circuit)
	    रोक;
	}
      }
    }

    अचिन्हित पूर्णाङ्क word_index = 0;
    अचिन्हित पूर्णाङ्क non_word_index = new_word_गन्ती;
    अचिन्हित पूर्णाङ्क new_ri_गन्ती = 0;
    लागि (r = 0; r < ri_गन्ती; r++)
      यदि (delta_sz[r])
      {
	अचिन्हित new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_गन्ती++;
      }

    word_Size_गन्ती = new_word_गन्ती | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_गन्ती;

    यदि (unlikely (!c->extend (एतत्))) return_trace (असत्य);

    लागि (r = 0; r < new_ri_गन्ती; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_size = get_row_size ();
    अचिन्हित गन्ती = item_गन्ती;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      अचिन्हित पूर्णाङ्क old = inner_map.backward (i);
      लागि (अचिन्हित पूर्णाङ्क r = 0; r < new_ri_गन्ती; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (सत्य);
  }

  शून्य collect_region_refs (hb_set_t &region_indices, अचर hb_inc_bimap_t &inner_map) अचर
  {
    अचर HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_size = get_row_size ();

    लागि (अचिन्हित पूर्णाङ्क r = 0; r < regionIndices.len; r++)
    {
      अचिन्हित पूर्णाङ्क region = regionIndices.arrayZ[r];
      यदि (region_indices.has (region)) जारी;
      लागि (hb_codepoint_t old_gid : inner_map.साँचोहरू())
	यदि (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  रोक;
	}
    }
  }

  सार्वजनिक:
  अचर HBUINT8 *get_delta_bytes () अचर
  { फर्काऊ &StructAfter<HBUINT8> (regionIndices); }

  रक्षित:
  HBUINT8 *get_delta_bytes ()
  { फर्काऊ &StructAfter<HBUINT8> (regionIndices); }

  सार्वजनिक:
  int32_t get_item_delta_fast (अचिन्हित पूर्णाङ्क item, अचिन्हित पूर्णाङ्क region,
			       अचर HBUINT8 *delta_bytes, अचिन्हित row_size) अचर
  {
    यदि (unlikely (item >= item_गन्ती || region >= regionIndices.len)) फर्काऊ 0;

    अचर HBINT8 *p = (अचर HBINT8 *) delta_bytes + item * row_size;
    अचिन्हित word_गन्ती_2 = word_गन्ती ();
    तार्किक is_long = long_शब्दहरू_2 ();
    यदि (is_long)
    {
      यदि (region < word_गन्ती_2)
	फर्काऊ ((अचर HBINT32 *) p)[region];
      अन्यथा
	फर्काऊ ((अचर HBINT16 *)(p + HBINT32::static_size * word_गन्ती_2))[region - word_गन्ती_2];
    }
    अन्यथा
    {
      यदि (region < word_गन्ती_2)
	फर्काऊ ((अचर HBINT16 *) p)[region];
      अन्यथा
	फर्काऊ (p + HBINT16::static_size * word_गन्ती_2)[region - word_गन्ती_2];
    }
  }
  int32_t get_item_delta (अचिन्हित पूर्णाङ्क item, अचिन्हित पूर्णाङ्क region) अचर
  {
     फर्काऊ get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  रक्षित:
  शून्य set_item_delta_fast (अचिन्हित पूर्णाङ्क item, अचिन्हित पूर्णाङ्क region, int32_t delta,
			    HBUINT8 *delta_bytes, अचिन्हित row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    अचिन्हित word_गन्ती_2 = word_गन्ती ();
    तार्किक is_long = long_शब्दहरू_2 ();
    यदि (is_long)
    {
      यदि (region < word_गन्ती_2)
	((HBINT32 *) p)[region] = delta;
      अन्यथा
	((HBINT16 *)(p + HBINT32::static_size * word_गन्ती_2))[region - word_गन्ती_2] = delta;
    }
    अन्यथा
    {
      यदि (region < word_गन्ती_2)
	((HBINT16 *) p)[region] = delta;
      अन्यथा
	(p + HBINT16::static_size * word_गन्ती_2)[region - word_गन्ती_2] = delta;
    }
  }
  शून्य set_item_delta (अचिन्हित पूर्णाङ्क item, अचिन्हित पूर्णाङ्क region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  तार्किक long_शब्दहरू_2 () अचर { फर्काऊ word_Size_गन्ती & 0x8000u /* LONG_WORDS */; }
  अचिन्हित word_गन्ती () अचर { फर्काऊ word_Size_गन्ती & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  रक्षित:
  HBUINT16		item_गन्ती;
  HBUINT16		word_Size_गन्ती;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

संरचना MultiVarData
{
  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  अचर TupleList &get_delta_sets () अचर
  { फर्काऊ एतत्+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  शून्य get_delta (अचिन्हित पूर्णाङ्क inner,
		  अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		  अचर SparseVarRegionList &regions,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) अचर
  {
    स्वतः &deltaSets = get_delta_sets ();

    स्वतः मानहरू_iter = deltaSets.fetcher (inner);
    अचिन्हित region_गन्ती = regionIndices.len;
    अचिन्हित skip = 0;
    लागि (अचिन्हित regionIndex = 0; regionIndex < region_गन्ती; regionIndex++)
    {
      भिन्न scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_गन्ती,
				       cache);
      // We skip lazily. Helps with the tail end.
      यदि (scalar == 0.0f)
        skip += out.length;
      अन्यथा
      {
        यदि (skip)
	{
	  मानहरू_iter.skip (skip);
	  skip = 0;
	}
	मानहरू_iter.add_to (out, scalar);
      }
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, एतत्));
  }

  अचिन्हित get_item_गन्ती () अचर
  { फर्काऊ get_delta_sets ().गन्ती; }

  शून्य collect_region_refs (hb_set_t &region_indices) अचर
  {
    लागि (अचिन्हित region : regionIndices)
      region_indices.add (region);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर MultiVarData *src,
		  अचर hb_inc_bimap_t &inner_map,
		  अचर hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित region_गन्ती_2 = src->regionIndices.len;
    अचिन्हित header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_गन्ती_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    यदि (unlikely (!c->extend_size (एतत्, header_size, असत्य)))
      return_trace (असत्य);
    format = 1;
    regionIndices.len = region_गन्ती_2;
    लागि (अचिन्हित i = 0; i < regionIndices.len; i++)
    {
      अचिन्हित old_region = src->regionIndices[i];
      यदि (unlikely (!region_map.has (old_region)))
	return_trace (असत्य);
      regionIndices[i] = region_map.get (old_region);
    }

    अचर स्वतः &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    अचिन्हित data_size = 0;
    यदि (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (असत्य);
    लागि (अचिन्हित i = 0; i < inner_map.get_population (); i++)
    {
      अचिन्हित old_inner = inner_map.backward (i);
      यदि (unlikely (old_inner >= src_delta_sets.गन्ती)) return_trace (असत्य);
      hb_ubytes_t bytes = static_cast<अचर CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    यदि (unlikely (data_size == अच_पूर्णांक_उच्च || delta_sets.in_error ()))
      return_trace (असत्य);

    स्वतः &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
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
  मित्र संरचना item_variations_t;

  hb_scalar_cache_t *create_cache () अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ hb_scalar_cache_t::create (0);
#endif
    फर्काऊ hb_scalar_cache_t::create ((एतत्+regions).region_गन्ती);
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  गुप्त:
  ढाँचा <typename Float = भिन्न>
  Float get_delta (अचिन्हित पूर्णाङ्क outer, अचिन्हित पूर्णाङ्क inner,
		   अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		   hb_scalar_cache_t *cache = nullptr) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ Float (0);
#endif

    यदि (unlikely (outer >= dataSets.len))
      फर्काऊ Float (0);

    फर्काऊ (एतत्+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_गन्ती,
					     एतत्+regions,
					     cache);
  }

  सार्वजनिक:
  तार्किक has_delta_set (अचिन्हित पूर्णाङ्क index) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ असत्य;
#endif

    अचिन्हित पूर्णाङ्क outer = index >> 16;
    अचिन्हित पूर्णाङ्क inner = index & 0xFFFF;
    फर्काऊ outer < dataSets.len &&
	   inner < (एतत्+dataSets[outer]).get_item_गन्ती ();
  }

  ढाँचा <typename Float = भिन्न>
  Float get_delta (अचिन्हित पूर्णाङ्क index,
		   अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		   hb_scalar_cache_t *cache = nullptr) अचर
  {
    अचिन्हित पूर्णाङ्क outer = index >> 16;
    अचिन्हित पूर्णाङ्क inner = index & 0xFFFF;
    फर्काऊ get_delta<Float> (outer, inner, coords, coord_गन्ती, cache);
  }
  ढाँचा <typename Float = भिन्न>
  Float get_delta (अचिन्हित पूर्णाङ्क index,
		   hb_array_t<अचर पूर्णाङ्क> coords,
		   hb_scalar_cache_t *cache = nullptr) अचर
  {
    फर्काऊ get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ सत्य;
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
                  अचर hb_vector_t<hb_tag_t>& axis_tags,
                  अचर hb_vector_t<अचर hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  अचर hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (एतत्);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    format = 1;
    यदि (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (असत्य);

    अचिन्हित अङ्क_var_data = vardata_encodings.length;
    यदि (!अङ्क_var_data) return_trace (असत्य);
    यदि (unlikely (!c->check_assign (dataSets.len, अङ्क_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);

    यदि (unlikely (!c->extend (dataSets))) return_trace (असत्य);
    लागि (अचिन्हित i = 0; i < अङ्क_var_data; i++)
      यदि (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].वस्तुहरू))
        return_trace (असत्य);

    return_trace (सत्य);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर ItemVariationStore *src,
		  अचर hb_array_t <अचर hb_inc_bimap_t> &inner_maps,
		  अचर hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (एतत्);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif

    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    अचिन्हित पूर्णाङ्क set_गन्ती = 0;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < inner_maps.length; i++)
      यदि (inner_maps[i].get_population ())
	set_गन्ती++;

    format = 1;

    अचर स्वतः &src_regions = src+src->regions;

    hb_set_t region_indices;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    यदि (region_indices.in_error ())
      return_trace (असत्य);

    region_indices.del_range ((src_regions).region_गन्ती, hb_set_t::INVALID);

    यदि (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (अचिन्हित _) { region_map.add(_); })
    ;
    यदि (region_map.in_error())
      return_trace (असत्य);

    यदि (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (असत्य);

    dataSets.len = set_गन्ती;
    यदि (unlikely (!c->extend (dataSets))) return_trace (असत्य);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    अचिन्हित पूर्णाङ्क set_index = 0;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < inner_maps.length; i++)
    {
      यदि (!inner_maps[i].get_population ()) जारी;
      यदि (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः *out = c->start_embed (एतत्);
    यदि (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    अचिन्हित गन्ती = dataSets.len;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      यदि (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      स्वतः &data = एतत्+dataSets[i];

      अचिन्हित item_गन्ती = data.get_item_गन्ती ();
      लागि (अचिन्हित j = 0; j < item_गन्ती; j++)
	map->add (j);
    }

    यदि (unlikely (!out->serialize (c, एतत्, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  तार्किक collect_dead_regions (अचर hb_map_t &axes_old_index_tag_map,
			     अचर hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) अचर
  {
#ifndef HB_NO_VAR
    यदि (!reachable_ranges.get_population ()) फर्काऊ सत्य;
    अचर VarRegionList &region_list = एतत्+regions;
    अचिन्हित गन्ती = region_list.region_गन्ती;
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      यदि (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	फर्काऊ असत्य;
      यदि (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    फर्काऊ !dead_regions.in_error ();
#अन्यथा
    फर्काऊ सत्य;
#endif
  }

  तार्किक subset (hb_subset_context_t *c, अचर hb_array_t<अचर hb_inc_bimap_t> &inner_maps) अचर
  {
    TRACE_SUBSET (एतत्);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif

    ItemVariationStore *varstore_अविभाज्य = c->serializer->start_embed<ItemVariationStore> ();
    यदि (unlikely (!varstore_अविभाज्य)) return_trace (असत्य);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    यदि (c->plan->has_avar2)
      यदि (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (असत्य);

    varstore_अविभाज्य->serialize (c->serializer, एतत्, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_अविभाज्य->dataSets);
  }

  अचिन्हित पूर्णाङ्क get_region_index_गन्ती (अचिन्हित पूर्णाङ्क major) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ 0;
#endif
    फर्काऊ (एतत्+dataSets[major]).get_region_index_गन्ती ();
  }

  शून्य get_region_scalars (अचिन्हित पूर्णाङ्क major,
			   अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
			   भिन्न *scalars /*OUT*/,
			   अचिन्हित पूर्णाङ्क अङ्क_scalars) अचर
  {
#ifdef HB_NO_VAR
    लागि (अचिन्हित i = 0; i < अङ्क_scalars; i++)
      scalars[i] = 0.f;
    फर्काऊ;
#endif

    (एतत्+dataSets[major]).get_region_scalars (coords, coord_गन्ती,
					       एतत्+regions,
					       &scalars[0], अङ्क_scalars);
  }

  अचिन्हित पूर्णाङ्क get_sub_table_गन्ती () अचर
   {
#ifdef HB_NO_VAR
     फर्काऊ 0;
#endif
     फर्काऊ dataSets.len;
   }

  अचर VarData& get_sub_table (अचिन्हित i) अचर
  {
#ifdef HB_NO_VAR
     फर्काऊ Null (VarData);
#endif
     फर्काऊ एतत्+dataSets[i];
  }

  अचर VarRegionList& get_region_list () अचर
  {
#ifdef HB_NO_VAR
     फर्काऊ Null (VarRegionList);
#endif
     फर्काऊ एतत्+regions;
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
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ hb_scalar_cache_t::create (0);
#endif
    स्वतः &r = एतत्+regions;
    अचिन्हित गन्ती = r.regions.len;

    फर्काऊ hb_scalar_cache_t::create (गन्ती, static_cache);
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  गुप्त:
  शून्य get_delta (अचिन्हित पूर्णाङ्क outer, अचिन्हित पूर्णाङ्क inner,
		  अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ;
#endif

    यदि (unlikely (outer >= dataSets.len))
      फर्काऊ;

    फर्काऊ (एतत्+dataSets[outer]).get_delta (inner,
					     coords, coord_गन्ती,
					     एतत्+regions,
					     out,
					     cache);
  }

  सार्वजनिक:
  शून्य get_delta (अचिन्हित पूर्णाङ्क index,
		  अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_गन्ती,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) अचर
  {
    अचिन्हित पूर्णाङ्क outer = index >> 16;
    अचिन्हित पूर्णाङ्क inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_गन्ती, out, cache);
  }
  शून्य get_delta (अचिन्हित पूर्णाङ्क index,
		  hb_array_t<अचर पूर्णाङ्क> coords,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) अचर
  {
    फर्काऊ get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
#ifdef HB_NO_VAR
    फर्काऊ सत्य;
#endif

    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, एतत्) &&
		  dataSets.sanitize (c, एतत्));
  }

  तार्किक create_subset_plan (अचर hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) अचर
  {
    यदि (unlikely (!inner_maps->resize (dataSets.len))) फर्काऊ असत्य;
    लागि (hb_codepoint_t var_idx : var_indices)
    {
      अचिन्हित outer = var_idx >> 16;
      अचिन्हित inner = var_idx & 0xFFFFu;
      यदि (unlikely (outer >= dataSets.len ||
		    inner >= (एतत्+dataSets[outer]).get_item_गन्ती ()))
	फर्काऊ असत्य;
      (*inner_maps)[outer].add (inner);
    }

    अचिन्हित new_outer = 0;
    लागि (अचिन्हित outer = 0; outer < inner_maps->length; outer++)
    {
      अचर hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      यदि (!inner_map.get_population ()) जारी;
      यदि (unlikely (inner_map.in_error ())) फर्काऊ असत्य;
      लागि (अचिन्हित new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	अचिन्हित old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    फर्काऊ !inner_maps->in_error () && !varidx_map->in_error ();
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर MultiItemVariationStore *src,
		  अचर hb_array_t<अचर hb_inc_bimap_t> &inner_maps,
		  अचर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (असत्य);
    format = 1;

    अचर स्वतः &src_regions = src+src->regions;
    hb_set_t region_indices;
    अचिन्हित set_गन्ती = 0;
    लागि (अचिन्हित i = 0; i < src->dataSets.len; i++)
    {
      यदि (!inner_maps[i].get_population ()) जारी;
      set_गन्ती++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    यदि (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (असत्य);

    dataSets.len = set_गन्ती;
    यदि (unlikely (!c->extend (dataSets))) return_trace (असत्य);
    अचिन्हित set_index = 0;
    लागि (अचिन्हित i = 0; i < src->dataSets.len; i++)
    {
      यदि (!inner_maps[i].get_population ()) जारी;
      यदि (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  रक्षित:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

ढाँचा <typename Map_गन्ती_T>
संरचना DeltaSetIndexMapFormat01
{
  मित्र संरचना DeltaSetIndexMap;

  माप_प्रकार get_size () अचर
  { फर्काऊ hb_unsigned_mul_add_saturate (map_गन्ती_2, get_width (), min_size); }

  गुप्त:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (एतत्));
  }

  ढाँचा <typename T>
  तार्किक serialize (hb_serialize_context_t *c, अचर T &plan)
  {
    अचिन्हित पूर्णाङ्क width = plan.get_width ();
    अचिन्हित पूर्णाङ्क inner_bit_गन्ती = plan.get_inner_bit_गन्ती ();
    अचर hb_array_t<अचर uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (output_map.length && ((((inner_bit_गन्ती-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (असत्य);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);

    entryFormat = ((width-1)<<4)|(inner_bit_गन्ती-1);
    map_गन्ती_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    यदि (unlikely (!p)) return_trace (असत्य);
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < output_map.length; i++)
    {
      अचिन्हित पूर्णाङ्क v = output_map.arrayZ[i];
      यदि (v)
      {
	अचिन्हित पूर्णाङ्क outer = v >> 16;
	अचिन्हित पूर्णाङ्क inner = v & 0xFFFF;
	अचिन्हित पूर्णाङ्क u = (outer << inner_bit_गन्ती) | inner;
	लागि (अचिन्हित पूर्णाङ्क w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (सत्य);
  }

  HB_ALWAYS_INLINE
  uint32_t map (अचिन्हित पूर्णाङ्क v) अचर /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    यदि (!map_गन्ती_2)
      फर्काऊ v;
    फर्काऊ _map (v);
  }

  HB_HOT
  uint32_t _map (अचिन्हित पूर्णाङ्क v) अचर /* Returns 16.16 outer.inner. */
  {
    यदि (v >= map_गन्ती_2)
      v = map_गन्ती_2 - 1;

    अचिन्हित पूर्णाङ्क u = 0;
    { /* Fetch it. */
      अचिन्हित पूर्णाङ्क w = get_width ();
      अचर HBUINT8 *p = mapDataZ.arrayZ + w * v;
      लागि (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      अचिन्हित पूर्णाङ्क n = get_inner_bit_गन्ती ();
      अचिन्हित पूर्णाङ्क outer = u >> n;
      अचिन्हित पूर्णाङ्क inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    फर्काऊ u;
  }

  अचिन्हित get_map_गन्ती () अचर       { फर्काऊ map_गन्ती_2; }
  अचिन्हित get_width () अचर           { फर्काऊ ((entryFormat >> 4) & 3) + 1; }
  अचिन्हित get_inner_bit_गन्ती () अचर { फर्काऊ (entryFormat & 0xF) + 1; }


  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_गन्ती_2,
                                  get_width ()));
  }

  रक्षित:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_गन्ती_T     map_गन्ती_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2+Map_गन्ती_T::static_size, mapDataZ);
};

संरचना DeltaSetIndexMap
{
  ढाँचा <typename T>
  तार्किक serialize (hb_serialize_context_t *c, अचर T &plan)
  {
    TRACE_SERIALIZE (एतत्);
    अचिन्हित length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    पक्ष 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    स्वभाव:return_trace (असत्य);
    }
  }

  uint32_t map (अचिन्हित v) अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); फर्काऊ (u.format0.map (v));
    पक्ष 1: hb_barrier (); फर्काऊ (u.format1.map (v));
    स्वभाव:फर्काऊ v;
    }
  }

  अचिन्हित get_map_गन्ती () अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); फर्काऊ u.format0.get_map_गन्ती ();
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_map_गन्ती ();
    स्वभाव:फर्काऊ 0;
    }
  }

  अचिन्हित get_width () अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); फर्काऊ u.format0.get_width ();
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_width ();
    स्वभाव:फर्काऊ 0;
    }
  }

  अचिन्हित get_inner_bit_गन्ती () अचर
  {
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); फर्काऊ u.format0.get_inner_bit_गन्ती ();
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.get_inner_bit_गन्ती ();
    स्वभाव:फर्काऊ 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    विकल्प (u.format.v) {
    पक्ष 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    पक्ष 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्वभाव:return_trace (सत्य);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) अचर
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
  ItemVarStoreInstancer (अचर ItemVariationStore *varStore_,
			 अचर DeltaSetIndexMap *varIdxMap,
			 hb_array_t<अचर पूर्णाङ्क> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    यदि (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    यदि (!hb_any (coords))
      एतत्->coords = {};
  }

  चालक तार्किक () अचर { फर्काऊ varStore && तार्किक (coords); }

  भिन्न चालक[] (uint32_t varIdx) अचर
  { फर्काऊ (*एतत्) (varIdx); }

  भिन्न चालक() (uint32_t varIdx, अचिन्हित लघु offset = 0) अचर
  { फर्काऊ get_delta (varIdx, offset); }

  द्विगुण get_condition_delta (uint32_t varIdx) अचर
  { फर्काऊ get_delta<द्विगुण> (varIdx); }

  ढाँचा <typename Float = भिन्न>
  Float get_delta (uint32_t varIdx, अचिन्हित लघु offset = 0) अचर
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   यदि (!coords || varIdx == VarIdx::NO_VARIATION)
     फर्काऊ Float (0);

    varIdx += offset;
    यदि (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    फर्काऊ varStore->get_delta<Float> (varIdx, coords, cache);
  }

  अचर ItemVariationStore *varStore;
  अचर DeltaSetIndexMap *varIdxMap;
  hb_array_t<अचर पूर्णाङ्क> coords;
  hb_scalar_cache_t *cache;
};

संरचना MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (अचर MultiItemVariationStore *varStore,
			      अचर DeltaSetIndexMap *varIdxMap,
			      hb_array_t<अचर पूर्णाङ्क> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    यदि (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  चालक तार्किक () अचर { फर्काऊ varStore; }

  द्विगुण get_condition_delta (uint32_t varIdx) अचर
  { फर्काऊ (द्विगुण) (*एतत्)[varIdx]; }

  द्विगुण चालक[] (uint32_t varIdx) अचर
  {
    द्विगुण v = 0;
    (*एतत्) (hb_array (&v, 1), varIdx);
    फर्काऊ v;
  }

  शून्य चालक() (hb_array_t<द्विगुण> out, uint32_t varIdx, अचिन्हित लघु offset = 0) अचर
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    यदि (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      यदि (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    अन्यथा
      लागि (अचिन्हित i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  अचर MultiItemVariationStore *varStore;
  अचर DeltaSetIndexMap *varIdxMap;
  hb_array_t<अचर पूर्णाङ्क> coords;
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

ढाँचा <typename Instancer>
स्थिर तार्किक
_hb_recurse_condition_evaluate (अचर संरचना Condition &condition,
				अचर पूर्णाङ्क *coords,
				अचिन्हित पूर्णाङ्क coord_len,
				Instancer *instancer);

संरचना ConditionAxisRange
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    अचर hb_map_t *index_map = &c->plan->axes_index_map;
    यदि (index_map->is_empty ()) return_trace (सत्य);

    अचर hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    यदि (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (असत्य);

    अचर hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    यदि (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    अचर hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    यदि (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    भिन्न normalized_min = renormalize_मान ((द्विगुण) filter_Range_Min_मान.to_float (), axis_limit, axis_triple_distances, असत्य);
    भिन्न normalized_max = renormalize_मान ((द्विगुण) filter_Range_Max_मान.to_float (), axis_limit, axis_triple_distances, असत्य);
    out->filter_Range_Min_मान.set_float (normalized_min);
    out->filter_Range_Max_मान.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  गुप्त:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) अचर
  {
    //invalid axis index, drop the entire record
    यदि (!c->axes_index_tag_map->has (axisIndex))
      फर्काऊ DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    तार्किक axis_set_by_user = असत्य;
    यदि (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = सत्य;
    }

    भिन्न axis_min_val = axis_range.minimum;
    भिन्न axis_default_val = axis_range.बीच;
    भिन्न axis_max_val = axis_range.maximum;

    भिन्न filter_min_val = filter_Range_Min_मान.to_float ();
    भिन्न filter_max_val = filter_Range_Max_मान.to_float ();

    यदि (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = असत्य;

    //condition not met, drop the entire record
    यदि (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      फर्काऊ DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    यदि (axis_set_by_user && axis_range.is_point ())
      फर्काऊ DROP_COND_WITH_VAR;

    यदि (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_मान.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_मान.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      फर्काऊ KEEP_COND_WITH_VAR;
    }
    फर्काऊ KEEP_RECORD_WITH_VAR;
  }

  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer HB_UNUSED) अचर
  {
    पूर्णाङ्क coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    फर्काऊ filter_Range_Min_मान.to_int () <= coord && coord <= filter_Range_Max_मान.to_int ();
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_मान;
  F2DOT14	filter_Range_Max_मान;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना Condition_मान
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    यदि (varIdx == VarIdx::NO_VARIATION)
      return_trace (सत्य);

    hb_pair_t<अचिन्हित, पूर्णाङ्क> *condition_मान;
    यदि (c->plan->layout_condition_idx_मान_map.has ((uintptr_t) एतत्, &condition_मान))
    {
      out->default_मान_2 = condition_मान->second;
      out->varIdx = condition_मान->first;
      return_trace (सत्य);
    }

    hb_pair_t<अचिन्हित, पूर्णाङ्क> *new_varidx_delta;
    यदि (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (असत्य);

    यदि (unlikely (!c->serializer->check_assign (
		out->default_मान_2,
		default_मान_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (सत्य);
  }

  गुप्त:
  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer) अचर
  {
    द्विगुण value = default_मान_2;
    value += instancer->get_condition_delta (varIdx);
    फर्काऊ value > 0;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_मान_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना ConditionAnd
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);
    out->format = format;

    लागि (अचर स्वतः &offset : conditions)
    {
      यदि (offset.is_null ())
      {
	स्वतः *out_offset = out->conditions.serialize_append (c->serializer);
	यदि (unlikely (!out_offset)) return_trace (असत्य);
	*out_offset = 0;
      }
      अन्यथा यदि (unlikely (!subset_offset_array (c, out->conditions, एतत्) (offset)))
	return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  गुप्त:
  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = conditions.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (!_hb_recurse_condition_evaluate (एतत्+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);
    out->format = format;

    लागि (अचर स्वतः &offset : conditions)
    {
      यदि (offset.is_null ())
      {
	स्वतः *out_offset = out->conditions.serialize_append (c->serializer);
	यदि (unlikely (!out_offset)) return_trace (असत्य);
	*out_offset = 0;
      }
      अन्यथा यदि (unlikely (!subset_offset_array (c, out->conditions, एतत्) (offset)))
	return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  गुप्त:
  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = conditions.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (_hb_recurse_condition_evaluate (एतत्+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);
    यदि (condition.is_null ())
    {
      out->condition = 0;
      return_trace (सत्य);
    }
    return_trace (out->condition.serialize_subset (c, condition, एतत्));
  }

  गुप्त:
  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer) अचर
  {
    फर्काऊ !_hb_recurse_condition_evaluate (एतत्+condition,
					    coords, coord_len,
					    instancer);
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  मित्र संरचना ConditionSet;

  स्थिर तार्किक serialize_constant (hb_serialize_context_t *c, तार्किक value)
  {
    स्वतः *out = c->start_embed<Condition_मान> ();
    यदि (unlikely (!out || !c->extend_min (out))) फर्काऊ असत्य;
    out->format = 2;
    out->default_मान_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    फर्काऊ सत्य;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      अचिन्हित depth = HB_MAX_NESTING_LEVEL,
				      तार्किक materialize_true = असत्य) अचर;

  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer) अचर
  {
    विकल्प (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    पक्ष 0: फर्काऊ सत्य;
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.evaluate (coords, coord_len, instancer);
    पक्ष 2: hb_barrier (); फर्काऊ u.format2.evaluate (coords, coord_len, instancer);
    पक्ष 3: hb_barrier (); फर्काऊ u.format3.evaluate (coords, coord_len, instancer);
    पक्ष 4: hb_barrier (); फर्काऊ u.format4.evaluate (coords, coord_len, instancer);
    पक्ष 5: hb_barrier (); फर्काऊ u.format5.evaluate (coords, coord_len, instancer);
    स्वभाव:फर्काऊ असत्य;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             अचिन्हित depth = HB_MAX_NESTING_LEVEL) अचर
  {
    यदि (unlikely (!depth))
    {
      c->apply = असत्य;
      फर्काऊ DROP_RECORD_WITH_VAR;
    }

    विकल्प (u.format.v) {
    पक्ष 0: फर्काऊ KEEP_RECORD_WITH_VAR;
    पक्ष 1: hb_barrier (); फर्काऊ u.format1.keep_with_variations (c, condition_map);
    पक्ष 2: पक्ष 3: पक्ष 4: पक्ष 5: रोक;
    स्वभाव: c->apply = असत्य; फर्काऊ DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    तार्किक applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    यदि (c->all_axes_pinned)
      फर्काऊ applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    यदि (u.format.v == 2)
      फर्काऊ u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    स्वतः classify_child = [c, condition_map, depth] (अचर Condition &child)
    {
      स्वतः child_context = *c;
      child_context.apply = सत्य;
      फर्काऊ child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    यदि (u.format.v == 5)
    {
      स्वतः result = classify_child (&u.format5 + u.format5.condition);
      फर्काऊ result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    तार्किक is_and = u.format.v == 3;
    तार्किक variable = असत्य;
    अचर स्वतः &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    अचर शून्य *base = is_and ? (अचर शून्य *) &u.format3 : (अचर शून्य *) &u.format4;
    लागि (अचर स्वतः &offset : conditions)
    {
      स्वतः result = classify_child (base + offset);
      यदि (is_and && result == DROP_RECORD_WITH_VAR)
        फर्काऊ DROP_RECORD_WITH_VAR;
      यदि (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        फर्काऊ KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    फर्काऊ variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) अचर
  {
    यदि (unlikely (!c->may_dispatch (एतत्, &u.format.v))) फर्काऊ c->no_dispatch_return_मान ();
    TRACE_DISPATCH (एतत्, u.format.v);
    विकल्प (u.format.v) {
    पक्ष 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    पक्ष 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    पक्ष 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    पक्ष 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    पक्ष 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    स्वभाव:return_trace (c->default_return_मान ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);

    यदि (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मान ());

    return_trace (c->end_recursion (एतत्->dispatch (c)));
  }

  तार्किक collect_var_indices (hb_set_t *var_indices,
			    अचिन्हित depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *condition_मानहरू = nullptr) अचर;

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर Condition *src,
		  अचर hb_map_t &varidx_map,
		  अचर hb_map_t *axis_map = nullptr);

  गुप्त:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      अचिन्हित depth) अचर;

  रक्षित:
  सङ्घ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_मान	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

ढाँचा <typename Instancer>
तार्किक
_hb_recurse_condition_evaluate (अचर संरचना Condition &condition,
				अचर पूर्णाङ्क *coords,
				अचिन्हित पूर्णाङ्क coord_len,
				Instancer *instancer)
{
  फर्काऊ condition.evaluate (coords, coord_len, instancer);
}

संरचना ConditionList
{
  अचर Condition& चालक[] (अचिन्हित i) अचर
  { फर्काऊ एतत्+conditions[i]; }

  अचिन्हित get_गन्ती () अचर { फर्काऊ conditions.len; }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचर ConditionList *src,
		  अचर hb_inc_bimap_t &condition_map,
		  अचर hb_map_t &varidx_map,
		  अचर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (एतत्);
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    conditions.len = condition_map.get_population ();
    यदि (unlikely (!c->extend (conditions))) return_trace (असत्य);
    लागि (अचिन्हित i = 0; i < conditions.len; i++)
    {
      अचिन्हित old_index = condition_map.backward (i);
      यदि (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
Condition::collect_var_indices (hb_set_t *var_indices, अचिन्हित depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *condition_मानहरू) अचर
{
  यदि (unlikely (!depth)) फर्काऊ असत्य;
  विकल्प (u.format.v)
  {
    पक्ष 0:
    पक्ष 1:
      फर्काऊ सत्य;
    पक्ष 2:
      यदि (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        यदि (condition_मानहरू &&
            !condition_मानहरू->set ((uintptr_t) &u.format2,
                                    hb_pair (अचिन्हित (u.format2.varIdx), पूर्णाङ्क (u.format2.default_मान_2))))
          फर्काऊ असत्य;
      }
      फर्काऊ !var_indices->in_error ();
    पक्ष 3:
      लागि (अचर स्वतः &offset : u.format3.conditions)
	यदि (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मानहरू)))
	  फर्काऊ असत्य;
      फर्काऊ सत्य;
    पक्ष 4:
      लागि (अचर स्वतः &offset : u.format4.conditions)
	यदि (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मानहरू)))
	  फर्काऊ असत्य;
      फर्काऊ सत्य;
    पक्ष 5:
      फर्काऊ (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_मानहरू);
    स्वभाव:
      फर्काऊ असत्य;
  }
}

अन्तःस्थ तार्किक
Condition::serialize (hb_serialize_context_t *c,
		      अचर Condition *src,
		      अचर hb_map_t &varidx_map,
		      अचर hb_map_t *axis_map)
{
  TRACE_SERIALIZE (एतत्);
  विकल्प (src->u.format.v)
  {
    पक्ष 0:
    {
      स्वतः *out = c->start_embed<Condition_मान> ();
      यदि (unlikely (!out || !c->extend_min (out))) return_trace (असत्य);
      out->format = 2;
      out->default_मान_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (सत्य);
    }
    पक्ष 1:
    {
      स्वतः *out = c->embed (&src->u.format1);
      यदि (unlikely (!out)) return_trace (असत्य);
      यदि (axis_map)
      {
	यदि (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (असत्य);
	}
	यदि (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (असत्य);
      }
      return_trace (सत्य);
    }
    पक्ष 2:
    {
      स्वतः *out = c->embed (&src->u.format2);
      यदि (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (असत्य);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (सत्य);
    }
    पक्ष 3:
    {
      स्वतः *out = c->start_embed<ConditionAnd> ();
      यदि (unlikely (!c->extend_min (out))) return_trace (असत्य);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      यदि (unlikely (!c->extend (out->conditions))) return_trace (असत्य);
      लागि (अचिन्हित i = 0; i < out->conditions.len; i++)
	यदि (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (असत्य);
      return_trace (सत्य);
    }
    पक्ष 4:
    {
      स्वतः *out = c->start_embed<ConditionOr> ();
      यदि (unlikely (!c->extend_min (out))) return_trace (असत्य);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      यदि (unlikely (!c->extend (out->conditions))) return_trace (असत्य);
      लागि (अचिन्हित i = 0; i < out->conditions.len; i++)
	यदि (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (असत्य);
      return_trace (सत्य);
    }
    पक्ष 5:
    {
      स्वतः *out = c->embed (&src->u.format5);
      यदि (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (असत्य);
      return_trace (सत्य);
    }
    स्वभाव:
      return_trace (असत्य);
  }
}

अन्तःस्थ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    अचिन्हित depth,
				    तार्किक materialize_true) अचर
{
  स्वतः snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  यदि (result == LOOKUP_CONDITION_SUBSET_KEEP)
    फर्काऊ result;

  c->serializer->revert (snap);
  यदि (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    फर्काऊ result;

  फर्काऊ serialize_constant (c->serializer, सत्य) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

अन्तःस्थ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 अचिन्हित depth) अचर
{
  यदि (unlikely (!depth))
    फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;

  स्वतः subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   अचर Condition &condition)
      -> lookup_condition_subset_result_t
  {
    स्वतः snap = c->serializer->snapshot ();
    स्वतः *offset = out.serialize_append (c->serializer);
    यदि (unlikely (!offset))
      फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    यदि (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->झिक_pack ());
    अन्यथा
    {
      c->serializer->झिक_discard ();
      out.झिक ();
      c->serializer->revert (snap);
    }
    फर्काऊ result;
  };

  विकल्प (u.format.v)
  {
    पक्ष 0:
      फर्काऊ LOOKUP_CONDITION_SUBSET_TRUE;

    पक्ष 1:
    {
      अचर ConditionAxisRange &src = u.format1;
      यदि (c->plan->user_axes_location.is_empty ())
        फर्काऊ c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      यदि (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      यदि (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      भिन्न filter_min = src.filter_Range_Min_मान.to_float ();
      भिन्न filter_max = src.filter_Range_Max_मान.to_float ();
      यदि (filter_min > filter_max)
        फर्काऊ LOOKUP_CONDITION_SUBSET_FALSE;
      यदि (axis_limit.is_point ())
        फर्काऊ filter_min <= axis_limit.बीच &&
	       axis_limit.बीच <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      यदि (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        फर्काऊ LOOKUP_CONDITION_SUBSET_FALSE;
      यदि (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        फर्काऊ LOOKUP_CONDITION_SUBSET_TRUE;
      यदि (!c->plan->axes_index_map.has (src.axisIndex))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;

      स्वतः *out = c->serializer->embed (&src);
      यदि (unlikely (!out))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      यदि (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_मान.set_float (
	  renormalize_मान (filter_min, axis_limit, axis_distances, असत्य));
      out->filter_Range_Max_मान.set_float (
	  renormalize_मान (filter_max, axis_limit, axis_distances, असत्य));
      यदि (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;
      फर्काऊ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    पक्ष 2:
    {
      अचर Condition_मान &src = u.format2;
      स्वतः *out = c->serializer->embed (&src);
      यदि (unlikely (!out))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;

      यदि (src.varIdx == VarIdx::NO_VARIATION)
        फर्काऊ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णाङ्क) src.default_मान_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<अचिन्हित, पूर्णाङ्क> *condition_मान;
      यदि (c->plan->layout_condition_idx_मान_map.has ((uintptr_t) &src, &condition_मान))
      {
        out->default_मान_2 = condition_मान->second;
        out->varIdx = condition_मान->first;
        यदि (out->varIdx == VarIdx::NO_VARIATION)
          फर्काऊ out->default_मान_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        फर्काऊ LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<अचिन्हित, पूर्णाङ्क> *new_varidx_delta;
      यदि (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        फर्काऊ c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णाङ्क) src.default_मान_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      यदि (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        द्विगुण value = (पूर्णाङ्क) src.default_मान_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        फर्काऊ value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      पूर्णाङ्क default_मान = (पूर्णाङ्क) src.default_मान_2 +
			  hb_second (*new_varidx_delta);
      यदि (unlikely (!c->serializer->check_assign (
		out->default_मान_2,
		default_मान,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      फर्काऊ LOOKUP_CONDITION_SUBSET_KEEP;
    }

    पक्ष 3:
    {
      अचर ConditionAnd &src = u.format3;
      स्वतः *out = c->serializer->start_embed (&src);
      यदि (unlikely (!out || !c->serializer->extend_min (out)))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      लागि (अचर स्वतः &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        यदि (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          फर्काऊ result;
      }
      फर्काऊ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    पक्ष 4:
    {
      अचर ConditionOr &src = u.format4;
      स्वतः *out = c->serializer->start_embed (&src);
      यदि (unlikely (!out || !c->serializer->extend_min (out)))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      लागि (अचर स्वतः &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        यदि (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          फर्काऊ result;
      }
      फर्काऊ out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    पक्ष 5:
    {
      अचर ConditionNegate &src = u.format5;
      स्वतः *out = c->serializer->embed (&src);
      यदि (unlikely (!out))
        फर्काऊ LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      यदि (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->झिक_pack ());
        फर्काऊ result;
      }

      c->serializer->झिक_discard ();
      यदि (result == LOOKUP_CONDITION_SUBSET_TRUE)
        फर्काऊ LOOKUP_CONDITION_SUBSET_FALSE;
      यदि (result == LOOKUP_CONDITION_SUBSET_FALSE)
        फर्काऊ LOOKUP_CONDITION_SUBSET_TRUE;
      फर्काऊ result;
    }

    स्वभाव:
      फर्काऊ LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

संरचना ConditionSet
{
  तार्किक evaluate (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		 ItemVarStoreInstancer *instancer) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = conditions.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
      यदि (!(एतत्+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  शून्य keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) अचर
  {
    hb_map_t *condition_map = hb_map_create ();
    यदि (unlikely (!condition_map)) फर्काऊ;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    यदि (unlikely (!cond_set)) फर्काऊ;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = सत्य;
    तार्किक should_keep = असत्य;
    /* The range map cannot identify value or boolean expressions uniquely. */
    तार्किक can_deduplicate = सत्य;
    अचिन्हित अङ्क_kept_cond = 0, cond_idx = 0;
    लागि (अचर स्वतः& offset : conditions)
    {
      अचर Condition &condition = एतत्+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      यदि (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = असत्य;
        फर्काऊ;
      }

      यदि (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = सत्य;
        cond_set->add (cond_idx);
        अङ्क_kept_cond++;
      }

      यदि (ret == KEEP_RECORD_WITH_VAR)
        should_keep = सत्य;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = अङ्क_kept_cond == 0;
    यदि (!should_keep && c->all_axes_pinned) फर्काऊ;

    //check if condition_set is unique with variations
    यदि (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      फर्काऊ;

    यदि (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               तार्किक घुसाउ_catch_all) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    यदि (घुसाउ_catch_all) return_trace (सत्य);

    hb_set_t *retained_cond_set = nullptr;
    यदि (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    अचिन्हित पूर्णाङ्क गन्ती = conditions.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (retained_cond_set != nullptr && !retained_cond_set->has (i))
        जारी;
      अचर Condition &condition = एतत्+conditions[i];
      यदि (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, एतत्) (conditions[i]);
        जारी;
      }

      स्वतः *offset = out->conditions.serialize_append (c->serializer);
      यदि (unlikely (!offset)) return_trace (असत्य);
      c->serializer->push ();
      स्वतः result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, सत्य);
      यदि (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, असत्य) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      यदि (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->झिक_discard ();
        c->serializer->check_success (असत्य);
        return_trace (असत्य);
      }
      c->serializer->add_link (*offset, c->serializer->झिक_pack ());
    }

    return_trace (तार्किक (out->conditions));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, एतत्));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर स्वतः &offset : conditions)
      यदि (unlikely (!(एतत्+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_मानहरू)))
	c->layout_variation_indices->err ();
  }

  रक्षित:
  Array16OfOffset32To<Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, conditions);
};

संरचना FeatureTableSubstitutionRecord
{
  मित्र संरचना FeatureTableSubstitution;

  शून्य collect_lookups (अचर शून्य *base, hb_set_t *lookup_indexes /* OUT */) अचर
  {
    फर्काऊ (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  शून्य closure_features (अचर शून्य *base,
			 अचर hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) अचर
  {
    यदि ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  शून्य collect_feature_substitutes_with_variations (hb_hashmap_t<अचिन्हित, अचर Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    अचर hb_set_t *feature_indices,
                                                    अचर शून्य *base) अचर
  {
    यदि (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  तार्किक serialize (hb_subset_layout_context_t *c,
                  अचिन्हित feature_index,
                  अचर Feature *f, अचर Tag *tag)
  {
    TRACE_SERIALIZE (एतत्);
    hb_serialize_context_t *s = c->subset_context->serializer;
    यदि (unlikely (!s->extend_min (एतत्))) return_trace (असत्य);

    uint32_t *new_feature_idx;
    यदि (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (असत्य);

    यदि (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    s->push ();
    तार्किक ret = f->subset (c->subset_context, c, tag);
    यदि (ret) s->add_link (feature, s->झिक_pack ());
    अन्यथा s->झिक_discard ();

    return_trace (ret);
  }

  तार्किक subset (hb_subset_layout_context_t *c, अचर शून्य *base) अचर
  {
    TRACE_SUBSET (एतत्);
    uint32_t *new_feature_index;
    यदि (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (असत्य);

    स्वतः *out = c->subset_context->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->featureIndex = *new_feature_index;
    hb_pair_t<अचर शून्य*, अचर शून्य*> *original;
    यदि (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (असत्य);
    अचर Tag *tag = reinterpret_cast<अचर Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base,
		 अचर FeatureList *feature_list) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (unlikely (!c->check_struct (एतत्))) return_trace (असत्य);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    अचर Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
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
  शून्य collect_feature_indices (hb_set_t *feature_indices) अचर
  {
    लागि (अचर FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  अचर Feature *find_substitute (अचिन्हित पूर्णाङ्क feature_index) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = substitutions.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      अचर FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      यदि (record.featureIndex == feature_index)
	फर्काऊ &(एतत्+record.feature);
    }
    फर्काऊ nullptr;
  }

  शून्य collect_lookups (अचर hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) अचर
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([एतत्, lookup_indexes] (अचर FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (एतत्, lookup_indexes); })
    ;
  }

  ढाँचा <typename TagGetter>
  शून्य collect_name_ids (अचर hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) अचर
  {
    लागि (अचर FeatureTableSubstitutionRecord &record : substitutions)
      यदि (feature_index_map->has (record.featureIndex))
        (एतत्+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  शून्य closure_features (अचर hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) अचर
  {
    लागि (अचर FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (एतत्, lookup_indexes, feature_indexes);
  }

  तार्किक intersects_features (अचर hb_map_t *feature_index_map) अचर
  {
    लागि (अचर FeatureTableSubstitutionRecord& record : substitutions)
    {
      यदि (feature_index_map->has (record.featureIndex)) फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) अचर
  {
    लागि (अचर FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, एतत्);
  }

  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               तार्किक घुसाउ_catch_all) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    out->version.major = version.major;
    out->version.minor = version.minor;

    यदि (घुसाउ_catch_all)
    {
      लागि (अचिन्हित feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<अचर शून्य*, अचर शून्य*> *p;
        यदि (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (असत्य);
        स्वतः *o = out->substitutions.serialize_append (c->serializer);
        यदि (!o->serialize (l, feature_index,
                           reinterpret_cast<अचर Feature*> (p->first),
                           reinterpret_cast<अचर Tag*> (p->second)))
          return_trace (असत्य);
      }
      return_trace (सत्य);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), एतत्))
    ;

    return_trace (तार्किक (out->substitutions));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर FeatureList *feature_list) अचर
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
  मित्र संरचना FeatureVariations;

  शून्य collect_lookups (अचर शून्य     *base,
			अचर hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) अचर
  {
    फर्काऊ (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  ढाँचा <typename TagGetter>
  शून्य collect_name_ids (अचर शून्य *base,
                         अचर hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) अचर
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  शून्य closure_features (अचर शून्य     *base,
			 अचर hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) अचर
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  तार्किक intersects_features (अचर शून्य *base, अचर hb_map_t *feature_index_map) अचर
  {
    फर्काऊ (base+substitutions).intersects_features (feature_index_map);
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    अचर शून्य *base) अचर
  {
    (base+conditions).keep_with_variations (c);
    यदि (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = सत्य; // set variations only once
    }
  }

  तार्किक subset (hb_subset_layout_context_t *c, अचर शून्य *base,
               तार्किक घुसाउ_catch_all = असत्य) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->subset_context->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    यदि (!घुसाउ_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    अन्यथा
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, घुसाउ_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, घुसाउ_catch_all);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base,
		 अचर FeatureList *feature_list) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  अचर शून्य *base) अचर
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
  शून्य add_lookup_indexes_to (hb_set_t *lookup_indexes) अचर
  { lookupIndices.add_indexes_to (lookup_indexes); }

  तार्किक intersects_lookup_indexes (अचर hb_map_t *lookup_indexes) अचर
  { फर्काऊ lookupIndices.intersects (lookup_indexes); }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);

    स्वतः it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (तार्किक (out->lookupIndices));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  अचिन्हित get_condition_offset (अचर शून्य *base, अचर शून्य *variations) अचर
  { फर्काऊ (अचर वर्ण *) &(base+condition) - (अचर वर्ण *) variations; }

  ढाँचा <typename Instancer>
  तार्किक evaluate (अचर शून्य *base,
		 अचर पूर्णाङ्क *coords,
		 अचिन्हित पूर्णाङ्क coord_len,
		 Instancer *instancer) अचर
  { फर्काऊ (base+condition).evaluate (coords, coord_len, instancer); }

  तार्किक evaluate (अचर शून्य *base,
                 अचर शून्य *variations,
                 hb_sorted_array_t<अचर अचिन्हित> conditions) अचर
  {
    फर्काऊ !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     परिमाण (अचिन्हित),
                                     _hb_cmp_operator<अचिन्हित, अचिन्हित>);
  }

  तार्किक collect_condition (अचर शून्य *base,
                          अचर शून्य *variations,
                          अचर पूर्णाङ्क *coords,
                          अचिन्हित पूर्णाङ्क coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<अचिन्हित> *conditions) अचर
  {
    /* Null conditions always apply and need no key state. */
    यदि (!condition) फर्काऊ सत्य;

    अचिन्हित offset = get_condition_offset (base, variations);
    यदि (visited->has (offset)) फर्काऊ सत्य;
    visited->add (offset);
    यदि (unlikely (visited->in_error ())) फर्काऊ असत्य;

    यदि (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    फर्काऊ !conditions->in_error ();
  }

  शून्य add_lookup_indexes_to (अचर शून्य *base,
			      hb_set_t *lookup_indexes) अचर
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  तार्किक intersects_lookup_indexes (अचर शून्य *base,
				   अचर hb_map_t *lookup_indexes) अचर
  { फर्काऊ (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  अचर शून्य *base) अचर
  {
    यदि (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_मानहरू)))
      c->layout_variation_indices->err ();
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       अचर शून्य *base) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out)) return_trace (असत्य);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    सत्य);
    यदि (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->झिक_discard ();
      return_trace (असत्य);
    }
    c->serializer->add_link (out->condition, c->serializer->झिक_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
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
  तार्किक adds_default_lookups () अचर { फर्काऊ flags & 0x0001u; }

  तार्किक collect_conditions (अचर शून्य *variations,
                           अचर पूर्णाङ्क *coords,
                           अचिन्हित पूर्णाङ्क coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<अचिन्हित> *conditions) अचर
  {
    लागि (अचर LookupConditionRecord &record : records)
      यदि (unlikely (!record.collect_condition (एतत्, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	फर्काऊ असत्य;
    फर्काऊ सत्य;
  }

  शून्य collect_lookups (अचर शून्य *variations,
                       hb_sorted_array_t<अचर अचिन्हित> conditions,
                       hb_set_t *lookup_indexes) अचर
  {
    लागि (अचर LookupConditionRecord &record : records)
      यदि (record.evaluate (एतत्, variations, conditions))
	record.add_lookup_indexes_to (एतत्, lookup_indexes);
  }

  ढाँचा <typename Instancer>
  शून्य collect_lookups (अचर पूर्णाङ्क *coords,
			अचिन्हित पूर्णाङ्क coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) अचर
  {
    लागि (अचर LookupConditionRecord &record : records)
      यदि (record.evaluate (एतत्, coords, coord_len, instancer))
	record.add_lookup_indexes_to (एतत्, lookup_indexes);
  }

  शून्य collect_lookups (hb_set_t *lookup_indexes) अचर
  {
    लागि (अचर LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (एतत्, lookup_indexes);
  }

  तार्किक intersects_lookup_indexes (अचर hb_map_t *lookup_indexes) अचर
  {
    लागि (अचर LookupConditionRecord &record : records)
      यदि (record.intersects_lookup_indexes (एतत्, lookup_indexes))
	फर्काऊ सत्य;
    फर्काऊ असत्य;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर LookupConditionRecord &record : records)
      record.collect_variation_indices (c, एतत्);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    लागि (अचर LookupConditionRecord &record : records)
    {
      स्वतः snap = c->serializer->snapshot ();
      यदि (record.subset (c, l, एतत्))
	out->records.len++;
      अन्यथा
	c->serializer->revert (snap);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
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
  मित्र संरचना FeatureVariations;

  अचिन्हित get_feature_index () अचर { फर्काऊ featureIndex; }

  शून्य collect_lookups (अचर शून्य *base,
                       अचर Feature &feature,
                       hb_sorted_array_t<अचर अचिन्हित> conditions,
                       hb_set_t *lookup_indexes) अचर
  {
    अचर FeatureLookupsTable &feature_lookups = base+featureLookups;
    यदि (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  शून्य collect_lookups (अचर शून्य *base,
			अचर hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) अचर
  {
    यदि (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  शून्य closure_features (अचर शून्य *base,
			 अचर hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) अचर
  {
    यदि ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  अचर शून्य *base) अचर
  { (base+featureLookups).collect_variation_indices (c); }

  तार्किक subset (hb_subset_layout_context_t *l, अचर शून्य *base) अचर
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (एतत्);
    uint32_t *new_feature_index;
    यदि (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (असत्य);

    स्वतः *out = c->serializer->embed (एतत्);
    यदि (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर शून्य *base) अचर
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
  स्थिर constexpr अचिन्हित NOT_FOUND_INDEX = 0xFFFFFFFFu;

  अचर Array32Of<LookupVariationRecord> &get_lookup_variation_records () अचर
  {
    यदि (version.to_int () < 0x00010001u)
      फर्काऊ Null (Array32Of<LookupVariationRecord>);
    फर्काऊ StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  अचिन्हित record_गन्ती () अचर
  {
    फर्काऊ varRecords.len;
  }

  तार्किक find_index (अचर पूर्णाङ्क *coords, अचिन्हित पूर्णाङ्क coord_len,
		   अचिन्हित पूर्णाङ्क *index,
		   ItemVarStoreInstancer *instancer) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = varRecords.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      अचर FeatureVariationRecord &record = varRecords.arrayZ[i];
      यदि ((एतत्+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	फर्काऊ सत्य;
      }
    }
    *index = NOT_FOUND_INDEX;
    फर्काऊ असत्य;
  }

  अचर Feature *find_substitute (अचिन्हित पूर्णाङ्क variations_index,
				  अचिन्हित पूर्णाङ्क feature_index) अचर
  {
    अचर FeatureVariationRecord &record = varRecords[variations_index];
    फर्काऊ (एतत्+record.substitutions).find_substitute (feature_index);
  }

  तार्किक collect_lookup_conditions (अचर पूर्णाङ्क *coords,
                                  अचिन्हित पूर्णाङ्क coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<अचिन्हित> *state) अचर
  {
    hb_set_t visited_tables, visited_conditions;
    लागि (अचर LookupVariationRecord &record : get_lookup_variation_records ())
    {
      अचिन्हित offset = record.featureLookups;
      यदि (visited_tables.has (offset)) जारी;
      visited_tables.add (offset);
      यदि (unlikely (visited_tables.in_error ())) फर्काऊ असत्य;

      अचर FeatureLookupsTable &feature_lookups = एतत्+record.featureLookups;
      यदि (unlikely (!feature_lookups.collect_conditions (एतत्, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	फर्काऊ असत्य;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->क्विक (_hb_cmp_operator<अचिन्हित, अचिन्हित>);
    फर्काऊ सत्य;
  }

  तार्किक get_lookup_variations (अचिन्हित feature_index,
                              अचर Feature &feature,
                              hb_sorted_array_t<अचर अचिन्हित> conditions,
                              hb_set_t *lookup_indexes) अचर
  {
    लागि (अचर LookupVariationRecord &record : get_lookup_variation_records ())
    {
      यदि (record.featureIndex != feature_index) जारी;

      record.collect_lookups (एतत्, feature, conditions, lookup_indexes);
      फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = varRecords.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, एतत्);
      यदि (c->universal)
        रोक;
    }
    यदि (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed (*एतत्));
  }

  शून्य collect_lookups (अचर hb_set_t *feature_indexes,
			अचर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) अचर
  {
    अचिन्हित गन्ती = varRecords.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        जारी;
      varRecords[i].collect_lookups (एतत्, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  ढाँचा <typename TagGetter>
  शून्य collect_name_ids (अचर hb_map_t *feature_index_map,
                         अचर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) अचर
  {
    लागि (अचिन्हित i = 0; i < varRecords.len; i++)
    {
      यदि (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        जारी;
      varRecords[i].collect_name_ids (एतत्, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  शून्य collect_lookup_variation_lookups (अचर hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) अचर
  {
    लागि (अचर LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (एतत्, feature_indexes, lookup_indexes);
  }

  शून्य closure_features (अचर hb_map_t *lookup_indexes,
			 अचर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) अचर
  {
    अचिन्हित पूर्णाङ्क गन्ती = varRecords.len;
    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
    {
      यदि (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        जारी;
      varRecords[i].closure_features (एतत्, lookup_indexes, feature_indexes);
    }

    लागि (अचर LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (एतत्, lookup_indexes, feature_indexes);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचर FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, एतत्);
    लागि (अचर LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, एतत्);
  }

  शून्य collect_feature_indices (hb_set_t *feature_indices) अचर
  {
    लागि (अचर FeatureVariationRecord &record : varRecords)
      (एतत्+record.substitutions).collect_feature_indices (feature_indices);
  }

  तार्किक has_lookup_variations (अचिन्हित feature_index) अचर
  {
    लागि (अचर LookupVariationRecord &record : get_lookup_variation_records ())
    {
      यदि (record.featureIndex == feature_index)
	फर्काऊ सत्य;
      यदि (record.featureIndex > feature_index)
	रोक;
    }
    फर्काऊ असत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) अचर
  {
    TRACE_SUBSET (एतत्);
    स्वतः *out = c->serializer->start_embed (*एतत्);
    यदि (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    out->version.major = version.major;
    out->version.minor = version.minor;

    पूर्णाङ्क keep_up_to = -1;
    लागि (पूर्णाङ्क i = varRecords.len - 1; i >= 0; i--) {
      यदि (varRecords[i].intersects_features (एतत्, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        रोक;
      }
    }

    अचिन्हित गन्ती = (अचिन्हित) (keep_up_to + 1);
    लागि (अचिन्हित i = 0; i < गन्ती; i++)
    {
      यदि (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        जारी;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), एतत्) (varRecords[i]);
    }

    यदि (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      तार्किक घुसाउ_catch_all_record = सत्य;
      subset_record_array (l, &(out->varRecords), एतत्, घुसाउ_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    यदि (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      यदि (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (असत्य);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, एतत्))
      ;
    }

    return_trace (तार्किक (out->varRecords) ||
		  (out_lookup_records && तार्किक (*out_lookup_records)));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, अचर FeatureList *feature_list) अचर
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
  मित्र संरचना Device;

  गुप्त:

  hb_position_t get_x_delta (hb_font_t *font) अचर
  { फर्काऊ get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) अचर
  { फर्काऊ get_delta (font->y_ppem, font->y_scale); }

  सार्वजनिक:

  माप_प्रकार get_size () अचर
  {
    अचिन्हित पूर्णाङ्क f = deltaFormat;
    यदि (unlikely (f < 1 || f > 3 || startSize > endSize)) फर्काऊ 3 * HBUINT16::static_size;
    फर्काऊ HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) && c->check_range (एतत्, एतत्->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) अचर
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (c->embed<HintingDevice> (एतत्));
  }

  गुप्त:

  पूर्णाङ्क get_delta (अचिन्हित पूर्णाङ्क ppem, पूर्णाङ्क scale) अचर
  {
    यदि (!ppem) फर्काऊ 0;

    पूर्णाङ्क pixels = get_delta_pixels (ppem);

    यदि (!pixels) फर्काऊ 0;

    फर्काऊ (पूर्णाङ्क) (pixels * (int64_t) scale / ppem);
  }
  पूर्णाङ्क get_delta_pixels (अचिन्हित पूर्णाङ्क ppem_size) अचर
  {
    अचिन्हित पूर्णाङ्क f = deltaFormat;
    यदि (unlikely (f < 1 || f > 3))
      फर्काऊ 0;

    यदि (ppem_size < startSize || ppem_size > endSize)
      फर्काऊ 0;

    अचिन्हित पूर्णाङ्क s = ppem_size - startSize;

    अचिन्हित पूर्णाङ्क byte = delta_मान_Z[s >> (4 - f)];
    अचिन्हित पूर्णाङ्क bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    अचिन्हित पूर्णाङ्क mask = (0xFFFFu >> (16 - (1 << f)));

    पूर्णाङ्क delta = bits & mask;

    यदि ((अचिन्हित पूर्णाङ्क) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    फर्काऊ delta;
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
		delta_मान_Z;		/* Array of compressed data */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, delta_मान_Z);
};

संरचना VariationDevice
{
  मित्र संरचना Device;

  गुप्त:

  hb_position_t get_x_delta (hb_font_t *font,
			     अचर ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) अचर
  { फर्काऊ !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     अचर ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) अचर
  { फर्काऊ !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  भिन्न get_x_delta_f (hb_font_t *font,
		       अचर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) अचर
  { फर्काऊ !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  भिन्न get_y_delta_f (hb_font_t *font,
		       अचर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) अचर
  { फर्काऊ !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map) अचर
  {
    TRACE_SERIALIZE (एतत्);
    यदि (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<अचिन्हित, पूर्णाङ्क> *v;
    यदि (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    यदि (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (एतत्->static_size);
    स्वतः *out = c->embed (एतत्);
    यदि (unlikely (!out)) return_trace (nullptr);

    यदि (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  शून्य collect_variation_index (hb_collect_variation_indices_context_t *c) अचर
  { c->layout_variation_indices->add (varIdx); }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्));
  }

  गुप्त:

  भिन्न get_delta (hb_font_t *font,
		   अचर ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) अचर
  {
    फर्काऊ store.get_delta (varIdx, font->coords, font->अङ्क_coords, store_cache);
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
		       अचर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) अचर
  {
#ifndef HB_NO_VAR
    यदि (u.b.format == 0x8000)
    {
      hb_barrier ();
      फर्काऊ u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    फर्काऊ get_x_delta (font, store, store_cache);
  }

  भिन्न get_y_delta_f (hb_font_t *font,
		       अचर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) अचर
  {
#ifndef HB_NO_VAR
    यदि (u.b.format == 0x8000)
    {
      hb_barrier ();
      फर्काऊ u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    फर्काऊ get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     अचर ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) अचर
  {
    विकल्प (u.b.format)
    {
#ifndef HB_NO_HINTING
    पक्ष 1: पक्ष 2: पक्ष 3:
      hb_barrier ();
      फर्काऊ u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      फर्काऊ u.variation.get_x_delta (font, store, store_cache);
#endif
    स्वभाव:
      फर्काऊ 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     अचर ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) अचर
  {
    विकल्प (u.b.format)
    {
    पक्ष 1: पक्ष 2: पक्ष 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      फर्काऊ u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      फर्काऊ u.variation.get_y_delta (font, store, store_cache);
#endif
    स्वभाव:
      फर्काऊ 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    यदि (!u.b.format.sanitize (c)) return_trace (असत्य);
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
      return_trace (सत्य);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map=nullptr) अचर
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

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    विकल्प (u.b.format) {
#ifndef HB_NO_HINTING
    पक्ष 1:
    पक्ष 2:
    पक्ष 3:
      फर्काऊ;
#endif
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      फर्काऊ;
#endif
    स्वभाव:
      फर्काऊ;
    }
  }

  अचिन्हित get_variation_index () अचर
  {
    विकल्प (u.b.format) {
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      hb_barrier ();
      फर्काऊ u.variation.varIdx;
#endif
    स्वभाव:
      फर्काऊ HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  तार्किक is_variation_device () अचर
  {
    विकल्प (u.b.format) {
#ifndef HB_NO_VAR
    पक्ष 0x8000:
      फर्काऊ सत्य;
#endif
    स्वभाव:
      फर्काऊ असत्य;
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

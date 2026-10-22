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
static_assert (आकार (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_द्विमान_cache_t = hb_cache_t<14, 1, 8>;
static_assert (आकार (hb_ot_layout_द्विमान_cache_t) == 256, "");

#अन्तर्भाव "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


नामाकाश OT {

ढाँचा<typename Iterator>
स्थिर ओळीत तार्किक ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

स्थिर तार्किक ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    स्थिरांक hb_set_t &klasses,
    तार्किक use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

संरचना ItemVarStoreInstancer;

संरचना hb_collect_feature_substitutes_with_var_context_t
{
  स्थिरांक hb_map_t *axes_index_tag_map;
  स्थिरांक hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<अचिन्हित, स्थिरांक Feature*> *feature_substitutes_map;
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
  hb_prune_langsys_context_t (स्थिरांक शून्य         *table_,
                              hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              स्थिरांक hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_मोजणी (0),langsys_feature_मोजणी (0) {}

  तार्किक visitScript ()
  { परत script_मोजणी++ < HB_MAX_SCRIPTS; }

  तार्किक visitLangsys (अचिन्हित feature_मोजणी_2)
  {
    langsys_feature_मोजणी += feature_मोजणी_2;
    परत langsys_feature_मोजणी < HB_MAX_LANGSYS_FEATURE_मोजणी;
  }

  सार्वजनिक:
  स्थिरांक शून्य *table;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  स्थिरांक hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  खाजगी:
  अचिन्हित script_मोजणी;
  अचिन्हित langsys_feature_मोजणी;
};

संरचना hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  स्थिरांक अक्षर *get_name () { परत "SUBSET_LAYOUT"; }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  तार्किक visitScript ()
  {
    परत script_मोजणी++ < HB_MAX_SCRIPTS;
  }

  तार्किक visitLangSys ()
  {
    परत langsys_मोजणी++ < HB_MAX_LANGSYS;
  }

  तार्किक visitFeatureIndex (पूर्णांक मोजणी)
  {
    feature_index_मोजणी += मोजणी;
    परत feature_index_मोजणी < HB_MAX_FEATURE_INDICES;
  }

  तार्किक visitLookupIndex()
  {
    lookup_index_मोजणी++;
    परत lookup_index_मोजणी < HB_MAX_LOOKUP_VISIT_मोजणी;
  }

  hb_subset_context_t *subset_context;
  स्थिरांक hb_tag_t table_tag;
  स्थिरांक hb_map_t *lookup_index_map;
  स्थिरांक hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  स्थिरांक hb_map_t *feature_index_map;
  स्थिरांक hb_map_t *feature_map_w_duplicates;
  स्थिरांक hb_hashmap_t<अचिन्हित, स्थिरांक Feature*> *feature_substitutes_map;
  स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  स्थिरांक hb_set_t *catch_all_record_feature_idxes;
  स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<स्थिरांक शून्य*, स्थिरांक शून्य*>> *feature_idx_tag_map;

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
				script_मोजणी (0),
				langsys_मोजणी (0),
				feature_index_मोजणी (0),
				lookup_index_मोजणी (0)
  {
    जर (tag_ == HB_OT_TAG_GSUB)
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

  खाजगी:
  अचिन्हित script_मोजणी;
  अचिन्हित langsys_मोजणी;
  अचिन्हित feature_index_मोजणी;
  अचिन्हित lookup_index_मोजणी;
};

संरचना ItemVariationStore;
संरचना hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिरांक T &obj) { obj.collect_variation_indices (हे); परत hb_empty_t (); }
  स्थिर return_t default_return_मूल्य () { परत hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  स्थिरांक hb_set_t *glyph_set;
  स्थिरांक hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>> *condition_मूल्ये = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  स्थिरांक hb_set_t *glyph_set_,
					  स्थिरांक hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

ढाँचा<typename OutputArray>
संरचना subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 स्थिरांक शून्य *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  ढाँचा <typename T>
  तार्किक चालक () (T&& offset)
  {
    स्वतः snap = subset_context->serializer->snapshot ();
    स्वतः *o = out.serialize_append (subset_context->serializer);
    जर (unlikely (!o)) परत असत्य;
    तार्किक ret = o->serialize_subset (subset_context, offset, base);
    जर (!ret)
    {
      out.काढा ();
      subset_context->serializer->revert (snap);
    }
    परत ret;
  }

  खाजगी:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  स्थिरांक शून्य *base;
};


ढाँचा<typename OutputArray, typename Arg>
संरचना subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     स्थिरांक शून्य *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  ढाँचा <typename T>
  तार्किक चालक () (T&& offset)
  {
    स्वतः snap = subset_context->serializer->snapshot ();
    स्वतः *o = out.serialize_append (subset_context->serializer);
    जर (unlikely (!o)) परत असत्य;
    तार्किक ret = o->serialize_subset (subset_context, offset, base, arg);
    जर (!ret)
    {
      out.काढा ();
      subset_context->serializer->revert (snap);
    }
    परत ret;
  }

  खाजगी:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  स्थिरांक शून्य *base;
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
	       स्थिरांक शून्य *base) स्थिरांक
  { परत subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  ढाँचा<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  चालक () (hb_subset_context_t *subset_context, OutputArray& out,
	       स्थिरांक शून्य *base, Arg &&arg) स्थिरांक
  { परत subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

ढाँचा<typename OutputArray>
संरचना subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 स्थिरांक शून्य *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  ढाँचा <typename T>
  शून्य
  चालक () (T&& record)
  {
    स्वतः snap = subset_layout_context->subset_context->serializer->snapshot ();
    तार्किक ret = record.subset (subset_layout_context, base);
    जर (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अन्यथा out->len++;
  }

  खाजगी:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  स्थिरांक शून्य *base;
};

ढाँचा<typename OutputArray, typename Arg>
संरचना subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     स्थिरांक शून्य *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  ढाँचा <typename T>
  शून्य
  चालक () (T&& record)
  {
    स्वतः snap = subset_layout_context->subset_context->serializer->snapshot ();
    तार्किक ret = record.subset (subset_layout_context, base, arg);
    जर (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अन्यथा out->len++;
  }

  खाजगी:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  स्थिरांक शून्य *base;
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
	       स्थिरांक शून्य *base) स्थिरांक
  { परत subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  ढाँचा<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  चालक () (hb_subset_layout_context_t *c, OutputArray* out,
               स्थिरांक शून्य *base, Arg &&arg) स्थिरांक
  { परत subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


ढाँचा<typename OutputArray>
संरचना serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         स्थिरांक शून्य *base_,
                         स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  ढाँचा <typename T>
  तार्किक चालक () (T&& record)
  {
    जर (!serialize_context->copy (record, base, variation_map)) परत असत्य;
    out.len++;
    परत सत्य;
  }

  खाजगी:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  स्थिरांक शून्य *base;
  स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
संरचना
{
  ढाँचा<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  चालक () (hb_serialize_context_t *serialize_context, OutputArray& out,
               स्थिरांक शून्य *base,
               स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map) स्थिरांक
  { परत serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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
  तार्किक intersects (स्थिरांक hb_map_t *indexes) स्थिरांक
  { परत hb_any (*हे, indexes); }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    जर (!it) परत;
    जर (unlikely (!c->extend_min ((*हे)))) परत;

    साठी (स्थिरांक स्वतः _ : it)
    {
      जर (!l->visitLookupIndex()) थांबा;

      Index i;
      i = _;
      c->copy (i);
      हे->len++;
    }
  }

  अचिन्हित पूर्णांक get_indexes (अचिन्हित पूर्णांक start_offset,
			    अचिन्हित पूर्णांक *मोजणी_2 /* IN/OUT */,
			    अचिन्हित पूर्णांक *_indexes /* OUT */) स्थिरांक
  {
    जर (मोजणी_2 && _indexes)
    {
      + हे->as_array ().sub_array (start_offset, मोजणी_2)
      | hb_sink (hb_array (_indexes, *मोजणी_2))
      ;
    }
    परत हे->len;
  }

  शून्य add_indexes_to (hb_set_t* output /* OUT */) स्थिरांक
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
संरचना FeatureParamsSize
{
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे))) return_trace (असत्य);
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

    जर (!designSize)
      return_trace (असत्य);
    अन्यथा जर (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (सत्य);
    अन्यथा जर (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (असत्य);
    अन्यथा
      return_trace (सत्य);
  }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  { nameids_to_retain->add (subfamilyNameID); }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    return_trace ((तार्किक) c->serializer->embed (*हे));
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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (हे));
  }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  { nameids_to_retain->add (uiNameID); }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    return_trace ((तार्किक) c->serializer->embed (*हे));
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
  get_characters (अचिन्हित start_offset, अचिन्हित *char_मोजणी, hb_codepoint_t *chars) स्थिरांक
  {
    जर (char_मोजणी && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_मोजणी)
      | hb_sink (hb_array (chars, *char_मोजणी))
      ;
    }
    परत characters.len;
  }

  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  शून्य collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  {
    जर (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    जर (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    जर (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    जर (!firstParamUILabelNameID || !अंक_Named_Parameters || अंक_Named_Parameters >= 0x7FFF)
      परत;

    अचिन्हित last_name_id = (अचिन्हित) firstParamUILabelNameID + (अचिन्हित) अंक_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    return_trace ((तार्किक) c->serializer->embed (*हे));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
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
  HBUINT16	अंक_Named_Parameters;	/* Number of named parameters. (May
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
  तार्किक sanitize (hb_sanitize_context_t *c, hb_tag_t tag) स्थिरांक
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    परत सत्य;
#endif
    TRACE_SANITIZE (हे);
    जर (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    जर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    जर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (सत्य);
  }

  शून्य collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    परत;
#endif
    जर (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      परत (u.size.collect_name_ids (nameids_to_retain));
    जर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      परत (u.stylisticSet.collect_name_ids (nameids_to_retain));
    जर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      परत (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  तार्किक subset (hb_subset_context_t *c, स्थिरांक Tag* tag) स्थिरांक
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    परत असत्य;
#endif
    TRACE_SUBSET (हे);
    जर (!tag) return_trace (असत्य);
    जर (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    जर ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    जर ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (असत्य);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  स्थिरांक FeatureParamsSize& get_size_params (hb_tag_t tag) स्थिरांक
  {
    जर (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      परत u.size;
    परत Null (FeatureParamsSize);
  }
  स्थिरांक FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) स्थिरांक
  {
    जर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      परत u.stylisticSet;
    परत Null (FeatureParamsStylisticSet);
  }
  स्थिरांक FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) स्थिरांक
  {
    जर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      परत u.characterVariants;
    परत Null (FeatureParamsCharacterVariants);
  }
#endif

  खाजगी:
  संघ {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_MIN (0);
};

संरचना Record_sanitize_closure_t {
  hb_tag_t tag;
  स्थिरांक शून्य *list_base;
};

संरचना Feature
{
  अचिन्हित पूर्णांक get_lookup_मोजणी () स्थिरांक
  { परत lookupIndex.len; }
  hb_tag_t get_lookup_index (अचिन्हित पूर्णांक i) स्थिरांक
  { परत lookupIndex[i]; }
  अचिन्हित पूर्णांक get_lookup_indexes (अचिन्हित पूर्णांक start_index,
				   अचिन्हित पूर्णांक *lookup_मोजणी /* IN/OUT */,
				   अचिन्हित पूर्णांक *lookup_tags /* OUT */) स्थिरांक
  { परत lookupIndex.get_indexes (start_index, lookup_मोजणी, lookup_tags); }
  शून्य add_lookup_indexes_to (hb_set_t *lookup_indexes) स्थिरांक
  { lookupIndex.add_indexes_to (lookup_indexes); }

  स्थिरांक FeatureParams &get_feature_params () स्थिरांक
  { परत हे+featureParams; }

  तार्किक intersects_lookup_indexes (स्थिरांक hb_map_t *lookup_indexes) स्थिरांक
  { परत lookupIndex.intersects (lookup_indexes); }

  शून्य collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) स्थिरांक
  {
    जर (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  तार्किक subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       स्थिरांक Tag                   *tag = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    out->featureParams.serialize_subset (c, featureParams, हे, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    जर (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
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
		 स्थिरांक Record_sanitize_closure_t *closure = nullptr) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  featureParams.sanitize (c, हे, closure ? closure->tag : HB_TAG_NONE) &&
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
  पूर्णांक cmp (hb_tag_t a) स्थिरांक { परत tag.cmp (a); }

  तार्किक subset (hb_subset_layout_context_t *c, स्थिरांक शून्य *base, स्थिरांक शून्य *f_sub = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->subset_context->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    स्थिरांक Feature& f = *reinterpret_cast<स्थिरांक Feature *> (f_sub);
    स्वतः *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    तार्किक ret = f.subset (c->subset_context, c, &tag);
    जर (ret)
      s->add_link (out->offset, s->काढा_pack ());
    अन्यथा
      s->काढा_discard ();

    return_trace (ret);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    स्थिरांक Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (हे) &&
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
  स्थिरांक Offset16To<Type>& get_offset (अचिन्हित पूर्णांक i) स्थिरांक
  { परत (*हे)[i].offset; }
  Offset16To<Type>& get_offset (अचिन्हित पूर्णांक i)
  { परत (*हे)[i].offset; }
  स्थिरांक Tag& get_tag (अचिन्हित पूर्णांक i) स्थिरांक
  { परत (*हे)[i].tag; }
  अचिन्हित पूर्णांक get_tags (अचिन्हित पूर्णांक start_offset,
                         अचिन्हित पूर्णांक *record_मोजणी /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) स्थिरांक
  {
    जर (record_मोजणी && record_tags)
    {
      + हे->as_array ().sub_array (start_offset, record_मोजणी)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_मोजणी))
      ;
    }
    परत हे->len;
  }
  तार्किक find_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  {
    परत हे->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

ढाँचा <typename Type>
संरचना RecordListOf : RecordArrayOf<Type>
{
  स्थिरांक Type& चालक [] (अचिन्हित पूर्णांक i) स्थिरांक
  { परत हे+हे->get_offset (i); }

  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + हे->iter ()
    | hb_apply (subset_record_array (l, out, हे))
    ;
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (RecordArrayOf<Type>::sanitize (c, हे));
  }
};

संरचना RecordListOfFeature : RecordListOf<Feature>
{
  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + hb_enumerate (*हे)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, हे] (स्थिरांक hb_pair_t<अचिन्हित, स्थिरांक Record<Feature>&>& _)
                {
                  स्थिरांक Feature *f_sub = nullptr;
                  स्थिरांक Feature **f = nullptr;
                  जर (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, हे, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (सत्य);
  }
};

प्रकारव्याख्या RecordListOf<Feature> FeatureList;


संरचना LangSys
{
  अचिन्हित पूर्णांक get_feature_मोजणी () स्थिरांक
  { परत featureIndex.len; }
  hb_tag_t get_feature_index (अचिन्हित पूर्णांक i) स्थिरांक
  { परत featureIndex[i]; }
  अचिन्हित पूर्णांक get_feature_indexes (अचिन्हित पूर्णांक start_offset,
				    अचिन्हित पूर्णांक *feature_मोजणी_2 /* IN/OUT */,
				    अचिन्हित पूर्णांक *feature_indexes /* OUT */) स्थिरांक
  { परत featureIndex.get_indexes (start_offset, feature_मोजणी_2, feature_indexes); }
  शून्य add_feature_indexes_to (hb_set_t *feature_indexes) स्थिरांक
  { featureIndex.add_indexes_to (feature_indexes); }

  तार्किक has_required_feature () स्थिरांक { परत reqFeatureIndex != 0xFFFFu; }
  अचिन्हित पूर्णांक get_required_feature_index () स्थिरांक
  {
    जर (reqFeatureIndex == 0xFFFFu)
      परत Index::NOT_FOUND_INDEX;
   परत reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (*हे));
  }

  तार्किक compare (स्थिरांक LangSys& o, स्थिरांक hb_map_t *feature_index_map) स्थिरांक
  {
    जर (reqFeatureIndex != o.reqFeatureIndex)
      परत असत्य;

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

    साठी (; iter && o_iter; iter++, o_iter++)
    {
      अचिन्हित a = *iter;
      अचिन्हित b = *o_iter;
      जर (a != b) परत असत्य;
    }

    जर (iter || o_iter) परत असत्य;

    परत सत्य;
  }

  शून्य collect_features (hb_prune_langsys_context_t *c) स्थिरांक
  {
    जर (!has_required_feature () && !get_feature_मोजणी ()) परत;
    जर (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       स्थिरांक Tag                  *tag = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    स्थिरांक uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    जर (!l->visitFeatureIndex (featureIndex.len))
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
		 स्थिरांक Record_sanitize_closure_t * = nullptr) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && featureIndex.sanitize (c));
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
  अचिन्हित पूर्णांक get_lang_sys_मोजणी () स्थिरांक
  { परत langSys.len; }
  स्थिरांक Tag& get_lang_sys_tag (अचिन्हित पूर्णांक i) स्थिरांक
  { परत langSys.get_tag (i); }
  अचिन्हित पूर्णांक get_lang_sys_tags (अचिन्हित पूर्णांक start_offset,
				  अचिन्हित पूर्णांक *lang_sys_मोजणी /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) स्थिरांक
  { परत langSys.get_tags (start_offset, lang_sys_मोजणी, lang_sys_tags); }
  स्थिरांक LangSys& get_lang_sys (अचिन्हित पूर्णांक i) स्थिरांक
  {
    जर (i == Index::NOT_FOUND_INDEX) परत get_default_lang_sys ();
    परत हे+langSys[i].offset;
  }
  तार्किक find_lang_sys_index (hb_tag_t tag, अचिन्हित पूर्णांक *index) स्थिरांक
  { परत langSys.find_index (tag, index); }

  तार्किक has_default_lang_sys () स्थिरांक           { परत defaultLangSys != 0; }
  स्थिरांक LangSys& get_default_lang_sys () स्थिरांक { परत हे+defaultLangSys; }

  शून्य prune_langsys (hb_prune_langsys_context_t *c,
                      अचिन्हित script_index) स्थिरांक
  {
    जर (!has_default_lang_sys () && !get_lang_sys_मोजणी ()) परत;
    जर (!c->visitScript ()) परत;

    जर (!c->script_langsys_map->has (script_index))
    {
      जर (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	परत;
    }

    जर (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      स्थिरांक LangSys& d = get_default_lang_sys ();
      जर (c->visitLangsys (d.get_feature_मोजणी ())) {
        d.collect_features (c);
      }

      साठी (स्वतः _ : + hb_enumerate (langSys))
      {
        स्थिरांक LangSys& l = हे+_.second.offset;
        जर (!c->visitLangsys (l.get_feature_मोजणी ())) सुरूठेवा;
        जर (l.compare (d, c->duplicate_feature_map)) सुरूठेवा;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    अन्यथा
    {
      साठी (स्वतः _ : + hb_enumerate (langSys))
      {
        स्थिरांक LangSys& l = हे+_.second.offset;
        जर (!c->visitLangsys (l.get_feature_मोजणी ())) सुरूठेवा;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  तार्किक subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       स्थिरांक Tag                   *tag) स्थिरांक
  {
    TRACE_SUBSET (हे);
    जर (!l->visitScript ()) return_trace (असत्य);
    जर (tag && !c->plan->layout_scripts.has (*tag))
      परत असत्य;

    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    तार्किक defaultLang = असत्य;
    जर (has_default_lang_sys ())
    {
      c->serializer->push ();
      स्थिरांक LangSys& ls = हे+defaultLangSys;
      तार्किक ret = ls.subset (c, l);
      जर (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->काढा_discard ();
	out->defaultLangSys = 0;
      }
      अन्यथा
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->काढा_pack ());
	defaultLang = सत्य;
      }
    }

    स्थिरांक hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    जर (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (स्थिरांक Record<LangSys>& record) {परत l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), हे))
      ;
    }

    return_trace (तार्किक (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  तार्किक sanitize (hb_sanitize_context_t *c,
		 स्थिरांक Record_sanitize_closure_t * = nullptr) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (defaultLangSys.sanitize (c, हे) && langSys.sanitize (c, हे));
  }

  संरक्षित:
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
               hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    साठी (स्वतः _ : + hb_enumerate (*हे))
    {
      स्वतः snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      तार्किक ret = _.second.subset (l, हे);
      जर (!ret) c->serializer->revert (snap);
      अन्यथा out->len++;
    }

    return_trace (सत्य);
  }
};

प्रकारव्याख्या RecordListOfScript ScriptList;



संरचना LookupFlag : HBUINT16
{
  गणना Flags {
    उजवा_To_डावा		= 0x0001u,
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
  अचिन्हित पूर्णांक get_subtable_मोजणी () स्थिरांक { परत subTable.len; }

  ढाँचा <typename TSubTable>
  स्थिरांक Array16OfOffset16To<TSubTable>& get_subtables () स्थिरांक
  { परत reinterpret_cast<स्थिरांक Array16OfOffset16To<TSubTable> &> (subTable); }
  ढाँचा <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { परत reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  ढाँचा <typename TSubTable>
  स्थिरांक TSubTable& get_subtable (अचिन्हित पूर्णांक i) स्थिरांक
  { परत हे+get_subtables<TSubTable> ()[i]; }
  ढाँचा <typename TSubTable>
  TSubTable& get_subtable (अचिन्हित पूर्णांक i)
  { परत हे+get_subtables<TSubTable> ()[i]; }

  माप_प्रकार get_size () स्थिरांक
  {
    स्थिरांक HBUINT16 &markFilteringSet = StructAfter<स्थिरांक HBUINT16> (subTable);
    जर (lookupFlag & LookupFlag::UseMarkFilteringSet)
      परत (स्थिरांक अक्षर *) &StructAfter<स्थिरांक अक्षर> (markFilteringSet) - (स्थिरांक अक्षर *) हे;
    परत (स्थिरांक अक्षर *) &markFilteringSet - (स्थिरांक अक्षर *) हे;
  }

  अचिन्हित पूर्णांक get_type () स्थिरांक { परत lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () स्थिरांक
  {
    अचिन्हित पूर्णांक flag = lookupFlag;
    जर (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      स्थिरांक HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    परत flag;
  }

  ढाँचा <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    अचिन्हित पूर्णांक lookup_type = get_type ();
    TRACE_DISPATCH (हे, lookup_type);
    अचिन्हित पूर्णांक मोजणी = get_subtable_मोजणी ();
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      जर (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_मूल्य ());
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  अचिन्हित पूर्णांक lookup_type,
		  uint32_t lookup_props,
		  अचिन्हित पूर्णांक अंक_subtables)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    जर (unlikely (!subTable.serialize (c, अंक_subtables))) return_trace (असत्य);
    जर (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      जर (unlikely (!c->extend (हे))) return_trace (असत्य);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (सत्य);
  }

  ढाँचा <typename TSubTable>
  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    स्थिरांक hb_set_t *glyphset = c->plan->glyphset_gsub ();
    अचिन्हित पूर्णांक lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([हे, glyphset, lookup_type] (स्थिरांक Offset16To<TSubTable> &_) { परत (हे+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), हे, lookup_type))
    ;

    जर (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      स्थिरांक HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      जर (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
        जर (unlikely (!c->serializer->extend (out))) return_trace (असत्य);
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
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!(c->check_struct (हे) && subTable.sanitize (c))) return_trace (असत्य);
    hb_barrier ();

    अचिन्हित subtables = get_subtable_मोजणी ();
    जर (unlikely (!c->visit_subtables (subtables))) return_trace (असत्य);

    जर (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      स्थिरांक HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      जर (!markFilteringSet.sanitize (c)) return_trace (असत्य);
    }

    जर (unlikely (!get_subtables<TSubTable> ().sanitize (c, हे, get_type ())))
      return_trace (असत्य);

    जर (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      अचिन्हित पूर्णांक type = get_subtable<TSubTable> (0).u.extension.get_type ();
      साठी (अचिन्हित पूर्णांक i = 1; i < subtables; i++)
	जर (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  संरक्षित:
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
	       hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    + hb_enumerate (*हे)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, हे))
    ;
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, हे)));
  }
};


/*
 * Coverage Table
 */


स्थिर तार्किक ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  स्थिरांक hb_set_t &klasses,
                                          तार्किक use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  जर (!klass_map)
    परत ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  जर (!use_class_zero)
    klass_map->set (0, 0);

  अचिन्हित idx = klass_map->has (0) ? 1 : 0;
  साठी (स्थिरांक अचिन्हित k: klasses)
  {
    जर (klass_map->has (k)) सुरूठेवा;
    klass_map->set (k, idx);
    idx++;
  }


  जर (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    परत असत्य;

  साठी (अचिन्हित i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  परत ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

ढाँचा <typename Types>
संरचना ClassDefFormat1_3
{
  मित्र संरचना ClassDef;

  खाजगी:
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id) स्थिरांक
  {
    परत class_मूल्य[(अचिन्हित पूर्णांक) (glyph_id - startGlyph)];
  }

  अचिन्हित get_population () स्थिरांक
  {
    परत class_मूल्य.len;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    जर (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_मूल्य.len = 0;
      return_trace (सत्य);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    अचिन्हित glyph_मोजणी = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    जर (unlikely (!class_मूल्य.serialize (c, glyph_मोजणी))) return_trace (असत्य);
    साठी (स्थिरांक hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      अचिन्हित idx = gid_klass_pair.first - glyph_min;
      class_मूल्य[idx] = gid_klass_pair.second;
    }
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सत्य,
               तार्किक use_class_zero = सत्य,
               स्थिरांक Coverage* glyph_filter = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_मूल्य.len;

    साठी (स्थिरांक hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      जर (new_gid == HB_MAP_मूल्य_INVALID) सुरूठेवा;
      जर (glyph_filter && !glyph_filter->has(gid)) सुरूठेवा;

      अचिन्हित klass = class_मूल्य[gid - start];
      जर (!klass) सुरूठेवा;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    जर (use_class_zero)
    {
      स्थिरांक hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      अचिन्हित glyph_मोजणी = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_मोजणी <= glyph_and_klass.length;
    }
    जर (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (असत्य);
    return_trace (keep_empty_table || (तार्किक) glyph_and_klass);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && class_मूल्य.sanitize (c));
  }

  अचिन्हित cost () स्थिरांक { परत 1; }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  {
    अचिन्हित पूर्णांक start = 0;
    अचिन्हित पूर्णांक मोजणी = class_मूल्य.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (class_मूल्य[i])
	सुरूठेवा;

      जर (start != i)
	जर (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  परत असत्य;

      start = i + 1;
    }
    जर (start != मोजणी)
      जर (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + मोजणी)))
	परत असत्य;

    परत सत्य;
  }

  ढाँचा <typename set_t>
  तार्किक collect_class (set_t *glyphs, अचिन्हित klass) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = class_मूल्य.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (class_मूल्य[i] == klass) glyphs->add (startGlyph + i);
    परत सत्य;
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_मूल्य.len;
    साठी (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      जर (class_मूल्य[iter - start]) परत सत्य;
    परत असत्य;
  }
  तार्किक intersects_class (स्थिरांक hb_set_t *glyphs, अचिन्हित klass) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = class_मूल्य.len;
    जर (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मूल्य_INVALID;
      जर (!glyphs->next (&g)) परत असत्य;
      जर (g < startGlyph) परत सत्य;
      g = startGlyph + मोजणी - 1;
      जर (glyphs->next (&g)) परत सत्य;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    स्थिरांक typename Types::HBUINT *arr = class_मूल्य.arrayZ;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (arr[i] == klass && glyphs->has (startGlyph + i))
	परत सत्य;
    परत असत्य;
  }

  शून्य intersected_class_glyphs (स्थिरांक hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) स्थिरांक
  {
    अचिन्हित मोजणी = class_मूल्य.len;
    जर (klass == 0)
    {
      अचिन्हित start_glyph = startGlyph;
      साठी (uint32_t g = HB_SET_मूल्य_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      साठी (uint32_t g = startGlyph + मोजणी - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      परत;
    }

    साठी (अचिन्हित i = 0; i < मोजणी; i++)
      जर (class_मूल्य[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#जर 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    अचिन्हित start_glyph = startGlyph;
    अचिन्हित end_glyph = start_glyph + मोजणी;
    साठी (अचिन्हित g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      जर (class_मूल्य.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  शून्य intersected_classes (स्थिरांक hb_set_t *glyphs, hb_set_t *intersect_classes) स्थिरांक
  {
    जर (glyphs->is_empty ()) परत;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    जर (!class_मूल्य.len)
    {
      intersect_classes->add (0);
      परत;
    }

    hb_codepoint_t end_glyph = startGlyph + class_मूल्य.len - 1;
    जर (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    साठी (स्थिरांक स्वतः& _ : + hb_enumerate (class_मूल्य))
    {
      hb_codepoint_t g = startGlyph + _.first;
      जर (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  संरक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::ढाँचा ArrayOf<typename Types::HBUINT>
		class_मूल्य;	/* Array of Class Values--one per GlyphID */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_मूल्य);
};

ढाँचा <typename Types>
संरचना ClassDefFormat2_4
{
  मित्र संरचना ClassDef;

  खाजगी:
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id) स्थिरांक
  {
    परत rangeRecord.द्वा_खोज (glyph_id).value;
  }

  अचिन्हित get_population () स्थिरांक
  {
    typename Types::large_int ret = 0;
    साठी (स्थिरांक स्वतः &r : rangeRecord)
      ret += r.get_population ();
    परत ret > अच_पूर्णांक_उच्च ? अच_पूर्णांक_उच्च : (अचिन्हित) ret;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    जर (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (सत्य);
    }

    अचिन्हित unsorted = असत्य;
    अचिन्हित अंक_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    अचिन्हित prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    स्वतः *record = c->copy (range_rec);
    जर (unlikely (!record)) return_trace (असत्य);

    साठी (स्थिरांक स्वतः gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      अचिन्हित cur_klass = gid_klass_pair.second;

      जर (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	जर (unlikely (cur_gid < prev_gid))
	  unsorted = सत्य;

	जर (unlikely (!record)) थांबा;
	record->last = prev_gid;
	अंक_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    जर (unlikely (c->in_error ())) return_trace (असत्य);

    जर (likely (record)) record->last = prev_gid;
    rangeRecord.len = अंक_ranges;

    जर (unlikely (unsorted))
      rangeRecord.as_array ().क्विक (RangeRecord<Types>::cmp_range);

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सत्य,
               तार्किक use_class_zero = सत्य,
               स्थिरांक Coverage* glyph_filter = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    जर (glyph_set.get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len)
	< get_population ())
    {
      साठी (hb_codepoint_t g : glyph_set)
      {
	अचिन्हित klass = get_class (g);
	जर (!klass) सुरूठेवा;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	जर (new_gid == HB_MAP_मूल्य_INVALID) सुरूठेवा;
	जर (glyph_filter && !glyph_filter->has (g)) सुरूठेवा;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    अन्यथा
    {
      अचिन्हित अंक_source_glyphs = c->plan->source->get_अंक_glyphs ();
      साठी (स्वतः &range : rangeRecord)
      {
	अचिन्हित klass = range.value;
	जर (!klass) सुरूठेवा;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, अंक_source_glyphs);
	साठी (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  जर (new_gid == HB_MAP_मूल्य_INVALID) सुरूठेवा;
	  जर (glyph_filter && !glyph_filter->has (g)) सुरूठेवा;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    स्थिरांक hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    अचिन्हित glyph_मोजणी = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_मोजणी <= glyph_and_klass.length;
    जर (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (असत्य);
    return_trace (keep_empty_table || (तार्किक) glyph_and_klass);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (rangeRecord.sanitize (c));
  }

  अचिन्हित cost () स्थिरांक { परत hb_bit_storage ((अचिन्हित) rangeRecord.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  {
    साठी (स्वतः &range : rangeRecord)
      जर (range.value)
	जर (unlikely (!range.collect_coverage (glyphs)))
	  परत असत्य;
    परत सत्य;
  }

  ढाँचा <typename set_t>
  तार्किक collect_class (set_t *glyphs, अचिन्हित पूर्णांक klass) स्थिरांक
  {
    साठी (स्वतः &range : rangeRecord)
    {
      जर (range.value == klass)
	जर (unlikely (!range.collect_coverage (glyphs)))
	  परत असत्य;
    }
    परत सत्य;
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    जर (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len))
    {
      साठी (स्वतः g : *glyphs)
        जर (get_class (g))
	  परत सत्य;
      परत असत्य;
    }

    परत hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (स्थिरांक RangeRecord<Types> &range) { परत range.intersects (*glyphs) && range.value; }));
  }
  तार्किक intersects_class (स्थिरांक hb_set_t *glyphs, अचिन्हित klass) स्थिरांक
  {
    जर (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      जर (!rangeRecord.len)
        परत !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मूल्य_INVALID;
      hb_codepoint_t last = HB_SET_मूल्य_INVALID;
      स्वतः it = hb_iter (rangeRecord);
      साठी (स्वतः &range : it)
      {
        जर (it->first == last + 1)
	{
	  it++;
	  सुरूठेवा;
	}

	जर (!glyphs->next (&g))
	  थांबा;
	जर (g < range.first)
	  परत सत्य;
	g = range.last;
	last = g;
      }
      जर (g != HB_SET_मूल्य_INVALID && glyphs->next (&g))
	परत सत्य;
      /* Fall through. */
    }
    साठी (स्थिरांक स्वतः &range : rangeRecord)
      जर (range.value == klass && range.intersects (*glyphs))
	परत सत्य;
    परत असत्य;
  }

  शून्य intersected_class_glyphs (स्थिरांक hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) स्थिरांक
  {
    जर (klass == 0)
    {
      hb_codepoint_t g = HB_SET_मूल्य_INVALID;
      साठी (स्वतः &range : rangeRecord)
      {
	जर (!glyphs->next (&g))
	  जा done;
	पर्यंत (g < range.first)
	{
	  intersect_glyphs->add (g);
	  जर (!glyphs->next (&g))
	    जा done;
        }
        g = range.last;
      }
      पर्यंत (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      परत;
    }

    अचिन्हित मोजणी = rangeRecord.len;
    जर (मोजणी > glyphs->get_population () * hb_bit_storage (मोजणी))
    {
      साठी (स्वतः g : *glyphs)
      {
        अचिन्हित i;
        जर (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      परत;
    }

    साठी (स्वतः &range : rangeRecord)
    {
      जर (range.value != klass) सुरूठेवा;

      अचिन्हित end = range.last + 1;
      साठी (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  शून्य intersected_classes (स्थिरांक hb_set_t *glyphs, hb_set_t *intersect_classes) स्थिरांक
  {
    जर (glyphs->is_empty ()) परत;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    जर (!rangeRecord.len)
    {
      intersect_classes->add (0);
      परत;
    }

    hb_codepoint_t g = HB_SET_मूल्य_INVALID;
    साठी (स्वतः &range : rangeRecord)
    {
      जर (!glyphs->next (&g))
        थांबा;
      जर (g < range.first)
      {
        intersect_classes->add (0);
        थांबा;
      }
      g = range.last;
    }
    जर (g != HB_SET_मूल्य_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    साठी (स्थिरांक स्वतः& range : rangeRecord)
      जर (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  संरक्षित:
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
  अचिन्हित चालक [] (hb_codepoint_t k) स्थिरांक { परत get (k); }
  तार्किक has (hb_codepoint_t k) स्थिरांक { परत (*हे)[k]; }
  /* Projection. */
  hb_codepoint_t चालक () (hb_codepoint_t k) स्थिरांक { परत get (k); }

  अचिन्हित पूर्णांक get (hb_codepoint_t k) स्थिरांक { परत get_class (k); }
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.get_class (glyph_id);
    स्थिती 2: hb_barrier (); परत u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.get_class (glyph_id);
    स्थिती 4: hb_barrier (); परत u.format4.get_class (glyph_id);
#endif
    पूर्वनिर्धारित:परत 0;
    }
  }
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) स्थिरांक
  {
    अचिन्हित klass;
    जर (cache && cache->get (glyph_id, &klass)) परत klass;
    klass = get_class (glyph_id);
    जर (cache) cache->set (glyph_id, klass);
    परत klass;
  }

  अचिन्हित get_population () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.get_population ();
    स्थिती 2: hb_barrier (); परत u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.get_population ();
    स्थिती 4: hb_barrier (); परत u.format4.get_population ();
#endif
    पूर्वनिर्धारित:परत NOT_COVERED;
    }
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    स्वतः glyphs = + it_with_class_zero | hb_filter (hb_second);

    अचिन्हित format = 2;
    hb_codepoint_t glyph_max = 0;
    अचिन्हित class_max = 0;
    जर (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      अचिन्हित अंक_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      अचिन्हित prev_klass = (*glyphs).second;

      साठी (स्थिरांक स्वतः gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	अचिन्हित cur_klass = gid_klass_pair.second;
	जर (cur_klass > class_max) class_max = cur_klass;
	जर (cur_gid == glyph_min) सुरूठेवा;
        जर (cur_gid > glyph_max) glyph_max = cur_gid;
	जर (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  अंक_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      अचिन्हित glyph_span = glyph_max - glyph_min + 1;
      स्वतः consider = [&] (अचिन्हित candidate, uint64_t size)
      {
	जर (size >= best_size) परत;
	best_size = size;
	format = candidate;
      };

      जर (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      जर (glyph_max <= 0xFFFFu && अंक_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) अंक_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      जर (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      जर (glyph_max <= 0xFFFFFFu && अंक_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) अंक_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      जर (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (असत्य, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (असत्य);
      }
    }

    u.format.v = format;

    निवड (u.format.v)
    {
    स्थिती 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    स्थिती 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    स्थिती 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               तार्किक keep_empty_table = सत्य,
               तार्किक use_class_zero = सत्य,
               स्थिरांक Coverage* glyph_filter = nullptr) स्थिरांक
  {
    TRACE_SUBSET (हे);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    स्थिती 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    स्थिती 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    स्थिती 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    स्थिती 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  अचिन्हित cost () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.cost ();
    स्थिती 2: hb_barrier (); परत u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.cost ();
    स्थिती 4: hb_barrier (); परत u.format4.cost ();
#endif
    पूर्वनिर्धारित:परत 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  तार्किक collect_coverage (set_t *glyphs) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.collect_coverage (glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.collect_coverage (glyphs);
    स्थिती 4: hb_barrier (); परत u.format4.collect_coverage (glyphs);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  तार्किक collect_class (set_t *glyphs, अचिन्हित पूर्णांक klass) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.collect_class (glyphs, klass);
    स्थिती 2: hb_barrier (); परत u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.collect_class (glyphs, klass);
    स्थिती 4: hb_barrier (); परत u.format4.collect_class (glyphs, klass);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.intersects (glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersects (glyphs);
    स्थिती 4: hb_barrier (); परत u.format4.intersects (glyphs);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }
  तार्किक intersects_class (स्थिरांक hb_set_t *glyphs, अचिन्हित पूर्णांक klass) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.intersects_class (glyphs, klass);
    स्थिती 2: hb_barrier (); परत u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersects_class (glyphs, klass);
    स्थिती 4: hb_barrier (); परत u.format4.intersects_class (glyphs, klass);
#endif
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  शून्य intersected_class_glyphs (स्थिरांक hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    स्थिती 2: hb_barrier (); परत u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    स्थिती 4: hb_barrier (); परत u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    पूर्वनिर्धारित:परत;
    }
  }

  शून्य intersected_classes (स्थिरांक hb_set_t *glyphs, hb_set_t *intersect_classes) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); परत u.format1.intersected_classes (glyphs, intersect_classes);
    स्थिती 2: hb_barrier (); परत u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    स्थिती 3: hb_barrier (); परत u.format3.intersected_classes (glyphs, intersect_classes);
    स्थिती 4: hb_barrier (); परत u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    पूर्वनिर्धारित:परत;
    }
  }


  संरक्षित:
  संघ {
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
स्थिर ओळीत तार्किक ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ परत (c->start_embed<ClassDef> ()->serialize (c, it)); }


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
    पूर्णांक cmp (स्थिरांक chars_t& other) स्थिरांक
    {
      परत as_array ().cmp (other.as_array ());
    }

    hb_pair_t<अचिन्हित, अचिन्हित> get_width ()
    {
      अचिन्हित width = 0;
      अचिन्हित columns = 0;
      साठी (अचिन्हित i = 0; i < length; i++)
      {
	अचिन्हित v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      परत hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<अचिन्हित, अचिन्हित> combine_width (स्थिरांक chars_t& other) स्थिरांक
    {
      अचिन्हित combined_width = 0;
      अचिन्हित combined_columns = 0;
      साठी (अचिन्हित i = 0; i < length; i++)
      {
        अचिन्हित v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      परत hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<अचिन्हित, अचिन्हित> combine_width (स्थिरांक delta_row_encoding_t& other_encoding) स्थिरांक { परत chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  अचिन्हित width = 0;
  अचिन्हित overhead = 0;
  hb_vector_t<स्थिरांक hb_vector_t<पूर्णांक>*> वस्तुसंच;

  delta_row_encoding_t () = पूर्वनिर्धारित;
  delta_row_encoding_t (hb_vector_t<स्थिरांक hb_vector_t<पूर्णांक>*> &&rows, अचिन्हित अंक_cols)
  {
    प्रतिज्ञा (rows);

    वस्तुसंच = std::move (rows);

    जर (unlikely (!chars.resize (अंक_cols)))
      परत;

    calculate_chars ();
  }

  शून्य merge (स्थिरांक delta_row_encoding_t& other)
  {
    वस्तुसंच.alloc (वस्तुसंच.length + other.वस्तुसंच.length);
    साठी (स्वतः &row : other.वस्तुसंच)
      add_row (row);

    // Merge chars
    प्रतिज्ञा (chars.length == other.chars.length);
    साठी (अचिन्हित i = 0; i < chars.length; i++)
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
    प्रतिज्ञा (वस्तुसंच);

    तार्किक long_शब्द = असत्य;

    साठी (स्वतः &row : वस्तुसंच)
    {
      प्रतिज्ञा (row->length == chars.length);

      /* 0/1/2 byte encoding */
      साठी (अचिन्हित i = 0; i < row->length; i++)
      {
	पूर्णांक v =  row->arrayZ[i];
	जर (v == 0)
	  सुरूठेवा;
	अन्यथा जर (v > 32767 || v < -32768)
	{
	  long_शब्द = सत्य;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	अन्यथा जर (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	अन्यथा
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    जर (long_शब्द)
    {
      // Convert 1s to 2s
      साठी (स्वतः &v : chars)
	जर (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  तार्किक is_empty () स्थिरांक
  { परत !वस्तुसंच; }

  तार्किक in_error () स्थिरांक
  { परत chars.in_error () || वस्तुसंच.in_error (); }

  स्थिर ओळीत अचिन्हित get_chars_overhead (अचिन्हित अंक_columns)
  {
    अचिन्हित c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    परत c + अंक_columns * 2;
  }

  अचिन्हित get_gain (अचिन्हित additional_bytes_per_rows = 1) स्थिरांक
  {
    पूर्णांक मोजणी = वस्तुसंच.length;
    परत hb_max (0, (पूर्णांक) overhead - मोजणी * (पूर्णांक) additional_bytes_per_rows);
  }

  पूर्णांक gain_from_merging (स्थिरांक delta_row_encoding_t& other_encoding) स्थिरांक
  {
    // Back of the envelope calculations to reject early.
    चिन्हित additional_bytes_per_rows = other_encoding.width - width;
    जर (additional_bytes_per_rows > 0)
    {
      जर (get_gain (additional_bytes_per_rows) == 0)
        परत 0;
    }
    अन्यथा
    {
      जर (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	परत 0;
    }

    स्वतः pair = combine_width (other_encoding);
    अचिन्हित combined_width = pair.first;
    अचिन्हित combined_columns = pair.second;

    पूर्णांक combined_gain = (पूर्णांक) overhead + (पूर्णांक) other_encoding.overhead;
    combined_gain -= (combined_width - (पूर्णांक) width) * वस्तुसंच.length;
    combined_gain -= (combined_width - (पूर्णांक) other_encoding.width) * other_encoding.वस्तुसंच.length;
    combined_gain -= get_chars_overhead (combined_columns);

    परत combined_gain;
  }

  तार्किक add_row (स्थिरांक hb_vector_t<पूर्णांक>* row)
  { परत वस्तुसंच.push (row); }

  स्थिर पूर्णांक cmp (स्थिरांक शून्य *pa, स्थिरांक शून्य *pb)
  {
    स्थिरांक delta_row_encoding_t *a = (स्थिरांक delta_row_encoding_t *)pa;
    स्थिरांक delta_row_encoding_t *b = (स्थिरांक delta_row_encoding_t *)pb;

    जर (a->width != b->width)
      परत (पूर्णांक) a->width - (पूर्णांक) b->width;

    परत b->chars.cmp (a->chars);
  }
};

संरचना VarRegionAxis
{
  दशांश evaluate (पूर्णांक coord) स्थिरांक
  {
    पूर्णांक peak = peakCoord.to_int ();
    जर (peak == 0 || coord == peak)
      परत 1.f;
    अन्यथा जर (coord == 0) // Faster
      परत 0.f;

    पूर्णांक start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    जर (unlikely (start > peak || peak > end))
      परत 1.f;
    जर (unlikely (start < 0 && end > 0))
      परत 1.f;

    जर (coord <= start || end <= coord)
      परत 0.f;

    /* Interpolate */
    जर (coord < peak)
      परत दशांश (coord - start) / (peak - start);
    अन्यथा
      परत दशांश (end - coord) / (end - peak);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  तार्किक serialize (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (हे));
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
  दशांश evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len) स्थिरांक
  {
    अचिन्हित i = axisIndex;
    पूर्णांक coord = i < coord_len ? coords[i] : 0;
    परत axis.evaluate (coord);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक SparseVarRegionAxis *src,
		  स्थिरांक hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->embed (src);
    जर (unlikely (!out)) return_trace (असत्य);
    जर (axis_map)
    {
      जर (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (असत्य);
      }
      जर (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
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
  खाजगी:
  स्थिर constexpr अचिन्हित STATIC_LENGTH = 128;
  स्थिर constexpr दशांश INVALID = न_अंक;

  सार्वजनिक:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (स्थिरांक hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& चालक= (स्थिरांक hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& चालक= (hb_scalar_cache_t&&) = delete;

  स्थिर hb_scalar_cache_t *create (अचिन्हित पूर्णांक मोजणी,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    जर (!मोजणी) परत (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    जर (scratch_cache && मोजणी <= STATIC_LENGTH)
    {
      scratch_cache->length = मोजणी;
      scratch_cache->clear ();
      परत scratch_cache;
    }

    स्वतः *cache = (hb_scalar_cache_t *) hb_malloc (आकार (hb_scalar_cache_t) - आकार (static_मूल्ये) + आकार (static_मूल्ये[0]) * मोजणी);
    जर (unlikely (!cache)) परत (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = मोजणी;
    cache->clear ();

    परत cache;
  }

  स्थिर शून्य destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    जर (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  शून्य clear ()
  {
    स्वतः *values = &static_मूल्ये[0];
    अचिन्हित i = 0;
#ifndef HB_OPTIMIZE_SIZE
    साठी (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    साठी (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  तार्किक get (अचिन्हित i, दशांश *value) स्थिरांक
  {
    जर (unlikely (i >= length))
    {
      *value = 0.f;
      परत सत्य;
    }
    स्वतः *values = &static_मूल्ये[0];
    स्वतः *cached_मूल्य = &values[i];
    दशांश v = *cached_मूल्य;
    जर (std::isnan (v))
      परत असत्य;
    *value = v;
    परत सत्य;
  }

  HB_ALWAYS_INLINE
  शून्य set (अचिन्हित i, दशांश value)
  {
    जर (unlikely (i >= length)) परत;
    स्वतः *values = &static_मूल्ये[0];
    स्वतः *cached_मूल्य = &values[i];
    *cached_मूल्य = value;
  }

  सार्वजनिक:
  अचिन्हित length;
  खाजगी:
  mutable hb_atomic_t<दशांश> static_मूल्ये[STATIC_LENGTH];
};

संरचना VarRegionList
{
  खाजगी:
  दशांश evaluate_impl (अचिन्हित पूर्णांक region_index,
		       स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len) स्थिरांक
  {
    स्थिरांक VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_मोजणी_2);
    दशांश v = 1.f;

    अचिन्हित पूर्णांक मोजणी = axis_मोजणी_2;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      पूर्णांक coord = i < coord_len ? coords[i] : 0;
      दशांश factor = axes[i].evaluate (coord);
      जर (factor == 0.f)
      {
	v = 0.f;
	थांबा;
      }
      v *= factor;
    }

    परत v;
  }

  सार्वजनिक:
  HB_ALWAYS_INLINE
  दशांश evaluate (अचिन्हित पूर्णांक region_index,
		  स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		  hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    जर (unlikely (region_index >= region_मोजणी))
      परत 0.;

    दशांश v;
    जर (cache && cache->get (region_index, &v))
      परत v;

    v = evaluate_impl (region_index, coords, coord_len);

    जर (cache)
      cache->set (region_index, v);
    परत v;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_मोजणी_2 * region_मोजणी));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  स्थिरांक hb_vector_t<hb_tag_t>& axis_tags,
                  स्थिरांक hb_vector_t<स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित axis_मोजणी = axis_tags.length;
    अचिन्हित region_मोजणी_2 = regions.length;
    जर (!axis_मोजणी || !region_मोजणी_2) return_trace (असत्य);
    जर (unlikely (hb_unsigned_mul_overflows (axis_मोजणी * region_मोजणी_2,
                                             VarRegionAxis::static_size))) return_trace (असत्य);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    axis_मोजणी_2 = axis_मोजणी;
    region_मोजणी = region_मोजणी_2;

    साठी (अचिन्हित r = 0; r < region_मोजणी_2; r++)
    {
      स्थिरांक स्वतः& region = regions[r];
      साठी (अचिन्हित i = 0; i < axis_मोजणी; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        जर (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->मधला);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        अन्यथा
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        जर (!var_region_rec.serialize (c))
          return_trace (असत्य);
      }
    }
    return_trace (सत्य);
  }

  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक VarRegionList *src, स्थिरांक hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    axis_मोजणी_2 = src->axis_मोजणी_2;
    region_मोजणी = region_map.get_population ();
    जर (unlikely (hb_unsigned_mul_overflows (axis_मोजणी_2 * region_मोजणी,
					     VarRegionAxis::static_size))) return_trace (असत्य);
    जर (unlikely (!c->extend (हे))) return_trace (असत्य);
    अचिन्हित पूर्णांक region_मोजणी_2 = src->region_मोजणी;
    साठी (अचिन्हित पूर्णांक r = 0; r < region_मोजणी; r++)
    {
      अचिन्हित पूर्णांक backward = region_map.backward (r);
      जर (backward >= region_मोजणी_2) return_trace (असत्य);
      hb_memcpy (&axesZ[axis_मोजणी_2 * r], &src->axesZ[axis_मोजणी_2 * backward], VarRegionAxis::static_size * axis_मोजणी_2);
    }

    return_trace (सत्य);
  }

  तार्किक get_var_region (अचिन्हित region_index,
                       स्थिरांक hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) स्थिरांक
  {
    जर (region_index >= region_मोजणी) परत असत्य;
    स्थिरांक VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_मोजणी_2);
    साठी (अचिन्हित i = 0; i < axis_मोजणी_2; i++)
    {
      hb_tag_t *axis_tag;
      जर (!axes_old_index_tag_map.has (i, &axis_tag))
        परत असत्य;

      दशांश min_val = axis_region->startCoord.to_float ();
      दशांश def_val = axis_region->peakCoord.to_float ();
      दशांश max_val = axis_region->endCoord.to_float ();

      जर (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((द्विगुण) min_val, (द्विगुण) def_val, (द्विगुण) max_val));
      axis_region++;
    }
    परत !axis_tuples.in_error ();
  }

  तार्किक get_var_regions (स्थिरांक hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) स्थिरांक
  {
    जर (!regions.alloc (region_मोजणी))
      परत असत्य;

    साठी (अचिन्हित i = 0; i < region_मोजणी; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      जर (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        परत असत्य;
      regions.push (std::move (axis_tuples));
    }
    परत !regions.in_error ();
  }

  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_मोजणी_2, region_मोजणी)); }

  सार्वजनिक:
  HBUINT16	axis_मोजणी_2;
  HBUINT15	region_मोजणी;
  संरक्षित:
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
स्थिर ओळीत तार्किक
_hb_avar2_region_is_dead (स्थिरांक hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  स्थिरांक hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  साठी (स्वतः _ : axis_tuples)
  {
    Triple *range;
    जर (!reachable_ranges.has (_.first, &range)) सुरूठेवा;
    स्थिरांक Triple &tent = _.second;
    पूर्णांक start = (पूर्णांक) roundf ((दशांश) tent.minimum * 16384.f);
    पूर्णांक peak  = (पूर्णांक) roundf ((दशांश) tent.मधला * 16384.f);
    पूर्णांक end   = (पूर्णांक) roundf ((दशांश) tent.maximum * 16384.f);
    जर (!peak) सुरूठेवा;
    जर (start > peak || peak > end) सुरूठेवा;
    जर (start < 0 && end > 0) सुरूठेवा;
    पूर्णांक lo = (पूर्णांक) roundf ((दशांश) range->minimum * 16384.f);
    पूर्णांक hi = (पूर्णांक) roundf ((दशांश) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    जर (hi < start || lo > end)
      परत सत्य;
  }
  परत असत्य;
}

संरचना SparseVariationRegion
{
  दशांश evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len) स्थिरांक
  {
    दशांश v = 1.f;
    अचिन्हित पूर्णांक मोजणी = axes.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      दशांश factor = (हे+axes[i]).evaluate (coords, coord_len);
      जर (factor == 0.f)
	परत 0.;
      v *= factor;
    }
    परत v;
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक SparseVariationRegion *src,
		  स्थिरांक hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!axes.serialize (c, (अचिन्हित) src->axes.len))) return_trace (असत्य);
    साठी (अचिन्हित i = 0; i < axes.len; i++)
      जर (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (axes.sanitize (c, हे));
  }

  संरक्षित:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, axes);
};

संरचना SparseVarRegionList
{
  HB_ALWAYS_INLINE
  दशांश evaluate (अचिन्हित पूर्णांक region_index,
		  स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		  hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    जर (unlikely (region_index >= regions.len))
      परत 0.;

    दशांश v;
    जर (cache && cache->get (region_index, &v))
      परत v;

    स्थिरांक SparseVariationRegion &region = हे+regions[region_index];

    v = region.evaluate (coords, coord_len);
    जर (cache)
      cache->set (region_index, v);

    परत v;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (regions.sanitize (c, हे));
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक SparseVarRegionList *src,
		  स्थिरांक hb_inc_bimap_t &region_map,
		  स्थिरांक hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    regions.len = region_map.get_population ();
    जर (unlikely (!c->extend (regions))) return_trace (असत्य);

    साठी (अचिन्हित i = 0; i < regions.len; i++)
    {
      अचिन्हित old_index = region_map.backward (i);
      जर (unlikely (old_index >= src->regions.len ||
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
  अचिन्हित पूर्णांक get_item_मोजणी () स्थिरांक
  { परत item_मोजणी; }

  अचिन्हित पूर्णांक get_region_index_मोजणी () स्थिरांक
  { परत regionIndices.len; }

  अचिन्हित get_region_index (अचिन्हित i) स्थिरांक
  { परत i >= regionIndices.len ? -1 : regionIndices[i]; }

  माप_प्रकार get_row_size () स्थिरांक
  { परत (word_मोजणी () + regionIndices.len) * (long_शब्द_2 () ? 2 : 1); }

  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_मोजणी, get_row_size ())); }

  ढाँचा <typename Float = दशांश>
  Float _get_delta (अचिन्हित पूर्णांक inner,
		    स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		    स्थिरांक VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    जर (unlikely (inner >= item_मोजणी))
      परत 0.;
    तार्किक is_long = long_शब्द_2 ();
    अचिन्हित पूर्णांक मोजणी = regionIndices.len;
    अचिन्हित word_मोजणी_2 = word_मोजणी ();
    अचिन्हित पूर्णांक scount = is_long ? मोजणी : word_मोजणी_2;
    अचिन्हित पूर्णांक lcount = is_long ? word_मोजणी_2 : 0;

    स्थिरांक HBUINT8 *bytes = get_delta_bytes ();
    स्थिरांक HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    अचिन्हित पूर्णांक i = 0;

    स्थिरांक HBINT32 *lcursor = reinterpret_cast<स्थिरांक HBINT32 *> (row);
    साठी (; i < lcount; i++)
    {
      दशांश scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_मोजणी, cache);
      जर (scalar)
        delta += (Float) scalar * (पूर्णांक) *lcursor;
      lcursor++;
    }
    स्थिरांक HBINT16 *scursor = reinterpret_cast<स्थिरांक HBINT16 *> (lcursor);
    साठी (; i < scount; i++)
    {
      दशांश scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_मोजणी, cache);
      जर (scalar)
       delta += (Float) scalar * (पूर्णांक) *scursor;
      scursor++;
    }
    स्थिरांक HBINT8 *bcursor = reinterpret_cast<स्थिरांक HBINT8 *> (scursor);
    साठी (; i < मोजणी; i++)
    {
      दशांश scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_मोजणी, cache);
      जर (scalar)
        delta += (Float) scalar * (पूर्णांक) *bcursor;
      bcursor++;
    }

    परत delta;
  }

  ढाँचा <typename Float = दशांश>
  HB_ALWAYS_INLINE
  Float get_delta (अचिन्हित पूर्णांक inner,
		   स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		   स्थिरांक VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = regionIndices.len;
    जर (!मोजणी) परत Float (0); // This is quite common, so optimize it.
    परत _get_delta<Float> (inner, coords, coord_मोजणी, regions, cache);
  }

  शून्य get_region_scalars (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
			   स्थिरांक VarRegionList &regions,
			   दशांश *scalars /*OUT */,
			   अचिन्हित पूर्णांक अंक_scalars) स्थिरांक
  {
    अचिन्हित मोजणी = hb_min (अंक_scalars, regionIndices.len);
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_मोजणी);
    साठी (अचिन्हित पूर्णांक i = मोजणी; i < अंक_scalars; i++)
      scalars[i] = 0.f;
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_मोजणी () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_मोजणी,
				  get_row_size ()));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक has_long,
                  स्थिरांक hb_vector_t<स्थिरांक hb_vector_t<पूर्णांक>*>& rows)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित row_मोजणी_2 = rows.length;
    जर (!row_मोजणी_2) {
      // Nothing to serialize, will be empty.
      परत असत्य;
    }

    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    जर (unlikely (!c->check_assign (item_मोजणी, row_मोजणी_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);

    पूर्णांक min_threshold = has_long ? -32768 : -128;
    पूर्णांक max_threshold = has_long ? +32767 : +127;
    गणना delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    अचिन्हित अंक_regions = rows[0]->length;
    जर (!delta_sz.resize (अंक_regions))
      return_trace (असत्य);

    अचिन्हित word_मोजणी_2 = 0;
    साठी (अचिन्हित r = 0; r < अंक_regions; r++)
    {
      साठी (अचिन्हित i = 0; i < row_मोजणी_2; i++)
      {
        पूर्णांक delta = rows[i]->arrayZ[r];
        जर (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_मोजणी_2++;
          थांबा;
        }
        अन्यथा जर (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    अचिन्हित word_index = 0;
    अचिन्हित non_word_index = word_मोजणी_2;
    hb_map_t ri_map;
    साठी (अचिन्हित r = 0; r < अंक_regions; r++)
    {
      जर (!delta_sz[r]) सुरूठेवा;
      अचिन्हित new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      जर (!ri_map.set (new_r, r))
        return_trace (असत्य);
    }

    word_Size_मोजणी = word_मोजणी_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    अचिन्हित ri_मोजणी = ri_map.get_population ();
    regionIndices.len = ri_मोजणी;
    जर (unlikely (!c->extend (हे))) return_trace (असत्य);

    साठी (अचिन्हित r = 0; r < ri_मोजणी; r++)
    {
      hb_codepoint_t *idx;
      जर (!ri_map.has (r, &idx))
        return_trace (असत्य);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_size = get_row_size ();
    साठी (अचिन्हित पूर्णांक i = 0; i < row_मोजणी_2; i++)
    {
      साठी (अचिन्हित पूर्णांक r = 0; r < ri_मोजणी; r++)
      {
        पूर्णांक delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (सत्य);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक VarData *src,
		  स्थिरांक hb_inc_bimap_t &inner_map,
		  स्थिरांक hb_inc_bimap_t &region_map,
		  स्थिरांक hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    item_मोजणी = inner_map.get_next_मूल्य ();

    /* Optimize word count */
    अचिन्हित ri_मोजणी = src->regionIndices.len;
    गणना delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<अचिन्हित पूर्णांक> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_मोजणी);
    ri_map.resize (ri_मोजणी);
    अचिन्हित पूर्णांक new_word_मोजणी = 0;
    अचिन्हित पूर्णांक r;

    स्थिरांक HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    अचिन्हित src_row_size = src->get_row_size ();
    अचिन्हित src_word_मोजणी = src->word_मोजणी ();
    तार्किक     src_long_शब्द = src->long_शब्द_2 ();

    तार्किक has_long = असत्य;
    जर (src_long_शब्द)
    {
      साठी (r = 0; r < src_word_मोजणी; r++)
      {
        साठी (अचिन्हित old_gid : inner_map.किल्ल्या())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  जर (delta < -32768 || 32767 < delta)
	  {
	    has_long = सत्य;
	    थांबा;
	  }
        }
      }
    }

    चिन्हित min_threshold = has_long ? -32768 : -128;
    चिन्हित max_threshold = has_long ? +32767 : +127;
    साठी (r = 0; r < ri_मोजणी; r++)
    {
      तार्किक short_circuit = src_long_शब्द == has_long && src_word_मोजणी <= r;

      delta_sz[r] = kZero;
      जर (culled_regions && culled_regions->has (src->regionIndices[r]))
	सुरूठेवा; /* unreachable region: drop its column */
      साठी (अचिन्हित old_gid : inner_map.किल्ल्या())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	जर (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_मोजणी++;
	  थांबा;
	}
	अन्यथा जर (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  जर (short_circuit)
	    थांबा;
	}
      }
    }

    अचिन्हित पूर्णांक word_index = 0;
    अचिन्हित पूर्णांक non_word_index = new_word_मोजणी;
    अचिन्हित पूर्णांक new_ri_मोजणी = 0;
    साठी (r = 0; r < ri_मोजणी; r++)
      जर (delta_sz[r])
      {
	अचिन्हित new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_मोजणी++;
      }

    word_Size_मोजणी = new_word_मोजणी | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_मोजणी;

    जर (unlikely (!c->extend (हे))) return_trace (असत्य);

    साठी (r = 0; r < new_ri_मोजणी; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_size = get_row_size ();
    अचिन्हित मोजणी = item_मोजणी;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      अचिन्हित पूर्णांक old = inner_map.backward (i);
      साठी (अचिन्हित पूर्णांक r = 0; r < new_ri_मोजणी; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (सत्य);
  }

  शून्य collect_region_refs (hb_set_t &region_indices, स्थिरांक hb_inc_bimap_t &inner_map) स्थिरांक
  {
    स्थिरांक HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_size = get_row_size ();

    साठी (अचिन्हित पूर्णांक r = 0; r < regionIndices.len; r++)
    {
      अचिन्हित पूर्णांक region = regionIndices.arrayZ[r];
      जर (region_indices.has (region)) सुरूठेवा;
      साठी (hb_codepoint_t old_gid : inner_map.किल्ल्या())
	जर (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  थांबा;
	}
    }
  }

  सार्वजनिक:
  स्थिरांक HBUINT8 *get_delta_bytes () स्थिरांक
  { परत &StructAfter<HBUINT8> (regionIndices); }

  संरक्षित:
  HBUINT8 *get_delta_bytes ()
  { परत &StructAfter<HBUINT8> (regionIndices); }

  सार्वजनिक:
  int32_t get_item_delta_fast (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region,
			       स्थिरांक HBUINT8 *delta_bytes, अचिन्हित row_size) स्थिरांक
  {
    जर (unlikely (item >= item_मोजणी || region >= regionIndices.len)) परत 0;

    स्थिरांक HBINT8 *p = (स्थिरांक HBINT8 *) delta_bytes + item * row_size;
    अचिन्हित word_मोजणी_2 = word_मोजणी ();
    तार्किक is_long = long_शब्द_2 ();
    जर (is_long)
    {
      जर (region < word_मोजणी_2)
	परत ((स्थिरांक HBINT32 *) p)[region];
      अन्यथा
	परत ((स्थिरांक HBINT16 *)(p + HBINT32::static_size * word_मोजणी_2))[region - word_मोजणी_2];
    }
    अन्यथा
    {
      जर (region < word_मोजणी_2)
	परत ((स्थिरांक HBINT16 *) p)[region];
      अन्यथा
	परत (p + HBINT16::static_size * word_मोजणी_2)[region - word_मोजणी_2];
    }
  }
  int32_t get_item_delta (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region) स्थिरांक
  {
     परत get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  संरक्षित:
  शून्य set_item_delta_fast (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region, int32_t delta,
			    HBUINT8 *delta_bytes, अचिन्हित row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    अचिन्हित word_मोजणी_2 = word_मोजणी ();
    तार्किक is_long = long_शब्द_2 ();
    जर (is_long)
    {
      जर (region < word_मोजणी_2)
	((HBINT32 *) p)[region] = delta;
      अन्यथा
	((HBINT16 *)(p + HBINT32::static_size * word_मोजणी_2))[region - word_मोजणी_2] = delta;
    }
    अन्यथा
    {
      जर (region < word_मोजणी_2)
	((HBINT16 *) p)[region] = delta;
      अन्यथा
	(p + HBINT16::static_size * word_मोजणी_2)[region - word_मोजणी_2] = delta;
    }
  }
  शून्य set_item_delta (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  तार्किक long_शब्द_2 () स्थिरांक { परत word_Size_मोजणी & 0x8000u /* LONG_WORDS */; }
  अचिन्हित word_मोजणी () स्थिरांक { परत word_Size_मोजणी & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  संरक्षित:
  HBUINT16		item_मोजणी;
  HBUINT16		word_Size_मोजणी;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

संरचना MultiVarData
{
  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  स्थिरांक TupleList &get_delta_sets () स्थिरांक
  { परत हे+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  शून्य get_delta (अचिन्हित पूर्णांक inner,
		  स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		  स्थिरांक SparseVarRegionList &regions,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    स्वतः &deltaSets = get_delta_sets ();

    स्वतः मूल्ये_iter = deltaSets.fetcher (inner);
    अचिन्हित region_मोजणी = regionIndices.len;
    अचिन्हित skip = 0;
    साठी (अचिन्हित regionIndex = 0; regionIndex < region_मोजणी; regionIndex++)
    {
      दशांश scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_मोजणी,
				       cache);
      // We skip lazily. Helps with the tail end.
      जर (scalar == 0.0f)
        skip += out.length;
      अन्यथा
      {
        जर (skip)
	{
	  मूल्ये_iter.skip (skip);
	  skip = 0;
	}
	मूल्ये_iter.add_to (out, scalar);
      }
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, हे));
  }

  अचिन्हित get_item_मोजणी () स्थिरांक
  { परत get_delta_sets ().मोजणी; }

  शून्य collect_region_refs (hb_set_t &region_indices) स्थिरांक
  {
    साठी (अचिन्हित region : regionIndices)
      region_indices.add (region);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक MultiVarData *src,
		  स्थिरांक hb_inc_bimap_t &inner_map,
		  स्थिरांक hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित region_मोजणी_2 = src->regionIndices.len;
    अचिन्हित header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_मोजणी_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    जर (unlikely (!c->extend_size (हे, header_size, असत्य)))
      return_trace (असत्य);
    format = 1;
    regionIndices.len = region_मोजणी_2;
    साठी (अचिन्हित i = 0; i < regionIndices.len; i++)
    {
      अचिन्हित old_region = src->regionIndices[i];
      जर (unlikely (!region_map.has (old_region)))
	return_trace (असत्य);
      regionIndices[i] = region_map.get (old_region);
    }

    स्थिरांक स्वतः &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    अचिन्हित data_size = 0;
    जर (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (असत्य);
    साठी (अचिन्हित i = 0; i < inner_map.get_population (); i++)
    {
      अचिन्हित old_inner = inner_map.backward (i);
      जर (unlikely (old_inner >= src_delta_sets.मोजणी)) return_trace (असत्य);
      hb_ubytes_t bytes = static_cast<स्थिरांक CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    जर (unlikely (data_size == अच_पूर्णांक_उच्च || delta_sets.in_error ()))
      return_trace (असत्य);

    स्वतः &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  संरक्षित:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  सार्वजनिक:
  DEFINE_SIZE_MIN (7);
};

संरचना ItemVariationStore
{
  मित्र संरचना item_variations_t;

  hb_scalar_cache_t *create_cache () स्थिरांक
  {
#ifdef HB_NO_VAR
    परत hb_scalar_cache_t::create (0);
#endif
    परत hb_scalar_cache_t::create ((हे+regions).region_मोजणी);
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  खाजगी:
  ढाँचा <typename Float = दशांश>
  Float get_delta (अचिन्हित पूर्णांक outer, अचिन्हित पूर्णांक inner,
		   स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		   hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत Float (0);
#endif

    जर (unlikely (outer >= dataSets.len))
      परत Float (0);

    परत (हे+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_मोजणी,
					     हे+regions,
					     cache);
  }

  सार्वजनिक:
  तार्किक has_delta_set (अचिन्हित पूर्णांक index) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत असत्य;
#endif

    अचिन्हित पूर्णांक outer = index >> 16;
    अचिन्हित पूर्णांक inner = index & 0xFFFF;
    परत outer < dataSets.len &&
	   inner < (हे+dataSets[outer]).get_item_मोजणी ();
  }

  ढाँचा <typename Float = दशांश>
  Float get_delta (अचिन्हित पूर्णांक index,
		   स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		   hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    अचिन्हित पूर्णांक outer = index >> 16;
    अचिन्हित पूर्णांक inner = index & 0xFFFF;
    परत get_delta<Float> (outer, inner, coords, coord_मोजणी, cache);
  }
  ढाँचा <typename Float = दशांश>
  Float get_delta (अचिन्हित पूर्णांक index,
		   hb_array_t<स्थिरांक पूर्णांक> coords,
		   hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    परत get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत सत्य;
#endif

    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, हे) &&
		  dataSets.sanitize (c, हे));
  }

  तार्किक serialize (hb_serialize_context_t *c,
                  तार्किक has_long,
                  स्थिरांक hb_vector_t<hb_tag_t>& axis_tags,
                  स्थिरांक hb_vector_t<स्थिरांक hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  स्थिरांक hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (हे);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    format = 1;
    जर (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (असत्य);

    अचिन्हित अंक_var_data = vardata_encodings.length;
    जर (!अंक_var_data) return_trace (असत्य);
    जर (unlikely (!c->check_assign (dataSets.len, अंक_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);

    जर (unlikely (!c->extend (dataSets))) return_trace (असत्य);
    साठी (अचिन्हित i = 0; i < अंक_var_data; i++)
      जर (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].वस्तुसंच))
        return_trace (असत्य);

    return_trace (सत्य);
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक ItemVariationStore *src,
		  स्थिरांक hb_array_t <स्थिरांक hb_inc_bimap_t> &inner_maps,
		  स्थिरांक hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (हे);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif

    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    अचिन्हित पूर्णांक set_मोजणी = 0;
    साठी (अचिन्हित पूर्णांक i = 0; i < inner_maps.length; i++)
      जर (inner_maps[i].get_population ())
	set_मोजणी++;

    format = 1;

    स्थिरांक स्वतः &src_regions = src+src->regions;

    hb_set_t region_indices;
    साठी (अचिन्हित पूर्णांक i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    जर (region_indices.in_error ())
      return_trace (असत्य);

    region_indices.del_range ((src_regions).region_मोजणी, hb_set_t::INVALID);

    जर (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (अचिन्हित _) { region_map.add(_); })
    ;
    जर (region_map.in_error())
      return_trace (असत्य);

    जर (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (असत्य);

    dataSets.len = set_मोजणी;
    जर (unlikely (!c->extend (dataSets))) return_trace (असत्य);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    अचिन्हित पूर्णांक set_index = 0;
    साठी (अचिन्हित पूर्णांक i = 0; i < inner_maps.length; i++)
    {
      जर (!inner_maps[i].get_population ()) सुरूठेवा;
      जर (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->start_embed (हे);
    जर (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    अचिन्हित मोजणी = dataSets.len;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      जर (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      स्वतः &data = हे+dataSets[i];

      अचिन्हित item_मोजणी = data.get_item_मोजणी ();
      साठी (अचिन्हित j = 0; j < item_मोजणी; j++)
	map->add (j);
    }

    जर (unlikely (!out->serialize (c, हे, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  तार्किक collect_dead_regions (स्थिरांक hb_map_t &axes_old_index_tag_map,
			     स्थिरांक hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) स्थिरांक
  {
#ifndef HB_NO_VAR
    जर (!reachable_ranges.get_population ()) परत सत्य;
    स्थिरांक VarRegionList &region_list = हे+regions;
    अचिन्हित मोजणी = region_list.region_मोजणी;
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      जर (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	परत असत्य;
      जर (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    परत !dead_regions.in_error ();
#अन्यथा
    परत सत्य;
#endif
  }

  तार्किक subset (hb_subset_context_t *c, स्थिरांक hb_array_t<स्थिरांक hb_inc_bimap_t> &inner_maps) स्थिरांक
  {
    TRACE_SUBSET (हे);
#ifdef HB_NO_VAR
    return_trace (असत्य);
#endif

    ItemVariationStore *varstore_मूळसंख्या = c->serializer->start_embed<ItemVariationStore> ();
    जर (unlikely (!varstore_मूळसंख्या)) return_trace (असत्य);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    जर (c->plan->has_avar2)
      जर (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (असत्य);

    varstore_मूळसंख्या->serialize (c->serializer, हे, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_मूळसंख्या->dataSets);
  }

  अचिन्हित पूर्णांक get_region_index_मोजणी (अचिन्हित पूर्णांक major) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत 0;
#endif
    परत (हे+dataSets[major]).get_region_index_मोजणी ();
  }

  शून्य get_region_scalars (अचिन्हित पूर्णांक major,
			   स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
			   दशांश *scalars /*OUT*/,
			   अचिन्हित पूर्णांक अंक_scalars) स्थिरांक
  {
#ifdef HB_NO_VAR
    साठी (अचिन्हित i = 0; i < अंक_scalars; i++)
      scalars[i] = 0.f;
    परत;
#endif

    (हे+dataSets[major]).get_region_scalars (coords, coord_मोजणी,
					       हे+regions,
					       &scalars[0], अंक_scalars);
  }

  अचिन्हित पूर्णांक get_sub_table_मोजणी () स्थिरांक
   {
#ifdef HB_NO_VAR
     परत 0;
#endif
     परत dataSets.len;
   }

  स्थिरांक VarData& get_sub_table (अचिन्हित i) स्थिरांक
  {
#ifdef HB_NO_VAR
     परत Null (VarData);
#endif
     परत हे+dataSets[i];
  }

  स्थिरांक VarRegionList& get_region_list () स्थिरांक
  {
#ifdef HB_NO_VAR
     परत Null (VarRegionList);
#endif
     परत हे+regions;
  }

  संरक्षित:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

संरचना MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत hb_scalar_cache_t::create (0);
#endif
    स्वतः &r = हे+regions;
    अचिन्हित मोजणी = r.regions.len;

    परत hb_scalar_cache_t::create (मोजणी, static_cache);
  }

  स्थिर शून्य destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  खाजगी:
  शून्य get_delta (अचिन्हित पूर्णांक outer, अचिन्हित पूर्णांक inner,
		  स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत;
#endif

    जर (unlikely (outer >= dataSets.len))
      परत;

    परत (हे+dataSets[outer]).get_delta (inner,
					     coords, coord_मोजणी,
					     हे+regions,
					     out,
					     cache);
  }

  सार्वजनिक:
  शून्य get_delta (अचिन्हित पूर्णांक index,
		  स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_मोजणी,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    अचिन्हित पूर्णांक outer = index >> 16;
    अचिन्हित पूर्णांक inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_मोजणी, out, cache);
  }
  शून्य get_delta (अचिन्हित पूर्णांक index,
		  hb_array_t<स्थिरांक पूर्णांक> coords,
		  hb_array_t<द्विगुण> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिरांक
  {
    परत get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
#ifdef HB_NO_VAR
    परत सत्य;
#endif

    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, हे) &&
		  dataSets.sanitize (c, हे));
  }

  तार्किक create_subset_plan (स्थिरांक hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) स्थिरांक
  {
    जर (unlikely (!inner_maps->resize (dataSets.len))) परत असत्य;
    साठी (hb_codepoint_t var_idx : var_indices)
    {
      अचिन्हित outer = var_idx >> 16;
      अचिन्हित inner = var_idx & 0xFFFFu;
      जर (unlikely (outer >= dataSets.len ||
		    inner >= (हे+dataSets[outer]).get_item_मोजणी ()))
	परत असत्य;
      (*inner_maps)[outer].add (inner);
    }

    अचिन्हित new_outer = 0;
    साठी (अचिन्हित outer = 0; outer < inner_maps->length; outer++)
    {
      स्थिरांक hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      जर (!inner_map.get_population ()) सुरूठेवा;
      जर (unlikely (inner_map.in_error ())) परत असत्य;
      साठी (अचिन्हित new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	अचिन्हित old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    परत !inner_maps->in_error () && !varidx_map->in_error ();
  }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक MultiItemVariationStore *src,
		  स्थिरांक hb_array_t<स्थिरांक hb_inc_bimap_t> &inner_maps,
		  स्थिरांक hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (असत्य);
    format = 1;

    स्थिरांक स्वतः &src_regions = src+src->regions;
    hb_set_t region_indices;
    अचिन्हित set_मोजणी = 0;
    साठी (अचिन्हित i = 0; i < src->dataSets.len; i++)
    {
      जर (!inner_maps[i].get_population ()) सुरूठेवा;
      set_मोजणी++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    जर (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (असत्य);

    dataSets.len = set_मोजणी;
    जर (unlikely (!c->extend (dataSets))) return_trace (असत्य);
    अचिन्हित set_index = 0;
    साठी (अचिन्हित i = 0; i < src->dataSets.len; i++)
    {
      जर (!inner_maps[i].get_population ()) सुरूठेवा;
      जर (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  संरक्षित:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

ढाँचा <typename Map_मोजणी_T>
संरचना DeltaSetIndexMapFormat01
{
  मित्र संरचना DeltaSetIndexMap;

  माप_प्रकार get_size () स्थिरांक
  { परत hb_unsigned_mul_add_saturate (map_मोजणी_2, get_width (), min_size); }

  खाजगी:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (हे));
  }

  ढाँचा <typename T>
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक T &plan)
  {
    अचिन्हित पूर्णांक width = plan.get_width ();
    अचिन्हित पूर्णांक inner_bit_मोजणी = plan.get_inner_bit_मोजणी ();
    स्थिरांक hb_array_t<स्थिरांक uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (हे);
    जर (unlikely (output_map.length && ((((inner_bit_मोजणी-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (असत्य);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);

    entryFormat = ((width-1)<<4)|(inner_bit_मोजणी-1);
    map_मोजणी_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    जर (unlikely (!p)) return_trace (असत्य);
    साठी (अचिन्हित पूर्णांक i = 0; i < output_map.length; i++)
    {
      अचिन्हित पूर्णांक v = output_map.arrayZ[i];
      जर (v)
      {
	अचिन्हित पूर्णांक outer = v >> 16;
	अचिन्हित पूर्णांक inner = v & 0xFFFF;
	अचिन्हित पूर्णांक u = (outer << inner_bit_मोजणी) | inner;
	साठी (अचिन्हित पूर्णांक w = width; w > 0;)
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
  uint32_t map (अचिन्हित पूर्णांक v) स्थिरांक /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    जर (!map_मोजणी_2)
      परत v;
    परत _map (v);
  }

  HB_HOT
  uint32_t _map (अचिन्हित पूर्णांक v) स्थिरांक /* Returns 16.16 outer.inner. */
  {
    जर (v >= map_मोजणी_2)
      v = map_मोजणी_2 - 1;

    अचिन्हित पूर्णांक u = 0;
    { /* Fetch it. */
      अचिन्हित पूर्णांक w = get_width ();
      स्थिरांक HBUINT8 *p = mapDataZ.arrayZ + w * v;
      साठी (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      अचिन्हित पूर्णांक n = get_inner_bit_मोजणी ();
      अचिन्हित पूर्णांक outer = u >> n;
      अचिन्हित पूर्णांक inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    परत u;
  }

  अचिन्हित get_map_मोजणी () स्थिरांक       { परत map_मोजणी_2; }
  अचिन्हित get_width () स्थिरांक           { परत ((entryFormat >> 4) & 3) + 1; }
  अचिन्हित get_inner_bit_मोजणी () स्थिरांक { परत (entryFormat & 0xF) + 1; }


  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_मोजणी_2,
                                  get_width ()));
  }

  संरक्षित:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_मोजणी_T     map_मोजणी_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2+Map_मोजणी_T::static_size, mapDataZ);
};

संरचना DeltaSetIndexMap
{
  ढाँचा <typename T>
  तार्किक serialize (hb_serialize_context_t *c, स्थिरांक T &plan)
  {
    TRACE_SERIALIZE (हे);
    अचिन्हित length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    स्थिती 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    पूर्वनिर्धारित:return_trace (असत्य);
    }
  }

  uint32_t map (अचिन्हित v) स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); परत (u.format0.map (v));
    स्थिती 1: hb_barrier (); परत (u.format1.map (v));
    पूर्वनिर्धारित:परत v;
    }
  }

  अचिन्हित get_map_मोजणी () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); परत u.format0.get_map_मोजणी ();
    स्थिती 1: hb_barrier (); परत u.format1.get_map_मोजणी ();
    पूर्वनिर्धारित:परत 0;
    }
  }

  अचिन्हित get_width () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); परत u.format0.get_width ();
    स्थिती 1: hb_barrier (); परत u.format1.get_width ();
    पूर्वनिर्धारित:परत 0;
    }
  }

  अचिन्हित get_inner_bit_मोजणी () स्थिरांक
  {
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); परत u.format0.get_inner_bit_मोजणी ();
    स्थिती 1: hb_barrier (); परत u.format1.get_inner_bit_मोजणी ();
    पूर्वनिर्धारित:परत 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.format.v.sanitize (c)) return_trace (असत्य);
    hb_barrier ();
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    स्थिती 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    पूर्वनिर्धारित:return_trace (सत्य);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    निवड (u.format.v) {
    स्थिती 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    स्थिती 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    पूर्वनिर्धारित:return_trace (nullptr);
    }
  }

  संरक्षित:
  संघ {
  संरचना { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (1, format.v);
};


संरचना ItemVarStoreInstancer
{
  ItemVarStoreInstancer (स्थिरांक ItemVariationStore *varStore_,
			 स्थिरांक DeltaSetIndexMap *varIdxMap,
			 hb_array_t<स्थिरांक पूर्णांक> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    जर (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    जर (!hb_any (coords))
      हे->coords = {};
  }

  चालक तार्किक () स्थिरांक { परत varStore && तार्किक (coords); }

  दशांश चालक[] (uint32_t varIdx) स्थिरांक
  { परत (*हे) (varIdx); }

  दशांश चालक() (uint32_t varIdx, अचिन्हित लघु offset = 0) स्थिरांक
  { परत get_delta (varIdx, offset); }

  द्विगुण get_condition_delta (uint32_t varIdx) स्थिरांक
  { परत get_delta<द्विगुण> (varIdx); }

  ढाँचा <typename Float = दशांश>
  Float get_delta (uint32_t varIdx, अचिन्हित लघु offset = 0) स्थिरांक
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   जर (!coords || varIdx == VarIdx::NO_VARIATION)
     परत Float (0);

    varIdx += offset;
    जर (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    परत varStore->get_delta<Float> (varIdx, coords, cache);
  }

  स्थिरांक ItemVariationStore *varStore;
  स्थिरांक DeltaSetIndexMap *varIdxMap;
  hb_array_t<स्थिरांक पूर्णांक> coords;
  hb_scalar_cache_t *cache;
};

संरचना MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (स्थिरांक MultiItemVariationStore *varStore,
			      स्थिरांक DeltaSetIndexMap *varIdxMap,
			      hb_array_t<स्थिरांक पूर्णांक> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    जर (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  चालक तार्किक () स्थिरांक { परत varStore; }

  द्विगुण get_condition_delta (uint32_t varIdx) स्थिरांक
  { परत (द्विगुण) (*हे)[varIdx]; }

  द्विगुण चालक[] (uint32_t varIdx) स्थिरांक
  {
    द्विगुण v = 0;
    (*हे) (hb_array (&v, 1), varIdx);
    परत v;
  }

  शून्य चालक() (hb_array_t<द्विगुण> out, uint32_t varIdx, अचिन्हित लघु offset = 0) स्थिरांक
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    जर (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      जर (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    अन्यथा
      साठी (अचिन्हित i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  स्थिरांक MultiItemVariationStore *varStore;
  स्थिरांक DeltaSetIndexMap *varIdxMap;
  hb_array_t<स्थिरांक पूर्णांक> coords;
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
_hb_recurse_condition_evaluate (स्थिरांक संरचना Condition &condition,
				स्थिरांक पूर्णांक *coords,
				अचिन्हित पूर्णांक coord_len,
				Instancer *instancer);

संरचना ConditionAxisRange
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    स्थिरांक hb_map_t *index_map = &c->plan->axes_index_map;
    जर (index_map->is_empty ()) return_trace (सत्य);

    स्थिरांक hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    जर (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (असत्य);

    स्थिरांक hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    जर (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    स्थिरांक hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    जर (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    दशांश normalized_min = renormalize_मूल्य ((द्विगुण) filter_Range_Min_मूल्य.to_float (), axis_limit, axis_triple_distances, असत्य);
    दशांश normalized_max = renormalize_मूल्य ((द्विगुण) filter_Range_Max_मूल्य.to_float (), axis_limit, axis_triple_distances, असत्य);
    out->filter_Range_Min_मूल्य.set_float (normalized_min);
    out->filter_Range_Max_मूल्य.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  खाजगी:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) स्थिरांक
  {
    //invalid axis index, drop the entire record
    जर (!c->axes_index_tag_map->has (axisIndex))
      परत DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    तार्किक axis_set_by_user = असत्य;
    जर (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = सत्य;
    }

    दशांश axis_min_val = axis_range.minimum;
    दशांश axis_default_val = axis_range.मधला;
    दशांश axis_max_val = axis_range.maximum;

    दशांश filter_min_val = filter_Range_Min_मूल्य.to_float ();
    दशांश filter_max_val = filter_Range_Max_मूल्य.to_float ();

    जर (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = असत्य;

    //condition not met, drop the entire record
    जर (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      परत DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    जर (axis_set_by_user && axis_range.is_point ())
      परत DROP_COND_WITH_VAR;

    जर (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_मूल्य.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_मूल्य.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      परत KEEP_COND_WITH_VAR;
    }
    परत KEEP_RECORD_WITH_VAR;
  }

  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer HB_UNUSED) स्थिरांक
  {
    पूर्णांक coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    परत filter_Range_Min_मूल्य.to_int () <= coord && coord <= filter_Range_Max_मूल्य.to_int ();
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_मूल्य;
  F2DOT14	filter_Range_Max_मूल्य;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना Condition_मूल्य
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    जर (varIdx == VarIdx::NO_VARIATION)
      return_trace (सत्य);

    hb_pair_t<अचिन्हित, पूर्णांक> *condition_मूल्य;
    जर (c->plan->layout_condition_idx_मूल्य_map.has ((uintptr_t) हे, &condition_मूल्य))
    {
      out->default_मूल्य_2 = condition_मूल्य->second;
      out->varIdx = condition_मूल्य->first;
      return_trace (सत्य);
    }

    hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
    जर (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (असत्य);

    जर (unlikely (!c->serializer->check_assign (
		out->default_मूल्य_2,
		default_मूल्य_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (सत्य);
  }

  खाजगी:
  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिरांक
  {
    द्विगुण value = default_मूल्य_2;
    value += instancer->get_condition_delta (varIdx);
    परत value > 0;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_मूल्य_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना ConditionAnd
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);
    out->format = format;

    साठी (स्थिरांक स्वतः &offset : conditions)
    {
      जर (offset.is_null ())
      {
	स्वतः *out_offset = out->conditions.serialize_append (c->serializer);
	जर (unlikely (!out_offset)) return_trace (असत्य);
	*out_offset = 0;
      }
      अन्यथा जर (unlikely (!subset_offset_array (c, out->conditions, हे) (offset)))
	return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  खाजगी:
  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = conditions.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (!_hb_recurse_condition_evaluate (हे+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	परत असत्य;
    परत सत्य;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (conditions.sanitize (c, हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<संरचना Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (3, conditions);
};

संरचना ConditionOr
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);
    out->format = format;

    साठी (स्थिरांक स्वतः &offset : conditions)
    {
      जर (offset.is_null ())
      {
	स्वतः *out_offset = out->conditions.serialize_append (c->serializer);
	जर (unlikely (!out_offset)) return_trace (असत्य);
	*out_offset = 0;
      }
      अन्यथा जर (unlikely (!subset_offset_array (c, out->conditions, हे) (offset)))
	return_trace (असत्य);
    }

    return_trace (सत्य);
  }

  खाजगी:
  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = conditions.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (_hb_recurse_condition_evaluate (हे+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	परत सत्य;
    परत असत्य;
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (conditions.sanitize (c, हे));
  }

  संरक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<संरचना Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (3, conditions);
};

संरचना ConditionNegate
{
  मित्र संरचना Condition;

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);
    जर (condition.is_null ())
    {
      out->condition = 0;
      return_trace (सत्य);
    }
    return_trace (out->condition.serialize_subset (c, condition, हे));
  }

  खाजगी:
  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिरांक
  {
    परत !_hb_recurse_condition_evaluate (हे+condition,
					    coords, coord_len,
					    instancer);
  }

  सार्वजनिक:
  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (condition.sanitize (c, हे));
  }

  संरक्षित:
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
    स्वतः *out = c->start_embed<Condition_मूल्य> ();
    जर (unlikely (!out || !c->extend_min (out))) परत असत्य;
    out->format = 2;
    out->default_मूल्य_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    परत सत्य;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      अचिन्हित depth = HB_MAX_NESTING_LEVEL,
				      तार्किक materialize_true = असत्य) स्थिरांक;

  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिरांक
  {
    निवड (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    स्थिती 0: परत सत्य;
    स्थिती 1: hb_barrier (); परत u.format1.evaluate (coords, coord_len, instancer);
    स्थिती 2: hb_barrier (); परत u.format2.evaluate (coords, coord_len, instancer);
    स्थिती 3: hb_barrier (); परत u.format3.evaluate (coords, coord_len, instancer);
    स्थिती 4: hb_barrier (); परत u.format4.evaluate (coords, coord_len, instancer);
    स्थिती 5: hb_barrier (); परत u.format5.evaluate (coords, coord_len, instancer);
    पूर्वनिर्धारित:परत असत्य;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             अचिन्हित depth = HB_MAX_NESTING_LEVEL) स्थिरांक
  {
    जर (unlikely (!depth))
    {
      c->apply = असत्य;
      परत DROP_RECORD_WITH_VAR;
    }

    निवड (u.format.v) {
    स्थिती 0: परत KEEP_RECORD_WITH_VAR;
    स्थिती 1: hb_barrier (); परत u.format1.keep_with_variations (c, condition_map);
    स्थिती 2: स्थिती 3: स्थिती 4: स्थिती 5: थांबा;
    पूर्वनिर्धारित: c->apply = असत्य; परत DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    तार्किक applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    जर (c->all_axes_pinned)
      परत applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    जर (u.format.v == 2)
      परत u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    स्वतः classify_child = [c, condition_map, depth] (स्थिरांक Condition &child)
    {
      स्वतः child_context = *c;
      child_context.apply = सत्य;
      परत child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    जर (u.format.v == 5)
    {
      स्वतः result = classify_child (&u.format5 + u.format5.condition);
      परत result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    तार्किक is_and = u.format.v == 3;
    तार्किक variable = असत्य;
    स्थिरांक स्वतः &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    स्थिरांक शून्य *base = is_and ? (स्थिरांक शून्य *) &u.format3 : (स्थिरांक शून्य *) &u.format4;
    साठी (स्थिरांक स्वतः &offset : conditions)
    {
      स्वतः result = classify_child (base + offset);
      जर (is_and && result == DROP_RECORD_WITH_VAR)
        परत DROP_RECORD_WITH_VAR;
      जर (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        परत KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    परत variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिरांक
  {
    जर (unlikely (!c->may_dispatch (हे, &u.format.v))) परत c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (हे, u.format.v);
    निवड (u.format.v) {
    स्थिती 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    स्थिती 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    स्थिती 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    स्थिती 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    स्थिती 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    पूर्वनिर्धारित:return_trace (c->default_return_मूल्य ());
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    जर (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मूल्य ());

    return_trace (c->end_recursion (हे->dispatch (c)));
  }

  तार्किक collect_var_indices (hb_set_t *var_indices,
			    अचिन्हित depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>> *condition_मूल्ये = nullptr) स्थिरांक;

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक Condition *src,
		  स्थिरांक hb_map_t &varidx_map,
		  स्थिरांक hb_map_t *axis_map = nullptr);

  खाजगी:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      अचिन्हित depth) स्थिरांक;

  संरक्षित:
  संघ {
  संरचना { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_मूल्य	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  सार्वजनिक:
  DEFINE_SIZE_UNION (2, format.v);
};

ढाँचा <typename Instancer>
तार्किक
_hb_recurse_condition_evaluate (स्थिरांक संरचना Condition &condition,
				स्थिरांक पूर्णांक *coords,
				अचिन्हित पूर्णांक coord_len,
				Instancer *instancer)
{
  परत condition.evaluate (coords, coord_len, instancer);
}

संरचना ConditionList
{
  स्थिरांक Condition& चालक[] (अचिन्हित i) स्थिरांक
  { परत हे+conditions[i]; }

  अचिन्हित get_मोजणी () स्थिरांक { परत conditions.len; }

  तार्किक serialize (hb_serialize_context_t *c,
		  स्थिरांक ConditionList *src,
		  स्थिरांक hb_inc_bimap_t &condition_map,
		  स्थिरांक hb_map_t &varidx_map,
		  स्थिरांक hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (हे);
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    conditions.len = condition_map.get_population ();
    जर (unlikely (!c->extend (conditions))) return_trace (असत्य);
    साठी (अचिन्हित i = 0; i < conditions.len; i++)
    {
      अचिन्हित old_index = condition_map.backward (i);
      जर (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (असत्य);
    }
    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (conditions.sanitize (c, हे));
  }

  संरक्षित:
  Array32OfOffset32To<Condition> conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, conditions);
};

ओळीत तार्किक
Condition::collect_var_indices (hb_set_t *var_indices, अचिन्हित depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>> *condition_मूल्ये) स्थिरांक
{
  जर (unlikely (!depth)) परत असत्य;
  निवड (u.format.v)
  {
    स्थिती 0:
    स्थिती 1:
      परत सत्य;
    स्थिती 2:
      जर (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        जर (condition_मूल्ये &&
            !condition_मूल्ये->set ((uintptr_t) &u.format2,
                                    hb_pair (अचिन्हित (u.format2.varIdx), पूर्णांक (u.format2.default_मूल्य_2))))
          परत असत्य;
      }
      परत !var_indices->in_error ();
    स्थिती 3:
      साठी (स्थिरांक स्वतः &offset : u.format3.conditions)
	जर (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मूल्ये)))
	  परत असत्य;
      परत सत्य;
    स्थिती 4:
      साठी (स्थिरांक स्वतः &offset : u.format4.conditions)
	जर (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मूल्ये)))
	  परत असत्य;
      परत सत्य;
    स्थिती 5:
      परत (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_मूल्ये);
    पूर्वनिर्धारित:
      परत असत्य;
  }
}

ओळीत तार्किक
Condition::serialize (hb_serialize_context_t *c,
		      स्थिरांक Condition *src,
		      स्थिरांक hb_map_t &varidx_map,
		      स्थिरांक hb_map_t *axis_map)
{
  TRACE_SERIALIZE (हे);
  निवड (src->u.format.v)
  {
    स्थिती 0:
    {
      स्वतः *out = c->start_embed<Condition_मूल्य> ();
      जर (unlikely (!out || !c->extend_min (out))) return_trace (असत्य);
      out->format = 2;
      out->default_मूल्य_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (सत्य);
    }
    स्थिती 1:
    {
      स्वतः *out = c->embed (&src->u.format1);
      जर (unlikely (!out)) return_trace (असत्य);
      जर (axis_map)
      {
	जर (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (असत्य);
	}
	जर (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (असत्य);
      }
      return_trace (सत्य);
    }
    स्थिती 2:
    {
      स्वतः *out = c->embed (&src->u.format2);
      जर (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (असत्य);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (सत्य);
    }
    स्थिती 3:
    {
      स्वतः *out = c->start_embed<ConditionAnd> ();
      जर (unlikely (!c->extend_min (out))) return_trace (असत्य);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      जर (unlikely (!c->extend (out->conditions))) return_trace (असत्य);
      साठी (अचिन्हित i = 0; i < out->conditions.len; i++)
	जर (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (असत्य);
      return_trace (सत्य);
    }
    स्थिती 4:
    {
      स्वतः *out = c->start_embed<ConditionOr> ();
      जर (unlikely (!c->extend_min (out))) return_trace (असत्य);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      जर (unlikely (!c->extend (out->conditions))) return_trace (असत्य);
      साठी (अचिन्हित i = 0; i < out->conditions.len; i++)
	जर (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (असत्य);
      return_trace (सत्य);
    }
    स्थिती 5:
    {
      स्वतः *out = c->embed (&src->u.format5);
      जर (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (असत्य);
      return_trace (सत्य);
    }
    पूर्वनिर्धारित:
      return_trace (असत्य);
  }
}

ओळीत lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    अचिन्हित depth,
				    तार्किक materialize_true) स्थिरांक
{
  स्वतः snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  जर (result == LOOKUP_CONDITION_SUBSET_KEEP)
    परत result;

  c->serializer->revert (snap);
  जर (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    परत result;

  परत serialize_constant (c->serializer, सत्य) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

ओळीत lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 अचिन्हित depth) स्थिरांक
{
  जर (unlikely (!depth))
    परत LOOKUP_CONDITION_SUBSET_ERROR;

  स्वतः subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   स्थिरांक Condition &condition)
      -> lookup_condition_subset_result_t
  {
    स्वतः snap = c->serializer->snapshot ();
    स्वतः *offset = out.serialize_append (c->serializer);
    जर (unlikely (!offset))
      परत LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    जर (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->काढा_pack ());
    अन्यथा
    {
      c->serializer->काढा_discard ();
      out.काढा ();
      c->serializer->revert (snap);
    }
    परत result;
  };

  निवड (u.format.v)
  {
    स्थिती 0:
      परत LOOKUP_CONDITION_SUBSET_TRUE;

    स्थिती 1:
    {
      स्थिरांक ConditionAxisRange &src = u.format1;
      जर (c->plan->user_axes_location.is_empty ())
        परत c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      जर (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        परत LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      जर (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      दशांश filter_min = src.filter_Range_Min_मूल्य.to_float ();
      दशांश filter_max = src.filter_Range_Max_मूल्य.to_float ();
      जर (filter_min > filter_max)
        परत LOOKUP_CONDITION_SUBSET_FALSE;
      जर (axis_limit.is_point ())
        परत filter_min <= axis_limit.मधला &&
	       axis_limit.मधला <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      जर (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        परत LOOKUP_CONDITION_SUBSET_FALSE;
      जर (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        परत LOOKUP_CONDITION_SUBSET_TRUE;
      जर (!c->plan->axes_index_map.has (src.axisIndex))
        परत LOOKUP_CONDITION_SUBSET_ERROR;

      स्वतः *out = c->serializer->embed (&src);
      जर (unlikely (!out))
        परत LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      जर (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_मूल्य.set_float (
	  renormalize_मूल्य (filter_min, axis_limit, axis_distances, असत्य));
      out->filter_Range_Max_मूल्य.set_float (
	  renormalize_मूल्य (filter_max, axis_limit, axis_distances, असत्य));
      जर (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        परत LOOKUP_CONDITION_SUBSET_ERROR;
      परत LOOKUP_CONDITION_SUBSET_KEEP;
    }

    स्थिती 2:
    {
      स्थिरांक Condition_मूल्य &src = u.format2;
      स्वतः *out = c->serializer->embed (&src);
      जर (unlikely (!out))
        परत LOOKUP_CONDITION_SUBSET_ERROR;

      जर (src.varIdx == VarIdx::NO_VARIATION)
        परत c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णांक) src.default_मूल्य_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<अचिन्हित, पूर्णांक> *condition_मूल्य;
      जर (c->plan->layout_condition_idx_मूल्य_map.has ((uintptr_t) &src, &condition_मूल्य))
      {
        out->default_मूल्य_2 = condition_मूल्य->second;
        out->varIdx = condition_मूल्य->first;
        जर (out->varIdx == VarIdx::NO_VARIATION)
          परत out->default_मूल्य_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        परत LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      जर (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        परत c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णांक) src.default_मूल्य_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      जर (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        द्विगुण value = (पूर्णांक) src.default_मूल्य_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        परत value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      पूर्णांक default_मूल्य = (पूर्णांक) src.default_मूल्य_2 +
			  hb_second (*new_varidx_delta);
      जर (unlikely (!c->serializer->check_assign (
		out->default_मूल्य_2,
		default_मूल्य,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        परत LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      परत LOOKUP_CONDITION_SUBSET_KEEP;
    }

    स्थिती 3:
    {
      स्थिरांक ConditionAnd &src = u.format3;
      स्वतः *out = c->serializer->start_embed (&src);
      जर (unlikely (!out || !c->serializer->extend_min (out)))
        परत LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      साठी (स्थिरांक स्वतः &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        जर (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          परत result;
      }
      परत out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    स्थिती 4:
    {
      स्थिरांक ConditionOr &src = u.format4;
      स्वतः *out = c->serializer->start_embed (&src);
      जर (unlikely (!out || !c->serializer->extend_min (out)))
        परत LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      साठी (स्थिरांक स्वतः &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        जर (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          परत result;
      }
      परत out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    स्थिती 5:
    {
      स्थिरांक ConditionNegate &src = u.format5;
      स्वतः *out = c->serializer->embed (&src);
      जर (unlikely (!out))
        परत LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      जर (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->काढा_pack ());
        परत result;
      }

      c->serializer->काढा_discard ();
      जर (result == LOOKUP_CONDITION_SUBSET_TRUE)
        परत LOOKUP_CONDITION_SUBSET_FALSE;
      जर (result == LOOKUP_CONDITION_SUBSET_FALSE)
        परत LOOKUP_CONDITION_SUBSET_TRUE;
      परत result;
    }

    पूर्वनिर्धारित:
      परत LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

संरचना ConditionSet
{
  तार्किक evaluate (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 ItemVarStoreInstancer *instancer) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = conditions.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
      जर (!(हे+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	परत असत्य;
    परत सत्य;
  }

  शून्य keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिरांक
  {
    hb_map_t *condition_map = hb_map_create ();
    जर (unlikely (!condition_map)) परत;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    जर (unlikely (!cond_set)) परत;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = सत्य;
    तार्किक should_keep = असत्य;
    /* The range map cannot identify value or boolean expressions uniquely. */
    तार्किक can_deduplicate = सत्य;
    अचिन्हित अंक_kept_cond = 0, cond_idx = 0;
    साठी (स्थिरांक स्वतः& offset : conditions)
    {
      स्थिरांक Condition &condition = हे+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      जर (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = असत्य;
        परत;
      }

      जर (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = सत्य;
        cond_set->add (cond_idx);
        अंक_kept_cond++;
      }

      जर (ret == KEEP_RECORD_WITH_VAR)
        should_keep = सत्य;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = अंक_kept_cond == 0;
    जर (!should_keep && c->all_axes_pinned) परत;

    //check if condition_set is unique with variations
    जर (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      परत;

    जर (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  तार्किक subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               तार्किक घाला_catch_all) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    जर (घाला_catch_all) return_trace (सत्य);

    hb_set_t *retained_cond_set = nullptr;
    जर (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    अचिन्हित पूर्णांक मोजणी = conditions.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (retained_cond_set != nullptr && !retained_cond_set->has (i))
        सुरूठेवा;
      स्थिरांक Condition &condition = हे+conditions[i];
      जर (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, हे) (conditions[i]);
        सुरूठेवा;
      }

      स्वतः *offset = out->conditions.serialize_append (c->serializer);
      जर (unlikely (!offset)) return_trace (असत्य);
      c->serializer->push ();
      स्वतः result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, सत्य);
      जर (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, असत्य) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      जर (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->काढा_discard ();
        c->serializer->check_success (असत्य);
        return_trace (असत्य);
      }
      c->serializer->add_link (*offset, c->serializer->काढा_pack ());
    }

    return_trace (तार्किक (out->conditions));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (conditions.sanitize (c, हे));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    साठी (स्थिरांक स्वतः &offset : conditions)
      जर (unlikely (!(हे+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_मूल्ये)))
	c->layout_variation_indices->err ();
  }

  संरक्षित:
  Array16OfOffset32To<Condition>	conditions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, conditions);
};

संरचना FeatureTableSubstitutionRecord
{
  मित्र संरचना FeatureTableSubstitution;

  शून्य collect_lookups (स्थिरांक शून्य *base, hb_set_t *lookup_indexes /* OUT */) स्थिरांक
  {
    परत (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  शून्य closure_features (स्थिरांक शून्य *base,
			 स्थिरांक hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) स्थिरांक
  {
    जर ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  शून्य collect_feature_substitutes_with_variations (hb_hashmap_t<अचिन्हित, स्थिरांक Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    स्थिरांक hb_set_t *feature_indices,
                                                    स्थिरांक शून्य *base) स्थिरांक
  {
    जर (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  तार्किक serialize (hb_subset_layout_context_t *c,
                  अचिन्हित feature_index,
                  स्थिरांक Feature *f, स्थिरांक Tag *tag)
  {
    TRACE_SERIALIZE (हे);
    hb_serialize_context_t *s = c->subset_context->serializer;
    जर (unlikely (!s->extend_min (हे))) return_trace (असत्य);

    uint32_t *new_feature_idx;
    जर (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (असत्य);

    जर (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (असत्य);

    s->push ();
    तार्किक ret = f->subset (c->subset_context, c, tag);
    जर (ret) s->add_link (feature, s->काढा_pack ());
    अन्यथा s->काढा_discard ();

    return_trace (ret);
  }

  तार्किक subset (hb_subset_layout_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SUBSET (हे);
    uint32_t *new_feature_index;
    जर (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (असत्य);

    स्वतः *out = c->subset_context->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    out->featureIndex = *new_feature_index;
    hb_pair_t<स्थिरांक शून्य*, स्थिरांक शून्य*> *original;
    जर (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (असत्य);
    स्थिरांक Tag *tag = reinterpret_cast<स्थिरांक Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base,
		 स्थिरांक FeatureList *feature_list) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!c->check_struct (हे))) return_trace (असत्य);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    स्थिरांक Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  संरक्षित:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना FeatureTableSubstitution
{
  शून्य collect_feature_indices (hb_set_t *feature_indices) स्थिरांक
  {
    साठी (स्थिरांक FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  स्थिरांक Feature *find_substitute (अचिन्हित पूर्णांक feature_index) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = substitutions.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      स्थिरांक FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      जर (record.featureIndex == feature_index)
	परत &(हे+record.feature);
    }
    परत nullptr;
  }

  शून्य collect_lookups (स्थिरांक hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) स्थिरांक
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([हे, lookup_indexes] (स्थिरांक FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (हे, lookup_indexes); })
    ;
  }

  ढाँचा <typename TagGetter>
  शून्य collect_name_ids (स्थिरांक hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) स्थिरांक
  {
    साठी (स्थिरांक FeatureTableSubstitutionRecord &record : substitutions)
      जर (feature_index_map->has (record.featureIndex))
        (हे+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  शून्य closure_features (स्थिरांक hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) स्थिरांक
  {
    साठी (स्थिरांक FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (हे, lookup_indexes, feature_indexes);
  }

  तार्किक intersects_features (स्थिरांक hb_map_t *feature_index_map) स्थिरांक
  {
    साठी (स्थिरांक FeatureTableSubstitutionRecord& record : substitutions)
    {
      जर (feature_index_map->has (record.featureIndex)) परत सत्य;
    }
    परत असत्य;
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिरांक
  {
    साठी (स्थिरांक FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, हे);
  }

  तार्किक subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               तार्किक घाला_catch_all) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    out->version.major = version.major;
    out->version.minor = version.minor;

    जर (घाला_catch_all)
    {
      साठी (अचिन्हित feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<स्थिरांक शून्य*, स्थिरांक शून्य*> *p;
        जर (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (असत्य);
        स्वतः *o = out->substitutions.serialize_append (c->serializer);
        जर (!o->serialize (l, feature_index,
                           reinterpret_cast<स्थिरांक Feature*> (p->first),
                           reinterpret_cast<स्थिरांक Tag*> (p->second)))
          return_trace (असत्य);
      }
      return_trace (सत्य);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), हे))
    ;

    return_trace (तार्किक (out->substitutions));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक FeatureList *feature_list) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, हे, feature_list));
  }

  संरक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

संरचना FeatureVariationRecord
{
  मित्र संरचना FeatureVariations;

  शून्य collect_lookups (स्थिरांक शून्य     *base,
			स्थिरांक hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) स्थिरांक
  {
    परत (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  ढाँचा <typename TagGetter>
  शून्य collect_name_ids (स्थिरांक शून्य *base,
                         स्थिरांक hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) स्थिरांक
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  शून्य closure_features (स्थिरांक शून्य     *base,
			 स्थिरांक hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) स्थिरांक
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  तार्किक intersects_features (स्थिरांक शून्य *base, स्थिरांक hb_map_t *feature_index_map) स्थिरांक
  {
    परत (base+substitutions).intersects_features (feature_index_map);
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    स्थिरांक शून्य *base) स्थिरांक
  {
    (base+conditions).keep_with_variations (c);
    जर (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = सत्य; // set variations only once
    }
  }

  तार्किक subset (hb_subset_layout_context_t *c, स्थिरांक शून्य *base,
               तार्किक घाला_catch_all = असत्य) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->subset_context->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    जर (!घाला_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    अन्यथा
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, घाला_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, घाला_catch_all);

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base,
		 स्थिरांक FeatureList *feature_list) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  स्थिरांक शून्य *base) स्थिरांक
  { (base+conditions).collect_variation_indices (c); }

  संरक्षित:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना LookupIndexList
{
  शून्य add_lookup_indexes_to (hb_set_t *lookup_indexes) स्थिरांक
  { lookupIndices.add_indexes_to (lookup_indexes); }

  तार्किक intersects_lookup_indexes (स्थिरांक hb_map_t *lookup_indexes) स्थिरांक
  { परत lookupIndices.intersects (lookup_indexes); }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);

    स्वतः it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (तार्किक (out->lookupIndices));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (lookupIndices.sanitize (c));
  }

  संरक्षित:
  IndexArray	lookupIndices;
  सार्वजनिक:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

संरचना LookupConditionRecord
{
  अचिन्हित get_condition_offset (स्थिरांक शून्य *base, स्थिरांक शून्य *variations) स्थिरांक
  { परत (स्थिरांक अक्षर *) &(base+condition) - (स्थिरांक अक्षर *) variations; }

  ढाँचा <typename Instancer>
  तार्किक evaluate (स्थिरांक शून्य *base,
		 स्थिरांक पूर्णांक *coords,
		 अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिरांक
  { परत (base+condition).evaluate (coords, coord_len, instancer); }

  तार्किक evaluate (स्थिरांक शून्य *base,
                 स्थिरांक शून्य *variations,
                 hb_sorted_array_t<स्थिरांक अचिन्हित> conditions) स्थिरांक
  {
    परत !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     आकार (अचिन्हित),
                                     _hb_cmp_operator<अचिन्हित, अचिन्हित>);
  }

  तार्किक collect_condition (स्थिरांक शून्य *base,
                          स्थिरांक शून्य *variations,
                          स्थिरांक पूर्णांक *coords,
                          अचिन्हित पूर्णांक coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<अचिन्हित> *conditions) स्थिरांक
  {
    /* Null conditions always apply and need no key state. */
    जर (!condition) परत सत्य;

    अचिन्हित offset = get_condition_offset (base, variations);
    जर (visited->has (offset)) परत सत्य;
    visited->add (offset);
    जर (unlikely (visited->in_error ())) परत असत्य;

    जर (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    परत !conditions->in_error ();
  }

  शून्य add_lookup_indexes_to (स्थिरांक शून्य *base,
			      hb_set_t *lookup_indexes) स्थिरांक
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  तार्किक intersects_lookup_indexes (स्थिरांक शून्य *base,
				   स्थिरांक hb_map_t *lookup_indexes) स्थिरांक
  { परत (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  स्थिरांक शून्य *base) स्थिरांक
  {
    जर (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_मूल्ये)))
      c->layout_variation_indices->err ();
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    सत्य);
    जर (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->काढा_discard ();
      return_trace (असत्य);
    }
    c->serializer->add_link (out->condition, c->serializer->काढा_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  संरक्षित:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (8);
};

संरचना FeatureLookupsTable
{
  तार्किक adds_default_lookups () स्थिरांक { परत flags & 0x0001u; }

  तार्किक collect_conditions (स्थिरांक शून्य *variations,
                           स्थिरांक पूर्णांक *coords,
                           अचिन्हित पूर्णांक coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<अचिन्हित> *conditions) स्थिरांक
  {
    साठी (स्थिरांक LookupConditionRecord &record : records)
      जर (unlikely (!record.collect_condition (हे, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	परत असत्य;
    परत सत्य;
  }

  शून्य collect_lookups (स्थिरांक शून्य *variations,
                       hb_sorted_array_t<स्थिरांक अचिन्हित> conditions,
                       hb_set_t *lookup_indexes) स्थिरांक
  {
    साठी (स्थिरांक LookupConditionRecord &record : records)
      जर (record.evaluate (हे, variations, conditions))
	record.add_lookup_indexes_to (हे, lookup_indexes);
  }

  ढाँचा <typename Instancer>
  शून्य collect_lookups (स्थिरांक पूर्णांक *coords,
			अचिन्हित पूर्णांक coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) स्थिरांक
  {
    साठी (स्थिरांक LookupConditionRecord &record : records)
      जर (record.evaluate (हे, coords, coord_len, instancer))
	record.add_lookup_indexes_to (हे, lookup_indexes);
  }

  शून्य collect_lookups (hb_set_t *lookup_indexes) स्थिरांक
  {
    साठी (स्थिरांक LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (हे, lookup_indexes);
  }

  तार्किक intersects_lookup_indexes (स्थिरांक hb_map_t *lookup_indexes) स्थिरांक
  {
    साठी (स्थिरांक LookupConditionRecord &record : records)
      जर (record.intersects_lookup_indexes (हे, lookup_indexes))
	परत सत्य;
    परत असत्य;
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    साठी (स्थिरांक LookupConditionRecord &record : records)
      record.collect_variation_indices (c, हे);
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (हे);
    जर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (असत्य);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    साठी (स्थिरांक LookupConditionRecord &record : records)
    {
      स्वतः snap = c->serializer->snapshot ();
      जर (record.subset (c, l, हे))
	out->records.len++;
      अन्यथा
	c->serializer->revert (snap);
    }

    return_trace (सत्य);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, हे));
  }

  संरक्षित:
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

  अचिन्हित get_feature_index () स्थिरांक { परत featureIndex; }

  शून्य collect_lookups (स्थिरांक शून्य *base,
                       स्थिरांक Feature &feature,
                       hb_sorted_array_t<स्थिरांक अचिन्हित> conditions,
                       hb_set_t *lookup_indexes) स्थिरांक
  {
    स्थिरांक FeatureLookupsTable &feature_lookups = base+featureLookups;
    जर (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  शून्य collect_lookups (स्थिरांक शून्य *base,
			स्थिरांक hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) स्थिरांक
  {
    जर (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  शून्य closure_features (स्थिरांक शून्य *base,
			 स्थिरांक hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) स्थिरांक
  {
    जर ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  स्थिरांक शून्य *base) स्थिरांक
  { (base+featureLookups).collect_variation_indices (c); }

  तार्किक subset (hb_subset_layout_context_t *l, स्थिरांक शून्य *base) स्थिरांक
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (हे);
    uint32_t *new_feature_index;
    जर (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (असत्य);

    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (असत्य);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक शून्य *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) &&
		  featureLookups.sanitize (c, base));
  }

  संरक्षित:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना FeatureVariations
{
  स्थिर constexpr अचिन्हित NOT_FOUND_INDEX = 0xFFFFFFFFu;

  स्थिरांक Array32Of<LookupVariationRecord> &get_lookup_variation_records () स्थिरांक
  {
    जर (version.to_int () < 0x00010001u)
      परत Null (Array32Of<LookupVariationRecord>);
    परत StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  अचिन्हित record_मोजणी () स्थिरांक
  {
    परत varRecords.len;
  }

  तार्किक find_index (स्थिरांक पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		   अचिन्हित पूर्णांक *index,
		   ItemVarStoreInstancer *instancer) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = varRecords.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      स्थिरांक FeatureVariationRecord &record = varRecords.arrayZ[i];
      जर ((हे+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	परत सत्य;
      }
    }
    *index = NOT_FOUND_INDEX;
    परत असत्य;
  }

  स्थिरांक Feature *find_substitute (अचिन्हित पूर्णांक variations_index,
				  अचिन्हित पूर्णांक feature_index) स्थिरांक
  {
    स्थिरांक FeatureVariationRecord &record = varRecords[variations_index];
    परत (हे+record.substitutions).find_substitute (feature_index);
  }

  तार्किक collect_lookup_conditions (स्थिरांक पूर्णांक *coords,
                                  अचिन्हित पूर्णांक coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<अचिन्हित> *state) स्थिरांक
  {
    hb_set_t visited_tables, visited_conditions;
    साठी (स्थिरांक LookupVariationRecord &record : get_lookup_variation_records ())
    {
      अचिन्हित offset = record.featureLookups;
      जर (visited_tables.has (offset)) सुरूठेवा;
      visited_tables.add (offset);
      जर (unlikely (visited_tables.in_error ())) परत असत्य;

      स्थिरांक FeatureLookupsTable &feature_lookups = हे+record.featureLookups;
      जर (unlikely (!feature_lookups.collect_conditions (हे, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	परत असत्य;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->क्विक (_hb_cmp_operator<अचिन्हित, अचिन्हित>);
    परत सत्य;
  }

  तार्किक get_lookup_variations (अचिन्हित feature_index,
                              स्थिरांक Feature &feature,
                              hb_sorted_array_t<स्थिरांक अचिन्हित> conditions,
                              hb_set_t *lookup_indexes) स्थिरांक
  {
    साठी (स्थिरांक LookupVariationRecord &record : get_lookup_variation_records ())
    {
      जर (record.featureIndex != feature_index) सुरूठेवा;

      record.collect_lookups (हे, feature, conditions, lookup_indexes);
      परत सत्य;
    }
    परत असत्य;
  }

  शून्य collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = varRecords.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, हे);
      जर (c->universal)
        थांबा;
    }
    जर (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed (*हे));
  }

  शून्य collect_lookups (स्थिरांक hb_set_t *feature_indexes,
			स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) स्थिरांक
  {
    अचिन्हित मोजणी = varRecords.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        सुरूठेवा;
      varRecords[i].collect_lookups (हे, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  ढाँचा <typename TagGetter>
  शून्य collect_name_ids (स्थिरांक hb_map_t *feature_index_map,
                         स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) स्थिरांक
  {
    साठी (अचिन्हित i = 0; i < varRecords.len; i++)
    {
      जर (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        सुरूठेवा;
      varRecords[i].collect_name_ids (हे, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  शून्य collect_lookup_variation_lookups (स्थिरांक hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) स्थिरांक
  {
    साठी (स्थिरांक LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (हे, feature_indexes, lookup_indexes);
  }

  शून्य closure_features (स्थिरांक hb_map_t *lookup_indexes,
			 स्थिरांक hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) स्थिरांक
  {
    अचिन्हित पूर्णांक मोजणी = varRecords.len;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      जर (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        सुरूठेवा;
      varRecords[i].closure_features (हे, lookup_indexes, feature_indexes);
    }

    साठी (स्थिरांक LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (हे, lookup_indexes, feature_indexes);
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    साठी (स्थिरांक FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, हे);
    साठी (स्थिरांक LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, हे);
  }

  शून्य collect_feature_indices (hb_set_t *feature_indices) स्थिरांक
  {
    साठी (स्थिरांक FeatureVariationRecord &record : varRecords)
      (हे+record.substitutions).collect_feature_indices (feature_indices);
  }

  तार्किक has_lookup_variations (अचिन्हित feature_index) स्थिरांक
  {
    साठी (स्थिरांक LookupVariationRecord &record : get_lookup_variation_records ())
    {
      जर (record.featureIndex == feature_index)
	परत सत्य;
      जर (record.featureIndex > feature_index)
	थांबा;
    }
    परत असत्य;
  }

  तार्किक subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्वतः *out = c->serializer->start_embed (*हे);
    जर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (असत्य);

    out->version.major = version.major;
    out->version.minor = version.minor;

    पूर्णांक keep_up_to = -1;
    साठी (पूर्णांक i = varRecords.len - 1; i >= 0; i--) {
      जर (varRecords[i].intersects_features (हे, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        थांबा;
      }
    }

    अचिन्हित मोजणी = (अचिन्हित) (keep_up_to + 1);
    साठी (अचिन्हित i = 0; i < मोजणी; i++)
    {
      जर (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        सुरूठेवा;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), हे) (varRecords[i]);
    }

    जर (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      तार्किक घाला_catch_all_record = सत्य;
      subset_record_array (l, &(out->varRecords), हे, घाला_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    जर (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      जर (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (असत्य);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, हे))
      ;
    }

    return_trace (तार्किक (out->varRecords) ||
		  (out_lookup_records && तार्किक (*out_lookup_records)));
  }

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक FeatureList *feature_list) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, हे, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, हे)));
  }

  संरक्षित:
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

  खाजगी:

  hb_position_t get_x_delta (hb_font_t *font) स्थिरांक
  { परत get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) स्थिरांक
  { परत get_delta (font->y_ppem, font->y_scale); }

  सार्वजनिक:

  माप_प्रकार get_size () स्थिरांक
  {
    अचिन्हित पूर्णांक f = deltaFormat;
    जर (unlikely (f < 1 || f > 3 || startSize > endSize)) परत 3 * HBUINT16::static_size;
    परत HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे) && c->check_range (हे, हे->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    return_trace (c->embed<HintingDevice> (हे));
  }

  खाजगी:

  पूर्णांक get_delta (अचिन्हित पूर्णांक ppem, पूर्णांक scale) स्थिरांक
  {
    जर (!ppem) परत 0;

    पूर्णांक pixels = get_delta_pixels (ppem);

    जर (!pixels) परत 0;

    परत (पूर्णांक) (pixels * (int64_t) scale / ppem);
  }
  पूर्णांक get_delta_pixels (अचिन्हित पूर्णांक ppem_size) स्थिरांक
  {
    अचिन्हित पूर्णांक f = deltaFormat;
    जर (unlikely (f < 1 || f > 3))
      परत 0;

    जर (ppem_size < startSize || ppem_size > endSize)
      परत 0;

    अचिन्हित पूर्णांक s = ppem_size - startSize;

    अचिन्हित पूर्णांक byte = delta_मूल्य_Z[s >> (4 - f)];
    अचिन्हित पूर्णांक bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    अचिन्हित पूर्णांक mask = (0xFFFFu >> (16 - (1 << f)));

    पूर्णांक delta = bits & mask;

    जर ((अचिन्हित पूर्णांक) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    परत delta;
  }

  संरक्षित:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_मूल्य_Z;		/* Array of compressed data */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (6, delta_मूल्य_Z);
};

संरचना VariationDevice
{
  मित्र संरचना Device;

  खाजगी:

  hb_position_t get_x_delta (hb_font_t *font,
			     स्थिरांक ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  { परत !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     स्थिरांक ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  { परत !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  दशांश get_x_delta_f (hb_font_t *font,
		       स्थिरांक ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) स्थिरांक
  { परत !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  दशांश get_y_delta_f (hb_font_t *font,
		       स्थिरांक ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) स्थिरांक
  { परत !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    जर (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<अचिन्हित, पूर्णांक> *v;
    जर (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    जर (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (हे->static_size);
    स्वतः *out = c->embed (हे);
    जर (unlikely (!out)) return_trace (nullptr);

    जर (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  शून्य collect_variation_index (hb_collect_variation_indices_context_t *c) स्थिरांक
  { c->layout_variation_indices->add (varIdx); }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (c->check_struct (हे));
  }

  खाजगी:

  दशांश get_delta (hb_font_t *font,
		   स्थिरांक ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
    परत store.get_delta (varIdx, font->coords, font->अंक_coords, store_cache);
  }

  संरक्षित:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (6);
};

संरचना DeviceHeader
{
  संरक्षित:
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
  दशांश get_x_delta_f (hb_font_t *font,
		       स्थिरांक ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
#ifndef HB_NO_VAR
    जर (u.b.format == 0x8000)
    {
      hb_barrier ();
      परत u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    परत get_x_delta (font, store, store_cache);
  }

  दशांश get_y_delta_f (hb_font_t *font,
		       स्थिरांक ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
#ifndef HB_NO_VAR
    जर (u.b.format == 0x8000)
    {
      hb_barrier ();
      परत u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    परत get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     स्थिरांक ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
    निवड (u.b.format)
    {
#ifndef HB_NO_HINTING
    स्थिती 1: स्थिती 2: स्थिती 3:
      hb_barrier ();
      परत u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      hb_barrier ();
      परत u.variation.get_x_delta (font, store, store_cache);
#endif
    पूर्वनिर्धारित:
      परत 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     स्थिरांक ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) स्थिरांक
  {
    निवड (u.b.format)
    {
    स्थिती 1: स्थिती 2: स्थिती 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      परत u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      hb_barrier ();
      परत u.variation.get_y_delta (font, store, store_cache);
#endif
    पूर्वनिर्धारित:
      परत 0;
    }
  }

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (!u.b.format.sanitize (c)) return_trace (असत्य);
    निवड (u.b.format) {
#ifndef HB_NO_HINTING
    स्थिती 1: स्थिती 2: स्थिती 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    पूर्वनिर्धारित:
      return_trace (सत्य);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map=nullptr) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    निवड (u.b.format) {
#ifndef HB_NO_HINTING
    स्थिती 1:
    स्थिती 2:
    स्थिती 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    पूर्वनिर्धारित:
      return_trace (nullptr);
    }
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    निवड (u.b.format) {
#ifndef HB_NO_HINTING
    स्थिती 1:
    स्थिती 2:
    स्थिती 3:
      परत;
#endif
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      परत;
#endif
    पूर्वनिर्धारित:
      परत;
    }
  }

  अचिन्हित get_variation_index () स्थिरांक
  {
    निवड (u.b.format) {
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      hb_barrier ();
      परत u.variation.varIdx;
#endif
    पूर्वनिर्धारित:
      परत HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  तार्किक is_variation_device () स्थिरांक
  {
    निवड (u.b.format) {
#ifndef HB_NO_VAR
    स्थिती 0x8000:
      परत सत्य;
#endif
    पूर्वनिर्धारित:
      परत असत्य;
    }
  }

  संरक्षित:
  संघ {
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

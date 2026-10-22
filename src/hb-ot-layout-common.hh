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

#अगर_अघोषित HB_OT_LAYOUT_COMMON_HH
#घोषणा HB_OT_LAYOUT_COMMON_HH

#समावेश "hb.hh"
#समावेश "hb-ot-layout.hh"
#समावेश "hb-open-type.hh"
#समावेश "hb-set.hh"
#समावेश "hb-bimap.hh"
#समावेश "hb-cache.hh"
#समावेश "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (माप (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_द्विआधारी_cache_t = hb_cache_t<14, 1, 8>;
static_assert (माप (hb_ot_layout_द्विआधारी_cache_t) == 256, "");

#समावेश "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


namespace OT {

ढाँचा<typename Iterator>
अटल अंतरभूत bool ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

अटल bool ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    स्थिर hb_set_t &klasses,
    bool use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

काष्ठा ItemVarStoreInstancer;

काष्ठा hb_collect_feature_substitutes_with_var_context_t
{
  स्थिर hb_map_t *axes_सूचकांक_tag_map;
  स्थिर hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<अचिन्हित, स्थिर Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  bool apply;
  bool variation_applied;
  bool universal;
  अचिन्हित cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, अचिन्हित> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  bool all_axes_pinned;
};

काष्ठा hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (स्थिर व्योम         *table_,
                              hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              स्थिर hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_गिनती (0),langsys_feature_गिनती (0) {}

  bool visitScript ()
  { वापस script_गिनती++ < HB_MAX_SCRIPTS; }

  bool visitLangsys (अचिन्हित feature_गिनती_2)
  {
    langsys_feature_गिनती += feature_गिनती_2;
    वापस langsys_feature_गिनती < HB_MAX_LANGSYS_FEATURE_गिनती;
  }

  खुला:
  स्थिर व्योम *table;
  hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  स्थिर hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  निजी:
  अचिन्हित script_गिनती;
  अचिन्हित langsys_feature_गिनती;
};

काष्ठा hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  स्थिर अक्षर *get_name () { वापस "SUBSET_LAYOUT"; }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  bool visitScript ()
  {
    वापस script_गिनती++ < HB_MAX_SCRIPTS;
  }

  bool visitLangSys ()
  {
    वापस langsys_गिनती++ < HB_MAX_LANGSYS;
  }

  bool visit_Feature_सूचकांक (पूर्णांक गिनती)
  {
    feature_सूचकांक_गिनती += गिनती;
    वापस feature_सूचकांक_गिनती < HB_MAX_FEATURE_INDICES;
  }

  bool visit_Lookup_सूचकांक()
  {
    lookup_सूचकांक_गिनती++;
    वापस lookup_सूचकांक_गिनती < HB_MAX_LOOKUP_VISIT_गिनती;
  }

  hb_subset_context_t *subset_context;
  स्थिर hb_tag_t table_tag;
  स्थिर hb_map_t *lookup_सूचकांक_map;
  स्थिर hb_hashmap_t<अचिन्हित, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  स्थिर hb_map_t *feature_सूचकांक_map;
  स्थिर hb_map_t *feature_map_w_duplicates;
  स्थिर hb_hashmap_t<अचिन्हित, स्थिर Feature*> *feature_substitutes_map;
  स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  स्थिर hb_set_t *catch_all_record_feature_idxes;
  स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<स्थिर व्योम*, स्थिर व्योम*>> *feature_idx_tag_map;

  अचिन्हित cur_script_सूचकांक;
  अचिन्हित cur_feature_सूचकांक;
  अचिन्हित cur_feature_var_record_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_tag_t tag_) :
				subset_context (c_),
				table_tag (tag_),
				cur_script_सूचकांक (0xFFFFu),
				cur_feature_सूचकांक (सूचकांक::NOT_FOUND_सूचकांक),
				cur_feature_var_record_idx (0u),
				script_गिनती (0),
				langsys_गिनती (0),
				feature_सूचकांक_गिनती (0),
				lookup_सूचकांक_गिनती (0)
  {
    अगर (tag_ == HB_OT_TAG_GSUB)
    {
      lookup_सूचकांक_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_सूचकांक_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_record_cond_idx_map = c_->plan->user_axes_location.is_empty () ? nullptr : &c_->plan->gsub_feature_record_cond_idx_map;
      catch_all_record_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_tag_map = &c_->plan->gsub_old_feature_idx_tag_map;
    }
    अन्यथा
    {
      lookup_सूचकांक_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_सूचकांक_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_record_cond_idx_map = c_->plan->user_axes_location.is_empty () ? nullptr : &c_->plan->gpos_feature_record_cond_idx_map;
      catch_all_record_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_tag_map = &c_->plan->gpos_old_feature_idx_tag_map;
    }
  }

  निजी:
  अचिन्हित script_गिनती;
  अचिन्हित langsys_गिनती;
  अचिन्हित feature_सूचकांक_गिनती;
  अचिन्हित lookup_सूचकांक_गिनती;
};

काष्ठा ItemVariationStore;
काष्ठा hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  ढाँचा <typename T>
  return_t dispatch (स्थिर T &obj) { obj.collect_variation_indices (यह); वापस hb_empty_t (); }
  अटल return_t default_return_मूल्य () { वापस hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  स्थिर hb_set_t *glyph_set;
  स्थिर hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>> *condition_मूल्यावली = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  स्थिर hb_set_t *glyph_set_,
					  स्थिर hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

ढाँचा<typename OutputArray>
काष्ठा subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 स्थिर व्योम *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  ढाँचा <typename T>
  bool चालक () (T&& offset)
  {
    स्वतः snap = subset_context->serializer->snapshot ();
    स्वतः *o = out.serialize_append (subset_context->serializer);
    अगर (unlikely (!o)) वापस false;
    bool ret = o->serialize_subset (subset_context, offset, base);
    अगर (!ret)
    {
      out.निकालो ();
      subset_context->serializer->revert (snap);
    }
    वापस ret;
  }

  निजी:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  स्थिर व्योम *base;
};


ढाँचा<typename OutputArray, typename Arg>
काष्ठा subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     स्थिर व्योम *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  ढाँचा <typename T>
  bool चालक () (T&& offset)
  {
    स्वतः snap = subset_context->serializer->snapshot ();
    स्वतः *o = out.serialize_append (subset_context->serializer);
    अगर (unlikely (!o)) वापस false;
    bool ret = o->serialize_subset (subset_context, offset, base, arg);
    अगर (!ret)
    {
      out.निकालो ();
      subset_context->serializer->revert (snap);
    }
    वापस ret;
  }

  निजी:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  स्थिर व्योम *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
काष्ठा
{
  ढाँचा<typename OutputArray>
  subset_offset_array_t<OutputArray>
  चालक () (hb_subset_context_t *subset_context, OutputArray& out,
	       स्थिर व्योम *base) स्थिर
  { वापस subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  ढाँचा<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  चालक () (hb_subset_context_t *subset_context, OutputArray& out,
	       स्थिर व्योम *base, Arg &&arg) स्थिर
  { वापस subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

ढाँचा<typename OutputArray>
काष्ठा subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 स्थिर व्योम *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  ढाँचा <typename T>
  व्योम
  चालक () (T&& record)
  {
    स्वतः snap = subset_layout_context->subset_context->serializer->snapshot ();
    bool ret = record.subset (subset_layout_context, base);
    अगर (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अन्यथा out->len++;
  }

  निजी:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  स्थिर व्योम *base;
};

ढाँचा<typename OutputArray, typename Arg>
काष्ठा subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     स्थिर व्योम *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  ढाँचा <typename T>
  व्योम
  चालक () (T&& record)
  {
    स्वतः snap = subset_layout_context->subset_context->serializer->snapshot ();
    bool ret = record.subset (subset_layout_context, base, arg);
    अगर (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    अन्यथा out->len++;
  }

  निजी:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  स्थिर व्योम *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
काष्ठा
{
  ढाँचा<typename OutputArray>
  subset_record_array_t<OutputArray>
  चालक () (hb_subset_layout_context_t *c, OutputArray* out,
	       स्थिर व्योम *base) स्थिर
  { वापस subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  ढाँचा<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  चालक () (hb_subset_layout_context_t *c, OutputArray* out,
               स्थिर व्योम *base, Arg &&arg) स्थिर
  { वापस subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


ढाँचा<typename OutputArray>
काष्ठा serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         स्थिर व्योम *base_,
                         स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  ढाँचा <typename T>
  bool चालक () (T&& record)
  {
    अगर (!serialize_context->copy (record, base, variation_map)) वापस false;
    out.len++;
    वापस true;
  }

  निजी:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  स्थिर व्योम *base;
  स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
काष्ठा
{
  ढाँचा<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  चालक () (hb_serialize_context_t *serialize_context, OutputArray& out,
               स्थिर व्योम *base,
               स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *variation_map) स्थिर
  { वापस serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

काष्ठा सूचकांक_Array : Array16Of<सूचकांक>
{
  bool intersects (स्थिर hb_map_t *indexes) स्थिर
  { वापस hb_any (*यह, indexes); }

  ढाँचा <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    अगर (!it) वापस;
    अगर (unlikely (!c->extend_min ((*यह)))) वापस;

    क्रम (स्थिर स्वतः _ : it)
    {
      अगर (!l->visit_Lookup_सूचकांक()) अवरोध;

      सूचकांक i;
      i = _;
      c->copy (i);
      यह->len++;
    }
  }

  अचिन्हित पूर्णांक get_indexes (अचिन्हित पूर्णांक start_offset,
			    अचिन्हित पूर्णांक *गिनती_2 /* IN/OUT */,
			    अचिन्हित पूर्णांक *_indexes /* OUT */) स्थिर
  {
    अगर (गिनती_2 && _indexes)
    {
      + यह->as_array ().sub_array (start_offset, गिनती_2)
      | hb_sink (hb_array (_indexes, *गिनती_2))
      ;
    }
    वापस यह->len;
  }

  व्योम add_indexes_to (hb_set_t* output /* OUT */) स्थिर
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
काष्ठा Feature_Params_आकार
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह))) return_trace (false);
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

    अगर (!design_आकार)
      return_trace (false);
    अन्यथा अगर (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (true);
    अन्यथा अगर (design_आकार < rangeStart ||
	     design_आकार > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (false);
    अन्यथा
      return_trace (true);
  }

  व्योम collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) स्थिर
  { nameids_to_retain->add (subfamilyNameID); }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    return_trace ((bool) c->serializer->embed (*यह));
  }

  HBUINT16	design_आकार;	/* Represents the design size in 720/inch
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
  खुला:
  DEFINE_आकार_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
काष्ठा FeatureParamsStylisticSet
{
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (यह));
  }

  व्योम collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) स्थिर
  { nameids_to_retain->add (uiNameID); }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    return_trace ((bool) c->serializer->embed (*यह));
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
  खुला:
  DEFINE_आकार_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
काष्ठा FeatureParamsCharacterVariants
{
  अचिन्हित
  get_characters (अचिन्हित start_offset, अचिन्हित *char_गिनती, hb_codepoint_t *chars) स्थिर
  {
    अगर (char_गिनती && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_गिनती)
      | hb_sink (hb_array (chars, *char_गिनती))
      ;
    }
    वापस characters.len;
  }

  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_आकार, min_आकार); }

  व्योम collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) स्थिर
  {
    अगर (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    अगर (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    अगर (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    अगर (!firstParamUILabelNameID || !अंक_Named_Parameters || अंक_Named_Parameters >= 0x7FFF)
      वापस;

    अचिन्हित last_name_id = (अचिन्हित) firstParamUILabelNameID + (अचिन्हित) अंक_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    return_trace ((bool) c->serializer->embed (*यह));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
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
  खुला:
  DEFINE_आकार_ARRAY (14, characters);
};

काष्ठा FeatureParams
{
  bool sanitize (hb_sanitize_context_t *c, hb_tag_t tag) स्थिर
  {
#अगर_घोषित HB_NO_LAYOUT_FEATURE_PARAMS
    वापस true;
#पूर्ण_अगर
    TRACE_SANITIZE (यह);
    अगर (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.आकार.sanitize (c));
    अगर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    अगर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (true);
  }

  व्योम collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) स्थिर
  {
#अगर_घोषित HB_NO_LAYOUT_FEATURE_PARAMS
    वापस;
#पूर्ण_अगर
    अगर (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      वापस (u.आकार.collect_name_ids (nameids_to_retain));
    अगर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      वापस (u.stylisticSet.collect_name_ids (nameids_to_retain));
    अगर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      वापस (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  bool subset (hb_subset_context_t *c, स्थिर Tag* tag) स्थिर
  {
#अगर_घोषित HB_NO_LAYOUT_FEATURE_PARAMS
    वापस false;
#पूर्ण_अगर
    TRACE_SUBSET (यह);
    अगर (!tag) return_trace (false);
    अगर (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.आकार.subset (c));
    अगर ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    अगर ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (false);
  }

#अगर_अघोषित HB_NO_LAYOUT_FEATURE_PARAMS
  स्थिर Feature_Params_आकार& get_आकार_params (hb_tag_t tag) स्थिर
  {
    अगर (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      वापस u.आकार;
    वापस Null (Feature_Params_आकार);
  }
  स्थिर FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) स्थिर
  {
    अगर ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      वापस u.stylisticSet;
    वापस Null (FeatureParamsStylisticSet);
  }
  स्थिर FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) स्थिर
  {
    अगर ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      वापस u.characterVariants;
    वापस Null (FeatureParamsCharacterVariants);
  }
#पूर्ण_अगर

  निजी:
  जोड़ {
  Feature_Params_आकार			आकार;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  खुला:
  DEFINE_आकार_MIN (0);
};

काष्ठा Record_sanitize_closure_t {
  hb_tag_t tag;
  स्थिर व्योम *list_base;
};

काष्ठा Feature
{
  अचिन्हित पूर्णांक get_lookup_गिनती () स्थिर
  { वापस lookup_सूचकांक_2.len; }
  hb_tag_t get_lookup_सूचकांक (अचिन्हित पूर्णांक i) स्थिर
  { वापस lookup_सूचकांक_2[i]; }
  अचिन्हित पूर्णांक get_lookup_indexes (अचिन्हित पूर्णांक start_सूचकांक,
				   अचिन्हित पूर्णांक *lookup_गिनती /* IN/OUT */,
				   अचिन्हित पूर्णांक *lookup_tags /* OUT */) स्थिर
  { वापस lookup_सूचकांक_2.get_indexes (start_सूचकांक, lookup_गिनती, lookup_tags); }
  व्योम add_lookup_indexes_to (hb_set_t *lookup_indexes) स्थिर
  { lookup_सूचकांक_2.add_indexes_to (lookup_indexes); }

  स्थिर FeatureParams &get_feature_params () स्थिर
  { वापस यह+featureParams; }

  bool intersects_lookup_indexes (स्थिर hb_map_t *lookup_indexes) स्थिर
  { वापस lookup_सूचकांक_2.intersects (lookup_indexes); }

  व्योम collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) स्थिर
  {
    अगर (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  bool subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       स्थिर Tag                   *tag = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->featureParams.serialize_subset (c, featureParams, यह, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    अगर (l->cur_feature_सूचकांक != सूचकांक::NOT_FOUND_सूचकांक &&
	l->lookup_variations->has (l->cur_feature_सूचकांक, &lookup_variations))
    {
      स्वतः it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_सूचकांक_map)
      | hb_map (l->lookup_सूचकांक_map)
      ;
      out->lookup_सूचकांक_2.serialize (c->serializer, l, it);
    }
    अन्यथा
    {
      स्वतः it =
      + hb_iter (lookup_सूचकांक_2)
      | hb_filter (l->lookup_सूचकांक_map)
      | hb_map (l->lookup_सूचकांक_map)
      ;
      out->lookup_सूचकांक_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर Record_sanitize_closure_t *closure = nullptr) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  featureParams.sanitize (c, यह, closure ? closure->tag : HB_TAG_NONE) &&
		  lookup_सूचकांक_2.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  सूचकांक_Array	 lookup_सूचकांक_2;	/* Array of LookupList indices */
  खुला:
  DEFINE_आकार_ARRAY_SIZED (4, lookup_सूचकांक_2);
};

ढाँचा <typename Type>
काष्ठा Record
{
  पूर्णांक cmp (hb_tag_t a) स्थिर { वापस tag.cmp (a); }

  bool subset (hb_subset_layout_context_t *c, स्थिर व्योम *base, स्थिर व्योम *f_sub = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->subset_context->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    स्थिर Feature& f = *reinterpret_cast<स्थिर Feature *> (f_sub);
    स्वतः *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    bool ret = f.subset (c->subset_context, c, &tag);
    अगर (ret)
      s->add_link (out->offset, s->निकालो_pack ());
    अन्यथा
      s->निकालो_discard ();

    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    स्थिर Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (यह) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  खुला:
  DEFINE_आकार_STATIC (6);
};

ढाँचा <typename Type>
काष्ठा RecordArrayOf : SortedArray16Of<Record<Type>>
{
  स्थिर Offset16To<Type>& get_offset (अचिन्हित पूर्णांक i) स्थिर
  { वापस (*यह)[i].offset; }
  Offset16To<Type>& get_offset (अचिन्हित पूर्णांक i)
  { वापस (*यह)[i].offset; }
  स्थिर Tag& get_tag (अचिन्हित पूर्णांक i) स्थिर
  { वापस (*यह)[i].tag; }
  अचिन्हित पूर्णांक get_tags (अचिन्हित पूर्णांक start_offset,
                         अचिन्हित पूर्णांक *record_गिनती /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) स्थिर
  {
    अगर (record_गिनती && record_tags)
    {
      + यह->as_array ().sub_array (start_offset, record_गिनती)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_गिनती))
      ;
    }
    वापस यह->len;
  }
  bool find_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  {
    वापस यह->bfind (tag, सूचकांक_2, HB_NOT_FOUND_STORE, सूचकांक::NOT_FOUND_सूचकांक);
  }
};

ढाँचा <typename Type>
काष्ठा RecordListOf : RecordArrayOf<Type>
{
  स्थिर Type& चालक [] (अचिन्हित पूर्णांक i) स्थिर
  { वापस यह+यह->get_offset (i); }

  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + यह->iter ()
    | hb_apply (subset_record_array (l, out, यह))
    ;
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (RecordArrayOf<Type>::sanitize (c, यह));
  }
};

काष्ठा RecordListOfFeature : RecordListOf<Feature>
{
  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_enumerate (*यह)
    | hb_filter (l->feature_सूचकांक_map, hb_first)
    | hb_apply ([l, out, यह] (स्थिर hb_pair_t<अचिन्हित, स्थिर Record<Feature>&>& _)
                {
                  स्थिर Feature *f_sub = nullptr;
                  स्थिर Feature **f = nullptr;
                  अगर (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_सूचकांक = _.first;
                  subset_record_array (l, out, यह, f_sub) (_.second);
                  l->cur_feature_सूचकांक = सूचकांक::NOT_FOUND_सूचकांक;
                })
    ;

    return_trace (true);
  }
};

प्रकार RecordListOf<Feature> FeatureList;


काष्ठा LangSys
{
  अचिन्हित पूर्णांक get_feature_गिनती () स्थिर
  { वापस feature_सूचकांक_2.len; }
  hb_tag_t get_feature_सूचकांक (अचिन्हित पूर्णांक i) स्थिर
  { वापस feature_सूचकांक_2[i]; }
  अचिन्हित पूर्णांक get_feature_indexes (अचिन्हित पूर्णांक start_offset,
				    अचिन्हित पूर्णांक *feature_गिनती_2 /* IN/OUT */,
				    अचिन्हित पूर्णांक *feature_indexes /* OUT */) स्थिर
  { वापस feature_सूचकांक_2.get_indexes (start_offset, feature_गिनती_2, feature_indexes); }
  व्योम add_feature_indexes_to (hb_set_t *feature_indexes) स्थिर
  { feature_सूचकांक_2.add_indexes_to (feature_indexes); }

  bool has_required_feature () स्थिर { वापस req_Feature_सूचकांक != 0xFFFFu; }
  अचिन्हित पूर्णांक get_required_feature_सूचकांक () स्थिर
  {
    अगर (req_Feature_सूचकांक == 0xFFFFu)
      वापस सूचकांक::NOT_FOUND_सूचकांक;
   वापस req_Feature_सूचकांक;
  }

  LangSys* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (*यह));
  }

  bool compare (स्थिर LangSys& o, स्थिर hb_map_t *feature_सूचकांक_map) स्थिर
  {
    अगर (req_Feature_सूचकांक != o.req_Feature_सूचकांक)
      वापस false;

    स्वतः iter =
    + hb_iter (feature_सूचकांक_2)
    | hb_filter (feature_सूचकांक_map)
    | hb_map (feature_सूचकांक_map)
    ;

    स्वतः o_iter =
    + hb_iter (o.feature_सूचकांक_2)
    | hb_filter (feature_सूचकांक_map)
    | hb_map (feature_सूचकांक_map)
    ;

    क्रम (; iter && o_iter; iter++, o_iter++)
    {
      अचिन्हित a = *iter;
      अचिन्हित b = *o_iter;
      अगर (a != b) वापस false;
    }

    अगर (iter || o_iter) वापस false;

    वापस true;
  }

  व्योम collect_features (hb_prune_langsys_context_t *c) स्थिर
  {
    अगर (!has_required_feature () && !get_feature_गिनती ()) वापस;
    अगर (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_सूचकांक))
      c->new_feature_indexes->add (get_required_feature_सूचकांक ());

    + hb_iter (feature_सूचकांक_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       स्थिर Tag                  *tag = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    स्थिर uint32_t *v;
    out->req_Feature_सूचकांक = l->feature_map_w_duplicates->has (req_Feature_सूचकांक, &v) ? *v : 0xFFFFu;

    अगर (!l->visit_Feature_सूचकांक (feature_सूचकांक_2.len))
      return_trace (false);

    स्वतः it =
    + hb_iter (feature_सूचकांक_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    bool ret = bool (it);
    out->feature_सूचकांक_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर Record_sanitize_closure_t * = nullptr) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && feature_सूचकांक_2.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_सूचकांक;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  सूचकांक_Array	feature_सूचकांक_2;	/* Array of indices into the FeatureList */
  खुला:
  DEFINE_आकार_ARRAY_SIZED (6, feature_सूचकांक_2);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

काष्ठा Script
{
  अचिन्हित पूर्णांक get_lang_sys_गिनती () स्थिर
  { वापस langSys.len; }
  स्थिर Tag& get_lang_sys_tag (अचिन्हित पूर्णांक i) स्थिर
  { वापस langSys.get_tag (i); }
  अचिन्हित पूर्णांक get_lang_sys_tags (अचिन्हित पूर्णांक start_offset,
				  अचिन्हित पूर्णांक *lang_sys_गिनती /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) स्थिर
  { वापस langSys.get_tags (start_offset, lang_sys_गिनती, lang_sys_tags); }
  स्थिर LangSys& get_lang_sys (अचिन्हित पूर्णांक i) स्थिर
  {
    अगर (i == सूचकांक::NOT_FOUND_सूचकांक) वापस get_default_lang_sys ();
    वापस यह+langSys[i].offset;
  }
  bool find_lang_sys_सूचकांक (hb_tag_t tag, अचिन्हित पूर्णांक *सूचकांक_2) स्थिर
  { वापस langSys.find_सूचकांक (tag, सूचकांक_2); }

  bool has_default_lang_sys () स्थिर           { वापस defaultLangSys != 0; }
  स्थिर LangSys& get_default_lang_sys () स्थिर { वापस यह+defaultLangSys; }

  व्योम prune_langsys (hb_prune_langsys_context_t *c,
                      अचिन्हित script_सूचकांक) स्थिर
  {
    अगर (!has_default_lang_sys () && !get_lang_sys_गिनती ()) वापस;
    अगर (!c->visitScript ()) वापस;

    अगर (!c->script_langsys_map->has (script_सूचकांक))
    {
      अगर (unlikely (!c->script_langsys_map->set (script_सूचकांक, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	वापस;
    }

    अगर (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      स्थिर LangSys& d = get_default_lang_sys ();
      अगर (c->visitLangsys (d.get_feature_गिनती ())) {
        d.collect_features (c);
      }

      क्रम (स्वतः _ : + hb_enumerate (langSys))
      {
        स्थिर LangSys& l = यह+_.second.offset;
        अगर (!c->visitLangsys (l.get_feature_गिनती ())) जारी;
        अगर (l.compare (d, c->duplicate_feature_map)) जारी;

        l.collect_features (c);
        c->script_langsys_map->लो (script_सूचकांक)->add (_.first);
      }
    }
    अन्यथा
    {
      क्रम (स्वतः _ : + hb_enumerate (langSys))
      {
        स्थिर LangSys& l = यह+_.second.offset;
        अगर (!c->visitLangsys (l.get_feature_गिनती ())) जारी;
        l.collect_features (c);
        c->script_langsys_map->लो (script_सूचकांक)->add (_.first);
      }
    }
  }

  bool subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       स्थिर Tag                   *tag) स्थिर
  {
    TRACE_SUBSET (यह);
    अगर (!l->visitScript ()) return_trace (false);
    अगर (tag && !c->plan->layout_scripts.has (*tag))
      वापस false;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    bool defaultLang = false;
    अगर (has_default_lang_sys ())
    {
      c->serializer->push ();
      स्थिर LangSys& ls = यह+defaultLangSys;
      bool ret = ls.subset (c, l);
      अगर (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->निकालो_discard ();
	out->defaultLangSys = 0;
      }
      अन्यथा
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->निकालो_pack ());
	defaultLang = true;
      }
    }

    स्थिर hb_set_t *active_langsys = l->script_langsys_map->लो (l->cur_script_सूचकांक);
    अगर (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (स्थिर Record<LangSys>& record) {वापस l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), यह))
      ;
    }

    return_trace (bool (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 स्थिर Record_sanitize_closure_t * = nullptr) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (defaultLangSys.sanitize (c, यह) && langSys.sanitize (c, यह));
  }

  रक्षित:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  खुला:
  DEFINE_आकार_ARRAY_SIZED (4, langSys);
};

काष्ठा RecordListOfScript : RecordListOf<Script>
{
  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    क्रम (स्वतः _ : + hb_enumerate (*यह))
    {
      स्वतः snap = c->serializer->snapshot ();
      l->cur_script_सूचकांक = _.first;
      bool ret = _.second.subset (l, यह);
      अगर (!ret) c->serializer->revert (snap);
      अन्यथा out->len++;
    }

    return_trace (true);
  }
};

प्रकार RecordListOfScript ScriptList;



काष्ठा LookupFlag : HBUINT16
{
  क्रमागत Flags {
    दायाँ_To_बायाँ		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  खुला:
  DEFINE_आकार_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
namespace OT {

काष्ठा Lookup
{
  अचिन्हित पूर्णांक get_subtable_गिनती () स्थिर { वापस subTable.len; }

  ढाँचा <typename TSubTable>
  स्थिर Array16OfOffset16To<TSubTable>& get_subtables () स्थिर
  { वापस reinterpret_cast<स्थिर Array16OfOffset16To<TSubTable> &> (subTable); }
  ढाँचा <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { वापस reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  ढाँचा <typename TSubTable>
  स्थिर TSubTable& get_subtable (अचिन्हित पूर्णांक i) स्थिर
  { वापस यह+get_subtables<TSubTable> ()[i]; }
  ढाँचा <typename TSubTable>
  TSubTable& get_subtable (अचिन्हित पूर्णांक i)
  { वापस यह+get_subtables<TSubTable> ()[i]; }

  माप_प्रकार get_आकार () स्थिर
  {
    स्थिर HBUINT16 &markFilteringSet = StructAfter<स्थिर HBUINT16> (subTable);
    अगर (lookupFlag & LookupFlag::UseMarkFilteringSet)
      वापस (स्थिर अक्षर *) &StructAfter<स्थिर अक्षर> (markFilteringSet) - (स्थिर अक्षर *) यह;
    वापस (स्थिर अक्षर *) &markFilteringSet - (स्थिर अक्षर *) यह;
  }

  अचिन्हित पूर्णांक get_type () स्थिर { वापस lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () स्थिर
  {
    अचिन्हित पूर्णांक flag = lookupFlag;
    अगर (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      स्थिर HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    वापस flag;
  }

  ढाँचा <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अचिन्हित पूर्णांक lookup_type = get_type ();
    TRACE_DISPATCH (यह, lookup_type);
    अचिन्हित पूर्णांक गिनती = get_subtable_गिनती ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      अगर (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_मूल्य ());
  }

  bool serialize (hb_serialize_context_t *c,
		  अचिन्हित पूर्णांक lookup_type,
		  uint32_t lookup_props,
		  अचिन्हित पूर्णांक अंक_subtables)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    अगर (unlikely (!subTable.serialize (c, अंक_subtables))) return_trace (false);
    अगर (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      अगर (unlikely (!c->extend (यह))) return_trace (false);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (true);
  }

  ढाँचा <typename TSubTable>
  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    स्थिर hb_set_t *glyphset = c->plan->glyphset_gsub ();
    अचिन्हित पूर्णांक lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([यह, glyphset, lookup_type] (स्थिर Offset16To<TSubTable> &_) { वापस (यह+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), यह, lookup_type))
    ;

    अगर (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      स्थिर HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      अगर (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
        अगर (unlikely (!c->serializer->extend (out))) return_trace (false);
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
    return_trace (true);
  }

  ढाँचा <typename TSubTable>
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!(c->check_struct (यह) && subTable.sanitize (c))) return_trace (false);
    hb_barrier ();

    अचिन्हित subtables = get_subtable_गिनती ();
    अगर (unlikely (!c->visit_subtables (subtables))) return_trace (false);

    अगर (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      स्थिर HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      अगर (!markFilteringSet.sanitize (c)) return_trace (false);
    }

    अगर (unlikely (!get_subtables<TSubTable> ().sanitize (c, यह, get_type ())))
      return_trace (false);

    अगर (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      अचिन्हित पूर्णांक type = get_subtable<TSubTable> (0).u.extension.get_type ();
      क्रम (अचिन्हित पूर्णांक i = 1; i < subtables; i++)
	अगर (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (false);
    }
    return_trace (true);
  }

  रक्षित:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  खुला:
  DEFINE_आकार_ARRAY (6, subTable);
};

ढाँचा <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

ढाँचा <typename TLookup, typename OffsetType>
काष्ठा LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_enumerate (*यह)
    | hb_filter (l->lookup_सूचकांक_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, यह))
    ;
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, यह)));
  }
};


/*
 * Coverage Table
 */


अटल bool ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  स्थिर hb_set_t &klasses,
                                          bool use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  अगर (!klass_map)
    वापस ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  अगर (!use_class_zero)
    klass_map->set (0, 0);

  अचिन्हित idx = klass_map->has (0) ? 1 : 0;
  क्रम (स्थिर अचिन्हित k: klasses)
  {
    अगर (klass_map->has (k)) जारी;
    klass_map->set (k, idx);
    idx++;
  }


  अगर (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    वापस false;

  क्रम (अचिन्हित i = 0; i < glyph_and_klass.लंबाई; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->लो (klass);
  }

  वापस ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

ढाँचा <typename Types>
काष्ठा ClassDefFormat1_3
{
  मित्र काष्ठा ClassDef;

  निजी:
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id) स्थिर
  {
    वापस class_मूल्य[(अचिन्हित पूर्णांक) (glyph_id - startGlyph)];
  }

  अचिन्हित get_population () स्थिर
  {
    वापस class_मूल्य.len;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    अगर (unlikely (!it))
    {
      classFormat = Types::आकार == 2 ? 1 : 3;
      startGlyph = 0;
      class_मूल्य.len = 0;
      return_trace (true);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    अचिन्हित glyph_गिनती = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    अगर (unlikely (!class_मूल्य.serialize (c, glyph_गिनती))) return_trace (false);
    क्रम (स्थिर hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      अचिन्हित idx = gid_klass_pair.first - glyph_min;
      class_मूल्य[idx] = gid_klass_pair.second;
    }
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               bool keep_empty_table = true,
               bool use_class_zero = true,
               स्थिर Coverage* glyph_filter = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_मूल्य.len;

    क्रम (स्थिर hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      अगर (new_gid == HB_MAP_मूल्य_INVALID) जारी;
      अगर (glyph_filter && !glyph_filter->has(gid)) जारी;

      अचिन्हित klass = class_मूल्य[gid - start];
      अगर (!klass) जारी;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    अगर (use_class_zero)
    {
      स्थिर hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      अचिन्हित glyph_गिनती = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_गिनती <= glyph_and_klass.लंबाई;
    }
    अगर (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (false);
    return_trace (keep_empty_table || (bool) glyph_and_klass);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && class_मूल्य.sanitize (c));
  }

  अचिन्हित cost () स्थिर { वापस 1; }

  ढाँचा <typename set_t>
  bool collect_coverage (set_t *glyphs) स्थिर
  {
    अचिन्हित पूर्णांक start = 0;
    अचिन्हित पूर्णांक गिनती = class_मूल्य.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (class_मूल्य[i])
	जारी;

      अगर (start != i)
	अगर (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  वापस false;

      start = i + 1;
    }
    अगर (start != गिनती)
      अगर (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + गिनती)))
	वापस false;

    वापस true;
  }

  ढाँचा <typename set_t>
  bool collect_class (set_t *glyphs, अचिन्हित klass) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = class_मूल्य.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (class_मूल्य[i] == klass) glyphs->add (startGlyph + i);
    वापस true;
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_मूल्य.len;
    क्रम (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      अगर (class_मूल्य[iter - start]) वापस true;
    वापस false;
  }
  bool intersects_class (स्थिर hb_set_t *glyphs, अचिन्हित klass) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = class_मूल्य.len;
    अगर (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मूल्य_INVALID;
      अगर (!glyphs->next (&g)) वापस false;
      अगर (g < startGlyph) वापस true;
      g = startGlyph + गिनती - 1;
      अगर (glyphs->next (&g)) वापस true;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    स्थिर typename Types::HBUINT *arr = class_मूल्य.arrayZ;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (arr[i] == klass && glyphs->has (startGlyph + i))
	वापस true;
    वापस false;
  }

  व्योम intersected_class_glyphs (स्थिर hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) स्थिर
  {
    अचिन्हित गिनती = class_मूल्य.len;
    अगर (klass == 0)
    {
      अचिन्हित start_glyph = startGlyph;
      क्रम (uint32_t g = HB_SET_मूल्य_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      क्रम (uint32_t g = startGlyph + गिनती - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      वापस;
    }

    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      अगर (class_मूल्य[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#अगर 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    अचिन्हित start_glyph = startGlyph;
    अचिन्हित end_glyph = start_glyph + गिनती;
    क्रम (अचिन्हित g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      अगर (class_मूल्य.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#पूर्ण_अगर
  }

  व्योम intersected_classes (स्थिर hb_set_t *glyphs, hb_set_t *intersect_classes) स्थिर
  {
    अगर (glyphs->is_empty ()) वापस;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    अगर (!class_मूल्य.len)
    {
      intersect_classes->add (0);
      वापस;
    }

    hb_codepoint_t end_glyph = startGlyph + class_मूल्य.len - 1;
    अगर (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    क्रम (स्थिर स्वतः& _ : + hb_enumerate (class_मूल्य))
    {
      hb_codepoint_t g = startGlyph + _.first;
      अगर (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  रक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::ढाँचा ArrayOf<typename Types::HBUINT>
		class_मूल्य;	/* Array of Class Values--one per GlyphID */
  खुला:
  DEFINE_आकार_ARRAY (2 + 2 * Types::आकार, class_मूल्य);
};

ढाँचा <typename Types>
काष्ठा ClassDefFormat2_4
{
  मित्र काष्ठा ClassDef;

  निजी:
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id) स्थिर
  {
    वापस rangeRecord.द्वा_खोज (glyph_id).value;
  }

  अचिन्हित get_population () स्थिर
  {
    typename Types::large_int ret = 0;
    क्रम (स्थिर स्वतः &r : rangeRecord)
      ret += r.get_population ();
    वापस ret > अच_पूर्णांक_उच्च ? अच_पूर्णांक_उच्च : (अचिन्हित) ret;
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    अगर (unlikely (!it))
    {
      classFormat = Types::आकार == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (true);
    }

    अचिन्हित unsorted = false;
    अचिन्हित अंक_ranges = 1;
    hb_codepoint_t पूर्व_gid = (*it).first;
    अचिन्हित पूर्व_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = पूर्व_gid;
    range_rec.last = पूर्व_gid;
    range_rec.value = पूर्व_klass;

    स्वतः *record = c->copy (range_rec);
    अगर (unlikely (!record)) return_trace (false);

    क्रम (स्थिर स्वतः gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      अचिन्हित cur_klass = gid_klass_pair.second;

      अगर (cur_gid != पूर्व_gid + 1 ||
	  cur_klass != पूर्व_klass)
      {

	अगर (unlikely (cur_gid < पूर्व_gid))
	  unsorted = true;

	अगर (unlikely (!record)) अवरोध;
	record->last = पूर्व_gid;
	अंक_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      पूर्व_klass = cur_klass;
      पूर्व_gid = cur_gid;
    }

    अगर (unlikely (c->in_error ())) return_trace (false);

    अगर (likely (record)) record->last = पूर्व_gid;
    rangeRecord.len = अंक_ranges;

    अगर (unlikely (unsorted))
      rangeRecord.as_array ().क्विक (RangeRecord<Types>::cmp_range);

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               bool keep_empty_table = true,
               bool use_class_zero = true,
               स्थिर Coverage* glyph_filter = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    अगर (glyph_set.get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len)
	< get_population ())
    {
      क्रम (hb_codepoint_t g : glyph_set)
      {
	अचिन्हित klass = get_class (g);
	अगर (!klass) जारी;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	अगर (new_gid == HB_MAP_मूल्य_INVALID) जारी;
	अगर (glyph_filter && !glyph_filter->has (g)) जारी;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    अन्यथा
    {
      अचिन्हित अंक_source_glyphs = c->plan->source->get_अंक_glyphs ();
      क्रम (स्वतः &range : rangeRecord)
      {
	अचिन्हित klass = range.value;
	अगर (!klass) जारी;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, अंक_source_glyphs);
	क्रम (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  अगर (new_gid == HB_MAP_मूल्य_INVALID) जारी;
	  अगर (glyph_filter && !glyph_filter->has (g)) जारी;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    स्थिर hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    अचिन्हित glyph_गिनती = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_गिनती <= glyph_and_klass.लंबाई;
    अगर (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (false);
    return_trace (keep_empty_table || (bool) glyph_and_klass);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (rangeRecord.sanitize (c));
  }

  अचिन्हित cost () स्थिर { वापस hb_bit_storage ((अचिन्हित) rangeRecord.len); /* bsearch cost */ }

  ढाँचा <typename set_t>
  bool collect_coverage (set_t *glyphs) स्थिर
  {
    क्रम (स्वतः &range : rangeRecord)
      अगर (range.value)
	अगर (unlikely (!range.collect_coverage (glyphs)))
	  वापस false;
    वापस true;
  }

  ढाँचा <typename set_t>
  bool collect_class (set_t *glyphs, अचिन्हित पूर्णांक klass) स्थिर
  {
    क्रम (स्वतः &range : rangeRecord)
    {
      अगर (range.value == klass)
	अगर (unlikely (!range.collect_coverage (glyphs)))
	  वापस false;
    }
    वापस true;
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    अगर (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((अचिन्हित) rangeRecord.len))
    {
      क्रम (स्वतः g : *glyphs)
        अगर (get_class (g))
	  वापस true;
      वापस false;
    }

    वापस hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (स्थिर RangeRecord<Types> &range) { वापस range.intersects (*glyphs) && range.value; }));
  }
  bool intersects_class (स्थिर hb_set_t *glyphs, अचिन्हित klass) स्थिर
  {
    अगर (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      अगर (!rangeRecord.len)
        वापस !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_मूल्य_INVALID;
      hb_codepoint_t last = HB_SET_मूल्य_INVALID;
      स्वतः it = hb_iter (rangeRecord);
      क्रम (स्वतः &range : it)
      {
        अगर (it->first == last + 1)
	{
	  it++;
	  जारी;
	}

	अगर (!glyphs->next (&g))
	  अवरोध;
	अगर (g < range.first)
	  वापस true;
	g = range.last;
	last = g;
      }
      अगर (g != HB_SET_मूल्य_INVALID && glyphs->next (&g))
	वापस true;
      /* Fall through. */
    }
    क्रम (स्थिर स्वतः &range : rangeRecord)
      अगर (range.value == klass && range.intersects (*glyphs))
	वापस true;
    वापस false;
  }

  व्योम intersected_class_glyphs (स्थिर hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) स्थिर
  {
    अगर (klass == 0)
    {
      hb_codepoint_t g = HB_SET_मूल्य_INVALID;
      क्रम (स्वतः &range : rangeRecord)
      {
	अगर (!glyphs->next (&g))
	  जाओ done;
	जबतक (g < range.first)
	{
	  intersect_glyphs->add (g);
	  अगर (!glyphs->next (&g))
	    जाओ done;
        }
        g = range.last;
      }
      जबतक (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      वापस;
    }

    अचिन्हित गिनती = rangeRecord.len;
    अगर (गिनती > glyphs->get_population () * hb_bit_storage (गिनती))
    {
      क्रम (स्वतः g : *glyphs)
      {
        अचिन्हित i;
        अगर (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      वापस;
    }

    क्रम (स्वतः &range : rangeRecord)
    {
      अगर (range.value != klass) जारी;

      अचिन्हित end = range.last + 1;
      क्रम (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  व्योम intersected_classes (स्थिर hb_set_t *glyphs, hb_set_t *intersect_classes) स्थिर
  {
    अगर (glyphs->is_empty ()) वापस;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    अगर (!rangeRecord.len)
    {
      intersect_classes->add (0);
      वापस;
    }

    hb_codepoint_t g = HB_SET_मूल्य_INVALID;
    क्रम (स्वतः &range : rangeRecord)
    {
      अगर (!glyphs->next (&g))
        अवरोध;
      अगर (g < range.first)
      {
        intersect_classes->add (0);
        अवरोध;
      }
      g = range.last;
    }
    अगर (g != HB_SET_मूल्य_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    क्रम (स्थिर स्वतः& range : rangeRecord)
      अगर (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  रक्षित:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::ढाँचा SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::आकार, rangeRecord);
};

काष्ठा ClassDef
{
  /* Has interface. */
  अचिन्हित चालक [] (hb_codepoint_t k) स्थिर { वापस लो (k); }
  bool has (hb_codepoint_t k) स्थिर { वापस (*यह)[k]; }
  /* Projection. */
  hb_codepoint_t चालक () (hb_codepoint_t k) स्थिर { वापस लो (k); }

  अचिन्हित पूर्णांक लो (hb_codepoint_t k) स्थिर { वापस get_class (k); }
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_class (glyph_id);
    हाल 2: hb_barrier (); वापस u.format2.get_class (glyph_id);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.get_class (glyph_id);
    हाल 4: hb_barrier (); वापस u.format4.get_class (glyph_id);
#पूर्ण_अगर
    शेष:वापस 0;
    }
  }
  अचिन्हित पूर्णांक get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) स्थिर
  {
    अचिन्हित klass;
    अगर (cache && cache->लो (glyph_id, &klass)) वापस klass;
    klass = get_class (glyph_id);
    अगर (cache) cache->set (glyph_id, klass);
    वापस klass;
  }

  अचिन्हित get_population () स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.get_population ();
    हाल 2: hb_barrier (); वापस u.format2.get_population ();
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.get_population ();
    हाल 4: hb_barrier (); वापस u.format4.get_population ();
#पूर्ण_अगर
    शेष:वापस NOT_COVERED;
    }
  }

  ढाँचा<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    स्वतः glyphs = + it_with_class_zero | hb_filter (hb_second);

    अचिन्हित format = 2;
    hb_codepoint_t glyph_max = 0;
    अचिन्हित class_max = 0;
    अगर (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      अचिन्हित अंक_ranges = 1;
      hb_codepoint_t पूर्व_gid = glyph_min;
      अचिन्हित पूर्व_klass = (*glyphs).second;

      क्रम (स्थिर स्वतः gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	अचिन्हित cur_klass = gid_klass_pair.second;
	अगर (cur_klass > class_max) class_max = cur_klass;
	अगर (cur_gid == glyph_min) जारी;
        अगर (cur_gid > glyph_max) glyph_max = cur_gid;
	अगर (cur_gid != पूर्व_gid + 1 ||
	    cur_klass != पूर्व_klass)
	  अंक_ranges++;

	पूर्व_gid = cur_gid;
	पूर्व_klass = cur_klass;
      }

      uint64_t best_आकार = UINT64_MAX;
      अचिन्हित glyph_span = glyph_max - glyph_min + 1;
      स्वतः consider = [&] (अचिन्हित candidate, uint64_t आकार)
      {
	अगर (आकार >= best_आकार) वापस;
	best_आकार = आकार;
	format = candidate;
      };

      अगर (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_आकार * glyph_span);
      अगर (glyph_max <= 0xFFFFu && अंक_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) अंक_ranges *
		     (2 * HBGlyphID16::static_आकार + HBUINT16::static_आकार));
#अगर_अघोषित HB_NO_BEYOND_64K
      अगर (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_आकार * glyph_span);
      अगर (glyph_max <= 0xFFFFFFu && अंक_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) अंक_ranges *
		     (2 * HBGlyphID24::static_आकार + HBUINT16::static_आकार));
#पूर्ण_अगर
      अगर (unlikely (best_आकार == UINT64_MAX))
      {
	c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (false);
      }
    }

    u.format.v = format;

    चयन (u.format.v)
    {
    हाल 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    हाल 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    हाल 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#पूर्ण_अगर
    शेष:return_trace (false);
    }
  }

  bool subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               bool keep_empty_table = true,
               bool use_class_zero = true,
               स्थिर Coverage* glyph_filter = nullptr) स्थिर
  {
    TRACE_SUBSET (यह);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    हाल 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    हाल 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#पूर्ण_अगर
    शेष:return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    हाल 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    हाल 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#पूर्ण_अगर
    शेष:return_trace (true);
    }
  }

  अचिन्हित cost () स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.cost ();
    हाल 2: hb_barrier (); वापस u.format2.cost ();
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.cost ();
    हाल 4: hb_barrier (); वापस u.format4.cost ();
#पूर्ण_अगर
    शेष:वापस 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  bool collect_coverage (set_t *glyphs) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.collect_coverage (glyphs);
    हाल 2: hb_barrier (); वापस u.format2.collect_coverage (glyphs);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.collect_coverage (glyphs);
    हाल 4: hb_barrier (); वापस u.format4.collect_coverage (glyphs);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename set_t>
  bool collect_class (set_t *glyphs, अचिन्हित पूर्णांक klass) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.collect_class (glyphs, klass);
    हाल 2: hb_barrier (); वापस u.format2.collect_class (glyphs, klass);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.collect_class (glyphs, klass);
    हाल 4: hb_barrier (); वापस u.format4.collect_class (glyphs, klass);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.intersects (glyphs);
    हाल 2: hb_barrier (); वापस u.format2.intersects (glyphs);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersects (glyphs);
    हाल 4: hb_barrier (); वापस u.format4.intersects (glyphs);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }
  bool intersects_class (स्थिर hb_set_t *glyphs, अचिन्हित पूर्णांक klass) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.intersects_class (glyphs, klass);
    हाल 2: hb_barrier (); वापस u.format2.intersects_class (glyphs, klass);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersects_class (glyphs, klass);
    हाल 4: hb_barrier (); वापस u.format4.intersects_class (glyphs, klass);
#पूर्ण_अगर
    शेष:वापस false;
    }
  }

  व्योम intersected_class_glyphs (स्थिर hb_set_t *glyphs, अचिन्हित klass, hb_set_t *intersect_glyphs) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    हाल 2: hb_barrier (); वापस u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    हाल 4: hb_barrier (); वापस u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#पूर्ण_अगर
    शेष:वापस;
    }
  }

  व्योम intersected_classes (स्थिर hb_set_t *glyphs, hb_set_t *intersect_classes) स्थिर
  {
    चयन (u.format.v) {
    हाल 1: hb_barrier (); वापस u.format1.intersected_classes (glyphs, intersect_classes);
    हाल 2: hb_barrier (); वापस u.format2.intersected_classes (glyphs, intersect_classes);
#अगर_अघोषित HB_NO_BEYOND_64K
    हाल 3: hb_barrier (); वापस u.format3.intersected_classes (glyphs, intersect_classes);
    हाल 4: hb_barrier (); वापस u.format4.intersected_classes (glyphs, intersect_classes);
#पूर्ण_अगर
    शेष:वापस;
    }
  }


  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#अगर_अघोषित HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};

ढाँचा<typename Iterator>
अटल अंतरभूत bool ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ वापस (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
काष्ठा delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  काष्ठा chars_t : hb_vector_t<uint8_t>
  {
    पूर्णांक cmp (स्थिर chars_t& other) स्थिर
    {
      वापस as_array ().cmp (other.as_array ());
    }

    hb_pair_t<अचिन्हित, अचिन्हित> get_width ()
    {
      अचिन्हित width = 0;
      अचिन्हित columns = 0;
      क्रम (अचिन्हित i = 0; i < लंबाई; i++)
      {
	अचिन्हित v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      वापस hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<अचिन्हित, अचिन्हित> combine_width (स्थिर chars_t& other) स्थिर
    {
      अचिन्हित combined_width = 0;
      अचिन्हित combined_columns = 0;
      क्रम (अचिन्हित i = 0; i < लंबाई; i++)
      {
        अचिन्हित v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      वापस hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<अचिन्हित, अचिन्हित> combine_width (स्थिर delta_row_encoding_t& other_encoding) स्थिर { वापस chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  अचिन्हित width = 0;
  अचिन्हित overhead = 0;
  hb_vector_t<स्थिर hb_vector_t<पूर्णांक>*> वस्तुएँ;

  delta_row_encoding_t () = शेष;
  delta_row_encoding_t (hb_vector_t<स्थिर hb_vector_t<पूर्णांक>*> &&rows, अचिन्हित अंक_cols)
  {
    निश्चित (rows);

    वस्तुएँ = std::move (rows);

    अगर (unlikely (!chars.resize (अंक_cols)))
      वापस;

    calculate_chars ();
  }

  व्योम merge (स्थिर delta_row_encoding_t& other)
  {
    वस्तुएँ.alloc (वस्तुएँ.लंबाई + other.वस्तुएँ.लंबाई);
    क्रम (स्वतः &row : other.वस्तुएँ)
      add_row (row);

    // Merge chars
    निश्चित (chars.लंबाई == other.chars.लंबाई);
    क्रम (अचिन्हित i = 0; i < chars.लंबाई; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  व्योम chars_changed ()
  {
    स्वतः _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  व्योम calculate_chars ()
  {
    निश्चित (वस्तुएँ);

    bool long_शब्दगण = false;

    क्रम (स्वतः &row : वस्तुएँ)
    {
      निश्चित (row->लंबाई == chars.लंबाई);

      /* 0/1/2 byte encoding */
      क्रम (अचिन्हित i = 0; i < row->लंबाई; i++)
      {
	पूर्णांक v =  row->arrayZ[i];
	अगर (v == 0)
	  जारी;
	अन्यथा अगर (v > 32767 || v < -32768)
	{
	  long_शब्दगण = true;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	अन्यथा अगर (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	अन्यथा
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    अगर (long_शब्दगण)
    {
      // Convert 1s to 2s
      क्रम (स्वतः &v : chars)
	अगर (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  bool is_empty () स्थिर
  { वापस !वस्तुएँ; }

  bool in_error () स्थिर
  { वापस chars.in_error () || वस्तुएँ.in_error (); }

  अटल अंतरभूत अचिन्हित get_chars_overhead (अचिन्हित अंक_columns)
  {
    अचिन्हित c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    वापस c + अंक_columns * 2;
  }

  अचिन्हित get_gain (अचिन्हित additional_bytes_per_rows = 1) स्थिर
  {
    पूर्णांक गिनती = वस्तुएँ.लंबाई;
    वापस hb_max (0, (पूर्णांक) overhead - गिनती * (पूर्णांक) additional_bytes_per_rows);
  }

  पूर्णांक gain_from_merging (स्थिर delta_row_encoding_t& other_encoding) स्थिर
  {
    // Back of the envelope calculations to reject early.
    चिन्हित additional_bytes_per_rows = other_encoding.width - width;
    अगर (additional_bytes_per_rows > 0)
    {
      अगर (get_gain (additional_bytes_per_rows) == 0)
        वापस 0;
    }
    अन्यथा
    {
      अगर (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	वापस 0;
    }

    स्वतः pair = combine_width (other_encoding);
    अचिन्हित combined_width = pair.first;
    अचिन्हित combined_columns = pair.second;

    पूर्णांक combined_gain = (पूर्णांक) overhead + (पूर्णांक) other_encoding.overhead;
    combined_gain -= (combined_width - (पूर्णांक) width) * वस्तुएँ.लंबाई;
    combined_gain -= (combined_width - (पूर्णांक) other_encoding.width) * other_encoding.वस्तुएँ.लंबाई;
    combined_gain -= get_chars_overhead (combined_columns);

    वापस combined_gain;
  }

  bool add_row (स्थिर hb_vector_t<पूर्णांक>* row)
  { वापस वस्तुएँ.push (row); }

  अटल पूर्णांक cmp (स्थिर व्योम *pa, स्थिर व्योम *pb)
  {
    स्थिर delta_row_encoding_t *a = (स्थिर delta_row_encoding_t *)pa;
    स्थिर delta_row_encoding_t *b = (स्थिर delta_row_encoding_t *)pb;

    अगर (a->width != b->width)
      वापस (पूर्णांक) a->width - (पूर्णांक) b->width;

    वापस b->chars.cmp (a->chars);
  }
};

काष्ठा VarRegionAxis
{
  भग्न evaluate (पूर्णांक coord) स्थिर
  {
    पूर्णांक peak = peakCoord.to_int ();
    अगर (peak == 0 || coord == peak)
      वापस 1.f;
    अन्यथा अगर (coord == 0) // Faster
      वापस 0.f;

    पूर्णांक start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    अगर (unlikely (start > peak || peak > end))
      वापस 1.f;
    अगर (unlikely (start < 0 && end > 0))
      वापस 1.f;

    अगर (coord <= start || end <= coord)
      वापस 0.f;

    /* Interpolate */
    अगर (coord < peak)
      वापस भग्न (coord - start) / (peak - start);
    अन्यथा
      वापस भग्न (end - coord) / (end - peak);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  bool serialize (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (यह));
  }

  खुला:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  खुला:
  DEFINE_आकार_STATIC (6);
};
काष्ठा SparseVarRegionAxis
{
  भग्न evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len) स्थिर
  {
    अचिन्हित i = axis_सूचकांक_2;
    पूर्णांक coord = i < coord_len ? coords[i] : 0;
    वापस axis.evaluate (coord);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर SparseVarRegionAxis *src,
		  स्थिर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->embed (src);
    अगर (unlikely (!out)) return_trace (false);
    अगर (axis_map)
    {
      अगर (unlikely (!axis_map->has (out->axis_सूचकांक_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (false);
      }
      अगर (unlikely (!c->check_assign (out->axis_सूचकांक_2, axis_map->लो (out->axis_सूचकांक_2),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (false);
    }
    return_trace (true);
  }

  खुला:
  HBUINT16 axis_सूचकांक_2;
  VarRegionAxis axis;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा hb_scalar_cache_t
{
  निजी:
  अटल constexpr अचिन्हित STATIC_लंबाई = 128;
  अटल constexpr भग्न INVALID = न_अंक;

  खुला:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (स्थिर hb_scalar_cache_t&) = मिटाओ;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = मिटाओ;
  hb_scalar_cache_t& चालक= (स्थिर hb_scalar_cache_t&) = मिटाओ;
  hb_scalar_cache_t& चालक= (hb_scalar_cache_t&&) = मिटाओ;

  अटल hb_scalar_cache_t *create (अचिन्हित पूर्णांक गिनती,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    अगर (!गिनती) वापस (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    अगर (scratch_cache && गिनती <= STATIC_लंबाई)
    {
      scratch_cache->लंबाई = गिनती;
      scratch_cache->सफा ();
      वापस scratch_cache;
    }

    स्वतः *cache = (hb_scalar_cache_t *) hb_malloc (माप (hb_scalar_cache_t) - माप (static_मूल्यावली) + माप (static_मूल्यावली[0]) * गिनती);
    अगर (unlikely (!cache)) वापस (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->लंबाई = गिनती;
    cache->सफा ();

    वापस cache;
  }

  अटल व्योम destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    अगर (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  व्योम सफा ()
  {
    स्वतः *values = &static_मूल्यावली[0];
    अचिन्हित i = 0;
#अगर_अघोषित HB_OPTIMIZE_आकार
    क्रम (; i + 3 < लंबाई; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#पूर्ण_अगर
    क्रम (; i < लंबाई; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  bool लो (अचिन्हित i, भग्न *value) स्थिर
  {
    अगर (unlikely (i >= लंबाई))
    {
      *value = 0.f;
      वापस true;
    }
    स्वतः *values = &static_मूल्यावली[0];
    स्वतः *cached_मूल्य = &values[i];
    भग्न v = *cached_मूल्य;
    अगर (std::isnan (v))
      वापस false;
    *value = v;
    वापस true;
  }

  HB_ALWAYS_INLINE
  व्योम set (अचिन्हित i, भग्न value)
  {
    अगर (unlikely (i >= लंबाई)) वापस;
    स्वतः *values = &static_मूल्यावली[0];
    स्वतः *cached_मूल्य = &values[i];
    *cached_मूल्य = value;
  }

  खुला:
  अचिन्हित लंबाई;
  निजी:
  mutable hb_atomic_t<भग्न> static_मूल्यावली[STATIC_लंबाई];
};

काष्ठा VarRegionList
{
  निजी:
  भग्न evaluate_impl (अचिन्हित पूर्णांक region_सूचकांक,
		       स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len) स्थिर
  {
    स्थिर VarRegionAxis *axes = axesZ.arrayZ + (region_सूचकांक * axis_गिनती_2);
    भग्न v = 1.f;

    अचिन्हित पूर्णांक गिनती = axis_गिनती_2;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      पूर्णांक coord = i < coord_len ? coords[i] : 0;
      भग्न factor = axes[i].evaluate (coord);
      अगर (factor == 0.f)
      {
	v = 0.f;
	अवरोध;
      }
      v *= factor;
    }

    वापस v;
  }

  खुला:
  HB_ALWAYS_INLINE
  भग्न evaluate (अचिन्हित पूर्णांक region_सूचकांक,
		  स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		  hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    अगर (unlikely (region_सूचकांक >= region_गिनती))
      वापस 0.;

    भग्न v;
    अगर (cache && cache->लो (region_सूचकांक, &v))
      वापस v;

    v = evaluate_impl (region_सूचकांक, coords, coord_len);

    अगर (cache)
      cache->set (region_सूचकांक, v);
    वापस v;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_गिनती_2 * region_गिनती));
  }

  bool serialize (hb_serialize_context_t *c,
                  स्थिर hb_vector_t<hb_tag_t>& axis_tags,
                  स्थिर hb_vector_t<स्थिर hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित axis_गिनती = axis_tags.लंबाई;
    अचिन्हित region_गिनती_2 = regions.लंबाई;
    अगर (!axis_गिनती || !region_गिनती_2) return_trace (false);
    अगर (unlikely (hb_unsigned_mul_overflows (axis_गिनती * region_गिनती_2,
                                             VarRegionAxis::static_आकार))) return_trace (false);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    axis_गिनती_2 = axis_गिनती;
    region_गिनती = region_गिनती_2;

    क्रम (अचिन्हित r = 0; r < region_गिनती_2; r++)
    {
      स्थिर स्वतः& region = regions[r];
      क्रम (अचिन्हित i = 0; i < axis_गिनती; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        अगर (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->मध्य);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        अन्यथा
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        अगर (!var_region_rec.serialize (c))
          return_trace (false);
      }
    }
    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c, स्थिर VarRegionList *src, स्थिर hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    axis_गिनती_2 = src->axis_गिनती_2;
    region_गिनती = region_map.get_population ();
    अगर (unlikely (hb_unsigned_mul_overflows (axis_गिनती_2 * region_गिनती,
					     VarRegionAxis::static_आकार))) return_trace (false);
    अगर (unlikely (!c->extend (यह))) return_trace (false);
    अचिन्हित पूर्णांक region_गिनती_2 = src->region_गिनती;
    क्रम (अचिन्हित पूर्णांक r = 0; r < region_गिनती; r++)
    {
      अचिन्हित पूर्णांक backward = region_map.backward (r);
      अगर (backward >= region_गिनती_2) return_trace (false);
      hb_memcpy (&axesZ[axis_गिनती_2 * r], &src->axesZ[axis_गिनती_2 * backward], VarRegionAxis::static_आकार * axis_गिनती_2);
    }

    return_trace (true);
  }

  bool get_var_region (अचिन्हित region_सूचकांक,
                       स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) स्थिर
  {
    अगर (region_सूचकांक >= region_गिनती) वापस false;
    स्थिर VarRegionAxis* axis_region = axesZ.arrayZ + (region_सूचकांक * axis_गिनती_2);
    क्रम (अचिन्हित i = 0; i < axis_गिनती_2; i++)
    {
      hb_tag_t *axis_tag;
      अगर (!axes_old_सूचकांक_tag_map.has (i, &axis_tag))
        वापस false;

      भग्न min_val = axis_region->startCoord.to_float ();
      भग्न def_val = axis_region->peakCoord.to_float ();
      भग्न max_val = axis_region->endCoord.to_float ();

      अगर (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((द्विगुन) min_val, (द्विगुन) def_val, (द्विगुन) max_val));
      axis_region++;
    }
    वापस !axis_tuples.in_error ();
  }

  bool get_var_regions (स्थिर hb_map_t& axes_old_सूचकांक_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) स्थिर
  {
    अगर (!regions.alloc (region_गिनती))
      वापस false;

    क्रम (अचिन्हित i = 0; i < region_गिनती; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      अगर (!get_var_region (i, axes_old_सूचकांक_tag_map, axis_tuples))
        वापस false;
      regions.push (std::move (axis_tuples));
    }
    वापस !regions.in_error ();
  }

  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_add_saturate (min_आकार,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_आकार,
							       axis_गिनती_2, region_गिनती)); }

  खुला:
  HBUINT16	axis_गिनती_2;
  HBUINT15	region_गिनती;
  रक्षित:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  खुला:
  DEFINE_आकार_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
अटल अंतरभूत bool
_hb_avar2_region_is_dead (स्थिर hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  स्थिर hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  क्रम (स्वतः _ : axis_tuples)
  {
    Triple *range;
    अगर (!reachable_ranges.has (_.first, &range)) जारी;
    स्थिर Triple &tent = _.second;
    पूर्णांक start = (पूर्णांक) roundf ((भग्न) tent.minimum * 16384.f);
    पूर्णांक peak  = (पूर्णांक) roundf ((भग्न) tent.मध्य * 16384.f);
    पूर्णांक end   = (पूर्णांक) roundf ((भग्न) tent.maximum * 16384.f);
    अगर (!peak) जारी;
    अगर (start > peak || peak > end) जारी;
    अगर (start < 0 && end > 0) जारी;
    पूर्णांक lo = (पूर्णांक) roundf ((भग्न) range->minimum * 16384.f);
    पूर्णांक hi = (पूर्णांक) roundf ((भग्न) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    अगर (hi < start || lo > end)
      वापस true;
  }
  वापस false;
}

काष्ठा SparseVariationRegion
{
  भग्न evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len) स्थिर
  {
    भग्न v = 1.f;
    अचिन्हित पूर्णांक गिनती = axes.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      भग्न factor = (यह+axes[i]).evaluate (coords, coord_len);
      अगर (factor == 0.f)
	वापस 0.;
      v *= factor;
    }
    वापस v;
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर SparseVariationRegion *src,
		  स्थिर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!axes.serialize (c, (अचिन्हित) src->axes.len))) return_trace (false);
    क्रम (अचिन्हित i = 0; i < axes.len; i++)
      अगर (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (axes.sanitize (c, यह));
  }

  रक्षित:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  खुला:
  DEFINE_आकार_ARRAY (2, axes);
};

काष्ठा SparseVarRegionList
{
  HB_ALWAYS_INLINE
  भग्न evaluate (अचिन्हित पूर्णांक region_सूचकांक,
		  स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		  hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    अगर (unlikely (region_सूचकांक >= regions.len))
      वापस 0.;

    भग्न v;
    अगर (cache && cache->लो (region_सूचकांक, &v))
      वापस v;

    स्थिर SparseVariationRegion &region = यह+regions[region_सूचकांक];

    v = region.evaluate (coords, coord_len);
    अगर (cache)
      cache->set (region_सूचकांक, v);

    वापस v;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (regions.sanitize (c, यह));
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर SparseVarRegionList *src,
		  स्थिर hb_inc_bimap_t &region_map,
		  स्थिर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    regions.len = region_map.get_population ();
    अगर (unlikely (!c->extend (regions))) return_trace (false);

    क्रम (अचिन्हित i = 0; i < regions.len; i++)
    {
      अचिन्हित old_सूचकांक = region_map.backward (i);
      अगर (unlikely (old_सूचकांक >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_सूचकांक]),
					     axis_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  खुला:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  खुला:
  DEFINE_आकार_ARRAY (4, regions);
};


काष्ठा Var_तथ्य
{
  अचिन्हित पूर्णांक get_item_गिनती () स्थिर
  { वापस item_गिनती; }

  अचिन्हित पूर्णांक get_region_सूचकांक_गिनती () स्थिर
  { वापस regionIndices.len; }

  अचिन्हित get_region_सूचकांक (अचिन्हित i) स्थिर
  { वापस i >= regionIndices.len ? -1 : regionIndices[i]; }

  माप_प्रकार get_row_आकार () स्थिर
  { वापस (word_गिनती () + regionIndices.len) * (long_शब्दगण_2 () ? 2 : 1); }

  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_add_saturate (min_आकार - regionIndices.min_आकार,
				     regionIndices.get_आकार (),
				     hb_unsigned_mul_saturate (item_गिनती, get_row_आकार ())); }

  ढाँचा <typename Float = भग्न>
  Float _get_delta (अचिन्हित पूर्णांक inner,
		    स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		    स्थिर VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    अगर (unlikely (inner >= item_गिनती))
      वापस 0.;
    bool is_long = long_शब्दगण_2 ();
    अचिन्हित पूर्णांक गिनती = regionIndices.len;
    अचिन्हित word_गिनती_2 = word_गिनती ();
    अचिन्हित पूर्णांक scount = is_long ? गिनती : word_गिनती_2;
    अचिन्हित पूर्णांक lcount = is_long ? word_गिनती_2 : 0;

    स्थिर HBUINT8 *bytes = get_delta_bytes ();
    स्थिर HBUINT8 *row = bytes + inner * get_row_आकार ();

    Float delta = 0;
    अचिन्हित पूर्णांक i = 0;

    स्थिर HBINT32 *lcursor = reinterpret_cast<स्थिर HBINT32 *> (row);
    क्रम (; i < lcount; i++)
    {
      भग्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गिनती, cache);
      अगर (scalar)
        delta += (Float) scalar * (पूर्णांक) *lcursor;
      lcursor++;
    }
    स्थिर HBINT16 *scursor = reinterpret_cast<स्थिर HBINT16 *> (lcursor);
    क्रम (; i < scount; i++)
    {
      भग्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गिनती, cache);
      अगर (scalar)
       delta += (Float) scalar * (पूर्णांक) *scursor;
      scursor++;
    }
    स्थिर HBINT8 *bcursor = reinterpret_cast<स्थिर HBINT8 *> (scursor);
    क्रम (; i < गिनती; i++)
    {
      भग्न scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गिनती, cache);
      अगर (scalar)
        delta += (Float) scalar * (पूर्णांक) *bcursor;
      bcursor++;
    }

    वापस delta;
  }

  ढाँचा <typename Float = भग्न>
  HB_ALWAYS_INLINE
  Float get_delta (अचिन्हित पूर्णांक inner,
		   स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		   स्थिर VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = regionIndices.len;
    अगर (!गिनती) वापस Float (0); // This is quite common, so optimize it.
    वापस _get_delta<Float> (inner, coords, coord_गिनती, regions, cache);
  }

  व्योम get_region_scalars (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
			   स्थिर VarRegionList &regions,
			   भग्न *scalars /*OUT */,
			   अचिन्हित पूर्णांक अंक_scalars) स्थिर
  {
    अचिन्हित गिनती = hb_min (अंक_scalars, regionIndices.len);
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_गिनती);
    क्रम (अचिन्हित पूर्णांक i = गिनती; i < अंक_scalars; i++)
      scalars[i] = 0.f;
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_गिनती () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_गिनती,
				  get_row_आकार ()));
  }

  bool serialize (hb_serialize_context_t *c,
                  bool has_long,
                  स्थिर hb_vector_t<स्थिर hb_vector_t<पूर्णांक>*>& rows)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित row_गिनती_2 = rows.लंबाई;
    अगर (!row_गिनती_2) {
      // Nothing to serialize, will be empty.
      वापस false;
    }

    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    अगर (unlikely (!c->check_assign (item_गिनती, row_गिनती_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    पूर्णांक min_threshold = has_long ? -32768 : -128;
    पूर्णांक max_threshold = has_long ? +32767 : +127;
    क्रमागत delta_आकार_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_आकार_t> delta_sz;
    अचिन्हित अंक_regions = rows[0]->लंबाई;
    अगर (!delta_sz.resize (अंक_regions))
      return_trace (false);

    अचिन्हित word_गिनती_2 = 0;
    क्रम (अचिन्हित r = 0; r < अंक_regions; r++)
    {
      क्रम (अचिन्हित i = 0; i < row_गिनती_2; i++)
      {
        पूर्णांक delta = rows[i]->arrayZ[r];
        अगर (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_गिनती_2++;
          अवरोध;
        }
        अन्यथा अगर (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    अचिन्हित word_सूचकांक = 0;
    अचिन्हित non_word_सूचकांक = word_गिनती_2;
    hb_map_t ri_map;
    क्रम (अचिन्हित r = 0; r < अंक_regions; r++)
    {
      अगर (!delta_sz[r]) जारी;
      अचिन्हित new_r = (delta_sz[r] == kWord)? word_सूचकांक++ : non_word_सूचकांक++;
      अगर (!ri_map.set (new_r, r))
        return_trace (false);
    }

    word_आकार_गिनती = word_गिनती_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    अचिन्हित ri_गिनती = ri_map.get_population ();
    regionIndices.len = ri_गिनती;
    अगर (unlikely (!c->extend (यह))) return_trace (false);

    क्रम (अचिन्हित r = 0; r < ri_गिनती; r++)
    {
      hb_codepoint_t *idx;
      अगर (!ri_map.has (r, &idx))
        return_trace (false);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_आकार = get_row_आकार ();
    क्रम (अचिन्हित पूर्णांक i = 0; i < row_गिनती_2; i++)
    {
      क्रम (अचिन्हित पूर्णांक r = 0; r < ri_गिनती; r++)
      {
        पूर्णांक delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_आकार);
      }
    }
    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर Var_तथ्य *src,
		  स्थिर hb_inc_bimap_t &inner_map,
		  स्थिर hb_inc_bimap_t &region_map,
		  स्थिर hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    item_गिनती = inner_map.get_अग्र_मूल्य ();

    /* Optimize word count */
    अचिन्हित ri_गिनती = src->regionIndices.len;
    क्रमागत delta_आकार_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_आकार_t> delta_sz;
    hb_vector_t<अचिन्हित पूर्णांक> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_गिनती);
    ri_map.resize (ri_गिनती);
    अचिन्हित पूर्णांक new_word_गिनती = 0;
    अचिन्हित पूर्णांक r;

    स्थिर HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    अचिन्हित src_row_आकार = src->get_row_आकार ();
    अचिन्हित src_word_गिनती = src->word_गिनती ();
    bool     src_long_शब्दगण = src->long_शब्दगण_2 ();

    bool has_long = false;
    अगर (src_long_शब्दगण)
    {
      क्रम (r = 0; r < src_word_गिनती; r++)
      {
        क्रम (अचिन्हित old_gid : inner_map.कुंजियाँ())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_आकार);
	  अगर (delta < -32768 || 32767 < delta)
	  {
	    has_long = true;
	    अवरोध;
	  }
        }
      }
    }

    चिन्हित min_threshold = has_long ? -32768 : -128;
    चिन्हित max_threshold = has_long ? +32767 : +127;
    क्रम (r = 0; r < ri_गिनती; r++)
    {
      bool short_circuit = src_long_शब्दगण == has_long && src_word_गिनती <= r;

      delta_sz[r] = kZero;
      अगर (culled_regions && culled_regions->has (src->regionIndices[r]))
	जारी; /* unreachable region: drop its column */
      क्रम (अचिन्हित old_gid : inner_map.कुंजियाँ())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_आकार);
	अगर (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_गिनती++;
	  अवरोध;
	}
	अन्यथा अगर (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  अगर (short_circuit)
	    अवरोध;
	}
      }
    }

    अचिन्हित पूर्णांक word_सूचकांक = 0;
    अचिन्हित पूर्णांक non_word_सूचकांक = new_word_गिनती;
    अचिन्हित पूर्णांक new_ri_गिनती = 0;
    क्रम (r = 0; r < ri_गिनती; r++)
      अगर (delta_sz[r])
      {
	अचिन्हित new_r = (delta_sz[r] == kWord)? word_सूचकांक++ : non_word_सूचकांक++;
	ri_map[new_r] = r;
	new_ri_गिनती++;
      }

    word_आकार_गिनती = new_word_गिनती | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_गिनती;

    अगर (unlikely (!c->extend (यह))) return_trace (false);

    क्रम (r = 0; r < new_ri_गिनती; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_आकार = get_row_आकार ();
    अचिन्हित गिनती = item_गिनती;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अचिन्हित पूर्णांक old = inner_map.backward (i);
      क्रम (अचिन्हित पूर्णांक r = 0; r < new_ri_गिनती; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_आकार),
			     delta_bytes, row_आकार);
    }

    return_trace (true);
  }

  व्योम collect_region_refs (hb_set_t &region_indices, स्थिर hb_inc_bimap_t &inner_map) स्थिर
  {
    स्थिर HBUINT8 *delta_bytes = get_delta_bytes ();
    अचिन्हित row_आकार = get_row_आकार ();

    क्रम (अचिन्हित पूर्णांक r = 0; r < regionIndices.len; r++)
    {
      अचिन्हित पूर्णांक region = regionIndices.arrayZ[r];
      अगर (region_indices.has (region)) जारी;
      क्रम (hb_codepoint_t old_gid : inner_map.कुंजियाँ())
	अगर (get_item_delta_fast (old_gid, r, delta_bytes, row_आकार) != 0)
	{
	  region_indices.add (region);
	  अवरोध;
	}
    }
  }

  खुला:
  स्थिर HBUINT8 *get_delta_bytes () स्थिर
  { वापस &StructAfter<HBUINT8> (regionIndices); }

  रक्षित:
  HBUINT8 *get_delta_bytes ()
  { वापस &StructAfter<HBUINT8> (regionIndices); }

  खुला:
  int32_t get_item_delta_fast (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region,
			       स्थिर HBUINT8 *delta_bytes, अचिन्हित row_आकार) स्थिर
  {
    अगर (unlikely (item >= item_गिनती || region >= regionIndices.len)) वापस 0;

    स्थिर HBINT8 *p = (स्थिर HBINT8 *) delta_bytes + item * row_आकार;
    अचिन्हित word_गिनती_2 = word_गिनती ();
    bool is_long = long_शब्दगण_2 ();
    अगर (is_long)
    {
      अगर (region < word_गिनती_2)
	वापस ((स्थिर HBINT32 *) p)[region];
      अन्यथा
	वापस ((स्थिर HBINT16 *)(p + HBINT32::static_आकार * word_गिनती_2))[region - word_गिनती_2];
    }
    अन्यथा
    {
      अगर (region < word_गिनती_2)
	वापस ((स्थिर HBINT16 *) p)[region];
      अन्यथा
	वापस (p + HBINT16::static_आकार * word_गिनती_2)[region - word_गिनती_2];
    }
  }
  int32_t get_item_delta (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region) स्थिर
  {
     वापस get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_आकार ());
  }

  रक्षित:
  व्योम set_item_delta_fast (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region, int32_t delta,
			    HBUINT8 *delta_bytes, अचिन्हित row_आकार)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_आकार;
    अचिन्हित word_गिनती_2 = word_गिनती ();
    bool is_long = long_शब्दगण_2 ();
    अगर (is_long)
    {
      अगर (region < word_गिनती_2)
	((HBINT32 *) p)[region] = delta;
      अन्यथा
	((HBINT16 *)(p + HBINT32::static_आकार * word_गिनती_2))[region - word_गिनती_2] = delta;
    }
    अन्यथा
    {
      अगर (region < word_गिनती_2)
	((HBINT16 *) p)[region] = delta;
      अन्यथा
	(p + HBINT16::static_आकार * word_गिनती_2)[region - word_गिनती_2] = delta;
    }
  }
  व्योम set_item_delta (अचिन्हित पूर्णांक item, अचिन्हित पूर्णांक region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_आकार ());
  }

  bool long_शब्दगण_2 () स्थिर { वापस word_आकार_गिनती & 0x8000u /* LONG_WORDS */; }
  अचिन्हित word_गिनती () स्थिर { वापस word_आकार_गिनती & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  रक्षित:
  HBUINT16		item_गिनती;
  HBUINT16		word_आकार_गिनती;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  खुला:
  DEFINE_आकार_ARRAY (6, regionIndices);
};

काष्ठा Multi_Var_तथ्य
{
  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_add_saturate (min_आकार - regionIndices.min_आकार,
				     regionIndices.get_आकार ()); }

  स्थिर TupleList &get_delta_sets () स्थिर
  { वापस यह+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  व्योम get_delta (अचिन्हित पूर्णांक inner,
		  स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		  स्थिर SparseVarRegionList &regions,
		  hb_array_t<द्विगुन> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    स्वतः &deltaSets = get_delta_sets ();

    स्वतः मूल्यावली_iter = deltaSets.fetcher (inner);
    अचिन्हित region_गिनती = regionIndices.len;
    अचिन्हित skip = 0;
    क्रम (अचिन्हित region_सूचकांक_2 = 0; region_सूचकांक_2 < region_गिनती; region_सूचकांक_2++)
    {
      भग्न scalar = regions.evaluate (regionIndices.arrayZ[region_सूचकांक_2],
				       coords, coord_गिनती,
				       cache);
      // We skip lazily. Helps with the tail end.
      अगर (scalar == 0.0f)
        skip += out.लंबाई;
      अन्यथा
      {
        अगर (skip)
	{
	  मूल्यावली_iter.skip (skip);
	  skip = 0;
	}
	मूल्यावली_iter.add_to (out, scalar);
      }
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, यह));
  }

  अचिन्हित get_item_गिनती () स्थिर
  { वापस get_delta_sets ().गिनती; }

  व्योम collect_region_refs (hb_set_t &region_indices) स्थिर
  {
    क्रम (अचिन्हित region : regionIndices)
      region_indices.add (region);
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर Multi_Var_तथ्य *src,
		  स्थिर hb_inc_bimap_t &inner_map,
		  स्थिर hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित region_गिनती_2 = src->regionIndices.len;
    अचिन्हित header_आकार = HBUINT8::static_आकार +
			   Array16Of<HBUINT16>::min_आकार +
			   region_गिनती_2 * HBUINT16::static_आकार +
			   HBUINT32::static_आकार;
    अगर (unlikely (!c->extend_आकार (यह, header_आकार, false)))
      return_trace (false);
    format = 1;
    regionIndices.len = region_गिनती_2;
    क्रम (अचिन्हित i = 0; i < regionIndices.len; i++)
    {
      अचिन्हित old_region = src->regionIndices[i];
      अगर (unlikely (!region_map.has (old_region)))
	return_trace (false);
      regionIndices[i] = region_map.लो (old_region);
    }

    स्थिर स्वतः &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    अचिन्हित तथ्य_आकार = 0;
    अगर (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (false);
    क्रम (अचिन्हित i = 0; i < inner_map.get_population (); i++)
    {
      अचिन्हित old_inner = inner_map.backward (i);
      अगर (unlikely (old_inner >= src_delta_sets.गिनती)) return_trace (false);
      hb_ubytes_t bytes = static_cast<स्थिर CFF2_सूचकांक &> (src_delta_sets)[old_inner];
      तथ्य_आकार = hb_unsigned_add_saturate (तथ्य_आकार, bytes.लंबाई);
      delta_sets.push (bytes);
    }
    अगर (unlikely (तथ्य_आकार == अच_पूर्णांक_उच्च || delta_sets.in_error ()))
      return_trace (false);

    स्वतः &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &तथ्य_आकार));
  }

  रक्षित:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  खुला:
  DEFINE_आकार_MIN (7);
};

काष्ठा ItemVariationStore
{
  मित्र काष्ठा item_variations_t;

  hb_scalar_cache_t *create_cache () स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस hb_scalar_cache_t::create (0);
#पूर्ण_अगर
    वापस hb_scalar_cache_t::create ((यह+regions).region_गिनती);
  }

  अटल व्योम destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  निजी:
  ढाँचा <typename Float = भग्न>
  Float get_delta (अचिन्हित पूर्णांक outer, अचिन्हित पूर्णांक inner,
		   स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		   hb_scalar_cache_t *cache = nullptr) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस Float (0);
#पूर्ण_अगर

    अगर (unlikely (outer >= तथ्य_Sets.len))
      वापस Float (0);

    वापस (यह+तथ्य_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_गिनती,
					     यह+regions,
					     cache);
  }

  खुला:
  bool has_delta_set (अचिन्हित पूर्णांक सूचकांक_2) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस false;
#पूर्ण_अगर

    अचिन्हित पूर्णांक outer = सूचकांक_2 >> 16;
    अचिन्हित पूर्णांक inner = सूचकांक_2 & 0xFFFF;
    वापस outer < तथ्य_Sets.len &&
	   inner < (यह+तथ्य_Sets[outer]).get_item_गिनती ();
  }

  ढाँचा <typename Float = भग्न>
  Float get_delta (अचिन्हित पूर्णांक सूचकांक_2,
		   स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		   hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    अचिन्हित पूर्णांक outer = सूचकांक_2 >> 16;
    अचिन्हित पूर्णांक inner = सूचकांक_2 & 0xFFFF;
    वापस get_delta<Float> (outer, inner, coords, coord_गिनती, cache);
  }
  ढाँचा <typename Float = भग्न>
  Float get_delta (अचिन्हित पूर्णांक सूचकांक_2,
		   hb_array_t<स्थिर पूर्णांक> coords,
		   hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    वापस get_delta<Float> (सूचकांक_2,
		      coords.arrayZ, coords.लंबाई,
		      cache);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस true;
#पूर्ण_अगर

    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, यह) &&
		  तथ्य_Sets.sanitize (c, यह));
  }

  bool serialize (hb_serialize_context_t *c,
                  bool has_long,
                  स्थिर hb_vector_t<hb_tag_t>& axis_tags,
                  स्थिर hb_vector_t<स्थिर hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  स्थिर hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (यह);
#अगर_घोषित HB_NO_VAR
    return_trace (false);
#पूर्ण_अगर
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    format = 1;
    अगर (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (false);

    अचिन्हित अंक_var_तथ्य = vardata_encodings.लंबाई;
    अगर (!अंक_var_तथ्य) return_trace (false);
    अगर (unlikely (!c->check_assign (तथ्य_Sets.len, अंक_var_तथ्य,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    अगर (unlikely (!c->extend (तथ्य_Sets))) return_trace (false);
    क्रम (अचिन्हित i = 0; i < अंक_var_तथ्य; i++)
      अगर (!तथ्य_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].वस्तुएँ))
        return_trace (false);

    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर ItemVariationStore *src,
		  स्थिर hb_array_t <स्थिर hb_inc_bimap_t> &inner_maps,
		  स्थिर hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (यह);
#अगर_घोषित HB_NO_VAR
    return_trace (false);
#पूर्ण_अगर

    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    अचिन्हित पूर्णांक set_गिनती = 0;
    क्रम (अचिन्हित पूर्णांक i = 0; i < inner_maps.लंबाई; i++)
      अगर (inner_maps[i].get_population ())
	set_गिनती++;

    format = 1;

    स्थिर स्वतः &src_regions = src+src->regions;

    hb_set_t region_indices;
    क्रम (अचिन्हित पूर्णांक i = 0; i < inner_maps.लंबाई; i++)
      (src+src->तथ्य_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    अगर (region_indices.in_error ())
      return_trace (false);

    region_indices.del_range ((src_regions).region_गिनती, hb_set_t::INVALID);

    अगर (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (अचिन्हित _) { region_map.add(_); })
    ;
    अगर (region_map.in_error())
      return_trace (false);

    अगर (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (false);

    तथ्य_Sets.len = set_गिनती;
    अगर (unlikely (!c->extend (तथ्य_Sets))) return_trace (false);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    अचिन्हित पूर्णांक set_सूचकांक = 0;
    क्रम (अचिन्हित पूर्णांक i = 0; i < inner_maps.लंबाई; i++)
    {
      अगर (!inner_maps[i].get_population ()) जारी;
      अगर (unlikely (!तथ्य_Sets[set_सूचकांक++]
		     .serialize_serialize (c, &(src+src->तथ्य_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (false);
    }

    return_trace (true);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->start_embed (यह);
    अगर (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    अचिन्हित गिनती = तथ्य_Sets.len;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      अगर (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      स्वतः &तथ्य = यह+तथ्य_Sets[i];

      अचिन्हित item_गिनती = तथ्य.get_item_गिनती ();
      क्रम (अचिन्हित j = 0; j < item_गिनती; j++)
	map->add (j);
    }

    अगर (unlikely (!out->serialize (c, यह, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  bool collect_dead_regions (स्थिर hb_map_t &axes_old_सूचकांक_tag_map,
			     स्थिर hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    अगर (!reachable_ranges.get_population ()) वापस true;
    स्थिर VarRegionList &region_list = यह+regions;
    अचिन्हित गिनती = region_list.region_गिनती;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      अगर (!region_list.get_var_region (i, axes_old_सूचकांक_tag_map, axis_tuples))
	वापस false;
      अगर (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    वापस !dead_regions.in_error ();
#अन्यथा
    वापस true;
#पूर्ण_अगर
  }

  bool subset (hb_subset_context_t *c, स्थिर hb_array_t<स्थिर hb_inc_bimap_t> &inner_maps) स्थिर
  {
    TRACE_SUBSET (यह);
#अगर_घोषित HB_NO_VAR
    return_trace (false);
#पूर्ण_अगर

    ItemVariationStore *varstore_अभाज्य = c->serializer->start_embed<ItemVariationStore> ();
    अगर (unlikely (!varstore_अभाज्य)) return_trace (false);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    अगर (c->plan->has_avar2)
      अगर (!collect_dead_regions (c->plan->axes_old_सूचकांक_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (false);

    varstore_अभाज्य->serialize (c->serializer, यह, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_अभाज्य->तथ्य_Sets);
  }

  अचिन्हित पूर्णांक get_region_सूचकांक_गिनती (अचिन्हित पूर्णांक major) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस 0;
#पूर्ण_अगर
    वापस (यह+तथ्य_Sets[major]).get_region_सूचकांक_गिनती ();
  }

  व्योम get_region_scalars (अचिन्हित पूर्णांक major,
			   स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
			   भग्न *scalars /*OUT*/,
			   अचिन्हित पूर्णांक अंक_scalars) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    क्रम (अचिन्हित i = 0; i < अंक_scalars; i++)
      scalars[i] = 0.f;
    वापस;
#पूर्ण_अगर

    (यह+तथ्य_Sets[major]).get_region_scalars (coords, coord_गिनती,
					       यह+regions,
					       &scalars[0], अंक_scalars);
  }

  अचिन्हित पूर्णांक get_sub_table_गिनती () स्थिर
   {
#अगर_घोषित HB_NO_VAR
     वापस 0;
#पूर्ण_अगर
     वापस तथ्य_Sets.len;
   }

  स्थिर Var_तथ्य& get_sub_table (अचिन्हित i) स्थिर
  {
#अगर_घोषित HB_NO_VAR
     वापस Null (Var_तथ्य);
#पूर्ण_अगर
     वापस यह+तथ्य_Sets[i];
  }

  स्थिर VarRegionList& get_region_list () स्थिर
  {
#अगर_घोषित HB_NO_VAR
     वापस Null (VarRegionList);
#पूर्ण_अगर
     वापस यह+regions;
  }

  रक्षित:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<Var_तथ्य>		तथ्य_Sets;
  खुला:
  DEFINE_आकार_ARRAY_SIZED (8, तथ्य_Sets);
};

काष्ठा MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस hb_scalar_cache_t::create (0);
#पूर्ण_अगर
    स्वतः &r = यह+regions;
    अचिन्हित गिनती = r.regions.len;

    वापस hb_scalar_cache_t::create (गिनती, static_cache);
  }

  अटल व्योम destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  निजी:
  व्योम get_delta (अचिन्हित पूर्णांक outer, अचिन्हित पूर्णांक inner,
		  स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		  hb_array_t<द्विगुन> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस;
#पूर्ण_अगर

    अगर (unlikely (outer >= तथ्य_Sets.len))
      वापस;

    वापस (यह+तथ्य_Sets[outer]).get_delta (inner,
					     coords, coord_गिनती,
					     यह+regions,
					     out,
					     cache);
  }

  खुला:
  व्योम get_delta (अचिन्हित पूर्णांक सूचकांक_2,
		  स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_गिनती,
		  hb_array_t<द्विगुन> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    अचिन्हित पूर्णांक outer = सूचकांक_2 >> 16;
    अचिन्हित पूर्णांक inner = सूचकांक_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_गिनती, out, cache);
  }
  व्योम get_delta (अचिन्हित पूर्णांक सूचकांक_2,
		  hb_array_t<स्थिर पूर्णांक> coords,
		  hb_array_t<द्विगुन> out,
		  hb_scalar_cache_t *cache = nullptr) स्थिर
  {
    वापस get_delta (सूचकांक_2,
		      coords.arrayZ, coords.लंबाई,
		      out,
		      cache);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
#अगर_घोषित HB_NO_VAR
    वापस true;
#पूर्ण_अगर

    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, यह) &&
		  तथ्य_Sets.sanitize (c, यह));
  }

  bool create_subset_plan (स्थिर hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) स्थिर
  {
    अगर (unlikely (!inner_maps->resize (तथ्य_Sets.len))) वापस false;
    क्रम (hb_codepoint_t var_idx : var_indices)
    {
      अचिन्हित outer = var_idx >> 16;
      अचिन्हित inner = var_idx & 0xFFFFu;
      अगर (unlikely (outer >= तथ्य_Sets.len ||
		    inner >= (यह+तथ्य_Sets[outer]).get_item_गिनती ()))
	वापस false;
      (*inner_maps)[outer].add (inner);
    }

    अचिन्हित new_outer = 0;
    क्रम (अचिन्हित outer = 0; outer < inner_maps->लंबाई; outer++)
    {
      स्थिर hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      अगर (!inner_map.get_population ()) जारी;
      अगर (unlikely (inner_map.in_error ())) वापस false;
      क्रम (अचिन्हित new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	अचिन्हित old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    वापस !inner_maps->in_error () && !varidx_map->in_error ();
  }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर MultiItemVariationStore *src,
		  स्थिर hb_array_t<स्थिर hb_inc_bimap_t> &inner_maps,
		  स्थिर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह) ||
		  inner_maps.लंबाई < src->तथ्य_Sets.len))
      return_trace (false);
    format = 1;

    स्थिर स्वतः &src_regions = src+src->regions;
    hb_set_t region_indices;
    अचिन्हित set_गिनती = 0;
    क्रम (अचिन्हित i = 0; i < src->तथ्य_Sets.len; i++)
    {
      अगर (!inner_maps[i].get_population ()) जारी;
      set_गिनती++;
      (src+src->तथ्य_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    अगर (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (false);

    तथ्य_Sets.len = set_गिनती;
    अगर (unlikely (!c->extend (तथ्य_Sets))) return_trace (false);
    अचिन्हित set_सूचकांक = 0;
    क्रम (अचिन्हित i = 0; i < src->तथ्य_Sets.len; i++)
    {
      अगर (!inner_maps[i].get_population ()) जारी;
      अगर (unlikely (!तथ्य_Sets[set_सूचकांक++].serialize_serialize (
			    c, &(src+src->तथ्य_Sets[i]), inner_maps[i], region_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  रक्षित:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<Multi_Var_तथ्य>	तथ्य_Sets;
  खुला:
  DEFINE_आकार_ARRAY_SIZED (8, तथ्य_Sets);
};

ढाँचा <typename Map_गिनती_T>
काष्ठा Delta_Set_सूचकांक_Map_Format01
{
  मित्र काष्ठा Delta_Set_सूचकांक_Map;

  माप_प्रकार get_आकार () स्थिर
  { वापस hb_unsigned_mul_add_saturate (map_गिनती_2, get_width (), min_आकार); }

  निजी:
  Delta_Set_सूचकांक_Map_Format01* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (यह));
  }

  ढाँचा <typename T>
  bool serialize (hb_serialize_context_t *c, स्थिर T &plan)
  {
    अचिन्हित पूर्णांक width = plan.get_width ();
    अचिन्हित पूर्णांक inner_bit_गिनती = plan.get_inner_bit_गिनती ();
    स्थिर hb_array_t<स्थिर uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (यह);
    अगर (unlikely (output_map.लंबाई && ((((inner_bit_गिनती-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (false);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    entryFormat = ((width-1)<<4)|(inner_bit_गिनती-1);
    map_गिनती_2 = output_map.लंबाई;
    HBUINT8 *p = c->allocate_आकार<HBUINT8> (width * output_map.लंबाई);
    अगर (unlikely (!p)) return_trace (false);
    क्रम (अचिन्हित पूर्णांक i = 0; i < output_map.लंबाई; i++)
    {
      अचिन्हित पूर्णांक v = output_map.arrayZ[i];
      अगर (v)
      {
	अचिन्हित पूर्णांक outer = v >> 16;
	अचिन्हित पूर्णांक inner = v & 0xFFFF;
	अचिन्हित पूर्णांक u = (outer << inner_bit_गिनती) | inner;
	क्रम (अचिन्हित पूर्णांक w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (true);
  }

  HB_ALWAYS_INLINE
  uint32_t map (अचिन्हित पूर्णांक v) स्थिर /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    अगर (!map_गिनती_2)
      वापस v;
    वापस _map (v);
  }

  HB_HOT
  uint32_t _map (अचिन्हित पूर्णांक v) स्थिर /* Returns 16.16 outer.inner. */
  {
    अगर (v >= map_गिनती_2)
      v = map_गिनती_2 - 1;

    अचिन्हित पूर्णांक u = 0;
    { /* Fetch it. */
      अचिन्हित पूर्णांक w = get_width ();
      स्थिर HBUINT8 *p = map_तथ्य_Z.arrayZ + w * v;
      क्रम (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      अचिन्हित पूर्णांक n = get_inner_bit_गिनती ();
      अचिन्हित पूर्णांक outer = u >> n;
      अचिन्हित पूर्णांक inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    वापस u;
  }

  अचिन्हित get_map_गिनती () स्थिर       { वापस map_गिनती_2; }
  अचिन्हित get_width () स्थिर           { वापस ((entryFormat >> 4) & 3) + 1; }
  अचिन्हित get_inner_bit_गिनती () स्थिर { वापस (entryFormat & 0xF) + 1; }


  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  hb_barrier () &&
                  c->check_range (map_तथ्य_Z.arrayZ,
                                  map_गिनती_2,
                                  get_width ()));
  }

  रक्षित:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_गिनती_T     map_गिनती_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                map_तथ्य_Z;       /* The delta-set index mapping data. */

  खुला:
  DEFINE_आकार_ARRAY (2+Map_गिनती_T::static_आकार, map_तथ्य_Z);
};

काष्ठा Delta_Set_सूचकांक_Map
{
  ढाँचा <typename T>
  bool serialize (hb_serialize_context_t *c, स्थिर T &plan)
  {
    TRACE_SERIALIZE (यह);
    अचिन्हित लंबाई = plan.get_output_map ().लंबाई;
    u.format.v = लंबाई <= 0xFFFF ? 0 : 1;
    चयन (u.format.v) {
    हाल 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    हाल 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    शेष:return_trace (false);
    }
  }

  uint32_t map (अचिन्हित v) स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); वापस (u.format0.map (v));
    हाल 1: hb_barrier (); वापस (u.format1.map (v));
    शेष:वापस v;
    }
  }

  अचिन्हित get_map_गिनती () स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); वापस u.format0.get_map_गिनती ();
    हाल 1: hb_barrier (); वापस u.format1.get_map_गिनती ();
    शेष:वापस 0;
    }
  }

  अचिन्हित get_width () स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); वापस u.format0.get_width ();
    हाल 1: hb_barrier (); वापस u.format1.get_width ();
    शेष:वापस 0;
    }
  }

  अचिन्हित get_inner_bit_गिनती () स्थिर
  {
    चयन (u.format.v) {
    हाल 0: hb_barrier (); वापस u.format0.get_inner_bit_गिनती ();
    हाल 1: hb_barrier (); वापस u.format1.get_inner_bit_गिनती ();
    शेष:वापस 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    चयन (u.format.v) {
    हाल 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    हाल 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    शेष:return_trace (true);
    }
  }

  Delta_Set_सूचकांक_Map* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    चयन (u.format.v) {
    हाल 0: hb_barrier (); return_trace (reinterpret_cast<Delta_Set_सूचकांक_Map *> (u.format0.copy (c)));
    हाल 1: hb_barrier (); return_trace (reinterpret_cast<Delta_Set_सूचकांक_Map *> (u.format1.copy (c)));
    शेष:return_trace (nullptr);
    }
  }

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT8 v; }             format;         /* Format identifier */
  Delta_Set_सूचकांक_Map_Format01<HBUINT16> format0;
  Delta_Set_सूचकांक_Map_Format01<HBUINT32> format1;
  } u;
  खुला:
  DEFINE_आकार_UNION (1, format.v);
};


काष्ठा ItemVarStoreInstancer
{
  ItemVarStoreInstancer (स्थिर ItemVariationStore *varStore_,
			 स्थिर Delta_Set_सूचकांक_Map *varIdxMap,
			 hb_array_t<स्थिर पूर्णांक> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    अगर (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    अगर (!hb_any (coords))
      यह->coords = {};
  }

  चालक bool () स्थिर { वापस varStore && bool (coords); }

  भग्न चालक[] (uint32_t varIdx) स्थिर
  { वापस (*यह) (varIdx); }

  भग्न चालक() (uint32_t varIdx, अचिन्हित लघु offset = 0) स्थिर
  { वापस get_delta (varIdx, offset); }

  द्विगुन get_condition_delta (uint32_t varIdx) स्थिर
  { वापस get_delta<द्विगुन> (varIdx); }

  ढाँचा <typename Float = भग्न>
  Float get_delta (uint32_t varIdx, अचिन्हित लघु offset = 0) स्थिर
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   अगर (!coords || varIdx == VarIdx::NO_VARIATION)
     वापस Float (0);

    varIdx += offset;
    अगर (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    वापस varStore->get_delta<Float> (varIdx, coords, cache);
  }

  स्थिर ItemVariationStore *varStore;
  स्थिर Delta_Set_सूचकांक_Map *varIdxMap;
  hb_array_t<स्थिर पूर्णांक> coords;
  hb_scalar_cache_t *cache;
};

काष्ठा MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (स्थिर MultiItemVariationStore *varStore,
			      स्थिर Delta_Set_सूचकांक_Map *varIdxMap,
			      hb_array_t<स्थिर पूर्णांक> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    अगर (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  चालक bool () स्थिर { वापस varStore; }

  द्विगुन get_condition_delta (uint32_t varIdx) स्थिर
  { वापस (द्विगुन) (*यह)[varIdx]; }

  द्विगुन चालक[] (uint32_t varIdx) स्थिर
  {
    द्विगुन v = 0;
    (*यह) (hb_array (&v, 1), varIdx);
    वापस v;
  }

  व्योम चालक() (hb_array_t<द्विगुन> out, uint32_t varIdx, अचिन्हित लघु offset = 0) स्थिर
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    अगर (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      अगर (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    अन्यथा
      क्रम (अचिन्हित i = 0; i < out.लंबाई; i++)
        out.arrayZ[i] = 0.;
  }

  स्थिर MultiItemVariationStore *varStore;
  स्थिर Delta_Set_सूचकांक_Map *varIdxMap;
  hb_array_t<स्थिर पूर्णांक> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
क्रमागत Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

क्रमागत lookup_condition_subset_परिणाम_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

काष्ठा Condition;

ढाँचा <typename Instancer>
अटल bool
_hb_recurse_condition_evaluate (स्थिर काष्ठा Condition &condition,
				स्थिर पूर्णांक *coords,
				अचिन्हित पूर्णांक coord_len,
				Instancer *instancer);

काष्ठा ConditionAxisRange
{
  मित्र काष्ठा Condition;

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    स्थिर hb_map_t *सूचकांक_map = &c->plan->axes_सूचकांक_map;
    अगर (सूचकांक_map->is_empty ()) return_trace (true);

    स्थिर hb_map_t& axes_old_सूचकांक_tag_map = c->plan->axes_old_सूचकांक_tag_map;
    hb_codepoint_t *axis_tag;
    अगर (!axes_old_सूचकांक_tag_map.has (axis_सूचकांक_2, &axis_tag) ||
        !सूचकांक_map->has (axis_सूचकांक_2))
      return_trace (false);

    स्थिर hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    अगर (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    स्थिर hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    अगर (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    भग्न normalized_min = renormalize_मूल्य ((द्विगुन) filter_Range_Min_मूल्य.to_float (), axis_limit, axis_triple_distances, false);
    भग्न normalized_max = renormalize_मूल्य ((द्विगुन) filter_Range_Max_मूल्य.to_float (), axis_limit, axis_triple_distances, false);
    out->filter_Range_Min_मूल्य.set_float (normalized_min);
    out->filter_Range_Max_मूल्य.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axis_सूचकांक_2, सूचकांक_map->लो (axis_सूचकांक_2),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  निजी:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) स्थिर
  {
    //invalid axis index, drop the entire record
    अगर (!c->axes_सूचकांक_tag_map->has (axis_सूचकांक_2))
      वापस DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_सूचकांक_tag_map->लो (axis_सूचकांक_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    bool axis_set_by_user = false;
    अगर (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = true;
    }

    भग्न axis_min_val = axis_range.minimum;
    भग्न axis_default_val = axis_range.मध्य;
    भग्न axis_max_val = axis_range.maximum;

    भग्न filter_min_val = filter_Range_Min_मूल्य.to_float ();
    भग्न filter_max_val = filter_Range_Max_मूल्य.to_float ();

    अगर (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = false;

    //condition not met, drop the entire record
    अगर (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      वापस DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    अगर (axis_set_by_user && axis_range.is_point ())
      वापस DROP_COND_WITH_VAR;

    अगर (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_मूल्य.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_मूल्य.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axis_सूचकांक_2, val);
      वापस KEEP_COND_WITH_VAR;
    }
    वापस KEEP_RECORD_WITH_VAR;
  }

  ढाँचा <typename Instancer>
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer HB_UNUSED) स्थिर
  {
    पूर्णांक coord = axis_सूचकांक_2 < coord_len ? coords[axis_सूचकांक_2] : 0;
    वापस filter_Range_Min_मूल्य.to_int () <= coord && coord <= filter_Range_Max_मूल्य.to_int ();
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_सूचकांक_2;
  F2DOT14	filter_Range_Min_मूल्य;
  F2DOT14	filter_Range_Max_मूल्य;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा Condition_मूल्य
{
  मित्र काष्ठा Condition;

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    अगर (varIdx == VarIdx::NO_VARIATION)
      return_trace (true);

    hb_pair_t<अचिन्हित, पूर्णांक> *condition_मूल्य;
    अगर (c->plan->layout_condition_idx_मूल्य_map.has ((uintptr_t) यह, &condition_मूल्य))
    {
      out->default_मूल्य_2 = condition_मूल्य->second;
      out->varIdx = condition_मूल्य->first;
      return_trace (true);
    }

    hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
    अगर (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (false);

    अगर (unlikely (!c->serializer->check_assign (
		out->default_मूल्य_2,
		default_मूल्य_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (true);
  }

  निजी:
  ढाँचा <typename Instancer>
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिर
  {
    द्विगुन value = default_मूल्य_2;
    value += instancer->get_condition_delta (varIdx);
    वापस value > 0;
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_मूल्य_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा ConditionAnd
{
  मित्र काष्ठा Condition;

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);
    out->format = format;

    क्रम (स्थिर स्वतः &offset : conditions)
    {
      अगर (offset.is_null ())
      {
	स्वतः *out_offset = out->conditions.serialize_append (c->serializer);
	अगर (unlikely (!out_offset)) return_trace (false);
	*out_offset = 0;
      }
      अन्यथा अगर (unlikely (!subset_offset_array (c, out->conditions, यह) (offset)))
	return_trace (false);
    }

    return_trace (true);
  }

  निजी:
  ढाँचा <typename Instancer>
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = conditions.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!_hb_recurse_condition_evaluate (यह+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	वापस false;
    वापस true;
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (conditions.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<काष्ठा Condition>	conditions;
  खुला:
  DEFINE_आकार_ARRAY (3, conditions);
};

काष्ठा ConditionOr
{
  मित्र काष्ठा Condition;

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);
    out->format = format;

    क्रम (स्थिर स्वतः &offset : conditions)
    {
      अगर (offset.is_null ())
      {
	स्वतः *out_offset = out->conditions.serialize_append (c->serializer);
	अगर (unlikely (!out_offset)) return_trace (false);
	*out_offset = 0;
      }
      अन्यथा अगर (unlikely (!subset_offset_array (c, out->conditions, यह) (offset)))
	return_trace (false);
    }

    return_trace (true);
  }

  निजी:
  ढाँचा <typename Instancer>
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = conditions.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (_hb_recurse_condition_evaluate (यह+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	वापस true;
    वापस false;
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (conditions.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<काष्ठा Condition>	conditions;
  खुला:
  DEFINE_आकार_ARRAY (3, conditions);
};

काष्ठा ConditionNegate
{
  मित्र काष्ठा Condition;

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);
    अगर (condition.is_null ())
    {
      out->condition = 0;
      return_trace (true);
    }
    return_trace (out->condition.serialize_subset (c, condition, यह));
  }

  निजी:
  ढाँचा <typename Instancer>
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिर
  {
    वापस !_hb_recurse_condition_evaluate (यह+condition,
					    coords, coord_len,
					    instancer);
  }

  खुला:
  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (condition.sanitize (c, यह));
  }

  रक्षित:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<काष्ठा Condition>	condition;
  खुला:
  DEFINE_आकार_STATIC (5);
};

काष्ठा Condition
{
  मित्र काष्ठा ConditionSet;

  अटल bool serialize_constant (hb_serialize_context_t *c, bool value)
  {
    स्वतः *out = c->start_embed<Condition_मूल्य> ();
    अगर (unlikely (!out || !c->extend_min (out))) वापस false;
    out->format = 2;
    out->default_मूल्य_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    वापस true;
  }

  lookup_condition_subset_परिणाम_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      अचिन्हित depth = HB_MAX_NESTING_LEVEL,
				      bool materialize_true = false) स्थिर;

  ढाँचा <typename Instancer>
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिर
  {
    चयन (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    हाल 0: वापस true;
    हाल 1: hb_barrier (); वापस u.format1.evaluate (coords, coord_len, instancer);
    हाल 2: hb_barrier (); वापस u.format2.evaluate (coords, coord_len, instancer);
    हाल 3: hb_barrier (); वापस u.format3.evaluate (coords, coord_len, instancer);
    हाल 4: hb_barrier (); वापस u.format4.evaluate (coords, coord_len, instancer);
    हाल 5: hb_barrier (); वापस u.format5.evaluate (coords, coord_len, instancer);
    शेष:वापस false;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             अचिन्हित depth = HB_MAX_NESTING_LEVEL) स्थिर
  {
    अगर (unlikely (!depth))
    {
      c->apply = false;
      वापस DROP_RECORD_WITH_VAR;
    }

    चयन (u.format.v) {
    हाल 0: वापस KEEP_RECORD_WITH_VAR;
    हाल 1: hb_barrier (); वापस u.format1.keep_with_variations (c, condition_map);
    हाल 2: हाल 3: हाल 4: हाल 5: अवरोध;
    शेष: c->apply = false; वापस DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    bool applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.लंबाई, c->instancer);
    c->apply &= applies;
    अगर (c->all_axes_pinned)
      वापस applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    अगर (u.format.v == 2)
      वापस u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    स्वतः classify_child = [c, condition_map, depth] (स्थिर Condition &child)
    {
      स्वतः child_context = *c;
      child_context.apply = true;
      वापस child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    अगर (u.format.v == 5)
    {
      स्वतः परिणाम = classify_child (&u.format5 + u.format5.condition);
      वापस परिणाम == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             परिणाम == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    bool is_and = u.format.v == 3;
    bool variable = false;
    स्थिर स्वतः &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    स्थिर व्योम *base = is_and ? (स्थिर व्योम *) &u.format3 : (स्थिर व्योम *) &u.format4;
    क्रम (स्थिर स्वतः &offset : conditions)
    {
      स्वतः परिणाम = classify_child (base + offset);
      अगर (is_and && परिणाम == DROP_RECORD_WITH_VAR)
        वापस DROP_RECORD_WITH_VAR;
      अगर (!is_and && (परिणाम == DROP_COND_WITH_VAR || परिणाम == KEEP_RECORD_WITH_VAR))
        वापस KEEP_RECORD_WITH_VAR;
      variable |= परिणाम == KEEP_COND_WITH_VAR;
    }
    वापस variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  ढाँचा <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) स्थिर
  {
    अगर (unlikely (!c->may_dispatch (यह, &u.format.v))) वापस c->no_dispatch_return_मूल्य ();
    TRACE_DISPATCH (यह, u.format.v);
    चयन (u.format.v) {
    हाल 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    हाल 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    हाल 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    हाल 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    हाल 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    शेष:return_trace (c->default_return_मूल्य ());
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);

    अगर (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_मूल्य ());

    return_trace (c->end_recursion (यह->dispatch (c)));
  }

  bool collect_var_indices (hb_set_t *var_indices,
			    अचिन्हित depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>> *condition_मूल्यावली = nullptr) स्थिर;

  bool serialize (hb_serialize_context_t *c,
		  स्थिर Condition *src,
		  स्थिर hb_map_t &varidx_map,
		  स्थिर hb_map_t *axis_map = nullptr);

  निजी:
  lookup_condition_subset_परिणाम_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      अचिन्हित depth) स्थिर;

  रक्षित:
  जोड़ {
  काष्ठा { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_मूल्य	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  खुला:
  DEFINE_आकार_UNION (2, format.v);
};

ढाँचा <typename Instancer>
bool
_hb_recurse_condition_evaluate (स्थिर काष्ठा Condition &condition,
				स्थिर पूर्णांक *coords,
				अचिन्हित पूर्णांक coord_len,
				Instancer *instancer)
{
  वापस condition.evaluate (coords, coord_len, instancer);
}

काष्ठा ConditionList
{
  स्थिर Condition& चालक[] (अचिन्हित i) स्थिर
  { वापस यह+conditions[i]; }

  अचिन्हित get_गिनती () स्थिर { वापस conditions.len; }

  bool serialize (hb_serialize_context_t *c,
		  स्थिर ConditionList *src,
		  स्थिर hb_inc_bimap_t &condition_map,
		  स्थिर hb_map_t &varidx_map,
		  स्थिर hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    conditions.len = condition_map.get_population ();
    अगर (unlikely (!c->extend (conditions))) return_trace (false);
    क्रम (अचिन्हित i = 0; i < conditions.len; i++)
    {
      अचिन्हित old_सूचकांक = condition_map.backward (i);
      अगर (unlikely (old_सूचकांक >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_सूचकांक]), varidx_map, axis_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (conditions.sanitize (c, यह));
  }

  रक्षित:
  Array32OfOffset32To<Condition> conditions;
  खुला:
  DEFINE_आकार_ARRAY (4, conditions);
};

अंतरभूत bool
Condition::collect_var_indices (hb_set_t *var_indices, अचिन्हित depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<अचिन्हित, पूर्णांक>> *condition_मूल्यावली) स्थिर
{
  अगर (unlikely (!depth)) वापस false;
  चयन (u.format.v)
  {
    हाल 0:
    हाल 1:
      वापस true;
    हाल 2:
      अगर (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        अगर (condition_मूल्यावली &&
            !condition_मूल्यावली->set ((uintptr_t) &u.format2,
                                    hb_pair (अचिन्हित (u.format2.varIdx), पूर्णांक (u.format2.default_मूल्य_2))))
          वापस false;
      }
      वापस !var_indices->in_error ();
    हाल 3:
      क्रम (स्थिर स्वतः &offset : u.format3.conditions)
	अगर (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मूल्यावली)))
	  वापस false;
      वापस true;
    हाल 4:
      क्रम (स्थिर स्वतः &offset : u.format4.conditions)
	अगर (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_मूल्यावली)))
	  वापस false;
      वापस true;
    हाल 5:
      वापस (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_मूल्यावली);
    शेष:
      वापस false;
  }
}

अंतरभूत bool
Condition::serialize (hb_serialize_context_t *c,
		      स्थिर Condition *src,
		      स्थिर hb_map_t &varidx_map,
		      स्थिर hb_map_t *axis_map)
{
  TRACE_SERIALIZE (यह);
  चयन (src->u.format.v)
  {
    हाल 0:
    {
      स्वतः *out = c->start_embed<Condition_मूल्य> ();
      अगर (unlikely (!out || !c->extend_min (out))) return_trace (false);
      out->format = 2;
      out->default_मूल्य_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (true);
    }
    हाल 1:
    {
      स्वतः *out = c->embed (&src->u.format1);
      अगर (unlikely (!out)) return_trace (false);
      अगर (axis_map)
      {
	अगर (unlikely (!axis_map->has (out->axis_सूचकांक_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (false);
	}
	अगर (unlikely (!c->check_assign (out->axis_सूचकांक_2, axis_map->लो (out->axis_सूचकांक_2),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (false);
      }
      return_trace (true);
    }
    हाल 2:
    {
      स्वतः *out = c->embed (&src->u.format2);
      अगर (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (false);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.लो (src->u.format2.varIdx);
      return_trace (true);
    }
    हाल 3:
    {
      स्वतः *out = c->start_embed<ConditionAnd> ();
      अगर (unlikely (!c->extend_min (out))) return_trace (false);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      अगर (unlikely (!c->extend (out->conditions))) return_trace (false);
      क्रम (अचिन्हित i = 0; i < out->conditions.len; i++)
	अगर (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (false);
      return_trace (true);
    }
    हाल 4:
    {
      स्वतः *out = c->start_embed<ConditionOr> ();
      अगर (unlikely (!c->extend_min (out))) return_trace (false);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      अगर (unlikely (!c->extend (out->conditions))) return_trace (false);
      क्रम (अचिन्हित i = 0; i < out->conditions.len; i++)
	अगर (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (false);
      return_trace (true);
    }
    हाल 5:
    {
      स्वतः *out = c->embed (&src->u.format5);
      अगर (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (false);
      return_trace (true);
    }
    शेष:
      return_trace (false);
  }
}

अंतरभूत lookup_condition_subset_परिणाम_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    अचिन्हित depth,
				    bool materialize_true) स्थिर
{
  स्वतः snap = c->serializer->snapshot ();
  lookup_condition_subset_परिणाम_t परिणाम =
      subset_lookup_condition_impl (c, depth);
  अगर (परिणाम == LOOKUP_CONDITION_SUBSET_KEEP)
    वापस परिणाम;

  c->serializer->revert (snap);
  अगर (परिणाम != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    वापस परिणाम;

  वापस serialize_constant (c->serializer, true) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

अंतरभूत lookup_condition_subset_परिणाम_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 अचिन्हित depth) स्थिर
{
  अगर (unlikely (!depth))
    वापस LOOKUP_CONDITION_SUBSET_ERROR;

  स्वतः subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   स्थिर Condition &condition)
      -> lookup_condition_subset_परिणाम_t
  {
    स्वतः snap = c->serializer->snapshot ();
    स्वतः *offset = out.serialize_append (c->serializer);
    अगर (unlikely (!offset))
      वापस LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_परिणाम_t परिणाम =
        condition.subset_lookup_condition (c, depth - 1);
    अगर (परिणाम == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->निकालो_pack ());
    अन्यथा
    {
      c->serializer->निकालो_discard ();
      out.निकालो ();
      c->serializer->revert (snap);
    }
    वापस परिणाम;
  };

  चयन (u.format.v)
  {
    हाल 0:
      वापस LOOKUP_CONDITION_SUBSET_TRUE;

    हाल 1:
    {
      स्थिर ConditionAxisRange &src = u.format1;
      अगर (c->plan->user_axes_location.is_empty ())
        वापस c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      अगर (!c->plan->axes_old_सूचकांक_tag_map.has (src.axis_सूचकांक_2, &axis_tag))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      अगर (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      भग्न filter_min = src.filter_Range_Min_मूल्य.to_float ();
      भग्न filter_max = src.filter_Range_Max_मूल्य.to_float ();
      अगर (filter_min > filter_max)
        वापस LOOKUP_CONDITION_SUBSET_FALSE;
      अगर (axis_limit.is_point ())
        वापस filter_min <= axis_limit.मध्य &&
	       axis_limit.मध्य <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      अगर (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        वापस LOOKUP_CONDITION_SUBSET_FALSE;
      अगर (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        वापस LOOKUP_CONDITION_SUBSET_TRUE;
      अगर (!c->plan->axes_सूचकांक_map.has (src.axis_सूचकांक_2))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;

      स्वतः *out = c->serializer->embed (&src);
      अगर (unlikely (!out))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      अगर (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_मूल्य.set_float (
	  renormalize_मूल्य (filter_min, axis_limit, axis_distances, false));
      out->filter_Range_Max_मूल्य.set_float (
	  renormalize_मूल्य (filter_max, axis_limit, axis_distances, false));
      अगर (unlikely (!c->serializer->check_assign (
		out->axis_सूचकांक_2,
		c->plan->axes_सूचकांक_map.लो (src.axis_सूचकांक_2),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;
      वापस LOOKUP_CONDITION_SUBSET_KEEP;
    }

    हाल 2:
    {
      स्थिर Condition_मूल्य &src = u.format2;
      स्वतः *out = c->serializer->embed (&src);
      अगर (unlikely (!out))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;

      अगर (src.varIdx == VarIdx::NO_VARIATION)
        वापस c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णांक) src.default_मूल्य_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<अचिन्हित, पूर्णांक> *condition_मूल्य;
      अगर (c->plan->layout_condition_idx_मूल्य_map.has ((uintptr_t) &src, &condition_मूल्य))
      {
        out->default_मूल्य_2 = condition_मूल्य->second;
        out->varIdx = condition_मूल्य->first;
        अगर (out->varIdx == VarIdx::NO_VARIATION)
          वापस out->default_मूल्य_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        वापस LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<अचिन्हित, पूर्णांक> *new_varidx_delta;
      अगर (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        वापस c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((पूर्णांक) src.default_मूल्य_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      अगर (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        द्विगुन value = (पूर्णांक) src.default_मूल्य_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        वापस value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      पूर्णांक default_मूल्य = (पूर्णांक) src.default_मूल्य_2 +
			  hb_second (*new_varidx_delta);
      अगर (unlikely (!c->serializer->check_assign (
		out->default_मूल्य_2,
		default_मूल्य,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      वापस LOOKUP_CONDITION_SUBSET_KEEP;
    }

    हाल 3:
    {
      स्थिर ConditionAnd &src = u.format3;
      स्वतः *out = c->serializer->start_embed (&src);
      अगर (unlikely (!out || !c->serializer->extend_min (out)))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      क्रम (स्थिर स्वतः &offset : src.conditions)
      {
        lookup_condition_subset_परिणाम_t परिणाम =
            subset_offset (out->conditions, &src + offset);
        अगर (परिणाम == LOOKUP_CONDITION_SUBSET_ERROR ||
	    परिणाम == LOOKUP_CONDITION_SUBSET_FALSE)
          वापस परिणाम;
      }
      वापस out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    हाल 4:
    {
      स्थिर ConditionOr &src = u.format4;
      स्वतः *out = c->serializer->start_embed (&src);
      अगर (unlikely (!out || !c->serializer->extend_min (out)))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      क्रम (स्थिर स्वतः &offset : src.conditions)
      {
        lookup_condition_subset_परिणाम_t परिणाम =
            subset_offset (out->conditions, &src + offset);
        अगर (परिणाम == LOOKUP_CONDITION_SUBSET_ERROR ||
	    परिणाम == LOOKUP_CONDITION_SUBSET_TRUE)
          वापस परिणाम;
      }
      वापस out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    हाल 5:
    {
      स्थिर ConditionNegate &src = u.format5;
      स्वतः *out = c->serializer->embed (&src);
      अगर (unlikely (!out))
        वापस LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_परिणाम_t परिणाम =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      अगर (परिणाम == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->निकालो_pack ());
        वापस परिणाम;
      }

      c->serializer->निकालो_discard ();
      अगर (परिणाम == LOOKUP_CONDITION_SUBSET_TRUE)
        वापस LOOKUP_CONDITION_SUBSET_FALSE;
      अगर (परिणाम == LOOKUP_CONDITION_SUBSET_FALSE)
        वापस LOOKUP_CONDITION_SUBSET_TRUE;
      वापस परिणाम;
    }

    शेष:
      वापस LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

काष्ठा ConditionSet
{
  bool evaluate (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		 ItemVarStoreInstancer *instancer) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = conditions.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!(यह+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	वापस false;
    वापस true;
  }

  व्योम keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिर
  {
    hb_map_t *condition_map = hb_map_create ();
    अगर (unlikely (!condition_map)) वापस;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    अगर (unlikely (!cond_set)) वापस;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = true;
    bool should_keep = false;
    /* The range map cannot identify value or boolean expressions uniquely. */
    bool can_deduplicate = true;
    अचिन्हित अंक_kept_cond = 0, cond_idx = 0;
    क्रम (स्थिर स्वतः& offset : conditions)
    {
      स्थिर Condition &condition = यह+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      अगर (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = false;
        वापस;
      }

      अगर (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = true;
        cond_set->add (cond_idx);
        अंक_kept_cond++;
      }

      अगर (ret == KEEP_RECORD_WITH_VAR)
        should_keep = true;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = अंक_kept_cond == 0;
    अगर (!should_keep && c->all_axes_pinned) वापस;

    //check if condition_set is unique with variations
    अगर (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      वापस;

    अगर (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               bool जोड़ो_catch_all) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    अगर (जोड़ो_catch_all) return_trace (true);

    hb_set_t *retained_cond_set = nullptr;
    अगर (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->लो (l->cur_feature_var_record_idx);

    अचिन्हित पूर्णांक गिनती = conditions.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (retained_cond_set != nullptr && !retained_cond_set->has (i))
        जारी;
      स्थिर Condition &condition = यह+conditions[i];
      अगर (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, यह) (conditions[i]);
        जारी;
      }

      स्वतः *offset = out->conditions.serialize_append (c->serializer);
      अगर (unlikely (!offset)) return_trace (false);
      c->serializer->push ();
      स्वतः परिणाम = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, true);
      अगर (परिणाम == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        परिणाम = Condition::serialize_constant (c->serializer, false) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      अगर (unlikely (परिणाम == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->निकालो_discard ();
        c->serializer->check_success (false);
        return_trace (false);
      }
      c->serializer->add_link (*offset, c->serializer->निकालो_pack ());
    }

    return_trace (bool (out->conditions));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (conditions.sanitize (c, यह));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर स्वतः &offset : conditions)
      अगर (unlikely (!(यह+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_मूल्यावली)))
	c->layout_variation_indices->err ();
  }

  रक्षित:
  Array16OfOffset32To<Condition>	conditions;
  खुला:
  DEFINE_आकार_ARRAY (2, conditions);
};

काष्ठा FeatureTableSubstitutionRecord
{
  मित्र काष्ठा FeatureTableSubstitution;

  व्योम collect_lookups (स्थिर व्योम *base, hb_set_t *lookup_indexes /* OUT */) स्थिर
  {
    वापस (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  व्योम closure_features (स्थिर व्योम *base,
			 स्थिर hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) स्थिर
  {
    अगर ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (feature_सूचकांक_2);
  }

  व्योम collect_feature_substitutes_with_variations (hb_hashmap_t<अचिन्हित, स्थिर Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    स्थिर hb_set_t *feature_indices,
                                                    स्थिर व्योम *base) स्थिर
  {
    अगर (feature_indices->has (feature_सूचकांक_2))
    {
      feature_substitutes_map->set (feature_सूचकांक_2, &(base+feature));
      catch_all_record_feature_idxes.add (feature_सूचकांक_2);
    }
  }

  bool serialize (hb_subset_layout_context_t *c,
                  अचिन्हित feature_सूचकांक,
                  स्थिर Feature *f, स्थिर Tag *tag)
  {
    TRACE_SERIALIZE (यह);
    hb_serialize_context_t *s = c->subset_context->serializer;
    अगर (unlikely (!s->extend_min (यह))) return_trace (false);

    uint32_t *new_feature_idx;
    अगर (!c->feature_map_w_duplicates->has (feature_सूचकांक, &new_feature_idx))
      return_trace (false);

    अगर (!s->check_assign (feature_सूचकांक_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    s->push ();
    bool ret = f->subset (c->subset_context, c, tag);
    अगर (ret) s->add_link (feature, s->निकालो_pack ());
    अन्यथा s->निकालो_discard ();

    return_trace (ret);
  }

  bool subset (hb_subset_layout_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SUBSET (यह);
    uint32_t *new_feature_सूचकांक;
    अगर (!c->feature_map_w_duplicates->has (feature_सूचकांक_2, &new_feature_सूचकांक))
      return_trace (false);

    स्वतः *out = c->subset_context->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    out->feature_सूचकांक_2 = *new_feature_सूचकांक;
    hb_pair_t<स्थिर व्योम*, स्थिर व्योम*> *original;
    अगर (!c->feature_idx_tag_map->has (feature_सूचकांक_2, &original))
      return_trace (false);
    स्थिर Tag *tag = reinterpret_cast<स्थिर Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base,
		 स्थिर FeatureList *feature_list) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!c->check_struct (यह))) return_trace (false);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    स्थिर Record_sanitize_closure_t closure = {feature_list->get_tag (feature_सूचकांक_2), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  रक्षित:
  HBUINT16		feature_सूचकांक_2;
  Offset32To<Feature>	feature;
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा FeatureTableSubstitution
{
  व्योम collect_feature_indices (hb_set_t *feature_indices) स्थिर
  {
    क्रम (स्थिर FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.feature_सूचकांक_2);
  }

  स्थिर Feature *find_substitute (अचिन्हित पूर्णांक feature_सूचकांक) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = substitutions.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      स्थिर FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      अगर (record.feature_सूचकांक_2 == feature_सूचकांक)
	वापस &(यह+record.feature);
    }
    वापस nullptr;
  }

  व्योम collect_lookups (स्थिर hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) स्थिर
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::feature_सूचकांक_2)
    | hb_apply ([यह, lookup_indexes] (स्थिर FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (यह, lookup_indexes); })
    ;
  }

  ढाँचा <typename TagGetter>
  व्योम collect_name_ids (स्थिर hb_map_t *feature_सूचकांक_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) स्थिर
  {
    क्रम (स्थिर FeatureTableSubstitutionRecord &record : substitutions)
      अगर (feature_सूचकांक_map->has (record.feature_सूचकांक_2))
        (यह+record.feature).collect_name_ids (get_tag (record.feature_सूचकांक_2), nameids_to_retain);
  }

  व्योम closure_features (स्थिर hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) स्थिर
  {
    क्रम (स्थिर FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (यह, lookup_indexes, feature_indexes);
  }

  bool intersects_features (स्थिर hb_map_t *feature_सूचकांक_map) स्थिर
  {
    क्रम (स्थिर FeatureTableSubstitutionRecord& record : substitutions)
    {
      अगर (feature_सूचकांक_map->has (record.feature_सूचकांक_2)) वापस true;
    }
    वापस false;
  }

  व्योम collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिर
  {
    क्रम (स्थिर FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, यह);
  }

  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               bool जोड़ो_catch_all) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;

    अगर (जोड़ो_catch_all)
    {
      क्रम (अचिन्हित feature_सूचकांक : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<स्थिर व्योम*, स्थिर व्योम*> *p;
        अगर (!l->feature_idx_tag_map->has (feature_सूचकांक, &p))
          return_trace (false);
        स्वतः *o = out->substitutions.serialize_append (c->serializer);
        अगर (!o->serialize (l, feature_सूचकांक,
                           reinterpret_cast<स्थिर Feature*> (p->first),
                           reinterpret_cast<स्थिर Tag*> (p->second)))
          return_trace (false);
      }
      return_trace (true);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), यह))
    ;

    return_trace (bool (out->substitutions));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर FeatureList *feature_list) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, यह, feature_list));
  }

  रक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  खुला:
  DEFINE_आकार_ARRAY (6, substitutions);
};

काष्ठा FeatureVariationRecord
{
  मित्र काष्ठा FeatureVariations;

  व्योम collect_lookups (स्थिर व्योम     *base,
			स्थिर hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) स्थिर
  {
    वापस (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  ढाँचा <typename TagGetter>
  व्योम collect_name_ids (स्थिर व्योम *base,
                         स्थिर hb_map_t *feature_सूचकांक_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) स्थिर
  {
    (base+substitutions).collect_name_ids (feature_सूचकांक_map, get_tag, nameids_to_retain);
  }

  व्योम closure_features (स्थिर व्योम     *base,
			 स्थिर hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) स्थिर
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  bool intersects_features (स्थिर व्योम *base, स्थिर hb_map_t *feature_सूचकांक_map) स्थिर
  {
    वापस (base+substitutions).intersects_features (feature_सूचकांक_map);
  }

  व्योम collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    स्थिर व्योम *base) स्थिर
  {
    (base+conditions).keep_with_variations (c);
    अगर (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = true; // set variations only once
    }
  }

  bool subset (hb_subset_layout_context_t *c, स्थिर व्योम *base,
               bool जोड़ो_catch_all = false) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->subset_context->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->लो (c->cur_feature_var_record_idx).लो () : nullptr;
    अगर (!जोड़ो_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    अन्यथा
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, जोड़ो_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, जोड़ो_catch_all);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base,
		 स्थिर FeatureList *feature_list) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  स्थिर व्योम *base) स्थिर
  { (base+conditions).collect_variation_indices (c); }

  रक्षित:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा Lookup_सूचकांक_List
{
  व्योम add_lookup_indexes_to (hb_set_t *lookup_indexes) स्थिर
  { lookupIndices.add_indexes_to (lookup_indexes); }

  bool intersects_lookup_indexes (स्थिर hb_map_t *lookup_indexes) स्थिर
  { वापस lookupIndices.intersects (lookup_indexes); }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);

    स्वतः it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_सूचकांक_map)
    | hb_map (l->lookup_सूचकांक_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (bool (out->lookupIndices));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (lookupIndices.sanitize (c));
  }

  रक्षित:
  सूचकांक_Array	lookupIndices;
  खुला:
  DEFINE_आकार_ARRAY_SIZED (2, lookupIndices);
};

काष्ठा LookupConditionRecord
{
  अचिन्हित get_condition_offset (स्थिर व्योम *base, स्थिर व्योम *variations) स्थिर
  { वापस (स्थिर अक्षर *) &(base+condition) - (स्थिर अक्षर *) variations; }

  ढाँचा <typename Instancer>
  bool evaluate (स्थिर व्योम *base,
		 स्थिर पूर्णांक *coords,
		 अचिन्हित पूर्णांक coord_len,
		 Instancer *instancer) स्थिर
  { वापस (base+condition).evaluate (coords, coord_len, instancer); }

  bool evaluate (स्थिर व्योम *base,
                 स्थिर व्योम *variations,
                 hb_sorted_array_t<स्थिर अचिन्हित> conditions) स्थिर
  {
    वापस !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.लंबाई,
                                     माप (अचिन्हित),
                                     _hb_cmp_operator<अचिन्हित, अचिन्हित>);
  }

  bool collect_condition (स्थिर व्योम *base,
                          स्थिर व्योम *variations,
                          स्थिर पूर्णांक *coords,
                          अचिन्हित पूर्णांक coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<अचिन्हित> *conditions) स्थिर
  {
    /* Null conditions always apply and need no key state. */
    अगर (!condition) वापस true;

    अचिन्हित offset = get_condition_offset (base, variations);
    अगर (visited->has (offset)) वापस true;
    visited->add (offset);
    अगर (unlikely (visited->in_error ())) वापस false;

    अगर (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    वापस !conditions->in_error ();
  }

  व्योम add_lookup_indexes_to (स्थिर व्योम *base,
			      hb_set_t *lookup_indexes) स्थिर
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  bool intersects_lookup_indexes (स्थिर व्योम *base,
				   स्थिर hb_map_t *lookup_indexes) स्थिर
  { वापस (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  स्थिर व्योम *base) स्थिर
  {
    अगर (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_मूल्यावली)))
      c->layout_variation_indices->err ();
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       स्थिर व्योम *base) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_परिणाम_t परिणाम =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    true);
    अगर (परिणाम != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->निकालो_discard ();
      return_trace (false);
    }
    c->serializer->add_link (out->condition, c->serializer->निकालो_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  रक्षित:
  Offset32To<Condition>		condition;
  Offset32To<Lookup_सूचकांक_List>	lookupIndices;
  खुला:
  DEFINE_आकार_STATIC (8);
};

काष्ठा FeatureLookupsTable
{
  bool adds_default_lookups () स्थिर { वापस flags & 0x0001u; }

  bool collect_conditions (स्थिर व्योम *variations,
                           स्थिर पूर्णांक *coords,
                           अचिन्हित पूर्णांक coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<अचिन्हित> *conditions) स्थिर
  {
    क्रम (स्थिर LookupConditionRecord &record : records)
      अगर (unlikely (!record.collect_condition (यह, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	वापस false;
    वापस true;
  }

  व्योम collect_lookups (स्थिर व्योम *variations,
                       hb_sorted_array_t<स्थिर अचिन्हित> conditions,
                       hb_set_t *lookup_indexes) स्थिर
  {
    क्रम (स्थिर LookupConditionRecord &record : records)
      अगर (record.evaluate (यह, variations, conditions))
	record.add_lookup_indexes_to (यह, lookup_indexes);
  }

  ढाँचा <typename Instancer>
  व्योम collect_lookups (स्थिर पूर्णांक *coords,
			अचिन्हित पूर्णांक coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) स्थिर
  {
    क्रम (स्थिर LookupConditionRecord &record : records)
      अगर (record.evaluate (यह, coords, coord_len, instancer))
	record.add_lookup_indexes_to (यह, lookup_indexes);
  }

  व्योम collect_lookups (hb_set_t *lookup_indexes) स्थिर
  {
    क्रम (स्थिर LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (यह, lookup_indexes);
  }

  bool intersects_lookup_indexes (स्थिर hb_map_t *lookup_indexes) स्थिर
  {
    क्रम (स्थिर LookupConditionRecord &record : records)
      अगर (record.intersects_lookup_indexes (यह, lookup_indexes))
	वापस true;
    वापस false;
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर LookupConditionRecord &record : records)
      record.collect_variation_indices (c, यह);
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    क्रम (स्थिर LookupConditionRecord &record : records)
    {
      स्वतः snap = c->serializer->snapshot ();
      अगर (record.subset (c, l, यह))
	out->records.len++;
      अन्यथा
	c->serializer->revert (snap);
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, यह));
  }

  रक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  खुला:
  DEFINE_आकार_ARRAY (10, records);
};

काष्ठा LookupVariationRecord
{
  मित्र काष्ठा FeatureVariations;

  अचिन्हित get_feature_सूचकांक () स्थिर { वापस feature_सूचकांक_2; }

  व्योम collect_lookups (स्थिर व्योम *base,
                       स्थिर Feature &feature,
                       hb_sorted_array_t<स्थिर अचिन्हित> conditions,
                       hb_set_t *lookup_indexes) स्थिर
  {
    स्थिर FeatureLookupsTable &feature_lookups = base+featureLookups;
    अगर (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  व्योम collect_lookups (स्थिर व्योम *base,
			स्थिर hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) स्थिर
  {
    अगर (feature_indexes->has (feature_सूचकांक_2))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  व्योम closure_features (स्थिर व्योम *base,
			 स्थिर hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) स्थिर
  {
    अगर ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (feature_सूचकांक_2);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  स्थिर व्योम *base) स्थिर
  { (base+featureLookups).collect_variation_indices (c); }

  bool subset (hb_subset_layout_context_t *l, स्थिर व्योम *base) स्थिर
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (यह);
    uint32_t *new_feature_सूचकांक;
    अगर (!l->feature_map_w_duplicates->has (feature_सूचकांक_2, &new_feature_सूचकांक))
      return_trace (false);

    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out ||
		  !c->serializer->check_assign (out->feature_सूचकांक_2,
						*new_feature_सूचकांक,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर व्योम *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  featureLookups.sanitize (c, base));
  }

  रक्षित:
  HBUINT16			feature_सूचकांक_2;
  Offset32To<FeatureLookupsTable>	featureLookups;
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा FeatureVariations
{
  अटल constexpr अचिन्हित NOT_FOUND_सूचकांक = 0xFFFFFFFFu;

  स्थिर Array32Of<LookupVariationRecord> &get_lookup_variation_records () स्थिर
  {
    अगर (version.to_int () < 0x00010001u)
      वापस Null (Array32Of<LookupVariationRecord>);
    वापस StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  अचिन्हित record_गिनती () स्थिर
  {
    वापस varRecords.len;
  }

  bool find_सूचकांक (स्थिर पूर्णांक *coords, अचिन्हित पूर्णांक coord_len,
		   अचिन्हित पूर्णांक *सूचकांक_2,
		   ItemVarStoreInstancer *instancer) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = varRecords.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      स्थिर FeatureVariationRecord &record = varRecords.arrayZ[i];
      अगर ((यह+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*सूचकांक_2 = i;
	वापस true;
      }
    }
    *सूचकांक_2 = NOT_FOUND_सूचकांक;
    वापस false;
  }

  स्थिर Feature *find_substitute (अचिन्हित पूर्णांक variations_सूचकांक,
				  अचिन्हित पूर्णांक feature_सूचकांक) स्थिर
  {
    स्थिर FeatureVariationRecord &record = varRecords[variations_सूचकांक];
    वापस (यह+record.substitutions).find_substitute (feature_सूचकांक);
  }

  bool collect_lookup_conditions (स्थिर पूर्णांक *coords,
                                  अचिन्हित पूर्णांक coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<अचिन्हित> *state) स्थिर
  {
    hb_set_t visited_tables, visited_conditions;
    क्रम (स्थिर LookupVariationRecord &record : get_lookup_variation_records ())
    {
      अचिन्हित offset = record.featureLookups;
      अगर (visited_tables.has (offset)) जारी;
      visited_tables.add (offset);
      अगर (unlikely (visited_tables.in_error ())) वापस false;

      स्थिर FeatureLookupsTable &feature_lookups = यह+record.featureLookups;
      अगर (unlikely (!feature_lookups.collect_conditions (यह, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	वापस false;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->क्विक (_hb_cmp_operator<अचिन्हित, अचिन्हित>);
    वापस true;
  }

  bool get_lookup_variations (अचिन्हित feature_सूचकांक,
                              स्थिर Feature &feature,
                              hb_sorted_array_t<स्थिर अचिन्हित> conditions,
                              hb_set_t *lookup_indexes) स्थिर
  {
    क्रम (स्थिर LookupVariationRecord &record : get_lookup_variation_records ())
    {
      अगर (record.feature_सूचकांक_2 != feature_सूचकांक) जारी;

      record.collect_lookups (यह, feature, conditions, lookup_indexes);
      वापस true;
    }
    वापस false;
  }

  व्योम collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = varRecords.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, यह);
      अगर (c->universal)
        अवरोध;
    }
    अगर (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed (*यह));
  }

  व्योम collect_lookups (स्थिर hb_set_t *feature_indexes,
			स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) स्थिर
  {
    अचिन्हित गिनती = varRecords.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        जारी;
      varRecords[i].collect_lookups (यह, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  ढाँचा <typename TagGetter>
  व्योम collect_name_ids (स्थिर hb_map_t *feature_सूचकांक_map,
                         स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) स्थिर
  {
    क्रम (अचिन्हित i = 0; i < varRecords.len; i++)
    {
      अगर (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        जारी;
      varRecords[i].collect_name_ids (यह, feature_सूचकांक_map, get_tag, nameids_to_retain);
    }
  }

  व्योम collect_lookup_variation_lookups (स्थिर hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) स्थिर
  {
    क्रम (स्थिर LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (यह, feature_indexes, lookup_indexes);
  }

  व्योम closure_features (स्थिर hb_map_t *lookup_indexes,
			 स्थिर hb_hashmap_t<अचिन्हित, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) स्थिर
  {
    अचिन्हित पूर्णांक गिनती = varRecords.len;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
    {
      अगर (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        जारी;
      varRecords[i].closure_features (यह, lookup_indexes, feature_indexes);
    }

    क्रम (स्थिर LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (यह, lookup_indexes, feature_indexes);
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    क्रम (स्थिर FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, यह);
    क्रम (स्थिर LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, यह);
  }

  व्योम collect_feature_indices (hb_set_t *feature_indices) स्थिर
  {
    क्रम (स्थिर FeatureVariationRecord &record : varRecords)
      (यह+record.substitutions).collect_feature_indices (feature_indices);
  }

  bool has_lookup_variations (अचिन्हित feature_सूचकांक) स्थिर
  {
    क्रम (स्थिर LookupVariationRecord &record : get_lookup_variation_records ())
    {
      अगर (record.feature_सूचकांक_2 == feature_सूचकांक)
	वापस true;
      अगर (record.feature_सूचकांक_2 > feature_सूचकांक)
	अवरोध;
    }
    वापस false;
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) स्थिर
  {
    TRACE_SUBSET (यह);
    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;

    पूर्णांक keep_up_to = -1;
    क्रम (पूर्णांक i = varRecords.len - 1; i >= 0; i--) {
      अगर (varRecords[i].intersects_features (यह, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        अवरोध;
      }
    }

    अचिन्हित गिनती = (अचिन्हित) (keep_up_to + 1);
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      अगर (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        जारी;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), यह) (varRecords[i]);
    }

    अगर (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      bool जोड़ो_catch_all_record = true;
      subset_record_array (l, &(out->varRecords), यह, जोड़ो_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    अगर (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      अगर (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (false);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, यह))
      ;
    }

    return_trace (bool (out->varRecords) ||
		  (out_lookup_records && bool (*out_lookup_records)));
  }

  bool sanitize (hb_sanitize_context_t *c, स्थिर FeatureList *feature_list) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, यह, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, यह)));
  }

  रक्षित:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  खुला:
  DEFINE_आकार_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

काष्ठा HintingDevice
{
  मित्र काष्ठा Device;

  निजी:

  hb_position_t get_x_delta (hb_font_t *font) स्थिर
  { वापस get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) स्थिर
  { वापस get_delta (font->y_ppem, font->y_scale); }

  खुला:

  माप_प्रकार get_आकार () स्थिर
  {
    अचिन्हित पूर्णांक f = deltaFormat;
    अगर (unlikely (f < 1 || f > 3 || start_आकार > end_आकार)) वापस 3 * HBUINT16::static_आकार;
    वापस HBUINT16::static_आकार * (4 + ((end_आकार - start_आकार) >> (4 - f)));
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) && c->check_range (यह, यह->get_आकार ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);
    return_trace (c->embed<HintingDevice> (यह));
  }

  निजी:

  पूर्णांक get_delta (अचिन्हित पूर्णांक ppem, पूर्णांक scale) स्थिर
  {
    अगर (!ppem) वापस 0;

    पूर्णांक pixels = get_delta_pixels (ppem);

    अगर (!pixels) वापस 0;

    वापस (पूर्णांक) (pixels * (int64_t) scale / ppem);
  }
  पूर्णांक get_delta_pixels (अचिन्हित पूर्णांक ppem_आकार) स्थिर
  {
    अचिन्हित पूर्णांक f = deltaFormat;
    अगर (unlikely (f < 1 || f > 3))
      वापस 0;

    अगर (ppem_आकार < start_आकार || ppem_आकार > end_आकार)
      वापस 0;

    अचिन्हित पूर्णांक s = ppem_आकार - start_आकार;

    अचिन्हित पूर्णांक byte = delta_मूल्य_Z[s >> (4 - f)];
    अचिन्हित पूर्णांक bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    अचिन्हित पूर्णांक mask = (0xFFFFu >> (16 - (1 << f)));

    पूर्णांक delta = bits & mask;

    अगर ((अचिन्हित पूर्णांक) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    वापस delta;
  }

  रक्षित:
  HBUINT16	start_आकार;		/* Smallest size to correct--in ppem */
  HBUINT16	end_आकार;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_मूल्य_Z;		/* Array of compressed data */
  खुला:
  DEFINE_आकार_ARRAY (6, delta_मूल्य_Z);
};

काष्ठा VariationDevice
{
  मित्र काष्ठा Device;

  निजी:

  hb_position_t get_x_delta (hb_font_t *font,
			     स्थिर ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) स्थिर
  { वापस !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     स्थिर ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) स्थिर
  { वापस !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  भग्न get_x_delta_f (hb_font_t *font,
		       स्थिर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) स्थिर
  { वापस !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  भग्न get_y_delta_f (hb_font_t *font,
		       स्थिर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) स्थिर
  { वापस !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map) स्थिर
  {
    TRACE_SERIALIZE (यह);
    अगर (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<अचिन्हित, पूर्णांक> *v;
    अगर (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    अगर (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
      return_trace (nullptr);

    c->start_zerocopy (यह->static_आकार);
    स्वतः *out = c->embed (यह);
    अगर (unlikely (!out)) return_trace (nullptr);

    अगर (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  व्योम collect_variation_सूचकांक (hb_collect_variation_indices_context_t *c) स्थिर
  { c->layout_variation_indices->add (varIdx); }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह));
  }

  निजी:

  भग्न get_delta (hb_font_t *font,
		   स्थिर ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    वापस store.get_delta (varIdx, font->coords, font->अंक_coords, store_cache);
  }

  रक्षित:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा DeviceHeader
{
  रक्षित:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  खुला:
  HBUINT16		format;		/* Format identifier */
  खुला:
  DEFINE_आकार_STATIC (6);
};

काष्ठा Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  भग्न get_x_delta_f (hb_font_t *font,
		       स्थिर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    अगर (u.b.format == 0x8000)
    {
      hb_barrier ();
      वापस u.variation.get_x_delta_f (font, store, store_cache);
    }
#पूर्ण_अगर
    वापस get_x_delta (font, store, store_cache);
  }

  भग्न get_y_delta_f (hb_font_t *font,
		       स्थिर ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
#अगर_अघोषित HB_NO_VAR
    अगर (u.b.format == 0x8000)
    {
      hb_barrier ();
      वापस u.variation.get_y_delta_f (font, store, store_cache);
    }
#पूर्ण_अगर
    वापस get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     स्थिर ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    चयन (u.b.format)
    {
#अगर_अघोषित HB_NO_HINTING
    हाल 1: हाल 2: हाल 3:
      hb_barrier ();
      वापस u.hinting.get_x_delta (font);
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      hb_barrier ();
      वापस u.variation.get_x_delta (font, store, store_cache);
#पूर्ण_अगर
    शेष:
      वापस 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     स्थिर ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) स्थिर
  {
    चयन (u.b.format)
    {
    हाल 1: हाल 2: हाल 3:
#अगर_अघोषित HB_NO_HINTING
      hb_barrier ();
      वापस u.hinting.get_y_delta (font);
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      hb_barrier ();
      वापस u.variation.get_y_delta (font, store, store_cache);
#पूर्ण_अगर
    शेष:
      वापस 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (!u.b.format.sanitize (c)) return_trace (false);
    चयन (u.b.format) {
#अगर_अघोषित HB_NO_HINTING
    हाल 1: हाल 2: हाल 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#पूर्ण_अगर
    शेष:
      return_trace (true);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map=nullptr) स्थिर
  {
    TRACE_SERIALIZE (यह);
    चयन (u.b.format) {
#अगर_अघोषित HB_NO_HINTING
    हाल 1:
    हाल 2:
    हाल 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#पूर्ण_अगर
    शेष:
      return_trace (nullptr);
    }
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    चयन (u.b.format) {
#अगर_अघोषित HB_NO_HINTING
    हाल 1:
    हाल 2:
    हाल 3:
      वापस;
#पूर्ण_अगर
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      hb_barrier ();
      u.variation.collect_variation_सूचकांक (c);
      वापस;
#पूर्ण_अगर
    शेष:
      वापस;
    }
  }

  अचिन्हित get_variation_सूचकांक () स्थिर
  {
    चयन (u.b.format) {
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      hb_barrier ();
      वापस u.variation.varIdx;
#पूर्ण_अगर
    शेष:
      वापस HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक;
    }
  }

  bool is_variation_device () स्थिर
  {
    चयन (u.b.format) {
#अगर_अघोषित HB_NO_VAR
    हाल 0x8000:
      वापस true;
#पूर्ण_अगर
    शेष:
      वापस false;
    }
  }

  रक्षित:
  जोड़ {
  DeviceHeader		b;
  HintingDevice		hinting;
#अगर_अघोषित HB_NO_VAR
  VariationDevice	variation;
#पूर्ण_अगर
  } u;
  खुला:
  DEFINE_आकार_UNION (6, b);
};


} /* namespace OT */


#पूर्ण_अगर /* HB_OT_LAYOUT_COMMON_HH */

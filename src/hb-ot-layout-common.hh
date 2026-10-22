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

#ضمن "hb.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-set.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-cache.hh"
#ضمن "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (حجم (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_ثنائی_cache_t = hb_cache_t<14, 1, 8>;
static_assert (حجم (hb_ot_layout_ثنائی_cache_t) == 256, "");

#ضمن "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


نطاق OT {

template<typename Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ساكن منطقي ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_set_t &klasses,
    منطقي use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

بنية ItemVarStoreInstancer;

بنية hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_index_tag_map;
  ثابت hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  منطقي apply;
  منطقي variation_applied;
  منطقي universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  منطقي all_axes_pinned;
};

بنية hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت فراغ         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_گنتی (0),langsys_feature_گنتی (0) {}

  منطقي visitScript ()
  { أرجع script_گنتی++ < HB_MAX_SCRIPTS; }

  منطقي visitLangsys (unsigned feature_گنتی_2)
  {
    langsys_feature_گنتی += feature_گنتی_2;
    أرجع langsys_feature_گنتی < HB_MAX_LANGSYS_FEATURE_گنتی;
  }

  عام:
  ثابت فراغ *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  خاص:
  unsigned script_گنتی;
  unsigned langsys_feature_گنتی;
};

بنية hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { أرجع "SUBSET_LAYOUT"; }
  ساكن return_t default_return_قدر () { أرجع hb_empty_t (); }

  منطقي visitScript ()
  {
    أرجع script_گنتی++ < HB_MAX_SCRIPTS;
  }

  منطقي visitLangSys ()
  {
    أرجع langsys_گنتی++ < HB_MAX_LANGSYS;
  }

  منطقي visitFeatureIndex (صحيح گنتی)
  {
    feature_index_گنتی += گنتی;
    أرجع feature_index_گنتی < HB_MAX_FEATURE_INDICES;
  }

  منطقي visitLookupIndex()
  {
    lookup_index_گنتی++;
    أرجع lookup_index_گنتی < HB_MAX_LOOKUP_VISIT_گنتی;
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
  ثابت hb_hashmap_t<unsigned, hb_pair_t<ثابت فراغ*, ثابت فراغ*>> *feature_idx_tag_map;

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
				script_گنتی (0),
				langsys_گنتی (0),
				feature_index_گنتی (0),
				lookup_index_گنتی (0)
  {
    إذا (tag_ == HB_OT_TAG_GSUB)
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
    وإلا
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

  خاص:
  unsigned script_گنتی;
  unsigned langsys_گنتی;
  unsigned feature_index_گنتی;
  unsigned lookup_index_گنتی;
};

بنية ItemVariationStore;
بنية hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (هذا); أرجع hb_empty_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  ثابت hb_set_t *glyph_set;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, صحيح>> *condition_اقدار = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  ثابت hb_set_t *glyph_set_,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
بنية subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 ثابت فراغ *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  منطقي operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, offset, base);
    إذا (!ret)
    {
      out.نکالو ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ثابت فراغ *base;
};


template<typename OutputArray, typename Arg>
بنية subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     ثابت فراغ *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  منطقي operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, offset, base, arg);
    إذا (!ret)
    {
      out.نکالو ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  ثابت فراغ *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
بنية
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ثابت فراغ *base) ثابت
  { أرجع subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       ثابت فراغ *base, Arg &&arg) ثابت
  { أرجع subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
بنية subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 ثابت فراغ *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  فراغ
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = record.subset (subset_layout_context, base);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا out->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ثابت فراغ *base;
};

template<typename OutputArray, typename Arg>
بنية subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     ثابت فراغ *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  فراغ
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = record.subset (subset_layout_context, base, arg);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا out->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  ثابت فراغ *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
بنية
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       ثابت فراغ *base) ثابت
  { أرجع subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               ثابت فراغ *base, Arg &&arg) ثابت
  { أرجع subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
بنية serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         ثابت فراغ *base_,
                         ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  منطقي operator () (T&& record)
  {
    إذا (!serialize_context->copy (record, base, variation_map)) أرجع خطأ;
    out.len++;
    أرجع true;
  }

  خاص:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  ثابت فراغ *base;
  ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
بنية
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               ثابت فراغ *base,
               ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *variation_map) ثابت
  { أرجع serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

بنية IndexArray : Array16Of<Index>
{
  منطقي intersects (ثابت hb_map_t *indexes) ثابت
  { أرجع hb_any (*هذا, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    إذا (!it) أرجع;
    إذا (unlikely (!c->extend_min ((*هذا)))) أرجع;

    لكل (ثابت auto _ : it)
    {
      إذا (!l->visitLookupIndex()) اكسر;

      Index i;
      i = _;
      c->copy (i);
      هذا->len++;
    }
  }

  unsigned صحيح get_indexes (unsigned صحيح start_offset,
			    unsigned صحيح *گنتی_2 /* IN/OUT */,
			    unsigned صحيح *_indexes /* OUT */) ثابت
  {
    إذا (گنتی_2 && _indexes)
    {
      + هذا->as_array ().sub_array (start_offset, گنتی_2)
      | hb_sink (hb_array (_indexes, *گنتی_2))
      ;
    }
    أرجع هذا->len;
  }

  فراغ add_indexes_to (hb_set_t* output /* OUT */) ثابت
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
بنية FeatureParamsSize
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
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

    إذا (!designSize)
      return_trace (خطأ);
    وإلا إذا (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (true);
    وإلا إذا (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (خطأ);
    وإلا
      return_trace (true);
  }

  فراغ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ثابت
  { nameids_to_retain->add (subfamilyNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
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
  عام:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
بنية FeatureParamsStylisticSet
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (هذا));
  }

  فراغ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ثابت
  { nameids_to_retain->add (uiNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
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
  عام:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
بنية FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_گنتی, hb_codepoint_t *chars) ثابت
  {
    إذا (char_گنتی && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_گنتی)
      | hb_sink (hb_array (chars, *char_گنتی))
      ;
    }
    أرجع characters.len;
  }

  size_t get_size () ثابت
  { أرجع hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  فراغ collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    إذا (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    إذا (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    إذا (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    إذا (!firstParamUILabelNameID || !عدد_Named_Parameters || عدد_Named_Parameters >= 0x7FFF)
      أرجع;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) عدد_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
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
  HBUINT16	عدد_Named_Parameters;	/* Number of named parameters. (May
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
  عام:
  DEFINE_SIZE_ARRAY (14, characters);
};

بنية FeatureParams
{
  منطقي sanitize (hb_sanitize_context_t *c, hb_tag_t tag) ثابت
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع true;
#endif
    TRACE_SANITIZE (هذا);
    إذا (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    إذا ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    إذا ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (true);
  }

  فراغ collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع;
#endif
    إذا (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      أرجع (u.size.collect_name_ids (nameids_to_retain));
    إذا ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      أرجع (u.stylisticSet.collect_name_ids (nameids_to_retain));
    إذا ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      أرجع (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  منطقي subset (hb_subset_context_t *c, ثابت Tag* tag) ثابت
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع خطأ;
#endif
    TRACE_SUBSET (هذا);
    إذا (!tag) return_trace (خطأ);
    إذا (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    إذا ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    إذا ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (خطأ);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت FeatureParamsSize& get_size_params (hb_tag_t tag) ثابت
  {
    إذا (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      أرجع u.size;
    أرجع Null (FeatureParamsSize);
  }
  ثابت FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) ثابت
  {
    إذا ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      أرجع u.stylisticSet;
    أرجع Null (FeatureParamsStylisticSet);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) ثابت
  {
    إذا ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      أرجع u.characterVariants;
    أرجع Null (FeatureParamsCharacterVariants);
  }
#endif

  خاص:
  اتحاد {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عام:
  DEFINE_SIZE_MIN (0);
};

بنية Record_sanitize_closure_t {
  hb_tag_t tag;
  ثابت فراغ *list_base;
};

بنية Feature
{
  unsigned صحيح get_lookup_گنتی () ثابت
  { أرجع lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned صحيح i) ثابت
  { أرجع lookupIndex[i]; }
  unsigned صحيح get_lookup_indexes (unsigned صحيح start_index,
				   unsigned صحيح *lookup_گنتی /* IN/OUT */,
				   unsigned صحيح *lookup_tags /* OUT */) ثابت
  { أرجع lookupIndex.get_indexes (start_index, lookup_گنتی, lookup_tags); }
  فراغ add_lookup_indexes_to (hb_set_t *lookup_indexes) ثابت
  { lookupIndex.add_indexes_to (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { أرجع هذا+featureParams; }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookupIndex.intersects (lookup_indexes); }

  فراغ collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) ثابت
  {
    إذا (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت Tag                   *tag = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    out->featureParams.serialize_subset (c, featureParams, هذا, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    إذا (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    وإلا
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
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t *closure = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureParams.sanitize (c, هذا, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  عام:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
بنية Record
{
  صحيح cmp (hb_tag_t a) ثابت { أرجع tag.cmp (a); }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *base, ثابت فراغ *f_sub = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    منطقي ret = f.subset (c->subset_context, c, &tag);
    إذا (ret)
      s->add_link (out->offset, s->نکالو_pack ());
    وإلا
      s->نکالو_discard ();

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (هذا) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  عام:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
بنية RecordArrayOf : SortedArray16Of<Record<Type>>
{
  ثابت Offset16To<Type>& get_offset (unsigned صحيح i) ثابت
  { أرجع (*هذا)[i].offset; }
  Offset16To<Type>& get_offset (unsigned صحيح i)
  { أرجع (*هذا)[i].offset; }
  ثابت Tag& get_tag (unsigned صحيح i) ثابت
  { أرجع (*هذا)[i].tag; }
  unsigned صحيح get_tags (unsigned صحيح start_offset,
                         unsigned صحيح *record_گنتی /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) ثابت
  {
    إذا (record_گنتی && record_tags)
    {
      + هذا->as_array ().sub_array (start_offset, record_گنتی)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_گنتی))
      ;
    }
    أرجع هذا->len;
  }
  منطقي find_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  {
    أرجع هذا->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
بنية RecordListOf : RecordArrayOf<Type>
{
  ثابت Type& operator [] (unsigned صحيح i) ثابت
  { أرجع هذا+هذا->get_offset (i); }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    + هذا->iter ()
    | hb_apply (subset_record_array (l, out, هذا))
    ;
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (RecordArrayOf<Type>::sanitize (c, هذا));
  }
};

بنية RecordListOfFeature : RecordListOf<Feature>
{
  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, هذا] (ثابت hb_pair_t<unsigned, ثابت Record<Feature>&>& _)
                {
                  ثابت Feature *f_sub = nullptr;
                  ثابت Feature **f = nullptr;
                  إذا (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, هذا, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (true);
  }
};

تعريفنوع RecordListOf<Feature> FeatureList;


بنية LangSys
{
  unsigned صحيح get_feature_گنتی () ثابت
  { أرجع featureIndex.len; }
  hb_tag_t get_feature_index (unsigned صحيح i) ثابت
  { أرجع featureIndex[i]; }
  unsigned صحيح get_feature_indexes (unsigned صحيح start_offset,
				    unsigned صحيح *feature_گنتی_2 /* IN/OUT */,
				    unsigned صحيح *feature_indexes /* OUT */) ثابت
  { أرجع featureIndex.get_indexes (start_offset, feature_گنتی_2, feature_indexes); }
  فراغ add_feature_indexes_to (hb_set_t *feature_indexes) ثابت
  { featureIndex.add_indexes_to (feature_indexes); }

  منطقي has_required_feature () ثابت { أرجع reqFeatureIndex != 0xFFFFu; }
  unsigned صحيح get_required_feature_index () ثابت
  {
    إذا (reqFeatureIndex == 0xFFFFu)
      أرجع Index::NOT_FOUND_INDEX;
   أرجع reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  منطقي compare (ثابت LangSys& o, ثابت hb_map_t *feature_index_map) ثابت
  {
    إذا (reqFeatureIndex != o.reqFeatureIndex)
      أرجع خطأ;

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

    لكل (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      إذا (a != b) أرجع خطأ;
    }

    إذا (iter || o_iter) أرجع خطأ;

    أرجع true;
  }

  فراغ collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    إذا (!has_required_feature () && !get_feature_گنتی ()) أرجع;
    إذا (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت Tag                  *tag = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    ثابت uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    إذا (!l->visitFeatureIndex (featureIndex.len))
      return_trace (خطأ);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقي ret = منطقي (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  عام:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

بنية Script
{
  unsigned صحيح get_lang_sys_گنتی () ثابت
  { أرجع langSys.len; }
  ثابت Tag& get_lang_sys_tag (unsigned صحيح i) ثابت
  { أرجع langSys.get_tag (i); }
  unsigned صحيح get_lang_sys_tags (unsigned صحيح start_offset,
				  unsigned صحيح *lang_sys_گنتی /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) ثابت
  { أرجع langSys.get_tags (start_offset, lang_sys_گنتی, lang_sys_tags); }
  ثابت LangSys& get_lang_sys (unsigned صحيح i) ثابت
  {
    إذا (i == Index::NOT_FOUND_INDEX) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].offset;
  }
  منطقي find_lang_sys_index (hb_tag_t tag, unsigned صحيح *index) ثابت
  { أرجع langSys.find_index (tag, index); }

  منطقي has_default_lang_sys () ثابت           { أرجع defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  فراغ prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) ثابت
  {
    إذا (!has_default_lang_sys () && !get_lang_sys_گنتی ()) أرجع;
    إذا (!c->visitScript ()) أرجع;

    إذا (!c->script_langsys_map->has (script_index))
    {
      إذا (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	أرجع;
    }

    إذا (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      إذا (c->visitLangsys (d.get_feature_گنتی ())) {
        d.collect_features (c);
      }

      لكل (auto _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.offset;
        إذا (!c->visitLangsys (l.get_feature_گنتی ())) استمر;
        إذا (l.compare (d, c->duplicate_feature_map)) استمر;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    وإلا
    {
      لكل (auto _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.offset;
        إذا (!c->visitLangsys (l.get_feature_گنتی ())) استمر;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت Tag                   *tag) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!l->visitScript ()) return_trace (خطأ);
    إذا (tag && !c->plan->layout_scripts.has (*tag))
      أرجع خطأ;

    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    منطقي defaultLang = خطأ;
    إذا (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = هذا+defaultLangSys;
      منطقي ret = ls.subset (c, l);
      إذا (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->نکالو_discard ();
	out->defaultLangSys = 0;
      }
      وإلا
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->نکالو_pack ());
	defaultLang = true;
      }
    }

    ثابت hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    إذا (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (ثابت Record<LangSys>& record) {أرجع l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), هذا))
      ;
    }

    return_trace (منطقي (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت Record_sanitize_closure_t * = nullptr) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (defaultLangSys.sanitize (c, هذا) && langSys.sanitize (c, هذا));
  }

  محمي:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

بنية RecordListOfScript : RecordListOf<Script>
{
  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    لكل (auto _ : + hb_enumerate (*هذا))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      منطقي ret = _.second.subset (l, هذا);
      إذا (!ret) c->serializer->revert (snap);
      وإلا out->len++;
    }

    return_trace (true);
  }
};

تعريفنوع RecordListOfScript ScriptList;



بنية LookupFlag : HBUINT16
{
  تعداد Flags {
    دایاں_To_بایاں		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  عام:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
نطاق OT {

بنية Lookup
{
  unsigned صحيح get_subtable_گنتی () ثابت { أرجع subTable.len; }

  template <typename TSubTable>
  ثابت Array16OfOffset16To<TSubTable>& get_subtables () ثابت
  { أرجع reinterpret_cast<ثابت Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { أرجع reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  ثابت TSubTable& get_subtable (unsigned صحيح i) ثابت
  { أرجع هذا+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned صحيح i)
  { أرجع هذا+get_subtables<TSubTable> ()[i]; }

  size_t get_size () ثابت
  {
    ثابت HBUINT16 &markFilteringSet = StructAfter<ثابت HBUINT16> (subTable);
    إذا (lookupFlag & LookupFlag::UseMarkFilteringSet)
      أرجع (ثابت حرف *) &StructAfter<ثابت حرف> (markFilteringSet) - (ثابت حرف *) هذا;
    أرجع (ثابت حرف *) &markFilteringSet - (ثابت حرف *) هذا;
  }

  unsigned صحيح get_type () ثابت { أرجع lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    unsigned صحيح flag = lookupFlag;
    إذا (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      ثابت HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    أرجع flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    unsigned صحيح lookup_type = get_type ();
    TRACE_DISPATCH (هذا, lookup_type);
    unsigned صحيح گنتی = get_subtable_گنتی ();
    لكل (unsigned صحيح i = 0; i < گنتی; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      إذا (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_قدر ());
  }

  منطقي serialize (hb_serialize_context_t *c,
		  unsigned صحيح lookup_type,
		  uint32_t lookup_props,
		  unsigned صحيح عدد_subtables)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    إذا (unlikely (!subTable.serialize (c, عدد_subtables))) return_trace (خطأ);
    إذا (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (true);
  }

  template <typename TSubTable>
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    ثابت hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned صحيح lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([هذا, glyphset, lookup_type] (ثابت Offset16To<TSubTable> &_) { أرجع (هذا+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), هذا, lookup_type))
    ;

    إذا (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ثابت HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      إذا (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      وإلا
      {
        إذا (unlikely (!c->serializer->extend (out))) return_trace (خطأ);
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

  template <typename TSubTable>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) && subTable.sanitize (c))) return_trace (خطأ);
    hb_barrier ();

    unsigned subtables = get_subtable_گنتی ();
    إذا (unlikely (!c->visit_subtables (subtables))) return_trace (خطأ);

    إذا (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      ثابت HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      إذا (!markFilteringSet.sanitize (c)) return_trace (خطأ);
    }

    إذا (unlikely (!get_subtables<TSubTable> ().sanitize (c, هذا, get_type ())))
      return_trace (خطأ);

    إذا (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned صحيح type = get_subtable<TSubTable> (0).u.extension.get_type ();
      لكل (unsigned صحيح i = 1; i < subtables; i++)
	إذا (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (خطأ);
    }
    return_trace (true);
  }

  محمي:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عام:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
بنية LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, هذا))
    ;
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, هذا)));
  }
};


/*
 * Coverage Table
 */


ساكن منطقي ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_set_t &klasses,
                                          منطقي use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  إذا (!klass_map)
    أرجع ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  إذا (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  لكل (ثابت unsigned k: klasses)
  {
    إذا (klass_map->has (k)) استمر;
    klass_map->set (k, idx);
    idx++;
  }


  إذا (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    أرجع خطأ;

  لكل (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  أرجع ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
بنية ClassDefFormat1_3
{
  friend بنية ClassDef;

  خاص:
  unsigned صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع class_قدر[(unsigned صحيح) (glyph_id - startGlyph)];
  }

  unsigned get_population () ثابت
  {
    أرجع class_قدر.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_قدر.len = 0;
      return_trace (true);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_گنتی = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    إذا (unlikely (!class_قدر.serialize (c, glyph_گنتی))) return_trace (خطأ);
    لكل (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_قدر[idx] = gid_klass_pair.second;
    }
    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               منطقي keep_empty_table = true,
               منطقي use_class_zero = true,
               ثابت Coverage* glyph_filter = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_قدر.len;

    لكل (ثابت hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
      إذا (glyph_filter && !glyph_filter->has(gid)) استمر;

      unsigned klass = class_قدر[gid - start];
      إذا (!klass) استمر;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    إذا (use_class_zero)
    {
      ثابت hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_گنتی = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_گنتی <= glyph_and_klass.length;
    }
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_empty_table || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && class_قدر.sanitize (c));
  }

  unsigned cost () ثابت { أرجع 1; }

  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  {
    unsigned صحيح start = 0;
    unsigned صحيح گنتی = class_قدر.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (class_قدر[i])
	استمر;

      إذا (start != i)
	إذا (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  أرجع خطأ;

      start = i + 1;
    }
    إذا (start != گنتی)
      إذا (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + گنتی)))
	أرجع خطأ;

    أرجع true;
  }

  template <typename set_t>
  منطقي collect_class (set_t *glyphs, unsigned klass) ثابت
  {
    unsigned صحيح گنتی = class_قدر.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      إذا (class_قدر[i] == klass) glyphs->add (startGlyph + i);
    أرجع true;
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_قدر.len;
    لكل (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      إذا (class_قدر[iter - start]) أرجع true;
    أرجع خطأ;
  }
  منطقي intersects_class (ثابت hb_set_t *glyphs, unsigned klass) ثابت
  {
    unsigned صحيح گنتی = class_قدر.len;
    إذا (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_قدر_INVALID;
      إذا (!glyphs->next (&g)) أرجع خطأ;
      إذا (g < startGlyph) أرجع true;
      g = startGlyph + گنتی - 1;
      إذا (glyphs->next (&g)) أرجع true;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت typename Types::HBUINT *arr = class_قدر.arrayZ;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      إذا (arr[i] == klass && glyphs->has (startGlyph + i))
	أرجع true;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ثابت
  {
    unsigned گنتی = class_قدر.len;
    إذا (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      لكل (uint32_t g = HB_SET_قدر_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      لكل (uint32_t g = startGlyph + گنتی - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      أرجع;
    }

    لكل (unsigned i = 0; i < گنتی; i++)
      إذا (class_قدر[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#إذا 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + گنتی;
    لكل (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      إذا (class_قدر.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  فراغ intersected_classes (ثابت hb_set_t *glyphs, hb_set_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    إذا (!class_قدر.len)
    {
      intersect_classes->add (0);
      أرجع;
    }

    hb_codepoint_t end_glyph = startGlyph + class_قدر.len - 1;
    إذا (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    لكل (ثابت auto& _ : + hb_enumerate (class_قدر))
    {
      hb_codepoint_t g = startGlyph + _.first;
      إذا (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_قدر;	/* Array of Class Values--one per GlyphID */
  عام:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_قدر);
};

template <typename Types>
بنية ClassDefFormat2_4
{
  friend بنية ClassDef;

  خاص:
  unsigned صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () ثابت
  {
    typename Types::large_int ret = 0;
    لكل (ثابت auto &r : rangeRecord)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (true);
    }

    unsigned unsorted = خطأ;
    unsigned عدد_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    إذا (unlikely (!record)) return_trace (خطأ);

    لكل (ثابت auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      إذا (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	إذا (unlikely (cur_gid < prev_gid))
	  unsorted = true;

	إذا (unlikely (!record)) اكسر;
	record->last = prev_gid;
	عدد_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    إذا (unlikely (c->in_error ())) return_trace (خطأ);

    إذا (likely (record)) record->last = prev_gid;
    rangeRecord.len = عدد_ranges;

    إذا (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (true);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               منطقي keep_empty_table = true,
               منطقي use_class_zero = true,
               ثابت Coverage* glyph_filter = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    إذا (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      لكل (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	إذا (!klass) استمر;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
	إذا (glyph_filter && !glyph_filter->has (g)) استمر;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    وإلا
    {
      unsigned عدد_source_glyphs = c->plan->source->get_عدد_glyphs ();
      لكل (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	إذا (!klass) استمر;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, عدد_source_glyphs);
	لكل (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
	  إذا (glyph_filter && !glyph_filter->has (g)) استمر;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    ثابت hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_گنتی = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_گنتی <= glyph_and_klass.length;
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_empty_table || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () ثابت { أرجع hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  {
    لكل (auto &range : rangeRecord)
      إذا (range.value)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    أرجع true;
  }

  template <typename set_t>
  منطقي collect_class (set_t *glyphs, unsigned صحيح klass) ثابت
  {
    لكل (auto &range : rangeRecord)
    {
      إذا (range.value == klass)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    }
    أرجع true;
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    إذا (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      لكل (auto g : *glyphs)
        إذا (get_class (g))
	  أرجع true;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (ثابت RangeRecord<Types> &range) { أرجع range.intersects (*glyphs) && range.value; }));
  }
  منطقي intersects_class (ثابت hb_set_t *glyphs, unsigned klass) ثابت
  {
    إذا (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      إذا (!rangeRecord.len)
        أرجع !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_قدر_INVALID;
      hb_codepoint_t last = HB_SET_قدر_INVALID;
      auto it = hb_iter (rangeRecord);
      لكل (auto &range : it)
      {
        إذا (it->first == last + 1)
	{
	  it++;
	  استمر;
	}

	إذا (!glyphs->next (&g))
	  اكسر;
	إذا (g < range.first)
	  أرجع true;
	g = range.last;
	last = g;
      }
      إذا (g != HB_SET_قدر_INVALID && glyphs->next (&g))
	أرجع true;
      /* Fall through. */
    }
    لكل (ثابت auto &range : rangeRecord)
      إذا (range.value == klass && range.intersects (*glyphs))
	أرجع true;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ثابت
  {
    إذا (klass == 0)
    {
      hb_codepoint_t g = HB_SET_قدر_INVALID;
      لكل (auto &range : rangeRecord)
      {
	إذا (!glyphs->next (&g))
	  اذهب done;
	طالما (g < range.first)
	{
	  intersect_glyphs->add (g);
	  إذا (!glyphs->next (&g))
	    اذهب done;
        }
        g = range.last;
      }
      طالما (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      أرجع;
    }

    unsigned گنتی = rangeRecord.len;
    إذا (گنتی > glyphs->get_population () * hb_bit_storage (گنتی))
    {
      لكل (auto g : *glyphs)
      {
        unsigned i;
        إذا (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      أرجع;
    }

    لكل (auto &range : rangeRecord)
    {
      إذا (range.value != klass) استمر;

      unsigned end = range.last + 1;
      لكل (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  فراغ intersected_classes (ثابت hb_set_t *glyphs, hb_set_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    إذا (!rangeRecord.len)
    {
      intersect_classes->add (0);
      أرجع;
    }

    hb_codepoint_t g = HB_SET_قدر_INVALID;
    لكل (auto &range : rangeRecord)
    {
      إذا (!glyphs->next (&g))
        اكسر;
      إذا (g < range.first)
      {
        intersect_classes->add (0);
        اكسر;
      }
      g = range.last;
    }
    إذا (g != HB_SET_قدر_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    لكل (ثابت auto& range : rangeRecord)
      إذا (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عام:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

بنية ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) ثابت { أرجع get (k); }

  unsigned صحيح get (hb_codepoint_t k) ثابت { أرجع get_class (k); }
  unsigned صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_class (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_class (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_class (glyph_id);
#endif
    افتراضي:أرجع 0;
    }
  }
  unsigned صحيح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    unsigned klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    klass = get_class (glyph_id);
    إذا (cache) cache->set (glyph_id, klass);
    أرجع klass;
  }

  unsigned get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#endif
    افتراضي:أرجع NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    إذا (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned عدد_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      لكل (ثابت auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	إذا (cur_klass > class_max) class_max = cur_klass;
	إذا (cur_gid == glyph_min) استمر;
        إذا (cur_gid > glyph_max) glyph_max = cur_gid;
	إذا (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  عدد_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	إذا (size >= best_size) أرجع;
	best_size = size;
	format = candidate;
      };

      إذا (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      إذا (glyph_max <= 0xFFFFu && عدد_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      إذا (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      إذا (glyph_max <= 0xFFFFFFu && عدد_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      إذا (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (خطأ);
      }
    }

    u.format.v = format;

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               منطقي keep_empty_table = true,
               منطقي use_class_zero = true,
               ثابت Coverage* glyph_filter = nullptr) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    حالة 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالة 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    افتراضي:return_trace (true);
    }
  }

  unsigned cost () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.cost ();
    حالة 2: hb_barrier (); أرجع u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.cost ();
    حالة 4: hb_barrier (); أرجع u.format4.cost ();
#endif
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  منطقي collect_coverage (set_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#endif
    افتراضي:أرجع خطأ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  منطقي collect_class (set_t *glyphs, unsigned صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.collect_class (glyphs, klass);
#endif
    افتراضي:أرجع خطأ;
    }
  }

  منطقي intersects (ثابت hb_set_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#endif
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_class (ثابت hb_set_t *glyphs, unsigned صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_class (glyphs, klass);
#endif
    افتراضي:أرجع خطأ;
    }
  }

  فراغ intersected_class_glyphs (ثابت hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    افتراضي:أرجع;
    }
  }

  فراغ intersected_classes (ثابت hb_set_t *glyphs, hb_set_t *intersect_classes) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_classes (glyphs, intersect_classes);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_classes (glyphs, intersect_classes);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    افتراضي:أرجع;
    }
  }


  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ أرجع (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
بنية delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  بنية chars_t : hb_vector_t<uint8_t>
  {
    صحيح cmp (ثابت chars_t& other) ثابت
    {
      أرجع as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      لكل (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      أرجع hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (ثابت chars_t& other) ثابت
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      لكل (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      أرجع hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (ثابت delta_row_encoding_t& other_encoding) ثابت { أرجع chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<ثابت hb_vector_t<صحيح>*> اشیاء;

  delta_row_encoding_t () = افتراضي;
  delta_row_encoding_t (hb_vector_t<ثابت hb_vector_t<صحيح>*> &&rows, unsigned عدد_cols)
  {
    أكد (rows);

    اشیاء = std::move (rows);

    إذا (unlikely (!chars.resize (عدد_cols)))
      أرجع;

    calculate_chars ();
  }

  فراغ merge (ثابت delta_row_encoding_t& other)
  {
    اشیاء.alloc (اشیاء.length + other.اشیاء.length);
    لكل (auto &row : other.اشیاء)
      add_row (row);

    // Merge chars
    أكد (chars.length == other.chars.length);
    لكل (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  فراغ chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  فراغ calculate_chars ()
  {
    أكد (اشیاء);

    منطقي long_الفاظ = خطأ;

    لكل (auto &row : اشیاء)
    {
      أكد (row->length == chars.length);

      /* 0/1/2 byte encoding */
      لكل (unsigned i = 0; i < row->length; i++)
      {
	صحيح v =  row->arrayZ[i];
	إذا (v == 0)
	  استمر;
	وإلا إذا (v > 32767 || v < -32768)
	{
	  long_الفاظ = true;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	وإلا إذا (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	وإلا
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    إذا (long_الفاظ)
    {
      // Convert 1s to 2s
      لكل (auto &v : chars)
	إذا (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  منطقي is_empty () ثابت
  { أرجع !اشیاء; }

  منطقي in_error () ثابت
  { أرجع chars.in_error () || اشیاء.in_error (); }

  ساكن مضمن unsigned get_chars_overhead (unsigned عدد_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    أرجع c + عدد_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) ثابت
  {
    صحيح گنتی = اشیاء.length;
    أرجع hb_max (0, (صحيح) overhead - گنتی * (صحيح) additional_bytes_per_rows);
  }

  صحيح gain_from_merging (ثابت delta_row_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    إذا (additional_bytes_per_rows > 0)
    {
      إذا (get_gain (additional_bytes_per_rows) == 0)
        أرجع 0;
    }
    وإلا
    {
      إذا (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	أرجع 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    صحيح combined_gain = (صحيح) overhead + (صحيح) other_encoding.overhead;
    combined_gain -= (combined_width - (صحيح) width) * اشیاء.length;
    combined_gain -= (combined_width - (صحيح) other_encoding.width) * other_encoding.اشیاء.length;
    combined_gain -= get_chars_overhead (combined_columns);

    أرجع combined_gain;
  }

  منطقي add_row (ثابت hb_vector_t<صحيح>* row)
  { أرجع اشیاء.push (row); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت delta_row_encoding_t *a = (ثابت delta_row_encoding_t *)pa;
    ثابت delta_row_encoding_t *b = (ثابت delta_row_encoding_t *)pb;

    إذا (a->width != b->width)
      أرجع (صحيح) a->width - (صحيح) b->width;

    أرجع b->chars.cmp (a->chars);
  }
};

بنية VarRegionAxis
{
  عائم evaluate (صحيح coord) ثابت
  {
    صحيح peak = peakCoord.to_int ();
    إذا (peak == 0 || coord == peak)
      أرجع 1.f;
    وإلا إذا (coord == 0) // Faster
      أرجع 0.f;

    صحيح start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    إذا (unlikely (start > peak || peak > end))
      أرجع 1.f;
    إذا (unlikely (start < 0 && end > 0))
      أرجع 1.f;

    إذا (coord <= start || end <= coord)
      أرجع 0.f;

    /* Interpolate */
    إذا (coord < peak)
      أرجع عائم (coord - start) / (peak - start);
    وإلا
      أرجع عائم (end - coord) / (end - peak);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  منطقي serialize (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  عام:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  عام:
  DEFINE_SIZE_STATIC (6);
};
بنية SparseVarRegionAxis
{
  عائم evaluate (ثابت صحيح *coords, unsigned صحيح coord_len) ثابت
  {
    unsigned i = axisIndex;
    صحيح coord = i < coord_len ? coords[i] : 0;
    أرجع axis.evaluate (coord);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت SparseVarRegionAxis *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->embed (src);
    إذا (unlikely (!out)) return_trace (خطأ);
    إذا (axis_map)
    {
      إذا (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (خطأ);
      }
      إذا (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (خطأ);
    }
    return_trace (true);
  }

  عام:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية hb_scalar_cache_t
{
  خاص:
  ساكن constexpr unsigned STATIC_LENGTH = 128;
  ساكن constexpr عائم INVALID = NAN;

  عام:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (ثابت hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  ساكن hb_scalar_cache_t *create (unsigned صحيح گنتی,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    إذا (!گنتی) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    إذا (scratch_cache && گنتی <= STATIC_LENGTH)
    {
      scratch_cache->length = گنتی;
      scratch_cache->clear ();
      أرجع scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (حجم (hb_scalar_cache_t) - حجم (static_اقدار) + حجم (static_اقدار[0]) * گنتی);
    إذا (unlikely (!cache)) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = گنتی;
    cache->clear ();

    أرجع cache;
  }

  ساكن فراغ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    إذا (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  فراغ clear ()
  {
    auto *values = &static_اقدار[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    لكل (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    لكل (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقي get (unsigned i, عائم *value) ثابت
  {
    إذا (unlikely (i >= length))
    {
      *value = 0.f;
      أرجع true;
    }
    auto *values = &static_اقدار[0];
    auto *cached_قدر = &values[i];
    عائم v = *cached_قدر;
    إذا (std::isnan (v))
      أرجع خطأ;
    *value = v;
    أرجع true;
  }

  HB_ALWAYS_INLINE
  فراغ set (unsigned i, عائم value)
  {
    إذا (unlikely (i >= length)) أرجع;
    auto *values = &static_اقدار[0];
    auto *cached_قدر = &values[i];
    *cached_قدر = value;
  }

  عام:
  unsigned length;
  خاص:
  mutable hb_atomic_t<عائم> static_اقدار[STATIC_LENGTH];
};

بنية VarRegionList
{
  خاص:
  عائم evaluate_impl (unsigned صحيح region_index,
		       ثابت صحيح *coords, unsigned صحيح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_گنتی_2);
    عائم v = 1.f;

    unsigned صحيح گنتی = axis_گنتی_2;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      صحيح coord = i < coord_len ? coords[i] : 0;
      عائم factor = axes[i].evaluate (coord);
      إذا (factor == 0.f)
      {
	v = 0.f;
	اكسر;
      }
      v *= factor;
    }

    أرجع v;
  }

  عام:
  HB_ALWAYS_INLINE
  عائم evaluate (unsigned صحيح region_index,
		  ثابت صحيح *coords, unsigned صحيح coord_len,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    إذا (unlikely (region_index >= region_گنتی))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_index, &v))
      أرجع v;

    v = evaluate_impl (region_index, coords, coord_len);

    إذا (cache)
      cache->set (region_index, v);
    أرجع v;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_گنتی_2 * region_گنتی));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_vector_t<hb_tag_t>& axis_tags,
                  ثابت hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (هذا);
    unsigned axis_گنتی = axis_tags.length;
    unsigned region_گنتی_2 = regions.length;
    إذا (!axis_گنتی || !region_گنتی_2) return_trace (خطأ);
    إذا (unlikely (hb_unsigned_mul_overflows (axis_گنتی * region_گنتی_2,
                                             VarRegionAxis::static_size))) return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_گنتی_2 = axis_گنتی;
    region_گنتی = region_گنتی_2;

    لكل (unsigned r = 0; r < region_گنتی_2; r++)
    {
      ثابت auto& region = regions[r];
      لكل (unsigned i = 0; i < axis_گنتی; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        إذا (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->درمیان);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        وإلا
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        إذا (!var_region_rec.serialize (c))
          return_trace (خطأ);
      }
    }
    return_trace (true);
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت VarRegionList *src, ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_گنتی_2 = src->axis_گنتی_2;
    region_گنتی = region_map.get_population ();
    إذا (unlikely (hb_unsigned_mul_overflows (axis_گنتی_2 * region_گنتی,
					     VarRegionAxis::static_size))) return_trace (خطأ);
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
    unsigned صحيح region_گنتی_2 = src->region_گنتی;
    لكل (unsigned صحيح r = 0; r < region_گنتی; r++)
    {
      unsigned صحيح backward = region_map.backward (r);
      إذا (backward >= region_گنتی_2) return_trace (خطأ);
      hb_memcpy (&axesZ[axis_گنتی_2 * r], &src->axesZ[axis_گنتی_2 * backward], VarRegionAxis::static_size * axis_گنتی_2);
    }

    return_trace (true);
  }

  منطقي get_var_region (unsigned region_index,
                       ثابت hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) ثابت
  {
    إذا (region_index >= region_گنتی) أرجع خطأ;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_گنتی_2);
    لكل (unsigned i = 0; i < axis_گنتی_2; i++)
    {
      hb_tag_t *axis_tag;
      إذا (!axes_old_index_tag_map.has (i, &axis_tag))
        أرجع خطأ;

      عائم min_val = axis_region->startCoord.to_float ();
      عائم def_val = axis_region->peakCoord.to_float ();
      عائم max_val = axis_region->endCoord.to_float ();

      إذا (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((مزدوج) min_val, (مزدوج) def_val, (مزدوج) max_val));
      axis_region++;
    }
    أرجع !axis_tuples.in_error ();
  }

  منطقي get_var_regions (ثابت hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) ثابت
  {
    إذا (!regions.alloc (region_گنتی))
      أرجع خطأ;

    لكل (unsigned i = 0; i < region_گنتی; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      إذا (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        أرجع خطأ;
      regions.push (std::move (axis_tuples));
    }
    أرجع !regions.in_error ();
  }

  size_t get_size () ثابت
  { أرجع hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_گنتی_2, region_گنتی)); }

  عام:
  HBUINT16	axis_گنتی_2;
  HBUINT15	region_گنتی;
  محمي:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  عام:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ساكن مضمن منطقي
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  لكل (auto _ : axis_tuples)
  {
    Triple *range;
    إذا (!reachable_ranges.has (_.first, &range)) استمر;
    ثابت Triple &tent = _.second;
    صحيح start = (صحيح) roundf ((عائم) tent.minimum * 16384.f);
    صحيح peak  = (صحيح) roundf ((عائم) tent.درمیان * 16384.f);
    صحيح end   = (صحيح) roundf ((عائم) tent.maximum * 16384.f);
    إذا (!peak) استمر;
    إذا (start > peak || peak > end) استمر;
    إذا (start < 0 && end > 0) استمر;
    صحيح lo = (صحيح) roundf ((عائم) range->minimum * 16384.f);
    صحيح hi = (صحيح) roundf ((عائم) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    إذا (hi < start || lo > end)
      أرجع true;
  }
  أرجع خطأ;
}

بنية SparseVariationRegion
{
  عائم evaluate (ثابت صحيح *coords, unsigned صحيح coord_len) ثابت
  {
    عائم v = 1.f;
    unsigned صحيح گنتی = axes.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      عائم factor = (هذا+axes[i]).evaluate (coords, coord_len);
      إذا (factor == 0.f)
	أرجع 0.;
      v *= factor;
    }
    أرجع v;
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت SparseVariationRegion *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (خطأ);
    لكل (unsigned i = 0; i < axes.len; i++)
      إذا (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (خطأ);
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (axes.sanitize (c, هذا));
  }

  محمي:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  عام:
  DEFINE_SIZE_ARRAY (2, axes);
};

بنية SparseVarRegionList
{
  HB_ALWAYS_INLINE
  عائم evaluate (unsigned صحيح region_index,
		  ثابت صحيح *coords, unsigned صحيح coord_len,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    إذا (unlikely (region_index >= regions.len))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_index, &v))
      أرجع v;

    ثابت SparseVariationRegion &region = هذا+regions[region_index];

    v = region.evaluate (coords, coord_len);
    إذا (cache)
      cache->set (region_index, v);

    أرجع v;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (regions.sanitize (c, هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت SparseVarRegionList *src,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    regions.len = region_map.get_population ();
    إذا (unlikely (!c->extend (regions))) return_trace (خطأ);

    لكل (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      إذا (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (خطأ);
    }
    return_trace (true);
  }

  عام:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  عام:
  DEFINE_SIZE_ARRAY (4, regions);
};


بنية VarData
{
  unsigned صحيح get_item_گنتی () ثابت
  { أرجع item_گنتی; }

  unsigned صحيح get_region_index_گنتی () ثابت
  { أرجع regionIndices.len; }

  unsigned get_region_index (unsigned i) ثابت
  { أرجع i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () ثابت
  { أرجع (word_گنتی () + regionIndices.len) * (long_الفاظ_2 () ? 2 : 1); }

  size_t get_size () ثابت
  { أرجع hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_گنتی, get_row_size ())); }

  template <typename Float = عائم>
  Float _get_delta (unsigned صحيح inner,
		    ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) ثابت
  {
    إذا (unlikely (inner >= item_گنتی))
      أرجع 0.;
    منطقي is_long = long_الفاظ_2 ();
    unsigned صحيح گنتی = regionIndices.len;
    unsigned word_گنتی_2 = word_گنتی ();
    unsigned صحيح scount = is_long ? گنتی : word_گنتی_2;
    unsigned صحيح lcount = is_long ? word_گنتی_2 : 0;

    ثابت HBUINT8 *bytes = get_delta_bytes ();
    ثابت HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned صحيح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (row);
    لكل (; i < lcount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *lcursor;
      lcursor++;
    }
    ثابت HBINT16 *scursor = reinterpret_cast<ثابت HBINT16 *> (lcursor);
    لكل (; i < scount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی, cache);
      إذا (scalar)
       delta += (Float) scalar * (صحيح) *scursor;
      scursor++;
    }
    ثابت HBINT8 *bcursor = reinterpret_cast<ثابت HBINT8 *> (scursor);
    لكل (; i < گنتی; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *bcursor;
      bcursor++;
    }

    أرجع delta;
  }

  template <typename Float = عائم>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned صحيح inner,
		   ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
    unsigned صحيح گنتی = regionIndices.len;
    إذا (!گنتی) أرجع Float (0); // This is quite common, so optimize it.
    أرجع _get_delta<Float> (inner, coords, coord_گنتی, regions, cache);
  }

  فراغ get_region_scalars (ثابت صحيح *coords, unsigned صحيح coord_گنتی,
			   ثابت VarRegionList &regions,
			   عائم *scalars /*OUT */,
			   unsigned صحيح عدد_scalars) ثابت
  {
    unsigned گنتی = hb_min (عدد_scalars, regionIndices.len);
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی);
    لكل (unsigned صحيح i = گنتی; i < عدد_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_گنتی () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_گنتی,
				  get_row_size ()));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_vector_t<ثابت hb_vector_t<صحيح>*>& rows)
  {
    TRACE_SERIALIZE (هذا);
    unsigned row_گنتی_2 = rows.length;
    إذا (!row_گنتی_2) {
      // Nothing to serialize, will be empty.
      أرجع خطأ;
    }

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!c->check_assign (item_گنتی, row_گنتی_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (خطأ);

    صحيح min_threshold = has_long ? -32768 : -128;
    صحيح max_threshold = has_long ? +32767 : +127;
    تعداد delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned عدد_regions = rows[0]->length;
    إذا (!delta_sz.resize (عدد_regions))
      return_trace (خطأ);

    unsigned word_گنتی_2 = 0;
    لكل (unsigned r = 0; r < عدد_regions; r++)
    {
      لكل (unsigned i = 0; i < row_گنتی_2; i++)
      {
        صحيح delta = rows[i]->arrayZ[r];
        إذا (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_گنتی_2++;
          اكسر;
        }
        وإلا إذا (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_گنتی_2;
    hb_map_t ri_map;
    لكل (unsigned r = 0; r < عدد_regions; r++)
    {
      إذا (!delta_sz[r]) استمر;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      إذا (!ri_map.set (new_r, r))
        return_trace (خطأ);
    }

    word_Size_گنتی = word_گنتی_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_گنتی = ri_map.get_population ();
    regionIndices.len = ri_گنتی;
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (unsigned r = 0; r < ri_گنتی; r++)
    {
      hb_codepoint_t *idx;
      إذا (!ri_map.has (r, &idx))
        return_trace (خطأ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    لكل (unsigned صحيح i = 0; i < row_گنتی_2; i++)
    {
      لكل (unsigned صحيح r = 0; r < ri_گنتی; r++)
      {
        صحيح delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (true);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت VarData *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    item_گنتی = inner_map.get_next_قدر ();

    /* Optimize word count */
    unsigned ri_گنتی = src->regionIndices.len;
    تعداد delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned صحيح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_گنتی);
    ri_map.resize (ri_گنتی);
    unsigned صحيح new_word_گنتی = 0;
    unsigned صحيح r;

    ثابت HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_گنتی = src->word_گنتی ();
    منطقي     src_long_الفاظ = src->long_الفاظ_2 ();

    منطقي has_long = خطأ;
    إذا (src_long_الفاظ)
    {
      لكل (r = 0; r < src_word_گنتی; r++)
      {
        لكل (unsigned old_gid : inner_map.کلیدیں())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  إذا (delta < -32768 || 32767 < delta)
	  {
	    has_long = true;
	    اكسر;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    لكل (r = 0; r < ri_گنتی; r++)
    {
      منطقي short_circuit = src_long_الفاظ == has_long && src_word_گنتی <= r;

      delta_sz[r] = kZero;
      إذا (culled_regions && culled_regions->has (src->regionIndices[r]))
	استمر; /* unreachable region: drop its column */
      لكل (unsigned old_gid : inner_map.کلیدیں())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	إذا (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_گنتی++;
	  اكسر;
	}
	وإلا إذا (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  إذا (short_circuit)
	    اكسر;
	}
      }
    }

    unsigned صحيح word_index = 0;
    unsigned صحيح non_word_index = new_word_گنتی;
    unsigned صحيح new_ri_گنتی = 0;
    لكل (r = 0; r < ri_گنتی; r++)
      إذا (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_گنتی++;
      }

    word_Size_گنتی = new_word_گنتی | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_گنتی;

    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (r = 0; r < new_ri_گنتی; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned گنتی = item_گنتی;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      unsigned صحيح old = inner_map.backward (i);
      لكل (unsigned صحيح r = 0; r < new_ri_گنتی; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (true);
  }

  فراغ collect_region_refs (hb_set_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    لكل (unsigned صحيح r = 0; r < regionIndices.len; r++)
    {
      unsigned صحيح region = regionIndices.arrayZ[r];
      إذا (region_indices.has (region)) استمر;
      لكل (hb_codepoint_t old_gid : inner_map.کلیدیں())
	إذا (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  اكسر;
	}
    }
  }

  عام:
  ثابت HBUINT8 *get_delta_bytes () ثابت
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  محمي:
  HBUINT8 *get_delta_bytes ()
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  عام:
  int32_t get_item_delta_fast (unsigned صحيح item, unsigned صحيح region,
			       ثابت HBUINT8 *delta_bytes, unsigned row_size) ثابت
  {
    إذا (unlikely (item >= item_گنتی || region >= regionIndices.len)) أرجع 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_bytes + item * row_size;
    unsigned word_گنتی_2 = word_گنتی ();
    منطقي is_long = long_الفاظ_2 ();
    إذا (is_long)
    {
      إذا (region < word_گنتی_2)
	أرجع ((ثابت HBINT32 *) p)[region];
      وإلا
	أرجع ((ثابت HBINT16 *)(p + HBINT32::static_size * word_گنتی_2))[region - word_گنتی_2];
    }
    وإلا
    {
      إذا (region < word_گنتی_2)
	أرجع ((ثابت HBINT16 *) p)[region];
      وإلا
	أرجع (p + HBINT16::static_size * word_گنتی_2)[region - word_گنتی_2];
    }
  }
  int32_t get_item_delta (unsigned صحيح item, unsigned صحيح region) ثابت
  {
     أرجع get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  محمي:
  فراغ set_item_delta_fast (unsigned صحيح item, unsigned صحيح region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_گنتی_2 = word_گنتی ();
    منطقي is_long = long_الفاظ_2 ();
    إذا (is_long)
    {
      إذا (region < word_گنتی_2)
	((HBINT32 *) p)[region] = delta;
      وإلا
	((HBINT16 *)(p + HBINT32::static_size * word_گنتی_2))[region - word_گنتی_2] = delta;
    }
    وإلا
    {
      إذا (region < word_گنتی_2)
	((HBINT16 *) p)[region] = delta;
      وإلا
	(p + HBINT16::static_size * word_گنتی_2)[region - word_گنتی_2] = delta;
    }
  }
  فراغ set_item_delta (unsigned صحيح item, unsigned صحيح region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  منطقي long_الفاظ_2 () ثابت { أرجع word_Size_گنتی & 0x8000u /* LONG_WORDS */; }
  unsigned word_گنتی () ثابت { أرجع word_Size_گنتی & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محمي:
  HBUINT16		item_گنتی;
  HBUINT16		word_Size_گنتی;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عام:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

بنية MultiVarData
{
  size_t get_size () ثابت
  { أرجع hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { أرجع هذا+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  فراغ get_delta (unsigned صحيح inner,
		  ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<مزدوج> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    auto &deltaSets = get_delta_sets ();

    auto اقدار_iter = deltaSets.fetcher (inner);
    unsigned region_گنتی = regionIndices.len;
    unsigned skip = 0;
    لكل (unsigned regionIndex = 0; regionIndex < region_گنتی; regionIndex++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_گنتی,
				       cache);
      // We skip lazily. Helps with the tail end.
      إذا (scalar == 0.0f)
        skip += out.length;
      وإلا
      {
        إذا (skip)
	{
	  اقدار_iter.skip (skip);
	  skip = 0;
	}
	اقدار_iter.add_to (out, scalar);
      }
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, هذا));
  }

  unsigned get_item_گنتی () ثابت
  { أرجع get_delta_sets ().گنتی; }

  فراغ collect_region_refs (hb_set_t &region_indices) ثابت
  {
    لكل (unsigned region : regionIndices)
      region_indices.add (region);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت MultiVarData *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    unsigned region_گنتی_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_گنتی_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    إذا (unlikely (!c->extend_size (هذا, header_size, خطأ)))
      return_trace (خطأ);
    format = 1;
    regionIndices.len = region_گنتی_2;
    لكل (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      إذا (unlikely (!region_map.has (old_region)))
	return_trace (خطأ);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    إذا (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (خطأ);
    لكل (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      إذا (unlikely (old_inner >= src_delta_sets.گنتی)) return_trace (خطأ);
      hb_ubytes_t bytes = static_cast<ثابت CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    إذا (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (خطأ);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  محمي:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  عام:
  DEFINE_SIZE_MIN (7);
};

بنية ItemVariationStore
{
  friend بنية item_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#ifdef HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#endif
    أرجع hb_scalar_cache_t::create ((هذا+regions).region_گنتی);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  خاص:
  template <typename Float = عائم>
  Float get_delta (unsigned صحيح outer, unsigned صحيح inner,
		   ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
#ifdef HB_NO_VAR
    أرجع Float (0);
#endif

    إذا (unlikely (outer >= dataSets.len))
      أرجع Float (0);

    أرجع (هذا+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_گنتی,
					     هذا+regions,
					     cache);
  }

  عام:
  منطقي has_delta_set (unsigned صحيح index) ثابت
  {
#ifdef HB_NO_VAR
    أرجع خطأ;
#endif

    unsigned صحيح outer = index >> 16;
    unsigned صحيح inner = index & 0xFFFF;
    أرجع outer < dataSets.len &&
	   inner < (هذا+dataSets[outer]).get_item_گنتی ();
  }

  template <typename Float = عائم>
  Float get_delta (unsigned صحيح index,
		   ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
    unsigned صحيح outer = index >> 16;
    unsigned صحيح inner = index & 0xFFFF;
    أرجع get_delta<Float> (outer, inner, coords, coord_گنتی, cache);
  }
  template <typename Float = عائم>
  Float get_delta (unsigned صحيح index,
		   hb_array_t<ثابت صحيح> coords,
		   hb_scalar_cache_t *cache = nullptr) ثابت
  {
    أرجع get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#ifdef HB_NO_VAR
    أرجع true;
#endif

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  dataSets.sanitize (c, هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_vector_t<hb_tag_t>& axis_tags,
                  ثابت hb_vector_t<ثابت hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  ثابت hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (هذا);
#ifdef HB_NO_VAR
    return_trace (خطأ);
#endif
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    format = 1;
    إذا (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (خطأ);

    unsigned عدد_var_data = vardata_encodings.length;
    إذا (!عدد_var_data) return_trace (خطأ);
    إذا (unlikely (!c->check_assign (dataSets.len, عدد_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (خطأ);

    إذا (unlikely (!c->extend (dataSets))) return_trace (خطأ);
    لكل (unsigned i = 0; i < عدد_var_data; i++)
      إذا (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].اشیاء))
        return_trace (خطأ);

    return_trace (true);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت ItemVariationStore *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (هذا);
#ifdef HB_NO_VAR
    return_trace (خطأ);
#endif

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    unsigned صحيح set_گنتی = 0;
    لكل (unsigned صحيح i = 0; i < inner_maps.length; i++)
      إذا (inner_maps[i].get_population ())
	set_گنتی++;

    format = 1;

    ثابت auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    لكل (unsigned صحيح i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    إذا (region_indices.in_error ())
      return_trace (خطأ);

    region_indices.del_range ((src_regions).region_گنتی, hb_set_t::INVALID);

    إذا (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    إذا (region_map.in_error())
      return_trace (خطأ);

    إذا (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (خطأ);

    dataSets.len = set_گنتی;
    إذا (unlikely (!c->extend (dataSets))) return_trace (خطأ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned صحيح set_index = 0;
    لكل (unsigned صحيح i = 0; i < inner_maps.length; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (خطأ);
    }

    return_trace (true);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    auto *out = c->start_embed (هذا);
    إذا (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned گنتی = dataSets.len;
    لكل (unsigned i = 0; i < گنتی; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      إذا (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = هذا+dataSets[i];

      unsigned item_گنتی = data.get_item_گنتی ();
      لكل (unsigned j = 0; j < item_گنتی; j++)
	map->add (j);
    }

    إذا (unlikely (!out->serialize (c, هذا, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقي collect_dead_regions (ثابت hb_map_t &axes_old_index_tag_map,
			     ثابت hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) ثابت
  {
#ifndef HB_NO_VAR
    إذا (!reachable_ranges.get_population ()) أرجع true;
    ثابت VarRegionList &region_list = هذا+regions;
    unsigned گنتی = region_list.region_گنتی;
    لكل (unsigned i = 0; i < گنتی; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      إذا (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	أرجع خطأ;
      إذا (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    أرجع !dead_regions.in_error ();
#وإلا
    أرجع true;
#endif
  }

  منطقي subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (هذا);
#ifdef HB_NO_VAR
    return_trace (خطأ);
#endif

    ItemVariationStore *varstore_مفرد = c->serializer->start_embed<ItemVariationStore> ();
    إذا (unlikely (!varstore_مفرد)) return_trace (خطأ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    إذا (c->plan->has_avar2)
      إذا (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (خطأ);

    varstore_مفرد->serialize (c->serializer, هذا, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_مفرد->dataSets);
  }

  unsigned صحيح get_region_index_گنتی (unsigned صحيح major) ثابت
  {
#ifdef HB_NO_VAR
    أرجع 0;
#endif
    أرجع (هذا+dataSets[major]).get_region_index_گنتی ();
  }

  فراغ get_region_scalars (unsigned صحيح major,
			   ثابت صحيح *coords, unsigned صحيح coord_گنتی,
			   عائم *scalars /*OUT*/,
			   unsigned صحيح عدد_scalars) ثابت
  {
#ifdef HB_NO_VAR
    لكل (unsigned i = 0; i < عدد_scalars; i++)
      scalars[i] = 0.f;
    أرجع;
#endif

    (هذا+dataSets[major]).get_region_scalars (coords, coord_گنتی,
					       هذا+regions,
					       &scalars[0], عدد_scalars);
  }

  unsigned صحيح get_sub_table_گنتی () ثابت
   {
#ifdef HB_NO_VAR
     أرجع 0;
#endif
     أرجع dataSets.len;
   }

  ثابت VarData& get_sub_table (unsigned i) ثابت
  {
#ifdef HB_NO_VAR
     أرجع Null (VarData);
#endif
     أرجع هذا+dataSets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#ifdef HB_NO_VAR
     أرجع Null (VarRegionList);
#endif
     أرجع هذا+regions;
  }

  محمي:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  عام:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

بنية MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) ثابت
  {
#ifdef HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#endif
    auto &r = هذا+regions;
    unsigned گنتی = r.regions.len;

    أرجع hb_scalar_cache_t::create (گنتی, static_cache);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  خاص:
  فراغ get_delta (unsigned صحيح outer, unsigned صحيح inner,
		  ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		  hb_array_t<مزدوج> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
#ifdef HB_NO_VAR
    أرجع;
#endif

    إذا (unlikely (outer >= dataSets.len))
      أرجع;

    أرجع (هذا+dataSets[outer]).get_delta (inner,
					     coords, coord_گنتی,
					     هذا+regions,
					     out,
					     cache);
  }

  عام:
  فراغ get_delta (unsigned صحيح index,
		  ثابت صحيح *coords, unsigned صحيح coord_گنتی,
		  hb_array_t<مزدوج> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    unsigned صحيح outer = index >> 16;
    unsigned صحيح inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_گنتی, out, cache);
  }
  فراغ get_delta (unsigned صحيح index,
		  hb_array_t<ثابت صحيح> coords,
		  hb_array_t<مزدوج> out,
		  hb_scalar_cache_t *cache = nullptr) ثابت
  {
    أرجع get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#ifdef HB_NO_VAR
    أرجع true;
#endif

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  dataSets.sanitize (c, هذا));
  }

  منطقي create_subset_plan (ثابت hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    إذا (unlikely (!inner_maps->resize (dataSets.len))) أرجع خطأ;
    لكل (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      إذا (unlikely (outer >= dataSets.len ||
		    inner >= (هذا+dataSets[outer]).get_item_گنتی ()))
	أرجع خطأ;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    لكل (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      إذا (!inner_map.get_population ()) استمر;
      إذا (unlikely (inner_map.in_error ())) أرجع خطأ;
      لكل (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    أرجع !inner_maps->in_error () && !varidx_map->in_error ();
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت MultiItemVariationStore *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (خطأ);
    format = 1;

    ثابت auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_گنتی = 0;
    لكل (unsigned i = 0; i < src->dataSets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      set_گنتی++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    إذا (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (خطأ);

    dataSets.len = set_گنتی;
    إذا (unlikely (!c->extend (dataSets))) return_trace (خطأ);
    unsigned set_index = 0;
    لكل (unsigned i = 0; i < src->dataSets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (خطأ);
    }
    return_trace (true);
  }

  محمي:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  عام:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_گنتی_T>
بنية DeltaSetIndexMapFormat01
{
  friend بنية DeltaSetIndexMap;

  size_t get_size () ثابت
  { أرجع hb_unsigned_mul_add_saturate (map_گنتی_2, get_width (), min_size); }

  خاص:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  template <typename T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    unsigned صحيح width = plan.get_width ();
    unsigned صحيح inner_bit_گنتی = plan.get_inner_bit_گنتی ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (هذا);
    إذا (unlikely (output_map.length && ((((inner_bit_گنتی-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    entryFormat = ((width-1)<<4)|(inner_bit_گنتی-1);
    map_گنتی_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    إذا (unlikely (!p)) return_trace (خطأ);
    لكل (unsigned صحيح i = 0; i < output_map.length; i++)
    {
      unsigned صحيح v = output_map.arrayZ[i];
      إذا (v)
      {
	unsigned صحيح outer = v >> 16;
	unsigned صحيح inner = v & 0xFFFF;
	unsigned صحيح u = (outer << inner_bit_گنتی) | inner;
	لكل (unsigned صحيح w = width; w > 0;)
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
  uint32_t map (unsigned صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    إذا (!map_گنتی_2)
      أرجع v;
    أرجع _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    إذا (v >= map_گنتی_2)
      v = map_گنتی_2 - 1;

    unsigned صحيح u = 0;
    { /* Fetch it. */
      unsigned صحيح w = get_width ();
      ثابت HBUINT8 *p = mapDataZ.arrayZ + w * v;
      لكل (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned صحيح n = get_inner_bit_گنتی ();
      unsigned صحيح outer = u >> n;
      unsigned صحيح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    أرجع u;
  }

  unsigned get_map_گنتی () ثابت       { أرجع map_گنتی_2; }
  unsigned get_width () ثابت           { أرجع ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_گنتی () ثابت { أرجع (entryFormat & 0xF) + 1; }


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_گنتی_2,
                                  get_width ()));
  }

  محمي:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_گنتی_T     map_گنتی_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  عام:
  DEFINE_SIZE_ARRAY (2+Map_گنتی_T::static_size, mapDataZ);
};

بنية DeltaSetIndexMap
{
  template <typename T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (هذا);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    افتراضي:return_trace (خطأ);
    }
  }

  uint32_t map (unsigned v) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع (u.format0.map (v));
    حالة 1: hb_barrier (); أرجع (u.format1.map (v));
    افتراضي:أرجع v;
    }
  }

  unsigned get_map_گنتی () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_map_گنتی ();
    حالة 1: hb_barrier (); أرجع u.format1.get_map_گنتی ();
    افتراضي:أرجع 0;
    }
  }

  unsigned get_width () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_width ();
    حالة 1: hb_barrier (); أرجع u.format1.get_width ();
    افتراضي:أرجع 0;
    }
  }

  unsigned get_inner_bit_گنتی () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_inner_bit_گنتی ();
    حالة 1: hb_barrier (); أرجع u.format1.get_inner_bit_گنتی ();
    افتراضي:أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.format.v.sanitize (c)) return_trace (خطأ);
    hb_barrier ();
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالة 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    افتراضي:return_trace (true);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    حالة 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    افتراضي:return_trace (nullptr);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  عام:
  DEFINE_SIZE_UNION (1, format.v);
};


بنية ItemVarStoreInstancer
{
  ItemVarStoreInstancer (ثابت ItemVariationStore *varStore_,
			 ثابت DeltaSetIndexMap *varIdxMap,
			 hb_array_t<ثابت صحيح> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    إذا (!hb_any (coords))
      هذا->coords = {};
  }

  operator منطقي () ثابت { أرجع varStore && منطقي (coords); }

  عائم operator[] (uint32_t varIdx) ثابت
  { أرجع (*هذا) (varIdx); }

  عائم operator() (uint32_t varIdx, unsigned short offset = 0) ثابت
  { أرجع get_delta (varIdx, offset); }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع get_delta<مزدوج> (varIdx); }

  template <typename Float = عائم>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   إذا (!coords || varIdx == VarIdx::NO_VARIATION)
     أرجع Float (0);

    varIdx += offset;
    إذا (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    أرجع varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت ItemVariationStore *varStore;
  ثابت DeltaSetIndexMap *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};

بنية MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (ثابت MultiItemVariationStore *varStore,
			      ثابت DeltaSetIndexMap *varIdxMap,
			      hb_array_t<ثابت صحيح> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator منطقي () ثابت { أرجع varStore; }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع (مزدوج) (*هذا)[varIdx]; }

  مزدوج operator[] (uint32_t varIdx) ثابت
  {
    مزدوج v = 0;
    (*هذا) (hb_array (&v, 1), varIdx);
    أرجع v;
  }

  فراغ operator() (hb_array_t<مزدوج> out, uint32_t varIdx, unsigned short offset = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    إذا (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      إذا (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    وإلا
      لكل (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  ثابت MultiItemVariationStore *varStore;
  ثابت DeltaSetIndexMap *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
تعداد Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

تعداد lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

بنية Condition;

template <typename Instancer>
ساكن منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				unsigned صحيح coord_len,
				Instancer *instancer);

بنية ConditionAxisRange
{
  friend بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    ثابت hb_map_t *index_map = &c->plan->axes_index_map;
    إذا (index_map->is_empty ()) return_trace (true);

    ثابت hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    إذا (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (خطأ);

    ثابت hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    إذا (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    ثابت hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    إذا (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    عائم normalized_min = renormalize_قدر ((مزدوج) filter_Range_Min_قدر.to_float (), axis_limit, axis_triple_distances, خطأ);
    عائم normalized_max = renormalize_قدر ((مزدوج) filter_Range_Max_قدر.to_float (), axis_limit, axis_triple_distances, خطأ);
    out->filter_Range_Min_قدر.set_float (normalized_min);
    out->filter_Range_Max_قدر.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  خاص:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) ثابت
  {
    //invalid axis index, drop the entire record
    إذا (!c->axes_index_tag_map->has (axisIndex))
      أرجع DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    منطقي axis_set_by_user = خطأ;
    إذا (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = true;
    }

    عائم axis_min_val = axis_range.minimum;
    عائم axis_default_val = axis_range.درمیان;
    عائم axis_max_val = axis_range.maximum;

    عائم filter_min_val = filter_Range_Min_قدر.to_float ();
    عائم filter_max_val = filter_Range_Max_قدر.to_float ();

    إذا (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = خطأ;

    //condition not met, drop the entire record
    إذا (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      أرجع DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    إذا (axis_set_by_user && axis_range.is_point ())
      أرجع DROP_COND_WITH_VAR;

    إذا (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_قدر.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_قدر.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      أرجع KEEP_COND_WITH_VAR;
    }
    أرجع KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 Instancer *instancer HB_UNUSED) ثابت
  {
    صحيح coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    أرجع filter_Range_Min_قدر.to_int () <= coord && coord <= filter_Range_Max_قدر.to_int ();
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_قدر;
  F2DOT14	filter_Range_Max_قدر;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية Condition_قدر
{
  friend بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    إذا (varIdx == VarIdx::NO_VARIATION)
      return_trace (true);

    hb_pair_t<unsigned, صحيح> *condition_قدر;
    إذا (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) هذا, &condition_قدر))
    {
      out->default_قدر_2 = condition_قدر->second;
      out->varIdx = condition_قدر->first;
      return_trace (true);
    }

    hb_pair_t<unsigned, صحيح> *new_varidx_delta;
    إذا (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (خطأ);

    إذا (unlikely (!c->serializer->check_assign (
		out->default_قدر_2,
		default_قدر_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (خطأ);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (true);
  }

  خاص:
  template <typename Instancer>
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    مزدوج value = default_قدر_2;
    value += instancer->get_condition_delta (varIdx);
    أرجع value > 0;
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_قدر_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية ConditionAnd
{
  friend بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (خطأ);
    out->format = format;

    لكل (ثابت auto &offset : conditions)
    {
      إذا (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	إذا (unlikely (!out_offset)) return_trace (خطأ);
	*out_offset = 0;
      }
      وإلا إذا (unlikely (!subset_offset_array (c, out->conditions, هذا) (offset)))
	return_trace (خطأ);
    }

    return_trace (true);
  }

  خاص:
  template <typename Instancer>
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    unsigned صحيح گنتی = conditions.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      إذا (!_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	أرجع خطأ;
    أرجع true;
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<بنية Condition>	conditions;
  عام:
  DEFINE_SIZE_ARRAY (3, conditions);
};

بنية ConditionOr
{
  friend بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (خطأ);
    out->format = format;

    لكل (ثابت auto &offset : conditions)
    {
      إذا (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	إذا (unlikely (!out_offset)) return_trace (خطأ);
	*out_offset = 0;
      }
      وإلا إذا (unlikely (!subset_offset_array (c, out->conditions, هذا) (offset)))
	return_trace (خطأ);
    }

    return_trace (true);
  }

  خاص:
  template <typename Instancer>
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    unsigned صحيح گنتی = conditions.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      إذا (_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	أرجع true;
    أرجع خطأ;
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<بنية Condition>	conditions;
  عام:
  DEFINE_SIZE_ARRAY (3, conditions);
};

بنية ConditionNegate
{
  friend بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);
    إذا (condition.is_null ())
    {
      out->condition = 0;
      return_trace (true);
    }
    return_trace (out->condition.serialize_subset (c, condition, هذا));
  }

  خاص:
  template <typename Instancer>
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    أرجع !_hb_recurse_condition_evaluate (هذا+condition,
					    coords, coord_len,
					    instancer);
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (condition.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<بنية Condition>	condition;
  عام:
  DEFINE_SIZE_STATIC (5);
};

بنية Condition
{
  friend بنية ConditionSet;

  ساكن منطقي serialize_constant (hb_serialize_context_t *c, منطقي value)
  {
    auto *out = c->start_embed<Condition_قدر> ();
    إذا (unlikely (!out || !c->extend_min (out))) أرجع خطأ;
    out->format = 2;
    out->default_قدر_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    أرجع true;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      منطقي materialize_true = خطأ) ثابت;

  template <typename Instancer>
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بدل (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    حالة 0: أرجع true;
    حالة 1: hb_barrier (); أرجع u.format1.evaluate (coords, coord_len, instancer);
    حالة 2: hb_barrier (); أرجع u.format2.evaluate (coords, coord_len, instancer);
    حالة 3: hb_barrier (); أرجع u.format3.evaluate (coords, coord_len, instancer);
    حالة 4: hb_barrier (); أرجع u.format4.evaluate (coords, coord_len, instancer);
    حالة 5: hb_barrier (); أرجع u.format5.evaluate (coords, coord_len, instancer);
    افتراضي:أرجع خطأ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) ثابت
  {
    إذا (unlikely (!depth))
    {
      c->apply = خطأ;
      أرجع DROP_RECORD_WITH_VAR;
    }

    بدل (u.format.v) {
    حالة 0: أرجع KEEP_RECORD_WITH_VAR;
    حالة 1: hb_barrier (); أرجع u.format1.keep_with_variations (c, condition_map);
    حالة 2: حالة 3: حالة 4: حالة 5: اكسر;
    افتراضي: c->apply = خطأ; أرجع DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    منطقي applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    إذا (c->all_axes_pinned)
      أرجع applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    إذا (u.format.v == 2)
      أرجع u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (ثابت Condition &child)
    {
      auto child_context = *c;
      child_context.apply = true;
      أرجع child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    إذا (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      أرجع result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    منطقي is_and = u.format.v == 3;
    منطقي variable = خطأ;
    ثابت auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت فراغ *base = is_and ? (ثابت فراغ *) &u.format3 : (ثابت فراغ *) &u.format4;
    لكل (ثابت auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      إذا (is_and && result == DROP_RECORD_WITH_VAR)
        أرجع DROP_RECORD_WITH_VAR;
      إذا (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        أرجع KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    أرجع variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_قدر ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->end_recursion (هذا->dispatch (c)));
  }

  منطقي collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, صحيح>> *condition_اقدار = nullptr) ثابت;

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = nullptr);

  خاص:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) ثابت;

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_قدر	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  عام:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				unsigned صحيح coord_len,
				Instancer *instancer)
{
  أرجع condition.evaluate (coords, coord_len, instancer);
}

بنية ConditionList
{
  ثابت Condition& operator[] (unsigned i) ثابت
  { أرجع هذا+conditions[i]; }

  unsigned get_گنتی () ثابت { أرجع conditions.len; }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت ConditionList *src,
		  ثابت hb_inc_bimap_t &condition_map,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    conditions.len = condition_map.get_population ();
    إذا (unlikely (!c->extend (conditions))) return_trace (خطأ);
    لكل (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      إذا (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (خطأ);
    }
    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  Array32OfOffset32To<Condition> conditions;
  عام:
  DEFINE_SIZE_ARRAY (4, conditions);
};

مضمن منطقي
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, صحيح>> *condition_اقدار) ثابت
{
  إذا (unlikely (!depth)) أرجع خطأ;
  بدل (u.format.v)
  {
    حالة 0:
    حالة 1:
      أرجع true;
    حالة 2:
      إذا (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        إذا (condition_اقدار &&
            !condition_اقدار->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), صحيح (u.format2.default_قدر_2))))
          أرجع خطأ;
      }
      أرجع !var_indices->in_error ();
    حالة 3:
      لكل (ثابت auto &offset : u.format3.conditions)
	إذا (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_اقدار)))
	  أرجع خطأ;
      أرجع true;
    حالة 4:
      لكل (ثابت auto &offset : u.format4.conditions)
	إذا (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_اقدار)))
	  أرجع خطأ;
      أرجع true;
    حالة 5:
      أرجع (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_اقدار);
    افتراضي:
      أرجع خطأ;
  }
}

مضمن منطقي
Condition::serialize (hb_serialize_context_t *c,
		      ثابت Condition *src,
		      ثابت hb_map_t &varidx_map,
		      ثابت hb_map_t *axis_map)
{
  TRACE_SERIALIZE (هذا);
  بدل (src->u.format.v)
  {
    حالة 0:
    {
      auto *out = c->start_embed<Condition_قدر> ();
      إذا (unlikely (!out || !c->extend_min (out))) return_trace (خطأ);
      out->format = 2;
      out->default_قدر_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (true);
    }
    حالة 1:
    {
      auto *out = c->embed (&src->u.format1);
      إذا (unlikely (!out)) return_trace (خطأ);
      إذا (axis_map)
      {
	إذا (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (خطأ);
	}
	إذا (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (خطأ);
      }
      return_trace (true);
    }
    حالة 2:
    {
      auto *out = c->embed (&src->u.format2);
      إذا (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (خطأ);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (true);
    }
    حالة 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      إذا (unlikely (!c->extend_min (out))) return_trace (خطأ);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      إذا (unlikely (!c->extend (out->conditions))) return_trace (خطأ);
      لكل (unsigned i = 0; i < out->conditions.len; i++)
	إذا (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (true);
    }
    حالة 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      إذا (unlikely (!c->extend_min (out))) return_trace (خطأ);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      إذا (unlikely (!c->extend (out->conditions))) return_trace (خطأ);
      لكل (unsigned i = 0; i < out->conditions.len; i++)
	إذا (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (true);
    }
    حالة 5:
    {
      auto *out = c->embed (&src->u.format5);
      إذا (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (خطأ);
      return_trace (true);
    }
    افتراضي:
      return_trace (خطأ);
  }
}

مضمن lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    منطقي materialize_true) ثابت
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  إذا (result == LOOKUP_CONDITION_SUBSET_KEEP)
    أرجع result;

  c->serializer->revert (snap);
  إذا (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    أرجع result;

  أرجع serialize_constant (c->serializer, true) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

مضمن lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) ثابت
{
  إذا (unlikely (!depth))
    أرجع LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   ثابت Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    إذا (unlikely (!offset))
      أرجع LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    إذا (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->نکالو_pack ());
    وإلا
    {
      c->serializer->نکالو_discard ();
      out.نکالو ();
      c->serializer->revert (snap);
    }
    أرجع result;
  };

  بدل (u.format.v)
  {
    حالة 0:
      أرجع LOOKUP_CONDITION_SUBSET_TRUE;

    حالة 1:
    {
      ثابت ConditionAxisRange &src = u.format1;
      إذا (c->plan->user_axes_location.is_empty ())
        أرجع c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      إذا (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      إذا (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      عائم filter_min = src.filter_Range_Min_قدر.to_float ();
      عائم filter_max = src.filter_Range_Max_قدر.to_float ();
      إذا (filter_min > filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_limit.is_point ())
        أرجع filter_min <= axis_limit.درمیان &&
	       axis_limit.درمیان <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      إذا (!c->plan->axes_index_map.has (src.axisIndex))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      إذا (unlikely (!out))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      إذا (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_قدر.set_float (
	  renormalize_قدر (filter_min, axis_limit, axis_distances, خطأ));
      out->filter_Range_Max_قدر.set_float (
	  renormalize_قدر (filter_max, axis_limit, axis_distances, خطأ));
      إذا (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 2:
    {
      ثابت Condition_قدر &src = u.format2;
      auto *out = c->serializer->embed (&src);
      إذا (unlikely (!out))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      إذا (src.varIdx == VarIdx::NO_VARIATION)
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, صحيح> *condition_قدر;
      إذا (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) &src, &condition_قدر))
      {
        out->default_قدر_2 = condition_قدر->second;
        out->varIdx = condition_قدر->first;
        إذا (out->varIdx == VarIdx::NO_VARIATION)
          أرجع out->default_قدر_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        أرجع LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      إذا (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        مزدوج value = (صحيح) src.default_قدر_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        أرجع value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحيح default_قدر = (صحيح) src.default_قدر_2 +
			  hb_second (*new_varidx_delta);
      إذا (unlikely (!c->serializer->check_assign (
		out->default_قدر_2,
		default_قدر,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 3:
    {
      ثابت ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      إذا (unlikely (!out || !c->serializer->extend_min (out)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      لكل (ثابت auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        إذا (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          أرجع result;
      }
      أرجع out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالة 4:
    {
      ثابت ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      إذا (unlikely (!out || !c->serializer->extend_min (out)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      لكل (ثابت auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        إذا (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          أرجع result;
      }
      أرجع out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالة 5:
    {
      ثابت ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      إذا (unlikely (!out))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      إذا (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->نکالو_pack ());
        أرجع result;
      }

      c->serializer->نکالو_discard ();
      إذا (result == LOOKUP_CONDITION_SUBSET_TRUE)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (result == LOOKUP_CONDITION_SUBSET_FALSE)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      أرجع result;
    }

    افتراضي:
      أرجع LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

بنية ConditionSet
{
  منطقي evaluate (ثابت صحيح *coords, unsigned صحيح coord_len,
		 ItemVarStoreInstancer *instancer) ثابت
  {
    unsigned صحيح گنتی = conditions.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
      إذا (!(هذا+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	أرجع خطأ;
    أرجع true;
  }

  فراغ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    إذا (unlikely (!condition_map)) أرجع;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    إذا (unlikely (!cond_set)) أرجع;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = true;
    منطقي should_keep = خطأ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقي can_deduplicate = true;
    unsigned عدد_kept_cond = 0, cond_idx = 0;
    لكل (ثابت auto& offset : conditions)
    {
      ثابت Condition &condition = هذا+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      إذا (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = خطأ;
        أرجع;
      }

      إذا (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = true;
        cond_set->add (cond_idx);
        عدد_kept_cond++;
      }

      إذا (ret == KEEP_RECORD_WITH_VAR)
        should_keep = true;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = عدد_kept_cond == 0;
    إذا (!should_keep && c->all_axes_pinned) أرجع;

    //check if condition_set is unique with variations
    إذا (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      أرجع;

    إذا (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقي داخل_کرو_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    إذا (داخل_کرو_catch_all) return_trace (true);

    hb_set_t *retained_cond_set = nullptr;
    إذا (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned صحيح گنتی = conditions.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (retained_cond_set != nullptr && !retained_cond_set->has (i))
        استمر;
      ثابت Condition &condition = هذا+conditions[i];
      إذا (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, هذا) (conditions[i]);
        استمر;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      إذا (unlikely (!offset)) return_trace (خطأ);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, true);
      إذا (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, خطأ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      إذا (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->نکالو_discard ();
        c->serializer->check_success (خطأ);
        return_trace (خطأ);
      }
      c->serializer->add_link (*offset, c->serializer->نکالو_pack ());
    }

    return_trace (منطقي (out->conditions));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت auto &offset : conditions)
      إذا (unlikely (!(هذا+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_اقدار)))
	c->layout_variation_indices->err ();
  }

  محمي:
  Array16OfOffset32To<Condition>	conditions;
  عام:
  DEFINE_SIZE_ARRAY (2, conditions);
};

بنية FeatureTableSubstitutionRecord
{
  friend بنية FeatureTableSubstitution;

  فراغ collect_lookups (ثابت فراغ *base, hb_set_t *lookup_indexes /* OUT */) ثابت
  {
    أرجع (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  فراغ closure_features (ثابت فراغ *base,
			 ثابت hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    إذا ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  فراغ collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, ثابت Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    ثابت hb_set_t *feature_indices,
                                                    ثابت فراغ *base) ثابت
  {
    إذا (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  منطقي serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  ثابت Feature *f, ثابت Tag *tag)
  {
    TRACE_SERIALIZE (هذا);
    hb_serialize_context_t *s = c->subset_context->serializer;
    إذا (unlikely (!s->extend_min (هذا))) return_trace (خطأ);

    uint32_t *new_feature_idx;
    إذا (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (خطأ);

    إذا (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (خطأ);

    s->push ();
    منطقي ret = f->subset (c->subset_context, c, tag);
    إذا (ret) s->add_link (feature, s->نکالو_pack ());
    وإلا s->نکالو_discard ();

    return_trace (ret);
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_index;
    إذا (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (خطأ);

    auto *out = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    out->featureIndex = *new_feature_index;
    hb_pair_t<ثابت فراغ*, ثابت فراغ*> *original;
    إذا (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (خطأ);
    ثابت Tag *tag = reinterpret_cast<ثابت Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  محمي:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية FeatureTableSubstitution
{
  فراغ collect_feature_indices (hb_set_t *feature_indices) ثابت
  {
    لكل (ثابت FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  ثابت Feature *find_substitute (unsigned صحيح feature_index) ثابت
  {
    unsigned صحيح گنتی = substitutions.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      ثابت FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      إذا (record.featureIndex == feature_index)
	أرجع &(هذا+record.feature);
    }
    أرجع nullptr;
  }

  فراغ collect_lookups (ثابت hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([هذا, lookup_indexes] (ثابت FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (هذا, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ثابت
  {
    لكل (ثابت FeatureTableSubstitutionRecord &record : substitutions)
      إذا (feature_index_map->has (record.featureIndex))
        (هذا+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  فراغ closure_features (ثابت hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    لكل (ثابت FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (هذا, lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت hb_map_t *feature_index_map) ثابت
  {
    لكل (ثابت FeatureTableSubstitutionRecord& record : substitutions)
    {
      إذا (feature_index_map->has (record.featureIndex)) أرجع true;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    لكل (ثابت FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, هذا);
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقي داخل_کرو_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    out->version.major = version.major;
    out->version.minor = version.minor;

    إذا (داخل_کرو_catch_all)
    {
      لكل (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<ثابت فراغ*, ثابت فراغ*> *p;
        إذا (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (خطأ);
        auto *o = out->substitutions.serialize_append (c->serializer);
        إذا (!o->serialize (l, feature_index,
                           reinterpret_cast<ثابت Feature*> (p->first),
                           reinterpret_cast<ثابت Tag*> (p->second)))
          return_trace (خطأ);
      }
      return_trace (true);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), هذا))
    ;

    return_trace (منطقي (out->substitutions));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, هذا, feature_list));
  }

  محمي:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  عام:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

بنية FeatureVariationRecord
{
  friend بنية FeatureVariations;

  فراغ collect_lookups (ثابت فراغ     *base,
			ثابت hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
    أرجع (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  فراغ collect_name_ids (ثابت فراغ *base,
                         ثابت hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ثابت
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  فراغ closure_features (ثابت فراغ     *base,
			 ثابت hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت فراغ *base, ثابت hb_map_t *feature_index_map) ثابت
  {
    أرجع (base+substitutions).intersects_features (feature_index_map);
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت فراغ *base) ثابت
  {
    (base+conditions).keep_with_variations (c);
    إذا (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = true; // set variations only once
    }
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *base,
               منطقي داخل_کرو_catch_all = خطأ) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    إذا (!داخل_کرو_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    وإلا
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, داخل_کرو_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, داخل_کرو_catch_all);

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *base) ثابت
  { (base+conditions).collect_variation_indices (c); }

  محمي:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية LookupIndexList
{
  فراغ add_lookup_indexes_to (hb_set_t *lookup_indexes) ثابت
  { lookupIndices.add_indexes_to (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookupIndices.intersects (lookup_indexes); }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (خطأ);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقي (out->lookupIndices));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (lookupIndices.sanitize (c));
  }

  محمي:
  IndexArray	lookupIndices;
  عام:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

بنية LookupConditionRecord
{
  unsigned get_condition_offset (ثابت فراغ *base, ثابت فراغ *variations) ثابت
  { أرجع (ثابت حرف *) &(base+condition) - (ثابت حرف *) variations; }

  template <typename Instancer>
  منطقي evaluate (ثابت فراغ *base,
		 ثابت صحيح *coords,
		 unsigned صحيح coord_len,
		 Instancer *instancer) ثابت
  { أرجع (base+condition).evaluate (coords, coord_len, instancer); }

  منطقي evaluate (ثابت فراغ *base,
                 ثابت فراغ *variations,
                 hb_sorted_array_t<ثابت unsigned> conditions) ثابت
  {
    أرجع !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     حجم (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  منطقي collect_condition (ثابت فراغ *base,
                          ثابت فراغ *variations,
                          ثابت صحيح *coords,
                          unsigned صحيح coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    إذا (!condition) أرجع true;

    unsigned offset = get_condition_offset (base, variations);
    إذا (visited->has (offset)) أرجع true;
    visited->add (offset);
    إذا (unlikely (visited->in_error ())) أرجع خطأ;

    إذا (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    أرجع !conditions->in_error ();
  }

  فراغ add_lookup_indexes_to (ثابت فراغ *base,
			      hb_set_t *lookup_indexes) ثابت
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت فراغ *base,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *base) ثابت
  {
    إذا (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_اقدار)))
      c->layout_variation_indices->err ();
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت فراغ *base) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out)) return_trace (خطأ);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    true);
    إذا (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->نکالو_discard ();
      return_trace (خطأ);
    }
    c->serializer->add_link (out->condition, c->serializer->نکالو_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  محمي:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  عام:
  DEFINE_SIZE_STATIC (8);
};

بنية FeatureLookupsTable
{
  منطقي adds_default_lookups () ثابت { أرجع flags & 0x0001u; }

  منطقي collect_conditions (ثابت فراغ *variations,
                           ثابت صحيح *coords,
                           unsigned صحيح coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) ثابت
  {
    لكل (ثابت LookupConditionRecord &record : records)
      إذا (unlikely (!record.collect_condition (هذا, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	أرجع خطأ;
    أرجع true;
  }

  فراغ collect_lookups (ثابت فراغ *variations,
                       hb_sorted_array_t<ثابت unsigned> conditions,
                       hb_set_t *lookup_indexes) ثابت
  {
    لكل (ثابت LookupConditionRecord &record : records)
      إذا (record.evaluate (هذا, variations, conditions))
	record.add_lookup_indexes_to (هذا, lookup_indexes);
  }

  template <typename Instancer>
  فراغ collect_lookups (ثابت صحيح *coords,
			unsigned صحيح coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) ثابت
  {
    لكل (ثابت LookupConditionRecord &record : records)
      إذا (record.evaluate (هذا, coords, coord_len, instancer))
	record.add_lookup_indexes_to (هذا, lookup_indexes);
  }

  فراغ collect_lookups (hb_set_t *lookup_indexes) ثابت
  {
    لكل (ثابت LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (هذا, lookup_indexes);
  }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    لكل (ثابت LookupConditionRecord &record : records)
      إذا (record.intersects_lookup_indexes (هذا, lookup_indexes))
	أرجع true;
    أرجع خطأ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت LookupConditionRecord &record : records)
      record.collect_variation_indices (c, هذا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (خطأ);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    لكل (ثابت LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      إذا (record.subset (c, l, هذا))
	out->records.len++;
      وإلا
	c->serializer->revert (snap);
    }

    return_trace (true);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, هذا));
  }

  محمي:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  عام:
  DEFINE_SIZE_ARRAY (10, records);
};

بنية LookupVariationRecord
{
  friend بنية FeatureVariations;

  unsigned get_feature_index () ثابت { أرجع featureIndex; }

  فراغ collect_lookups (ثابت فراغ *base,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت unsigned> conditions,
                       hb_set_t *lookup_indexes) ثابت
  {
    ثابت FeatureLookupsTable &feature_lookups = base+featureLookups;
    إذا (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  فراغ collect_lookups (ثابت فراغ *base,
			ثابت hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) ثابت
  {
    إذا (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  فراغ closure_features (ثابت فراغ *base,
			 ثابت hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) ثابت
  {
    إذا ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *base) ثابت
  { (base+featureLookups).collect_variation_indices (c); }

  منطقي subset (hb_subset_layout_context_t *l, ثابت فراغ *base) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_index;
    إذا (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (خطأ);

    auto *out = c->serializer->embed (هذا);
    إذا (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (خطأ);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *base) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureLookups.sanitize (c, base));
  }

  محمي:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية FeatureVariations
{
  ساكن constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  ثابت Array32Of<LookupVariationRecord> &get_lookup_variation_records () ثابت
  {
    إذا (version.to_int () < 0x00010001u)
      أرجع Null (Array32Of<LookupVariationRecord>);
    أرجع StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_گنتی () ثابت
  {
    أرجع varRecords.len;
  }

  منطقي find_index (ثابت صحيح *coords, unsigned صحيح coord_len,
		   unsigned صحيح *index,
		   ItemVarStoreInstancer *instancer) ثابت
  {
    unsigned صحيح گنتی = varRecords.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      ثابت FeatureVariationRecord &record = varRecords.arrayZ[i];
      إذا ((هذا+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	أرجع true;
      }
    }
    *index = NOT_FOUND_INDEX;
    أرجع خطأ;
  }

  ثابت Feature *find_substitute (unsigned صحيح variations_index,
				  unsigned صحيح feature_index) ثابت
  {
    ثابت FeatureVariationRecord &record = varRecords[variations_index];
    أرجع (هذا+record.substitutions).find_substitute (feature_index);
  }

  منطقي collect_lookup_conditions (ثابت صحيح *coords,
                                  unsigned صحيح coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) ثابت
  {
    hb_set_t visited_tables, visited_conditions;
    لكل (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      إذا (visited_tables.has (offset)) استمر;
      visited_tables.add (offset);
      إذا (unlikely (visited_tables.in_error ())) أرجع خطأ;

      ثابت FeatureLookupsTable &feature_lookups = هذا+record.featureLookups;
      إذا (unlikely (!feature_lookups.collect_conditions (هذا, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	أرجع خطأ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    أرجع true;
  }

  منطقي get_lookup_variations (unsigned feature_index,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت unsigned> conditions,
                              hb_set_t *lookup_indexes) ثابت
  {
    لكل (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
    {
      إذا (record.featureIndex != feature_index) استمر;

      record.collect_lookups (هذا, feature, conditions, lookup_indexes);
      أرجع true;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    unsigned صحيح گنتی = varRecords.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, هذا);
      إذا (c->universal)
        اكسر;
    }
    إذا (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  فراغ collect_lookups (ثابت hb_set_t *feature_indexes,
			ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) ثابت
  {
    unsigned گنتی = varRecords.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        استمر;
      varRecords[i].collect_lookups (هذا, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_index_map,
                         ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) ثابت
  {
    لكل (unsigned i = 0; i < varRecords.len; i++)
    {
      إذا (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        استمر;
      varRecords[i].collect_name_ids (هذا, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  فراغ collect_lookup_variation_lookups (ثابت hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) ثابت
  {
    لكل (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (هذا, feature_indexes, lookup_indexes);
  }

  فراغ closure_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) ثابت
  {
    unsigned صحيح گنتی = varRecords.len;
    لكل (unsigned صحيح i = 0; i < گنتی; i++)
    {
      إذا (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        استمر;
      varRecords[i].closure_features (هذا, lookup_indexes, feature_indexes);
    }

    لكل (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (هذا, lookup_indexes, feature_indexes);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, هذا);
    لكل (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, هذا);
  }

  فراغ collect_feature_indices (hb_set_t *feature_indices) ثابت
  {
    لكل (ثابت FeatureVariationRecord &record : varRecords)
      (هذا+record.substitutions).collect_feature_indices (feature_indices);
  }

  منطقي has_lookup_variations (unsigned feature_index) ثابت
  {
    لكل (ثابت LookupVariationRecord &record : get_lookup_variation_records ())
    {
      إذا (record.featureIndex == feature_index)
	أرجع true;
      إذا (record.featureIndex > feature_index)
	اكسر;
    }
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    auto *out = c->serializer->start_embed (*هذا);
    إذا (unlikely (!out || !c->serializer->extend_min (out))) return_trace (خطأ);

    out->version.major = version.major;
    out->version.minor = version.minor;

    صحيح keep_up_to = -1;
    لكل (صحيح i = varRecords.len - 1; i >= 0; i--) {
      إذا (varRecords[i].intersects_features (هذا, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        اكسر;
      }
    }

    unsigned گنتی = (unsigned) (keep_up_to + 1);
    لكل (unsigned i = 0; i < گنتی; i++)
    {
      إذا (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        استمر;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), هذا) (varRecords[i]);
    }

    إذا (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      منطقي داخل_کرو_catch_all_record = true;
      subset_record_array (l, &(out->varRecords), هذا, داخل_کرو_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    إذا (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      إذا (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (خطأ);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, هذا))
      ;
    }

    return_trace (منطقي (out->varRecords) ||
		  (out_lookup_records && منطقي (*out_lookup_records)));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, هذا, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, هذا)));
  }

  محمي:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  عام:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

بنية HintingDevice
{
  friend بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->y_ppem, font->y_scale); }

  عام:

  size_t get_size () ثابت
  {
    unsigned صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3 || startSize > endSize)) أرجع 3 * HBUINT16::static_size;
    أرجع HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && c->check_range (هذا, هذا->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed<HintingDevice> (هذا));
  }

  خاص:

  صحيح get_delta (unsigned صحيح ppem, صحيح scale) ثابت
  {
    إذا (!ppem) أرجع 0;

    صحيح pixels = get_delta_pixels (ppem);

    إذا (!pixels) أرجع 0;

    أرجع (صحيح) (pixels * (int64_t) scale / ppem);
  }
  صحيح get_delta_pixels (unsigned صحيح ppem_size) ثابت
  {
    unsigned صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3))
      أرجع 0;

    إذا (ppem_size < startSize || ppem_size > endSize)
      أرجع 0;

    unsigned صحيح s = ppem_size - startSize;

    unsigned صحيح byte = delta_قدر_Z[s >> (4 - f)];
    unsigned صحيح bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned صحيح mask = (0xFFFFu >> (16 - (1 << f)));

    صحيح delta = bits & mask;

    إذا ((unsigned صحيح) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    أرجع delta;
  }

  محمي:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_قدر_Z;		/* Array of compressed data */
  عام:
  DEFINE_SIZE_ARRAY (6, delta_قدر_Z);
};

بنية VariationDevice
{
  friend بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  عائم get_x_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, صحيح> *v;
    إذا (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    إذا (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (هذا->static_size);
    auto *out = c->embed (هذا);
    إذا (unlikely (!out)) return_trace (nullptr);

    إذا (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  فراغ collect_variation_index (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->add (varIdx); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:

  عائم get_delta (hb_font_t *font,
		   ثابت ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    أرجع store.get_delta (varIdx, font->coords, font->عدد_coords, store_cache);
  }

  محمي:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية DeviceHeader
{
  محمي:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  عام:
  HBUINT16		format;		/* Format identifier */
  عام:
  DEFINE_SIZE_STATIC (6);
};

بنية Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  عائم get_x_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
#ifndef HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    أرجع get_x_delta (font, store, store_cache);
  }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
#ifndef HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    أرجع get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    بدل (u.b.format)
    {
#ifndef HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      أرجع u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_x_delta (font, store, store_cache);
#endif
    افتراضي:
      أرجع 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) ثابت
  {
    بدل (u.b.format)
    {
    حالة 1: حالة 2: حالة 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      أرجع u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_y_delta (font, store, store_cache);
#endif
    افتراضي:
      أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.b.format.sanitize (c)) return_trace (خطأ);
    بدل (u.b.format) {
#ifndef HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    افتراضي:
      return_trace (true);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map=nullptr) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.b.format) {
#ifndef HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    افتراضي:
      return_trace (nullptr);
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.b.format) {
#ifndef HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      أرجع;
#endif
#ifndef HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      أرجع;
#endif
    افتراضي:
      أرجع;
    }
  }

  unsigned get_variation_index () ثابت
  {
    بدل (u.b.format) {
#ifndef HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.varIdx;
#endif
    افتراضي:
      أرجع HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  منطقي is_variation_device () ثابت
  {
    بدل (u.b.format) {
#ifndef HB_NO_VAR
    حالة 0x8000:
      أرجع true;
#endif
    افتراضي:
      أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  عام:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

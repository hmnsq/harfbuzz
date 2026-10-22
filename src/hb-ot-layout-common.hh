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

#include "hb.hh"
#include "hb-ot-layout.hh"
#include "hb-open-type.hh"
#include "hb-set.hh"
#include "hb-bimap.hh"
#include "hb-cache.hh"
#include "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (parimaa_na (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_binary_cache_t = hb_cache_t<14, 1, 8>;
static_assert (parimaa_na (hb_ot_layout_binary_cache_t) == 256, "");

#include "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


namespace OT {

template<typename Iterator>
static anta_hstha bool ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

static bool ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    const hb_set_t &klasses,
    bool use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

sa_nracanaa ItemVarStoreInstancer;

sa_nracanaa hb_collect_feature_substitutes_with_var_context_t
{
  const hb_map_t *axes_index_tag_map;
  const hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, const Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  bool apply;
  bool variation_applied;
  bool universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  bool all_axes_pinned;
};

sa_nracanaa hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (const void         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              const hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_count (0),langsys_feature_count (0) {}

  bool visitScript ()
  { ལོག script_count++ < HB_MAX_SCRIPTS; }

  bool visitLangsys (unsigned feature_count)
  {
    langsys_feature_count += feature_count;
    ལོག langsys_feature_count < HB_MAX_LANGSYS_FEATURE_COUNT;
  }

  public:
  const void *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  const hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  private:
  unsigned script_count;
  unsigned langsys_feature_count;
};

sa_nracanaa hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  const var_na *get_name () { ལོག "SUBSET_LAYOUT"; }
  static return_t default_return_value () { ལོག hb_empty_t (); }

  bool visitScript ()
  {
    ལོག script_count++ < HB_MAX_SCRIPTS;
  }

  bool visitLangSys ()
  {
    ལོག langsys_count++ < HB_MAX_LANGSYS;
  }

  bool visitFeatureIndex (ཧྲིལ count)
  {
    feature_index_count += count;
    ལོག feature_index_count < HB_MAX_FEATURE_INDICES;
  }

  bool visitLookupIndex()
  {
    lookup_index_count++;
    ལོག lookup_index_count < HB_MAX_LOOKUP_VISIT_COUNT;
  }

  hb_subset_context_t *subset_context;
  const hb_tag_t table_tag;
  const hb_map_t *lookup_index_map;
  const hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  const hb_map_t *feature_index_map;
  const hb_map_t *feature_map_w_duplicates;
  const hb_hashmap_t<unsigned, const Feature*> *feature_substitutes_map;
  const hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  const hb_set_t *catch_all_record_feature_idxes;
  const hb_hashmap_t<unsigned, hb_pair_t<const void*, const void*>> *feature_idx_tag_map;

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
				script_count (0),
				langsys_count (0),
				feature_index_count (0),
				lookup_index_count (0)
  {
    གལ་སྲིད (tag_ == HB_OT_TAG_GSUB)
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
    དེ་མིན
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

  private:
  unsigned script_count;
  unsigned langsys_count;
  unsigned feature_index_count;
  unsigned lookup_index_count;
};

sa_nracanaa ItemVariationStore;
sa_nracanaa hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (const T &obj) { obj.collect_variation_indices (this); ལོག hb_empty_t (); }
  static return_t default_return_value () { ལོག hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  const hb_set_t *glyph_set;
  const hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ཧྲིལ>> *condition_values = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  const hb_set_t *glyph_set_,
					  const hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
sa_nracanaa subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 const void *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  bool operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    གལ་སྲིད (unlikely (!o)) ལོག false;
    bool ret = o->serialize_subset (subset_context, offset, base);
    གལ་སྲིད (!ret)
    {
      out.pop ();
      subset_context->serializer->revert (snap);
    }
    ལོག ret;
  }

  private:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  const void *base;
};


template<typename OutputArray, typename Arg>
sa_nracanaa subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     const void *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  bool operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    གལ་སྲིད (unlikely (!o)) ལོག false;
    bool ret = o->serialize_subset (subset_context, offset, base, arg);
    གལ་སྲིད (!ret)
    {
      out.pop ();
      subset_context->serializer->revert (snap);
    }
    ལོག ret;
  }

  private:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  const void *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
sa_nracanaa
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       const void *base) const
  { ལོག subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       const void *base, Arg &&arg) const
  { ལོག subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
sa_nracanaa subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 const void *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  void
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    bool ret = record.subset (subset_layout_context, base);
    གལ་སྲིད (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    དེ་མིན out->len++;
  }

  private:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  const void *base;
};

template<typename OutputArray, typename Arg>
sa_nracanaa subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     const void *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  void
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    bool ret = record.subset (subset_layout_context, base, arg);
    གལ་སྲིད (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    དེ་མིན out->len++;
  }

  private:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  const void *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
sa_nracanaa
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       const void *base) const
  { ལོག subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               const void *base, Arg &&arg) const
  { ལོག subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
sa_nracanaa serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         const void *base_,
                         const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  bool operator () (T&& record)
  {
    གལ་སྲིད (!serialize_context->copy (record, base, variation_map)) ལོག false;
    out.len++;
    ལོག true;
  }

  private:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  const void *base;
  const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
sa_nracanaa
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               const void *base,
               const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *variation_map) const
  { ལོག serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

sa_nracanaa IndexArray : Array16Of<Index>
{
  bool intersects (const hb_map_t *indexes) const
  { ལོག hb_any (*this, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    གལ་སྲིད (!it) ལོག;
    གལ་སྲིད (unlikely (!c->extend_min ((*this)))) ལོག;

    དོན་ལུ (const auto _ : it)
    {
      གལ་སྲིད (!l->visitLookupIndex()) bha_nga;

      Index i;
      i = _;
      c->copy (i);
      this->len++;
    }
  }

  unsigned ཧྲིལ get_indexes (unsigned ཧྲིལ start_offset,
			    unsigned ཧྲིལ *_count /* IN/OUT */,
			    unsigned ཧྲིལ *_indexes /* OUT */) const
  {
    གལ་སྲིད (_count && _indexes)
    {
      + this->as_array ().sub_array (start_offset, _count)
      | hb_sink (hb_array (_indexes, *_count))
      ;
    }
    ལོག this->len;
  }

  void add_indexes_to (hb_set_t* output /* OUT */) const
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
sa_nracanaa FeatureParamsSize
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (unlikely (!c->check_struct (this))) return_trace (false);
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

    གལ་སྲིད (!designSize)
      return_trace (false);
    དེ་མིན གལ་སྲིད (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (true);
    དེ་མིན གལ་སྲིད (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (false);
    དེ་མིན
      return_trace (true);
  }

  void collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) const
  { nameids_to_retain->add (subfamilyNameID); }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    return_trace ((bool) c->serializer->embed (*this));
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
  public:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
sa_nracanaa FeatureParamsStylisticSet
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (this));
  }

  void collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) const
  { nameids_to_retain->add (uiNameID); }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    return_trace ((bool) c->serializer->embed (*this));
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
  public:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
sa_nracanaa FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_count, hb_codepoint_t *chars) const
  {
    གལ་སྲིད (char_count && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_count)
      | hb_sink (hb_array (chars, *char_count))
      ;
    }
    ལོག characters.len;
  }

  size_t get_size () const
  { ལོག hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  void collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) const
  {
    གལ་སྲིད (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    གལ་སྲིད (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    གལ་སྲིད (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    གལ་སྲིད (!firstParamUILabelNameID || !numNamedParameters || numNamedParameters >= 0x7FFF)
      ལོག;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) numNamedParameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    return_trace ((bool) c->serializer->embed (*this));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
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
  HBUINT16	numNamedParameters;	/* Number of named parameters. (May
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
  public:
  DEFINE_SIZE_ARRAY (14, characters);
};

sa_nracanaa FeatureParams
{
  bool sanitize (hb_sanitize_context_t *c, hb_tag_t tag) const
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ལོག true;
#endif
    TRACE_SANITIZE (this);
    གལ་སྲིད (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    གལ་སྲིད ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    གལ་སྲིད ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (true);
  }

  void collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) const
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ལོག;
#endif
    གལ་སྲིད (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ལོག (u.size.collect_name_ids (nameids_to_retain));
    གལ་སྲིད ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ལོག (u.stylisticSet.collect_name_ids (nameids_to_retain));
    གལ་སྲིད ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ལོག (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  bool subset (hb_subset_context_t *c, const Tag* tag) const
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    ལོག false;
#endif
    TRACE_SUBSET (this);
    གལ་སྲིད (!tag) return_trace (false);
    གལ་སྲིད (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    གལ་སྲིད ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    གལ་སྲིད ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (false);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  const FeatureParamsSize& get_size_params (hb_tag_t tag) const
  {
    གལ་སྲིད (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      ལོག u.size;
    ལོག Null (FeatureParamsSize);
  }
  const FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) const
  {
    གལ་སྲིད ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      ལོག u.stylisticSet;
    ལོག Null (FeatureParamsStylisticSet);
  }
  const FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) const
  {
    གལ་སྲིད ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      ལོག u.characterVariants;
    ལོག Null (FeatureParamsCharacterVariants);
  }
#endif

  private:
  sa_ngha {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  public:
  DEFINE_SIZE_MIN (0);
};

sa_nracanaa Record_sanitize_closure_t {
  hb_tag_t tag;
  const void *list_base;
};

sa_nracanaa Feature
{
  unsigned ཧྲིལ get_lookup_count () const
  { ལོག lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned ཧྲིལ i) const
  { ལོག lookupIndex[i]; }
  unsigned ཧྲིལ get_lookup_indexes (unsigned ཧྲིལ start_index,
				   unsigned ཧྲིལ *lookup_count /* IN/OUT */,
				   unsigned ཧྲིལ *lookup_tags /* OUT */) const
  { ལོག lookupIndex.get_indexes (start_index, lookup_count, lookup_tags); }
  void add_lookup_indexes_to (hb_set_t *lookup_indexes) const
  { lookupIndex.add_indexes_to (lookup_indexes); }

  const FeatureParams &get_feature_params () const
  { ལོག this+featureParams; }

  bool intersects_lookup_indexes (const hb_map_t *lookup_indexes) const
  { ལོག lookupIndex.intersects (lookup_indexes); }

  void collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) const
  {
    གལ་སྲིད (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  bool subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       const Tag                   *tag = nullptr) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->featureParams.serialize_subset (c, featureParams, this, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    གལ་སྲིད (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    དེ་མིན
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

  bool sanitize (hb_sanitize_context_t *c,
		 const Record_sanitize_closure_t *closure = nullptr) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  featureParams.sanitize (c, this, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  public:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
sa_nracanaa Record
{
  ཧྲིལ cmp (hb_tag_t a) const { ལོག tag.cmp (a); }

  bool subset (hb_subset_layout_context_t *c, const void *base, const void *f_sub = nullptr) const
  {
    TRACE_SUBSET (this);
    auto *out = c->subset_context->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);

    གལ་སྲིད (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    const Feature& f = *reinterpret_cast<const Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    bool ret = f.subset (c->subset_context, c, &tag);
    གལ་སྲིད (ret)
      s->add_link (out->offset, s->pop_pack ());
    དེ་མིན
      s->pop_discard ();

    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    const Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (this) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  public:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
sa_nracanaa RecordArrayOf : SortedArray16Of<Record<Type>>
{
  const Offset16To<Type>& get_offset (unsigned ཧྲིལ i) const
  { ལོག (*this)[i].offset; }
  Offset16To<Type>& get_offset (unsigned ཧྲིལ i)
  { ལོག (*this)[i].offset; }
  const Tag& get_tag (unsigned ཧྲིལ i) const
  { ལོག (*this)[i].tag; }
  unsigned ཧྲིལ get_tags (unsigned ཧྲིལ start_offset,
                         unsigned ཧྲིལ *record_count /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) const
  {
    གལ་སྲིད (record_count && record_tags)
    {
      + this->as_array ().sub_array (start_offset, record_count)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_count))
      ;
    }
    ལོག this->len;
  }
  bool find_index (hb_tag_t tag, unsigned ཧྲིལ *index) const
  {
    ལོག this->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
sa_nracanaa RecordListOf : RecordArrayOf<Type>
{
  const Type& operator [] (unsigned ཧྲིལ i) const
  { ལོག this+this->get_offset (i); }

  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + this->iter ()
    | hb_apply (subset_record_array (l, out, this))
    ;
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (RecordArrayOf<Type>::sanitize (c, this));
  }
};

sa_nracanaa RecordListOfFeature : RecordListOf<Feature>
{
  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_enumerate (*this)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, this] (const hb_pair_t<unsigned, const Record<Feature>&>& _)
                {
                  const Feature *f_sub = nullptr;
                  const Feature **f = nullptr;
                  གལ་སྲིད (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, this, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (true);
  }
};

typedef RecordListOf<Feature> FeatureList;


sa_nracanaa LangSys
{
  unsigned ཧྲིལ get_feature_count () const
  { ལོག featureIndex.len; }
  hb_tag_t get_feature_index (unsigned ཧྲིལ i) const
  { ལོག featureIndex[i]; }
  unsigned ཧྲིལ get_feature_indexes (unsigned ཧྲིལ start_offset,
				    unsigned ཧྲིལ *feature_count /* IN/OUT */,
				    unsigned ཧྲིལ *feature_indexes /* OUT */) const
  { ལོག featureIndex.get_indexes (start_offset, feature_count, feature_indexes); }
  void add_feature_indexes_to (hb_set_t *feature_indexes) const
  { featureIndex.add_indexes_to (feature_indexes); }

  bool has_required_feature () const { ལོག reqFeatureIndex != 0xFFFFu; }
  unsigned ཧྲིལ get_required_feature_index () const
  {
    གལ་སྲིད (reqFeatureIndex == 0xFFFFu)
      ལོག Index::NOT_FOUND_INDEX;
   ལོག reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (*this));
  }

  bool compare (const LangSys& o, const hb_map_t *feature_index_map) const
  {
    གལ་སྲིད (reqFeatureIndex != o.reqFeatureIndex)
      ལོག false;

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

    དོན་ལུ (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      གལ་སྲིད (a != b) ལོག false;
    }

    གལ་སྲིད (iter || o_iter) ལོག false;

    ལོག true;
  }

  void collect_features (hb_prune_langsys_context_t *c) const
  {
    གལ་སྲིད (!has_required_feature () && !get_feature_count ()) ལོག;
    གལ་སྲིད (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       const Tag                  *tag = nullptr) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    const uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    གལ་སྲིད (!l->visitFeatureIndex (featureIndex.len))
      return_trace (false);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    bool ret = bool (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 const Record_sanitize_closure_t * = nullptr) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  public:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

sa_nracanaa Script
{
  unsigned ཧྲིལ get_lang_sys_count () const
  { ལོག langSys.len; }
  const Tag& get_lang_sys_tag (unsigned ཧྲིལ i) const
  { ལོག langSys.get_tag (i); }
  unsigned ཧྲིལ get_lang_sys_tags (unsigned ཧྲིལ start_offset,
				  unsigned ཧྲིལ *lang_sys_count /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) const
  { ལོག langSys.get_tags (start_offset, lang_sys_count, lang_sys_tags); }
  const LangSys& get_lang_sys (unsigned ཧྲིལ i) const
  {
    གལ་སྲིད (i == Index::NOT_FOUND_INDEX) ལོག get_default_lang_sys ();
    ལོག this+langSys[i].offset;
  }
  bool find_lang_sys_index (hb_tag_t tag, unsigned ཧྲིལ *index) const
  { ལོག langSys.find_index (tag, index); }

  bool has_default_lang_sys () const           { ལོག defaultLangSys != 0; }
  const LangSys& get_default_lang_sys () const { ལོག this+defaultLangSys; }

  void prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) const
  {
    གལ་སྲིད (!has_default_lang_sys () && !get_lang_sys_count ()) ལོག;
    གལ་སྲིད (!c->visitScript ()) ལོག;

    གལ་སྲིད (!c->script_langsys_map->has (script_index))
    {
      གལ་སྲིད (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	ལོག;
    }

    གལ་སྲིད (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      const LangSys& d = get_default_lang_sys ();
      གལ་སྲིད (c->visitLangsys (d.get_feature_count ())) {
        d.collect_features (c);
      }

      དོན་ལུ (auto _ : + hb_enumerate (langSys))
      {
        const LangSys& l = this+_.second.offset;
        གལ་སྲིད (!c->visitLangsys (l.get_feature_count ())) continue;
        གལ་སྲིད (l.compare (d, c->duplicate_feature_map)) continue;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    དེ་མིན
    {
      དོན་ལུ (auto _ : + hb_enumerate (langSys))
      {
        const LangSys& l = this+_.second.offset;
        གལ་སྲིད (!c->visitLangsys (l.get_feature_count ())) continue;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  bool subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       const Tag                   *tag) const
  {
    TRACE_SUBSET (this);
    གལ་སྲིད (!l->visitScript ()) return_trace (false);
    གལ་སྲིད (tag && !c->plan->layout_scripts.has (*tag))
      ལོག false;

    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    bool defaultLang = false;
    གལ་སྲིད (has_default_lang_sys ())
    {
      c->serializer->push ();
      const LangSys& ls = this+defaultLangSys;
      bool ret = ls.subset (c, l);
      གལ་སྲིད (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->pop_discard ();
	out->defaultLangSys = 0;
      }
      དེ་མིན
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->pop_pack ());
	defaultLang = true;
      }
    }

    const hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    གལ་སྲིད (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (const Record<LangSys>& record) {ལོག l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), this))
      ;
    }

    return_trace (bool (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  bool sanitize (hb_sanitize_context_t *c,
		 const Record_sanitize_closure_t * = nullptr) const
  {
    TRACE_SANITIZE (this);
    return_trace (defaultLangSys.sanitize (c, this) && langSys.sanitize (c, this));
  }

  rak_sita:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  public:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

sa_nracanaa RecordListOfScript : RecordListOf<Script>
{
  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    དོན་ལུ (auto _ : + hb_enumerate (*this))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      bool ret = _.second.subset (l, this);
      གལ་སྲིད (!ret) c->serializer->revert (snap);
      དེ་མིན out->len++;
    }

    return_trace (true);
  }
};

typedef RecordListOfScript ScriptList;



sa_nracanaa LookupFlag : HBUINT16
{
  ga_nanaa Flags {
    RightToLeft		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  public:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
namespace OT {

sa_nracanaa Lookup
{
  unsigned ཧྲིལ get_subtable_count () const { ལོག subTable.len; }

  template <typename TSubTable>
  const Array16OfOffset16To<TSubTable>& get_subtables () const
  { ལོག reinterpret_cast<const Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { ལོག reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  const TSubTable& get_subtable (unsigned ཧྲིལ i) const
  { ལོག this+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned ཧྲིལ i)
  { ལོག this+get_subtables<TSubTable> ()[i]; }

  size_t get_size () const
  {
    const HBUINT16 &markFilteringSet = StructAfter<const HBUINT16> (subTable);
    གལ་སྲིད (lookupFlag & LookupFlag::UseMarkFilteringSet)
      ལོག (const var_na *) &StructAfter<const var_na> (markFilteringSet) - (const var_na *) this;
    ལོག (const var_na *) &markFilteringSet - (const var_na *) this;
  }

  unsigned ཧྲིལ get_type () const { ལོག lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () const
  {
    unsigned ཧྲིལ flag = lookupFlag;
    གལ་སྲིད (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      const HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    ལོག flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    unsigned ཧྲིལ lookup_type = get_type ();
    TRACE_DISPATCH (this, lookup_type);
    unsigned ཧྲིལ count = get_subtable_count ();
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      གལ་སྲིད (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_value ());
  }

  bool serialize (hb_serialize_context_t *c,
		  unsigned ཧྲིལ lookup_type,
		  uint32_t lookup_props,
		  unsigned ཧྲིལ num_subtables)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    གལ་སྲིད (unlikely (!subTable.serialize (c, num_subtables))) return_trace (false);
    གལ་སྲིད (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      གལ་སྲིད (unlikely (!c->extend (this))) return_trace (false);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (true);
  }

  template <typename TSubTable>
  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    const hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned ཧྲིལ lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([this, glyphset, lookup_type] (const Offset16To<TSubTable> &_) { ལོག (this+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), this, lookup_type))
    ;

    གལ་སྲིད (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      const HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      གལ་སྲིད (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      དེ་མིན
      {
        གལ་སྲིད (unlikely (!c->serializer->extend (out))) return_trace (false);
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
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (!(c->check_struct (this) && subTable.sanitize (c))) return_trace (false);
    hb_barrier ();

    unsigned subtables = get_subtable_count ();
    གལ་སྲིད (unlikely (!c->visit_subtables (subtables))) return_trace (false);

    གལ་སྲིད (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      const HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      གལ་སྲིད (!markFilteringSet.sanitize (c)) return_trace (false);
    }

    གལ་སྲིད (unlikely (!get_subtables<TSubTable> ().sanitize (c, this, get_type ())))
      return_trace (false);

    གལ་སྲིད (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned ཧྲིལ type = get_subtable<TSubTable> (0).u.extension.get_type ();
      དོན་ལུ (unsigned ཧྲིལ i = 1; i < subtables; i++)
	གལ་སྲིད (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (false);
    }
    return_trace (true);
  }

  rak_sita:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  public:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
sa_nracanaa LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    གལ་སྲིད (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_enumerate (*this)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, this))
    ;
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, this)));
  }
};


/*
 * Coverage Table
 */


static bool ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  const hb_set_t &klasses,
                                          bool use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  གལ་སྲིད (!klass_map)
    ལོག ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  གལ་སྲིད (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  དོན་ལུ (const unsigned k: klasses)
  {
    གལ་སྲིད (klass_map->has (k)) continue;
    klass_map->set (k, idx);
    idx++;
  }


  གལ་སྲིད (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    ལོག false;

  དོན་ལུ (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  ལོག ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
sa_nracanaa ClassDefFormat1_3
{
  friend sa_nracanaa ClassDef;

  private:
  unsigned ཧྲིལ get_class (hb_codepoint_t glyph_id) const
  {
    ལོག classValue[(unsigned ཧྲིལ) (glyph_id - startGlyph)];
  }

  unsigned get_population () const
  {
    ལོག classValue.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    གལ་སྲིད (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      classValue.len = 0;
      return_trace (true);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_count = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    གལ་སྲིད (unlikely (!classValue.serialize (c, glyph_count))) return_trace (false);
    དོན་ལུ (const hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      classValue[idx] = gid_klass_pair.second;
    }
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               bool keep_empty_table = true,
               bool use_class_zero = true,
               const Coverage* glyph_filter = nullptr) const
  {
    TRACE_SUBSET (this);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + classValue.len;

    དོན་ལུ (const hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      གལ་སྲིད (new_gid == HB_MAP_VALUE_INVALID) continue;
      གལ་སྲིད (glyph_filter && !glyph_filter->has(gid)) continue;

      unsigned klass = classValue[gid - start];
      གལ་སྲིད (!klass) continue;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    གལ་སྲིད (use_class_zero)
    {
      const hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_count = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_count <= glyph_and_klass.length;
    }
    གལ་སྲིད (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (false);
    return_trace (keep_empty_table || (bool) glyph_and_klass);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && classValue.sanitize (c));
  }

  unsigned cost () const { ལོག 1; }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    unsigned ཧྲིལ start = 0;
    unsigned ཧྲིལ count = classValue.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      གལ་སྲིད (classValue[i])
	continue;

      གལ་སྲིད (start != i)
	གལ་སྲིད (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  ལོག false;

      start = i + 1;
    }
    གལ་སྲིད (start != count)
      གལ་སྲིད (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + count)))
	ལོག false;

    ལོག true;
  }

  template <typename set_t>
  bool collect_class (set_t *glyphs, unsigned klass) const
  {
    unsigned ཧྲིལ count = classValue.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
      གལ་སྲིད (classValue[i] == klass) glyphs->add (startGlyph + i);
    ལོག true;
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + classValue.len;
    དོན་ལུ (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      གལ་སྲིད (classValue[iter - start]) ལོག true;
    ལོག false;
  }
  bool intersects_class (const hb_set_t *glyphs, unsigned klass) const
  {
    unsigned ཧྲིལ count = classValue.len;
    གལ་སྲིད (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_VALUE_INVALID;
      གལ་སྲིད (!glyphs->next (&g)) ལོག false;
      གལ་སྲིད (g < startGlyph) ལོག true;
      g = startGlyph + count - 1;
      གལ་སྲིད (glyphs->next (&g)) ལོག true;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    const typename Types::HBUINT *arr = classValue.arrayZ;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
      གལ་སྲིད (arr[i] == klass && glyphs->has (startGlyph + i))
	ལོག true;
    ལོག false;
  }

  void intersected_class_glyphs (const hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) const
  {
    unsigned count = classValue.len;
    གལ་སྲིད (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      དོན་ལུ (uint32_t g = HB_SET_VALUE_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      དོན་ལུ (uint32_t g = startGlyph + count - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      ལོག;
    }

    དོན་ལུ (unsigned i = 0; i < count; i++)
      གལ་སྲིད (classValue[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#གལ་སྲིད 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + count;
    དོན་ལུ (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      གལ་སྲིད (classValue.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  void intersected_classes (const hb_set_t *glyphs, hb_set_t *intersect_classes) const
  {
    གལ་སྲིད (glyphs->is_empty ()) ལོག;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    གལ་སྲིད (!classValue.len)
    {
      intersect_classes->add (0);
      ལོག;
    }

    hb_codepoint_t end_glyph = startGlyph + classValue.len - 1;
    གལ་སྲིད (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    དོན་ལུ (const auto& _ : + hb_enumerate (classValue))
    {
      hb_codepoint_t g = startGlyph + _.first;
      གལ་སྲིད (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  rak_sita:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		classValue;	/* Array of Class Values--one per GlyphID */
  public:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, classValue);
};

template <typename Types>
sa_nracanaa ClassDefFormat2_4
{
  friend sa_nracanaa ClassDef;

  private:
  unsigned ཧྲིལ get_class (hb_codepoint_t glyph_id) const
  {
    ལོག rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () const
  {
    typename Types::large_int ret = 0;
    དོན་ལུ (const auto &r : rangeRecord)
      ret += r.get_population ();
    ལོག ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    གལ་སྲིད (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (true);
    }

    unsigned unsorted = false;
    unsigned num_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    གལ་སྲིད (unlikely (!record)) return_trace (false);

    དོན་ལུ (const auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      གལ་སྲིད (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	གལ་སྲིད (unlikely (cur_gid < prev_gid))
	  unsorted = true;

	གལ་སྲིད (unlikely (!record)) bha_nga;
	record->last = prev_gid;
	num_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    གལ་སྲིད (unlikely (c->in_error ())) return_trace (false);

    གལ་སྲིད (likely (record)) record->last = prev_gid;
    rangeRecord.len = num_ranges;

    གལ་སྲིད (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (true);
  }

  bool subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               bool keep_empty_table = true,
               bool use_class_zero = true,
               const Coverage* glyph_filter = nullptr) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    གལ་སྲིད (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      དོན་ལུ (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	གལ་སྲིད (!klass) continue;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	གལ་སྲིད (new_gid == HB_MAP_VALUE_INVALID) continue;
	གལ་སྲིད (glyph_filter && !glyph_filter->has (g)) continue;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    དེ་མིན
    {
      unsigned num_source_glyphs = c->plan->source->get_num_glyphs ();
      དོན་ལུ (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	གལ་སྲིད (!klass) continue;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, num_source_glyphs);
	དོན་ལུ (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  གལ་སྲིད (new_gid == HB_MAP_VALUE_INVALID) continue;
	  གལ་སྲིད (glyph_filter && !glyph_filter->has (g)) continue;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    const hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_count = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_count <= glyph_and_klass.length;
    གལ་སྲིད (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (false);
    return_trace (keep_empty_table || (bool) glyph_and_klass);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () const { ལོག hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    དོན་ལུ (auto &range : rangeRecord)
      གལ་སྲིད (range.value)
	གལ་སྲིད (unlikely (!range.collect_coverage (glyphs)))
	  ལོག false;
    ལོག true;
  }

  template <typename set_t>
  bool collect_class (set_t *glyphs, unsigned ཧྲིལ klass) const
  {
    དོན་ལུ (auto &range : rangeRecord)
    {
      གལ་སྲིད (range.value == klass)
	གལ་སྲིད (unlikely (!range.collect_coverage (glyphs)))
	  ལོག false;
    }
    ལོག true;
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    གལ་སྲིད (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      དོན་ལུ (auto g : *glyphs)
        གལ་སྲིད (get_class (g))
	  ལོག true;
      ལོག false;
    }

    ལོག hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (const RangeRecord<Types> &range) { ལོག range.intersects (*glyphs) && range.value; }));
  }
  bool intersects_class (const hb_set_t *glyphs, unsigned klass) const
  {
    གལ་སྲིད (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      གལ་སྲིད (!rangeRecord.len)
        ལོག !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_VALUE_INVALID;
      hb_codepoint_t last = HB_SET_VALUE_INVALID;
      auto it = hb_iter (rangeRecord);
      དོན་ལུ (auto &range : it)
      {
        གལ་སྲིད (it->first == last + 1)
	{
	  it++;
	  continue;
	}

	གལ་སྲིད (!glyphs->next (&g))
	  bha_nga;
	གལ་སྲིད (g < range.first)
	  ལོག true;
	g = range.last;
	last = g;
      }
      གལ་སྲིད (g != HB_SET_VALUE_INVALID && glyphs->next (&g))
	ལོག true;
      /* Fall through. */
    }
    དོན་ལུ (const auto &range : rangeRecord)
      གལ་སྲིད (range.value == klass && range.intersects (*glyphs))
	ལོག true;
    ལོག false;
  }

  void intersected_class_glyphs (const hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) const
  {
    གལ་སྲིད (klass == 0)
    {
      hb_codepoint_t g = HB_SET_VALUE_INVALID;
      དོན་ལུ (auto &range : rangeRecord)
      {
	གལ་སྲིད (!glyphs->next (&g))
	  goto done;
	སྐབས (g < range.first)
	{
	  intersect_glyphs->add (g);
	  གལ་སྲིད (!glyphs->next (&g))
	    goto done;
        }
        g = range.last;
      }
      སྐབས (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      ལོག;
    }

    unsigned count = rangeRecord.len;
    གལ་སྲིད (count > glyphs->get_population () * hb_bit_storage (count))
    {
      དོན་ལུ (auto g : *glyphs)
      {
        unsigned i;
        གལ་སྲིད (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      ལོག;
    }

    དོན་ལུ (auto &range : rangeRecord)
    {
      གལ་སྲིད (range.value != klass) continue;

      unsigned end = range.last + 1;
      དོན་ལུ (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  void intersected_classes (const hb_set_t *glyphs, hb_set_t *intersect_classes) const
  {
    གལ་སྲིད (glyphs->is_empty ()) ལོག;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    གལ་སྲིད (!rangeRecord.len)
    {
      intersect_classes->add (0);
      ལོག;
    }

    hb_codepoint_t g = HB_SET_VALUE_INVALID;
    དོན་ལུ (auto &range : rangeRecord)
    {
      གལ་སྲིད (!glyphs->next (&g))
        bha_nga;
      གལ་སྲིད (g < range.first)
      {
        intersect_classes->add (0);
        bha_nga;
      }
      g = range.last;
    }
    གལ་སྲིད (g != HB_SET_VALUE_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    དོན་ལུ (const auto& range : rangeRecord)
      གལ་སྲིད (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  rak_sita:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

sa_nracanaa ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) const { ལོག get (k); }
  bool has (hb_codepoint_t k) const { ལོག (*this)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) const { ལོག get (k); }

  unsigned ཧྲིལ get (hb_codepoint_t k) const { ལོག get_class (k); }
  unsigned ཧྲིལ get_class (hb_codepoint_t glyph_id) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.get_class (glyph_id);
    pak_sa 2: hb_barrier (); ལོག u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.get_class (glyph_id);
    pak_sa 4: hb_barrier (); ལོག u.format4.get_class (glyph_id);
#endif
    default:ལོག 0;
    }
  }
  unsigned ཧྲིལ get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) const
  {
    unsigned klass;
    གལ་སྲིད (cache && cache->get (glyph_id, &klass)) ལོག klass;
    klass = get_class (glyph_id);
    གལ་སྲིད (cache) cache->set (glyph_id, klass);
    ལོག klass;
  }

  unsigned get_population () const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.get_population ();
    pak_sa 2: hb_barrier (); ལོག u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.get_population ();
    pak_sa 4: hb_barrier (); ལོག u.format4.get_population ();
#endif
    default:ལོག NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    གལ་སྲིད (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned num_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      དོན་ལུ (const auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	གལ་སྲིད (cur_klass > class_max) class_max = cur_klass;
	གལ་སྲིད (cur_gid == glyph_min) continue;
        གལ་སྲིད (cur_gid > glyph_max) glyph_max = cur_gid;
	གལ་སྲིད (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  num_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	གལ་སྲིད (size >= best_size) ལོག;
	best_size = size;
	format = candidate;
      };

      གལ་སྲིད (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      གལ་སྲིད (glyph_max <= 0xFFFFu && num_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) num_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      གལ་སྲིད (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      གལ་སྲིད (glyph_max <= 0xFFFFFFu && num_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) num_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      གལ་སྲིད (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (false);
      }
    }

    u.format.v = format;

    switch (u.format.v)
    {
    pak_sa 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    pak_sa 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    pak_sa 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    default:return_trace (false);
    }
  }

  bool subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               bool keep_empty_table = true,
               bool use_class_zero = true,
               const Coverage* glyph_filter = nullptr) const
  {
    TRACE_SUBSET (this);
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    pak_sa 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    pak_sa 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    default:return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    pak_sa 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    pak_sa 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    default:return_trace (true);
    }
  }

  unsigned cost () const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.cost ();
    pak_sa 2: hb_barrier (); ལོག u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.cost ();
    pak_sa 4: hb_barrier (); ལོག u.format4.cost ();
#endif
    default:ལོག 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.collect_coverage (glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.collect_coverage (glyphs);
    pak_sa 4: hb_barrier (); ལོག u.format4.collect_coverage (glyphs);
#endif
    default:ལོག false;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  bool collect_class (set_t *glyphs, unsigned ཧྲིལ klass) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.collect_class (glyphs, klass);
    pak_sa 2: hb_barrier (); ལོག u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.collect_class (glyphs, klass);
    pak_sa 4: hb_barrier (); ལོག u.format4.collect_class (glyphs, klass);
#endif
    default:ལོག false;
    }
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersects (glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersects (glyphs);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersects (glyphs);
#endif
    default:ལོག false;
    }
  }
  bool intersects_class (const hb_set_t *glyphs, unsigned ཧྲིལ klass) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersects_class (glyphs, klass);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersects_class (glyphs, klass);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersects_class (glyphs, klass);
#endif
    default:ལོག false;
    }
  }

  void intersected_class_glyphs (const hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    default:ལོག;
    }
  }

  void intersected_classes (const hb_set_t *glyphs, hb_set_t *intersect_classes) const
  {
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); ལོག u.format1.intersected_classes (glyphs, intersect_classes);
    pak_sa 2: hb_barrier (); ལོག u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    pak_sa 3: hb_barrier (); ལོག u.format3.intersected_classes (glyphs, intersect_classes);
    pak_sa 4: hb_barrier (); ལོག u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    default:ལོག;
    }
  }


  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
static anta_hstha bool ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ ལོག (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
sa_nracanaa delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  sa_nracanaa chars_t : hb_vector_t<uint8_t>
  {
    ཧྲིལ cmp (const chars_t& other) const
    {
      ལོག as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      དོན་ལུ (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      ལོག hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (const chars_t& other) const
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      དོན་ལུ (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      ལོག hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (const delta_row_encoding_t& other_encoding) const { ལོག chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<const hb_vector_t<ཧྲིལ>*> items;

  delta_row_encoding_t () = default;
  delta_row_encoding_t (hb_vector_t<const hb_vector_t<ཧྲིལ>*> &&rows, unsigned num_cols)
  {
    assert (rows);

    items = std::move (rows);

    གལ་སྲིད (unlikely (!chars.resize (num_cols)))
      ལོག;

    calculate_chars ();
  }

  void merge (const delta_row_encoding_t& other)
  {
    items.alloc (items.length + other.items.length);
    དོན་ལུ (auto &row : other.items)
      add_row (row);

    // Merge chars
    assert (chars.length == other.chars.length);
    དོན་ལུ (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  void chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  void calculate_chars ()
  {
    assert (items);

    bool long_words = false;

    དོན་ལུ (auto &row : items)
    {
      assert (row->length == chars.length);

      /* 0/1/2 byte encoding */
      དོན་ལུ (unsigned i = 0; i < row->length; i++)
      {
	ཧྲིལ v =  row->arrayZ[i];
	གལ་སྲིད (v == 0)
	  continue;
	དེ་མིན གལ་སྲིད (v > 32767 || v < -32768)
	{
	  long_words = true;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	དེ་མིན གལ་སྲིད (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	དེ་མིན
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    གལ་སྲིད (long_words)
    {
      // Convert 1s to 2s
      དོན་ལུ (auto &v : chars)
	གལ་སྲིད (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  bool is_empty () const
  { ལོག !items; }

  bool in_error () const
  { ལོག chars.in_error () || items.in_error (); }

  static anta_hstha unsigned get_chars_overhead (unsigned num_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    ལོག c + num_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) const
  {
    ཧྲིལ count = items.length;
    ལོག hb_max (0, (ཧྲིལ) overhead - count * (ཧྲིལ) additional_bytes_per_rows);
  }

  ཧྲིལ gain_from_merging (const delta_row_encoding_t& other_encoding) const
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    གལ་སྲིད (additional_bytes_per_rows > 0)
    {
      གལ་སྲིད (get_gain (additional_bytes_per_rows) == 0)
        ལོག 0;
    }
    དེ་མིན
    {
      གལ་སྲིད (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	ལོག 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    ཧྲིལ combined_gain = (ཧྲིལ) overhead + (ཧྲིལ) other_encoding.overhead;
    combined_gain -= (combined_width - (ཧྲིལ) width) * items.length;
    combined_gain -= (combined_width - (ཧྲིལ) other_encoding.width) * other_encoding.items.length;
    combined_gain -= get_chars_overhead (combined_columns);

    ལོག combined_gain;
  }

  bool add_row (const hb_vector_t<ཧྲིལ>* row)
  { ལོག items.push (row); }

  static ཧྲིལ cmp (const void *pa, const void *pb)
  {
    const delta_row_encoding_t *a = (const delta_row_encoding_t *)pa;
    const delta_row_encoding_t *b = (const delta_row_encoding_t *)pb;

    གལ་སྲིད (a->width != b->width)
      ལོག (ཧྲིལ) a->width - (ཧྲིལ) b->width;

    ལོག b->chars.cmp (a->chars);
  }
};

sa_nracanaa VarRegionAxis
{
  float evaluate (ཧྲིལ coord) const
  {
    ཧྲིལ peak = peakCoord.to_int ();
    གལ་སྲིད (peak == 0 || coord == peak)
      ལོག 1.f;
    དེ་མིན གལ་སྲིད (coord == 0) // Faster
      ལོག 0.f;

    ཧྲིལ start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    གལ་སྲིད (unlikely (start > peak || peak > end))
      ལོག 1.f;
    གལ་སྲིད (unlikely (start < 0 && end > 0))
      ལོག 1.f;

    གལ་སྲིད (coord <= start || end <= coord)
      ལོག 0.f;

    /* Interpolate */
    གལ་སྲིད (coord < peak)
      ལོག float (coord - start) / (peak - start);
    དེ་མིན
      ལོག float (end - coord) / (end - peak);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  bool serialize (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (this));
  }

  public:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  public:
  DEFINE_SIZE_STATIC (6);
};
sa_nracanaa SparseVarRegionAxis
{
  float evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len) const
  {
    unsigned i = axisIndex;
    ཧྲིལ coord = i < coord_len ? coords[i] : 0;
    ལོག axis.evaluate (coord);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  bool serialize (hb_serialize_context_t *c,
		  const SparseVarRegionAxis *src,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    auto *out = c->embed (src);
    གལ་སྲིད (unlikely (!out)) return_trace (false);
    གལ་སྲིད (axis_map)
    {
      གལ་སྲིད (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (false);
      }
      གལ་སྲིད (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (false);
    }
    return_trace (true);
  }

  public:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa hb_scalar_cache_t
{
  private:
  static constexpr unsigned STATIC_LENGTH = 128;
  static constexpr float INVALID = NAN;

  public:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (const hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (const hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  static hb_scalar_cache_t *create (unsigned ཧྲིལ count,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    གལ་སྲིད (!count) ལོག (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    གལ་སྲིད (scratch_cache && count <= STATIC_LENGTH)
    {
      scratch_cache->length = count;
      scratch_cache->clear ();
      ལོག scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (parimaa_na (hb_scalar_cache_t) - parimaa_na (static_values) + parimaa_na (static_values[0]) * count);
    གལ་སྲིད (unlikely (!cache)) ལོག (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = count;
    cache->clear ();

    ལོག cache;
  }

  static void destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    གལ་སྲིད (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  void clear ()
  {
    auto *values = &static_values[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    དོན་ལུ (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    དོན་ལུ (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  bool get (unsigned i, float *value) const
  {
    གལ་སྲིད (unlikely (i >= length))
    {
      *value = 0.f;
      ལོག true;
    }
    auto *values = &static_values[0];
    auto *cached_value = &values[i];
    float v = *cached_value;
    གལ་སྲིད (std::isnan (v))
      ལོག false;
    *value = v;
    ལོག true;
  }

  HB_ALWAYS_INLINE
  void set (unsigned i, float value)
  {
    གལ་སྲིད (unlikely (i >= length)) ལོག;
    auto *values = &static_values[0];
    auto *cached_value = &values[i];
    *cached_value = value;
  }

  public:
  unsigned length;
  private:
  mutable hb_atomic_t<float> static_values[STATIC_LENGTH];
};

sa_nracanaa VarRegionList
{
  private:
  float evaluate_impl (unsigned ཧྲིལ region_index,
		       const ཧྲིལ *coords, unsigned ཧྲིལ coord_len) const
  {
    const VarRegionAxis *axes = axesZ.arrayZ + (region_index * axisCount);
    float v = 1.f;

    unsigned ཧྲིལ count = axisCount;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      ཧྲིལ coord = i < coord_len ? coords[i] : 0;
      float factor = axes[i].evaluate (coord);
      གལ་སྲིད (factor == 0.f)
      {
	v = 0.f;
	bha_nga;
      }
      v *= factor;
    }

    ལོག v;
  }

  public:
  HB_ALWAYS_INLINE
  float evaluate (unsigned ཧྲིལ region_index,
		  const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    གལ་སྲིད (unlikely (region_index >= regionCount))
      ལོག 0.;

    float v;
    གལ་སྲིད (cache && cache->get (region_index, &v))
      ལོག v;

    v = evaluate_impl (region_index, coords, coord_len);

    གལ་སྲིད (cache)
      cache->set (region_index, v);
    ལོག v;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axisCount * regionCount));
  }

  bool serialize (hb_serialize_context_t *c,
                  const hb_vector_t<hb_tag_t>& axis_tags,
                  const hb_vector_t<const hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (this);
    unsigned axis_count = axis_tags.length;
    unsigned region_count = regions.length;
    གལ་སྲིད (!axis_count || !region_count) return_trace (false);
    གལ་སྲིད (unlikely (hb_unsigned_mul_overflows (axis_count * region_count,
                                             VarRegionAxis::static_size))) return_trace (false);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    axisCount = axis_count;
    regionCount = region_count;

    དོན་ལུ (unsigned r = 0; r < region_count; r++)
    {
      const auto& region = regions[r];
      དོན་ལུ (unsigned i = 0; i < axis_count; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        གལ་སྲིད (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->middle);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        དེ་མིན
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        གལ་སྲིད (!var_region_rec.serialize (c))
          return_trace (false);
      }
    }
    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c, const VarRegionList *src, const hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    axisCount = src->axisCount;
    regionCount = region_map.get_population ();
    གལ་སྲིད (unlikely (hb_unsigned_mul_overflows (axisCount * regionCount,
					     VarRegionAxis::static_size))) return_trace (false);
    གལ་སྲིད (unlikely (!c->extend (this))) return_trace (false);
    unsigned ཧྲིལ region_count = src->regionCount;
    དོན་ལུ (unsigned ཧྲིལ r = 0; r < regionCount; r++)
    {
      unsigned ཧྲིལ backward = region_map.backward (r);
      གལ་སྲིད (backward >= region_count) return_trace (false);
      hb_memcpy (&axesZ[axisCount * r], &src->axesZ[axisCount * backward], VarRegionAxis::static_size * axisCount);
    }

    return_trace (true);
  }

  bool get_var_region (unsigned region_index,
                       const hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) const
  {
    གལ་སྲིད (region_index >= regionCount) ལོག false;
    const VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axisCount);
    དོན་ལུ (unsigned i = 0; i < axisCount; i++)
    {
      hb_tag_t *axis_tag;
      གལ་སྲིད (!axes_old_index_tag_map.has (i, &axis_tag))
        ལོག false;

      float min_val = axis_region->startCoord.to_float ();
      float def_val = axis_region->peakCoord.to_float ();
      float max_val = axis_region->endCoord.to_float ();

      གལ་སྲིད (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((dviguu_na) min_val, (dviguu_na) def_val, (dviguu_na) max_val));
      axis_region++;
    }
    ལོག !axis_tuples.in_error ();
  }

  bool get_var_regions (const hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) const
  {
    གལ་སྲིད (!regions.alloc (regionCount))
      ལོག false;

    དོན་ལུ (unsigned i = 0; i < regionCount; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      གལ་སྲིད (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        ལོག false;
      regions.push (std::move (axis_tuples));
    }
    ལོག !regions.in_error ();
  }

  size_t get_size () const
  { ལོག hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axisCount, regionCount)); }

  public:
  HBUINT16	axisCount;
  HBUINT15	regionCount;
  rak_sita:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  public:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
static anta_hstha bool
_hb_avar2_region_is_dead (const hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  དོན་ལུ (auto _ : axis_tuples)
  {
    Triple *range;
    གལ་སྲིད (!reachable_ranges.has (_.first, &range)) continue;
    const Triple &tent = _.second;
    ཧྲིལ start = (ཧྲིལ) roundf ((float) tent.minimum * 16384.f);
    ཧྲིལ peak  = (ཧྲིལ) roundf ((float) tent.middle * 16384.f);
    ཧྲིལ end   = (ཧྲིལ) roundf ((float) tent.maximum * 16384.f);
    གལ་སྲིད (!peak) continue;
    གལ་སྲིད (start > peak || peak > end) continue;
    གལ་སྲིད (start < 0 && end > 0) continue;
    ཧྲིལ lo = (ཧྲིལ) roundf ((float) range->minimum * 16384.f);
    ཧྲིལ hi = (ཧྲིལ) roundf ((float) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    གལ་སྲིད (hi < start || lo > end)
      ལོག true;
  }
  ལོག false;
}

sa_nracanaa SparseVariationRegion
{
  float evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len) const
  {
    float v = 1.f;
    unsigned ཧྲིལ count = axes.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      float factor = (this+axes[i]).evaluate (coords, coord_len);
      གལ་སྲིད (factor == 0.f)
	ལོག 0.;
      v *= factor;
    }
    ལོག v;
  }

  bool serialize (hb_serialize_context_t *c,
		  const SparseVariationRegion *src,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (false);
    དོན་ལུ (unsigned i = 0; i < axes.len; i++)
      གལ་སྲིད (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (axes.sanitize (c, this));
  }

  rak_sita:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  public:
  DEFINE_SIZE_ARRAY (2, axes);
};

sa_nracanaa SparseVarRegionList
{
  HB_ALWAYS_INLINE
  float evaluate (unsigned ཧྲིལ region_index,
		  const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    གལ་སྲིད (unlikely (region_index >= regions.len))
      ལོག 0.;

    float v;
    གལ་སྲིད (cache && cache->get (region_index, &v))
      ལོག v;

    const SparseVariationRegion &region = this+regions[region_index];

    v = region.evaluate (coords, coord_len);
    གལ་སྲིད (cache)
      cache->set (region_index, v);

    ལོག v;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (regions.sanitize (c, this));
  }

  bool serialize (hb_serialize_context_t *c,
		  const SparseVarRegionList *src,
		  const hb_inc_bimap_t &region_map,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    regions.len = region_map.get_population ();
    གལ་སྲིད (unlikely (!c->extend (regions))) return_trace (false);

    དོན་ལུ (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      གལ་སྲིད (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  public:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  public:
  DEFINE_SIZE_ARRAY (4, regions);
};


sa_nracanaa VarData
{
  unsigned ཧྲིལ get_item_count () const
  { ལོག itemCount; }

  unsigned ཧྲིལ get_region_index_count () const
  { ལོག regionIndices.len; }

  unsigned get_region_index (unsigned i) const
  { ལོག i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () const
  { ལོག (wordCount () + regionIndices.len) * (longWords () ? 2 : 1); }

  size_t get_size () const
  { ལོག hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (itemCount, get_row_size ())); }

  template <typename Float = float>
  Float _get_delta (unsigned ཧྲིལ inner,
		    const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		    const VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) const
  {
    གལ་སྲིད (unlikely (inner >= itemCount))
      ལོག 0.;
    bool is_long = longWords ();
    unsigned ཧྲིལ count = regionIndices.len;
    unsigned word_count = wordCount ();
    unsigned ཧྲིལ scount = is_long ? count : word_count;
    unsigned ཧྲིལ lcount = is_long ? word_count : 0;

    const HBUINT8 *bytes = get_delta_bytes ();
    const HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned ཧྲིལ i = 0;

    const HBINT32 *lcursor = reinterpret_cast<const HBINT32 *> (row);
    དོན་ལུ (; i < lcount; i++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count, cache);
      གལ་སྲིད (scalar)
        delta += (Float) scalar * (ཧྲིལ) *lcursor;
      lcursor++;
    }
    const HBINT16 *scursor = reinterpret_cast<const HBINT16 *> (lcursor);
    དོན་ལུ (; i < scount; i++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count, cache);
      གལ་སྲིད (scalar)
       delta += (Float) scalar * (ཧྲིལ) *scursor;
      scursor++;
    }
    const HBINT8 *bcursor = reinterpret_cast<const HBINT8 *> (scursor);
    དོན་ལུ (; i < count; i++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count, cache);
      གལ་སྲིད (scalar)
        delta += (Float) scalar * (ཧྲིལ) *bcursor;
      bcursor++;
    }

    ལོག delta;
  }

  template <typename Float = float>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned ཧྲིལ inner,
		   const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		   const VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) const
  {
    unsigned ཧྲིལ count = regionIndices.len;
    གལ་སྲིད (!count) ལོག Float (0); // This is quite common, so optimize it.
    ལོག _get_delta<Float> (inner, coords, coord_count, regions, cache);
  }

  void get_region_scalars (const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
			   const VarRegionList &regions,
			   float *scalars /*OUT */,
			   unsigned ཧྲིལ num_scalars) const
  {
    unsigned count = hb_min (num_scalars, regionIndices.len);
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count);
    དོན་ལུ (unsigned ཧྲིལ i = count; i < num_scalars; i++)
      scalars[i] = 0.f;
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  wordCount () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  itemCount,
				  get_row_size ()));
  }

  bool serialize (hb_serialize_context_t *c,
                  bool has_long,
                  const hb_vector_t<const hb_vector_t<ཧྲིལ>*>& rows)
  {
    TRACE_SERIALIZE (this);
    unsigned row_count = rows.length;
    གལ་སྲིད (!row_count) {
      // Nothing to serialize, will be empty.
      ལོག false;
    }

    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    གལ་སྲིད (unlikely (!c->check_assign (itemCount, row_count,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    ཧྲིལ min_threshold = has_long ? -32768 : -128;
    ཧྲིལ max_threshold = has_long ? +32767 : +127;
    ga_nanaa delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned num_regions = rows[0]->length;
    གལ་སྲིད (!delta_sz.resize (num_regions))
      return_trace (false);

    unsigned word_count = 0;
    དོན་ལུ (unsigned r = 0; r < num_regions; r++)
    {
      དོན་ལུ (unsigned i = 0; i < row_count; i++)
      {
        ཧྲིལ delta = rows[i]->arrayZ[r];
        གལ་སྲིད (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_count++;
          bha_nga;
        }
        དེ་མིན གལ་སྲིད (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_count;
    hb_map_t ri_map;
    དོན་ལུ (unsigned r = 0; r < num_regions; r++)
    {
      གལ་སྲིད (!delta_sz[r]) continue;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      གལ་སྲིད (!ri_map.set (new_r, r))
        return_trace (false);
    }

    wordSizeCount = word_count | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_count = ri_map.get_population ();
    regionIndices.len = ri_count;
    གལ་སྲིད (unlikely (!c->extend (this))) return_trace (false);

    དོན་ལུ (unsigned r = 0; r < ri_count; r++)
    {
      hb_codepoint_t *idx;
      གལ་སྲིད (!ri_map.has (r, &idx))
        return_trace (false);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < row_count; i++)
    {
      དོན་ལུ (unsigned ཧྲིལ r = 0; r < ri_count; r++)
      {
        ཧྲིལ delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c,
		  const VarData *src,
		  const hb_inc_bimap_t &inner_map,
		  const hb_inc_bimap_t &region_map,
		  const hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    itemCount = inner_map.get_next_value ();

    /* Optimize word count */
    unsigned ri_count = src->regionIndices.len;
    ga_nanaa delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned ཧྲིལ> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_count);
    ri_map.resize (ri_count);
    unsigned ཧྲིལ new_word_count = 0;
    unsigned ཧྲིལ r;

    const HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_count = src->wordCount ();
    bool     src_long_words = src->longWords ();

    bool has_long = false;
    གལ་སྲིད (src_long_words)
    {
      དོན་ལུ (r = 0; r < src_word_count; r++)
      {
        དོན་ལུ (unsigned old_gid : inner_map.keys())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  གལ་སྲིད (delta < -32768 || 32767 < delta)
	  {
	    has_long = true;
	    bha_nga;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    དོན་ལུ (r = 0; r < ri_count; r++)
    {
      bool short_circuit = src_long_words == has_long && src_word_count <= r;

      delta_sz[r] = kZero;
      གལ་སྲིད (culled_regions && culled_regions->has (src->regionIndices[r]))
	continue; /* unreachable region: drop its column */
      དོན་ལུ (unsigned old_gid : inner_map.keys())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	གལ་སྲིད (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_count++;
	  bha_nga;
	}
	དེ་མིན གལ་སྲིད (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  གལ་སྲིད (short_circuit)
	    bha_nga;
	}
      }
    }

    unsigned ཧྲིལ word_index = 0;
    unsigned ཧྲིལ non_word_index = new_word_count;
    unsigned ཧྲིལ new_ri_count = 0;
    དོན་ལུ (r = 0; r < ri_count; r++)
      གལ་སྲིད (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_count++;
      }

    wordSizeCount = new_word_count | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_count;

    གལ་སྲིད (unlikely (!c->extend (this))) return_trace (false);

    དོན་ལུ (r = 0; r < new_ri_count; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned count = itemCount;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      unsigned ཧྲིལ old = inner_map.backward (i);
      དོན་ལུ (unsigned ཧྲིལ r = 0; r < new_ri_count; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (true);
  }

  void collect_region_refs (hb_set_t &region_indices, const hb_inc_bimap_t &inner_map) const
  {
    const HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    དོན་ལུ (unsigned ཧྲིལ r = 0; r < regionIndices.len; r++)
    {
      unsigned ཧྲིལ region = regionIndices.arrayZ[r];
      གལ་སྲིད (region_indices.has (region)) continue;
      དོན་ལུ (hb_codepoint_t old_gid : inner_map.keys())
	གལ་སྲིད (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  bha_nga;
	}
    }
  }

  public:
  const HBUINT8 *get_delta_bytes () const
  { ལོག &StructAfter<HBUINT8> (regionIndices); }

  rak_sita:
  HBUINT8 *get_delta_bytes ()
  { ལོག &StructAfter<HBUINT8> (regionIndices); }

  public:
  int32_t get_item_delta_fast (unsigned ཧྲིལ item, unsigned ཧྲིལ region,
			       const HBUINT8 *delta_bytes, unsigned row_size) const
  {
    གལ་སྲིད (unlikely (item >= itemCount || region >= regionIndices.len)) ལོག 0;

    const HBINT8 *p = (const HBINT8 *) delta_bytes + item * row_size;
    unsigned word_count = wordCount ();
    bool is_long = longWords ();
    གལ་སྲིད (is_long)
    {
      གལ་སྲིད (region < word_count)
	ལོག ((const HBINT32 *) p)[region];
      དེ་མིན
	ལོག ((const HBINT16 *)(p + HBINT32::static_size * word_count))[region - word_count];
    }
    དེ་མིན
    {
      གལ་སྲིད (region < word_count)
	ལོག ((const HBINT16 *) p)[region];
      དེ་མིན
	ལོག (p + HBINT16::static_size * word_count)[region - word_count];
    }
  }
  int32_t get_item_delta (unsigned ཧྲིལ item, unsigned ཧྲིལ region) const
  {
     ལོག get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  rak_sita:
  void set_item_delta_fast (unsigned ཧྲིལ item, unsigned ཧྲིལ region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_count = wordCount ();
    bool is_long = longWords ();
    གལ་སྲིད (is_long)
    {
      གལ་སྲིད (region < word_count)
	((HBINT32 *) p)[region] = delta;
      དེ་མིན
	((HBINT16 *)(p + HBINT32::static_size * word_count))[region - word_count] = delta;
    }
    དེ་མིན
    {
      གལ་སྲིད (region < word_count)
	((HBINT16 *) p)[region] = delta;
      དེ་མིན
	(p + HBINT16::static_size * word_count)[region - word_count] = delta;
    }
  }
  void set_item_delta (unsigned ཧྲིལ item, unsigned ཧྲིལ region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  bool longWords () const { ལོག wordSizeCount & 0x8000u /* LONG_WORDS */; }
  unsigned wordCount () const { ལོག wordSizeCount & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  rak_sita:
  HBUINT16		itemCount;
  HBUINT16		wordSizeCount;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  public:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

sa_nracanaa MultiVarData
{
  size_t get_size () const
  { ལོག hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  const TupleList &get_delta_sets () const
  { ལོག this+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  void get_delta (unsigned ཧྲིལ inner,
		  const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		  const SparseVarRegionList &regions,
		  hb_array_t<dviguu_na> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    auto &deltaSets = get_delta_sets ();

    auto values_iter = deltaSets.fetcher (inner);
    unsigned regionCount = regionIndices.len;
    unsigned skip = 0;
    དོན་ལུ (unsigned regionIndex = 0; regionIndex < regionCount; regionIndex++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_count,
				       cache);
      // We skip lazily. Helps with the tail end.
      གལ་སྲིད (scalar == 0.0f)
        skip += out.length;
      དེ་མིན
      {
        གལ་སྲིད (skip)
	{
	  values_iter.skip (skip);
	  skip = 0;
	}
	values_iter.add_to (out, scalar);
      }
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, this));
  }

  unsigned get_item_count () const
  { ལོག get_delta_sets ().count; }

  void collect_region_refs (hb_set_t &region_indices) const
  {
    དོན་ལུ (unsigned region : regionIndices)
      region_indices.add (region);
  }

  bool serialize (hb_serialize_context_t *c,
		  const MultiVarData *src,
		  const hb_inc_bimap_t &inner_map,
		  const hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (this);
    unsigned region_count = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_count * HBUINT16::static_size +
			   HBUINT32::static_size;
    གལ་སྲིད (unlikely (!c->extend_size (this, header_size, false)))
      return_trace (false);
    format = 1;
    regionIndices.len = region_count;
    དོན་ལུ (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      གལ་སྲིད (unlikely (!region_map.has (old_region)))
	return_trace (false);
      regionIndices[i] = region_map.get (old_region);
    }

    const auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    གལ་སྲིད (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (false);
    དོན་ལུ (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      གལ་སྲིད (unlikely (old_inner >= src_delta_sets.count)) return_trace (false);
      hb_ubytes_t bytes = static_cast<const CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    གལ་སྲིད (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (false);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  rak_sita:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  public:
  DEFINE_SIZE_MIN (7);
};

sa_nracanaa ItemVariationStore
{
  friend sa_nracanaa item_variations_t;

  hb_scalar_cache_t *create_cache () const
  {
#ifdef HB_NO_VAR
    ལོག hb_scalar_cache_t::create (0);
#endif
    ལོག hb_scalar_cache_t::create ((this+regions).regionCount);
  }

  static void destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  private:
  template <typename Float = float>
  Float get_delta (unsigned ཧྲིལ outer, unsigned ཧྲིལ inner,
		   const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		   hb_scalar_cache_t *cache = nullptr) const
  {
#ifdef HB_NO_VAR
    ལོག Float (0);
#endif

    གལ་སྲིད (unlikely (outer >= dataSets.len))
      ལོག Float (0);

    ལོག (this+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_count,
					     this+regions,
					     cache);
  }

  public:
  bool has_delta_set (unsigned ཧྲིལ index) const
  {
#ifdef HB_NO_VAR
    ལོག false;
#endif

    unsigned ཧྲིལ outer = index >> 16;
    unsigned ཧྲིལ inner = index & 0xFFFF;
    ལོག outer < dataSets.len &&
	   inner < (this+dataSets[outer]).get_item_count ();
  }

  template <typename Float = float>
  Float get_delta (unsigned ཧྲིལ index,
		   const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		   hb_scalar_cache_t *cache = nullptr) const
  {
    unsigned ཧྲིལ outer = index >> 16;
    unsigned ཧྲིལ inner = index & 0xFFFF;
    ལོག get_delta<Float> (outer, inner, coords, coord_count, cache);
  }
  template <typename Float = float>
  Float get_delta (unsigned ཧྲིལ index,
		   hb_array_t<const ཧྲིལ> coords,
		   hb_scalar_cache_t *cache = nullptr) const
  {
    ལོག get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
#ifdef HB_NO_VAR
    ལོག true;
#endif

    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, this) &&
		  dataSets.sanitize (c, this));
  }

  bool serialize (hb_serialize_context_t *c,
                  bool has_long,
                  const hb_vector_t<hb_tag_t>& axis_tags,
                  const hb_vector_t<const hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  const hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (this);
#ifdef HB_NO_VAR
    return_trace (false);
#endif
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    format = 1;
    གལ་སྲིད (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (false);

    unsigned num_var_data = vardata_encodings.length;
    གལ་སྲིད (!num_var_data) return_trace (false);
    གལ་སྲིད (unlikely (!c->check_assign (dataSets.len, num_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    གལ་སྲིད (unlikely (!c->extend (dataSets))) return_trace (false);
    དོན་ལུ (unsigned i = 0; i < num_var_data; i++)
      གལ་སྲིད (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].items))
        return_trace (false);

    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c,
		  const ItemVariationStore *src,
		  const hb_array_t <const hb_inc_bimap_t> &inner_maps,
		  const hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (this);
#ifdef HB_NO_VAR
    return_trace (false);
#endif

    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    unsigned ཧྲིལ set_count = 0;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < inner_maps.length; i++)
      གལ་སྲིད (inner_maps[i].get_population ())
	set_count++;

    format = 1;

    const auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    གལ་སྲིད (region_indices.in_error ())
      return_trace (false);

    region_indices.del_range ((src_regions).regionCount, hb_set_t::INVALID);

    གལ་སྲིད (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    གལ་སྲིད (region_map.in_error())
      return_trace (false);

    གལ་སྲིད (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (false);

    dataSets.len = set_count;
    གལ་སྲིད (unlikely (!c->extend (dataSets))) return_trace (false);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned ཧྲིལ set_index = 0;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < inner_maps.length; i++)
    {
      གལ་སྲིད (!inner_maps[i].get_population ()) continue;
      གལ་སྲིད (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (false);
    }

    return_trace (true);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->start_embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned count = dataSets.len;
    དོན་ལུ (unsigned i = 0; i < count; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      གལ་སྲིད (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = this+dataSets[i];

      unsigned itemCount = data.get_item_count ();
      དོན་ལུ (unsigned j = 0; j < itemCount; j++)
	map->add (j);
    }

    གལ་སྲིད (unlikely (!out->serialize (c, this, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  bool collect_dead_regions (const hb_map_t &axes_old_index_tag_map,
			     const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) const
  {
#ifndef HB_NO_VAR
    གལ་སྲིད (!reachable_ranges.get_population ()) ལོག true;
    const VarRegionList &region_list = this+regions;
    unsigned count = region_list.regionCount;
    དོན་ལུ (unsigned i = 0; i < count; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      གལ་སྲིད (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	ལོག false;
      གལ་སྲིད (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    ལོག !dead_regions.in_error ();
#དེ་མིན
    ལོག true;
#endif
  }

  bool subset (hb_subset_context_t *c, const hb_array_t<const hb_inc_bimap_t> &inner_maps) const
  {
    TRACE_SUBSET (this);
#ifdef HB_NO_VAR
    return_trace (false);
#endif

    ItemVariationStore *varstore_prime = c->serializer->start_embed<ItemVariationStore> ();
    གལ་སྲིད (unlikely (!varstore_prime)) return_trace (false);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    གལ་སྲིད (c->plan->has_avar2)
      གལ་སྲིད (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (false);

    varstore_prime->serialize (c->serializer, this, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_prime->dataSets);
  }

  unsigned ཧྲིལ get_region_index_count (unsigned ཧྲིལ major) const
  {
#ifdef HB_NO_VAR
    ལོག 0;
#endif
    ལོག (this+dataSets[major]).get_region_index_count ();
  }

  void get_region_scalars (unsigned ཧྲིལ major,
			   const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
			   float *scalars /*OUT*/,
			   unsigned ཧྲིལ num_scalars) const
  {
#ifdef HB_NO_VAR
    དོན་ལུ (unsigned i = 0; i < num_scalars; i++)
      scalars[i] = 0.f;
    ལོག;
#endif

    (this+dataSets[major]).get_region_scalars (coords, coord_count,
					       this+regions,
					       &scalars[0], num_scalars);
  }

  unsigned ཧྲིལ get_sub_table_count () const
   {
#ifdef HB_NO_VAR
     ལོག 0;
#endif
     ལོག dataSets.len;
   }

  const VarData& get_sub_table (unsigned i) const
  {
#ifdef HB_NO_VAR
     ལོག Null (VarData);
#endif
     ལོག this+dataSets[i];
  }

  const VarRegionList& get_region_list () const
  {
#ifdef HB_NO_VAR
     ལོག Null (VarRegionList);
#endif
     ལོག this+regions;
  }

  rak_sita:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  public:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

sa_nracanaa MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) const
  {
#ifdef HB_NO_VAR
    ལོག hb_scalar_cache_t::create (0);
#endif
    auto &r = this+regions;
    unsigned count = r.regions.len;

    ལོག hb_scalar_cache_t::create (count, static_cache);
  }

  static void destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  private:
  void get_delta (unsigned ཧྲིལ outer, unsigned ཧྲིལ inner,
		  const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		  hb_array_t<dviguu_na> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
#ifdef HB_NO_VAR
    ལོག;
#endif

    གལ་སྲིད (unlikely (outer >= dataSets.len))
      ལོག;

    ལོག (this+dataSets[outer]).get_delta (inner,
					     coords, coord_count,
					     this+regions,
					     out,
					     cache);
  }

  public:
  void get_delta (unsigned ཧྲིལ index,
		  const ཧྲིལ *coords, unsigned ཧྲིལ coord_count,
		  hb_array_t<dviguu_na> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    unsigned ཧྲིལ outer = index >> 16;
    unsigned ཧྲིལ inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_count, out, cache);
  }
  void get_delta (unsigned ཧྲིལ index,
		  hb_array_t<const ཧྲིལ> coords,
		  hb_array_t<dviguu_na> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    ལོག get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
#ifdef HB_NO_VAR
    ལོག true;
#endif

    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, this) &&
		  dataSets.sanitize (c, this));
  }

  bool create_subset_plan (const hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) const
  {
    གལ་སྲིད (unlikely (!inner_maps->resize (dataSets.len))) ལོག false;
    དོན་ལུ (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      གལ་སྲིད (unlikely (outer >= dataSets.len ||
		    inner >= (this+dataSets[outer]).get_item_count ()))
	ལོག false;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    དོན་ལུ (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      const hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      གལ་སྲིད (!inner_map.get_population ()) continue;
      གལ་སྲིད (unlikely (inner_map.in_error ())) ལོག false;
      དོན་ལུ (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    ལོག !inner_maps->in_error () && !varidx_map->in_error ();
  }

  bool serialize (hb_serialize_context_t *c,
		  const MultiItemVariationStore *src,
		  const hb_array_t<const hb_inc_bimap_t> &inner_maps,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (false);
    format = 1;

    const auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_count = 0;
    དོན་ལུ (unsigned i = 0; i < src->dataSets.len; i++)
    {
      གལ་སྲིད (!inner_maps[i].get_population ()) continue;
      set_count++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    གལ་སྲིད (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (false);

    dataSets.len = set_count;
    གལ་སྲིད (unlikely (!c->extend (dataSets))) return_trace (false);
    unsigned set_index = 0;
    དོན་ལུ (unsigned i = 0; i < src->dataSets.len; i++)
    {
      གལ་སྲིད (!inner_maps[i].get_population ()) continue;
      གལ་སྲིད (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  rak_sita:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  public:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename MapCountT>
sa_nracanaa DeltaSetIndexMapFormat01
{
  friend sa_nracanaa DeltaSetIndexMap;

  size_t get_size () const
  { ལོག hb_unsigned_mul_add_saturate (mapCount, get_width (), min_size); }

  private:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (this));
  }

  template <typename T>
  bool serialize (hb_serialize_context_t *c, const T &plan)
  {
    unsigned ཧྲིལ width = plan.get_width ();
    unsigned ཧྲིལ inner_bit_count = plan.get_inner_bit_count ();
    const hb_array_t<const uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (output_map.length && ((((inner_bit_count-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (false);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);

    entryFormat = ((width-1)<<4)|(inner_bit_count-1);
    mapCount = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    གལ་སྲིད (unlikely (!p)) return_trace (false);
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < output_map.length; i++)
    {
      unsigned ཧྲིལ v = output_map.arrayZ[i];
      གལ་སྲིད (v)
      {
	unsigned ཧྲིལ outer = v >> 16;
	unsigned ཧྲིལ inner = v & 0xFFFF;
	unsigned ཧྲིལ u = (outer << inner_bit_count) | inner;
	དོན་ལུ (unsigned ཧྲིལ w = width; w > 0;)
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
  uint32_t map (unsigned ཧྲིལ v) const /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    གལ་སྲིད (!mapCount)
      ལོག v;
    ལོག _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned ཧྲིལ v) const /* Returns 16.16 outer.inner. */
  {
    གལ་སྲིད (v >= mapCount)
      v = mapCount - 1;

    unsigned ཧྲིལ u = 0;
    { /* Fetch it. */
      unsigned ཧྲིལ w = get_width ();
      const HBUINT8 *p = mapDataZ.arrayZ + w * v;
      དོན་ལུ (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned ཧྲིལ n = get_inner_bit_count ();
      unsigned ཧྲིལ outer = u >> n;
      unsigned ཧྲིལ inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    ལོག u;
  }

  unsigned get_map_count () const       { ལོག mapCount; }
  unsigned get_width () const           { ལོག ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_count () const { ལོག (entryFormat & 0xF) + 1; }


  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  mapCount,
                                  get_width ()));
  }

  rak_sita:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  MapCountT     mapCount;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  public:
  DEFINE_SIZE_ARRAY (2+MapCountT::static_size, mapDataZ);
};

sa_nracanaa DeltaSetIndexMap
{
  template <typename T>
  bool serialize (hb_serialize_context_t *c, const T &plan)
  {
    TRACE_SERIALIZE (this);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    pak_sa 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    default:return_trace (false);
    }
  }

  uint32_t map (unsigned v) const
  {
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); ལོག (u.format0.map (v));
    pak_sa 1: hb_barrier (); ལོག (u.format1.map (v));
    default:ལོག v;
    }
  }

  unsigned get_map_count () const
  {
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); ལོག u.format0.get_map_count ();
    pak_sa 1: hb_barrier (); ལོག u.format1.get_map_count ();
    default:ལོག 0;
    }
  }

  unsigned get_width () const
  {
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); ལོག u.format0.get_width ();
    pak_sa 1: hb_barrier (); ལོག u.format1.get_width ();
    default:ལོག 0;
    }
  }

  unsigned get_inner_bit_count () const
  {
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); ལོག u.format0.get_inner_bit_count ();
    pak_sa 1: hb_barrier (); ལོག u.format1.get_inner_bit_count ();
    default:ལོག 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    pak_sa 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    default:return_trace (true);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    switch (u.format.v) {
    pak_sa 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    pak_sa 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    default:return_trace (nullptr);
    }
  }

  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  public:
  DEFINE_SIZE_UNION (1, format.v);
};


sa_nracanaa ItemVarStoreInstancer
{
  ItemVarStoreInstancer (const ItemVariationStore *varStore_,
			 const DeltaSetIndexMap *varIdxMap,
			 hb_array_t<const ཧྲིལ> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    གལ་སྲིད (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    གལ་སྲིད (!hb_any (coords))
      this->coords = {};
  }

  operator bool () const { ལོག varStore && bool (coords); }

  float operator[] (uint32_t varIdx) const
  { ལོག (*this) (varIdx); }

  float operator() (uint32_t varIdx, unsigned short offset = 0) const
  { ལོག get_delta (varIdx, offset); }

  dviguu_na get_condition_delta (uint32_t varIdx) const
  { ལོག get_delta<dviguu_na> (varIdx); }

  template <typename Float = float>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) const
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   གལ་སྲིད (!coords || varIdx == VarIdx::NO_VARIATION)
     ལོག Float (0);

    varIdx += offset;
    གལ་སྲིད (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    ལོག varStore->get_delta<Float> (varIdx, coords, cache);
  }

  const ItemVariationStore *varStore;
  const DeltaSetIndexMap *varIdxMap;
  hb_array_t<const ཧྲིལ> coords;
  hb_scalar_cache_t *cache;
};

sa_nracanaa MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (const MultiItemVariationStore *varStore,
			      const DeltaSetIndexMap *varIdxMap,
			      hb_array_t<const ཧྲིལ> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    གལ་སྲིད (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator bool () const { ལོག varStore; }

  dviguu_na get_condition_delta (uint32_t varIdx) const
  { ལོག (dviguu_na) (*this)[varIdx]; }

  dviguu_na operator[] (uint32_t varIdx) const
  {
    dviguu_na v = 0;
    (*this) (hb_array (&v, 1), varIdx);
    ལོག v;
  }

  void operator() (hb_array_t<dviguu_na> out, uint32_t varIdx, unsigned short offset = 0) const
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    གལ་སྲིད (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      གལ་སྲིད (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    དེ་མིན
      དོན་ལུ (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  const MultiItemVariationStore *varStore;
  const DeltaSetIndexMap *varIdxMap;
  hb_array_t<const ཧྲིལ> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
ga_nanaa Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

ga_nanaa lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

sa_nracanaa Condition;

template <typename Instancer>
static bool
_hb_recurse_condition_evaluate (const sa_nracanaa Condition &condition,
				const ཧྲིལ *coords,
				unsigned ཧྲིལ coord_len,
				Instancer *instancer);

sa_nracanaa ConditionAxisRange
{
  friend sa_nracanaa Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);

    const hb_map_t *index_map = &c->plan->axes_index_map;
    གལ་སྲིད (index_map->is_empty ()) return_trace (true);

    const hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    གལ་སྲིད (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (false);

    const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    གལ་སྲིད (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    གལ་སྲིད (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    float normalized_min = renormalizeValue ((dviguu_na) filterRangeMinValue.to_float (), axis_limit, axis_triple_distances, false);
    float normalized_max = renormalizeValue ((dviguu_na) filterRangeMaxValue.to_float (), axis_limit, axis_triple_distances, false);
    out->filterRangeMinValue.set_float (normalized_min);
    out->filterRangeMaxValue.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  private:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) const
  {
    //invalid axis index, drop the entire record
    གལ་སྲིད (!c->axes_index_tag_map->has (axisIndex))
      ལོག DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    bool axis_set_by_user = false;
    གལ་སྲིད (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = true;
    }

    float axis_min_val = axis_range.minimum;
    float axis_default_val = axis_range.middle;
    float axis_max_val = axis_range.maximum;

    float filter_min_val = filterRangeMinValue.to_float ();
    float filter_max_val = filterRangeMaxValue.to_float ();

    གལ་སྲིད (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = false;

    //condition not met, drop the entire record
    གལ་སྲིད (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      ལོག DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    གལ་སྲིད (axis_set_by_user && axis_range.is_point ())
      ལོག DROP_COND_WITH_VAR;

    གལ་སྲིད (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filterRangeMaxValue.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filterRangeMinValue.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      ལོག KEEP_COND_WITH_VAR;
    }
    ལོག KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 Instancer *instancer HB_UNUSED) const
  {
    ཧྲིལ coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    ལོག filterRangeMinValue.to_int () <= coord && coord <= filterRangeMaxValue.to_int ();
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filterRangeMinValue;
  F2DOT14	filterRangeMaxValue;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa ConditionValue
{
  friend sa_nracanaa Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);

    གལ་སྲིད (varIdx == VarIdx::NO_VARIATION)
      return_trace (true);

    hb_pair_t<unsigned, ཧྲིལ> *condition_value;
    གལ་སྲིད (c->plan->layout_condition_idx_value_map.has ((uintptr_t) this, &condition_value))
    {
      out->defaultValue = condition_value->second;
      out->varIdx = condition_value->first;
      return_trace (true);
    }

    hb_pair_t<unsigned, ཧྲིལ> *new_varidx_delta;
    གལ་སྲིད (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (false);

    གལ་སྲིད (unlikely (!c->serializer->check_assign (
		out->defaultValue,
		defaultValue + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (true);
  }

  private:
  template <typename Instancer>
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 Instancer *instancer) const
  {
    dviguu_na value = defaultValue;
    value += instancer->get_condition_delta (varIdx);
    ལོག value > 0;
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	defaultValue;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa ConditionAnd
{
  friend sa_nracanaa Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);
    out->format = format;

    དོན་ལུ (const auto &offset : conditions)
    {
      གལ་སྲིད (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	གལ་སྲིད (unlikely (!out_offset)) return_trace (false);
	*out_offset = 0;
      }
      དེ་མིན གལ་སྲིད (unlikely (!subset_offset_array (c, out->conditions, this) (offset)))
	return_trace (false);
    }

    return_trace (true);
  }

  private:
  template <typename Instancer>
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 Instancer *instancer) const
  {
    unsigned ཧྲིལ count = conditions.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
      གལ་སྲིད (!_hb_recurse_condition_evaluate (this+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	ལོག false;
    ལོག true;
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<sa_nracanaa Condition>	conditions;
  public:
  DEFINE_SIZE_ARRAY (3, conditions);
};

sa_nracanaa ConditionOr
{
  friend sa_nracanaa Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);
    out->format = format;

    དོན་ལུ (const auto &offset : conditions)
    {
      གལ་སྲིད (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	གལ་སྲིད (unlikely (!out_offset)) return_trace (false);
	*out_offset = 0;
      }
      དེ་མིན གལ་སྲིད (unlikely (!subset_offset_array (c, out->conditions, this) (offset)))
	return_trace (false);
    }

    return_trace (true);
  }

  private:
  template <typename Instancer>
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 Instancer *instancer) const
  {
    unsigned ཧྲིལ count = conditions.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
      གལ་སྲིད (_hb_recurse_condition_evaluate (this+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	ལོག true;
    ལོག false;
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<sa_nracanaa Condition>	conditions;
  public:
  DEFINE_SIZE_ARRAY (3, conditions);
};

sa_nracanaa ConditionNegate
{
  friend sa_nracanaa Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);
    གལ་སྲིད (condition.is_null ())
    {
      out->condition = 0;
      return_trace (true);
    }
    return_trace (out->condition.serialize_subset (c, condition, this));
  }

  private:
  template <typename Instancer>
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 Instancer *instancer) const
  {
    ལོག !_hb_recurse_condition_evaluate (this+condition,
					    coords, coord_len,
					    instancer);
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (condition.sanitize (c, this));
  }

  rak_sita:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<sa_nracanaa Condition>	condition;
  public:
  DEFINE_SIZE_STATIC (5);
};

sa_nracanaa Condition
{
  friend sa_nracanaa ConditionSet;

  static bool serialize_constant (hb_serialize_context_t *c, bool value)
  {
    auto *out = c->start_embed<ConditionValue> ();
    གལ་སྲིད (unlikely (!out || !c->extend_min (out))) ལོག false;
    out->format = 2;
    out->defaultValue = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    ལོག true;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      bool materialize_true = false) const;

  template <typename Instancer>
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 Instancer *instancer) const
  {
    switch (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    pak_sa 0: ལོག true;
    pak_sa 1: hb_barrier (); ལོག u.format1.evaluate (coords, coord_len, instancer);
    pak_sa 2: hb_barrier (); ལོག u.format2.evaluate (coords, coord_len, instancer);
    pak_sa 3: hb_barrier (); ལོག u.format3.evaluate (coords, coord_len, instancer);
    pak_sa 4: hb_barrier (); ལོག u.format4.evaluate (coords, coord_len, instancer);
    pak_sa 5: hb_barrier (); ལོག u.format5.evaluate (coords, coord_len, instancer);
    default:ལོག false;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) const
  {
    གལ་སྲིད (unlikely (!depth))
    {
      c->apply = false;
      ལོག DROP_RECORD_WITH_VAR;
    }

    switch (u.format.v) {
    pak_sa 0: ལོག KEEP_RECORD_WITH_VAR;
    pak_sa 1: hb_barrier (); ལོག u.format1.keep_with_variations (c, condition_map);
    pak_sa 2: pak_sa 3: pak_sa 4: pak_sa 5: bha_nga;
    default: c->apply = false; ལོག DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    bool applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    གལ་སྲིད (c->all_axes_pinned)
      ལོག applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    གལ་སྲིད (u.format.v == 2)
      ལོག u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (const Condition &child)
    {
      auto child_context = *c;
      child_context.apply = true;
      ལོག child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    གལ་སྲིད (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      ལོག result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    bool is_and = u.format.v == 3;
    bool variable = false;
    const auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    const void *base = is_and ? (const void *) &u.format3 : (const void *) &u.format4;
    དོན་ལུ (const auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      གལ་སྲིད (is_and && result == DROP_RECORD_WITH_VAR)
        ལོག DROP_RECORD_WITH_VAR;
      གལ་སྲིད (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        ལོག KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    ལོག variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    གལ་སྲིད (unlikely (!c->may_dispatch (this, &u.format.v))) ལོག c->no_dispatch_return_value ();
    TRACE_DISPATCH (this, u.format.v);
    switch (u.format.v) {
    pak_sa 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    pak_sa 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    pak_sa 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    pak_sa 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    pak_sa 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    default:return_trace (c->default_return_value ());
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);

    གལ་སྲིད (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_value ());

    return_trace (c->end_recursion (this->dispatch (c)));
  }

  bool collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ཧྲིལ>> *condition_values = nullptr) const;

  bool serialize (hb_serialize_context_t *c,
		  const Condition *src,
		  const hb_map_t &varidx_map,
		  const hb_map_t *axis_map = nullptr);

  private:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) const;

  rak_sita:
  sa_ngha {
  sa_nracanaa { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  ConditionValue	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  public:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
bool
_hb_recurse_condition_evaluate (const sa_nracanaa Condition &condition,
				const ཧྲིལ *coords,
				unsigned ཧྲིལ coord_len,
				Instancer *instancer)
{
  ལོག condition.evaluate (coords, coord_len, instancer);
}

sa_nracanaa ConditionList
{
  const Condition& operator[] (unsigned i) const
  { ལོག this+conditions[i]; }

  unsigned get_count () const { ལོག conditions.len; }

  bool serialize (hb_serialize_context_t *c,
		  const ConditionList *src,
		  const hb_inc_bimap_t &condition_map,
		  const hb_map_t &varidx_map,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (unlikely (!c->extend_min (this))) return_trace (false);
    conditions.len = condition_map.get_population ();
    གལ་སྲིད (unlikely (!c->extend (conditions))) return_trace (false);
    དོན་ལུ (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      གལ་སྲིད (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, this));
  }

  rak_sita:
  Array32OfOffset32To<Condition> conditions;
  public:
  DEFINE_SIZE_ARRAY (4, conditions);
};

anta_hstha bool
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, ཧྲིལ>> *condition_values) const
{
  གལ་སྲིད (unlikely (!depth)) ལོག false;
  switch (u.format.v)
  {
    pak_sa 0:
    pak_sa 1:
      ལོག true;
    pak_sa 2:
      གལ་སྲིད (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        གལ་སྲིད (condition_values &&
            !condition_values->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), ཧྲིལ (u.format2.defaultValue))))
          ལོག false;
      }
      ལོག !var_indices->in_error ();
    pak_sa 3:
      དོན་ལུ (const auto &offset : u.format3.conditions)
	གལ་སྲིད (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_values)))
	  ལོག false;
      ལོག true;
    pak_sa 4:
      དོན་ལུ (const auto &offset : u.format4.conditions)
	གལ་སྲིད (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_values)))
	  ལོག false;
      ལོག true;
    pak_sa 5:
      ལོག (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_values);
    default:
      ལོག false;
  }
}

anta_hstha bool
Condition::serialize (hb_serialize_context_t *c,
		      const Condition *src,
		      const hb_map_t &varidx_map,
		      const hb_map_t *axis_map)
{
  TRACE_SERIALIZE (this);
  switch (src->u.format.v)
  {
    pak_sa 0:
    {
      auto *out = c->start_embed<ConditionValue> ();
      གལ་སྲིད (unlikely (!out || !c->extend_min (out))) return_trace (false);
      out->format = 2;
      out->defaultValue = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (true);
    }
    pak_sa 1:
    {
      auto *out = c->embed (&src->u.format1);
      གལ་སྲིད (unlikely (!out)) return_trace (false);
      གལ་སྲིད (axis_map)
      {
	གལ་སྲིད (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (false);
	}
	གལ་སྲིད (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (false);
      }
      return_trace (true);
    }
    pak_sa 2:
    {
      auto *out = c->embed (&src->u.format2);
      གལ་སྲིད (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (false);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (true);
    }
    pak_sa 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      གལ་སྲིད (unlikely (!c->extend_min (out))) return_trace (false);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      གལ་སྲིད (unlikely (!c->extend (out->conditions))) return_trace (false);
      དོན་ལུ (unsigned i = 0; i < out->conditions.len; i++)
	གལ་སྲིད (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (false);
      return_trace (true);
    }
    pak_sa 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      གལ་སྲིད (unlikely (!c->extend_min (out))) return_trace (false);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      གལ་སྲིད (unlikely (!c->extend (out->conditions))) return_trace (false);
      དོན་ལུ (unsigned i = 0; i < out->conditions.len; i++)
	གལ་སྲིད (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (false);
      return_trace (true);
    }
    pak_sa 5:
    {
      auto *out = c->embed (&src->u.format5);
      གལ་སྲིད (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (false);
      return_trace (true);
    }
    default:
      return_trace (false);
  }
}

anta_hstha lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    bool materialize_true) const
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_KEEP)
    ལོག result;

  c->serializer->revert (snap);
  གལ་སྲིད (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    ལོག result;

  ལོག serialize_constant (c->serializer, true) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

anta_hstha lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) const
{
  གལ་སྲིད (unlikely (!depth))
    ལོག LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   const Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    གལ་སྲིད (unlikely (!offset))
      ལོག LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->pop_pack ());
    དེ་མིན
    {
      c->serializer->pop_discard ();
      out.pop ();
      c->serializer->revert (snap);
    }
    ལོག result;
  };

  switch (u.format.v)
  {
    pak_sa 0:
      ལོག LOOKUP_CONDITION_SUBSET_TRUE;

    pak_sa 1:
    {
      const ConditionAxisRange &src = u.format1;
      གལ་སྲིད (c->plan->user_axes_location.is_empty ())
        ལོག c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      གལ་སྲིད (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      གལ་སྲིད (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      float filter_min = src.filterRangeMinValue.to_float ();
      float filter_max = src.filterRangeMaxValue.to_float ();
      གལ་སྲིད (filter_min > filter_max)
        ལོག LOOKUP_CONDITION_SUBSET_FALSE;
      གལ་སྲིད (axis_limit.is_point ())
        ལོག filter_min <= axis_limit.middle &&
	       axis_limit.middle <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      གལ་སྲིད (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        ལོག LOOKUP_CONDITION_SUBSET_FALSE;
      གལ་སྲིད (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        ལོག LOOKUP_CONDITION_SUBSET_TRUE;
      གལ་སྲིད (!c->plan->axes_index_map.has (src.axisIndex))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      གལ་སྲིད (unlikely (!out))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      གལ་སྲིད (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filterRangeMinValue.set_float (
	  renormalizeValue (filter_min, axis_limit, axis_distances, false));
      out->filterRangeMaxValue.set_float (
	  renormalizeValue (filter_max, axis_limit, axis_distances, false));
      གལ་སྲིད (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;
      ལོག LOOKUP_CONDITION_SUBSET_KEEP;
    }

    pak_sa 2:
    {
      const ConditionValue &src = u.format2;
      auto *out = c->serializer->embed (&src);
      གལ་སྲིད (unlikely (!out))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;

      གལ་སྲིད (src.varIdx == VarIdx::NO_VARIATION)
        ལོག c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ཧྲིལ) src.defaultValue > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, ཧྲིལ> *condition_value;
      གལ་སྲིད (c->plan->layout_condition_idx_value_map.has ((uintptr_t) &src, &condition_value))
      {
        out->defaultValue = condition_value->second;
        out->varIdx = condition_value->first;
        གལ་སྲིད (out->varIdx == VarIdx::NO_VARIATION)
          ལོག out->defaultValue > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        ལོག LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, ཧྲིལ> *new_varidx_delta;
      གལ་སྲིད (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        ལོག c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((ཧྲིལ) src.defaultValue > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      གལ་སྲིད (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        dviguu_na value = (ཧྲིལ) src.defaultValue +
                      c->plan->layout_variation_delta (src.varIdx);
        ལོག value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      ཧྲིལ default_value = (ཧྲིལ) src.defaultValue +
			  hb_second (*new_varidx_delta);
      གལ་སྲིད (unlikely (!c->serializer->check_assign (
		out->defaultValue,
		default_value,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      ལོག LOOKUP_CONDITION_SUBSET_KEEP;
    }

    pak_sa 3:
    {
      const ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out)))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      དོན་ལུ (const auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          ལོག result;
      }
      ལོག out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    pak_sa 4:
    {
      const ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out)))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      དོན་ལུ (const auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          ལོག result;
      }
      ལོག out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    pak_sa 5:
    {
      const ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      གལ་སྲིད (unlikely (!out))
        ལོག LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->pop_pack ());
        ལོག result;
      }

      c->serializer->pop_discard ();
      གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_TRUE)
        ལོག LOOKUP_CONDITION_SUBSET_FALSE;
      གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_FALSE)
        ལོག LOOKUP_CONDITION_SUBSET_TRUE;
      ལོག result;
    }

    default:
      ལོག LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

sa_nracanaa ConditionSet
{
  bool evaluate (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		 ItemVarStoreInstancer *instancer) const
  {
    unsigned ཧྲིལ count = conditions.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
      གལ་སྲིད (!(this+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	ལོག false;
    ལོག true;
  }

  void keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) const
  {
    hb_map_t *condition_map = hb_map_create ();
    གལ་སྲིད (unlikely (!condition_map)) ལོག;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    གལ་སྲིད (unlikely (!cond_set)) ལོག;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = true;
    bool should_keep = false;
    /* The range map cannot identify value or boolean expressions uniquely. */
    bool can_deduplicate = true;
    unsigned num_kept_cond = 0, cond_idx = 0;
    དོན་ལུ (const auto& offset : conditions)
    {
      const Condition &condition = this+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      གལ་སྲིད (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = false;
        ལོག;
      }

      གལ་སྲིད (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = true;
        cond_set->add (cond_idx);
        num_kept_cond++;
      }

      གལ་སྲིད (ret == KEEP_RECORD_WITH_VAR)
        should_keep = true;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = num_kept_cond == 0;
    གལ་སྲིད (!should_keep && c->all_axes_pinned) ལོག;

    //check if condition_set is unique with variations
    གལ་སྲིད (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      ལོག;

    གལ་སྲིད (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               bool insert_catch_all) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    གལ་སྲིད (insert_catch_all) return_trace (true);

    hb_set_t *retained_cond_set = nullptr;
    གལ་སྲིད (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned ཧྲིལ count = conditions.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      གལ་སྲིད (retained_cond_set != nullptr && !retained_cond_set->has (i))
        continue;
      const Condition &condition = this+conditions[i];
      གལ་སྲིད (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, this) (conditions[i]);
        continue;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      གལ་སྲིད (unlikely (!offset)) return_trace (false);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, true);
      གལ་སྲིད (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, false) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      གལ་སྲིད (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->pop_discard ();
        c->serializer->check_success (false);
        return_trace (false);
      }
      c->serializer->add_link (*offset, c->serializer->pop_pack ());
    }

    return_trace (bool (out->conditions));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, this));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    དོན་ལུ (const auto &offset : conditions)
      གལ་སྲིད (unlikely (!(this+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_values)))
	c->layout_variation_indices->err ();
  }

  rak_sita:
  Array16OfOffset32To<Condition>	conditions;
  public:
  DEFINE_SIZE_ARRAY (2, conditions);
};

sa_nracanaa FeatureTableSubstitutionRecord
{
  friend sa_nracanaa FeatureTableSubstitution;

  void collect_lookups (const void *base, hb_set_t *lookup_indexes /* OUT */) const
  {
    ལོག (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  void closure_features (const void *base,
			 const hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    གལ་སྲིད ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  void collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, const Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    const hb_set_t *feature_indices,
                                                    const void *base) const
  {
    གལ་སྲིད (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  bool serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  const Feature *f, const Tag *tag)
  {
    TRACE_SERIALIZE (this);
    hb_serialize_context_t *s = c->subset_context->serializer;
    གལ་སྲིད (unlikely (!s->extend_min (this))) return_trace (false);

    uint32_t *new_feature_idx;
    གལ་སྲིད (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (false);

    གལ་སྲིད (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    s->push ();
    bool ret = f->subset (c->subset_context, c, tag);
    གལ་སྲིད (ret) s->add_link (feature, s->pop_pack ());
    དེ་མིན s->pop_discard ();

    return_trace (ret);
  }

  bool subset (hb_subset_layout_context_t *c, const void *base) const
  {
    TRACE_SUBSET (this);
    uint32_t *new_feature_index;
    གལ་སྲིད (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (false);

    auto *out = c->subset_context->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);

    out->featureIndex = *new_feature_index;
    hb_pair_t<const void*, const void*> *original;
    གལ་སྲིད (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (false);
    const Tag *tag = reinterpret_cast<const Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base,
		 const FeatureList *feature_list) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (unlikely (!c->check_struct (this))) return_trace (false);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    const Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  rak_sita:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa FeatureTableSubstitution
{
  void collect_feature_indices (hb_set_t *feature_indices) const
  {
    དོན་ལུ (const FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  const Feature *find_substitute (unsigned ཧྲིལ feature_index) const
  {
    unsigned ཧྲིལ count = substitutions.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      const FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      གལ་སྲིད (record.featureIndex == feature_index)
	ལོག &(this+record.feature);
    }
    ལོག nullptr;
  }

  void collect_lookups (const hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) const
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([this, lookup_indexes] (const FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (this, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  void collect_name_ids (const hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) const
  {
    དོན་ལུ (const FeatureTableSubstitutionRecord &record : substitutions)
      གལ་སྲིད (feature_index_map->has (record.featureIndex))
        (this+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  void closure_features (const hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    དོན་ལུ (const FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (this, lookup_indexes, feature_indexes);
  }

  bool intersects_features (const hb_map_t *feature_index_map) const
  {
    དོན་ལུ (const FeatureTableSubstitutionRecord& record : substitutions)
    {
      གལ་སྲིད (feature_index_map->has (record.featureIndex)) ལོག true;
    }
    ལོག false;
  }

  void collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) const
  {
    དོན་ལུ (const FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, this);
  }

  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               bool insert_catch_all) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;

    གལ་སྲིད (insert_catch_all)
    {
      དོན་ལུ (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<const void*, const void*> *p;
        གལ་སྲིད (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (false);
        auto *o = out->substitutions.serialize_append (c->serializer);
        གལ་སྲིད (!o->serialize (l, feature_index,
                           reinterpret_cast<const Feature*> (p->first),
                           reinterpret_cast<const Tag*> (p->second)))
          return_trace (false);
      }
      return_trace (true);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), this))
    ;

    return_trace (bool (out->substitutions));
  }

  bool sanitize (hb_sanitize_context_t *c, const FeatureList *feature_list) const
  {
    TRACE_SANITIZE (this);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, this, feature_list));
  }

  rak_sita:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  public:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

sa_nracanaa FeatureVariationRecord
{
  friend sa_nracanaa FeatureVariations;

  void collect_lookups (const void     *base,
			const hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) const
  {
    ལོག (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  void collect_name_ids (const void *base,
                         const hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) const
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  void closure_features (const void     *base,
			 const hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  bool intersects_features (const void *base, const hb_map_t *feature_index_map) const
  {
    ལོག (base+substitutions).intersects_features (feature_index_map);
  }

  void collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    const void *base) const
  {
    (base+conditions).keep_with_variations (c);
    གལ་སྲིད (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = true; // set variations only once
    }
  }

  bool subset (hb_subset_layout_context_t *c, const void *base,
               bool insert_catch_all = false) const
  {
    TRACE_SUBSET (this);
    auto *out = c->subset_context->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    གལ་སྲིད (!insert_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    དེ་མིན
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, insert_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, insert_catch_all);

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base,
		 const FeatureList *feature_list) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  const void *base) const
  { (base+conditions).collect_variation_indices (c); }

  rak_sita:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa LookupIndexList
{
  void add_lookup_indexes_to (hb_set_t *lookup_indexes) const
  { lookupIndices.add_indexes_to (lookup_indexes); }

  bool intersects_lookup_indexes (const hb_map_t *lookup_indexes) const
  { ལོག lookupIndices.intersects (lookup_indexes); }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (bool (out->lookupIndices));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (lookupIndices.sanitize (c));
  }

  rak_sita:
  IndexArray	lookupIndices;
  public:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

sa_nracanaa LookupConditionRecord
{
  unsigned get_condition_offset (const void *base, const void *variations) const
  { ལོག (const var_na *) &(base+condition) - (const var_na *) variations; }

  template <typename Instancer>
  bool evaluate (const void *base,
		 const ཧྲིལ *coords,
		 unsigned ཧྲིལ coord_len,
		 Instancer *instancer) const
  { ལོག (base+condition).evaluate (coords, coord_len, instancer); }

  bool evaluate (const void *base,
                 const void *variations,
                 hb_sorted_array_t<const unsigned> conditions) const
  {
    ལོག !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     parimaa_na (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  bool collect_condition (const void *base,
                          const void *variations,
                          const ཧྲིལ *coords,
                          unsigned ཧྲིལ coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) const
  {
    /* Null conditions always apply and need no key state. */
    གལ་སྲིད (!condition) ལོག true;

    unsigned offset = get_condition_offset (base, variations);
    གལ་སྲིད (visited->has (offset)) ལོག true;
    visited->add (offset);
    གལ་སྲིད (unlikely (visited->in_error ())) ལོག false;

    གལ་སྲིད (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    ལོག !conditions->in_error ();
  }

  void add_lookup_indexes_to (const void *base,
			      hb_set_t *lookup_indexes) const
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  bool intersects_lookup_indexes (const void *base,
				   const hb_map_t *lookup_indexes) const
  { ལོག (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  const void *base) const
  {
    གལ་སྲིད (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_values)))
      c->layout_variation_indices->err ();
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       const void *base) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (false);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    true);
    གལ་སྲིད (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->pop_discard ();
      return_trace (false);
    }
    c->serializer->add_link (out->condition, c->serializer->pop_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  rak_sita:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  public:
  DEFINE_SIZE_STATIC (8);
};

sa_nracanaa FeatureLookupsTable
{
  bool adds_default_lookups () const { ལོག flags & 0x0001u; }

  bool collect_conditions (const void *variations,
                           const ཧྲིལ *coords,
                           unsigned ཧྲིལ coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) const
  {
    དོན་ལུ (const LookupConditionRecord &record : records)
      གལ་སྲིད (unlikely (!record.collect_condition (this, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	ལོག false;
    ལོག true;
  }

  void collect_lookups (const void *variations,
                       hb_sorted_array_t<const unsigned> conditions,
                       hb_set_t *lookup_indexes) const
  {
    དོན་ལུ (const LookupConditionRecord &record : records)
      གལ་སྲིད (record.evaluate (this, variations, conditions))
	record.add_lookup_indexes_to (this, lookup_indexes);
  }

  template <typename Instancer>
  void collect_lookups (const ཧྲིལ *coords,
			unsigned ཧྲིལ coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) const
  {
    དོན་ལུ (const LookupConditionRecord &record : records)
      གལ་སྲིད (record.evaluate (this, coords, coord_len, instancer))
	record.add_lookup_indexes_to (this, lookup_indexes);
  }

  void collect_lookups (hb_set_t *lookup_indexes) const
  {
    དོན་ལུ (const LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (this, lookup_indexes);
  }

  bool intersects_lookup_indexes (const hb_map_t *lookup_indexes) const
  {
    དོན་ལུ (const LookupConditionRecord &record : records)
      གལ་སྲིད (record.intersects_lookup_indexes (this, lookup_indexes))
	ལོག true;
    ལོག false;
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    དོན་ལུ (const LookupConditionRecord &record : records)
      record.collect_variation_indices (c, this);
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    དོན་ལུ (const LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      གལ་སྲིད (record.subset (c, l, this))
	out->records.len++;
      དེ་མིན
	c->serializer->revert (snap);
    }

    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, this));
  }

  rak_sita:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  public:
  DEFINE_SIZE_ARRAY (10, records);
};

sa_nracanaa LookupVariationRecord
{
  friend sa_nracanaa FeatureVariations;

  unsigned get_feature_index () const { ལོག featureIndex; }

  void collect_lookups (const void *base,
                       const Feature &feature,
                       hb_sorted_array_t<const unsigned> conditions,
                       hb_set_t *lookup_indexes) const
  {
    const FeatureLookupsTable &feature_lookups = base+featureLookups;
    གལ་སྲིད (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  void collect_lookups (const void *base,
			const hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) const
  {
    གལ་སྲིད (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  void closure_features (const void *base,
			 const hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) const
  {
    གལ་སྲིད ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  const void *base) const
  { (base+featureLookups).collect_variation_indices (c); }

  bool subset (hb_subset_layout_context_t *l, const void *base) const
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (this);
    uint32_t *new_feature_index;
    གལ་སྲིད (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (false);

    auto *out = c->serializer->embed (this);
    གལ་སྲིད (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  featureLookups.sanitize (c, base));
  }

  rak_sita:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa FeatureVariations
{
  static constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  const Array32Of<LookupVariationRecord> &get_lookup_variation_records () const
  {
    གལ་སྲིད (version.to_int () < 0x00010001u)
      ལོག Null (Array32Of<LookupVariationRecord>);
    ལོག StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_count () const
  {
    ལོག varRecords.len;
  }

  bool find_index (const ཧྲིལ *coords, unsigned ཧྲིལ coord_len,
		   unsigned ཧྲིལ *index,
		   ItemVarStoreInstancer *instancer) const
  {
    unsigned ཧྲིལ count = varRecords.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      const FeatureVariationRecord &record = varRecords.arrayZ[i];
      གལ་སྲིད ((this+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	ལོག true;
      }
    }
    *index = NOT_FOUND_INDEX;
    ལོག false;
  }

  const Feature *find_substitute (unsigned ཧྲིལ variations_index,
				  unsigned ཧྲིལ feature_index) const
  {
    const FeatureVariationRecord &record = varRecords[variations_index];
    ལོག (this+record.substitutions).find_substitute (feature_index);
  }

  bool collect_lookup_conditions (const ཧྲིལ *coords,
                                  unsigned ཧྲིལ coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) const
  {
    hb_set_t visited_tables, visited_conditions;
    དོན་ལུ (const LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      གལ་སྲིད (visited_tables.has (offset)) continue;
      visited_tables.add (offset);
      གལ་སྲིད (unlikely (visited_tables.in_error ())) ལོག false;

      const FeatureLookupsTable &feature_lookups = this+record.featureLookups;
      གལ་སྲིད (unlikely (!feature_lookups.collect_conditions (this, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	ལོག false;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    ལོག true;
  }

  bool get_lookup_variations (unsigned feature_index,
                              const Feature &feature,
                              hb_sorted_array_t<const unsigned> conditions,
                              hb_set_t *lookup_indexes) const
  {
    དོན་ལུ (const LookupVariationRecord &record : get_lookup_variation_records ())
    {
      གལ་སྲིད (record.featureIndex != feature_index) continue;

      record.collect_lookups (this, feature, conditions, lookup_indexes);
      ལོག true;
    }
    ལོག false;
  }

  void collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) const
  {
    unsigned ཧྲིལ count = varRecords.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, this);
      གལ་སྲིད (c->universal)
        bha_nga;
    }
    གལ་སྲིད (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (*this));
  }

  void collect_lookups (const hb_set_t *feature_indexes,
			const hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) const
  {
    unsigned count = varRecords.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      གལ་སྲིད (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        continue;
      varRecords[i].collect_lookups (this, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  void collect_name_ids (const hb_map_t *feature_index_map,
                         const hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) const
  {
    དོན་ལུ (unsigned i = 0; i < varRecords.len; i++)
    {
      གལ་སྲིད (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        continue;
      varRecords[i].collect_name_ids (this, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  void collect_lookup_variation_lookups (const hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) const
  {
    དོན་ལུ (const LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (this, feature_indexes, lookup_indexes);
  }

  void closure_features (const hb_map_t *lookup_indexes,
			 const hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    unsigned ཧྲིལ count = varRecords.len;
    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++)
    {
      གལ་སྲིད (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        continue;
      varRecords[i].closure_features (this, lookup_indexes, feature_indexes);
    }

    དོན་ལུ (const LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (this, lookup_indexes, feature_indexes);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    དོན་ལུ (const FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, this);
    དོན་ལུ (const LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, this);
  }

  void collect_feature_indices (hb_set_t *feature_indices) const
  {
    དོན་ལུ (const FeatureVariationRecord &record : varRecords)
      (this+record.substitutions).collect_feature_indices (feature_indices);
  }

  bool has_lookup_variations (unsigned feature_index) const
  {
    དོན་ལུ (const LookupVariationRecord &record : get_lookup_variation_records ())
    {
      གལ་སྲིད (record.featureIndex == feature_index)
	ལོག true;
      གལ་སྲིད (record.featureIndex > feature_index)
	bha_nga;
    }
    ལོག false;
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    གལ་སྲིད (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;

    ཧྲིལ keep_up_to = -1;
    དོན་ལུ (ཧྲིལ i = varRecords.len - 1; i >= 0; i--) {
      གལ་སྲིད (varRecords[i].intersects_features (this, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        bha_nga;
      }
    }

    unsigned count = (unsigned) (keep_up_to + 1);
    དོན་ལུ (unsigned i = 0; i < count; i++)
    {
      གལ་སྲིད (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        continue;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), this) (varRecords[i]);
    }

    གལ་སྲིད (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      bool insert_catch_all_record = true;
      subset_record_array (l, &(out->varRecords), this, insert_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    གལ་སྲིད (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      གལ་སྲིད (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (false);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, this))
      ;
    }

    return_trace (bool (out->varRecords) ||
		  (out_lookup_records && bool (*out_lookup_records)));
  }

  bool sanitize (hb_sanitize_context_t *c, const FeatureList *feature_list) const
  {
    TRACE_SANITIZE (this);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, this, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, this)));
  }

  rak_sita:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  public:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

sa_nracanaa HintingDevice
{
  friend sa_nracanaa Device;

  private:

  hb_position_t get_x_delta (hb_font_t *font) const
  { ལོག get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) const
  { ལོག get_delta (font->y_ppem, font->y_scale); }

  public:

  size_t get_size () const
  {
    unsigned ཧྲིལ f = deltaFormat;
    གལ་སྲིད (unlikely (f < 1 || f > 3 || startSize > endSize)) ལོག 3 * HBUINT16::static_size;
    ལོག HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) && c->check_range (this, this->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed<HintingDevice> (this));
  }

  private:

  ཧྲིལ get_delta (unsigned ཧྲིལ ppem, ཧྲིལ scale) const
  {
    གལ་སྲིད (!ppem) ལོག 0;

    ཧྲིལ pixels = get_delta_pixels (ppem);

    གལ་སྲིད (!pixels) ལོག 0;

    ལོག (ཧྲིལ) (pixels * (int64_t) scale / ppem);
  }
  ཧྲིལ get_delta_pixels (unsigned ཧྲིལ ppem_size) const
  {
    unsigned ཧྲིལ f = deltaFormat;
    གལ་སྲིད (unlikely (f < 1 || f > 3))
      ལོག 0;

    གལ་སྲིད (ppem_size < startSize || ppem_size > endSize)
      ལོག 0;

    unsigned ཧྲིལ s = ppem_size - startSize;

    unsigned ཧྲིལ byte = deltaValueZ[s >> (4 - f)];
    unsigned ཧྲིལ bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned ཧྲིལ mask = (0xFFFFu >> (16 - (1 << f)));

    ཧྲིལ delta = bits & mask;

    གལ་སྲིད ((unsigned ཧྲིལ) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    ལོག delta;
  }

  rak_sita:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		deltaValueZ;		/* Array of compressed data */
  public:
  DEFINE_SIZE_ARRAY (6, deltaValueZ);
};

sa_nracanaa VariationDevice
{
  friend sa_nracanaa Device;

  private:

  hb_position_t get_x_delta (hb_font_t *font,
			     const ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) const
  { ལོག !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     const ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) const
  { ལོག !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  float get_x_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) const
  { ལོག !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  float get_y_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) const
  { ལོག !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *layout_variation_idx_delta_map) const
  {
    TRACE_SERIALIZE (this);
    གལ་སྲིད (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, ཧྲིལ> *v;
    གལ་སྲིད (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    གལ་སྲིད (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (this->static_size);
    auto *out = c->embed (this);
    གལ་སྲིད (unlikely (!out)) return_trace (nullptr);

    གལ་སྲིད (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  void collect_variation_index (hb_collect_variation_indices_context_t *c) const
  { c->layout_variation_indices->add (varIdx); }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  private:

  float get_delta (hb_font_t *font,
		   const ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) const
  {
    ལོག store.get_delta (varIdx, font->coords, font->num_coords, store_cache);
  }

  rak_sita:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa DeviceHeader
{
  rak_sita:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  public:
  HBUINT16		format;		/* Format identifier */
  public:
  DEFINE_SIZE_STATIC (6);
};

sa_nracanaa Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  float get_x_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) const
  {
#ifndef HB_NO_VAR
    གལ་སྲིད (u.b.format == 0x8000)
    {
      hb_barrier ();
      ལོག u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    ལོག get_x_delta (font, store, store_cache);
  }

  float get_y_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) const
  {
#ifndef HB_NO_VAR
    གལ་སྲིད (u.b.format == 0x8000)
    {
      hb_barrier ();
      ལོག u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    ལོག get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     const ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) const
  {
    switch (u.b.format)
    {
#ifndef HB_NO_HINTING
    pak_sa 1: pak_sa 2: pak_sa 3:
      hb_barrier ();
      ལོག u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      hb_barrier ();
      ལོག u.variation.get_x_delta (font, store, store_cache);
#endif
    default:
      ལོག 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     const ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) const
  {
    switch (u.b.format)
    {
    pak_sa 1: pak_sa 2: pak_sa 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      ལོག u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      hb_barrier ();
      ལོག u.variation.get_y_delta (font, store, store_cache);
#endif
    default:
      ལོག 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    གལ་སྲིད (!u.b.format.sanitize (c)) return_trace (false);
    switch (u.b.format) {
#ifndef HB_NO_HINTING
    pak_sa 1: pak_sa 2: pak_sa 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    default:
      return_trace (true);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *layout_variation_idx_delta_map=nullptr) const
  {
    TRACE_SERIALIZE (this);
    switch (u.b.format) {
#ifndef HB_NO_HINTING
    pak_sa 1:
    pak_sa 2:
    pak_sa 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    default:
      return_trace (nullptr);
    }
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    switch (u.b.format) {
#ifndef HB_NO_HINTING
    pak_sa 1:
    pak_sa 2:
    pak_sa 3:
      ལོག;
#endif
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      ལོག;
#endif
    default:
      ལོག;
    }
  }

  unsigned get_variation_index () const
  {
    switch (u.b.format) {
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      hb_barrier ();
      ལོག u.variation.varIdx;
#endif
    default:
      ལོག HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  bool is_variation_device () const
  {
    switch (u.b.format) {
#ifndef HB_NO_VAR
    pak_sa 0x8000:
      ལོག true;
#endif
    default:
      ལོག false;
    }
  }

  rak_sita:
  sa_ngha {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  public:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

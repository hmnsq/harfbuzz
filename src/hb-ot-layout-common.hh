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
static_assert (sizeof (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_binary_cache_t = hb_cache_t<14, 1, 8>;
static_assert (sizeof (hb_ot_layout_binary_cache_t) == 256, "");

#include "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


namespace OT {

template<typename Iterator>
static inline bool ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

static bool ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    const hb_set_t &klasses,
    bool use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

struct ItemVarStoreInstancer;

struct hb_collect_feature_substitutes_with_var_context_t
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

struct hb_prune_langsys_context_t
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
  { 戻る script_count++ < HB_MAX_SCRIPTS; }

  bool visitLangsys (unsigned feature_count)
  {
    langsys_feature_count += feature_count;
    戻る langsys_feature_count < HB_MAX_LANGSYS_FEATURE_COUNT;
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

struct hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  const char *get_name () { 戻る "SUBSET_LAYOUT"; }
  static return_t default_return_value () { 戻る hb_empty_t (); }

  bool visitScript ()
  {
    戻る script_count++ < HB_MAX_SCRIPTS;
  }

  bool visitLangSys ()
  {
    戻る langsys_count++ < HB_MAX_LANGSYS;
  }

  bool visitFeatureIndex (整数 count)
  {
    feature_index_count += count;
    戻る feature_index_count < HB_MAX_FEATURE_INDICES;
  }

  bool visitLookupIndex()
  {
    lookup_index_count++;
    戻る lookup_index_count < HB_MAX_LOOKUP_VISIT_COUNT;
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
    もし (tag_ == HB_OT_TAG_GSUB)
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
    否则
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

struct ItemVariationStore;
struct hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (const T &obj) { obj.collect_variation_indices (this); 戻る hb_empty_t (); }
  static return_t default_return_value () { 戻る hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  const hb_set_t *glyph_set;
  const hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, 整数>> *condition_values = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  const hb_set_t *glyph_set_,
					  const hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
struct subset_offset_array_t
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
    もし (unlikely (!o)) 戻る false;
    bool ret = o->serialize_subset (subset_context, offset, base);
    もし (!ret)
    {
      out.pop ();
      subset_context->serializer->revert (snap);
    }
    戻る ret;
  }

  private:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  const void *base;
};


template<typename OutputArray, typename Arg>
struct subset_offset_array_arg_t
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
    もし (unlikely (!o)) 戻る false;
    bool ret = o->serialize_subset (subset_context, offset, base, arg);
    もし (!ret)
    {
      out.pop ();
      subset_context->serializer->revert (snap);
    }
    戻る ret;
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
struct
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       const void *base) const
  { 戻る subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       const void *base, Arg &&arg) const
  { 戻る subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
struct subset_record_array_t
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
    もし (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    否则 out->len++;
  }

  private:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  const void *base;
};

template<typename OutputArray, typename Arg>
struct subset_record_array_arg_t
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
    もし (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    否则 out->len++;
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
struct
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       const void *base) const
  { 戻る subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               const void *base, Arg &&arg) const
  { 戻る subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
struct serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         const void *base_,
                         const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 整数>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  bool operator () (T&& record)
  {
    もし (!serialize_context->copy (record, base, variation_map)) 戻る false;
    out.len++;
    戻る true;
  }

  private:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  const void *base;
  const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 整数>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
struct
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               const void *base,
               const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 整数>> *variation_map) const
  { 戻る serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

struct IndexArray : Array16Of<Index>
{
  bool intersects (const hb_map_t *indexes) const
  { 戻る hb_any (*this, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  void serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    もし (!it) 戻る;
    もし (unlikely (!c->extend_min ((*this)))) 戻る;

    ために (const auto _ : it)
    {
      もし (!l->visitLookupIndex()) break;

      Index i;
      i = _;
      c->copy (i);
      this->len++;
    }
  }

  unsigned 整数 get_indexes (unsigned 整数 start_offset,
			    unsigned 整数 *_count /* IN/OUT */,
			    unsigned 整数 *_indexes /* OUT */) const
  {
    もし (_count && _indexes)
    {
      + this->as_array ().sub_array (start_offset, _count)
      | hb_sink (hb_array (_indexes, *_count))
      ;
    }
    戻る this->len;
  }

  void add_indexes_to (hb_set_t* output /* OUT */) const
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
struct FeatureParamsSize
{
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    もし (unlikely (!c->check_struct (this))) return_trace (false);
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

    もし (!designSize)
      return_trace (false);
    否则 もし (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (true);
    否则 もし (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (false);
    否则
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
struct FeatureParamsStylisticSet
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
struct FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_count, hb_codepoint_t *chars) const
  {
    もし (char_count && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_count)
      | hb_sink (hb_array (chars, *char_count))
      ;
    }
    戻る characters.len;
  }

  size_t get_size () const
  { 戻る hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  void collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) const
  {
    もし (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    もし (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    もし (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    もし (!firstParamUILabelNameID || !numNamedParameters || numNamedParameters >= 0x7FFF)
      戻る;

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

struct FeatureParams
{
  bool sanitize (hb_sanitize_context_t *c, hb_tag_t tag) const
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    戻る true;
#endif
    TRACE_SANITIZE (this);
    もし (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    もし ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    もし ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (true);
  }

  void collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) const
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    戻る;
#endif
    もし (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      戻る (u.size.collect_name_ids (nameids_to_retain));
    もし ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      戻る (u.stylisticSet.collect_name_ids (nameids_to_retain));
    もし ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      戻る (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  bool subset (hb_subset_context_t *c, const Tag* tag) const
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    戻る false;
#endif
    TRACE_SUBSET (this);
    もし (!tag) return_trace (false);
    もし (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    もし ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    もし ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (false);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  const FeatureParamsSize& get_size_params (hb_tag_t tag) const
  {
    もし (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      戻る u.size;
    戻る Null (FeatureParamsSize);
  }
  const FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) const
  {
    もし ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      戻る u.stylisticSet;
    戻る Null (FeatureParamsStylisticSet);
  }
  const FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) const
  {
    もし ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      戻る u.characterVariants;
    戻る Null (FeatureParamsCharacterVariants);
  }
#endif

  private:
  union {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  public:
  DEFINE_SIZE_MIN (0);
};

struct Record_sanitize_closure_t {
  hb_tag_t tag;
  const void *list_base;
};

struct Feature
{
  unsigned 整数 get_lookup_count () const
  { 戻る lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned 整数 i) const
  { 戻る lookupIndex[i]; }
  unsigned 整数 get_lookup_indexes (unsigned 整数 start_index,
				   unsigned 整数 *lookup_count /* IN/OUT */,
				   unsigned 整数 *lookup_tags /* OUT */) const
  { 戻る lookupIndex.get_indexes (start_index, lookup_count, lookup_tags); }
  void add_lookup_indexes_to (hb_set_t *lookup_indexes) const
  { lookupIndex.add_indexes_to (lookup_indexes); }

  const FeatureParams &get_feature_params () const
  { 戻る this+featureParams; }

  bool intersects_lookup_indexes (const hb_map_t *lookup_indexes) const
  { 戻る lookupIndex.intersects (lookup_indexes); }

  void collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) const
  {
    もし (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  bool subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       const Tag                   *tag = nullptr) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    out->featureParams.serialize_subset (c, featureParams, this, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    もし (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    否则
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
struct Record
{
  整数 cmp (hb_tag_t a) const { 戻る tag.cmp (a); }

  bool subset (hb_subset_layout_context_t *c, const void *base, const void *f_sub = nullptr) const
  {
    TRACE_SUBSET (this);
    auto *out = c->subset_context->serializer->embed (this);
    もし (unlikely (!out)) return_trace (false);

    もし (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    const Feature& f = *reinterpret_cast<const Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    bool ret = f.subset (c->subset_context, c, &tag);
    もし (ret)
      s->add_link (out->offset, s->pop_pack ());
    否则
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
struct RecordArrayOf : SortedArray16Of<Record<Type>>
{
  const Offset16To<Type>& get_offset (unsigned 整数 i) const
  { 戻る (*this)[i].offset; }
  Offset16To<Type>& get_offset (unsigned 整数 i)
  { 戻る (*this)[i].offset; }
  const Tag& get_tag (unsigned 整数 i) const
  { 戻る (*this)[i].tag; }
  unsigned 整数 get_tags (unsigned 整数 start_offset,
                         unsigned 整数 *record_count /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) const
  {
    もし (record_count && record_tags)
    {
      + this->as_array ().sub_array (start_offset, record_count)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_count))
      ;
    }
    戻る this->len;
  }
  bool find_index (hb_tag_t tag, unsigned 整数 *index) const
  {
    戻る this->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
struct RecordListOf : RecordArrayOf<Type>
{
  const Type& operator [] (unsigned 整数 i) const
  { 戻る this+this->get_offset (i); }

  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

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

struct RecordListOfFeature : RecordListOf<Feature>
{
  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    + hb_enumerate (*this)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, this] (const hb_pair_t<unsigned, const Record<Feature>&>& _)
                {
                  const Feature *f_sub = nullptr;
                  const Feature **f = nullptr;
                  もし (l->feature_substitutes_map->has (_.first, &f))
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


struct LangSys
{
  unsigned 整数 get_feature_count () const
  { 戻る featureIndex.len; }
  hb_tag_t get_feature_index (unsigned 整数 i) const
  { 戻る featureIndex[i]; }
  unsigned 整数 get_feature_indexes (unsigned 整数 start_offset,
				    unsigned 整数 *feature_count /* IN/OUT */,
				    unsigned 整数 *feature_indexes /* OUT */) const
  { 戻る featureIndex.get_indexes (start_offset, feature_count, feature_indexes); }
  void add_feature_indexes_to (hb_set_t *feature_indexes) const
  { featureIndex.add_indexes_to (feature_indexes); }

  bool has_required_feature () const { 戻る reqFeatureIndex != 0xFFFFu; }
  unsigned 整数 get_required_feature_index () const
  {
    もし (reqFeatureIndex == 0xFFFFu)
      戻る Index::NOT_FOUND_INDEX;
   戻る reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (*this));
  }

  bool compare (const LangSys& o, const hb_map_t *feature_index_map) const
  {
    もし (reqFeatureIndex != o.reqFeatureIndex)
      戻る false;

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

    ために (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      もし (a != b) 戻る false;
    }

    もし (iter || o_iter) 戻る false;

    戻る true;
  }

  void collect_features (hb_prune_langsys_context_t *c) const
  {
    もし (!has_required_feature () && !get_feature_count ()) 戻る;
    もし (has_required_feature () &&
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
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    const uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    もし (!l->visitFeatureIndex (featureIndex.len))
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

struct Script
{
  unsigned 整数 get_lang_sys_count () const
  { 戻る langSys.len; }
  const Tag& get_lang_sys_tag (unsigned 整数 i) const
  { 戻る langSys.get_tag (i); }
  unsigned 整数 get_lang_sys_tags (unsigned 整数 start_offset,
				  unsigned 整数 *lang_sys_count /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) const
  { 戻る langSys.get_tags (start_offset, lang_sys_count, lang_sys_tags); }
  const LangSys& get_lang_sys (unsigned 整数 i) const
  {
    もし (i == Index::NOT_FOUND_INDEX) 戻る get_default_lang_sys ();
    戻る this+langSys[i].offset;
  }
  bool find_lang_sys_index (hb_tag_t tag, unsigned 整数 *index) const
  { 戻る langSys.find_index (tag, index); }

  bool has_default_lang_sys () const           { 戻る defaultLangSys != 0; }
  const LangSys& get_default_lang_sys () const { 戻る this+defaultLangSys; }

  void prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) const
  {
    もし (!has_default_lang_sys () && !get_lang_sys_count ()) 戻る;
    もし (!c->visitScript ()) 戻る;

    もし (!c->script_langsys_map->has (script_index))
    {
      もし (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	戻る;
    }

    もし (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      const LangSys& d = get_default_lang_sys ();
      もし (c->visitLangsys (d.get_feature_count ())) {
        d.collect_features (c);
      }

      ために (auto _ : + hb_enumerate (langSys))
      {
        const LangSys& l = this+_.second.offset;
        もし (!c->visitLangsys (l.get_feature_count ())) continue;
        もし (l.compare (d, c->duplicate_feature_map)) continue;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    否则
    {
      ために (auto _ : + hb_enumerate (langSys))
      {
        const LangSys& l = this+_.second.offset;
        もし (!c->visitLangsys (l.get_feature_count ())) continue;
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
    もし (!l->visitScript ()) return_trace (false);
    もし (tag && !c->plan->layout_scripts.has (*tag))
      戻る false;

    auto *out = c->serializer->start_embed (*this);
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    bool defaultLang = false;
    もし (has_default_lang_sys ())
    {
      c->serializer->push ();
      const LangSys& ls = this+defaultLangSys;
      bool ret = ls.subset (c, l);
      もし (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->pop_discard ();
	out->defaultLangSys = 0;
      }
      否则
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->pop_pack ());
	defaultLang = true;
      }
    }

    const hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    もし (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (const Record<LangSys>& record) {戻る l->visitLangSys (); })
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

  protected:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  public:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

struct RecordListOfScript : RecordListOf<Script>
{
  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    ために (auto _ : + hb_enumerate (*this))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      bool ret = _.second.subset (l, this);
      もし (!ret) c->serializer->revert (snap);
      否则 out->len++;
    }

    return_trace (true);
  }
};

typedef RecordListOfScript ScriptList;



struct LookupFlag : HBUINT16
{
  enum Flags {
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

struct Lookup
{
  unsigned 整数 get_subtable_count () const { 戻る subTable.len; }

  template <typename TSubTable>
  const Array16OfOffset16To<TSubTable>& get_subtables () const
  { 戻る reinterpret_cast<const Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { 戻る reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  const TSubTable& get_subtable (unsigned 整数 i) const
  { 戻る this+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned 整数 i)
  { 戻る this+get_subtables<TSubTable> ()[i]; }

  size_t get_size () const
  {
    const HBUINT16 &markFilteringSet = StructAfter<const HBUINT16> (subTable);
    もし (lookupFlag & LookupFlag::UseMarkFilteringSet)
      戻る (const char *) &StructAfter<const char> (markFilteringSet) - (const char *) this;
    戻る (const char *) &markFilteringSet - (const char *) this;
  }

  unsigned 整数 get_type () const { 戻る lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () const
  {
    unsigned 整数 flag = lookupFlag;
    もし (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      const HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    戻る flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    unsigned 整数 lookup_type = get_type ();
    TRACE_DISPATCH (this, lookup_type);
    unsigned 整数 count = get_subtable_count ();
    ために (unsigned 整数 i = 0; i < count; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      もし (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_value ());
  }

  bool serialize (hb_serialize_context_t *c,
		  unsigned 整数 lookup_type,
		  uint32_t lookup_props,
		  unsigned 整数 num_subtables)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this))) return_trace (false);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    もし (unlikely (!subTable.serialize (c, num_subtables))) return_trace (false);
    もし (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      もし (unlikely (!c->extend (this))) return_trace (false);
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
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    const hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned 整数 lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([this, glyphset, lookup_type] (const Offset16To<TSubTable> &_) { 戻る (this+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), this, lookup_type))
    ;

    もし (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      const HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      もし (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      否则
      {
        もし (unlikely (!c->serializer->extend (out))) return_trace (false);
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
    もし (!(c->check_struct (this) && subTable.sanitize (c))) return_trace (false);
    hb_barrier ();

    unsigned subtables = get_subtable_count ();
    もし (unlikely (!c->visit_subtables (subtables))) return_trace (false);

    もし (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      const HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      もし (!markFilteringSet.sanitize (c)) return_trace (false);
    }

    もし (unlikely (!get_subtables<TSubTable> ().sanitize (c, this, get_type ())))
      return_trace (false);

    もし (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned 整数 type = get_subtable<TSubTable> (0).u.extension.get_type ();
      ために (unsigned 整数 i = 1; i < subtables; i++)
	もし (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (false);
    }
    return_trace (true);
  }

  protected:
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
struct LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  bool subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!c->serializer->extend_min (out))) return_trace (false);

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
  もし (!klass_map)
    戻る ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  もし (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  ために (const unsigned k: klasses)
  {
    もし (klass_map->has (k)) continue;
    klass_map->set (k, idx);
    idx++;
  }


  もし (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    戻る false;

  ために (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  戻る ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
struct ClassDefFormat1_3
{
  friend struct ClassDef;

  private:
  unsigned 整数 get_class (hb_codepoint_t glyph_id) const
  {
    戻る classValue[(unsigned 整数) (glyph_id - startGlyph)];
  }

  unsigned get_population () const
  {
    戻る classValue.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this))) return_trace (false);

    もし (unlikely (!it))
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
    もし (unlikely (!classValue.serialize (c, glyph_count))) return_trace (false);
    ために (const hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
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

    ために (const hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      もし (new_gid == HB_MAP_VALUE_INVALID) continue;
      もし (glyph_filter && !glyph_filter->has(gid)) continue;

      unsigned klass = classValue[gid - start];
      もし (!klass) continue;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    もし (use_class_zero)
    {
      const hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_count = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_count <= glyph_and_klass.length;
    }
    もし (!ClassDef_remap_and_serialize (c->serializer,
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

  unsigned cost () const { 戻る 1; }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    unsigned 整数 start = 0;
    unsigned 整数 count = classValue.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      もし (classValue[i])
	continue;

      もし (start != i)
	もし (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  戻る false;

      start = i + 1;
    }
    もし (start != count)
      もし (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + count)))
	戻る false;

    戻る true;
  }

  template <typename set_t>
  bool collect_class (set_t *glyphs, unsigned klass) const
  {
    unsigned 整数 count = classValue.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (classValue[i] == klass) glyphs->add (startGlyph + i);
    戻る true;
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + classValue.len;
    ために (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      もし (classValue[iter - start]) 戻る true;
    戻る false;
  }
  bool intersects_class (const hb_set_t *glyphs, unsigned klass) const
  {
    unsigned 整数 count = classValue.len;
    もし (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_VALUE_INVALID;
      もし (!glyphs->next (&g)) 戻る false;
      もし (g < startGlyph) 戻る true;
      g = startGlyph + count - 1;
      もし (glyphs->next (&g)) 戻る true;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    const typename Types::HBUINT *arr = classValue.arrayZ;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (arr[i] == klass && glyphs->has (startGlyph + i))
	戻る true;
    戻る false;
  }

  void intersected_class_glyphs (const hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) const
  {
    unsigned count = classValue.len;
    もし (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      ために (uint32_t g = HB_SET_VALUE_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      ために (uint32_t g = startGlyph + count - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      戻る;
    }

    ために (unsigned i = 0; i < count; i++)
      もし (classValue[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#もし 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + count;
    ために (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      もし (classValue.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  void intersected_classes (const hb_set_t *glyphs, hb_set_t *intersect_classes) const
  {
    もし (glyphs->is_empty ()) 戻る;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    もし (!classValue.len)
    {
      intersect_classes->add (0);
      戻る;
    }

    hb_codepoint_t end_glyph = startGlyph + classValue.len - 1;
    もし (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    ために (const auto& _ : + hb_enumerate (classValue))
    {
      hb_codepoint_t g = startGlyph + _.first;
      もし (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  protected:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		classValue;	/* Array of Class Values--one per GlyphID */
  public:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, classValue);
};

template <typename Types>
struct ClassDefFormat2_4
{
  friend struct ClassDef;

  private:
  unsigned 整数 get_class (hb_codepoint_t glyph_id) const
  {
    戻る rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () const
  {
    typename Types::large_int ret = 0;
    ために (const auto &r : rangeRecord)
      ret += r.get_population ();
    戻る ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this))) return_trace (false);

    もし (unlikely (!it))
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
    もし (unlikely (!record)) return_trace (false);

    ために (const auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      もし (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	もし (unlikely (cur_gid < prev_gid))
	  unsorted = true;

	もし (unlikely (!record)) break;
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

    もし (unlikely (c->in_error ())) return_trace (false);

    もし (likely (record)) record->last = prev_gid;
    rangeRecord.len = num_ranges;

    もし (unlikely (unsorted))
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

    もし (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      ために (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	もし (!klass) continue;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	もし (new_gid == HB_MAP_VALUE_INVALID) continue;
	もし (glyph_filter && !glyph_filter->has (g)) continue;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    否则
    {
      unsigned num_source_glyphs = c->plan->source->get_num_glyphs ();
      ために (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	もし (!klass) continue;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, num_source_glyphs);
	ために (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  もし (new_gid == HB_MAP_VALUE_INVALID) continue;
	  もし (glyph_filter && !glyph_filter->has (g)) continue;

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
    もし (!ClassDef_remap_and_serialize (c->serializer,
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

  unsigned cost () const { 戻る hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    ために (auto &range : rangeRecord)
      もし (range.value)
	もし (unlikely (!range.collect_coverage (glyphs)))
	  戻る false;
    戻る true;
  }

  template <typename set_t>
  bool collect_class (set_t *glyphs, unsigned 整数 klass) const
  {
    ために (auto &range : rangeRecord)
    {
      もし (range.value == klass)
	もし (unlikely (!range.collect_coverage (glyphs)))
	  戻る false;
    }
    戻る true;
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    もし (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      ために (auto g : *glyphs)
        もし (get_class (g))
	  戻る true;
      戻る false;
    }

    戻る hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (const RangeRecord<Types> &range) { 戻る range.intersects (*glyphs) && range.value; }));
  }
  bool intersects_class (const hb_set_t *glyphs, unsigned klass) const
  {
    もし (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      もし (!rangeRecord.len)
        戻る !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_VALUE_INVALID;
      hb_codepoint_t last = HB_SET_VALUE_INVALID;
      auto it = hb_iter (rangeRecord);
      ために (auto &range : it)
      {
        もし (it->first == last + 1)
	{
	  it++;
	  continue;
	}

	もし (!glyphs->next (&g))
	  break;
	もし (g < range.first)
	  戻る true;
	g = range.last;
	last = g;
      }
      もし (g != HB_SET_VALUE_INVALID && glyphs->next (&g))
	戻る true;
      /* Fall through. */
    }
    ために (const auto &range : rangeRecord)
      もし (range.value == klass && range.intersects (*glyphs))
	戻る true;
    戻る false;
  }

  void intersected_class_glyphs (const hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) const
  {
    もし (klass == 0)
    {
      hb_codepoint_t g = HB_SET_VALUE_INVALID;
      ために (auto &range : rangeRecord)
      {
	もし (!glyphs->next (&g))
	  goto done;
	ながら (g < range.first)
	{
	  intersect_glyphs->add (g);
	  もし (!glyphs->next (&g))
	    goto done;
        }
        g = range.last;
      }
      ながら (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      戻る;
    }

    unsigned count = rangeRecord.len;
    もし (count > glyphs->get_population () * hb_bit_storage (count))
    {
      ために (auto g : *glyphs)
      {
        unsigned i;
        もし (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      戻る;
    }

    ために (auto &range : rangeRecord)
    {
      もし (range.value != klass) continue;

      unsigned end = range.last + 1;
      ために (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  void intersected_classes (const hb_set_t *glyphs, hb_set_t *intersect_classes) const
  {
    もし (glyphs->is_empty ()) 戻る;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    もし (!rangeRecord.len)
    {
      intersect_classes->add (0);
      戻る;
    }

    hb_codepoint_t g = HB_SET_VALUE_INVALID;
    ために (auto &range : rangeRecord)
    {
      もし (!glyphs->next (&g))
        break;
      もし (g < range.first)
      {
        intersect_classes->add (0);
        break;
      }
      g = range.last;
    }
    もし (g != HB_SET_VALUE_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    ために (const auto& range : rangeRecord)
      もし (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  protected:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  public:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

struct ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) const { 戻る get (k); }
  bool has (hb_codepoint_t k) const { 戻る (*this)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) const { 戻る get (k); }

  unsigned 整数 get (hb_codepoint_t k) const { 戻る get_class (k); }
  unsigned 整数 get_class (hb_codepoint_t glyph_id) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.get_class (glyph_id);
    case 2: hb_barrier (); 戻る u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.get_class (glyph_id);
    case 4: hb_barrier (); 戻る u.format4.get_class (glyph_id);
#endif
    default:戻る 0;
    }
  }
  unsigned 整数 get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) const
  {
    unsigned klass;
    もし (cache && cache->get (glyph_id, &klass)) 戻る klass;
    klass = get_class (glyph_id);
    もし (cache) cache->set (glyph_id, klass);
    戻る klass;
  }

  unsigned get_population () const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.get_population ();
    case 2: hb_barrier (); 戻る u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.get_population ();
    case 4: hb_barrier (); 戻る u.format4.get_population ();
#endif
    default:戻る NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this))) return_trace (false);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    もし (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned num_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      ために (const auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	もし (cur_klass > class_max) class_max = cur_klass;
	もし (cur_gid == glyph_min) continue;
        もし (cur_gid > glyph_max) glyph_max = cur_gid;
	もし (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  num_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	もし (size >= best_size) 戻る;
	best_size = size;
	format = candidate;
      };

      もし (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      もし (glyph_max <= 0xFFFFu && num_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) num_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      もし (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      もし (glyph_max <= 0xFFFFFFu && num_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) num_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      もし (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (false, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (false);
      }
    }

    u.format.v = format;

    switch (u.format.v)
    {
    case 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    case 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    case 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
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
    case 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    case 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    case 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    default:return_trace (false);
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    もし (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    case 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    case 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    default:return_trace (true);
    }
  }

  unsigned cost () const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.cost ();
    case 2: hb_barrier (); 戻る u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.cost ();
    case 4: hb_barrier (); 戻る u.format4.cost ();
#endif
    default:戻る 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  bool collect_coverage (set_t *glyphs) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.collect_coverage (glyphs);
    case 2: hb_barrier (); 戻る u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.collect_coverage (glyphs);
    case 4: hb_barrier (); 戻る u.format4.collect_coverage (glyphs);
#endif
    default:戻る false;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  bool collect_class (set_t *glyphs, unsigned 整数 klass) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.collect_class (glyphs, klass);
    case 2: hb_barrier (); 戻る u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.collect_class (glyphs, klass);
    case 4: hb_barrier (); 戻る u.format4.collect_class (glyphs, klass);
#endif
    default:戻る false;
    }
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.intersects (glyphs);
    case 2: hb_barrier (); 戻る u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.intersects (glyphs);
    case 4: hb_barrier (); 戻る u.format4.intersects (glyphs);
#endif
    default:戻る false;
    }
  }
  bool intersects_class (const hb_set_t *glyphs, unsigned 整数 klass) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.intersects_class (glyphs, klass);
    case 2: hb_barrier (); 戻る u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.intersects_class (glyphs, klass);
    case 4: hb_barrier (); 戻る u.format4.intersects_class (glyphs, klass);
#endif
    default:戻る false;
    }
  }

  void intersected_class_glyphs (const hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    case 2: hb_barrier (); 戻る u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    case 4: hb_barrier (); 戻る u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    default:戻る;
    }
  }

  void intersected_classes (const hb_set_t *glyphs, hb_set_t *intersect_classes) const
  {
    switch (u.format.v) {
    case 1: hb_barrier (); 戻る u.format1.intersected_classes (glyphs, intersect_classes);
    case 2: hb_barrier (); 戻る u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    case 3: hb_barrier (); 戻る u.format3.intersected_classes (glyphs, intersect_classes);
    case 4: hb_barrier (); 戻る u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    default:戻る;
    }
  }


  protected:
  union {
  struct { HBUINT16 v; }	format;		/* Format identifier */
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
static inline bool ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ 戻る (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
struct delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  struct chars_t : hb_vector_t<uint8_t>
  {
    整数 cmp (const chars_t& other) const
    {
      戻る as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      ために (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      戻る hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (const chars_t& other) const
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      ために (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      戻る hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (const delta_row_encoding_t& other_encoding) const { 戻る chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<const hb_vector_t<整数>*> items;

  delta_row_encoding_t () = default;
  delta_row_encoding_t (hb_vector_t<const hb_vector_t<整数>*> &&rows, unsigned num_cols)
  {
    assert (rows);

    items = std::move (rows);

    もし (unlikely (!chars.resize (num_cols)))
      戻る;

    calculate_chars ();
  }

  void merge (const delta_row_encoding_t& other)
  {
    items.alloc (items.length + other.items.length);
    ために (auto &row : other.items)
      add_row (row);

    // Merge chars
    assert (chars.length == other.chars.length);
    ために (unsigned i = 0; i < chars.length; i++)
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

    ために (auto &row : items)
    {
      assert (row->length == chars.length);

      /* 0/1/2 byte encoding */
      ために (unsigned i = 0; i < row->length; i++)
      {
	整数 v =  row->arrayZ[i];
	もし (v == 0)
	  continue;
	否则 もし (v > 32767 || v < -32768)
	{
	  long_words = true;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	否则 もし (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	否则
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    もし (long_words)
    {
      // Convert 1s to 2s
      ために (auto &v : chars)
	もし (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  bool is_empty () const
  { 戻る !items; }

  bool in_error () const
  { 戻る chars.in_error () || items.in_error (); }

  static inline unsigned get_chars_overhead (unsigned num_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    戻る c + num_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) const
  {
    整数 count = items.length;
    戻る hb_max (0, (整数) overhead - count * (整数) additional_bytes_per_rows);
  }

  整数 gain_from_merging (const delta_row_encoding_t& other_encoding) const
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    もし (additional_bytes_per_rows > 0)
    {
      もし (get_gain (additional_bytes_per_rows) == 0)
        戻る 0;
    }
    否则
    {
      もし (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	戻る 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    整数 combined_gain = (整数) overhead + (整数) other_encoding.overhead;
    combined_gain -= (combined_width - (整数) width) * items.length;
    combined_gain -= (combined_width - (整数) other_encoding.width) * other_encoding.items.length;
    combined_gain -= get_chars_overhead (combined_columns);

    戻る combined_gain;
  }

  bool add_row (const hb_vector_t<整数>* row)
  { 戻る items.push (row); }

  static 整数 cmp (const void *pa, const void *pb)
  {
    const delta_row_encoding_t *a = (const delta_row_encoding_t *)pa;
    const delta_row_encoding_t *b = (const delta_row_encoding_t *)pb;

    もし (a->width != b->width)
      戻る (整数) a->width - (整数) b->width;

    戻る b->chars.cmp (a->chars);
  }
};

struct VarRegionAxis
{
  float evaluate (整数 coord) const
  {
    整数 peak = peakCoord.to_int ();
    もし (peak == 0 || coord == peak)
      戻る 1.f;
    否则 もし (coord == 0) // Faster
      戻る 0.f;

    整数 start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    もし (unlikely (start > peak || peak > end))
      戻る 1.f;
    もし (unlikely (start < 0 && end > 0))
      戻る 1.f;

    もし (coord <= start || end <= coord)
      戻る 0.f;

    /* Interpolate */
    もし (coord < peak)
      戻る float (coord - start) / (peak - start);
    否则
      戻る float (end - coord) / (end - peak);
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
struct SparseVarRegionAxis
{
  float evaluate (const 整数 *coords, unsigned 整数 coord_len) const
  {
    unsigned i = axisIndex;
    整数 coord = i < coord_len ? coords[i] : 0;
    戻る axis.evaluate (coord);
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
    もし (unlikely (!out)) return_trace (false);
    もし (axis_map)
    {
      もし (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (false);
      }
      もし (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
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

struct hb_scalar_cache_t
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

  static hb_scalar_cache_t *create (unsigned 整数 count,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    もし (!count) 戻る (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    もし (scratch_cache && count <= STATIC_LENGTH)
    {
      scratch_cache->length = count;
      scratch_cache->clear ();
      戻る scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (sizeof (hb_scalar_cache_t) - sizeof (static_values) + sizeof (static_values[0]) * count);
    もし (unlikely (!cache)) 戻る (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = count;
    cache->clear ();

    戻る cache;
  }

  static void destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    もし (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  void clear ()
  {
    auto *values = &static_values[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    ために (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    ために (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  bool get (unsigned i, float *value) const
  {
    もし (unlikely (i >= length))
    {
      *value = 0.f;
      戻る true;
    }
    auto *values = &static_values[0];
    auto *cached_value = &values[i];
    float v = *cached_value;
    もし (std::isnan (v))
      戻る false;
    *value = v;
    戻る true;
  }

  HB_ALWAYS_INLINE
  void set (unsigned i, float value)
  {
    もし (unlikely (i >= length)) 戻る;
    auto *values = &static_values[0];
    auto *cached_value = &values[i];
    *cached_value = value;
  }

  public:
  unsigned length;
  private:
  mutable hb_atomic_t<float> static_values[STATIC_LENGTH];
};

struct VarRegionList
{
  private:
  float evaluate_impl (unsigned 整数 region_index,
		       const 整数 *coords, unsigned 整数 coord_len) const
  {
    const VarRegionAxis *axes = axesZ.arrayZ + (region_index * axisCount);
    float v = 1.f;

    unsigned 整数 count = axisCount;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      整数 coord = i < coord_len ? coords[i] : 0;
      float factor = axes[i].evaluate (coord);
      もし (factor == 0.f)
      {
	v = 0.f;
	break;
      }
      v *= factor;
    }

    戻る v;
  }

  public:
  HB_ALWAYS_INLINE
  float evaluate (unsigned 整数 region_index,
		  const 整数 *coords, unsigned 整数 coord_len,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    もし (unlikely (region_index >= regionCount))
      戻る 0.;

    float v;
    もし (cache && cache->get (region_index, &v))
      戻る v;

    v = evaluate_impl (region_index, coords, coord_len);

    もし (cache)
      cache->set (region_index, v);
    戻る v;
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
    もし (!axis_count || !region_count) return_trace (false);
    もし (unlikely (hb_unsigned_mul_overflows (axis_count * region_count,
                                             VarRegionAxis::static_size))) return_trace (false);
    もし (unlikely (!c->extend_min (this))) return_trace (false);
    axisCount = axis_count;
    regionCount = region_count;

    ために (unsigned r = 0; r < region_count; r++)
    {
      const auto& region = regions[r];
      ために (unsigned i = 0; i < axis_count; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        もし (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->middle);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        否则
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        もし (!var_region_rec.serialize (c))
          return_trace (false);
      }
    }
    return_trace (true);
  }

  bool serialize (hb_serialize_context_t *c, const VarRegionList *src, const hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this))) return_trace (false);
    axisCount = src->axisCount;
    regionCount = region_map.get_population ();
    もし (unlikely (hb_unsigned_mul_overflows (axisCount * regionCount,
					     VarRegionAxis::static_size))) return_trace (false);
    もし (unlikely (!c->extend (this))) return_trace (false);
    unsigned 整数 region_count = src->regionCount;
    ために (unsigned 整数 r = 0; r < regionCount; r++)
    {
      unsigned 整数 backward = region_map.backward (r);
      もし (backward >= region_count) return_trace (false);
      hb_memcpy (&axesZ[axisCount * r], &src->axesZ[axisCount * backward], VarRegionAxis::static_size * axisCount);
    }

    return_trace (true);
  }

  bool get_var_region (unsigned region_index,
                       const hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) const
  {
    もし (region_index >= regionCount) 戻る false;
    const VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axisCount);
    ために (unsigned i = 0; i < axisCount; i++)
    {
      hb_tag_t *axis_tag;
      もし (!axes_old_index_tag_map.has (i, &axis_tag))
        戻る false;

      float min_val = axis_region->startCoord.to_float ();
      float def_val = axis_region->peakCoord.to_float ();
      float max_val = axis_region->endCoord.to_float ();

      もし (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((double) min_val, (double) def_val, (double) max_val));
      axis_region++;
    }
    戻る !axis_tuples.in_error ();
  }

  bool get_var_regions (const hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) const
  {
    もし (!regions.alloc (regionCount))
      戻る false;

    ために (unsigned i = 0; i < regionCount; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      もし (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        戻る false;
      regions.push (std::move (axis_tuples));
    }
    戻る !regions.in_error ();
  }

  size_t get_size () const
  { 戻る hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axisCount, regionCount)); }

  public:
  HBUINT16	axisCount;
  HBUINT15	regionCount;
  protected:
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
static inline bool
_hb_avar2_region_is_dead (const hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  ために (auto _ : axis_tuples)
  {
    Triple *range;
    もし (!reachable_ranges.has (_.first, &range)) continue;
    const Triple &tent = _.second;
    整数 start = (整数) roundf ((float) tent.minimum * 16384.f);
    整数 peak  = (整数) roundf ((float) tent.middle * 16384.f);
    整数 end   = (整数) roundf ((float) tent.maximum * 16384.f);
    もし (!peak) continue;
    もし (start > peak || peak > end) continue;
    もし (start < 0 && end > 0) continue;
    整数 lo = (整数) roundf ((float) range->minimum * 16384.f);
    整数 hi = (整数) roundf ((float) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    もし (hi < start || lo > end)
      戻る true;
  }
  戻る false;
}

struct SparseVariationRegion
{
  float evaluate (const 整数 *coords, unsigned 整数 coord_len) const
  {
    float v = 1.f;
    unsigned 整数 count = axes.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      float factor = (this+axes[i]).evaluate (coords, coord_len);
      もし (factor == 0.f)
	戻る 0.;
      v *= factor;
    }
    戻る v;
  }

  bool serialize (hb_serialize_context_t *c,
		  const SparseVariationRegion *src,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (false);
    ために (unsigned i = 0; i < axes.len; i++)
      もし (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (false);
    return_trace (true);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (axes.sanitize (c, this));
  }

  protected:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  public:
  DEFINE_SIZE_ARRAY (2, axes);
};

struct SparseVarRegionList
{
  HB_ALWAYS_INLINE
  float evaluate (unsigned 整数 region_index,
		  const 整数 *coords, unsigned 整数 coord_len,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    もし (unlikely (region_index >= regions.len))
      戻る 0.;

    float v;
    もし (cache && cache->get (region_index, &v))
      戻る v;

    const SparseVariationRegion &region = this+regions[region_index];

    v = region.evaluate (coords, coord_len);
    もし (cache)
      cache->set (region_index, v);

    戻る v;
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
    もし (unlikely (!c->extend_min (this))) return_trace (false);
    regions.len = region_map.get_population ();
    もし (unlikely (!c->extend (regions))) return_trace (false);

    ために (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      もし (unlikely (old_index >= src->regions.len ||
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


struct VarData
{
  unsigned 整数 get_item_count () const
  { 戻る itemCount; }

  unsigned 整数 get_region_index_count () const
  { 戻る regionIndices.len; }

  unsigned get_region_index (unsigned i) const
  { 戻る i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () const
  { 戻る (wordCount () + regionIndices.len) * (longWords () ? 2 : 1); }

  size_t get_size () const
  { 戻る hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (itemCount, get_row_size ())); }

  template <typename Float = float>
  Float _get_delta (unsigned 整数 inner,
		    const 整数 *coords, unsigned 整数 coord_count,
		    const VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) const
  {
    もし (unlikely (inner >= itemCount))
      戻る 0.;
    bool is_long = longWords ();
    unsigned 整数 count = regionIndices.len;
    unsigned word_count = wordCount ();
    unsigned 整数 scount = is_long ? count : word_count;
    unsigned 整数 lcount = is_long ? word_count : 0;

    const HBUINT8 *bytes = get_delta_bytes ();
    const HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned 整数 i = 0;

    const HBINT32 *lcursor = reinterpret_cast<const HBINT32 *> (row);
    ために (; i < lcount; i++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count, cache);
      もし (scalar)
        delta += (Float) scalar * (整数) *lcursor;
      lcursor++;
    }
    const HBINT16 *scursor = reinterpret_cast<const HBINT16 *> (lcursor);
    ために (; i < scount; i++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count, cache);
      もし (scalar)
       delta += (Float) scalar * (整数) *scursor;
      scursor++;
    }
    const HBINT8 *bcursor = reinterpret_cast<const HBINT8 *> (scursor);
    ために (; i < count; i++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count, cache);
      もし (scalar)
        delta += (Float) scalar * (整数) *bcursor;
      bcursor++;
    }

    戻る delta;
  }

  template <typename Float = float>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned 整数 inner,
		   const 整数 *coords, unsigned 整数 coord_count,
		   const VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) const
  {
    unsigned 整数 count = regionIndices.len;
    もし (!count) 戻る Float (0); // This is quite common, so optimize it.
    戻る _get_delta<Float> (inner, coords, coord_count, regions, cache);
  }

  void get_region_scalars (const 整数 *coords, unsigned 整数 coord_count,
			   const VarRegionList &regions,
			   float *scalars /*OUT */,
			   unsigned 整数 num_scalars) const
  {
    unsigned count = hb_min (num_scalars, regionIndices.len);
    ために (unsigned 整数 i = 0; i < count; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_count);
    ために (unsigned 整数 i = count; i < num_scalars; i++)
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
                  const hb_vector_t<const hb_vector_t<整数>*>& rows)
  {
    TRACE_SERIALIZE (this);
    unsigned row_count = rows.length;
    もし (!row_count) {
      // Nothing to serialize, will be empty.
      戻る false;
    }

    もし (unlikely (!c->extend_min (this))) return_trace (false);
    もし (unlikely (!c->check_assign (itemCount, row_count,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    整数 min_threshold = has_long ? -32768 : -128;
    整数 max_threshold = has_long ? +32767 : +127;
    enum delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned num_regions = rows[0]->length;
    もし (!delta_sz.resize (num_regions))
      return_trace (false);

    unsigned word_count = 0;
    ために (unsigned r = 0; r < num_regions; r++)
    {
      ために (unsigned i = 0; i < row_count; i++)
      {
        整数 delta = rows[i]->arrayZ[r];
        もし (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_count++;
          break;
        }
        否则 もし (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_count;
    hb_map_t ri_map;
    ために (unsigned r = 0; r < num_regions; r++)
    {
      もし (!delta_sz[r]) continue;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      もし (!ri_map.set (new_r, r))
        return_trace (false);
    }

    wordSizeCount = word_count | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_count = ri_map.get_population ();
    regionIndices.len = ri_count;
    もし (unlikely (!c->extend (this))) return_trace (false);

    ために (unsigned r = 0; r < ri_count; r++)
    {
      hb_codepoint_t *idx;
      もし (!ri_map.has (r, &idx))
        return_trace (false);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    ために (unsigned 整数 i = 0; i < row_count; i++)
    {
      ために (unsigned 整数 r = 0; r < ri_count; r++)
      {
        整数 delta = rows[i]->arrayZ[ri_map[r]];
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
    もし (unlikely (!c->extend_min (this))) return_trace (false);
    itemCount = inner_map.get_next_value ();

    /* Optimize word count */
    unsigned ri_count = src->regionIndices.len;
    enum delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned 整数> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_count);
    ri_map.resize (ri_count);
    unsigned 整数 new_word_count = 0;
    unsigned 整数 r;

    const HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_count = src->wordCount ();
    bool     src_long_words = src->longWords ();

    bool has_long = false;
    もし (src_long_words)
    {
      ために (r = 0; r < src_word_count; r++)
      {
        ために (unsigned old_gid : inner_map.keys())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  もし (delta < -32768 || 32767 < delta)
	  {
	    has_long = true;
	    break;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    ために (r = 0; r < ri_count; r++)
    {
      bool short_circuit = src_long_words == has_long && src_word_count <= r;

      delta_sz[r] = kZero;
      もし (culled_regions && culled_regions->has (src->regionIndices[r]))
	continue; /* unreachable region: drop its column */
      ために (unsigned old_gid : inner_map.keys())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	もし (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_count++;
	  break;
	}
	否则 もし (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  もし (short_circuit)
	    break;
	}
      }
    }

    unsigned 整数 word_index = 0;
    unsigned 整数 non_word_index = new_word_count;
    unsigned 整数 new_ri_count = 0;
    ために (r = 0; r < ri_count; r++)
      もし (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_count++;
      }

    wordSizeCount = new_word_count | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_count;

    もし (unlikely (!c->extend (this))) return_trace (false);

    ために (r = 0; r < new_ri_count; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned count = itemCount;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      unsigned 整数 old = inner_map.backward (i);
      ために (unsigned 整数 r = 0; r < new_ri_count; r++)
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

    ために (unsigned 整数 r = 0; r < regionIndices.len; r++)
    {
      unsigned 整数 region = regionIndices.arrayZ[r];
      もし (region_indices.has (region)) continue;
      ために (hb_codepoint_t old_gid : inner_map.keys())
	もし (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  break;
	}
    }
  }

  public:
  const HBUINT8 *get_delta_bytes () const
  { 戻る &StructAfter<HBUINT8> (regionIndices); }

  protected:
  HBUINT8 *get_delta_bytes ()
  { 戻る &StructAfter<HBUINT8> (regionIndices); }

  public:
  int32_t get_item_delta_fast (unsigned 整数 item, unsigned 整数 region,
			       const HBUINT8 *delta_bytes, unsigned row_size) const
  {
    もし (unlikely (item >= itemCount || region >= regionIndices.len)) 戻る 0;

    const HBINT8 *p = (const HBINT8 *) delta_bytes + item * row_size;
    unsigned word_count = wordCount ();
    bool is_long = longWords ();
    もし (is_long)
    {
      もし (region < word_count)
	戻る ((const HBINT32 *) p)[region];
      否则
	戻る ((const HBINT16 *)(p + HBINT32::static_size * word_count))[region - word_count];
    }
    否则
    {
      もし (region < word_count)
	戻る ((const HBINT16 *) p)[region];
      否则
	戻る (p + HBINT16::static_size * word_count)[region - word_count];
    }
  }
  int32_t get_item_delta (unsigned 整数 item, unsigned 整数 region) const
  {
     戻る get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  protected:
  void set_item_delta_fast (unsigned 整数 item, unsigned 整数 region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_count = wordCount ();
    bool is_long = longWords ();
    もし (is_long)
    {
      もし (region < word_count)
	((HBINT32 *) p)[region] = delta;
      否则
	((HBINT16 *)(p + HBINT32::static_size * word_count))[region - word_count] = delta;
    }
    否则
    {
      もし (region < word_count)
	((HBINT16 *) p)[region] = delta;
      否则
	(p + HBINT16::static_size * word_count)[region - word_count] = delta;
    }
  }
  void set_item_delta (unsigned 整数 item, unsigned 整数 region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  bool longWords () const { 戻る wordSizeCount & 0x8000u /* LONG_WORDS */; }
  unsigned wordCount () const { 戻る wordSizeCount & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  protected:
  HBUINT16		itemCount;
  HBUINT16		wordSizeCount;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  public:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

struct MultiVarData
{
  size_t get_size () const
  { 戻る hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  const TupleList &get_delta_sets () const
  { 戻る this+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  void get_delta (unsigned 整数 inner,
		  const 整数 *coords, unsigned 整数 coord_count,
		  const SparseVarRegionList &regions,
		  hb_array_t<double> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    auto &deltaSets = get_delta_sets ();

    auto values_iter = deltaSets.fetcher (inner);
    unsigned regionCount = regionIndices.len;
    unsigned skip = 0;
    ために (unsigned regionIndex = 0; regionIndex < regionCount; regionIndex++)
    {
      float scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_count,
				       cache);
      // We skip lazily. Helps with the tail end.
      もし (scalar == 0.0f)
        skip += out.length;
      否则
      {
        もし (skip)
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
  { 戻る get_delta_sets ().count; }

  void collect_region_refs (hb_set_t &region_indices) const
  {
    ために (unsigned region : regionIndices)
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
    もし (unlikely (!c->extend_size (this, header_size, false)))
      return_trace (false);
    format = 1;
    regionIndices.len = region_count;
    ために (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      もし (unlikely (!region_map.has (old_region)))
	return_trace (false);
      regionIndices[i] = region_map.get (old_region);
    }

    const auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    もし (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (false);
    ために (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      もし (unlikely (old_inner >= src_delta_sets.count)) return_trace (false);
      hb_ubytes_t bytes = static_cast<const CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    もし (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (false);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  protected:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  public:
  DEFINE_SIZE_MIN (7);
};

struct ItemVariationStore
{
  friend struct item_variations_t;

  hb_scalar_cache_t *create_cache () const
  {
#ifdef HB_NO_VAR
    戻る hb_scalar_cache_t::create (0);
#endif
    戻る hb_scalar_cache_t::create ((this+regions).regionCount);
  }

  static void destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  private:
  template <typename Float = float>
  Float get_delta (unsigned 整数 outer, unsigned 整数 inner,
		   const 整数 *coords, unsigned 整数 coord_count,
		   hb_scalar_cache_t *cache = nullptr) const
  {
#ifdef HB_NO_VAR
    戻る Float (0);
#endif

    もし (unlikely (outer >= dataSets.len))
      戻る Float (0);

    戻る (this+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_count,
					     this+regions,
					     cache);
  }

  public:
  bool has_delta_set (unsigned 整数 index) const
  {
#ifdef HB_NO_VAR
    戻る false;
#endif

    unsigned 整数 outer = index >> 16;
    unsigned 整数 inner = index & 0xFFFF;
    戻る outer < dataSets.len &&
	   inner < (this+dataSets[outer]).get_item_count ();
  }

  template <typename Float = float>
  Float get_delta (unsigned 整数 index,
		   const 整数 *coords, unsigned 整数 coord_count,
		   hb_scalar_cache_t *cache = nullptr) const
  {
    unsigned 整数 outer = index >> 16;
    unsigned 整数 inner = index & 0xFFFF;
    戻る get_delta<Float> (outer, inner, coords, coord_count, cache);
  }
  template <typename Float = float>
  Float get_delta (unsigned 整数 index,
		   hb_array_t<const 整数> coords,
		   hb_scalar_cache_t *cache = nullptr) const
  {
    戻る get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
#ifdef HB_NO_VAR
    戻る true;
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
    もし (unlikely (!c->extend_min (this))) return_trace (false);

    format = 1;
    もし (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (false);

    unsigned num_var_data = vardata_encodings.length;
    もし (!num_var_data) return_trace (false);
    もし (unlikely (!c->check_assign (dataSets.len, num_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);

    もし (unlikely (!c->extend (dataSets))) return_trace (false);
    ために (unsigned i = 0; i < num_var_data; i++)
      もし (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].items))
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

    もし (unlikely (!c->extend_min (this))) return_trace (false);

    unsigned 整数 set_count = 0;
    ために (unsigned 整数 i = 0; i < inner_maps.length; i++)
      もし (inner_maps[i].get_population ())
	set_count++;

    format = 1;

    const auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    ために (unsigned 整数 i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    もし (region_indices.in_error ())
      return_trace (false);

    region_indices.del_range ((src_regions).regionCount, hb_set_t::INVALID);

    もし (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    もし (region_map.in_error())
      return_trace (false);

    もし (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (false);

    dataSets.len = set_count;
    もし (unlikely (!c->extend (dataSets))) return_trace (false);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned 整数 set_index = 0;
    ために (unsigned 整数 i = 0; i < inner_maps.length; i++)
    {
      もし (!inner_maps[i].get_population ()) continue;
      もし (unlikely (!dataSets[set_index++]
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
    もし (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned count = dataSets.len;
    ために (unsigned i = 0; i < count; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      もし (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = this+dataSets[i];

      unsigned itemCount = data.get_item_count ();
      ために (unsigned j = 0; j < itemCount; j++)
	map->add (j);
    }

    もし (unlikely (!out->serialize (c, this, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  bool collect_dead_regions (const hb_map_t &axes_old_index_tag_map,
			     const hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) const
  {
#ifndef HB_NO_VAR
    もし (!reachable_ranges.get_population ()) 戻る true;
    const VarRegionList &region_list = this+regions;
    unsigned count = region_list.regionCount;
    ために (unsigned i = 0; i < count; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      もし (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	戻る false;
      もし (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    戻る !dead_regions.in_error ();
#否则
    戻る true;
#endif
  }

  bool subset (hb_subset_context_t *c, const hb_array_t<const hb_inc_bimap_t> &inner_maps) const
  {
    TRACE_SUBSET (this);
#ifdef HB_NO_VAR
    return_trace (false);
#endif

    ItemVariationStore *varstore_prime = c->serializer->start_embed<ItemVariationStore> ();
    もし (unlikely (!varstore_prime)) return_trace (false);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    もし (c->plan->has_avar2)
      もし (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (false);

    varstore_prime->serialize (c->serializer, this, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_prime->dataSets);
  }

  unsigned 整数 get_region_index_count (unsigned 整数 major) const
  {
#ifdef HB_NO_VAR
    戻る 0;
#endif
    戻る (this+dataSets[major]).get_region_index_count ();
  }

  void get_region_scalars (unsigned 整数 major,
			   const 整数 *coords, unsigned 整数 coord_count,
			   float *scalars /*OUT*/,
			   unsigned 整数 num_scalars) const
  {
#ifdef HB_NO_VAR
    ために (unsigned i = 0; i < num_scalars; i++)
      scalars[i] = 0.f;
    戻る;
#endif

    (this+dataSets[major]).get_region_scalars (coords, coord_count,
					       this+regions,
					       &scalars[0], num_scalars);
  }

  unsigned 整数 get_sub_table_count () const
   {
#ifdef HB_NO_VAR
     戻る 0;
#endif
     戻る dataSets.len;
   }

  const VarData& get_sub_table (unsigned i) const
  {
#ifdef HB_NO_VAR
     戻る Null (VarData);
#endif
     戻る this+dataSets[i];
  }

  const VarRegionList& get_region_list () const
  {
#ifdef HB_NO_VAR
     戻る Null (VarRegionList);
#endif
     戻る this+regions;
  }

  protected:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  public:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

struct MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) const
  {
#ifdef HB_NO_VAR
    戻る hb_scalar_cache_t::create (0);
#endif
    auto &r = this+regions;
    unsigned count = r.regions.len;

    戻る hb_scalar_cache_t::create (count, static_cache);
  }

  static void destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  private:
  void get_delta (unsigned 整数 outer, unsigned 整数 inner,
		  const 整数 *coords, unsigned 整数 coord_count,
		  hb_array_t<double> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
#ifdef HB_NO_VAR
    戻る;
#endif

    もし (unlikely (outer >= dataSets.len))
      戻る;

    戻る (this+dataSets[outer]).get_delta (inner,
					     coords, coord_count,
					     this+regions,
					     out,
					     cache);
  }

  public:
  void get_delta (unsigned 整数 index,
		  const 整数 *coords, unsigned 整数 coord_count,
		  hb_array_t<double> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    unsigned 整数 outer = index >> 16;
    unsigned 整数 inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_count, out, cache);
  }
  void get_delta (unsigned 整数 index,
		  hb_array_t<const 整数> coords,
		  hb_array_t<double> out,
		  hb_scalar_cache_t *cache = nullptr) const
  {
    戻る get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
#ifdef HB_NO_VAR
    戻る true;
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
    もし (unlikely (!inner_maps->resize (dataSets.len))) 戻る false;
    ために (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      もし (unlikely (outer >= dataSets.len ||
		    inner >= (this+dataSets[outer]).get_item_count ()))
	戻る false;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    ために (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      const hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      もし (!inner_map.get_population ()) continue;
      もし (unlikely (inner_map.in_error ())) 戻る false;
      ために (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    戻る !inner_maps->in_error () && !varidx_map->in_error ();
  }

  bool serialize (hb_serialize_context_t *c,
		  const MultiItemVariationStore *src,
		  const hb_array_t<const hb_inc_bimap_t> &inner_maps,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (false);
    format = 1;

    const auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_count = 0;
    ために (unsigned i = 0; i < src->dataSets.len; i++)
    {
      もし (!inner_maps[i].get_population ()) continue;
      set_count++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    もし (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (false);

    dataSets.len = set_count;
    もし (unlikely (!c->extend (dataSets))) return_trace (false);
    unsigned set_index = 0;
    ために (unsigned i = 0; i < src->dataSets.len; i++)
    {
      もし (!inner_maps[i].get_population ()) continue;
      もし (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (false);
    }
    return_trace (true);
  }

  protected:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  public:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename MapCountT>
struct DeltaSetIndexMapFormat01
{
  friend struct DeltaSetIndexMap;

  size_t get_size () const
  { 戻る hb_unsigned_mul_add_saturate (mapCount, get_width (), min_size); }

  private:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    return_trace (c->embed (this));
  }

  template <typename T>
  bool serialize (hb_serialize_context_t *c, const T &plan)
  {
    unsigned 整数 width = plan.get_width ();
    unsigned 整数 inner_bit_count = plan.get_inner_bit_count ();
    const hb_array_t<const uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (this);
    もし (unlikely (output_map.length && ((((inner_bit_count-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (false);
    もし (unlikely (!c->extend_min (this))) return_trace (false);

    entryFormat = ((width-1)<<4)|(inner_bit_count-1);
    mapCount = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    もし (unlikely (!p)) return_trace (false);
    ために (unsigned 整数 i = 0; i < output_map.length; i++)
    {
      unsigned 整数 v = output_map.arrayZ[i];
      もし (v)
      {
	unsigned 整数 outer = v >> 16;
	unsigned 整数 inner = v & 0xFFFF;
	unsigned 整数 u = (outer << inner_bit_count) | inner;
	ために (unsigned 整数 w = width; w > 0;)
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
  uint32_t map (unsigned 整数 v) const /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    もし (!mapCount)
      戻る v;
    戻る _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned 整数 v) const /* Returns 16.16 outer.inner. */
  {
    もし (v >= mapCount)
      v = mapCount - 1;

    unsigned 整数 u = 0;
    { /* Fetch it. */
      unsigned 整数 w = get_width ();
      const HBUINT8 *p = mapDataZ.arrayZ + w * v;
      ために (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned 整数 n = get_inner_bit_count ();
      unsigned 整数 outer = u >> n;
      unsigned 整数 inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    戻る u;
  }

  unsigned get_map_count () const       { 戻る mapCount; }
  unsigned get_width () const           { 戻る ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_count () const { 戻る (entryFormat & 0xF) + 1; }


  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  mapCount,
                                  get_width ()));
  }

  protected:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  MapCountT     mapCount;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  public:
  DEFINE_SIZE_ARRAY (2+MapCountT::static_size, mapDataZ);
};

struct DeltaSetIndexMap
{
  template <typename T>
  bool serialize (hb_serialize_context_t *c, const T &plan)
  {
    TRACE_SERIALIZE (this);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    switch (u.format.v) {
    case 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    case 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    default:return_trace (false);
    }
  }

  uint32_t map (unsigned v) const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); 戻る (u.format0.map (v));
    case 1: hb_barrier (); 戻る (u.format1.map (v));
    default:戻る v;
    }
  }

  unsigned get_map_count () const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); 戻る u.format0.get_map_count ();
    case 1: hb_barrier (); 戻る u.format1.get_map_count ();
    default:戻る 0;
    }
  }

  unsigned get_width () const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); 戻る u.format0.get_width ();
    case 1: hb_barrier (); 戻る u.format1.get_width ();
    default:戻る 0;
    }
  }

  unsigned get_inner_bit_count () const
  {
    switch (u.format.v) {
    case 0: hb_barrier (); 戻る u.format0.get_inner_bit_count ();
    case 1: hb_barrier (); 戻る u.format1.get_inner_bit_count ();
    default:戻る 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    もし (!u.format.v.sanitize (c)) return_trace (false);
    hb_barrier ();
    switch (u.format.v) {
    case 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    case 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    default:return_trace (true);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) const
  {
    TRACE_SERIALIZE (this);
    switch (u.format.v) {
    case 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    case 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    default:return_trace (nullptr);
    }
  }

  protected:
  union {
  struct { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  public:
  DEFINE_SIZE_UNION (1, format.v);
};


struct ItemVarStoreInstancer
{
  ItemVarStoreInstancer (const ItemVariationStore *varStore_,
			 const DeltaSetIndexMap *varIdxMap,
			 hb_array_t<const 整数> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    もし (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    もし (!hb_any (coords))
      this->coords = {};
  }

  operator bool () const { 戻る varStore && bool (coords); }

  float operator[] (uint32_t varIdx) const
  { 戻る (*this) (varIdx); }

  float operator() (uint32_t varIdx, unsigned short offset = 0) const
  { 戻る get_delta (varIdx, offset); }

  double get_condition_delta (uint32_t varIdx) const
  { 戻る get_delta<double> (varIdx); }

  template <typename Float = float>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) const
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   もし (!coords || varIdx == VarIdx::NO_VARIATION)
     戻る Float (0);

    varIdx += offset;
    もし (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    戻る varStore->get_delta<Float> (varIdx, coords, cache);
  }

  const ItemVariationStore *varStore;
  const DeltaSetIndexMap *varIdxMap;
  hb_array_t<const 整数> coords;
  hb_scalar_cache_t *cache;
};

struct MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (const MultiItemVariationStore *varStore,
			      const DeltaSetIndexMap *varIdxMap,
			      hb_array_t<const 整数> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    もし (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator bool () const { 戻る varStore; }

  double get_condition_delta (uint32_t varIdx) const
  { 戻る (double) (*this)[varIdx]; }

  double operator[] (uint32_t varIdx) const
  {
    double v = 0;
    (*this) (hb_array (&v, 1), varIdx);
    戻る v;
  }

  void operator() (hb_array_t<double> out, uint32_t varIdx, unsigned short offset = 0) const
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    もし (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      もし (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    否则
      ために (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  const MultiItemVariationStore *varStore;
  const DeltaSetIndexMap *varIdxMap;
  hb_array_t<const 整数> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
enum Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

enum lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

struct Condition;

template <typename Instancer>
static bool
_hb_recurse_condition_evaluate (const struct Condition &condition,
				const 整数 *coords,
				unsigned 整数 coord_len,
				Instancer *instancer);

struct ConditionAxisRange
{
  friend struct Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    もし (unlikely (!out)) return_trace (false);

    const hb_map_t *index_map = &c->plan->axes_index_map;
    もし (index_map->is_empty ()) return_trace (true);

    const hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    もし (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (false);

    const hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    もし (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    const hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    もし (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    float normalized_min = renormalizeValue ((double) filterRangeMinValue.to_float (), axis_limit, axis_triple_distances, false);
    float normalized_max = renormalizeValue ((double) filterRangeMaxValue.to_float (), axis_limit, axis_triple_distances, false);
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
    もし (!c->axes_index_tag_map->has (axisIndex))
      戻る DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    bool axis_set_by_user = false;
    もし (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = true;
    }

    float axis_min_val = axis_range.minimum;
    float axis_default_val = axis_range.middle;
    float axis_max_val = axis_range.maximum;

    float filter_min_val = filterRangeMinValue.to_float ();
    float filter_max_val = filterRangeMaxValue.to_float ();

    もし (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = false;

    //condition not met, drop the entire record
    もし (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      戻る DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    もし (axis_set_by_user && axis_range.is_point ())
      戻る DROP_COND_WITH_VAR;

    もし (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filterRangeMaxValue.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filterRangeMinValue.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      戻る KEEP_COND_WITH_VAR;
    }
    戻る KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 Instancer *instancer HB_UNUSED) const
  {
    整数 coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    戻る filterRangeMinValue.to_int () <= coord && coord <= filterRangeMaxValue.to_int ();
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filterRangeMinValue;
  F2DOT14	filterRangeMaxValue;
  public:
  DEFINE_SIZE_STATIC (8);
};

struct ConditionValue
{
  friend struct Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    もし (unlikely (!out)) return_trace (false);

    もし (varIdx == VarIdx::NO_VARIATION)
      return_trace (true);

    hb_pair_t<unsigned, 整数> *condition_value;
    もし (c->plan->layout_condition_idx_value_map.has ((uintptr_t) this, &condition_value))
    {
      out->defaultValue = condition_value->second;
      out->varIdx = condition_value->first;
      return_trace (true);
    }

    hb_pair_t<unsigned, 整数> *new_varidx_delta;
    もし (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (false);

    もし (unlikely (!c->serializer->check_assign (
		out->defaultValue,
		defaultValue + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (false);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (true);
  }

  private:
  template <typename Instancer>
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 Instancer *instancer) const
  {
    double value = defaultValue;
    value += instancer->get_condition_delta (varIdx);
    戻る value > 0;
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (c->check_struct (this));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	defaultValue;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  public:
  DEFINE_SIZE_STATIC (8);
};

struct ConditionAnd
{
  friend struct Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);
    out->format = format;

    ために (const auto &offset : conditions)
    {
      もし (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	もし (unlikely (!out_offset)) return_trace (false);
	*out_offset = 0;
      }
      否则 もし (unlikely (!subset_offset_array (c, out->conditions, this) (offset)))
	return_trace (false);
    }

    return_trace (true);
  }

  private:
  template <typename Instancer>
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 Instancer *instancer) const
  {
    unsigned 整数 count = conditions.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (!_hb_recurse_condition_evaluate (this+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	戻る false;
    戻る true;
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, this));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<struct Condition>	conditions;
  public:
  DEFINE_SIZE_ARRAY (3, conditions);
};

struct ConditionOr
{
  friend struct Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);
    out->format = format;

    ために (const auto &offset : conditions)
    {
      もし (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	もし (unlikely (!out_offset)) return_trace (false);
	*out_offset = 0;
      }
      否则 もし (unlikely (!subset_offset_array (c, out->conditions, this) (offset)))
	return_trace (false);
    }

    return_trace (true);
  }

  private:
  template <typename Instancer>
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 Instancer *instancer) const
  {
    unsigned 整数 count = conditions.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (_hb_recurse_condition_evaluate (this+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	戻る true;
    戻る false;
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (conditions.sanitize (c, this));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<struct Condition>	conditions;
  public:
  DEFINE_SIZE_ARRAY (3, conditions);
};

struct ConditionNegate
{
  friend struct Condition;

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->embed (this);
    もし (unlikely (!out)) return_trace (false);
    もし (condition.is_null ())
    {
      out->condition = 0;
      return_trace (true);
    }
    return_trace (out->condition.serialize_subset (c, condition, this));
  }

  private:
  template <typename Instancer>
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 Instancer *instancer) const
  {
    戻る !_hb_recurse_condition_evaluate (this+condition,
					    coords, coord_len,
					    instancer);
  }

  public:
  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (condition.sanitize (c, this));
  }

  protected:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<struct Condition>	condition;
  public:
  DEFINE_SIZE_STATIC (5);
};

struct Condition
{
  friend struct ConditionSet;

  static bool serialize_constant (hb_serialize_context_t *c, bool value)
  {
    auto *out = c->start_embed<ConditionValue> ();
    もし (unlikely (!out || !c->extend_min (out))) 戻る false;
    out->format = 2;
    out->defaultValue = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    戻る true;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      bool materialize_true = false) const;

  template <typename Instancer>
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 Instancer *instancer) const
  {
    switch (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    case 0: 戻る true;
    case 1: hb_barrier (); 戻る u.format1.evaluate (coords, coord_len, instancer);
    case 2: hb_barrier (); 戻る u.format2.evaluate (coords, coord_len, instancer);
    case 3: hb_barrier (); 戻る u.format3.evaluate (coords, coord_len, instancer);
    case 4: hb_barrier (); 戻る u.format4.evaluate (coords, coord_len, instancer);
    case 5: hb_barrier (); 戻る u.format5.evaluate (coords, coord_len, instancer);
    default:戻る false;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) const
  {
    もし (unlikely (!depth))
    {
      c->apply = false;
      戻る DROP_RECORD_WITH_VAR;
    }

    switch (u.format.v) {
    case 0: 戻る KEEP_RECORD_WITH_VAR;
    case 1: hb_barrier (); 戻る u.format1.keep_with_variations (c, condition_map);
    case 2: case 3: case 4: case 5: break;
    default: c->apply = false; 戻る DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    bool applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    もし (c->all_axes_pinned)
      戻る applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    もし (u.format.v == 2)
      戻る u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (const Condition &child)
    {
      auto child_context = *c;
      child_context.apply = true;
      戻る child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    もし (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      戻る result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    bool is_and = u.format.v == 3;
    bool variable = false;
    const auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    const void *base = is_and ? (const void *) &u.format3 : (const void *) &u.format4;
    ために (const auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      もし (is_and && result == DROP_RECORD_WITH_VAR)
        戻る DROP_RECORD_WITH_VAR;
      もし (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        戻る KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    戻る variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) const
  {
    もし (unlikely (!c->may_dispatch (this, &u.format.v))) 戻る c->no_dispatch_return_value ();
    TRACE_DISPATCH (this, u.format.v);
    switch (u.format.v) {
    case 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    case 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    case 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    case 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    case 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    default:return_trace (c->default_return_value ());
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);

    もし (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_value ());

    return_trace (c->end_recursion (this->dispatch (c)));
  }

  bool collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, 整数>> *condition_values = nullptr) const;

  bool serialize (hb_serialize_context_t *c,
		  const Condition *src,
		  const hb_map_t &varidx_map,
		  const hb_map_t *axis_map = nullptr);

  private:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) const;

  protected:
  union {
  struct { HBUINT16 v; }	format;		/* Format identifier */
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
_hb_recurse_condition_evaluate (const struct Condition &condition,
				const 整数 *coords,
				unsigned 整数 coord_len,
				Instancer *instancer)
{
  戻る condition.evaluate (coords, coord_len, instancer);
}

struct ConditionList
{
  const Condition& operator[] (unsigned i) const
  { 戻る this+conditions[i]; }

  unsigned get_count () const { 戻る conditions.len; }

  bool serialize (hb_serialize_context_t *c,
		  const ConditionList *src,
		  const hb_inc_bimap_t &condition_map,
		  const hb_map_t &varidx_map,
		  const hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (this);
    もし (unlikely (!c->extend_min (this))) return_trace (false);
    conditions.len = condition_map.get_population ();
    もし (unlikely (!c->extend (conditions))) return_trace (false);
    ために (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      もし (unlikely (old_index >= src->conditions.len ||
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

  protected:
  Array32OfOffset32To<Condition> conditions;
  public:
  DEFINE_SIZE_ARRAY (4, conditions);
};

inline bool
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, 整数>> *condition_values) const
{
  もし (unlikely (!depth)) 戻る false;
  switch (u.format.v)
  {
    case 0:
    case 1:
      戻る true;
    case 2:
      もし (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        もし (condition_values &&
            !condition_values->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), 整数 (u.format2.defaultValue))))
          戻る false;
      }
      戻る !var_indices->in_error ();
    case 3:
      ために (const auto &offset : u.format3.conditions)
	もし (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_values)))
	  戻る false;
      戻る true;
    case 4:
      ために (const auto &offset : u.format4.conditions)
	もし (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_values)))
	  戻る false;
      戻る true;
    case 5:
      戻る (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_values);
    default:
      戻る false;
  }
}

inline bool
Condition::serialize (hb_serialize_context_t *c,
		      const Condition *src,
		      const hb_map_t &varidx_map,
		      const hb_map_t *axis_map)
{
  TRACE_SERIALIZE (this);
  switch (src->u.format.v)
  {
    case 0:
    {
      auto *out = c->start_embed<ConditionValue> ();
      もし (unlikely (!out || !c->extend_min (out))) return_trace (false);
      out->format = 2;
      out->defaultValue = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (true);
    }
    case 1:
    {
      auto *out = c->embed (&src->u.format1);
      もし (unlikely (!out)) return_trace (false);
      もし (axis_map)
      {
	もし (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (false);
	}
	もし (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (false);
      }
      return_trace (true);
    }
    case 2:
    {
      auto *out = c->embed (&src->u.format2);
      もし (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (false);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (true);
    }
    case 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      もし (unlikely (!c->extend_min (out))) return_trace (false);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      もし (unlikely (!c->extend (out->conditions))) return_trace (false);
      ために (unsigned i = 0; i < out->conditions.len; i++)
	もし (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (false);
      return_trace (true);
    }
    case 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      もし (unlikely (!c->extend_min (out))) return_trace (false);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      もし (unlikely (!c->extend (out->conditions))) return_trace (false);
      ために (unsigned i = 0; i < out->conditions.len; i++)
	もし (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (false);
      return_trace (true);
    }
    case 5:
    {
      auto *out = c->embed (&src->u.format5);
      もし (unlikely (!out ||
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

inline lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    bool materialize_true) const
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  もし (result == LOOKUP_CONDITION_SUBSET_KEEP)
    戻る result;

  c->serializer->revert (snap);
  もし (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    戻る result;

  戻る serialize_constant (c->serializer, true) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

inline lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) const
{
  もし (unlikely (!depth))
    戻る LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   const Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    もし (unlikely (!offset))
      戻る LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    もし (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->pop_pack ());
    否则
    {
      c->serializer->pop_discard ();
      out.pop ();
      c->serializer->revert (snap);
    }
    戻る result;
  };

  switch (u.format.v)
  {
    case 0:
      戻る LOOKUP_CONDITION_SUBSET_TRUE;

    case 1:
    {
      const ConditionAxisRange &src = u.format1;
      もし (c->plan->user_axes_location.is_empty ())
        戻る c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      もし (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      もし (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      float filter_min = src.filterRangeMinValue.to_float ();
      float filter_max = src.filterRangeMaxValue.to_float ();
      もし (filter_min > filter_max)
        戻る LOOKUP_CONDITION_SUBSET_FALSE;
      もし (axis_limit.is_point ())
        戻る filter_min <= axis_limit.middle &&
	       axis_limit.middle <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      もし (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        戻る LOOKUP_CONDITION_SUBSET_FALSE;
      もし (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        戻る LOOKUP_CONDITION_SUBSET_TRUE;
      もし (!c->plan->axes_index_map.has (src.axisIndex))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      もし (unlikely (!out))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      もし (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filterRangeMinValue.set_float (
	  renormalizeValue (filter_min, axis_limit, axis_distances, false));
      out->filterRangeMaxValue.set_float (
	  renormalizeValue (filter_max, axis_limit, axis_distances, false));
      もし (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;
      戻る LOOKUP_CONDITION_SUBSET_KEEP;
    }

    case 2:
    {
      const ConditionValue &src = u.format2;
      auto *out = c->serializer->embed (&src);
      もし (unlikely (!out))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;

      もし (src.varIdx == VarIdx::NO_VARIATION)
        戻る c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((整数) src.defaultValue > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, 整数> *condition_value;
      もし (c->plan->layout_condition_idx_value_map.has ((uintptr_t) &src, &condition_value))
      {
        out->defaultValue = condition_value->second;
        out->varIdx = condition_value->first;
        もし (out->varIdx == VarIdx::NO_VARIATION)
          戻る out->defaultValue > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        戻る LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, 整数> *new_varidx_delta;
      もし (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        戻る c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((整数) src.defaultValue > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      もし (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        double value = (整数) src.defaultValue +
                      c->plan->layout_variation_delta (src.varIdx);
        戻る value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      整数 default_value = (整数) src.defaultValue +
			  hb_second (*new_varidx_delta);
      もし (unlikely (!c->serializer->check_assign (
		out->defaultValue,
		default_value,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      戻る LOOKUP_CONDITION_SUBSET_KEEP;
    }

    case 3:
    {
      const ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      もし (unlikely (!out || !c->serializer->extend_min (out)))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      ために (const auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        もし (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          戻る result;
      }
      戻る out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    case 4:
    {
      const ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      もし (unlikely (!out || !c->serializer->extend_min (out)))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      ために (const auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        もし (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          戻る result;
      }
      戻る out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    case 5:
    {
      const ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      もし (unlikely (!out))
        戻る LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      もし (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->pop_pack ());
        戻る result;
      }

      c->serializer->pop_discard ();
      もし (result == LOOKUP_CONDITION_SUBSET_TRUE)
        戻る LOOKUP_CONDITION_SUBSET_FALSE;
      もし (result == LOOKUP_CONDITION_SUBSET_FALSE)
        戻る LOOKUP_CONDITION_SUBSET_TRUE;
      戻る result;
    }

    default:
      戻る LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

struct ConditionSet
{
  bool evaluate (const 整数 *coords, unsigned 整数 coord_len,
		 ItemVarStoreInstancer *instancer) const
  {
    unsigned 整数 count = conditions.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (!(this+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	戻る false;
    戻る true;
  }

  void keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) const
  {
    hb_map_t *condition_map = hb_map_create ();
    もし (unlikely (!condition_map)) 戻る;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    もし (unlikely (!cond_set)) 戻る;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = true;
    bool should_keep = false;
    /* The range map cannot identify value or boolean expressions uniquely. */
    bool can_deduplicate = true;
    unsigned num_kept_cond = 0, cond_idx = 0;
    ために (const auto& offset : conditions)
    {
      const Condition &condition = this+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      もし (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = false;
        戻る;
      }

      もし (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = true;
        cond_set->add (cond_idx);
        num_kept_cond++;
      }

      もし (ret == KEEP_RECORD_WITH_VAR)
        should_keep = true;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = num_kept_cond == 0;
    もし (!should_keep && c->all_axes_pinned) 戻る;

    //check if condition_set is unique with variations
    もし (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      戻る;

    もし (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  bool subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               bool insert_catch_all) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    もし (insert_catch_all) return_trace (true);

    hb_set_t *retained_cond_set = nullptr;
    もし (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned 整数 count = conditions.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      もし (retained_cond_set != nullptr && !retained_cond_set->has (i))
        continue;
      const Condition &condition = this+conditions[i];
      もし (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, this) (conditions[i]);
        continue;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      もし (unlikely (!offset)) return_trace (false);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, true);
      もし (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, false) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      もし (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
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
    ために (const auto &offset : conditions)
      もし (unlikely (!(this+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_values)))
	c->layout_variation_indices->err ();
  }

  protected:
  Array16OfOffset32To<Condition>	conditions;
  public:
  DEFINE_SIZE_ARRAY (2, conditions);
};

struct FeatureTableSubstitutionRecord
{
  friend struct FeatureTableSubstitution;

  void collect_lookups (const void *base, hb_set_t *lookup_indexes /* OUT */) const
  {
    戻る (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  void closure_features (const void *base,
			 const hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    もし ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  void collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, const Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    const hb_set_t *feature_indices,
                                                    const void *base) const
  {
    もし (feature_indices->has (featureIndex))
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
    もし (unlikely (!s->extend_min (this))) return_trace (false);

    uint32_t *new_feature_idx;
    もし (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (false);

    もし (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (false);

    s->push ();
    bool ret = f->subset (c->subset_context, c, tag);
    もし (ret) s->add_link (feature, s->pop_pack ());
    否则 s->pop_discard ();

    return_trace (ret);
  }

  bool subset (hb_subset_layout_context_t *c, const void *base) const
  {
    TRACE_SUBSET (this);
    uint32_t *new_feature_index;
    もし (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (false);

    auto *out = c->subset_context->serializer->embed (this);
    もし (unlikely (!out)) return_trace (false);

    out->featureIndex = *new_feature_index;
    hb_pair_t<const void*, const void*> *original;
    もし (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (false);
    const Tag *tag = reinterpret_cast<const Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  bool sanitize (hb_sanitize_context_t *c, const void *base,
		 const FeatureList *feature_list) const
  {
    TRACE_SANITIZE (this);
    もし (unlikely (!c->check_struct (this))) return_trace (false);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    const Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  protected:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  public:
  DEFINE_SIZE_STATIC (6);
};

struct FeatureTableSubstitution
{
  void collect_feature_indices (hb_set_t *feature_indices) const
  {
    ために (const FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  const Feature *find_substitute (unsigned 整数 feature_index) const
  {
    unsigned 整数 count = substitutions.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      const FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      もし (record.featureIndex == feature_index)
	戻る &(this+record.feature);
    }
    戻る nullptr;
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
    ために (const FeatureTableSubstitutionRecord &record : substitutions)
      もし (feature_index_map->has (record.featureIndex))
        (this+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  void closure_features (const hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    ために (const FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (this, lookup_indexes, feature_indexes);
  }

  bool intersects_features (const hb_map_t *feature_index_map) const
  {
    ために (const FeatureTableSubstitutionRecord& record : substitutions)
    {
      もし (feature_index_map->has (record.featureIndex)) 戻る true;
    }
    戻る false;
  }

  void collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) const
  {
    ために (const FeatureTableSubstitutionRecord& record : substitutions)
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
    もし (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;

    もし (insert_catch_all)
    {
      ために (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<const void*, const void*> *p;
        もし (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (false);
        auto *o = out->substitutions.serialize_append (c->serializer);
        もし (!o->serialize (l, feature_index,
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

  protected:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  public:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

struct FeatureVariationRecord
{
  friend struct FeatureVariations;

  void collect_lookups (const void     *base,
			const hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) const
  {
    戻る (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
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
    戻る (base+substitutions).intersects_features (feature_index_map);
  }

  void collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    const void *base) const
  {
    (base+conditions).keep_with_variations (c);
    もし (c->apply && !c->variation_applied)
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
    もし (unlikely (!out)) return_trace (false);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    もし (!insert_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    否则
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

  protected:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  public:
  DEFINE_SIZE_STATIC (8);
};

struct LookupIndexList
{
  void add_lookup_indexes_to (hb_set_t *lookup_indexes) const
  { lookupIndices.add_indexes_to (lookup_indexes); }

  bool intersects_lookup_indexes (const hb_map_t *lookup_indexes) const
  { 戻る lookupIndices.intersects (lookup_indexes); }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!out || !c->serializer->extend_min (out)))
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

  protected:
  IndexArray	lookupIndices;
  public:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

struct LookupConditionRecord
{
  unsigned get_condition_offset (const void *base, const void *variations) const
  { 戻る (const char *) &(base+condition) - (const char *) variations; }

  template <typename Instancer>
  bool evaluate (const void *base,
		 const 整数 *coords,
		 unsigned 整数 coord_len,
		 Instancer *instancer) const
  { 戻る (base+condition).evaluate (coords, coord_len, instancer); }

  bool evaluate (const void *base,
                 const void *variations,
                 hb_sorted_array_t<const unsigned> conditions) const
  {
    戻る !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     sizeof (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  bool collect_condition (const void *base,
                          const void *variations,
                          const 整数 *coords,
                          unsigned 整数 coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) const
  {
    /* Null conditions always apply and need no key state. */
    もし (!condition) 戻る true;

    unsigned offset = get_condition_offset (base, variations);
    もし (visited->has (offset)) 戻る true;
    visited->add (offset);
    もし (unlikely (visited->in_error ())) 戻る false;

    もし (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    戻る !conditions->in_error ();
  }

  void add_lookup_indexes_to (const void *base,
			      hb_set_t *lookup_indexes) const
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  bool intersects_lookup_indexes (const void *base,
				   const hb_map_t *lookup_indexes) const
  { 戻る (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  const void *base) const
  {
    もし (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
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
    もし (unlikely (!out)) return_trace (false);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    true);
    もし (result != LOOKUP_CONDITION_SUBSET_KEEP)
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

  protected:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  public:
  DEFINE_SIZE_STATIC (8);
};

struct FeatureLookupsTable
{
  bool adds_default_lookups () const { 戻る flags & 0x0001u; }

  bool collect_conditions (const void *variations,
                           const 整数 *coords,
                           unsigned 整数 coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) const
  {
    ために (const LookupConditionRecord &record : records)
      もし (unlikely (!record.collect_condition (this, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	戻る false;
    戻る true;
  }

  void collect_lookups (const void *variations,
                       hb_sorted_array_t<const unsigned> conditions,
                       hb_set_t *lookup_indexes) const
  {
    ために (const LookupConditionRecord &record : records)
      もし (record.evaluate (this, variations, conditions))
	record.add_lookup_indexes_to (this, lookup_indexes);
  }

  template <typename Instancer>
  void collect_lookups (const 整数 *coords,
			unsigned 整数 coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) const
  {
    ために (const LookupConditionRecord &record : records)
      もし (record.evaluate (this, coords, coord_len, instancer))
	record.add_lookup_indexes_to (this, lookup_indexes);
  }

  void collect_lookups (hb_set_t *lookup_indexes) const
  {
    ために (const LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (this, lookup_indexes);
  }

  bool intersects_lookup_indexes (const hb_map_t *lookup_indexes) const
  {
    ために (const LookupConditionRecord &record : records)
      もし (record.intersects_lookup_indexes (this, lookup_indexes))
	戻る true;
    戻る false;
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ために (const LookupConditionRecord &record : records)
      record.collect_variation_indices (c, this);
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    ために (const LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      もし (record.subset (c, l, this))
	out->records.len++;
      否则
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

  protected:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  public:
  DEFINE_SIZE_ARRAY (10, records);
};

struct LookupVariationRecord
{
  friend struct FeatureVariations;

  unsigned get_feature_index () const { 戻る featureIndex; }

  void collect_lookups (const void *base,
                       const Feature &feature,
                       hb_sorted_array_t<const unsigned> conditions,
                       hb_set_t *lookup_indexes) const
  {
    const FeatureLookupsTable &feature_lookups = base+featureLookups;
    もし (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  void collect_lookups (const void *base,
			const hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) const
  {
    もし (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  void closure_features (const void *base,
			 const hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) const
  {
    もし ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
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
    もし (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (false);

    auto *out = c->serializer->embed (this);
    もし (unlikely (!out ||
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

  protected:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  public:
  DEFINE_SIZE_STATIC (6);
};

struct FeatureVariations
{
  static constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  const Array32Of<LookupVariationRecord> &get_lookup_variation_records () const
  {
    もし (version.to_int () < 0x00010001u)
      戻る Null (Array32Of<LookupVariationRecord>);
    戻る StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_count () const
  {
    戻る varRecords.len;
  }

  bool find_index (const 整数 *coords, unsigned 整数 coord_len,
		   unsigned 整数 *index,
		   ItemVarStoreInstancer *instancer) const
  {
    unsigned 整数 count = varRecords.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      const FeatureVariationRecord &record = varRecords.arrayZ[i];
      もし ((this+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	戻る true;
      }
    }
    *index = NOT_FOUND_INDEX;
    戻る false;
  }

  const Feature *find_substitute (unsigned 整数 variations_index,
				  unsigned 整数 feature_index) const
  {
    const FeatureVariationRecord &record = varRecords[variations_index];
    戻る (this+record.substitutions).find_substitute (feature_index);
  }

  bool collect_lookup_conditions (const 整数 *coords,
                                  unsigned 整数 coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) const
  {
    hb_set_t visited_tables, visited_conditions;
    ために (const LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      もし (visited_tables.has (offset)) continue;
      visited_tables.add (offset);
      もし (unlikely (visited_tables.in_error ())) 戻る false;

      const FeatureLookupsTable &feature_lookups = this+record.featureLookups;
      もし (unlikely (!feature_lookups.collect_conditions (this, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	戻る false;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    戻る true;
  }

  bool get_lookup_variations (unsigned feature_index,
                              const Feature &feature,
                              hb_sorted_array_t<const unsigned> conditions,
                              hb_set_t *lookup_indexes) const
  {
    ために (const LookupVariationRecord &record : get_lookup_variation_records ())
    {
      もし (record.featureIndex != feature_index) continue;

      record.collect_lookups (this, feature, conditions, lookup_indexes);
      戻る true;
    }
    戻る false;
  }

  void collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) const
  {
    unsigned 整数 count = varRecords.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, this);
      もし (c->universal)
        break;
    }
    もし (c->universal || c->record_cond_idx_map->is_empty ())
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
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      もし (feature_record_cond_idx_map &&
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
    ために (unsigned i = 0; i < varRecords.len; i++)
    {
      もし (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        continue;
      varRecords[i].collect_name_ids (this, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  void collect_lookup_variation_lookups (const hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) const
  {
    ために (const LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (this, feature_indexes, lookup_indexes);
  }

  void closure_features (const hb_map_t *lookup_indexes,
			 const hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) const
  {
    unsigned 整数 count = varRecords.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      もし (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        continue;
      varRecords[i].closure_features (this, lookup_indexes, feature_indexes);
    }

    ために (const LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (this, lookup_indexes, feature_indexes);
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c) const
  {
    ために (const FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, this);
    ために (const LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, this);
  }

  void collect_feature_indices (hb_set_t *feature_indices) const
  {
    ために (const FeatureVariationRecord &record : varRecords)
      (this+record.substitutions).collect_feature_indices (feature_indices);
  }

  bool has_lookup_variations (unsigned feature_index) const
  {
    ために (const LookupVariationRecord &record : get_lookup_variation_records ())
    {
      もし (record.featureIndex == feature_index)
	戻る true;
      もし (record.featureIndex > feature_index)
	break;
    }
    戻る false;
  }

  bool subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) const
  {
    TRACE_SUBSET (this);
    auto *out = c->serializer->start_embed (*this);
    もし (unlikely (!out || !c->serializer->extend_min (out))) return_trace (false);

    out->version.major = version.major;
    out->version.minor = version.minor;

    整数 keep_up_to = -1;
    ために (整数 i = varRecords.len - 1; i >= 0; i--) {
      もし (varRecords[i].intersects_features (this, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        break;
      }
    }

    unsigned count = (unsigned) (keep_up_to + 1);
    ために (unsigned i = 0; i < count; i++)
    {
      もし (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        continue;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), this) (varRecords[i]);
    }

    もし (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      bool insert_catch_all_record = true;
      subset_record_array (l, &(out->varRecords), this, insert_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    もし (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      もし (unlikely (!c->serializer->extend_min (out_lookup_records)))
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

  protected:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  public:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

struct HintingDevice
{
  friend struct Device;

  private:

  hb_position_t get_x_delta (hb_font_t *font) const
  { 戻る get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) const
  { 戻る get_delta (font->y_ppem, font->y_scale); }

  public:

  size_t get_size () const
  {
    unsigned 整数 f = deltaFormat;
    もし (unlikely (f < 1 || f > 3 || startSize > endSize)) 戻る 3 * HBUINT16::static_size;
    戻る HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
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

  整数 get_delta (unsigned 整数 ppem, 整数 scale) const
  {
    もし (!ppem) 戻る 0;

    整数 pixels = get_delta_pixels (ppem);

    もし (!pixels) 戻る 0;

    戻る (整数) (pixels * (int64_t) scale / ppem);
  }
  整数 get_delta_pixels (unsigned 整数 ppem_size) const
  {
    unsigned 整数 f = deltaFormat;
    もし (unlikely (f < 1 || f > 3))
      戻る 0;

    もし (ppem_size < startSize || ppem_size > endSize)
      戻る 0;

    unsigned 整数 s = ppem_size - startSize;

    unsigned 整数 byte = deltaValueZ[s >> (4 - f)];
    unsigned 整数 bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned 整数 mask = (0xFFFFu >> (16 - (1 << f)));

    整数 delta = bits & mask;

    もし ((unsigned 整数) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    戻る delta;
  }

  protected:
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

struct VariationDevice
{
  friend struct Device;

  private:

  hb_position_t get_x_delta (hb_font_t *font,
			     const ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) const
  { 戻る !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     const ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) const
  { 戻る !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  float get_x_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) const
  { 戻る !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  float get_y_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) const
  { 戻る !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 整数>> *layout_variation_idx_delta_map) const
  {
    TRACE_SERIALIZE (this);
    もし (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, 整数> *v;
    もし (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    もし (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (this->static_size);
    auto *out = c->embed (this);
    もし (unlikely (!out)) return_trace (nullptr);

    もし (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
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
    戻る store.get_delta (varIdx, font->coords, font->num_coords, store_cache);
  }

  protected:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  public:
  DEFINE_SIZE_STATIC (6);
};

struct DeviceHeader
{
  protected:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  public:
  HBUINT16		format;		/* Format identifier */
  public:
  DEFINE_SIZE_STATIC (6);
};

struct Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  float get_x_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) const
  {
#ifndef HB_NO_VAR
    もし (u.b.format == 0x8000)
    {
      hb_barrier ();
      戻る u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    戻る get_x_delta (font, store, store_cache);
  }

  float get_y_delta_f (hb_font_t *font,
		       const ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) const
  {
#ifndef HB_NO_VAR
    もし (u.b.format == 0x8000)
    {
      hb_barrier ();
      戻る u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    戻る get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     const ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) const
  {
    switch (u.b.format)
    {
#ifndef HB_NO_HINTING
    case 1: case 2: case 3:
      hb_barrier ();
      戻る u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    case 0x8000:
      hb_barrier ();
      戻る u.variation.get_x_delta (font, store, store_cache);
#endif
    default:
      戻る 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     const ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) const
  {
    switch (u.b.format)
    {
    case 1: case 2: case 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      戻る u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    case 0x8000:
      hb_barrier ();
      戻る u.variation.get_y_delta (font, store, store_cache);
#endif
    default:
      戻る 0;
    }
  }

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    もし (!u.b.format.sanitize (c)) return_trace (false);
    switch (u.b.format) {
#ifndef HB_NO_HINTING
    case 1: case 2: case 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    case 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    default:
      return_trace (true);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                const hb_hashmap_t<unsigned, hb_pair_t<unsigned, 整数>> *layout_variation_idx_delta_map=nullptr) const
  {
    TRACE_SERIALIZE (this);
    switch (u.b.format) {
#ifndef HB_NO_HINTING
    case 1:
    case 2:
    case 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    case 0x8000:
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
    case 1:
    case 2:
    case 3:
      戻る;
#endif
#ifndef HB_NO_VAR
    case 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      戻る;
#endif
    default:
      戻る;
    }
  }

  unsigned get_variation_index () const
  {
    switch (u.b.format) {
#ifndef HB_NO_VAR
    case 0x8000:
      hb_barrier ();
      戻る u.variation.varIdx;
#endif
    default:
      戻る HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  bool is_variation_device () const
  {
    switch (u.b.format) {
#ifndef HB_NO_VAR
    case 0x8000:
      戻る true;
#endif
    default:
      戻る false;
    }
  }

  protected:
  union {
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

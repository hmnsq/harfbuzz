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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-ot-layout.hh"
#અન્તર્ભાવ "hb-open-type.hh"
#અન્તર્ભાવ "hb-set.hh"
#અન્તર્ભાવ "hb-bimap.hh"
#અન્તર્ભાવ "hb-cache.hh"
#અન્તર્ભાવ "hb-ot-dual.hh"

using hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (કદ (hb_ot_layout_mapping_cache_t) == 512, "");

using hb_ot_layout_દ્વિઅંકી_cache_t = hb_cache_t<14, 1, 8>;
static_assert (કદ (hb_ot_layout_દ્વિઅંકી_cache_t) == 256, "");

#અન્તર્ભાવ "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
using OT::Layout::Common::Coverage;
using OT::Layout::Common::RangeRecord;
using OT::Layout::SmallTypes;
using OT::Layout::MediumTypes;


નામાકાશ OT {

template<typename Iterator>
સ્થિર અંતઃરેખ તાર્કિક ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

સ્થિર તાર્કિક ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    અચલ hb_set_t &klasses,
    તાર્કિક use_class_zero,
    hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

સંરચના ItemVarStoreInstancer;

સંરચના hb_collect_feature_substitutes_with_var_context_t
{
  અચલ hb_map_t *axes_index_tag_map;
  અચલ hb_hashmap_t<hb_tag_t, Triple> *axes_location;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *record_cond_idx_map;
  hb_hashmap_t<unsigned, અચલ Feature*> *feature_substitutes_map;
  hb_set_t& catch_all_record_feature_idxes;

  // not stored in subset_plan
  hb_set_t *feature_indices;
  તાર્કિક apply;
  તાર્કિક variation_applied;
  તાર્કિક universal;
  unsigned cur_record_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, unsigned> *conditionset_map;
  ItemVarStoreInstancer *instancer;
  તાર્કિક all_axes_pinned;
};

સંરચના hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (અચલ શૂન્ય         *table_,
                              hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map_,
                              અચલ hb_map_t     *duplicate_feature_map_,
                              hb_set_t           *new_collected_feature_indexes_)
      :table (table_),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_ગણતરી (0),langsys_feature_ગણતરી (0) {}

  તાર્કિક visitScript ()
  { પાછા script_ગણતરી++ < HB_MAX_SCRIPTS; }

  તાર્કિક visitLangsys (unsigned feature_ગણતરી_2)
  {
    langsys_feature_ગણતરી += feature_ગણતરી_2;
    પાછા langsys_feature_ગણતરી < HB_MAX_LANGSYS_FEATURE_ગણતરી;
  }

  જાહેર:
  અચલ શૂન્ય *table;
  hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  અચલ hb_map_t     *duplicate_feature_map;
  hb_set_t           *new_feature_indexes;

  ખાનગી:
  unsigned script_ગણતરી;
  unsigned langsys_feature_ગણતરી;
};

સંરચના hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_empty_t, HB_DEBUG_SUBSET>
{
  અચલ અક્ષર *get_name () { પાછા "SUBSET_LAYOUT"; }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  તાર્કિક visitScript ()
  {
    પાછા script_ગણતરી++ < HB_MAX_SCRIPTS;
  }

  તાર્કિક visitLangSys ()
  {
    પાછા langsys_ગણતરી++ < HB_MAX_LANGSYS;
  }

  તાર્કિક visitFeatureIndex (પૂર્ણાંક ગણતરી)
  {
    feature_index_ગણતરી += ગણતરી;
    પાછા feature_index_ગણતરી < HB_MAX_FEATURE_INDICES;
  }

  તાર્કિક visitLookupIndex()
  {
    lookup_index_ગણતરી++;
    પાછા lookup_index_ગણતરી < HB_MAX_LOOKUP_VISIT_ગણતરી;
  }

  hb_subset_context_t *subset_context;
  અચલ hb_tag_t table_tag;
  અચલ hb_map_t *lookup_index_map;
  અચલ hb_hashmap_t<unsigned, hb::unique_ptr<hb_set_t>> *script_langsys_map;
  અચલ hb_map_t *feature_index_map;
  અચલ hb_map_t *feature_map_w_duplicates;
  અચલ hb_hashmap_t<unsigned, અચલ Feature*> *feature_substitutes_map;
  અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *lookup_variations;
  hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map;
  અચલ hb_set_t *catch_all_record_feature_idxes;
  અચલ hb_hashmap_t<unsigned, hb_pair_t<અચલ શૂન્ય*, અચલ શૂન્ય*>> *feature_idx_tag_map;

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
				script_ગણતરી (0),
				langsys_ગણતરી (0),
				feature_index_ગણતરી (0),
				lookup_index_ગણતરી (0)
  {
    જો (tag_ == HB_OT_TAG_GSUB)
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
    નહીંતર
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

  ખાનગી:
  unsigned script_ગણતરી;
  unsigned langsys_ગણતરી;
  unsigned feature_index_ગણતરી;
  unsigned lookup_index_ગણતરી;
};

સંરચના ItemVariationStore;
સંરચના hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  template <typename T>
  return_t dispatch (અચલ T &obj) { obj.collect_variation_indices (આ); પાછા hb_empty_t (); }
  સ્થિર return_t default_return_કિંમત () { પાછા hb_empty_t (); }

  hb_set_t *layout_variation_indices;
  અચલ hb_set_t *glyph_set;
  અચલ hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, પૂર્ણાંક>> *condition_કિંમતો = nullptr;

  hb_collect_variation_indices_context_t (hb_set_t *layout_variation_indices_,
					  અચલ hb_set_t *glyph_set_,
					  અચલ hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_set (glyph_set_),
					gpos_lookups (gpos_lookups_) {}
};

template<typename OutputArray>
સંરચના subset_offset_array_t
{
  subset_offset_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& out_,
			 અચલ શૂન્ય *base_) : subset_context (subset_context_),
					      out (out_), base (base_) {}

  template <typename T>
  તાર્કિક operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    જો (unlikely (!o)) પાછા ખોટું;
    તાર્કિક ret = o->serialize_subset (subset_context, offset, base);
    જો (!ret)
    {
      out.કાઢો ();
      subset_context->serializer->revert (snap);
    }
    પાછા ret;
  }

  ખાનગી:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  અચલ શૂન્ય *base;
};


template<typename OutputArray, typename Arg>
સંરચના subset_offset_array_arg_t
{
  subset_offset_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& out_,
			     અચલ શૂન્ય *base_,
			     Arg &&arg_) : subset_context (subset_context_), out (out_),
					  base (base_), arg (arg_) {}

  template <typename T>
  તાર્કિક operator () (T&& offset)
  {
    auto snap = subset_context->serializer->snapshot ();
    auto *o = out.serialize_append (subset_context->serializer);
    જો (unlikely (!o)) પાછા ખોટું;
    તાર્કિક ret = o->serialize_subset (subset_context, offset, base, arg);
    જો (!ret)
    {
      out.કાઢો ();
      subset_context->serializer->revert (snap);
    }
    પાછા ret;
  }

  ખાનગી:
  hb_subset_context_t *subset_context;
  OutputArray &out;
  અચલ શૂન્ય *base;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
સંરચના
{
  template<typename OutputArray>
  subset_offset_array_t<OutputArray>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       અચલ શૂન્ય *base) અચલ
  { પાછા subset_offset_array_t<OutputArray> (subset_context, out, base); }

  /* Variant with one extra argument passed to serialize_subset */
  template<typename OutputArray, typename Arg>
  subset_offset_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_context_t *subset_context, OutputArray& out,
	       અચલ શૂન્ય *base, Arg &&arg) અચલ
  { પાછા subset_offset_array_arg_t<OutputArray, Arg> (subset_context, out, base, arg); }
}
HB_FUNCOBJ (subset_offset_array);

template<typename OutputArray>
સંરચના subset_record_array_t
{
  subset_record_array_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			 અચલ શૂન્ય *base_) : subset_layout_context (c_),
					      out (out_), base (base_) {}

  template <typename T>
  શૂન્ય
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    તાર્કિક ret = record.subset (subset_layout_context, base);
    જો (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    નહીંતર out->len++;
  }

  ખાનગી:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  અચલ શૂન્ય *base;
};

template<typename OutputArray, typename Arg>
સંરચના subset_record_array_arg_t
{
  subset_record_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* out_,
			     અચલ શૂન્ય *base_,
			     Arg &&arg_) : subset_layout_context (c_),
					   out (out_), base (base_), arg (arg_) {}

  template <typename T>
  શૂન્ય
  operator () (T&& record)
  {
    auto snap = subset_layout_context->subset_context->serializer->snapshot ();
    તાર્કિક ret = record.subset (subset_layout_context, base, arg);
    જો (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    નહીંતર out->len++;
  }

  ખાનગી:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *out;
  અચલ શૂન્ય *base;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
સંરચના
{
  template<typename OutputArray>
  subset_record_array_t<OutputArray>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
	       અચલ શૂન્ય *base) અચલ
  { પાછા subset_record_array_t<OutputArray> (c, out, base); }

  /* Variant with one extra argument passed to subset */
  template<typename OutputArray, typename Arg>
  subset_record_array_arg_t<OutputArray, Arg>
  operator () (hb_subset_layout_context_t *c, OutputArray* out,
               અચલ શૂન્ય *base, Arg &&arg) અચલ
  { પાછા subset_record_array_arg_t<OutputArray, Arg> (c, out, base, arg); }
}
HB_FUNCOBJ (subset_record_array);


template<typename OutputArray>
સંરચના serialize_math_record_array_t
{
  serialize_math_record_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& out_,
                         અચલ શૂન્ય *base_,
                         અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              out (out_), base (base_), variation_map (variation_map_) {}

  template <typename T>
  તાર્કિક operator () (T&& record)
  {
    જો (!serialize_context->copy (record, base, variation_map)) પાછા ખોટું;
    out.len++;
    પાછા સાચું;
  }

  ખાનગી:
  hb_serialize_context_t *serialize_context;
  OutputArray &out;
  અચલ શૂન્ય *base;
  અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
સંરચના
{
  template<typename OutputArray>
  serialize_math_record_array_t<OutputArray>
  operator () (hb_serialize_context_t *serialize_context, OutputArray& out,
               અચલ શૂન્ય *base,
               અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *variation_map) અચલ
  { પાછા serialize_math_record_array_t<OutputArray> (serialize_context, out, base, variation_map); }

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

સંરચના IndexArray : Array16Of<Index>
{
  તાર્કિક intersects (અચલ hb_map_t *indexes) અચલ
  { પાછા hb_any (*આ, indexes); }

  template <typename Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  શૂન્ય serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    જો (!it) પાછા;
    જો (unlikely (!c->extend_min ((*આ)))) પાછા;

    માટે (અચલ auto _ : it)
    {
      જો (!l->visitLookupIndex()) તોડો;

      Index i;
      i = _;
      c->copy (i);
      આ->len++;
    }
  }

  unsigned પૂર્ણાંક get_indexes (unsigned પૂર્ણાંક start_offset,
			    unsigned પૂર્ણાંક *ગણતરી_2 /* IN/OUT */,
			    unsigned પૂર્ણાંક *_indexes /* OUT */) અચલ
  {
    જો (ગણતરી_2 && _indexes)
    {
      + આ->as_array ().sub_array (start_offset, ગણતરી_2)
      | hb_sink (hb_array (_indexes, *ગણતરી_2))
      ;
    }
    પાછા આ->len;
  }

  શૂન્ય add_indexes_to (hb_set_t* output /* OUT */) અચલ
  {
    output->add_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
સંરચના FeatureParamsSize
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ))) return_trace (ખોટું);
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

    જો (!designSize)
      return_trace (ખોટું);
    નહીંતર જો (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     rangeStart == 0 &&
	     rangeEnd == 0)
      return_trace (સાચું);
    નહીંતર જો (designSize < rangeStart ||
	     designSize > rangeEnd ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (ખોટું);
    નહીંતર
      return_trace (સાચું);
  }

  શૂન્ય collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) અચલ
  { nameids_to_retain->add (subfamilyNameID); }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    return_trace ((તાર્કિક) c->serializer->embed (*આ));
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
  જાહેર:
  DEFINE_SIZE_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
સંરચના FeatureParamsStylisticSet
{
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (આ));
  }

  શૂન્ય collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) અચલ
  { nameids_to_retain->add (uiNameID); }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    return_trace ((તાર્કિક) c->serializer->embed (*આ));
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
  જાહેર:
  DEFINE_SIZE_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
સંરચના FeatureParamsCharacterVariants
{
  unsigned
  get_characters (unsigned start_offset, unsigned *char_ગણતરી, hb_codepoint_t *chars) અચલ
  {
    જો (char_ગણતરી && chars)
    {
      + characters.as_array ().sub_array (start_offset, char_ગણતરી)
      | hb_sink (hb_array (chars, *char_ગણતરી))
      ;
    }
    પાછા characters.len;
  }

  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (characters.len, HBUINT24::static_size, min_size); }

  શૂન્ય collect_name_ids (hb_set_t *nameids_to_retain /* OUT */) અચલ
  {
    જો (featUILableNameID) nameids_to_retain->add (featUILableNameID);
    જો (featUITooltipTextNameID) nameids_to_retain->add (featUITooltipTextNameID);
    જો (sampleTextNameID) nameids_to_retain->add (sampleTextNameID);

    જો (!firstParamUILabelNameID || !અંક_Named_Parameters || અંક_Named_Parameters >= 0x7FFF)
      પાછા;

    unsigned last_name_id = (unsigned) firstParamUILabelNameID + (unsigned) અંક_Named_Parameters - 1;
    nameids_to_retain->add_range (firstParamUILabelNameID, last_name_id);
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    return_trace ((તાર્કિક) c->serializer->embed (*આ));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
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
  HBUINT16	અંક_Named_Parameters;	/* Number of named parameters. (May
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
  જાહેર:
  DEFINE_SIZE_ARRAY (14, characters);
};

સંરચના FeatureParams
{
  તાર્કિક sanitize (hb_sanitize_context_t *c, hb_tag_t tag) અચલ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    પાછા સાચું;
#endif
    TRACE_SANITIZE (આ);
    જો (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.sanitize (c));
    જો ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.sanitize (c));
    જો ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (સાચું);
  }

  શૂન્ય collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) અચલ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    પાછા;
#endif
    જો (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      પાછા (u.size.collect_name_ids (nameids_to_retain));
    જો ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      પાછા (u.stylisticSet.collect_name_ids (nameids_to_retain));
    જો ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      પાછા (u.characterVariants.collect_name_ids (nameids_to_retain));
  }

  તાર્કિક subset (hb_subset_context_t *c, અચલ Tag* tag) અચલ
  {
#ifdef HB_NO_LAYOUT_FEATURE_PARAMS
    પાછા ખોટું;
#endif
    TRACE_SUBSET (આ);
    જો (!tag) return_trace (ખોટું);
    જો (*tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      return_trace (u.size.subset (c));
    જો ((*tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylisticSet.subset (c));
    જો ((*tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (ખોટું);
  }

#ifndef HB_NO_LAYOUT_FEATURE_PARAMS
  અચલ FeatureParamsSize& get_size_params (hb_tag_t tag) અચલ
  {
    જો (tag == HB_TAG ('s','i','z','e') && hb_barrier ())
      પાછા u.size;
    પાછા Null (FeatureParamsSize);
  }
  અચલ FeatureParamsStylisticSet& get_stylistic_set_params (hb_tag_t tag) અચલ
  {
    જો ((tag & 0xFFFF0000u) == HB_TAG ('s','s','\0','\0')) /* ssXX */
      પાછા u.stylisticSet;
    પાછા Null (FeatureParamsStylisticSet);
  }
  અચલ FeatureParamsCharacterVariants& get_character_variants_params (hb_tag_t tag) અચલ
  {
    જો ((tag & 0xFFFF0000u) == HB_TAG ('c','v','\0','\0')) /* cvXX */
      પાછા u.characterVariants;
    પાછા Null (FeatureParamsCharacterVariants);
  }
#endif

  ખાનગી:
  સંઘ {
  FeatureParamsSize			size;
  FeatureParamsStylisticSet		stylisticSet;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  જાહેર:
  DEFINE_SIZE_MIN (0);
};

સંરચના Record_sanitize_closure_t {
  hb_tag_t tag;
  અચલ શૂન્ય *list_base;
};

સંરચના Feature
{
  unsigned પૂર્ણાંક get_lookup_ગણતરી () અચલ
  { પાછા lookupIndex.len; }
  hb_tag_t get_lookup_index (unsigned પૂર્ણાંક i) અચલ
  { પાછા lookupIndex[i]; }
  unsigned પૂર્ણાંક get_lookup_indexes (unsigned પૂર્ણાંક start_index,
				   unsigned પૂર્ણાંક *lookup_ગણતરી /* IN/OUT */,
				   unsigned પૂર્ણાંક *lookup_tags /* OUT */) અચલ
  { પાછા lookupIndex.get_indexes (start_index, lookup_ગણતરી, lookup_tags); }
  શૂન્ય add_lookup_indexes_to (hb_set_t *lookup_indexes) અચલ
  { lookupIndex.add_indexes_to (lookup_indexes); }

  અચલ FeatureParams &get_feature_params () અચલ
  { પાછા આ+featureParams; }

  તાર્કિક intersects_lookup_indexes (અચલ hb_map_t *lookup_indexes) અચલ
  { પાછા lookupIndex.intersects (lookup_indexes); }

  શૂન્ય collect_name_ids (hb_tag_t tag, hb_set_t *nameids_to_retain /* OUT */) અચલ
  {
    જો (featureParams)
      get_feature_params ().collect_name_ids (tag, nameids_to_retain);
  }

  તાર્કિક subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       અચલ Tag                   *tag = nullptr) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    out->featureParams.serialize_subset (c, featureParams, આ, tag);

    hb::shared_ptr<hb_set_t> *lookup_variations;
    જો (l->cur_feature_index != Index::NOT_FOUND_INDEX &&
	l->lookup_variations->has (l->cur_feature_index, &lookup_variations))
    {
      auto it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_index_map)
      | hb_map (l->lookup_index_map)
      ;
      out->lookupIndex.serialize (c->serializer, l, it);
    }
    નહીંતર
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
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ Record_sanitize_closure_t *closure = nullptr) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  featureParams.sanitize (c, આ, closure ? closure->tag : HB_TAG_NONE) &&
		  lookupIndex.sanitize (c));
  }

  Offset16To<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  IndexArray	 lookupIndex;	/* Array of LookupList indices */
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (4, lookupIndex);
};

template <typename Type>
સંરચના Record
{
  પૂર્ણાંક cmp (hb_tag_t a) અચલ { પાછા tag.cmp (a); }

  તાર્કિક subset (hb_subset_layout_context_t *c, અચલ શૂન્ય *base, અચલ શૂન્ય *f_sub = nullptr) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->subset_context->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (!f_sub)
      return_trace (out->offset.serialize_subset (c->subset_context, offset, base, c, &tag));

    અચલ Feature& f = *reinterpret_cast<અચલ Feature *> (f_sub);
    auto *s = c->subset_context->serializer;
    s->push ();

    out->offset = 0;
    તાર્કિક ret = f.subset (c->subset_context, c, &tag);
    જો (ret)
      s->add_link (out->offset, s->કાઢો_pack ());
    નહીંતર
      s->કાઢો_discard ();

    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    અચલ Record_sanitize_closure_t closure = {tag, base};
    return_trace (c->check_struct (આ) &&
		  offset.sanitize (c, base, &closure));
  }

  Tag           tag;            /* 4-byte Tag identifier */
  Offset16To<Type>
                offset;         /* Offset from beginning of object holding
                                 * the Record */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

template <typename Type>
સંરચના RecordArrayOf : SortedArray16Of<Record<Type>>
{
  અચલ Offset16To<Type>& get_offset (unsigned પૂર્ણાંક i) અચલ
  { પાછા (*આ)[i].offset; }
  Offset16To<Type>& get_offset (unsigned પૂર્ણાંક i)
  { પાછા (*આ)[i].offset; }
  અચલ Tag& get_tag (unsigned પૂર્ણાંક i) અચલ
  { પાછા (*આ)[i].tag; }
  unsigned પૂર્ણાંક get_tags (unsigned પૂર્ણાંક start_offset,
                         unsigned પૂર્ણાંક *record_ગણતરી /* IN/OUT */,
                         hb_tag_t     *record_tags /* OUT */) અચલ
  {
    જો (record_ગણતરી && record_tags)
    {
      + આ->as_array ().sub_array (start_offset, record_ગણતરી)
      | hb_map (&Record<Type>::tag)
      | hb_sink (hb_array (record_tags, *record_ગણતરી))
      ;
    }
    પાછા આ->len;
  }
  તાર્કિક find_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  {
    પાછા આ->bfind (tag, index, HB_NOT_FOUND_STORE, Index::NOT_FOUND_INDEX);
  }
};

template <typename Type>
સંરચના RecordListOf : RecordArrayOf<Type>
{
  અચલ Type& operator [] (unsigned પૂર્ણાંક i) અચલ
  { પાછા આ+આ->get_offset (i); }

  તાર્કિક subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    + આ->iter ()
    | hb_apply (subset_record_array (l, out, આ))
    ;
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (RecordArrayOf<Type>::sanitize (c, આ));
  }
};

સંરચના RecordListOfFeature : RecordListOf<Feature>
{
  તાર્કિક subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    + hb_enumerate (*આ)
    | hb_filter (l->feature_index_map, hb_first)
    | hb_apply ([l, out, આ] (અચલ hb_pair_t<unsigned, અચલ Record<Feature>&>& _)
                {
                  અચલ Feature *f_sub = nullptr;
                  અચલ Feature **f = nullptr;
                  જો (l->feature_substitutes_map->has (_.first, &f))
                    f_sub = *f;

                  l->cur_feature_index = _.first;
                  subset_record_array (l, out, આ, f_sub) (_.second);
                  l->cur_feature_index = Index::NOT_FOUND_INDEX;
                })
    ;

    return_trace (સાચું);
  }
};

પ્રકારવ્યાખ્યા RecordListOf<Feature> FeatureList;


સંરચના LangSys
{
  unsigned પૂર્ણાંક get_feature_ગણતરી () અચલ
  { પાછા featureIndex.len; }
  hb_tag_t get_feature_index (unsigned પૂર્ણાંક i) અચલ
  { પાછા featureIndex[i]; }
  unsigned પૂર્ણાંક get_feature_indexes (unsigned પૂર્ણાંક start_offset,
				    unsigned પૂર્ણાંક *feature_ગણતરી_2 /* IN/OUT */,
				    unsigned પૂર્ણાંક *feature_indexes /* OUT */) અચલ
  { પાછા featureIndex.get_indexes (start_offset, feature_ગણતરી_2, feature_indexes); }
  શૂન્ય add_feature_indexes_to (hb_set_t *feature_indexes) અચલ
  { featureIndex.add_indexes_to (feature_indexes); }

  તાર્કિક has_required_feature () અચલ { પાછા reqFeatureIndex != 0xFFFFu; }
  unsigned પૂર્ણાંક get_required_feature_index () અચલ
  {
    જો (reqFeatureIndex == 0xFFFFu)
      પાછા Index::NOT_FOUND_INDEX;
   પાછા reqFeatureIndex;
  }

  LangSys* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (*આ));
  }

  તાર્કિક compare (અચલ LangSys& o, અચલ hb_map_t *feature_index_map) અચલ
  {
    જો (reqFeatureIndex != o.reqFeatureIndex)
      પાછા ખોટું;

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

    માટે (; iter && o_iter; iter++, o_iter++)
    {
      unsigned a = *iter;
      unsigned b = *o_iter;
      જો (a != b) પાછા ખોટું;
    }

    જો (iter || o_iter) પાછા ખોટું;

    પાછા સાચું;
  }

  શૂન્ય collect_features (hb_prune_langsys_context_t *c) અચલ
  {
    જો (!has_required_feature () && !get_feature_ગણતરી ()) પાછા;
    જો (has_required_feature () &&
        c->duplicate_feature_map->has (reqFeatureIndex))
      c->new_feature_indexes->add (get_required_feature_index ());

    + hb_iter (featureIndex)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  તાર્કિક subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       અચલ Tag                  *tag = nullptr) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    અચલ uint32_t *v;
    out->reqFeatureIndex = l->feature_map_w_duplicates->has (reqFeatureIndex, &v) ? *v : 0xFFFFu;

    જો (!l->visitFeatureIndex (featureIndex.len))
      return_trace (ખોટું);

    auto it =
    + hb_iter (featureIndex)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    તાર્કિક ret = તાર્કિક (it);
    out->featureIndex.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ Record_sanitize_closure_t * = nullptr) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && featureIndex.sanitize (c));
  }

  Offset16	lookupOrderZ;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	reqFeatureIndex;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  IndexArray	featureIndex;	/* Array of indices into the FeatureList */
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (6, featureIndex);
};
DECLARE_NULL_NAMESPACE_BYTES (OT, LangSys);

સંરચના Script
{
  unsigned પૂર્ણાંક get_lang_sys_ગણતરી () અચલ
  { પાછા langSys.len; }
  અચલ Tag& get_lang_sys_tag (unsigned પૂર્ણાંક i) અચલ
  { પાછા langSys.get_tag (i); }
  unsigned પૂર્ણાંક get_lang_sys_tags (unsigned પૂર્ણાંક start_offset,
				  unsigned પૂર્ણાંક *lang_sys_ગણતરી /* IN/OUT */,
				  hb_tag_t     *lang_sys_tags /* OUT */) અચલ
  { પાછા langSys.get_tags (start_offset, lang_sys_ગણતરી, lang_sys_tags); }
  અચલ LangSys& get_lang_sys (unsigned પૂર્ણાંક i) અચલ
  {
    જો (i == Index::NOT_FOUND_INDEX) પાછા get_default_lang_sys ();
    પાછા આ+langSys[i].offset;
  }
  તાર્કિક find_lang_sys_index (hb_tag_t tag, unsigned પૂર્ણાંક *index) અચલ
  { પાછા langSys.find_index (tag, index); }

  તાર્કિક has_default_lang_sys () અચલ           { પાછા defaultLangSys != 0; }
  અચલ LangSys& get_default_lang_sys () અચલ { પાછા આ+defaultLangSys; }

  શૂન્ય prune_langsys (hb_prune_langsys_context_t *c,
                      unsigned script_index) અચલ
  {
    જો (!has_default_lang_sys () && !get_lang_sys_ગણતરી ()) પાછા;
    જો (!c->visitScript ()) પાછા;

    જો (!c->script_langsys_map->has (script_index))
    {
      જો (unlikely (!c->script_langsys_map->set (script_index, hb::unique_ptr<hb_set_t> {hb_set_create ()})))
	પાછા;
    }

    જો (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      અચલ LangSys& d = get_default_lang_sys ();
      જો (c->visitLangsys (d.get_feature_ગણતરી ())) {
        d.collect_features (c);
      }

      માટે (auto _ : + hb_enumerate (langSys))
      {
        અચલ LangSys& l = આ+_.second.offset;
        જો (!c->visitLangsys (l.get_feature_ગણતરી ())) ચાલુ;
        જો (l.compare (d, c->duplicate_feature_map)) ચાલુ;

        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
    નહીંતર
    {
      માટે (auto _ : + hb_enumerate (langSys))
      {
        અચલ LangSys& l = આ+_.second.offset;
        જો (!c->visitLangsys (l.get_feature_ગણતરી ())) ચાલુ;
        l.collect_features (c);
        c->script_langsys_map->get (script_index)->add (_.first);
      }
    }
  }

  તાર્કિક subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       અચલ Tag                   *tag) અચલ
  {
    TRACE_SUBSET (આ);
    જો (!l->visitScript ()) return_trace (ખોટું);
    જો (tag && !c->plan->layout_scripts.has (*tag))
      પાછા ખોટું;

    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    તાર્કિક defaultLang = ખોટું;
    જો (has_default_lang_sys ())
    {
      c->serializer->push ();
      અચલ LangSys& ls = આ+defaultLangSys;
      તાર્કિક ret = ls.subset (c, l);
      જો (!ret && tag && *tag != HB_TAG ('D', 'F', 'L', 'T'))
      {
	c->serializer->કાઢો_discard ();
	out->defaultLangSys = 0;
      }
      નહીંતર
      {
	c->serializer->add_link (out->defaultLangSys, c->serializer->કાઢો_pack ());
	defaultLang = સાચું;
      }
    }

    અચલ hb_set_t *active_langsys = l->script_langsys_map->get (l->cur_script_index);
    જો (active_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (active_langsys, hb_first)
      | hb_map (hb_second)
      | hb_filter ([=] (અચલ Record<LangSys>& record) {પાછા l->visitLangSys (); })
      | hb_apply (subset_record_array (l, &(out->langSys), આ))
      ;
    }

    return_trace (તાર્કિક (out->langSys.len) || defaultLang || l->table_tag == HB_OT_TAG_GSUB);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c,
		 અચલ Record_sanitize_closure_t * = nullptr) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (defaultLangSys.sanitize (c, આ) && langSys.sanitize (c, આ));
  }

  સુરક્ષિત:
  Offset16To<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  RecordArrayOf<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (4, langSys);
};

સંરચના RecordListOfScript : RecordListOf<Script>
{
  તાર્કિક subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    માટે (auto _ : + hb_enumerate (*આ))
    {
      auto snap = c->serializer->snapshot ();
      l->cur_script_index = _.first;
      તાર્કિક ret = _.second.subset (l, આ);
      જો (!ret) c->serializer->revert (snap);
      નહીંતર out->len++;
    }

    return_trace (સાચું);
  }
};

પ્રકારવ્યાખ્યા RecordListOfScript ScriptList;



સંરચના LookupFlag : HBUINT16
{
  ગણના Flags {
    જમણું_To_ડાબું		= 0x0001u,
    IgnoreBaseGlyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    UseMarkFilteringSet	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  જાહેર:
  DEFINE_SIZE_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
નામાકાશ OT {

સંરચના Lookup
{
  unsigned પૂર્ણાંક get_subtable_ગણતરી () અચલ { પાછા subTable.len; }

  template <typename TSubTable>
  અચલ Array16OfOffset16To<TSubTable>& get_subtables () અચલ
  { પાછા reinterpret_cast<અચલ Array16OfOffset16To<TSubTable> &> (subTable); }
  template <typename TSubTable>
  Array16OfOffset16To<TSubTable>& get_subtables ()
  { પાછા reinterpret_cast<Array16OfOffset16To<TSubTable> &> (subTable); }

  template <typename TSubTable>
  અચલ TSubTable& get_subtable (unsigned પૂર્ણાંક i) અચલ
  { પાછા આ+get_subtables<TSubTable> ()[i]; }
  template <typename TSubTable>
  TSubTable& get_subtable (unsigned પૂર્ણાંક i)
  { પાછા આ+get_subtables<TSubTable> ()[i]; }

  size_t get_size () અચલ
  {
    અચલ HBUINT16 &markFilteringSet = StructAfter<અચલ HBUINT16> (subTable);
    જો (lookupFlag & LookupFlag::UseMarkFilteringSet)
      પાછા (અચલ અક્ષર *) &StructAfter<અચલ અક્ષર> (markFilteringSet) - (અચલ અક્ષર *) આ;
    પાછા (અચલ અક્ષર *) &markFilteringSet - (અચલ અક્ષર *) આ;
  }

  unsigned પૂર્ણાંક get_type () અચલ { પાછા lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () અચલ
  {
    unsigned પૂર્ણાંક flag = lookupFlag;
    જો (unlikely (flag & LookupFlag::UseMarkFilteringSet))
    {
      અચલ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      flag += (markFilteringSet << 16);
    }
    પાછા flag;
  }

  template <typename TSubTable, typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    unsigned પૂર્ણાંક lookup_type = get_type ();
    TRACE_DISPATCH (આ, lookup_type);
    unsigned પૂર્ણાંક ગણતરી = get_subtable_ગણતરી ();
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++) {
      typename context_t::return_t r = get_subtable<TSubTable> (i).dispatch (c, lookup_type, ds...);
      જો (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_કિંમત ());
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  unsigned પૂર્ણાંક lookup_type,
		  uint32_t lookup_props,
		  unsigned પૂર્ણાંક અંક_subtables)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    જો (unlikely (!subTable.serialize (c, અંક_subtables))) return_trace (ખોટું);
    જો (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      જો (unlikely (!c->extend (આ))) return_trace (ખોટું);
      HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      markFilteringSet = lookup_props >> 16;
    }
    return_trace (સાચું);
  }

  template <typename TSubTable>
  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);
    out->lookupType = lookupType;
    out->lookupFlag = lookupFlag;

    અચલ hb_set_t *glyphset = c->plan->glyphset_gsub ();
    unsigned પૂર્ણાંક lookup_type = get_type ();
    + hb_iter (get_subtables <TSubTable> ())
    | hb_filter ([આ, glyphset, lookup_type] (અચલ Offset16To<TSubTable> &_) { પાછા (આ+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_offset_array (c, out->get_subtables<TSubTable> (), આ, lookup_type))
    ;

    જો (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      અચલ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      hb_codepoint_t *idx;
      જો (!c->plan->used_mark_sets_map.has (markFilteringSet, &idx))
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
      નહીંતર
      {
        જો (unlikely (!c->serializer->extend (out))) return_trace (ખોટું);
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
    return_trace (સાચું);
  }

  template <typename TSubTable>
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!(c->check_struct (આ) && subTable.sanitize (c))) return_trace (ખોટું);
    hb_barrier ();

    unsigned subtables = get_subtable_ગણતરી ();
    જો (unlikely (!c->visit_subtables (subtables))) return_trace (ખોટું);

    જો (lookupFlag & LookupFlag::UseMarkFilteringSet)
    {
      અચલ HBUINT16 &markFilteringSet = StructAfter<HBUINT16> (subTable);
      જો (!markFilteringSet.sanitize (c)) return_trace (ખોટું);
    }

    જો (unlikely (!get_subtables<TSubTable> ().sanitize (c, આ, get_type ())))
      return_trace (ખોટું);

    જો (unlikely (get_type () == TSubTable::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      unsigned પૂર્ણાંક type = get_subtable<TSubTable> (0).u.extension.get_type ();
      માટે (unsigned પૂર્ણાંક i = 1; i < subtables; i++)
	જો (get_subtable<TSubTable> (i).u.extension.get_type () != type)
	  return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  સુરક્ષિત:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16Of<Offset16>
		subTable;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  જાહેર:
  DEFINE_SIZE_ARRAY (6, subTable);
};

template <typename Types>
using LookupList = List16OfOffsetTo<Lookup, typename Types::HBLUINT>;

template <typename TLookup, typename OffsetType>
સંરચના LookupOffsetList : List16OfOffsetTo<TLookup, OffsetType>
{
  તાર્કિક subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    + hb_enumerate (*આ)
    | hb_filter (l->lookup_index_map, hb_first)
    | hb_map (hb_second)
    | hb_apply (subset_offset_array (c, *out, આ))
    ;
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace ((List16OfOffsetTo<TLookup, OffsetType>::sanitize (c, આ)));
  }
};


/*
 * Coverage Table
 */


સ્થિર તાર્કિક ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  અચલ hb_set_t &klasses,
                                          તાર્કિક use_class_zero,
                                          hb_sorted_vector_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  જો (!klass_map)
    પાછા ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  જો (!use_class_zero)
    klass_map->set (0, 0);

  unsigned idx = klass_map->has (0) ? 1 : 0;
  માટે (અચલ unsigned k: klasses)
  {
    જો (klass_map->has (k)) ચાલુ;
    klass_map->set (k, idx);
    idx++;
  }


  જો (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    પાછા ખોટું;

  માટે (unsigned i = 0; i < glyph_and_klass.length; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  પાછા ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

template <typename Types>
સંરચના ClassDefFormat1_3
{
  friend સંરચના ClassDef;

  ખાનગી:
  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id) અચલ
  {
    પાછા class_કિંમત[(unsigned પૂર્ણાંક) (glyph_id - startGlyph)];
  }

  unsigned get_population () અચલ
  {
    પાછા class_કિંમત.len;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    જો (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 1 : 3;
      startGlyph = 0;
      class_કિંમત.len = 0;
      return_trace (સાચું);
    }

    hb_codepoint_t glyph_min = (*it).first;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_first)
			       | hb_reduce (hb_max, 0u);
    unsigned glyph_ગણતરી = glyph_max - glyph_min + 1;

    startGlyph = glyph_min;
    જો (unlikely (!class_કિંમત.serialize (c, glyph_ગણતરી))) return_trace (ખોટું);
    માટે (અચલ hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      unsigned idx = gid_klass_pair.first - glyph_min;
      class_કિંમત[idx] = gid_klass_pair.second;
    }
    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               તાર્કિક keep_empty_table = સાચું,
               તાર્કિક use_class_zero = સાચું,
               અચલ Coverage* glyph_filter = nullptr) અચલ
  {
    TRACE_SUBSET (આ);

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end   = start + class_કિંમત.len;

    માટે (અચલ hb_codepoint_t gid : + hb_range (start, end))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      જો (new_gid == HB_MAP_કિંમત_INVALID) ચાલુ;
      જો (glyph_filter && !glyph_filter->has(gid)) ચાલુ;

      unsigned klass = class_કિંમત[gid - start];
      જો (!klass) ચાલુ;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.add (klass);
    }

    જો (use_class_zero)
    {
      અચલ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
      unsigned glyph_ગણતરી = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_ગણતરી <= glyph_and_klass.length;
    }
    જો (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ખોટું);
    return_trace (keep_empty_table || (તાર્કિક) glyph_and_klass);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && class_કિંમત.sanitize (c));
  }

  unsigned cost () અચલ { પાછા 1; }

  template <typename set_t>
  તાર્કિક collect_coverage (set_t *glyphs) અચલ
  {
    unsigned પૂર્ણાંક start = 0;
    unsigned પૂર્ણાંક ગણતરી = class_કિંમત.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (class_કિંમત[i])
	ચાલુ;

      જો (start != i)
	જો (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + i)))
	  પાછા ખોટું;

      start = i + 1;
    }
    જો (start != ગણતરી)
      જો (unlikely (!glyphs->add_range (startGlyph + start, startGlyph + ગણતરી)))
	પાછા ખોટું;

    પાછા સાચું;
  }

  template <typename set_t>
  તાર્કિક collect_class (set_t *glyphs, unsigned klass) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = class_કિંમત.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (class_કિંમત[i] == klass) glyphs->add (startGlyph + i);
    પાછા સાચું;
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    hb_codepoint_t start = startGlyph;
    hb_codepoint_t end = startGlyph + class_કિંમત.len;
    માટે (hb_codepoint_t iter = startGlyph - 1;
	 glyphs->next (&iter) && iter < end;)
      જો (class_કિંમત[iter - start]) પાછા સાચું;
    પાછા ખોટું;
  }
  તાર્કિક intersects_class (અચલ hb_set_t *glyphs, unsigned klass) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = class_કિંમત.len;
    જો (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_કિંમત_INVALID;
      જો (!glyphs->next (&g)) પાછા ખોટું;
      જો (g < startGlyph) પાછા સાચું;
      g = startGlyph + ગણતરી - 1;
      જો (glyphs->next (&g)) પાછા સાચું;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    અચલ typename Types::HBUINT *arr = class_કિંમત.arrayZ;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (arr[i] == klass && glyphs->has (startGlyph + i))
	પાછા સાચું;
    પાછા ખોટું;
  }

  શૂન્ય intersected_class_glyphs (અચલ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) અચલ
  {
    unsigned ગણતરી = class_કિંમત.len;
    જો (klass == 0)
    {
      unsigned start_glyph = startGlyph;
      માટે (uint32_t g = HB_SET_કિંમત_INVALID;
	   glyphs->next (&g) && g < start_glyph;)
	intersect_glyphs->add (g);

      માટે (uint32_t g = startGlyph + ગણતરી - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->add (g);

      પાછા;
    }

    માટે (unsigned i = 0; i < ગણતરી; i++)
      જો (class_કિંમત[i] == klass && glyphs->has (startGlyph + i))
	intersect_glyphs->add (startGlyph + i);

#જો 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    unsigned start_glyph = startGlyph;
    unsigned end_glyph = start_glyph + ગણતરી;
    માટે (unsigned g = startGlyph - 1;
	 glyphs->next (&g) && g < end_glyph;)
      જો (class_કિંમત.arrayZ[g - start_glyph] == klass)
        intersect_glyphs->add (g);
#endif
  }

  શૂન્ય intersected_classes (અચલ hb_set_t *glyphs, hb_set_t *intersect_classes) અચલ
  {
    જો (glyphs->is_empty ()) પાછા;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    જો (!class_કિંમત.len)
    {
      intersect_classes->add (0);
      પાછા;
    }

    hb_codepoint_t end_glyph = startGlyph + class_કિંમત.len - 1;
    જો (glyphs->get_min () < startGlyph ||
        glyphs->get_max () > end_glyph)
      intersect_classes->add (0);

    માટે (અચલ auto& _ : + hb_enumerate (class_કિંમત))
    {
      hb_codepoint_t g = startGlyph + _.first;
      જો (glyphs->has (g))
        intersect_classes->add (_.second);
    }
  }

  સુરક્ષિત:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  typename Types::HBGlyphID
		 startGlyph;	/* First GlyphID of the classValueArray */
  typename Types::template ArrayOf<typename Types::HBUINT>
		class_કિંમત;	/* Array of Class Values--one per GlyphID */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + 2 * Types::size, class_કિંમત);
};

template <typename Types>
સંરચના ClassDefFormat2_4
{
  friend સંરચના ClassDef;

  ખાનગી:
  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id) અચલ
  {
    પાછા rangeRecord.bsearch (glyph_id).value;
  }

  unsigned get_population () અચલ
  {
    typename Types::large_int ret = 0;
    માટે (અચલ auto &r : rangeRecord)
      ret += r.get_population ();
    પાછા ret > UINT_MAX ? UINT_MAX : (unsigned) ret;
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    જો (unlikely (!it))
    {
      classFormat = Types::size == 2 ? 2 : 4;
      rangeRecord.len = 0;
      return_trace (સાચું);
    }

    unsigned unsorted = ખોટું;
    unsigned અંક_ranges = 1;
    hb_codepoint_t prev_gid = (*it).first;
    unsigned prev_klass = (*it).second;

    RangeRecord<Types> range_rec;
    range_rec.first = prev_gid;
    range_rec.last = prev_gid;
    range_rec.value = prev_klass;

    auto *record = c->copy (range_rec);
    જો (unlikely (!record)) return_trace (ખોટું);

    માટે (અચલ auto gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.first;
      unsigned cur_klass = gid_klass_pair.second;

      જો (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	જો (unlikely (cur_gid < prev_gid))
	  unsorted = સાચું;

	જો (unlikely (!record)) તોડો;
	record->last = prev_gid;
	અંક_ranges++;

	range_rec.first = cur_gid;
	range_rec.last = cur_gid;
	range_rec.value = cur_klass;

	record = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    જો (unlikely (c->in_error ())) return_trace (ખોટું);

    જો (likely (record)) record->last = prev_gid;
    rangeRecord.len = અંક_ranges;

    જો (unlikely (unsorted))
      rangeRecord.as_array ().qsort (RangeRecord<Types>::cmp_range);

    return_trace (સાચું);
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               તાર્કિક keep_empty_table = સાચું,
               તાર્કિક use_class_zero = સાચું,
               અચલ Coverage* glyph_filter = nullptr) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyph_set = *c->plan->glyphset_gsub ();

    hb_sorted_vector_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_set_t orig_klasses;

    જો (glyph_set.get_population () * hb_bit_storage ((unsigned) rangeRecord.len)
	< get_population ())
    {
      માટે (hb_codepoint_t g : glyph_set)
      {
	unsigned klass = get_class (g);
	જો (!klass) ચાલુ;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	જો (new_gid == HB_MAP_કિંમત_INVALID) ચાલુ;
	જો (glyph_filter && !glyph_filter->has (g)) ચાલુ;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.add (klass);
      }
    }
    નહીંતર
    {
      unsigned અંક_source_glyphs = c->plan->source->get_અંક_glyphs ();
      માટે (auto &range : rangeRecord)
      {
	unsigned klass = range.value;
	જો (!klass) ચાલુ;
	hb_codepoint_t start = range.first;
	hb_codepoint_t end   = hb_min (range.last + 1, અંક_source_glyphs);
	માટે (hb_codepoint_t g = start; g < end; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  જો (new_gid == HB_MAP_કિંમત_INVALID) ચાલુ;
	  જો (glyph_filter && !glyph_filter->has (g)) ચાલુ;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.add (klass);
	}
      }
    }

    અચલ hb_set_t& glyphset = *c->plan->glyphset_gsub ();
    unsigned glyph_ગણતરી = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_ગણતરી <= glyph_and_klass.length;
    જો (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (ખોટું);
    return_trace (keep_empty_table || (તાર્કિક) glyph_and_klass);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (rangeRecord.sanitize (c));
  }

  unsigned cost () અચલ { પાછા hb_bit_storage ((unsigned) rangeRecord.len); /* bsearch cost */ }

  template <typename set_t>
  તાર્કિક collect_coverage (set_t *glyphs) અચલ
  {
    માટે (auto &range : rangeRecord)
      જો (range.value)
	જો (unlikely (!range.collect_coverage (glyphs)))
	  પાછા ખોટું;
    પાછા સાચું;
  }

  template <typename set_t>
  તાર્કિક collect_class (set_t *glyphs, unsigned પૂર્ણાંક klass) અચલ
  {
    માટે (auto &range : rangeRecord)
    {
      જો (range.value == klass)
	જો (unlikely (!range.collect_coverage (glyphs)))
	  પાછા ખોટું;
    }
    પાછા સાચું;
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    જો (rangeRecord.len > glyphs->get_population () * hb_bit_storage ((unsigned) rangeRecord.len))
    {
      માટે (auto g : *glyphs)
        જો (get_class (g))
	  પાછા સાચું;
      પાછા ખોટું;
    }

    પાછા hb_any (+ hb_iter (rangeRecord)
                   | hb_map ([glyphs] (અચલ RangeRecord<Types> &range) { પાછા range.intersects (*glyphs) && range.value; }));
  }
  તાર્કિક intersects_class (અચલ hb_set_t *glyphs, unsigned klass) અચલ
  {
    જો (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      જો (!rangeRecord.len)
        પાછા !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_SET_કિંમત_INVALID;
      hb_codepoint_t last = HB_SET_કિંમત_INVALID;
      auto it = hb_iter (rangeRecord);
      માટે (auto &range : it)
      {
        જો (it->first == last + 1)
	{
	  it++;
	  ચાલુ;
	}

	જો (!glyphs->next (&g))
	  તોડો;
	જો (g < range.first)
	  પાછા સાચું;
	g = range.last;
	last = g;
      }
      જો (g != HB_SET_કિંમત_INVALID && glyphs->next (&g))
	પાછા સાચું;
      /* Fall through. */
    }
    માટે (અચલ auto &range : rangeRecord)
      જો (range.value == klass && range.intersects (*glyphs))
	પાછા સાચું;
    પાછા ખોટું;
  }

  શૂન્ય intersected_class_glyphs (અચલ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) અચલ
  {
    જો (klass == 0)
    {
      hb_codepoint_t g = HB_SET_કિંમત_INVALID;
      માટે (auto &range : rangeRecord)
      {
	જો (!glyphs->next (&g))
	  જાઓ done;
	જ્યારે (g < range.first)
	{
	  intersect_glyphs->add (g);
	  જો (!glyphs->next (&g))
	    જાઓ done;
        }
        g = range.last;
      }
      જ્યારે (glyphs->next (&g))
	intersect_glyphs->add (g);
      done:

      પાછા;
    }

    unsigned ગણતરી = rangeRecord.len;
    જો (ગણતરી > glyphs->get_population () * hb_bit_storage (ગણતરી))
    {
      માટે (auto g : *glyphs)
      {
        unsigned i;
        જો (rangeRecord.as_array ().bfind (g, &i) &&
	    rangeRecord.arrayZ[i].value == klass)
	  intersect_glyphs->add (g);
      }
      પાછા;
    }

    માટે (auto &range : rangeRecord)
    {
      જો (range.value != klass) ચાલુ;

      unsigned end = range.last + 1;
      માટે (hb_codepoint_t g = range.first - 1;
	   glyphs->next (&g) && g < end;)
	intersect_glyphs->add (g);
    }
  }

  શૂન્ય intersected_classes (અચલ hb_set_t *glyphs, hb_set_t *intersect_classes) અચલ
  {
    જો (glyphs->is_empty ()) પાછા;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    જો (!rangeRecord.len)
    {
      intersect_classes->add (0);
      પાછા;
    }

    hb_codepoint_t g = HB_SET_કિંમત_INVALID;
    માટે (auto &range : rangeRecord)
    {
      જો (!glyphs->next (&g))
        તોડો;
      જો (g < range.first)
      {
        intersect_classes->add (0);
        તોડો;
      }
      g = range.last;
    }
    જો (g != HB_SET_કિંમત_INVALID && glyphs->next (&g))
      intersect_classes->add (0);

    માટે (અચલ auto& range : rangeRecord)
      જો (range.intersects (*glyphs))
        intersect_classes->add (range.value);
  }

  સુરક્ષિત:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  typename Types::template SortedArrayOf<RangeRecord<Types>>
		rangeRecord;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  જાહેર:
  DEFINE_SIZE_ARRAY (2 + Types::size, rangeRecord);
};

સંરચના ClassDef
{
  /* Has interface. */
  unsigned operator [] (hb_codepoint_t k) અચલ { પાછા get (k); }
  તાર્કિક has (hb_codepoint_t k) અચલ { પાછા (*આ)[k]; }
  /* Projection. */
  hb_codepoint_t operator () (hb_codepoint_t k) અચલ { પાછા get (k); }

  unsigned પૂર્ણાંક get (hb_codepoint_t k) અચલ { પાછા get_class (k); }
  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_class (glyph_id);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_class (glyph_id);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_class (glyph_id);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_class (glyph_id);
#endif
    મૂળભૂત:પાછા 0;
    }
  }
  unsigned પૂર્ણાંક get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) અચલ
  {
    unsigned klass;
    જો (cache && cache->get (glyph_id, &klass)) પાછા klass;
    klass = get_class (glyph_id);
    જો (cache) cache->set (glyph_id, klass);
    પાછા klass;
  }

  unsigned get_population () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_population ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.get_population ();
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.get_population ();
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.get_population ();
#endif
    મૂળભૂત:પાછા NOT_COVERED;
    }
  }

  template<typename Iterator,
	   hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    auto glyphs = + it_with_class_zero | hb_filter (hb_second);

    unsigned format = 2;
    hb_codepoint_t glyph_max = 0;
    unsigned class_max = 0;
    જો (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).first;
      glyph_max = glyph_min;

      unsigned અંક_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      unsigned prev_klass = (*glyphs).second;

      માટે (અચલ auto gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.first;
	unsigned cur_klass = gid_klass_pair.second;
	જો (cur_klass > class_max) class_max = cur_klass;
	જો (cur_gid == glyph_min) ચાલુ;
        જો (cur_gid > glyph_max) glyph_max = cur_gid;
	જો (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  અંક_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t best_size = UINT64_MAX;
      unsigned glyph_span = glyph_max - glyph_min + 1;
      auto consider = [&] (unsigned candidate, uint64_t size)
      {
	જો (size >= best_size) પાછા;
	best_size = size;
	format = candidate;
      };

      જો (glyph_max <= 0xFFFFu && glyph_span <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_size * glyph_span);
      જો (glyph_max <= 0xFFFFu && અંક_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) અંક_ranges *
		     (2 * HBGlyphID16::static_size + HBUINT16::static_size));
#ifndef HB_NO_BEYOND_64K
      જો (glyph_max <= 0xFFFFFFu && glyph_span <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_size * glyph_span);
      જો (glyph_max <= 0xFFFFFFu && અંક_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) અંક_ranges *
		     (2 * HBGlyphID24::static_size + HBUINT16::static_size));
#endif
      જો (unlikely (best_size == UINT64_MAX))
      {
	c->check_success (ખોટું, HB_SERIALIZE_ERROR_INT_OVERFLOW);
	return_trace (ખોટું);
      }
    }

    u.format.v = format;

    પસંદગી (u.format.v)
    {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_second)));
#endif
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = nullptr /*OUT*/,
               તાર્કિક keep_empty_table = સાચું,
               તાર્કિક use_class_zero = સાચું,
               અચલ Coverage* glyph_filter = nullptr) અચલ
  {
    TRACE_SUBSET (આ);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_empty_table, use_class_zero, glyph_filter));
#endif
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    સ્થિતિ 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    સ્થિતિ 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#endif
    મૂળભૂત:return_trace (સાચું);
    }
  }

  unsigned cost () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.cost ();
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.cost ();
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.cost ();
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.cost ();
#endif
    મૂળભૂત:પાછા 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  તાર્કિક collect_coverage (set_t *glyphs) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.collect_coverage (glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.collect_coverage (glyphs);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.collect_coverage (glyphs);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.collect_coverage (glyphs);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename set_t>
  તાર્કિક collect_class (set_t *glyphs, unsigned પૂર્ણાંક klass) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.collect_class (glyphs, klass);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.collect_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.collect_class (glyphs, klass);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.collect_class (glyphs, klass);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersects (glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersects (glyphs);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersects (glyphs);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersects (glyphs);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }
  તાર્કિક intersects_class (અચલ hb_set_t *glyphs, unsigned પૂર્ણાંક klass) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersects_class (glyphs, klass);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersects_class (glyphs, klass);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersects_class (glyphs, klass);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersects_class (glyphs, klass);
#endif
    મૂળભૂત:પાછા ખોટું;
    }
  }

  શૂન્ય intersected_class_glyphs (અચલ hb_set_t *glyphs, unsigned klass, hb_set_t *intersect_glyphs) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#endif
    મૂળભૂત:પાછા;
    }
  }

  શૂન્ય intersected_classes (અચલ hb_set_t *glyphs, hb_set_t *intersect_classes) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.intersected_classes (glyphs, intersect_classes);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.intersected_classes (glyphs, intersect_classes);
#ifndef HB_NO_BEYOND_64K
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.intersected_classes (glyphs, intersect_classes);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.intersected_classes (glyphs, intersect_classes);
#endif
    મૂળભૂત:પાછા;
    }
  }


  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#ifndef HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#endif
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};

template<typename Iterator>
સ્થિર અંતઃરેખ તાર્કિક ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ પાછા (c->start_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
સંરચના delta_row_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  સંરચના chars_t : hb_vector_t<uint8_t>
  {
    પૂર્ણાંક cmp (અચલ chars_t& other) અચલ
    {
      પાછા as_array ().cmp (other.as_array ());
    }

    hb_pair_t<unsigned, unsigned> get_width ()
    {
      unsigned width = 0;
      unsigned columns = 0;
      માટે (unsigned i = 0; i < length; i++)
      {
	unsigned v = arrayZ[i];
	width += v;
	columns += (v != 0);
      }
      પાછા hb_pair (width, columns);
    }

    HB_HOT
    hb_pair_t<unsigned, unsigned> combine_width (અચલ chars_t& other) અચલ
    {
      unsigned combined_width = 0;
      unsigned combined_columns = 0;
      માટે (unsigned i = 0; i < length; i++)
      {
        unsigned v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_width += v;
	combined_columns += (v != 0);
      }
      પાછા hb_pair (combined_width, combined_columns);
    }
  };

  hb_pair_t<unsigned, unsigned> combine_width (અચલ delta_row_encoding_t& other_encoding) અચલ { પાછા chars.combine_width (other_encoding.chars); }

  // Actual data

  chars_t chars;
  unsigned width = 0;
  unsigned overhead = 0;
  hb_vector_t<અચલ hb_vector_t<પૂર્ણાંક>*> વસ્તુઓ;

  delta_row_encoding_t () = મૂળભૂત;
  delta_row_encoding_t (hb_vector_t<અચલ hb_vector_t<પૂર્ણાંક>*> &&rows, unsigned અંક_cols)
  {
    પ્રતિજ્ઞા (rows);

    વસ્તુઓ = std::move (rows);

    જો (unlikely (!chars.resize (અંક_cols)))
      પાછા;

    calculate_chars ();
  }

  શૂન્ય merge (અચલ delta_row_encoding_t& other)
  {
    વસ્તુઓ.alloc (વસ્તુઓ.length + other.વસ્તુઓ.length);
    માટે (auto &row : other.વસ્તુઓ)
      add_row (row);

    // Merge chars
    પ્રતિજ્ઞા (chars.length == other.chars.length);
    માટે (unsigned i = 0; i < chars.length; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_changed ();
  }

  શૂન્ય chars_changed ()
  {
    auto _ = chars.get_width ();
    width = _.first;
    overhead = get_chars_overhead (_.second);
  }

  શૂન્ય calculate_chars ()
  {
    પ્રતિજ્ઞા (વસ્તુઓ);

    તાર્કિક long_શબ્દો = ખોટું;

    માટે (auto &row : વસ્તુઓ)
    {
      પ્રતિજ્ઞા (row->length == chars.length);

      /* 0/1/2 byte encoding */
      માટે (unsigned i = 0; i < row->length; i++)
      {
	પૂર્ણાંક v =  row->arrayZ[i];
	જો (v == 0)
	  ચાલુ;
	નહીંતર જો (v > 32767 || v < -32768)
	{
	  long_શબ્દો = સાચું;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	નહીંતર જો (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	નહીંતર
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    જો (long_શબ્દો)
    {
      // Convert 1s to 2s
      માટે (auto &v : chars)
	જો (v == 1)
	  v = 2;
    }

    chars_changed ();
  }

  તાર્કિક is_empty () અચલ
  { પાછા !વસ્તુઓ; }

  તાર્કિક in_error () અચલ
  { પાછા chars.in_error () || વસ્તુઓ.in_error (); }

  સ્થિર અંતઃરેખ unsigned get_chars_overhead (unsigned અંક_columns)
  {
    unsigned c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    પાછા c + અંક_columns * 2;
  }

  unsigned get_gain (unsigned additional_bytes_per_rows = 1) અચલ
  {
    પૂર્ણાંક ગણતરી = વસ્તુઓ.length;
    પાછા hb_max (0, (પૂર્ણાંક) overhead - ગણતરી * (પૂર્ણાંક) additional_bytes_per_rows);
  }

  પૂર્ણાંક gain_from_merging (અચલ delta_row_encoding_t& other_encoding) અચલ
  {
    // Back of the envelope calculations to reject early.
    signed additional_bytes_per_rows = other_encoding.width - width;
    જો (additional_bytes_per_rows > 0)
    {
      જો (get_gain (additional_bytes_per_rows) == 0)
        પાછા 0;
    }
    નહીંતર
    {
      જો (other_encoding.get_gain (-additional_bytes_per_rows) == 0)
	પાછા 0;
    }

    auto pair = combine_width (other_encoding);
    unsigned combined_width = pair.first;
    unsigned combined_columns = pair.second;

    પૂર્ણાંક combined_gain = (પૂર્ણાંક) overhead + (પૂર્ણાંક) other_encoding.overhead;
    combined_gain -= (combined_width - (પૂર્ણાંક) width) * વસ્તુઓ.length;
    combined_gain -= (combined_width - (પૂર્ણાંક) other_encoding.width) * other_encoding.વસ્તુઓ.length;
    combined_gain -= get_chars_overhead (combined_columns);

    પાછા combined_gain;
  }

  તાર્કિક add_row (અચલ hb_vector_t<પૂર્ણાંક>* row)
  { પાછા વસ્તુઓ.push (row); }

  સ્થિર પૂર્ણાંક cmp (અચલ શૂન્ય *pa, અચલ શૂન્ય *pb)
  {
    અચલ delta_row_encoding_t *a = (અચલ delta_row_encoding_t *)pa;
    અચલ delta_row_encoding_t *b = (અચલ delta_row_encoding_t *)pb;

    જો (a->width != b->width)
      પાછા (પૂર્ણાંક) a->width - (પૂર્ણાંક) b->width;

    પાછા b->chars.cmp (a->chars);
  }
};

સંરચના VarRegionAxis
{
  દશાંશ evaluate (પૂર્ણાંક coord) અચલ
  {
    પૂર્ણાંક peak = peakCoord.to_int ();
    જો (peak == 0 || coord == peak)
      પાછા 1.f;
    નહીંતર જો (coord == 0) // Faster
      પાછા 0.f;

    પૂર્ણાંક start = startCoord.to_int (), end = endCoord.to_int ();

    /* TODO Move these to sanitize(). */
    જો (unlikely (start > peak || peak > end))
      પાછા 1.f;
    જો (unlikely (start < 0 && end > 0))
      પાછા 1.f;

    જો (coord <= start || end <= coord)
      પાછા 0.f;

    /* Interpolate */
    જો (coord < peak)
      પાછા દશાંશ (coord - start) / (peak - start);
    નહીંતર
      પાછા દશાંશ (end - coord) / (end - peak);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  તાર્કિક serialize (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (આ));
  }

  જાહેર:
  F2DOT14	startCoord;
  F2DOT14	peakCoord;
  F2DOT14	endCoord;
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};
સંરચના SparseVarRegionAxis
{
  દશાંશ evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len) અચલ
  {
    unsigned i = axisIndex;
    પૂર્ણાંક coord = i < coord_len ? coords[i] : 0;
    પાછા axis.evaluate (coord);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ SparseVarRegionAxis *src,
		  અચલ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->embed (src);
    જો (unlikely (!out)) return_trace (ખોટું);
    જો (axis_map)
    {
      જો (unlikely (!axis_map->has (out->axisIndex)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (ખોટું);
      }
      જો (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  જાહેર:
  HBUINT16 axisIndex;
  VarRegionAxis axis;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના hb_scalar_cache_t
{
  ખાનગી:
  સ્થિર constexpr unsigned STATIC_LENGTH = 128;
  સ્થિર constexpr દશાંશ INVALID = NAN;

  જાહેર:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (અચલ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = delete;
  hb_scalar_cache_t& operator= (અચલ hb_scalar_cache_t&) = delete;
  hb_scalar_cache_t& operator= (hb_scalar_cache_t&&) = delete;

  સ્થિર hb_scalar_cache_t *create (unsigned પૂર્ણાંક ગણતરી,
				    hb_scalar_cache_t *scratch_cache = nullptr)
  {
    જો (!ગણતરી) પાછા (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    જો (scratch_cache && ગણતરી <= STATIC_LENGTH)
    {
      scratch_cache->length = ગણતરી;
      scratch_cache->clear ();
      પાછા scratch_cache;
    }

    auto *cache = (hb_scalar_cache_t *) hb_malloc (કદ (hb_scalar_cache_t) - કદ (static_કિંમતો) + કદ (static_કિંમતો[0]) * ગણતરી);
    જો (unlikely (!cache)) પાછા (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->length = ગણતરી;
    cache->clear ();

    પાછા cache;
  }

  સ્થિર શૂન્ય destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = nullptr)
  {
    જો (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  શૂન્ય clear ()
  {
    auto *values = &static_કિંમતો[0];
    unsigned i = 0;
#ifndef HB_OPTIMIZE_SIZE
    માટે (; i + 3 < length; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#endif
    માટે (; i < length; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  તાર્કિક get (unsigned i, દશાંશ *value) અચલ
  {
    જો (unlikely (i >= length))
    {
      *value = 0.f;
      પાછા સાચું;
    }
    auto *values = &static_કિંમતો[0];
    auto *cached_કિંમત = &values[i];
    દશાંશ v = *cached_કિંમત;
    જો (std::isnan (v))
      પાછા ખોટું;
    *value = v;
    પાછા સાચું;
  }

  HB_ALWAYS_INLINE
  શૂન્ય set (unsigned i, દશાંશ value)
  {
    જો (unlikely (i >= length)) પાછા;
    auto *values = &static_કિંમતો[0];
    auto *cached_કિંમત = &values[i];
    *cached_કિંમત = value;
  }

  જાહેર:
  unsigned length;
  ખાનગી:
  mutable hb_atomic_t<દશાંશ> static_કિંમતો[STATIC_LENGTH];
};

સંરચના VarRegionList
{
  ખાનગી:
  દશાંશ evaluate_impl (unsigned પૂર્ણાંક region_index,
		       અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len) અચલ
  {
    અચલ VarRegionAxis *axes = axesZ.arrayZ + (region_index * axis_ગણતરી_2);
    દશાંશ v = 1.f;

    unsigned પૂર્ણાંક ગણતરી = axis_ગણતરી_2;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      પૂર્ણાંક coord = i < coord_len ? coords[i] : 0;
      દશાંશ factor = axes[i].evaluate (coord);
      જો (factor == 0.f)
      {
	v = 0.f;
	તોડો;
      }
      v *= factor;
    }

    પાછા v;
  }

  જાહેર:
  HB_ALWAYS_INLINE
  દશાંશ evaluate (unsigned પૂર્ણાંક region_index,
		  અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		  hb_scalar_cache_t *cache = nullptr) અચલ
  {
    જો (unlikely (region_index >= region_ગણતરી))
      પાછા 0.;

    દશાંશ v;
    જો (cache && cache->get (region_index, &v))
      પાછા v;

    v = evaluate_impl (region_index, coords, coord_len);

    જો (cache)
      cache->set (region_index, v);
    પાછા v;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_ગણતરી_2 * region_ગણતરી));
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
                  અચલ hb_vector_t<hb_tag_t>& axis_tags,
                  અચલ hb_vector_t<અચલ hb_hashmap_t<hb_tag_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (આ);
    unsigned axis_ગણતરી = axis_tags.length;
    unsigned region_ગણતરી_2 = regions.length;
    જો (!axis_ગણતરી || !region_ગણતરી_2) return_trace (ખોટું);
    જો (unlikely (hb_unsigned_mul_overflows (axis_ગણતરી * region_ગણતરી_2,
                                             VarRegionAxis::static_size))) return_trace (ખોટું);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    axis_ગણતરી_2 = axis_ગણતરી;
    region_ગણતરી = region_ગણતરી_2;

    માટે (unsigned r = 0; r < region_ગણતરી_2; r++)
    {
      અચલ auto& region = regions[r];
      માટે (unsigned i = 0; i < axis_ગણતરી; i++)
      {
        hb_tag_t tag = axis_tags.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        જો (region->has (tag, &coords))
        {
          var_region_rec.startCoord.set_float (coords->minimum);
          var_region_rec.peakCoord.set_float (coords->મધ્ય);
          var_region_rec.endCoord.set_float (coords->maximum);
        }
        નહીંતર
        {
          var_region_rec.startCoord.set_int (0);
          var_region_rec.peakCoord.set_int (0);
          var_region_rec.endCoord.set_int (0);
        }
        જો (!var_region_rec.serialize (c))
          return_trace (ખોટું);
      }
    }
    return_trace (સાચું);
  }

  તાર્કિક serialize (hb_serialize_context_t *c, અચલ VarRegionList *src, અચલ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    axis_ગણતરી_2 = src->axis_ગણતરી_2;
    region_ગણતરી = region_map.get_population ();
    જો (unlikely (hb_unsigned_mul_overflows (axis_ગણતરી_2 * region_ગણતરી,
					     VarRegionAxis::static_size))) return_trace (ખોટું);
    જો (unlikely (!c->extend (આ))) return_trace (ખોટું);
    unsigned પૂર્ણાંક region_ગણતરી_2 = src->region_ગણતરી;
    માટે (unsigned પૂર્ણાંક r = 0; r < region_ગણતરી; r++)
    {
      unsigned પૂર્ણાંક backward = region_map.backward (r);
      જો (backward >= region_ગણતરી_2) return_trace (ખોટું);
      hb_memcpy (&axesZ[axis_ગણતરી_2 * r], &src->axesZ[axis_ગણતરી_2 * backward], VarRegionAxis::static_size * axis_ગણતરી_2);
    }

    return_trace (સાચું);
  }

  તાર્કિક get_var_region (unsigned region_index,
                       અચલ hb_map_t& axes_old_index_tag_map,
                       hb_hashmap_t<hb_tag_t, Triple>& axis_tuples /* OUT */) અચલ
  {
    જો (region_index >= region_ગણતરી) પાછા ખોટું;
    અચલ VarRegionAxis* axis_region = axesZ.arrayZ + (region_index * axis_ગણતરી_2);
    માટે (unsigned i = 0; i < axis_ગણતરી_2; i++)
    {
      hb_tag_t *axis_tag;
      જો (!axes_old_index_tag_map.has (i, &axis_tag))
        પાછા ખોટું;

      દશાંશ min_val = axis_region->startCoord.to_float ();
      દશાંશ def_val = axis_region->peakCoord.to_float ();
      દશાંશ max_val = axis_region->endCoord.to_float ();

      જો (def_val != 0.f)
        axis_tuples.set (*axis_tag, Triple ((દ્વિગુણ) min_val, (દ્વિગુણ) def_val, (દ્વિગુણ) max_val));
      axis_region++;
    }
    પાછા !axis_tuples.in_error ();
  }

  તાર્કિક get_var_regions (અચલ hb_map_t& axes_old_index_tag_map,
                        hb_vector_t<hb_hashmap_t<hb_tag_t, Triple>>& regions /* OUT */) અચલ
  {
    જો (!regions.alloc (region_ગણતરી))
      પાછા ખોટું;

    માટે (unsigned i = 0; i < region_ગણતરી; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      જો (!get_var_region (i, axes_old_index_tag_map, axis_tuples))
        પાછા ખોટું;
      regions.push (std::move (axis_tuples));
    }
    પાછા !regions.in_error ();
  }

  size_t get_size () અચલ
  { પાછા hb_unsigned_add_saturate (min_size,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_size,
							       axis_ગણતરી_2, region_ગણતરી)); }

  જાહેર:
  HBUINT16	axis_ગણતરી_2;
  HBUINT15	region_ગણતરી;
  સુરક્ષિત:
  UnsizedArrayOf<VarRegionAxis>
		axesZ;
  જાહેર:
  DEFINE_SIZE_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
સ્થિર અંતઃરેખ તાર્કિક
_hb_avar2_region_is_dead (અચલ hb_hashmap_t<hb_tag_t, Triple> &axis_tuples,
			  અચલ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges)
{
  માટે (auto _ : axis_tuples)
  {
    Triple *range;
    જો (!reachable_ranges.has (_.first, &range)) ચાલુ;
    અચલ Triple &tent = _.second;
    પૂર્ણાંક start = (પૂર્ણાંક) roundf ((દશાંશ) tent.minimum * 16384.f);
    પૂર્ણાંક peak  = (પૂર્ણાંક) roundf ((દશાંશ) tent.મધ્ય * 16384.f);
    પૂર્ણાંક end   = (પૂર્ણાંક) roundf ((દશાંશ) tent.maximum * 16384.f);
    જો (!peak) ચાલુ;
    જો (start > peak || peak > end) ચાલુ;
    જો (start < 0 && end > 0) ચાલુ;
    પૂર્ણાંક lo = (પૂર્ણાંક) roundf ((દશાંશ) range->minimum * 16384.f);
    પૂર્ણાંક hi = (પૂર્ણાંક) roundf ((દશાંશ) range->maximum * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    જો (hi < start || lo > end)
      પાછા સાચું;
  }
  પાછા ખોટું;
}

સંરચના SparseVariationRegion
{
  દશાંશ evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len) અચલ
  {
    દશાંશ v = 1.f;
    unsigned પૂર્ણાંક ગણતરી = axes.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      દશાંશ factor = (આ+axes[i]).evaluate (coords, coord_len);
      જો (factor == 0.f)
	પાછા 0.;
      v *= factor;
    }
    પાછા v;
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ SparseVariationRegion *src,
		  અચલ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!axes.serialize (c, (unsigned) src->axes.len))) return_trace (ખોટું);
    માટે (unsigned i = 0; i < axes.len; i++)
      જો (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (ખોટું);
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (axes.sanitize (c, આ));
  }

  સુરક્ષિત:
  Array16Of<Offset32To<SparseVarRegionAxis>> axes;
  જાહેર:
  DEFINE_SIZE_ARRAY (2, axes);
};

સંરચના SparseVarRegionList
{
  HB_ALWAYS_INLINE
  દશાંશ evaluate (unsigned પૂર્ણાંક region_index,
		  અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		  hb_scalar_cache_t *cache = nullptr) અચલ
  {
    જો (unlikely (region_index >= regions.len))
      પાછા 0.;

    દશાંશ v;
    જો (cache && cache->get (region_index, &v))
      પાછા v;

    અચલ SparseVariationRegion &region = આ+regions[region_index];

    v = region.evaluate (coords, coord_len);
    જો (cache)
      cache->set (region_index, v);

    પાછા v;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (regions.sanitize (c, આ));
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ SparseVarRegionList *src,
		  અચલ hb_inc_bimap_t &region_map,
		  અચલ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    regions.len = region_map.get_population ();
    જો (unlikely (!c->extend (regions))) return_trace (ખોટું);

    માટે (unsigned i = 0; i < regions.len; i++)
    {
      unsigned old_index = region_map.backward (i);
      જો (unlikely (old_index >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_index]),
					     axis_map)))
	return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  જાહેર:
  Array32Of<Offset32To<SparseVariationRegion>>
		regions;
  જાહેર:
  DEFINE_SIZE_ARRAY (4, regions);
};


સંરચના VarData
{
  unsigned પૂર્ણાંક get_item_ગણતરી () અચલ
  { પાછા item_ગણતરી; }

  unsigned પૂર્ણાંક get_region_index_ગણતરી () અચલ
  { પાછા regionIndices.len; }

  unsigned get_region_index (unsigned i) અચલ
  { પાછા i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_row_size () અચલ
  { પાછા (word_ગણતરી () + regionIndices.len) * (long_શબ્દો_2 () ? 2 : 1); }

  size_t get_size () અચલ
  { પાછા hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size (),
				     hb_unsigned_mul_saturate (item_ગણતરી, get_row_size ())); }

  template <typename Float = દશાંશ>
  Float _get_delta (unsigned પૂર્ણાંક inner,
		    અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		    અચલ VarRegionList &regions,
		    hb_scalar_cache_t *cache = nullptr) અચલ
  {
    જો (unlikely (inner >= item_ગણતરી))
      પાછા 0.;
    તાર્કિક is_long = long_શબ્દો_2 ();
    unsigned પૂર્ણાંક ગણતરી = regionIndices.len;
    unsigned word_ગણતરી_2 = word_ગણતરી ();
    unsigned પૂર્ણાંક scount = is_long ? ગણતરી : word_ગણતરી_2;
    unsigned પૂર્ણાંક lcount = is_long ? word_ગણતરી_2 : 0;

    અચલ HBUINT8 *bytes = get_delta_bytes ();
    અચલ HBUINT8 *row = bytes + inner * get_row_size ();

    Float delta = 0;
    unsigned પૂર્ણાંક i = 0;

    અચલ HBINT32 *lcursor = reinterpret_cast<અચલ HBINT32 *> (row);
    માટે (; i < lcount; i++)
    {
      દશાંશ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ગણતરી, cache);
      જો (scalar)
        delta += (Float) scalar * (પૂર્ણાંક) *lcursor;
      lcursor++;
    }
    અચલ HBINT16 *scursor = reinterpret_cast<અચલ HBINT16 *> (lcursor);
    માટે (; i < scount; i++)
    {
      દશાંશ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ગણતરી, cache);
      જો (scalar)
       delta += (Float) scalar * (પૂર્ણાંક) *scursor;
      scursor++;
    }
    અચલ HBINT8 *bcursor = reinterpret_cast<અચલ HBINT8 *> (scursor);
    માટે (; i < ગણતરી; i++)
    {
      દશાંશ scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ગણતરી, cache);
      જો (scalar)
        delta += (Float) scalar * (પૂર્ણાંક) *bcursor;
      bcursor++;
    }

    પાછા delta;
  }

  template <typename Float = દશાંશ>
  HB_ALWAYS_INLINE
  Float get_delta (unsigned પૂર્ણાંક inner,
		   અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		   અચલ VarRegionList &regions,
		   hb_scalar_cache_t *cache = nullptr) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = regionIndices.len;
    જો (!ગણતરી) પાછા Float (0); // This is quite common, so optimize it.
    પાછા _get_delta<Float> (inner, coords, coord_ગણતરી, regions, cache);
  }

  શૂન્ય get_region_scalars (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
			   અચલ VarRegionList &regions,
			   દશાંશ *scalars /*OUT */,
			   unsigned પૂર્ણાંક અંક_scalars) અચલ
  {
    unsigned ગણતરી = hb_min (અંક_scalars, regionIndices.len);
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ગણતરી);
    માટે (unsigned પૂર્ણાંક i = ગણતરી; i < અંક_scalars; i++)
      scalars[i] = 0.f;
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  word_ગણતરી () <= regionIndices.len &&
		  c->check_range (get_delta_bytes (),
				  item_ગણતરી,
				  get_row_size ()));
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
                  તાર્કિક has_long,
                  અચલ hb_vector_t<અચલ hb_vector_t<પૂર્ણાંક>*>& rows)
  {
    TRACE_SERIALIZE (આ);
    unsigned row_ગણતરી_2 = rows.length;
    જો (!row_ગણતરી_2) {
      // Nothing to serialize, will be empty.
      પાછા ખોટું;
    }

    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    જો (unlikely (!c->check_assign (item_ગણતરી, row_ગણતરી_2,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ખોટું);

    પૂર્ણાંક min_threshold = has_long ? -32768 : -128;
    પૂર્ણાંક max_threshold = has_long ? +32767 : +127;
    ગણના delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    unsigned અંક_regions = rows[0]->length;
    જો (!delta_sz.resize (અંક_regions))
      return_trace (ખોટું);

    unsigned word_ગણતરી_2 = 0;
    માટે (unsigned r = 0; r < અંક_regions; r++)
    {
      માટે (unsigned i = 0; i < row_ગણતરી_2; i++)
      {
        પૂર્ણાંક delta = rows[i]->arrayZ[r];
        જો (delta < min_threshold || delta > max_threshold)
        {
          delta_sz[r] = kWord;
          word_ગણતરી_2++;
          તોડો;
        }
        નહીંતર જો (delta != 0)
        {
          delta_sz[r] = kNonWord;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    unsigned word_index = 0;
    unsigned non_word_index = word_ગણતરી_2;
    hb_map_t ri_map;
    માટે (unsigned r = 0; r < અંક_regions; r++)
    {
      જો (!delta_sz[r]) ચાલુ;
      unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
      જો (!ri_map.set (new_r, r))
        return_trace (ખોટું);
    }

    word_Size_ગણતરી = word_ગણતરી_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    unsigned ri_ગણતરી = ri_map.get_population ();
    regionIndices.len = ri_ગણતરી;
    જો (unlikely (!c->extend (આ))) return_trace (ખોટું);

    માટે (unsigned r = 0; r < ri_ગણતરી; r++)
    {
      hb_codepoint_t *idx;
      જો (!ri_map.has (r, &idx))
        return_trace (ખોટું);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    માટે (unsigned પૂર્ણાંક i = 0; i < row_ગણતરી_2; i++)
    {
      માટે (unsigned પૂર્ણાંક r = 0; r < ri_ગણતરી; r++)
      {
        પૂર્ણાંક delta = rows[i]->arrayZ[ri_map[r]];
        set_item_delta_fast (i, r, delta, delta_bytes, row_size);
      }
    }
    return_trace (સાચું);
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ VarData *src,
		  અચલ hb_inc_bimap_t &inner_map,
		  અચલ hb_inc_bimap_t &region_map,
		  અચલ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    item_ગણતરી = inner_map.get_next_કિંમત ();

    /* Optimize word count */
    unsigned ri_ગણતરી = src->regionIndices.len;
    ગણના delta_size_t { kZero=0, kNonWord, kWord };
    hb_vector_t<delta_size_t> delta_sz;
    hb_vector_t<unsigned પૂર્ણાંક> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_ગણતરી);
    ri_map.resize (ri_ગણતરી);
    unsigned પૂર્ણાંક new_word_ગણતરી = 0;
    unsigned પૂર્ણાંક r;

    અચલ HBUINT8 *src_delta_bytes = src->get_delta_bytes ();
    unsigned src_row_size = src->get_row_size ();
    unsigned src_word_ગણતરી = src->word_ગણતરી ();
    તાર્કિક     src_long_શબ્દો = src->long_શબ્દો_2 ();

    તાર્કિક has_long = ખોટું;
    જો (src_long_શબ્દો)
    {
      માટે (r = 0; r < src_word_ગણતરી; r++)
      {
        માટે (unsigned old_gid : inner_map.ચાવીઓ())
	{
	  int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	  જો (delta < -32768 || 32767 < delta)
	  {
	    has_long = સાચું;
	    તોડો;
	  }
        }
      }
    }

    signed min_threshold = has_long ? -32768 : -128;
    signed max_threshold = has_long ? +32767 : +127;
    માટે (r = 0; r < ri_ગણતરી; r++)
    {
      તાર્કિક short_circuit = src_long_શબ્દો == has_long && src_word_ગણતરી <= r;

      delta_sz[r] = kZero;
      જો (culled_regions && culled_regions->has (src->regionIndices[r]))
	ચાલુ; /* unreachable region: drop its column */
      માટે (unsigned old_gid : inner_map.ચાવીઓ())
      {
	int32_t delta = src->get_item_delta_fast (old_gid, r, src_delta_bytes, src_row_size);
	જો (delta < min_threshold || max_threshold < delta)
	{
	  delta_sz[r] = kWord;
	  new_word_ગણતરી++;
	  તોડો;
	}
	નહીંતર જો (delta != 0)
	{
	  delta_sz[r] = kNonWord;
	  જો (short_circuit)
	    તોડો;
	}
      }
    }

    unsigned પૂર્ણાંક word_index = 0;
    unsigned પૂર્ણાંક non_word_index = new_word_ગણતરી;
    unsigned પૂર્ણાંક new_ri_ગણતરી = 0;
    માટે (r = 0; r < ri_ગણતરી; r++)
      જો (delta_sz[r])
      {
	unsigned new_r = (delta_sz[r] == kWord)? word_index++ : non_word_index++;
	ri_map[new_r] = r;
	new_ri_ગણતરી++;
      }

    word_Size_ગણતરી = new_word_ગણતરી | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_ગણતરી;

    જો (unlikely (!c->extend (આ))) return_trace (ખોટું);

    માટે (r = 0; r < new_ri_ગણતરી; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();
    unsigned ગણતરી = item_ગણતરી;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      unsigned પૂર્ણાંક old = inner_map.backward (i);
      માટે (unsigned પૂર્ણાંક r = 0; r < new_ri_ગણતરી; r++)
	set_item_delta_fast (i, r,
			     src->get_item_delta_fast (old, ri_map[r],
						       src_delta_bytes, src_row_size),
			     delta_bytes, row_size);
    }

    return_trace (સાચું);
  }

  શૂન્ય collect_region_refs (hb_set_t &region_indices, અચલ hb_inc_bimap_t &inner_map) અચલ
  {
    અચલ HBUINT8 *delta_bytes = get_delta_bytes ();
    unsigned row_size = get_row_size ();

    માટે (unsigned પૂર્ણાંક r = 0; r < regionIndices.len; r++)
    {
      unsigned પૂર્ણાંક region = regionIndices.arrayZ[r];
      જો (region_indices.has (region)) ચાલુ;
      માટે (hb_codepoint_t old_gid : inner_map.ચાવીઓ())
	જો (get_item_delta_fast (old_gid, r, delta_bytes, row_size) != 0)
	{
	  region_indices.add (region);
	  તોડો;
	}
    }
  }

  જાહેર:
  અચલ HBUINT8 *get_delta_bytes () અચલ
  { પાછા &StructAfter<HBUINT8> (regionIndices); }

  સુરક્ષિત:
  HBUINT8 *get_delta_bytes ()
  { પાછા &StructAfter<HBUINT8> (regionIndices); }

  જાહેર:
  int32_t get_item_delta_fast (unsigned પૂર્ણાંક item, unsigned પૂર્ણાંક region,
			       અચલ HBUINT8 *delta_bytes, unsigned row_size) અચલ
  {
    જો (unlikely (item >= item_ગણતરી || region >= regionIndices.len)) પાછા 0;

    અચલ HBINT8 *p = (અચલ HBINT8 *) delta_bytes + item * row_size;
    unsigned word_ગણતરી_2 = word_ગણતરી ();
    તાર્કિક is_long = long_શબ્દો_2 ();
    જો (is_long)
    {
      જો (region < word_ગણતરી_2)
	પાછા ((અચલ HBINT32 *) p)[region];
      નહીંતર
	પાછા ((અચલ HBINT16 *)(p + HBINT32::static_size * word_ગણતરી_2))[region - word_ગણતરી_2];
    }
    નહીંતર
    {
      જો (region < word_ગણતરી_2)
	પાછા ((અચલ HBINT16 *) p)[region];
      નહીંતર
	પાછા (p + HBINT16::static_size * word_ગણતરી_2)[region - word_ગણતરી_2];
    }
  }
  int32_t get_item_delta (unsigned પૂર્ણાંક item, unsigned પૂર્ણાંક region) અચલ
  {
     પાછા get_item_delta_fast (item, region,
				 get_delta_bytes (),
				 get_row_size ());
  }

  સુરક્ષિત:
  શૂન્ય set_item_delta_fast (unsigned પૂર્ણાંક item, unsigned પૂર્ણાંક region, int32_t delta,
			    HBUINT8 *delta_bytes, unsigned row_size)
  {
    HBINT8 *p = (HBINT8 *) delta_bytes + item * row_size;
    unsigned word_ગણતરી_2 = word_ગણતરી ();
    તાર્કિક is_long = long_શબ્દો_2 ();
    જો (is_long)
    {
      જો (region < word_ગણતરી_2)
	((HBINT32 *) p)[region] = delta;
      નહીંતર
	((HBINT16 *)(p + HBINT32::static_size * word_ગણતરી_2))[region - word_ગણતરી_2] = delta;
    }
    નહીંતર
    {
      જો (region < word_ગણતરી_2)
	((HBINT16 *) p)[region] = delta;
      નહીંતર
	(p + HBINT16::static_size * word_ગણતરી_2)[region - word_ગણતરી_2] = delta;
    }
  }
  શૂન્ય set_item_delta (unsigned પૂર્ણાંક item, unsigned પૂર્ણાંક region, int32_t delta)
  {
    set_item_delta_fast (item, region, delta,
			 get_delta_bytes (),
			 get_row_size ());
  }

  તાર્કિક long_શબ્દો_2 () અચલ { પાછા word_Size_ગણતરી & 0x8000u /* LONG_WORDS */; }
  unsigned word_ગણતરી () અચલ { પાછા word_Size_ગણતરી & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  સુરક્ષિત:
  HBUINT16		item_ગણતરી;
  HBUINT16		word_Size_ગણતરી;
  Array16Of<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  જાહેર:
  DEFINE_SIZE_ARRAY (6, regionIndices);
};

સંરચના MultiVarData
{
  size_t get_size () અચલ
  { પાછા hb_unsigned_add_saturate (min_size - regionIndices.min_size,
				     regionIndices.get_size ()); }

  અચલ TupleList &get_delta_sets () અચલ
  { પાછા આ+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  શૂન્ય get_delta (unsigned પૂર્ણાંક inner,
		  અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		  અચલ SparseVarRegionList &regions,
		  hb_array_t<દ્વિગુણ> out,
		  hb_scalar_cache_t *cache = nullptr) અચલ
  {
    auto &deltaSets = get_delta_sets ();

    auto કિંમતો_iter = deltaSets.fetcher (inner);
    unsigned region_ગણતરી = regionIndices.len;
    unsigned skip = 0;
    માટે (unsigned regionIndex = 0; regionIndex < region_ગણતરી; regionIndex++)
    {
      દશાંશ scalar = regions.evaluate (regionIndices.arrayZ[regionIndex],
				       coords, coord_ગણતરી,
				       cache);
      // We skip lazily. Helps with the tail end.
      જો (scalar == 0.0f)
        skip += out.length;
      નહીંતર
      {
        જો (skip)
	{
	  કિંમતો_iter.skip (skip);
	  skip = 0;
	}
	કિંમતો_iter.add_to (out, scalar);
      }
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, આ));
  }

  unsigned get_item_ગણતરી () અચલ
  { પાછા get_delta_sets ().ગણતરી; }

  શૂન્ય collect_region_refs (hb_set_t &region_indices) અચલ
  {
    માટે (unsigned region : regionIndices)
      region_indices.add (region);
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ MultiVarData *src,
		  અચલ hb_inc_bimap_t &inner_map,
		  અચલ hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (આ);
    unsigned region_ગણતરી_2 = src->regionIndices.len;
    unsigned header_size = HBUINT8::static_size +
			   Array16Of<HBUINT16>::min_size +
			   region_ગણતરી_2 * HBUINT16::static_size +
			   HBUINT32::static_size;
    જો (unlikely (!c->extend_size (આ, header_size, ખોટું)))
      return_trace (ખોટું);
    format = 1;
    regionIndices.len = region_ગણતરી_2;
    માટે (unsigned i = 0; i < regionIndices.len; i++)
    {
      unsigned old_region = src->regionIndices[i];
      જો (unlikely (!region_map.has (old_region)))
	return_trace (ખોટું);
      regionIndices[i] = region_map.get (old_region);
    }

    અચલ auto &src_delta_sets = src->get_delta_sets ();
    hb_vector_t<hb_ubytes_t> delta_sets;
    unsigned data_size = 0;
    જો (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (ખોટું);
    માટે (unsigned i = 0; i < inner_map.get_population (); i++)
    {
      unsigned old_inner = inner_map.backward (i);
      જો (unlikely (old_inner >= src_delta_sets.ગણતરી)) return_trace (ખોટું);
      hb_ubytes_t bytes = static_cast<અચલ CFF2Index &> (src_delta_sets)[old_inner];
      data_size = hb_unsigned_add_saturate (data_size, bytes.length);
      delta_sets.push (bytes);
    }
    જો (unlikely (data_size == UINT_MAX || delta_sets.in_error ()))
      return_trace (ખોટું);

    auto &out_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (out_delta_sets.serialize_serialize (c, delta_sets.iter (), &data_size));
  }

  સુરક્ષિત:
  HBUINT8	      format; // 1
  Array16Of<HBUINT16> regionIndices;
  Offset32To<TupleList> deltaSetsX;
  જાહેર:
  DEFINE_SIZE_MIN (7);
};

સંરચના ItemVariationStore
{
  friend સંરચના item_variations_t;

  hb_scalar_cache_t *create_cache () અચલ
  {
#ifdef HB_NO_VAR
    પાછા hb_scalar_cache_t::create (0);
#endif
    પાછા hb_scalar_cache_t::create ((આ+regions).region_ગણતરી);
  }

  સ્થિર શૂન્ય destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  ખાનગી:
  template <typename Float = દશાંશ>
  Float get_delta (unsigned પૂર્ણાંક outer, unsigned પૂર્ણાંક inner,
		   અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		   hb_scalar_cache_t *cache = nullptr) અચલ
  {
#ifdef HB_NO_VAR
    પાછા Float (0);
#endif

    જો (unlikely (outer >= dataSets.len))
      પાછા Float (0);

    પાછા (આ+dataSets[outer]).get_delta<Float> (inner,
					     coords, coord_ગણતરી,
					     આ+regions,
					     cache);
  }

  જાહેર:
  તાર્કિક has_delta_set (unsigned પૂર્ણાંક index) અચલ
  {
#ifdef HB_NO_VAR
    પાછા ખોટું;
#endif

    unsigned પૂર્ણાંક outer = index >> 16;
    unsigned પૂર્ણાંક inner = index & 0xFFFF;
    પાછા outer < dataSets.len &&
	   inner < (આ+dataSets[outer]).get_item_ગણતરી ();
  }

  template <typename Float = દશાંશ>
  Float get_delta (unsigned પૂર્ણાંક index,
		   અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		   hb_scalar_cache_t *cache = nullptr) અચલ
  {
    unsigned પૂર્ણાંક outer = index >> 16;
    unsigned પૂર્ણાંક inner = index & 0xFFFF;
    પાછા get_delta<Float> (outer, inner, coords, coord_ગણતરી, cache);
  }
  template <typename Float = દશાંશ>
  Float get_delta (unsigned પૂર્ણાંક index,
		   hb_array_t<અચલ પૂર્ણાંક> coords,
		   hb_scalar_cache_t *cache = nullptr) અચલ
  {
    પાછા get_delta<Float> (index,
		      coords.arrayZ, coords.length,
		      cache);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
#ifdef HB_NO_VAR
    પાછા સાચું;
#endif

    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, આ) &&
		  dataSets.sanitize (c, આ));
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
                  તાર્કિક has_long,
                  અચલ hb_vector_t<hb_tag_t>& axis_tags,
                  અચલ hb_vector_t<અચલ hb_hashmap_t<hb_tag_t, Triple>*>& region_list,
                  અચલ hb_vector_t<delta_row_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (આ);
#ifdef HB_NO_VAR
    return_trace (ખોટું);
#endif
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    format = 1;
    જો (!regions.serialize_serialize (c, axis_tags, region_list))
      return_trace (ખોટું);

    unsigned અંક_var_data = vardata_encodings.length;
    જો (!અંક_var_data) return_trace (ખોટું);
    જો (unlikely (!c->check_assign (dataSets.len, અંક_var_data,
                                    HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ખોટું);

    જો (unlikely (!c->extend (dataSets))) return_trace (ખોટું);
    માટે (unsigned i = 0; i < અંક_var_data; i++)
      જો (!dataSets[i].serialize_serialize (c, has_long, vardata_encodings[i].વસ્તુઓ))
        return_trace (ખોટું);

    return_trace (સાચું);
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ ItemVariationStore *src,
		  અચલ hb_array_t <અચલ hb_inc_bimap_t> &inner_maps,
		  અચલ hb_set_t *culled_regions = nullptr)
  {
    TRACE_SERIALIZE (આ);
#ifdef HB_NO_VAR
    return_trace (ખોટું);
#endif

    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    unsigned પૂર્ણાંક set_ગણતરી = 0;
    માટે (unsigned પૂર્ણાંક i = 0; i < inner_maps.length; i++)
      જો (inner_maps[i].get_population ())
	set_ગણતરી++;

    format = 1;

    અચલ auto &src_regions = src+src->regions;

    hb_set_t region_indices;
    માટે (unsigned પૂર્ણાંક i = 0; i < inner_maps.length; i++)
      (src+src->dataSets[i]).collect_region_refs (region_indices, inner_maps[i]);

    જો (region_indices.in_error ())
      return_trace (ખોટું);

    region_indices.del_range ((src_regions).region_ગણતરી, hb_set_t::INVALID);

    જો (culled_regions)
      region_indices.subtract (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (unsigned _) { region_map.add(_); })
    ;
    જો (region_map.in_error())
      return_trace (ખોટું);

    જો (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (ખોટું);

    dataSets.len = set_ગણતરી;
    જો (unlikely (!c->extend (dataSets))) return_trace (ખોટું);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    unsigned પૂર્ણાંક set_index = 0;
    માટે (unsigned પૂર્ણાંક i = 0; i < inner_maps.length; i++)
    {
      જો (!inner_maps[i].get_population ()) ચાલુ;
      જો (unlikely (!dataSets[set_index++]
		     .serialize_serialize (c, &(src+src->dataSets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  ItemVariationStore *copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    auto *out = c->start_embed (આ);
    જો (unlikely (!out)) return_trace (nullptr);

    hb_vector_t <hb_inc_bimap_t> inner_maps;
    unsigned ગણતરી = dataSets.len;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      જો (unlikely (!c->propagate_error(inner_maps)))
        return_trace(nullptr);
      auto &data = આ+dataSets[i];

      unsigned item_ગણતરી = data.get_item_ગણતરી ();
      માટે (unsigned j = 0; j < item_ગણતરી; j++)
	map->add (j);
    }

    જો (unlikely (!out->serialize (c, આ, inner_maps))) return_trace (nullptr);

    return_trace (out);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  તાર્કિક collect_dead_regions (અચલ hb_map_t &axes_old_index_tag_map,
			     અચલ hb_hashmap_t<hb_tag_t, Triple> &reachable_ranges,
			     hb_set_t &dead_regions /* OUT */) અચલ
  {
#ifndef HB_NO_VAR
    જો (!reachable_ranges.get_population ()) પાછા સાચું;
    અચલ VarRegionList &region_list = આ+regions;
    unsigned ગણતરી = region_list.region_ગણતરી;
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      hb_hashmap_t<hb_tag_t, Triple> axis_tuples;
      જો (!region_list.get_var_region (i, axes_old_index_tag_map, axis_tuples))
	પાછા ખોટું;
      જો (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.add (i);
    }
    પાછા !dead_regions.in_error ();
#નહીંતર
    પાછા સાચું;
#endif
  }

  તાર્કિક subset (hb_subset_context_t *c, અચલ hb_array_t<અચલ hb_inc_bimap_t> &inner_maps) અચલ
  {
    TRACE_SUBSET (આ);
#ifdef HB_NO_VAR
    return_trace (ખોટું);
#endif

    ItemVariationStore *varstore_અવિભાજ્ય = c->serializer->start_embed<ItemVariationStore> ();
    જો (unlikely (!varstore_અવિભાજ્ય)) return_trace (ખોટું);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_set_t dead_regions;
    જો (c->plan->has_avar2)
      જો (!collect_dead_regions (c->plan->axes_old_index_tag_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (ખોટું);

    varstore_અવિભાજ્ય->serialize (c->serializer, આ, inner_maps,
			       dead_regions.get_population () ? &dead_regions : nullptr);

    return_trace (
        !c->serializer->in_error()
        && varstore_અવિભાજ્ય->dataSets);
  }

  unsigned પૂર્ણાંક get_region_index_ગણતરી (unsigned પૂર્ણાંક major) અચલ
  {
#ifdef HB_NO_VAR
    પાછા 0;
#endif
    પાછા (આ+dataSets[major]).get_region_index_ગણતરી ();
  }

  શૂન્ય get_region_scalars (unsigned પૂર્ણાંક major,
			   અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
			   દશાંશ *scalars /*OUT*/,
			   unsigned પૂર્ણાંક અંક_scalars) અચલ
  {
#ifdef HB_NO_VAR
    માટે (unsigned i = 0; i < અંક_scalars; i++)
      scalars[i] = 0.f;
    પાછા;
#endif

    (આ+dataSets[major]).get_region_scalars (coords, coord_ગણતરી,
					       આ+regions,
					       &scalars[0], અંક_scalars);
  }

  unsigned પૂર્ણાંક get_sub_table_ગણતરી () અચલ
   {
#ifdef HB_NO_VAR
     પાછા 0;
#endif
     પાછા dataSets.len;
   }

  અચલ VarData& get_sub_table (unsigned i) અચલ
  {
#ifdef HB_NO_VAR
     પાછા Null (VarData);
#endif
     પાછા આ+dataSets[i];
  }

  અચલ VarRegionList& get_region_list () અચલ
  {
#ifdef HB_NO_VAR
     પાછા Null (VarRegionList);
#endif
     પાછા આ+regions;
  }

  સુરક્ષિત:
  HBUINT16				format;
  Offset32To<VarRegionList>		regions;
  Array16OfOffset32To<VarData>		dataSets;
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

સંરચના MultiItemVariationStore
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = nullptr) અચલ
  {
#ifdef HB_NO_VAR
    પાછા hb_scalar_cache_t::create (0);
#endif
    auto &r = આ+regions;
    unsigned ગણતરી = r.regions.len;

    પાછા hb_scalar_cache_t::create (ગણતરી, static_cache);
  }

  સ્થિર શૂન્ય destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = nullptr)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  ખાનગી:
  શૂન્ય get_delta (unsigned પૂર્ણાંક outer, unsigned પૂર્ણાંક inner,
		  અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		  hb_array_t<દ્વિગુણ> out,
		  hb_scalar_cache_t *cache = nullptr) અચલ
  {
#ifdef HB_NO_VAR
    પાછા;
#endif

    જો (unlikely (outer >= dataSets.len))
      પાછા;

    પાછા (આ+dataSets[outer]).get_delta (inner,
					     coords, coord_ગણતરી,
					     આ+regions,
					     out,
					     cache);
  }

  જાહેર:
  શૂન્ય get_delta (unsigned પૂર્ણાંક index,
		  અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_ગણતરી,
		  hb_array_t<દ્વિગુણ> out,
		  hb_scalar_cache_t *cache = nullptr) અચલ
  {
    unsigned પૂર્ણાંક outer = index >> 16;
    unsigned પૂર્ણાંક inner = index & 0xFFFF;
    get_delta (outer, inner, coords, coord_ગણતરી, out, cache);
  }
  શૂન્ય get_delta (unsigned પૂર્ણાંક index,
		  hb_array_t<અચલ પૂર્ણાંક> coords,
		  hb_array_t<દ્વિગુણ> out,
		  hb_scalar_cache_t *cache = nullptr) અચલ
  {
    પાછા get_delta (index,
		      coords.arrayZ, coords.length,
		      out,
		      cache);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
#ifdef HB_NO_VAR
    પાછા સાચું;
#endif

    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, આ) &&
		  dataSets.sanitize (c, આ));
  }

  તાર્કિક create_subset_plan (અચલ hb_set_t &var_indices,
			   hb_vector_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) અચલ
  {
    જો (unlikely (!inner_maps->resize (dataSets.len))) પાછા ખોટું;
    માટે (hb_codepoint_t var_idx : var_indices)
    {
      unsigned outer = var_idx >> 16;
      unsigned inner = var_idx & 0xFFFFu;
      જો (unlikely (outer >= dataSets.len ||
		    inner >= (આ+dataSets[outer]).get_item_ગણતરી ()))
	પાછા ખોટું;
      (*inner_maps)[outer].add (inner);
    }

    unsigned new_outer = 0;
    માટે (unsigned outer = 0; outer < inner_maps->length; outer++)
    {
      અચલ hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      જો (!inner_map.get_population ()) ચાલુ;
      જો (unlikely (inner_map.in_error ())) પાછા ખોટું;
      માટે (unsigned new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	unsigned old_inner = inner_map.backward (new_inner);
	varidx_map->set ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    પાછા !inner_maps->in_error () && !varidx_map->in_error ();
  }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ MultiItemVariationStore *src,
		  અચલ hb_array_t<અચલ hb_inc_bimap_t> &inner_maps,
		  અચલ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ) ||
		  inner_maps.length < src->dataSets.len))
      return_trace (ખોટું);
    format = 1;

    અચલ auto &src_regions = src+src->regions;
    hb_set_t region_indices;
    unsigned set_ગણતરી = 0;
    માટે (unsigned i = 0; i < src->dataSets.len; i++)
    {
      જો (!inner_maps[i].get_population ()) ચાલુ;
      set_ગણતરી++;
      (src+src->dataSets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_set_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.add_set (&region_indices);
    જો (unlikely (region_indices.in_error () || region_map.in_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (ખોટું);

    dataSets.len = set_ગણતરી;
    જો (unlikely (!c->extend (dataSets))) return_trace (ખોટું);
    unsigned set_index = 0;
    માટે (unsigned i = 0; i < src->dataSets.len; i++)
    {
      જો (!inner_maps[i].get_population ()) ચાલુ;
      જો (unlikely (!dataSets[set_index++].serialize_serialize (
			    c, &(src+src->dataSets[i]), inner_maps[i], region_map)))
	return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  સુરક્ષિત:
  HBUINT16				format; // 1
  Offset32To<SparseVarRegionList>	regions;
  Array16OfOffset32To<MultiVarData>	dataSets;
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (8, dataSets);
};

template <typename Map_ગણતરી_T>
સંરચના DeltaSetIndexMapFormat01
{
  friend સંરચના DeltaSetIndexMap;

  size_t get_size () અચલ
  { પાછા hb_unsigned_mul_add_saturate (map_ગણતરી_2, get_width (), min_size); }

  ખાનગી:
  DeltaSetIndexMapFormat01* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (આ));
  }

  template <typename T>
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ T &plan)
  {
    unsigned પૂર્ણાંક width = plan.get_width ();
    unsigned પૂર્ણાંક inner_bit_ગણતરી = plan.get_inner_bit_ગણતરી ();
    અચલ hb_array_t<અચલ uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (આ);
    જો (unlikely (output_map.length && ((((inner_bit_ગણતરી-1)&~0xF)!=0) || (((width-1)&~0x3)!=0))))
      return_trace (ખોટું);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);

    entryFormat = ((width-1)<<4)|(inner_bit_ગણતરી-1);
    map_ગણતરી_2 = output_map.length;
    HBUINT8 *p = c->allocate_size<HBUINT8> (width * output_map.length);
    જો (unlikely (!p)) return_trace (ખોટું);
    માટે (unsigned પૂર્ણાંક i = 0; i < output_map.length; i++)
    {
      unsigned પૂર્ણાંક v = output_map.arrayZ[i];
      જો (v)
      {
	unsigned પૂર્ણાંક outer = v >> 16;
	unsigned પૂર્ણાંક inner = v & 0xFFFF;
	unsigned પૂર્ણાંક u = (outer << inner_bit_ગણતરી) | inner;
	માટે (unsigned પૂર્ણાંક w = width; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += width;
    }
    return_trace (સાચું);
  }

  HB_ALWAYS_INLINE
  uint32_t map (unsigned પૂર્ણાંક v) અચલ /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    જો (!map_ગણતરી_2)
      પાછા v;
    પાછા _map (v);
  }

  HB_HOT
  uint32_t _map (unsigned પૂર્ણાંક v) અચલ /* Returns 16.16 outer.inner. */
  {
    જો (v >= map_ગણતરી_2)
      v = map_ગણતરી_2 - 1;

    unsigned પૂર્ણાંક u = 0;
    { /* Fetch it. */
      unsigned પૂર્ણાંક w = get_width ();
      અચલ HBUINT8 *p = mapDataZ.arrayZ + w * v;
      માટે (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      unsigned પૂર્ણાંક n = get_inner_bit_ગણતરી ();
      unsigned પૂર્ણાંક outer = u >> n;
      unsigned પૂર્ણાંક inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    પાછા u;
  }

  unsigned get_map_ગણતરી () અચલ       { પાછા map_ગણતરી_2; }
  unsigned get_width () અચલ           { પાછા ((entryFormat >> 4) & 3) + 1; }
  unsigned get_inner_bit_ગણતરી () અચલ { પાછા (entryFormat & 0xF) + 1; }


  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  hb_barrier () &&
                  c->check_range (mapDataZ.arrayZ,
                                  map_ગણતરી_2,
                                  get_width ()));
  }

  સુરક્ષિત:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       entryFormat;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_ગણતરી_T     map_ગણતરી_2;       /* The number of mapping entries. */
  UnsizedArrayOf<HBUINT8>
                mapDataZ;       /* The delta-set index mapping data. */

  જાહેર:
  DEFINE_SIZE_ARRAY (2+Map_ગણતરી_T::static_size, mapDataZ);
};

સંરચના DeltaSetIndexMap
{
  template <typename T>
  તાર્કિક serialize (hb_serialize_context_t *c, અચલ T &plan)
  {
    TRACE_SERIALIZE (આ);
    unsigned length = plan.get_output_map ().length;
    u.format.v = length <= 0xFFFF ? 0 : 1;
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    મૂળભૂત:return_trace (ખોટું);
    }
  }

  uint32_t map (unsigned v) અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); પાછા (u.format0.map (v));
    સ્થિતિ 1: hb_barrier (); પાછા (u.format1.map (v));
    મૂળભૂત:પાછા v;
    }
  }

  unsigned get_map_ગણતરી () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_map_ગણતરી ();
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_map_ગણતરી ();
    મૂળભૂત:પાછા 0;
    }
  }

  unsigned get_width () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_width ();
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_width ();
    મૂળભૂત:પાછા 0;
    }
  }

  unsigned get_inner_bit_ગણતરી () અચલ
  {
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); પાછા u.format0.get_inner_bit_ગણતરી ();
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.get_inner_bit_ગણતરી ();
    મૂળભૂત:પાછા 0;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.format.v.sanitize (c)) return_trace (ખોટું);
    hb_barrier ();
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    સ્થિતિ 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    મૂળભૂત:return_trace (સાચું);
    }
  }

  DeltaSetIndexMap* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    પસંદગી (u.format.v) {
    સ્થિતિ 0: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format0.copy (c)));
    સ્થિતિ 1: hb_barrier (); return_trace (reinterpret_cast<DeltaSetIndexMap *> (u.format1.copy (c)));
    મૂળભૂત:return_trace (nullptr);
    }
  }

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT8 v; }             format;         /* Format identifier */
  DeltaSetIndexMapFormat01<HBUINT16> format0;
  DeltaSetIndexMapFormat01<HBUINT32> format1;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (1, format.v);
};


સંરચના ItemVarStoreInstancer
{
  ItemVarStoreInstancer (અચલ ItemVariationStore *varStore_,
			 અચલ DeltaSetIndexMap *varIdxMap,
			 hb_array_t<અચલ પૂર્ણાંક> coords,
			 hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    જો (!varStore)
      varStore = &Null(ItemVariationStore);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    જો (!hb_any (coords))
      આ->coords = {};
  }

  operator તાર્કિક () અચલ { પાછા varStore && તાર્કિક (coords); }

  દશાંશ operator[] (uint32_t varIdx) અચલ
  { પાછા (*આ) (varIdx); }

  દશાંશ operator() (uint32_t varIdx, unsigned short offset = 0) અચલ
  { પાછા get_delta (varIdx, offset); }

  દ્વિગુણ get_condition_delta (uint32_t varIdx) અચલ
  { પાછા get_delta<દ્વિગુણ> (varIdx); }

  template <typename Float = દશાંશ>
  Float get_delta (uint32_t varIdx, unsigned short offset = 0) અચલ
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   જો (!coords || varIdx == VarIdx::NO_VARIATION)
     પાછા Float (0);

    varIdx += offset;
    જો (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    પાછા varStore->get_delta<Float> (varIdx, coords, cache);
  }

  અચલ ItemVariationStore *varStore;
  અચલ DeltaSetIndexMap *varIdxMap;
  hb_array_t<અચલ પૂર્ણાંક> coords;
  hb_scalar_cache_t *cache;
};

સંરચના MultiItemVarStoreInstancer
{
  MultiItemVarStoreInstancer (અચલ MultiItemVariationStore *varStore,
			      અચલ DeltaSetIndexMap *varIdxMap,
			      hb_array_t<અચલ પૂર્ણાંક> coords,
			      hb_scalar_cache_t *cache = nullptr) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    જો (!varStore)
      varStore = &Null(MultiItemVariationStore);
  }

  operator તાર્કિક () અચલ { પાછા varStore; }

  દ્વિગુણ get_condition_delta (uint32_t varIdx) અચલ
  { પાછા (દ્વિગુણ) (*આ)[varIdx]; }

  દ્વિગુણ operator[] (uint32_t varIdx) અચલ
  {
    દ્વિગુણ v = 0;
    (*આ) (hb_array (&v, 1), varIdx);
    પાછા v;
  }

  શૂન્ય operator() (hb_array_t<દ્વિગુણ> out, uint32_t varIdx, unsigned short offset = 0) અચલ
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    જો (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += offset;
      જો (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, out, cache);
    }
    નહીંતર
      માટે (unsigned i = 0; i < out.length; i++)
        out.arrayZ[i] = 0.;
  }

  અચલ MultiItemVariationStore *varStore;
  અચલ DeltaSetIndexMap *varIdxMap;
  hb_array_t<અચલ પૂર્ણાંક> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
ગણના Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_RECORD_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_RECORD_WITH_VAR = 3,
};

ગણના lookup_condition_subset_result_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

સંરચના Condition;

template <typename Instancer>
સ્થિર તાર્કિક
_hb_recurse_condition_evaluate (અચલ સંરચના Condition &condition,
				અચલ પૂર્ણાંક *coords,
				unsigned પૂર્ણાંક coord_len,
				Instancer *instancer);

સંરચના ConditionAxisRange
{
  friend સંરચના Condition;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    અચલ hb_map_t *index_map = &c->plan->axes_index_map;
    જો (index_map->is_empty ()) return_trace (સાચું);

    અચલ hb_map_t& axes_old_index_tag_map = c->plan->axes_old_index_tag_map;
    hb_codepoint_t *axis_tag;
    જો (!axes_old_index_tag_map.has (axisIndex, &axis_tag) ||
        !index_map->has (axisIndex))
      return_trace (ખોટું);

    અચલ hb_hashmap_t<hb_tag_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_limit{-1.0, 0.0, 1.0};
    Triple *normalized_limit;
    જો (normalized_axes_location.has (*axis_tag, &normalized_limit))
      axis_limit = *normalized_limit;

    અચલ hb_hashmap_t<hb_tag_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    જો (axes_triple_distances.has (*axis_tag, &triple_dists))
      axis_triple_distances = *triple_dists;

    દશાંશ normalized_min = renormalize_કિંમત ((દ્વિગુણ) filter_Range_Min_કિંમત.to_float (), axis_limit, axis_triple_distances, ખોટું);
    દશાંશ normalized_max = renormalize_કિંમત ((દ્વિગુણ) filter_Range_Max_કિંમત.to_float (), axis_limit, axis_triple_distances, ખોટું);
    out->filter_Range_Min_કિંમત.set_float (normalized_min);
    out->filter_Range_Max_કિંમત.set_float (normalized_max);

    return_trace (c->serializer->check_assign (out->axisIndex, index_map->get (axisIndex),
                                               HB_SERIALIZE_ERROR_INT_OVERFLOW));
  }

  ખાનગી:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */) અચલ
  {
    //invalid axis index, drop the entire record
    જો (!c->axes_index_tag_map->has (axisIndex))
      પાછા DROP_RECORD_WITH_VAR;

    hb_tag_t axis_tag = c->axes_index_tag_map->get (axisIndex);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_limit;
    તાર્કિક axis_set_by_user = ખોટું;
    જો (c->axes_location->has (axis_tag, &axis_limit))
    {
      axis_range = *axis_limit;
      axis_set_by_user = સાચું;
    }

    દશાંશ axis_min_val = axis_range.minimum;
    દશાંશ axis_default_val = axis_range.મધ્ય;
    દશાંશ axis_max_val = axis_range.maximum;

    દશાંશ filter_min_val = filter_Range_Min_કિંમત.to_float ();
    દશાંશ filter_max_val = filter_Range_Max_કિંમત.to_float ();

    જો (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = ખોટું;

    //condition not met, drop the entire record
    જો (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      પાછા DROP_RECORD_WITH_VAR;

    //condition met and axis pinned, drop the condition
    જો (axis_set_by_user && axis_range.is_point ())
      પાછા DROP_COND_WITH_VAR;

    જો (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_કિંમત.to_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_કિંમત.to_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->set (axisIndex, val);
      પાછા KEEP_COND_WITH_VAR;
    }
    પાછા KEEP_RECORD_WITH_VAR;
  }

  template <typename Instancer>
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer HB_UNUSED) અચલ
  {
    પૂર્ણાંક coord = axisIndex < coord_len ? coords[axisIndex] : 0;
    પાછા filter_Range_Min_કિંમત.to_int () <= coord && coord <= filter_Range_Max_કિંમત.to_int ();
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axisIndex;
  F2DOT14	filter_Range_Min_કિંમત;
  F2DOT14	filter_Range_Max_કિંમત;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના Condition_કિંમત
{
  friend સંરચના Condition;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    જો (varIdx == VarIdx::NO_VARIATION)
      return_trace (સાચું);

    hb_pair_t<unsigned, પૂર્ણાંક> *condition_કિંમત;
    જો (c->plan->layout_condition_idx_કિંમત_map.has ((uintptr_t) આ, &condition_કિંમત))
    {
      out->default_કિંમત_2 = condition_કિંમત->second;
      out->varIdx = condition_કિંમત->first;
      return_trace (સાચું);
    }

    hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
    જો (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (ખોટું);

    જો (unlikely (!c->serializer->check_assign (
		out->default_કિંમત_2,
		default_કિંમત_2 + hb_second (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ખોટું);
    out->varIdx = hb_first (*new_varidx_delta);
    return_trace (સાચું);
  }

  ખાનગી:
  template <typename Instancer>
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer) અચલ
  {
    દ્વિગુણ value = default_કિંમત_2;
    value += instancer->get_condition_delta (varIdx);
    પાછા value > 0;
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_કિંમત_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના ConditionAnd
{
  friend સંરચના Condition;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ખોટું);
    out->format = format;

    માટે (અચલ auto &offset : conditions)
    {
      જો (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	જો (unlikely (!out_offset)) return_trace (ખોટું);
	*out_offset = 0;
      }
      નહીંતર જો (unlikely (!subset_offset_array (c, out->conditions, આ) (offset)))
	return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  ખાનગી:
  template <typename Instancer>
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = conditions.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!_hb_recurse_condition_evaluate (આ+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	પાછા ખોટું;
    પાછા સાચું;
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (conditions.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8OfOffset24To<સંરચના Condition>	conditions;
  જાહેર:
  DEFINE_SIZE_ARRAY (3, conditions);
};

સંરચના ConditionOr
{
  friend સંરચના Condition;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ખોટું);
    out->format = format;

    માટે (અચલ auto &offset : conditions)
    {
      જો (offset.is_null ())
      {
	auto *out_offset = out->conditions.serialize_append (c->serializer);
	જો (unlikely (!out_offset)) return_trace (ખોટું);
	*out_offset = 0;
      }
      નહીંતર જો (unlikely (!subset_offset_array (c, out->conditions, આ) (offset)))
	return_trace (ખોટું);
    }

    return_trace (સાચું);
  }

  ખાનગી:
  template <typename Instancer>
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = conditions.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (_hb_recurse_condition_evaluate (આ+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	પાછા સાચું;
    પાછા ખોટું;
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (conditions.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8OfOffset24To<સંરચના Condition>	conditions;
  જાહેર:
  DEFINE_SIZE_ARRAY (3, conditions);
};

સંરચના ConditionNegate
{
  friend સંરચના Condition;

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);
    જો (condition.is_null ())
    {
      out->condition = 0;
      return_trace (સાચું);
    }
    return_trace (out->condition.serialize_subset (c, condition, આ));
  }

  ખાનગી:
  template <typename Instancer>
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer) અચલ
  {
    પાછા !_hb_recurse_condition_evaluate (આ+condition,
					    coords, coord_len,
					    instancer);
  }

  જાહેર:
  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (condition.sanitize (c, આ));
  }

  સુરક્ષિત:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24To<સંરચના Condition>	condition;
  જાહેર:
  DEFINE_SIZE_STATIC (5);
};

સંરચના Condition
{
  friend સંરચના ConditionSet;

  સ્થિર તાર્કિક serialize_constant (hb_serialize_context_t *c, તાર્કિક value)
  {
    auto *out = c->start_embed<Condition_કિંમત> ();
    જો (unlikely (!out || !c->extend_min (out))) પાછા ખોટું;
    out->format = 2;
    out->default_કિંમત_2 = value ? 1 : 0;
    out->varIdx = VarIdx::NO_VARIATION;
    પાછા સાચું;
  }

  lookup_condition_subset_result_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      unsigned depth = HB_MAX_NESTING_LEVEL,
				      તાર્કિક materialize_true = ખોટું) અચલ;

  template <typename Instancer>
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer) અચલ
  {
    પસંદગી (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    સ્થિતિ 0: પાછા સાચું;
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.evaluate (coords, coord_len, instancer);
    સ્થિતિ 2: hb_barrier (); પાછા u.format2.evaluate (coords, coord_len, instancer);
    સ્થિતિ 3: hb_barrier (); પાછા u.format3.evaluate (coords, coord_len, instancer);
    સ્થિતિ 4: hb_barrier (); પાછા u.format4.evaluate (coords, coord_len, instancer);
    સ્થિતિ 5: hb_barrier (); પાછા u.format5.evaluate (coords, coord_len, instancer);
    મૂળભૂત:પાછા ખોટું;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /* OUT */,
                                             unsigned depth = HB_MAX_NESTING_LEVEL) અચલ
  {
    જો (unlikely (!depth))
    {
      c->apply = ખોટું;
      પાછા DROP_RECORD_WITH_VAR;
    }

    પસંદગી (u.format.v) {
    સ્થિતિ 0: પાછા KEEP_RECORD_WITH_VAR;
    સ્થિતિ 1: hb_barrier (); પાછા u.format1.keep_with_variations (c, condition_map);
    સ્થિતિ 2: સ્થિતિ 3: સ્થિતિ 4: સ્થિતિ 5: તોડો;
    મૂળભૂત: c->apply = ખોટું; પાછા DROP_RECORD_WITH_VAR;
    }

    hb_barrier ();
    તાર્કિક applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.length, c->instancer);
    c->apply &= applies;
    જો (c->all_axes_pinned)
      પાછા applies ? DROP_COND_WITH_VAR : DROP_RECORD_WITH_VAR;

    જો (u.format.v == 2)
      પાછા u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_set (u.format2.varIdx) ?
             (applies ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    auto classify_child = [c, condition_map, depth] (અચલ Condition &child)
    {
      auto child_context = *c;
      child_context.apply = સાચું;
      પાછા child.keep_with_variations (&child_context, condition_map, depth - 1);
    };

    જો (u.format.v == 5)
    {
      auto result = classify_child (&u.format5 + u.format5.condition);
      પાછા result == DROP_RECORD_WITH_VAR ? KEEP_RECORD_WITH_VAR :
             result == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_RECORD_WITH_VAR;
    }

    તાર્કિક is_and = u.format.v == 3;
    તાર્કિક variable = ખોટું;
    અચલ auto &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    અચલ શૂન્ય *base = is_and ? (અચલ શૂન્ય *) &u.format3 : (અચલ શૂન્ય *) &u.format4;
    માટે (અચલ auto &offset : conditions)
    {
      auto result = classify_child (base + offset);
      જો (is_and && result == DROP_RECORD_WITH_VAR)
        પાછા DROP_RECORD_WITH_VAR;
      જો (!is_and && (result == DROP_COND_WITH_VAR || result == KEEP_RECORD_WITH_VAR))
        પાછા KEEP_RECORD_WITH_VAR;
      variable |= result == KEEP_COND_WITH_VAR;
    }
    પાછા variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_RECORD_WITH_VAR : DROP_RECORD_WITH_VAR;
  }

  template <typename context_t, typename ...Ts>
  typename context_t::return_t dispatch (context_t *c, Ts&&... ds) અચલ
  {
    જો (unlikely (!c->may_dispatch (આ, &u.format.v))) પાછા c->no_dispatch_return_કિંમત ();
    TRACE_DISPATCH (આ, u.format.v);
    પસંદગી (u.format.v) {
    સ્થિતિ 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    સ્થિતિ 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    સ્થિતિ 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    સ્થિતિ 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    સ્થિતિ 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    મૂળભૂત:return_trace (c->default_return_કિંમત ());
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);

    જો (unlikely (!c->check_start_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_કિંમત ());

    return_trace (c->end_recursion (આ->dispatch (c)));
  }

  તાર્કિક collect_var_indices (hb_set_t *var_indices,
			    unsigned depth = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, પૂર્ણાંક>> *condition_કિંમતો = nullptr) અચલ;

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ Condition *src,
		  અચલ hb_map_t &varidx_map,
		  અચલ hb_map_t *axis_map = nullptr);

  ખાનગી:
  lookup_condition_subset_result_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      unsigned depth) અચલ;

  સુરક્ષિત:
  સંઘ {
  સંરચના { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_કિંમત	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (2, format.v);
};

template <typename Instancer>
તાર્કિક
_hb_recurse_condition_evaluate (અચલ સંરચના Condition &condition,
				અચલ પૂર્ણાંક *coords,
				unsigned પૂર્ણાંક coord_len,
				Instancer *instancer)
{
  પાછા condition.evaluate (coords, coord_len, instancer);
}

સંરચના ConditionList
{
  અચલ Condition& operator[] (unsigned i) અચલ
  { પાછા આ+conditions[i]; }

  unsigned get_ગણતરી () અચલ { પાછા conditions.len; }

  તાર્કિક serialize (hb_serialize_context_t *c,
		  અચલ ConditionList *src,
		  અચલ hb_inc_bimap_t &condition_map,
		  અચલ hb_map_t &varidx_map,
		  અચલ hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (આ);
    જો (unlikely (!c->extend_min (આ))) return_trace (ખોટું);
    conditions.len = condition_map.get_population ();
    જો (unlikely (!c->extend (conditions))) return_trace (ખોટું);
    માટે (unsigned i = 0; i < conditions.len; i++)
    {
      unsigned old_index = condition_map.backward (i);
      જો (unlikely (old_index >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_index]), varidx_map, axis_map)))
	return_trace (ખોટું);
    }
    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (conditions.sanitize (c, આ));
  }

  સુરક્ષિત:
  Array32OfOffset32To<Condition> conditions;
  જાહેર:
  DEFINE_SIZE_ARRAY (4, conditions);
};

અંતઃરેખ તાર્કિક
Condition::collect_var_indices (hb_set_t *var_indices, unsigned depth,
                               hb_hashmap_t<uintptr_t, hb_pair_t<unsigned, પૂર્ણાંક>> *condition_કિંમતો) અચલ
{
  જો (unlikely (!depth)) પાછા ખોટું;
  પસંદગી (u.format.v)
  {
    સ્થિતિ 0:
    સ્થિતિ 1:
      પાછા સાચું;
    સ્થિતિ 2:
      જો (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->add (u.format2.varIdx);
        જો (condition_કિંમતો &&
            !condition_કિંમતો->set ((uintptr_t) &u.format2,
                                    hb_pair (unsigned (u.format2.varIdx), પૂર્ણાંક (u.format2.default_કિંમત_2))))
          પાછા ખોટું;
      }
      પાછા !var_indices->in_error ();
    સ્થિતિ 3:
      માટે (અચલ auto &offset : u.format3.conditions)
	જો (unlikely (!(&u.format3+offset).collect_var_indices (var_indices,
							     depth - 1, condition_કિંમતો)))
	  પાછા ખોટું;
      પાછા સાચું;
    સ્થિતિ 4:
      માટે (અચલ auto &offset : u.format4.conditions)
	જો (unlikely (!(&u.format4+offset).collect_var_indices (var_indices,
							     depth - 1, condition_કિંમતો)))
	  પાછા ખોટું;
      પાછા સાચું;
    સ્થિતિ 5:
      પાછા (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  depth - 1, condition_કિંમતો);
    મૂળભૂત:
      પાછા ખોટું;
  }
}

અંતઃરેખ તાર્કિક
Condition::serialize (hb_serialize_context_t *c,
		      અચલ Condition *src,
		      અચલ hb_map_t &varidx_map,
		      અચલ hb_map_t *axis_map)
{
  TRACE_SERIALIZE (આ);
  પસંદગી (src->u.format.v)
  {
    સ્થિતિ 0:
    {
      auto *out = c->start_embed<Condition_કિંમત> ();
      જો (unlikely (!out || !c->extend_min (out))) return_trace (ખોટું);
      out->format = 2;
      out->default_કિંમત_2 = 1;
      out->varIdx = VarIdx::NO_VARIATION;
      return_trace (સાચું);
    }
    સ્થિતિ 1:
    {
      auto *out = c->embed (&src->u.format1);
      જો (unlikely (!out)) return_trace (ખોટું);
      જો (axis_map)
      {
	જો (unlikely (!axis_map->has (out->axisIndex)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (ખોટું);
	}
	જો (unlikely (!c->check_assign (out->axisIndex, axis_map->get (out->axisIndex),
				      HB_SERIALIZE_ERROR_INT_OVERFLOW)))
	  return_trace (ખોટું);
      }
      return_trace (સાચું);
    }
    સ્થિતિ 2:
    {
      auto *out = c->embed (&src->u.format2);
      જો (unlikely (!out ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (ખોટું);
      out->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (સાચું);
    }
    સ્થિતિ 3:
    {
      auto *out = c->start_embed<ConditionAnd> ();
      જો (unlikely (!c->extend_min (out))) return_trace (ખોટું);
      out->format = 3;
      out->conditions.len = src->u.format3.conditions.len;
      જો (unlikely (!c->extend (out->conditions))) return_trace (ખોટું);
      માટે (unsigned i = 0; i < out->conditions.len; i++)
	જો (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ખોટું);
      return_trace (સાચું);
    }
    સ્થિતિ 4:
    {
      auto *out = c->start_embed<ConditionOr> ();
      જો (unlikely (!c->extend_min (out))) return_trace (ખોટું);
      out->format = 4;
      out->conditions.len = src->u.format4.conditions.len;
      જો (unlikely (!c->extend (out->conditions))) return_trace (ખોટું);
      માટે (unsigned i = 0; i < out->conditions.len; i++)
	જો (unlikely (!out->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (ખોટું);
      return_trace (સાચું);
    }
    સ્થિતિ 5:
    {
      auto *out = c->embed (&src->u.format5);
      જો (unlikely (!out ||
		    !out->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (ખોટું);
      return_trace (સાચું);
    }
    મૂળભૂત:
      return_trace (ખોટું);
  }
}

અંતઃરેખ lookup_condition_subset_result_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    unsigned depth,
				    તાર્કિક materialize_true) અચલ
{
  auto snap = c->serializer->snapshot ();
  lookup_condition_subset_result_t result =
      subset_lookup_condition_impl (c, depth);
  જો (result == LOOKUP_CONDITION_SUBSET_KEEP)
    પાછા result;

  c->serializer->revert (snap);
  જો (result != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    પાછા result;

  પાછા serialize_constant (c->serializer, સાચું) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

અંતઃરેખ lookup_condition_subset_result_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 unsigned depth) અચલ
{
  જો (unlikely (!depth))
    પાછા LOOKUP_CONDITION_SUBSET_ERROR;

  auto subset_offset = [c, depth] (Array8OfOffset24To<Condition> &out,
				   અચલ Condition &condition)
      -> lookup_condition_subset_result_t
  {
    auto snap = c->serializer->snapshot ();
    auto *offset = out.serialize_append (c->serializer);
    જો (unlikely (!offset))
      પાછા LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_result_t result =
        condition.subset_lookup_condition (c, depth - 1);
    જો (result == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->add_link (*offset, c->serializer->કાઢો_pack ());
    નહીંતર
    {
      c->serializer->કાઢો_discard ();
      out.કાઢો ();
      c->serializer->revert (snap);
    }
    પાછા result;
  };

  પસંદગી (u.format.v)
  {
    સ્થિતિ 0:
      પાછા LOOKUP_CONDITION_SUBSET_TRUE;

    સ્થિતિ 1:
    {
      અચલ ConditionAxisRange &src = u.format1;
      જો (c->plan->user_axes_location.is_empty ())
        પાછા c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_tag;
      જો (!c->plan->axes_old_index_tag_map.has (src.axisIndex, &axis_tag))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_limit {-1.f, 0.f, 1.f};
      Triple *normalized_limit;
      જો (c->plan->axes_location.has (*axis_tag, &normalized_limit))
        axis_limit = *normalized_limit;

      દશાંશ filter_min = src.filter_Range_Min_કિંમત.to_float ();
      દશાંશ filter_max = src.filter_Range_Max_કિંમત.to_float ();
      જો (filter_min > filter_max)
        પાછા LOOKUP_CONDITION_SUBSET_FALSE;
      જો (axis_limit.is_point ())
        પાછા filter_min <= axis_limit.મધ્ય &&
	       axis_limit.મધ્ય <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      જો (axis_limit.maximum < filter_min ||
	  filter_max < axis_limit.minimum)
        પાછા LOOKUP_CONDITION_SUBSET_FALSE;
      જો (filter_min <= axis_limit.minimum &&
	  axis_limit.maximum <= filter_max)
        પાછા LOOKUP_CONDITION_SUBSET_TRUE;
      જો (!c->plan->axes_index_map.has (src.axisIndex))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;

      auto *out = c->serializer->embed (&src);
      જો (unlikely (!out))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      જો (c->plan->axes_triple_distances.has (*axis_tag, &distances))
        axis_distances = *distances;

      out->filter_Range_Min_કિંમત.set_float (
	  renormalize_કિંમત (filter_min, axis_limit, axis_distances, ખોટું));
      out->filter_Range_Max_કિંમત.set_float (
	  renormalize_કિંમત (filter_max, axis_limit, axis_distances, ખોટું));
      જો (unlikely (!c->serializer->check_assign (
		out->axisIndex,
		c->plan->axes_index_map.get (src.axisIndex),
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;
      પાછા LOOKUP_CONDITION_SUBSET_KEEP;
    }

    સ્થિતિ 2:
    {
      અચલ Condition_કિંમત &src = u.format2;
      auto *out = c->serializer->embed (&src);
      જો (unlikely (!out))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;

      જો (src.varIdx == VarIdx::NO_VARIATION)
        પાછા c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((પૂર્ણાંક) src.default_કિંમત_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<unsigned, પૂર્ણાંક> *condition_કિંમત;
      જો (c->plan->layout_condition_idx_કિંમત_map.has ((uintptr_t) &src, &condition_કિંમત))
      {
        out->default_કિંમત_2 = condition_કિંમત->second;
        out->varIdx = condition_કિંમત->first;
        જો (out->varIdx == VarIdx::NO_VARIATION)
          પાછા out->default_કિંમત_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        પાછા LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<unsigned, પૂર્ણાંક> *new_varidx_delta;
      જો (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        પાછા c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((પૂર્ણાંક) src.default_કિંમત_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      જો (!c->plan->user_axes_location.is_empty () &&
          hb_first (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        દ્વિગુણ value = (પૂર્ણાંક) src.default_કિંમત_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        પાછા value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      પૂર્ણાંક default_કિંમત = (પૂર્ણાંક) src.default_કિંમત_2 +
			  hb_second (*new_varidx_delta);
      જો (unlikely (!c->serializer->check_assign (
		out->default_કિંમત_2,
		default_કિંમત,
		HB_SERIALIZE_ERROR_INT_OVERFLOW)))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;
      out->varIdx = hb_first (*new_varidx_delta);

      પાછા LOOKUP_CONDITION_SUBSET_KEEP;
    }

    સ્થિતિ 3:
    {
      અચલ ConditionAnd &src = u.format3;
      auto *out = c->serializer->start_embed (&src);
      જો (unlikely (!out || !c->serializer->extend_min (out)))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 3;

      માટે (અચલ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        જો (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_FALSE)
          પાછા result;
      }
      પાછા out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    સ્થિતિ 4:
    {
      અચલ ConditionOr &src = u.format4;
      auto *out = c->serializer->start_embed (&src);
      જો (unlikely (!out || !c->serializer->extend_min (out)))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;
      out->format = 4;

      માટે (અચલ auto &offset : src.conditions)
      {
        lookup_condition_subset_result_t result =
            subset_offset (out->conditions, &src + offset);
        જો (result == LOOKUP_CONDITION_SUBSET_ERROR ||
	    result == LOOKUP_CONDITION_SUBSET_TRUE)
          પાછા result;
      }
      પાછા out->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    સ્થિતિ 5:
    {
      અચલ ConditionNegate &src = u.format5;
      auto *out = c->serializer->embed (&src);
      જો (unlikely (!out))
        પાછા LOOKUP_CONDITION_SUBSET_ERROR;
      out->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_result_t result =
          (&src + src.condition).subset_lookup_condition (c, depth - 1);
      જો (result == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->add_link (out->condition,
				 c->serializer->કાઢો_pack ());
        પાછા result;
      }

      c->serializer->કાઢો_discard ();
      જો (result == LOOKUP_CONDITION_SUBSET_TRUE)
        પાછા LOOKUP_CONDITION_SUBSET_FALSE;
      જો (result == LOOKUP_CONDITION_SUBSET_FALSE)
        પાછા LOOKUP_CONDITION_SUBSET_TRUE;
      પાછા result;
    }

    મૂળભૂત:
      પાછા LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

સંરચના ConditionSet
{
  તાર્કિક evaluate (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		 ItemVarStoreInstancer *instancer) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = conditions.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
      જો (!(આ+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	પાછા ખોટું;
    પાછા સાચું;
  }

  શૂન્ય keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) અચલ
  {
    hb_map_t *condition_map = hb_map_create ();
    જો (unlikely (!condition_map)) પાછા;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_set_t *cond_set = hb_set_create ();
    જો (unlikely (!cond_set)) પાછા;
    hb::shared_ptr<hb_set_t> s {cond_set};

    c->apply = સાચું;
    તાર્કિક should_keep = ખોટું;
    /* The range map cannot identify value or boolean expressions uniquely. */
    તાર્કિક can_deduplicate = સાચું;
    unsigned અંક_kept_cond = 0, cond_idx = 0;
    માટે (અચલ auto& offset : conditions)
    {
      અચલ Condition &condition = આ+offset;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      જો (ret == DROP_RECORD_WITH_VAR)
      {
        c->apply = ખોટું;
        પાછા;
      }

      જો (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = સાચું;
        cond_set->add (cond_idx);
        અંક_kept_cond++;
      }

      જો (ret == KEEP_RECORD_WITH_VAR)
        should_keep = સાચું;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = અંક_kept_cond == 0;
    જો (!should_keep && c->all_axes_pinned) પાછા;

    //check if condition_set is unique with variations
    જો (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      પાછા;

    જો (can_deduplicate)
      c->conditionset_map->set (p, 1);
    c->record_cond_idx_map->set (c->cur_record_idx, s);
  }

  તાર્કિક subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               તાર્કિક ઉમેરો_catch_all) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    જો (ઉમેરો_catch_all) return_trace (સાચું);

    hb_set_t *retained_cond_set = nullptr;
    જો (l->feature_record_cond_idx_map != nullptr)
      retained_cond_set = l->feature_record_cond_idx_map->get (l->cur_feature_var_record_idx);

    unsigned પૂર્ણાંક ગણતરી = conditions.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (retained_cond_set != nullptr && !retained_cond_set->has (i))
        ચાલુ;
      અચલ Condition &condition = આ+conditions[i];
      જો (condition.u.format.v == 1)
      {
        subset_offset_array (c, out->conditions, આ) (conditions[i]);
        ચાલુ;
      }

      auto *offset = out->conditions.serialize_append (c->serializer);
      જો (unlikely (!offset)) return_trace (ખોટું);
      c->serializer->push ();
      auto result = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, સાચું);
      જો (result == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        result = Condition::serialize_constant (c->serializer, ખોટું) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      જો (unlikely (result == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->કાઢો_discard ();
        c->serializer->check_success (ખોટું);
        return_trace (ખોટું);
      }
      c->serializer->add_link (*offset, c->serializer->કાઢો_pack ());
    }

    return_trace (તાર્કિક (out->conditions));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (conditions.sanitize (c, આ));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ auto &offset : conditions)
      જો (unlikely (!(આ+offset).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_કિંમતો)))
	c->layout_variation_indices->err ();
  }

  સુરક્ષિત:
  Array16OfOffset32To<Condition>	conditions;
  જાહેર:
  DEFINE_SIZE_ARRAY (2, conditions);
};

સંરચના FeatureTableSubstitutionRecord
{
  friend સંરચના FeatureTableSubstitution;

  શૂન્ય collect_lookups (અચલ શૂન્ય *base, hb_set_t *lookup_indexes /* OUT */) અચલ
  {
    પાછા (base+feature).add_lookup_indexes_to (lookup_indexes);
  }

  શૂન્ય closure_features (અચલ શૂન્ય *base,
			 અચલ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) અચલ
  {
    જો ((base+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  શૂન્ય collect_feature_substitutes_with_variations (hb_hashmap_t<unsigned, અચલ Feature*> *feature_substitutes_map,
                                                    hb_set_t& catch_all_record_feature_idxes,
                                                    અચલ hb_set_t *feature_indices,
                                                    અચલ શૂન્ય *base) અચલ
  {
    જો (feature_indices->has (featureIndex))
    {
      feature_substitutes_map->set (featureIndex, &(base+feature));
      catch_all_record_feature_idxes.add (featureIndex);
    }
  }

  તાર્કિક serialize (hb_subset_layout_context_t *c,
                  unsigned feature_index,
                  અચલ Feature *f, અચલ Tag *tag)
  {
    TRACE_SERIALIZE (આ);
    hb_serialize_context_t *s = c->subset_context->serializer;
    જો (unlikely (!s->extend_min (આ))) return_trace (ખોટું);

    uint32_t *new_feature_idx;
    જો (!c->feature_map_w_duplicates->has (feature_index, &new_feature_idx))
      return_trace (ખોટું);

    જો (!s->check_assign (featureIndex, *new_feature_idx, HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (ખોટું);

    s->push ();
    તાર્કિક ret = f->subset (c->subset_context, c, tag);
    જો (ret) s->add_link (feature, s->કાઢો_pack ());
    નહીંતર s->કાઢો_discard ();

    return_trace (ret);
  }

  તાર્કિક subset (hb_subset_layout_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SUBSET (આ);
    uint32_t *new_feature_index;
    જો (!c->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (ખોટું);

    auto *out = c->subset_context->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    out->featureIndex = *new_feature_index;
    hb_pair_t<અચલ શૂન્ય*, અચલ શૂન્ય*> *original;
    જો (!c->feature_idx_tag_map->has (featureIndex, &original))
      return_trace (ખોટું);
    અચલ Tag *tag = reinterpret_cast<અચલ Tag*> (original->second);
    return_trace (out->feature.serialize_subset (c->subset_context, feature, base, c, tag));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base,
		 અચલ FeatureList *feature_list) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (unlikely (!c->check_struct (આ))) return_trace (ખોટું);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    અચલ Record_sanitize_closure_t closure = {feature_list->get_tag (featureIndex), base};
    return_trace (feature.sanitize (c, base, &closure));
  }

  સુરક્ષિત:
  HBUINT16		featureIndex;
  Offset32To<Feature>	feature;
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના FeatureTableSubstitution
{
  શૂન્ય collect_feature_indices (hb_set_t *feature_indices) અચલ
  {
    માટે (અચલ FeatureTableSubstitutionRecord &record : substitutions)
      feature_indices->add (record.featureIndex);
  }

  અચલ Feature *find_substitute (unsigned પૂર્ણાંક feature_index) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = substitutions.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      અચલ FeatureTableSubstitutionRecord &record = substitutions.arrayZ[i];
      જો (record.featureIndex == feature_index)
	પાછા &(આ+record.feature);
    }
    પાછા nullptr;
  }

  શૂન્ય collect_lookups (અચલ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) અચલ
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &FeatureTableSubstitutionRecord::featureIndex)
    | hb_apply ([આ, lookup_indexes] (અચલ FeatureTableSubstitutionRecord& r)
		{ r.collect_lookups (આ, lookup_indexes); })
    ;
  }

  template <typename TagGetter>
  શૂન્ય collect_name_ids (અચલ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) અચલ
  {
    માટે (અચલ FeatureTableSubstitutionRecord &record : substitutions)
      જો (feature_index_map->has (record.featureIndex))
        (આ+record.feature).collect_name_ids (get_tag (record.featureIndex), nameids_to_retain);
  }

  શૂન્ય closure_features (અચલ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) અચલ
  {
    માટે (અચલ FeatureTableSubstitutionRecord& record : substitutions)
      record.closure_features (આ, lookup_indexes, feature_indexes);
  }

  તાર્કિક intersects_features (અચલ hb_map_t *feature_index_map) અચલ
  {
    માટે (અચલ FeatureTableSubstitutionRecord& record : substitutions)
    {
      જો (feature_index_map->has (record.featureIndex)) પાછા સાચું;
    }
    પાછા ખોટું;
  }

  શૂન્ય collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) અચલ
  {
    માટે (અચલ FeatureTableSubstitutionRecord& record : substitutions)
      record.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_record_feature_idxes,
                                                          c->feature_indices, આ);
  }

  તાર્કિક subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               તાર્કિક ઉમેરો_catch_all) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    out->version.major = version.major;
    out->version.minor = version.minor;

    જો (ઉમેરો_catch_all)
    {
      માટે (unsigned feature_index : *(l->catch_all_record_feature_idxes))
      {
        hb_pair_t<અચલ શૂન્ય*, અચલ શૂન્ય*> *p;
        જો (!l->feature_idx_tag_map->has (feature_index, &p))
          return_trace (ખોટું);
        auto *o = out->substitutions.serialize_append (c->serializer);
        જો (!o->serialize (l, feature_index,
                           reinterpret_cast<અચલ Feature*> (p->first),
                           reinterpret_cast<અચલ Tag*> (p->second)))
          return_trace (ખોટું);
      }
      return_trace (સાચું);
    }

    + substitutions.iter ()
    | hb_apply (subset_record_array (l, &(out->substitutions), આ))
    ;

    return_trace (તાર્કિક (out->substitutions));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ FeatureList *feature_list) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  substitutions.sanitize (c, આ, feature_list));
  }

  સુરક્ષિત:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array16Of<FeatureTableSubstitutionRecord>
			substitutions;
  જાહેર:
  DEFINE_SIZE_ARRAY (6, substitutions);
};

સંરચના FeatureVariationRecord
{
  friend સંરચના FeatureVariations;

  શૂન્ય collect_lookups (અચલ શૂન્ય     *base,
			અચલ hb_set_t *feature_indexes,
			hb_set_t       *lookup_indexes /* OUT */) અચલ
  {
    પાછા (base+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  શૂન્ય collect_name_ids (અચલ શૂન્ય *base,
                         અચલ hb_map_t *feature_index_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) અચલ
  {
    (base+substitutions).collect_name_ids (feature_index_map, get_tag, nameids_to_retain);
  }

  શૂન્ય closure_features (અચલ શૂન્ય     *base,
			 અચલ hb_map_t *lookup_indexes,
			 hb_set_t       *feature_indexes /* OUT */) અચલ
  {
    (base+substitutions).closure_features (lookup_indexes, feature_indexes);
  }

  તાર્કિક intersects_features (અચલ શૂન્ય *base, અચલ hb_map_t *feature_index_map) અચલ
  {
    પાછા (base+substitutions).intersects_features (feature_index_map);
  }

  શૂન્ય collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    અચલ શૂન્ય *base) અચલ
  {
    (base+conditions).keep_with_variations (c);
    જો (c->apply && !c->variation_applied)
    {
      (base+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = સાચું; // set variations only once
    }
  }

  તાર્કિક subset (hb_subset_layout_context_t *c, અચલ શૂન્ય *base,
               તાર્કિક ઉમેરો_catch_all = ખોટું) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->subset_context->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    hb_set_t *retained_conditions = c->feature_record_cond_idx_map ?
        c->feature_record_cond_idx_map->get (c->cur_feature_var_record_idx).get () : nullptr;
    જો (!ઉમેરો_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      out->conditions = 0;
    નહીંતર
      out->conditions.serialize_subset (c->subset_context, conditions, base, c, ઉમેરો_catch_all);
    out->substitutions.serialize_subset (c->subset_context, substitutions, base, c, ઉમેરો_catch_all);

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base,
		 અચલ FeatureList *feature_list) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (conditions.sanitize (c, base) &&
		  substitutions.sanitize (c, base, feature_list));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  અચલ શૂન્ય *base) અચલ
  { (base+conditions).collect_variation_indices (c); }

  સુરક્ષિત:
  Offset32To<ConditionSet>
			conditions;
  Offset32To<FeatureTableSubstitution>
			substitutions;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના LookupIndexList
{
  શૂન્ય add_lookup_indexes_to (hb_set_t *lookup_indexes) અચલ
  { lookupIndices.add_indexes_to (lookup_indexes); }

  તાર્કિક intersects_lookup_indexes (અચલ hb_map_t *lookup_indexes) અચલ
  { પાછા lookupIndices.intersects (lookup_indexes); }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ખોટું);

    auto it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_index_map)
    | hb_map (l->lookup_index_map)
    ;
    out->lookupIndices.serialize (c->serializer, l, it);
    return_trace (તાર્કિક (out->lookupIndices));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (lookupIndices.sanitize (c));
  }

  સુરક્ષિત:
  IndexArray	lookupIndices;
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (2, lookupIndices);
};

સંરચના LookupConditionRecord
{
  unsigned get_condition_offset (અચલ શૂન્ય *base, અચલ શૂન્ય *variations) અચલ
  { પાછા (અચલ અક્ષર *) &(base+condition) - (અચલ અક્ષર *) variations; }

  template <typename Instancer>
  તાર્કિક evaluate (અચલ શૂન્ય *base,
		 અચલ પૂર્ણાંક *coords,
		 unsigned પૂર્ણાંક coord_len,
		 Instancer *instancer) અચલ
  { પાછા (base+condition).evaluate (coords, coord_len, instancer); }

  તાર્કિક evaluate (અચલ શૂન્ય *base,
                 અચલ શૂન્ય *variations,
                 hb_sorted_array_t<અચલ unsigned> conditions) અચલ
  {
    પાછા !condition || hb_bsearch (get_condition_offset (base, variations),
                                     conditions.arrayZ, conditions.length,
                                     કદ (unsigned),
                                     _hb_cmp_operator<unsigned, unsigned>);
  }

  તાર્કિક collect_condition (અચલ શૂન્ય *base,
                          અચલ શૂન્ય *variations,
                          અચલ પૂર્ણાંક *coords,
                          unsigned પૂર્ણાંક coord_len,
                          ItemVarStoreInstancer *instancer,
                          hb_set_t *visited,
                          hb_vector_t<unsigned> *conditions) અચલ
  {
    /* Null conditions always apply and need no key state. */
    જો (!condition) પાછા સાચું;

    unsigned offset = get_condition_offset (base, variations);
    જો (visited->has (offset)) પાછા સાચું;
    visited->add (offset);
    જો (unlikely (visited->in_error ())) પાછા ખોટું;

    જો (evaluate (base, coords, coord_len, instancer))
      conditions->push (offset);
    પાછા !conditions->in_error ();
  }

  શૂન્ય add_lookup_indexes_to (અચલ શૂન્ય *base,
			      hb_set_t *lookup_indexes) અચલ
  { (base+lookupIndices).add_lookup_indexes_to (lookup_indexes); }

  તાર્કિક intersects_lookup_indexes (અચલ શૂન્ય *base,
				   અચલ hb_map_t *lookup_indexes) અચલ
  { પાછા (base+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  અચલ શૂન્ય *base) અચલ
  {
    જો (unlikely (!(base+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_કિંમતો)))
      c->layout_variation_indices->err ();
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       અચલ શૂન્ય *base) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out)) return_trace (ખોટું);

    out->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_result_t result =
        (base + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    સાચું);
    જો (result != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->કાઢો_discard ();
      return_trace (ખોટું);
    }
    c->serializer->add_link (out->condition, c->serializer->કાઢો_pack ());

    return_trace (out->lookupIndices.serialize_subset (c, lookupIndices, base, l));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (condition.sanitize (c, base) &&
		  lookupIndices.sanitize (c, base));
  }

  સુરક્ષિત:
  Offset32To<Condition>		condition;
  Offset32To<LookupIndexList>	lookupIndices;
  જાહેર:
  DEFINE_SIZE_STATIC (8);
};

સંરચના FeatureLookupsTable
{
  તાર્કિક adds_default_lookups () અચલ { પાછા flags & 0x0001u; }

  તાર્કિક collect_conditions (અચલ શૂન્ય *variations,
                           અચલ પૂર્ણાંક *coords,
                           unsigned પૂર્ણાંક coord_len,
                           ItemVarStoreInstancer *instancer,
                           hb_set_t *visited,
                           hb_vector_t<unsigned> *conditions) અચલ
  {
    માટે (અચલ LookupConditionRecord &record : records)
      જો (unlikely (!record.collect_condition (આ, variations, coords, coord_len,
                                               instancer, visited, conditions)))
	પાછા ખોટું;
    પાછા સાચું;
  }

  શૂન્ય collect_lookups (અચલ શૂન્ય *variations,
                       hb_sorted_array_t<અચલ unsigned> conditions,
                       hb_set_t *lookup_indexes) અચલ
  {
    માટે (અચલ LookupConditionRecord &record : records)
      જો (record.evaluate (આ, variations, conditions))
	record.add_lookup_indexes_to (આ, lookup_indexes);
  }

  template <typename Instancer>
  શૂન્ય collect_lookups (અચલ પૂર્ણાંક *coords,
			unsigned પૂર્ણાંક coord_len,
			Instancer *instancer,
			hb_set_t *lookup_indexes) અચલ
  {
    માટે (અચલ LookupConditionRecord &record : records)
      જો (record.evaluate (આ, coords, coord_len, instancer))
	record.add_lookup_indexes_to (આ, lookup_indexes);
  }

  શૂન્ય collect_lookups (hb_set_t *lookup_indexes) અચલ
  {
    માટે (અચલ LookupConditionRecord &record : records)
      record.add_lookup_indexes_to (આ, lookup_indexes);
  }

  તાર્કિક intersects_lookup_indexes (અચલ hb_map_t *lookup_indexes) અચલ
  {
    માટે (અચલ LookupConditionRecord &record : records)
      જો (record.intersects_lookup_indexes (આ, lookup_indexes))
	પાછા સાચું;
    પાછા ખોટું;
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ LookupConditionRecord &record : records)
      record.collect_variation_indices (c, આ);
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (આ);
    જો (unlikely (!out || !c->serializer->extend_min (out)))
      return_trace (ખોટું);

    out->version.major = version.major;
    out->version.minor = version.minor;
    out->flags = flags;

    માટે (અચલ LookupConditionRecord &record : records)
    {
      auto snap = c->serializer->snapshot ();
      જો (record.subset (c, l, આ))
	out->records.len++;
      નહીંતર
	c->serializer->revert (snap);
    }

    return_trace (સાચું);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  records.sanitize (c, આ));
  }

  સુરક્ષિત:
  FixedVersion<>	version;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32Of<LookupConditionRecord>
		records;
  જાહેર:
  DEFINE_SIZE_ARRAY (10, records);
};

સંરચના LookupVariationRecord
{
  friend સંરચના FeatureVariations;

  unsigned get_feature_index () અચલ { પાછા featureIndex; }

  શૂન્ય collect_lookups (અચલ શૂન્ય *base,
                       અચલ Feature &feature,
                       hb_sorted_array_t<અચલ unsigned> conditions,
                       hb_set_t *lookup_indexes) અચલ
  {
    અચલ FeatureLookupsTable &feature_lookups = base+featureLookups;
    જો (feature_lookups.adds_default_lookups ())
      feature.add_lookup_indexes_to (lookup_indexes);
    feature_lookups.collect_lookups (base, conditions, lookup_indexes);
  }

  શૂન્ય collect_lookups (અચલ શૂન્ય *base,
			અચલ hb_set_t *feature_indexes,
			hb_set_t *lookup_indexes) અચલ
  {
    જો (feature_indexes->has (featureIndex))
      (base+featureLookups).collect_lookups (lookup_indexes);
  }

  શૂન્ય closure_features (અચલ શૂન્ય *base,
			 અચલ hb_map_t *lookup_indexes,
			 hb_set_t *feature_indexes) અચલ
  {
    જો ((base+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->add (featureIndex);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  અચલ શૂન્ય *base) અચલ
  { (base+featureLookups).collect_variation_indices (c); }

  તાર્કિક subset (hb_subset_layout_context_t *l, અચલ શૂન્ય *base) અચલ
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (આ);
    uint32_t *new_feature_index;
    જો (!l->feature_map_w_duplicates->has (featureIndex, &new_feature_index))
      return_trace (ખોટું);

    auto *out = c->serializer->embed (આ);
    જો (unlikely (!out ||
		  !c->serializer->check_assign (out->featureIndex,
						*new_feature_index,
						HB_SERIALIZE_ERROR_INT_OVERFLOW)))
      return_trace (ખોટું);

    return_trace (out->featureLookups.serialize_subset (c, featureLookups, base, l));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ શૂન્ય *base) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) &&
		  featureLookups.sanitize (c, base));
  }

  સુરક્ષિત:
  HBUINT16			featureIndex;
  Offset32To<FeatureLookupsTable>	featureLookups;
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના FeatureVariations
{
  સ્થિર constexpr unsigned NOT_FOUND_INDEX = 0xFFFFFFFFu;

  અચલ Array32Of<LookupVariationRecord> &get_lookup_variation_records () અચલ
  {
    જો (version.to_int () < 0x00010001u)
      પાછા Null (Array32Of<LookupVariationRecord>);
    પાછા StructAfter<Array32Of<LookupVariationRecord>> (varRecords);
  }

  unsigned record_ગણતરી () અચલ
  {
    પાછા varRecords.len;
  }

  તાર્કિક find_index (અચલ પૂર્ણાંક *coords, unsigned પૂર્ણાંક coord_len,
		   unsigned પૂર્ણાંક *index,
		   ItemVarStoreInstancer *instancer) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = varRecords.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      અચલ FeatureVariationRecord &record = varRecords.arrayZ[i];
      જો ((આ+record.conditions).evaluate (coords, coord_len, instancer))
      {
	*index = i;
	પાછા સાચું;
      }
    }
    *index = NOT_FOUND_INDEX;
    પાછા ખોટું;
  }

  અચલ Feature *find_substitute (unsigned પૂર્ણાંક variations_index,
				  unsigned પૂર્ણાંક feature_index) અચલ
  {
    અચલ FeatureVariationRecord &record = varRecords[variations_index];
    પાછા (આ+record.substitutions).find_substitute (feature_index);
  }

  તાર્કિક collect_lookup_conditions (અચલ પૂર્ણાંક *coords,
                                  unsigned પૂર્ણાંક coord_len,
                                  ItemVarStoreInstancer *instancer,
                                  hb_vector_t<unsigned> *state) અચલ
  {
    hb_set_t visited_tables, visited_conditions;
    માટે (અચલ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      unsigned offset = record.featureLookups;
      જો (visited_tables.has (offset)) ચાલુ;
      visited_tables.add (offset);
      જો (unlikely (visited_tables.in_error ())) પાછા ખોટું;

      અચલ FeatureLookupsTable &feature_lookups = આ+record.featureLookups;
      જો (unlikely (!feature_lookups.collect_conditions (આ, coords, coord_len,
                                                         instancer, &visited_conditions,
                                                         state)))
	પાછા ખોટું;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<unsigned, unsigned>);
    પાછા સાચું;
  }

  તાર્કિક get_lookup_variations (unsigned feature_index,
                              અચલ Feature &feature,
                              hb_sorted_array_t<અચલ unsigned> conditions,
                              hb_set_t *lookup_indexes) અચલ
  {
    માટે (અચલ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      જો (record.featureIndex != feature_index) ચાલુ;

      record.collect_lookups (આ, feature, conditions, lookup_indexes);
      પાછા સાચું;
    }
    પાછા ખોટું;
  }

  શૂન્ય collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = varRecords.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      c->cur_record_idx = i;
      varRecords[i].collect_feature_substitutes_with_variations (c, આ);
      જો (c->universal)
        તોડો;
    }
    જો (c->universal || c->record_cond_idx_map->is_empty ())
      c->catch_all_record_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed (*આ));
  }

  શૂન્ય collect_lookups (અચલ hb_set_t *feature_indexes,
			અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			hb_set_t       *lookup_indexes /* OUT */) અચલ
  {
    unsigned ગણતરી = varRecords.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (feature_record_cond_idx_map &&
          !feature_record_cond_idx_map->has (i))
        ચાલુ;
      varRecords[i].collect_lookups (આ, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  template <typename TagGetter>
  શૂન્ય collect_name_ids (અચલ hb_map_t *feature_index_map,
                         અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
                         TagGetter get_tag,
                         hb_set_t *nameids_to_retain) અચલ
  {
    માટે (unsigned i = 0; i < varRecords.len; i++)
    {
      જો (feature_record_cond_idx_map && !feature_record_cond_idx_map->has (i))
        ચાલુ;
      varRecords[i].collect_name_ids (આ, feature_index_map, get_tag, nameids_to_retain);
    }
  }

  શૂન્ય collect_lookup_variation_lookups (અચલ hb_set_t *feature_indexes,
					 hb_set_t *lookup_indexes /* OUT */) અચલ
  {
    માટે (અચલ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_lookups (આ, feature_indexes, lookup_indexes);
  }

  શૂન્ય closure_features (અચલ hb_map_t *lookup_indexes,
			 અચલ hb_hashmap_t<unsigned, hb::shared_ptr<hb_set_t>> *feature_record_cond_idx_map,
			 hb_set_t       *feature_indexes /* OUT */) અચલ
  {
    unsigned પૂર્ણાંક ગણતરી = varRecords.len;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      જો (feature_record_cond_idx_map != nullptr &&
          !feature_record_cond_idx_map->has (i))
        ચાલુ;
      varRecords[i].closure_features (આ, lookup_indexes, feature_indexes);
    }

    માટે (અચલ LookupVariationRecord &record : get_lookup_variation_records ())
      record.closure_features (આ, lookup_indexes, feature_indexes);
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (અચલ FeatureVariationRecord &record : varRecords)
      record.collect_variation_indices (c, આ);
    માટે (અચલ LookupVariationRecord &record : get_lookup_variation_records ())
      record.collect_variation_indices (c, આ);
  }

  શૂન્ય collect_feature_indices (hb_set_t *feature_indices) અચલ
  {
    માટે (અચલ FeatureVariationRecord &record : varRecords)
      (આ+record.substitutions).collect_feature_indices (feature_indices);
  }

  તાર્કિક has_lookup_variations (unsigned feature_index) અચલ
  {
    માટે (અચલ LookupVariationRecord &record : get_lookup_variation_records ())
    {
      જો (record.featureIndex == feature_index)
	પાછા સાચું;
      જો (record.featureIndex > feature_index)
	તોડો;
    }
    પાછા ખોટું;
  }

  તાર્કિક subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) અચલ
  {
    TRACE_SUBSET (આ);
    auto *out = c->serializer->start_embed (*આ);
    જો (unlikely (!out || !c->serializer->extend_min (out))) return_trace (ખોટું);

    out->version.major = version.major;
    out->version.minor = version.minor;

    પૂર્ણાંક keep_up_to = -1;
    માટે (પૂર્ણાંક i = varRecords.len - 1; i >= 0; i--) {
      જો (varRecords[i].intersects_features (આ, l->feature_map_w_duplicates)) {
        keep_up_to = i;
        તોડો;
      }
    }

    unsigned ગણતરી = (unsigned) (keep_up_to + 1);
    માટે (unsigned i = 0; i < ગણતરી; i++)
    {
      જો (l->feature_record_cond_idx_map != nullptr &&
          !l->feature_record_cond_idx_map->has (i))
        ચાલુ;

      l->cur_feature_var_record_idx = i;
      subset_record_array (l, &(out->varRecords), આ) (varRecords[i]);
    }

    જો (out->varRecords.len && !l->catch_all_record_feature_idxes->is_empty ())
    {
      તાર્કિક ઉમેરો_catch_all_record = સાચું;
      subset_record_array (l, &(out->varRecords), આ, ઉમેરો_catch_all_record) (varRecords[0]);
    }

    Array32Of<LookupVariationRecord> *out_lookup_records = nullptr;
    જો (version.minor >= 1)
    {
      out_lookup_records = &StructAfter<Array32Of<LookupVariationRecord>> (out->varRecords);
      જો (unlikely (!c->serializer->extend_min (out_lookup_records)))
	return_trace (ખોટું);

      + get_lookup_variation_records ().iter ()
      | hb_apply (subset_record_array (l, out_lookup_records, આ))
      ;
    }

    return_trace (તાર્કિક (out->varRecords) ||
		  (out_lookup_records && તાર્કિક (*out_lookup_records)));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c, અચલ FeatureList *feature_list) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (version.sanitize (c) &&
		  hb_barrier () &&
		  likely (version.major == 1) &&
		  varRecords.sanitize (c, આ, feature_list) &&
		  (version.minor < 1 ||
		   get_lookup_variation_records ().sanitize (c, આ)));
  }

  સુરક્ષિત:
  FixedVersion<>	version;	/* Version--0x00010000u */
  Array32Of<FeatureVariationRecord>
			varRecords;
  જાહેર:
  DEFINE_SIZE_ARRAY_SIZED (8, varRecords);
};


/*
 * Device Tables
 */

સંરચના HintingDevice
{
  friend સંરચના Device;

  ખાનગી:

  hb_position_t get_x_delta (hb_font_t *font) અચલ
  { પાછા get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) અચલ
  { પાછા get_delta (font->y_ppem, font->y_scale); }

  જાહેર:

  size_t get_size () અચલ
  {
    unsigned પૂર્ણાંક f = deltaFormat;
    જો (unlikely (f < 1 || f > 3 || startSize > endSize)) પાછા 3 * HBUINT16::static_size;
    પાછા HBUINT16::static_size * (4 + ((endSize - startSize) >> (4 - f)));
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ) && c->check_range (આ, આ->get_size ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) અચલ
  {
    TRACE_SERIALIZE (આ);
    return_trace (c->embed<HintingDevice> (આ));
  }

  ખાનગી:

  પૂર્ણાંક get_delta (unsigned પૂર્ણાંક ppem, પૂર્ણાંક scale) અચલ
  {
    જો (!ppem) પાછા 0;

    પૂર્ણાંક pixels = get_delta_pixels (ppem);

    જો (!pixels) પાછા 0;

    પાછા (પૂર્ણાંક) (pixels * (int64_t) scale / ppem);
  }
  પૂર્ણાંક get_delta_pixels (unsigned પૂર્ણાંક ppem_size) અચલ
  {
    unsigned પૂર્ણાંક f = deltaFormat;
    જો (unlikely (f < 1 || f > 3))
      પાછા 0;

    જો (ppem_size < startSize || ppem_size > endSize)
      પાછા 0;

    unsigned પૂર્ણાંક s = ppem_size - startSize;

    unsigned પૂર્ણાંક byte = delta_કિંમત_Z[s >> (4 - f)];
    unsigned પૂર્ણાંક bits = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    unsigned પૂર્ણાંક mask = (0xFFFFu >> (16 - (1 << f)));

    પૂર્ણાંક delta = bits & mask;

    જો ((unsigned પૂર્ણાંક) delta >= ((mask + 1) >> 1))
      delta -= mask + 1;

    પાછા delta;
  }

  સુરક્ષિત:
  HBUINT16	startSize;		/* Smallest size to correct--in ppem */
  HBUINT16	endSize;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  UnsizedArrayOf<HBUINT16>
		delta_કિંમત_Z;		/* Array of compressed data */
  જાહેર:
  DEFINE_SIZE_ARRAY (6, delta_કિંમત_Z);
};

સંરચના VariationDevice
{
  friend સંરચના Device;

  ખાનગી:

  hb_position_t get_x_delta (hb_font_t *font,
			     અચલ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) અચલ
  { પાછા !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     અચલ ItemVariationStore &store,
			     hb_scalar_cache_t *store_cache = nullptr) અચલ
  { પાછા !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  દશાંશ get_x_delta_f (hb_font_t *font,
		       અચલ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) અચલ
  { પાછા !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  દશાંશ get_y_delta_f (hb_font_t *font,
		       અચલ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache) અચલ
  { પાછા !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map) અચલ
  {
    TRACE_SERIALIZE (આ);
    જો (!layout_variation_idx_delta_map) return_trace (nullptr);

    hb_pair_t<unsigned, પૂર્ણાંક> *v;
    જો (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (nullptr);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    જો (hb_first (*v) == HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
      return_trace (nullptr);

    c->start_zerocopy (આ->static_size);
    auto *out = c->embed (આ);
    જો (unlikely (!out)) return_trace (nullptr);

    જો (!c->check_assign (out->varIdx, hb_first (*v), HB_SERIALIZE_ERROR_INT_OVERFLOW))
      return_trace (nullptr);
    return_trace (out);
  }

  શૂન્ય collect_variation_index (hb_collect_variation_indices_context_t *c) અચલ
  { c->layout_variation_indices->add (varIdx); }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (c->check_struct (આ));
  }

  ખાનગી:

  દશાંશ get_delta (hb_font_t *font,
		   અચલ ItemVariationStore &store,
		   hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    પાછા store.get_delta (varIdx, font->coords, font->અંક_coords, store_cache);
  }

  સુરક્ષિત:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના DeviceHeader
{
  સુરક્ષિત:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  જાહેર:
  HBUINT16		format;		/* Format identifier */
  જાહેર:
  DEFINE_SIZE_STATIC (6);
};

સંરચના Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  દશાંશ get_x_delta_f (hb_font_t *font,
		       અચલ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
#ifndef HB_NO_VAR
    જો (u.b.format == 0x8000)
    {
      hb_barrier ();
      પાછા u.variation.get_x_delta_f (font, store, store_cache);
    }
#endif
    પાછા get_x_delta (font, store, store_cache);
  }

  દશાંશ get_y_delta_f (hb_font_t *font,
		       અચલ ItemVariationStore &store,
		       hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
#ifndef HB_NO_VAR
    જો (u.b.format == 0x8000)
    {
      hb_barrier ();
      પાછા u.variation.get_y_delta_f (font, store, store_cache);
    }
#endif
    પાછા get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     અચલ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    પસંદગી (u.b.format)
    {
#ifndef HB_NO_HINTING
    સ્થિતિ 1: સ્થિતિ 2: સ્થિતિ 3:
      hb_barrier ();
      પાછા u.hinting.get_x_delta (font);
#endif
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      hb_barrier ();
      પાછા u.variation.get_x_delta (font, store, store_cache);
#endif
    મૂળભૂત:
      પાછા 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     અચલ ItemVariationStore &store=Null (ItemVariationStore),
			     hb_scalar_cache_t *store_cache = nullptr) અચલ
  {
    પસંદગી (u.b.format)
    {
    સ્થિતિ 1: સ્થિતિ 2: સ્થિતિ 3:
#ifndef HB_NO_HINTING
      hb_barrier ();
      પાછા u.hinting.get_y_delta (font);
#endif
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      hb_barrier ();
      પાછા u.variation.get_y_delta (font, store, store_cache);
#endif
    મૂળભૂત:
      પાછા 0;
    }
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    જો (!u.b.format.sanitize (c)) return_trace (ખોટું);
    પસંદગી (u.b.format) {
#ifndef HB_NO_HINTING
    સ્થિતિ 1: સ્થિતિ 2: સ્થિતિ 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#endif
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#endif
    મૂળભૂત:
      return_trace (સાચું);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map=nullptr) અચલ
  {
    TRACE_SERIALIZE (આ);
    પસંદગી (u.b.format) {
#ifndef HB_NO_HINTING
    સ્થિતિ 1:
    સ્થિતિ 2:
    સ્થિતિ 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#endif
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#endif
    મૂળભૂત:
      return_trace (nullptr);
    }
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    પસંદગી (u.b.format) {
#ifndef HB_NO_HINTING
    સ્થિતિ 1:
    સ્થિતિ 2:
    સ્થિતિ 3:
      પાછા;
#endif
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      hb_barrier ();
      u.variation.collect_variation_index (c);
      પાછા;
#endif
    મૂળભૂત:
      પાછા;
    }
  }

  unsigned get_variation_index () અચલ
  {
    પસંદગી (u.b.format) {
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      hb_barrier ();
      પાછા u.variation.varIdx;
#endif
    મૂળભૂત:
      પાછા HB_OT_LAYOUT_NO_VARIATIONS_INDEX;
    }
  }

  તાર્કિક is_variation_device () અચલ
  {
    પસંદગી (u.b.format) {
#ifndef HB_NO_VAR
    સ્થિતિ 0x8000:
      પાછા સાચું;
#endif
    મૂળભૂત:
      પાછા ખોટું;
    }
  }

  સુરક્ષિત:
  સંઘ {
  DeviceHeader		b;
  HintingDevice		hinting;
#ifndef HB_NO_VAR
  VariationDevice	variation;
#endif
  } u;
  જાહેર:
  DEFINE_SIZE_UNION (6, b);
};


} /* namespace OT */


#endif /* HB_OT_LAYOUT_COMMON_HH */

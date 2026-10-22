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

#که_نه_تعریف_شوی HB_OT_LAYOUT_COMMON_HH
#تعریف_کړه HB_OT_LAYOUT_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-set.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-cache.hh"
#ضمن "hb-ot-dual.hh"

کارول hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (حجم (hb_ot_layout_mapping_cache_t) == 512, "");

کارول hb_ot_layout_دوګونی_cache_t = hb_cache_t<14, 1, 8>;
static_assert (حجم (hb_ot_layout_دوګونی_cache_t) == 256, "");

#ضمن "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
کارول OT::Layout::Common::Coverage;
کارول OT::Layout::Common::Range_ریکارډ;
کارول OT::Layout::SmallTypes;
کارول OT::Layout::MediumTypes;


نطاق OT {

کينډۍ<ډول_نوم Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ساكن منطقي ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_سېټ_t &klasses,
    منطقي use_class_zero,
    hb_sorted_ویکټور_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

بنية توکی_Var_Store_Instancer;

بنية hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_شاخص_نښان_map;
  ثابت hb_hashmap_t<hb_نښان_t, Triple> *axes_location;
  hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *ریکارډ_cond_idx_map;
  hb_hashmap_t<بینښه, ثابت Feature*> *feature_substitutes_map;
  hb_سېټ_t& catch_all_ریکارډ_feature_idxes;

  // not stored in subset_plan
  hb_سېټ_t *feature_indices;
  منطقي apply;
  منطقي variation_applied;
  منطقي universal;
  بینښه cur_ریکارډ_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, بینښه> *conditionset_map;
  توکی_Var_Store_Instancer *instancer;
  منطقي all_axes_pinned;
};

بنية hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت فراغ         *جدول_2,
                              hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_سېټ_t           *new_collected_feature_indexes_)
      :جدول (جدول_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_شمېر (0),langsys_feature_شمېر (0) {}

  منطقي لیدنه_Script ()
  { أرجع script_شمېر++ < HB_MAX_SCRIPTS; }

  منطقي لیدنه_Langsys (بینښه feature_شمېر_2)
  {
    langsys_feature_شمېر += feature_شمېر_2;
    أرجع langsys_feature_شمېر < HB_MAX_LANGSYS_FEATURE_شمېر;
  }

  عام:
  ثابت فراغ *جدول;
  hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_سېټ_t           *new_feature_indexes;

  خاص:
  بینښه script_شمېر;
  بینښه langsys_feature_شمېر;
};

بنية hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_تشه_t, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { أرجع "SUBSET_LAYOUT"; }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  منطقي لیدنه_Script ()
  {
    أرجع script_شمېر++ < HB_MAX_SCRIPTS;
  }

  منطقي لیدنه_Lang_Sys ()
  {
    أرجع langsys_شمېر++ < HB_MAX_LANGSYS;
  }

  منطقي لیدنه_Feature_شاخص (صحيح شمېر)
  {
    feature_شاخص_شمېر += شمېر;
    أرجع feature_شاخص_شمېر < HB_MAX_FEATURE_INDICES;
  }

  منطقي لیدنه_Lookup_شاخص()
  {
    lookup_شاخص_شمېر++;
    أرجع lookup_شاخص_شمېر < HB_MAX_LOOKUP_لیدنه_شمېر;
  }

  hb_subset_context_t *subset_context;
  ثابت hb_نښان_t جدول_نښان_2;
  ثابت hb_map_t *lookup_شاخص_map;
  ثابت hb_hashmap_t<بینښه, hb::unique_ptr<hb_سېټ_t>> *script_langsys_map;
  ثابت hb_map_t *feature_شاخص_map;
  ثابت hb_map_t *feature_map_w_duplicates;
  ثابت hb_hashmap_t<بینښه, ثابت Feature*> *feature_substitutes_map;
  ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *lookup_variations;
  hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map;
  ثابت hb_سېټ_t *catch_all_ریکارډ_feature_idxes;
  ثابت hb_hashmap_t<بینښه, hb_pair_t<ثابت فراغ*, ثابت فراغ*>> *feature_idx_نښان_map;

  بینښه cur_script_شاخص;
  بینښه cur_feature_شاخص;
  بینښه cur_feature_var_ریکارډ_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_نښان_t نښان_4) :
				subset_context (c_),
				جدول_نښان_2 (نښان_4),
				cur_script_شاخص (0xFFFFu),
				cur_feature_شاخص (شاخص::NOT_وموندل_شاخص),
				cur_feature_var_ریکارډ_idx (0u),
				script_شمېر (0),
				langsys_شمېر (0),
				feature_شاخص_شمېر (0),
				lookup_شاخص_شمېر (0)
  {
    إذا (نښان_4 == HB_OT_نښان_GSUB)
    {
      lookup_شاخص_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_شاخص_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_ریکارډ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? تش_اشاره : &c_->plan->gsub_feature_ریکارډ_cond_idx_map;
      catch_all_ریکارډ_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_نښان_map = &c_->plan->gsub_old_feature_idx_نښان_map;
    }
    وإلا
    {
      lookup_شاخص_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_شاخص_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_ریکارډ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? تش_اشاره : &c_->plan->gpos_feature_ریکارډ_cond_idx_map;
      catch_all_ریکارډ_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_نښان_map = &c_->plan->gpos_old_feature_idx_نښان_map;
    }
  }

  خاص:
  بینښه script_شمېر;
  بینښه langsys_شمېر;
  بینښه feature_شاخص_شمېر;
  بینښه lookup_شاخص_شمېر;
};

بنية توکی_Variation_Store;
بنية hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  کينډۍ <ډول_نوم T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (هذا); أرجع hb_تشه_t (); }
  ساكن return_t default_return_ارزښت () { أرجع hb_تشه_t (); }

  hb_سېټ_t *layout_variation_indices;
  ثابت hb_سېټ_t *glyph_سېټ;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<بینښه, صحيح>> *condition_ارزښتونه = تش_اشاره;

  hb_collect_variation_indices_context_t (hb_سېټ_t *layout_variation_indices_,
					  ثابت hb_سېټ_t *glyph_سېټ_2,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_سېټ (glyph_سېټ_2),
					gpos_lookups (gpos_lookups_) {}
};

کينډۍ<ډول_نوم OutputArray>
بنية subset_دننه_واټن_array_t
{
  subset_دننه_واټن_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& بهر_4,
			 ثابت فراغ *بنسټ_2) : subset_context (subset_context_),
					      بهر (بهر_4), بنسټ (بنسټ_2) {}

  کينډۍ <ډول_نوم T>
  منطقي چلوونکی () (T&& دننه_واټن)
  {
    خپلکاری snap = subset_context->serializer->snapshot ();
    خپلکاری *o = بهر.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, دننه_واټن, بنسټ);
    إذا (!ret)
    {
      بهر.راوباسه ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &بهر;
  ثابت فراغ *بنسټ;
};


کينډۍ<ډول_نوم OutputArray, ډول_نوم Arg>
بنية subset_دننه_واټن_array_arg_t
{
  subset_دننه_واټن_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& بهر_4,
			     ثابت فراغ *بنسټ_2,
			     Arg &&arg_) : subset_context (subset_context_), بهر (بهر_4),
					  بنسټ (بنسټ_2), arg (arg_) {}

  کينډۍ <ډول_نوم T>
  منطقي چلوونکی () (T&& دننه_واټن)
  {
    خپلکاری snap = subset_context->serializer->snapshot ();
    خپلکاری *o = بهر.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, دننه_واټن, بنسټ, arg);
    إذا (!ret)
    {
      بهر.راوباسه ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &بهر;
  ثابت فراغ *بنسټ;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
بنية
{
  کينډۍ<ډول_نوم OutputArray>
  subset_دننه_واټن_array_t<OutputArray>
  چلوونکی () (hb_subset_context_t *subset_context, OutputArray& بهر,
	       ثابت فراغ *بنسټ) ثابت
  { أرجع subset_دننه_واټن_array_t<OutputArray> (subset_context, بهر, بنسټ); }

  /* Variant with one extra argument passed to serialize_subset */
  کينډۍ<ډول_نوم OutputArray, ډول_نوم Arg>
  subset_دننه_واټن_array_arg_t<OutputArray, Arg>
  چلوونکی () (hb_subset_context_t *subset_context, OutputArray& بهر,
	       ثابت فراغ *بنسټ, Arg &&arg) ثابت
  { أرجع subset_دننه_واټن_array_arg_t<OutputArray, Arg> (subset_context, بهر, بنسټ, arg); }
}
HB_FUNCOBJ (subset_دننه_واټن_array);

کينډۍ<ډول_نوم OutputArray>
بنية subset_ریکارډ_array_t
{
  subset_ریکارډ_array_t (hb_subset_layout_context_t *c_, OutputArray* بهر_4,
			 ثابت فراغ *بنسټ_2) : subset_layout_context (c_),
					      بهر (بهر_4), بنسټ (بنسټ_2) {}

  کينډۍ <ډول_نوم T>
  فراغ
  چلوونکی () (T&& ریکارډ)
  {
    خپلکاری snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = ریکارډ.subset (subset_layout_context, بنسټ);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا بهر->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *بهر;
  ثابت فراغ *بنسټ;
};

کينډۍ<ډول_نوم OutputArray, ډول_نوم Arg>
بنية subset_ریکارډ_array_arg_t
{
  subset_ریکارډ_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* بهر_4,
			     ثابت فراغ *بنسټ_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   بهر (بهر_4), بنسټ (بنسټ_2), arg (arg_) {}

  کينډۍ <ډول_نوم T>
  فراغ
  چلوونکی () (T&& ریکارډ)
  {
    خپلکاری snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = ریکارډ.subset (subset_layout_context, بنسټ, arg);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا بهر->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *بهر;
  ثابت فراغ *بنسټ;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
بنية
{
  کينډۍ<ډول_نوم OutputArray>
  subset_ریکارډ_array_t<OutputArray>
  چلوونکی () (hb_subset_layout_context_t *c, OutputArray* بهر,
	       ثابت فراغ *بنسټ) ثابت
  { أرجع subset_ریکارډ_array_t<OutputArray> (c, بهر, بنسټ); }

  /* Variant with one extra argument passed to subset */
  کينډۍ<ډول_نوم OutputArray, ډول_نوم Arg>
  subset_ریکارډ_array_arg_t<OutputArray, Arg>
  چلوونکی () (hb_subset_layout_context_t *c, OutputArray* بهر,
               ثابت فراغ *بنسټ, Arg &&arg) ثابت
  { أرجع subset_ریکارډ_array_arg_t<OutputArray, Arg> (c, بهر, بنسټ, arg); }
}
HB_FUNCOBJ (subset_ریکارډ_array);


کينډۍ<ډول_نوم OutputArray>
بنية serialize_math_ریکارډ_array_t
{
  serialize_math_ریکارډ_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& بهر_4,
                         ثابت فراغ *بنسټ_2,
                         ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              بهر (بهر_4), بنسټ (بنسټ_2), variation_map (variation_map_) {}

  کينډۍ <ډول_نوم T>
  منطقي چلوونکی () (T&& ریکارډ)
  {
    إذا (!serialize_context->copy (ریکارډ, بنسټ, variation_map)) أرجع خطأ;
    بهر.len++;
    أرجع ریښتیا;
  }

  خاص:
  hb_serialize_context_t *serialize_context;
  OutputArray &بهر;
  ثابت فراغ *بنسټ;
  ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
بنية
{
  کينډۍ<ډول_نوم OutputArray>
  serialize_math_ریکارډ_array_t<OutputArray>
  چلوونکی () (hb_serialize_context_t *serialize_context, OutputArray& بهر,
               ثابت فراغ *بنسټ,
               ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *variation_map) ثابت
  { أرجع serialize_math_ریکارډ_array_t<OutputArray> (serialize_context, بهر, بنسټ, variation_map); }

}
HB_FUNCOBJ (serialize_math_ریکارډ_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

بنية شاخص_Array : Array16_د<شاخص>
{
  منطقي intersects (ثابت hb_map_t *indexes) ثابت
  { أرجع hb_any (*هذا, indexes); }

  کينډۍ <ډول_نوم Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    إذا (!it) أرجع;
    إذا (unlikely (!c->extend_min ((*هذا)))) أرجع;

    لكل (ثابت خپلکاری _ : it)
    {
      إذا (!l->لیدنه_Lookup_شاخص()) اكسر;

      شاخص i;
      i = _;
      c->copy (i);
      هذا->len++;
    }
  }

  بینښه صحيح get_indexes (بینښه صحيح پیل_دننه_واټن,
			    بینښه صحيح *شمېر_2 /* IN/OUT */,
			    بینښه صحيح *_indexes /*بهر*/) ثابت
  {
    إذا (شمېر_2 && _indexes)
    {
      + هذا->as_array ().sub_array (پیل_دننه_واټن, شمېر_2)
      | hb_sink (hb_array (_indexes, *شمېر_2))
      ;
    }
    أرجع هذا->len;
  }

  فراغ جمع_کړه_indexes_منزل (hb_سېټ_t* output /*بهر*/) ثابت
  {
    output->جمع_کړه_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
بنية Feature_Params_اندازه
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

    إذا (!design_اندازه)
      return_trace (خطأ);
    وإلا إذا (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     range_پیل == 0 &&
	     range_پای_ټکی == 0)
      return_trace (ریښتیا);
    وإلا إذا (design_اندازه < range_پیل ||
	     design_اندازه > range_پای_ټکی ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (خطأ);
    وإلا
      return_trace (ریښتیا);
  }

  فراغ collect_name_ids (hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  { nameids_منزل_retain->جمع_کړه (subfamilyNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  HBUINT16	design_اندازه;	/* Represents the design size in 720/inch
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
  HBUINT16	range_پیل;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	range_پای_ټکی;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  عام:
  DEFINE_اندازه_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
بنية Feature_Params_Stylistic_سېټ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (هذا));
  }

  فراغ collect_name_ids (hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  { nameids_منزل_retain->جمع_کړه (uiNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  HBUINT16	نسخه;	/* (set to 0): This corresponds to a “minor”
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
  DEFINE_اندازه_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
بنية FeatureParamsCharacterVariants
{
  بینښه
  get_characters (بینښه پیل_دننه_واټن, بینښه *char_شمېر, hb_codepoint_t *chars) ثابت
  {
    إذا (char_شمېر && chars)
    {
      + characters.as_array ().sub_array (پیل_دننه_واټن, char_شمېر)
      | hb_sink (hb_array (chars, *char_شمېر))
      ;
    }
    أرجع characters.len;
  }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (characters.len, HBUINT24::static_اندازه, min_اندازه); }

  فراغ collect_name_ids (hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  {
    إذا (featUILableNameID) nameids_منزل_retain->جمع_کړه (featUILableNameID);
    إذا (feat_UITooltip_متن_Name_ID) nameids_منزل_retain->جمع_کړه (feat_UITooltip_متن_Name_ID);
    إذا (sample_متن_Name_ID) nameids_منزل_retain->جمع_کړه (sample_متن_Name_ID);

    إذا (!لومړی_Param_UILabel_Name_ID || !عدد_Named_Parameters || عدد_Named_Parameters >= 0x7FFF)
      أرجع;

    بینښه وروستی_name_id = (بینښه) لومړی_Param_UILabel_Name_ID + (بینښه) عدد_Named_Parameters - 1;
    nameids_منزل_retain->جمع_کړه_range (لومړی_Param_UILabel_Name_ID, وروستی_name_id);
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
  NameID	feat_UITooltip_متن_Name_ID;/* The ‘name’ table name ID that
					 * specifies a string (or strings,
					 * for multiple languages) that an
					 * application can use for tooltip
					 * text for this feature. (May be
					 * nullptr.) */
  NameID	sample_متن_Name_ID;	/* The ‘name’ table name ID that
					 * specifies sample text that
					 * illustrates the effect of this
					 * feature. (May be NULL.) */
  HBUINT16	عدد_Named_Parameters;	/* Number of named parameters. (May
					 * be zero.) */
  NameID	لومړی_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_د<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  عام:
  DEFINE_اندازه_ARRAY (14, characters);
};

بنية FeatureParams
{
  منطقي sanitize (hb_sanitize_context_t *c, hb_نښان_t نښان_2) ثابت
  {
#که_تعریف_شوی HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع ریښتیا;
#د_که_پای
    TRACE_SANITIZE (هذا);
    إذا (نښان_2 == HB_نښان ('s','i','z','e') && hb_barrier ())
      return_trace (u.اندازه.sanitize (c));
    إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سېټ.sanitize (c));
    إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (ریښتیا);
  }

  فراغ collect_name_ids (hb_نښان_t نښان_2, hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  {
#که_تعریف_شوی HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع;
#د_که_پای
    إذا (نښان_2 == HB_نښان ('s','i','z','e') && hb_barrier ())
      أرجع (u.اندازه.collect_name_ids (nameids_منزل_retain));
    إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('s','s','\0','\0')) /* ssXX */
      أرجع (u.stylistic_سېټ.collect_name_ids (nameids_منزل_retain));
    إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('c','v','\0','\0')) /* cvXX */
      أرجع (u.characterVariants.collect_name_ids (nameids_منزل_retain));
  }

  منطقي subset (hb_subset_context_t *c, ثابت نښان* نښان_2) ثابت
  {
#که_تعریف_شوی HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع خطأ;
#د_که_پای
    TRACE_SUBSET (هذا);
    إذا (!نښان_2) return_trace (خطأ);
    إذا (*نښان_2 == HB_نښان ('s','i','z','e') && hb_barrier ())
      return_trace (u.اندازه.subset (c));
    إذا ((*نښان_2 & 0xFFFF0000u) == HB_نښان ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سېټ.subset (c));
    إذا ((*نښان_2 & 0xFFFF0000u) == HB_نښان ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (خطأ);
  }

#که_نه_تعریف_شوی HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت Feature_Params_اندازه& get_اندازه_params (hb_نښان_t نښان_2) ثابت
  {
    إذا (نښان_2 == HB_نښان ('s','i','z','e') && hb_barrier ())
      أرجع u.اندازه;
    أرجع Null (Feature_Params_اندازه);
  }
  ثابت Feature_Params_Stylistic_سېټ& get_stylistic_سېټ_params (hb_نښان_t نښان_2) ثابت
  {
    إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('s','s','\0','\0')) /* ssXX */
      أرجع u.stylistic_سېټ;
    أرجع Null (Feature_Params_Stylistic_سېټ);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_نښان_t نښان_2) ثابت
  {
    إذا ((نښان_2 & 0xFFFF0000u) == HB_نښان ('c','v','\0','\0')) /* cvXX */
      أرجع u.characterVariants;
    أرجع Null (FeatureParamsCharacterVariants);
  }
#د_که_پای

  خاص:
  اتحاد {
  Feature_Params_اندازه			اندازه;
  Feature_Params_Stylistic_سېټ		stylistic_سېټ;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عام:
  DEFINE_اندازه_MIN (0);
};

بنية ریکارډ_sanitize_تړون_t {
  hb_نښان_t نښان_2;
  ثابت فراغ *list_بنسټ;
};

بنية Feature
{
  بینښه صحيح get_lookup_شمېر () ثابت
  { أرجع lookup_شاخص_2.len; }
  hb_نښان_t get_lookup_شاخص (بینښه صحيح i) ثابت
  { أرجع lookup_شاخص_2[i]; }
  بینښه صحيح get_lookup_indexes (بینښه صحيح پیل_شاخص,
				   بینښه صحيح *lookup_شمېر /* IN/OUT */,
				   بینښه صحيح *lookup_نښانونه /*بهر*/) ثابت
  { أرجع lookup_شاخص_2.get_indexes (پیل_شاخص, lookup_شمېر, lookup_نښانونه); }
  فراغ جمع_کړه_lookup_indexes_منزل (hb_سېټ_t *lookup_indexes) ثابت
  { lookup_شاخص_2.جمع_کړه_indexes_منزل (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { أرجع هذا+featureParams; }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookup_شاخص_2.intersects (lookup_indexes); }

  فراغ collect_name_ids (hb_نښان_t نښان_2, hb_سېټ_t *nameids_منزل_retain /*بهر*/) ثابت
  {
    إذا (featureParams)
      get_feature_params ().collect_name_ids (نښان_2, nameids_منزل_retain);
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت نښان                   *نښان_2 = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->featureParams.serialize_subset (c, featureParams, هذا, نښان_2);

    hb::shared_ptr<hb_سېټ_t> *lookup_variations;
    إذا (l->cur_feature_شاخص != شاخص::NOT_وموندل_شاخص &&
	l->lookup_variations->has (l->cur_feature_شاخص, &lookup_variations))
    {
      خپلکاری it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_شاخص_map)
      | hb_map (l->lookup_شاخص_map)
      ;
      بهر->lookup_شاخص_2.serialize (c->serializer, l, it);
    }
    وإلا
    {
      خپلکاری it =
      + hb_iter (lookup_شاخص_2)
      | hb_filter (l->lookup_شاخص_map)
      | hb_map (l->lookup_شاخص_map)
      ;
      بهر->lookup_شاخص_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارډ_sanitize_تړون_t *تړون = تش_اشاره) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureParams.sanitize (c, هذا, تړون ? تړون->نښان_2 : HB_نښان_NONE) &&
		  lookup_شاخص_2.sanitize (c));
  }

  Offset16_منزل<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  شاخص_Array	 lookup_شاخص_2;	/* Array of LookupList indices */
  عام:
  DEFINE_اندازه_ARRAY_SIZED (4, lookup_شاخص_2);
};

کينډۍ <ډول_نوم Type>
بنية ریکارډ_2
{
  صحيح cmp (hb_نښان_t a) ثابت { أرجع نښان_2.cmp (a); }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *بنسټ, ثابت فراغ *f_sub = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (!f_sub)
      return_trace (بهر->دننه_واټن.serialize_subset (c->subset_context, دننه_واټن, بنسټ, c, &نښان_2));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    خپلکاری *s = c->subset_context->serializer;
    s->push ();

    بهر->دننه_واټن = 0;
    منطقي ret = f.subset (c->subset_context, c, &نښان_2);
    إذا (ret)
      s->جمع_کړه_link (بهر->دننه_واټن, s->راوباسه_pack ());
    وإلا
      s->راوباسه_discard ();

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت ریکارډ_sanitize_تړون_t تړون = {نښان_2, بنسټ};
    return_trace (c->check_struct (هذا) &&
		  دننه_واټن.sanitize (c, بنسټ, &تړون));
  }

  نښان           نښان_2;            /* 4-byte Tag identifier */
  Offset16_منزل<Type>
                دننه_واټن;         /* Offset from beginning of object holding
                                 * the Record */
  عام:
  DEFINE_اندازه_STATIC (6);
};

کينډۍ <ډول_نوم Type>
بنية ریکارډ_Array_د : Sorted_Array16_د<ریکارډ_2<Type>>
{
  ثابت Offset16_منزل<Type>& get_دننه_واټن (بینښه صحيح i) ثابت
  { أرجع (*هذا)[i].دننه_واټن; }
  Offset16_منزل<Type>& get_دننه_واټن (بینښه صحيح i)
  { أرجع (*هذا)[i].دننه_واټن; }
  ثابت نښان& get_نښان (بینښه صحيح i) ثابت
  { أرجع (*هذا)[i].نښان_2; }
  بینښه صحيح get_نښانونه (بینښه صحيح پیل_دننه_واټن,
                         بینښه صحيح *ریکارډ_شمېر /* IN/OUT */,
                         hb_نښان_t     *ریکارډ_نښانونه /*بهر*/) ثابت
  {
    إذا (ریکارډ_شمېر && ریکارډ_نښانونه)
    {
      + هذا->as_array ().sub_array (پیل_دننه_واټن, ریکارډ_شمېر)
      | hb_map (&ریکارډ_2<Type>::نښان_2)
      | hb_sink (hb_array (ریکارډ_نښانونه, *ریکارډ_شمېر))
      ;
    }
    أرجع هذا->len;
  }
  منطقي پیدا_کړه_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  {
    أرجع هذا->bfind (نښان_2, شاخص_2, HB_NOT_وموندل_STORE, شاخص::NOT_وموندل_شاخص);
  }
};

کينډۍ <ډول_نوم Type>
بنية ریکارډ_List_د : ریکارډ_Array_د<Type>
{
  ثابت Type& چلوونکی [] (بینښه صحيح i) ثابت
  { أرجع هذا+هذا->get_دننه_واټن (i); }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    + هذا->iter ()
    | hb_apply (subset_ریکارډ_array (l, بهر, هذا))
    ;
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ریکارډ_Array_د<Type>::sanitize (c, هذا));
  }
};

بنية ریکارډ_List_د_Feature : ریکارډ_List_د<Feature>
{
  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->feature_شاخص_map, hb_لومړی)
    | hb_apply ([l, بهر, هذا] (ثابت hb_pair_t<بینښه, ثابت ریکارډ_2<Feature>&>& _)
                {
                  ثابت Feature *f_sub = تش_اشاره;
                  ثابت Feature **f = تش_اشاره;
                  إذا (l->feature_substitutes_map->has (_.لومړی, &f))
                    f_sub = *f;

                  l->cur_feature_شاخص = _.لومړی;
                  subset_ریکارډ_array (l, بهر, هذا, f_sub) (_.second);
                  l->cur_feature_شاخص = شاخص::NOT_وموندل_شاخص;
                })
    ;

    return_trace (ریښتیا);
  }
};

تعريفنوع ریکارډ_List_د<Feature> FeatureList;


بنية LangSys
{
  بینښه صحيح get_feature_شمېر () ثابت
  { أرجع feature_شاخص_2.len; }
  hb_نښان_t get_feature_شاخص (بینښه صحيح i) ثابت
  { أرجع feature_شاخص_2[i]; }
  بینښه صحيح get_feature_indexes (بینښه صحيح پیل_دننه_واټن,
				    بینښه صحيح *feature_شمېر_2 /* IN/OUT */,
				    بینښه صحيح *feature_indexes /*بهر*/) ثابت
  { أرجع feature_شاخص_2.get_indexes (پیل_دننه_واټن, feature_شمېر_2, feature_indexes); }
  فراغ جمع_کړه_feature_indexes_منزل (hb_سېټ_t *feature_indexes) ثابت
  { feature_شاخص_2.جمع_کړه_indexes_منزل (feature_indexes); }

  منطقي has_required_feature () ثابت { أرجع req_Feature_شاخص != 0xFFFFu; }
  بینښه صحيح get_required_feature_شاخص () ثابت
  {
    إذا (req_Feature_شاخص == 0xFFFFu)
      أرجع شاخص::NOT_وموندل_شاخص;
   أرجع req_Feature_شاخص;
  }

  LangSys* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  منطقي compare (ثابت LangSys& o, ثابت hb_map_t *feature_شاخص_map) ثابت
  {
    إذا (req_Feature_شاخص != o.req_Feature_شاخص)
      أرجع خطأ;

    خپلکاری iter =
    + hb_iter (feature_شاخص_2)
    | hb_filter (feature_شاخص_map)
    | hb_map (feature_شاخص_map)
    ;

    خپلکاری o_iter =
    + hb_iter (o.feature_شاخص_2)
    | hb_filter (feature_شاخص_map)
    | hb_map (feature_شاخص_map)
    ;

    لكل (; iter && o_iter; iter++, o_iter++)
    {
      بینښه a = *iter;
      بینښه b = *o_iter;
      إذا (a != b) أرجع خطأ;
    }

    إذا (iter || o_iter) أرجع خطأ;

    أرجع ریښتیا;
  }

  فراغ collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    إذا (!has_required_feature () && !get_feature_شمېر ()) أرجع;
    إذا (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_شاخص))
      c->new_feature_indexes->جمع_کړه (get_required_feature_شاخص ());

    + hb_iter (feature_شاخص_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت نښان                  *نښان_2 = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    ثابت uint32_t *v;
    بهر->req_Feature_شاخص = l->feature_map_w_duplicates->has (req_Feature_شاخص, &v) ? *v : 0xFFFFu;

    إذا (!l->لیدنه_Feature_شاخص (feature_شاخص_2.len))
      return_trace (خطأ);

    خپلکاری it =
    + hb_iter (feature_شاخص_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقي ret = منطقي (it);
    بهر->feature_شاخص_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارډ_sanitize_تړون_t * = تش_اشاره) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && feature_شاخص_2.sanitize (c));
  }

  Offset16	lookup_ترتیب_Z;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_شاخص;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  شاخص_Array	feature_شاخص_2;	/* Array of indices into the FeatureList */
  عام:
  DEFINE_اندازه_ARRAY_SIZED (6, feature_شاخص_2);
};
DECLARE_NULL_NAMESPACE_بایټونه (OT, LangSys);

بنية Script
{
  بینښه صحيح get_lang_sys_شمېر () ثابت
  { أرجع langSys.len; }
  ثابت نښان& get_lang_sys_نښان (بینښه صحيح i) ثابت
  { أرجع langSys.get_نښان (i); }
  بینښه صحيح get_lang_sys_نښانونه (بینښه صحيح پیل_دننه_واټن,
				  بینښه صحيح *lang_sys_شمېر /* IN/OUT */,
				  hb_نښان_t     *lang_sys_نښانونه /*بهر*/) ثابت
  { أرجع langSys.get_نښانونه (پیل_دننه_واټن, lang_sys_شمېر, lang_sys_نښانونه); }
  ثابت LangSys& get_lang_sys (بینښه صحيح i) ثابت
  {
    إذا (i == شاخص::NOT_وموندل_شاخص) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].دننه_واټن;
  }
  منطقي پیدا_کړه_lang_sys_شاخص (hb_نښان_t نښان_2, بینښه صحيح *شاخص_2) ثابت
  { أرجع langSys.پیدا_کړه_شاخص (نښان_2, شاخص_2); }

  منطقي has_default_lang_sys () ثابت           { أرجع defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  فراغ prune_langsys (hb_prune_langsys_context_t *c,
                      بینښه script_شاخص) ثابت
  {
    إذا (!has_default_lang_sys () && !get_lang_sys_شمېر ()) أرجع;
    إذا (!c->لیدنه_Script ()) أرجع;

    إذا (!c->script_langsys_map->has (script_شاخص))
    {
      إذا (unlikely (!c->script_langsys_map->سېټ (script_شاخص, hb::unique_ptr<hb_سېټ_t> {hb_سېټ_create ()})))
	أرجع;
    }

    إذا (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      إذا (c->لیدنه_Langsys (d.get_feature_شمېر ())) {
        d.collect_features (c);
      }

      لكل (خپلکاری _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.دننه_واټن;
        إذا (!c->لیدنه_Langsys (l.get_feature_شمېر ())) استمر;
        إذا (l.compare (d, c->duplicate_feature_map)) استمر;

        l.collect_features (c);
        c->script_langsys_map->get (script_شاخص)->جمع_کړه (_.لومړی);
      }
    }
    وإلا
    {
      لكل (خپلکاری _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.دننه_واټن;
        إذا (!c->لیدنه_Langsys (l.get_feature_شمېر ())) استمر;
        l.collect_features (c);
        c->script_langsys_map->get (script_شاخص)->جمع_کړه (_.لومړی);
      }
    }
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت نښان                   *نښان_2) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!l->لیدنه_Script ()) return_trace (خطأ);
    إذا (نښان_2 && !c->plan->layout_scripts.has (*نښان_2))
      أرجع خطأ;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    منطقي defaultLang = خطأ;
    إذا (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = هذا+defaultLangSys;
      منطقي ret = ls.subset (c, l);
      إذا (!ret && نښان_2 && *نښان_2 != HB_نښان ('D', 'F', 'L', 'T'))
      {
	c->serializer->راوباسه_discard ();
	بهر->defaultLangSys = 0;
      }
      وإلا
      {
	c->serializer->جمع_کړه_link (بهر->defaultLangSys, c->serializer->راوباسه_pack ());
	defaultLang = ریښتیا;
      }
    }

    ثابت hb_سېټ_t *فعال_langsys = l->script_langsys_map->get (l->cur_script_شاخص);
    إذا (فعال_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (فعال_langsys, hb_لومړی)
      | hb_map (hb_دوهم)
      | hb_filter ([=] (ثابت ریکارډ_2<LangSys>& ریکارډ) {أرجع l->لیدنه_Lang_Sys (); })
      | hb_apply (subset_ریکارډ_array (l, &(بهر->langSys), هذا))
      ;
    }

    return_trace (منطقي (بهر->langSys.len) || defaultLang || l->جدول_نښان_2 == HB_OT_نښان_GSUB);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارډ_sanitize_تړون_t * = تش_اشاره) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (defaultLangSys.sanitize (c, هذا) && langSys.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  ریکارډ_Array_د<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_اندازه_ARRAY_SIZED (4, langSys);
};

بنية ریکارډ_List_د_Script : ریکارډ_List_د<Script>
{
  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    لكل (خپلکاری _ : + hb_enumerate (*هذا))
    {
      خپلکاری snap = c->serializer->snapshot ();
      l->cur_script_شاخص = _.لومړی;
      منطقي ret = _.second.subset (l, هذا);
      إذا (!ret) c->serializer->revert (snap);
      وإلا بهر->len++;
    }

    return_trace (ریښتیا);
  }
};

تعريفنوع ریکارډ_List_د_Script ScriptList;



بنية LookupFlag : HBUINT16
{
  تعداد Flags {
    ښی_منزل_کیڼ		= 0x0001u,
    Ignore_بنسټ_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_سېټ	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  عام:
  DEFINE_اندازه_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
نطاق OT {

بنية Lookup
{
  بینښه صحيح get_subtable_شمېر () ثابت { أرجع sub_جدول.len; }

  کينډۍ <ډول_نوم TSub_جدول>
  ثابت Array16_د_Offset16_منزل<TSub_جدول>& get_subtables () ثابت
  { أرجع reinterpret_cast<ثابت Array16_د_Offset16_منزل<TSub_جدول> &> (sub_جدول); }
  کينډۍ <ډول_نوم TSub_جدول>
  Array16_د_Offset16_منزل<TSub_جدول>& get_subtables ()
  { أرجع reinterpret_cast<Array16_د_Offset16_منزل<TSub_جدول> &> (sub_جدول); }

  کينډۍ <ډول_نوم TSub_جدول>
  ثابت TSub_جدول& get_subtable (بینښه صحيح i) ثابت
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }
  کينډۍ <ډول_نوم TSub_جدول>
  TSub_جدول& get_subtable (بینښه صحيح i)
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }

  size_t get_اندازه () ثابت
  {
    ثابت HBUINT16 &mark_Filtering_سېټ = StructAfter<ثابت HBUINT16> (sub_جدول);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سېټ)
      أرجع (ثابت حرف *) &StructAfter<ثابت حرف> (mark_Filtering_سېټ) - (ثابت حرف *) هذا;
    أرجع (ثابت حرف *) &mark_Filtering_سېټ - (ثابت حرف *) هذا;
  }

  بینښه صحيح get_type () ثابت { أرجع lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    بینښه صحيح flag = lookupFlag;
    إذا (unlikely (flag & LookupFlag::Use_Mark_Filtering_سېټ))
    {
      ثابت HBUINT16 &mark_Filtering_سېټ = StructAfter<HBUINT16> (sub_جدول);
      flag += (mark_Filtering_سېټ << 16);
    }
    أرجع flag;
  }

  کينډۍ <ډول_نوم TSub_جدول, ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بینښه صحيح lookup_type = get_type ();
    TRACE_DISPATCH (هذا, lookup_type);
    بینښه صحيح شمېر = get_subtable_شمېر ();
    لكل (بینښه صحيح i = 0; i < شمېر; i++) {
      ډول_نوم context_t::return_t r = get_subtable<TSub_جدول> (i).dispatch (c, lookup_type, ds...);
      إذا (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_ارزښت ());
  }

  منطقي serialize (hb_serialize_context_t *c,
		  بینښه صحيح lookup_type,
		  uint32_t lookup_props,
		  بینښه صحيح عدد_subtables)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    إذا (unlikely (!sub_جدول.serialize (c, عدد_subtables))) return_trace (خطأ);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سېټ)
    {
      إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
      HBUINT16 &mark_Filtering_سېټ = StructAfter<HBUINT16> (sub_جدول);
      mark_Filtering_سېټ = lookup_props >> 16;
    }
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم TSub_جدول>
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->lookupType = lookupType;
    بهر->lookupFlag = lookupFlag;

    ثابت hb_سېټ_t *glyphset = c->plan->glyphset_gsub ();
    بینښه صحيح lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_جدول> ())
    | hb_filter ([هذا, glyphset, lookup_type] (ثابت Offset16_منزل<TSub_جدول> &_) { أرجع (هذا+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_دننه_واټن_array (c, بهر->get_subtables<TSub_جدول> (), هذا, lookup_type))
    ;

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سېټ)
    {
      ثابت HBUINT16 &mark_Filtering_سېټ = StructAfter<HBUINT16> (sub_جدول);
      hb_codepoint_t *idx;
      إذا (!c->plan->کارول_شوی_mark_sets_map.has (mark_Filtering_سېټ, &idx))
      {
        بینښه new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_سېټ;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        بهر->lookupFlag = new_flag;
      }
      وإلا
      {
        إذا (unlikely (!c->serializer->extend (بهر))) return_trace (خطأ);
        HBUINT16 &بهر_Mark_Filtering_سېټ = StructAfter<HBUINT16> (بهر->sub_جدول);
        بهر_Mark_Filtering_سېټ = *idx;
      }
    }

    // Always keep the lookup even if it's empty. The rest of layout subsetting depends on lookup
    // indices being consistent with those computed during planning. So if an empty lookup is
    // discarded during the subset phase it will invalidate all subsequent lookup indices.
    // Generally we shouldn't end up with an empty lookup as we pre-prune them during the planning
    // phase, but it can happen in rare cases such as when during closure subtable is considered
    // degenerate (see: https://github.com/harfbuzz/harfbuzz/issues/3853)
    return_trace (ریښتیا);
  }

  کينډۍ <ډول_نوم TSub_جدول>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) && sub_جدول.sanitize (c))) return_trace (خطأ);
    hb_barrier ();

    بینښه subtables = get_subtable_شمېر ();
    إذا (unlikely (!c->لیدنه_subtables (subtables))) return_trace (خطأ);

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سېټ)
    {
      ثابت HBUINT16 &mark_Filtering_سېټ = StructAfter<HBUINT16> (sub_جدول);
      إذا (!mark_Filtering_سېټ.sanitize (c)) return_trace (خطأ);
    }

    إذا (unlikely (!get_subtables<TSub_جدول> ().sanitize (c, هذا, get_type ())))
      return_trace (خطأ);

    إذا (unlikely (get_type () == TSub_جدول::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      بینښه صحيح type = get_subtable<TSub_جدول> (0).u.extension.get_type ();
      لكل (بینښه صحيح i = 1; i < subtables; i++)
	إذا (get_subtable<TSub_جدول> (i).u.extension.get_type () != type)
	  return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  محمي:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_د<Offset16>
		sub_جدول;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عام:
  DEFINE_اندازه_ARRAY (6, sub_جدول);
};

کينډۍ <ډول_نوم Types>
کارول LookupList = List16_د_دننه_واټن_منزل<Lookup, ډول_نوم Types::HBLUINT>;

کينډۍ <ډول_نوم TLookup, ډول_نوم دننه_واټن_Type>
بنية Lookup_دننه_واټن_List : List16_د_دننه_واټن_منزل<TLookup, دننه_واټن_Type>
{
  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->lookup_شاخص_map, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_apply (subset_دننه_واټن_array (c, *بهر, هذا))
    ;
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((List16_د_دننه_واټن_منزل<TLookup, دننه_واټن_Type>::sanitize (c, هذا)));
  }
};


/*
 * Coverage Table
 */


ساكن منطقي ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_سېټ_t &klasses,
                                          منطقي use_class_zero,
                                          hb_sorted_ویکټور_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  إذا (!klass_map)
    أرجع ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  إذا (!use_class_zero)
    klass_map->سېټ (0, 0);

  بینښه idx = klass_map->has (0) ? 1 : 0;
  لكل (ثابت بینښه k: klasses)
  {
    إذا (klass_map->has (k)) استمر;
    klass_map->سېټ (k, idx);
    idx++;
  }


  إذا (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    أرجع خطأ;

  لكل (بینښه i = 0; i < glyph_and_klass.اوږدتیا; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  أرجع ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

کينډۍ <ډول_نوم Types>
بنية ClassDefFormat1_3
{
  ملګری بنية ClassDef;

  خاص:
  بینښه صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع class_ارزښت[(بینښه صحيح) (glyph_id - پیل_Glyph)];
  }

  بینښه get_population () ثابت
  {
    أرجع class_ارزښت.len;
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::اندازه == 2 ? 1 : 3;
      پیل_Glyph = 0;
      class_ارزښت.len = 0;
      return_trace (ریښتیا);
    }

    hb_codepoint_t glyph_min = (*it).لومړی;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_لومړی)
			       | hb_reduce (hb_max, 0u);
    بینښه glyph_شمېر = glyph_max - glyph_min + 1;

    پیل_Glyph = glyph_min;
    إذا (unlikely (!class_ارزښت.serialize (c, glyph_شمېر))) return_trace (خطأ);
    لكل (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      بینښه idx = gid_klass_pair.لومړی - glyph_min;
      class_ارزښت[idx] = gid_klass_pair.second;
    }
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = تش_اشاره /*OUT*/,
               منطقي keep_تشه_جدول = ریښتیا,
               منطقي use_class_zero = ریښتیا,
               ثابت Coverage* glyph_filter = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_sorted_ویکټور_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سېټ_t orig_klasses;

    hb_codepoint_t پیل = پیل_Glyph;
    hb_codepoint_t پای_ټکی   = پیل + class_ارزښت.len;

    لكل (ثابت hb_codepoint_t gid : + hb_range (پیل, پای_ټکی))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      إذا (new_gid == HB_MAP_ارزښت_INVALID) استمر;
      إذا (glyph_filter && !glyph_filter->has(gid)) استمر;

      بینښه klass = class_ارزښت[gid - پیل];
      إذا (!klass) استمر;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.جمع_کړه (klass);
    }

    إذا (use_class_zero)
    {
      ثابت hb_سېټ_t& glyphset = *c->plan->glyphset_gsub ();
      بینښه glyph_شمېر = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_شمېر <= glyph_and_klass.اوږدتیا;
    }
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_تشه_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && class_ارزښت.sanitize (c));
  }

  بینښه لګښت () ثابت { أرجع 1; }

  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_coverage (سېټ_t *glyphs) ثابت
  {
    بینښه صحيح پیل = 0;
    بینښه صحيح شمېر = class_ارزښت.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (class_ارزښت[i])
	استمر;

      إذا (پیل != i)
	إذا (unlikely (!glyphs->جمع_کړه_range (پیل_Glyph + پیل, پیل_Glyph + i)))
	  أرجع خطأ;

      پیل = i + 1;
    }
    إذا (پیل != شمېر)
      إذا (unlikely (!glyphs->جمع_کړه_range (پیل_Glyph + پیل, پیل_Glyph + شمېر)))
	أرجع خطأ;

    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_class (سېټ_t *glyphs, بینښه klass) ثابت
  {
    بینښه صحيح شمېر = class_ارزښت.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (class_ارزښت[i] == klass) glyphs->جمع_کړه (پیل_Glyph + i);
    أرجع ریښتیا;
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    hb_codepoint_t پیل = پیل_Glyph;
    hb_codepoint_t پای_ټکی = پیل_Glyph + class_ارزښت.len;
    لكل (hb_codepoint_t iter = پیل_Glyph - 1;
	 glyphs->next (&iter) && iter < پای_ټکی;)
      إذا (class_ارزښت[iter - پیل]) أرجع ریښتیا;
    أرجع خطأ;
  }
  منطقي intersects_class (ثابت hb_سېټ_t *glyphs, بینښه klass) ثابت
  {
    بینښه صحيح شمېر = class_ارزښت.len;
    إذا (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سېټ_ارزښت_INVALID;
      إذا (!glyphs->next (&g)) أرجع خطأ;
      إذا (g < پیل_Glyph) أرجع ریښتیا;
      g = پیل_Glyph + شمېر - 1;
      إذا (glyphs->next (&g)) أرجع ریښتیا;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت ډول_نوم Types::HBUINT *arr = class_ارزښت.arrayZ;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (arr[i] == klass && glyphs->has (پیل_Glyph + i))
	أرجع ریښتیا;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_سېټ_t *glyphs, بینښه klass, hb_سېټ_t *intersect_glyphs) ثابت
  {
    بینښه شمېر = class_ارزښت.len;
    إذا (klass == 0)
    {
      بینښه پیل_glyph = پیل_Glyph;
      لكل (uint32_t g = HB_سېټ_ارزښت_INVALID;
	   glyphs->next (&g) && g < پیل_glyph;)
	intersect_glyphs->جمع_کړه (g);

      لكل (uint32_t g = پیل_Glyph + شمېر - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->جمع_کړه (g);

      أرجع;
    }

    لكل (بینښه i = 0; i < شمېر; i++)
      إذا (class_ارزښت[i] == klass && glyphs->has (پیل_Glyph + i))
	intersect_glyphs->جمع_کړه (پیل_Glyph + i);

#إذا 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    بینښه پیل_glyph = پیل_Glyph;
    بینښه پای_ټکی_glyph = پیل_glyph + شمېر;
    لكل (بینښه g = پیل_Glyph - 1;
	 glyphs->next (&g) && g < پای_ټکی_glyph;)
      إذا (class_ارزښت.arrayZ[g - پیل_glyph] == klass)
        intersect_glyphs->جمع_کړه (g);
#د_که_پای
  }

  فراغ intersected_classes (ثابت hb_سېټ_t *glyphs, hb_سېټ_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    إذا (!class_ارزښت.len)
    {
      intersect_classes->جمع_کړه (0);
      أرجع;
    }

    hb_codepoint_t پای_ټکی_glyph = پیل_Glyph + class_ارزښت.len - 1;
    إذا (glyphs->get_min () < پیل_Glyph ||
        glyphs->get_max () > پای_ټکی_glyph)
      intersect_classes->جمع_کړه (0);

    لكل (ثابت خپلکاری& _ : + hb_enumerate (class_ارزښت))
    {
      hb_codepoint_t g = پیل_Glyph + _.لومړی;
      إذا (glyphs->has (g))
        intersect_classes->جمع_کړه (_.second);
    }
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  ډول_نوم Types::HBGlyphID
		 پیل_Glyph;	/* First GlyphID of the classValueArray */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::HBUINT>
		class_ارزښت;	/* Array of Class Values--one per GlyphID */
  عام:
  DEFINE_اندازه_ARRAY (2 + 2 * Types::اندازه, class_ارزښت);
};

کينډۍ <ډول_نوم Types>
بنية ClassDefFormat2_4
{
  ملګری بنية ClassDef;

  خاص:
  بینښه صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع range_ریکارډ.bsearch (glyph_id).value;
  }

  بینښه get_population () ثابت
  {
    ډول_نوم Types::large_int ret = 0;
    لكل (ثابت خپلکاری &r : range_ریکارډ)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بینښه) ret;
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::اندازه == 2 ? 2 : 4;
      range_ریکارډ.len = 0;
      return_trace (ریښتیا);
    }

    بینښه unsorted = خطأ;
    بینښه عدد_ranges = 1;
    hb_codepoint_t prev_gid = (*it).لومړی;
    بینښه prev_klass = (*it).second;

    Range_ریکارډ<Types> range_rec;
    range_rec.لومړی = prev_gid;
    range_rec.وروستی = prev_gid;
    range_rec.value = prev_klass;

    خپلکاری *ریکارډ = c->copy (range_rec);
    إذا (unlikely (!ریکارډ)) return_trace (خطأ);

    لكل (ثابت خپلکاری gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.لومړی;
      بینښه cur_klass = gid_klass_pair.second;

      إذا (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	إذا (unlikely (cur_gid < prev_gid))
	  unsorted = ریښتیا;

	إذا (unlikely (!ریکارډ)) اكسر;
	ریکارډ->وروستی = prev_gid;
	عدد_ranges++;

	range_rec.لومړی = cur_gid;
	range_rec.وروستی = cur_gid;
	range_rec.value = cur_klass;

	ریکارډ = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    إذا (unlikely (c->دننه_error ())) return_trace (خطأ);

    إذا (likely (ریکارډ)) ریکارډ->وروستی = prev_gid;
    range_ریکارډ.len = عدد_ranges;

    إذا (unlikely (unsorted))
      range_ریکارډ.as_array ().qsort (Range_ریکارډ<Types>::cmp_range);

    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = تش_اشاره /*OUT*/,
               منطقي keep_تشه_جدول = ریښتیا,
               منطقي use_class_zero = ریښتیا,
               ثابت Coverage* glyph_filter = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyph_سېټ = *c->plan->glyphset_gsub ();

    hb_sorted_ویکټور_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سېټ_t orig_klasses;

    إذا (glyph_سېټ.get_population () * hb_بټ_storage ((بینښه) range_ریکارډ.len)
	< get_population ())
    {
      لكل (hb_codepoint_t g : glyph_سېټ)
      {
	بینښه klass = get_class (g);
	إذا (!klass) استمر;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	إذا (new_gid == HB_MAP_ارزښت_INVALID) استمر;
	إذا (glyph_filter && !glyph_filter->has (g)) استمر;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.جمع_کړه (klass);
      }
    }
    وإلا
    {
      بینښه عدد_سرچینه_glyphs = c->plan->سرچینه->get_عدد_glyphs ();
      لكل (خپلکاری &range : range_ریکارډ)
      {
	بینښه klass = range.value;
	إذا (!klass) استمر;
	hb_codepoint_t پیل = range.لومړی;
	hb_codepoint_t پای_ټکی   = hb_min (range.وروستی + 1, عدد_سرچینه_glyphs);
	لكل (hb_codepoint_t g = پیل; g < پای_ټکی; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  إذا (new_gid == HB_MAP_ارزښت_INVALID) استمر;
	  إذا (glyph_filter && !glyph_filter->has (g)) استمر;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.جمع_کړه (klass);
	}
      }
    }

    ثابت hb_سېټ_t& glyphset = *c->plan->glyphset_gsub ();
    بینښه glyph_شمېر = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_شمېر <= glyph_and_klass.اوږدتیا;
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_تشه_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_ریکارډ.sanitize (c));
  }

  بینښه لګښت () ثابت { أرجع hb_بټ_storage ((بینښه) range_ریکارډ.len); /* bsearch cost */ }

  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_coverage (سېټ_t *glyphs) ثابت
  {
    لكل (خپلکاری &range : range_ریکارډ)
      إذا (range.value)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    أرجع ریښتیا;
  }

  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_class (سېټ_t *glyphs, بینښه صحيح klass) ثابت
  {
    لكل (خپلکاری &range : range_ریکارډ)
    {
      إذا (range.value == klass)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    }
    أرجع ریښتیا;
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    إذا (range_ریکارډ.len > glyphs->get_population () * hb_بټ_storage ((بینښه) range_ریکارډ.len))
    {
      لكل (خپلکاری g : *glyphs)
        إذا (get_class (g))
	  أرجع ریښتیا;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_ریکارډ)
                   | hb_map ([glyphs] (ثابت Range_ریکارډ<Types> &range) { أرجع range.intersects (*glyphs) && range.value; }));
  }
  منطقي intersects_class (ثابت hb_سېټ_t *glyphs, بینښه klass) ثابت
  {
    إذا (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      إذا (!range_ریکارډ.len)
        أرجع !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سېټ_ارزښت_INVALID;
      hb_codepoint_t وروستی = HB_سېټ_ارزښت_INVALID;
      خپلکاری it = hb_iter (range_ریکارډ);
      لكل (خپلکاری &range : it)
      {
        إذا (it->لومړی == وروستی + 1)
	{
	  it++;
	  استمر;
	}

	إذا (!glyphs->next (&g))
	  اكسر;
	إذا (g < range.لومړی)
	  أرجع ریښتیا;
	g = range.وروستی;
	وروستی = g;
      }
      إذا (g != HB_سېټ_ارزښت_INVALID && glyphs->next (&g))
	أرجع ریښتیا;
      /* Fall through. */
    }
    لكل (ثابت خپلکاری &range : range_ریکارډ)
      إذا (range.value == klass && range.intersects (*glyphs))
	أرجع ریښتیا;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_سېټ_t *glyphs, بینښه klass, hb_سېټ_t *intersect_glyphs) ثابت
  {
    إذا (klass == 0)
    {
      hb_codepoint_t g = HB_سېټ_ارزښت_INVALID;
      لكل (خپلکاری &range : range_ریکارډ)
      {
	إذا (!glyphs->next (&g))
	  اذهب بشپړ;
	طالما (g < range.لومړی)
	{
	  intersect_glyphs->جمع_کړه (g);
	  إذا (!glyphs->next (&g))
	    اذهب بشپړ;
        }
        g = range.وروستی;
      }
      طالما (glyphs->next (&g))
	intersect_glyphs->جمع_کړه (g);
      بشپړ:

      أرجع;
    }

    بینښه شمېر = range_ریکارډ.len;
    إذا (شمېر > glyphs->get_population () * hb_بټ_storage (شمېر))
    {
      لكل (خپلکاری g : *glyphs)
      {
        بینښه i;
        إذا (range_ریکارډ.as_array ().bfind (g, &i) &&
	    range_ریکارډ.arrayZ[i].value == klass)
	  intersect_glyphs->جمع_کړه (g);
      }
      أرجع;
    }

    لكل (خپلکاری &range : range_ریکارډ)
    {
      إذا (range.value != klass) استمر;

      بینښه پای_ټکی = range.وروستی + 1;
      لكل (hb_codepoint_t g = range.لومړی - 1;
	   glyphs->next (&g) && g < پای_ټکی;)
	intersect_glyphs->جمع_کړه (g);
    }
  }

  فراغ intersected_classes (ثابت hb_سېټ_t *glyphs, hb_سېټ_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    إذا (!range_ریکارډ.len)
    {
      intersect_classes->جمع_کړه (0);
      أرجع;
    }

    hb_codepoint_t g = HB_سېټ_ارزښت_INVALID;
    لكل (خپلکاری &range : range_ریکارډ)
    {
      إذا (!glyphs->next (&g))
        اكسر;
      إذا (g < range.لومړی)
      {
        intersect_classes->جمع_کړه (0);
        اكسر;
      }
      g = range.وروستی;
    }
    إذا (g != HB_سېټ_ارزښت_INVALID && glyphs->next (&g))
      intersect_classes->جمع_کړه (0);

    لكل (ثابت خپلکاری& range : range_ریکارډ)
      إذا (range.intersects (*glyphs))
        intersect_classes->جمع_کړه (range.value);
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ Sorted_Array_د<Range_ریکارډ<Types>>
		range_ریکارډ;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::اندازه, range_ریکارډ);
};

بنية ClassDef
{
  /* Has interface. */
  بینښه چلوونکی [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Projection. */
  hb_codepoint_t چلوونکی () (hb_codepoint_t k) ثابت { أرجع get (k); }

  بینښه صحيح get (hb_codepoint_t k) ثابت { أرجع get_class (k); }
  بینښه صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_class (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_class (glyph_id);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_class (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_class (glyph_id);
#د_که_پای
    افتراضي:أرجع 0;
    }
  }
  بینښه صحيح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    بینښه klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    klass = get_class (glyph_id);
    إذا (cache) cache->سېټ (glyph_id, klass);
    أرجع klass;
  }

  بینښه get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#د_که_پای
    افتراضي:أرجع NOT_COVERED;
    }
  }

  کينډۍ<ډول_نوم Iterator,
	   hb_requires (hb_is_sorted_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    خپلکاری glyphs = + it_with_class_zero | hb_filter (hb_دوهم);

    بینښه format = 2;
    hb_codepoint_t glyph_max = 0;
    بینښه class_max = 0;
    إذا (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).لومړی;
      glyph_max = glyph_min;

      بینښه عدد_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      بینښه prev_klass = (*glyphs).second;

      لكل (ثابت خپلکاری gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.لومړی;
	بینښه cur_klass = gid_klass_pair.second;
	إذا (cur_klass > class_max) class_max = cur_klass;
	إذا (cur_gid == glyph_min) استمر;
        إذا (cur_gid > glyph_max) glyph_max = cur_gid;
	إذا (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  عدد_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t غوره_اندازه = UINT64_MAX;
      بینښه glyph_پراختیا = glyph_max - glyph_min + 1;
      خپلکاری consider = [&] (بینښه candidate, uint64_t اندازه)
      {
	إذا (اندازه >= غوره_اندازه) أرجع;
	غوره_اندازه = اندازه;
	format = candidate;
      };

      إذا (glyph_max <= 0xFFFFu && glyph_پراختیا <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_اندازه * glyph_پراختیا);
      إذا (glyph_max <= 0xFFFFu && عدد_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID16::static_اندازه + HBUINT16::static_اندازه));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
      إذا (glyph_max <= 0xFFFFFFu && glyph_پراختیا <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_اندازه * glyph_پراختیا);
      إذا (glyph_max <= 0xFFFFFFu && عدد_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID24::static_اندازه + HBUINT16::static_اندازه));
#د_که_پای
      إذا (unlikely (غوره_اندازه == UINT64_MAX))
      {
	c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_ډکېدنه);
	return_trace (خطأ);
      }
    }

    u.format.v = format;

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_دوهم)));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_دوهم)));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_دوهم)));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_دوهم)));
#د_که_پای
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = تش_اشاره /*OUT*/,
               منطقي keep_تشه_جدول = ریښتیا,
               منطقي use_class_zero = ریښتیا,
               ثابت Coverage* glyph_filter = تش_اشاره) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_تشه_جدول, use_class_zero, glyph_filter));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_تشه_جدول, use_class_zero, glyph_filter));
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_تشه_جدول, use_class_zero, glyph_filter));
    حالة 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_تشه_جدول, use_class_zero, glyph_filter));
#د_که_پای
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
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#د_که_پای
    افتراضي:return_trace (ریښتیا);
    }
  }

  بینښه لګښت () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.لګښت ();
    حالة 2: hb_barrier (); أرجع u.format2.لګښت ();
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.لګښت ();
    حالة 4: hb_barrier (); أرجع u.format4.لګښت ();
#د_که_پای
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_coverage (سېټ_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  کينډۍ <ډول_نوم سېټ_t>
  منطقي collect_class (سېټ_t *glyphs, بینښه صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.collect_class (glyphs, klass);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.collect_class (glyphs, klass);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_class (ثابت hb_سېټ_t *glyphs, بینښه صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_class (glyphs, klass);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_class (glyphs, klass);
#د_که_پای
    افتراضي:أرجع خطأ;
    }
  }

  فراغ intersected_class_glyphs (ثابت hb_سېټ_t *glyphs, بینښه klass, hb_سېټ_t *intersect_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#د_که_پای
    افتراضي:أرجع;
    }
  }

  فراغ intersected_classes (ثابت hb_سېټ_t *glyphs, hb_سېټ_t *intersect_classes) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_classes (glyphs, intersect_classes);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_classes (glyphs, intersect_classes);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_classes (glyphs, intersect_classes);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_classes (glyphs, intersect_classes);
#د_که_پای
    افتراضي:أرجع;
    }
  }


  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

کينډۍ<ډول_نوم Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ أرجع (c->پیل_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
بنية delta_لیکه_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  بنية chars_t : hb_ویکټور_t<uint8_t>
  {
    صحيح cmp (ثابت chars_t& other) ثابت
    {
      أرجع as_array ().cmp (other.as_array ());
    }

    hb_pair_t<بینښه, بینښه> get_پلنوالی ()
    {
      بینښه پلنوالی = 0;
      بینښه columns = 0;
      لكل (بینښه i = 0; i < اوږدتیا; i++)
      {
	بینښه v = arrayZ[i];
	پلنوالی += v;
	columns += (v != 0);
      }
      أرجع hb_pair (پلنوالی, columns);
    }

    HB_HOT
    hb_pair_t<بینښه, بینښه> combine_پلنوالی (ثابت chars_t& other) ثابت
    {
      بینښه combined_پلنوالی = 0;
      بینښه combined_columns = 0;
      لكل (بینښه i = 0; i < اوږدتیا; i++)
      {
        بینښه v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_پلنوالی += v;
	combined_columns += (v != 0);
      }
      أرجع hb_pair (combined_پلنوالی, combined_columns);
    }
  };

  hb_pair_t<بینښه, بینښه> combine_پلنوالی (ثابت delta_لیکه_encoding_t& other_encoding) ثابت { أرجع chars.combine_پلنوالی (other_encoding.chars); }

  // Actual data

  chars_t chars;
  بینښه پلنوالی = 0;
  بینښه overhead = 0;
  hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح>*> توکي;

  delta_لیکه_encoding_t () = افتراضي;
  delta_لیکه_encoding_t (hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح>*> &&لیکې, بینښه عدد_ستون_شمېر)
  {
    أكد (لیکې);

    توکي = std::move (لیکې);

    إذا (unlikely (!chars.resize (عدد_ستون_شمېر)))
      أرجع;

    calculate_chars ();
  }

  فراغ یوځای (ثابت delta_لیکه_encoding_t& other)
  {
    توکي.alloc (توکي.اوږدتیا + other.توکي.اوږدتیا);
    لكل (خپلکاری &لیکه : other.توکي)
      جمع_کړه_لیکه (لیکه);

    // Merge chars
    أكد (chars.اوږدتیا == other.chars.اوږدتیا);
    لكل (بینښه i = 0; i < chars.اوږدتیا; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_بدل_شوی ();
  }

  فراغ chars_بدل_شوی ()
  {
    خپلکاری _ = chars.get_پلنوالی ();
    پلنوالی = _.لومړی;
    overhead = get_chars_overhead (_.second);
  }

  فراغ calculate_chars ()
  {
    أكد (توکي);

    منطقي long_ټکي = خطأ;

    لكل (خپلکاری &لیکه : توکي)
    {
      أكد (لیکه->اوږدتیا == chars.اوږدتیا);

      /* 0/1/2 byte encoding */
      لكل (بینښه i = 0; i < لیکه->اوږدتیا; i++)
      {
	صحيح v =  لیکه->arrayZ[i];
	إذا (v == 0)
	  استمر;
	وإلا إذا (v > 32767 || v < -32768)
	{
	  long_ټکي = ریښتیا;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	وإلا إذا (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	وإلا
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    إذا (long_ټکي)
    {
      // Convert 1s to 2s
      لكل (خپلکاری &v : chars)
	إذا (v == 1)
	  v = 2;
    }

    chars_بدل_شوی ();
  }

  منطقي is_empty () ثابت
  { أرجع !توکي; }

  منطقي دننه_error () ثابت
  { أرجع chars.دننه_error () || توکي.دننه_error (); }

  ساكن مضمن بینښه get_chars_overhead (بینښه عدد_columns)
  {
    بینښه c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    أرجع c + عدد_columns * 2;
  }

  بینښه get_gain (بینښه additional_بایټونه_فی_لیکې = 1) ثابت
  {
    صحيح شمېر = توکي.اوږدتیا;
    أرجع hb_max (0, (صحيح) overhead - شمېر * (صحيح) additional_بایټونه_فی_لیکې);
  }

  صحيح gain_from_merging (ثابت delta_لیکه_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    نښهدار additional_بایټونه_فی_لیکې = other_encoding.پلنوالی - پلنوالی;
    إذا (additional_بایټونه_فی_لیکې > 0)
    {
      إذا (get_gain (additional_بایټونه_فی_لیکې) == 0)
        أرجع 0;
    }
    وإلا
    {
      إذا (other_encoding.get_gain (-additional_بایټونه_فی_لیکې) == 0)
	أرجع 0;
    }

    خپلکاری pair = combine_پلنوالی (other_encoding);
    بینښه combined_پلنوالی = pair.لومړی;
    بینښه combined_columns = pair.second;

    صحيح combined_gain = (صحيح) overhead + (صحيح) other_encoding.overhead;
    combined_gain -= (combined_پلنوالی - (صحيح) پلنوالی) * توکي.اوږدتیا;
    combined_gain -= (combined_پلنوالی - (صحيح) other_encoding.پلنوالی) * other_encoding.توکي.اوږدتیا;
    combined_gain -= get_chars_overhead (combined_columns);

    أرجع combined_gain;
  }

  منطقي جمع_کړه_لیکه (ثابت hb_ویکټور_t<صحيح>* لیکه)
  { أرجع توکي.push (لیکه); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت delta_لیکه_encoding_t *a = (ثابت delta_لیکه_encoding_t *)pa;
    ثابت delta_لیکه_encoding_t *b = (ثابت delta_لیکه_encoding_t *)pb;

    إذا (a->پلنوالی != b->پلنوالی)
      أرجع (صحيح) a->پلنوالی - (صحيح) b->پلنوالی;

    أرجع b->chars.cmp (a->chars);
  }
};

بنية VarRegionAxis
{
  عائم evaluate (صحيح coord) ثابت
  {
    صحيح peak = peakCoord.منزل_int ();
    إذا (peak == 0 || coord == peak)
      أرجع 1.f;
    وإلا إذا (coord == 0) // Faster
      أرجع 0.f;

    صحيح پیل = پیل_Coord.منزل_int (), پای_ټکی = پای_ټکی_Coord.منزل_int ();

    /* TODO Move these to sanitize(). */
    إذا (unlikely (پیل > peak || peak > پای_ټکی))
      أرجع 1.f;
    إذا (unlikely (پیل < 0 && پای_ټکی > 0))
      أرجع 1.f;

    إذا (coord <= پیل || پای_ټکی <= coord)
      أرجع 0.f;

    /* Interpolate */
    إذا (coord < peak)
      أرجع عائم (coord - پیل) / (peak - پیل);
    وإلا
      أرجع عائم (پای_ټکی - coord) / (پای_ټکی - peak);
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
  F2DOT14	پیل_Coord;
  F2DOT14	peakCoord;
  F2DOT14	پای_ټکی_Coord;
  عام:
  DEFINE_اندازه_STATIC (6);
};
بنية SparseVarRegionAxis
{
  عائم evaluate (ثابت صحيح *coords, بینښه صحيح coord_len) ثابت
  {
    بینښه i = axis_شاخص_2;
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
    خپلکاری *بهر = c->embed (src);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    إذا (axis_map)
    {
      إذا (unlikely (!axis_map->has (بهر->axis_شاخص_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (خطأ);
      }
      إذا (unlikely (!c->check_ټاکل (بهر->axis_شاخص_2, axis_map->get (بهر->axis_شاخص_2),
				    HB_SERIALIZE_ERROR_INT_ډکېدنه)))
	return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  عام:
  HBUINT16 axis_شاخص_2;
  VarRegionAxis axis;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية hb_scalar_cache_t
{
  خاص:
  ساكن constexpr بینښه STATIC_اوږدتیا = 128;
  ساكن constexpr عائم INVALID = NAN;

  عام:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = ړنګول;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = ړنګول;
  hb_scalar_cache_t& چلوونکی= (ثابت hb_scalar_cache_t&) = ړنګول;
  hb_scalar_cache_t& چلوونکی= (hb_scalar_cache_t&&) = ړنګول;

  ساكن hb_scalar_cache_t *create (بینښه صحيح شمېر,
				    hb_scalar_cache_t *scratch_cache = تش_اشاره)
  {
    إذا (!شمېر) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    إذا (scratch_cache && شمېر <= STATIC_اوږدتیا)
    {
      scratch_cache->اوږدتیا = شمېر;
      scratch_cache->clear ();
      أرجع scratch_cache;
    }

    خپلکاری *cache = (hb_scalar_cache_t *) hb_malloc (حجم (hb_scalar_cache_t) - حجم (static_ارزښتونه) + حجم (static_ارزښتونه[0]) * شمېر);
    إذا (unlikely (!cache)) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->اوږدتیا = شمېر;
    cache->clear ();

    أرجع cache;
  }

  ساكن فراغ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = تش_اشاره)
  {
    إذا (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  فراغ clear ()
  {
    خپلکاری *values = &static_ارزښتونه[0];
    بینښه i = 0;
#که_نه_تعریف_شوی HB_OPTIMIZE_اندازه
    لكل (; i + 3 < اوږدتیا; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#د_که_پای
    لكل (; i < اوږدتیا; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقي get (بینښه i, عائم *value) ثابت
  {
    إذا (unlikely (i >= اوږدتیا))
    {
      *value = 0.f;
      أرجع ریښتیا;
    }
    خپلکاری *values = &static_ارزښتونه[0];
    خپلکاری *cached_ارزښت = &values[i];
    عائم v = *cached_ارزښت;
    إذا (std::isnan (v))
      أرجع خطأ;
    *value = v;
    أرجع ریښتیا;
  }

  HB_ALWAYS_INLINE
  فراغ سېټ (بینښه i, عائم value)
  {
    إذا (unlikely (i >= اوږدتیا)) أرجع;
    خپلکاری *values = &static_ارزښتونه[0];
    خپلکاری *cached_ارزښت = &values[i];
    *cached_ارزښت = value;
  }

  عام:
  بینښه اوږدتیا;
  خاص:
  بدلیدونکی hb_atomic_t<عائم> static_ارزښتونه[STATIC_اوږدتیا];
};

بنية VarRegionList
{
  خاص:
  عائم evaluate_impl (بینښه صحيح region_شاخص,
		       ثابت صحيح *coords, بینښه صحيح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_شاخص * axis_شمېر_2);
    عائم v = 1.f;

    بینښه صحيح شمېر = axis_شمېر_2;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      صحيح coord = i < coord_len ? coords[i] : 0;
      عائم ضریب = axes[i].evaluate (coord);
      إذا (ضریب == 0.f)
      {
	v = 0.f;
	اكسر;
      }
      v *= ضریب;
    }

    أرجع v;
  }

  عام:
  HB_ALWAYS_INLINE
  عائم evaluate (بینښه صحيح region_شاخص,
		  ثابت صحيح *coords, بینښه صحيح coord_len,
		  hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    إذا (unlikely (region_شاخص >= region_شمېر))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_شاخص, &v))
      أرجع v;

    v = evaluate_impl (region_شاخص, coords, coord_len);

    إذا (cache)
      cache->سېټ (region_شاخص, v);
    أرجع v;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_شمېر_2 * region_شمېر));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_ویکټور_t<hb_نښان_t>& axis_نښانونه,
                  ثابت hb_ویکټور_t<ثابت hb_hashmap_t<hb_نښان_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (هذا);
    بینښه axis_شمېر = axis_نښانونه.اوږدتیا;
    بینښه region_شمېر_2 = regions.اوږدتیا;
    إذا (!axis_شمېر || !region_شمېر_2) return_trace (خطأ);
    إذا (unlikely (hb_unsigned_mul_overflows (axis_شمېر * region_شمېر_2,
                                             VarRegionAxis::static_اندازه))) return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_شمېر_2 = axis_شمېر;
    region_شمېر = region_شمېر_2;

    لكل (بینښه r = 0; r < region_شمېر_2; r++)
    {
      ثابت خپلکاری& region = regions[r];
      لكل (بینښه i = 0; i < axis_شمېر; i++)
      {
        hb_نښان_t نښان_2 = axis_نښانونه.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        إذا (region->has (نښان_2, &coords))
        {
          var_region_rec.پیل_Coord.سېټ_float (coords->minimum);
          var_region_rec.peakCoord.سېټ_float (coords->منځ);
          var_region_rec.پای_ټکی_Coord.سېټ_float (coords->اعظمي);
        }
        وإلا
        {
          var_region_rec.پیل_Coord.سېټ_int (0);
          var_region_rec.peakCoord.سېټ_int (0);
          var_region_rec.پای_ټکی_Coord.سېټ_int (0);
        }
        إذا (!var_region_rec.serialize (c))
          return_trace (خطأ);
      }
    }
    return_trace (ریښتیا);
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت VarRegionList *src, ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_شمېر_2 = src->axis_شمېر_2;
    region_شمېر = region_map.get_population ();
    إذا (unlikely (hb_unsigned_mul_overflows (axis_شمېر_2 * region_شمېر,
					     VarRegionAxis::static_اندازه))) return_trace (خطأ);
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
    بینښه صحيح region_شمېر_2 = src->region_شمېر;
    لكل (بینښه صحيح r = 0; r < region_شمېر; r++)
    {
      بینښه صحيح backward = region_map.backward (r);
      إذا (backward >= region_شمېر_2) return_trace (خطأ);
      hb_memcpy (&axesZ[axis_شمېر_2 * r], &src->axesZ[axis_شمېر_2 * backward], VarRegionAxis::static_اندازه * axis_شمېر_2);
    }

    return_trace (ریښتیا);
  }

  منطقي get_var_region (بینښه region_شاخص,
                       ثابت hb_map_t& axes_old_شاخص_نښان_map,
                       hb_hashmap_t<hb_نښان_t, Triple>& axis_tuples /*بهر*/) ثابت
  {
    إذا (region_شاخص >= region_شمېر) أرجع خطأ;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_شاخص * axis_شمېر_2);
    لكل (بینښه i = 0; i < axis_شمېر_2; i++)
    {
      hb_نښان_t *axis_نښان_2;
      إذا (!axes_old_شاخص_نښان_map.has (i, &axis_نښان_2))
        أرجع خطأ;

      عائم min_val = axis_region->پیل_Coord.منزل_float ();
      عائم def_val = axis_region->peakCoord.منزل_float ();
      عائم max_val = axis_region->پای_ټکی_Coord.منزل_float ();

      إذا (def_val != 0.f)
        axis_tuples.سېټ (*axis_نښان_2, Triple ((مزدوج) min_val, (مزدوج) def_val, (مزدوج) max_val));
      axis_region++;
    }
    أرجع !axis_tuples.دننه_error ();
  }

  منطقي get_var_regions (ثابت hb_map_t& axes_old_شاخص_نښان_map,
                        hb_ویکټور_t<hb_hashmap_t<hb_نښان_t, Triple>>& regions /*بهر*/) ثابت
  {
    إذا (!regions.alloc (region_شمېر))
      أرجع خطأ;

    لكل (بینښه i = 0; i < region_شمېر; i++)
    {
      hb_hashmap_t<hb_نښان_t, Triple> axis_tuples;
      إذا (!get_var_region (i, axes_old_شاخص_نښان_map, axis_tuples))
        أرجع خطأ;
      regions.push (std::move (axis_tuples));
    }
    أرجع !regions.دننه_error ();
  }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_اندازه,
							       axis_شمېر_2, region_شمېر)); }

  عام:
  HBUINT16	axis_شمېر_2;
  HBUINT15	region_شمېر;
  محمي:
  Unsized_Array_د<VarRegionAxis>
		axesZ;
  عام:
  DEFINE_اندازه_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ساكن مضمن منطقي
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_نښان_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_نښان_t, Triple> &reachable_ranges)
{
  لكل (خپلکاری _ : axis_tuples)
  {
    Triple *range;
    إذا (!reachable_ranges.has (_.لومړی, &range)) استمر;
    ثابت Triple &tent = _.second;
    صحيح پیل = (صحيح) roundf ((عائم) tent.minimum * 16384.f);
    صحيح peak  = (صحيح) roundf ((عائم) tent.منځ * 16384.f);
    صحيح پای_ټکی   = (صحيح) roundf ((عائم) tent.اعظمي * 16384.f);
    إذا (!peak) استمر;
    إذا (پیل > peak || peak > پای_ټکی) استمر;
    إذا (پیل < 0 && پای_ټکی > 0) استمر;
    صحيح lo = (صحيح) roundf ((عائم) range->minimum * 16384.f);
    صحيح hi = (صحيح) roundf ((عائم) range->اعظمي * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    إذا (hi < پیل || lo > پای_ټکی)
      أرجع ریښتیا;
  }
  أرجع خطأ;
}

بنية SparseVariationRegion
{
  عائم evaluate (ثابت صحيح *coords, بینښه صحيح coord_len) ثابت
  {
    عائم v = 1.f;
    بینښه صحيح شمېر = axes.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      عائم ضریب = (هذا+axes[i]).evaluate (coords, coord_len);
      إذا (ضریب == 0.f)
	أرجع 0.;
      v *= ضریب;
    }
    أرجع v;
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت SparseVariationRegion *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!axes.serialize (c, (بینښه) src->axes.len))) return_trace (خطأ);
    لكل (بینښه i = 0; i < axes.len; i++)
      إذا (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (axes.sanitize (c, هذا));
  }

  محمي:
  Array16_د<Offset32_منزل<SparseVarRegionAxis>> axes;
  عام:
  DEFINE_اندازه_ARRAY (2, axes);
};

بنية SparseVarRegionList
{
  HB_ALWAYS_INLINE
  عائم evaluate (بینښه صحيح region_شاخص,
		  ثابت صحيح *coords, بینښه صحيح coord_len,
		  hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    إذا (unlikely (region_شاخص >= regions.len))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_شاخص, &v))
      أرجع v;

    ثابت SparseVariationRegion &region = هذا+regions[region_شاخص];

    v = region.evaluate (coords, coord_len);
    إذا (cache)
      cache->سېټ (region_شاخص, v);

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

    لكل (بینښه i = 0; i < regions.len; i++)
    {
      بینښه old_شاخص = region_map.backward (i);
      إذا (unlikely (old_شاخص >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_شاخص]),
					     axis_map)))
	return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  عام:
  Array32_د<Offset32_منزل<SparseVariationRegion>>
		regions;
  عام:
  DEFINE_اندازه_ARRAY (4, regions);
};


بنية Var_معلومات
{
  بینښه صحيح get_توکی_شمېر () ثابت
  { أرجع توکی_شمېر; }

  بینښه صحيح get_region_شاخص_شمېر () ثابت
  { أرجع regionIndices.len; }

  بینښه get_region_شاخص (بینښه i) ثابت
  { أرجع i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_لیکه_اندازه () ثابت
  { أرجع (کلمه_شمېر () + regionIndices.len) * (long_ټکي_2 () ? 2 : 1); }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه - regionIndices.min_اندازه,
				     regionIndices.get_اندازه (),
				     hb_unsigned_mul_saturate (توکی_شمېر, get_لیکه_اندازه ())); }

  کينډۍ <ډول_نوم Float = عائم>
  Float _get_delta (بینښه صحيح inner,
		    ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    إذا (unlikely (inner >= توکی_شمېر))
      أرجع 0.;
    منطقي is_long = long_ټکي_2 ();
    بینښه صحيح شمېر = regionIndices.len;
    بینښه کلمه_شمېر_2 = کلمه_شمېر ();
    بینښه صحيح scount = is_long ? شمېر : کلمه_شمېر_2;
    بینښه صحيح lcount = is_long ? کلمه_شمېر_2 : 0;

    ثابت HBUINT8 *بایټونه = get_delta_بایټونه ();
    ثابت HBUINT8 *لیکه = بایټونه + inner * get_لیکه_اندازه ();

    Float delta = 0;
    بینښه صحيح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (لیکه);
    لكل (; i < lcount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمېر, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *lcursor;
      lcursor++;
    }
    ثابت HBINT16 *scursor = reinterpret_cast<ثابت HBINT16 *> (lcursor);
    لكل (; i < scount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمېر, cache);
      إذا (scalar)
       delta += (Float) scalar * (صحيح) *scursor;
      scursor++;
    }
    ثابت HBINT8 *bcursor = reinterpret_cast<ثابت HBINT8 *> (scursor);
    لكل (; i < شمېر; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمېر, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *bcursor;
      bcursor++;
    }

    أرجع delta;
  }

  کينډۍ <ډول_نوم Float = عائم>
  HB_ALWAYS_INLINE
  Float get_delta (بینښه صحيح inner,
		   ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    بینښه صحيح شمېر = regionIndices.len;
    إذا (!شمېر) أرجع Float (0); // This is quite common, so optimize it.
    أرجع _get_delta<Float> (inner, coords, coord_شمېر, regions, cache);
  }

  فراغ get_region_scalars (ثابت صحيح *coords, بینښه صحيح coord_شمېر,
			   ثابت VarRegionList &regions,
			   عائم *scalars /*OUT */,
			   بینښه صحيح عدد_scalars) ثابت
  {
    بینښه شمېر = hb_min (عدد_scalars, regionIndices.len);
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمېر);
    لكل (بینښه صحيح i = شمېر; i < عدد_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  کلمه_شمېر () <= regionIndices.len &&
		  c->check_range (get_delta_بایټونه (),
				  توکی_شمېر,
				  get_لیکه_اندازه ()));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_ویکټور_t<ثابت hb_ویکټور_t<صحيح>*>& لیکې)
  {
    TRACE_SERIALIZE (هذا);
    بینښه لیکه_شمېر_3 = لیکې.اوږدتیا;
    إذا (!لیکه_شمېر_3) {
      // Nothing to serialize, will be empty.
      أرجع خطأ;
    }

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!c->check_ټاکل (توکی_شمېر, لیکه_شمېر_3,
                                    HB_SERIALIZE_ERROR_INT_ډکېدنه)))
      return_trace (خطأ);

    صحيح min_پوله = has_long ? -32768 : -128;
    صحيح max_پوله = has_long ? +32767 : +127;
    تعداد delta_اندازه_t { kZero=0, k_Non_کلمه, k_کلمه };
    hb_ویکټور_t<delta_اندازه_t> delta_sz;
    بینښه عدد_regions = لیکې[0]->اوږدتیا;
    إذا (!delta_sz.resize (عدد_regions))
      return_trace (خطأ);

    بینښه کلمه_شمېر_2 = 0;
    لكل (بینښه r = 0; r < عدد_regions; r++)
    {
      لكل (بینښه i = 0; i < لیکه_شمېر_3; i++)
      {
        صحيح delta = لیکې[i]->arrayZ[r];
        إذا (delta < min_پوله || delta > max_پوله)
        {
          delta_sz[r] = k_کلمه;
          کلمه_شمېر_2++;
          اكسر;
        }
        وإلا إذا (delta != 0)
        {
          delta_sz[r] = k_Non_کلمه;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    بینښه کلمه_شاخص = 0;
    بینښه non_کلمه_شاخص = کلمه_شمېر_2;
    hb_map_t ri_map;
    لكل (بینښه r = 0; r < عدد_regions; r++)
    {
      إذا (!delta_sz[r]) استمر;
      بینښه new_r = (delta_sz[r] == k_کلمه)? کلمه_شاخص++ : non_کلمه_شاخص++;
      إذا (!ri_map.سېټ (new_r, r))
        return_trace (خطأ);
    }

    کلمه_اندازه_شمېر = کلمه_شمېر_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    بینښه ri_شمېر = ri_map.get_population ();
    regionIndices.len = ri_شمېر;
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (بینښه r = 0; r < ri_شمېر; r++)
    {
      hb_codepoint_t *idx;
      إذا (!ri_map.has (r, &idx))
        return_trace (خطأ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_بایټونه = get_delta_بایټونه ();
    بینښه لیکه_اندازه = get_لیکه_اندازه ();
    لكل (بینښه صحيح i = 0; i < لیکه_شمېر_3; i++)
    {
      لكل (بینښه صحيح r = 0; r < ri_شمېر; r++)
      {
        صحيح delta = لیکې[i]->arrayZ[ri_map[r]];
        سېټ_توکی_delta_fast (i, r, delta, delta_بایټونه, لیکه_اندازه);
      }
    }
    return_trace (ریښتیا);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Var_معلومات *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_سېټ_t *culled_regions = تش_اشاره)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    توکی_شمېر = inner_map.get_بل_ارزښت ();

    /* Optimize word count */
    بینښه ri_شمېر = src->regionIndices.len;
    تعداد delta_اندازه_t { kZero=0, k_Non_کلمه, k_کلمه };
    hb_ویکټور_t<delta_اندازه_t> delta_sz;
    hb_ویکټور_t<بینښه صحيح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_شمېر);
    ri_map.resize (ri_شمېر);
    بینښه صحيح new_کلمه_شمېر = 0;
    بینښه صحيح r;

    ثابت HBUINT8 *src_delta_بایټونه = src->get_delta_بایټونه ();
    بینښه src_لیکه_اندازه = src->get_لیکه_اندازه ();
    بینښه src_کلمه_شمېر = src->کلمه_شمېر ();
    منطقي     src_long_ټکي = src->long_ټکي_2 ();

    منطقي has_long = خطأ;
    إذا (src_long_ټکي)
    {
      لكل (r = 0; r < src_کلمه_شمېر; r++)
      {
        لكل (بینښه old_gid : inner_map.کلیګانې())
	{
	  int32_t delta = src->get_توکی_delta_fast (old_gid, r, src_delta_بایټونه, src_لیکه_اندازه);
	  إذا (delta < -32768 || 32767 < delta)
	  {
	    has_long = ریښتیا;
	    اكسر;
	  }
        }
      }
    }

    نښهدار min_پوله = has_long ? -32768 : -128;
    نښهدار max_پوله = has_long ? +32767 : +127;
    لكل (r = 0; r < ri_شمېر; r++)
    {
      منطقي short_circuit = src_long_ټکي == has_long && src_کلمه_شمېر <= r;

      delta_sz[r] = kZero;
      إذا (culled_regions && culled_regions->has (src->regionIndices[r]))
	استمر; /* unreachable region: drop its column */
      لكل (بینښه old_gid : inner_map.کلیګانې())
      {
	int32_t delta = src->get_توکی_delta_fast (old_gid, r, src_delta_بایټونه, src_لیکه_اندازه);
	إذا (delta < min_پوله || max_پوله < delta)
	{
	  delta_sz[r] = k_کلمه;
	  new_کلمه_شمېر++;
	  اكسر;
	}
	وإلا إذا (delta != 0)
	{
	  delta_sz[r] = k_Non_کلمه;
	  إذا (short_circuit)
	    اكسر;
	}
      }
    }

    بینښه صحيح کلمه_شاخص = 0;
    بینښه صحيح non_کلمه_شاخص = new_کلمه_شمېر;
    بینښه صحيح new_ri_شمېر = 0;
    لكل (r = 0; r < ri_شمېر; r++)
      إذا (delta_sz[r])
      {
	بینښه new_r = (delta_sz[r] == k_کلمه)? کلمه_شاخص++ : non_کلمه_شاخص++;
	ri_map[new_r] = r;
	new_ri_شمېر++;
      }

    کلمه_اندازه_شمېر = new_کلمه_شمېر | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_شمېر;

    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (r = 0; r < new_ri_شمېر; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_بایټونه = get_delta_بایټونه ();
    بینښه لیکه_اندازه = get_لیکه_اندازه ();
    بینښه شمېر = توکی_شمېر;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      بینښه صحيح old = inner_map.backward (i);
      لكل (بینښه صحيح r = 0; r < new_ri_شمېر; r++)
	سېټ_توکی_delta_fast (i, r,
			     src->get_توکی_delta_fast (old, ri_map[r],
						       src_delta_بایټونه, src_لیکه_اندازه),
			     delta_بایټونه, لیکه_اندازه);
    }

    return_trace (ریښتیا);
  }

  فراغ collect_region_refs (hb_سېټ_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_بایټونه = get_delta_بایټونه ();
    بینښه لیکه_اندازه = get_لیکه_اندازه ();

    لكل (بینښه صحيح r = 0; r < regionIndices.len; r++)
    {
      بینښه صحيح region = regionIndices.arrayZ[r];
      إذا (region_indices.has (region)) استمر;
      لكل (hb_codepoint_t old_gid : inner_map.کلیګانې())
	إذا (get_توکی_delta_fast (old_gid, r, delta_بایټونه, لیکه_اندازه) != 0)
	{
	  region_indices.جمع_کړه (region);
	  اكسر;
	}
    }
  }

  عام:
  ثابت HBUINT8 *get_delta_بایټونه () ثابت
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  محمي:
  HBUINT8 *get_delta_بایټونه ()
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  عام:
  int32_t get_توکی_delta_fast (بینښه صحيح توکی, بینښه صحيح region,
			       ثابت HBUINT8 *delta_بایټونه, بینښه لیکه_اندازه) ثابت
  {
    إذا (unlikely (توکی >= توکی_شمېر || region >= regionIndices.len)) أرجع 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_بایټونه + توکی * لیکه_اندازه;
    بینښه کلمه_شمېر_2 = کلمه_شمېر ();
    منطقي is_long = long_ټکي_2 ();
    إذا (is_long)
    {
      إذا (region < کلمه_شمېر_2)
	أرجع ((ثابت HBINT32 *) p)[region];
      وإلا
	أرجع ((ثابت HBINT16 *)(p + HBINT32::static_اندازه * کلمه_شمېر_2))[region - کلمه_شمېر_2];
    }
    وإلا
    {
      إذا (region < کلمه_شمېر_2)
	أرجع ((ثابت HBINT16 *) p)[region];
      وإلا
	أرجع (p + HBINT16::static_اندازه * کلمه_شمېر_2)[region - کلمه_شمېر_2];
    }
  }
  int32_t get_توکی_delta (بینښه صحيح توکی, بینښه صحيح region) ثابت
  {
     أرجع get_توکی_delta_fast (توکی, region,
				 get_delta_بایټونه (),
				 get_لیکه_اندازه ());
  }

  محمي:
  فراغ سېټ_توکی_delta_fast (بینښه صحيح توکی, بینښه صحيح region, int32_t delta,
			    HBUINT8 *delta_بایټونه, بینښه لیکه_اندازه)
  {
    HBINT8 *p = (HBINT8 *) delta_بایټونه + توکی * لیکه_اندازه;
    بینښه کلمه_شمېر_2 = کلمه_شمېر ();
    منطقي is_long = long_ټکي_2 ();
    إذا (is_long)
    {
      إذا (region < کلمه_شمېر_2)
	((HBINT32 *) p)[region] = delta;
      وإلا
	((HBINT16 *)(p + HBINT32::static_اندازه * کلمه_شمېر_2))[region - کلمه_شمېر_2] = delta;
    }
    وإلا
    {
      إذا (region < کلمه_شمېر_2)
	((HBINT16 *) p)[region] = delta;
      وإلا
	(p + HBINT16::static_اندازه * کلمه_شمېر_2)[region - کلمه_شمېر_2] = delta;
    }
  }
  فراغ سېټ_توکی_delta (بینښه صحيح توکی, بینښه صحيح region, int32_t delta)
  {
    سېټ_توکی_delta_fast (توکی, region, delta,
			 get_delta_بایټونه (),
			 get_لیکه_اندازه ());
  }

  منطقي long_ټکي_2 () ثابت { أرجع کلمه_اندازه_شمېر & 0x8000u /* LONG_WORDS */; }
  بینښه کلمه_شمېر () ثابت { أرجع کلمه_اندازه_شمېر & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محمي:
  HBUINT16		توکی_شمېر;
  HBUINT16		کلمه_اندازه_شمېر;
  Array16_د<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عام:
  DEFINE_اندازه_ARRAY (6, regionIndices);
};

بنية Multi_Var_معلومات
{
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کړه_saturate (min_اندازه - regionIndices.min_اندازه,
				     regionIndices.get_اندازه ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { أرجع هذا+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  فراغ get_delta (بینښه صحيح inner,
		  ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<مزدوج> بهر,
		  hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    خپلکاری &deltaSets = get_delta_sets ();

    خپلکاری ارزښتونه_iter = deltaSets.fetcher (inner);
    بینښه region_شمېر = regionIndices.len;
    بینښه skip = 0;
    لكل (بینښه region_شاخص_2 = 0; region_شاخص_2 < region_شمېر; region_شاخص_2++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[region_شاخص_2],
				       coords, coord_شمېر,
				       cache);
      // We skip lazily. Helps with the tail end.
      إذا (scalar == 0.0f)
        skip += بهر.اوږدتیا;
      وإلا
      {
        إذا (skip)
	{
	  ارزښتونه_iter.skip (skip);
	  skip = 0;
	}
	ارزښتونه_iter.جمع_کړه_منزل_2 (بهر, scalar);
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

  بینښه get_توکی_شمېر () ثابت
  { أرجع get_delta_sets ().شمېر; }

  فراغ collect_region_refs (hb_سېټ_t &region_indices) ثابت
  {
    لكل (بینښه region : regionIndices)
      region_indices.جمع_کړه (region);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_Var_معلومات *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    بینښه region_شمېر_2 = src->regionIndices.len;
    بینښه سرلیک_اندازه = HBUINT8::static_اندازه +
			   Array16_د<HBUINT16>::min_اندازه +
			   region_شمېر_2 * HBUINT16::static_اندازه +
			   HBUINT32::static_اندازه;
    إذا (unlikely (!c->extend_اندازه (هذا, سرلیک_اندازه, خطأ)))
      return_trace (خطأ);
    format = 1;
    regionIndices.len = region_شمېر_2;
    لكل (بینښه i = 0; i < regionIndices.len; i++)
    {
      بینښه old_region = src->regionIndices[i];
      إذا (unlikely (!region_map.has (old_region)))
	return_trace (خطأ);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت خپلکاری &src_delta_sets = src->get_delta_sets ();
    hb_ویکټور_t<hb_ubytes_t> delta_sets;
    بینښه معلومات_اندازه = 0;
    إذا (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (خطأ);
    لكل (بینښه i = 0; i < inner_map.get_population (); i++)
    {
      بینښه old_inner = inner_map.backward (i);
      إذا (unlikely (old_inner >= src_delta_sets.شمېر)) return_trace (خطأ);
      hb_ubytes_t بایټونه = static_cast<ثابت CFF2_شاخص &> (src_delta_sets)[old_inner];
      معلومات_اندازه = hb_unsigned_جمع_کړه_saturate (معلومات_اندازه, بایټونه.اوږدتیا);
      delta_sets.push (بایټونه);
    }
    إذا (unlikely (معلومات_اندازه == UINT_MAX || delta_sets.دننه_error ()))
      return_trace (خطأ);

    خپلکاری &بهر_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (بهر_delta_sets.serialize_serialize (c, delta_sets.iter (), &معلومات_اندازه));
  }

  محمي:
  HBUINT8	      format; // 1
  Array16_د<HBUINT16> regionIndices;
  Offset32_منزل<TupleList> deltaSetsX;
  عام:
  DEFINE_اندازه_MIN (7);
};

بنية توکی_Variation_Store
{
  ملګری بنية توکی_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#د_که_پای
    أرجع hb_scalar_cache_t::create ((هذا+regions).region_شمېر);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  خاص:
  کينډۍ <ډول_نوم Float = عائم>
  Float get_delta (بینښه صحيح outer, بینښه صحيح inner,
		   ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		   hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع Float (0);
#د_که_پای

    إذا (unlikely (outer >= معلومات_Sets.len))
      أرجع Float (0);

    أرجع (هذا+معلومات_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_شمېر,
					     هذا+regions,
					     cache);
  }

  عام:
  منطقي has_delta_سېټ (بینښه صحيح شاخص_2) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع خطأ;
#د_که_پای

    بینښه صحيح outer = شاخص_2 >> 16;
    بینښه صحيح inner = شاخص_2 & 0xFFFF;
    أرجع outer < معلومات_Sets.len &&
	   inner < (هذا+معلومات_Sets[outer]).get_توکی_شمېر ();
  }

  کينډۍ <ډول_نوم Float = عائم>
  Float get_delta (بینښه صحيح شاخص_2,
		   ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		   hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    بینښه صحيح outer = شاخص_2 >> 16;
    بینښه صحيح inner = شاخص_2 & 0xFFFF;
    أرجع get_delta<Float> (outer, inner, coords, coord_شمېر, cache);
  }
  کينډۍ <ډول_نوم Float = عائم>
  Float get_delta (بینښه صحيح شاخص_2,
		   hb_array_t<ثابت صحيح> coords,
		   hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    أرجع get_delta<Float> (شاخص_2,
		      coords.arrayZ, coords.اوږدتیا,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع ریښتیا;
#د_که_پای

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  معلومات_Sets.sanitize (c, هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_ویکټور_t<hb_نښان_t>& axis_نښانونه,
                  ثابت hb_ویکټور_t<ثابت hb_hashmap_t<hb_نښان_t, Triple>*>& region_list,
                  ثابت hb_ویکټور_t<delta_لیکه_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (هذا);
#که_تعریف_شوی HB_NO_VAR
    return_trace (خطأ);
#د_که_پای
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    format = 1;
    إذا (!regions.serialize_serialize (c, axis_نښانونه, region_list))
      return_trace (خطأ);

    بینښه عدد_var_معلومات = vardata_encodings.اوږدتیا;
    إذا (!عدد_var_معلومات) return_trace (خطأ);
    إذا (unlikely (!c->check_ټاکل (معلومات_Sets.len, عدد_var_معلومات,
                                    HB_SERIALIZE_ERROR_INT_ډکېدنه)))
      return_trace (خطأ);

    إذا (unlikely (!c->extend (معلومات_Sets))) return_trace (خطأ);
    لكل (بینښه i = 0; i < عدد_var_معلومات; i++)
      إذا (!معلومات_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].توکي))
        return_trace (خطأ);

    return_trace (ریښتیا);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت توکی_Variation_Store *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_سېټ_t *culled_regions = تش_اشاره)
  {
    TRACE_SERIALIZE (هذا);
#که_تعریف_شوی HB_NO_VAR
    return_trace (خطأ);
#د_که_پای

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بینښه صحيح سېټ_شمېر = 0;
    لكل (بینښه صحيح i = 0; i < inner_maps.اوږدتیا; i++)
      إذا (inner_maps[i].get_population ())
	سېټ_شمېر++;

    format = 1;

    ثابت خپلکاری &src_regions = src+src->regions;

    hb_سېټ_t region_indices;
    لكل (بینښه صحيح i = 0; i < inner_maps.اوږدتیا; i++)
      (src+src->معلومات_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    إذا (region_indices.دننه_error ())
      return_trace (خطأ);

    region_indices.del_range ((src_regions).region_شمېر, hb_سېټ_t::INVALID);

    إذا (culled_regions)
      region_indices.کمول (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (بینښه _) { region_map.جمع_کړه(_); })
    ;
    إذا (region_map.دننه_error())
      return_trace (خطأ);

    إذا (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (خطأ);

    معلومات_Sets.len = سېټ_شمېر;
    إذا (unlikely (!c->extend (معلومات_Sets))) return_trace (خطأ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    بینښه صحيح سېټ_شاخص = 0;
    لكل (بینښه صحيح i = 0; i < inner_maps.اوږدتیا; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!معلومات_Sets[سېټ_شاخص++]
		     .serialize_serialize (c, &(src+src->معلومات_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  توکی_Variation_Store *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->پیل_embed (هذا);
    إذا (unlikely (!بهر)) return_trace (تش_اشاره);

    hb_ویکټور_t <hb_inc_bimap_t> inner_maps;
    بینښه شمېر = معلومات_Sets.len;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      إذا (unlikely (!c->propagate_error(inner_maps)))
        return_trace(تش_اشاره);
      خپلکاری &معلومات = هذا+معلومات_Sets[i];

      بینښه توکی_شمېر = معلومات.get_توکی_شمېر ();
      لكل (بینښه j = 0; j < توکی_شمېر; j++)
	map->جمع_کړه (j);
    }

    إذا (unlikely (!بهر->serialize (c, هذا, inner_maps))) return_trace (تش_اشاره);

    return_trace (بهر);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقي collect_dead_regions (ثابت hb_map_t &axes_old_شاخص_نښان_map,
			     ثابت hb_hashmap_t<hb_نښان_t, Triple> &reachable_ranges,
			     hb_سېټ_t &dead_regions /*بهر*/) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    إذا (!reachable_ranges.get_population ()) أرجع ریښتیا;
    ثابت VarRegionList &region_list = هذا+regions;
    بینښه شمېر = region_list.region_شمېر;
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      hb_hashmap_t<hb_نښان_t, Triple> axis_tuples;
      إذا (!region_list.get_var_region (i, axes_old_شاخص_نښان_map, axis_tuples))
	أرجع خطأ;
      إذا (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.جمع_کړه (i);
    }
    أرجع !dead_regions.دننه_error ();
#وإلا
    أرجع ریښتیا;
#د_که_پای
  }

  منطقي subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (هذا);
#که_تعریف_شوی HB_NO_VAR
    return_trace (خطأ);
#د_که_پای

    توکی_Variation_Store *varstore_اولي = c->serializer->پیل_embed<توکی_Variation_Store> ();
    إذا (unlikely (!varstore_اولي)) return_trace (خطأ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_سېټ_t dead_regions;
    إذا (c->plan->has_avar2)
      إذا (!collect_dead_regions (c->plan->axes_old_شاخص_نښان_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (خطأ);

    varstore_اولي->serialize (c->serializer, هذا, inner_maps,
			       dead_regions.get_population () ? &dead_regions : تش_اشاره);

    return_trace (
        !c->serializer->دننه_error()
        && varstore_اولي->معلومات_Sets);
  }

  بینښه صحيح get_region_شاخص_شمېر (بینښه صحيح major) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع 0;
#د_که_پای
    أرجع (هذا+معلومات_Sets[major]).get_region_شاخص_شمېر ();
  }

  فراغ get_region_scalars (بینښه صحيح major,
			   ثابت صحيح *coords, بینښه صحيح coord_شمېر,
			   عائم *scalars /*OUT*/,
			   بینښه صحيح عدد_scalars) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    لكل (بینښه i = 0; i < عدد_scalars; i++)
      scalars[i] = 0.f;
    أرجع;
#د_که_پای

    (هذا+معلومات_Sets[major]).get_region_scalars (coords, coord_شمېر,
					       هذا+regions,
					       &scalars[0], عدد_scalars);
  }

  بینښه صحيح get_sub_جدول_شمېر () ثابت
   {
#که_تعریف_شوی HB_NO_VAR
     أرجع 0;
#د_که_پای
     أرجع معلومات_Sets.len;
   }

  ثابت Var_معلومات& get_sub_جدول (بینښه i) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
     أرجع Null (Var_معلومات);
#د_که_پای
     أرجع هذا+معلومات_Sets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#که_تعریف_شوی HB_NO_VAR
     أرجع Null (VarRegionList);
#د_که_پای
     أرجع هذا+regions;
  }

  محمي:
  HBUINT16				format;
  Offset32_منزل<VarRegionList>		regions;
  Array16_د_Offset32_منزل<Var_معلومات>		معلومات_Sets;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (8, معلومات_Sets);
};

بنية Multi_توکی_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = تش_اشاره) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#د_که_پای
    خپلکاری &r = هذا+regions;
    بینښه شمېر = r.regions.len;

    أرجع hb_scalar_cache_t::create (شمېر, static_cache);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = تش_اشاره)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  خاص:
  فراغ get_delta (بینښه صحيح outer, بینښه صحيح inner,
		  ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		  hb_array_t<مزدوج> بهر,
		  hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع;
#د_که_پای

    إذا (unlikely (outer >= معلومات_Sets.len))
      أرجع;

    أرجع (هذا+معلومات_Sets[outer]).get_delta (inner,
					     coords, coord_شمېر,
					     هذا+regions,
					     بهر,
					     cache);
  }

  عام:
  فراغ get_delta (بینښه صحيح شاخص_2,
		  ثابت صحيح *coords, بینښه صحيح coord_شمېر,
		  hb_array_t<مزدوج> بهر,
		  hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    بینښه صحيح outer = شاخص_2 >> 16;
    بینښه صحيح inner = شاخص_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_شمېر, بهر, cache);
  }
  فراغ get_delta (بینښه صحيح شاخص_2,
		  hb_array_t<ثابت صحيح> coords,
		  hb_array_t<مزدوج> بهر,
		  hb_scalar_cache_t *cache = تش_اشاره) ثابت
  {
    أرجع get_delta (شاخص_2,
		      coords.arrayZ, coords.اوږدتیا,
		      بهر,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#که_تعریف_شوی HB_NO_VAR
    أرجع ریښتیا;
#د_که_پای

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  معلومات_Sets.sanitize (c, هذا));
  }

  منطقي create_subset_plan (ثابت hb_سېټ_t &var_indices,
			   hb_ویکټور_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    إذا (unlikely (!inner_maps->resize (معلومات_Sets.len))) أرجع خطأ;
    لكل (hb_codepoint_t var_idx : var_indices)
    {
      بینښه outer = var_idx >> 16;
      بینښه inner = var_idx & 0xFFFFu;
      إذا (unlikely (outer >= معلومات_Sets.len ||
		    inner >= (هذا+معلومات_Sets[outer]).get_توکی_شمېر ()))
	أرجع خطأ;
      (*inner_maps)[outer].جمع_کړه (inner);
    }

    بینښه new_outer = 0;
    لكل (بینښه outer = 0; outer < inner_maps->اوږدتیا; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      إذا (!inner_map.get_population ()) استمر;
      إذا (unlikely (inner_map.دننه_error ())) أرجع خطأ;
      لكل (بینښه new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	بینښه old_inner = inner_map.backward (new_inner);
	varidx_map->سېټ ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    أرجع !inner_maps->دننه_error () && !varidx_map->دننه_error ();
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_توکی_Variation_Store *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا) ||
		  inner_maps.اوږدتیا < src->معلومات_Sets.len))
      return_trace (خطأ);
    format = 1;

    ثابت خپلکاری &src_regions = src+src->regions;
    hb_سېټ_t region_indices;
    بینښه سېټ_شمېر = 0;
    لكل (بینښه i = 0; i < src->معلومات_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      سېټ_شمېر++;
      (src+src->معلومات_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_سېټ_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.جمع_کړه_سېټ (&region_indices);
    إذا (unlikely (region_indices.دننه_error () || region_map.دننه_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (خطأ);

    معلومات_Sets.len = سېټ_شمېر;
    إذا (unlikely (!c->extend (معلومات_Sets))) return_trace (خطأ);
    بینښه سېټ_شاخص = 0;
    لكل (بینښه i = 0; i < src->معلومات_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!معلومات_Sets[سېټ_شاخص++].serialize_serialize (
			    c, &(src+src->معلومات_Sets[i]), inner_maps[i], region_map)))
	return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  محمي:
  HBUINT16				format; // 1
  Offset32_منزل<SparseVarRegionList>	regions;
  Array16_د_Offset32_منزل<Multi_Var_معلومات>	معلومات_Sets;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (8, معلومات_Sets);
};

کينډۍ <ډول_نوم Map_شمېر_T>
بنية Delta_سېټ_شاخص_Map_Format01
{
  ملګری بنية Delta_سېټ_شاخص_Map;

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کړه_saturate (map_شمېر_2, get_پلنوالی (), min_اندازه); }

  خاص:
  Delta_سېټ_شاخص_Map_Format01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  کينډۍ <ډول_نوم T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    بینښه صحيح پلنوالی = plan.get_پلنوالی ();
    بینښه صحيح inner_بټ_شمېر = plan.get_inner_بټ_شمېر ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (هذا);
    إذا (unlikely (output_map.اوږدتیا && ((((inner_بټ_شمېر-1)&~0xF)!=0) || (((پلنوالی-1)&~0x3)!=0))))
      return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    ثبت_Format = ((پلنوالی-1)<<4)|(inner_بټ_شمېر-1);
    map_شمېر_2 = output_map.اوږدتیا;
    HBUINT8 *p = c->allocate_اندازه<HBUINT8> (پلنوالی * output_map.اوږدتیا);
    إذا (unlikely (!p)) return_trace (خطأ);
    لكل (بینښه صحيح i = 0; i < output_map.اوږدتیا; i++)
    {
      بینښه صحيح v = output_map.arrayZ[i];
      إذا (v)
      {
	بینښه صحيح outer = v >> 16;
	بینښه صحيح inner = v & 0xFFFF;
	بینښه صحيح u = (outer << inner_بټ_شمېر) | inner;
	لكل (بینښه صحيح w = پلنوالی; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += پلنوالی;
    }
    return_trace (ریښتیا);
  }

  HB_ALWAYS_INLINE
  uint32_t map (بینښه صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    إذا (!map_شمېر_2)
      أرجع v;
    أرجع _map (v);
  }

  HB_HOT
  uint32_t _map (بینښه صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    إذا (v >= map_شمېر_2)
      v = map_شمېر_2 - 1;

    بینښه صحيح u = 0;
    { /* Fetch it. */
      بینښه صحيح w = get_پلنوالی ();
      ثابت HBUINT8 *p = map_معلومات_Z.arrayZ + w * v;
      لكل (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      بینښه صحيح n = get_inner_بټ_شمېر ();
      بینښه صحيح outer = u >> n;
      بینښه صحيح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    أرجع u;
  }

  بینښه get_map_شمېر () ثابت       { أرجع map_شمېر_2; }
  بینښه get_پلنوالی () ثابت           { أرجع ((ثبت_Format >> 4) & 3) + 1; }
  بینښه get_inner_بټ_شمېر () ثابت { أرجع (ثبت_Format & 0xF) + 1; }


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  c->check_range (map_معلومات_Z.arrayZ,
                                  map_شمېر_2,
                                  get_پلنوالی ()));
  }

  محمي:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       ثبت_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_شمېر_T     map_شمېر_2;       /* The number of mapping entries. */
  Unsized_Array_د<HBUINT8>
                map_معلومات_Z;       /* The delta-set index mapping data. */

  عام:
  DEFINE_اندازه_ARRAY (2+Map_شمېر_T::static_اندازه, map_معلومات_Z);
};

بنية Delta_سېټ_شاخص_Map
{
  کينډۍ <ډول_نوم T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (هذا);
    بینښه اوږدتیا = plan.get_output_map ().اوږدتیا;
    u.format.v = اوږدتیا <= 0xFFFF ? 0 : 1;
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    افتراضي:return_trace (خطأ);
    }
  }

  uint32_t map (بینښه v) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع (u.format0.map (v));
    حالة 1: hb_barrier (); أرجع (u.format1.map (v));
    افتراضي:أرجع v;
    }
  }

  بینښه get_map_شمېر () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_map_شمېر ();
    حالة 1: hb_barrier (); أرجع u.format1.get_map_شمېر ();
    افتراضي:أرجع 0;
    }
  }

  بینښه get_پلنوالی () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_پلنوالی ();
    حالة 1: hb_barrier (); أرجع u.format1.get_پلنوالی ();
    افتراضي:أرجع 0;
    }
  }

  بینښه get_inner_بټ_شمېر () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_inner_بټ_شمېر ();
    حالة 1: hb_barrier (); أرجع u.format1.get_inner_بټ_شمېر ();
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
    افتراضي:return_trace (ریښتیا);
    }
  }

  Delta_سېټ_شاخص_Map* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (reinterpret_cast<Delta_سېټ_شاخص_Map *> (u.format0.copy (c)));
    حالة 1: hb_barrier (); return_trace (reinterpret_cast<Delta_سېټ_شاخص_Map *> (u.format1.copy (c)));
    افتراضي:return_trace (تش_اشاره);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }             format;         /* Format identifier */
  Delta_سېټ_شاخص_Map_Format01<HBUINT16> format0;
  Delta_سېټ_شاخص_Map_Format01<HBUINT32> format1;
  } u;
  عام:
  DEFINE_اندازه_UNION (1, format.v);
};


بنية توکی_Var_Store_Instancer
{
  توکی_Var_Store_Instancer (ثابت توکی_Variation_Store *varStore_,
			 ثابت Delta_سېټ_شاخص_Map *varIdxMap,
			 hb_array_t<ثابت صحيح> coords,
			 hb_scalar_cache_t *cache = تش_اشاره) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(توکی_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    إذا (!hb_any (coords))
      هذا->coords = {};
  }

  چلوونکی منطقي () ثابت { أرجع varStore && منطقي (coords); }

  عائم چلوونکی[] (uint32_t varIdx) ثابت
  { أرجع (*هذا) (varIdx); }

  عائم چلوونکی() (uint32_t varIdx, بینښه لنډ دننه_واټن = 0) ثابت
  { أرجع get_delta (varIdx, دننه_واټن); }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع get_delta<مزدوج> (varIdx); }

  کينډۍ <ډول_نوم Float = عائم>
  Float get_delta (uint32_t varIdx, بینښه لنډ دننه_واټن = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   إذا (!coords || varIdx == VarIdx::NO_VARIATION)
     أرجع Float (0);

    varIdx += دننه_واټن;
    إذا (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    أرجع varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت توکی_Variation_Store *varStore;
  ثابت Delta_سېټ_شاخص_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};

بنية Multi_توکی_Var_Store_Instancer
{
  Multi_توکی_Var_Store_Instancer (ثابت Multi_توکی_Variation_Store *varStore,
			      ثابت Delta_سېټ_شاخص_Map *varIdxMap,
			      hb_array_t<ثابت صحيح> coords,
			      hb_scalar_cache_t *cache = تش_اشاره) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(Multi_توکی_Variation_Store);
  }

  چلوونکی منطقي () ثابت { أرجع varStore; }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع (مزدوج) (*هذا)[varIdx]; }

  مزدوج چلوونکی[] (uint32_t varIdx) ثابت
  {
    مزدوج v = 0;
    (*هذا) (hb_array (&v, 1), varIdx);
    أرجع v;
  }

  فراغ چلوونکی() (hb_array_t<مزدوج> بهر, uint32_t varIdx, بینښه لنډ دننه_واټن = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    إذا (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += دننه_واټن;
      إذا (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, بهر, cache);
    }
    وإلا
      لكل (بینښه i = 0; i < بهر.اوږدتیا; i++)
        بهر.arrayZ[i] = 0.;
  }

  ثابت Multi_توکی_Variation_Store *varStore;
  ثابت Delta_سېټ_شاخص_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
تعداد Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_ریکارډ_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_ریکارډ_WITH_VAR = 3,
};

تعداد lookup_condition_subset_پایله_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

بنية Condition;

کينډۍ <ډول_نوم Instancer>
ساكن منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بینښه صحيح coord_len,
				Instancer *instancer);

بنية ConditionAxisRange
{
  ملګری بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    ثابت hb_map_t *شاخص_map = &c->plan->axes_شاخص_map;
    إذا (شاخص_map->is_empty ()) return_trace (ریښتیا);

    ثابت hb_map_t& axes_old_شاخص_نښان_map = c->plan->axes_old_شاخص_نښان_map;
    hb_codepoint_t *axis_نښان_2;
    إذا (!axes_old_شاخص_نښان_map.has (axis_شاخص_2, &axis_نښان_2) ||
        !شاخص_map->has (axis_شاخص_2))
      return_trace (خطأ);

    ثابت hb_hashmap_t<hb_نښان_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_حد{-1.0, 0.0, 1.0};
    Triple *normalized_حد;
    إذا (normalized_axes_location.has (*axis_نښان_2, &normalized_حد))
      axis_حد = *normalized_حد;

    ثابت hb_hashmap_t<hb_نښان_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    إذا (axes_triple_distances.has (*axis_نښان_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    عائم normalized_min = renormalize_ارزښت ((مزدوج) filter_Range_Min_ارزښت.منزل_float (), axis_حد, axis_triple_distances, خطأ);
    عائم normalized_max = renormalize_ارزښت ((مزدوج) filter_Range_Max_ارزښت.منزل_float (), axis_حد, axis_triple_distances, خطأ);
    بهر->filter_Range_Min_ارزښت.سېټ_float (normalized_min);
    بهر->filter_Range_Max_ارزښت.سېټ_float (normalized_max);

    return_trace (c->serializer->check_ټاکل (بهر->axis_شاخص_2, شاخص_map->get (axis_شاخص_2),
                                               HB_SERIALIZE_ERROR_INT_ډکېدنه));
  }

  خاص:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*بهر*/) ثابت
  {
    //invalid axis index, drop the entire record
    إذا (!c->axes_شاخص_نښان_map->has (axis_شاخص_2))
      أرجع DROP_ریکارډ_WITH_VAR;

    hb_نښان_t axis_نښان_2 = c->axes_شاخص_نښان_map->get (axis_شاخص_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_حد;
    منطقي axis_سېټ_by_user = خطأ;
    إذا (c->axes_location->has (axis_نښان_2, &axis_حد))
    {
      axis_range = *axis_حد;
      axis_سېټ_by_user = ریښتیا;
    }

    عائم axis_min_val = axis_range.minimum;
    عائم axis_default_val = axis_range.منځ;
    عائم axis_max_val = axis_range.اعظمي;

    عائم filter_min_val = filter_Range_Min_ارزښت.منزل_float ();
    عائم filter_max_val = filter_Range_Max_ارزښت.منزل_float ();

    إذا (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = خطأ;

    //condition not met, drop the entire record
    إذا (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      أرجع DROP_ریکارډ_WITH_VAR;

    //condition met and axis pinned, drop the condition
    إذا (axis_سېټ_by_user && axis_range.is_ټکی ())
      أرجع DROP_COND_WITH_VAR;

    إذا (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_ارزښت.منزل_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_ارزښت.منزل_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->سېټ (axis_شاخص_2, val);
      أرجع KEEP_COND_WITH_VAR;
    }
    أرجع KEEP_ریکارډ_WITH_VAR;
  }

  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
		 Instancer *instancer HB_ناکارول_شوی) ثابت
  {
    صحيح coord = axis_شاخص_2 < coord_len ? coords[axis_شاخص_2] : 0;
    أرجع filter_Range_Min_ارزښت.منزل_int () <= coord && coord <= filter_Range_Max_ارزښت.منزل_int ();
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_شاخص_2;
  F2DOT14	filter_Range_Min_ارزښت;
  F2DOT14	filter_Range_Max_ارزښت;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Condition_ارزښت
{
  ملګری بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    إذا (varIdx == VarIdx::NO_VARIATION)
      return_trace (ریښتیا);

    hb_pair_t<بینښه, صحيح> *condition_ارزښت;
    إذا (c->plan->layout_condition_idx_ارزښت_map.has ((uintptr_t) هذا, &condition_ارزښت))
    {
      بهر->default_ارزښت_2 = condition_ارزښت->second;
      بهر->varIdx = condition_ارزښت->لومړی;
      return_trace (ریښتیا);
    }

    hb_pair_t<بینښه, صحيح> *new_varidx_delta;
    إذا (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (خطأ);

    إذا (unlikely (!c->serializer->check_ټاکل (
		بهر->default_ارزښت_2,
		default_ارزښت_2 + hb_دوهم (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_ډکېدنه)))
      return_trace (خطأ);
    بهر->varIdx = hb_لومړی (*new_varidx_delta);
    return_trace (ریښتیا);
  }

  خاص:
  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    مزدوج value = default_ارزښت_2;
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
  HBINT16	default_ارزښت_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية ConditionAnd
{
  ملګری بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
      return_trace (خطأ);
    بهر->format = format;

    لكل (ثابت خپلکاری &دننه_واټن : conditions)
    {
      إذا (دننه_واټن.is_null ())
      {
	خپلکاری *بهر_دننه_واټن = بهر->conditions.serialize_append (c->serializer);
	إذا (unlikely (!بهر_دننه_واټن)) return_trace (خطأ);
	*بهر_دننه_واټن = 0;
      }
      وإلا إذا (unlikely (!subset_دننه_واټن_array (c, بهر->conditions, هذا) (دننه_واټن)))
	return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  خاص:
  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بینښه صحيح شمېر = conditions.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	أرجع خطأ;
    أرجع ریښتیا;
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8_د_Offset24_منزل<بنية Condition>	conditions;
  عام:
  DEFINE_اندازه_ARRAY (3, conditions);
};

بنية ConditionOr
{
  ملګری بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
      return_trace (خطأ);
    بهر->format = format;

    لكل (ثابت خپلکاری &دننه_واټن : conditions)
    {
      إذا (دننه_واټن.is_null ())
      {
	خپلکاری *بهر_دننه_واټن = بهر->conditions.serialize_append (c->serializer);
	إذا (unlikely (!بهر_دننه_واټن)) return_trace (خطأ);
	*بهر_دننه_واټن = 0;
      }
      وإلا إذا (unlikely (!subset_دننه_واټن_array (c, بهر->conditions, هذا) (دننه_واټن)))
	return_trace (خطأ);
    }

    return_trace (ریښتیا);
  }

  خاص:
  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بینښه صحيح شمېر = conditions.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	أرجع ریښتیا;
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
  Array8_د_Offset24_منزل<بنية Condition>	conditions;
  عام:
  DEFINE_اندازه_ARRAY (3, conditions);
};

بنية ConditionNegate
{
  ملګری بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);
    إذا (condition.is_null ())
    {
      بهر->condition = 0;
      return_trace (ریښتیا);
    }
    return_trace (بهر->condition.serialize_subset (c, condition, هذا));
  }

  خاص:
  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
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
  Offset24_منزل<بنية Condition>	condition;
  عام:
  DEFINE_اندازه_STATIC (5);
};

بنية Condition
{
  ملګری بنية Condition_سېټ;

  ساكن منطقي serialize_constant (hb_serialize_context_t *c, منطقي value)
  {
    خپلکاری *بهر = c->پیل_embed<Condition_ارزښت> ();
    إذا (unlikely (!بهر || !c->extend_min (بهر))) أرجع خطأ;
    بهر->format = 2;
    بهر->default_ارزښت_2 = value ? 1 : 0;
    بهر->varIdx = VarIdx::NO_VARIATION;
    أرجع ریښتیا;
  }

  lookup_condition_subset_پایله_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      بینښه ژوروالی = HB_MAX_NESTING_LEVEL,
				      منطقي materialize_true = خطأ) ثابت;

  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بدل (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    حالة 0: أرجع ریښتیا;
    حالة 1: hb_barrier (); أرجع u.format1.evaluate (coords, coord_len, instancer);
    حالة 2: hb_barrier (); أرجع u.format2.evaluate (coords, coord_len, instancer);
    حالة 3: hb_barrier (); أرجع u.format3.evaluate (coords, coord_len, instancer);
    حالة 4: hb_barrier (); أرجع u.format4.evaluate (coords, coord_len, instancer);
    حالة 5: hb_barrier (); أرجع u.format5.evaluate (coords, coord_len, instancer);
    افتراضي:أرجع خطأ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*بهر*/,
                                             بینښه ژوروالی = HB_MAX_NESTING_LEVEL) ثابت
  {
    إذا (unlikely (!ژوروالی))
    {
      c->apply = خطأ;
      أرجع DROP_ریکارډ_WITH_VAR;
    }

    بدل (u.format.v) {
    حالة 0: أرجع KEEP_ریکارډ_WITH_VAR;
    حالة 1: hb_barrier (); أرجع u.format1.keep_with_variations (c, condition_map);
    حالة 2: حالة 3: حالة 4: حالة 5: اكسر;
    افتراضي: c->apply = خطأ; أرجع DROP_ریکارډ_WITH_VAR;
    }

    hb_barrier ();
    منطقي applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.اوږدتیا, c->instancer);
    c->apply &= applies;
    إذا (c->all_axes_pinned)
      أرجع applies ? DROP_COND_WITH_VAR : DROP_ریکارډ_WITH_VAR;

    إذا (u.format.v == 2)
      أرجع u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_سېټ (u.format2.varIdx) ?
             (applies ? KEEP_ریکارډ_WITH_VAR : DROP_ریکارډ_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    خپلکاری classify_ماشوم = [c, condition_map, ژوروالی] (ثابت Condition &ماشوم)
    {
      خپلکاری ماشوم_context = *c;
      ماشوم_context.apply = ریښتیا;
      أرجع ماشوم.keep_with_variations (&ماشوم_context, condition_map, ژوروالی - 1);
    };

    إذا (u.format.v == 5)
    {
      خپلکاری پایله = classify_ماشوم (&u.format5 + u.format5.condition);
      أرجع پایله == DROP_ریکارډ_WITH_VAR ? KEEP_ریکارډ_WITH_VAR :
             پایله == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_ریکارډ_WITH_VAR;
    }

    منطقي is_and = u.format.v == 3;
    منطقي variable = خطأ;
    ثابت خپلکاری &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت فراغ *بنسټ = is_and ? (ثابت فراغ *) &u.format3 : (ثابت فراغ *) &u.format4;
    لكل (ثابت خپلکاری &دننه_واټن : conditions)
    {
      خپلکاری پایله = classify_ماشوم (بنسټ + دننه_واټن);
      إذا (is_and && پایله == DROP_ریکارډ_WITH_VAR)
        أرجع DROP_ریکارډ_WITH_VAR;
      إذا (!is_and && (پایله == DROP_COND_WITH_VAR || پایله == KEEP_ریکارډ_WITH_VAR))
        أرجع KEEP_ریکارډ_WITH_VAR;
      variable |= پایله == KEEP_COND_WITH_VAR;
    }
    أرجع variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_ریکارډ_WITH_VAR : DROP_ریکارډ_WITH_VAR;
  }

  کينډۍ <ډول_نوم context_t, ډول_نوم ...Ts>
  ډول_نوم context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_ارزښت ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_ارزښت ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_پیل_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_ارزښت ());

    return_trace (c->پای_ټکی_recursion (هذا->dispatch (c)));
  }

  منطقي collect_var_indices (hb_سېټ_t *var_indices,
			    بینښه ژوروالی = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<بینښه, صحيح>> *condition_ارزښتونه = تش_اشاره) ثابت;

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = تش_اشاره);

  خاص:
  lookup_condition_subset_پایله_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      بینښه ژوروالی) ثابت;

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_ارزښت	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

کينډۍ <ډول_نوم Instancer>
منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بینښه صحيح coord_len,
				Instancer *instancer)
{
  أرجع condition.evaluate (coords, coord_len, instancer);
}

بنية ConditionList
{
  ثابت Condition& چلوونکی[] (بینښه i) ثابت
  { أرجع هذا+conditions[i]; }

  بینښه get_شمېر () ثابت { أرجع conditions.len; }

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
    لكل (بینښه i = 0; i < conditions.len; i++)
    {
      بینښه old_شاخص = condition_map.backward (i);
      إذا (unlikely (old_شاخص >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_شاخص]), varidx_map, axis_map)))
	return_trace (خطأ);
    }
    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  Array32_د_Offset32_منزل<Condition> conditions;
  عام:
  DEFINE_اندازه_ARRAY (4, conditions);
};

مضمن منطقي
Condition::collect_var_indices (hb_سېټ_t *var_indices, بینښه ژوروالی,
                               hb_hashmap_t<uintptr_t, hb_pair_t<بینښه, صحيح>> *condition_ارزښتونه) ثابت
{
  إذا (unlikely (!ژوروالی)) أرجع خطأ;
  بدل (u.format.v)
  {
    حالة 0:
    حالة 1:
      أرجع ریښتیا;
    حالة 2:
      إذا (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->جمع_کړه (u.format2.varIdx);
        إذا (condition_ارزښتونه &&
            !condition_ارزښتونه->سېټ ((uintptr_t) &u.format2,
                                    hb_pair (بینښه (u.format2.varIdx), صحيح (u.format2.default_ارزښت_2))))
          أرجع خطأ;
      }
      أرجع !var_indices->دننه_error ();
    حالة 3:
      لكل (ثابت خپلکاری &دننه_واټن : u.format3.conditions)
	إذا (unlikely (!(&u.format3+دننه_واټن).collect_var_indices (var_indices,
							     ژوروالی - 1, condition_ارزښتونه)))
	  أرجع خطأ;
      أرجع ریښتیا;
    حالة 4:
      لكل (ثابت خپلکاری &دننه_واټن : u.format4.conditions)
	إذا (unlikely (!(&u.format4+دننه_واټن).collect_var_indices (var_indices,
							     ژوروالی - 1, condition_ارزښتونه)))
	  أرجع خطأ;
      أرجع ریښتیا;
    حالة 5:
      أرجع (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  ژوروالی - 1, condition_ارزښتونه);
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
      خپلکاری *بهر = c->پیل_embed<Condition_ارزښت> ();
      إذا (unlikely (!بهر || !c->extend_min (بهر))) return_trace (خطأ);
      بهر->format = 2;
      بهر->default_ارزښت_2 = 1;
      بهر->varIdx = VarIdx::NO_VARIATION;
      return_trace (ریښتیا);
    }
    حالة 1:
    {
      خپلکاری *بهر = c->embed (&src->u.format1);
      إذا (unlikely (!بهر)) return_trace (خطأ);
      إذا (axis_map)
      {
	إذا (unlikely (!axis_map->has (بهر->axis_شاخص_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (خطأ);
	}
	إذا (unlikely (!c->check_ټاکل (بهر->axis_شاخص_2, axis_map->get (بهر->axis_شاخص_2),
				      HB_SERIALIZE_ERROR_INT_ډکېدنه)))
	  return_trace (خطأ);
      }
      return_trace (ریښتیا);
    }
    حالة 2:
    {
      خپلکاری *بهر = c->embed (&src->u.format2);
      إذا (unlikely (!بهر ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (خطأ);
      بهر->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (ریښتیا);
    }
    حالة 3:
    {
      خپلکاری *بهر = c->پیل_embed<ConditionAnd> ();
      إذا (unlikely (!c->extend_min (بهر))) return_trace (خطأ);
      بهر->format = 3;
      بهر->conditions.len = src->u.format3.conditions.len;
      إذا (unlikely (!c->extend (بهر->conditions))) return_trace (خطأ);
      لكل (بینښه i = 0; i < بهر->conditions.len; i++)
	إذا (unlikely (!بهر->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (ریښتیا);
    }
    حالة 4:
    {
      خپلکاری *بهر = c->پیل_embed<ConditionOr> ();
      إذا (unlikely (!c->extend_min (بهر))) return_trace (خطأ);
      بهر->format = 4;
      بهر->conditions.len = src->u.format4.conditions.len;
      إذا (unlikely (!c->extend (بهر->conditions))) return_trace (خطأ);
      لكل (بینښه i = 0; i < بهر->conditions.len; i++)
	إذا (unlikely (!بهر->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (ریښتیا);
    }
    حالة 5:
    {
      خپلکاری *بهر = c->embed (&src->u.format5);
      إذا (unlikely (!بهر ||
		    !بهر->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (خطأ);
      return_trace (ریښتیا);
    }
    افتراضي:
      return_trace (خطأ);
  }
}

مضمن lookup_condition_subset_پایله_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    بینښه ژوروالی,
				    منطقي materialize_true) ثابت
{
  خپلکاری snap = c->serializer->snapshot ();
  lookup_condition_subset_پایله_t پایله =
      subset_lookup_condition_impl (c, ژوروالی);
  إذا (پایله == LOOKUP_CONDITION_SUBSET_KEEP)
    أرجع پایله;

  c->serializer->revert (snap);
  إذا (پایله != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    أرجع پایله;

  أرجع serialize_constant (c->serializer, ریښتیا) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

مضمن lookup_condition_subset_پایله_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 بینښه ژوروالی) ثابت
{
  إذا (unlikely (!ژوروالی))
    أرجع LOOKUP_CONDITION_SUBSET_ERROR;

  خپلکاری subset_دننه_واټن = [c, ژوروالی] (Array8_د_Offset24_منزل<Condition> &بهر,
				   ثابت Condition &condition)
      -> lookup_condition_subset_پایله_t
  {
    خپلکاری snap = c->serializer->snapshot ();
    خپلکاری *دننه_واټن = بهر.serialize_append (c->serializer);
    إذا (unlikely (!دننه_واټن))
      أرجع LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_پایله_t پایله =
        condition.subset_lookup_condition (c, ژوروالی - 1);
    إذا (پایله == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->جمع_کړه_link (*دننه_واټن, c->serializer->راوباسه_pack ());
    وإلا
    {
      c->serializer->راوباسه_discard ();
      بهر.راوباسه ();
      c->serializer->revert (snap);
    }
    أرجع پایله;
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

      hb_codepoint_t *axis_نښان_2;
      إذا (!c->plan->axes_old_شاخص_نښان_map.has (src.axis_شاخص_2, &axis_نښان_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_حد {-1.f, 0.f, 1.f};
      Triple *normalized_حد;
      إذا (c->plan->axes_location.has (*axis_نښان_2, &normalized_حد))
        axis_حد = *normalized_حد;

      عائم filter_min = src.filter_Range_Min_ارزښت.منزل_float ();
      عائم filter_max = src.filter_Range_Max_ارزښت.منزل_float ();
      إذا (filter_min > filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد.is_ټکی ())
        أرجع filter_min <= axis_حد.منځ &&
	       axis_حد.منځ <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد.اعظمي < filter_min ||
	  filter_max < axis_حد.minimum)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (filter_min <= axis_حد.minimum &&
	  axis_حد.اعظمي <= filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      إذا (!c->plan->axes_شاخص_map.has (src.axis_شاخص_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      خپلکاری *بهر = c->serializer->embed (&src);
      إذا (unlikely (!بهر))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      إذا (c->plan->axes_triple_distances.has (*axis_نښان_2, &distances))
        axis_distances = *distances;

      بهر->filter_Range_Min_ارزښت.سېټ_float (
	  renormalize_ارزښت (filter_min, axis_حد, axis_distances, خطأ));
      بهر->filter_Range_Max_ارزښت.سېټ_float (
	  renormalize_ارزښت (filter_max, axis_حد, axis_distances, خطأ));
      إذا (unlikely (!c->serializer->check_ټاکل (
		بهر->axis_شاخص_2,
		c->plan->axes_شاخص_map.get (src.axis_شاخص_2),
		HB_SERIALIZE_ERROR_INT_ډکېدنه)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 2:
    {
      ثابت Condition_ارزښت &src = u.format2;
      خپلکاری *بهر = c->serializer->embed (&src);
      إذا (unlikely (!بهر))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      إذا (src.varIdx == VarIdx::NO_VARIATION)
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_ارزښت_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<بینښه, صحيح> *condition_ارزښت;
      إذا (c->plan->layout_condition_idx_ارزښت_map.has ((uintptr_t) &src, &condition_ارزښت))
      {
        بهر->default_ارزښت_2 = condition_ارزښت->second;
        بهر->varIdx = condition_ارزښت->لومړی;
        إذا (بهر->varIdx == VarIdx::NO_VARIATION)
          أرجع بهر->default_ارزښت_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        أرجع LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<بینښه, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_ارزښت_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      إذا (!c->plan->user_axes_location.is_empty () &&
          hb_لومړی (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        مزدوج value = (صحيح) src.default_ارزښت_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        أرجع value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحيح default_ارزښت = (صحيح) src.default_ارزښت_2 +
			  hb_دوهم (*new_varidx_delta);
      إذا (unlikely (!c->serializer->check_ټاکل (
		بهر->default_ارزښت_2,
		default_ارزښت,
		HB_SERIALIZE_ERROR_INT_ډکېدنه)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بهر->varIdx = hb_لومړی (*new_varidx_delta);

      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 3:
    {
      ثابت ConditionAnd &src = u.format3;
      خپلکاری *بهر = c->serializer->پیل_embed (&src);
      إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بهر->format = 3;

      لكل (ثابت خپلکاری &دننه_واټن : src.conditions)
      {
        lookup_condition_subset_پایله_t پایله =
            subset_دننه_واټن (بهر->conditions, &src + دننه_واټن);
        إذا (پایله == LOOKUP_CONDITION_SUBSET_ERROR ||
	    پایله == LOOKUP_CONDITION_SUBSET_FALSE)
          أرجع پایله;
      }
      أرجع بهر->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالة 4:
    {
      ثابت ConditionOr &src = u.format4;
      خپلکاری *بهر = c->serializer->پیل_embed (&src);
      إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بهر->format = 4;

      لكل (ثابت خپلکاری &دننه_واټن : src.conditions)
      {
        lookup_condition_subset_پایله_t پایله =
            subset_دننه_واټن (بهر->conditions, &src + دننه_واټن);
        إذا (پایله == LOOKUP_CONDITION_SUBSET_ERROR ||
	    پایله == LOOKUP_CONDITION_SUBSET_TRUE)
          أرجع پایله;
      }
      أرجع بهر->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالة 5:
    {
      ثابت ConditionNegate &src = u.format5;
      خپلکاری *بهر = c->serializer->embed (&src);
      إذا (unlikely (!بهر))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بهر->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_پایله_t پایله =
          (&src + src.condition).subset_lookup_condition (c, ژوروالی - 1);
      إذا (پایله == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->جمع_کړه_link (بهر->condition,
				 c->serializer->راوباسه_pack ());
        أرجع پایله;
      }

      c->serializer->راوباسه_discard ();
      إذا (پایله == LOOKUP_CONDITION_SUBSET_TRUE)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (پایله == LOOKUP_CONDITION_SUBSET_FALSE)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      أرجع پایله;
    }

    افتراضي:
      أرجع LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

بنية Condition_سېټ
{
  منطقي evaluate (ثابت صحيح *coords, بینښه صحيح coord_len,
		 توکی_Var_Store_Instancer *instancer) ثابت
  {
    بینښه صحيح شمېر = conditions.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
      إذا (!(هذا+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	أرجع خطأ;
    أرجع ریښتیا;
  }

  فراغ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    إذا (unlikely (!condition_map)) أرجع;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_سېټ_t *cond_سېټ = hb_سېټ_create ();
    إذا (unlikely (!cond_سېټ)) أرجع;
    hb::shared_ptr<hb_سېټ_t> s {cond_سېټ};

    c->apply = ریښتیا;
    منطقي should_keep = خطأ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقي can_deduplicate = ریښتیا;
    بینښه عدد_kept_cond = 0, cond_idx = 0;
    لكل (ثابت خپلکاری& دننه_واټن : conditions)
    {
      ثابت Condition &condition = هذا+دننه_واټن;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      إذا (ret == DROP_ریکارډ_WITH_VAR)
      {
        c->apply = خطأ;
        أرجع;
      }

      إذا (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = ریښتیا;
        cond_سېټ->جمع_کړه (cond_idx);
        عدد_kept_cond++;
      }

      إذا (ret == KEEP_ریکارډ_WITH_VAR)
        should_keep = ریښتیا;

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
      c->conditionset_map->سېټ (p, 1);
    c->ریکارډ_cond_idx_map->سېټ (c->cur_ریکارډ_idx, s);
  }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقي ورننباسه_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    إذا (ورننباسه_catch_all) return_trace (ریښتیا);

    hb_سېټ_t *retained_cond_سېټ = تش_اشاره;
    إذا (l->feature_ریکارډ_cond_idx_map != تش_اشاره)
      retained_cond_سېټ = l->feature_ریکارډ_cond_idx_map->get (l->cur_feature_var_ریکارډ_idx);

    بینښه صحيح شمېر = conditions.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (retained_cond_سېټ != تش_اشاره && !retained_cond_سېټ->has (i))
        استمر;
      ثابت Condition &condition = هذا+conditions[i];
      إذا (condition.u.format.v == 1)
      {
        subset_دننه_واټن_array (c, بهر->conditions, هذا) (conditions[i]);
        استمر;
      }

      خپلکاری *دننه_واټن = بهر->conditions.serialize_append (c->serializer);
      إذا (unlikely (!دننه_واټن)) return_trace (خطأ);
      c->serializer->push ();
      خپلکاری پایله = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, ریښتیا);
      إذا (پایله == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        پایله = Condition::serialize_constant (c->serializer, خطأ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      إذا (unlikely (پایله == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->راوباسه_discard ();
        c->serializer->check_success (خطأ);
        return_trace (خطأ);
      }
      c->serializer->جمع_کړه_link (*دننه_واټن, c->serializer->راوباسه_pack ());
    }

    return_trace (منطقي (بهر->conditions));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خپلکاری &دننه_واټن : conditions)
      إذا (unlikely (!(هذا+دننه_واټن).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_ارزښتونه)))
	c->layout_variation_indices->err ();
  }

  محمي:
  Array16_د_Offset32_منزل<Condition>	conditions;
  عام:
  DEFINE_اندازه_ARRAY (2, conditions);
};

بنية Feature_جدول_Substitution_ریکارډ
{
  ملګری بنية Feature_جدول_Substitution;

  فراغ collect_lookups (ثابت فراغ *بنسټ, hb_سېټ_t *lookup_indexes /*بهر*/) ثابت
  {
    أرجع (بنسټ+feature).جمع_کړه_lookup_indexes_منزل (lookup_indexes);
  }

  فراغ تړون_features (ثابت فراغ *بنسټ,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سېټ_t       *feature_indexes /*بهر*/) ثابت
  {
    إذا ((بنسټ+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جمع_کړه (feature_شاخص_2);
  }

  فراغ collect_feature_substitutes_with_variations (hb_hashmap_t<بینښه, ثابت Feature*> *feature_substitutes_map,
                                                    hb_سېټ_t& catch_all_ریکارډ_feature_idxes,
                                                    ثابت hb_سېټ_t *feature_indices,
                                                    ثابت فراغ *بنسټ) ثابت
  {
    إذا (feature_indices->has (feature_شاخص_2))
    {
      feature_substitutes_map->سېټ (feature_شاخص_2, &(بنسټ+feature));
      catch_all_ریکارډ_feature_idxes.جمع_کړه (feature_شاخص_2);
    }
  }

  منطقي serialize (hb_subset_layout_context_t *c,
                  بینښه feature_شاخص,
                  ثابت Feature *f, ثابت نښان *نښان_2)
  {
    TRACE_SERIALIZE (هذا);
    hb_serialize_context_t *s = c->subset_context->serializer;
    إذا (unlikely (!s->extend_min (هذا))) return_trace (خطأ);

    uint32_t *new_feature_idx;
    إذا (!c->feature_map_w_duplicates->has (feature_شاخص, &new_feature_idx))
      return_trace (خطأ);

    إذا (!s->check_ټاکل (feature_شاخص_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (خطأ);

    s->push ();
    منطقي ret = f->subset (c->subset_context, c, نښان_2);
    إذا (ret) s->جمع_کړه_link (feature, s->راوباسه_pack ());
    وإلا s->راوباسه_discard ();

    return_trace (ret);
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_شاخص;
    إذا (!c->feature_map_w_duplicates->has (feature_شاخص_2, &new_feature_شاخص))
      return_trace (خطأ);

    خپلکاری *بهر = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->feature_شاخص_2 = *new_feature_شاخص;
    hb_pair_t<ثابت فراغ*, ثابت فراغ*> *original;
    إذا (!c->feature_idx_نښان_map->has (feature_شاخص_2, &original))
      return_trace (خطأ);
    ثابت نښان *نښان_2 = reinterpret_cast<ثابت نښان*> (original->second);
    return_trace (بهر->feature.serialize_subset (c->subset_context, feature, بنسټ, c, نښان_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت ریکارډ_sanitize_تړون_t تړون = {feature_list->get_نښان (feature_شاخص_2), بنسټ};
    return_trace (feature.sanitize (c, بنسټ, &تړون));
  }

  محمي:
  HBUINT16		feature_شاخص_2;
  Offset32_منزل<Feature>	feature;
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Feature_جدول_Substitution
{
  فراغ collect_feature_indices (hb_سېټ_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارډ &ریکارډ : substitutions)
      feature_indices->جمع_کړه (ریکارډ.feature_شاخص_2);
  }

  ثابت Feature *پیدا_کړه_substitute (بینښه صحيح feature_شاخص) ثابت
  {
    بینښه صحيح شمېر = substitutions.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      ثابت Feature_جدول_Substitution_ریکارډ &ریکارډ = substitutions.arrayZ[i];
      إذا (ریکارډ.feature_شاخص_2 == feature_شاخص)
	أرجع &(هذا+ریکارډ.feature);
    }
    أرجع تش_اشاره;
  }

  فراغ collect_lookups (ثابت hb_سېټ_t *feature_indexes,
			hb_سېټ_t       *lookup_indexes /*بهر*/) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_جدول_Substitution_ریکارډ::feature_شاخص_2)
    | hb_apply ([هذا, lookup_indexes] (ثابت Feature_جدول_Substitution_ریکارډ& r)
		{ r.collect_lookups (هذا, lookup_indexes); })
    ;
  }

  کينډۍ <ډول_نوم نښان_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_شاخص_map,
                         نښان_Getter get_نښان,
                         hb_سېټ_t *nameids_منزل_retain) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارډ &ریکارډ : substitutions)
      إذا (feature_شاخص_map->has (ریکارډ.feature_شاخص_2))
        (هذا+ریکارډ.feature).collect_name_ids (get_نښان (ریکارډ.feature_شاخص_2), nameids_منزل_retain);
  }

  فراغ تړون_features (ثابت hb_map_t *lookup_indexes,
			 hb_سېټ_t       *feature_indexes /*بهر*/) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارډ& ریکارډ : substitutions)
      ریکارډ.تړون_features (هذا, lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت hb_map_t *feature_شاخص_map) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارډ& ریکارډ : substitutions)
    {
      إذا (feature_شاخص_map->has (ریکارډ.feature_شاخص_2)) أرجع ریښتیا;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارډ& ریکارډ : substitutions)
      ریکارډ.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_ریکارډ_feature_idxes,
                                                          c->feature_indices, هذا);
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقي ورننباسه_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->نسخه.major = نسخه.major;
    بهر->نسخه.minor = نسخه.minor;

    إذا (ورننباسه_catch_all)
    {
      لكل (بینښه feature_شاخص : *(l->catch_all_ریکارډ_feature_idxes))
      {
        hb_pair_t<ثابت فراغ*, ثابت فراغ*> *p;
        إذا (!l->feature_idx_نښان_map->has (feature_شاخص, &p))
          return_trace (خطأ);
        خپلکاری *o = بهر->substitutions.serialize_append (c->serializer);
        إذا (!o->serialize (l, feature_شاخص,
                           reinterpret_cast<ثابت Feature*> (p->لومړی),
                           reinterpret_cast<ثابت نښان*> (p->second)))
          return_trace (خطأ);
      }
      return_trace (ریښتیا);
    }

    + substitutions.iter ()
    | hb_apply (subset_ریکارډ_array (l, &(بهر->substitutions), هذا))
    ;

    return_trace (منطقي (بهر->substitutions));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  substitutions.sanitize (c, هذا, feature_list));
  }

  محمي:
  Fixed_نسخه<>	نسخه;	/* Version--0x00010000u */
  Array16_د<Feature_جدول_Substitution_ریکارډ>
			substitutions;
  عام:
  DEFINE_اندازه_ARRAY (6, substitutions);
};

بنية Feature_Variation_ریکارډ
{
  ملګری بنية FeatureVariations;

  فراغ collect_lookups (ثابت فراغ     *بنسټ,
			ثابت hb_سېټ_t *feature_indexes,
			hb_سېټ_t       *lookup_indexes /*بهر*/) ثابت
  {
    أرجع (بنسټ+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  کينډۍ <ډول_نوم نښان_Getter>
  فراغ collect_name_ids (ثابت فراغ *بنسټ,
                         ثابت hb_map_t *feature_شاخص_map,
                         نښان_Getter get_نښان,
                         hb_سېټ_t *nameids_منزل_retain) ثابت
  {
    (بنسټ+substitutions).collect_name_ids (feature_شاخص_map, get_نښان, nameids_منزل_retain);
  }

  فراغ تړون_features (ثابت فراغ     *بنسټ,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سېټ_t       *feature_indexes /*بهر*/) ثابت
  {
    (بنسټ+substitutions).تړون_features (lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت فراغ *بنسټ, ثابت hb_map_t *feature_شاخص_map) ثابت
  {
    أرجع (بنسټ+substitutions).intersects_features (feature_شاخص_map);
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت فراغ *بنسټ) ثابت
  {
    (بنسټ+conditions).keep_with_variations (c);
    إذا (c->apply && !c->variation_applied)
    {
      (بنسټ+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = ریښتیا; // set variations only once
    }
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *بنسټ,
               منطقي ورننباسه_catch_all = خطأ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    hb_سېټ_t *retained_conditions = c->feature_ریکارډ_cond_idx_map ?
        c->feature_ریکارډ_cond_idx_map->get (c->cur_feature_var_ریکارډ_idx).get () : تش_اشاره;
    إذا (!ورننباسه_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      بهر->conditions = 0;
    وإلا
      بهر->conditions.serialize_subset (c->subset_context, conditions, بنسټ, c, ورننباسه_catch_all);
    بهر->substitutions.serialize_subset (c->subset_context, substitutions, بنسټ, c, ورننباسه_catch_all);

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, بنسټ) &&
		  substitutions.sanitize (c, بنسټ, feature_list));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *بنسټ) ثابت
  { (بنسټ+conditions).collect_variation_indices (c); }

  محمي:
  Offset32_منزل<Condition_سېټ>
			conditions;
  Offset32_منزل<Feature_جدول_Substitution>
			substitutions;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Lookup_شاخص_List
{
  فراغ جمع_کړه_lookup_indexes_منزل (hb_سېټ_t *lookup_indexes) ثابت
  { lookupIndices.جمع_کړه_indexes_منزل (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookupIndices.intersects (lookup_indexes); }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
      return_trace (خطأ);

    خپلکاری it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_شاخص_map)
    | hb_map (l->lookup_شاخص_map)
    ;
    بهر->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقي (بهر->lookupIndices));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (lookupIndices.sanitize (c));
  }

  محمي:
  شاخص_Array	lookupIndices;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (2, lookupIndices);
};

بنية Lookup_Condition_ریکارډ
{
  بینښه get_condition_دننه_واټن (ثابت فراغ *بنسټ, ثابت فراغ *variations) ثابت
  { أرجع (ثابت حرف *) &(بنسټ+condition) - (ثابت حرف *) variations; }

  کينډۍ <ډول_نوم Instancer>
  منطقي evaluate (ثابت فراغ *بنسټ,
		 ثابت صحيح *coords,
		 بینښه صحيح coord_len,
		 Instancer *instancer) ثابت
  { أرجع (بنسټ+condition).evaluate (coords, coord_len, instancer); }

  منطقي evaluate (ثابت فراغ *بنسټ,
                 ثابت فراغ *variations,
                 hb_sorted_array_t<ثابت بینښه> conditions) ثابت
  {
    أرجع !condition || hb_bsearch (get_condition_دننه_واټن (بنسټ, variations),
                                     conditions.arrayZ, conditions.اوږدتیا,
                                     حجم (بینښه),
                                     _hb_cmp_operator<بینښه, بینښه>);
  }

  منطقي collect_condition (ثابت فراغ *بنسټ,
                          ثابت فراغ *variations,
                          ثابت صحيح *coords,
                          بینښه صحيح coord_len,
                          توکی_Var_Store_Instancer *instancer,
                          hb_سېټ_t *لیدل_شوی,
                          hb_ویکټور_t<بینښه> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    إذا (!condition) أرجع ریښتیا;

    بینښه دننه_واټن = get_condition_دننه_واټن (بنسټ, variations);
    إذا (لیدل_شوی->has (دننه_واټن)) أرجع ریښتیا;
    لیدل_شوی->جمع_کړه (دننه_واټن);
    إذا (unlikely (لیدل_شوی->دننه_error ())) أرجع خطأ;

    إذا (evaluate (بنسټ, coords, coord_len, instancer))
      conditions->push (دننه_واټن);
    أرجع !conditions->دننه_error ();
  }

  فراغ جمع_کړه_lookup_indexes_منزل (ثابت فراغ *بنسټ,
			      hb_سېټ_t *lookup_indexes) ثابت
  { (بنسټ+lookupIndices).جمع_کړه_lookup_indexes_منزل (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت فراغ *بنسټ,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع (بنسټ+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *بنسټ) ثابت
  {
    إذا (unlikely (!(بنسټ+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_ارزښتونه)))
      c->layout_variation_indices->err ();
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    بهر->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_پایله_t پایله =
        (بنسټ + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    ریښتیا);
    إذا (پایله != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->راوباسه_discard ();
      return_trace (خطأ);
    }
    c->serializer->جمع_کړه_link (بهر->condition, c->serializer->راوباسه_pack ());

    return_trace (بهر->lookupIndices.serialize_subset (c, lookupIndices, بنسټ, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (condition.sanitize (c, بنسټ) &&
		  lookupIndices.sanitize (c, بنسټ));
  }

  محمي:
  Offset32_منزل<Condition>		condition;
  Offset32_منزل<Lookup_شاخص_List>	lookupIndices;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Feature_Lookups_جدول
{
  منطقي adds_default_lookups () ثابت { أرجع flags & 0x0001u; }

  منطقي collect_conditions (ثابت فراغ *variations,
                           ثابت صحيح *coords,
                           بینښه صحيح coord_len,
                           توکی_Var_Store_Instancer *instancer,
                           hb_سېټ_t *لیدل_شوی,
                           hb_ویکټور_t<بینښه> *conditions) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
      إذا (unlikely (!ریکارډ.collect_condition (هذا, variations, coords, coord_len,
                                               instancer, لیدل_شوی, conditions)))
	أرجع خطأ;
    أرجع ریښتیا;
  }

  فراغ collect_lookups (ثابت فراغ *variations,
                       hb_sorted_array_t<ثابت بینښه> conditions,
                       hb_سېټ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
      إذا (ریکارډ.evaluate (هذا, variations, conditions))
	ریکارډ.جمع_کړه_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  کينډۍ <ډول_نوم Instancer>
  فراغ collect_lookups (ثابت صحيح *coords,
			بینښه صحيح coord_len,
			Instancer *instancer,
			hb_سېټ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
      إذا (ریکارډ.evaluate (هذا, coords, coord_len, instancer))
	ریکارډ.جمع_کړه_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  فراغ collect_lookups (hb_سېټ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
      ریکارډ.جمع_کړه_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
      إذا (ریکارډ.intersects_lookup_indexes (هذا, lookup_indexes))
	أرجع ریښتیا;
    أرجع خطأ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
      ریکارډ.collect_variation_indices (c, هذا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر)))
      return_trace (خطأ);

    بهر->نسخه.major = نسخه.major;
    بهر->نسخه.minor = نسخه.minor;
    بهر->flags = flags;

    لكل (ثابت Lookup_Condition_ریکارډ &ریکارډ : ریکارډونه)
    {
      خپلکاری snap = c->serializer->snapshot ();
      إذا (ریکارډ.subset (c, l, هذا))
	بهر->ریکارډونه.len++;
      وإلا
	c->serializer->revert (snap);
    }

    return_trace (ریښتیا);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  ریکارډونه.sanitize (c, هذا));
  }

  محمي:
  Fixed_نسخه<>	نسخه;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_د<Lookup_Condition_ریکارډ>
		ریکارډونه;
  عام:
  DEFINE_اندازه_ARRAY (10, ریکارډونه);
};

بنية Lookup_Variation_ریکارډ
{
  ملګری بنية FeatureVariations;

  بینښه get_feature_شاخص () ثابت { أرجع feature_شاخص_2; }

  فراغ collect_lookups (ثابت فراغ *بنسټ,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت بینښه> conditions,
                       hb_سېټ_t *lookup_indexes) ثابت
  {
    ثابت Feature_Lookups_جدول &feature_lookups = بنسټ+featureLookups;
    إذا (feature_lookups.adds_default_lookups ())
      feature.جمع_کړه_lookup_indexes_منزل (lookup_indexes);
    feature_lookups.collect_lookups (بنسټ, conditions, lookup_indexes);
  }

  فراغ collect_lookups (ثابت فراغ *بنسټ,
			ثابت hb_سېټ_t *feature_indexes,
			hb_سېټ_t *lookup_indexes) ثابت
  {
    إذا (feature_indexes->has (feature_شاخص_2))
      (بنسټ+featureLookups).collect_lookups (lookup_indexes);
  }

  فراغ تړون_features (ثابت فراغ *بنسټ,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سېټ_t *feature_indexes) ثابت
  {
    إذا ((بنسټ+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جمع_کړه (feature_شاخص_2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *بنسټ) ثابت
  { (بنسټ+featureLookups).collect_variation_indices (c); }

  منطقي subset (hb_subset_layout_context_t *l, ثابت فراغ *بنسټ) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_شاخص;
    إذا (!l->feature_map_w_duplicates->has (feature_شاخص_2, &new_feature_شاخص))
      return_trace (خطأ);

    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر ||
		  !c->serializer->check_ټاکل (بهر->feature_شاخص_2,
						*new_feature_شاخص,
						HB_SERIALIZE_ERROR_INT_ډکېدنه)))
      return_trace (خطأ);

    return_trace (بهر->featureLookups.serialize_subset (c, featureLookups, بنسټ, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureLookups.sanitize (c, بنسټ));
  }

  محمي:
  HBUINT16			feature_شاخص_2;
  Offset32_منزل<Feature_Lookups_جدول>	featureLookups;
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية FeatureVariations
{
  ساكن constexpr بینښه NOT_وموندل_شاخص = 0xFFFFFFFFu;

  ثابت Array32_د<Lookup_Variation_ریکارډ> &get_lookup_variation_ریکارډونه () ثابت
  {
    إذا (نسخه.منزل_int () < 0x00010001u)
      أرجع Null (Array32_د<Lookup_Variation_ریکارډ>);
    أرجع StructAfter<Array32_د<Lookup_Variation_ریکارډ>> (var_ریکارډونه);
  }

  بینښه ریکارډ_شمېر () ثابت
  {
    أرجع var_ریکارډونه.len;
  }

  منطقي پیدا_کړه_شاخص (ثابت صحيح *coords, بینښه صحيح coord_len,
		   بینښه صحيح *شاخص_2,
		   توکی_Var_Store_Instancer *instancer) ثابت
  {
    بینښه صحيح شمېر = var_ریکارډونه.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      ثابت Feature_Variation_ریکارډ &ریکارډ = var_ریکارډونه.arrayZ[i];
      إذا ((هذا+ریکارډ.conditions).evaluate (coords, coord_len, instancer))
      {
	*شاخص_2 = i;
	أرجع ریښتیا;
      }
    }
    *شاخص_2 = NOT_وموندل_شاخص;
    أرجع خطأ;
  }

  ثابت Feature *پیدا_کړه_substitute (بینښه صحيح variations_شاخص,
				  بینښه صحيح feature_شاخص) ثابت
  {
    ثابت Feature_Variation_ریکارډ &ریکارډ = var_ریکارډونه[variations_شاخص];
    أرجع (هذا+ریکارډ.substitutions).پیدا_کړه_substitute (feature_شاخص);
  }

  منطقي collect_lookup_conditions (ثابت صحيح *coords,
                                  بینښه صحيح coord_len,
                                  توکی_Var_Store_Instancer *instancer,
                                  hb_ویکټور_t<بینښه> *state) ثابت
  {
    hb_سېټ_t لیدل_شوی_tables, لیدل_شوی_conditions;
    لكل (ثابت Lookup_Variation_ریکارډ &ریکارډ : get_lookup_variation_ریکارډونه ())
    {
      بینښه دننه_واټن = ریکارډ.featureLookups;
      إذا (لیدل_شوی_tables.has (دننه_واټن)) استمر;
      لیدل_شوی_tables.جمع_کړه (دننه_واټن);
      إذا (unlikely (لیدل_شوی_tables.دننه_error ())) أرجع خطأ;

      ثابت Feature_Lookups_جدول &feature_lookups = هذا+ریکارډ.featureLookups;
      إذا (unlikely (!feature_lookups.collect_conditions (هذا, coords, coord_len,
                                                         instancer, &لیدل_شوی_conditions,
                                                         state)))
	أرجع خطأ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<بینښه, بینښه>);
    أرجع ریښتیا;
  }

  منطقي get_lookup_variations (بینښه feature_شاخص,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت بینښه> conditions,
                              hb_سېټ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Variation_ریکارډ &ریکارډ : get_lookup_variation_ریکارډونه ())
    {
      إذا (ریکارډ.feature_شاخص_2 != feature_شاخص) استمر;

      ریکارډ.collect_lookups (هذا, feature, conditions, lookup_indexes);
      أرجع ریښتیا;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    بینښه صحيح شمېر = var_ریکارډونه.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      c->cur_ریکارډ_idx = i;
      var_ریکارډونه[i].collect_feature_substitutes_with_variations (c, هذا);
      إذا (c->universal)
        اكسر;
    }
    إذا (c->universal || c->ریکارډ_cond_idx_map->is_empty ())
      c->catch_all_ریکارډ_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  فراغ collect_lookups (ثابت hb_سېټ_t *feature_indexes,
			ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map,
			hb_سېټ_t       *lookup_indexes /*بهر*/) ثابت
  {
    بینښه شمېر = var_ریکارډونه.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (feature_ریکارډ_cond_idx_map &&
          !feature_ریکارډ_cond_idx_map->has (i))
        استمر;
      var_ریکارډونه[i].collect_lookups (هذا, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  کينډۍ <ډول_نوم نښان_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_شاخص_map,
                         ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map,
                         نښان_Getter get_نښان,
                         hb_سېټ_t *nameids_منزل_retain) ثابت
  {
    لكل (بینښه i = 0; i < var_ریکارډونه.len; i++)
    {
      إذا (feature_ریکارډ_cond_idx_map && !feature_ریکارډ_cond_idx_map->has (i))
        استمر;
      var_ریکارډونه[i].collect_name_ids (هذا, feature_شاخص_map, get_نښان, nameids_منزل_retain);
    }
  }

  فراغ collect_lookup_variation_lookups (ثابت hb_سېټ_t *feature_indexes,
					 hb_سېټ_t *lookup_indexes /*بهر*/) ثابت
  {
    لكل (ثابت Lookup_Variation_ریکارډ &ریکارډ : get_lookup_variation_ریکارډونه ())
      ریکارډ.collect_lookups (هذا, feature_indexes, lookup_indexes);
  }

  فراغ تړون_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<بینښه, hb::shared_ptr<hb_سېټ_t>> *feature_ریکارډ_cond_idx_map,
			 hb_سېټ_t       *feature_indexes /*بهر*/) ثابت
  {
    بینښه صحيح شمېر = var_ریکارډونه.len;
    لكل (بینښه صحيح i = 0; i < شمېر; i++)
    {
      إذا (feature_ریکارډ_cond_idx_map != تش_اشاره &&
          !feature_ریکارډ_cond_idx_map->has (i))
        استمر;
      var_ریکارډونه[i].تړون_features (هذا, lookup_indexes, feature_indexes);
    }

    لكل (ثابت Lookup_Variation_ریکارډ &ریکارډ : get_lookup_variation_ریکارډونه ())
      ریکارډ.تړون_features (هذا, lookup_indexes, feature_indexes);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Feature_Variation_ریکارډ &ریکارډ : var_ریکارډونه)
      ریکارډ.collect_variation_indices (c, هذا);
    لكل (ثابت Lookup_Variation_ریکارډ &ریکارډ : get_lookup_variation_ریکارډونه ())
      ریکارډ.collect_variation_indices (c, هذا);
  }

  فراغ collect_feature_indices (hb_سېټ_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_Variation_ریکارډ &ریکارډ : var_ریکارډونه)
      (هذا+ریکارډ.substitutions).collect_feature_indices (feature_indices);
  }

  منطقي has_lookup_variations (بینښه feature_شاخص) ثابت
  {
    لكل (ثابت Lookup_Variation_ریکارډ &ریکارډ : get_lookup_variation_ریکارډونه ())
    {
      إذا (ریکارډ.feature_شاخص_2 == feature_شاخص)
	أرجع ریښتیا;
      إذا (ریکارډ.feature_شاخص_2 > feature_شاخص)
	اكسر;
    }
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!بهر || !c->serializer->extend_min (بهر))) return_trace (خطأ);

    بهر->نسخه.major = نسخه.major;
    بهر->نسخه.minor = نسخه.minor;

    صحيح keep_پورته_منزل = -1;
    لكل (صحيح i = var_ریکارډونه.len - 1; i >= 0; i--) {
      إذا (var_ریکارډونه[i].intersects_features (هذا, l->feature_map_w_duplicates)) {
        keep_پورته_منزل = i;
        اكسر;
      }
    }

    بینښه شمېر = (بینښه) (keep_پورته_منزل + 1);
    لكل (بینښه i = 0; i < شمېر; i++)
    {
      إذا (l->feature_ریکارډ_cond_idx_map != تش_اشاره &&
          !l->feature_ریکارډ_cond_idx_map->has (i))
        استمر;

      l->cur_feature_var_ریکارډ_idx = i;
      subset_ریکارډ_array (l, &(بهر->var_ریکارډونه), هذا) (var_ریکارډونه[i]);
    }

    إذا (بهر->var_ریکارډونه.len && !l->catch_all_ریکارډ_feature_idxes->is_empty ())
    {
      منطقي ورننباسه_catch_all_ریکارډ = ریښتیا;
      subset_ریکارډ_array (l, &(بهر->var_ریکارډونه), هذا, ورننباسه_catch_all_ریکارډ) (var_ریکارډونه[0]);
    }

    Array32_د<Lookup_Variation_ریکارډ> *بهر_lookup_ریکارډونه = تش_اشاره;
    إذا (نسخه.minor >= 1)
    {
      بهر_lookup_ریکارډونه = &StructAfter<Array32_د<Lookup_Variation_ریکارډ>> (بهر->var_ریکارډونه);
      إذا (unlikely (!c->serializer->extend_min (بهر_lookup_ریکارډونه)))
	return_trace (خطأ);

      + get_lookup_variation_ریکارډونه ().iter ()
      | hb_apply (subset_ریکارډ_array (l, بهر_lookup_ریکارډونه, هذا))
      ;
    }

    return_trace (منطقي (بهر->var_ریکارډونه) ||
		  (بهر_lookup_ریکارډونه && منطقي (*بهر_lookup_ریکارډونه)));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  var_ریکارډونه.sanitize (c, هذا, feature_list) &&
		  (نسخه.minor < 1 ||
		   get_lookup_variation_ریکارډونه ().sanitize (c, هذا)));
  }

  محمي:
  Fixed_نسخه<>	نسخه;	/* Version--0x00010000u */
  Array32_د<Feature_Variation_ریکارډ>
			var_ریکارډونه;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (8, var_ریکارډونه);
};


/*
 * Device Tables
 */

بنية HintingDevice
{
  ملګری بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->y_ppem, font->y_scale); }

  عام:

  size_t get_اندازه () ثابت
  {
    بینښه صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3 || پیل_اندازه > پای_ټکی_اندازه)) أرجع 3 * HBUINT16::static_اندازه;
    أرجع HBUINT16::static_اندازه * (4 + ((پای_ټکی_اندازه - پیل_اندازه) >> (4 - f)));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && c->check_range (هذا, هذا->get_اندازه ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed<HintingDevice> (هذا));
  }

  خاص:

  صحيح get_delta (بینښه صحيح ppem, صحيح scale) ثابت
  {
    إذا (!ppem) أرجع 0;

    صحيح pixels = get_delta_pixels (ppem);

    إذا (!pixels) أرجع 0;

    أرجع (صحيح) (pixels * (int64_t) scale / ppem);
  }
  صحيح get_delta_pixels (بینښه صحيح ppem_اندازه) ثابت
  {
    بینښه صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3))
      أرجع 0;

    إذا (ppem_اندازه < پیل_اندازه || ppem_اندازه > پای_ټکی_اندازه)
      أرجع 0;

    بینښه صحيح s = ppem_اندازه - پیل_اندازه;

    بینښه صحيح byte = delta_ارزښت_Z[s >> (4 - f)];
    بینښه صحيح بټونه = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    بینښه صحيح ماسک = (0xFFFFu >> (16 - (1 << f)));

    صحيح delta = بټونه & ماسک;

    إذا ((بینښه صحيح) delta >= ((ماسک + 1) >> 1))
      delta -= ماسک + 1;

    أرجع delta;
  }

  محمي:
  HBUINT16	پیل_اندازه;		/* Smallest size to correct--in ppem */
  HBUINT16	پای_ټکی_اندازه;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_د<HBUINT16>
		delta_ارزښت_Z;		/* Array of compressed data */
  عام:
  DEFINE_اندازه_ARRAY (6, delta_ارزښت_Z);
};

بنية VariationDevice
{
  ملګری بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت توکی_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت توکی_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  عائم get_x_delta_f (hb_font_t *font,
		       ثابت توکی_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت توکی_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!layout_variation_idx_delta_map) return_trace (تش_اشاره);

    hb_pair_t<بینښه, صحيح> *v;
    إذا (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (تش_اشاره);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    إذا (hb_لومړی (*v) == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
      return_trace (تش_اشاره);

    c->پیل_zerocopy (هذا->static_اندازه);
    خپلکاری *بهر = c->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (تش_اشاره);

    إذا (!c->check_ټاکل (بهر->varIdx, hb_لومړی (*v), HB_SERIALIZE_ERROR_INT_ډکېدنه))
      return_trace (تش_اشاره);
    return_trace (بهر);
  }

  فراغ collect_variation_شاخص (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->جمع_کړه (varIdx); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:

  عائم get_delta (hb_font_t *font,
		   ثابت توکی_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    أرجع store.get_delta (varIdx, font->coords, font->عدد_coords, store_cache);
  }

  محمي:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Device_سرلیک
{
  محمي:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  عام:
  HBUINT16		format;		/* Format identifier */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  عائم get_x_delta_f (hb_font_t *font,
		       ثابت توکی_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_x_delta_f (font, store, store_cache);
    }
#د_که_پای
    أرجع get_x_delta (font, store, store_cache);
  }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت توکی_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
#که_نه_تعریف_شوی HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_y_delta_f (font, store, store_cache);
    }
#د_که_پای
    أرجع get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت توکی_Variation_Store &store=Null (توکی_Variation_Store),
			     hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    بدل (u.b.format)
    {
#که_نه_تعریف_شوی HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      أرجع u.hinting.get_x_delta (font);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_x_delta (font, store, store_cache);
#د_که_پای
    افتراضي:
      أرجع 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت توکی_Variation_Store &store=Null (توکی_Variation_Store),
			     hb_scalar_cache_t *store_cache = تش_اشاره) ثابت
  {
    بدل (u.b.format)
    {
    حالة 1: حالة 2: حالة 3:
#که_نه_تعریف_شوی HB_NO_HINTING
      hb_barrier ();
      أرجع u.hinting.get_y_delta (font);
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_y_delta (font, store, store_cache);
#د_که_پای
    افتراضي:
      أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.b.format.sanitize (c)) return_trace (خطأ);
    بدل (u.b.format) {
#که_نه_تعریف_شوی HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#د_که_پای
    افتراضي:
      return_trace (ریښتیا);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map=تش_اشاره) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.b.format) {
#که_نه_تعریف_شوی HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#د_که_پای
    افتراضي:
      return_trace (تش_اشاره);
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.b.format) {
#که_نه_تعریف_شوی HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      أرجع;
#د_که_پای
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      u.variation.collect_variation_شاخص (c);
      أرجع;
#د_که_پای
    افتراضي:
      أرجع;
    }
  }

  بینښه get_variation_شاخص () ثابت
  {
    بدل (u.b.format) {
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.varIdx;
#د_که_پای
    افتراضي:
      أرجع HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    }
  }

  منطقي is_variation_device () ثابت
  {
    بدل (u.b.format) {
#که_نه_تعریف_شوی HB_NO_VAR
    حالة 0x8000:
      أرجع ریښتیا;
#د_که_پای
    افتراضي:
      أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  Device_سرلیک		b;
  HintingDevice		hinting;
#که_نه_تعریف_شوی HB_NO_VAR
  VariationDevice	variation;
#د_که_پای
  } u;
  عام:
  DEFINE_اندازه_UNION (6, b);
};


} /* namespace OT */


#د_که_پای /* HB_OT_LAYOUT_COMMON_HH */

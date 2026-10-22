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

#اگر_تعریف_نشده HB_OT_LAYOUT_COMMON_HH
#تعریف_کن HB_OT_LAYOUT_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-set.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-cache.hh"
#ضمن "hb-ot-dual.hh"

استفاده hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (حجم (hb_ot_layout_mapping_cache_t) == 512, "");

استفاده hb_ot_layout_دودویی_cache_t = hb_cache_t<14, 1, 8>;
static_assert (حجم (hb_ot_layout_دودویی_cache_t) == 256, "");

#ضمن "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
استفاده OT::Layout::Common::Coverage;
استفاده OT::Layout::Common::Range_رکورد;
استفاده OT::Layout::SmallTypes;
استفاده OT::Layout::MediumTypes;


نطاق OT {

قالب<نام_نوع Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ساكن منطقي ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_مجموعه_t &klasses,
    منطقي use_class_zero,
    hb_sorted_وکتور_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

بنية قلم_Var_Store_Instancer;

بنية hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_شاخص_برچسب_map;
  ثابت hb_hashmap_t<hb_برچسب_t, Triple> *axes_location;
  hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *رکورد_cond_idx_map;
  hb_hashmap_t<بیعلامت, ثابت Feature*> *feature_substitutes_map;
  hb_مجموعه_t& catch_all_رکورد_feature_idxes;

  // not stored in subset_plan
  hb_مجموعه_t *feature_indices;
  منطقي apply;
  منطقي variation_applied;
  منطقي universal;
  بیعلامت cur_رکورد_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, بیعلامت> *conditionset_map;
  قلم_Var_Store_Instancer *instancer;
  منطقي all_axes_pinned;
};

بنية hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت فراغ         *جدول_2,
                              hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_مجموعه_t           *new_collected_feature_indexes_)
      :جدول (جدول_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_شمار (0),langsys_feature_شمار (0) {}

  منطقي بازدید_Script ()
  { أرجع script_شمار++ < HB_MAX_SCRIPTS; }

  منطقي بازدید_Langsys (بیعلامت feature_شمار_2)
  {
    langsys_feature_شمار += feature_شمار_2;
    أرجع langsys_feature_شمار < HB_MAX_LANGSYS_FEATURE_شمار;
  }

  عام:
  ثابت فراغ *جدول;
  hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_مجموعه_t           *new_feature_indexes;

  خاص:
  بیعلامت script_شمار;
  بیعلامت langsys_feature_شمار;
};

بنية hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_خالی_t, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { أرجع "SUBSET_LAYOUT"; }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  منطقي بازدید_Script ()
  {
    أرجع script_شمار++ < HB_MAX_SCRIPTS;
  }

  منطقي بازدید_Lang_Sys ()
  {
    أرجع langsys_شمار++ < HB_MAX_LANGSYS;
  }

  منطقي بازدید_Feature_شاخص (صحيح شمار)
  {
    feature_شاخص_شمار += شمار;
    أرجع feature_شاخص_شمار < HB_MAX_FEATURE_INDICES;
  }

  منطقي بازدید_Lookup_شاخص()
  {
    lookup_شاخص_شمار++;
    أرجع lookup_شاخص_شمار < HB_MAX_LOOKUP_بازدید_شمار;
  }

  hb_subset_context_t *subset_context;
  ثابت hb_برچسب_t جدول_برچسب_2;
  ثابت hb_map_t *lookup_شاخص_map;
  ثابت hb_hashmap_t<بیعلامت, hb::unique_ptr<hb_مجموعه_t>> *script_langsys_map;
  ثابت hb_map_t *feature_شاخص_map;
  ثابت hb_map_t *feature_map_w_duplicates;
  ثابت hb_hashmap_t<بیعلامت, ثابت Feature*> *feature_substitutes_map;
  ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *lookup_variations;
  hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map;
  ثابت hb_مجموعه_t *catch_all_رکورد_feature_idxes;
  ثابت hb_hashmap_t<بیعلامت, hb_pair_t<ثابت فراغ*, ثابت فراغ*>> *feature_idx_برچسب_map;

  بیعلامت cur_script_شاخص;
  بیعلامت cur_feature_شاخص;
  بیعلامت cur_feature_var_رکورد_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_برچسب_t برچسب_4) :
				subset_context (c_),
				جدول_برچسب_2 (برچسب_4),
				cur_script_شاخص (0xFFFFu),
				cur_feature_شاخص (شاخص::NOT_یافت_شاخص),
				cur_feature_var_رکورد_idx (0u),
				script_شمار (0),
				langsys_شمار (0),
				feature_شاخص_شمار (0),
				lookup_شاخص_شمار (0)
  {
    إذا (برچسب_4 == HB_OT_برچسب_GSUB)
    {
      lookup_شاخص_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_شاخص_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_رکورد_cond_idx_map = c_->plan->user_axes_location.is_empty () ? اشاره_تهی : &c_->plan->gsub_feature_رکورد_cond_idx_map;
      catch_all_رکورد_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_برچسب_map = &c_->plan->gsub_old_feature_idx_برچسب_map;
    }
    وإلا
    {
      lookup_شاخص_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_شاخص_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_رکورد_cond_idx_map = c_->plan->user_axes_location.is_empty () ? اشاره_تهی : &c_->plan->gpos_feature_رکورد_cond_idx_map;
      catch_all_رکورد_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_برچسب_map = &c_->plan->gpos_old_feature_idx_برچسب_map;
    }
  }

  خاص:
  بیعلامت script_شمار;
  بیعلامت langsys_شمار;
  بیعلامت feature_شاخص_شمار;
  بیعلامت lookup_شاخص_شمار;
};

بنية قلم_Variation_Store;
بنية hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  قالب <نام_نوع T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (هذا); أرجع hb_خالی_t (); }
  ساكن return_t default_return_مقدار () { أرجع hb_خالی_t (); }

  hb_مجموعه_t *layout_variation_indices;
  ثابت hb_مجموعه_t *glyph_مجموعه;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<بیعلامت, صحيح>> *condition_مقادیر = اشاره_تهی;

  hb_collect_variation_indices_context_t (hb_مجموعه_t *layout_variation_indices_,
					  ثابت hb_مجموعه_t *glyph_مجموعه_2,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_مجموعه (glyph_مجموعه_2),
					gpos_lookups (gpos_lookups_) {}
};

قالب<نام_نوع OutputArray>
بنية subset_جابه_جایی_array_t
{
  subset_جابه_جایی_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& بیرون_4,
			 ثابت فراغ *پایه_2) : subset_context (subset_context_),
					      بیرون (بیرون_4), پایه (پایه_2) {}

  قالب <نام_نوع T>
  منطقي عملگر () (T&& جابه_جایی)
  {
    خودکار snap = subset_context->serializer->snapshot ();
    خودکار *o = بیرون.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, جابه_جایی, پایه);
    إذا (!ret)
    {
      بیرون.برداشت ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &بیرون;
  ثابت فراغ *پایه;
};


قالب<نام_نوع OutputArray, نام_نوع Arg>
بنية subset_جابه_جایی_array_arg_t
{
  subset_جابه_جایی_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& بیرون_4,
			     ثابت فراغ *پایه_2,
			     Arg &&arg_) : subset_context (subset_context_), بیرون (بیرون_4),
					  پایه (پایه_2), arg (arg_) {}

  قالب <نام_نوع T>
  منطقي عملگر () (T&& جابه_جایی)
  {
    خودکار snap = subset_context->serializer->snapshot ();
    خودکار *o = بیرون.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, جابه_جایی, پایه, arg);
    إذا (!ret)
    {
      بیرون.برداشت ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &بیرون;
  ثابت فراغ *پایه;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
بنية
{
  قالب<نام_نوع OutputArray>
  subset_جابه_جایی_array_t<OutputArray>
  عملگر () (hb_subset_context_t *subset_context, OutputArray& بیرون,
	       ثابت فراغ *پایه) ثابت
  { أرجع subset_جابه_جایی_array_t<OutputArray> (subset_context, بیرون, پایه); }

  /* Variant with one extra argument passed to serialize_subset */
  قالب<نام_نوع OutputArray, نام_نوع Arg>
  subset_جابه_جایی_array_arg_t<OutputArray, Arg>
  عملگر () (hb_subset_context_t *subset_context, OutputArray& بیرون,
	       ثابت فراغ *پایه, Arg &&arg) ثابت
  { أرجع subset_جابه_جایی_array_arg_t<OutputArray, Arg> (subset_context, بیرون, پایه, arg); }
}
HB_FUNCOBJ (subset_جابه_جایی_array);

قالب<نام_نوع OutputArray>
بنية subset_رکورد_array_t
{
  subset_رکورد_array_t (hb_subset_layout_context_t *c_, OutputArray* بیرون_4,
			 ثابت فراغ *پایه_2) : subset_layout_context (c_),
					      بیرون (بیرون_4), پایه (پایه_2) {}

  قالب <نام_نوع T>
  فراغ
  عملگر () (T&& رکورد)
  {
    خودکار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = رکورد.subset (subset_layout_context, پایه);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا بیرون->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *بیرون;
  ثابت فراغ *پایه;
};

قالب<نام_نوع OutputArray, نام_نوع Arg>
بنية subset_رکورد_array_arg_t
{
  subset_رکورد_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* بیرون_4,
			     ثابت فراغ *پایه_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   بیرون (بیرون_4), پایه (پایه_2), arg (arg_) {}

  قالب <نام_نوع T>
  فراغ
  عملگر () (T&& رکورد)
  {
    خودکار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = رکورد.subset (subset_layout_context, پایه, arg);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا بیرون->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *بیرون;
  ثابت فراغ *پایه;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
بنية
{
  قالب<نام_نوع OutputArray>
  subset_رکورد_array_t<OutputArray>
  عملگر () (hb_subset_layout_context_t *c, OutputArray* بیرون,
	       ثابت فراغ *پایه) ثابت
  { أرجع subset_رکورد_array_t<OutputArray> (c, بیرون, پایه); }

  /* Variant with one extra argument passed to subset */
  قالب<نام_نوع OutputArray, نام_نوع Arg>
  subset_رکورد_array_arg_t<OutputArray, Arg>
  عملگر () (hb_subset_layout_context_t *c, OutputArray* بیرون,
               ثابت فراغ *پایه, Arg &&arg) ثابت
  { أرجع subset_رکورد_array_arg_t<OutputArray, Arg> (c, بیرون, پایه, arg); }
}
HB_FUNCOBJ (subset_رکورد_array);


قالب<نام_نوع OutputArray>
بنية serialize_math_رکورد_array_t
{
  serialize_math_رکورد_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& بیرون_4,
                         ثابت فراغ *پایه_2,
                         ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              بیرون (بیرون_4), پایه (پایه_2), variation_map (variation_map_) {}

  قالب <نام_نوع T>
  منطقي عملگر () (T&& رکورد)
  {
    إذا (!serialize_context->copy (رکورد, پایه, variation_map)) أرجع خطأ;
    بیرون.len++;
    أرجع درست;
  }

  خاص:
  hb_serialize_context_t *serialize_context;
  OutputArray &بیرون;
  ثابت فراغ *پایه;
  ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
بنية
{
  قالب<نام_نوع OutputArray>
  serialize_math_رکورد_array_t<OutputArray>
  عملگر () (hb_serialize_context_t *serialize_context, OutputArray& بیرون,
               ثابت فراغ *پایه,
               ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *variation_map) ثابت
  { أرجع serialize_math_رکورد_array_t<OutputArray> (serialize_context, بیرون, پایه, variation_map); }

}
HB_FUNCOBJ (serialize_math_رکورد_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

بنية شاخص_Array : Array16_از<شاخص>
{
  منطقي intersects (ثابت hb_map_t *indexes) ثابت
  { أرجع hb_any (*هذا, indexes); }

  قالب <نام_نوع Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    إذا (!it) أرجع;
    إذا (unlikely (!c->extend_min ((*هذا)))) أرجع;

    لكل (ثابت خودکار _ : it)
    {
      إذا (!l->بازدید_Lookup_شاخص()) اكسر;

      شاخص i;
      i = _;
      c->copy (i);
      هذا->len++;
    }
  }

  بیعلامت صحيح get_indexes (بیعلامت صحيح شروع_جابه_جایی,
			    بیعلامت صحيح *شمار_2 /* IN/OUT */,
			    بیعلامت صحيح *_indexes /*بیرون*/) ثابت
  {
    إذا (شمار_2 && _indexes)
    {
      + هذا->as_array ().sub_array (شروع_جابه_جایی, شمار_2)
      | hb_sink (hb_array (_indexes, *شمار_2))
      ;
    }
    أرجع هذا->len;
  }

  فراغ جمع_کن_indexes_مقصد (hb_مجموعه_t* output /*بیرون*/) ثابت
  {
    output->جمع_کن_array (as_array ());
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
	     range_شروع == 0 &&
	     range_انتها == 0)
      return_trace (درست);
    وإلا إذا (design_اندازه < range_شروع ||
	     design_اندازه > range_انتها ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (خطأ);
    وإلا
      return_trace (درست);
  }

  فراغ collect_name_ids (hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  { nameids_مقصد_retain->جمع_کن (subfamilyNameID); }

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
  HBUINT16	range_شروع;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	range_انتها;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  عام:
  DEFINE_اندازه_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
بنية Feature_Params_Stylistic_مجموعه
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (هذا));
  }

  فراغ collect_name_ids (hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  { nameids_مقصد_retain->جمع_کن (uiNameID); }

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
  بیعلامت
  get_characters (بیعلامت شروع_جابه_جایی, بیعلامت *char_شمار, hb_codepoint_t *chars) ثابت
  {
    إذا (char_شمار && chars)
    {
      + characters.as_array ().sub_array (شروع_جابه_جایی, char_شمار)
      | hb_sink (hb_array (chars, *char_شمار))
      ;
    }
    أرجع characters.len;
  }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (characters.len, HBUINT24::static_اندازه, min_اندازه); }

  فراغ collect_name_ids (hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  {
    إذا (featUILableNameID) nameids_مقصد_retain->جمع_کن (featUILableNameID);
    إذا (feat_UITooltip_متن_Name_ID) nameids_مقصد_retain->جمع_کن (feat_UITooltip_متن_Name_ID);
    إذا (sample_متن_Name_ID) nameids_مقصد_retain->جمع_کن (sample_متن_Name_ID);

    إذا (!اولی_Param_UILabel_Name_ID || !رقم_Named_Parameters || رقم_Named_Parameters >= 0x7FFF)
      أرجع;

    بیعلامت آخرین_name_id = (بیعلامت) اولی_Param_UILabel_Name_ID + (بیعلامت) رقم_Named_Parameters - 1;
    nameids_مقصد_retain->جمع_کن_range (اولی_Param_UILabel_Name_ID, آخرین_name_id);
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
  HBUINT16	رقم_Named_Parameters;	/* Number of named parameters. (May
					 * be zero.) */
  NameID	اولی_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_از<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  عام:
  DEFINE_اندازه_ARRAY (14, characters);
};

بنية FeatureParams
{
  منطقي sanitize (hb_sanitize_context_t *c, hb_برچسب_t برچسب_2) ثابت
  {
#اگر_تعریف_شده HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع درست;
#پایان_اگر
    TRACE_SANITIZE (هذا);
    إذا (برچسب_2 == HB_برچسب ('s','i','z','e') && hb_barrier ())
      return_trace (u.اندازه.sanitize (c));
    إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_مجموعه.sanitize (c));
    إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (درست);
  }

  فراغ collect_name_ids (hb_برچسب_t برچسب_2, hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  {
#اگر_تعریف_شده HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع;
#پایان_اگر
    إذا (برچسب_2 == HB_برچسب ('s','i','z','e') && hb_barrier ())
      أرجع (u.اندازه.collect_name_ids (nameids_مقصد_retain));
    إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('s','s','\0','\0')) /* ssXX */
      أرجع (u.stylistic_مجموعه.collect_name_ids (nameids_مقصد_retain));
    إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('c','v','\0','\0')) /* cvXX */
      أرجع (u.characterVariants.collect_name_ids (nameids_مقصد_retain));
  }

  منطقي subset (hb_subset_context_t *c, ثابت برچسب* برچسب_2) ثابت
  {
#اگر_تعریف_شده HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع خطأ;
#پایان_اگر
    TRACE_SUBSET (هذا);
    إذا (!برچسب_2) return_trace (خطأ);
    إذا (*برچسب_2 == HB_برچسب ('s','i','z','e') && hb_barrier ())
      return_trace (u.اندازه.subset (c));
    إذا ((*برچسب_2 & 0xFFFF0000u) == HB_برچسب ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_مجموعه.subset (c));
    إذا ((*برچسب_2 & 0xFFFF0000u) == HB_برچسب ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (خطأ);
  }

#اگر_تعریف_نشده HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت Feature_Params_اندازه& get_اندازه_params (hb_برچسب_t برچسب_2) ثابت
  {
    إذا (برچسب_2 == HB_برچسب ('s','i','z','e') && hb_barrier ())
      أرجع u.اندازه;
    أرجع Null (Feature_Params_اندازه);
  }
  ثابت Feature_Params_Stylistic_مجموعه& get_stylistic_مجموعه_params (hb_برچسب_t برچسب_2) ثابت
  {
    إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('s','s','\0','\0')) /* ssXX */
      أرجع u.stylistic_مجموعه;
    أرجع Null (Feature_Params_Stylistic_مجموعه);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_برچسب_t برچسب_2) ثابت
  {
    إذا ((برچسب_2 & 0xFFFF0000u) == HB_برچسب ('c','v','\0','\0')) /* cvXX */
      أرجع u.characterVariants;
    أرجع Null (FeatureParamsCharacterVariants);
  }
#پایان_اگر

  خاص:
  اتحاد {
  Feature_Params_اندازه			اندازه;
  Feature_Params_Stylistic_مجموعه		stylistic_مجموعه;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عام:
  DEFINE_اندازه_MIN (0);
};

بنية رکورد_sanitize_بستگی_t {
  hb_برچسب_t برچسب_2;
  ثابت فراغ *list_پایه;
};

بنية Feature
{
  بیعلامت صحيح get_lookup_شمار () ثابت
  { أرجع lookup_شاخص_2.len; }
  hb_برچسب_t get_lookup_شاخص (بیعلامت صحيح i) ثابت
  { أرجع lookup_شاخص_2[i]; }
  بیعلامت صحيح get_lookup_indexes (بیعلامت صحيح شروع_شاخص,
				   بیعلامت صحيح *lookup_شمار /* IN/OUT */,
				   بیعلامت صحيح *lookup_برچسبها /*بیرون*/) ثابت
  { أرجع lookup_شاخص_2.get_indexes (شروع_شاخص, lookup_شمار, lookup_برچسبها); }
  فراغ جمع_کن_lookup_indexes_مقصد (hb_مجموعه_t *lookup_indexes) ثابت
  { lookup_شاخص_2.جمع_کن_indexes_مقصد (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { أرجع هذا+featureParams; }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookup_شاخص_2.intersects (lookup_indexes); }

  فراغ collect_name_ids (hb_برچسب_t برچسب_2, hb_مجموعه_t *nameids_مقصد_retain /*بیرون*/) ثابت
  {
    إذا (featureParams)
      get_feature_params ().collect_name_ids (برچسب_2, nameids_مقصد_retain);
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت برچسب                   *برچسب_2 = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->featureParams.serialize_subset (c, featureParams, هذا, برچسب_2);

    hb::shared_ptr<hb_مجموعه_t> *lookup_variations;
    إذا (l->cur_feature_شاخص != شاخص::NOT_یافت_شاخص &&
	l->lookup_variations->has (l->cur_feature_شاخص, &lookup_variations))
    {
      خودکار it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_شاخص_map)
      | hb_map (l->lookup_شاخص_map)
      ;
      بیرون->lookup_شاخص_2.serialize (c->serializer, l, it);
    }
    وإلا
    {
      خودکار it =
      + hb_iter (lookup_شاخص_2)
      | hb_filter (l->lookup_شاخص_map)
      | hb_map (l->lookup_شاخص_map)
      ;
      بیرون->lookup_شاخص_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رکورد_sanitize_بستگی_t *بستگی = اشاره_تهی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureParams.sanitize (c, هذا, بستگی ? بستگی->برچسب_2 : HB_برچسب_NONE) &&
		  lookup_شاخص_2.sanitize (c));
  }

  Offset16_مقصد<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  شاخص_Array	 lookup_شاخص_2;	/* Array of LookupList indices */
  عام:
  DEFINE_اندازه_ARRAY_SIZED (4, lookup_شاخص_2);
};

قالب <نام_نوع Type>
بنية رکورد_2
{
  صحيح cmp (hb_برچسب_t a) ثابت { أرجع برچسب_2.cmp (a); }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *پایه, ثابت فراغ *f_sub = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (!f_sub)
      return_trace (بیرون->جابه_جایی.serialize_subset (c->subset_context, جابه_جایی, پایه, c, &برچسب_2));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    خودکار *s = c->subset_context->serializer;
    s->push ();

    بیرون->جابه_جایی = 0;
    منطقي ret = f.subset (c->subset_context, c, &برچسب_2);
    إذا (ret)
      s->جمع_کن_link (بیرون->جابه_جایی, s->برداشت_pack ());
    وإلا
      s->برداشت_discard ();

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت رکورد_sanitize_بستگی_t بستگی = {برچسب_2, پایه};
    return_trace (c->check_struct (هذا) &&
		  جابه_جایی.sanitize (c, پایه, &بستگی));
  }

  برچسب           برچسب_2;            /* 4-byte Tag identifier */
  Offset16_مقصد<Type>
                جابه_جایی;         /* Offset from beginning of object holding
                                 * the Record */
  عام:
  DEFINE_اندازه_STATIC (6);
};

قالب <نام_نوع Type>
بنية رکورد_Array_از : Sorted_Array16_از<رکورد_2<Type>>
{
  ثابت Offset16_مقصد<Type>& get_جابه_جایی (بیعلامت صحيح i) ثابت
  { أرجع (*هذا)[i].جابه_جایی; }
  Offset16_مقصد<Type>& get_جابه_جایی (بیعلامت صحيح i)
  { أرجع (*هذا)[i].جابه_جایی; }
  ثابت برچسب& get_برچسب (بیعلامت صحيح i) ثابت
  { أرجع (*هذا)[i].برچسب_2; }
  بیعلامت صحيح get_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
                         بیعلامت صحيح *رکورد_شمار /* IN/OUT */,
                         hb_برچسب_t     *رکورد_برچسبها /*بیرون*/) ثابت
  {
    إذا (رکورد_شمار && رکورد_برچسبها)
    {
      + هذا->as_array ().sub_array (شروع_جابه_جایی, رکورد_شمار)
      | hb_map (&رکورد_2<Type>::برچسب_2)
      | hb_sink (hb_array (رکورد_برچسبها, *رکورد_شمار))
      ;
    }
    أرجع هذا->len;
  }
  منطقي پیدا_کن_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  {
    أرجع هذا->bfind (برچسب_2, شاخص_2, HB_NOT_یافت_STORE, شاخص::NOT_یافت_شاخص);
  }
};

قالب <نام_نوع Type>
بنية رکورد_List_از : رکورد_Array_از<Type>
{
  ثابت Type& عملگر [] (بیعلامت صحيح i) ثابت
  { أرجع هذا+هذا->get_جابه_جایی (i); }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    + هذا->iter ()
    | hb_apply (subset_رکورد_array (l, بیرون, هذا))
    ;
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (رکورد_Array_از<Type>::sanitize (c, هذا));
  }
};

بنية رکورد_List_از_Feature : رکورد_List_از<Feature>
{
  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->feature_شاخص_map, hb_اولی)
    | hb_apply ([l, بیرون, هذا] (ثابت hb_pair_t<بیعلامت, ثابت رکورد_2<Feature>&>& _)
                {
                  ثابت Feature *f_sub = اشاره_تهی;
                  ثابت Feature **f = اشاره_تهی;
                  إذا (l->feature_substitutes_map->has (_.اولی, &f))
                    f_sub = *f;

                  l->cur_feature_شاخص = _.اولی;
                  subset_رکورد_array (l, بیرون, هذا, f_sub) (_.second);
                  l->cur_feature_شاخص = شاخص::NOT_یافت_شاخص;
                })
    ;

    return_trace (درست);
  }
};

تعريفنوع رکورد_List_از<Feature> FeatureList;


بنية LangSys
{
  بیعلامت صحيح get_feature_شمار () ثابت
  { أرجع feature_شاخص_2.len; }
  hb_برچسب_t get_feature_شاخص (بیعلامت صحيح i) ثابت
  { أرجع feature_شاخص_2[i]; }
  بیعلامت صحيح get_feature_indexes (بیعلامت صحيح شروع_جابه_جایی,
				    بیعلامت صحيح *feature_شمار_2 /* IN/OUT */,
				    بیعلامت صحيح *feature_indexes /*بیرون*/) ثابت
  { أرجع feature_شاخص_2.get_indexes (شروع_جابه_جایی, feature_شمار_2, feature_indexes); }
  فراغ جمع_کن_feature_indexes_مقصد (hb_مجموعه_t *feature_indexes) ثابت
  { feature_شاخص_2.جمع_کن_indexes_مقصد (feature_indexes); }

  منطقي has_required_feature () ثابت { أرجع req_Feature_شاخص != 0xFFFFu; }
  بیعلامت صحيح get_required_feature_شاخص () ثابت
  {
    إذا (req_Feature_شاخص == 0xFFFFu)
      أرجع شاخص::NOT_یافت_شاخص;
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

    خودکار iter =
    + hb_iter (feature_شاخص_2)
    | hb_filter (feature_شاخص_map)
    | hb_map (feature_شاخص_map)
    ;

    خودکار o_iter =
    + hb_iter (o.feature_شاخص_2)
    | hb_filter (feature_شاخص_map)
    | hb_map (feature_شاخص_map)
    ;

    لكل (; iter && o_iter; iter++, o_iter++)
    {
      بیعلامت a = *iter;
      بیعلامت b = *o_iter;
      إذا (a != b) أرجع خطأ;
    }

    إذا (iter || o_iter) أرجع خطأ;

    أرجع درست;
  }

  فراغ collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    إذا (!has_required_feature () && !get_feature_شمار ()) أرجع;
    إذا (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_شاخص))
      c->new_feature_indexes->جمع_کن (get_required_feature_شاخص ());

    + hb_iter (feature_شاخص_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت برچسب                  *برچسب_2 = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    ثابت uint32_t *v;
    بیرون->req_Feature_شاخص = l->feature_map_w_duplicates->has (req_Feature_شاخص, &v) ? *v : 0xFFFFu;

    إذا (!l->بازدید_Feature_شاخص (feature_شاخص_2.len))
      return_trace (خطأ);

    خودکار it =
    + hb_iter (feature_شاخص_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقي ret = منطقي (it);
    بیرون->feature_شاخص_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رکورد_sanitize_بستگی_t * = اشاره_تهی) ثابت
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
DECLARE_NULL_NAMESPACE_بایتها (OT, LangSys);

بنية Script
{
  بیعلامت صحيح get_lang_sys_شمار () ثابت
  { أرجع langSys.len; }
  ثابت برچسب& get_lang_sys_برچسب (بیعلامت صحيح i) ثابت
  { أرجع langSys.get_برچسب (i); }
  بیعلامت صحيح get_lang_sys_برچسبها (بیعلامت صحيح شروع_جابه_جایی,
				  بیعلامت صحيح *lang_sys_شمار /* IN/OUT */,
				  hb_برچسب_t     *lang_sys_برچسبها /*بیرون*/) ثابت
  { أرجع langSys.get_برچسبها (شروع_جابه_جایی, lang_sys_شمار, lang_sys_برچسبها); }
  ثابت LangSys& get_lang_sys (بیعلامت صحيح i) ثابت
  {
    إذا (i == شاخص::NOT_یافت_شاخص) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].جابه_جایی;
  }
  منطقي پیدا_کن_lang_sys_شاخص (hb_برچسب_t برچسب_2, بیعلامت صحيح *شاخص_2) ثابت
  { أرجع langSys.پیدا_کن_شاخص (برچسب_2, شاخص_2); }

  منطقي has_default_lang_sys () ثابت           { أرجع defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  فراغ prune_langsys (hb_prune_langsys_context_t *c,
                      بیعلامت script_شاخص) ثابت
  {
    إذا (!has_default_lang_sys () && !get_lang_sys_شمار ()) أرجع;
    إذا (!c->بازدید_Script ()) أرجع;

    إذا (!c->script_langsys_map->has (script_شاخص))
    {
      إذا (unlikely (!c->script_langsys_map->مجموعه (script_شاخص, hb::unique_ptr<hb_مجموعه_t> {hb_مجموعه_create ()})))
	أرجع;
    }

    إذا (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      إذا (c->بازدید_Langsys (d.get_feature_شمار ())) {
        d.collect_features (c);
      }

      لكل (خودکار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.جابه_جایی;
        إذا (!c->بازدید_Langsys (l.get_feature_شمار ())) استمر;
        إذا (l.compare (d, c->duplicate_feature_map)) استمر;

        l.collect_features (c);
        c->script_langsys_map->get (script_شاخص)->جمع_کن (_.اولی);
      }
    }
    وإلا
    {
      لكل (خودکار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.جابه_جایی;
        إذا (!c->بازدید_Langsys (l.get_feature_شمار ())) استمر;
        l.collect_features (c);
        c->script_langsys_map->get (script_شاخص)->جمع_کن (_.اولی);
      }
    }
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت برچسب                   *برچسب_2) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!l->بازدید_Script ()) return_trace (خطأ);
    إذا (برچسب_2 && !c->plan->layout_scripts.has (*برچسب_2))
      أرجع خطأ;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    منطقي defaultLang = خطأ;
    إذا (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = هذا+defaultLangSys;
      منطقي ret = ls.subset (c, l);
      إذا (!ret && برچسب_2 && *برچسب_2 != HB_برچسب ('D', 'F', 'L', 'T'))
      {
	c->serializer->برداشت_discard ();
	بیرون->defaultLangSys = 0;
      }
      وإلا
      {
	c->serializer->جمع_کن_link (بیرون->defaultLangSys, c->serializer->برداشت_pack ());
	defaultLang = درست;
      }
    }

    ثابت hb_مجموعه_t *فعال_langsys = l->script_langsys_map->get (l->cur_script_شاخص);
    إذا (فعال_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (فعال_langsys, hb_اولی)
      | hb_map (hb_دومی)
      | hb_filter ([=] (ثابت رکورد_2<LangSys>& رکورد) {أرجع l->بازدید_Lang_Sys (); })
      | hb_apply (subset_رکورد_array (l, &(بیرون->langSys), هذا))
      ;
    }

    return_trace (منطقي (بیرون->langSys.len) || defaultLang || l->جدول_برچسب_2 == HB_OT_برچسب_GSUB);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رکورد_sanitize_بستگی_t * = اشاره_تهی) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (defaultLangSys.sanitize (c, هذا) && langSys.sanitize (c, هذا));
  }

  محمي:
  Offset16_مقصد<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  رکورد_Array_از<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_اندازه_ARRAY_SIZED (4, langSys);
};

بنية رکورد_List_از_Script : رکورد_List_از<Script>
{
  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    لكل (خودکار _ : + hb_enumerate (*هذا))
    {
      خودکار snap = c->serializer->snapshot ();
      l->cur_script_شاخص = _.اولی;
      منطقي ret = _.second.subset (l, هذا);
      إذا (!ret) c->serializer->revert (snap);
      وإلا بیرون->len++;
    }

    return_trace (درست);
  }
};

تعريفنوع رکورد_List_از_Script ScriptList;



بنية LookupFlag : HBUINT16
{
  تعداد Flags {
    راست_مقصد_چپ		= 0x0001u,
    Ignore_پایه_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_مجموعه	= 0x0010u,
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
  بیعلامت صحيح get_subtable_شمار () ثابت { أرجع sub_جدول.len; }

  قالب <نام_نوع TSub_جدول>
  ثابت Array16_از_Offset16_مقصد<TSub_جدول>& get_subtables () ثابت
  { أرجع reinterpret_cast<ثابت Array16_از_Offset16_مقصد<TSub_جدول> &> (sub_جدول); }
  قالب <نام_نوع TSub_جدول>
  Array16_از_Offset16_مقصد<TSub_جدول>& get_subtables ()
  { أرجع reinterpret_cast<Array16_از_Offset16_مقصد<TSub_جدول> &> (sub_جدول); }

  قالب <نام_نوع TSub_جدول>
  ثابت TSub_جدول& get_subtable (بیعلامت صحيح i) ثابت
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }
  قالب <نام_نوع TSub_جدول>
  TSub_جدول& get_subtable (بیعلامت صحيح i)
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }

  size_t get_اندازه () ثابت
  {
    ثابت HBUINT16 &mark_Filtering_مجموعه = StructAfter<ثابت HBUINT16> (sub_جدول);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_مجموعه)
      أرجع (ثابت حرف *) &StructAfter<ثابت حرف> (mark_Filtering_مجموعه) - (ثابت حرف *) هذا;
    أرجع (ثابت حرف *) &mark_Filtering_مجموعه - (ثابت حرف *) هذا;
  }

  بیعلامت صحيح get_type () ثابت { أرجع lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    بیعلامت صحيح flag = lookupFlag;
    إذا (unlikely (flag & LookupFlag::Use_Mark_Filtering_مجموعه))
    {
      ثابت HBUINT16 &mark_Filtering_مجموعه = StructAfter<HBUINT16> (sub_جدول);
      flag += (mark_Filtering_مجموعه << 16);
    }
    أرجع flag;
  }

  قالب <نام_نوع TSub_جدول, نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بیعلامت صحيح lookup_type = get_type ();
    TRACE_DISPATCH (هذا, lookup_type);
    بیعلامت صحيح شمار = get_subtable_شمار ();
    لكل (بیعلامت صحيح i = 0; i < شمار; i++) {
      نام_نوع context_t::return_t r = get_subtable<TSub_جدول> (i).dispatch (c, lookup_type, ds...);
      إذا (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_مقدار ());
  }

  منطقي serialize (hb_serialize_context_t *c,
		  بیعلامت صحيح lookup_type,
		  uint32_t lookup_props,
		  بیعلامت صحيح رقم_subtables)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    إذا (unlikely (!sub_جدول.serialize (c, رقم_subtables))) return_trace (خطأ);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_مجموعه)
    {
      إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
      HBUINT16 &mark_Filtering_مجموعه = StructAfter<HBUINT16> (sub_جدول);
      mark_Filtering_مجموعه = lookup_props >> 16;
    }
    return_trace (درست);
  }

  قالب <نام_نوع TSub_جدول>
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->lookupType = lookupType;
    بیرون->lookupFlag = lookupFlag;

    ثابت hb_مجموعه_t *glyphset = c->plan->glyphset_gsub ();
    بیعلامت صحيح lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_جدول> ())
    | hb_filter ([هذا, glyphset, lookup_type] (ثابت Offset16_مقصد<TSub_جدول> &_) { أرجع (هذا+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_جابه_جایی_array (c, بیرون->get_subtables<TSub_جدول> (), هذا, lookup_type))
    ;

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_مجموعه)
    {
      ثابت HBUINT16 &mark_Filtering_مجموعه = StructAfter<HBUINT16> (sub_جدول);
      hb_codepoint_t *idx;
      إذا (!c->plan->استفاده_شده_mark_sets_map.has (mark_Filtering_مجموعه, &idx))
      {
        بیعلامت new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_مجموعه;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        بیرون->lookupFlag = new_flag;
      }
      وإلا
      {
        إذا (unlikely (!c->serializer->extend (بیرون))) return_trace (خطأ);
        HBUINT16 &بیرون_Mark_Filtering_مجموعه = StructAfter<HBUINT16> (بیرون->sub_جدول);
        بیرون_Mark_Filtering_مجموعه = *idx;
      }
    }

    // Always keep the lookup even if it's empty. The rest of layout subsetting depends on lookup
    // indices being consistent with those computed during planning. So if an empty lookup is
    // discarded during the subset phase it will invalidate all subsequent lookup indices.
    // Generally we shouldn't end up with an empty lookup as we pre-prune them during the planning
    // phase, but it can happen in rare cases such as when during closure subtable is considered
    // degenerate (see: https://github.com/harfbuzz/harfbuzz/issues/3853)
    return_trace (درست);
  }

  قالب <نام_نوع TSub_جدول>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) && sub_جدول.sanitize (c))) return_trace (خطأ);
    hb_barrier ();

    بیعلامت subtables = get_subtable_شمار ();
    إذا (unlikely (!c->بازدید_subtables (subtables))) return_trace (خطأ);

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_مجموعه)
    {
      ثابت HBUINT16 &mark_Filtering_مجموعه = StructAfter<HBUINT16> (sub_جدول);
      إذا (!mark_Filtering_مجموعه.sanitize (c)) return_trace (خطأ);
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
      بیعلامت صحيح type = get_subtable<TSub_جدول> (0).u.extension.get_type ();
      لكل (بیعلامت صحيح i = 1; i < subtables; i++)
	إذا (get_subtable<TSub_جدول> (i).u.extension.get_type () != type)
	  return_trace (خطأ);
    }
    return_trace (درست);
  }

  محمي:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_از<Offset16>
		sub_جدول;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عام:
  DEFINE_اندازه_ARRAY (6, sub_جدول);
};

قالب <نام_نوع Types>
استفاده LookupList = List16_از_جابه_جایی_مقصد<Lookup, نام_نوع Types::HBLUINT>;

قالب <نام_نوع TLookup, نام_نوع جابه_جایی_Type>
بنية Lookup_جابه_جایی_List : List16_از_جابه_جایی_مقصد<TLookup, جابه_جایی_Type>
{
  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->lookup_شاخص_map, hb_اولی)
    | hb_map (hb_دومی)
    | hb_apply (subset_جابه_جایی_array (c, *بیرون, هذا))
    ;
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((List16_از_جابه_جایی_مقصد<TLookup, جابه_جایی_Type>::sanitize (c, هذا)));
  }
};


/*
 * Coverage Table
 */


ساكن منطقي ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_مجموعه_t &klasses,
                                          منطقي use_class_zero,
                                          hb_sorted_وکتور_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  إذا (!klass_map)
    أرجع ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  إذا (!use_class_zero)
    klass_map->مجموعه (0, 0);

  بیعلامت idx = klass_map->has (0) ? 1 : 0;
  لكل (ثابت بیعلامت k: klasses)
  {
    إذا (klass_map->has (k)) استمر;
    klass_map->مجموعه (k, idx);
    idx++;
  }


  إذا (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    أرجع خطأ;

  لكل (بیعلامت i = 0; i < glyph_and_klass.درازی; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  أرجع ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

قالب <نام_نوع Types>
بنية ClassDefFormat1_3
{
  دوست بنية ClassDef;

  خاص:
  بیعلامت صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع class_مقدار[(بیعلامت صحيح) (glyph_id - شروع_Glyph)];
  }

  بیعلامت get_population () ثابت
  {
    أرجع class_مقدار.len;
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_sorted_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::اندازه == 2 ? 1 : 3;
      شروع_Glyph = 0;
      class_مقدار.len = 0;
      return_trace (درست);
    }

    hb_codepoint_t glyph_min = (*it).اولی;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_اولی)
			       | hb_reduce (hb_max, 0u);
    بیعلامت glyph_شمار = glyph_max - glyph_min + 1;

    شروع_Glyph = glyph_min;
    إذا (unlikely (!class_مقدار.serialize (c, glyph_شمار))) return_trace (خطأ);
    لكل (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      بیعلامت idx = gid_klass_pair.اولی - glyph_min;
      class_مقدار[idx] = gid_klass_pair.second;
    }
    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = اشاره_تهی /*OUT*/,
               منطقي keep_خالی_جدول = درست,
               منطقي use_class_zero = درست,
               ثابت Coverage* glyph_filter = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_sorted_وکتور_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_مجموعه_t orig_klasses;

    hb_codepoint_t شروع = شروع_Glyph;
    hb_codepoint_t انتها   = شروع + class_مقدار.len;

    لكل (ثابت hb_codepoint_t gid : + hb_range (شروع, انتها))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      إذا (new_gid == HB_MAP_مقدار_INVALID) استمر;
      إذا (glyph_filter && !glyph_filter->has(gid)) استمر;

      بیعلامت klass = class_مقدار[gid - شروع];
      إذا (!klass) استمر;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.جمع_کن (klass);
    }

    إذا (use_class_zero)
    {
      ثابت hb_مجموعه_t& glyphset = *c->plan->glyphset_gsub ();
      بیعلامت glyph_شمار = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_شمار <= glyph_and_klass.درازی;
    }
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_خالی_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && class_مقدار.sanitize (c));
  }

  بیعلامت هزینه () ثابت { أرجع 1; }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_coverage (مجموعه_t *glyphs) ثابت
  {
    بیعلامت صحيح شروع = 0;
    بیعلامت صحيح شمار = class_مقدار.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (class_مقدار[i])
	استمر;

      إذا (شروع != i)
	إذا (unlikely (!glyphs->جمع_کن_range (شروع_Glyph + شروع, شروع_Glyph + i)))
	  أرجع خطأ;

      شروع = i + 1;
    }
    إذا (شروع != شمار)
      إذا (unlikely (!glyphs->جمع_کن_range (شروع_Glyph + شروع, شروع_Glyph + شمار)))
	أرجع خطأ;

    أرجع درست;
  }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_class (مجموعه_t *glyphs, بیعلامت klass) ثابت
  {
    بیعلامت صحيح شمار = class_مقدار.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (class_مقدار[i] == klass) glyphs->جمع_کن (شروع_Glyph + i);
    أرجع درست;
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    hb_codepoint_t شروع = شروع_Glyph;
    hb_codepoint_t انتها = شروع_Glyph + class_مقدار.len;
    لكل (hb_codepoint_t iter = شروع_Glyph - 1;
	 glyphs->next (&iter) && iter < انتها;)
      إذا (class_مقدار[iter - شروع]) أرجع درست;
    أرجع خطأ;
  }
  منطقي intersects_class (ثابت hb_مجموعه_t *glyphs, بیعلامت klass) ثابت
  {
    بیعلامت صحيح شمار = class_مقدار.len;
    إذا (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_مجموعه_مقدار_INVALID;
      إذا (!glyphs->next (&g)) أرجع خطأ;
      إذا (g < شروع_Glyph) أرجع درست;
      g = شروع_Glyph + شمار - 1;
      إذا (glyphs->next (&g)) أرجع درست;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت نام_نوع Types::HBUINT *arr = class_مقدار.arrayZ;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (arr[i] == klass && glyphs->has (شروع_Glyph + i))
	أرجع درست;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_مجموعه_t *glyphs, بیعلامت klass, hb_مجموعه_t *intersect_glyphs) ثابت
  {
    بیعلامت شمار = class_مقدار.len;
    إذا (klass == 0)
    {
      بیعلامت شروع_glyph = شروع_Glyph;
      لكل (uint32_t g = HB_مجموعه_مقدار_INVALID;
	   glyphs->next (&g) && g < شروع_glyph;)
	intersect_glyphs->جمع_کن (g);

      لكل (uint32_t g = شروع_Glyph + شمار - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->جمع_کن (g);

      أرجع;
    }

    لكل (بیعلامت i = 0; i < شمار; i++)
      إذا (class_مقدار[i] == klass && glyphs->has (شروع_Glyph + i))
	intersect_glyphs->جمع_کن (شروع_Glyph + i);

#إذا 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    بیعلامت شروع_glyph = شروع_Glyph;
    بیعلامت انتها_glyph = شروع_glyph + شمار;
    لكل (بیعلامت g = شروع_Glyph - 1;
	 glyphs->next (&g) && g < انتها_glyph;)
      إذا (class_مقدار.arrayZ[g - شروع_glyph] == klass)
        intersect_glyphs->جمع_کن (g);
#پایان_اگر
  }

  فراغ intersected_classes (ثابت hb_مجموعه_t *glyphs, hb_مجموعه_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    إذا (!class_مقدار.len)
    {
      intersect_classes->جمع_کن (0);
      أرجع;
    }

    hb_codepoint_t انتها_glyph = شروع_Glyph + class_مقدار.len - 1;
    إذا (glyphs->get_min () < شروع_Glyph ||
        glyphs->get_max () > انتها_glyph)
      intersect_classes->جمع_کن (0);

    لكل (ثابت خودکار& _ : + hb_enumerate (class_مقدار))
    {
      hb_codepoint_t g = شروع_Glyph + _.اولی;
      إذا (glyphs->has (g))
        intersect_classes->جمع_کن (_.second);
    }
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  نام_نوع Types::HBGlyphID
		 شروع_Glyph;	/* First GlyphID of the classValueArray */
  نام_نوع Types::قالب Array_از<نام_نوع Types::HBUINT>
		class_مقدار;	/* Array of Class Values--one per GlyphID */
  عام:
  DEFINE_اندازه_ARRAY (2 + 2 * Types::اندازه, class_مقدار);
};

قالب <نام_نوع Types>
بنية ClassDefFormat2_4
{
  دوست بنية ClassDef;

  خاص:
  بیعلامت صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع range_رکورد.bsearch (glyph_id).value;
  }

  بیعلامت get_population () ثابت
  {
    نام_نوع Types::large_int ret = 0;
    لكل (ثابت خودکار &r : range_رکورد)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بیعلامت) ret;
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_sorted_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::اندازه == 2 ? 2 : 4;
      range_رکورد.len = 0;
      return_trace (درست);
    }

    بیعلامت unsorted = خطأ;
    بیعلامت رقم_ranges = 1;
    hb_codepoint_t prev_gid = (*it).اولی;
    بیعلامت prev_klass = (*it).second;

    Range_رکورد<Types> range_rec;
    range_rec.اولی = prev_gid;
    range_rec.آخرین = prev_gid;
    range_rec.value = prev_klass;

    خودکار *رکورد = c->copy (range_rec);
    إذا (unlikely (!رکورد)) return_trace (خطأ);

    لكل (ثابت خودکار gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.اولی;
      بیعلامت cur_klass = gid_klass_pair.second;

      إذا (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	إذا (unlikely (cur_gid < prev_gid))
	  unsorted = درست;

	إذا (unlikely (!رکورد)) اكسر;
	رکورد->آخرین = prev_gid;
	رقم_ranges++;

	range_rec.اولی = cur_gid;
	range_rec.آخرین = cur_gid;
	range_rec.value = cur_klass;

	رکورد = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    إذا (unlikely (c->درون_error ())) return_trace (خطأ);

    إذا (likely (رکورد)) رکورد->آخرین = prev_gid;
    range_رکورد.len = رقم_ranges;

    إذا (unlikely (unsorted))
      range_رکورد.as_array ().qsort (Range_رکورد<Types>::cmp_range);

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = اشاره_تهی /*OUT*/,
               منطقي keep_خالی_جدول = درست,
               منطقي use_class_zero = درست,
               ثابت Coverage* glyph_filter = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyph_مجموعه = *c->plan->glyphset_gsub ();

    hb_sorted_وکتور_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_مجموعه_t orig_klasses;

    إذا (glyph_مجموعه.get_population () * hb_بیت_storage ((بیعلامت) range_رکورد.len)
	< get_population ())
    {
      لكل (hb_codepoint_t g : glyph_مجموعه)
      {
	بیعلامت klass = get_class (g);
	إذا (!klass) استمر;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	إذا (new_gid == HB_MAP_مقدار_INVALID) استمر;
	إذا (glyph_filter && !glyph_filter->has (g)) استمر;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.جمع_کن (klass);
      }
    }
    وإلا
    {
      بیعلامت رقم_منبع_glyphs = c->plan->منبع->get_رقم_glyphs ();
      لكل (خودکار &range : range_رکورد)
      {
	بیعلامت klass = range.value;
	إذا (!klass) استمر;
	hb_codepoint_t شروع = range.اولی;
	hb_codepoint_t انتها   = hb_min (range.آخرین + 1, رقم_منبع_glyphs);
	لكل (hb_codepoint_t g = شروع; g < انتها; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  إذا (new_gid == HB_MAP_مقدار_INVALID) استمر;
	  إذا (glyph_filter && !glyph_filter->has (g)) استمر;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.جمع_کن (klass);
	}
      }
    }

    ثابت hb_مجموعه_t& glyphset = *c->plan->glyphset_gsub ();
    بیعلامت glyph_شمار = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_شمار <= glyph_and_klass.درازی;
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_خالی_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_رکورد.sanitize (c));
  }

  بیعلامت هزینه () ثابت { أرجع hb_بیت_storage ((بیعلامت) range_رکورد.len); /* bsearch cost */ }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_coverage (مجموعه_t *glyphs) ثابت
  {
    لكل (خودکار &range : range_رکورد)
      إذا (range.value)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    أرجع درست;
  }

  قالب <نام_نوع مجموعه_t>
  منطقي collect_class (مجموعه_t *glyphs, بیعلامت صحيح klass) ثابت
  {
    لكل (خودکار &range : range_رکورد)
    {
      إذا (range.value == klass)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    }
    أرجع درست;
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    إذا (range_رکورد.len > glyphs->get_population () * hb_بیت_storage ((بیعلامت) range_رکورد.len))
    {
      لكل (خودکار g : *glyphs)
        إذا (get_class (g))
	  أرجع درست;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_رکورد)
                   | hb_map ([glyphs] (ثابت Range_رکورد<Types> &range) { أرجع range.intersects (*glyphs) && range.value; }));
  }
  منطقي intersects_class (ثابت hb_مجموعه_t *glyphs, بیعلامت klass) ثابت
  {
    إذا (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      إذا (!range_رکورد.len)
        أرجع !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_مجموعه_مقدار_INVALID;
      hb_codepoint_t آخرین = HB_مجموعه_مقدار_INVALID;
      خودکار it = hb_iter (range_رکورد);
      لكل (خودکار &range : it)
      {
        إذا (it->اولی == آخرین + 1)
	{
	  it++;
	  استمر;
	}

	إذا (!glyphs->next (&g))
	  اكسر;
	إذا (g < range.اولی)
	  أرجع درست;
	g = range.آخرین;
	آخرین = g;
      }
      إذا (g != HB_مجموعه_مقدار_INVALID && glyphs->next (&g))
	أرجع درست;
      /* Fall through. */
    }
    لكل (ثابت خودکار &range : range_رکورد)
      إذا (range.value == klass && range.intersects (*glyphs))
	أرجع درست;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_مجموعه_t *glyphs, بیعلامت klass, hb_مجموعه_t *intersect_glyphs) ثابت
  {
    إذا (klass == 0)
    {
      hb_codepoint_t g = HB_مجموعه_مقدار_INVALID;
      لكل (خودکار &range : range_رکورد)
      {
	إذا (!glyphs->next (&g))
	  اذهب انجامشده;
	طالما (g < range.اولی)
	{
	  intersect_glyphs->جمع_کن (g);
	  إذا (!glyphs->next (&g))
	    اذهب انجامشده;
        }
        g = range.آخرین;
      }
      طالما (glyphs->next (&g))
	intersect_glyphs->جمع_کن (g);
      انجامشده:

      أرجع;
    }

    بیعلامت شمار = range_رکورد.len;
    إذا (شمار > glyphs->get_population () * hb_بیت_storage (شمار))
    {
      لكل (خودکار g : *glyphs)
      {
        بیعلامت i;
        إذا (range_رکورد.as_array ().bfind (g, &i) &&
	    range_رکورد.arrayZ[i].value == klass)
	  intersect_glyphs->جمع_کن (g);
      }
      أرجع;
    }

    لكل (خودکار &range : range_رکورد)
    {
      إذا (range.value != klass) استمر;

      بیعلامت انتها = range.آخرین + 1;
      لكل (hb_codepoint_t g = range.اولی - 1;
	   glyphs->next (&g) && g < انتها;)
	intersect_glyphs->جمع_کن (g);
    }
  }

  فراغ intersected_classes (ثابت hb_مجموعه_t *glyphs, hb_مجموعه_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    إذا (!range_رکورد.len)
    {
      intersect_classes->جمع_کن (0);
      أرجع;
    }

    hb_codepoint_t g = HB_مجموعه_مقدار_INVALID;
    لكل (خودکار &range : range_رکورد)
    {
      إذا (!glyphs->next (&g))
        اكسر;
      إذا (g < range.اولی)
      {
        intersect_classes->جمع_کن (0);
        اكسر;
      }
      g = range.آخرین;
    }
    إذا (g != HB_مجموعه_مقدار_INVALID && glyphs->next (&g))
      intersect_classes->جمع_کن (0);

    لكل (ثابت خودکار& range : range_رکورد)
      إذا (range.intersects (*glyphs))
        intersect_classes->جمع_کن (range.value);
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  نام_نوع Types::قالب Sorted_Array_از<Range_رکورد<Types>>
		range_رکورد;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::اندازه, range_رکورد);
};

بنية ClassDef
{
  /* Has interface. */
  بیعلامت عملگر [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Projection. */
  hb_codepoint_t عملگر () (hb_codepoint_t k) ثابت { أرجع get (k); }

  بیعلامت صحيح get (hb_codepoint_t k) ثابت { أرجع get_class (k); }
  بیعلامت صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_class (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_class (glyph_id);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_class (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_class (glyph_id);
#پایان_اگر
    افتراضي:أرجع 0;
    }
  }
  بیعلامت صحيح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    بیعلامت klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    klass = get_class (glyph_id);
    إذا (cache) cache->مجموعه (glyph_id, klass);
    أرجع klass;
  }

  بیعلامت get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#پایان_اگر
    افتراضي:أرجع NOT_COVERED;
    }
  }

  قالب<نام_نوع Iterator,
	   hb_requires (hb_is_sorted_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    خودکار glyphs = + it_with_class_zero | hb_filter (hb_دومی);

    بیعلامت format = 2;
    hb_codepoint_t glyph_max = 0;
    بیعلامت class_max = 0;
    إذا (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).اولی;
      glyph_max = glyph_min;

      بیعلامت رقم_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      بیعلامت prev_klass = (*glyphs).second;

      لكل (ثابت خودکار gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.اولی;
	بیعلامت cur_klass = gid_klass_pair.second;
	إذا (cur_klass > class_max) class_max = cur_klass;
	إذا (cur_gid == glyph_min) استمر;
        إذا (cur_gid > glyph_max) glyph_max = cur_gid;
	إذا (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  رقم_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t بهترین_اندازه = UINT64_MAX;
      بیعلامت glyph_بازه = glyph_max - glyph_min + 1;
      خودکار consider = [&] (بیعلامت candidate, uint64_t اندازه)
      {
	إذا (اندازه >= بهترین_اندازه) أرجع;
	بهترین_اندازه = اندازه;
	format = candidate;
      };

      إذا (glyph_max <= 0xFFFFu && glyph_بازه <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_اندازه * glyph_بازه);
      إذا (glyph_max <= 0xFFFFu && رقم_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) رقم_ranges *
		     (2 * HBGlyphID16::static_اندازه + HBUINT16::static_اندازه));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
      إذا (glyph_max <= 0xFFFFFFu && glyph_بازه <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_اندازه * glyph_بازه);
      إذا (glyph_max <= 0xFFFFFFu && رقم_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) رقم_ranges *
		     (2 * HBGlyphID24::static_اندازه + HBUINT16::static_اندازه));
#پایان_اگر
      إذا (unlikely (بهترین_اندازه == UINT64_MAX))
      {
	c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_سرریز);
	return_trace (خطأ);
      }
    }

    u.format.v = format;

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_دومی)));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_دومی)));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_دومی)));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_دومی)));
#پایان_اگر
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = اشاره_تهی /*OUT*/,
               منطقي keep_خالی_جدول = درست,
               منطقي use_class_zero = درست,
               ثابت Coverage* glyph_filter = اشاره_تهی) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_خالی_جدول, use_class_zero, glyph_filter));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_خالی_جدول, use_class_zero, glyph_filter));
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_خالی_جدول, use_class_zero, glyph_filter));
    حالة 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_خالی_جدول, use_class_zero, glyph_filter));
#پایان_اگر
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
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#پایان_اگر
    افتراضي:return_trace (درست);
    }
  }

  بیعلامت هزینه () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.هزینه ();
    حالة 2: hb_barrier (); أرجع u.format2.هزینه ();
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.هزینه ();
    حالة 4: hb_barrier (); أرجع u.format4.هزینه ();
#پایان_اگر
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  قالب <نام_نوع مجموعه_t>
  منطقي collect_coverage (مجموعه_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#پایان_اگر
    افتراضي:أرجع خطأ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  قالب <نام_نوع مجموعه_t>
  منطقي collect_class (مجموعه_t *glyphs, بیعلامت صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.collect_class (glyphs, klass);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.collect_class (glyphs, klass);
#پایان_اگر
    افتراضي:أرجع خطأ;
    }
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#پایان_اگر
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_class (ثابت hb_مجموعه_t *glyphs, بیعلامت صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_class (glyphs, klass);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_class (glyphs, klass);
#پایان_اگر
    افتراضي:أرجع خطأ;
    }
  }

  فراغ intersected_class_glyphs (ثابت hb_مجموعه_t *glyphs, بیعلامت klass, hb_مجموعه_t *intersect_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#پایان_اگر
    افتراضي:أرجع;
    }
  }

  فراغ intersected_classes (ثابت hb_مجموعه_t *glyphs, hb_مجموعه_t *intersect_classes) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_classes (glyphs, intersect_classes);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_classes (glyphs, intersect_classes);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_classes (glyphs, intersect_classes);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_classes (glyphs, intersect_classes);
#پایان_اگر
    افتراضي:أرجع;
    }
  }


  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#اگر_تعریف_نشده HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

قالب<نام_نوع Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ أرجع (c->شروع_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
بنية delta_ردیف_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  بنية chars_t : hb_وکتور_t<uint8_t>
  {
    صحيح cmp (ثابت chars_t& other) ثابت
    {
      أرجع as_array ().cmp (other.as_array ());
    }

    hb_pair_t<بیعلامت, بیعلامت> get_عرض ()
    {
      بیعلامت عرض = 0;
      بیعلامت columns = 0;
      لكل (بیعلامت i = 0; i < درازی; i++)
      {
	بیعلامت v = arrayZ[i];
	عرض += v;
	columns += (v != 0);
      }
      أرجع hb_pair (عرض, columns);
    }

    HB_HOT
    hb_pair_t<بیعلامت, بیعلامت> combine_عرض (ثابت chars_t& other) ثابت
    {
      بیعلامت combined_عرض = 0;
      بیعلامت combined_columns = 0;
      لكل (بیعلامت i = 0; i < درازی; i++)
      {
        بیعلامت v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_عرض += v;
	combined_columns += (v != 0);
      }
      أرجع hb_pair (combined_عرض, combined_columns);
    }
  };

  hb_pair_t<بیعلامت, بیعلامت> combine_عرض (ثابت delta_ردیف_encoding_t& other_encoding) ثابت { أرجع chars.combine_عرض (other_encoding.chars); }

  // Actual data

  chars_t chars;
  بیعلامت عرض = 0;
  بیعلامت overhead = 0;
  hb_وکتور_t<ثابت hb_وکتور_t<صحيح>*> اقلام;

  delta_ردیف_encoding_t () = افتراضي;
  delta_ردیف_encoding_t (hb_وکتور_t<ثابت hb_وکتور_t<صحيح>*> &&ردیفها, بیعلامت رقم_تعداد_ستون)
  {
    أكد (ردیفها);

    اقلام = std::move (ردیفها);

    إذا (unlikely (!chars.resize (رقم_تعداد_ستون)))
      أرجع;

    calculate_chars ();
  }

  فراغ ادغام (ثابت delta_ردیف_encoding_t& other)
  {
    اقلام.alloc (اقلام.درازی + other.اقلام.درازی);
    لكل (خودکار &ردیف : other.اقلام)
      جمع_کن_ردیف (ردیف);

    // Merge chars
    أكد (chars.درازی == other.chars.درازی);
    لكل (بیعلامت i = 0; i < chars.درازی; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_تغییریافته ();
  }

  فراغ chars_تغییریافته ()
  {
    خودکار _ = chars.get_عرض ();
    عرض = _.اولی;
    overhead = get_chars_overhead (_.second);
  }

  فراغ calculate_chars ()
  {
    أكد (اقلام);

    منطقي long_واژگان = خطأ;

    لكل (خودکار &ردیف : اقلام)
    {
      أكد (ردیف->درازی == chars.درازی);

      /* 0/1/2 byte encoding */
      لكل (بیعلامت i = 0; i < ردیف->درازی; i++)
      {
	صحيح v =  ردیف->arrayZ[i];
	إذا (v == 0)
	  استمر;
	وإلا إذا (v > 32767 || v < -32768)
	{
	  long_واژگان = درست;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	وإلا إذا (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	وإلا
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    إذا (long_واژگان)
    {
      // Convert 1s to 2s
      لكل (خودکار &v : chars)
	إذا (v == 1)
	  v = 2;
    }

    chars_تغییریافته ();
  }

  منطقي is_empty () ثابت
  { أرجع !اقلام; }

  منطقي درون_error () ثابت
  { أرجع chars.درون_error () || اقلام.درون_error (); }

  ساكن مضمن بیعلامت get_chars_overhead (بیعلامت رقم_columns)
  {
    بیعلامت c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    أرجع c + رقم_columns * 2;
  }

  بیعلامت get_gain (بیعلامت additional_بایتها_در_هر_ردیفها = 1) ثابت
  {
    صحيح شمار = اقلام.درازی;
    أرجع hb_max (0, (صحيح) overhead - شمار * (صحيح) additional_بایتها_در_هر_ردیفها);
  }

  صحيح gain_from_merging (ثابت delta_ردیف_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    علامتدار additional_بایتها_در_هر_ردیفها = other_encoding.عرض - عرض;
    إذا (additional_بایتها_در_هر_ردیفها > 0)
    {
      إذا (get_gain (additional_بایتها_در_هر_ردیفها) == 0)
        أرجع 0;
    }
    وإلا
    {
      إذا (other_encoding.get_gain (-additional_بایتها_در_هر_ردیفها) == 0)
	أرجع 0;
    }

    خودکار pair = combine_عرض (other_encoding);
    بیعلامت combined_عرض = pair.اولی;
    بیعلامت combined_columns = pair.second;

    صحيح combined_gain = (صحيح) overhead + (صحيح) other_encoding.overhead;
    combined_gain -= (combined_عرض - (صحيح) عرض) * اقلام.درازی;
    combined_gain -= (combined_عرض - (صحيح) other_encoding.عرض) * other_encoding.اقلام.درازی;
    combined_gain -= get_chars_overhead (combined_columns);

    أرجع combined_gain;
  }

  منطقي جمع_کن_ردیف (ثابت hb_وکتور_t<صحيح>* ردیف)
  { أرجع اقلام.push (ردیف); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت delta_ردیف_encoding_t *a = (ثابت delta_ردیف_encoding_t *)pa;
    ثابت delta_ردیف_encoding_t *b = (ثابت delta_ردیف_encoding_t *)pb;

    إذا (a->عرض != b->عرض)
      أرجع (صحيح) a->عرض - (صحيح) b->عرض;

    أرجع b->chars.cmp (a->chars);
  }
};

بنية VarRegionAxis
{
  عائم evaluate (صحيح coord) ثابت
  {
    صحيح peak = peakCoord.مقصد_int ();
    إذا (peak == 0 || coord == peak)
      أرجع 1.f;
    وإلا إذا (coord == 0) // Faster
      أرجع 0.f;

    صحيح شروع = شروع_Coord.مقصد_int (), انتها = انتها_Coord.مقصد_int ();

    /* TODO Move these to sanitize(). */
    إذا (unlikely (شروع > peak || peak > انتها))
      أرجع 1.f;
    إذا (unlikely (شروع < 0 && انتها > 0))
      أرجع 1.f;

    إذا (coord <= شروع || انتها <= coord)
      أرجع 0.f;

    /* Interpolate */
    إذا (coord < peak)
      أرجع عائم (coord - شروع) / (peak - شروع);
    وإلا
      أرجع عائم (انتها - coord) / (انتها - peak);
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
  F2DOT14	شروع_Coord;
  F2DOT14	peakCoord;
  F2DOT14	انتها_Coord;
  عام:
  DEFINE_اندازه_STATIC (6);
};
بنية SparseVarRegionAxis
{
  عائم evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len) ثابت
  {
    بیعلامت i = axis_شاخص_2;
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
    خودکار *بیرون = c->embed (src);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    إذا (axis_map)
    {
      إذا (unlikely (!axis_map->has (بیرون->axis_شاخص_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (خطأ);
      }
      إذا (unlikely (!c->check_تخصیص_بده (بیرون->axis_شاخص_2, axis_map->get (بیرون->axis_شاخص_2),
				    HB_SERIALIZE_ERROR_INT_سرریز)))
	return_trace (خطأ);
    }
    return_trace (درست);
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
  ساكن constexpr بیعلامت STATIC_درازی = 128;
  ساكن constexpr عائم INVALID = NAN;

  عام:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = حذف;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = حذف;
  hb_scalar_cache_t& عملگر= (ثابت hb_scalar_cache_t&) = حذف;
  hb_scalar_cache_t& عملگر= (hb_scalar_cache_t&&) = حذف;

  ساكن hb_scalar_cache_t *create (بیعلامت صحيح شمار,
				    hb_scalar_cache_t *scratch_cache = اشاره_تهی)
  {
    إذا (!شمار) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    إذا (scratch_cache && شمار <= STATIC_درازی)
    {
      scratch_cache->درازی = شمار;
      scratch_cache->clear ();
      أرجع scratch_cache;
    }

    خودکار *cache = (hb_scalar_cache_t *) hb_malloc (حجم (hb_scalar_cache_t) - حجم (static_مقادیر) + حجم (static_مقادیر[0]) * شمار);
    إذا (unlikely (!cache)) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->درازی = شمار;
    cache->clear ();

    أرجع cache;
  }

  ساكن فراغ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = اشاره_تهی)
  {
    إذا (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  فراغ clear ()
  {
    خودکار *values = &static_مقادیر[0];
    بیعلامت i = 0;
#اگر_تعریف_نشده HB_OPTIMIZE_اندازه
    لكل (; i + 3 < درازی; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#پایان_اگر
    لكل (; i < درازی; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقي get (بیعلامت i, عائم *value) ثابت
  {
    إذا (unlikely (i >= درازی))
    {
      *value = 0.f;
      أرجع درست;
    }
    خودکار *values = &static_مقادیر[0];
    خودکار *cached_مقدار = &values[i];
    عائم v = *cached_مقدار;
    إذا (std::isnan (v))
      أرجع خطأ;
    *value = v;
    أرجع درست;
  }

  HB_ALWAYS_INLINE
  فراغ مجموعه (بیعلامت i, عائم value)
  {
    إذا (unlikely (i >= درازی)) أرجع;
    خودکار *values = &static_مقادیر[0];
    خودکار *cached_مقدار = &values[i];
    *cached_مقدار = value;
  }

  عام:
  بیعلامت درازی;
  خاص:
  تغییرپذیر hb_atomic_t<عائم> static_مقادیر[STATIC_درازی];
};

بنية VarRegionList
{
  خاص:
  عائم evaluate_impl (بیعلامت صحيح region_شاخص,
		       ثابت صحيح *coords, بیعلامت صحيح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_شاخص * axis_شمار_2);
    عائم v = 1.f;

    بیعلامت صحيح شمار = axis_شمار_2;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
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
  عائم evaluate (بیعلامت صحيح region_شاخص,
		  ثابت صحيح *coords, بیعلامت صحيح coord_len,
		  hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    إذا (unlikely (region_شاخص >= region_شمار))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_شاخص, &v))
      أرجع v;

    v = evaluate_impl (region_شاخص, coords, coord_len);

    إذا (cache)
      cache->مجموعه (region_شاخص, v);
    أرجع v;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_شمار_2 * region_شمار));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_وکتور_t<hb_برچسب_t>& axis_برچسبها,
                  ثابت hb_وکتور_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت axis_شمار = axis_برچسبها.درازی;
    بیعلامت region_شمار_2 = regions.درازی;
    إذا (!axis_شمار || !region_شمار_2) return_trace (خطأ);
    إذا (unlikely (hb_unsigned_mul_overflows (axis_شمار * region_شمار_2,
                                             VarRegionAxis::static_اندازه))) return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_شمار_2 = axis_شمار;
    region_شمار = region_شمار_2;

    لكل (بیعلامت r = 0; r < region_شمار_2; r++)
    {
      ثابت خودکار& region = regions[r];
      لكل (بیعلامت i = 0; i < axis_شمار; i++)
      {
        hb_برچسب_t برچسب_2 = axis_برچسبها.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        إذا (region->has (برچسب_2, &coords))
        {
          var_region_rec.شروع_Coord.مجموعه_float (coords->minimum);
          var_region_rec.peakCoord.مجموعه_float (coords->میانه);
          var_region_rec.انتها_Coord.مجموعه_float (coords->بیشینه);
        }
        وإلا
        {
          var_region_rec.شروع_Coord.مجموعه_int (0);
          var_region_rec.peakCoord.مجموعه_int (0);
          var_region_rec.انتها_Coord.مجموعه_int (0);
        }
        إذا (!var_region_rec.serialize (c))
          return_trace (خطأ);
      }
    }
    return_trace (درست);
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت VarRegionList *src, ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_شمار_2 = src->axis_شمار_2;
    region_شمار = region_map.get_population ();
    إذا (unlikely (hb_unsigned_mul_overflows (axis_شمار_2 * region_شمار,
					     VarRegionAxis::static_اندازه))) return_trace (خطأ);
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
    بیعلامت صحيح region_شمار_2 = src->region_شمار;
    لكل (بیعلامت صحيح r = 0; r < region_شمار; r++)
    {
      بیعلامت صحيح backward = region_map.backward (r);
      إذا (backward >= region_شمار_2) return_trace (خطأ);
      hb_memcpy (&axesZ[axis_شمار_2 * r], &src->axesZ[axis_شمار_2 * backward], VarRegionAxis::static_اندازه * axis_شمار_2);
    }

    return_trace (درست);
  }

  منطقي get_var_region (بیعلامت region_شاخص,
                       ثابت hb_map_t& axes_old_شاخص_برچسب_map,
                       hb_hashmap_t<hb_برچسب_t, Triple>& axis_tuples /*بیرون*/) ثابت
  {
    إذا (region_شاخص >= region_شمار) أرجع خطأ;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_شاخص * axis_شمار_2);
    لكل (بیعلامت i = 0; i < axis_شمار_2; i++)
    {
      hb_برچسب_t *axis_برچسب_2;
      إذا (!axes_old_شاخص_برچسب_map.has (i, &axis_برچسب_2))
        أرجع خطأ;

      عائم min_val = axis_region->شروع_Coord.مقصد_float ();
      عائم def_val = axis_region->peakCoord.مقصد_float ();
      عائم max_val = axis_region->انتها_Coord.مقصد_float ();

      إذا (def_val != 0.f)
        axis_tuples.مجموعه (*axis_برچسب_2, Triple ((مزدوج) min_val, (مزدوج) def_val, (مزدوج) max_val));
      axis_region++;
    }
    أرجع !axis_tuples.درون_error ();
  }

  منطقي get_var_regions (ثابت hb_map_t& axes_old_شاخص_برچسب_map,
                        hb_وکتور_t<hb_hashmap_t<hb_برچسب_t, Triple>>& regions /*بیرون*/) ثابت
  {
    إذا (!regions.alloc (region_شمار))
      أرجع خطأ;

    لكل (بیعلامت i = 0; i < region_شمار; i++)
    {
      hb_hashmap_t<hb_برچسب_t, Triple> axis_tuples;
      إذا (!get_var_region (i, axes_old_شاخص_برچسب_map, axis_tuples))
        أرجع خطأ;
      regions.push (std::move (axis_tuples));
    }
    أرجع !regions.درون_error ();
  }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کن_saturate (min_اندازه,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_اندازه,
							       axis_شمار_2, region_شمار)); }

  عام:
  HBUINT16	axis_شمار_2;
  HBUINT15	region_شمار;
  محمي:
  Unsized_Array_از<VarRegionAxis>
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
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_برچسب_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_برچسب_t, Triple> &reachable_ranges)
{
  لكل (خودکار _ : axis_tuples)
  {
    Triple *range;
    إذا (!reachable_ranges.has (_.اولی, &range)) استمر;
    ثابت Triple &tent = _.second;
    صحيح شروع = (صحيح) roundf ((عائم) tent.minimum * 16384.f);
    صحيح peak  = (صحيح) roundf ((عائم) tent.میانه * 16384.f);
    صحيح انتها   = (صحيح) roundf ((عائم) tent.بیشینه * 16384.f);
    إذا (!peak) استمر;
    إذا (شروع > peak || peak > انتها) استمر;
    إذا (شروع < 0 && انتها > 0) استمر;
    صحيح lo = (صحيح) roundf ((عائم) range->minimum * 16384.f);
    صحيح hi = (صحيح) roundf ((عائم) range->بیشینه * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    إذا (hi < شروع || lo > انتها)
      أرجع درست;
  }
  أرجع خطأ;
}

بنية SparseVariationRegion
{
  عائم evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len) ثابت
  {
    عائم v = 1.f;
    بیعلامت صحيح شمار = axes.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
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
    إذا (unlikely (!axes.serialize (c, (بیعلامت) src->axes.len))) return_trace (خطأ);
    لكل (بیعلامت i = 0; i < axes.len; i++)
      إذا (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (خطأ);
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (axes.sanitize (c, هذا));
  }

  محمي:
  Array16_از<Offset32_مقصد<SparseVarRegionAxis>> axes;
  عام:
  DEFINE_اندازه_ARRAY (2, axes);
};

بنية SparseVarRegionList
{
  HB_ALWAYS_INLINE
  عائم evaluate (بیعلامت صحيح region_شاخص,
		  ثابت صحيح *coords, بیعلامت صحيح coord_len,
		  hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    إذا (unlikely (region_شاخص >= regions.len))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_شاخص, &v))
      أرجع v;

    ثابت SparseVariationRegion &region = هذا+regions[region_شاخص];

    v = region.evaluate (coords, coord_len);
    إذا (cache)
      cache->مجموعه (region_شاخص, v);

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

    لكل (بیعلامت i = 0; i < regions.len; i++)
    {
      بیعلامت old_شاخص = region_map.backward (i);
      إذا (unlikely (old_شاخص >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_شاخص]),
					     axis_map)))
	return_trace (خطأ);
    }
    return_trace (درست);
  }

  عام:
  Array32_از<Offset32_مقصد<SparseVariationRegion>>
		regions;
  عام:
  DEFINE_اندازه_ARRAY (4, regions);
};


بنية Var_داده
{
  بیعلامت صحيح get_قلم_شمار () ثابت
  { أرجع قلم_شمار; }

  بیعلامت صحيح get_region_شاخص_شمار () ثابت
  { أرجع regionIndices.len; }

  بیعلامت get_region_شاخص (بیعلامت i) ثابت
  { أرجع i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_ردیف_اندازه () ثابت
  { أرجع (کلمه_شمار () + regionIndices.len) * (long_واژگان_2 () ? 2 : 1); }

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کن_saturate (min_اندازه - regionIndices.min_اندازه,
				     regionIndices.get_اندازه (),
				     hb_unsigned_mul_saturate (قلم_شمار, get_ردیف_اندازه ())); }

  قالب <نام_نوع Float = عائم>
  Float _get_delta (بیعلامت صحيح inner,
		    ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    إذا (unlikely (inner >= قلم_شمار))
      أرجع 0.;
    منطقي is_long = long_واژگان_2 ();
    بیعلامت صحيح شمار = regionIndices.len;
    بیعلامت کلمه_شمار_2 = کلمه_شمار ();
    بیعلامت صحيح scount = is_long ? شمار : کلمه_شمار_2;
    بیعلامت صحيح lcount = is_long ? کلمه_شمار_2 : 0;

    ثابت HBUINT8 *بایتها = get_delta_بایتها ();
    ثابت HBUINT8 *ردیف = بایتها + inner * get_ردیف_اندازه ();

    Float delta = 0;
    بیعلامت صحيح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (ردیف);
    لكل (; i < lcount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *lcursor;
      lcursor++;
    }
    ثابت HBINT16 *scursor = reinterpret_cast<ثابت HBINT16 *> (lcursor);
    لكل (; i < scount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار, cache);
      إذا (scalar)
       delta += (Float) scalar * (صحيح) *scursor;
      scursor++;
    }
    ثابت HBINT8 *bcursor = reinterpret_cast<ثابت HBINT8 *> (scursor);
    لكل (; i < شمار; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *bcursor;
      bcursor++;
    }

    أرجع delta;
  }

  قالب <نام_نوع Float = عائم>
  HB_ALWAYS_INLINE
  Float get_delta (بیعلامت صحيح inner,
		   ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    بیعلامت صحيح شمار = regionIndices.len;
    إذا (!شمار) أرجع Float (0); // This is quite common, so optimize it.
    أرجع _get_delta<Float> (inner, coords, coord_شمار, regions, cache);
  }

  فراغ get_region_scalars (ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
			   ثابت VarRegionList &regions,
			   عائم *scalars /*OUT */,
			   بیعلامت صحيح رقم_scalars) ثابت
  {
    بیعلامت شمار = hb_min (رقم_scalars, regionIndices.len);
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_شمار);
    لكل (بیعلامت صحيح i = شمار; i < رقم_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  کلمه_شمار () <= regionIndices.len &&
		  c->check_range (get_delta_بایتها (),
				  قلم_شمار,
				  get_ردیف_اندازه ()));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_وکتور_t<ثابت hb_وکتور_t<صحيح>*>& ردیفها)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت ردیف_شمار_3 = ردیفها.درازی;
    إذا (!ردیف_شمار_3) {
      // Nothing to serialize, will be empty.
      أرجع خطأ;
    }

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!c->check_تخصیص_بده (قلم_شمار, ردیف_شمار_3,
                                    HB_SERIALIZE_ERROR_INT_سرریز)))
      return_trace (خطأ);

    صحيح min_آستانه = has_long ? -32768 : -128;
    صحيح max_آستانه = has_long ? +32767 : +127;
    تعداد delta_اندازه_t { kZero=0, k_Non_کلمه, k_کلمه };
    hb_وکتور_t<delta_اندازه_t> delta_sz;
    بیعلامت رقم_regions = ردیفها[0]->درازی;
    إذا (!delta_sz.resize (رقم_regions))
      return_trace (خطأ);

    بیعلامت کلمه_شمار_2 = 0;
    لكل (بیعلامت r = 0; r < رقم_regions; r++)
    {
      لكل (بیعلامت i = 0; i < ردیف_شمار_3; i++)
      {
        صحيح delta = ردیفها[i]->arrayZ[r];
        إذا (delta < min_آستانه || delta > max_آستانه)
        {
          delta_sz[r] = k_کلمه;
          کلمه_شمار_2++;
          اكسر;
        }
        وإلا إذا (delta != 0)
        {
          delta_sz[r] = k_Non_کلمه;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    بیعلامت کلمه_شاخص = 0;
    بیعلامت non_کلمه_شاخص = کلمه_شمار_2;
    hb_map_t ri_map;
    لكل (بیعلامت r = 0; r < رقم_regions; r++)
    {
      إذا (!delta_sz[r]) استمر;
      بیعلامت new_r = (delta_sz[r] == k_کلمه)? کلمه_شاخص++ : non_کلمه_شاخص++;
      إذا (!ri_map.مجموعه (new_r, r))
        return_trace (خطأ);
    }

    کلمه_اندازه_شمار = کلمه_شمار_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    بیعلامت ri_شمار = ri_map.get_population ();
    regionIndices.len = ri_شمار;
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (بیعلامت r = 0; r < ri_شمار; r++)
    {
      hb_codepoint_t *idx;
      إذا (!ri_map.has (r, &idx))
        return_trace (خطأ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_بایتها = get_delta_بایتها ();
    بیعلامت ردیف_اندازه = get_ردیف_اندازه ();
    لكل (بیعلامت صحيح i = 0; i < ردیف_شمار_3; i++)
    {
      لكل (بیعلامت صحيح r = 0; r < ri_شمار; r++)
      {
        صحيح delta = ردیفها[i]->arrayZ[ri_map[r]];
        مجموعه_قلم_delta_fast (i, r, delta, delta_بایتها, ردیف_اندازه);
      }
    }
    return_trace (درست);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Var_داده *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_مجموعه_t *culled_regions = اشاره_تهی)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    قلم_شمار = inner_map.get_بعدی_مقدار ();

    /* Optimize word count */
    بیعلامت ri_شمار = src->regionIndices.len;
    تعداد delta_اندازه_t { kZero=0, k_Non_کلمه, k_کلمه };
    hb_وکتور_t<delta_اندازه_t> delta_sz;
    hb_وکتور_t<بیعلامت صحيح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_شمار);
    ri_map.resize (ri_شمار);
    بیعلامت صحيح new_کلمه_شمار = 0;
    بیعلامت صحيح r;

    ثابت HBUINT8 *src_delta_بایتها = src->get_delta_بایتها ();
    بیعلامت src_ردیف_اندازه = src->get_ردیف_اندازه ();
    بیعلامت src_کلمه_شمار = src->کلمه_شمار ();
    منطقي     src_long_واژگان = src->long_واژگان_2 ();

    منطقي has_long = خطأ;
    إذا (src_long_واژگان)
    {
      لكل (r = 0; r < src_کلمه_شمار; r++)
      {
        لكل (بیعلامت old_gid : inner_map.کلیدها())
	{
	  int32_t delta = src->get_قلم_delta_fast (old_gid, r, src_delta_بایتها, src_ردیف_اندازه);
	  إذا (delta < -32768 || 32767 < delta)
	  {
	    has_long = درست;
	    اكسر;
	  }
        }
      }
    }

    علامتدار min_آستانه = has_long ? -32768 : -128;
    علامتدار max_آستانه = has_long ? +32767 : +127;
    لكل (r = 0; r < ri_شمار; r++)
    {
      منطقي short_circuit = src_long_واژگان == has_long && src_کلمه_شمار <= r;

      delta_sz[r] = kZero;
      إذا (culled_regions && culled_regions->has (src->regionIndices[r]))
	استمر; /* unreachable region: drop its column */
      لكل (بیعلامت old_gid : inner_map.کلیدها())
      {
	int32_t delta = src->get_قلم_delta_fast (old_gid, r, src_delta_بایتها, src_ردیف_اندازه);
	إذا (delta < min_آستانه || max_آستانه < delta)
	{
	  delta_sz[r] = k_کلمه;
	  new_کلمه_شمار++;
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

    بیعلامت صحيح کلمه_شاخص = 0;
    بیعلامت صحيح non_کلمه_شاخص = new_کلمه_شمار;
    بیعلامت صحيح new_ri_شمار = 0;
    لكل (r = 0; r < ri_شمار; r++)
      إذا (delta_sz[r])
      {
	بیعلامت new_r = (delta_sz[r] == k_کلمه)? کلمه_شاخص++ : non_کلمه_شاخص++;
	ri_map[new_r] = r;
	new_ri_شمار++;
      }

    کلمه_اندازه_شمار = new_کلمه_شمار | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_شمار;

    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (r = 0; r < new_ri_شمار; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_بایتها = get_delta_بایتها ();
    بیعلامت ردیف_اندازه = get_ردیف_اندازه ();
    بیعلامت شمار = قلم_شمار;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      بیعلامت صحيح old = inner_map.backward (i);
      لكل (بیعلامت صحيح r = 0; r < new_ri_شمار; r++)
	مجموعه_قلم_delta_fast (i, r,
			     src->get_قلم_delta_fast (old, ri_map[r],
						       src_delta_بایتها, src_ردیف_اندازه),
			     delta_بایتها, ردیف_اندازه);
    }

    return_trace (درست);
  }

  فراغ collect_region_refs (hb_مجموعه_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_بایتها = get_delta_بایتها ();
    بیعلامت ردیف_اندازه = get_ردیف_اندازه ();

    لكل (بیعلامت صحيح r = 0; r < regionIndices.len; r++)
    {
      بیعلامت صحيح region = regionIndices.arrayZ[r];
      إذا (region_indices.has (region)) استمر;
      لكل (hb_codepoint_t old_gid : inner_map.کلیدها())
	إذا (get_قلم_delta_fast (old_gid, r, delta_بایتها, ردیف_اندازه) != 0)
	{
	  region_indices.جمع_کن (region);
	  اكسر;
	}
    }
  }

  عام:
  ثابت HBUINT8 *get_delta_بایتها () ثابت
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  محمي:
  HBUINT8 *get_delta_بایتها ()
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  عام:
  int32_t get_قلم_delta_fast (بیعلامت صحيح قلم, بیعلامت صحيح region,
			       ثابت HBUINT8 *delta_بایتها, بیعلامت ردیف_اندازه) ثابت
  {
    إذا (unlikely (قلم >= قلم_شمار || region >= regionIndices.len)) أرجع 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_بایتها + قلم * ردیف_اندازه;
    بیعلامت کلمه_شمار_2 = کلمه_شمار ();
    منطقي is_long = long_واژگان_2 ();
    إذا (is_long)
    {
      إذا (region < کلمه_شمار_2)
	أرجع ((ثابت HBINT32 *) p)[region];
      وإلا
	أرجع ((ثابت HBINT16 *)(p + HBINT32::static_اندازه * کلمه_شمار_2))[region - کلمه_شمار_2];
    }
    وإلا
    {
      إذا (region < کلمه_شمار_2)
	أرجع ((ثابت HBINT16 *) p)[region];
      وإلا
	أرجع (p + HBINT16::static_اندازه * کلمه_شمار_2)[region - کلمه_شمار_2];
    }
  }
  int32_t get_قلم_delta (بیعلامت صحيح قلم, بیعلامت صحيح region) ثابت
  {
     أرجع get_قلم_delta_fast (قلم, region,
				 get_delta_بایتها (),
				 get_ردیف_اندازه ());
  }

  محمي:
  فراغ مجموعه_قلم_delta_fast (بیعلامت صحيح قلم, بیعلامت صحيح region, int32_t delta,
			    HBUINT8 *delta_بایتها, بیعلامت ردیف_اندازه)
  {
    HBINT8 *p = (HBINT8 *) delta_بایتها + قلم * ردیف_اندازه;
    بیعلامت کلمه_شمار_2 = کلمه_شمار ();
    منطقي is_long = long_واژگان_2 ();
    إذا (is_long)
    {
      إذا (region < کلمه_شمار_2)
	((HBINT32 *) p)[region] = delta;
      وإلا
	((HBINT16 *)(p + HBINT32::static_اندازه * کلمه_شمار_2))[region - کلمه_شمار_2] = delta;
    }
    وإلا
    {
      إذا (region < کلمه_شمار_2)
	((HBINT16 *) p)[region] = delta;
      وإلا
	(p + HBINT16::static_اندازه * کلمه_شمار_2)[region - کلمه_شمار_2] = delta;
    }
  }
  فراغ مجموعه_قلم_delta (بیعلامت صحيح قلم, بیعلامت صحيح region, int32_t delta)
  {
    مجموعه_قلم_delta_fast (قلم, region, delta,
			 get_delta_بایتها (),
			 get_ردیف_اندازه ());
  }

  منطقي long_واژگان_2 () ثابت { أرجع کلمه_اندازه_شمار & 0x8000u /* LONG_WORDS */; }
  بیعلامت کلمه_شمار () ثابت { أرجع کلمه_اندازه_شمار & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محمي:
  HBUINT16		قلم_شمار;
  HBUINT16		کلمه_اندازه_شمار;
  Array16_از<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عام:
  DEFINE_اندازه_ARRAY (6, regionIndices);
};

بنية Multi_Var_داده
{
  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_جمع_کن_saturate (min_اندازه - regionIndices.min_اندازه,
				     regionIndices.get_اندازه ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { أرجع هذا+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  فراغ get_delta (بیعلامت صحيح inner,
		  ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<مزدوج> بیرون,
		  hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    خودکار &deltaSets = get_delta_sets ();

    خودکار مقادیر_iter = deltaSets.fetcher (inner);
    بیعلامت region_شمار = regionIndices.len;
    بیعلامت skip = 0;
    لكل (بیعلامت region_شاخص_2 = 0; region_شاخص_2 < region_شمار; region_شاخص_2++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[region_شاخص_2],
				       coords, coord_شمار,
				       cache);
      // We skip lazily. Helps with the tail end.
      إذا (scalar == 0.0f)
        skip += بیرون.درازی;
      وإلا
      {
        إذا (skip)
	{
	  مقادیر_iter.skip (skip);
	  skip = 0;
	}
	مقادیر_iter.جمع_کن_مقصد_2 (بیرون, scalar);
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

  بیعلامت get_قلم_شمار () ثابت
  { أرجع get_delta_sets ().شمار; }

  فراغ collect_region_refs (hb_مجموعه_t &region_indices) ثابت
  {
    لكل (بیعلامت region : regionIndices)
      region_indices.جمع_کن (region);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_Var_داده *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت region_شمار_2 = src->regionIndices.len;
    بیعلامت سرآیند_اندازه = HBUINT8::static_اندازه +
			   Array16_از<HBUINT16>::min_اندازه +
			   region_شمار_2 * HBUINT16::static_اندازه +
			   HBUINT32::static_اندازه;
    إذا (unlikely (!c->extend_اندازه (هذا, سرآیند_اندازه, خطأ)))
      return_trace (خطأ);
    format = 1;
    regionIndices.len = region_شمار_2;
    لكل (بیعلامت i = 0; i < regionIndices.len; i++)
    {
      بیعلامت old_region = src->regionIndices[i];
      إذا (unlikely (!region_map.has (old_region)))
	return_trace (خطأ);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت خودکار &src_delta_sets = src->get_delta_sets ();
    hb_وکتور_t<hb_ubytes_t> delta_sets;
    بیعلامت داده_اندازه = 0;
    إذا (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (خطأ);
    لكل (بیعلامت i = 0; i < inner_map.get_population (); i++)
    {
      بیعلامت old_inner = inner_map.backward (i);
      إذا (unlikely (old_inner >= src_delta_sets.شمار)) return_trace (خطأ);
      hb_ubytes_t بایتها = static_cast<ثابت CFF2_شاخص &> (src_delta_sets)[old_inner];
      داده_اندازه = hb_unsigned_جمع_کن_saturate (داده_اندازه, بایتها.درازی);
      delta_sets.push (بایتها);
    }
    إذا (unlikely (داده_اندازه == UINT_MAX || delta_sets.درون_error ()))
      return_trace (خطأ);

    خودکار &بیرون_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (بیرون_delta_sets.serialize_serialize (c, delta_sets.iter (), &داده_اندازه));
  }

  محمي:
  HBUINT8	      format; // 1
  Array16_از<HBUINT16> regionIndices;
  Offset32_مقصد<TupleList> deltaSetsX;
  عام:
  DEFINE_اندازه_MIN (7);
};

بنية قلم_Variation_Store
{
  دوست بنية قلم_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#پایان_اگر
    أرجع hb_scalar_cache_t::create ((هذا+regions).region_شمار);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  خاص:
  قالب <نام_نوع Float = عائم>
  Float get_delta (بیعلامت صحيح outer, بیعلامت صحيح inner,
		   ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		   hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع Float (0);
#پایان_اگر

    إذا (unlikely (outer >= داده_Sets.len))
      أرجع Float (0);

    أرجع (هذا+داده_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_شمار,
					     هذا+regions,
					     cache);
  }

  عام:
  منطقي has_delta_مجموعه (بیعلامت صحيح شاخص_2) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع خطأ;
#پایان_اگر

    بیعلامت صحيح outer = شاخص_2 >> 16;
    بیعلامت صحيح inner = شاخص_2 & 0xFFFF;
    أرجع outer < داده_Sets.len &&
	   inner < (هذا+داده_Sets[outer]).get_قلم_شمار ();
  }

  قالب <نام_نوع Float = عائم>
  Float get_delta (بیعلامت صحيح شاخص_2,
		   ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		   hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    بیعلامت صحيح outer = شاخص_2 >> 16;
    بیعلامت صحيح inner = شاخص_2 & 0xFFFF;
    أرجع get_delta<Float> (outer, inner, coords, coord_شمار, cache);
  }
  قالب <نام_نوع Float = عائم>
  Float get_delta (بیعلامت صحيح شاخص_2,
		   hb_array_t<ثابت صحيح> coords,
		   hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    أرجع get_delta<Float> (شاخص_2,
		      coords.arrayZ, coords.درازی,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع درست;
#پایان_اگر

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  داده_Sets.sanitize (c, هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_وکتور_t<hb_برچسب_t>& axis_برچسبها,
                  ثابت hb_وکتور_t<ثابت hb_hashmap_t<hb_برچسب_t, Triple>*>& region_list,
                  ثابت hb_وکتور_t<delta_ردیف_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (هذا);
#اگر_تعریف_شده HB_NO_VAR
    return_trace (خطأ);
#پایان_اگر
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    format = 1;
    إذا (!regions.serialize_serialize (c, axis_برچسبها, region_list))
      return_trace (خطأ);

    بیعلامت رقم_var_داده = vardata_encodings.درازی;
    إذا (!رقم_var_داده) return_trace (خطأ);
    إذا (unlikely (!c->check_تخصیص_بده (داده_Sets.len, رقم_var_داده,
                                    HB_SERIALIZE_ERROR_INT_سرریز)))
      return_trace (خطأ);

    إذا (unlikely (!c->extend (داده_Sets))) return_trace (خطأ);
    لكل (بیعلامت i = 0; i < رقم_var_داده; i++)
      إذا (!داده_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].اقلام))
        return_trace (خطأ);

    return_trace (درست);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت قلم_Variation_Store *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_مجموعه_t *culled_regions = اشاره_تهی)
  {
    TRACE_SERIALIZE (هذا);
#اگر_تعریف_شده HB_NO_VAR
    return_trace (خطأ);
#پایان_اگر

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بیعلامت صحيح مجموعه_شمار = 0;
    لكل (بیعلامت صحيح i = 0; i < inner_maps.درازی; i++)
      إذا (inner_maps[i].get_population ())
	مجموعه_شمار++;

    format = 1;

    ثابت خودکار &src_regions = src+src->regions;

    hb_مجموعه_t region_indices;
    لكل (بیعلامت صحيح i = 0; i < inner_maps.درازی; i++)
      (src+src->داده_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    إذا (region_indices.درون_error ())
      return_trace (خطأ);

    region_indices.del_range ((src_regions).region_شمار, hb_مجموعه_t::INVALID);

    إذا (culled_regions)
      region_indices.تفریق (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (بیعلامت _) { region_map.جمع_کن(_); })
    ;
    إذا (region_map.درون_error())
      return_trace (خطأ);

    إذا (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (خطأ);

    داده_Sets.len = مجموعه_شمار;
    إذا (unlikely (!c->extend (داده_Sets))) return_trace (خطأ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    بیعلامت صحيح مجموعه_شاخص = 0;
    لكل (بیعلامت صحيح i = 0; i < inner_maps.درازی; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!داده_Sets[مجموعه_شاخص++]
		     .serialize_serialize (c, &(src+src->داده_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (خطأ);
    }

    return_trace (درست);
  }

  قلم_Variation_Store *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->شروع_embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (اشاره_تهی);

    hb_وکتور_t <hb_inc_bimap_t> inner_maps;
    بیعلامت شمار = داده_Sets.len;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      إذا (unlikely (!c->propagate_error(inner_maps)))
        return_trace(اشاره_تهی);
      خودکار &داده = هذا+داده_Sets[i];

      بیعلامت قلم_شمار = داده.get_قلم_شمار ();
      لكل (بیعلامت j = 0; j < قلم_شمار; j++)
	map->جمع_کن (j);
    }

    إذا (unlikely (!بیرون->serialize (c, هذا, inner_maps))) return_trace (اشاره_تهی);

    return_trace (بیرون);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقي collect_dead_regions (ثابت hb_map_t &axes_old_شاخص_برچسب_map,
			     ثابت hb_hashmap_t<hb_برچسب_t, Triple> &reachable_ranges,
			     hb_مجموعه_t &dead_regions /*بیرون*/) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    إذا (!reachable_ranges.get_population ()) أرجع درست;
    ثابت VarRegionList &region_list = هذا+regions;
    بیعلامت شمار = region_list.region_شمار;
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      hb_hashmap_t<hb_برچسب_t, Triple> axis_tuples;
      إذا (!region_list.get_var_region (i, axes_old_شاخص_برچسب_map, axis_tuples))
	أرجع خطأ;
      إذا (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.جمع_کن (i);
    }
    أرجع !dead_regions.درون_error ();
#وإلا
    أرجع درست;
#پایان_اگر
  }

  منطقي subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (هذا);
#اگر_تعریف_شده HB_NO_VAR
    return_trace (خطأ);
#پایان_اگر

    قلم_Variation_Store *varstore_اول = c->serializer->شروع_embed<قلم_Variation_Store> ();
    إذا (unlikely (!varstore_اول)) return_trace (خطأ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_مجموعه_t dead_regions;
    إذا (c->plan->has_avar2)
      إذا (!collect_dead_regions (c->plan->axes_old_شاخص_برچسب_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (خطأ);

    varstore_اول->serialize (c->serializer, هذا, inner_maps,
			       dead_regions.get_population () ? &dead_regions : اشاره_تهی);

    return_trace (
        !c->serializer->درون_error()
        && varstore_اول->داده_Sets);
  }

  بیعلامت صحيح get_region_شاخص_شمار (بیعلامت صحيح major) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع 0;
#پایان_اگر
    أرجع (هذا+داده_Sets[major]).get_region_شاخص_شمار ();
  }

  فراغ get_region_scalars (بیعلامت صحيح major,
			   ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
			   عائم *scalars /*OUT*/,
			   بیعلامت صحيح رقم_scalars) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    لكل (بیعلامت i = 0; i < رقم_scalars; i++)
      scalars[i] = 0.f;
    أرجع;
#پایان_اگر

    (هذا+داده_Sets[major]).get_region_scalars (coords, coord_شمار,
					       هذا+regions,
					       &scalars[0], رقم_scalars);
  }

  بیعلامت صحيح get_sub_جدول_شمار () ثابت
   {
#اگر_تعریف_شده HB_NO_VAR
     أرجع 0;
#پایان_اگر
     أرجع داده_Sets.len;
   }

  ثابت Var_داده& get_sub_جدول (بیعلامت i) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
     أرجع Null (Var_داده);
#پایان_اگر
     أرجع هذا+داده_Sets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
     أرجع Null (VarRegionList);
#پایان_اگر
     أرجع هذا+regions;
  }

  محمي:
  HBUINT16				format;
  Offset32_مقصد<VarRegionList>		regions;
  Array16_از_Offset32_مقصد<Var_داده>		داده_Sets;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (8, داده_Sets);
};

بنية Multi_قلم_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = اشاره_تهی) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#پایان_اگر
    خودکار &r = هذا+regions;
    بیعلامت شمار = r.regions.len;

    أرجع hb_scalar_cache_t::create (شمار, static_cache);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = اشاره_تهی)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  خاص:
  فراغ get_delta (بیعلامت صحيح outer, بیعلامت صحيح inner,
		  ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		  hb_array_t<مزدوج> بیرون,
		  hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع;
#پایان_اگر

    إذا (unlikely (outer >= داده_Sets.len))
      أرجع;

    أرجع (هذا+داده_Sets[outer]).get_delta (inner,
					     coords, coord_شمار,
					     هذا+regions,
					     بیرون,
					     cache);
  }

  عام:
  فراغ get_delta (بیعلامت صحيح شاخص_2,
		  ثابت صحيح *coords, بیعلامت صحيح coord_شمار,
		  hb_array_t<مزدوج> بیرون,
		  hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    بیعلامت صحيح outer = شاخص_2 >> 16;
    بیعلامت صحيح inner = شاخص_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_شمار, بیرون, cache);
  }
  فراغ get_delta (بیعلامت صحيح شاخص_2,
		  hb_array_t<ثابت صحيح> coords,
		  hb_array_t<مزدوج> بیرون,
		  hb_scalar_cache_t *cache = اشاره_تهی) ثابت
  {
    أرجع get_delta (شاخص_2,
		      coords.arrayZ, coords.درازی,
		      بیرون,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#اگر_تعریف_شده HB_NO_VAR
    أرجع درست;
#پایان_اگر

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  داده_Sets.sanitize (c, هذا));
  }

  منطقي create_subset_plan (ثابت hb_مجموعه_t &var_indices,
			   hb_وکتور_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    إذا (unlikely (!inner_maps->resize (داده_Sets.len))) أرجع خطأ;
    لكل (hb_codepoint_t var_idx : var_indices)
    {
      بیعلامت outer = var_idx >> 16;
      بیعلامت inner = var_idx & 0xFFFFu;
      إذا (unlikely (outer >= داده_Sets.len ||
		    inner >= (هذا+داده_Sets[outer]).get_قلم_شمار ()))
	أرجع خطأ;
      (*inner_maps)[outer].جمع_کن (inner);
    }

    بیعلامت new_outer = 0;
    لكل (بیعلامت outer = 0; outer < inner_maps->درازی; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      إذا (!inner_map.get_population ()) استمر;
      إذا (unlikely (inner_map.درون_error ())) أرجع خطأ;
      لكل (بیعلامت new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	بیعلامت old_inner = inner_map.backward (new_inner);
	varidx_map->مجموعه ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    أرجع !inner_maps->درون_error () && !varidx_map->درون_error ();
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_قلم_Variation_Store *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا) ||
		  inner_maps.درازی < src->داده_Sets.len))
      return_trace (خطأ);
    format = 1;

    ثابت خودکار &src_regions = src+src->regions;
    hb_مجموعه_t region_indices;
    بیعلامت مجموعه_شمار = 0;
    لكل (بیعلامت i = 0; i < src->داده_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      مجموعه_شمار++;
      (src+src->داده_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_مجموعه_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.جمع_کن_مجموعه (&region_indices);
    إذا (unlikely (region_indices.درون_error () || region_map.درون_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (خطأ);

    داده_Sets.len = مجموعه_شمار;
    إذا (unlikely (!c->extend (داده_Sets))) return_trace (خطأ);
    بیعلامت مجموعه_شاخص = 0;
    لكل (بیعلامت i = 0; i < src->داده_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!داده_Sets[مجموعه_شاخص++].serialize_serialize (
			    c, &(src+src->داده_Sets[i]), inner_maps[i], region_map)))
	return_trace (خطأ);
    }
    return_trace (درست);
  }

  محمي:
  HBUINT16				format; // 1
  Offset32_مقصد<SparseVarRegionList>	regions;
  Array16_از_Offset32_مقصد<Multi_Var_داده>	داده_Sets;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (8, داده_Sets);
};

قالب <نام_نوع Map_شمار_T>
بنية Delta_مجموعه_شاخص_Map_Format01
{
  دوست بنية Delta_مجموعه_شاخص_Map;

  size_t get_اندازه () ثابت
  { أرجع hb_unsigned_mul_جمع_کن_saturate (map_شمار_2, get_عرض (), min_اندازه); }

  خاص:
  Delta_مجموعه_شاخص_Map_Format01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  قالب <نام_نوع T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    بیعلامت صحيح عرض = plan.get_عرض ();
    بیعلامت صحيح inner_بیت_شمار = plan.get_inner_بیت_شمار ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (هذا);
    إذا (unlikely (output_map.درازی && ((((inner_بیت_شمار-1)&~0xF)!=0) || (((عرض-1)&~0x3)!=0))))
      return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    مدخل_Format = ((عرض-1)<<4)|(inner_بیت_شمار-1);
    map_شمار_2 = output_map.درازی;
    HBUINT8 *p = c->allocate_اندازه<HBUINT8> (عرض * output_map.درازی);
    إذا (unlikely (!p)) return_trace (خطأ);
    لكل (بیعلامت صحيح i = 0; i < output_map.درازی; i++)
    {
      بیعلامت صحيح v = output_map.arrayZ[i];
      إذا (v)
      {
	بیعلامت صحيح outer = v >> 16;
	بیعلامت صحيح inner = v & 0xFFFF;
	بیعلامت صحيح u = (outer << inner_بیت_شمار) | inner;
	لكل (بیعلامت صحيح w = عرض; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += عرض;
    }
    return_trace (درست);
  }

  HB_ALWAYS_INLINE
  uint32_t map (بیعلامت صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    إذا (!map_شمار_2)
      أرجع v;
    أرجع _map (v);
  }

  HB_HOT
  uint32_t _map (بیعلامت صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    إذا (v >= map_شمار_2)
      v = map_شمار_2 - 1;

    بیعلامت صحيح u = 0;
    { /* Fetch it. */
      بیعلامت صحيح w = get_عرض ();
      ثابت HBUINT8 *p = map_داده_Z.arrayZ + w * v;
      لكل (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      بیعلامت صحيح n = get_inner_بیت_شمار ();
      بیعلامت صحيح outer = u >> n;
      بیعلامت صحيح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    أرجع u;
  }

  بیعلامت get_map_شمار () ثابت       { أرجع map_شمار_2; }
  بیعلامت get_عرض () ثابت           { أرجع ((مدخل_Format >> 4) & 3) + 1; }
  بیعلامت get_inner_بیت_شمار () ثابت { أرجع (مدخل_Format & 0xF) + 1; }


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  c->check_range (map_داده_Z.arrayZ,
                                  map_شمار_2,
                                  get_عرض ()));
  }

  محمي:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       مدخل_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_شمار_T     map_شمار_2;       /* The number of mapping entries. */
  Unsized_Array_از<HBUINT8>
                map_داده_Z;       /* The delta-set index mapping data. */

  عام:
  DEFINE_اندازه_ARRAY (2+Map_شمار_T::static_اندازه, map_داده_Z);
};

بنية Delta_مجموعه_شاخص_Map
{
  قالب <نام_نوع T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (هذا);
    بیعلامت درازی = plan.get_output_map ().درازی;
    u.format.v = درازی <= 0xFFFF ? 0 : 1;
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    افتراضي:return_trace (خطأ);
    }
  }

  uint32_t map (بیعلامت v) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع (u.format0.map (v));
    حالة 1: hb_barrier (); أرجع (u.format1.map (v));
    افتراضي:أرجع v;
    }
  }

  بیعلامت get_map_شمار () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_map_شمار ();
    حالة 1: hb_barrier (); أرجع u.format1.get_map_شمار ();
    افتراضي:أرجع 0;
    }
  }

  بیعلامت get_عرض () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_عرض ();
    حالة 1: hb_barrier (); أرجع u.format1.get_عرض ();
    افتراضي:أرجع 0;
    }
  }

  بیعلامت get_inner_بیت_شمار () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_inner_بیت_شمار ();
    حالة 1: hb_barrier (); أرجع u.format1.get_inner_بیت_شمار ();
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
    افتراضي:return_trace (درست);
    }
  }

  Delta_مجموعه_شاخص_Map* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (reinterpret_cast<Delta_مجموعه_شاخص_Map *> (u.format0.copy (c)));
    حالة 1: hb_barrier (); return_trace (reinterpret_cast<Delta_مجموعه_شاخص_Map *> (u.format1.copy (c)));
    افتراضي:return_trace (اشاره_تهی);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }             format;         /* Format identifier */
  Delta_مجموعه_شاخص_Map_Format01<HBUINT16> format0;
  Delta_مجموعه_شاخص_Map_Format01<HBUINT32> format1;
  } u;
  عام:
  DEFINE_اندازه_UNION (1, format.v);
};


بنية قلم_Var_Store_Instancer
{
  قلم_Var_Store_Instancer (ثابت قلم_Variation_Store *varStore_,
			 ثابت Delta_مجموعه_شاخص_Map *varIdxMap,
			 hb_array_t<ثابت صحيح> coords,
			 hb_scalar_cache_t *cache = اشاره_تهی) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(قلم_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    إذا (!hb_any (coords))
      هذا->coords = {};
  }

  عملگر منطقي () ثابت { أرجع varStore && منطقي (coords); }

  عائم عملگر[] (uint32_t varIdx) ثابت
  { أرجع (*هذا) (varIdx); }

  عائم عملگر() (uint32_t varIdx, بیعلامت کوتاه جابه_جایی = 0) ثابت
  { أرجع get_delta (varIdx, جابه_جایی); }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع get_delta<مزدوج> (varIdx); }

  قالب <نام_نوع Float = عائم>
  Float get_delta (uint32_t varIdx, بیعلامت کوتاه جابه_جایی = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   إذا (!coords || varIdx == VarIdx::NO_VARIATION)
     أرجع Float (0);

    varIdx += جابه_جایی;
    إذا (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    أرجع varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت قلم_Variation_Store *varStore;
  ثابت Delta_مجموعه_شاخص_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};

بنية Multi_قلم_Var_Store_Instancer
{
  Multi_قلم_Var_Store_Instancer (ثابت Multi_قلم_Variation_Store *varStore,
			      ثابت Delta_مجموعه_شاخص_Map *varIdxMap,
			      hb_array_t<ثابت صحيح> coords,
			      hb_scalar_cache_t *cache = اشاره_تهی) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(Multi_قلم_Variation_Store);
  }

  عملگر منطقي () ثابت { أرجع varStore; }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع (مزدوج) (*هذا)[varIdx]; }

  مزدوج عملگر[] (uint32_t varIdx) ثابت
  {
    مزدوج v = 0;
    (*هذا) (hb_array (&v, 1), varIdx);
    أرجع v;
  }

  فراغ عملگر() (hb_array_t<مزدوج> بیرون, uint32_t varIdx, بیعلامت کوتاه جابه_جایی = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    إذا (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += جابه_جایی;
      إذا (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, بیرون, cache);
    }
    وإلا
      لكل (بیعلامت i = 0; i < بیرون.درازی; i++)
        بیرون.arrayZ[i] = 0.;
  }

  ثابت Multi_قلم_Variation_Store *varStore;
  ثابت Delta_مجموعه_شاخص_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
تعداد Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_رکورد_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_رکورد_WITH_VAR = 3,
};

تعداد lookup_condition_subset_نتیجه_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

بنية Condition;

قالب <نام_نوع Instancer>
ساكن منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بیعلامت صحيح coord_len,
				Instancer *instancer);

بنية ConditionAxisRange
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    ثابت hb_map_t *شاخص_map = &c->plan->axes_شاخص_map;
    إذا (شاخص_map->is_empty ()) return_trace (درست);

    ثابت hb_map_t& axes_old_شاخص_برچسب_map = c->plan->axes_old_شاخص_برچسب_map;
    hb_codepoint_t *axis_برچسب_2;
    إذا (!axes_old_شاخص_برچسب_map.has (axis_شاخص_2, &axis_برچسب_2) ||
        !شاخص_map->has (axis_شاخص_2))
      return_trace (خطأ);

    ثابت hb_hashmap_t<hb_برچسب_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_حد{-1.0, 0.0, 1.0};
    Triple *normalized_حد;
    إذا (normalized_axes_location.has (*axis_برچسب_2, &normalized_حد))
      axis_حد = *normalized_حد;

    ثابت hb_hashmap_t<hb_برچسب_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    إذا (axes_triple_distances.has (*axis_برچسب_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    عائم normalized_min = renormalize_مقدار ((مزدوج) filter_Range_Min_مقدار.مقصد_float (), axis_حد, axis_triple_distances, خطأ);
    عائم normalized_max = renormalize_مقدار ((مزدوج) filter_Range_Max_مقدار.مقصد_float (), axis_حد, axis_triple_distances, خطأ);
    بیرون->filter_Range_Min_مقدار.مجموعه_float (normalized_min);
    بیرون->filter_Range_Max_مقدار.مجموعه_float (normalized_max);

    return_trace (c->serializer->check_تخصیص_بده (بیرون->axis_شاخص_2, شاخص_map->get (axis_شاخص_2),
                                               HB_SERIALIZE_ERROR_INT_سرریز));
  }

  خاص:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*بیرون*/) ثابت
  {
    //invalid axis index, drop the entire record
    إذا (!c->axes_شاخص_برچسب_map->has (axis_شاخص_2))
      أرجع DROP_رکورد_WITH_VAR;

    hb_برچسب_t axis_برچسب_2 = c->axes_شاخص_برچسب_map->get (axis_شاخص_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_حد;
    منطقي axis_مجموعه_by_user = خطأ;
    إذا (c->axes_location->has (axis_برچسب_2, &axis_حد))
    {
      axis_range = *axis_حد;
      axis_مجموعه_by_user = درست;
    }

    عائم axis_min_val = axis_range.minimum;
    عائم axis_default_val = axis_range.میانه;
    عائم axis_max_val = axis_range.بیشینه;

    عائم filter_min_val = filter_Range_Min_مقدار.مقصد_float ();
    عائم filter_max_val = filter_Range_Max_مقدار.مقصد_float ();

    إذا (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = خطأ;

    //condition not met, drop the entire record
    إذا (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      أرجع DROP_رکورد_WITH_VAR;

    //condition met and axis pinned, drop the condition
    إذا (axis_مجموعه_by_user && axis_range.is_نقطه ())
      أرجع DROP_COND_WITH_VAR;

    إذا (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_مقدار.مقصد_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_مقدار.مقصد_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->مجموعه (axis_شاخص_2, val);
      أرجع KEEP_COND_WITH_VAR;
    }
    أرجع KEEP_رکورد_WITH_VAR;
  }

  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		 Instancer *instancer HB_استفادهنشده) ثابت
  {
    صحيح coord = axis_شاخص_2 < coord_len ? coords[axis_شاخص_2] : 0;
    أرجع filter_Range_Min_مقدار.مقصد_int () <= coord && coord <= filter_Range_Max_مقدار.مقصد_int ();
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
  F2DOT14	filter_Range_Min_مقدار;
  F2DOT14	filter_Range_Max_مقدار;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Condition_مقدار
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    إذا (varIdx == VarIdx::NO_VARIATION)
      return_trace (درست);

    hb_pair_t<بیعلامت, صحيح> *condition_مقدار;
    إذا (c->plan->layout_condition_idx_مقدار_map.has ((uintptr_t) هذا, &condition_مقدار))
    {
      بیرون->default_مقدار_2 = condition_مقدار->second;
      بیرون->varIdx = condition_مقدار->اولی;
      return_trace (درست);
    }

    hb_pair_t<بیعلامت, صحيح> *new_varidx_delta;
    إذا (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (خطأ);

    إذا (unlikely (!c->serializer->check_تخصیص_بده (
		بیرون->default_مقدار_2,
		default_مقدار_2 + hb_دومی (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_سرریز)))
      return_trace (خطأ);
    بیرون->varIdx = hb_اولی (*new_varidx_delta);
    return_trace (درست);
  }

  خاص:
  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    مزدوج value = default_مقدار_2;
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
  HBINT16	default_مقدار_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية ConditionAnd
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
      return_trace (خطأ);
    بیرون->format = format;

    لكل (ثابت خودکار &جابه_جایی : conditions)
    {
      إذا (جابه_جایی.is_null ())
      {
	خودکار *بیرون_جابه_جایی = بیرون->conditions.serialize_append (c->serializer);
	إذا (unlikely (!بیرون_جابه_جایی)) return_trace (خطأ);
	*بیرون_جابه_جایی = 0;
      }
      وإلا إذا (unlikely (!subset_جابه_جایی_array (c, بیرون->conditions, هذا) (جابه_جایی)))
	return_trace (خطأ);
    }

    return_trace (درست);
  }

  خاص:
  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بیعلامت صحيح شمار = conditions.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	أرجع خطأ;
    أرجع درست;
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8_از_Offset24_مقصد<بنية Condition>	conditions;
  عام:
  DEFINE_اندازه_ARRAY (3, conditions);
};

بنية ConditionOr
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
      return_trace (خطأ);
    بیرون->format = format;

    لكل (ثابت خودکار &جابه_جایی : conditions)
    {
      إذا (جابه_جایی.is_null ())
      {
	خودکار *بیرون_جابه_جایی = بیرون->conditions.serialize_append (c->serializer);
	إذا (unlikely (!بیرون_جابه_جایی)) return_trace (خطأ);
	*بیرون_جابه_جایی = 0;
      }
      وإلا إذا (unlikely (!subset_جابه_جایی_array (c, بیرون->conditions, هذا) (جابه_جایی)))
	return_trace (خطأ);
    }

    return_trace (درست);
  }

  خاص:
  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بیعلامت صحيح شمار = conditions.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	أرجع درست;
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
  Array8_از_Offset24_مقصد<بنية Condition>	conditions;
  عام:
  DEFINE_اندازه_ARRAY (3, conditions);
};

بنية ConditionNegate
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);
    إذا (condition.is_null ())
    {
      بیرون->condition = 0;
      return_trace (درست);
    }
    return_trace (بیرون->condition.serialize_subset (c, condition, هذا));
  }

  خاص:
  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
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
  Offset24_مقصد<بنية Condition>	condition;
  عام:
  DEFINE_اندازه_STATIC (5);
};

بنية Condition
{
  دوست بنية Condition_مجموعه;

  ساكن منطقي serialize_constant (hb_serialize_context_t *c, منطقي value)
  {
    خودکار *بیرون = c->شروع_embed<Condition_مقدار> ();
    إذا (unlikely (!بیرون || !c->extend_min (بیرون))) أرجع خطأ;
    بیرون->format = 2;
    بیرون->default_مقدار_2 = value ? 1 : 0;
    بیرون->varIdx = VarIdx::NO_VARIATION;
    أرجع درست;
  }

  lookup_condition_subset_نتیجه_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      بیعلامت عمق = HB_MAX_NESTING_LEVEL,
				      منطقي materialize_true = خطأ) ثابت;

  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بدل (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    حالة 0: أرجع درست;
    حالة 1: hb_barrier (); أرجع u.format1.evaluate (coords, coord_len, instancer);
    حالة 2: hb_barrier (); أرجع u.format2.evaluate (coords, coord_len, instancer);
    حالة 3: hb_barrier (); أرجع u.format3.evaluate (coords, coord_len, instancer);
    حالة 4: hb_barrier (); أرجع u.format4.evaluate (coords, coord_len, instancer);
    حالة 5: hb_barrier (); أرجع u.format5.evaluate (coords, coord_len, instancer);
    افتراضي:أرجع خطأ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*بیرون*/,
                                             بیعلامت عمق = HB_MAX_NESTING_LEVEL) ثابت
  {
    إذا (unlikely (!عمق))
    {
      c->apply = خطأ;
      أرجع DROP_رکورد_WITH_VAR;
    }

    بدل (u.format.v) {
    حالة 0: أرجع KEEP_رکورد_WITH_VAR;
    حالة 1: hb_barrier (); أرجع u.format1.keep_with_variations (c, condition_map);
    حالة 2: حالة 3: حالة 4: حالة 5: اكسر;
    افتراضي: c->apply = خطأ; أرجع DROP_رکورد_WITH_VAR;
    }

    hb_barrier ();
    منطقي applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.درازی, c->instancer);
    c->apply &= applies;
    إذا (c->all_axes_pinned)
      أرجع applies ? DROP_COND_WITH_VAR : DROP_رکورد_WITH_VAR;

    إذا (u.format.v == 2)
      أرجع u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_مجموعه (u.format2.varIdx) ?
             (applies ? KEEP_رکورد_WITH_VAR : DROP_رکورد_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    خودکار classify_فرزند = [c, condition_map, عمق] (ثابت Condition &فرزند)
    {
      خودکار فرزند_context = *c;
      فرزند_context.apply = درست;
      أرجع فرزند.keep_with_variations (&فرزند_context, condition_map, عمق - 1);
    };

    إذا (u.format.v == 5)
    {
      خودکار نتیجه = classify_فرزند (&u.format5 + u.format5.condition);
      أرجع نتیجه == DROP_رکورد_WITH_VAR ? KEEP_رکورد_WITH_VAR :
             نتیجه == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_رکورد_WITH_VAR;
    }

    منطقي is_and = u.format.v == 3;
    منطقي variable = خطأ;
    ثابت خودکار &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت فراغ *پایه = is_and ? (ثابت فراغ *) &u.format3 : (ثابت فراغ *) &u.format4;
    لكل (ثابت خودکار &جابه_جایی : conditions)
    {
      خودکار نتیجه = classify_فرزند (پایه + جابه_جایی);
      إذا (is_and && نتیجه == DROP_رکورد_WITH_VAR)
        أرجع DROP_رکورد_WITH_VAR;
      إذا (!is_and && (نتیجه == DROP_COND_WITH_VAR || نتیجه == KEEP_رکورد_WITH_VAR))
        أرجع KEEP_رکورد_WITH_VAR;
      variable |= نتیجه == KEEP_COND_WITH_VAR;
    }
    أرجع variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_رکورد_WITH_VAR : DROP_رکورد_WITH_VAR;
  }

  قالب <نام_نوع context_t, نام_نوع ...Ts>
  نام_نوع context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    إذا (unlikely (!c->may_dispatch (هذا, &u.format.v))) أرجع c->no_dispatch_return_مقدار ();
    TRACE_DISPATCH (هذا, u.format.v);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالة 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالة 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالة 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالة 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    افتراضي:return_trace (c->default_return_مقدار ());
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_شروع_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_مقدار ());

    return_trace (c->انتها_recursion (هذا->dispatch (c)));
  }

  منطقي collect_var_indices (hb_مجموعه_t *var_indices,
			    بیعلامت عمق = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<بیعلامت, صحيح>> *condition_مقادیر = اشاره_تهی) ثابت;

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = اشاره_تهی);

  خاص:
  lookup_condition_subset_نتیجه_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      بیعلامت عمق) ثابت;

  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_مقدار	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  عام:
  DEFINE_اندازه_UNION (2, format.v);
};

قالب <نام_نوع Instancer>
منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بیعلامت صحيح coord_len,
				Instancer *instancer)
{
  أرجع condition.evaluate (coords, coord_len, instancer);
}

بنية ConditionList
{
  ثابت Condition& عملگر[] (بیعلامت i) ثابت
  { أرجع هذا+conditions[i]; }

  بیعلامت get_شمار () ثابت { أرجع conditions.len; }

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
    لكل (بیعلامت i = 0; i < conditions.len; i++)
    {
      بیعلامت old_شاخص = condition_map.backward (i);
      إذا (unlikely (old_شاخص >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_شاخص]), varidx_map, axis_map)))
	return_trace (خطأ);
    }
    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  Array32_از_Offset32_مقصد<Condition> conditions;
  عام:
  DEFINE_اندازه_ARRAY (4, conditions);
};

مضمن منطقي
Condition::collect_var_indices (hb_مجموعه_t *var_indices, بیعلامت عمق,
                               hb_hashmap_t<uintptr_t, hb_pair_t<بیعلامت, صحيح>> *condition_مقادیر) ثابت
{
  إذا (unlikely (!عمق)) أرجع خطأ;
  بدل (u.format.v)
  {
    حالة 0:
    حالة 1:
      أرجع درست;
    حالة 2:
      إذا (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->جمع_کن (u.format2.varIdx);
        إذا (condition_مقادیر &&
            !condition_مقادیر->مجموعه ((uintptr_t) &u.format2,
                                    hb_pair (بیعلامت (u.format2.varIdx), صحيح (u.format2.default_مقدار_2))))
          أرجع خطأ;
      }
      أرجع !var_indices->درون_error ();
    حالة 3:
      لكل (ثابت خودکار &جابه_جایی : u.format3.conditions)
	إذا (unlikely (!(&u.format3+جابه_جایی).collect_var_indices (var_indices,
							     عمق - 1, condition_مقادیر)))
	  أرجع خطأ;
      أرجع درست;
    حالة 4:
      لكل (ثابت خودکار &جابه_جایی : u.format4.conditions)
	إذا (unlikely (!(&u.format4+جابه_جایی).collect_var_indices (var_indices,
							     عمق - 1, condition_مقادیر)))
	  أرجع خطأ;
      أرجع درست;
    حالة 5:
      أرجع (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  عمق - 1, condition_مقادیر);
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
      خودکار *بیرون = c->شروع_embed<Condition_مقدار> ();
      إذا (unlikely (!بیرون || !c->extend_min (بیرون))) return_trace (خطأ);
      بیرون->format = 2;
      بیرون->default_مقدار_2 = 1;
      بیرون->varIdx = VarIdx::NO_VARIATION;
      return_trace (درست);
    }
    حالة 1:
    {
      خودکار *بیرون = c->embed (&src->u.format1);
      إذا (unlikely (!بیرون)) return_trace (خطأ);
      إذا (axis_map)
      {
	إذا (unlikely (!axis_map->has (بیرون->axis_شاخص_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (خطأ);
	}
	إذا (unlikely (!c->check_تخصیص_بده (بیرون->axis_شاخص_2, axis_map->get (بیرون->axis_شاخص_2),
				      HB_SERIALIZE_ERROR_INT_سرریز)))
	  return_trace (خطأ);
      }
      return_trace (درست);
    }
    حالة 2:
    {
      خودکار *بیرون = c->embed (&src->u.format2);
      إذا (unlikely (!بیرون ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (خطأ);
      بیرون->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (درست);
    }
    حالة 3:
    {
      خودکار *بیرون = c->شروع_embed<ConditionAnd> ();
      إذا (unlikely (!c->extend_min (بیرون))) return_trace (خطأ);
      بیرون->format = 3;
      بیرون->conditions.len = src->u.format3.conditions.len;
      إذا (unlikely (!c->extend (بیرون->conditions))) return_trace (خطأ);
      لكل (بیعلامت i = 0; i < بیرون->conditions.len; i++)
	إذا (unlikely (!بیرون->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (درست);
    }
    حالة 4:
    {
      خودکار *بیرون = c->شروع_embed<ConditionOr> ();
      إذا (unlikely (!c->extend_min (بیرون))) return_trace (خطأ);
      بیرون->format = 4;
      بیرون->conditions.len = src->u.format4.conditions.len;
      إذا (unlikely (!c->extend (بیرون->conditions))) return_trace (خطأ);
      لكل (بیعلامت i = 0; i < بیرون->conditions.len; i++)
	إذا (unlikely (!بیرون->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (درست);
    }
    حالة 5:
    {
      خودکار *بیرون = c->embed (&src->u.format5);
      إذا (unlikely (!بیرون ||
		    !بیرون->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (خطأ);
      return_trace (درست);
    }
    افتراضي:
      return_trace (خطأ);
  }
}

مضمن lookup_condition_subset_نتیجه_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    بیعلامت عمق,
				    منطقي materialize_true) ثابت
{
  خودکار snap = c->serializer->snapshot ();
  lookup_condition_subset_نتیجه_t نتیجه =
      subset_lookup_condition_impl (c, عمق);
  إذا (نتیجه == LOOKUP_CONDITION_SUBSET_KEEP)
    أرجع نتیجه;

  c->serializer->revert (snap);
  إذا (نتیجه != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    أرجع نتیجه;

  أرجع serialize_constant (c->serializer, درست) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

مضمن lookup_condition_subset_نتیجه_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 بیعلامت عمق) ثابت
{
  إذا (unlikely (!عمق))
    أرجع LOOKUP_CONDITION_SUBSET_ERROR;

  خودکار subset_جابه_جایی = [c, عمق] (Array8_از_Offset24_مقصد<Condition> &بیرون,
				   ثابت Condition &condition)
      -> lookup_condition_subset_نتیجه_t
  {
    خودکار snap = c->serializer->snapshot ();
    خودکار *جابه_جایی = بیرون.serialize_append (c->serializer);
    إذا (unlikely (!جابه_جایی))
      أرجع LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_نتیجه_t نتیجه =
        condition.subset_lookup_condition (c, عمق - 1);
    إذا (نتیجه == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->جمع_کن_link (*جابه_جایی, c->serializer->برداشت_pack ());
    وإلا
    {
      c->serializer->برداشت_discard ();
      بیرون.برداشت ();
      c->serializer->revert (snap);
    }
    أرجع نتیجه;
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

      hb_codepoint_t *axis_برچسب_2;
      إذا (!c->plan->axes_old_شاخص_برچسب_map.has (src.axis_شاخص_2, &axis_برچسب_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_حد {-1.f, 0.f, 1.f};
      Triple *normalized_حد;
      إذا (c->plan->axes_location.has (*axis_برچسب_2, &normalized_حد))
        axis_حد = *normalized_حد;

      عائم filter_min = src.filter_Range_Min_مقدار.مقصد_float ();
      عائم filter_max = src.filter_Range_Max_مقدار.مقصد_float ();
      إذا (filter_min > filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد.is_نقطه ())
        أرجع filter_min <= axis_حد.میانه &&
	       axis_حد.میانه <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد.بیشینه < filter_min ||
	  filter_max < axis_حد.minimum)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (filter_min <= axis_حد.minimum &&
	  axis_حد.بیشینه <= filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      إذا (!c->plan->axes_شاخص_map.has (src.axis_شاخص_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      خودکار *بیرون = c->serializer->embed (&src);
      إذا (unlikely (!بیرون))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      إذا (c->plan->axes_triple_distances.has (*axis_برچسب_2, &distances))
        axis_distances = *distances;

      بیرون->filter_Range_Min_مقدار.مجموعه_float (
	  renormalize_مقدار (filter_min, axis_حد, axis_distances, خطأ));
      بیرون->filter_Range_Max_مقدار.مجموعه_float (
	  renormalize_مقدار (filter_max, axis_حد, axis_distances, خطأ));
      إذا (unlikely (!c->serializer->check_تخصیص_بده (
		بیرون->axis_شاخص_2,
		c->plan->axes_شاخص_map.get (src.axis_شاخص_2),
		HB_SERIALIZE_ERROR_INT_سرریز)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 2:
    {
      ثابت Condition_مقدار &src = u.format2;
      خودکار *بیرون = c->serializer->embed (&src);
      إذا (unlikely (!بیرون))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      إذا (src.varIdx == VarIdx::NO_VARIATION)
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_مقدار_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<بیعلامت, صحيح> *condition_مقدار;
      إذا (c->plan->layout_condition_idx_مقدار_map.has ((uintptr_t) &src, &condition_مقدار))
      {
        بیرون->default_مقدار_2 = condition_مقدار->second;
        بیرون->varIdx = condition_مقدار->اولی;
        إذا (بیرون->varIdx == VarIdx::NO_VARIATION)
          أرجع بیرون->default_مقدار_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        أرجع LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<بیعلامت, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_مقدار_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      إذا (!c->plan->user_axes_location.is_empty () &&
          hb_اولی (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        مزدوج value = (صحيح) src.default_مقدار_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        أرجع value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحيح default_مقدار = (صحيح) src.default_مقدار_2 +
			  hb_دومی (*new_varidx_delta);
      إذا (unlikely (!c->serializer->check_تخصیص_بده (
		بیرون->default_مقدار_2,
		default_مقدار,
		HB_SERIALIZE_ERROR_INT_سرریز)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بیرون->varIdx = hb_اولی (*new_varidx_delta);

      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 3:
    {
      ثابت ConditionAnd &src = u.format3;
      خودکار *بیرون = c->serializer->شروع_embed (&src);
      إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بیرون->format = 3;

      لكل (ثابت خودکار &جابه_جایی : src.conditions)
      {
        lookup_condition_subset_نتیجه_t نتیجه =
            subset_جابه_جایی (بیرون->conditions, &src + جابه_جایی);
        إذا (نتیجه == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتیجه == LOOKUP_CONDITION_SUBSET_FALSE)
          أرجع نتیجه;
      }
      أرجع بیرون->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالة 4:
    {
      ثابت ConditionOr &src = u.format4;
      خودکار *بیرون = c->serializer->شروع_embed (&src);
      إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بیرون->format = 4;

      لكل (ثابت خودکار &جابه_جایی : src.conditions)
      {
        lookup_condition_subset_نتیجه_t نتیجه =
            subset_جابه_جایی (بیرون->conditions, &src + جابه_جایی);
        إذا (نتیجه == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتیجه == LOOKUP_CONDITION_SUBSET_TRUE)
          أرجع نتیجه;
      }
      أرجع بیرون->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالة 5:
    {
      ثابت ConditionNegate &src = u.format5;
      خودکار *بیرون = c->serializer->embed (&src);
      إذا (unlikely (!بیرون))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      بیرون->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_نتیجه_t نتیجه =
          (&src + src.condition).subset_lookup_condition (c, عمق - 1);
      إذا (نتیجه == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->جمع_کن_link (بیرون->condition,
				 c->serializer->برداشت_pack ());
        أرجع نتیجه;
      }

      c->serializer->برداشت_discard ();
      إذا (نتیجه == LOOKUP_CONDITION_SUBSET_TRUE)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (نتیجه == LOOKUP_CONDITION_SUBSET_FALSE)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      أرجع نتیجه;
    }

    افتراضي:
      أرجع LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

بنية Condition_مجموعه
{
  منطقي evaluate (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		 قلم_Var_Store_Instancer *instancer) ثابت
  {
    بیعلامت صحيح شمار = conditions.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
      إذا (!(هذا+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	أرجع خطأ;
    أرجع درست;
  }

  فراغ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    إذا (unlikely (!condition_map)) أرجع;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_مجموعه_t *cond_مجموعه = hb_مجموعه_create ();
    إذا (unlikely (!cond_مجموعه)) أرجع;
    hb::shared_ptr<hb_مجموعه_t> s {cond_مجموعه};

    c->apply = درست;
    منطقي should_keep = خطأ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقي can_deduplicate = درست;
    بیعلامت رقم_kept_cond = 0, cond_idx = 0;
    لكل (ثابت خودکار& جابه_جایی : conditions)
    {
      ثابت Condition &condition = هذا+جابه_جایی;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      إذا (ret == DROP_رکورد_WITH_VAR)
      {
        c->apply = خطأ;
        أرجع;
      }

      إذا (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = درست;
        cond_مجموعه->جمع_کن (cond_idx);
        رقم_kept_cond++;
      }

      إذا (ret == KEEP_رکورد_WITH_VAR)
        should_keep = درست;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = رقم_kept_cond == 0;
    إذا (!should_keep && c->all_axes_pinned) أرجع;

    //check if condition_set is unique with variations
    إذا (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      أرجع;

    إذا (can_deduplicate)
      c->conditionset_map->مجموعه (p, 1);
    c->رکورد_cond_idx_map->مجموعه (c->cur_رکورد_idx, s);
  }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقي درج_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    إذا (درج_catch_all) return_trace (درست);

    hb_مجموعه_t *retained_cond_مجموعه = اشاره_تهی;
    إذا (l->feature_رکورد_cond_idx_map != اشاره_تهی)
      retained_cond_مجموعه = l->feature_رکورد_cond_idx_map->get (l->cur_feature_var_رکورد_idx);

    بیعلامت صحيح شمار = conditions.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (retained_cond_مجموعه != اشاره_تهی && !retained_cond_مجموعه->has (i))
        استمر;
      ثابت Condition &condition = هذا+conditions[i];
      إذا (condition.u.format.v == 1)
      {
        subset_جابه_جایی_array (c, بیرون->conditions, هذا) (conditions[i]);
        استمر;
      }

      خودکار *جابه_جایی = بیرون->conditions.serialize_append (c->serializer);
      إذا (unlikely (!جابه_جایی)) return_trace (خطأ);
      c->serializer->push ();
      خودکار نتیجه = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, درست);
      إذا (نتیجه == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        نتیجه = Condition::serialize_constant (c->serializer, خطأ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      إذا (unlikely (نتیجه == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->برداشت_discard ();
        c->serializer->check_success (خطأ);
        return_trace (خطأ);
      }
      c->serializer->جمع_کن_link (*جابه_جایی, c->serializer->برداشت_pack ());
    }

    return_trace (منطقي (بیرون->conditions));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &جابه_جایی : conditions)
      إذا (unlikely (!(هذا+جابه_جایی).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_مقادیر)))
	c->layout_variation_indices->err ();
  }

  محمي:
  Array16_از_Offset32_مقصد<Condition>	conditions;
  عام:
  DEFINE_اندازه_ARRAY (2, conditions);
};

بنية Feature_جدول_Substitution_رکورد
{
  دوست بنية Feature_جدول_Substitution;

  فراغ collect_lookups (ثابت فراغ *پایه, hb_مجموعه_t *lookup_indexes /*بیرون*/) ثابت
  {
    أرجع (پایه+feature).جمع_کن_lookup_indexes_مقصد (lookup_indexes);
  }

  فراغ بستگی_features (ثابت فراغ *پایه,
			 ثابت hb_map_t *lookup_indexes,
			 hb_مجموعه_t       *feature_indexes /*بیرون*/) ثابت
  {
    إذا ((پایه+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جمع_کن (feature_شاخص_2);
  }

  فراغ collect_feature_substitutes_with_variations (hb_hashmap_t<بیعلامت, ثابت Feature*> *feature_substitutes_map,
                                                    hb_مجموعه_t& catch_all_رکورد_feature_idxes,
                                                    ثابت hb_مجموعه_t *feature_indices,
                                                    ثابت فراغ *پایه) ثابت
  {
    إذا (feature_indices->has (feature_شاخص_2))
    {
      feature_substitutes_map->مجموعه (feature_شاخص_2, &(پایه+feature));
      catch_all_رکورد_feature_idxes.جمع_کن (feature_شاخص_2);
    }
  }

  منطقي serialize (hb_subset_layout_context_t *c,
                  بیعلامت feature_شاخص,
                  ثابت Feature *f, ثابت برچسب *برچسب_2)
  {
    TRACE_SERIALIZE (هذا);
    hb_serialize_context_t *s = c->subset_context->serializer;
    إذا (unlikely (!s->extend_min (هذا))) return_trace (خطأ);

    uint32_t *new_feature_idx;
    إذا (!c->feature_map_w_duplicates->has (feature_شاخص, &new_feature_idx))
      return_trace (خطأ);

    إذا (!s->check_تخصیص_بده (feature_شاخص_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (خطأ);

    s->push ();
    منطقي ret = f->subset (c->subset_context, c, برچسب_2);
    إذا (ret) s->جمع_کن_link (feature, s->برداشت_pack ());
    وإلا s->برداشت_discard ();

    return_trace (ret);
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_شاخص;
    إذا (!c->feature_map_w_duplicates->has (feature_شاخص_2, &new_feature_شاخص))
      return_trace (خطأ);

    خودکار *بیرون = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->feature_شاخص_2 = *new_feature_شاخص;
    hb_pair_t<ثابت فراغ*, ثابت فراغ*> *original;
    إذا (!c->feature_idx_برچسب_map->has (feature_شاخص_2, &original))
      return_trace (خطأ);
    ثابت برچسب *برچسب_2 = reinterpret_cast<ثابت برچسب*> (original->second);
    return_trace (بیرون->feature.serialize_subset (c->subset_context, feature, پایه, c, برچسب_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت رکورد_sanitize_بستگی_t بستگی = {feature_list->get_برچسب (feature_شاخص_2), پایه};
    return_trace (feature.sanitize (c, پایه, &بستگی));
  }

  محمي:
  HBUINT16		feature_شاخص_2;
  Offset32_مقصد<Feature>	feature;
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Feature_جدول_Substitution
{
  فراغ collect_feature_indices (hb_مجموعه_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رکورد &رکورد : substitutions)
      feature_indices->جمع_کن (رکورد.feature_شاخص_2);
  }

  ثابت Feature *پیدا_کن_substitute (بیعلامت صحيح feature_شاخص) ثابت
  {
    بیعلامت صحيح شمار = substitutions.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      ثابت Feature_جدول_Substitution_رکورد &رکورد = substitutions.arrayZ[i];
      إذا (رکورد.feature_شاخص_2 == feature_شاخص)
	أرجع &(هذا+رکورد.feature);
    }
    أرجع اشاره_تهی;
  }

  فراغ collect_lookups (ثابت hb_مجموعه_t *feature_indexes,
			hb_مجموعه_t       *lookup_indexes /*بیرون*/) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_جدول_Substitution_رکورد::feature_شاخص_2)
    | hb_apply ([هذا, lookup_indexes] (ثابت Feature_جدول_Substitution_رکورد& r)
		{ r.collect_lookups (هذا, lookup_indexes); })
    ;
  }

  قالب <نام_نوع برچسب_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_شاخص_map,
                         برچسب_Getter get_برچسب,
                         hb_مجموعه_t *nameids_مقصد_retain) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رکورد &رکورد : substitutions)
      إذا (feature_شاخص_map->has (رکورد.feature_شاخص_2))
        (هذا+رکورد.feature).collect_name_ids (get_برچسب (رکورد.feature_شاخص_2), nameids_مقصد_retain);
  }

  فراغ بستگی_features (ثابت hb_map_t *lookup_indexes,
			 hb_مجموعه_t       *feature_indexes /*بیرون*/) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رکورد& رکورد : substitutions)
      رکورد.بستگی_features (هذا, lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت hb_map_t *feature_شاخص_map) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رکورد& رکورد : substitutions)
    {
      إذا (feature_شاخص_map->has (رکورد.feature_شاخص_2)) أرجع درست;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رکورد& رکورد : substitutions)
      رکورد.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_رکورد_feature_idxes,
                                                          c->feature_indices, هذا);
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقي درج_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->نسخه.major = نسخه.major;
    بیرون->نسخه.minor = نسخه.minor;

    إذا (درج_catch_all)
    {
      لكل (بیعلامت feature_شاخص : *(l->catch_all_رکورد_feature_idxes))
      {
        hb_pair_t<ثابت فراغ*, ثابت فراغ*> *p;
        إذا (!l->feature_idx_برچسب_map->has (feature_شاخص, &p))
          return_trace (خطأ);
        خودکار *o = بیرون->substitutions.serialize_append (c->serializer);
        إذا (!o->serialize (l, feature_شاخص,
                           reinterpret_cast<ثابت Feature*> (p->اولی),
                           reinterpret_cast<ثابت برچسب*> (p->second)))
          return_trace (خطأ);
      }
      return_trace (درست);
    }

    + substitutions.iter ()
    | hb_apply (subset_رکورد_array (l, &(بیرون->substitutions), هذا))
    ;

    return_trace (منطقي (بیرون->substitutions));
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
  Array16_از<Feature_جدول_Substitution_رکورد>
			substitutions;
  عام:
  DEFINE_اندازه_ARRAY (6, substitutions);
};

بنية Feature_Variation_رکورد
{
  دوست بنية FeatureVariations;

  فراغ collect_lookups (ثابت فراغ     *پایه,
			ثابت hb_مجموعه_t *feature_indexes,
			hb_مجموعه_t       *lookup_indexes /*بیرون*/) ثابت
  {
    أرجع (پایه+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  قالب <نام_نوع برچسب_Getter>
  فراغ collect_name_ids (ثابت فراغ *پایه,
                         ثابت hb_map_t *feature_شاخص_map,
                         برچسب_Getter get_برچسب,
                         hb_مجموعه_t *nameids_مقصد_retain) ثابت
  {
    (پایه+substitutions).collect_name_ids (feature_شاخص_map, get_برچسب, nameids_مقصد_retain);
  }

  فراغ بستگی_features (ثابت فراغ     *پایه,
			 ثابت hb_map_t *lookup_indexes,
			 hb_مجموعه_t       *feature_indexes /*بیرون*/) ثابت
  {
    (پایه+substitutions).بستگی_features (lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت فراغ *پایه, ثابت hb_map_t *feature_شاخص_map) ثابت
  {
    أرجع (پایه+substitutions).intersects_features (feature_شاخص_map);
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت فراغ *پایه) ثابت
  {
    (پایه+conditions).keep_with_variations (c);
    إذا (c->apply && !c->variation_applied)
    {
      (پایه+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = درست; // set variations only once
    }
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *پایه,
               منطقي درج_catch_all = خطأ) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    hb_مجموعه_t *retained_conditions = c->feature_رکورد_cond_idx_map ?
        c->feature_رکورد_cond_idx_map->get (c->cur_feature_var_رکورد_idx).get () : اشاره_تهی;
    إذا (!درج_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      بیرون->conditions = 0;
    وإلا
      بیرون->conditions.serialize_subset (c->subset_context, conditions, پایه, c, درج_catch_all);
    بیرون->substitutions.serialize_subset (c->subset_context, substitutions, پایه, c, درج_catch_all);

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, پایه) &&
		  substitutions.sanitize (c, پایه, feature_list));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *پایه) ثابت
  { (پایه+conditions).collect_variation_indices (c); }

  محمي:
  Offset32_مقصد<Condition_مجموعه>
			conditions;
  Offset32_مقصد<Feature_جدول_Substitution>
			substitutions;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Lookup_شاخص_List
{
  فراغ جمع_کن_lookup_indexes_مقصد (hb_مجموعه_t *lookup_indexes) ثابت
  { lookupIndices.جمع_کن_indexes_مقصد (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookupIndices.intersects (lookup_indexes); }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
      return_trace (خطأ);

    خودکار it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_شاخص_map)
    | hb_map (l->lookup_شاخص_map)
    ;
    بیرون->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقي (بیرون->lookupIndices));
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

بنية Lookup_Condition_رکورد
{
  بیعلامت get_condition_جابه_جایی (ثابت فراغ *پایه, ثابت فراغ *variations) ثابت
  { أرجع (ثابت حرف *) &(پایه+condition) - (ثابت حرف *) variations; }

  قالب <نام_نوع Instancer>
  منطقي evaluate (ثابت فراغ *پایه,
		 ثابت صحيح *coords,
		 بیعلامت صحيح coord_len,
		 Instancer *instancer) ثابت
  { أرجع (پایه+condition).evaluate (coords, coord_len, instancer); }

  منطقي evaluate (ثابت فراغ *پایه,
                 ثابت فراغ *variations,
                 hb_sorted_array_t<ثابت بیعلامت> conditions) ثابت
  {
    أرجع !condition || hb_bsearch (get_condition_جابه_جایی (پایه, variations),
                                     conditions.arrayZ, conditions.درازی,
                                     حجم (بیعلامت),
                                     _hb_cmp_operator<بیعلامت, بیعلامت>);
  }

  منطقي collect_condition (ثابت فراغ *پایه,
                          ثابت فراغ *variations,
                          ثابت صحيح *coords,
                          بیعلامت صحيح coord_len,
                          قلم_Var_Store_Instancer *instancer,
                          hb_مجموعه_t *دیدهشده,
                          hb_وکتور_t<بیعلامت> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    إذا (!condition) أرجع درست;

    بیعلامت جابه_جایی = get_condition_جابه_جایی (پایه, variations);
    إذا (دیدهشده->has (جابه_جایی)) أرجع درست;
    دیدهشده->جمع_کن (جابه_جایی);
    إذا (unlikely (دیدهشده->درون_error ())) أرجع خطأ;

    إذا (evaluate (پایه, coords, coord_len, instancer))
      conditions->push (جابه_جایی);
    أرجع !conditions->درون_error ();
  }

  فراغ جمع_کن_lookup_indexes_مقصد (ثابت فراغ *پایه,
			      hb_مجموعه_t *lookup_indexes) ثابت
  { (پایه+lookupIndices).جمع_کن_lookup_indexes_مقصد (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت فراغ *پایه,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع (پایه+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *پایه) ثابت
  {
    إذا (unlikely (!(پایه+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_مقادیر)))
      c->layout_variation_indices->err ();
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت فراغ *پایه) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    بیرون->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_نتیجه_t نتیجه =
        (پایه + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    درست);
    إذا (نتیجه != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->برداشت_discard ();
      return_trace (خطأ);
    }
    c->serializer->جمع_کن_link (بیرون->condition, c->serializer->برداشت_pack ());

    return_trace (بیرون->lookupIndices.serialize_subset (c, lookupIndices, پایه, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (condition.sanitize (c, پایه) &&
		  lookupIndices.sanitize (c, پایه));
  }

  محمي:
  Offset32_مقصد<Condition>		condition;
  Offset32_مقصد<Lookup_شاخص_List>	lookupIndices;
  عام:
  DEFINE_اندازه_STATIC (8);
};

بنية Feature_Lookups_جدول
{
  منطقي adds_default_lookups () ثابت { أرجع flags & 0x0001u; }

  منطقي collect_conditions (ثابت فراغ *variations,
                           ثابت صحيح *coords,
                           بیعلامت صحيح coord_len,
                           قلم_Var_Store_Instancer *instancer,
                           hb_مجموعه_t *دیدهشده,
                           hb_وکتور_t<بیعلامت> *conditions) ثابت
  {
    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
      إذا (unlikely (!رکورد.collect_condition (هذا, variations, coords, coord_len,
                                               instancer, دیدهشده, conditions)))
	أرجع خطأ;
    أرجع درست;
  }

  فراغ collect_lookups (ثابت فراغ *variations,
                       hb_sorted_array_t<ثابت بیعلامت> conditions,
                       hb_مجموعه_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
      إذا (رکورد.evaluate (هذا, variations, conditions))
	رکورد.جمع_کن_lookup_indexes_مقصد (هذا, lookup_indexes);
  }

  قالب <نام_نوع Instancer>
  فراغ collect_lookups (ثابت صحيح *coords,
			بیعلامت صحيح coord_len,
			Instancer *instancer,
			hb_مجموعه_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
      إذا (رکورد.evaluate (هذا, coords, coord_len, instancer))
	رکورد.جمع_کن_lookup_indexes_مقصد (هذا, lookup_indexes);
  }

  فراغ collect_lookups (hb_مجموعه_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
      رکورد.جمع_کن_lookup_indexes_مقصد (هذا, lookup_indexes);
  }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
      إذا (رکورد.intersects_lookup_indexes (هذا, lookup_indexes))
	أرجع درست;
    أرجع خطأ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
      رکورد.collect_variation_indices (c, هذا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون)))
      return_trace (خطأ);

    بیرون->نسخه.major = نسخه.major;
    بیرون->نسخه.minor = نسخه.minor;
    بیرون->flags = flags;

    لكل (ثابت Lookup_Condition_رکورد &رکورد : رکوردها)
    {
      خودکار snap = c->serializer->snapshot ();
      إذا (رکورد.subset (c, l, هذا))
	بیرون->رکوردها.len++;
      وإلا
	c->serializer->revert (snap);
    }

    return_trace (درست);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  رکوردها.sanitize (c, هذا));
  }

  محمي:
  Fixed_نسخه<>	نسخه;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_از<Lookup_Condition_رکورد>
		رکوردها;
  عام:
  DEFINE_اندازه_ARRAY (10, رکوردها);
};

بنية Lookup_Variation_رکورد
{
  دوست بنية FeatureVariations;

  بیعلامت get_feature_شاخص () ثابت { أرجع feature_شاخص_2; }

  فراغ collect_lookups (ثابت فراغ *پایه,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت بیعلامت> conditions,
                       hb_مجموعه_t *lookup_indexes) ثابت
  {
    ثابت Feature_Lookups_جدول &feature_lookups = پایه+featureLookups;
    إذا (feature_lookups.adds_default_lookups ())
      feature.جمع_کن_lookup_indexes_مقصد (lookup_indexes);
    feature_lookups.collect_lookups (پایه, conditions, lookup_indexes);
  }

  فراغ collect_lookups (ثابت فراغ *پایه,
			ثابت hb_مجموعه_t *feature_indexes,
			hb_مجموعه_t *lookup_indexes) ثابت
  {
    إذا (feature_indexes->has (feature_شاخص_2))
      (پایه+featureLookups).collect_lookups (lookup_indexes);
  }

  فراغ بستگی_features (ثابت فراغ *پایه,
			 ثابت hb_map_t *lookup_indexes,
			 hb_مجموعه_t *feature_indexes) ثابت
  {
    إذا ((پایه+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جمع_کن (feature_شاخص_2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *پایه) ثابت
  { (پایه+featureLookups).collect_variation_indices (c); }

  منطقي subset (hb_subset_layout_context_t *l, ثابت فراغ *پایه) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_شاخص;
    إذا (!l->feature_map_w_duplicates->has (feature_شاخص_2, &new_feature_شاخص))
      return_trace (خطأ);

    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون ||
		  !c->serializer->check_تخصیص_بده (بیرون->feature_شاخص_2,
						*new_feature_شاخص,
						HB_SERIALIZE_ERROR_INT_سرریز)))
      return_trace (خطأ);

    return_trace (بیرون->featureLookups.serialize_subset (c, featureLookups, پایه, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureLookups.sanitize (c, پایه));
  }

  محمي:
  HBUINT16			feature_شاخص_2;
  Offset32_مقصد<Feature_Lookups_جدول>	featureLookups;
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية FeatureVariations
{
  ساكن constexpr بیعلامت NOT_یافت_شاخص = 0xFFFFFFFFu;

  ثابت Array32_از<Lookup_Variation_رکورد> &get_lookup_variation_رکوردها () ثابت
  {
    إذا (نسخه.مقصد_int () < 0x00010001u)
      أرجع Null (Array32_از<Lookup_Variation_رکورد>);
    أرجع StructAfter<Array32_از<Lookup_Variation_رکورد>> (var_رکوردها);
  }

  بیعلامت رکورد_شمار () ثابت
  {
    أرجع var_رکوردها.len;
  }

  منطقي پیدا_کن_شاخص (ثابت صحيح *coords, بیعلامت صحيح coord_len,
		   بیعلامت صحيح *شاخص_2,
		   قلم_Var_Store_Instancer *instancer) ثابت
  {
    بیعلامت صحيح شمار = var_رکوردها.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      ثابت Feature_Variation_رکورد &رکورد = var_رکوردها.arrayZ[i];
      إذا ((هذا+رکورد.conditions).evaluate (coords, coord_len, instancer))
      {
	*شاخص_2 = i;
	أرجع درست;
      }
    }
    *شاخص_2 = NOT_یافت_شاخص;
    أرجع خطأ;
  }

  ثابت Feature *پیدا_کن_substitute (بیعلامت صحيح variations_شاخص,
				  بیعلامت صحيح feature_شاخص) ثابت
  {
    ثابت Feature_Variation_رکورد &رکورد = var_رکوردها[variations_شاخص];
    أرجع (هذا+رکورد.substitutions).پیدا_کن_substitute (feature_شاخص);
  }

  منطقي collect_lookup_conditions (ثابت صحيح *coords,
                                  بیعلامت صحيح coord_len,
                                  قلم_Var_Store_Instancer *instancer,
                                  hb_وکتور_t<بیعلامت> *state) ثابت
  {
    hb_مجموعه_t دیدهشده_tables, دیدهشده_conditions;
    لكل (ثابت Lookup_Variation_رکورد &رکورد : get_lookup_variation_رکوردها ())
    {
      بیعلامت جابه_جایی = رکورد.featureLookups;
      إذا (دیدهشده_tables.has (جابه_جایی)) استمر;
      دیدهشده_tables.جمع_کن (جابه_جایی);
      إذا (unlikely (دیدهشده_tables.درون_error ())) أرجع خطأ;

      ثابت Feature_Lookups_جدول &feature_lookups = هذا+رکورد.featureLookups;
      إذا (unlikely (!feature_lookups.collect_conditions (هذا, coords, coord_len,
                                                         instancer, &دیدهشده_conditions,
                                                         state)))
	أرجع خطأ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<بیعلامت, بیعلامت>);
    أرجع درست;
  }

  منطقي get_lookup_variations (بیعلامت feature_شاخص,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت بیعلامت> conditions,
                              hb_مجموعه_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Variation_رکورد &رکورد : get_lookup_variation_رکوردها ())
    {
      إذا (رکورد.feature_شاخص_2 != feature_شاخص) استمر;

      رکورد.collect_lookups (هذا, feature, conditions, lookup_indexes);
      أرجع درست;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    بیعلامت صحيح شمار = var_رکوردها.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      c->cur_رکورد_idx = i;
      var_رکوردها[i].collect_feature_substitutes_with_variations (c, هذا);
      إذا (c->universal)
        اكسر;
    }
    إذا (c->universal || c->رکورد_cond_idx_map->is_empty ())
      c->catch_all_رکورد_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  فراغ collect_lookups (ثابت hb_مجموعه_t *feature_indexes,
			ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map,
			hb_مجموعه_t       *lookup_indexes /*بیرون*/) ثابت
  {
    بیعلامت شمار = var_رکوردها.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (feature_رکورد_cond_idx_map &&
          !feature_رکورد_cond_idx_map->has (i))
        استمر;
      var_رکوردها[i].collect_lookups (هذا, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  قالب <نام_نوع برچسب_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_شاخص_map,
                         ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map,
                         برچسب_Getter get_برچسب,
                         hb_مجموعه_t *nameids_مقصد_retain) ثابت
  {
    لكل (بیعلامت i = 0; i < var_رکوردها.len; i++)
    {
      إذا (feature_رکورد_cond_idx_map && !feature_رکورد_cond_idx_map->has (i))
        استمر;
      var_رکوردها[i].collect_name_ids (هذا, feature_شاخص_map, get_برچسب, nameids_مقصد_retain);
    }
  }

  فراغ collect_lookup_variation_lookups (ثابت hb_مجموعه_t *feature_indexes,
					 hb_مجموعه_t *lookup_indexes /*بیرون*/) ثابت
  {
    لكل (ثابت Lookup_Variation_رکورد &رکورد : get_lookup_variation_رکوردها ())
      رکورد.collect_lookups (هذا, feature_indexes, lookup_indexes);
  }

  فراغ بستگی_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<بیعلامت, hb::shared_ptr<hb_مجموعه_t>> *feature_رکورد_cond_idx_map,
			 hb_مجموعه_t       *feature_indexes /*بیرون*/) ثابت
  {
    بیعلامت صحيح شمار = var_رکوردها.len;
    لكل (بیعلامت صحيح i = 0; i < شمار; i++)
    {
      إذا (feature_رکورد_cond_idx_map != اشاره_تهی &&
          !feature_رکورد_cond_idx_map->has (i))
        استمر;
      var_رکوردها[i].بستگی_features (هذا, lookup_indexes, feature_indexes);
    }

    لكل (ثابت Lookup_Variation_رکورد &رکورد : get_lookup_variation_رکوردها ())
      رکورد.بستگی_features (هذا, lookup_indexes, feature_indexes);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Feature_Variation_رکورد &رکورد : var_رکوردها)
      رکورد.collect_variation_indices (c, هذا);
    لكل (ثابت Lookup_Variation_رکورد &رکورد : get_lookup_variation_رکوردها ())
      رکورد.collect_variation_indices (c, هذا);
  }

  فراغ collect_feature_indices (hb_مجموعه_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_Variation_رکورد &رکورد : var_رکوردها)
      (هذا+رکورد.substitutions).collect_feature_indices (feature_indices);
  }

  منطقي has_lookup_variations (بیعلامت feature_شاخص) ثابت
  {
    لكل (ثابت Lookup_Variation_رکورد &رکورد : get_lookup_variation_رکوردها ())
    {
      إذا (رکورد.feature_شاخص_2 == feature_شاخص)
	أرجع درست;
      إذا (رکورد.feature_شاخص_2 > feature_شاخص)
	اكسر;
    }
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!بیرون || !c->serializer->extend_min (بیرون))) return_trace (خطأ);

    بیرون->نسخه.major = نسخه.major;
    بیرون->نسخه.minor = نسخه.minor;

    صحيح keep_به_بالا_مقصد = -1;
    لكل (صحيح i = var_رکوردها.len - 1; i >= 0; i--) {
      إذا (var_رکوردها[i].intersects_features (هذا, l->feature_map_w_duplicates)) {
        keep_به_بالا_مقصد = i;
        اكسر;
      }
    }

    بیعلامت شمار = (بیعلامت) (keep_به_بالا_مقصد + 1);
    لكل (بیعلامت i = 0; i < شمار; i++)
    {
      إذا (l->feature_رکورد_cond_idx_map != اشاره_تهی &&
          !l->feature_رکورد_cond_idx_map->has (i))
        استمر;

      l->cur_feature_var_رکورد_idx = i;
      subset_رکورد_array (l, &(بیرون->var_رکوردها), هذا) (var_رکوردها[i]);
    }

    إذا (بیرون->var_رکوردها.len && !l->catch_all_رکورد_feature_idxes->is_empty ())
    {
      منطقي درج_catch_all_رکورد = درست;
      subset_رکورد_array (l, &(بیرون->var_رکوردها), هذا, درج_catch_all_رکورد) (var_رکوردها[0]);
    }

    Array32_از<Lookup_Variation_رکورد> *بیرون_lookup_رکوردها = اشاره_تهی;
    إذا (نسخه.minor >= 1)
    {
      بیرون_lookup_رکوردها = &StructAfter<Array32_از<Lookup_Variation_رکورد>> (بیرون->var_رکوردها);
      إذا (unlikely (!c->serializer->extend_min (بیرون_lookup_رکوردها)))
	return_trace (خطأ);

      + get_lookup_variation_رکوردها ().iter ()
      | hb_apply (subset_رکورد_array (l, بیرون_lookup_رکوردها, هذا))
      ;
    }

    return_trace (منطقي (بیرون->var_رکوردها) ||
		  (بیرون_lookup_رکوردها && منطقي (*بیرون_lookup_رکوردها)));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخه.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخه.major == 1) &&
		  var_رکوردها.sanitize (c, هذا, feature_list) &&
		  (نسخه.minor < 1 ||
		   get_lookup_variation_رکوردها ().sanitize (c, هذا)));
  }

  محمي:
  Fixed_نسخه<>	نسخه;	/* Version--0x00010000u */
  Array32_از<Feature_Variation_رکورد>
			var_رکوردها;
  عام:
  DEFINE_اندازه_ARRAY_SIZED (8, var_رکوردها);
};


/*
 * Device Tables
 */

بنية HintingDevice
{
  دوست بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->y_ppem, font->y_scale); }

  عام:

  size_t get_اندازه () ثابت
  {
    بیعلامت صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3 || شروع_اندازه > انتها_اندازه)) أرجع 3 * HBUINT16::static_اندازه;
    أرجع HBUINT16::static_اندازه * (4 + ((انتها_اندازه - شروع_اندازه) >> (4 - f)));
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

  صحيح get_delta (بیعلامت صحيح ppem, صحيح scale) ثابت
  {
    إذا (!ppem) أرجع 0;

    صحيح pixels = get_delta_pixels (ppem);

    إذا (!pixels) أرجع 0;

    أرجع (صحيح) (pixels * (int64_t) scale / ppem);
  }
  صحيح get_delta_pixels (بیعلامت صحيح ppem_اندازه) ثابت
  {
    بیعلامت صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3))
      أرجع 0;

    إذا (ppem_اندازه < شروع_اندازه || ppem_اندازه > انتها_اندازه)
      أرجع 0;

    بیعلامت صحيح s = ppem_اندازه - شروع_اندازه;

    بیعلامت صحيح byte = delta_مقدار_Z[s >> (4 - f)];
    بیعلامت صحيح بیتها = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    بیعلامت صحيح ماسک = (0xFFFFu >> (16 - (1 << f)));

    صحيح delta = بیتها & ماسک;

    إذا ((بیعلامت صحيح) delta >= ((ماسک + 1) >> 1))
      delta -= ماسک + 1;

    أرجع delta;
  }

  محمي:
  HBUINT16	شروع_اندازه;		/* Smallest size to correct--in ppem */
  HBUINT16	انتها_اندازه;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_از<HBUINT16>
		delta_مقدار_Z;		/* Array of compressed data */
  عام:
  DEFINE_اندازه_ARRAY (6, delta_مقدار_Z);
};

بنية VariationDevice
{
  دوست بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت قلم_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت قلم_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  عائم get_x_delta_f (hb_font_t *font,
		       ثابت قلم_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت قلم_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!layout_variation_idx_delta_map) return_trace (اشاره_تهی);

    hb_pair_t<بیعلامت, صحيح> *v;
    إذا (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (اشاره_تهی);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    إذا (hb_اولی (*v) == HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
      return_trace (اشاره_تهی);

    c->شروع_zerocopy (هذا->static_اندازه);
    خودکار *بیرون = c->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (اشاره_تهی);

    إذا (!c->check_تخصیص_بده (بیرون->varIdx, hb_اولی (*v), HB_SERIALIZE_ERROR_INT_سرریز))
      return_trace (اشاره_تهی);
    return_trace (بیرون);
  }

  فراغ collect_variation_شاخص (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->جمع_کن (varIdx); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:

  عائم get_delta (hb_font_t *font,
		   ثابت قلم_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    أرجع store.get_delta (varIdx, font->coords, font->رقم_coords, store_cache);
  }

  محمي:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عام:
  DEFINE_اندازه_STATIC (6);
};

بنية Device_سرآیند
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
		       ثابت قلم_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_x_delta_f (font, store, store_cache);
    }
#پایان_اگر
    أرجع get_x_delta (font, store, store_cache);
  }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت قلم_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
#اگر_تعریف_نشده HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_y_delta_f (font, store, store_cache);
    }
#پایان_اگر
    أرجع get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت قلم_Variation_Store &store=Null (قلم_Variation_Store),
			     hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    بدل (u.b.format)
    {
#اگر_تعریف_نشده HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      أرجع u.hinting.get_x_delta (font);
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_x_delta (font, store, store_cache);
#پایان_اگر
    افتراضي:
      أرجع 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت قلم_Variation_Store &store=Null (قلم_Variation_Store),
			     hb_scalar_cache_t *store_cache = اشاره_تهی) ثابت
  {
    بدل (u.b.format)
    {
    حالة 1: حالة 2: حالة 3:
#اگر_تعریف_نشده HB_NO_HINTING
      hb_barrier ();
      أرجع u.hinting.get_y_delta (font);
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_y_delta (font, store, store_cache);
#پایان_اگر
    افتراضي:
      أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.b.format.sanitize (c)) return_trace (خطأ);
    بدل (u.b.format) {
#اگر_تعریف_نشده HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#پایان_اگر
    افتراضي:
      return_trace (درست);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map=اشاره_تهی) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.b.format) {
#اگر_تعریف_نشده HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#پایان_اگر
    افتراضي:
      return_trace (اشاره_تهی);
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.b.format) {
#اگر_تعریف_نشده HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      أرجع;
#پایان_اگر
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      u.variation.collect_variation_شاخص (c);
      أرجع;
#پایان_اگر
    افتراضي:
      أرجع;
    }
  }

  بیعلامت get_variation_شاخص () ثابت
  {
    بدل (u.b.format) {
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.varIdx;
#پایان_اگر
    افتراضي:
      أرجع HB_OT_LAYOUT_NO_VARIATIONS_شاخص;
    }
  }

  منطقي is_variation_device () ثابت
  {
    بدل (u.b.format) {
#اگر_تعریف_نشده HB_NO_VAR
    حالة 0x8000:
      أرجع درست;
#پایان_اگر
    افتراضي:
      أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  Device_سرآیند		b;
  HintingDevice		hinting;
#اگر_تعریف_نشده HB_NO_VAR
  VariationDevice	variation;
#پایان_اگر
  } u;
  عام:
  DEFINE_اندازه_UNION (6, b);
};


} /* namespace OT */


#پایان_اگر /* HB_OT_LAYOUT_COMMON_HH */

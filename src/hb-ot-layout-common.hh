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

#اگر_غیر_تعریف_شدہ HB_OT_LAYOUT_COMMON_HH
#تعریف_کرو HB_OT_LAYOUT_COMMON_HH

#شامل "hb.hh"
#شامل "hb-ot-layout.hh"
#شامل "hb-open-type.hh"
#شامل "hb-set.hh"
#شامل "hb-bimap.hh"
#شامل "hb-cache.hh"
#شامل "hb-ot-dual.hh"

استعمال hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (حجم (hb_ot_layout_mapping_cache_t) == 512, "");

استعمال hb_ot_layout_ثنائی_cache_t = hb_cache_t<14, 1, 8>;
static_assert (حجم (hb_ot_layout_ثنائی_cache_t) == 256, "");

#شامل "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
استعمال OT::Layout::Common::Coverage;
استعمال OT::Layout::Common::Range_ریکارڈ;
استعمال OT::Layout::SmallTypes;
استعمال OT::Layout::MediumTypes;


نامخلا OT {

سانچہ<قسم_نام Iterator>
ساکن اندرونی منطقی ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ساکن منطقی ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_سیٹ_t &klasses,
    منطقی use_class_zero,
    hb_sorted_سمتیہ_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

ساخت شے_Var_Store_Instancer;

ساخت hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_اشاریہ_لیبل_map;
  ثابت hb_hashmap_t<hb_لیبل_t, Triple> *axes_location;
  hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *ریکارڈ_cond_idx_map;
  hb_hashmap_t<غیرعلامتی, ثابت Feature*> *feature_substitutes_map;
  hb_سیٹ_t& catch_all_ریکارڈ_feature_idxes;

  // not stored in subset_plan
  hb_سیٹ_t *feature_indices;
  منطقی apply;
  منطقی variation_applied;
  منطقی universal;
  غیرعلامتی cur_ریکارڈ_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, غیرعلامتی> *conditionset_map;
  شے_Var_Store_Instancer *instancer;
  منطقی all_axes_pinned;
};

ساخت hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت خالی         *جدول_2,
                              hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_سیٹ_t           *new_collected_feature_indexes_)
      :جدول (جدول_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_گنتی (0),langsys_feature_گنتی (0) {}

  منطقی دورہ_Script ()
  { واپس script_گنتی++ < HB_MAX_SCRIPTS; }

  منطقی دورہ_Langsys (غیرعلامتی feature_گنتی_2)
  {
    langsys_feature_گنتی += feature_گنتی_2;
    واپس langsys_feature_گنتی < HB_MAX_LANGSYS_FEATURE_گنتی;
  }

  عوامی:
  ثابت خالی *جدول;
  hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_سیٹ_t           *new_feature_indexes;

  نجی:
  غیرعلامتی script_گنتی;
  غیرعلامتی langsys_feature_گنتی;
};

ساخت hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_خالی_خانے_t, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { واپس "SUBSET_LAYOUT"; }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  منطقی دورہ_Script ()
  {
    واپس script_گنتی++ < HB_MAX_SCRIPTS;
  }

  منطقی دورہ_Lang_Sys ()
  {
    واپس langsys_گنتی++ < HB_MAX_LANGSYS;
  }

  منطقی دورہ_Feature_اشاریہ (صحیح گنتی)
  {
    feature_اشاریہ_گنتی += گنتی;
    واپس feature_اشاریہ_گنتی < HB_MAX_FEATURE_INDICES;
  }

  منطقی دورہ_Lookup_اشاریہ()
  {
    lookup_اشاریہ_گنتی++;
    واپس lookup_اشاریہ_گنتی < HB_MAX_LOOKUP_دورہ_گنتی;
  }

  hb_subset_context_t *subset_context;
  ثابت hb_لیبل_t جدول_لیبل_2;
  ثابت hb_map_t *lookup_اشاریہ_map;
  ثابت hb_hashmap_t<غیرعلامتی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map;
  ثابت hb_map_t *feature_اشاریہ_map;
  ثابت hb_map_t *feature_map_w_duplicates;
  ثابت hb_hashmap_t<غیرعلامتی, ثابت Feature*> *feature_substitutes_map;
  ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations;
  hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map;
  ثابت hb_سیٹ_t *catch_all_ریکارڈ_feature_idxes;
  ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<ثابت خالی*, ثابت خالی*>> *feature_idx_لیبل_map;

  غیرعلامتی cur_script_اشاریہ;
  غیرعلامتی cur_feature_اشاریہ;
  غیرعلامتی cur_feature_var_ریکارڈ_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_لیبل_t لیبل_4) :
				subset_context (c_),
				جدول_لیبل_2 (لیبل_4),
				cur_script_اشاریہ (0xFFFFu),
				cur_feature_اشاریہ (اشاریہ::NOT_ملا_اشاریہ),
				cur_feature_var_ریکارڈ_idx (0u),
				script_گنتی (0),
				langsys_گنتی (0),
				feature_اشاریہ_گنتی (0),
				lookup_اشاریہ_گنتی (0)
  {
    اگر (لیبل_4 == HB_OT_لیبل_GSUB)
    {
      lookup_اشاریہ_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_اشاریہ_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_ریکارڈ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? خالی_اشاریہ : &c_->plan->gsub_feature_ریکارڈ_cond_idx_map;
      catch_all_ریکارڈ_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_لیبل_map = &c_->plan->gsub_old_feature_idx_لیبل_map;
    }
    ورنہ
    {
      lookup_اشاریہ_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_اشاریہ_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_ریکارڈ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? خالی_اشاریہ : &c_->plan->gpos_feature_ریکارڈ_cond_idx_map;
      catch_all_ریکارڈ_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_لیبل_map = &c_->plan->gpos_old_feature_idx_لیبل_map;
    }
  }

  نجی:
  غیرعلامتی script_گنتی;
  غیرعلامتی langsys_گنتی;
  غیرعلامتی feature_اشاریہ_گنتی;
  غیرعلامتی lookup_اشاریہ_گنتی;
};

ساخت شے_Variation_Store;
ساخت hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  سانچہ <قسم_نام T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (یہ); واپس hb_خالی_خانے_t (); }
  ساکن return_t default_return_قدر () { واپس hb_خالی_خانے_t (); }

  hb_سیٹ_t *layout_variation_indices;
  ثابت hb_سیٹ_t *glyph_سیٹ;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<غیرعلامتی, صحیح>> *condition_اقدار = خالی_اشاریہ;

  hb_collect_variation_indices_context_t (hb_سیٹ_t *layout_variation_indices_,
					  ثابت hb_سیٹ_t *glyph_سیٹ_2,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_سیٹ (glyph_سیٹ_2),
					gpos_lookups (gpos_lookups_) {}
};

سانچہ<قسم_نام OutputArray>
ساخت subset_انحراف_array_t
{
  subset_انحراف_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& برآمد_4,
			 ثابت خالی *بنیاد_2) : subset_context (subset_context_),
					      برآمد (برآمد_4), بنیاد (بنیاد_2) {}

  سانچہ <قسم_نام T>
  منطقی عامل () (T&& انحراف)
  {
    خودکار snap = subset_context->serializer->snapshot ();
    خودکار *o = برآمد.serialize_append (subset_context->serializer);
    اگر (unlikely (!o)) واپس جھوٹ;
    منطقی ret = o->serialize_subset (subset_context, انحراف, بنیاد);
    اگر (!ret)
    {
      برآمد.نکالو ();
      subset_context->serializer->revert (snap);
    }
    واپس ret;
  }

  نجی:
  hb_subset_context_t *subset_context;
  OutputArray &برآمد;
  ثابت خالی *بنیاد;
};


سانچہ<قسم_نام OutputArray, قسم_نام Arg>
ساخت subset_انحراف_array_arg_t
{
  subset_انحراف_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& برآمد_4,
			     ثابت خالی *بنیاد_2,
			     Arg &&arg_) : subset_context (subset_context_), برآمد (برآمد_4),
					  بنیاد (بنیاد_2), arg (arg_) {}

  سانچہ <قسم_نام T>
  منطقی عامل () (T&& انحراف)
  {
    خودکار snap = subset_context->serializer->snapshot ();
    خودکار *o = برآمد.serialize_append (subset_context->serializer);
    اگر (unlikely (!o)) واپس جھوٹ;
    منطقی ret = o->serialize_subset (subset_context, انحراف, بنیاد, arg);
    اگر (!ret)
    {
      برآمد.نکالو ();
      subset_context->serializer->revert (snap);
    }
    واپس ret;
  }

  نجی:
  hb_subset_context_t *subset_context;
  OutputArray &برآمد;
  ثابت خالی *بنیاد;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
ساخت
{
  سانچہ<قسم_نام OutputArray>
  subset_انحراف_array_t<OutputArray>
  عامل () (hb_subset_context_t *subset_context, OutputArray& برآمد,
	       ثابت خالی *بنیاد) ثابت
  { واپس subset_انحراف_array_t<OutputArray> (subset_context, برآمد, بنیاد); }

  /* Variant with one extra argument passed to serialize_subset */
  سانچہ<قسم_نام OutputArray, قسم_نام Arg>
  subset_انحراف_array_arg_t<OutputArray, Arg>
  عامل () (hb_subset_context_t *subset_context, OutputArray& برآمد,
	       ثابت خالی *بنیاد, Arg &&arg) ثابت
  { واپس subset_انحراف_array_arg_t<OutputArray, Arg> (subset_context, برآمد, بنیاد, arg); }
}
HB_FUNCOBJ (subset_انحراف_array);

سانچہ<قسم_نام OutputArray>
ساخت subset_ریکارڈ_array_t
{
  subset_ریکارڈ_array_t (hb_subset_layout_context_t *c_, OutputArray* برآمد_4,
			 ثابت خالی *بنیاد_2) : subset_layout_context (c_),
					      برآمد (برآمد_4), بنیاد (بنیاد_2) {}

  سانچہ <قسم_نام T>
  خالی
  عامل () (T&& ریکارڈ)
  {
    خودکار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقی ret = ریکارڈ.subset (subset_layout_context, بنیاد);
    اگر (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ورنہ برآمد->len++;
  }

  نجی:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *برآمد;
  ثابت خالی *بنیاد;
};

سانچہ<قسم_نام OutputArray, قسم_نام Arg>
ساخت subset_ریکارڈ_array_arg_t
{
  subset_ریکارڈ_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* برآمد_4,
			     ثابت خالی *بنیاد_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   برآمد (برآمد_4), بنیاد (بنیاد_2), arg (arg_) {}

  سانچہ <قسم_نام T>
  خالی
  عامل () (T&& ریکارڈ)
  {
    خودکار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقی ret = ریکارڈ.subset (subset_layout_context, بنیاد, arg);
    اگر (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    ورنہ برآمد->len++;
  }

  نجی:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *برآمد;
  ثابت خالی *بنیاد;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
ساخت
{
  سانچہ<قسم_نام OutputArray>
  subset_ریکارڈ_array_t<OutputArray>
  عامل () (hb_subset_layout_context_t *c, OutputArray* برآمد,
	       ثابت خالی *بنیاد) ثابت
  { واپس subset_ریکارڈ_array_t<OutputArray> (c, برآمد, بنیاد); }

  /* Variant with one extra argument passed to subset */
  سانچہ<قسم_نام OutputArray, قسم_نام Arg>
  subset_ریکارڈ_array_arg_t<OutputArray, Arg>
  عامل () (hb_subset_layout_context_t *c, OutputArray* برآمد,
               ثابت خالی *بنیاد, Arg &&arg) ثابت
  { واپس subset_ریکارڈ_array_arg_t<OutputArray, Arg> (c, برآمد, بنیاد, arg); }
}
HB_FUNCOBJ (subset_ریکارڈ_array);


سانچہ<قسم_نام OutputArray>
ساخت serialize_math_ریکارڈ_array_t
{
  serialize_math_ریکارڈ_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& برآمد_4,
                         ثابت خالی *بنیاد_2,
                         ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              برآمد (برآمد_4), بنیاد (بنیاد_2), variation_map (variation_map_) {}

  سانچہ <قسم_نام T>
  منطقی عامل () (T&& ریکارڈ)
  {
    اگر (!serialize_context->copy (ریکارڈ, بنیاد, variation_map)) واپس جھوٹ;
    برآمد.len++;
    واپس سچ;
  }

  نجی:
  hb_serialize_context_t *serialize_context;
  OutputArray &برآمد;
  ثابت خالی *بنیاد;
  ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
ساخت
{
  سانچہ<قسم_نام OutputArray>
  serialize_math_ریکارڈ_array_t<OutputArray>
  عامل () (hb_serialize_context_t *serialize_context, OutputArray& برآمد,
               ثابت خالی *بنیاد,
               ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *variation_map) ثابت
  { واپس serialize_math_ریکارڈ_array_t<OutputArray> (serialize_context, برآمد, بنیاد, variation_map); }

}
HB_FUNCOBJ (serialize_math_ریکارڈ_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

ساخت اشاریہ_Array : Array16_کا<اشاریہ>
{
  منطقی intersects (ثابت hb_map_t *indexes) ثابت
  { واپس hb_any (*یہ, indexes); }

  سانچہ <قسم_نام Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    اگر (!it) واپس;
    اگر (unlikely (!c->extend_min ((*یہ)))) واپس;

    برائے (ثابت خودکار _ : it)
    {
      اگر (!l->دورہ_Lookup_اشاریہ()) توڑو;

      اشاریہ i;
      i = _;
      c->copy (i);
      یہ->len++;
    }
  }

  غیرعلامتی صحیح get_indexes (غیرعلامتی صحیح آغاز_انحراف,
			    غیرعلامتی صحیح *گنتی_2 /* IN/OUT */,
			    غیرعلامتی صحیح *_indexes /*برآمد*/) ثابت
  {
    اگر (گنتی_2 && _indexes)
    {
      + یہ->as_array ().sub_array (آغاز_انحراف, گنتی_2)
      | hb_sink (hb_array (_indexes, *گنتی_2))
      ;
    }
    واپس یہ->len;
  }

  خالی جوڑو_indexes_مقصد (hb_سیٹ_t* output /*برآمد*/) ثابت
  {
    output->جوڑو_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
ساخت Feature_Params_سائز
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ))) return_trace (جھوٹ);
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

    اگر (!design_سائز)
      return_trace (جھوٹ);
    ورنہ اگر (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     range_آغاز == 0 &&
	     range_انتہا == 0)
      return_trace (سچ);
    ورنہ اگر (design_سائز < range_آغاز ||
	     design_سائز > range_انتہا ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (جھوٹ);
    ورنہ
      return_trace (سچ);
  }

  خالی collect_name_ids (hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  { nameids_مقصد_retain->جوڑو (subfamilyNameID); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    return_trace ((منطقی) c->serializer->embed (*یہ));
  }

  HBUINT16	design_سائز;	/* Represents the design size in 720/inch
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
  HBUINT16	range_آغاز;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	range_انتہا;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  عوامی:
  DEFINE_سائز_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
ساخت Feature_Params_Stylistic_سیٹ
{
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (یہ));
  }

  خالی collect_name_ids (hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  { nameids_مقصد_retain->جوڑو (uiNameID); }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    return_trace ((منطقی) c->serializer->embed (*یہ));
  }

  HBUINT16	نسخہ;	/* (set to 0): This corresponds to a “minor”
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
  عوامی:
  DEFINE_سائز_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
ساخت FeatureParamsCharacterVariants
{
  غیرعلامتی
  get_characters (غیرعلامتی آغاز_انحراف, غیرعلامتی *char_گنتی, hb_codepoint_t *chars) ثابت
  {
    اگر (char_گنتی && chars)
    {
      + characters.as_array ().sub_array (آغاز_انحراف, char_گنتی)
      | hb_sink (hb_array (chars, *char_گنتی))
      ;
    }
    واپس characters.len;
  }

  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (characters.len, HBUINT24::static_سائز, min_سائز); }

  خالی collect_name_ids (hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  {
    اگر (featUILableNameID) nameids_مقصد_retain->جوڑو (featUILableNameID);
    اگر (feat_UITooltip_متن_Name_ID) nameids_مقصد_retain->جوڑو (feat_UITooltip_متن_Name_ID);
    اگر (sample_متن_Name_ID) nameids_مقصد_retain->جوڑو (sample_متن_Name_ID);

    اگر (!پہلا_Param_UILabel_Name_ID || !عدد_Named_Parameters || عدد_Named_Parameters >= 0x7FFF)
      واپس;

    غیرعلامتی آخری_name_id = (غیرعلامتی) پہلا_Param_UILabel_Name_ID + (غیرعلامتی) عدد_Named_Parameters - 1;
    nameids_مقصد_retain->جوڑو_range (پہلا_Param_UILabel_Name_ID, آخری_name_id);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    return_trace ((منطقی) c->serializer->embed (*یہ));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
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
  NameID	پہلا_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_کا<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  عوامی:
  DEFINE_سائز_ARRAY (14, characters);
};

ساخت FeatureParams
{
  منطقی sanitize (hb_sanitize_context_t *c, hb_لیبل_t لیبل_2) ثابت
  {
#اگر_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
    واپس سچ;
#اگر_ختم
    TRACE_SANITIZE (یہ);
    اگر (لیبل_2 == HB_لیبل ('s','i','z','e') && hb_barrier ())
      return_trace (u.سائز.sanitize (c));
    اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سیٹ.sanitize (c));
    اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (سچ);
  }

  خالی collect_name_ids (hb_لیبل_t لیبل_2, hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  {
#اگر_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
    واپس;
#اگر_ختم
    اگر (لیبل_2 == HB_لیبل ('s','i','z','e') && hb_barrier ())
      واپس (u.سائز.collect_name_ids (nameids_مقصد_retain));
    اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('s','s','\0','\0')) /* ssXX */
      واپس (u.stylistic_سیٹ.collect_name_ids (nameids_مقصد_retain));
    اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('c','v','\0','\0')) /* cvXX */
      واپس (u.characterVariants.collect_name_ids (nameids_مقصد_retain));
  }

  منطقی subset (hb_subset_context_t *c, ثابت لیبل* لیبل_2) ثابت
  {
#اگر_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
    واپس جھوٹ;
#اگر_ختم
    TRACE_SUBSET (یہ);
    اگر (!لیبل_2) return_trace (جھوٹ);
    اگر (*لیبل_2 == HB_لیبل ('s','i','z','e') && hb_barrier ())
      return_trace (u.سائز.subset (c));
    اگر ((*لیبل_2 & 0xFFFF0000u) == HB_لیبل ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سیٹ.subset (c));
    اگر ((*لیبل_2 & 0xFFFF0000u) == HB_لیبل ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (جھوٹ);
  }

#اگر_غیر_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت Feature_Params_سائز& get_سائز_params (hb_لیبل_t لیبل_2) ثابت
  {
    اگر (لیبل_2 == HB_لیبل ('s','i','z','e') && hb_barrier ())
      واپس u.سائز;
    واپس Null (Feature_Params_سائز);
  }
  ثابت Feature_Params_Stylistic_سیٹ& get_stylistic_سیٹ_params (hb_لیبل_t لیبل_2) ثابت
  {
    اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('s','s','\0','\0')) /* ssXX */
      واپس u.stylistic_سیٹ;
    واپس Null (Feature_Params_Stylistic_سیٹ);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_لیبل_t لیبل_2) ثابت
  {
    اگر ((لیبل_2 & 0xFFFF0000u) == HB_لیبل ('c','v','\0','\0')) /* cvXX */
      واپس u.characterVariants;
    واپس Null (FeatureParamsCharacterVariants);
  }
#اگر_ختم

  نجی:
  اتحاد {
  Feature_Params_سائز			سائز;
  Feature_Params_Stylistic_سیٹ		stylistic_سیٹ;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عوامی:
  DEFINE_سائز_MIN (0);
};

ساخت ریکارڈ_sanitize_احاطہ_t {
  hb_لیبل_t لیبل_2;
  ثابت خالی *list_بنیاد;
};

ساخت Feature
{
  غیرعلامتی صحیح get_lookup_گنتی () ثابت
  { واپس lookup_اشاریہ_2.len; }
  hb_لیبل_t get_lookup_اشاریہ (غیرعلامتی صحیح i) ثابت
  { واپس lookup_اشاریہ_2[i]; }
  غیرعلامتی صحیح get_lookup_indexes (غیرعلامتی صحیح آغاز_اشاریہ,
				   غیرعلامتی صحیح *lookup_گنتی /* IN/OUT */,
				   غیرعلامتی صحیح *lookup_لیبل_فہرست /*برآمد*/) ثابت
  { واپس lookup_اشاریہ_2.get_indexes (آغاز_اشاریہ, lookup_گنتی, lookup_لیبل_فہرست); }
  خالی جوڑو_lookup_indexes_مقصد (hb_سیٹ_t *lookup_indexes) ثابت
  { lookup_اشاریہ_2.جوڑو_indexes_مقصد (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { واپس یہ+featureParams; }

  منطقی intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { واپس lookup_اشاریہ_2.intersects (lookup_indexes); }

  خالی collect_name_ids (hb_لیبل_t لیبل_2, hb_سیٹ_t *nameids_مقصد_retain /*برآمد*/) ثابت
  {
    اگر (featureParams)
      get_feature_params ().collect_name_ids (لیبل_2, nameids_مقصد_retain);
  }

  منطقی subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت لیبل                   *لیبل_2 = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->featureParams.serialize_subset (c, featureParams, یہ, لیبل_2);

    hb::shared_ptr<hb_سیٹ_t> *lookup_variations;
    اگر (l->cur_feature_اشاریہ != اشاریہ::NOT_ملا_اشاریہ &&
	l->lookup_variations->has (l->cur_feature_اشاریہ, &lookup_variations))
    {
      خودکار it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_اشاریہ_map)
      | hb_map (l->lookup_اشاریہ_map)
      ;
      برآمد->lookup_اشاریہ_2.serialize (c->serializer, l, it);
    }
    ورنہ
    {
      خودکار it =
      + hb_iter (lookup_اشاریہ_2)
      | hb_filter (l->lookup_اشاریہ_map)
      | hb_map (l->lookup_اشاریہ_map)
      ;
      برآمد->lookup_اشاریہ_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_احاطہ_t *احاطہ = خالی_اشاریہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  featureParams.sanitize (c, یہ, احاطہ ? احاطہ->لیبل_2 : HB_لیبل_NONE) &&
		  lookup_اشاریہ_2.sanitize (c));
  }

  Offset16_مقصد<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  اشاریہ_Array	 lookup_اشاریہ_2;	/* Array of LookupList indices */
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (4, lookup_اشاریہ_2);
};

سانچہ <قسم_نام Type>
ساخت ریکارڈ_2
{
  صحیح cmp (hb_لیبل_t a) ثابت { واپس لیبل_2.cmp (a); }

  منطقی subset (hb_subset_layout_context_t *c, ثابت خالی *بنیاد, ثابت خالی *f_sub = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->subset_context->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (!f_sub)
      return_trace (برآمد->انحراف.serialize_subset (c->subset_context, انحراف, بنیاد, c, &لیبل_2));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    خودکار *s = c->subset_context->serializer;
    s->push ();

    برآمد->انحراف = 0;
    منطقی ret = f.subset (c->subset_context, c, &لیبل_2);
    اگر (ret)
      s->جوڑو_link (برآمد->انحراف, s->نکالو_pack ());
    ورنہ
      s->نکالو_discard ();

    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    ثابت ریکارڈ_sanitize_احاطہ_t احاطہ = {لیبل_2, بنیاد};
    return_trace (c->check_struct (یہ) &&
		  انحراف.sanitize (c, بنیاد, &احاطہ));
  }

  لیبل           لیبل_2;            /* 4-byte Tag identifier */
  Offset16_مقصد<Type>
                انحراف;         /* Offset from beginning of object holding
                                 * the Record */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

سانچہ <قسم_نام Type>
ساخت ریکارڈ_Array_کا : Sorted_Array16_کا<ریکارڈ_2<Type>>
{
  ثابت Offset16_مقصد<Type>& get_انحراف (غیرعلامتی صحیح i) ثابت
  { واپس (*یہ)[i].انحراف; }
  Offset16_مقصد<Type>& get_انحراف (غیرعلامتی صحیح i)
  { واپس (*یہ)[i].انحراف; }
  ثابت لیبل& get_لیبل (غیرعلامتی صحیح i) ثابت
  { واپس (*یہ)[i].لیبل_2; }
  غیرعلامتی صحیح get_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
                         غیرعلامتی صحیح *ریکارڈ_گنتی /* IN/OUT */,
                         hb_لیبل_t     *ریکارڈ_لیبل_فہرست /*برآمد*/) ثابت
  {
    اگر (ریکارڈ_گنتی && ریکارڈ_لیبل_فہرست)
    {
      + یہ->as_array ().sub_array (آغاز_انحراف, ریکارڈ_گنتی)
      | hb_map (&ریکارڈ_2<Type>::لیبل_2)
      | hb_sink (hb_array (ریکارڈ_لیبل_فہرست, *ریکارڈ_گنتی))
      ;
    }
    واپس یہ->len;
  }
  منطقی ڈھونڈو_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  {
    واپس یہ->bfind (لیبل_2, اشاریہ_2, HB_NOT_ملا_STORE, اشاریہ::NOT_ملا_اشاریہ);
  }
};

سانچہ <قسم_نام Type>
ساخت ریکارڈ_List_کا : ریکارڈ_Array_کا<Type>
{
  ثابت Type& عامل [] (غیرعلامتی صحیح i) ثابت
  { واپس یہ+یہ->get_انحراف (i); }

  منطقی subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    + یہ->iter ()
    | hb_apply (subset_ریکارڈ_array (l, برآمد, یہ))
    ;
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (ریکارڈ_Array_کا<Type>::sanitize (c, یہ));
  }
};

ساخت ریکارڈ_List_کا_Feature : ریکارڈ_List_کا<Feature>
{
  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    + hb_enumerate (*یہ)
    | hb_filter (l->feature_اشاریہ_map, hb_پہلا)
    | hb_apply ([l, برآمد, یہ] (ثابت hb_pair_t<غیرعلامتی, ثابت ریکارڈ_2<Feature>&>& _)
                {
                  ثابت Feature *f_sub = خالی_اشاریہ;
                  ثابت Feature **f = خالی_اشاریہ;
                  اگر (l->feature_substitutes_map->has (_.پہلا, &f))
                    f_sub = *f;

                  l->cur_feature_اشاریہ = _.پہلا;
                  subset_ریکارڈ_array (l, برآمد, یہ, f_sub) (_.second);
                  l->cur_feature_اشاریہ = اشاریہ::NOT_ملا_اشاریہ;
                })
    ;

    return_trace (سچ);
  }
};

قسمتعریف ریکارڈ_List_کا<Feature> FeatureList;


ساخت LangSys
{
  غیرعلامتی صحیح get_feature_گنتی () ثابت
  { واپس feature_اشاریہ_2.len; }
  hb_لیبل_t get_feature_اشاریہ (غیرعلامتی صحیح i) ثابت
  { واپس feature_اشاریہ_2[i]; }
  غیرعلامتی صحیح get_feature_indexes (غیرعلامتی صحیح آغاز_انحراف,
				    غیرعلامتی صحیح *feature_گنتی_2 /* IN/OUT */,
				    غیرعلامتی صحیح *feature_indexes /*برآمد*/) ثابت
  { واپس feature_اشاریہ_2.get_indexes (آغاز_انحراف, feature_گنتی_2, feature_indexes); }
  خالی جوڑو_feature_indexes_مقصد (hb_سیٹ_t *feature_indexes) ثابت
  { feature_اشاریہ_2.جوڑو_indexes_مقصد (feature_indexes); }

  منطقی has_required_feature () ثابت { واپس req_Feature_اشاریہ != 0xFFFFu; }
  غیرعلامتی صحیح get_required_feature_اشاریہ () ثابت
  {
    اگر (req_Feature_اشاریہ == 0xFFFFu)
      واپس اشاریہ::NOT_ملا_اشاریہ;
   واپس req_Feature_اشاریہ;
  }

  LangSys* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (*یہ));
  }

  منطقی compare (ثابت LangSys& o, ثابت hb_map_t *feature_اشاریہ_map) ثابت
  {
    اگر (req_Feature_اشاریہ != o.req_Feature_اشاریہ)
      واپس جھوٹ;

    خودکار iter =
    + hb_iter (feature_اشاریہ_2)
    | hb_filter (feature_اشاریہ_map)
    | hb_map (feature_اشاریہ_map)
    ;

    خودکار o_iter =
    + hb_iter (o.feature_اشاریہ_2)
    | hb_filter (feature_اشاریہ_map)
    | hb_map (feature_اشاریہ_map)
    ;

    برائے (; iter && o_iter; iter++, o_iter++)
    {
      غیرعلامتی a = *iter;
      غیرعلامتی b = *o_iter;
      اگر (a != b) واپس جھوٹ;
    }

    اگر (iter || o_iter) واپس جھوٹ;

    واپس سچ;
  }

  خالی collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    اگر (!has_required_feature () && !get_feature_گنتی ()) واپس;
    اگر (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_اشاریہ))
      c->new_feature_indexes->جوڑو (get_required_feature_اشاریہ ());

    + hb_iter (feature_اشاریہ_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقی subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت لیبل                  *لیبل_2 = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    ثابت uint32_t *v;
    برآمد->req_Feature_اشاریہ = l->feature_map_w_duplicates->has (req_Feature_اشاریہ, &v) ? *v : 0xFFFFu;

    اگر (!l->دورہ_Feature_اشاریہ (feature_اشاریہ_2.len))
      return_trace (جھوٹ);

    خودکار it =
    + hb_iter (feature_اشاریہ_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقی ret = منطقی (it);
    برآمد->feature_اشاریہ_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_احاطہ_t * = خالی_اشاریہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && feature_اشاریہ_2.sanitize (c));
  }

  Offset16	lookup_ترتیب_Z;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_اشاریہ;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  اشاریہ_Array	feature_اشاریہ_2;	/* Array of indices into the FeatureList */
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (6, feature_اشاریہ_2);
};
DECLARE_NULL_NAMESPACE_بائٹیں (OT, LangSys);

ساخت Script
{
  غیرعلامتی صحیح get_lang_sys_گنتی () ثابت
  { واپس langSys.len; }
  ثابت لیبل& get_lang_sys_لیبل (غیرعلامتی صحیح i) ثابت
  { واپس langSys.get_لیبل (i); }
  غیرعلامتی صحیح get_lang_sys_لیبل_فہرست (غیرعلامتی صحیح آغاز_انحراف,
				  غیرعلامتی صحیح *lang_sys_گنتی /* IN/OUT */,
				  hb_لیبل_t     *lang_sys_لیبل_فہرست /*برآمد*/) ثابت
  { واپس langSys.get_لیبل_فہرست (آغاز_انحراف, lang_sys_گنتی, lang_sys_لیبل_فہرست); }
  ثابت LangSys& get_lang_sys (غیرعلامتی صحیح i) ثابت
  {
    اگر (i == اشاریہ::NOT_ملا_اشاریہ) واپس get_default_lang_sys ();
    واپس یہ+langSys[i].انحراف;
  }
  منطقی ڈھونڈو_lang_sys_اشاریہ (hb_لیبل_t لیبل_2, غیرعلامتی صحیح *اشاریہ_2) ثابت
  { واپس langSys.ڈھونڈو_اشاریہ (لیبل_2, اشاریہ_2); }

  منطقی has_default_lang_sys () ثابت           { واپس defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { واپس یہ+defaultLangSys; }

  خالی prune_langsys (hb_prune_langsys_context_t *c,
                      غیرعلامتی script_اشاریہ) ثابت
  {
    اگر (!has_default_lang_sys () && !get_lang_sys_گنتی ()) واپس;
    اگر (!c->دورہ_Script ()) واپس;

    اگر (!c->script_langsys_map->has (script_اشاریہ))
    {
      اگر (unlikely (!c->script_langsys_map->سیٹ (script_اشاریہ, hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_create ()})))
	واپس;
    }

    اگر (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      اگر (c->دورہ_Langsys (d.get_feature_گنتی ())) {
        d.collect_features (c);
      }

      برائے (خودکار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = یہ+_.second.انحراف;
        اگر (!c->دورہ_Langsys (l.get_feature_گنتی ())) جاری;
        اگر (l.compare (d, c->duplicate_feature_map)) جاری;

        l.collect_features (c);
        c->script_langsys_map->get (script_اشاریہ)->جوڑو (_.پہلا);
      }
    }
    ورنہ
    {
      برائے (خودکار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = یہ+_.second.انحراف;
        اگر (!c->دورہ_Langsys (l.get_feature_گنتی ())) جاری;
        l.collect_features (c);
        c->script_langsys_map->get (script_اشاریہ)->جوڑو (_.پہلا);
      }
    }
  }

  منطقی subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت لیبل                   *لیبل_2) ثابت
  {
    TRACE_SUBSET (یہ);
    اگر (!l->دورہ_Script ()) return_trace (جھوٹ);
    اگر (لیبل_2 && !c->plan->layout_scripts.has (*لیبل_2))
      واپس جھوٹ;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    منطقی defaultLang = جھوٹ;
    اگر (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = یہ+defaultLangSys;
      منطقی ret = ls.subset (c, l);
      اگر (!ret && لیبل_2 && *لیبل_2 != HB_لیبل ('D', 'F', 'L', 'T'))
      {
	c->serializer->نکالو_discard ();
	برآمد->defaultLangSys = 0;
      }
      ورنہ
      {
	c->serializer->جوڑو_link (برآمد->defaultLangSys, c->serializer->نکالو_pack ());
	defaultLang = سچ;
      }
    }

    ثابت hb_سیٹ_t *فعال_langsys = l->script_langsys_map->get (l->cur_script_اشاریہ);
    اگر (فعال_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (فعال_langsys, hb_پہلا)
      | hb_map (hb_دوسرا)
      | hb_filter ([=] (ثابت ریکارڈ_2<LangSys>& ریکارڈ) {واپس l->دورہ_Lang_Sys (); })
      | hb_apply (subset_ریکارڈ_array (l, &(برآمد->langSys), یہ))
      ;
    }

    return_trace (منطقی (برآمد->langSys.len) || defaultLang || l->جدول_لیبل_2 == HB_OT_لیبل_GSUB);
  }

  منطقی sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_احاطہ_t * = خالی_اشاریہ) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (defaultLangSys.sanitize (c, یہ) && langSys.sanitize (c, یہ));
  }

  محفوظ:
  Offset16_مقصد<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  ریکارڈ_Array_کا<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (4, langSys);
};

ساخت ریکارڈ_List_کا_Script : ریکارڈ_List_کا<Script>
{
  منطقی subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برائے (خودکار _ : + hb_enumerate (*یہ))
    {
      خودکار snap = c->serializer->snapshot ();
      l->cur_script_اشاریہ = _.پہلا;
      منطقی ret = _.second.subset (l, یہ);
      اگر (!ret) c->serializer->revert (snap);
      ورنہ برآمد->len++;
    }

    return_trace (سچ);
  }
};

قسمتعریف ریکارڈ_List_کا_Script ScriptList;



ساخت LookupFlag : HBUINT16
{
  شمار Flags {
    دایاں_مقصد_بایاں		= 0x0001u,
    Ignore_بنیاد_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_سیٹ	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  عوامی:
  DEFINE_سائز_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
نامخلا OT {

ساخت Lookup
{
  غیرعلامتی صحیح get_subtable_گنتی () ثابت { واپس sub_جدول.len; }

  سانچہ <قسم_نام TSub_جدول>
  ثابت Array16_کا_Offset16_مقصد<TSub_جدول>& get_subtables () ثابت
  { واپس reinterpret_cast<ثابت Array16_کا_Offset16_مقصد<TSub_جدول> &> (sub_جدول); }
  سانچہ <قسم_نام TSub_جدول>
  Array16_کا_Offset16_مقصد<TSub_جدول>& get_subtables ()
  { واپس reinterpret_cast<Array16_کا_Offset16_مقصد<TSub_جدول> &> (sub_جدول); }

  سانچہ <قسم_نام TSub_جدول>
  ثابت TSub_جدول& get_subtable (غیرعلامتی صحیح i) ثابت
  { واپس یہ+get_subtables<TSub_جدول> ()[i]; }
  سانچہ <قسم_نام TSub_جدول>
  TSub_جدول& get_subtable (غیرعلامتی صحیح i)
  { واپس یہ+get_subtables<TSub_جدول> ()[i]; }

  size_t get_سائز () ثابت
  {
    ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<ثابت HBUINT16> (sub_جدول);
    اگر (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
      واپس (ثابت حرف *) &StructAfter<ثابت حرف> (mark_Filtering_سیٹ) - (ثابت حرف *) یہ;
    واپس (ثابت حرف *) &mark_Filtering_سیٹ - (ثابت حرف *) یہ;
  }

  غیرعلامتی صحیح get_type () ثابت { واپس lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    غیرعلامتی صحیح flag = lookupFlag;
    اگر (unlikely (flag & LookupFlag::Use_Mark_Filtering_سیٹ))
    {
      ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      flag += (mark_Filtering_سیٹ << 16);
    }
    واپس flag;
  }

  سانچہ <قسم_نام TSub_جدول, قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    غیرعلامتی صحیح lookup_type = get_type ();
    TRACE_DISPATCH (یہ, lookup_type);
    غیرعلامتی صحیح گنتی = get_subtable_گنتی ();
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++) {
      قسم_نام context_t::return_t r = get_subtable<TSub_جدول> (i).dispatch (c, lookup_type, ds...);
      اگر (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_قدر ());
  }

  منطقی serialize (hb_serialize_context_t *c,
		  غیرعلامتی صحیح lookup_type,
		  uint32_t lookup_props,
		  غیرعلامتی صحیح عدد_subtables)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    اگر (unlikely (!sub_جدول.serialize (c, عدد_subtables))) return_trace (جھوٹ);
    اگر (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      اگر (unlikely (!c->extend (یہ))) return_trace (جھوٹ);
      HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      mark_Filtering_سیٹ = lookup_props >> 16;
    }
    return_trace (سچ);
  }

  سانچہ <قسم_نام TSub_جدول>
  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->lookupType = lookupType;
    برآمد->lookupFlag = lookupFlag;

    ثابت hb_سیٹ_t *glyphset = c->plan->glyphset_gsub ();
    غیرعلامتی صحیح lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_جدول> ())
    | hb_filter ([یہ, glyphset, lookup_type] (ثابت Offset16_مقصد<TSub_جدول> &_) { واپس (یہ+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_انحراف_array (c, برآمد->get_subtables<TSub_جدول> (), یہ, lookup_type))
    ;

    اگر (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      hb_codepoint_t *idx;
      اگر (!c->plan->مستعمل_mark_sets_map.has (mark_Filtering_سیٹ, &idx))
      {
        غیرعلامتی new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_سیٹ;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        برآمد->lookupFlag = new_flag;
      }
      ورنہ
      {
        اگر (unlikely (!c->serializer->extend (برآمد))) return_trace (جھوٹ);
        HBUINT16 &برآمد_Mark_Filtering_سیٹ = StructAfter<HBUINT16> (برآمد->sub_جدول);
        برآمد_Mark_Filtering_سیٹ = *idx;
      }
    }

    // Always keep the lookup even if it's empty. The rest of layout subsetting depends on lookup
    // indices being consistent with those computed during planning. So if an empty lookup is
    // discarded during the subset phase it will invalidate all subsequent lookup indices.
    // Generally we shouldn't end up with an empty lookup as we pre-prune them during the planning
    // phase, but it can happen in rare cases such as when during closure subtable is considered
    // degenerate (see: https://github.com/harfbuzz/harfbuzz/issues/3853)
    return_trace (سچ);
  }

  سانچہ <قسم_نام TSub_جدول>
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!(c->check_struct (یہ) && sub_جدول.sanitize (c))) return_trace (جھوٹ);
    hb_barrier ();

    غیرعلامتی subtables = get_subtable_گنتی ();
    اگر (unlikely (!c->دورہ_subtables (subtables))) return_trace (جھوٹ);

    اگر (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      اگر (!mark_Filtering_سیٹ.sanitize (c)) return_trace (جھوٹ);
    }

    اگر (unlikely (!get_subtables<TSub_جدول> ().sanitize (c, یہ, get_type ())))
      return_trace (جھوٹ);

    اگر (unlikely (get_type () == TSub_جدول::Extension))
    {
      hb_barrier ();

      /* The spec says all subtables of an Extension lookup should
       * have the same type, which shall not be the Extension type
       * itself (but we already checked for that).
       * This is specially important if one has a reverse type!
       */
      غیرعلامتی صحیح type = get_subtable<TSub_جدول> (0).u.extension.get_type ();
      برائے (غیرعلامتی صحیح i = 1; i < subtables; i++)
	اگر (get_subtable<TSub_جدول> (i).u.extension.get_type () != type)
	  return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  محفوظ:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_کا<Offset16>
		sub_جدول;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عوامی:
  DEFINE_سائز_ARRAY (6, sub_جدول);
};

سانچہ <قسم_نام Types>
استعمال LookupList = List16_کا_انحراف_مقصد<Lookup, قسم_نام Types::HBLUINT>;

سانچہ <قسم_نام TLookup, قسم_نام انحراف_Type>
ساخت Lookup_انحراف_List : List16_کا_انحراف_مقصد<TLookup, انحراف_Type>
{
  منطقی subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    + hb_enumerate (*یہ)
    | hb_filter (l->lookup_اشاریہ_map, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_apply (subset_انحراف_array (c, *برآمد, یہ))
    ;
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace ((List16_کا_انحراف_مقصد<TLookup, انحراف_Type>::sanitize (c, یہ)));
  }
};


/*
 * Coverage Table
 */


ساکن منطقی ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_سیٹ_t &klasses,
                                          منطقی use_class_zero,
                                          hb_sorted_سمتیہ_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  اگر (!klass_map)
    واپس ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  اگر (!use_class_zero)
    klass_map->سیٹ (0, 0);

  غیرعلامتی idx = klass_map->has (0) ? 1 : 0;
  برائے (ثابت غیرعلامتی k: klasses)
  {
    اگر (klass_map->has (k)) جاری;
    klass_map->سیٹ (k, idx);
    idx++;
  }


  اگر (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    واپس جھوٹ;

  برائے (غیرعلامتی i = 0; i < glyph_and_klass.لمبائی; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  واپس ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

سانچہ <قسم_نام Types>
ساخت ClassDefFormat1_3
{
  دوست ساخت ClassDef;

  نجی:
  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id) ثابت
  {
    واپس class_قدر[(غیرعلامتی صحیح) (glyph_id - آغاز_Glyph)];
  }

  غیرعلامتی get_population () ثابت
  {
    واپس class_قدر.len;
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    اگر (unlikely (!it))
    {
      classFormat = Types::سائز == 2 ? 1 : 3;
      آغاز_Glyph = 0;
      class_قدر.len = 0;
      return_trace (سچ);
    }

    hb_codepoint_t glyph_min = (*it).پہلا;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_پہلا)
			       | hb_reduce (hb_max, 0u);
    غیرعلامتی glyph_گنتی = glyph_max - glyph_min + 1;

    آغاز_Glyph = glyph_min;
    اگر (unlikely (!class_قدر.serialize (c, glyph_گنتی))) return_trace (جھوٹ);
    برائے (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      غیرعلامتی idx = gid_klass_pair.پہلا - glyph_min;
      class_قدر[idx] = gid_klass_pair.second;
    }
    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالی_اشاریہ /*OUT*/,
               منطقی keep_خالی_خانے_جدول = سچ,
               منطقی use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);

    hb_sorted_سمتیہ_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سیٹ_t orig_klasses;

    hb_codepoint_t آغاز = آغاز_Glyph;
    hb_codepoint_t انتہا   = آغاز + class_قدر.len;

    برائے (ثابت hb_codepoint_t gid : + hb_range (آغاز, انتہا))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      اگر (new_gid == HB_MAP_قدر_INVALID) جاری;
      اگر (glyph_filter && !glyph_filter->has(gid)) جاری;

      غیرعلامتی klass = class_قدر[gid - آغاز];
      اگر (!klass) جاری;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.جوڑو (klass);
    }

    اگر (use_class_zero)
    {
      ثابت hb_سیٹ_t& glyphset = *c->plan->glyphset_gsub ();
      غیرعلامتی glyph_گنتی = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_گنتی <= glyph_and_klass.لمبائی;
    }
    اگر (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (جھوٹ);
    return_trace (keep_خالی_خانے_جدول || (منطقی) glyph_and_klass);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && class_قدر.sanitize (c));
  }

  غیرعلامتی لاگت () ثابت { واپس 1; }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    غیرعلامتی صحیح آغاز = 0;
    غیرعلامتی صحیح گنتی = class_قدر.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (class_قدر[i])
	جاری;

      اگر (آغاز != i)
	اگر (unlikely (!glyphs->جوڑو_range (آغاز_Glyph + آغاز, آغاز_Glyph + i)))
	  واپس جھوٹ;

      آغاز = i + 1;
    }
    اگر (آغاز != گنتی)
      اگر (unlikely (!glyphs->جوڑو_range (آغاز_Glyph + آغاز, آغاز_Glyph + گنتی)))
	واپس جھوٹ;

    واپس سچ;
  }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_class (سیٹ_t *glyphs, غیرعلامتی klass) ثابت
  {
    غیرعلامتی صحیح گنتی = class_قدر.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (class_قدر[i] == klass) glyphs->جوڑو (آغاز_Glyph + i);
    واپس سچ;
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    hb_codepoint_t آغاز = آغاز_Glyph;
    hb_codepoint_t انتہا = آغاز_Glyph + class_قدر.len;
    برائے (hb_codepoint_t iter = آغاز_Glyph - 1;
	 glyphs->next (&iter) && iter < انتہا;)
      اگر (class_قدر[iter - آغاز]) واپس سچ;
    واپس جھوٹ;
  }
  منطقی intersects_class (ثابت hb_سیٹ_t *glyphs, غیرعلامتی klass) ثابت
  {
    غیرعلامتی صحیح گنتی = class_قدر.len;
    اگر (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
      اگر (!glyphs->next (&g)) واپس جھوٹ;
      اگر (g < آغاز_Glyph) واپس سچ;
      g = آغاز_Glyph + گنتی - 1;
      اگر (glyphs->next (&g)) واپس سچ;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت قسم_نام Types::HBUINT *arr = class_قدر.arrayZ;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (arr[i] == klass && glyphs->has (آغاز_Glyph + i))
	واپس سچ;
    واپس جھوٹ;
  }

  خالی intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, غیرعلامتی klass, hb_سیٹ_t *intersect_glyphs) ثابت
  {
    غیرعلامتی گنتی = class_قدر.len;
    اگر (klass == 0)
    {
      غیرعلامتی آغاز_glyph = آغاز_Glyph;
      برائے (uint32_t g = HB_سیٹ_قدر_INVALID;
	   glyphs->next (&g) && g < آغاز_glyph;)
	intersect_glyphs->جوڑو (g);

      برائے (uint32_t g = آغاز_Glyph + گنتی - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->جوڑو (g);

      واپس;
    }

    برائے (غیرعلامتی i = 0; i < گنتی; i++)
      اگر (class_قدر[i] == klass && glyphs->has (آغاز_Glyph + i))
	intersect_glyphs->جوڑو (آغاز_Glyph + i);

#اگر 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    غیرعلامتی آغاز_glyph = آغاز_Glyph;
    غیرعلامتی انتہا_glyph = آغاز_glyph + گنتی;
    برائے (غیرعلامتی g = آغاز_Glyph - 1;
	 glyphs->next (&g) && g < انتہا_glyph;)
      اگر (class_قدر.arrayZ[g - آغاز_glyph] == klass)
        intersect_glyphs->جوڑو (g);
#اگر_ختم
  }

  خالی intersected_classes (ثابت hb_سیٹ_t *glyphs, hb_سیٹ_t *intersect_classes) ثابت
  {
    اگر (glyphs->is_empty ()) واپس;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    اگر (!class_قدر.len)
    {
      intersect_classes->جوڑو (0);
      واپس;
    }

    hb_codepoint_t انتہا_glyph = آغاز_Glyph + class_قدر.len - 1;
    اگر (glyphs->get_min () < آغاز_Glyph ||
        glyphs->get_max () > انتہا_glyph)
      intersect_classes->جوڑو (0);

    برائے (ثابت خودکار& _ : + hb_enumerate (class_قدر))
    {
      hb_codepoint_t g = آغاز_Glyph + _.پہلا;
      اگر (glyphs->has (g))
        intersect_classes->جوڑو (_.second);
    }
  }

  محفوظ:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  قسم_نام Types::HBGlyphID
		 آغاز_Glyph;	/* First GlyphID of the classValueArray */
  قسم_نام Types::سانچہ Array_کا<قسم_نام Types::HBUINT>
		class_قدر;	/* Array of Class Values--one per GlyphID */
  عوامی:
  DEFINE_سائز_ARRAY (2 + 2 * Types::سائز, class_قدر);
};

سانچہ <قسم_نام Types>
ساخت ClassDefFormat2_4
{
  دوست ساخت ClassDef;

  نجی:
  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id) ثابت
  {
    واپس range_ریکارڈ.bsearch (glyph_id).value;
  }

  غیرعلامتی get_population () ثابت
  {
    قسم_نام Types::large_int ret = 0;
    برائے (ثابت خودکار &r : range_ریکارڈ)
      ret += r.get_population ();
    واپس ret > UINT_MAX ? UINT_MAX : (غیرعلامتی) ret;
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    اگر (unlikely (!it))
    {
      classFormat = Types::سائز == 2 ? 2 : 4;
      range_ریکارڈ.len = 0;
      return_trace (سچ);
    }

    غیرعلامتی unsorted = جھوٹ;
    غیرعلامتی عدد_ranges = 1;
    hb_codepoint_t پچھلا_gid = (*it).پہلا;
    غیرعلامتی پچھلا_klass = (*it).second;

    Range_ریکارڈ<Types> range_rec;
    range_rec.پہلا = پچھلا_gid;
    range_rec.آخری = پچھلا_gid;
    range_rec.value = پچھلا_klass;

    خودکار *ریکارڈ = c->copy (range_rec);
    اگر (unlikely (!ریکارڈ)) return_trace (جھوٹ);

    برائے (ثابت خودکار gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.پہلا;
      غیرعلامتی cur_klass = gid_klass_pair.second;

      اگر (cur_gid != پچھلا_gid + 1 ||
	  cur_klass != پچھلا_klass)
      {

	اگر (unlikely (cur_gid < پچھلا_gid))
	  unsorted = سچ;

	اگر (unlikely (!ریکارڈ)) توڑو;
	ریکارڈ->آخری = پچھلا_gid;
	عدد_ranges++;

	range_rec.پہلا = cur_gid;
	range_rec.آخری = cur_gid;
	range_rec.value = cur_klass;

	ریکارڈ = c->copy (range_rec);
      }

      پچھلا_klass = cur_klass;
      پچھلا_gid = cur_gid;
    }

    اگر (unlikely (c->اندر_error ())) return_trace (جھوٹ);

    اگر (likely (ریکارڈ)) ریکارڈ->آخری = پچھلا_gid;
    range_ریکارڈ.len = عدد_ranges;

    اگر (unlikely (unsorted))
      range_ریکارڈ.as_array ().qsort (Range_ریکارڈ<Types>::cmp_range);

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالی_اشاریہ /*OUT*/,
               منطقی keep_خالی_خانے_جدول = سچ,
               منطقی use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyph_سیٹ = *c->plan->glyphset_gsub ();

    hb_sorted_سمتیہ_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سیٹ_t orig_klasses;

    اگر (glyph_سیٹ.get_population () * hb_ثنائی_ہندسہ_storage ((غیرعلامتی) range_ریکارڈ.len)
	< get_population ())
    {
      برائے (hb_codepoint_t g : glyph_سیٹ)
      {
	غیرعلامتی klass = get_class (g);
	اگر (!klass) جاری;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	اگر (new_gid == HB_MAP_قدر_INVALID) جاری;
	اگر (glyph_filter && !glyph_filter->has (g)) جاری;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.جوڑو (klass);
      }
    }
    ورنہ
    {
      غیرعلامتی عدد_ماخذ_glyphs = c->plan->ماخذ->get_عدد_glyphs ();
      برائے (خودکار &range : range_ریکارڈ)
      {
	غیرعلامتی klass = range.value;
	اگر (!klass) جاری;
	hb_codepoint_t آغاز = range.پہلا;
	hb_codepoint_t انتہا   = hb_min (range.آخری + 1, عدد_ماخذ_glyphs);
	برائے (hb_codepoint_t g = آغاز; g < انتہا; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  اگر (new_gid == HB_MAP_قدر_INVALID) جاری;
	  اگر (glyph_filter && !glyph_filter->has (g)) جاری;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.جوڑو (klass);
	}
      }
    }

    ثابت hb_سیٹ_t& glyphset = *c->plan->glyphset_gsub ();
    غیرعلامتی glyph_گنتی = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_گنتی <= glyph_and_klass.لمبائی;
    اگر (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (جھوٹ);
    return_trace (keep_خالی_خانے_جدول || (منطقی) glyph_and_klass);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (range_ریکارڈ.sanitize (c));
  }

  غیرعلامتی لاگت () ثابت { واپس hb_ثنائی_ہندسہ_storage ((غیرعلامتی) range_ریکارڈ.len); /* bsearch cost */ }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    برائے (خودکار &range : range_ریکارڈ)
      اگر (range.value)
	اگر (unlikely (!range.collect_coverage (glyphs)))
	  واپس جھوٹ;
    واپس سچ;
  }

  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_class (سیٹ_t *glyphs, غیرعلامتی صحیح klass) ثابت
  {
    برائے (خودکار &range : range_ریکارڈ)
    {
      اگر (range.value == klass)
	اگر (unlikely (!range.collect_coverage (glyphs)))
	  واپس جھوٹ;
    }
    واپس سچ;
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    اگر (range_ریکارڈ.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((غیرعلامتی) range_ریکارڈ.len))
    {
      برائے (خودکار g : *glyphs)
        اگر (get_class (g))
	  واپس سچ;
      واپس جھوٹ;
    }

    واپس hb_any (+ hb_iter (range_ریکارڈ)
                   | hb_map ([glyphs] (ثابت Range_ریکارڈ<Types> &range) { واپس range.intersects (*glyphs) && range.value; }));
  }
  منطقی intersects_class (ثابت hb_سیٹ_t *glyphs, غیرعلامتی klass) ثابت
  {
    اگر (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      اگر (!range_ریکارڈ.len)
        واپس !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
      hb_codepoint_t آخری = HB_سیٹ_قدر_INVALID;
      خودکار it = hb_iter (range_ریکارڈ);
      برائے (خودکار &range : it)
      {
        اگر (it->پہلا == آخری + 1)
	{
	  it++;
	  جاری;
	}

	اگر (!glyphs->next (&g))
	  توڑو;
	اگر (g < range.پہلا)
	  واپس سچ;
	g = range.آخری;
	آخری = g;
      }
      اگر (g != HB_سیٹ_قدر_INVALID && glyphs->next (&g))
	واپس سچ;
      /* Fall through. */
    }
    برائے (ثابت خودکار &range : range_ریکارڈ)
      اگر (range.value == klass && range.intersects (*glyphs))
	واپس سچ;
    واپس جھوٹ;
  }

  خالی intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, غیرعلامتی klass, hb_سیٹ_t *intersect_glyphs) ثابت
  {
    اگر (klass == 0)
    {
      hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
      برائے (خودکار &range : range_ریکارڈ)
      {
	اگر (!glyphs->next (&g))
	  جاؤ مکمل;
	جب (g < range.پہلا)
	{
	  intersect_glyphs->جوڑو (g);
	  اگر (!glyphs->next (&g))
	    جاؤ مکمل;
        }
        g = range.آخری;
      }
      جب (glyphs->next (&g))
	intersect_glyphs->جوڑو (g);
      مکمل:

      واپس;
    }

    غیرعلامتی گنتی = range_ریکارڈ.len;
    اگر (گنتی > glyphs->get_population () * hb_ثنائی_ہندسہ_storage (گنتی))
    {
      برائے (خودکار g : *glyphs)
      {
        غیرعلامتی i;
        اگر (range_ریکارڈ.as_array ().bfind (g, &i) &&
	    range_ریکارڈ.arrayZ[i].value == klass)
	  intersect_glyphs->جوڑو (g);
      }
      واپس;
    }

    برائے (خودکار &range : range_ریکارڈ)
    {
      اگر (range.value != klass) جاری;

      غیرعلامتی انتہا = range.آخری + 1;
      برائے (hb_codepoint_t g = range.پہلا - 1;
	   glyphs->next (&g) && g < انتہا;)
	intersect_glyphs->جوڑو (g);
    }
  }

  خالی intersected_classes (ثابت hb_سیٹ_t *glyphs, hb_سیٹ_t *intersect_classes) ثابت
  {
    اگر (glyphs->is_empty ()) واپس;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    اگر (!range_ریکارڈ.len)
    {
      intersect_classes->جوڑو (0);
      واپس;
    }

    hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
    برائے (خودکار &range : range_ریکارڈ)
    {
      اگر (!glyphs->next (&g))
        توڑو;
      اگر (g < range.پہلا)
      {
        intersect_classes->جوڑو (0);
        توڑو;
      }
      g = range.آخری;
    }
    اگر (g != HB_سیٹ_قدر_INVALID && glyphs->next (&g))
      intersect_classes->جوڑو (0);

    برائے (ثابت خودکار& range : range_ریکارڈ)
      اگر (range.intersects (*glyphs))
        intersect_classes->جوڑو (range.value);
  }

  محفوظ:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  قسم_نام Types::سانچہ Sorted_Array_کا<Range_ریکارڈ<Types>>
		range_ریکارڈ;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::سائز, range_ریکارڈ);
};

ساخت ClassDef
{
  /* Has interface. */
  غیرعلامتی عامل [] (hb_codepoint_t k) ثابت { واپس get (k); }
  منطقی has (hb_codepoint_t k) ثابت { واپس (*یہ)[k]; }
  /* Projection. */
  hb_codepoint_t عامل () (hb_codepoint_t k) ثابت { واپس get (k); }

  غیرعلامتی صحیح get (hb_codepoint_t k) ثابت { واپس get_class (k); }
  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_class (glyph_id);
    حالت 2: hb_barrier (); واپس u.format2.get_class (glyph_id);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.get_class (glyph_id);
    حالت 4: hb_barrier (); واپس u.format4.get_class (glyph_id);
#اگر_ختم
    طےشدہ:واپس 0;
    }
  }
  غیرعلامتی صحیح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    غیرعلامتی klass;
    اگر (cache && cache->get (glyph_id, &klass)) واپس klass;
    klass = get_class (glyph_id);
    اگر (cache) cache->سیٹ (glyph_id, klass);
    واپس klass;
  }

  غیرعلامتی get_population () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.get_population ();
    حالت 2: hb_barrier (); واپس u.format2.get_population ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.get_population ();
    حالت 4: hb_barrier (); واپس u.format4.get_population ();
#اگر_ختم
    طےشدہ:واپس NOT_COVERED;
    }
  }

  سانچہ<قسم_نام Iterator,
	   hb_requires (hb_is_sorted_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    خودکار glyphs = + it_with_class_zero | hb_filter (hb_دوسرا);

    غیرعلامتی format = 2;
    hb_codepoint_t glyph_max = 0;
    غیرعلامتی class_max = 0;
    اگر (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).پہلا;
      glyph_max = glyph_min;

      غیرعلامتی عدد_ranges = 1;
      hb_codepoint_t پچھلا_gid = glyph_min;
      غیرعلامتی پچھلا_klass = (*glyphs).second;

      برائے (ثابت خودکار gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.پہلا;
	غیرعلامتی cur_klass = gid_klass_pair.second;
	اگر (cur_klass > class_max) class_max = cur_klass;
	اگر (cur_gid == glyph_min) جاری;
        اگر (cur_gid > glyph_max) glyph_max = cur_gid;
	اگر (cur_gid != پچھلا_gid + 1 ||
	    cur_klass != پچھلا_klass)
	  عدد_ranges++;

	پچھلا_gid = cur_gid;
	پچھلا_klass = cur_klass;
      }

      uint64_t بہترین_سائز = UINT64_MAX;
      غیرعلامتی glyph_وسعت = glyph_max - glyph_min + 1;
      خودکار consider = [&] (غیرعلامتی candidate, uint64_t سائز)
      {
	اگر (سائز >= بہترین_سائز) واپس;
	بہترین_سائز = سائز;
	format = candidate;
      };

      اگر (glyph_max <= 0xFFFFu && glyph_وسعت <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_سائز * glyph_وسعت);
      اگر (glyph_max <= 0xFFFFu && عدد_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID16::static_سائز + HBUINT16::static_سائز));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
      اگر (glyph_max <= 0xFFFFFFu && glyph_وسعت <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_سائز * glyph_وسعت);
      اگر (glyph_max <= 0xFFFFFFu && عدد_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID24::static_سائز + HBUINT16::static_سائز));
#اگر_ختم
      اگر (unlikely (بہترین_سائز == UINT64_MAX))
      {
	c->check_success (جھوٹ, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز);
	return_trace (جھوٹ);
      }
    }

    u.format.v = format;

    انتخاب (u.format.v)
    {
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_دوسرا)));
    حالت 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_دوسرا)));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_دوسرا)));
    حالت 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_دوسرا)));
#اگر_ختم
    طےشدہ:return_trace (جھوٹ);
    }
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالی_اشاریہ /*OUT*/,
               منطقی keep_خالی_خانے_جدول = سچ,
               منطقی use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالی_اشاریہ) ثابت
  {
    TRACE_SUBSET (یہ);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_خالی_خانے_جدول, use_class_zero, glyph_filter));
    حالت 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_خالی_خانے_جدول, use_class_zero, glyph_filter));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_خالی_خانے_جدول, use_class_zero, glyph_filter));
    حالت 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_خالی_خانے_جدول, use_class_zero, glyph_filter));
#اگر_ختم
    طےشدہ:return_trace (جھوٹ);
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    حالت 2: hb_barrier (); return_trace (u.format2.sanitize (c));
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالت 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#اگر_ختم
    طےشدہ:return_trace (سچ);
    }
  }

  غیرعلامتی لاگت () ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.لاگت ();
    حالت 2: hb_barrier (); واپس u.format2.لاگت ();
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.لاگت ();
    حالت 4: hb_barrier (); واپس u.format4.لاگت ();
#اگر_ختم
    طےشدہ:واپس 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_coverage (سیٹ_t *glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.collect_coverage (glyphs);
    حالت 2: hb_barrier (); واپس u.format2.collect_coverage (glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.collect_coverage (glyphs);
    حالت 4: hb_barrier (); واپس u.format4.collect_coverage (glyphs);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچہ <قسم_نام سیٹ_t>
  منطقی collect_class (سیٹ_t *glyphs, غیرعلامتی صحیح klass) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.collect_class (glyphs, klass);
    حالت 2: hb_barrier (); واپس u.format2.collect_class (glyphs, klass);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.collect_class (glyphs, klass);
    حالت 4: hb_barrier (); واپس u.format4.collect_class (glyphs, klass);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.intersects (glyphs);
    حالت 2: hb_barrier (); واپس u.format2.intersects (glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersects (glyphs);
    حالت 4: hb_barrier (); واپس u.format4.intersects (glyphs);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }
  منطقی intersects_class (ثابت hb_سیٹ_t *glyphs, غیرعلامتی صحیح klass) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.intersects_class (glyphs, klass);
    حالت 2: hb_barrier (); واپس u.format2.intersects_class (glyphs, klass);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersects_class (glyphs, klass);
    حالت 4: hb_barrier (); واپس u.format4.intersects_class (glyphs, klass);
#اگر_ختم
    طےشدہ:واپس جھوٹ;
    }
  }

  خالی intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, غیرعلامتی klass, hb_سیٹ_t *intersect_glyphs) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالت 2: hb_barrier (); واپس u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالت 4: hb_barrier (); واپس u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#اگر_ختم
    طےشدہ:واپس;
    }
  }

  خالی intersected_classes (ثابت hb_سیٹ_t *glyphs, hb_سیٹ_t *intersect_classes) ثابت
  {
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); واپس u.format1.intersected_classes (glyphs, intersect_classes);
    حالت 2: hb_barrier (); واپس u.format2.intersected_classes (glyphs, intersect_classes);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    حالت 3: hb_barrier (); واپس u.format3.intersected_classes (glyphs, intersect_classes);
    حالت 4: hb_barrier (); واپس u.format4.intersected_classes (glyphs, intersect_classes);
#اگر_ختم
    طےشدہ:واپس;
    }
  }


  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};

سانچہ<قسم_نام Iterator>
ساکن اندرونی منطقی ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ واپس (c->آغاز_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
ساخت delta_صف_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  ساخت chars_t : hb_سمتیہ_t<uint8_t>
  {
    صحیح cmp (ثابت chars_t& other) ثابت
    {
      واپس as_array ().cmp (other.as_array ());
    }

    hb_pair_t<غیرعلامتی, غیرعلامتی> get_چوڑائی ()
    {
      غیرعلامتی چوڑائی = 0;
      غیرعلامتی columns = 0;
      برائے (غیرعلامتی i = 0; i < لمبائی; i++)
      {
	غیرعلامتی v = arrayZ[i];
	چوڑائی += v;
	columns += (v != 0);
      }
      واپس hb_pair (چوڑائی, columns);
    }

    HB_HOT
    hb_pair_t<غیرعلامتی, غیرعلامتی> combine_چوڑائی (ثابت chars_t& other) ثابت
    {
      غیرعلامتی combined_چوڑائی = 0;
      غیرعلامتی combined_columns = 0;
      برائے (غیرعلامتی i = 0; i < لمبائی; i++)
      {
        غیرعلامتی v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_چوڑائی += v;
	combined_columns += (v != 0);
      }
      واپس hb_pair (combined_چوڑائی, combined_columns);
    }
  };

  hb_pair_t<غیرعلامتی, غیرعلامتی> combine_چوڑائی (ثابت delta_صف_encoding_t& other_encoding) ثابت { واپس chars.combine_چوڑائی (other_encoding.chars); }

  // Actual data

  chars_t chars;
  غیرعلامتی چوڑائی = 0;
  غیرعلامتی overhead = 0;
  hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح>*> اشیاء;

  delta_صف_encoding_t () = طےشدہ;
  delta_صف_encoding_t (hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح>*> &&صفیں, غیرعلامتی عدد_ستون_تعداد)
  {
    أكد (صفیں);

    اشیاء = std::move (صفیں);

    اگر (unlikely (!chars.resize (عدد_ستون_تعداد)))
      واپس;

    calculate_chars ();
  }

  خالی ملاؤ (ثابت delta_صف_encoding_t& other)
  {
    اشیاء.alloc (اشیاء.لمبائی + other.اشیاء.لمبائی);
    برائے (خودکار &صف : other.اشیاء)
      جوڑو_صف (صف);

    // Merge chars
    أكد (chars.لمبائی == other.chars.لمبائی);
    برائے (غیرعلامتی i = 0; i < chars.لمبائی; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_بدلا ();
  }

  خالی chars_بدلا ()
  {
    خودکار _ = chars.get_چوڑائی ();
    چوڑائی = _.پہلا;
    overhead = get_chars_overhead (_.second);
  }

  خالی calculate_chars ()
  {
    أكد (اشیاء);

    منطقی long_الفاظ = جھوٹ;

    برائے (خودکار &صف : اشیاء)
    {
      أكد (صف->لمبائی == chars.لمبائی);

      /* 0/1/2 byte encoding */
      برائے (غیرعلامتی i = 0; i < صف->لمبائی; i++)
      {
	صحیح v =  صف->arrayZ[i];
	اگر (v == 0)
	  جاری;
	ورنہ اگر (v > 32767 || v < -32768)
	{
	  long_الفاظ = سچ;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	ورنہ اگر (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	ورنہ
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    اگر (long_الفاظ)
    {
      // Convert 1s to 2s
      برائے (خودکار &v : chars)
	اگر (v == 1)
	  v = 2;
    }

    chars_بدلا ();
  }

  منطقی is_empty () ثابت
  { واپس !اشیاء; }

  منطقی اندر_error () ثابت
  { واپس chars.اندر_error () || اشیاء.اندر_error (); }

  ساکن اندرونی غیرعلامتی get_chars_overhead (غیرعلامتی عدد_columns)
  {
    غیرعلامتی c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    واپس c + عدد_columns * 2;
  }

  غیرعلامتی get_gain (غیرعلامتی additional_بائٹیں_فی_صفیں = 1) ثابت
  {
    صحیح گنتی = اشیاء.لمبائی;
    واپس hb_max (0, (صحیح) overhead - گنتی * (صحیح) additional_بائٹیں_فی_صفیں);
  }

  صحیح gain_from_merging (ثابت delta_صف_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    علامتی additional_بائٹیں_فی_صفیں = other_encoding.چوڑائی - چوڑائی;
    اگر (additional_بائٹیں_فی_صفیں > 0)
    {
      اگر (get_gain (additional_بائٹیں_فی_صفیں) == 0)
        واپس 0;
    }
    ورنہ
    {
      اگر (other_encoding.get_gain (-additional_بائٹیں_فی_صفیں) == 0)
	واپس 0;
    }

    خودکار pair = combine_چوڑائی (other_encoding);
    غیرعلامتی combined_چوڑائی = pair.پہلا;
    غیرعلامتی combined_columns = pair.second;

    صحیح combined_gain = (صحیح) overhead + (صحیح) other_encoding.overhead;
    combined_gain -= (combined_چوڑائی - (صحیح) چوڑائی) * اشیاء.لمبائی;
    combined_gain -= (combined_چوڑائی - (صحیح) other_encoding.چوڑائی) * other_encoding.اشیاء.لمبائی;
    combined_gain -= get_chars_overhead (combined_columns);

    واپس combined_gain;
  }

  منطقی جوڑو_صف (ثابت hb_سمتیہ_t<صحیح>* صف)
  { واپس اشیاء.push (صف); }

  ساکن صحیح cmp (ثابت خالی *pa, ثابت خالی *pb)
  {
    ثابت delta_صف_encoding_t *a = (ثابت delta_صف_encoding_t *)pa;
    ثابت delta_صف_encoding_t *b = (ثابت delta_صف_encoding_t *)pb;

    اگر (a->چوڑائی != b->چوڑائی)
      واپس (صحیح) a->چوڑائی - (صحیح) b->چوڑائی;

    واپس b->chars.cmp (a->chars);
  }
};

ساخت VarRegionAxis
{
  اعشاری evaluate (صحیح coord) ثابت
  {
    صحیح peak = peakCoord.مقصد_int ();
    اگر (peak == 0 || coord == peak)
      واپس 1.f;
    ورنہ اگر (coord == 0) // Faster
      واپس 0.f;

    صحیح آغاز = آغاز_Coord.مقصد_int (), انتہا = انتہا_Coord.مقصد_int ();

    /* TODO Move these to sanitize(). */
    اگر (unlikely (آغاز > peak || peak > انتہا))
      واپس 1.f;
    اگر (unlikely (آغاز < 0 && انتہا > 0))
      واپس 1.f;

    اگر (coord <= آغاز || انتہا <= coord)
      واپس 0.f;

    /* Interpolate */
    اگر (coord < peak)
      واپس اعشاری (coord - آغاز) / (peak - آغاز);
    ورنہ
      واپس اعشاری (انتہا - coord) / (انتہا - peak);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  منطقی serialize (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (یہ));
  }

  عوامی:
  F2DOT14	آغاز_Coord;
  F2DOT14	peakCoord;
  F2DOT14	انتہا_Coord;
  عوامی:
  DEFINE_سائز_STATIC (6);
};
ساخت SparseVarRegionAxis
{
  اعشاری evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len) ثابت
  {
    غیرعلامتی i = axis_اشاریہ_2;
    صحیح coord = i < coord_len ? coords[i] : 0;
    واپس axis.evaluate (coord);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت SparseVarRegionAxis *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->embed (src);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    اگر (axis_map)
    {
      اگر (unlikely (!axis_map->has (برآمد->axis_اشاریہ_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (جھوٹ);
      }
      اگر (unlikely (!c->check_تفویض (برآمد->axis_اشاریہ_2, axis_map->get (برآمد->axis_اشاریہ_2),
				    HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
	return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  عوامی:
  HBUINT16 axis_اشاریہ_2;
  VarRegionAxis axis;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت hb_scalar_cache_t
{
  نجی:
  ساکن constexpr غیرعلامتی STATIC_لمبائی = 128;
  ساکن constexpr اعشاری INVALID = NAN;

  عوامی:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = حذف;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = حذف;
  hb_scalar_cache_t& عامل= (ثابت hb_scalar_cache_t&) = حذف;
  hb_scalar_cache_t& عامل= (hb_scalar_cache_t&&) = حذف;

  ساکن hb_scalar_cache_t *create (غیرعلامتی صحیح گنتی,
				    hb_scalar_cache_t *scratch_cache = خالی_اشاریہ)
  {
    اگر (!گنتی) واپس (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    اگر (scratch_cache && گنتی <= STATIC_لمبائی)
    {
      scratch_cache->لمبائی = گنتی;
      scratch_cache->clear ();
      واپس scratch_cache;
    }

    خودکار *cache = (hb_scalar_cache_t *) hb_malloc (حجم (hb_scalar_cache_t) - حجم (static_اقدار) + حجم (static_اقدار[0]) * گنتی);
    اگر (unlikely (!cache)) واپس (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->لمبائی = گنتی;
    cache->clear ();

    واپس cache;
  }

  ساکن خالی destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = خالی_اشاریہ)
  {
    اگر (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  خالی clear ()
  {
    خودکار *values = &static_اقدار[0];
    غیرعلامتی i = 0;
#اگر_غیر_تعریف_شدہ HB_OPTIMIZE_سائز
    برائے (; i + 3 < لمبائی; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#اگر_ختم
    برائے (; i < لمبائی; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقی get (غیرعلامتی i, اعشاری *value) ثابت
  {
    اگر (unlikely (i >= لمبائی))
    {
      *value = 0.f;
      واپس سچ;
    }
    خودکار *values = &static_اقدار[0];
    خودکار *cached_قدر = &values[i];
    اعشاری v = *cached_قدر;
    اگر (std::isnan (v))
      واپس جھوٹ;
    *value = v;
    واپس سچ;
  }

  HB_ALWAYS_INLINE
  خالی سیٹ (غیرعلامتی i, اعشاری value)
  {
    اگر (unlikely (i >= لمبائی)) واپس;
    خودکار *values = &static_اقدار[0];
    خودکار *cached_قدر = &values[i];
    *cached_قدر = value;
  }

  عوامی:
  غیرعلامتی لمبائی;
  نجی:
  تغیرپذیر hb_atomic_t<اعشاری> static_اقدار[STATIC_لمبائی];
};

ساخت VarRegionList
{
  نجی:
  اعشاری evaluate_impl (غیرعلامتی صحیح region_اشاریہ,
		       ثابت صحیح *coords, غیرعلامتی صحیح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_اشاریہ * axis_گنتی_2);
    اعشاری v = 1.f;

    غیرعلامتی صحیح گنتی = axis_گنتی_2;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      صحیح coord = i < coord_len ? coords[i] : 0;
      اعشاری ضربی_عدد = axes[i].evaluate (coord);
      اگر (ضربی_عدد == 0.f)
      {
	v = 0.f;
	توڑو;
      }
      v *= ضربی_عدد;
    }

    واپس v;
  }

  عوامی:
  HB_ALWAYS_INLINE
  اعشاری evaluate (غیرعلامتی صحیح region_اشاریہ,
		  ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		  hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (region_اشاریہ >= region_گنتی))
      واپس 0.;

    اعشاری v;
    اگر (cache && cache->get (region_اشاریہ, &v))
      واپس v;

    v = evaluate_impl (region_اشاریہ, coords, coord_len);

    اگر (cache)
      cache->سیٹ (region_اشاریہ, v);
    واپس v;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_گنتی_2 * region_گنتی));
  }

  منطقی serialize (hb_serialize_context_t *c,
                  ثابت hb_سمتیہ_t<hb_لیبل_t>& axis_لیبل_فہرست,
                  ثابت hb_سمتیہ_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی axis_گنتی = axis_لیبل_فہرست.لمبائی;
    غیرعلامتی region_گنتی_2 = regions.لمبائی;
    اگر (!axis_گنتی || !region_گنتی_2) return_trace (جھوٹ);
    اگر (unlikely (hb_unsigned_mul_overflows (axis_گنتی * region_گنتی_2,
                                             VarRegionAxis::static_سائز))) return_trace (جھوٹ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    axis_گنتی_2 = axis_گنتی;
    region_گنتی = region_گنتی_2;

    برائے (غیرعلامتی r = 0; r < region_گنتی_2; r++)
    {
      ثابت خودکار& region = regions[r];
      برائے (غیرعلامتی i = 0; i < axis_گنتی; i++)
      {
        hb_لیبل_t لیبل_2 = axis_لیبل_فہرست.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        اگر (region->has (لیبل_2, &coords))
        {
          var_region_rec.آغاز_Coord.سیٹ_float (coords->minimum);
          var_region_rec.peakCoord.سیٹ_float (coords->درمیان);
          var_region_rec.انتہا_Coord.سیٹ_float (coords->زیادہ_سے_زیادہ);
        }
        ورنہ
        {
          var_region_rec.آغاز_Coord.سیٹ_int (0);
          var_region_rec.peakCoord.سیٹ_int (0);
          var_region_rec.انتہا_Coord.سیٹ_int (0);
        }
        اگر (!var_region_rec.serialize (c))
          return_trace (جھوٹ);
      }
    }
    return_trace (سچ);
  }

  منطقی serialize (hb_serialize_context_t *c, ثابت VarRegionList *src, ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    axis_گنتی_2 = src->axis_گنتی_2;
    region_گنتی = region_map.get_population ();
    اگر (unlikely (hb_unsigned_mul_overflows (axis_گنتی_2 * region_گنتی,
					     VarRegionAxis::static_سائز))) return_trace (جھوٹ);
    اگر (unlikely (!c->extend (یہ))) return_trace (جھوٹ);
    غیرعلامتی صحیح region_گنتی_2 = src->region_گنتی;
    برائے (غیرعلامتی صحیح r = 0; r < region_گنتی; r++)
    {
      غیرعلامتی صحیح backward = region_map.backward (r);
      اگر (backward >= region_گنتی_2) return_trace (جھوٹ);
      hb_memcpy (&axesZ[axis_گنتی_2 * r], &src->axesZ[axis_گنتی_2 * backward], VarRegionAxis::static_سائز * axis_گنتی_2);
    }

    return_trace (سچ);
  }

  منطقی get_var_region (غیرعلامتی region_اشاریہ,
                       ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
                       hb_hashmap_t<hb_لیبل_t, Triple>& axis_tuples /*برآمد*/) ثابت
  {
    اگر (region_اشاریہ >= region_گنتی) واپس جھوٹ;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_اشاریہ * axis_گنتی_2);
    برائے (غیرعلامتی i = 0; i < axis_گنتی_2; i++)
    {
      hb_لیبل_t *axis_لیبل_2;
      اگر (!axes_old_اشاریہ_لیبل_map.has (i, &axis_لیبل_2))
        واپس جھوٹ;

      اعشاری min_val = axis_region->آغاز_Coord.مقصد_float ();
      اعشاری def_val = axis_region->peakCoord.مقصد_float ();
      اعشاری max_val = axis_region->انتہا_Coord.مقصد_float ();

      اگر (def_val != 0.f)
        axis_tuples.سیٹ (*axis_لیبل_2, Triple ((دوہرا) min_val, (دوہرا) def_val, (دوہرا) max_val));
      axis_region++;
    }
    واپس !axis_tuples.اندر_error ();
  }

  منطقی get_var_regions (ثابت hb_map_t& axes_old_اشاریہ_لیبل_map,
                        hb_سمتیہ_t<hb_hashmap_t<hb_لیبل_t, Triple>>& regions /*برآمد*/) ثابت
  {
    اگر (!regions.alloc (region_گنتی))
      واپس جھوٹ;

    برائے (غیرعلامتی i = 0; i < region_گنتی; i++)
    {
      hb_hashmap_t<hb_لیبل_t, Triple> axis_tuples;
      اگر (!get_var_region (i, axes_old_اشاریہ_لیبل_map, axis_tuples))
        واپس جھوٹ;
      regions.push (std::move (axis_tuples));
    }
    واپس !regions.اندر_error ();
  }

  size_t get_سائز () ثابت
  { واپس hb_unsigned_جوڑو_saturate (min_سائز,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_سائز,
							       axis_گنتی_2, region_گنتی)); }

  عوامی:
  HBUINT16	axis_گنتی_2;
  HBUINT15	region_گنتی;
  محفوظ:
  Unsized_Array_کا<VarRegionAxis>
		axesZ;
  عوامی:
  DEFINE_سائز_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ساکن اندرونی منطقی
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_لیبل_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_لیبل_t, Triple> &reachable_ranges)
{
  برائے (خودکار _ : axis_tuples)
  {
    Triple *range;
    اگر (!reachable_ranges.has (_.پہلا, &range)) جاری;
    ثابت Triple &tent = _.second;
    صحیح آغاز = (صحیح) roundf ((اعشاری) tent.minimum * 16384.f);
    صحیح peak  = (صحیح) roundf ((اعشاری) tent.درمیان * 16384.f);
    صحیح انتہا   = (صحیح) roundf ((اعشاری) tent.زیادہ_سے_زیادہ * 16384.f);
    اگر (!peak) جاری;
    اگر (آغاز > peak || peak > انتہا) جاری;
    اگر (آغاز < 0 && انتہا > 0) جاری;
    صحیح lo = (صحیح) roundf ((اعشاری) range->minimum * 16384.f);
    صحیح hi = (صحیح) roundf ((اعشاری) range->زیادہ_سے_زیادہ * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    اگر (hi < آغاز || lo > انتہا)
      واپس سچ;
  }
  واپس جھوٹ;
}

ساخت SparseVariationRegion
{
  اعشاری evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len) ثابت
  {
    اعشاری v = 1.f;
    غیرعلامتی صحیح گنتی = axes.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اعشاری ضربی_عدد = (یہ+axes[i]).evaluate (coords, coord_len);
      اگر (ضربی_عدد == 0.f)
	واپس 0.;
      v *= ضربی_عدد;
    }
    واپس v;
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت SparseVariationRegion *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!axes.serialize (c, (غیرعلامتی) src->axes.len))) return_trace (جھوٹ);
    برائے (غیرعلامتی i = 0; i < axes.len; i++)
      اگر (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (جھوٹ);
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (axes.sanitize (c, یہ));
  }

  محفوظ:
  Array16_کا<Offset32_مقصد<SparseVarRegionAxis>> axes;
  عوامی:
  DEFINE_سائز_ARRAY (2, axes);
};

ساخت SparseVarRegionList
{
  HB_ALWAYS_INLINE
  اعشاری evaluate (غیرعلامتی صحیح region_اشاریہ,
		  ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		  hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (region_اشاریہ >= regions.len))
      واپس 0.;

    اعشاری v;
    اگر (cache && cache->get (region_اشاریہ, &v))
      واپس v;

    ثابت SparseVariationRegion &region = یہ+regions[region_اشاریہ];

    v = region.evaluate (coords, coord_len);
    اگر (cache)
      cache->سیٹ (region_اشاریہ, v);

    واپس v;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (regions.sanitize (c, یہ));
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت SparseVarRegionList *src,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    regions.len = region_map.get_population ();
    اگر (unlikely (!c->extend (regions))) return_trace (جھوٹ);

    برائے (غیرعلامتی i = 0; i < regions.len; i++)
    {
      غیرعلامتی old_اشاریہ = region_map.backward (i);
      اگر (unlikely (old_اشاریہ >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_اشاریہ]),
					     axis_map)))
	return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  عوامی:
  Array32_کا<Offset32_مقصد<SparseVariationRegion>>
		regions;
  عوامی:
  DEFINE_سائز_ARRAY (4, regions);
};


ساخت Var_ڈیٹا
{
  غیرعلامتی صحیح get_شے_گنتی () ثابت
  { واپس شے_گنتی; }

  غیرعلامتی صحیح get_region_اشاریہ_گنتی () ثابت
  { واپس regionIndices.len; }

  غیرعلامتی get_region_اشاریہ (غیرعلامتی i) ثابت
  { واپس i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_صف_سائز () ثابت
  { واپس (لفظ_گنتی () + regionIndices.len) * (long_الفاظ_2 () ? 2 : 1); }

  size_t get_سائز () ثابت
  { واپس hb_unsigned_جوڑو_saturate (min_سائز - regionIndices.min_سائز,
				     regionIndices.get_سائز (),
				     hb_unsigned_mul_saturate (شے_گنتی, get_صف_سائز ())); }

  سانچہ <قسم_نام Float = اعشاری>
  Float _get_delta (غیرعلامتی صحیح inner,
		    ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    اگر (unlikely (inner >= شے_گنتی))
      واپس 0.;
    منطقی is_long = long_الفاظ_2 ();
    غیرعلامتی صحیح گنتی = regionIndices.len;
    غیرعلامتی لفظ_گنتی_2 = لفظ_گنتی ();
    غیرعلامتی صحیح scount = is_long ? گنتی : لفظ_گنتی_2;
    غیرعلامتی صحیح lcount = is_long ? لفظ_گنتی_2 : 0;

    ثابت HBUINT8 *بائٹیں = get_delta_بائٹیں ();
    ثابت HBUINT8 *صف = بائٹیں + inner * get_صف_سائز ();

    Float delta = 0;
    غیرعلامتی صحیح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (صف);
    برائے (; i < lcount; i++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی, cache);
      اگر (scalar)
        delta += (Float) scalar * (صحیح) *lcursor;
      lcursor++;
    }
    ثابت HBINT16 *scursor = reinterpret_cast<ثابت HBINT16 *> (lcursor);
    برائے (; i < scount; i++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی, cache);
      اگر (scalar)
       delta += (Float) scalar * (صحیح) *scursor;
      scursor++;
    }
    ثابت HBINT8 *bcursor = reinterpret_cast<ثابت HBINT8 *> (scursor);
    برائے (; i < گنتی; i++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی, cache);
      اگر (scalar)
        delta += (Float) scalar * (صحیح) *bcursor;
      bcursor++;
    }

    واپس delta;
  }

  سانچہ <قسم_نام Float = اعشاری>
  HB_ALWAYS_INLINE
  Float get_delta (غیرعلامتی صحیح inner,
		   ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    غیرعلامتی صحیح گنتی = regionIndices.len;
    اگر (!گنتی) واپس Float (0); // This is quite common, so optimize it.
    واپس _get_delta<Float> (inner, coords, coord_گنتی, regions, cache);
  }

  خالی get_region_scalars (ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
			   ثابت VarRegionList &regions,
			   اعشاری *scalars /*OUT */,
			   غیرعلامتی صحیح عدد_scalars) ثابت
  {
    غیرعلامتی گنتی = hb_min (عدد_scalars, regionIndices.len);
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی);
    برائے (غیرعلامتی صحیح i = گنتی; i < عدد_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  لفظ_گنتی () <= regionIndices.len &&
		  c->check_range (get_delta_بائٹیں (),
				  شے_گنتی,
				  get_صف_سائز ()));
  }

  منطقی serialize (hb_serialize_context_t *c,
                  منطقی has_long,
                  ثابت hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحیح>*>& صفیں)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی صف_گنتی_3 = صفیں.لمبائی;
    اگر (!صف_گنتی_3) {
      // Nothing to serialize, will be empty.
      واپس جھوٹ;
    }

    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    اگر (unlikely (!c->check_تفویض (شے_گنتی, صف_گنتی_3,
                                    HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
      return_trace (جھوٹ);

    صحیح min_حد = has_long ? -32768 : -128;
    صحیح max_حد = has_long ? +32767 : +127;
    شمار delta_سائز_t { kZero=0, k_Non_لفظ, k_لفظ };
    hb_سمتیہ_t<delta_سائز_t> delta_sz;
    غیرعلامتی عدد_regions = صفیں[0]->لمبائی;
    اگر (!delta_sz.resize (عدد_regions))
      return_trace (جھوٹ);

    غیرعلامتی لفظ_گنتی_2 = 0;
    برائے (غیرعلامتی r = 0; r < عدد_regions; r++)
    {
      برائے (غیرعلامتی i = 0; i < صف_گنتی_3; i++)
      {
        صحیح delta = صفیں[i]->arrayZ[r];
        اگر (delta < min_حد || delta > max_حد)
        {
          delta_sz[r] = k_لفظ;
          لفظ_گنتی_2++;
          توڑو;
        }
        ورنہ اگر (delta != 0)
        {
          delta_sz[r] = k_Non_لفظ;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    غیرعلامتی لفظ_اشاریہ = 0;
    غیرعلامتی non_لفظ_اشاریہ = لفظ_گنتی_2;
    hb_map_t ri_map;
    برائے (غیرعلامتی r = 0; r < عدد_regions; r++)
    {
      اگر (!delta_sz[r]) جاری;
      غیرعلامتی new_r = (delta_sz[r] == k_لفظ)? لفظ_اشاریہ++ : non_لفظ_اشاریہ++;
      اگر (!ri_map.سیٹ (new_r, r))
        return_trace (جھوٹ);
    }

    لفظ_سائز_گنتی = لفظ_گنتی_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    غیرعلامتی ri_گنتی = ri_map.get_population ();
    regionIndices.len = ri_گنتی;
    اگر (unlikely (!c->extend (یہ))) return_trace (جھوٹ);

    برائے (غیرعلامتی r = 0; r < ri_گنتی; r++)
    {
      hb_codepoint_t *idx;
      اگر (!ri_map.has (r, &idx))
        return_trace (جھوٹ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_بائٹیں = get_delta_بائٹیں ();
    غیرعلامتی صف_سائز = get_صف_سائز ();
    برائے (غیرعلامتی صحیح i = 0; i < صف_گنتی_3; i++)
    {
      برائے (غیرعلامتی صحیح r = 0; r < ri_گنتی; r++)
      {
        صحیح delta = صفیں[i]->arrayZ[ri_map[r]];
        سیٹ_شے_delta_fast (i, r, delta, delta_بائٹیں, صف_سائز);
      }
    }
    return_trace (سچ);
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Var_ڈیٹا *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_سیٹ_t *culled_regions = خالی_اشاریہ)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    شے_گنتی = inner_map.get_اگلا_قدر ();

    /* Optimize word count */
    غیرعلامتی ri_گنتی = src->regionIndices.len;
    شمار delta_سائز_t { kZero=0, k_Non_لفظ, k_لفظ };
    hb_سمتیہ_t<delta_سائز_t> delta_sz;
    hb_سمتیہ_t<غیرعلامتی صحیح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_گنتی);
    ri_map.resize (ri_گنتی);
    غیرعلامتی صحیح new_لفظ_گنتی = 0;
    غیرعلامتی صحیح r;

    ثابت HBUINT8 *src_delta_بائٹیں = src->get_delta_بائٹیں ();
    غیرعلامتی src_صف_سائز = src->get_صف_سائز ();
    غیرعلامتی src_لفظ_گنتی = src->لفظ_گنتی ();
    منطقی     src_long_الفاظ = src->long_الفاظ_2 ();

    منطقی has_long = جھوٹ;
    اگر (src_long_الفاظ)
    {
      برائے (r = 0; r < src_لفظ_گنتی; r++)
      {
        برائے (غیرعلامتی old_gid : inner_map.کلیدیں())
	{
	  int32_t delta = src->get_شے_delta_fast (old_gid, r, src_delta_بائٹیں, src_صف_سائز);
	  اگر (delta < -32768 || 32767 < delta)
	  {
	    has_long = سچ;
	    توڑو;
	  }
        }
      }
    }

    علامتی min_حد = has_long ? -32768 : -128;
    علامتی max_حد = has_long ? +32767 : +127;
    برائے (r = 0; r < ri_گنتی; r++)
    {
      منطقی short_circuit = src_long_الفاظ == has_long && src_لفظ_گنتی <= r;

      delta_sz[r] = kZero;
      اگر (culled_regions && culled_regions->has (src->regionIndices[r]))
	جاری; /* unreachable region: drop its column */
      برائے (غیرعلامتی old_gid : inner_map.کلیدیں())
      {
	int32_t delta = src->get_شے_delta_fast (old_gid, r, src_delta_بائٹیں, src_صف_سائز);
	اگر (delta < min_حد || max_حد < delta)
	{
	  delta_sz[r] = k_لفظ;
	  new_لفظ_گنتی++;
	  توڑو;
	}
	ورنہ اگر (delta != 0)
	{
	  delta_sz[r] = k_Non_لفظ;
	  اگر (short_circuit)
	    توڑو;
	}
      }
    }

    غیرعلامتی صحیح لفظ_اشاریہ = 0;
    غیرعلامتی صحیح non_لفظ_اشاریہ = new_لفظ_گنتی;
    غیرعلامتی صحیح new_ri_گنتی = 0;
    برائے (r = 0; r < ri_گنتی; r++)
      اگر (delta_sz[r])
      {
	غیرعلامتی new_r = (delta_sz[r] == k_لفظ)? لفظ_اشاریہ++ : non_لفظ_اشاریہ++;
	ri_map[new_r] = r;
	new_ri_گنتی++;
      }

    لفظ_سائز_گنتی = new_لفظ_گنتی | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_گنتی;

    اگر (unlikely (!c->extend (یہ))) return_trace (جھوٹ);

    برائے (r = 0; r < new_ri_گنتی; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_بائٹیں = get_delta_بائٹیں ();
    غیرعلامتی صف_سائز = get_صف_سائز ();
    غیرعلامتی گنتی = شے_گنتی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      غیرعلامتی صحیح old = inner_map.backward (i);
      برائے (غیرعلامتی صحیح r = 0; r < new_ri_گنتی; r++)
	سیٹ_شے_delta_fast (i, r,
			     src->get_شے_delta_fast (old, ri_map[r],
						       src_delta_بائٹیں, src_صف_سائز),
			     delta_بائٹیں, صف_سائز);
    }

    return_trace (سچ);
  }

  خالی collect_region_refs (hb_سیٹ_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_بائٹیں = get_delta_بائٹیں ();
    غیرعلامتی صف_سائز = get_صف_سائز ();

    برائے (غیرعلامتی صحیح r = 0; r < regionIndices.len; r++)
    {
      غیرعلامتی صحیح region = regionIndices.arrayZ[r];
      اگر (region_indices.has (region)) جاری;
      برائے (hb_codepoint_t old_gid : inner_map.کلیدیں())
	اگر (get_شے_delta_fast (old_gid, r, delta_بائٹیں, صف_سائز) != 0)
	{
	  region_indices.جوڑو (region);
	  توڑو;
	}
    }
  }

  عوامی:
  ثابت HBUINT8 *get_delta_بائٹیں () ثابت
  { واپس &StructAfter<HBUINT8> (regionIndices); }

  محفوظ:
  HBUINT8 *get_delta_بائٹیں ()
  { واپس &StructAfter<HBUINT8> (regionIndices); }

  عوامی:
  int32_t get_شے_delta_fast (غیرعلامتی صحیح شے, غیرعلامتی صحیح region,
			       ثابت HBUINT8 *delta_بائٹیں, غیرعلامتی صف_سائز) ثابت
  {
    اگر (unlikely (شے >= شے_گنتی || region >= regionIndices.len)) واپس 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_بائٹیں + شے * صف_سائز;
    غیرعلامتی لفظ_گنتی_2 = لفظ_گنتی ();
    منطقی is_long = long_الفاظ_2 ();
    اگر (is_long)
    {
      اگر (region < لفظ_گنتی_2)
	واپس ((ثابت HBINT32 *) p)[region];
      ورنہ
	واپس ((ثابت HBINT16 *)(p + HBINT32::static_سائز * لفظ_گنتی_2))[region - لفظ_گنتی_2];
    }
    ورنہ
    {
      اگر (region < لفظ_گنتی_2)
	واپس ((ثابت HBINT16 *) p)[region];
      ورنہ
	واپس (p + HBINT16::static_سائز * لفظ_گنتی_2)[region - لفظ_گنتی_2];
    }
  }
  int32_t get_شے_delta (غیرعلامتی صحیح شے, غیرعلامتی صحیح region) ثابت
  {
     واپس get_شے_delta_fast (شے, region,
				 get_delta_بائٹیں (),
				 get_صف_سائز ());
  }

  محفوظ:
  خالی سیٹ_شے_delta_fast (غیرعلامتی صحیح شے, غیرعلامتی صحیح region, int32_t delta,
			    HBUINT8 *delta_بائٹیں, غیرعلامتی صف_سائز)
  {
    HBINT8 *p = (HBINT8 *) delta_بائٹیں + شے * صف_سائز;
    غیرعلامتی لفظ_گنتی_2 = لفظ_گنتی ();
    منطقی is_long = long_الفاظ_2 ();
    اگر (is_long)
    {
      اگر (region < لفظ_گنتی_2)
	((HBINT32 *) p)[region] = delta;
      ورنہ
	((HBINT16 *)(p + HBINT32::static_سائز * لفظ_گنتی_2))[region - لفظ_گنتی_2] = delta;
    }
    ورنہ
    {
      اگر (region < لفظ_گنتی_2)
	((HBINT16 *) p)[region] = delta;
      ورنہ
	(p + HBINT16::static_سائز * لفظ_گنتی_2)[region - لفظ_گنتی_2] = delta;
    }
  }
  خالی سیٹ_شے_delta (غیرعلامتی صحیح شے, غیرعلامتی صحیح region, int32_t delta)
  {
    سیٹ_شے_delta_fast (شے, region, delta,
			 get_delta_بائٹیں (),
			 get_صف_سائز ());
  }

  منطقی long_الفاظ_2 () ثابت { واپس لفظ_سائز_گنتی & 0x8000u /* LONG_WORDS */; }
  غیرعلامتی لفظ_گنتی () ثابت { واپس لفظ_سائز_گنتی & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محفوظ:
  HBUINT16		شے_گنتی;
  HBUINT16		لفظ_سائز_گنتی;
  Array16_کا<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عوامی:
  DEFINE_سائز_ARRAY (6, regionIndices);
};

ساخت Multi_Var_ڈیٹا
{
  size_t get_سائز () ثابت
  { واپس hb_unsigned_جوڑو_saturate (min_سائز - regionIndices.min_سائز,
				     regionIndices.get_سائز ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { واپس یہ+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  خالی get_delta (غیرعلامتی صحیح inner,
		  ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<دوہرا> برآمد,
		  hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    خودکار &deltaSets = get_delta_sets ();

    خودکار اقدار_iter = deltaSets.fetcher (inner);
    غیرعلامتی region_گنتی = regionIndices.len;
    غیرعلامتی skip = 0;
    برائے (غیرعلامتی region_اشاریہ_2 = 0; region_اشاریہ_2 < region_گنتی; region_اشاریہ_2++)
    {
      اعشاری scalar = regions.evaluate (regionIndices.arrayZ[region_اشاریہ_2],
				       coords, coord_گنتی,
				       cache);
      // We skip lazily. Helps with the tail end.
      اگر (scalar == 0.0f)
        skip += برآمد.لمبائی;
      ورنہ
      {
        اگر (skip)
	{
	  اقدار_iter.skip (skip);
	  skip = 0;
	}
	اقدار_iter.جوڑو_مقصد_2 (برآمد, scalar);
      }
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (format.sanitize (c) &&
		  hb_barrier () &&
		  format == 1 &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  StructAfter<decltype (deltaSetsX)> (regionIndices).sanitize (c, یہ));
  }

  غیرعلامتی get_شے_گنتی () ثابت
  { واپس get_delta_sets ().گنتی; }

  خالی collect_region_refs (hb_سیٹ_t &region_indices) ثابت
  {
    برائے (غیرعلامتی region : regionIndices)
      region_indices.جوڑو (region);
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Multi_Var_ڈیٹا *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی region_گنتی_2 = src->regionIndices.len;
    غیرعلامتی سرخی_سائز = HBUINT8::static_سائز +
			   Array16_کا<HBUINT16>::min_سائز +
			   region_گنتی_2 * HBUINT16::static_سائز +
			   HBUINT32::static_سائز;
    اگر (unlikely (!c->extend_سائز (یہ, سرخی_سائز, جھوٹ)))
      return_trace (جھوٹ);
    format = 1;
    regionIndices.len = region_گنتی_2;
    برائے (غیرعلامتی i = 0; i < regionIndices.len; i++)
    {
      غیرعلامتی old_region = src->regionIndices[i];
      اگر (unlikely (!region_map.has (old_region)))
	return_trace (جھوٹ);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت خودکار &src_delta_sets = src->get_delta_sets ();
    hb_سمتیہ_t<hb_ubytes_t> delta_sets;
    غیرعلامتی ڈیٹا_سائز = 0;
    اگر (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (جھوٹ);
    برائے (غیرعلامتی i = 0; i < inner_map.get_population (); i++)
    {
      غیرعلامتی old_inner = inner_map.backward (i);
      اگر (unlikely (old_inner >= src_delta_sets.گنتی)) return_trace (جھوٹ);
      hb_ubytes_t بائٹیں = static_cast<ثابت CFF2_اشاریہ &> (src_delta_sets)[old_inner];
      ڈیٹا_سائز = hb_unsigned_جوڑو_saturate (ڈیٹا_سائز, بائٹیں.لمبائی);
      delta_sets.push (بائٹیں);
    }
    اگر (unlikely (ڈیٹا_سائز == UINT_MAX || delta_sets.اندر_error ()))
      return_trace (جھوٹ);

    خودکار &برآمد_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (برآمد_delta_sets.serialize_serialize (c, delta_sets.iter (), &ڈیٹا_سائز));
  }

  محفوظ:
  HBUINT8	      format; // 1
  Array16_کا<HBUINT16> regionIndices;
  Offset32_مقصد<TupleList> deltaSetsX;
  عوامی:
  DEFINE_سائز_MIN (7);
};

ساخت شے_Variation_Store
{
  دوست ساخت شے_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس hb_scalar_cache_t::create (0);
#اگر_ختم
    واپس hb_scalar_cache_t::create ((یہ+regions).region_گنتی);
  }

  ساکن خالی destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  نجی:
  سانچہ <قسم_نام Float = اعشاری>
  Float get_delta (غیرعلامتی صحیح outer, غیرعلامتی صحیح inner,
		   ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		   hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس Float (0);
#اگر_ختم

    اگر (unlikely (outer >= ڈیٹا_Sets.len))
      واپس Float (0);

    واپس (یہ+ڈیٹا_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_گنتی,
					     یہ+regions,
					     cache);
  }

  عوامی:
  منطقی has_delta_سیٹ (غیرعلامتی صحیح اشاریہ_2) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس جھوٹ;
#اگر_ختم

    غیرعلامتی صحیح outer = اشاریہ_2 >> 16;
    غیرعلامتی صحیح inner = اشاریہ_2 & 0xFFFF;
    واپس outer < ڈیٹا_Sets.len &&
	   inner < (یہ+ڈیٹا_Sets[outer]).get_شے_گنتی ();
  }

  سانچہ <قسم_نام Float = اعشاری>
  Float get_delta (غیرعلامتی صحیح اشاریہ_2,
		   ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		   hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    غیرعلامتی صحیح outer = اشاریہ_2 >> 16;
    غیرعلامتی صحیح inner = اشاریہ_2 & 0xFFFF;
    واپس get_delta<Float> (outer, inner, coords, coord_گنتی, cache);
  }
  سانچہ <قسم_نام Float = اعشاری>
  Float get_delta (غیرعلامتی صحیح اشاریہ_2,
		   hb_array_t<ثابت صحیح> coords,
		   hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    واپس get_delta<Float> (اشاریہ_2,
		      coords.arrayZ, coords.لمبائی,
		      cache);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس سچ;
#اگر_ختم

    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, یہ) &&
		  ڈیٹا_Sets.sanitize (c, یہ));
  }

  منطقی serialize (hb_serialize_context_t *c,
                  منطقی has_long,
                  ثابت hb_سمتیہ_t<hb_لیبل_t>& axis_لیبل_فہرست,
                  ثابت hb_سمتیہ_t<ثابت hb_hashmap_t<hb_لیبل_t, Triple>*>& region_list,
                  ثابت hb_سمتیہ_t<delta_صف_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (یہ);
#اگر_تعریف_شدہ HB_NO_VAR
    return_trace (جھوٹ);
#اگر_ختم
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    format = 1;
    اگر (!regions.serialize_serialize (c, axis_لیبل_فہرست, region_list))
      return_trace (جھوٹ);

    غیرعلامتی عدد_var_ڈیٹا = vardata_encodings.لمبائی;
    اگر (!عدد_var_ڈیٹا) return_trace (جھوٹ);
    اگر (unlikely (!c->check_تفویض (ڈیٹا_Sets.len, عدد_var_ڈیٹا,
                                    HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
      return_trace (جھوٹ);

    اگر (unlikely (!c->extend (ڈیٹا_Sets))) return_trace (جھوٹ);
    برائے (غیرعلامتی i = 0; i < عدد_var_ڈیٹا; i++)
      اگر (!ڈیٹا_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].اشیاء))
        return_trace (جھوٹ);

    return_trace (سچ);
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت شے_Variation_Store *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_سیٹ_t *culled_regions = خالی_اشاریہ)
  {
    TRACE_SERIALIZE (یہ);
#اگر_تعریف_شدہ HB_NO_VAR
    return_trace (جھوٹ);
#اگر_ختم

    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    غیرعلامتی صحیح سیٹ_گنتی = 0;
    برائے (غیرعلامتی صحیح i = 0; i < inner_maps.لمبائی; i++)
      اگر (inner_maps[i].get_population ())
	سیٹ_گنتی++;

    format = 1;

    ثابت خودکار &src_regions = src+src->regions;

    hb_سیٹ_t region_indices;
    برائے (غیرعلامتی صحیح i = 0; i < inner_maps.لمبائی; i++)
      (src+src->ڈیٹا_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    اگر (region_indices.اندر_error ())
      return_trace (جھوٹ);

    region_indices.del_range ((src_regions).region_گنتی, hb_سیٹ_t::INVALID);

    اگر (culled_regions)
      region_indices.تفریق (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (غیرعلامتی _) { region_map.جوڑو(_); })
    ;
    اگر (region_map.اندر_error())
      return_trace (جھوٹ);

    اگر (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (جھوٹ);

    ڈیٹا_Sets.len = سیٹ_گنتی;
    اگر (unlikely (!c->extend (ڈیٹا_Sets))) return_trace (جھوٹ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    غیرعلامتی صحیح سیٹ_اشاریہ = 0;
    برائے (غیرعلامتی صحیح i = 0; i < inner_maps.لمبائی; i++)
    {
      اگر (!inner_maps[i].get_population ()) جاری;
      اگر (unlikely (!ڈیٹا_Sets[سیٹ_اشاریہ++]
		     .serialize_serialize (c, &(src+src->ڈیٹا_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  شے_Variation_Store *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->آغاز_embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (خالی_اشاریہ);

    hb_سمتیہ_t <hb_inc_bimap_t> inner_maps;
    غیرعلامتی گنتی = ڈیٹا_Sets.len;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      اگر (unlikely (!c->propagate_error(inner_maps)))
        return_trace(خالی_اشاریہ);
      خودکار &ڈیٹا = یہ+ڈیٹا_Sets[i];

      غیرعلامتی شے_گنتی = ڈیٹا.get_شے_گنتی ();
      برائے (غیرعلامتی j = 0; j < شے_گنتی; j++)
	map->جوڑو (j);
    }

    اگر (unlikely (!برآمد->serialize (c, یہ, inner_maps))) return_trace (خالی_اشاریہ);

    return_trace (برآمد);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقی collect_dead_regions (ثابت hb_map_t &axes_old_اشاریہ_لیبل_map,
			     ثابت hb_hashmap_t<hb_لیبل_t, Triple> &reachable_ranges,
			     hb_سیٹ_t &dead_regions /*برآمد*/) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (!reachable_ranges.get_population ()) واپس سچ;
    ثابت VarRegionList &region_list = یہ+regions;
    غیرعلامتی گنتی = region_list.region_گنتی;
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      hb_hashmap_t<hb_لیبل_t, Triple> axis_tuples;
      اگر (!region_list.get_var_region (i, axes_old_اشاریہ_لیبل_map, axis_tuples))
	واپس جھوٹ;
      اگر (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.جوڑو (i);
    }
    واپس !dead_regions.اندر_error ();
#ورنہ
    واپس سچ;
#اگر_ختم
  }

  منطقی subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (یہ);
#اگر_تعریف_شدہ HB_NO_VAR
    return_trace (جھوٹ);
#اگر_ختم

    شے_Variation_Store *varstore_مفرد = c->serializer->آغاز_embed<شے_Variation_Store> ();
    اگر (unlikely (!varstore_مفرد)) return_trace (جھوٹ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_سیٹ_t dead_regions;
    اگر (c->plan->has_avar2)
      اگر (!collect_dead_regions (c->plan->axes_old_اشاریہ_لیبل_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (جھوٹ);

    varstore_مفرد->serialize (c->serializer, یہ, inner_maps,
			       dead_regions.get_population () ? &dead_regions : خالی_اشاریہ);

    return_trace (
        !c->serializer->اندر_error()
        && varstore_مفرد->ڈیٹا_Sets);
  }

  غیرعلامتی صحیح get_region_اشاریہ_گنتی (غیرعلامتی صحیح major) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس 0;
#اگر_ختم
    واپس (یہ+ڈیٹا_Sets[major]).get_region_اشاریہ_گنتی ();
  }

  خالی get_region_scalars (غیرعلامتی صحیح major,
			   ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
			   اعشاری *scalars /*OUT*/,
			   غیرعلامتی صحیح عدد_scalars) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    برائے (غیرعلامتی i = 0; i < عدد_scalars; i++)
      scalars[i] = 0.f;
    واپس;
#اگر_ختم

    (یہ+ڈیٹا_Sets[major]).get_region_scalars (coords, coord_گنتی,
					       یہ+regions,
					       &scalars[0], عدد_scalars);
  }

  غیرعلامتی صحیح get_sub_جدول_گنتی () ثابت
   {
#اگر_تعریف_شدہ HB_NO_VAR
     واپس 0;
#اگر_ختم
     واپس ڈیٹا_Sets.len;
   }

  ثابت Var_ڈیٹا& get_sub_جدول (غیرعلامتی i) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
     واپس Null (Var_ڈیٹا);
#اگر_ختم
     واپس یہ+ڈیٹا_Sets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
     واپس Null (VarRegionList);
#اگر_ختم
     واپس یہ+regions;
  }

  محفوظ:
  HBUINT16				format;
  Offset32_مقصد<VarRegionList>		regions;
  Array16_کا_Offset32_مقصد<Var_ڈیٹا>		ڈیٹا_Sets;
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (8, ڈیٹا_Sets);
};

ساخت Multi_شے_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = خالی_اشاریہ) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس hb_scalar_cache_t::create (0);
#اگر_ختم
    خودکار &r = یہ+regions;
    غیرعلامتی گنتی = r.regions.len;

    واپس hb_scalar_cache_t::create (گنتی, static_cache);
  }

  ساکن خالی destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = خالی_اشاریہ)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  نجی:
  خالی get_delta (غیرعلامتی صحیح outer, غیرعلامتی صحیح inner,
		  ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		  hb_array_t<دوہرا> برآمد,
		  hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس;
#اگر_ختم

    اگر (unlikely (outer >= ڈیٹا_Sets.len))
      واپس;

    واپس (یہ+ڈیٹا_Sets[outer]).get_delta (inner,
					     coords, coord_گنتی,
					     یہ+regions,
					     برآمد,
					     cache);
  }

  عوامی:
  خالی get_delta (غیرعلامتی صحیح اشاریہ_2,
		  ثابت صحیح *coords, غیرعلامتی صحیح coord_گنتی,
		  hb_array_t<دوہرا> برآمد,
		  hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    غیرعلامتی صحیح outer = اشاریہ_2 >> 16;
    غیرعلامتی صحیح inner = اشاریہ_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_گنتی, برآمد, cache);
  }
  خالی get_delta (غیرعلامتی صحیح اشاریہ_2,
		  hb_array_t<ثابت صحیح> coords,
		  hb_array_t<دوہرا> برآمد,
		  hb_scalar_cache_t *cache = خالی_اشاریہ) ثابت
  {
    واپس get_delta (اشاریہ_2,
		      coords.arrayZ, coords.لمبائی,
		      برآمد,
		      cache);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
#اگر_تعریف_شدہ HB_NO_VAR
    واپس سچ;
#اگر_ختم

    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, یہ) &&
		  ڈیٹا_Sets.sanitize (c, یہ));
  }

  منطقی create_subset_plan (ثابت hb_سیٹ_t &var_indices,
			   hb_سمتیہ_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    اگر (unlikely (!inner_maps->resize (ڈیٹا_Sets.len))) واپس جھوٹ;
    برائے (hb_codepoint_t var_idx : var_indices)
    {
      غیرعلامتی outer = var_idx >> 16;
      غیرعلامتی inner = var_idx & 0xFFFFu;
      اگر (unlikely (outer >= ڈیٹا_Sets.len ||
		    inner >= (یہ+ڈیٹا_Sets[outer]).get_شے_گنتی ()))
	واپس جھوٹ;
      (*inner_maps)[outer].جوڑو (inner);
    }

    غیرعلامتی new_outer = 0;
    برائے (غیرعلامتی outer = 0; outer < inner_maps->لمبائی; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      اگر (!inner_map.get_population ()) جاری;
      اگر (unlikely (inner_map.اندر_error ())) واپس جھوٹ;
      برائے (غیرعلامتی new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	غیرعلامتی old_inner = inner_map.backward (new_inner);
	varidx_map->سیٹ ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    واپس !inner_maps->اندر_error () && !varidx_map->اندر_error ();
  }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Multi_شے_Variation_Store *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ) ||
		  inner_maps.لمبائی < src->ڈیٹا_Sets.len))
      return_trace (جھوٹ);
    format = 1;

    ثابت خودکار &src_regions = src+src->regions;
    hb_سیٹ_t region_indices;
    غیرعلامتی سیٹ_گنتی = 0;
    برائے (غیرعلامتی i = 0; i < src->ڈیٹا_Sets.len; i++)
    {
      اگر (!inner_maps[i].get_population ()) جاری;
      سیٹ_گنتی++;
      (src+src->ڈیٹا_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_سیٹ_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.جوڑو_سیٹ (&region_indices);
    اگر (unlikely (region_indices.اندر_error () || region_map.اندر_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (جھوٹ);

    ڈیٹا_Sets.len = سیٹ_گنتی;
    اگر (unlikely (!c->extend (ڈیٹا_Sets))) return_trace (جھوٹ);
    غیرعلامتی سیٹ_اشاریہ = 0;
    برائے (غیرعلامتی i = 0; i < src->ڈیٹا_Sets.len; i++)
    {
      اگر (!inner_maps[i].get_population ()) جاری;
      اگر (unlikely (!ڈیٹا_Sets[سیٹ_اشاریہ++].serialize_serialize (
			    c, &(src+src->ڈیٹا_Sets[i]), inner_maps[i], region_map)))
	return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  محفوظ:
  HBUINT16				format; // 1
  Offset32_مقصد<SparseVarRegionList>	regions;
  Array16_کا_Offset32_مقصد<Multi_Var_ڈیٹا>	ڈیٹا_Sets;
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (8, ڈیٹا_Sets);
};

سانچہ <قسم_نام Map_گنتی_T>
ساخت Delta_سیٹ_اشاریہ_Map_Format01
{
  دوست ساخت Delta_سیٹ_اشاریہ_Map;

  size_t get_سائز () ثابت
  { واپس hb_unsigned_mul_جوڑو_saturate (map_گنتی_2, get_چوڑائی (), min_سائز); }

  نجی:
  Delta_سیٹ_اشاریہ_Map_Format01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (یہ));
  }

  سانچہ <قسم_نام T>
  منطقی serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    غیرعلامتی صحیح چوڑائی = plan.get_چوڑائی ();
    غیرعلامتی صحیح inner_ثنائی_ہندسہ_گنتی = plan.get_inner_ثنائی_ہندسہ_گنتی ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (یہ);
    اگر (unlikely (output_map.لمبائی && ((((inner_ثنائی_ہندسہ_گنتی-1)&~0xF)!=0) || (((چوڑائی-1)&~0x3)!=0))))
      return_trace (جھوٹ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);

    اندراج_Format = ((چوڑائی-1)<<4)|(inner_ثنائی_ہندسہ_گنتی-1);
    map_گنتی_2 = output_map.لمبائی;
    HBUINT8 *p = c->allocate_سائز<HBUINT8> (چوڑائی * output_map.لمبائی);
    اگر (unlikely (!p)) return_trace (جھوٹ);
    برائے (غیرعلامتی صحیح i = 0; i < output_map.لمبائی; i++)
    {
      غیرعلامتی صحیح v = output_map.arrayZ[i];
      اگر (v)
      {
	غیرعلامتی صحیح outer = v >> 16;
	غیرعلامتی صحیح inner = v & 0xFFFF;
	غیرعلامتی صحیح u = (outer << inner_ثنائی_ہندسہ_گنتی) | inner;
	برائے (غیرعلامتی صحیح w = چوڑائی; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += چوڑائی;
    }
    return_trace (سچ);
  }

  HB_ALWAYS_INLINE
  uint32_t map (غیرعلامتی صحیح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    اگر (!map_گنتی_2)
      واپس v;
    واپس _map (v);
  }

  HB_HOT
  uint32_t _map (غیرعلامتی صحیح v) ثابت /* Returns 16.16 outer.inner. */
  {
    اگر (v >= map_گنتی_2)
      v = map_گنتی_2 - 1;

    غیرعلامتی صحیح u = 0;
    { /* Fetch it. */
      غیرعلامتی صحیح w = get_چوڑائی ();
      ثابت HBUINT8 *p = map_ڈیٹا_Z.arrayZ + w * v;
      برائے (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      غیرعلامتی صحیح n = get_inner_ثنائی_ہندسہ_گنتی ();
      غیرعلامتی صحیح outer = u >> n;
      غیرعلامتی صحیح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    واپس u;
  }

  غیرعلامتی get_map_گنتی () ثابت       { واپس map_گنتی_2; }
  غیرعلامتی get_چوڑائی () ثابت           { واپس ((اندراج_Format >> 4) & 3) + 1; }
  غیرعلامتی get_inner_ثنائی_ہندسہ_گنتی () ثابت { واپس (اندراج_Format & 0xF) + 1; }


  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  hb_barrier () &&
                  c->check_range (map_ڈیٹا_Z.arrayZ,
                                  map_گنتی_2,
                                  get_چوڑائی ()));
  }

  محفوظ:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       اندراج_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_گنتی_T     map_گنتی_2;       /* The number of mapping entries. */
  Unsized_Array_کا<HBUINT8>
                map_ڈیٹا_Z;       /* The delta-set index mapping data. */

  عوامی:
  DEFINE_سائز_ARRAY (2+Map_گنتی_T::static_سائز, map_ڈیٹا_Z);
};

ساخت Delta_سیٹ_اشاریہ_Map
{
  سانچہ <قسم_نام T>
  منطقی serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (یہ);
    غیرعلامتی لمبائی = plan.get_output_map ().لمبائی;
    u.format.v = لمبائی <= 0xFFFF ? 0 : 1;
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالت 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    طےشدہ:return_trace (جھوٹ);
    }
  }

  uint32_t map (غیرعلامتی v) ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); واپس (u.format0.map (v));
    حالت 1: hb_barrier (); واپس (u.format1.map (v));
    طےشدہ:واپس v;
    }
  }

  غیرعلامتی get_map_گنتی () ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); واپس u.format0.get_map_گنتی ();
    حالت 1: hb_barrier (); واپس u.format1.get_map_گنتی ();
    طےشدہ:واپس 0;
    }
  }

  غیرعلامتی get_چوڑائی () ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); واپس u.format0.get_چوڑائی ();
    حالت 1: hb_barrier (); واپس u.format1.get_چوڑائی ();
    طےشدہ:واپس 0;
    }
  }

  غیرعلامتی get_inner_ثنائی_ہندسہ_گنتی () ثابت
  {
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); واپس u.format0.get_inner_ثنائی_ہندسہ_گنتی ();
    حالت 1: hb_barrier (); واپس u.format1.get_inner_ثنائی_ہندسہ_گنتی ();
    طےشدہ:واپس 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.format.v.sanitize (c)) return_trace (جھوٹ);
    hb_barrier ();
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (u.format0.sanitize (c));
    حالت 1: hb_barrier (); return_trace (u.format1.sanitize (c));
    طےشدہ:return_trace (سچ);
    }
  }

  Delta_سیٹ_اشاریہ_Map* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    انتخاب (u.format.v) {
    حالت 0: hb_barrier (); return_trace (reinterpret_cast<Delta_سیٹ_اشاریہ_Map *> (u.format0.copy (c)));
    حالت 1: hb_barrier (); return_trace (reinterpret_cast<Delta_سیٹ_اشاریہ_Map *> (u.format1.copy (c)));
    طےشدہ:return_trace (خالی_اشاریہ);
    }
  }

  محفوظ:
  اتحاد {
  ساخت { HBUINT8 v; }             format;         /* Format identifier */
  Delta_سیٹ_اشاریہ_Map_Format01<HBUINT16> format0;
  Delta_سیٹ_اشاریہ_Map_Format01<HBUINT32> format1;
  } u;
  عوامی:
  DEFINE_سائز_UNION (1, format.v);
};


ساخت شے_Var_Store_Instancer
{
  شے_Var_Store_Instancer (ثابت شے_Variation_Store *varStore_,
			 ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap,
			 hb_array_t<ثابت صحیح> coords,
			 hb_scalar_cache_t *cache = خالی_اشاریہ) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    اگر (!varStore)
      varStore = &Null(شے_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    اگر (!hb_any (coords))
      یہ->coords = {};
  }

  عامل منطقی () ثابت { واپس varStore && منطقی (coords); }

  اعشاری عامل[] (uint32_t varIdx) ثابت
  { واپس (*یہ) (varIdx); }

  اعشاری عامل() (uint32_t varIdx, غیرعلامتی مختصر انحراف = 0) ثابت
  { واپس get_delta (varIdx, انحراف); }

  دوہرا get_condition_delta (uint32_t varIdx) ثابت
  { واپس get_delta<دوہرا> (varIdx); }

  سانچہ <قسم_نام Float = اعشاری>
  Float get_delta (uint32_t varIdx, غیرعلامتی مختصر انحراف = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   اگر (!coords || varIdx == VarIdx::NO_VARIATION)
     واپس Float (0);

    varIdx += انحراف;
    اگر (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    واپس varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت شے_Variation_Store *varStore;
  ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap;
  hb_array_t<ثابت صحیح> coords;
  hb_scalar_cache_t *cache;
};

ساخت Multi_شے_Var_Store_Instancer
{
  Multi_شے_Var_Store_Instancer (ثابت Multi_شے_Variation_Store *varStore,
			      ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap,
			      hb_array_t<ثابت صحیح> coords,
			      hb_scalar_cache_t *cache = خالی_اشاریہ) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    اگر (!varStore)
      varStore = &Null(Multi_شے_Variation_Store);
  }

  عامل منطقی () ثابت { واپس varStore; }

  دوہرا get_condition_delta (uint32_t varIdx) ثابت
  { واپس (دوہرا) (*یہ)[varIdx]; }

  دوہرا عامل[] (uint32_t varIdx) ثابت
  {
    دوہرا v = 0;
    (*یہ) (hb_array (&v, 1), varIdx);
    واپس v;
  }

  خالی عامل() (hb_array_t<دوہرا> برآمد, uint32_t varIdx, غیرعلامتی مختصر انحراف = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    اگر (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += انحراف;
      اگر (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, برآمد, cache);
    }
    ورنہ
      برائے (غیرعلامتی i = 0; i < برآمد.لمبائی; i++)
        برآمد.arrayZ[i] = 0.;
  }

  ثابت Multi_شے_Variation_Store *varStore;
  ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap;
  hb_array_t<ثابت صحیح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
شمار Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_ریکارڈ_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_ریکارڈ_WITH_VAR = 3,
};

شمار lookup_condition_subset_نتیجہ_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

ساخت Condition;

سانچہ <قسم_نام Instancer>
ساکن منطقی
_hb_recurse_condition_evaluate (ثابت ساخت Condition &condition,
				ثابت صحیح *coords,
				غیرعلامتی صحیح coord_len,
				Instancer *instancer);

ساخت ConditionAxisRange
{
  دوست ساخت Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    ثابت hb_map_t *اشاریہ_map = &c->plan->axes_اشاریہ_map;
    اگر (اشاریہ_map->is_empty ()) return_trace (سچ);

    ثابت hb_map_t& axes_old_اشاریہ_لیبل_map = c->plan->axes_old_اشاریہ_لیبل_map;
    hb_codepoint_t *axis_لیبل_2;
    اگر (!axes_old_اشاریہ_لیبل_map.has (axis_اشاریہ_2, &axis_لیبل_2) ||
        !اشاریہ_map->has (axis_اشاریہ_2))
      return_trace (جھوٹ);

    ثابت hb_hashmap_t<hb_لیبل_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_حد_بندی{-1.0, 0.0, 1.0};
    Triple *normalized_حد_بندی;
    اگر (normalized_axes_location.has (*axis_لیبل_2, &normalized_حد_بندی))
      axis_حد_بندی = *normalized_حد_بندی;

    ثابت hb_hashmap_t<hb_لیبل_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    اگر (axes_triple_distances.has (*axis_لیبل_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    اعشاری normalized_min = renormalize_قدر ((دوہرا) filter_Range_Min_قدر.مقصد_float (), axis_حد_بندی, axis_triple_distances, جھوٹ);
    اعشاری normalized_max = renormalize_قدر ((دوہرا) filter_Range_Max_قدر.مقصد_float (), axis_حد_بندی, axis_triple_distances, جھوٹ);
    برآمد->filter_Range_Min_قدر.سیٹ_float (normalized_min);
    برآمد->filter_Range_Max_قدر.سیٹ_float (normalized_max);

    return_trace (c->serializer->check_تفویض (برآمد->axis_اشاریہ_2, اشاریہ_map->get (axis_اشاریہ_2),
                                               HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز));
  }

  نجی:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*برآمد*/) ثابت
  {
    //invalid axis index, drop the entire record
    اگر (!c->axes_اشاریہ_لیبل_map->has (axis_اشاریہ_2))
      واپس DROP_ریکارڈ_WITH_VAR;

    hb_لیبل_t axis_لیبل_2 = c->axes_اشاریہ_لیبل_map->get (axis_اشاریہ_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_حد_بندی;
    منطقی axis_سیٹ_by_user = جھوٹ;
    اگر (c->axes_location->has (axis_لیبل_2, &axis_حد_بندی))
    {
      axis_range = *axis_حد_بندی;
      axis_سیٹ_by_user = سچ;
    }

    اعشاری axis_min_val = axis_range.minimum;
    اعشاری axis_default_val = axis_range.درمیان;
    اعشاری axis_max_val = axis_range.زیادہ_سے_زیادہ;

    اعشاری filter_min_val = filter_Range_Min_قدر.مقصد_float ();
    اعشاری filter_max_val = filter_Range_Max_قدر.مقصد_float ();

    اگر (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = جھوٹ;

    //condition not met, drop the entire record
    اگر (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      واپس DROP_ریکارڈ_WITH_VAR;

    //condition met and axis pinned, drop the condition
    اگر (axis_سیٹ_by_user && axis_range.is_نقطہ ())
      واپس DROP_COND_WITH_VAR;

    اگر (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_قدر.مقصد_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_قدر.مقصد_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->سیٹ (axis_اشاریہ_2, val);
      واپس KEEP_COND_WITH_VAR;
    }
    واپس KEEP_ریکارڈ_WITH_VAR;
  }

  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 Instancer *instancer HB_غیر_مستعمل) ثابت
  {
    صحیح coord = axis_اشاریہ_2 < coord_len ? coords[axis_اشاریہ_2] : 0;
    واپس filter_Range_Min_قدر.مقصد_int () <= coord && coord <= filter_Range_Max_قدر.مقصد_int ();
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_اشاریہ_2;
  F2DOT14	filter_Range_Min_قدر;
  F2DOT14	filter_Range_Max_قدر;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Condition_قدر
{
  دوست ساخت Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    اگر (varIdx == VarIdx::NO_VARIATION)
      return_trace (سچ);

    hb_pair_t<غیرعلامتی, صحیح> *condition_قدر;
    اگر (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) یہ, &condition_قدر))
    {
      برآمد->default_قدر_2 = condition_قدر->second;
      برآمد->varIdx = condition_قدر->پہلا;
      return_trace (سچ);
    }

    hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
    اگر (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (جھوٹ);

    اگر (unlikely (!c->serializer->check_تفویض (
		برآمد->default_قدر_2,
		default_قدر_2 + hb_دوسرا (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
      return_trace (جھوٹ);
    برآمد->varIdx = hb_پہلا (*new_varidx_delta);
    return_trace (سچ);
  }

  نجی:
  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    دوہرا value = default_قدر_2;
    value += instancer->get_condition_delta (varIdx);
    واپس value > 0;
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 2 */
  HBINT16	default_قدر_2;   /* Value at default instance. */
  VarIdx	varIdx;		/* Variation index */
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت ConditionAnd
{
  دوست ساخت Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
      return_trace (جھوٹ);
    برآمد->format = format;

    برائے (ثابت خودکار &انحراف : conditions)
    {
      اگر (انحراف.is_null ())
      {
	خودکار *برآمد_انحراف = برآمد->conditions.serialize_append (c->serializer);
	اگر (unlikely (!برآمد_انحراف)) return_trace (جھوٹ);
	*برآمد_انحراف = 0;
      }
      ورنہ اگر (unlikely (!subset_انحراف_array (c, برآمد->conditions, یہ) (انحراف)))
	return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  نجی:
  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    غیرعلامتی صحیح گنتی = conditions.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!_hb_recurse_condition_evaluate (یہ+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	واپس جھوٹ;
    واپس سچ;
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (conditions.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8_کا_Offset24_مقصد<ساخت Condition>	conditions;
  عوامی:
  DEFINE_سائز_ARRAY (3, conditions);
};

ساخت ConditionOr
{
  دوست ساخت Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
      return_trace (جھوٹ);
    برآمد->format = format;

    برائے (ثابت خودکار &انحراف : conditions)
    {
      اگر (انحراف.is_null ())
      {
	خودکار *برآمد_انحراف = برآمد->conditions.serialize_append (c->serializer);
	اگر (unlikely (!برآمد_انحراف)) return_trace (جھوٹ);
	*برآمد_انحراف = 0;
      }
      ورنہ اگر (unlikely (!subset_انحراف_array (c, برآمد->conditions, یہ) (انحراف)))
	return_trace (جھوٹ);
    }

    return_trace (سچ);
  }

  نجی:
  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    غیرعلامتی صحیح گنتی = conditions.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (_hb_recurse_condition_evaluate (یہ+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	واپس سچ;
    واپس جھوٹ;
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (conditions.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 4 */
  Array8_کا_Offset24_مقصد<ساخت Condition>	conditions;
  عوامی:
  DEFINE_سائز_ARRAY (3, conditions);
};

ساخت ConditionNegate
{
  دوست ساخت Condition;

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);
    اگر (condition.is_null ())
    {
      برآمد->condition = 0;
      return_trace (سچ);
    }
    return_trace (برآمد->condition.serialize_subset (c, condition, یہ));
  }

  نجی:
  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    واپس !_hb_recurse_condition_evaluate (یہ+condition,
					    coords, coord_len,
					    instancer);
  }

  عوامی:
  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (condition.sanitize (c, یہ));
  }

  محفوظ:
  HBUINT16	format;		/* Format identifier--format = 5 */
  Offset24_مقصد<ساخت Condition>	condition;
  عوامی:
  DEFINE_سائز_STATIC (5);
};

ساخت Condition
{
  دوست ساخت Condition_سیٹ;

  ساکن منطقی serialize_constant (hb_serialize_context_t *c, منطقی value)
  {
    خودکار *برآمد = c->آغاز_embed<Condition_قدر> ();
    اگر (unlikely (!برآمد || !c->extend_min (برآمد))) واپس جھوٹ;
    برآمد->format = 2;
    برآمد->default_قدر_2 = value ? 1 : 0;
    برآمد->varIdx = VarIdx::NO_VARIATION;
    واپس سچ;
  }

  lookup_condition_subset_نتیجہ_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      غیرعلامتی گہرائی = HB_MAX_NESTING_LEVEL,
				      منطقی materialize_true = جھوٹ) ثابت;

  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 Instancer *instancer) ثابت
  {
    انتخاب (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    حالت 0: واپس سچ;
    حالت 1: hb_barrier (); واپس u.format1.evaluate (coords, coord_len, instancer);
    حالت 2: hb_barrier (); واپس u.format2.evaluate (coords, coord_len, instancer);
    حالت 3: hb_barrier (); واپس u.format3.evaluate (coords, coord_len, instancer);
    حالت 4: hb_barrier (); واپس u.format4.evaluate (coords, coord_len, instancer);
    حالت 5: hb_barrier (); واپس u.format5.evaluate (coords, coord_len, instancer);
    طےشدہ:واپس جھوٹ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*برآمد*/,
                                             غیرعلامتی گہرائی = HB_MAX_NESTING_LEVEL) ثابت
  {
    اگر (unlikely (!گہرائی))
    {
      c->apply = جھوٹ;
      واپس DROP_ریکارڈ_WITH_VAR;
    }

    انتخاب (u.format.v) {
    حالت 0: واپس KEEP_ریکارڈ_WITH_VAR;
    حالت 1: hb_barrier (); واپس u.format1.keep_with_variations (c, condition_map);
    حالت 2: حالت 3: حالت 4: حالت 5: توڑو;
    طےشدہ: c->apply = جھوٹ; واپس DROP_ریکارڈ_WITH_VAR;
    }

    hb_barrier ();
    منطقی applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.لمبائی, c->instancer);
    c->apply &= applies;
    اگر (c->all_axes_pinned)
      واپس applies ? DROP_COND_WITH_VAR : DROP_ریکارڈ_WITH_VAR;

    اگر (u.format.v == 2)
      واپس u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_سیٹ (u.format2.varIdx) ?
             (applies ? KEEP_ریکارڈ_WITH_VAR : DROP_ریکارڈ_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    خودکار classify_بچہ = [c, condition_map, گہرائی] (ثابت Condition &بچہ)
    {
      خودکار بچہ_context = *c;
      بچہ_context.apply = سچ;
      واپس بچہ.keep_with_variations (&بچہ_context, condition_map, گہرائی - 1);
    };

    اگر (u.format.v == 5)
    {
      خودکار نتیجہ = classify_بچہ (&u.format5 + u.format5.condition);
      واپس نتیجہ == DROP_ریکارڈ_WITH_VAR ? KEEP_ریکارڈ_WITH_VAR :
             نتیجہ == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_ریکارڈ_WITH_VAR;
    }

    منطقی is_and = u.format.v == 3;
    منطقی variable = جھوٹ;
    ثابت خودکار &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت خالی *بنیاد = is_and ? (ثابت خالی *) &u.format3 : (ثابت خالی *) &u.format4;
    برائے (ثابت خودکار &انحراف : conditions)
    {
      خودکار نتیجہ = classify_بچہ (بنیاد + انحراف);
      اگر (is_and && نتیجہ == DROP_ریکارڈ_WITH_VAR)
        واپس DROP_ریکارڈ_WITH_VAR;
      اگر (!is_and && (نتیجہ == DROP_COND_WITH_VAR || نتیجہ == KEEP_ریکارڈ_WITH_VAR))
        واپس KEEP_ریکارڈ_WITH_VAR;
      variable |= نتیجہ == KEEP_COND_WITH_VAR;
    }
    واپس variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_ریکارڈ_WITH_VAR : DROP_ریکارڈ_WITH_VAR;
  }

  سانچہ <قسم_نام context_t, قسم_نام ...Ts>
  قسم_نام context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    اگر (unlikely (!c->may_dispatch (یہ, &u.format.v))) واپس c->no_dispatch_return_قدر ();
    TRACE_DISPATCH (یہ, u.format.v);
    انتخاب (u.format.v) {
    حالت 1: hb_barrier (); return_trace (c->dispatch (u.format1, std::forward<Ts> (ds)...));
    حالت 2: hb_barrier (); return_trace (c->dispatch (u.format2, std::forward<Ts> (ds)...));
    حالت 3: hb_barrier (); return_trace (c->dispatch (u.format3, std::forward<Ts> (ds)...));
    حالت 4: hb_barrier (); return_trace (c->dispatch (u.format4, std::forward<Ts> (ds)...));
    حالت 5: hb_barrier (); return_trace (c->dispatch (u.format5, std::forward<Ts> (ds)...));
    طےشدہ:return_trace (c->default_return_قدر ());
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);

    اگر (unlikely (!c->check_آغاز_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->انتہا_recursion (یہ->dispatch (c)));
  }

  منطقی collect_var_indices (hb_سیٹ_t *var_indices,
			    غیرعلامتی گہرائی = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<غیرعلامتی, صحیح>> *condition_اقدار = خالی_اشاریہ) ثابت;

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = خالی_اشاریہ);

  نجی:
  lookup_condition_subset_نتیجہ_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      غیرعلامتی گہرائی) ثابت;

  محفوظ:
  اتحاد {
  ساخت { HBUINT16 v; }	format;		/* Format identifier */
  ConditionAxisRange	format1;
  Condition_قدر	format2;
  ConditionAnd		format3;
  ConditionOr		format4;
  ConditionNegate	format5;
  } u;
  عوامی:
  DEFINE_سائز_UNION (2, format.v);
};

سانچہ <قسم_نام Instancer>
منطقی
_hb_recurse_condition_evaluate (ثابت ساخت Condition &condition,
				ثابت صحیح *coords,
				غیرعلامتی صحیح coord_len,
				Instancer *instancer)
{
  واپس condition.evaluate (coords, coord_len, instancer);
}

ساخت ConditionList
{
  ثابت Condition& عامل[] (غیرعلامتی i) ثابت
  { واپس یہ+conditions[i]; }

  غیرعلامتی get_گنتی () ثابت { واپس conditions.len; }

  منطقی serialize (hb_serialize_context_t *c,
		  ثابت ConditionList *src,
		  ثابت hb_inc_bimap_t &condition_map,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (یہ);
    اگر (unlikely (!c->extend_min (یہ))) return_trace (جھوٹ);
    conditions.len = condition_map.get_population ();
    اگر (unlikely (!c->extend (conditions))) return_trace (جھوٹ);
    برائے (غیرعلامتی i = 0; i < conditions.len; i++)
    {
      غیرعلامتی old_اشاریہ = condition_map.backward (i);
      اگر (unlikely (old_اشاریہ >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_اشاریہ]), varidx_map, axis_map)))
	return_trace (جھوٹ);
    }
    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (conditions.sanitize (c, یہ));
  }

  محفوظ:
  Array32_کا_Offset32_مقصد<Condition> conditions;
  عوامی:
  DEFINE_سائز_ARRAY (4, conditions);
};

اندرونی منطقی
Condition::collect_var_indices (hb_سیٹ_t *var_indices, غیرعلامتی گہرائی,
                               hb_hashmap_t<uintptr_t, hb_pair_t<غیرعلامتی, صحیح>> *condition_اقدار) ثابت
{
  اگر (unlikely (!گہرائی)) واپس جھوٹ;
  انتخاب (u.format.v)
  {
    حالت 0:
    حالت 1:
      واپس سچ;
    حالت 2:
      اگر (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->جوڑو (u.format2.varIdx);
        اگر (condition_اقدار &&
            !condition_اقدار->سیٹ ((uintptr_t) &u.format2,
                                    hb_pair (غیرعلامتی (u.format2.varIdx), صحیح (u.format2.default_قدر_2))))
          واپس جھوٹ;
      }
      واپس !var_indices->اندر_error ();
    حالت 3:
      برائے (ثابت خودکار &انحراف : u.format3.conditions)
	اگر (unlikely (!(&u.format3+انحراف).collect_var_indices (var_indices,
							     گہرائی - 1, condition_اقدار)))
	  واپس جھوٹ;
      واپس سچ;
    حالت 4:
      برائے (ثابت خودکار &انحراف : u.format4.conditions)
	اگر (unlikely (!(&u.format4+انحراف).collect_var_indices (var_indices,
							     گہرائی - 1, condition_اقدار)))
	  واپس جھوٹ;
      واپس سچ;
    حالت 5:
      واپس (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  گہرائی - 1, condition_اقدار);
    طےشدہ:
      واپس جھوٹ;
  }
}

اندرونی منطقی
Condition::serialize (hb_serialize_context_t *c,
		      ثابت Condition *src,
		      ثابت hb_map_t &varidx_map,
		      ثابت hb_map_t *axis_map)
{
  TRACE_SERIALIZE (یہ);
  انتخاب (src->u.format.v)
  {
    حالت 0:
    {
      خودکار *برآمد = c->آغاز_embed<Condition_قدر> ();
      اگر (unlikely (!برآمد || !c->extend_min (برآمد))) return_trace (جھوٹ);
      برآمد->format = 2;
      برآمد->default_قدر_2 = 1;
      برآمد->varIdx = VarIdx::NO_VARIATION;
      return_trace (سچ);
    }
    حالت 1:
    {
      خودکار *برآمد = c->embed (&src->u.format1);
      اگر (unlikely (!برآمد)) return_trace (جھوٹ);
      اگر (axis_map)
      {
	اگر (unlikely (!axis_map->has (برآمد->axis_اشاریہ_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (جھوٹ);
	}
	اگر (unlikely (!c->check_تفویض (برآمد->axis_اشاریہ_2, axis_map->get (برآمد->axis_اشاریہ_2),
				      HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
	  return_trace (جھوٹ);
      }
      return_trace (سچ);
    }
    حالت 2:
    {
      خودکار *برآمد = c->embed (&src->u.format2);
      اگر (unlikely (!برآمد ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (جھوٹ);
      برآمد->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (سچ);
    }
    حالت 3:
    {
      خودکار *برآمد = c->آغاز_embed<ConditionAnd> ();
      اگر (unlikely (!c->extend_min (برآمد))) return_trace (جھوٹ);
      برآمد->format = 3;
      برآمد->conditions.len = src->u.format3.conditions.len;
      اگر (unlikely (!c->extend (برآمد->conditions))) return_trace (جھوٹ);
      برائے (غیرعلامتی i = 0; i < برآمد->conditions.len; i++)
	اگر (unlikely (!برآمد->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (جھوٹ);
      return_trace (سچ);
    }
    حالت 4:
    {
      خودکار *برآمد = c->آغاز_embed<ConditionOr> ();
      اگر (unlikely (!c->extend_min (برآمد))) return_trace (جھوٹ);
      برآمد->format = 4;
      برآمد->conditions.len = src->u.format4.conditions.len;
      اگر (unlikely (!c->extend (برآمد->conditions))) return_trace (جھوٹ);
      برائے (غیرعلامتی i = 0; i < برآمد->conditions.len; i++)
	اگر (unlikely (!برآمد->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (جھوٹ);
      return_trace (سچ);
    }
    حالت 5:
    {
      خودکار *برآمد = c->embed (&src->u.format5);
      اگر (unlikely (!برآمد ||
		    !برآمد->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (جھوٹ);
      return_trace (سچ);
    }
    طےشدہ:
      return_trace (جھوٹ);
  }
}

اندرونی lookup_condition_subset_نتیجہ_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    غیرعلامتی گہرائی,
				    منطقی materialize_true) ثابت
{
  خودکار snap = c->serializer->snapshot ();
  lookup_condition_subset_نتیجہ_t نتیجہ =
      subset_lookup_condition_impl (c, گہرائی);
  اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_KEEP)
    واپس نتیجہ;

  c->serializer->revert (snap);
  اگر (نتیجہ != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    واپس نتیجہ;

  واپس serialize_constant (c->serializer, سچ) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

اندرونی lookup_condition_subset_نتیجہ_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 غیرعلامتی گہرائی) ثابت
{
  اگر (unlikely (!گہرائی))
    واپس LOOKUP_CONDITION_SUBSET_ERROR;

  خودکار subset_انحراف = [c, گہرائی] (Array8_کا_Offset24_مقصد<Condition> &برآمد,
				   ثابت Condition &condition)
      -> lookup_condition_subset_نتیجہ_t
  {
    خودکار snap = c->serializer->snapshot ();
    خودکار *انحراف = برآمد.serialize_append (c->serializer);
    اگر (unlikely (!انحراف))
      واپس LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_نتیجہ_t نتیجہ =
        condition.subset_lookup_condition (c, گہرائی - 1);
    اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->جوڑو_link (*انحراف, c->serializer->نکالو_pack ());
    ورنہ
    {
      c->serializer->نکالو_discard ();
      برآمد.نکالو ();
      c->serializer->revert (snap);
    }
    واپس نتیجہ;
  };

  انتخاب (u.format.v)
  {
    حالت 0:
      واپس LOOKUP_CONDITION_SUBSET_TRUE;

    حالت 1:
    {
      ثابت ConditionAxisRange &src = u.format1;
      اگر (c->plan->user_axes_location.is_empty ())
        واپس c->serializer->embed (&src) ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               LOOKUP_CONDITION_SUBSET_ERROR;

      hb_codepoint_t *axis_لیبل_2;
      اگر (!c->plan->axes_old_اشاریہ_لیبل_map.has (src.axis_اشاریہ_2, &axis_لیبل_2))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_حد_بندی {-1.f, 0.f, 1.f};
      Triple *normalized_حد_بندی;
      اگر (c->plan->axes_location.has (*axis_لیبل_2, &normalized_حد_بندی))
        axis_حد_بندی = *normalized_حد_بندی;

      اعشاری filter_min = src.filter_Range_Min_قدر.مقصد_float ();
      اعشاری filter_max = src.filter_Range_Max_قدر.مقصد_float ();
      اگر (filter_min > filter_max)
        واپس LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (axis_حد_بندی.is_نقطہ ())
        واپس filter_min <= axis_حد_بندی.درمیان &&
	       axis_حد_بندی.درمیان <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (axis_حد_بندی.زیادہ_سے_زیادہ < filter_min ||
	  filter_max < axis_حد_بندی.minimum)
        واپس LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (filter_min <= axis_حد_بندی.minimum &&
	  axis_حد_بندی.زیادہ_سے_زیادہ <= filter_max)
        واپس LOOKUP_CONDITION_SUBSET_TRUE;
      اگر (!c->plan->axes_اشاریہ_map.has (src.axis_اشاریہ_2))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;

      خودکار *برآمد = c->serializer->embed (&src);
      اگر (unlikely (!برآمد))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      اگر (c->plan->axes_triple_distances.has (*axis_لیبل_2, &distances))
        axis_distances = *distances;

      برآمد->filter_Range_Min_قدر.سیٹ_float (
	  renormalize_قدر (filter_min, axis_حد_بندی, axis_distances, جھوٹ));
      برآمد->filter_Range_Max_قدر.سیٹ_float (
	  renormalize_قدر (filter_max, axis_حد_بندی, axis_distances, جھوٹ));
      اگر (unlikely (!c->serializer->check_تفویض (
		برآمد->axis_اشاریہ_2,
		c->plan->axes_اشاریہ_map.get (src.axis_اشاریہ_2),
		HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;
      واپس LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالت 2:
    {
      ثابت Condition_قدر &src = u.format2;
      خودکار *برآمد = c->serializer->embed (&src);
      اگر (unlikely (!برآمد))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;

      اگر (src.varIdx == VarIdx::NO_VARIATION)
        واپس c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحیح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<غیرعلامتی, صحیح> *condition_قدر;
      اگر (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) &src, &condition_قدر))
      {
        برآمد->default_قدر_2 = condition_قدر->second;
        برآمد->varIdx = condition_قدر->پہلا;
        اگر (برآمد->varIdx == VarIdx::NO_VARIATION)
          واپس برآمد->default_قدر_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        واپس LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<غیرعلامتی, صحیح> *new_varidx_delta;
      اگر (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        واپس c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحیح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      اگر (!c->plan->user_axes_location.is_empty () &&
          hb_پہلا (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        دوہرا value = (صحیح) src.default_قدر_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        واپس value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحیح default_قدر = (صحیح) src.default_قدر_2 +
			  hb_دوسرا (*new_varidx_delta);
      اگر (unlikely (!c->serializer->check_تفویض (
		برآمد->default_قدر_2,
		default_قدر,
		HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;
      برآمد->varIdx = hb_پہلا (*new_varidx_delta);

      واپس LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالت 3:
    {
      ثابت ConditionAnd &src = u.format3;
      خودکار *برآمد = c->serializer->آغاز_embed (&src);
      اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;
      برآمد->format = 3;

      برائے (ثابت خودکار &انحراف : src.conditions)
      {
        lookup_condition_subset_نتیجہ_t نتیجہ =
            subset_انحراف (برآمد->conditions, &src + انحراف);
        اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتیجہ == LOOKUP_CONDITION_SUBSET_FALSE)
          واپس نتیجہ;
      }
      واپس برآمد->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالت 4:
    {
      ثابت ConditionOr &src = u.format4;
      خودکار *برآمد = c->serializer->آغاز_embed (&src);
      اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;
      برآمد->format = 4;

      برائے (ثابت خودکار &انحراف : src.conditions)
      {
        lookup_condition_subset_نتیجہ_t نتیجہ =
            subset_انحراف (برآمد->conditions, &src + انحراف);
        اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتیجہ == LOOKUP_CONDITION_SUBSET_TRUE)
          واپس نتیجہ;
      }
      واپس برآمد->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالت 5:
    {
      ثابت ConditionNegate &src = u.format5;
      خودکار *برآمد = c->serializer->embed (&src);
      اگر (unlikely (!برآمد))
        واپس LOOKUP_CONDITION_SUBSET_ERROR;
      برآمد->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_نتیجہ_t نتیجہ =
          (&src + src.condition).subset_lookup_condition (c, گہرائی - 1);
      اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->جوڑو_link (برآمد->condition,
				 c->serializer->نکالو_pack ());
        واپس نتیجہ;
      }

      c->serializer->نکالو_discard ();
      اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_TRUE)
        واپس LOOKUP_CONDITION_SUBSET_FALSE;
      اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_FALSE)
        واپس LOOKUP_CONDITION_SUBSET_TRUE;
      واپس نتیجہ;
    }

    طےشدہ:
      واپس LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

ساخت Condition_سیٹ
{
  منطقی evaluate (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		 شے_Var_Store_Instancer *instancer) ثابت
  {
    غیرعلامتی صحیح گنتی = conditions.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
      اگر (!(یہ+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	واپس جھوٹ;
    واپس سچ;
  }

  خالی keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    اگر (unlikely (!condition_map)) واپس;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_سیٹ_t *cond_سیٹ = hb_سیٹ_create ();
    اگر (unlikely (!cond_سیٹ)) واپس;
    hb::shared_ptr<hb_سیٹ_t> s {cond_سیٹ};

    c->apply = سچ;
    منطقی should_keep = جھوٹ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقی can_deduplicate = سچ;
    غیرعلامتی عدد_kept_cond = 0, cond_idx = 0;
    برائے (ثابت خودکار& انحراف : conditions)
    {
      ثابت Condition &condition = یہ+انحراف;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      اگر (ret == DROP_ریکارڈ_WITH_VAR)
      {
        c->apply = جھوٹ;
        واپس;
      }

      اگر (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = سچ;
        cond_سیٹ->جوڑو (cond_idx);
        عدد_kept_cond++;
      }

      اگر (ret == KEEP_ریکارڈ_WITH_VAR)
        should_keep = سچ;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = عدد_kept_cond == 0;
    اگر (!should_keep && c->all_axes_pinned) واپس;

    //check if condition_set is unique with variations
    اگر (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      واپس;

    اگر (can_deduplicate)
      c->conditionset_map->سیٹ (p, 1);
    c->ریکارڈ_cond_idx_map->سیٹ (c->cur_ریکارڈ_idx, s);
  }

  منطقی subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقی داخل_کرو_catch_all) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    اگر (داخل_کرو_catch_all) return_trace (سچ);

    hb_سیٹ_t *retained_cond_سیٹ = خالی_اشاریہ;
    اگر (l->feature_ریکارڈ_cond_idx_map != خالی_اشاریہ)
      retained_cond_سیٹ = l->feature_ریکارڈ_cond_idx_map->get (l->cur_feature_var_ریکارڈ_idx);

    غیرعلامتی صحیح گنتی = conditions.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (retained_cond_سیٹ != خالی_اشاریہ && !retained_cond_سیٹ->has (i))
        جاری;
      ثابت Condition &condition = یہ+conditions[i];
      اگر (condition.u.format.v == 1)
      {
        subset_انحراف_array (c, برآمد->conditions, یہ) (conditions[i]);
        جاری;
      }

      خودکار *انحراف = برآمد->conditions.serialize_append (c->serializer);
      اگر (unlikely (!انحراف)) return_trace (جھوٹ);
      c->serializer->push ();
      خودکار نتیجہ = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, سچ);
      اگر (نتیجہ == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        نتیجہ = Condition::serialize_constant (c->serializer, جھوٹ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      اگر (unlikely (نتیجہ == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->نکالو_discard ();
        c->serializer->check_success (جھوٹ);
        return_trace (جھوٹ);
      }
      c->serializer->جوڑو_link (*انحراف, c->serializer->نکالو_pack ());
    }

    return_trace (منطقی (برآمد->conditions));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (conditions.sanitize (c, یہ));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت خودکار &انحراف : conditions)
      اگر (unlikely (!(یہ+انحراف).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_اقدار)))
	c->layout_variation_indices->err ();
  }

  محفوظ:
  Array16_کا_Offset32_مقصد<Condition>	conditions;
  عوامی:
  DEFINE_سائز_ARRAY (2, conditions);
};

ساخت Feature_جدول_Substitution_ریکارڈ
{
  دوست ساخت Feature_جدول_Substitution;

  خالی collect_lookups (ثابت خالی *بنیاد, hb_سیٹ_t *lookup_indexes /*برآمد*/) ثابت
  {
    واپس (بنیاد+feature).جوڑو_lookup_indexes_مقصد (lookup_indexes);
  }

  خالی احاطہ_features (ثابت خالی *بنیاد,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t       *feature_indexes /*برآمد*/) ثابت
  {
    اگر ((بنیاد+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جوڑو (feature_اشاریہ_2);
  }

  خالی collect_feature_substitutes_with_variations (hb_hashmap_t<غیرعلامتی, ثابت Feature*> *feature_substitutes_map,
                                                    hb_سیٹ_t& catch_all_ریکارڈ_feature_idxes,
                                                    ثابت hb_سیٹ_t *feature_indices,
                                                    ثابت خالی *بنیاد) ثابت
  {
    اگر (feature_indices->has (feature_اشاریہ_2))
    {
      feature_substitutes_map->سیٹ (feature_اشاریہ_2, &(بنیاد+feature));
      catch_all_ریکارڈ_feature_idxes.جوڑو (feature_اشاریہ_2);
    }
  }

  منطقی serialize (hb_subset_layout_context_t *c,
                  غیرعلامتی feature_اشاریہ,
                  ثابت Feature *f, ثابت لیبل *لیبل_2)
  {
    TRACE_SERIALIZE (یہ);
    hb_serialize_context_t *s = c->subset_context->serializer;
    اگر (unlikely (!s->extend_min (یہ))) return_trace (جھوٹ);

    uint32_t *new_feature_idx;
    اگر (!c->feature_map_w_duplicates->has (feature_اشاریہ, &new_feature_idx))
      return_trace (جھوٹ);

    اگر (!s->check_تفویض (feature_اشاریہ_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (جھوٹ);

    s->push ();
    منطقی ret = f->subset (c->subset_context, c, لیبل_2);
    اگر (ret) s->جوڑو_link (feature, s->نکالو_pack ());
    ورنہ s->نکالو_discard ();

    return_trace (ret);
  }

  منطقی subset (hb_subset_layout_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    uint32_t *new_feature_اشاریہ;
    اگر (!c->feature_map_w_duplicates->has (feature_اشاریہ_2, &new_feature_اشاریہ))
      return_trace (جھوٹ);

    خودکار *برآمد = c->subset_context->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->feature_اشاریہ_2 = *new_feature_اشاریہ;
    hb_pair_t<ثابت خالی*, ثابت خالی*> *original;
    اگر (!c->feature_idx_لیبل_map->has (feature_اشاریہ_2, &original))
      return_trace (جھوٹ);
    ثابت لیبل *لیبل_2 = reinterpret_cast<ثابت لیبل*> (original->second);
    return_trace (برآمد->feature.serialize_subset (c->subset_context, feature, بنیاد, c, لیبل_2));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!c->check_struct (یہ))) return_trace (جھوٹ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت ریکارڈ_sanitize_احاطہ_t احاطہ = {feature_list->get_لیبل (feature_اشاریہ_2), بنیاد};
    return_trace (feature.sanitize (c, بنیاد, &احاطہ));
  }

  محفوظ:
  HBUINT16		feature_اشاریہ_2;
  Offset32_مقصد<Feature>	feature;
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت Feature_جدول_Substitution
{
  خالی collect_feature_indices (hb_سیٹ_t *feature_indices) ثابت
  {
    برائے (ثابت Feature_جدول_Substitution_ریکارڈ &ریکارڈ : substitutions)
      feature_indices->جوڑو (ریکارڈ.feature_اشاریہ_2);
  }

  ثابت Feature *ڈھونڈو_substitute (غیرعلامتی صحیح feature_اشاریہ) ثابت
  {
    غیرعلامتی صحیح گنتی = substitutions.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      ثابت Feature_جدول_Substitution_ریکارڈ &ریکارڈ = substitutions.arrayZ[i];
      اگر (ریکارڈ.feature_اشاریہ_2 == feature_اشاریہ)
	واپس &(یہ+ریکارڈ.feature);
    }
    واپس خالی_اشاریہ;
  }

  خالی collect_lookups (ثابت hb_سیٹ_t *feature_indexes,
			hb_سیٹ_t       *lookup_indexes /*برآمد*/) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_جدول_Substitution_ریکارڈ::feature_اشاریہ_2)
    | hb_apply ([یہ, lookup_indexes] (ثابت Feature_جدول_Substitution_ریکارڈ& r)
		{ r.collect_lookups (یہ, lookup_indexes); })
    ;
  }

  سانچہ <قسم_نام لیبل_Getter>
  خالی collect_name_ids (ثابت hb_map_t *feature_اشاریہ_map,
                         لیبل_Getter get_لیبل,
                         hb_سیٹ_t *nameids_مقصد_retain) ثابت
  {
    برائے (ثابت Feature_جدول_Substitution_ریکارڈ &ریکارڈ : substitutions)
      اگر (feature_اشاریہ_map->has (ریکارڈ.feature_اشاریہ_2))
        (یہ+ریکارڈ.feature).collect_name_ids (get_لیبل (ریکارڈ.feature_اشاریہ_2), nameids_مقصد_retain);
  }

  خالی احاطہ_features (ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t       *feature_indexes /*برآمد*/) ثابت
  {
    برائے (ثابت Feature_جدول_Substitution_ریکارڈ& ریکارڈ : substitutions)
      ریکارڈ.احاطہ_features (یہ, lookup_indexes, feature_indexes);
  }

  منطقی intersects_features (ثابت hb_map_t *feature_اشاریہ_map) ثابت
  {
    برائے (ثابت Feature_جدول_Substitution_ریکارڈ& ریکارڈ : substitutions)
    {
      اگر (feature_اشاریہ_map->has (ریکارڈ.feature_اشاریہ_2)) واپس سچ;
    }
    واپس جھوٹ;
  }

  خالی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    برائے (ثابت Feature_جدول_Substitution_ریکارڈ& ریکارڈ : substitutions)
      ریکارڈ.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_ریکارڈ_feature_idxes,
                                                          c->feature_indices, یہ);
  }

  منطقی subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقی داخل_کرو_catch_all) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->نسخہ.major = نسخہ.major;
    برآمد->نسخہ.minor = نسخہ.minor;

    اگر (داخل_کرو_catch_all)
    {
      برائے (غیرعلامتی feature_اشاریہ : *(l->catch_all_ریکارڈ_feature_idxes))
      {
        hb_pair_t<ثابت خالی*, ثابت خالی*> *p;
        اگر (!l->feature_idx_لیبل_map->has (feature_اشاریہ, &p))
          return_trace (جھوٹ);
        خودکار *o = برآمد->substitutions.serialize_append (c->serializer);
        اگر (!o->serialize (l, feature_اشاریہ,
                           reinterpret_cast<ثابت Feature*> (p->پہلا),
                           reinterpret_cast<ثابت لیبل*> (p->second)))
          return_trace (جھوٹ);
      }
      return_trace (سچ);
    }

    + substitutions.iter ()
    | hb_apply (subset_ریکارڈ_array (l, &(برآمد->substitutions), یہ))
    ;

    return_trace (منطقی (برآمد->substitutions));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  substitutions.sanitize (c, یہ, feature_list));
  }

  محفوظ:
  Fixed_نسخہ<>	نسخہ;	/* Version--0x00010000u */
  Array16_کا<Feature_جدول_Substitution_ریکارڈ>
			substitutions;
  عوامی:
  DEFINE_سائز_ARRAY (6, substitutions);
};

ساخت Feature_Variation_ریکارڈ
{
  دوست ساخت FeatureVariations;

  خالی collect_lookups (ثابت خالی     *بنیاد,
			ثابت hb_سیٹ_t *feature_indexes,
			hb_سیٹ_t       *lookup_indexes /*برآمد*/) ثابت
  {
    واپس (بنیاد+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  سانچہ <قسم_نام لیبل_Getter>
  خالی collect_name_ids (ثابت خالی *بنیاد,
                         ثابت hb_map_t *feature_اشاریہ_map,
                         لیبل_Getter get_لیبل,
                         hb_سیٹ_t *nameids_مقصد_retain) ثابت
  {
    (بنیاد+substitutions).collect_name_ids (feature_اشاریہ_map, get_لیبل, nameids_مقصد_retain);
  }

  خالی احاطہ_features (ثابت خالی     *بنیاد,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t       *feature_indexes /*برآمد*/) ثابت
  {
    (بنیاد+substitutions).احاطہ_features (lookup_indexes, feature_indexes);
  }

  منطقی intersects_features (ثابت خالی *بنیاد, ثابت hb_map_t *feature_اشاریہ_map) ثابت
  {
    واپس (بنیاد+substitutions).intersects_features (feature_اشاریہ_map);
  }

  خالی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت خالی *بنیاد) ثابت
  {
    (بنیاد+conditions).keep_with_variations (c);
    اگر (c->apply && !c->variation_applied)
    {
      (بنیاد+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = سچ; // set variations only once
    }
  }

  منطقی subset (hb_subset_layout_context_t *c, ثابت خالی *بنیاد,
               منطقی داخل_کرو_catch_all = جھوٹ) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->subset_context->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    hb_سیٹ_t *retained_conditions = c->feature_ریکارڈ_cond_idx_map ?
        c->feature_ریکارڈ_cond_idx_map->get (c->cur_feature_var_ریکارڈ_idx).get () : خالی_اشاریہ;
    اگر (!داخل_کرو_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      برآمد->conditions = 0;
    ورنہ
      برآمد->conditions.serialize_subset (c->subset_context, conditions, بنیاد, c, داخل_کرو_catch_all);
    برآمد->substitutions.serialize_subset (c->subset_context, substitutions, بنیاد, c, داخل_کرو_catch_all);

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (conditions.sanitize (c, بنیاد) &&
		  substitutions.sanitize (c, بنیاد, feature_list));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت خالی *بنیاد) ثابت
  { (بنیاد+conditions).collect_variation_indices (c); }

  محفوظ:
  Offset32_مقصد<Condition_سیٹ>
			conditions;
  Offset32_مقصد<Feature_جدول_Substitution>
			substitutions;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Lookup_اشاریہ_List
{
  خالی جوڑو_lookup_indexes_مقصد (hb_سیٹ_t *lookup_indexes) ثابت
  { lookupIndices.جوڑو_indexes_مقصد (lookup_indexes); }

  منطقی intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { واپس lookupIndices.intersects (lookup_indexes); }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
      return_trace (جھوٹ);

    خودکار it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_اشاریہ_map)
    | hb_map (l->lookup_اشاریہ_map)
    ;
    برآمد->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقی (برآمد->lookupIndices));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (lookupIndices.sanitize (c));
  }

  محفوظ:
  اشاریہ_Array	lookupIndices;
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (2, lookupIndices);
};

ساخت Lookup_Condition_ریکارڈ
{
  غیرعلامتی get_condition_انحراف (ثابت خالی *بنیاد, ثابت خالی *variations) ثابت
  { واپس (ثابت حرف *) &(بنیاد+condition) - (ثابت حرف *) variations; }

  سانچہ <قسم_نام Instancer>
  منطقی evaluate (ثابت خالی *بنیاد,
		 ثابت صحیح *coords,
		 غیرعلامتی صحیح coord_len,
		 Instancer *instancer) ثابت
  { واپس (بنیاد+condition).evaluate (coords, coord_len, instancer); }

  منطقی evaluate (ثابت خالی *بنیاد,
                 ثابت خالی *variations,
                 hb_sorted_array_t<ثابت غیرعلامتی> conditions) ثابت
  {
    واپس !condition || hb_bsearch (get_condition_انحراف (بنیاد, variations),
                                     conditions.arrayZ, conditions.لمبائی,
                                     حجم (غیرعلامتی),
                                     _hb_cmp_operator<غیرعلامتی, غیرعلامتی>);
  }

  منطقی collect_condition (ثابت خالی *بنیاد,
                          ثابت خالی *variations,
                          ثابت صحیح *coords,
                          غیرعلامتی صحیح coord_len,
                          شے_Var_Store_Instancer *instancer,
                          hb_سیٹ_t *دیکھا,
                          hb_سمتیہ_t<غیرعلامتی> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    اگر (!condition) واپس سچ;

    غیرعلامتی انحراف = get_condition_انحراف (بنیاد, variations);
    اگر (دیکھا->has (انحراف)) واپس سچ;
    دیکھا->جوڑو (انحراف);
    اگر (unlikely (دیکھا->اندر_error ())) واپس جھوٹ;

    اگر (evaluate (بنیاد, coords, coord_len, instancer))
      conditions->push (انحراف);
    واپس !conditions->اندر_error ();
  }

  خالی جوڑو_lookup_indexes_مقصد (ثابت خالی *بنیاد,
			      hb_سیٹ_t *lookup_indexes) ثابت
  { (بنیاد+lookupIndices).جوڑو_lookup_indexes_مقصد (lookup_indexes); }

  منطقی intersects_lookup_indexes (ثابت خالی *بنیاد,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { واپس (بنیاد+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت خالی *بنیاد) ثابت
  {
    اگر (unlikely (!(بنیاد+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_اقدار)))
      c->layout_variation_indices->err ();
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت خالی *بنیاد) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    برآمد->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_نتیجہ_t نتیجہ =
        (بنیاد + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    سچ);
    اگر (نتیجہ != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->نکالو_discard ();
      return_trace (جھوٹ);
    }
    c->serializer->جوڑو_link (برآمد->condition, c->serializer->نکالو_pack ());

    return_trace (برآمد->lookupIndices.serialize_subset (c, lookupIndices, بنیاد, l));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (condition.sanitize (c, بنیاد) &&
		  lookupIndices.sanitize (c, بنیاد));
  }

  محفوظ:
  Offset32_مقصد<Condition>		condition;
  Offset32_مقصد<Lookup_اشاریہ_List>	lookupIndices;
  عوامی:
  DEFINE_سائز_STATIC (8);
};

ساخت Feature_Lookups_جدول
{
  منطقی adds_default_lookups () ثابت { واپس flags & 0x0001u; }

  منطقی collect_conditions (ثابت خالی *variations,
                           ثابت صحیح *coords,
                           غیرعلامتی صحیح coord_len,
                           شے_Var_Store_Instancer *instancer,
                           hb_سیٹ_t *دیکھا,
                           hb_سمتیہ_t<غیرعلامتی> *conditions) ثابت
  {
    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
      اگر (unlikely (!ریکارڈ.collect_condition (یہ, variations, coords, coord_len,
                                               instancer, دیکھا, conditions)))
	واپس جھوٹ;
    واپس سچ;
  }

  خالی collect_lookups (ثابت خالی *variations,
                       hb_sorted_array_t<ثابت غیرعلامتی> conditions,
                       hb_سیٹ_t *lookup_indexes) ثابت
  {
    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
      اگر (ریکارڈ.evaluate (یہ, variations, conditions))
	ریکارڈ.جوڑو_lookup_indexes_مقصد (یہ, lookup_indexes);
  }

  سانچہ <قسم_نام Instancer>
  خالی collect_lookups (ثابت صحیح *coords,
			غیرعلامتی صحیح coord_len,
			Instancer *instancer,
			hb_سیٹ_t *lookup_indexes) ثابت
  {
    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
      اگر (ریکارڈ.evaluate (یہ, coords, coord_len, instancer))
	ریکارڈ.جوڑو_lookup_indexes_مقصد (یہ, lookup_indexes);
  }

  خالی collect_lookups (hb_سیٹ_t *lookup_indexes) ثابت
  {
    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
      ریکارڈ.جوڑو_lookup_indexes_مقصد (یہ, lookup_indexes);
  }

  منطقی intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
      اگر (ریکارڈ.intersects_lookup_indexes (یہ, lookup_indexes))
	واپس سچ;
    واپس جھوٹ;
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
      ریکارڈ.collect_variation_indices (c, یہ);
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد)))
      return_trace (جھوٹ);

    برآمد->نسخہ.major = نسخہ.major;
    برآمد->نسخہ.minor = نسخہ.minor;
    برآمد->flags = flags;

    برائے (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : اندراجات)
    {
      خودکار snap = c->serializer->snapshot ();
      اگر (ریکارڈ.subset (c, l, یہ))
	برآمد->اندراجات.len++;
      ورنہ
	c->serializer->revert (snap);
    }

    return_trace (سچ);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  اندراجات.sanitize (c, یہ));
  }

  محفوظ:
  Fixed_نسخہ<>	نسخہ;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_کا<Lookup_Condition_ریکارڈ>
		اندراجات;
  عوامی:
  DEFINE_سائز_ARRAY (10, اندراجات);
};

ساخت Lookup_Variation_ریکارڈ
{
  دوست ساخت FeatureVariations;

  غیرعلامتی get_feature_اشاریہ () ثابت { واپس feature_اشاریہ_2; }

  خالی collect_lookups (ثابت خالی *بنیاد,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت غیرعلامتی> conditions,
                       hb_سیٹ_t *lookup_indexes) ثابت
  {
    ثابت Feature_Lookups_جدول &feature_lookups = بنیاد+featureLookups;
    اگر (feature_lookups.adds_default_lookups ())
      feature.جوڑو_lookup_indexes_مقصد (lookup_indexes);
    feature_lookups.collect_lookups (بنیاد, conditions, lookup_indexes);
  }

  خالی collect_lookups (ثابت خالی *بنیاد,
			ثابت hb_سیٹ_t *feature_indexes,
			hb_سیٹ_t *lookup_indexes) ثابت
  {
    اگر (feature_indexes->has (feature_اشاریہ_2))
      (بنیاد+featureLookups).collect_lookups (lookup_indexes);
  }

  خالی احاطہ_features (ثابت خالی *بنیاد,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t *feature_indexes) ثابت
  {
    اگر ((بنیاد+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جوڑو (feature_اشاریہ_2);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت خالی *بنیاد) ثابت
  { (بنیاد+featureLookups).collect_variation_indices (c); }

  منطقی subset (hb_subset_layout_context_t *l, ثابت خالی *بنیاد) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (یہ);
    uint32_t *new_feature_اشاریہ;
    اگر (!l->feature_map_w_duplicates->has (feature_اشاریہ_2, &new_feature_اشاریہ))
      return_trace (جھوٹ);

    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد ||
		  !c->serializer->check_تفویض (برآمد->feature_اشاریہ_2,
						*new_feature_اشاریہ,
						HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز)))
      return_trace (جھوٹ);

    return_trace (برآمد->featureLookups.serialize_subset (c, featureLookups, بنیاد, l));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت خالی *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  featureLookups.sanitize (c, بنیاد));
  }

  محفوظ:
  HBUINT16			feature_اشاریہ_2;
  Offset32_مقصد<Feature_Lookups_جدول>	featureLookups;
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت FeatureVariations
{
  ساکن constexpr غیرعلامتی NOT_ملا_اشاریہ = 0xFFFFFFFFu;

  ثابت Array32_کا<Lookup_Variation_ریکارڈ> &get_lookup_variation_اندراجات () ثابت
  {
    اگر (نسخہ.مقصد_int () < 0x00010001u)
      واپس Null (Array32_کا<Lookup_Variation_ریکارڈ>);
    واپس StructAfter<Array32_کا<Lookup_Variation_ریکارڈ>> (var_اندراجات);
  }

  غیرعلامتی ریکارڈ_گنتی () ثابت
  {
    واپس var_اندراجات.len;
  }

  منطقی ڈھونڈو_اشاریہ (ثابت صحیح *coords, غیرعلامتی صحیح coord_len,
		   غیرعلامتی صحیح *اشاریہ_2,
		   شے_Var_Store_Instancer *instancer) ثابت
  {
    غیرعلامتی صحیح گنتی = var_اندراجات.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      ثابت Feature_Variation_ریکارڈ &ریکارڈ = var_اندراجات.arrayZ[i];
      اگر ((یہ+ریکارڈ.conditions).evaluate (coords, coord_len, instancer))
      {
	*اشاریہ_2 = i;
	واپس سچ;
      }
    }
    *اشاریہ_2 = NOT_ملا_اشاریہ;
    واپس جھوٹ;
  }

  ثابت Feature *ڈھونڈو_substitute (غیرعلامتی صحیح variations_اشاریہ,
				  غیرعلامتی صحیح feature_اشاریہ) ثابت
  {
    ثابت Feature_Variation_ریکارڈ &ریکارڈ = var_اندراجات[variations_اشاریہ];
    واپس (یہ+ریکارڈ.substitutions).ڈھونڈو_substitute (feature_اشاریہ);
  }

  منطقی collect_lookup_conditions (ثابت صحیح *coords,
                                  غیرعلامتی صحیح coord_len,
                                  شے_Var_Store_Instancer *instancer,
                                  hb_سمتیہ_t<غیرعلامتی> *state) ثابت
  {
    hb_سیٹ_t دیکھا_tables, دیکھا_conditions;
    برائے (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_اندراجات ())
    {
      غیرعلامتی انحراف = ریکارڈ.featureLookups;
      اگر (دیکھا_tables.has (انحراف)) جاری;
      دیکھا_tables.جوڑو (انحراف);
      اگر (unlikely (دیکھا_tables.اندر_error ())) واپس جھوٹ;

      ثابت Feature_Lookups_جدول &feature_lookups = یہ+ریکارڈ.featureLookups;
      اگر (unlikely (!feature_lookups.collect_conditions (یہ, coords, coord_len,
                                                         instancer, &دیکھا_conditions,
                                                         state)))
	واپس جھوٹ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<غیرعلامتی, غیرعلامتی>);
    واپس سچ;
  }

  منطقی get_lookup_variations (غیرعلامتی feature_اشاریہ,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت غیرعلامتی> conditions,
                              hb_سیٹ_t *lookup_indexes) ثابت
  {
    برائے (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_اندراجات ())
    {
      اگر (ریکارڈ.feature_اشاریہ_2 != feature_اشاریہ) جاری;

      ریکارڈ.collect_lookups (یہ, feature, conditions, lookup_indexes);
      واپس سچ;
    }
    واپس جھوٹ;
  }

  خالی collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    غیرعلامتی صحیح گنتی = var_اندراجات.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      c->cur_ریکارڈ_idx = i;
      var_اندراجات[i].collect_feature_substitutes_with_variations (c, یہ);
      اگر (c->universal)
        توڑو;
    }
    اگر (c->universal || c->ریکارڈ_cond_idx_map->is_empty ())
      c->catch_all_ریکارڈ_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed (*یہ));
  }

  خالی collect_lookups (ثابت hb_سیٹ_t *feature_indexes,
			ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
			hb_سیٹ_t       *lookup_indexes /*برآمد*/) ثابت
  {
    غیرعلامتی گنتی = var_اندراجات.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (feature_ریکارڈ_cond_idx_map &&
          !feature_ریکارڈ_cond_idx_map->has (i))
        جاری;
      var_اندراجات[i].collect_lookups (یہ, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  سانچہ <قسم_نام لیبل_Getter>
  خالی collect_name_ids (ثابت hb_map_t *feature_اشاریہ_map,
                         ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
                         لیبل_Getter get_لیبل,
                         hb_سیٹ_t *nameids_مقصد_retain) ثابت
  {
    برائے (غیرعلامتی i = 0; i < var_اندراجات.len; i++)
    {
      اگر (feature_ریکارڈ_cond_idx_map && !feature_ریکارڈ_cond_idx_map->has (i))
        جاری;
      var_اندراجات[i].collect_name_ids (یہ, feature_اشاریہ_map, get_لیبل, nameids_مقصد_retain);
    }
  }

  خالی collect_lookup_variation_lookups (ثابت hb_سیٹ_t *feature_indexes,
					 hb_سیٹ_t *lookup_indexes /*برآمد*/) ثابت
  {
    برائے (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_اندراجات ())
      ریکارڈ.collect_lookups (یہ, feature_indexes, lookup_indexes);
  }

  خالی احاطہ_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<غیرعلامتی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
			 hb_سیٹ_t       *feature_indexes /*برآمد*/) ثابت
  {
    غیرعلامتی صحیح گنتی = var_اندراجات.len;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    {
      اگر (feature_ریکارڈ_cond_idx_map != خالی_اشاریہ &&
          !feature_ریکارڈ_cond_idx_map->has (i))
        جاری;
      var_اندراجات[i].احاطہ_features (یہ, lookup_indexes, feature_indexes);
    }

    برائے (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_اندراجات ())
      ریکارڈ.احاطہ_features (یہ, lookup_indexes, feature_indexes);
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (ثابت Feature_Variation_ریکارڈ &ریکارڈ : var_اندراجات)
      ریکارڈ.collect_variation_indices (c, یہ);
    برائے (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_اندراجات ())
      ریکارڈ.collect_variation_indices (c, یہ);
  }

  خالی collect_feature_indices (hb_سیٹ_t *feature_indices) ثابت
  {
    برائے (ثابت Feature_Variation_ریکارڈ &ریکارڈ : var_اندراجات)
      (یہ+ریکارڈ.substitutions).collect_feature_indices (feature_indices);
  }

  منطقی has_lookup_variations (غیرعلامتی feature_اشاریہ) ثابت
  {
    برائے (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_اندراجات ())
    {
      اگر (ریکارڈ.feature_اشاریہ_2 == feature_اشاریہ)
	واپس سچ;
      اگر (ریکارڈ.feature_اشاریہ_2 > feature_اشاریہ)
	توڑو;
    }
    واپس جھوٹ;
  }

  منطقی subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (یہ);
    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!برآمد || !c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    برآمد->نسخہ.major = نسخہ.major;
    برآمد->نسخہ.minor = نسخہ.minor;

    صحیح keep_اوپر_مقصد = -1;
    برائے (صحیح i = var_اندراجات.len - 1; i >= 0; i--) {
      اگر (var_اندراجات[i].intersects_features (یہ, l->feature_map_w_duplicates)) {
        keep_اوپر_مقصد = i;
        توڑو;
      }
    }

    غیرعلامتی گنتی = (غیرعلامتی) (keep_اوپر_مقصد + 1);
    برائے (غیرعلامتی i = 0; i < گنتی; i++)
    {
      اگر (l->feature_ریکارڈ_cond_idx_map != خالی_اشاریہ &&
          !l->feature_ریکارڈ_cond_idx_map->has (i))
        جاری;

      l->cur_feature_var_ریکارڈ_idx = i;
      subset_ریکارڈ_array (l, &(برآمد->var_اندراجات), یہ) (var_اندراجات[i]);
    }

    اگر (برآمد->var_اندراجات.len && !l->catch_all_ریکارڈ_feature_idxes->is_empty ())
    {
      منطقی داخل_کرو_catch_all_ریکارڈ = سچ;
      subset_ریکارڈ_array (l, &(برآمد->var_اندراجات), یہ, داخل_کرو_catch_all_ریکارڈ) (var_اندراجات[0]);
    }

    Array32_کا<Lookup_Variation_ریکارڈ> *برآمد_lookup_اندراجات = خالی_اشاریہ;
    اگر (نسخہ.minor >= 1)
    {
      برآمد_lookup_اندراجات = &StructAfter<Array32_کا<Lookup_Variation_ریکارڈ>> (برآمد->var_اندراجات);
      اگر (unlikely (!c->serializer->extend_min (برآمد_lookup_اندراجات)))
	return_trace (جھوٹ);

      + get_lookup_variation_اندراجات ().iter ()
      | hb_apply (subset_ریکارڈ_array (l, برآمد_lookup_اندراجات, یہ))
      ;
    }

    return_trace (منطقی (برآمد->var_اندراجات) ||
		  (برآمد_lookup_اندراجات && منطقی (*برآمد_lookup_اندراجات)));
  }

  منطقی sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  var_اندراجات.sanitize (c, یہ, feature_list) &&
		  (نسخہ.minor < 1 ||
		   get_lookup_variation_اندراجات ().sanitize (c, یہ)));
  }

  محفوظ:
  Fixed_نسخہ<>	نسخہ;	/* Version--0x00010000u */
  Array32_کا<Feature_Variation_ریکارڈ>
			var_اندراجات;
  عوامی:
  DEFINE_سائز_ARRAY_SIZED (8, var_اندراجات);
};


/*
 * Device Tables
 */

ساخت HintingDevice
{
  دوست ساخت Device;

  نجی:

  hb_position_t get_x_delta (hb_font_t *font) ثابت
  { واپس get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ثابت
  { واپس get_delta (font->y_ppem, font->y_scale); }

  عوامی:

  size_t get_سائز () ثابت
  {
    غیرعلامتی صحیح f = deltaFormat;
    اگر (unlikely (f < 1 || f > 3 || آغاز_سائز > انتہا_سائز)) واپس 3 * HBUINT16::static_سائز;
    واپس HBUINT16::static_سائز * (4 + ((انتہا_سائز - آغاز_سائز) >> (4 - f)));
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) && c->check_range (یہ, یہ->get_سائز ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);
    return_trace (c->embed<HintingDevice> (یہ));
  }

  نجی:

  صحیح get_delta (غیرعلامتی صحیح ppem, صحیح scale) ثابت
  {
    اگر (!ppem) واپس 0;

    صحیح pixels = get_delta_pixels (ppem);

    اگر (!pixels) واپس 0;

    واپس (صحیح) (pixels * (int64_t) scale / ppem);
  }
  صحیح get_delta_pixels (غیرعلامتی صحیح ppem_سائز) ثابت
  {
    غیرعلامتی صحیح f = deltaFormat;
    اگر (unlikely (f < 1 || f > 3))
      واپس 0;

    اگر (ppem_سائز < آغاز_سائز || ppem_سائز > انتہا_سائز)
      واپس 0;

    غیرعلامتی صحیح s = ppem_سائز - آغاز_سائز;

    غیرعلامتی صحیح byte = delta_قدر_Z[s >> (4 - f)];
    غیرعلامتی صحیح بٹ = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    غیرعلامتی صحیح نقاب = (0xFFFFu >> (16 - (1 << f)));

    صحیح delta = بٹ & نقاب;

    اگر ((غیرعلامتی صحیح) delta >= ((نقاب + 1) >> 1))
      delta -= نقاب + 1;

    واپس delta;
  }

  محفوظ:
  HBUINT16	آغاز_سائز;		/* Smallest size to correct--in ppem */
  HBUINT16	انتہا_سائز;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_کا<HBUINT16>
		delta_قدر_Z;		/* Array of compressed data */
  عوامی:
  DEFINE_سائز_ARRAY (6, delta_قدر_Z);
};

ساخت VariationDevice
{
  دوست ساخت Device;

  نجی:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  { واپس !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  { واپس !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  اعشاری get_x_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { واپس !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  اعشاری get_y_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { واپس !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (یہ);
    اگر (!layout_variation_idx_delta_map) return_trace (خالی_اشاریہ);

    hb_pair_t<غیرعلامتی, صحیح> *v;
    اگر (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (خالی_اشاریہ);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    اگر (hb_پہلا (*v) == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
      return_trace (خالی_اشاریہ);

    c->آغاز_zerocopy (یہ->static_سائز);
    خودکار *برآمد = c->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (خالی_اشاریہ);

    اگر (!c->check_تفویض (برآمد->varIdx, hb_پہلا (*v), HB_SERIALIZE_ERROR_INT_حد_سے_تجاوز))
      return_trace (خالی_اشاریہ);
    return_trace (برآمد);
  }

  خالی collect_variation_اشاریہ (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->جوڑو (varIdx); }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ));
  }

  نجی:

  اعشاری get_delta (hb_font_t *font,
		   ثابت شے_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    واپس store.get_delta (varIdx, font->coords, font->عدد_coords, store_cache);
  }

  محفوظ:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت Device_سرخی
{
  محفوظ:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  عوامی:
  HBUINT16		format;		/* Format identifier */
  عوامی:
  DEFINE_سائز_STATIC (6);
};

ساخت Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  اعشاری get_x_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (u.b.format == 0x8000)
    {
      hb_barrier ();
      واپس u.variation.get_x_delta_f (font, store, store_cache);
    }
#اگر_ختم
    واپس get_x_delta (font, store, store_cache);
  }

  اعشاری get_y_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    اگر (u.b.format == 0x8000)
    {
      hb_barrier ();
      واپس u.variation.get_y_delta_f (font, store, store_cache);
    }
#اگر_ختم
    واپس get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store=Null (شے_Variation_Store),
			     hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    انتخاب (u.b.format)
    {
#اگر_غیر_تعریف_شدہ HB_NO_HINTING
    حالت 1: حالت 2: حالت 3:
      hb_barrier ();
      واپس u.hinting.get_x_delta (font);
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      واپس u.variation.get_x_delta (font, store, store_cache);
#اگر_ختم
    طےشدہ:
      واپس 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store=Null (شے_Variation_Store),
			     hb_scalar_cache_t *store_cache = خالی_اشاریہ) ثابت
  {
    انتخاب (u.b.format)
    {
    حالت 1: حالت 2: حالت 3:
#اگر_غیر_تعریف_شدہ HB_NO_HINTING
      hb_barrier ();
      واپس u.hinting.get_y_delta (font);
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      واپس u.variation.get_y_delta (font, store, store_cache);
#اگر_ختم
    طےشدہ:
      واپس 0;
    }
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (!u.b.format.sanitize (c)) return_trace (جھوٹ);
    انتخاب (u.b.format) {
#اگر_غیر_تعریف_شدہ HB_NO_HINTING
    حالت 1: حالت 2: حالت 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#اگر_ختم
    طےشدہ:
      return_trace (سچ);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map=خالی_اشاریہ) ثابت
  {
    TRACE_SERIALIZE (یہ);
    انتخاب (u.b.format) {
#اگر_غیر_تعریف_شدہ HB_NO_HINTING
    حالت 1:
    حالت 2:
    حالت 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#اگر_ختم
    طےشدہ:
      return_trace (خالی_اشاریہ);
    }
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    انتخاب (u.b.format) {
#اگر_غیر_تعریف_شدہ HB_NO_HINTING
    حالت 1:
    حالت 2:
    حالت 3:
      واپس;
#اگر_ختم
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      u.variation.collect_variation_اشاریہ (c);
      واپس;
#اگر_ختم
    طےشدہ:
      واپس;
    }
  }

  غیرعلامتی get_variation_اشاریہ () ثابت
  {
    انتخاب (u.b.format) {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      hb_barrier ();
      واپس u.variation.varIdx;
#اگر_ختم
    طےشدہ:
      واپس HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    }
  }

  منطقی is_variation_device () ثابت
  {
    انتخاب (u.b.format) {
#اگر_غیر_تعریف_شدہ HB_NO_VAR
    حالت 0x8000:
      واپس سچ;
#اگر_ختم
    طےشدہ:
      واپس جھوٹ;
    }
  }

  محفوظ:
  اتحاد {
  Device_سرخی		b;
  HintingDevice		hinting;
#اگر_غیر_تعریف_شدہ HB_NO_VAR
  VariationDevice	variation;
#اگر_ختم
  } u;
  عوامی:
  DEFINE_سائز_UNION (6, b);
};


} /* namespace OT */


#اگر_ختم /* HB_OT_LAYOUT_COMMON_HH */

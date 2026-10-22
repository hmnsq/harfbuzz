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

#جے_بنا_تعریف HB_OT_LAYOUT_COMMON_HH
#تعریف_کرو HB_OT_LAYOUT_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-set.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-cache.hh"
#ضمن "hb-ot-dual.hh"

ورتوں hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (حجم (hb_ot_layout_mapping_cache_t) == 512, "");

ورتوں hb_ot_layout_ثنائی_cache_t = hb_cache_t<14, 1, 8>;
static_assert (حجم (hb_ot_layout_ثنائی_cache_t) == 256, "");

#ضمن "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
ورتوں OT::Layout::Common::Coverage;
ورتوں OT::Layout::Common::Range_ریکارڈ;
ورتوں OT::Layout::SmallTypes;
ورتوں OT::Layout::MediumTypes;


نطاق OT {

سانچا<قسم_ناں Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ساكن منطقي ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_سیٹ_t &klasses,
    منطقي use_class_zero,
    hb_sorted_سمتیہ_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

بنية شے_Var_Store_Instancer;

بنية hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_اشاریہ_ٹیگ_map;
  ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> *axes_location;
  hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *ریکارڈ_cond_idx_map;
  hb_hashmap_t<بنانشانی, ثابت Feature*> *feature_substitutes_map;
  hb_سیٹ_t& catch_all_ریکارڈ_feature_idxes;

  // not stored in subset_plan
  hb_سیٹ_t *feature_indices;
  منطقي apply;
  منطقي variation_applied;
  منطقي universal;
  بنانشانی cur_ریکارڈ_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, بنانشانی> *conditionset_map;
  شے_Var_Store_Instancer *instancer;
  منطقي all_axes_pinned;
};

بنية hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت فراغ         *جدول_2,
                              hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_سیٹ_t           *new_collected_feature_indexes_)
      :جدول (جدول_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_گنتی (0),langsys_feature_گنتی (0) {}

  منطقي ویکھو_Script ()
  { أرجع script_گنتی++ < HB_MAX_SCRIPTS; }

  منطقي ویکھو_Langsys (بنانشانی feature_گنتی_2)
  {
    langsys_feature_گنتی += feature_گنتی_2;
    أرجع langsys_feature_گنتی < HB_MAX_LANGSYS_FEATURE_گنتی;
  }

  عام:
  ثابت فراغ *جدول;
  hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_سیٹ_t           *new_feature_indexes;

  خاص:
  بنانشانی script_گنتی;
  بنانشانی langsys_feature_گنتی;
};

بنية hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_خالی_تھاواں_t, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { أرجع "SUBSET_LAYOUT"; }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  منطقي ویکھو_Script ()
  {
    أرجع script_گنتی++ < HB_MAX_SCRIPTS;
  }

  منطقي ویکھو_Lang_Sys ()
  {
    أرجع langsys_گنتی++ < HB_MAX_LANGSYS;
  }

  منطقي ویکھو_Feature_اشاریہ (صحيح گنتی)
  {
    feature_اشاریہ_گنتی += گنتی;
    أرجع feature_اشاریہ_گنتی < HB_MAX_FEATURE_INDICES;
  }

  منطقي ویکھو_Lookup_اشاریہ()
  {
    lookup_اشاریہ_گنتی++;
    أرجع lookup_اشاریہ_گنتی < HB_MAX_LOOKUP_ویکھو_گنتی;
  }

  hb_subset_context_t *subset_context;
  ثابت hb_ٹیگ_t جدول_ٹیگ_2;
  ثابت hb_map_t *lookup_اشاریہ_map;
  ثابت hb_hashmap_t<بنانشانی, hb::unique_ptr<hb_سیٹ_t>> *script_langsys_map;
  ثابت hb_map_t *feature_اشاریہ_map;
  ثابت hb_map_t *feature_map_w_duplicates;
  ثابت hb_hashmap_t<بنانشانی, ثابت Feature*> *feature_substitutes_map;
  ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *lookup_variations;
  hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map;
  ثابت hb_سیٹ_t *catch_all_ریکارڈ_feature_idxes;
  ثابت hb_hashmap_t<بنانشانی, hb_pair_t<ثابت فراغ*, ثابت فراغ*>> *feature_idx_ٹیگ_map;

  بنانشانی cur_script_اشاریہ;
  بنانشانی cur_feature_اشاریہ;
  بنانشانی cur_feature_var_ریکارڈ_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_ٹیگ_t ٹیگ_4) :
				subset_context (c_),
				جدول_ٹیگ_2 (ٹیگ_4),
				cur_script_اشاریہ (0xFFFFu),
				cur_feature_اشاریہ (اشاریہ::NOT_لبھیا_اشاریہ),
				cur_feature_var_ریکارڈ_idx (0u),
				script_گنتی (0),
				langsys_گنتی (0),
				feature_اشاریہ_گنتی (0),
				lookup_اشاریہ_گنتی (0)
  {
    إذا (ٹیگ_4 == HB_OT_ٹیگ_GSUB)
    {
      lookup_اشاریہ_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_اشاریہ_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_ریکارڈ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? خالی_سوچک : &c_->plan->gsub_feature_ریکارڈ_cond_idx_map;
      catch_all_ریکارڈ_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_ٹیگ_map = &c_->plan->gsub_old_feature_idx_ٹیگ_map;
    }
    وإلا
    {
      lookup_اشاریہ_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_اشاریہ_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_ریکارڈ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? خالی_سوچک : &c_->plan->gpos_feature_ریکارڈ_cond_idx_map;
      catch_all_ریکارڈ_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_ٹیگ_map = &c_->plan->gpos_old_feature_idx_ٹیگ_map;
    }
  }

  خاص:
  بنانشانی script_گنتی;
  بنانشانی langsys_گنتی;
  بنانشانی feature_اشاریہ_گنتی;
  بنانشانی lookup_اشاریہ_گنتی;
};

بنية شے_Variation_Store;
بنية hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  سانچا <قسم_ناں T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (هذا); أرجع hb_خالی_تھاواں_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالی_تھاواں_t (); }

  hb_سیٹ_t *layout_variation_indices;
  ثابت hb_سیٹ_t *glyph_سیٹ;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<بنانشانی, صحيح>> *condition_اقدار = خالی_سوچک;

  hb_collect_variation_indices_context_t (hb_سیٹ_t *layout_variation_indices_,
					  ثابت hb_سیٹ_t *glyph_سیٹ_2,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_سیٹ (glyph_سیٹ_2),
					gpos_lookups (gpos_lookups_) {}
};

سانچا<قسم_ناں OutputArray>
بنية subset_وتھ_array_t
{
  subset_وتھ_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& باہرلا_4,
			 ثابت فراغ *ادھار_2) : subset_context (subset_context_),
					      باہرلا (باہرلا_4), ادھار (ادھار_2) {}

  سانچا <قسم_ناں T>
  منطقي عامل () (T&& وتھ)
  {
    خودکار snap = subset_context->serializer->snapshot ();
    خودکار *o = باہرلا.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, وتھ, ادھار);
    إذا (!ret)
    {
      باہرلا.نکالو ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &باہرلا;
  ثابت فراغ *ادھار;
};


سانچا<قسم_ناں OutputArray, قسم_ناں Arg>
بنية subset_وتھ_array_arg_t
{
  subset_وتھ_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& باہرلا_4,
			     ثابت فراغ *ادھار_2,
			     Arg &&arg_) : subset_context (subset_context_), باہرلا (باہرلا_4),
					  ادھار (ادھار_2), arg (arg_) {}

  سانچا <قسم_ناں T>
  منطقي عامل () (T&& وتھ)
  {
    خودکار snap = subset_context->serializer->snapshot ();
    خودکار *o = باہرلا.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, وتھ, ادھار, arg);
    إذا (!ret)
    {
      باہرلا.نکالو ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &باہرلا;
  ثابت فراغ *ادھار;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
بنية
{
  سانچا<قسم_ناں OutputArray>
  subset_وتھ_array_t<OutputArray>
  عامل () (hb_subset_context_t *subset_context, OutputArray& باہرلا,
	       ثابت فراغ *ادھار) ثابت
  { أرجع subset_وتھ_array_t<OutputArray> (subset_context, باہرلا, ادھار); }

  /* Variant with one extra argument passed to serialize_subset */
  سانچا<قسم_ناں OutputArray, قسم_ناں Arg>
  subset_وتھ_array_arg_t<OutputArray, Arg>
  عامل () (hb_subset_context_t *subset_context, OutputArray& باہرلا,
	       ثابت فراغ *ادھار, Arg &&arg) ثابت
  { أرجع subset_وتھ_array_arg_t<OutputArray, Arg> (subset_context, باہرلا, ادھار, arg); }
}
HB_FUNCOBJ (subset_وتھ_array);

سانچا<قسم_ناں OutputArray>
بنية subset_ریکارڈ_array_t
{
  subset_ریکارڈ_array_t (hb_subset_layout_context_t *c_, OutputArray* باہرلا_4,
			 ثابت فراغ *ادھار_2) : subset_layout_context (c_),
					      باہرلا (باہرلا_4), ادھار (ادھار_2) {}

  سانچا <قسم_ناں T>
  فراغ
  عامل () (T&& ریکارڈ)
  {
    خودکار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = ریکارڈ.subset (subset_layout_context, ادھار);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا باہرلا->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *باہرلا;
  ثابت فراغ *ادھار;
};

سانچا<قسم_ناں OutputArray, قسم_ناں Arg>
بنية subset_ریکارڈ_array_arg_t
{
  subset_ریکارڈ_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* باہرلا_4,
			     ثابت فراغ *ادھار_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   باہرلا (باہرلا_4), ادھار (ادھار_2), arg (arg_) {}

  سانچا <قسم_ناں T>
  فراغ
  عامل () (T&& ریکارڈ)
  {
    خودکار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = ریکارڈ.subset (subset_layout_context, ادھار, arg);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا باہرلا->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *باہرلا;
  ثابت فراغ *ادھار;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
بنية
{
  سانچا<قسم_ناں OutputArray>
  subset_ریکارڈ_array_t<OutputArray>
  عامل () (hb_subset_layout_context_t *c, OutputArray* باہرلا,
	       ثابت فراغ *ادھار) ثابت
  { أرجع subset_ریکارڈ_array_t<OutputArray> (c, باہرلا, ادھار); }

  /* Variant with one extra argument passed to subset */
  سانچا<قسم_ناں OutputArray, قسم_ناں Arg>
  subset_ریکارڈ_array_arg_t<OutputArray, Arg>
  عامل () (hb_subset_layout_context_t *c, OutputArray* باہرلا,
               ثابت فراغ *ادھار, Arg &&arg) ثابت
  { أرجع subset_ریکارڈ_array_arg_t<OutputArray, Arg> (c, باہرلا, ادھار, arg); }
}
HB_FUNCOBJ (subset_ریکارڈ_array);


سانچا<قسم_ناں OutputArray>
بنية serialize_math_ریکارڈ_array_t
{
  serialize_math_ریکارڈ_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& باہرلا_4,
                         ثابت فراغ *ادھار_2,
                         ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              باہرلا (باہرلا_4), ادھار (ادھار_2), variation_map (variation_map_) {}

  سانچا <قسم_ناں T>
  منطقي عامل () (T&& ریکارڈ)
  {
    إذا (!serialize_context->copy (ریکارڈ, ادھار, variation_map)) أرجع خطأ;
    باہرلا.len++;
    أرجع سچ;
  }

  خاص:
  hb_serialize_context_t *serialize_context;
  OutputArray &باہرلا;
  ثابت فراغ *ادھار;
  ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
بنية
{
  سانچا<قسم_ناں OutputArray>
  serialize_math_ریکارڈ_array_t<OutputArray>
  عامل () (hb_serialize_context_t *serialize_context, OutputArray& باہرلا,
               ثابت فراغ *ادھار,
               ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *variation_map) ثابت
  { أرجع serialize_math_ریکارڈ_array_t<OutputArray> (serialize_context, باہرلا, ادھار, variation_map); }

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

بنية اشاریہ_Array : Array16_دا<اشاریہ>
{
  منطقي intersects (ثابت hb_map_t *indexes) ثابت
  { أرجع hb_any (*هذا, indexes); }

  سانچا <قسم_ناں Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    إذا (!it) أرجع;
    إذا (unlikely (!c->extend_min ((*هذا)))) أرجع;

    لكل (ثابت خودکار _ : it)
    {
      إذا (!l->ویکھو_Lookup_اشاریہ()) اكسر;

      اشاریہ i;
      i = _;
      c->copy (i);
      هذا->len++;
    }
  }

  بنانشانی صحيح get_indexes (بنانشانی صحيح شروع_وتھ,
			    بنانشانی صحيح *گنتی_2 /* IN/OUT */,
			    بنانشانی صحيح *_indexes /*باہرلا*/) ثابت
  {
    إذا (گنتی_2 && _indexes)
    {
      + هذا->as_array ().sub_array (شروع_وتھ, گنتی_2)
      | hb_sink (hb_array (_indexes, *گنتی_2))
      ;
    }
    أرجع هذا->len;
  }

  فراغ جوڑو_indexes_منزل (hb_سیٹ_t* output /*باہرلا*/) ثابت
  {
    output->جوڑو_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
بنية Feature_Params_ناپ
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

    إذا (!design_ناپ)
      return_trace (خطأ);
    وإلا إذا (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     range_شروع == 0 &&
	     range_سرا == 0)
      return_trace (سچ);
    وإلا إذا (design_ناپ < range_شروع ||
	     design_ناپ > range_سرا ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (خطأ);
    وإلا
      return_trace (سچ);
  }

  فراغ collect_name_ids (hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  { nameids_منزل_retain->جوڑو (subfamilyNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  HBUINT16	design_ناپ;	/* Represents the design size in 720/inch
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
  HBUINT16	range_سرا;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  عام:
  DEFINE_ناپ_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
بنية Feature_Params_Stylistic_سیٹ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (هذا));
  }

  فراغ collect_name_ids (hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  { nameids_منزل_retain->جوڑو (uiNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
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
  عام:
  DEFINE_ناپ_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
بنية FeatureParamsCharacterVariants
{
  بنانشانی
  get_characters (بنانشانی شروع_وتھ, بنانشانی *char_گنتی, hb_codepoint_t *chars) ثابت
  {
    إذا (char_گنتی && chars)
    {
      + characters.as_array ().sub_array (شروع_وتھ, char_گنتی)
      | hb_sink (hb_array (chars, *char_گنتی))
      ;
    }
    أرجع characters.len;
  }

  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (characters.len, HBUINT24::static_ناپ, min_ناپ); }

  فراغ collect_name_ids (hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  {
    إذا (featUILableNameID) nameids_منزل_retain->جوڑو (featUILableNameID);
    إذا (feat_UITooltip_متن_Name_ID) nameids_منزل_retain->جوڑو (feat_UITooltip_متن_Name_ID);
    إذا (sample_متن_Name_ID) nameids_منزل_retain->جوڑو (sample_متن_Name_ID);

    إذا (!پہلا_Param_UILabel_Name_ID || !عدد_Named_Parameters || عدد_Named_Parameters >= 0x7FFF)
      أرجع;

    بنانشانی آخری_name_id = (بنانشانی) پہلا_Param_UILabel_Name_ID + (بنانشانی) عدد_Named_Parameters - 1;
    nameids_منزل_retain->جوڑو_range (پہلا_Param_UILabel_Name_ID, آخری_name_id);
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
  NameID	پہلا_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_دا<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  عام:
  DEFINE_ناپ_ARRAY (14, characters);
};

بنية FeatureParams
{
  منطقي sanitize (hb_sanitize_context_t *c, hb_ٹیگ_t ٹیگ_2) ثابت
  {
#جے_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع سچ;
#جے_انت
    TRACE_SANITIZE (هذا);
    إذا (ٹیگ_2 == HB_ٹیگ ('s','i','z','e') && hb_barrier ())
      return_trace (u.ناپ.sanitize (c));
    إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سیٹ.sanitize (c));
    إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (سچ);
  }

  فراغ collect_name_ids (hb_ٹیگ_t ٹیگ_2, hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  {
#جے_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع;
#جے_انت
    إذا (ٹیگ_2 == HB_ٹیگ ('s','i','z','e') && hb_barrier ())
      أرجع (u.ناپ.collect_name_ids (nameids_منزل_retain));
    إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('s','s','\0','\0')) /* ssXX */
      أرجع (u.stylistic_سیٹ.collect_name_ids (nameids_منزل_retain));
    إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('c','v','\0','\0')) /* cvXX */
      أرجع (u.characterVariants.collect_name_ids (nameids_منزل_retain));
  }

  منطقي subset (hb_subset_context_t *c, ثابت ٹیگ* ٹیگ_2) ثابت
  {
#جے_تعریف_شدہ HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع خطأ;
#جے_انت
    TRACE_SUBSET (هذا);
    إذا (!ٹیگ_2) return_trace (خطأ);
    إذا (*ٹیگ_2 == HB_ٹیگ ('s','i','z','e') && hb_barrier ())
      return_trace (u.ناپ.subset (c));
    إذا ((*ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سیٹ.subset (c));
    إذا ((*ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (خطأ);
  }

#جے_بنا_تعریف HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت Feature_Params_ناپ& get_ناپ_params (hb_ٹیگ_t ٹیگ_2) ثابت
  {
    إذا (ٹیگ_2 == HB_ٹیگ ('s','i','z','e') && hb_barrier ())
      أرجع u.ناپ;
    أرجع Null (Feature_Params_ناپ);
  }
  ثابت Feature_Params_Stylistic_سیٹ& get_stylistic_سیٹ_params (hb_ٹیگ_t ٹیگ_2) ثابت
  {
    إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('s','s','\0','\0')) /* ssXX */
      أرجع u.stylistic_سیٹ;
    أرجع Null (Feature_Params_Stylistic_سیٹ);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_ٹیگ_t ٹیگ_2) ثابت
  {
    إذا ((ٹیگ_2 & 0xFFFF0000u) == HB_ٹیگ ('c','v','\0','\0')) /* cvXX */
      أرجع u.characterVariants;
    أرجع Null (FeatureParamsCharacterVariants);
  }
#جے_انت

  خاص:
  اتحاد {
  Feature_Params_ناپ			ناپ;
  Feature_Params_Stylistic_سیٹ		stylistic_سیٹ;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عام:
  DEFINE_ناپ_MIN (0);
};

بنية ریکارڈ_sanitize_گھیرا_t {
  hb_ٹیگ_t ٹیگ_2;
  ثابت فراغ *list_ادھار;
};

بنية Feature
{
  بنانشانی صحيح get_lookup_گنتی () ثابت
  { أرجع lookup_اشاریہ_2.len; }
  hb_ٹیگ_t get_lookup_اشاریہ (بنانشانی صحيح i) ثابت
  { أرجع lookup_اشاریہ_2[i]; }
  بنانشانی صحيح get_lookup_indexes (بنانشانی صحيح شروع_اشاریہ,
				   بنانشانی صحيح *lookup_گنتی /* IN/OUT */,
				   بنانشانی صحيح *lookup_ٹیگ_سوچی /*باہرلا*/) ثابت
  { أرجع lookup_اشاریہ_2.get_indexes (شروع_اشاریہ, lookup_گنتی, lookup_ٹیگ_سوچی); }
  فراغ جوڑو_lookup_indexes_منزل (hb_سیٹ_t *lookup_indexes) ثابت
  { lookup_اشاریہ_2.جوڑو_indexes_منزل (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { أرجع هذا+featureParams; }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookup_اشاریہ_2.intersects (lookup_indexes); }

  فراغ collect_name_ids (hb_ٹیگ_t ٹیگ_2, hb_سیٹ_t *nameids_منزل_retain /*باہرلا*/) ثابت
  {
    إذا (featureParams)
      get_feature_params ().collect_name_ids (ٹیگ_2, nameids_منزل_retain);
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت ٹیگ                   *ٹیگ_2 = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->featureParams.serialize_subset (c, featureParams, هذا, ٹیگ_2);

    hb::shared_ptr<hb_سیٹ_t> *lookup_variations;
    إذا (l->cur_feature_اشاریہ != اشاریہ::NOT_لبھیا_اشاریہ &&
	l->lookup_variations->has (l->cur_feature_اشاریہ, &lookup_variations))
    {
      خودکار it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_اشاریہ_map)
      | hb_map (l->lookup_اشاریہ_map)
      ;
      باہرلا->lookup_اشاریہ_2.serialize (c->serializer, l, it);
    }
    وإلا
    {
      خودکار it =
      + hb_iter (lookup_اشاریہ_2)
      | hb_filter (l->lookup_اشاریہ_map)
      | hb_map (l->lookup_اشاریہ_map)
      ;
      باہرلا->lookup_اشاریہ_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_گھیرا_t *گھیرا = خالی_سوچک) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureParams.sanitize (c, هذا, گھیرا ? گھیرا->ٹیگ_2 : HB_ٹیگ_NONE) &&
		  lookup_اشاریہ_2.sanitize (c));
  }

  Offset16_منزل<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  اشاریہ_Array	 lookup_اشاریہ_2;	/* Array of LookupList indices */
  عام:
  DEFINE_ناپ_ARRAY_SIZED (4, lookup_اشاریہ_2);
};

سانچا <قسم_ناں Type>
بنية ریکارڈ_2
{
  صحيح cmp (hb_ٹیگ_t a) ثابت { أرجع ٹیگ_2.cmp (a); }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *ادھار, ثابت فراغ *f_sub = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (!f_sub)
      return_trace (باہرلا->وتھ.serialize_subset (c->subset_context, وتھ, ادھار, c, &ٹیگ_2));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    خودکار *s = c->subset_context->serializer;
    s->push ();

    باہرلا->وتھ = 0;
    منطقي ret = f.subset (c->subset_context, c, &ٹیگ_2);
    إذا (ret)
      s->جوڑو_link (باہرلا->وتھ, s->نکالو_pack ());
    وإلا
      s->نکالو_discard ();

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت ریکارڈ_sanitize_گھیرا_t گھیرا = {ٹیگ_2, ادھار};
    return_trace (c->check_struct (هذا) &&
		  وتھ.sanitize (c, ادھار, &گھیرا));
  }

  ٹیگ           ٹیگ_2;            /* 4-byte Tag identifier */
  Offset16_منزل<Type>
                وتھ;         /* Offset from beginning of object holding
                                 * the Record */
  عام:
  DEFINE_ناپ_STATIC (6);
};

سانچا <قسم_ناں Type>
بنية ریکارڈ_Array_دا : Sorted_Array16_دا<ریکارڈ_2<Type>>
{
  ثابت Offset16_منزل<Type>& get_وتھ (بنانشانی صحيح i) ثابت
  { أرجع (*هذا)[i].وتھ; }
  Offset16_منزل<Type>& get_وتھ (بنانشانی صحيح i)
  { أرجع (*هذا)[i].وتھ; }
  ثابت ٹیگ& get_ٹیگ (بنانشانی صحيح i) ثابت
  { أرجع (*هذا)[i].ٹیگ_2; }
  بنانشانی صحيح get_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
                         بنانشانی صحيح *ریکارڈ_گنتی /* IN/OUT */,
                         hb_ٹیگ_t     *ریکارڈ_ٹیگ_سوچی /*باہرلا*/) ثابت
  {
    إذا (ریکارڈ_گنتی && ریکارڈ_ٹیگ_سوچی)
    {
      + هذا->as_array ().sub_array (شروع_وتھ, ریکارڈ_گنتی)
      | hb_map (&ریکارڈ_2<Type>::ٹیگ_2)
      | hb_sink (hb_array (ریکارڈ_ٹیگ_سوچی, *ریکارڈ_گنتی))
      ;
    }
    أرجع هذا->len;
  }
  منطقي لبھو_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  {
    أرجع هذا->bfind (ٹیگ_2, اشاریہ_2, HB_NOT_لبھیا_STORE, اشاریہ::NOT_لبھیا_اشاریہ);
  }
};

سانچا <قسم_ناں Type>
بنية ریکارڈ_List_دا : ریکارڈ_Array_دا<Type>
{
  ثابت Type& عامل [] (بنانشانی صحيح i) ثابت
  { أرجع هذا+هذا->get_وتھ (i); }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    + هذا->iter ()
    | hb_apply (subset_ریکارڈ_array (l, باہرلا, هذا))
    ;
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ریکارڈ_Array_دا<Type>::sanitize (c, هذا));
  }
};

بنية ریکارڈ_List_دا_Feature : ریکارڈ_List_دا<Feature>
{
  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->feature_اشاریہ_map, hb_پہلا)
    | hb_apply ([l, باہرلا, هذا] (ثابت hb_pair_t<بنانشانی, ثابت ریکارڈ_2<Feature>&>& _)
                {
                  ثابت Feature *f_sub = خالی_سوچک;
                  ثابت Feature **f = خالی_سوچک;
                  إذا (l->feature_substitutes_map->has (_.پہلا, &f))
                    f_sub = *f;

                  l->cur_feature_اشاریہ = _.پہلا;
                  subset_ریکارڈ_array (l, باہرلا, هذا, f_sub) (_.second);
                  l->cur_feature_اشاریہ = اشاریہ::NOT_لبھیا_اشاریہ;
                })
    ;

    return_trace (سچ);
  }
};

تعريفنوع ریکارڈ_List_دا<Feature> FeatureList;


بنية LangSys
{
  بنانشانی صحيح get_feature_گنتی () ثابت
  { أرجع feature_اشاریہ_2.len; }
  hb_ٹیگ_t get_feature_اشاریہ (بنانشانی صحيح i) ثابت
  { أرجع feature_اشاریہ_2[i]; }
  بنانشانی صحيح get_feature_indexes (بنانشانی صحيح شروع_وتھ,
				    بنانشانی صحيح *feature_گنتی_2 /* IN/OUT */,
				    بنانشانی صحيح *feature_indexes /*باہرلا*/) ثابت
  { أرجع feature_اشاریہ_2.get_indexes (شروع_وتھ, feature_گنتی_2, feature_indexes); }
  فراغ جوڑو_feature_indexes_منزل (hb_سیٹ_t *feature_indexes) ثابت
  { feature_اشاریہ_2.جوڑو_indexes_منزل (feature_indexes); }

  منطقي has_required_feature () ثابت { أرجع req_Feature_اشاریہ != 0xFFFFu; }
  بنانشانی صحيح get_required_feature_اشاریہ () ثابت
  {
    إذا (req_Feature_اشاریہ == 0xFFFFu)
      أرجع اشاریہ::NOT_لبھیا_اشاریہ;
   أرجع req_Feature_اشاریہ;
  }

  LangSys* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  منطقي compare (ثابت LangSys& o, ثابت hb_map_t *feature_اشاریہ_map) ثابت
  {
    إذا (req_Feature_اشاریہ != o.req_Feature_اشاریہ)
      أرجع خطأ;

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

    لكل (; iter && o_iter; iter++, o_iter++)
    {
      بنانشانی a = *iter;
      بنانشانی b = *o_iter;
      إذا (a != b) أرجع خطأ;
    }

    إذا (iter || o_iter) أرجع خطأ;

    أرجع سچ;
  }

  فراغ collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    إذا (!has_required_feature () && !get_feature_گنتی ()) أرجع;
    إذا (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_اشاریہ))
      c->new_feature_indexes->جوڑو (get_required_feature_اشاریہ ());

    + hb_iter (feature_اشاریہ_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت ٹیگ                  *ٹیگ_2 = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    ثابت uint32_t *v;
    باہرلا->req_Feature_اشاریہ = l->feature_map_w_duplicates->has (req_Feature_اشاریہ, &v) ? *v : 0xFFFFu;

    إذا (!l->ویکھو_Feature_اشاریہ (feature_اشاریہ_2.len))
      return_trace (خطأ);

    خودکار it =
    + hb_iter (feature_اشاریہ_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقي ret = منطقي (it);
    باہرلا->feature_اشاریہ_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_گھیرا_t * = خالی_سوچک) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && feature_اشاریہ_2.sanitize (c));
  }

  Offset16	lookup_ترتیب_Z;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_اشاریہ;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  اشاریہ_Array	feature_اشاریہ_2;	/* Array of indices into the FeatureList */
  عام:
  DEFINE_ناپ_ARRAY_SIZED (6, feature_اشاریہ_2);
};
DECLARE_NULL_NAMESPACE_بائٹاں (OT, LangSys);

بنية Script
{
  بنانشانی صحيح get_lang_sys_گنتی () ثابت
  { أرجع langSys.len; }
  ثابت ٹیگ& get_lang_sys_ٹیگ (بنانشانی صحيح i) ثابت
  { أرجع langSys.get_ٹیگ (i); }
  بنانشانی صحيح get_lang_sys_ٹیگ_سوچی (بنانشانی صحيح شروع_وتھ,
				  بنانشانی صحيح *lang_sys_گنتی /* IN/OUT */,
				  hb_ٹیگ_t     *lang_sys_ٹیگ_سوچی /*باہرلا*/) ثابت
  { أرجع langSys.get_ٹیگ_سوچی (شروع_وتھ, lang_sys_گنتی, lang_sys_ٹیگ_سوچی); }
  ثابت LangSys& get_lang_sys (بنانشانی صحيح i) ثابت
  {
    إذا (i == اشاریہ::NOT_لبھیا_اشاریہ) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].وتھ;
  }
  منطقي لبھو_lang_sys_اشاریہ (hb_ٹیگ_t ٹیگ_2, بنانشانی صحيح *اشاریہ_2) ثابت
  { أرجع langSys.لبھو_اشاریہ (ٹیگ_2, اشاریہ_2); }

  منطقي has_default_lang_sys () ثابت           { أرجع defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  فراغ prune_langsys (hb_prune_langsys_context_t *c,
                      بنانشانی script_اشاریہ) ثابت
  {
    إذا (!has_default_lang_sys () && !get_lang_sys_گنتی ()) أرجع;
    إذا (!c->ویکھو_Script ()) أرجع;

    إذا (!c->script_langsys_map->has (script_اشاریہ))
    {
      إذا (unlikely (!c->script_langsys_map->سیٹ (script_اشاریہ, hb::unique_ptr<hb_سیٹ_t> {hb_سیٹ_create ()})))
	أرجع;
    }

    إذا (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      إذا (c->ویکھو_Langsys (d.get_feature_گنتی ())) {
        d.collect_features (c);
      }

      لكل (خودکار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.وتھ;
        إذا (!c->ویکھو_Langsys (l.get_feature_گنتی ())) استمر;
        إذا (l.compare (d, c->duplicate_feature_map)) استمر;

        l.collect_features (c);
        c->script_langsys_map->get (script_اشاریہ)->جوڑو (_.پہلا);
      }
    }
    وإلا
    {
      لكل (خودکار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.وتھ;
        إذا (!c->ویکھو_Langsys (l.get_feature_گنتی ())) استمر;
        l.collect_features (c);
        c->script_langsys_map->get (script_اشاریہ)->جوڑو (_.پہلا);
      }
    }
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت ٹیگ                   *ٹیگ_2) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!l->ویکھو_Script ()) return_trace (خطأ);
    إذا (ٹیگ_2 && !c->plan->layout_scripts.has (*ٹیگ_2))
      أرجع خطأ;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    منطقي defaultLang = خطأ;
    إذا (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = هذا+defaultLangSys;
      منطقي ret = ls.subset (c, l);
      إذا (!ret && ٹیگ_2 && *ٹیگ_2 != HB_ٹیگ ('D', 'F', 'L', 'T'))
      {
	c->serializer->نکالو_discard ();
	باہرلا->defaultLangSys = 0;
      }
      وإلا
      {
	c->serializer->جوڑو_link (باہرلا->defaultLangSys, c->serializer->نکالو_pack ());
	defaultLang = سچ;
      }
    }

    ثابت hb_سیٹ_t *سرگرم_langsys = l->script_langsys_map->get (l->cur_script_اشاریہ);
    إذا (سرگرم_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (سرگرم_langsys, hb_پہلا)
      | hb_map (hb_دوجا)
      | hb_filter ([=] (ثابت ریکارڈ_2<LangSys>& ریکارڈ) {أرجع l->ویکھو_Lang_Sys (); })
      | hb_apply (subset_ریکارڈ_array (l, &(باہرلا->langSys), هذا))
      ;
    }

    return_trace (منطقي (باہرلا->langSys.len) || defaultLang || l->جدول_ٹیگ_2 == HB_OT_ٹیگ_GSUB);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت ریکارڈ_sanitize_گھیرا_t * = خالی_سوچک) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (defaultLangSys.sanitize (c, هذا) && langSys.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  ریکارڈ_Array_دا<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_ناپ_ARRAY_SIZED (4, langSys);
};

بنية ریکارڈ_List_دا_Script : ریکارڈ_List_دا<Script>
{
  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    لكل (خودکار _ : + hb_enumerate (*هذا))
    {
      خودکار snap = c->serializer->snapshot ();
      l->cur_script_اشاریہ = _.پہلا;
      منطقي ret = _.second.subset (l, هذا);
      إذا (!ret) c->serializer->revert (snap);
      وإلا باہرلا->len++;
    }

    return_trace (سچ);
  }
};

تعريفنوع ریکارڈ_List_دا_Script ScriptList;



بنية LookupFlag : HBUINT16
{
  تعداد Flags {
    دایاں_منزل_بایاں		= 0x0001u,
    Ignore_ادھار_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_سیٹ	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  عام:
  DEFINE_ناپ_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
نطاق OT {

بنية Lookup
{
  بنانشانی صحيح get_subtable_گنتی () ثابت { أرجع sub_جدول.len; }

  سانچا <قسم_ناں TSub_جدول>
  ثابت Array16_دا_Offset16_منزل<TSub_جدول>& get_subtables () ثابت
  { أرجع reinterpret_cast<ثابت Array16_دا_Offset16_منزل<TSub_جدول> &> (sub_جدول); }
  سانچا <قسم_ناں TSub_جدول>
  Array16_دا_Offset16_منزل<TSub_جدول>& get_subtables ()
  { أرجع reinterpret_cast<Array16_دا_Offset16_منزل<TSub_جدول> &> (sub_جدول); }

  سانچا <قسم_ناں TSub_جدول>
  ثابت TSub_جدول& get_subtable (بنانشانی صحيح i) ثابت
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }
  سانچا <قسم_ناں TSub_جدول>
  TSub_جدول& get_subtable (بنانشانی صحيح i)
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }

  size_t get_ناپ () ثابت
  {
    ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<ثابت HBUINT16> (sub_جدول);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
      أرجع (ثابت حرف *) &StructAfter<ثابت حرف> (mark_Filtering_سیٹ) - (ثابت حرف *) هذا;
    أرجع (ثابت حرف *) &mark_Filtering_سیٹ - (ثابت حرف *) هذا;
  }

  بنانشانی صحيح get_type () ثابت { أرجع lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    بنانشانی صحيح flag = lookupFlag;
    إذا (unlikely (flag & LookupFlag::Use_Mark_Filtering_سیٹ))
    {
      ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      flag += (mark_Filtering_سیٹ << 16);
    }
    أرجع flag;
  }

  سانچا <قسم_ناں TSub_جدول, قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشانی صحيح lookup_type = get_type ();
    TRACE_DISPATCH (هذا, lookup_type);
    بنانشانی صحيح گنتی = get_subtable_گنتی ();
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++) {
      قسم_ناں context_t::return_t r = get_subtable<TSub_جدول> (i).dispatch (c, lookup_type, ds...);
      إذا (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_قدر ());
  }

  منطقي serialize (hb_serialize_context_t *c,
		  بنانشانی صحيح lookup_type,
		  uint32_t lookup_props,
		  بنانشانی صحيح عدد_subtables)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    إذا (unlikely (!sub_جدول.serialize (c, عدد_subtables))) return_trace (خطأ);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
      HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      mark_Filtering_سیٹ = lookup_props >> 16;
    }
    return_trace (سچ);
  }

  سانچا <قسم_ناں TSub_جدول>
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->lookupType = lookupType;
    باہرلا->lookupFlag = lookupFlag;

    ثابت hb_سیٹ_t *glyphset = c->plan->glyphset_gsub ();
    بنانشانی صحيح lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_جدول> ())
    | hb_filter ([هذا, glyphset, lookup_type] (ثابت Offset16_منزل<TSub_جدول> &_) { أرجع (هذا+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_وتھ_array (c, باہرلا->get_subtables<TSub_جدول> (), هذا, lookup_type))
    ;

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      hb_codepoint_t *idx;
      إذا (!c->plan->ورتیا_mark_sets_map.has (mark_Filtering_سیٹ, &idx))
      {
        بنانشانی new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_سیٹ;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        باہرلا->lookupFlag = new_flag;
      }
      وإلا
      {
        إذا (unlikely (!c->serializer->extend (باہرلا))) return_trace (خطأ);
        HBUINT16 &باہرلا_Mark_Filtering_سیٹ = StructAfter<HBUINT16> (باہرلا->sub_جدول);
        باہرلا_Mark_Filtering_سیٹ = *idx;
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

  سانچا <قسم_ناں TSub_جدول>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) && sub_جدول.sanitize (c))) return_trace (خطأ);
    hb_barrier ();

    بنانشانی subtables = get_subtable_گنتی ();
    إذا (unlikely (!c->ویکھو_subtables (subtables))) return_trace (خطأ);

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سیٹ)
    {
      ثابت HBUINT16 &mark_Filtering_سیٹ = StructAfter<HBUINT16> (sub_جدول);
      إذا (!mark_Filtering_سیٹ.sanitize (c)) return_trace (خطأ);
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
      بنانشانی صحيح type = get_subtable<TSub_جدول> (0).u.extension.get_type ();
      لكل (بنانشانی صحيح i = 1; i < subtables; i++)
	إذا (get_subtable<TSub_جدول> (i).u.extension.get_type () != type)
	  return_trace (خطأ);
    }
    return_trace (سچ);
  }

  محمي:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_دا<Offset16>
		sub_جدول;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عام:
  DEFINE_ناپ_ARRAY (6, sub_جدول);
};

سانچا <قسم_ناں Types>
ورتوں LookupList = List16_دا_وتھ_منزل<Lookup, قسم_ناں Types::HBLUINT>;

سانچا <قسم_ناں TLookup, قسم_ناں وتھ_Type>
بنية Lookup_وتھ_List : List16_دا_وتھ_منزل<TLookup, وتھ_Type>
{
  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->lookup_اشاریہ_map, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_apply (subset_وتھ_array (c, *باہرلا, هذا))
    ;
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((List16_دا_وتھ_منزل<TLookup, وتھ_Type>::sanitize (c, هذا)));
  }
};


/*
 * Coverage Table
 */


ساكن منطقي ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_سیٹ_t &klasses,
                                          منطقي use_class_zero,
                                          hb_sorted_سمتیہ_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  إذا (!klass_map)
    أرجع ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  إذا (!use_class_zero)
    klass_map->سیٹ (0, 0);

  بنانشانی idx = klass_map->has (0) ? 1 : 0;
  لكل (ثابت بنانشانی k: klasses)
  {
    إذا (klass_map->has (k)) استمر;
    klass_map->سیٹ (k, idx);
    idx++;
  }


  إذا (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    أرجع خطأ;

  لكل (بنانشانی i = 0; i < glyph_and_klass.لمائی; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  أرجع ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

سانچا <قسم_ناں Types>
بنية ClassDefFormat1_3
{
  یار بنية ClassDef;

  خاص:
  بنانشانی صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع class_قدر[(بنانشانی صحيح) (glyph_id - شروع_Glyph)];
  }

  بنانشانی get_population () ثابت
  {
    أرجع class_قدر.len;
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::ناپ == 2 ? 1 : 3;
      شروع_Glyph = 0;
      class_قدر.len = 0;
      return_trace (سچ);
    }

    hb_codepoint_t glyph_min = (*it).پہلا;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_پہلا)
			       | hb_reduce (hb_max, 0u);
    بنانشانی glyph_گنتی = glyph_max - glyph_min + 1;

    شروع_Glyph = glyph_min;
    إذا (unlikely (!class_قدر.serialize (c, glyph_گنتی))) return_trace (خطأ);
    لكل (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      بنانشانی idx = gid_klass_pair.پہلا - glyph_min;
      class_قدر[idx] = gid_klass_pair.second;
    }
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالی_سوچک /*OUT*/,
               منطقي keep_خالی_تھاواں_جدول = سچ,
               منطقي use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_sorted_سمتیہ_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سیٹ_t orig_klasses;

    hb_codepoint_t شروع = شروع_Glyph;
    hb_codepoint_t سرا   = شروع + class_قدر.len;

    لكل (ثابت hb_codepoint_t gid : + hb_range (شروع, سرا))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
      إذا (glyph_filter && !glyph_filter->has(gid)) استمر;

      بنانشانی klass = class_قدر[gid - شروع];
      إذا (!klass) استمر;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.جوڑو (klass);
    }

    إذا (use_class_zero)
    {
      ثابت hb_سیٹ_t& glyphset = *c->plan->glyphset_gsub ();
      بنانشانی glyph_گنتی = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_گنتی <= glyph_and_klass.لمائی;
    }
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_خالی_تھاواں_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && class_قدر.sanitize (c));
  }

  بنانشانی لاگت () ثابت { أرجع 1; }

  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_coverage (سیٹ_t *glyphs) ثابت
  {
    بنانشانی صحيح شروع = 0;
    بنانشانی صحيح گنتی = class_قدر.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (class_قدر[i])
	استمر;

      إذا (شروع != i)
	إذا (unlikely (!glyphs->جوڑو_range (شروع_Glyph + شروع, شروع_Glyph + i)))
	  أرجع خطأ;

      شروع = i + 1;
    }
    إذا (شروع != گنتی)
      إذا (unlikely (!glyphs->جوڑو_range (شروع_Glyph + شروع, شروع_Glyph + گنتی)))
	أرجع خطأ;

    أرجع سچ;
  }

  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_class (سیٹ_t *glyphs, بنانشانی klass) ثابت
  {
    بنانشانی صحيح گنتی = class_قدر.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (class_قدر[i] == klass) glyphs->جوڑو (شروع_Glyph + i);
    أرجع سچ;
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    hb_codepoint_t شروع = شروع_Glyph;
    hb_codepoint_t سرا = شروع_Glyph + class_قدر.len;
    لكل (hb_codepoint_t iter = شروع_Glyph - 1;
	 glyphs->next (&iter) && iter < سرا;)
      إذا (class_قدر[iter - شروع]) أرجع سچ;
    أرجع خطأ;
  }
  منطقي intersects_class (ثابت hb_سیٹ_t *glyphs, بنانشانی klass) ثابت
  {
    بنانشانی صحيح گنتی = class_قدر.len;
    إذا (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
      إذا (!glyphs->next (&g)) أرجع خطأ;
      إذا (g < شروع_Glyph) أرجع سچ;
      g = شروع_Glyph + گنتی - 1;
      إذا (glyphs->next (&g)) أرجع سچ;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت قسم_ناں Types::HBUINT *arr = class_قدر.arrayZ;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (arr[i] == klass && glyphs->has (شروع_Glyph + i))
	أرجع سچ;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, بنانشانی klass, hb_سیٹ_t *intersect_glyphs) ثابت
  {
    بنانشانی گنتی = class_قدر.len;
    إذا (klass == 0)
    {
      بنانشانی شروع_glyph = شروع_Glyph;
      لكل (uint32_t g = HB_سیٹ_قدر_INVALID;
	   glyphs->next (&g) && g < شروع_glyph;)
	intersect_glyphs->جوڑو (g);

      لكل (uint32_t g = شروع_Glyph + گنتی - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->جوڑو (g);

      أرجع;
    }

    لكل (بنانشانی i = 0; i < گنتی; i++)
      إذا (class_قدر[i] == klass && glyphs->has (شروع_Glyph + i))
	intersect_glyphs->جوڑو (شروع_Glyph + i);

#إذا 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    بنانشانی شروع_glyph = شروع_Glyph;
    بنانشانی سرا_glyph = شروع_glyph + گنتی;
    لكل (بنانشانی g = شروع_Glyph - 1;
	 glyphs->next (&g) && g < سرا_glyph;)
      إذا (class_قدر.arrayZ[g - شروع_glyph] == klass)
        intersect_glyphs->جوڑو (g);
#جے_انت
  }

  فراغ intersected_classes (ثابت hb_سیٹ_t *glyphs, hb_سیٹ_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    إذا (!class_قدر.len)
    {
      intersect_classes->جوڑو (0);
      أرجع;
    }

    hb_codepoint_t سرا_glyph = شروع_Glyph + class_قدر.len - 1;
    إذا (glyphs->get_min () < شروع_Glyph ||
        glyphs->get_max () > سرا_glyph)
      intersect_classes->جوڑو (0);

    لكل (ثابت خودکار& _ : + hb_enumerate (class_قدر))
    {
      hb_codepoint_t g = شروع_Glyph + _.پہلا;
      إذا (glyphs->has (g))
        intersect_classes->جوڑو (_.second);
    }
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  قسم_ناں Types::HBGlyphID
		 شروع_Glyph;	/* First GlyphID of the classValueArray */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::HBUINT>
		class_قدر;	/* Array of Class Values--one per GlyphID */
  عام:
  DEFINE_ناپ_ARRAY (2 + 2 * Types::ناپ, class_قدر);
};

سانچا <قسم_ناں Types>
بنية ClassDefFormat2_4
{
  یار بنية ClassDef;

  خاص:
  بنانشانی صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع range_ریکارڈ.bsearch (glyph_id).value;
  }

  بنانشانی get_population () ثابت
  {
    قسم_ناں Types::large_int ret = 0;
    لكل (ثابت خودکار &r : range_ریکارڈ)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بنانشانی) ret;
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::ناپ == 2 ? 2 : 4;
      range_ریکارڈ.len = 0;
      return_trace (سچ);
    }

    بنانشانی unsorted = خطأ;
    بنانشانی عدد_ranges = 1;
    hb_codepoint_t prev_gid = (*it).پہلا;
    بنانشانی prev_klass = (*it).second;

    Range_ریکارڈ<Types> range_rec;
    range_rec.پہلا = prev_gid;
    range_rec.آخری = prev_gid;
    range_rec.value = prev_klass;

    خودکار *ریکارڈ = c->copy (range_rec);
    إذا (unlikely (!ریکارڈ)) return_trace (خطأ);

    لكل (ثابت خودکار gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.پہلا;
      بنانشانی cur_klass = gid_klass_pair.second;

      إذا (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	إذا (unlikely (cur_gid < prev_gid))
	  unsorted = سچ;

	إذا (unlikely (!ریکارڈ)) اكسر;
	ریکارڈ->آخری = prev_gid;
	عدد_ranges++;

	range_rec.پہلا = cur_gid;
	range_rec.آخری = cur_gid;
	range_rec.value = cur_klass;

	ریکارڈ = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    إذا (unlikely (c->وچ_error ())) return_trace (خطأ);

    إذا (likely (ریکارڈ)) ریکارڈ->آخری = prev_gid;
    range_ریکارڈ.len = عدد_ranges;

    إذا (unlikely (unsorted))
      range_ریکارڈ.as_array ().qsort (Range_ریکارڈ<Types>::cmp_range);

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالی_سوچک /*OUT*/,
               منطقي keep_خالی_تھاواں_جدول = سچ,
               منطقي use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyph_سیٹ = *c->plan->glyphset_gsub ();

    hb_sorted_سمتیہ_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سیٹ_t orig_klasses;

    إذا (glyph_سیٹ.get_population () * hb_ثنائی_ہندسہ_storage ((بنانشانی) range_ریکارڈ.len)
	< get_population ())
    {
      لكل (hb_codepoint_t g : glyph_سیٹ)
      {
	بنانشانی klass = get_class (g);
	إذا (!klass) استمر;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
	إذا (glyph_filter && !glyph_filter->has (g)) استمر;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.جوڑو (klass);
      }
    }
    وإلا
    {
      بنانشانی عدد_سروت_glyphs = c->plan->سروت->get_عدد_glyphs ();
      لكل (خودکار &range : range_ریکارڈ)
      {
	بنانشانی klass = range.value;
	إذا (!klass) استمر;
	hb_codepoint_t شروع = range.پہلا;
	hb_codepoint_t سرا   = hb_min (range.آخری + 1, عدد_سروت_glyphs);
	لكل (hb_codepoint_t g = شروع; g < سرا; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
	  إذا (glyph_filter && !glyph_filter->has (g)) استمر;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.جوڑو (klass);
	}
      }
    }

    ثابت hb_سیٹ_t& glyphset = *c->plan->glyphset_gsub ();
    بنانشانی glyph_گنتی = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_گنتی <= glyph_and_klass.لمائی;
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_خالی_تھاواں_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_ریکارڈ.sanitize (c));
  }

  بنانشانی لاگت () ثابت { أرجع hb_ثنائی_ہندسہ_storage ((بنانشانی) range_ریکارڈ.len); /* bsearch cost */ }

  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_coverage (سیٹ_t *glyphs) ثابت
  {
    لكل (خودکار &range : range_ریکارڈ)
      إذا (range.value)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    أرجع سچ;
  }

  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_class (سیٹ_t *glyphs, بنانشانی صحيح klass) ثابت
  {
    لكل (خودکار &range : range_ریکارڈ)
    {
      إذا (range.value == klass)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    }
    أرجع سچ;
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    إذا (range_ریکارڈ.len > glyphs->get_population () * hb_ثنائی_ہندسہ_storage ((بنانشانی) range_ریکارڈ.len))
    {
      لكل (خودکار g : *glyphs)
        إذا (get_class (g))
	  أرجع سچ;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_ریکارڈ)
                   | hb_map ([glyphs] (ثابت Range_ریکارڈ<Types> &range) { أرجع range.intersects (*glyphs) && range.value; }));
  }
  منطقي intersects_class (ثابت hb_سیٹ_t *glyphs, بنانشانی klass) ثابت
  {
    إذا (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      إذا (!range_ریکارڈ.len)
        أرجع !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
      hb_codepoint_t آخری = HB_سیٹ_قدر_INVALID;
      خودکار it = hb_iter (range_ریکارڈ);
      لكل (خودکار &range : it)
      {
        إذا (it->پہلا == آخری + 1)
	{
	  it++;
	  استمر;
	}

	إذا (!glyphs->next (&g))
	  اكسر;
	إذا (g < range.پہلا)
	  أرجع سچ;
	g = range.آخری;
	آخری = g;
      }
      إذا (g != HB_سیٹ_قدر_INVALID && glyphs->next (&g))
	أرجع سچ;
      /* Fall through. */
    }
    لكل (ثابت خودکار &range : range_ریکارڈ)
      إذا (range.value == klass && range.intersects (*glyphs))
	أرجع سچ;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, بنانشانی klass, hb_سیٹ_t *intersect_glyphs) ثابت
  {
    إذا (klass == 0)
    {
      hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
      لكل (خودکار &range : range_ریکارڈ)
      {
	إذا (!glyphs->next (&g))
	  اذهب پورا;
	طالما (g < range.پہلا)
	{
	  intersect_glyphs->جوڑو (g);
	  إذا (!glyphs->next (&g))
	    اذهب پورا;
        }
        g = range.آخری;
      }
      طالما (glyphs->next (&g))
	intersect_glyphs->جوڑو (g);
      پورا:

      أرجع;
    }

    بنانشانی گنتی = range_ریکارڈ.len;
    إذا (گنتی > glyphs->get_population () * hb_ثنائی_ہندسہ_storage (گنتی))
    {
      لكل (خودکار g : *glyphs)
      {
        بنانشانی i;
        إذا (range_ریکارڈ.as_array ().bfind (g, &i) &&
	    range_ریکارڈ.arrayZ[i].value == klass)
	  intersect_glyphs->جوڑو (g);
      }
      أرجع;
    }

    لكل (خودکار &range : range_ریکارڈ)
    {
      إذا (range.value != klass) استمر;

      بنانشانی سرا = range.آخری + 1;
      لكل (hb_codepoint_t g = range.پہلا - 1;
	   glyphs->next (&g) && g < سرا;)
	intersect_glyphs->جوڑو (g);
    }
  }

  فراغ intersected_classes (ثابت hb_سیٹ_t *glyphs, hb_سیٹ_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    إذا (!range_ریکارڈ.len)
    {
      intersect_classes->جوڑو (0);
      أرجع;
    }

    hb_codepoint_t g = HB_سیٹ_قدر_INVALID;
    لكل (خودکار &range : range_ریکارڈ)
    {
      إذا (!glyphs->next (&g))
        اكسر;
      إذا (g < range.پہلا)
      {
        intersect_classes->جوڑو (0);
        اكسر;
      }
      g = range.آخری;
    }
    إذا (g != HB_سیٹ_قدر_INVALID && glyphs->next (&g))
      intersect_classes->جوڑو (0);

    لكل (ثابت خودکار& range : range_ریکارڈ)
      إذا (range.intersects (*glyphs))
        intersect_classes->جوڑو (range.value);
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  قسم_ناں Types::سانچا Sorted_Array_دا<Range_ریکارڈ<Types>>
		range_ریکارڈ;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::ناپ, range_ریکارڈ);
};

بنية ClassDef
{
  /* Has interface. */
  بنانشانی عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Projection. */
  hb_codepoint_t عامل () (hb_codepoint_t k) ثابت { أرجع get (k); }

  بنانشانی صحيح get (hb_codepoint_t k) ثابت { أرجع get_class (k); }
  بنانشانی صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_class (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_class (glyph_id);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_class (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_class (glyph_id);
#جے_انت
    افتراضي:أرجع 0;
    }
  }
  بنانشانی صحيح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    بنانشانی klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    klass = get_class (glyph_id);
    إذا (cache) cache->سیٹ (glyph_id, klass);
    أرجع klass;
  }

  بنانشانی get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#جے_انت
    افتراضي:أرجع NOT_COVERED;
    }
  }

  سانچا<قسم_ناں Iterator,
	   hb_requires (hb_is_sorted_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    خودکار glyphs = + it_with_class_zero | hb_filter (hb_دوجا);

    بنانشانی format = 2;
    hb_codepoint_t glyph_max = 0;
    بنانشانی class_max = 0;
    إذا (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).پہلا;
      glyph_max = glyph_min;

      بنانشانی عدد_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      بنانشانی prev_klass = (*glyphs).second;

      لكل (ثابت خودکار gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.پہلا;
	بنانشانی cur_klass = gid_klass_pair.second;
	إذا (cur_klass > class_max) class_max = cur_klass;
	إذا (cur_gid == glyph_min) استمر;
        إذا (cur_gid > glyph_max) glyph_max = cur_gid;
	إذا (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  عدد_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t بہترین_ناپ = UINT64_MAX;
      بنانشانی glyph_وسعت = glyph_max - glyph_min + 1;
      خودکار consider = [&] (بنانشانی candidate, uint64_t ناپ)
      {
	إذا (ناپ >= بہترین_ناپ) أرجع;
	بہترین_ناپ = ناپ;
	format = candidate;
      };

      إذا (glyph_max <= 0xFFFFu && glyph_وسعت <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_ناپ * glyph_وسعت);
      إذا (glyph_max <= 0xFFFFu && عدد_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID16::static_ناپ + HBUINT16::static_ناپ));
#جے_بنا_تعریف HB_NO_BEYOND_64K
      إذا (glyph_max <= 0xFFFFFFu && glyph_وسعت <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_ناپ * glyph_وسعت);
      إذا (glyph_max <= 0xFFFFFFu && عدد_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) عدد_ranges *
		     (2 * HBGlyphID24::static_ناپ + HBUINT16::static_ناپ));
#جے_انت
      إذا (unlikely (بہترین_ناپ == UINT64_MAX))
      {
	c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ);
	return_trace (خطأ);
      }
    }

    u.format.v = format;

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_دوجا)));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_دوجا)));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_دوجا)));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_دوجا)));
#جے_انت
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالی_سوچک /*OUT*/,
               منطقي keep_خالی_تھاواں_جدول = سچ,
               منطقي use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالی_سوچک) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_خالی_تھاواں_جدول, use_class_zero, glyph_filter));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_خالی_تھاواں_جدول, use_class_zero, glyph_filter));
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_خالی_تھاواں_جدول, use_class_zero, glyph_filter));
    حالة 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_خالی_تھاواں_جدول, use_class_zero, glyph_filter));
#جے_انت
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
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#جے_انت
    افتراضي:return_trace (سچ);
    }
  }

  بنانشانی لاگت () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.لاگت ();
    حالة 2: hb_barrier (); أرجع u.format2.لاگت ();
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.لاگت ();
    حالة 4: hb_barrier (); أرجع u.format4.لاگت ();
#جے_انت
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_coverage (سیٹ_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچا <قسم_ناں سیٹ_t>
  منطقي collect_class (سیٹ_t *glyphs, بنانشانی صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.collect_class (glyphs, klass);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.collect_class (glyphs, klass);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_class (ثابت hb_سیٹ_t *glyphs, بنانشانی صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_class (glyphs, klass);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_class (glyphs, klass);
#جے_انت
    افتراضي:أرجع خطأ;
    }
  }

  فراغ intersected_class_glyphs (ثابت hb_سیٹ_t *glyphs, بنانشانی klass, hb_سیٹ_t *intersect_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#جے_انت
    افتراضي:أرجع;
    }
  }

  فراغ intersected_classes (ثابت hb_سیٹ_t *glyphs, hb_سیٹ_t *intersect_classes) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_classes (glyphs, intersect_classes);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_classes (glyphs, intersect_classes);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_classes (glyphs, intersect_classes);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_classes (glyphs, intersect_classes);
#جے_انت
    افتراضي:أرجع;
    }
  }


  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#جے_بنا_تعریف HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_UNION (2, format.v);
};

سانچا<قسم_ناں Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ أرجع (c->شروع_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
بنية delta_صف_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  بنية chars_t : hb_سمتیہ_t<uint8_t>
  {
    صحيح cmp (ثابت chars_t& other) ثابت
    {
      أرجع as_array ().cmp (other.as_array ());
    }

    hb_pair_t<بنانشانی, بنانشانی> get_چوڑائی ()
    {
      بنانشانی چوڑائی = 0;
      بنانشانی columns = 0;
      لكل (بنانشانی i = 0; i < لمائی; i++)
      {
	بنانشانی v = arrayZ[i];
	چوڑائی += v;
	columns += (v != 0);
      }
      أرجع hb_pair (چوڑائی, columns);
    }

    HB_HOT
    hb_pair_t<بنانشانی, بنانشانی> combine_چوڑائی (ثابت chars_t& other) ثابت
    {
      بنانشانی combined_چوڑائی = 0;
      بنانشانی combined_columns = 0;
      لكل (بنانشانی i = 0; i < لمائی; i++)
      {
        بنانشانی v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_چوڑائی += v;
	combined_columns += (v != 0);
      }
      أرجع hb_pair (combined_چوڑائی, combined_columns);
    }
  };

  hb_pair_t<بنانشانی, بنانشانی> combine_چوڑائی (ثابت delta_صف_encoding_t& other_encoding) ثابت { أرجع chars.combine_چوڑائی (other_encoding.chars); }

  // Actual data

  chars_t chars;
  بنانشانی چوڑائی = 0;
  بنانشانی overhead = 0;
  hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح>*> اشیاء;

  delta_صف_encoding_t () = افتراضي;
  delta_صف_encoding_t (hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح>*> &&صفاں, بنانشانی عدد_خانے_گنتی)
  {
    أكد (صفاں);

    اشیاء = std::move (صفاں);

    إذا (unlikely (!chars.resize (عدد_خانے_گنتی)))
      أرجع;

    calculate_chars ();
  }

  فراغ ملاؤ (ثابت delta_صف_encoding_t& other)
  {
    اشیاء.alloc (اشیاء.لمائی + other.اشیاء.لمائی);
    لكل (خودکار &صف : other.اشیاء)
      جوڑو_صف (صف);

    // Merge chars
    أكد (chars.لمائی == other.chars.لمائی);
    لكل (بنانشانی i = 0; i < chars.لمائی; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_بدلیا ();
  }

  فراغ chars_بدلیا ()
  {
    خودکار _ = chars.get_چوڑائی ();
    چوڑائی = _.پہلا;
    overhead = get_chars_overhead (_.second);
  }

  فراغ calculate_chars ()
  {
    أكد (اشیاء);

    منطقي long_الفاظ = خطأ;

    لكل (خودکار &صف : اشیاء)
    {
      أكد (صف->لمائی == chars.لمائی);

      /* 0/1/2 byte encoding */
      لكل (بنانشانی i = 0; i < صف->لمائی; i++)
      {
	صحيح v =  صف->arrayZ[i];
	إذا (v == 0)
	  استمر;
	وإلا إذا (v > 32767 || v < -32768)
	{
	  long_الفاظ = سچ;
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
      لكل (خودکار &v : chars)
	إذا (v == 1)
	  v = 2;
    }

    chars_بدلیا ();
  }

  منطقي is_empty () ثابت
  { أرجع !اشیاء; }

  منطقي وچ_error () ثابت
  { أرجع chars.وچ_error () || اشیاء.وچ_error (); }

  ساكن مضمن بنانشانی get_chars_overhead (بنانشانی عدد_columns)
  {
    بنانشانی c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    أرجع c + عدد_columns * 2;
  }

  بنانشانی get_gain (بنانشانی additional_بائٹاں_فی_صفاں = 1) ثابت
  {
    صحيح گنتی = اشیاء.لمائی;
    أرجع hb_max (0, (صحيح) overhead - گنتی * (صحيح) additional_بائٹاں_فی_صفاں);
  }

  صحيح gain_from_merging (ثابت delta_صف_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    نشانیوالا additional_بائٹاں_فی_صفاں = other_encoding.چوڑائی - چوڑائی;
    إذا (additional_بائٹاں_فی_صفاں > 0)
    {
      إذا (get_gain (additional_بائٹاں_فی_صفاں) == 0)
        أرجع 0;
    }
    وإلا
    {
      إذا (other_encoding.get_gain (-additional_بائٹاں_فی_صفاں) == 0)
	أرجع 0;
    }

    خودکار pair = combine_چوڑائی (other_encoding);
    بنانشانی combined_چوڑائی = pair.پہلا;
    بنانشانی combined_columns = pair.second;

    صحيح combined_gain = (صحيح) overhead + (صحيح) other_encoding.overhead;
    combined_gain -= (combined_چوڑائی - (صحيح) چوڑائی) * اشیاء.لمائی;
    combined_gain -= (combined_چوڑائی - (صحيح) other_encoding.چوڑائی) * other_encoding.اشیاء.لمائی;
    combined_gain -= get_chars_overhead (combined_columns);

    أرجع combined_gain;
  }

  منطقي جوڑو_صف (ثابت hb_سمتیہ_t<صحيح>* صف)
  { أرجع اشیاء.push (صف); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت delta_صف_encoding_t *a = (ثابت delta_صف_encoding_t *)pa;
    ثابت delta_صف_encoding_t *b = (ثابت delta_صف_encoding_t *)pb;

    إذا (a->چوڑائی != b->چوڑائی)
      أرجع (صحيح) a->چوڑائی - (صحيح) b->چوڑائی;

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

    صحيح شروع = شروع_Coord.منزل_int (), سرا = سرا_Coord.منزل_int ();

    /* TODO Move these to sanitize(). */
    إذا (unlikely (شروع > peak || peak > سرا))
      أرجع 1.f;
    إذا (unlikely (شروع < 0 && سرا > 0))
      أرجع 1.f;

    إذا (coord <= شروع || سرا <= coord)
      أرجع 0.f;

    /* Interpolate */
    إذا (coord < peak)
      أرجع عائم (coord - شروع) / (peak - شروع);
    وإلا
      أرجع عائم (سرا - coord) / (سرا - peak);
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
  F2DOT14	سرا_Coord;
  عام:
  DEFINE_ناپ_STATIC (6);
};
بنية SparseVarRegionAxis
{
  عائم evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len) ثابت
  {
    بنانشانی i = axis_اشاریہ_2;
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
    خودکار *باہرلا = c->embed (src);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    إذا (axis_map)
    {
      إذا (unlikely (!axis_map->has (باہرلا->axis_اشاریہ_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (خطأ);
      }
      إذا (unlikely (!c->check_سونپو (باہرلا->axis_اشاریہ_2, axis_map->get (باہرلا->axis_اشاریہ_2),
				    HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  عام:
  HBUINT16 axis_اشاریہ_2;
  VarRegionAxis axis;
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية hb_scalar_cache_t
{
  خاص:
  ساكن constexpr بنانشانی STATIC_لمائی = 128;
  ساكن constexpr عائم INVALID = NAN;

  عام:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = مٹاؤ;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = مٹاؤ;
  hb_scalar_cache_t& عامل= (ثابت hb_scalar_cache_t&) = مٹاؤ;
  hb_scalar_cache_t& عامل= (hb_scalar_cache_t&&) = مٹاؤ;

  ساكن hb_scalar_cache_t *create (بنانشانی صحيح گنتی,
				    hb_scalar_cache_t *scratch_cache = خالی_سوچک)
  {
    إذا (!گنتی) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    إذا (scratch_cache && گنتی <= STATIC_لمائی)
    {
      scratch_cache->لمائی = گنتی;
      scratch_cache->clear ();
      أرجع scratch_cache;
    }

    خودکار *cache = (hb_scalar_cache_t *) hb_malloc (حجم (hb_scalar_cache_t) - حجم (static_اقدار) + حجم (static_اقدار[0]) * گنتی);
    إذا (unlikely (!cache)) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->لمائی = گنتی;
    cache->clear ();

    أرجع cache;
  }

  ساكن فراغ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = خالی_سوچک)
  {
    إذا (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  فراغ clear ()
  {
    خودکار *values = &static_اقدار[0];
    بنانشانی i = 0;
#جے_بنا_تعریف HB_OPTIMIZE_ناپ
    لكل (; i + 3 < لمائی; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#جے_انت
    لكل (; i < لمائی; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقي get (بنانشانی i, عائم *value) ثابت
  {
    إذا (unlikely (i >= لمائی))
    {
      *value = 0.f;
      أرجع سچ;
    }
    خودکار *values = &static_اقدار[0];
    خودکار *cached_قدر = &values[i];
    عائم v = *cached_قدر;
    إذا (std::isnan (v))
      أرجع خطأ;
    *value = v;
    أرجع سچ;
  }

  HB_ALWAYS_INLINE
  فراغ سیٹ (بنانشانی i, عائم value)
  {
    إذا (unlikely (i >= لمائی)) أرجع;
    خودکار *values = &static_اقدار[0];
    خودکار *cached_قدر = &values[i];
    *cached_قدر = value;
  }

  عام:
  بنانشانی لمائی;
  خاص:
  بدلنجوگ hb_atomic_t<عائم> static_اقدار[STATIC_لمائی];
};

بنية VarRegionList
{
  خاص:
  عائم evaluate_impl (بنانشانی صحيح region_اشاریہ,
		       ثابت صحيح *coords, بنانشانی صحيح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_اشاریہ * axis_گنتی_2);
    عائم v = 1.f;

    بنانشانی صحيح گنتی = axis_گنتی_2;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      صحيح coord = i < coord_len ? coords[i] : 0;
      عائم ضارب = axes[i].evaluate (coord);
      إذا (ضارب == 0.f)
      {
	v = 0.f;
	اكسر;
      }
      v *= ضارب;
    }

    أرجع v;
  }

  عام:
  HB_ALWAYS_INLINE
  عائم evaluate (بنانشانی صحيح region_اشاریہ,
		  ثابت صحيح *coords, بنانشانی صحيح coord_len,
		  hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    إذا (unlikely (region_اشاریہ >= region_گنتی))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_اشاریہ, &v))
      أرجع v;

    v = evaluate_impl (region_اشاریہ, coords, coord_len);

    إذا (cache)
      cache->سیٹ (region_اشاریہ, v);
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
                  ثابت hb_سمتیہ_t<hb_ٹیگ_t>& axis_ٹیگ_سوچی,
                  ثابت hb_سمتیہ_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی axis_گنتی = axis_ٹیگ_سوچی.لمائی;
    بنانشانی region_گنتی_2 = regions.لمائی;
    إذا (!axis_گنتی || !region_گنتی_2) return_trace (خطأ);
    إذا (unlikely (hb_unsigned_mul_overflows (axis_گنتی * region_گنتی_2,
                                             VarRegionAxis::static_ناپ))) return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_گنتی_2 = axis_گنتی;
    region_گنتی = region_گنتی_2;

    لكل (بنانشانی r = 0; r < region_گنتی_2; r++)
    {
      ثابت خودکار& region = regions[r];
      لكل (بنانشانی i = 0; i < axis_گنتی; i++)
      {
        hb_ٹیگ_t ٹیگ_2 = axis_ٹیگ_سوچی.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        إذا (region->has (ٹیگ_2, &coords))
        {
          var_region_rec.شروع_Coord.سیٹ_float (coords->minimum);
          var_region_rec.peakCoord.سیٹ_float (coords->درمیان);
          var_region_rec.سرا_Coord.سیٹ_float (coords->ودھ_توں_ودھ);
        }
        وإلا
        {
          var_region_rec.شروع_Coord.سیٹ_int (0);
          var_region_rec.peakCoord.سیٹ_int (0);
          var_region_rec.سرا_Coord.سیٹ_int (0);
        }
        إذا (!var_region_rec.serialize (c))
          return_trace (خطأ);
      }
    }
    return_trace (سچ);
  }

  منطقي serialize (hb_serialize_context_t *c, ثابت VarRegionList *src, ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_گنتی_2 = src->axis_گنتی_2;
    region_گنتی = region_map.get_population ();
    إذا (unlikely (hb_unsigned_mul_overflows (axis_گنتی_2 * region_گنتی,
					     VarRegionAxis::static_ناپ))) return_trace (خطأ);
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
    بنانشانی صحيح region_گنتی_2 = src->region_گنتی;
    لكل (بنانشانی صحيح r = 0; r < region_گنتی; r++)
    {
      بنانشانی صحيح backward = region_map.backward (r);
      إذا (backward >= region_گنتی_2) return_trace (خطأ);
      hb_memcpy (&axesZ[axis_گنتی_2 * r], &src->axesZ[axis_گنتی_2 * backward], VarRegionAxis::static_ناپ * axis_گنتی_2);
    }

    return_trace (سچ);
  }

  منطقي get_var_region (بنانشانی region_اشاریہ,
                       ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
                       hb_hashmap_t<hb_ٹیگ_t, Triple>& axis_tuples /*باہرلا*/) ثابت
  {
    إذا (region_اشاریہ >= region_گنتی) أرجع خطأ;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_اشاریہ * axis_گنتی_2);
    لكل (بنانشانی i = 0; i < axis_گنتی_2; i++)
    {
      hb_ٹیگ_t *axis_ٹیگ_2;
      إذا (!axes_old_اشاریہ_ٹیگ_map.has (i, &axis_ٹیگ_2))
        أرجع خطأ;

      عائم min_val = axis_region->شروع_Coord.منزل_float ();
      عائم def_val = axis_region->peakCoord.منزل_float ();
      عائم max_val = axis_region->سرا_Coord.منزل_float ();

      إذا (def_val != 0.f)
        axis_tuples.سیٹ (*axis_ٹیگ_2, Triple ((مزدوج) min_val, (مزدوج) def_val, (مزدوج) max_val));
      axis_region++;
    }
    أرجع !axis_tuples.وچ_error ();
  }

  منطقي get_var_regions (ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map,
                        hb_سمتیہ_t<hb_hashmap_t<hb_ٹیگ_t, Triple>>& regions /*باہرلا*/) ثابت
  {
    إذا (!regions.alloc (region_گنتی))
      أرجع خطأ;

    لكل (بنانشانی i = 0; i < region_گنتی; i++)
    {
      hb_hashmap_t<hb_ٹیگ_t, Triple> axis_tuples;
      إذا (!get_var_region (i, axes_old_اشاریہ_ٹیگ_map, axis_tuples))
        أرجع خطأ;
      regions.push (std::move (axis_tuples));
    }
    أرجع !regions.وچ_error ();
  }

  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_جوڑو_saturate (min_ناپ,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_ناپ,
							       axis_گنتی_2, region_گنتی)); }

  عام:
  HBUINT16	axis_گنتی_2;
  HBUINT15	region_گنتی;
  محمي:
  Unsized_Array_دا<VarRegionAxis>
		axesZ;
  عام:
  DEFINE_ناپ_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ساكن مضمن منطقي
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> &reachable_ranges)
{
  لكل (خودکار _ : axis_tuples)
  {
    Triple *range;
    إذا (!reachable_ranges.has (_.پہلا, &range)) استمر;
    ثابت Triple &tent = _.second;
    صحيح شروع = (صحيح) roundf ((عائم) tent.minimum * 16384.f);
    صحيح peak  = (صحيح) roundf ((عائم) tent.درمیان * 16384.f);
    صحيح سرا   = (صحيح) roundf ((عائم) tent.ودھ_توں_ودھ * 16384.f);
    إذا (!peak) استمر;
    إذا (شروع > peak || peak > سرا) استمر;
    إذا (شروع < 0 && سرا > 0) استمر;
    صحيح lo = (صحيح) roundf ((عائم) range->minimum * 16384.f);
    صحيح hi = (صحيح) roundf ((عائم) range->ودھ_توں_ودھ * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    إذا (hi < شروع || lo > سرا)
      أرجع سچ;
  }
  أرجع خطأ;
}

بنية SparseVariationRegion
{
  عائم evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len) ثابت
  {
    عائم v = 1.f;
    بنانشانی صحيح گنتی = axes.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      عائم ضارب = (هذا+axes[i]).evaluate (coords, coord_len);
      إذا (ضارب == 0.f)
	أرجع 0.;
      v *= ضارب;
    }
    أرجع v;
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت SparseVariationRegion *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!axes.serialize (c, (بنانشانی) src->axes.len))) return_trace (خطأ);
    لكل (بنانشانی i = 0; i < axes.len; i++)
      إذا (unlikely (src->axes[i].is_null () ||
		    !axes[i].serialize_serialize (c, &(src+src->axes[i]), axis_map)))
	return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (axes.sanitize (c, هذا));
  }

  محمي:
  Array16_دا<Offset32_منزل<SparseVarRegionAxis>> axes;
  عام:
  DEFINE_ناپ_ARRAY (2, axes);
};

بنية SparseVarRegionList
{
  HB_ALWAYS_INLINE
  عائم evaluate (بنانشانی صحيح region_اشاریہ,
		  ثابت صحيح *coords, بنانشانی صحيح coord_len,
		  hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    إذا (unlikely (region_اشاریہ >= regions.len))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_اشاریہ, &v))
      أرجع v;

    ثابت SparseVariationRegion &region = هذا+regions[region_اشاریہ];

    v = region.evaluate (coords, coord_len);
    إذا (cache)
      cache->سیٹ (region_اشاریہ, v);

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

    لكل (بنانشانی i = 0; i < regions.len; i++)
    {
      بنانشانی old_اشاریہ = region_map.backward (i);
      إذا (unlikely (old_اشاریہ >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_اشاریہ]),
					     axis_map)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  عام:
  Array32_دا<Offset32_منزل<SparseVariationRegion>>
		regions;
  عام:
  DEFINE_ناپ_ARRAY (4, regions);
};


بنية Var_انکڑے
{
  بنانشانی صحيح get_شے_گنتی () ثابت
  { أرجع شے_گنتی; }

  بنانشانی صحيح get_region_اشاریہ_گنتی () ثابت
  { أرجع regionIndices.len; }

  بنانشانی get_region_اشاریہ (بنانشانی i) ثابت
  { أرجع i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_صف_ناپ () ثابت
  { أرجع (لفظ_گنتی () + regionIndices.len) * (long_الفاظ_2 () ? 2 : 1); }

  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_جوڑو_saturate (min_ناپ - regionIndices.min_ناپ,
				     regionIndices.get_ناپ (),
				     hb_unsigned_mul_saturate (شے_گنتی, get_صف_ناپ ())); }

  سانچا <قسم_ناں Float = عائم>
  Float _get_delta (بنانشانی صحيح inner,
		    ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    إذا (unlikely (inner >= شے_گنتی))
      أرجع 0.;
    منطقي is_long = long_الفاظ_2 ();
    بنانشانی صحيح گنتی = regionIndices.len;
    بنانشانی لفظ_گنتی_2 = لفظ_گنتی ();
    بنانشانی صحيح scount = is_long ? گنتی : لفظ_گنتی_2;
    بنانشانی صحيح lcount = is_long ? لفظ_گنتی_2 : 0;

    ثابت HBUINT8 *بائٹاں = get_delta_بائٹاں ();
    ثابت HBUINT8 *صف = بائٹاں + inner * get_صف_ناپ ();

    Float delta = 0;
    بنانشانی صحيح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (صف);
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

  سانچا <قسم_ناں Float = عائم>
  HB_ALWAYS_INLINE
  Float get_delta (بنانشانی صحيح inner,
		   ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    بنانشانی صحيح گنتی = regionIndices.len;
    إذا (!گنتی) أرجع Float (0); // This is quite common, so optimize it.
    أرجع _get_delta<Float> (inner, coords, coord_گنتی, regions, cache);
  }

  فراغ get_region_scalars (ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
			   ثابت VarRegionList &regions,
			   عائم *scalars /*OUT */,
			   بنانشانی صحيح عدد_scalars) ثابت
  {
    بنانشانی گنتی = hb_min (عدد_scalars, regionIndices.len);
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_گنتی);
    لكل (بنانشانی صحيح i = گنتی; i < عدد_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  لفظ_گنتی () <= regionIndices.len &&
		  c->check_range (get_delta_بائٹاں (),
				  شے_گنتی,
				  get_صف_ناپ ()));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_سمتیہ_t<ثابت hb_سمتیہ_t<صحيح>*>& صفاں)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی صف_گنتی_3 = صفاں.لمائی;
    إذا (!صف_گنتی_3) {
      // Nothing to serialize, will be empty.
      أرجع خطأ;
    }

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!c->check_سونپو (شے_گنتی, صف_گنتی_3,
                                    HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
      return_trace (خطأ);

    صحيح min_حد = has_long ? -32768 : -128;
    صحيح max_حد = has_long ? +32767 : +127;
    تعداد delta_ناپ_t { kZero=0, k_Non_لفظ, k_لفظ };
    hb_سمتیہ_t<delta_ناپ_t> delta_sz;
    بنانشانی عدد_regions = صفاں[0]->لمائی;
    إذا (!delta_sz.resize (عدد_regions))
      return_trace (خطأ);

    بنانشانی لفظ_گنتی_2 = 0;
    لكل (بنانشانی r = 0; r < عدد_regions; r++)
    {
      لكل (بنانشانی i = 0; i < صف_گنتی_3; i++)
      {
        صحيح delta = صفاں[i]->arrayZ[r];
        إذا (delta < min_حد || delta > max_حد)
        {
          delta_sz[r] = k_لفظ;
          لفظ_گنتی_2++;
          اكسر;
        }
        وإلا إذا (delta != 0)
        {
          delta_sz[r] = k_Non_لفظ;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    بنانشانی لفظ_اشاریہ = 0;
    بنانشانی non_لفظ_اشاریہ = لفظ_گنتی_2;
    hb_map_t ri_map;
    لكل (بنانشانی r = 0; r < عدد_regions; r++)
    {
      إذا (!delta_sz[r]) استمر;
      بنانشانی new_r = (delta_sz[r] == k_لفظ)? لفظ_اشاریہ++ : non_لفظ_اشاریہ++;
      إذا (!ri_map.سیٹ (new_r, r))
        return_trace (خطأ);
    }

    لفظ_ناپ_گنتی = لفظ_گنتی_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    بنانشانی ri_گنتی = ri_map.get_population ();
    regionIndices.len = ri_گنتی;
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (بنانشانی r = 0; r < ri_گنتی; r++)
    {
      hb_codepoint_t *idx;
      إذا (!ri_map.has (r, &idx))
        return_trace (خطأ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_بائٹاں = get_delta_بائٹاں ();
    بنانشانی صف_ناپ = get_صف_ناپ ();
    لكل (بنانشانی صحيح i = 0; i < صف_گنتی_3; i++)
    {
      لكل (بنانشانی صحيح r = 0; r < ri_گنتی; r++)
      {
        صحيح delta = صفاں[i]->arrayZ[ri_map[r]];
        سیٹ_شے_delta_fast (i, r, delta, delta_بائٹاں, صف_ناپ);
      }
    }
    return_trace (سچ);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Var_انکڑے *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_سیٹ_t *culled_regions = خالی_سوچک)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    شے_گنتی = inner_map.get_اگلا_قدر ();

    /* Optimize word count */
    بنانشانی ri_گنتی = src->regionIndices.len;
    تعداد delta_ناپ_t { kZero=0, k_Non_لفظ, k_لفظ };
    hb_سمتیہ_t<delta_ناپ_t> delta_sz;
    hb_سمتیہ_t<بنانشانی صحيح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_گنتی);
    ri_map.resize (ri_گنتی);
    بنانشانی صحيح new_لفظ_گنتی = 0;
    بنانشانی صحيح r;

    ثابت HBUINT8 *src_delta_بائٹاں = src->get_delta_بائٹاں ();
    بنانشانی src_صف_ناپ = src->get_صف_ناپ ();
    بنانشانی src_لفظ_گنتی = src->لفظ_گنتی ();
    منطقي     src_long_الفاظ = src->long_الفاظ_2 ();

    منطقي has_long = خطأ;
    إذا (src_long_الفاظ)
    {
      لكل (r = 0; r < src_لفظ_گنتی; r++)
      {
        لكل (بنانشانی old_gid : inner_map.کلیدیں())
	{
	  int32_t delta = src->get_شے_delta_fast (old_gid, r, src_delta_بائٹاں, src_صف_ناپ);
	  إذا (delta < -32768 || 32767 < delta)
	  {
	    has_long = سچ;
	    اكسر;
	  }
        }
      }
    }

    نشانیوالا min_حد = has_long ? -32768 : -128;
    نشانیوالا max_حد = has_long ? +32767 : +127;
    لكل (r = 0; r < ri_گنتی; r++)
    {
      منطقي short_circuit = src_long_الفاظ == has_long && src_لفظ_گنتی <= r;

      delta_sz[r] = kZero;
      إذا (culled_regions && culled_regions->has (src->regionIndices[r]))
	استمر; /* unreachable region: drop its column */
      لكل (بنانشانی old_gid : inner_map.کلیدیں())
      {
	int32_t delta = src->get_شے_delta_fast (old_gid, r, src_delta_بائٹاں, src_صف_ناپ);
	إذا (delta < min_حد || max_حد < delta)
	{
	  delta_sz[r] = k_لفظ;
	  new_لفظ_گنتی++;
	  اكسر;
	}
	وإلا إذا (delta != 0)
	{
	  delta_sz[r] = k_Non_لفظ;
	  إذا (short_circuit)
	    اكسر;
	}
      }
    }

    بنانشانی صحيح لفظ_اشاریہ = 0;
    بنانشانی صحيح non_لفظ_اشاریہ = new_لفظ_گنتی;
    بنانشانی صحيح new_ri_گنتی = 0;
    لكل (r = 0; r < ri_گنتی; r++)
      إذا (delta_sz[r])
      {
	بنانشانی new_r = (delta_sz[r] == k_لفظ)? لفظ_اشاریہ++ : non_لفظ_اشاریہ++;
	ri_map[new_r] = r;
	new_ri_گنتی++;
      }

    لفظ_ناپ_گنتی = new_لفظ_گنتی | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_گنتی;

    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (r = 0; r < new_ri_گنتی; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_بائٹاں = get_delta_بائٹاں ();
    بنانشانی صف_ناپ = get_صف_ناپ ();
    بنانشانی گنتی = شے_گنتی;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      بنانشانی صحيح old = inner_map.backward (i);
      لكل (بنانشانی صحيح r = 0; r < new_ri_گنتی; r++)
	سیٹ_شے_delta_fast (i, r,
			     src->get_شے_delta_fast (old, ri_map[r],
						       src_delta_بائٹاں, src_صف_ناپ),
			     delta_بائٹاں, صف_ناپ);
    }

    return_trace (سچ);
  }

  فراغ collect_region_refs (hb_سیٹ_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_بائٹاں = get_delta_بائٹاں ();
    بنانشانی صف_ناپ = get_صف_ناپ ();

    لكل (بنانشانی صحيح r = 0; r < regionIndices.len; r++)
    {
      بنانشانی صحيح region = regionIndices.arrayZ[r];
      إذا (region_indices.has (region)) استمر;
      لكل (hb_codepoint_t old_gid : inner_map.کلیدیں())
	إذا (get_شے_delta_fast (old_gid, r, delta_بائٹاں, صف_ناپ) != 0)
	{
	  region_indices.جوڑو (region);
	  اكسر;
	}
    }
  }

  عام:
  ثابت HBUINT8 *get_delta_بائٹاں () ثابت
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  محمي:
  HBUINT8 *get_delta_بائٹاں ()
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  عام:
  int32_t get_شے_delta_fast (بنانشانی صحيح شے, بنانشانی صحيح region,
			       ثابت HBUINT8 *delta_بائٹاں, بنانشانی صف_ناپ) ثابت
  {
    إذا (unlikely (شے >= شے_گنتی || region >= regionIndices.len)) أرجع 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_بائٹاں + شے * صف_ناپ;
    بنانشانی لفظ_گنتی_2 = لفظ_گنتی ();
    منطقي is_long = long_الفاظ_2 ();
    إذا (is_long)
    {
      إذا (region < لفظ_گنتی_2)
	أرجع ((ثابت HBINT32 *) p)[region];
      وإلا
	أرجع ((ثابت HBINT16 *)(p + HBINT32::static_ناپ * لفظ_گنتی_2))[region - لفظ_گنتی_2];
    }
    وإلا
    {
      إذا (region < لفظ_گنتی_2)
	أرجع ((ثابت HBINT16 *) p)[region];
      وإلا
	أرجع (p + HBINT16::static_ناپ * لفظ_گنتی_2)[region - لفظ_گنتی_2];
    }
  }
  int32_t get_شے_delta (بنانشانی صحيح شے, بنانشانی صحيح region) ثابت
  {
     أرجع get_شے_delta_fast (شے, region,
				 get_delta_بائٹاں (),
				 get_صف_ناپ ());
  }

  محمي:
  فراغ سیٹ_شے_delta_fast (بنانشانی صحيح شے, بنانشانی صحيح region, int32_t delta,
			    HBUINT8 *delta_بائٹاں, بنانشانی صف_ناپ)
  {
    HBINT8 *p = (HBINT8 *) delta_بائٹاں + شے * صف_ناپ;
    بنانشانی لفظ_گنتی_2 = لفظ_گنتی ();
    منطقي is_long = long_الفاظ_2 ();
    إذا (is_long)
    {
      إذا (region < لفظ_گنتی_2)
	((HBINT32 *) p)[region] = delta;
      وإلا
	((HBINT16 *)(p + HBINT32::static_ناپ * لفظ_گنتی_2))[region - لفظ_گنتی_2] = delta;
    }
    وإلا
    {
      إذا (region < لفظ_گنتی_2)
	((HBINT16 *) p)[region] = delta;
      وإلا
	(p + HBINT16::static_ناپ * لفظ_گنتی_2)[region - لفظ_گنتی_2] = delta;
    }
  }
  فراغ سیٹ_شے_delta (بنانشانی صحيح شے, بنانشانی صحيح region, int32_t delta)
  {
    سیٹ_شے_delta_fast (شے, region, delta,
			 get_delta_بائٹاں (),
			 get_صف_ناپ ());
  }

  منطقي long_الفاظ_2 () ثابت { أرجع لفظ_ناپ_گنتی & 0x8000u /* LONG_WORDS */; }
  بنانشانی لفظ_گنتی () ثابت { أرجع لفظ_ناپ_گنتی & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محمي:
  HBUINT16		شے_گنتی;
  HBUINT16		لفظ_ناپ_گنتی;
  Array16_دا<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عام:
  DEFINE_ناپ_ARRAY (6, regionIndices);
};

بنية Multi_Var_انکڑے
{
  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_جوڑو_saturate (min_ناپ - regionIndices.min_ناپ,
				     regionIndices.get_ناپ ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { أرجع هذا+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  فراغ get_delta (بنانشانی صحيح inner,
		  ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<مزدوج> باہرلا,
		  hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    خودکار &deltaSets = get_delta_sets ();

    خودکار اقدار_iter = deltaSets.fetcher (inner);
    بنانشانی region_گنتی = regionIndices.len;
    بنانشانی skip = 0;
    لكل (بنانشانی region_اشاریہ_2 = 0; region_اشاریہ_2 < region_گنتی; region_اشاریہ_2++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[region_اشاریہ_2],
				       coords, coord_گنتی,
				       cache);
      // We skip lazily. Helps with the tail end.
      إذا (scalar == 0.0f)
        skip += باہرلا.لمائی;
      وإلا
      {
        إذا (skip)
	{
	  اقدار_iter.skip (skip);
	  skip = 0;
	}
	اقدار_iter.جوڑو_منزل_2 (باہرلا, scalar);
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

  بنانشانی get_شے_گنتی () ثابت
  { أرجع get_delta_sets ().گنتی; }

  فراغ collect_region_refs (hb_سیٹ_t &region_indices) ثابت
  {
    لكل (بنانشانی region : regionIndices)
      region_indices.جوڑو (region);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_Var_انکڑے *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی region_گنتی_2 = src->regionIndices.len;
    بنانشانی سرخی_ناپ = HBUINT8::static_ناپ +
			   Array16_دا<HBUINT16>::min_ناپ +
			   region_گنتی_2 * HBUINT16::static_ناپ +
			   HBUINT32::static_ناپ;
    إذا (unlikely (!c->extend_ناپ (هذا, سرخی_ناپ, خطأ)))
      return_trace (خطأ);
    format = 1;
    regionIndices.len = region_گنتی_2;
    لكل (بنانشانی i = 0; i < regionIndices.len; i++)
    {
      بنانشانی old_region = src->regionIndices[i];
      إذا (unlikely (!region_map.has (old_region)))
	return_trace (خطأ);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت خودکار &src_delta_sets = src->get_delta_sets ();
    hb_سمتیہ_t<hb_ubytes_t> delta_sets;
    بنانشانی انکڑے_ناپ = 0;
    إذا (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (خطأ);
    لكل (بنانشانی i = 0; i < inner_map.get_population (); i++)
    {
      بنانشانی old_inner = inner_map.backward (i);
      إذا (unlikely (old_inner >= src_delta_sets.گنتی)) return_trace (خطأ);
      hb_ubytes_t بائٹاں = static_cast<ثابت CFF2_اشاریہ &> (src_delta_sets)[old_inner];
      انکڑے_ناپ = hb_unsigned_جوڑو_saturate (انکڑے_ناپ, بائٹاں.لمائی);
      delta_sets.push (بائٹاں);
    }
    إذا (unlikely (انکڑے_ناپ == UINT_MAX || delta_sets.وچ_error ()))
      return_trace (خطأ);

    خودکار &باہرلا_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (باہرلا_delta_sets.serialize_serialize (c, delta_sets.iter (), &انکڑے_ناپ));
  }

  محمي:
  HBUINT8	      format; // 1
  Array16_دا<HBUINT16> regionIndices;
  Offset32_منزل<TupleList> deltaSetsX;
  عام:
  DEFINE_ناپ_MIN (7);
};

بنية شے_Variation_Store
{
  یار بنية شے_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#جے_انت
    أرجع hb_scalar_cache_t::create ((هذا+regions).region_گنتی);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  خاص:
  سانچا <قسم_ناں Float = عائم>
  Float get_delta (بنانشانی صحيح outer, بنانشانی صحيح inner,
		   ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		   hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع Float (0);
#جے_انت

    إذا (unlikely (outer >= انکڑے_Sets.len))
      أرجع Float (0);

    أرجع (هذا+انکڑے_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_گنتی,
					     هذا+regions,
					     cache);
  }

  عام:
  منطقي has_delta_سیٹ (بنانشانی صحيح اشاریہ_2) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع خطأ;
#جے_انت

    بنانشانی صحيح outer = اشاریہ_2 >> 16;
    بنانشانی صحيح inner = اشاریہ_2 & 0xFFFF;
    أرجع outer < انکڑے_Sets.len &&
	   inner < (هذا+انکڑے_Sets[outer]).get_شے_گنتی ();
  }

  سانچا <قسم_ناں Float = عائم>
  Float get_delta (بنانشانی صحيح اشاریہ_2,
		   ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		   hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    بنانشانی صحيح outer = اشاریہ_2 >> 16;
    بنانشانی صحيح inner = اشاریہ_2 & 0xFFFF;
    أرجع get_delta<Float> (outer, inner, coords, coord_گنتی, cache);
  }
  سانچا <قسم_ناں Float = عائم>
  Float get_delta (بنانشانی صحيح اشاریہ_2,
		   hb_array_t<ثابت صحيح> coords,
		   hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    أرجع get_delta<Float> (اشاریہ_2,
		      coords.arrayZ, coords.لمائی,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع سچ;
#جے_انت

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  انکڑے_Sets.sanitize (c, هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_سمتیہ_t<hb_ٹیگ_t>& axis_ٹیگ_سوچی,
                  ثابت hb_سمتیہ_t<ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>*>& region_list,
                  ثابت hb_سمتیہ_t<delta_صف_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (هذا);
#جے_تعریف_شدہ HB_NO_VAR
    return_trace (خطأ);
#جے_انت
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    format = 1;
    إذا (!regions.serialize_serialize (c, axis_ٹیگ_سوچی, region_list))
      return_trace (خطأ);

    بنانشانی عدد_var_انکڑے = vardata_encodings.لمائی;
    إذا (!عدد_var_انکڑے) return_trace (خطأ);
    إذا (unlikely (!c->check_سونپو (انکڑے_Sets.len, عدد_var_انکڑے,
                                    HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
      return_trace (خطأ);

    إذا (unlikely (!c->extend (انکڑے_Sets))) return_trace (خطأ);
    لكل (بنانشانی i = 0; i < عدد_var_انکڑے; i++)
      إذا (!انکڑے_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].اشیاء))
        return_trace (خطأ);

    return_trace (سچ);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت شے_Variation_Store *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_سیٹ_t *culled_regions = خالی_سوچک)
  {
    TRACE_SERIALIZE (هذا);
#جے_تعریف_شدہ HB_NO_VAR
    return_trace (خطأ);
#جے_انت

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بنانشانی صحيح سیٹ_گنتی = 0;
    لكل (بنانشانی صحيح i = 0; i < inner_maps.لمائی; i++)
      إذا (inner_maps[i].get_population ())
	سیٹ_گنتی++;

    format = 1;

    ثابت خودکار &src_regions = src+src->regions;

    hb_سیٹ_t region_indices;
    لكل (بنانشانی صحيح i = 0; i < inner_maps.لمائی; i++)
      (src+src->انکڑے_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    إذا (region_indices.وچ_error ())
      return_trace (خطأ);

    region_indices.del_range ((src_regions).region_گنتی, hb_سیٹ_t::INVALID);

    إذا (culled_regions)
      region_indices.گھٹاؤ (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (بنانشانی _) { region_map.جوڑو(_); })
    ;
    إذا (region_map.وچ_error())
      return_trace (خطأ);

    إذا (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (خطأ);

    انکڑے_Sets.len = سیٹ_گنتی;
    إذا (unlikely (!c->extend (انکڑے_Sets))) return_trace (خطأ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    بنانشانی صحيح سیٹ_اشاریہ = 0;
    لكل (بنانشانی صحيح i = 0; i < inner_maps.لمائی; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!انکڑے_Sets[سیٹ_اشاریہ++]
		     .serialize_serialize (c, &(src+src->انکڑے_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (خطأ);
    }

    return_trace (سچ);
  }

  شے_Variation_Store *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->شروع_embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خالی_سوچک);

    hb_سمتیہ_t <hb_inc_bimap_t> inner_maps;
    بنانشانی گنتی = انکڑے_Sets.len;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      إذا (unlikely (!c->propagate_error(inner_maps)))
        return_trace(خالی_سوچک);
      خودکار &انکڑے = هذا+انکڑے_Sets[i];

      بنانشانی شے_گنتی = انکڑے.get_شے_گنتی ();
      لكل (بنانشانی j = 0; j < شے_گنتی; j++)
	map->جوڑو (j);
    }

    إذا (unlikely (!باہرلا->serialize (c, هذا, inner_maps))) return_trace (خالی_سوچک);

    return_trace (باہرلا);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقي collect_dead_regions (ثابت hb_map_t &axes_old_اشاریہ_ٹیگ_map,
			     ثابت hb_hashmap_t<hb_ٹیگ_t, Triple> &reachable_ranges,
			     hb_سیٹ_t &dead_regions /*باہرلا*/) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    إذا (!reachable_ranges.get_population ()) أرجع سچ;
    ثابت VarRegionList &region_list = هذا+regions;
    بنانشانی گنتی = region_list.region_گنتی;
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      hb_hashmap_t<hb_ٹیگ_t, Triple> axis_tuples;
      إذا (!region_list.get_var_region (i, axes_old_اشاریہ_ٹیگ_map, axis_tuples))
	أرجع خطأ;
      إذا (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.جوڑو (i);
    }
    أرجع !dead_regions.وچ_error ();
#وإلا
    أرجع سچ;
#جے_انت
  }

  منطقي subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (هذا);
#جے_تعریف_شدہ HB_NO_VAR
    return_trace (خطأ);
#جے_انت

    شے_Variation_Store *varstore_مفرد = c->serializer->شروع_embed<شے_Variation_Store> ();
    إذا (unlikely (!varstore_مفرد)) return_trace (خطأ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_سیٹ_t dead_regions;
    إذا (c->plan->has_avar2)
      إذا (!collect_dead_regions (c->plan->axes_old_اشاریہ_ٹیگ_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (خطأ);

    varstore_مفرد->serialize (c->serializer, هذا, inner_maps,
			       dead_regions.get_population () ? &dead_regions : خالی_سوچک);

    return_trace (
        !c->serializer->وچ_error()
        && varstore_مفرد->انکڑے_Sets);
  }

  بنانشانی صحيح get_region_اشاریہ_گنتی (بنانشانی صحيح major) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع 0;
#جے_انت
    أرجع (هذا+انکڑے_Sets[major]).get_region_اشاریہ_گنتی ();
  }

  فراغ get_region_scalars (بنانشانی صحيح major,
			   ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
			   عائم *scalars /*OUT*/,
			   بنانشانی صحيح عدد_scalars) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    لكل (بنانشانی i = 0; i < عدد_scalars; i++)
      scalars[i] = 0.f;
    أرجع;
#جے_انت

    (هذا+انکڑے_Sets[major]).get_region_scalars (coords, coord_گنتی,
					       هذا+regions,
					       &scalars[0], عدد_scalars);
  }

  بنانشانی صحيح get_sub_جدول_گنتی () ثابت
   {
#جے_تعریف_شدہ HB_NO_VAR
     أرجع 0;
#جے_انت
     أرجع انکڑے_Sets.len;
   }

  ثابت Var_انکڑے& get_sub_جدول (بنانشانی i) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
     أرجع Null (Var_انکڑے);
#جے_انت
     أرجع هذا+انکڑے_Sets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
     أرجع Null (VarRegionList);
#جے_انت
     أرجع هذا+regions;
  }

  محمي:
  HBUINT16				format;
  Offset32_منزل<VarRegionList>		regions;
  Array16_دا_Offset32_منزل<Var_انکڑے>		انکڑے_Sets;
  عام:
  DEFINE_ناپ_ARRAY_SIZED (8, انکڑے_Sets);
};

بنية Multi_شے_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = خالی_سوچک) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#جے_انت
    خودکار &r = هذا+regions;
    بنانشانی گنتی = r.regions.len;

    أرجع hb_scalar_cache_t::create (گنتی, static_cache);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = خالی_سوچک)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  خاص:
  فراغ get_delta (بنانشانی صحيح outer, بنانشانی صحيح inner,
		  ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		  hb_array_t<مزدوج> باہرلا,
		  hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع;
#جے_انت

    إذا (unlikely (outer >= انکڑے_Sets.len))
      أرجع;

    أرجع (هذا+انکڑے_Sets[outer]).get_delta (inner,
					     coords, coord_گنتی,
					     هذا+regions,
					     باہرلا,
					     cache);
  }

  عام:
  فراغ get_delta (بنانشانی صحيح اشاریہ_2,
		  ثابت صحيح *coords, بنانشانی صحيح coord_گنتی,
		  hb_array_t<مزدوج> باہرلا,
		  hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    بنانشانی صحيح outer = اشاریہ_2 >> 16;
    بنانشانی صحيح inner = اشاریہ_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_گنتی, باہرلا, cache);
  }
  فراغ get_delta (بنانشانی صحيح اشاریہ_2,
		  hb_array_t<ثابت صحيح> coords,
		  hb_array_t<مزدوج> باہرلا,
		  hb_scalar_cache_t *cache = خالی_سوچک) ثابت
  {
    أرجع get_delta (اشاریہ_2,
		      coords.arrayZ, coords.لمائی,
		      باہرلا,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#جے_تعریف_شدہ HB_NO_VAR
    أرجع سچ;
#جے_انت

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  انکڑے_Sets.sanitize (c, هذا));
  }

  منطقي create_subset_plan (ثابت hb_سیٹ_t &var_indices,
			   hb_سمتیہ_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    إذا (unlikely (!inner_maps->resize (انکڑے_Sets.len))) أرجع خطأ;
    لكل (hb_codepoint_t var_idx : var_indices)
    {
      بنانشانی outer = var_idx >> 16;
      بنانشانی inner = var_idx & 0xFFFFu;
      إذا (unlikely (outer >= انکڑے_Sets.len ||
		    inner >= (هذا+انکڑے_Sets[outer]).get_شے_گنتی ()))
	أرجع خطأ;
      (*inner_maps)[outer].جوڑو (inner);
    }

    بنانشانی new_outer = 0;
    لكل (بنانشانی outer = 0; outer < inner_maps->لمائی; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      إذا (!inner_map.get_population ()) استمر;
      إذا (unlikely (inner_map.وچ_error ())) أرجع خطأ;
      لكل (بنانشانی new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	بنانشانی old_inner = inner_map.backward (new_inner);
	varidx_map->سیٹ ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    أرجع !inner_maps->وچ_error () && !varidx_map->وچ_error ();
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_شے_Variation_Store *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا) ||
		  inner_maps.لمائی < src->انکڑے_Sets.len))
      return_trace (خطأ);
    format = 1;

    ثابت خودکار &src_regions = src+src->regions;
    hb_سیٹ_t region_indices;
    بنانشانی سیٹ_گنتی = 0;
    لكل (بنانشانی i = 0; i < src->انکڑے_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      سیٹ_گنتی++;
      (src+src->انکڑے_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_سیٹ_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.جوڑو_سیٹ (&region_indices);
    إذا (unlikely (region_indices.وچ_error () || region_map.وچ_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (خطأ);

    انکڑے_Sets.len = سیٹ_گنتی;
    إذا (unlikely (!c->extend (انکڑے_Sets))) return_trace (خطأ);
    بنانشانی سیٹ_اشاریہ = 0;
    لكل (بنانشانی i = 0; i < src->انکڑے_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!انکڑے_Sets[سیٹ_اشاریہ++].serialize_serialize (
			    c, &(src+src->انکڑے_Sets[i]), inner_maps[i], region_map)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  محمي:
  HBUINT16				format; // 1
  Offset32_منزل<SparseVarRegionList>	regions;
  Array16_دا_Offset32_منزل<Multi_Var_انکڑے>	انکڑے_Sets;
  عام:
  DEFINE_ناپ_ARRAY_SIZED (8, انکڑے_Sets);
};

سانچا <قسم_ناں Map_گنتی_T>
بنية Delta_سیٹ_اشاریہ_Map_Format01
{
  یار بنية Delta_سیٹ_اشاریہ_Map;

  size_t get_ناپ () ثابت
  { أرجع hb_unsigned_mul_جوڑو_saturate (map_گنتی_2, get_چوڑائی (), min_ناپ); }

  خاص:
  Delta_سیٹ_اشاریہ_Map_Format01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  سانچا <قسم_ناں T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    بنانشانی صحيح چوڑائی = plan.get_چوڑائی ();
    بنانشانی صحيح inner_ثنائی_ہندسہ_گنتی = plan.get_inner_ثنائی_ہندسہ_گنتی ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (هذا);
    إذا (unlikely (output_map.لمائی && ((((inner_ثنائی_ہندسہ_گنتی-1)&~0xF)!=0) || (((چوڑائی-1)&~0x3)!=0))))
      return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    اندراج_Format = ((چوڑائی-1)<<4)|(inner_ثنائی_ہندسہ_گنتی-1);
    map_گنتی_2 = output_map.لمائی;
    HBUINT8 *p = c->allocate_ناپ<HBUINT8> (چوڑائی * output_map.لمائی);
    إذا (unlikely (!p)) return_trace (خطأ);
    لكل (بنانشانی صحيح i = 0; i < output_map.لمائی; i++)
    {
      بنانشانی صحيح v = output_map.arrayZ[i];
      إذا (v)
      {
	بنانشانی صحيح outer = v >> 16;
	بنانشانی صحيح inner = v & 0xFFFF;
	بنانشانی صحيح u = (outer << inner_ثنائی_ہندسہ_گنتی) | inner;
	لكل (بنانشانی صحيح w = چوڑائی; w > 0;)
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
  uint32_t map (بنانشانی صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    إذا (!map_گنتی_2)
      أرجع v;
    أرجع _map (v);
  }

  HB_HOT
  uint32_t _map (بنانشانی صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    إذا (v >= map_گنتی_2)
      v = map_گنتی_2 - 1;

    بنانشانی صحيح u = 0;
    { /* Fetch it. */
      بنانشانی صحيح w = get_چوڑائی ();
      ثابت HBUINT8 *p = map_انکڑے_Z.arrayZ + w * v;
      لكل (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      بنانشانی صحيح n = get_inner_ثنائی_ہندسہ_گنتی ();
      بنانشانی صحيح outer = u >> n;
      بنانشانی صحيح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    أرجع u;
  }

  بنانشانی get_map_گنتی () ثابت       { أرجع map_گنتی_2; }
  بنانشانی get_چوڑائی () ثابت           { أرجع ((اندراج_Format >> 4) & 3) + 1; }
  بنانشانی get_inner_ثنائی_ہندسہ_گنتی () ثابت { أرجع (اندراج_Format & 0xF) + 1; }


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  c->check_range (map_انکڑے_Z.arrayZ,
                                  map_گنتی_2,
                                  get_چوڑائی ()));
  }

  محمي:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       اندراج_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_گنتی_T     map_گنتی_2;       /* The number of mapping entries. */
  Unsized_Array_دا<HBUINT8>
                map_انکڑے_Z;       /* The delta-set index mapping data. */

  عام:
  DEFINE_ناپ_ARRAY (2+Map_گنتی_T::static_ناپ, map_انکڑے_Z);
};

بنية Delta_سیٹ_اشاریہ_Map
{
  سانچا <قسم_ناں T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (هذا);
    بنانشانی لمائی = plan.get_output_map ().لمائی;
    u.format.v = لمائی <= 0xFFFF ? 0 : 1;
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    افتراضي:return_trace (خطأ);
    }
  }

  uint32_t map (بنانشانی v) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع (u.format0.map (v));
    حالة 1: hb_barrier (); أرجع (u.format1.map (v));
    افتراضي:أرجع v;
    }
  }

  بنانشانی get_map_گنتی () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_map_گنتی ();
    حالة 1: hb_barrier (); أرجع u.format1.get_map_گنتی ();
    افتراضي:أرجع 0;
    }
  }

  بنانشانی get_چوڑائی () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_چوڑائی ();
    حالة 1: hb_barrier (); أرجع u.format1.get_چوڑائی ();
    افتراضي:أرجع 0;
    }
  }

  بنانشانی get_inner_ثنائی_ہندسہ_گنتی () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_inner_ثنائی_ہندسہ_گنتی ();
    حالة 1: hb_barrier (); أرجع u.format1.get_inner_ثنائی_ہندسہ_گنتی ();
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
    افتراضي:return_trace (سچ);
    }
  }

  Delta_سیٹ_اشاریہ_Map* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (reinterpret_cast<Delta_سیٹ_اشاریہ_Map *> (u.format0.copy (c)));
    حالة 1: hb_barrier (); return_trace (reinterpret_cast<Delta_سیٹ_اشاریہ_Map *> (u.format1.copy (c)));
    افتراضي:return_trace (خالی_سوچک);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }             format;         /* Format identifier */
  Delta_سیٹ_اشاریہ_Map_Format01<HBUINT16> format0;
  Delta_سیٹ_اشاریہ_Map_Format01<HBUINT32> format1;
  } u;
  عام:
  DEFINE_ناپ_UNION (1, format.v);
};


بنية شے_Var_Store_Instancer
{
  شے_Var_Store_Instancer (ثابت شے_Variation_Store *varStore_,
			 ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap,
			 hb_array_t<ثابت صحيح> coords,
			 hb_scalar_cache_t *cache = خالی_سوچک) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(شے_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    إذا (!hb_any (coords))
      هذا->coords = {};
  }

  عامل منطقي () ثابت { أرجع varStore && منطقي (coords); }

  عائم عامل[] (uint32_t varIdx) ثابت
  { أرجع (*هذا) (varIdx); }

  عائم عامل() (uint32_t varIdx, بنانشانی چھوٹا وتھ = 0) ثابت
  { أرجع get_delta (varIdx, وتھ); }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع get_delta<مزدوج> (varIdx); }

  سانچا <قسم_ناں Float = عائم>
  Float get_delta (uint32_t varIdx, بنانشانی چھوٹا وتھ = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   إذا (!coords || varIdx == VarIdx::NO_VARIATION)
     أرجع Float (0);

    varIdx += وتھ;
    إذا (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    أرجع varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت شے_Variation_Store *varStore;
  ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};

بنية Multi_شے_Var_Store_Instancer
{
  Multi_شے_Var_Store_Instancer (ثابت Multi_شے_Variation_Store *varStore,
			      ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap,
			      hb_array_t<ثابت صحيح> coords,
			      hb_scalar_cache_t *cache = خالی_سوچک) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(Multi_شے_Variation_Store);
  }

  عامل منطقي () ثابت { أرجع varStore; }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع (مزدوج) (*هذا)[varIdx]; }

  مزدوج عامل[] (uint32_t varIdx) ثابت
  {
    مزدوج v = 0;
    (*هذا) (hb_array (&v, 1), varIdx);
    أرجع v;
  }

  فراغ عامل() (hb_array_t<مزدوج> باہرلا, uint32_t varIdx, بنانشانی چھوٹا وتھ = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    إذا (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += وتھ;
      إذا (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, باہرلا, cache);
    }
    وإلا
      لكل (بنانشانی i = 0; i < باہرلا.لمائی; i++)
        باہرلا.arrayZ[i] = 0.;
  }

  ثابت Multi_شے_Variation_Store *varStore;
  ثابت Delta_سیٹ_اشاریہ_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
تعداد Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_ریکارڈ_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_ریکارڈ_WITH_VAR = 3,
};

تعداد lookup_condition_subset_نتیجہ_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

بنية Condition;

سانچا <قسم_ناں Instancer>
ساكن منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بنانشانی صحيح coord_len,
				Instancer *instancer);

بنية ConditionAxisRange
{
  یار بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    ثابت hb_map_t *اشاریہ_map = &c->plan->axes_اشاریہ_map;
    إذا (اشاریہ_map->is_empty ()) return_trace (سچ);

    ثابت hb_map_t& axes_old_اشاریہ_ٹیگ_map = c->plan->axes_old_اشاریہ_ٹیگ_map;
    hb_codepoint_t *axis_ٹیگ_2;
    إذا (!axes_old_اشاریہ_ٹیگ_map.has (axis_اشاریہ_2, &axis_ٹیگ_2) ||
        !اشاریہ_map->has (axis_اشاریہ_2))
      return_trace (خطأ);

    ثابت hb_hashmap_t<hb_ٹیگ_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_حد_بندی{-1.0, 0.0, 1.0};
    Triple *normalized_حد_بندی;
    إذا (normalized_axes_location.has (*axis_ٹیگ_2, &normalized_حد_بندی))
      axis_حد_بندی = *normalized_حد_بندی;

    ثابت hb_hashmap_t<hb_ٹیگ_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    إذا (axes_triple_distances.has (*axis_ٹیگ_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    عائم normalized_min = renormalize_قدر ((مزدوج) filter_Range_Min_قدر.منزل_float (), axis_حد_بندی, axis_triple_distances, خطأ);
    عائم normalized_max = renormalize_قدر ((مزدوج) filter_Range_Max_قدر.منزل_float (), axis_حد_بندی, axis_triple_distances, خطأ);
    باہرلا->filter_Range_Min_قدر.سیٹ_float (normalized_min);
    باہرلا->filter_Range_Max_قدر.سیٹ_float (normalized_max);

    return_trace (c->serializer->check_سونپو (باہرلا->axis_اشاریہ_2, اشاریہ_map->get (axis_اشاریہ_2),
                                               HB_SERIALIZE_ERROR_INT_حد_توں_ودھ));
  }

  خاص:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*باہرلا*/) ثابت
  {
    //invalid axis index, drop the entire record
    إذا (!c->axes_اشاریہ_ٹیگ_map->has (axis_اشاریہ_2))
      أرجع DROP_ریکارڈ_WITH_VAR;

    hb_ٹیگ_t axis_ٹیگ_2 = c->axes_اشاریہ_ٹیگ_map->get (axis_اشاریہ_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_حد_بندی;
    منطقي axis_سیٹ_by_user = خطأ;
    إذا (c->axes_location->has (axis_ٹیگ_2, &axis_حد_بندی))
    {
      axis_range = *axis_حد_بندی;
      axis_سیٹ_by_user = سچ;
    }

    عائم axis_min_val = axis_range.minimum;
    عائم axis_default_val = axis_range.درمیان;
    عائم axis_max_val = axis_range.ودھ_توں_ودھ;

    عائم filter_min_val = filter_Range_Min_قدر.منزل_float ();
    عائم filter_max_val = filter_Range_Max_قدر.منزل_float ();

    إذا (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = خطأ;

    //condition not met, drop the entire record
    إذا (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      أرجع DROP_ریکارڈ_WITH_VAR;

    //condition met and axis pinned, drop the condition
    إذا (axis_سیٹ_by_user && axis_range.is_نقطہ ())
      أرجع DROP_COND_WITH_VAR;

    إذا (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_قدر.منزل_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_قدر.منزل_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->سیٹ (axis_اشاریہ_2, val);
      أرجع KEEP_COND_WITH_VAR;
    }
    أرجع KEEP_ریکارڈ_WITH_VAR;
  }

  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
		 Instancer *instancer HB_بن_ورتیا) ثابت
  {
    صحيح coord = axis_اشاریہ_2 < coord_len ? coords[axis_اشاریہ_2] : 0;
    أرجع filter_Range_Min_قدر.منزل_int () <= coord && coord <= filter_Range_Max_قدر.منزل_int ();
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 1 */
  HBUINT16	axis_اشاریہ_2;
  F2DOT14	filter_Range_Min_قدر;
  F2DOT14	filter_Range_Max_قدر;
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية Condition_قدر
{
  یار بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    إذا (varIdx == VarIdx::NO_VARIATION)
      return_trace (سچ);

    hb_pair_t<بنانشانی, صحيح> *condition_قدر;
    إذا (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) هذا, &condition_قدر))
    {
      باہرلا->default_قدر_2 = condition_قدر->second;
      باہرلا->varIdx = condition_قدر->پہلا;
      return_trace (سچ);
    }

    hb_pair_t<بنانشانی, صحيح> *new_varidx_delta;
    إذا (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (خطأ);

    إذا (unlikely (!c->serializer->check_سونپو (
		باہرلا->default_قدر_2,
		default_قدر_2 + hb_دوجا (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
      return_trace (خطأ);
    باہرلا->varIdx = hb_پہلا (*new_varidx_delta);
    return_trace (سچ);
  }

  خاص:
  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
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
  DEFINE_ناپ_STATIC (8);
};

بنية ConditionAnd
{
  یار بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
      return_trace (خطأ);
    باہرلا->format = format;

    لكل (ثابت خودکار &وتھ : conditions)
    {
      إذا (وتھ.is_null ())
      {
	خودکار *باہرلا_وتھ = باہرلا->conditions.serialize_append (c->serializer);
	إذا (unlikely (!باہرلا_وتھ)) return_trace (خطأ);
	*باہرلا_وتھ = 0;
      }
      وإلا إذا (unlikely (!subset_وتھ_array (c, باہرلا->conditions, هذا) (وتھ)))
	return_trace (خطأ);
    }

    return_trace (سچ);
  }

  خاص:
  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بنانشانی صحيح گنتی = conditions.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					   coords, coord_len,
					   instancer))
	أرجع خطأ;
    أرجع سچ;
  }

  عام:
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  HBUINT16	format;		/* Format identifier--format = 3 */
  Array8_دا_Offset24_منزل<بنية Condition>	conditions;
  عام:
  DEFINE_ناپ_ARRAY (3, conditions);
};

بنية ConditionOr
{
  یار بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
      return_trace (خطأ);
    باہرلا->format = format;

    لكل (ثابت خودکار &وتھ : conditions)
    {
      إذا (وتھ.is_null ())
      {
	خودکار *باہرلا_وتھ = باہرلا->conditions.serialize_append (c->serializer);
	إذا (unlikely (!باہرلا_وتھ)) return_trace (خطأ);
	*باہرلا_وتھ = 0;
      }
      وإلا إذا (unlikely (!subset_وتھ_array (c, باہرلا->conditions, هذا) (وتھ)))
	return_trace (خطأ);
    }

    return_trace (سچ);
  }

  خاص:
  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بنانشانی صحيح گنتی = conditions.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (_hb_recurse_condition_evaluate (هذا+conditions.arrayZ[i],
					  coords, coord_len,
					  instancer))
	أرجع سچ;
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
  Array8_دا_Offset24_منزل<بنية Condition>	conditions;
  عام:
  DEFINE_ناپ_ARRAY (3, conditions);
};

بنية ConditionNegate
{
  یار بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);
    إذا (condition.is_null ())
    {
      باہرلا->condition = 0;
      return_trace (سچ);
    }
    return_trace (باہرلا->condition.serialize_subset (c, condition, هذا));
  }

  خاص:
  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
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
  DEFINE_ناپ_STATIC (5);
};

بنية Condition
{
  یار بنية Condition_سیٹ;

  ساكن منطقي serialize_constant (hb_serialize_context_t *c, منطقي value)
  {
    خودکار *باہرلا = c->شروع_embed<Condition_قدر> ();
    إذا (unlikely (!باہرلا || !c->extend_min (باہرلا))) أرجع خطأ;
    باہرلا->format = 2;
    باہرلا->default_قدر_2 = value ? 1 : 0;
    باہرلا->varIdx = VarIdx::NO_VARIATION;
    أرجع سچ;
  }

  lookup_condition_subset_نتیجہ_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      بنانشانی ڈونگھائی = HB_MAX_NESTING_LEVEL,
				      منطقي materialize_true = خطأ) ثابت;

  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بدل (u.format.v) {
    /* A null condition offset resolves to the nil Condition, whose reserved
     * format 0 represents the True condition required by the specification. */
    حالة 0: أرجع سچ;
    حالة 1: hb_barrier (); أرجع u.format1.evaluate (coords, coord_len, instancer);
    حالة 2: hb_barrier (); أرجع u.format2.evaluate (coords, coord_len, instancer);
    حالة 3: hb_barrier (); أرجع u.format3.evaluate (coords, coord_len, instancer);
    حالة 4: hb_barrier (); أرجع u.format4.evaluate (coords, coord_len, instancer);
    حالة 5: hb_barrier (); أرجع u.format5.evaluate (coords, coord_len, instancer);
    افتراضي:أرجع خطأ;
    }
  }

  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*باہرلا*/,
                                             بنانشانی ڈونگھائی = HB_MAX_NESTING_LEVEL) ثابت
  {
    إذا (unlikely (!ڈونگھائی))
    {
      c->apply = خطأ;
      أرجع DROP_ریکارڈ_WITH_VAR;
    }

    بدل (u.format.v) {
    حالة 0: أرجع KEEP_ریکارڈ_WITH_VAR;
    حالة 1: hb_barrier (); أرجع u.format1.keep_with_variations (c, condition_map);
    حالة 2: حالة 3: حالة 4: حالة 5: اكسر;
    افتراضي: c->apply = خطأ; أرجع DROP_ریکارڈ_WITH_VAR;
    }

    hb_barrier ();
    منطقي applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.لمائی, c->instancer);
    c->apply &= applies;
    إذا (c->all_axes_pinned)
      أرجع applies ? DROP_COND_WITH_VAR : DROP_ریکارڈ_WITH_VAR;

    إذا (u.format.v == 2)
      أرجع u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_سیٹ (u.format2.varIdx) ?
             (applies ? KEEP_ریکارڈ_WITH_VAR : DROP_ریکارڈ_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    خودکار classify_بچہ = [c, condition_map, ڈونگھائی] (ثابت Condition &بچہ)
    {
      خودکار بچہ_context = *c;
      بچہ_context.apply = سچ;
      أرجع بچہ.keep_with_variations (&بچہ_context, condition_map, ڈونگھائی - 1);
    };

    إذا (u.format.v == 5)
    {
      خودکار نتیجہ = classify_بچہ (&u.format5 + u.format5.condition);
      أرجع نتیجہ == DROP_ریکارڈ_WITH_VAR ? KEEP_ریکارڈ_WITH_VAR :
             نتیجہ == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_ریکارڈ_WITH_VAR;
    }

    منطقي is_and = u.format.v == 3;
    منطقي variable = خطأ;
    ثابت خودکار &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت فراغ *ادھار = is_and ? (ثابت فراغ *) &u.format3 : (ثابت فراغ *) &u.format4;
    لكل (ثابت خودکار &وتھ : conditions)
    {
      خودکار نتیجہ = classify_بچہ (ادھار + وتھ);
      إذا (is_and && نتیجہ == DROP_ریکارڈ_WITH_VAR)
        أرجع DROP_ریکارڈ_WITH_VAR;
      إذا (!is_and && (نتیجہ == DROP_COND_WITH_VAR || نتیجہ == KEEP_ریکارڈ_WITH_VAR))
        أرجع KEEP_ریکارڈ_WITH_VAR;
      variable |= نتیجہ == KEEP_COND_WITH_VAR;
    }
    أرجع variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_ریکارڈ_WITH_VAR : DROP_ریکارڈ_WITH_VAR;
  }

  سانچا <قسم_ناں context_t, قسم_ناں ...Ts>
  قسم_ناں context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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

    إذا (unlikely (!c->check_شروع_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->سرا_recursion (هذا->dispatch (c)));
  }

  منطقي collect_var_indices (hb_سیٹ_t *var_indices,
			    بنانشانی ڈونگھائی = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<بنانشانی, صحيح>> *condition_اقدار = خالی_سوچک) ثابت;

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = خالی_سوچک);

  خاص:
  lookup_condition_subset_نتیجہ_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      بنانشانی ڈونگھائی) ثابت;

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
  DEFINE_ناپ_UNION (2, format.v);
};

سانچا <قسم_ناں Instancer>
منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بنانشانی صحيح coord_len,
				Instancer *instancer)
{
  أرجع condition.evaluate (coords, coord_len, instancer);
}

بنية ConditionList
{
  ثابت Condition& عامل[] (بنانشانی i) ثابت
  { أرجع هذا+conditions[i]; }

  بنانشانی get_گنتی () ثابت { أرجع conditions.len; }

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
    لكل (بنانشانی i = 0; i < conditions.len; i++)
    {
      بنانشانی old_اشاریہ = condition_map.backward (i);
      إذا (unlikely (old_اشاریہ >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_اشاریہ]), varidx_map, axis_map)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  محمي:
  Array32_دا_Offset32_منزل<Condition> conditions;
  عام:
  DEFINE_ناپ_ARRAY (4, conditions);
};

مضمن منطقي
Condition::collect_var_indices (hb_سیٹ_t *var_indices, بنانشانی ڈونگھائی,
                               hb_hashmap_t<uintptr_t, hb_pair_t<بنانشانی, صحيح>> *condition_اقدار) ثابت
{
  إذا (unlikely (!ڈونگھائی)) أرجع خطأ;
  بدل (u.format.v)
  {
    حالة 0:
    حالة 1:
      أرجع سچ;
    حالة 2:
      إذا (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->جوڑو (u.format2.varIdx);
        إذا (condition_اقدار &&
            !condition_اقدار->سیٹ ((uintptr_t) &u.format2,
                                    hb_pair (بنانشانی (u.format2.varIdx), صحيح (u.format2.default_قدر_2))))
          أرجع خطأ;
      }
      أرجع !var_indices->وچ_error ();
    حالة 3:
      لكل (ثابت خودکار &وتھ : u.format3.conditions)
	إذا (unlikely (!(&u.format3+وتھ).collect_var_indices (var_indices,
							     ڈونگھائی - 1, condition_اقدار)))
	  أرجع خطأ;
      أرجع سچ;
    حالة 4:
      لكل (ثابت خودکار &وتھ : u.format4.conditions)
	إذا (unlikely (!(&u.format4+وتھ).collect_var_indices (var_indices,
							     ڈونگھائی - 1, condition_اقدار)))
	  أرجع خطأ;
      أرجع سچ;
    حالة 5:
      أرجع (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  ڈونگھائی - 1, condition_اقدار);
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
      خودکار *باہرلا = c->شروع_embed<Condition_قدر> ();
      إذا (unlikely (!باہرلا || !c->extend_min (باہرلا))) return_trace (خطأ);
      باہرلا->format = 2;
      باہرلا->default_قدر_2 = 1;
      باہرلا->varIdx = VarIdx::NO_VARIATION;
      return_trace (سچ);
    }
    حالة 1:
    {
      خودکار *باہرلا = c->embed (&src->u.format1);
      إذا (unlikely (!باہرلا)) return_trace (خطأ);
      إذا (axis_map)
      {
	إذا (unlikely (!axis_map->has (باہرلا->axis_اشاریہ_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (خطأ);
	}
	إذا (unlikely (!c->check_سونپو (باہرلا->axis_اشاریہ_2, axis_map->get (باہرلا->axis_اشاریہ_2),
				      HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
	  return_trace (خطأ);
      }
      return_trace (سچ);
    }
    حالة 2:
    {
      خودکار *باہرلا = c->embed (&src->u.format2);
      إذا (unlikely (!باہرلا ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (خطأ);
      باہرلا->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (سچ);
    }
    حالة 3:
    {
      خودکار *باہرلا = c->شروع_embed<ConditionAnd> ();
      إذا (unlikely (!c->extend_min (باہرلا))) return_trace (خطأ);
      باہرلا->format = 3;
      باہرلا->conditions.len = src->u.format3.conditions.len;
      إذا (unlikely (!c->extend (باہرلا->conditions))) return_trace (خطأ);
      لكل (بنانشانی i = 0; i < باہرلا->conditions.len; i++)
	إذا (unlikely (!باہرلا->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (سچ);
    }
    حالة 4:
    {
      خودکار *باہرلا = c->شروع_embed<ConditionOr> ();
      إذا (unlikely (!c->extend_min (باہرلا))) return_trace (خطأ);
      باہرلا->format = 4;
      باہرلا->conditions.len = src->u.format4.conditions.len;
      إذا (unlikely (!c->extend (باہرلا->conditions))) return_trace (خطأ);
      لكل (بنانشانی i = 0; i < باہرلا->conditions.len; i++)
	إذا (unlikely (!باہرلا->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (سچ);
    }
    حالة 5:
    {
      خودکار *باہرلا = c->embed (&src->u.format5);
      إذا (unlikely (!باہرلا ||
		    !باہرلا->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (خطأ);
      return_trace (سچ);
    }
    افتراضي:
      return_trace (خطأ);
  }
}

مضمن lookup_condition_subset_نتیجہ_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    بنانشانی ڈونگھائی,
				    منطقي materialize_true) ثابت
{
  خودکار snap = c->serializer->snapshot ();
  lookup_condition_subset_نتیجہ_t نتیجہ =
      subset_lookup_condition_impl (c, ڈونگھائی);
  إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_KEEP)
    أرجع نتیجہ;

  c->serializer->revert (snap);
  إذا (نتیجہ != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    أرجع نتیجہ;

  أرجع serialize_constant (c->serializer, سچ) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

مضمن lookup_condition_subset_نتیجہ_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 بنانشانی ڈونگھائی) ثابت
{
  إذا (unlikely (!ڈونگھائی))
    أرجع LOOKUP_CONDITION_SUBSET_ERROR;

  خودکار subset_وتھ = [c, ڈونگھائی] (Array8_دا_Offset24_منزل<Condition> &باہرلا,
				   ثابت Condition &condition)
      -> lookup_condition_subset_نتیجہ_t
  {
    خودکار snap = c->serializer->snapshot ();
    خودکار *وتھ = باہرلا.serialize_append (c->serializer);
    إذا (unlikely (!وتھ))
      أرجع LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_نتیجہ_t نتیجہ =
        condition.subset_lookup_condition (c, ڈونگھائی - 1);
    إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->جوڑو_link (*وتھ, c->serializer->نکالو_pack ());
    وإلا
    {
      c->serializer->نکالو_discard ();
      باہرلا.نکالو ();
      c->serializer->revert (snap);
    }
    أرجع نتیجہ;
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

      hb_codepoint_t *axis_ٹیگ_2;
      إذا (!c->plan->axes_old_اشاریہ_ٹیگ_map.has (src.axis_اشاریہ_2, &axis_ٹیگ_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_حد_بندی {-1.f, 0.f, 1.f};
      Triple *normalized_حد_بندی;
      إذا (c->plan->axes_location.has (*axis_ٹیگ_2, &normalized_حد_بندی))
        axis_حد_بندی = *normalized_حد_بندی;

      عائم filter_min = src.filter_Range_Min_قدر.منزل_float ();
      عائم filter_max = src.filter_Range_Max_قدر.منزل_float ();
      إذا (filter_min > filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد_بندی.is_نقطہ ())
        أرجع filter_min <= axis_حد_بندی.درمیان &&
	       axis_حد_بندی.درمیان <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد_بندی.ودھ_توں_ودھ < filter_min ||
	  filter_max < axis_حد_بندی.minimum)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (filter_min <= axis_حد_بندی.minimum &&
	  axis_حد_بندی.ودھ_توں_ودھ <= filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      إذا (!c->plan->axes_اشاریہ_map.has (src.axis_اشاریہ_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      خودکار *باہرلا = c->serializer->embed (&src);
      إذا (unlikely (!باہرلا))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      إذا (c->plan->axes_triple_distances.has (*axis_ٹیگ_2, &distances))
        axis_distances = *distances;

      باہرلا->filter_Range_Min_قدر.سیٹ_float (
	  renormalize_قدر (filter_min, axis_حد_بندی, axis_distances, خطأ));
      باہرلا->filter_Range_Max_قدر.سیٹ_float (
	  renormalize_قدر (filter_max, axis_حد_بندی, axis_distances, خطأ));
      إذا (unlikely (!c->serializer->check_سونپو (
		باہرلا->axis_اشاریہ_2,
		c->plan->axes_اشاریہ_map.get (src.axis_اشاریہ_2),
		HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 2:
    {
      ثابت Condition_قدر &src = u.format2;
      خودکار *باہرلا = c->serializer->embed (&src);
      إذا (unlikely (!باہرلا))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      إذا (src.varIdx == VarIdx::NO_VARIATION)
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<بنانشانی, صحيح> *condition_قدر;
      إذا (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) &src, &condition_قدر))
      {
        باہرلا->default_قدر_2 = condition_قدر->second;
        باہرلا->varIdx = condition_قدر->پہلا;
        إذا (باہرلا->varIdx == VarIdx::NO_VARIATION)
          أرجع باہرلا->default_قدر_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        أرجع LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<بنانشانی, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      إذا (!c->plan->user_axes_location.is_empty () &&
          hb_پہلا (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        مزدوج value = (صحيح) src.default_قدر_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        أرجع value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحيح default_قدر = (صحيح) src.default_قدر_2 +
			  hb_دوجا (*new_varidx_delta);
      إذا (unlikely (!c->serializer->check_سونپو (
		باہرلا->default_قدر_2,
		default_قدر,
		HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      باہرلا->varIdx = hb_پہلا (*new_varidx_delta);

      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 3:
    {
      ثابت ConditionAnd &src = u.format3;
      خودکار *باہرلا = c->serializer->شروع_embed (&src);
      إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      باہرلا->format = 3;

      لكل (ثابت خودکار &وتھ : src.conditions)
      {
        lookup_condition_subset_نتیجہ_t نتیجہ =
            subset_وتھ (باہرلا->conditions, &src + وتھ);
        إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتیجہ == LOOKUP_CONDITION_SUBSET_FALSE)
          أرجع نتیجہ;
      }
      أرجع باہرلا->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالة 4:
    {
      ثابت ConditionOr &src = u.format4;
      خودکار *باہرلا = c->serializer->شروع_embed (&src);
      إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      باہرلا->format = 4;

      لكل (ثابت خودکار &وتھ : src.conditions)
      {
        lookup_condition_subset_نتیجہ_t نتیجہ =
            subset_وتھ (باہرلا->conditions, &src + وتھ);
        إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتیجہ == LOOKUP_CONDITION_SUBSET_TRUE)
          أرجع نتیجہ;
      }
      أرجع باہرلا->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالة 5:
    {
      ثابت ConditionNegate &src = u.format5;
      خودکار *باہرلا = c->serializer->embed (&src);
      إذا (unlikely (!باہرلا))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      باہرلا->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_نتیجہ_t نتیجہ =
          (&src + src.condition).subset_lookup_condition (c, ڈونگھائی - 1);
      إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->جوڑو_link (باہرلا->condition,
				 c->serializer->نکالو_pack ());
        أرجع نتیجہ;
      }

      c->serializer->نکالو_discard ();
      إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_TRUE)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_FALSE)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      أرجع نتیجہ;
    }

    افتراضي:
      أرجع LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

بنية Condition_سیٹ
{
  منطقي evaluate (ثابت صحيح *coords, بنانشانی صحيح coord_len,
		 شے_Var_Store_Instancer *instancer) ثابت
  {
    بنانشانی صحيح گنتی = conditions.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
      إذا (!(هذا+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	أرجع خطأ;
    أرجع سچ;
  }

  فراغ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    إذا (unlikely (!condition_map)) أرجع;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_سیٹ_t *cond_سیٹ = hb_سیٹ_create ();
    إذا (unlikely (!cond_سیٹ)) أرجع;
    hb::shared_ptr<hb_سیٹ_t> s {cond_سیٹ};

    c->apply = سچ;
    منطقي should_keep = خطأ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقي can_deduplicate = سچ;
    بنانشانی عدد_kept_cond = 0, cond_idx = 0;
    لكل (ثابت خودکار& وتھ : conditions)
    {
      ثابت Condition &condition = هذا+وتھ;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      إذا (ret == DROP_ریکارڈ_WITH_VAR)
      {
        c->apply = خطأ;
        أرجع;
      }

      إذا (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = سچ;
        cond_سیٹ->جوڑو (cond_idx);
        عدد_kept_cond++;
      }

      إذا (ret == KEEP_ریکارڈ_WITH_VAR)
        should_keep = سچ;

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
      c->conditionset_map->سیٹ (p, 1);
    c->ریکارڈ_cond_idx_map->سیٹ (c->cur_ریکارڈ_idx, s);
  }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقي داخل_کرو_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    إذا (داخل_کرو_catch_all) return_trace (سچ);

    hb_سیٹ_t *retained_cond_سیٹ = خالی_سوچک;
    إذا (l->feature_ریکارڈ_cond_idx_map != خالی_سوچک)
      retained_cond_سیٹ = l->feature_ریکارڈ_cond_idx_map->get (l->cur_feature_var_ریکارڈ_idx);

    بنانشانی صحيح گنتی = conditions.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (retained_cond_سیٹ != خالی_سوچک && !retained_cond_سیٹ->has (i))
        استمر;
      ثابت Condition &condition = هذا+conditions[i];
      إذا (condition.u.format.v == 1)
      {
        subset_وتھ_array (c, باہرلا->conditions, هذا) (conditions[i]);
        استمر;
      }

      خودکار *وتھ = باہرلا->conditions.serialize_append (c->serializer);
      إذا (unlikely (!وتھ)) return_trace (خطأ);
      c->serializer->push ();
      خودکار نتیجہ = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, سچ);
      إذا (نتیجہ == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        نتیجہ = Condition::serialize_constant (c->serializer, خطأ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      إذا (unlikely (نتیجہ == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->نکالو_discard ();
        c->serializer->check_success (خطأ);
        return_trace (خطأ);
      }
      c->serializer->جوڑو_link (*وتھ, c->serializer->نکالو_pack ());
    }

    return_trace (منطقي (باہرلا->conditions));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودکار &وتھ : conditions)
      إذا (unlikely (!(هذا+وتھ).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_اقدار)))
	c->layout_variation_indices->err ();
  }

  محمي:
  Array16_دا_Offset32_منزل<Condition>	conditions;
  عام:
  DEFINE_ناپ_ARRAY (2, conditions);
};

بنية Feature_جدول_Substitution_ریکارڈ
{
  یار بنية Feature_جدول_Substitution;

  فراغ collect_lookups (ثابت فراغ *ادھار, hb_سیٹ_t *lookup_indexes /*باہرلا*/) ثابت
  {
    أرجع (ادھار+feature).جوڑو_lookup_indexes_منزل (lookup_indexes);
  }

  فراغ گھیرا_features (ثابت فراغ *ادھار,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t       *feature_indexes /*باہرلا*/) ثابت
  {
    إذا ((ادھار+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جوڑو (feature_اشاریہ_2);
  }

  فراغ collect_feature_substitutes_with_variations (hb_hashmap_t<بنانشانی, ثابت Feature*> *feature_substitutes_map,
                                                    hb_سیٹ_t& catch_all_ریکارڈ_feature_idxes,
                                                    ثابت hb_سیٹ_t *feature_indices,
                                                    ثابت فراغ *ادھار) ثابت
  {
    إذا (feature_indices->has (feature_اشاریہ_2))
    {
      feature_substitutes_map->سیٹ (feature_اشاریہ_2, &(ادھار+feature));
      catch_all_ریکارڈ_feature_idxes.جوڑو (feature_اشاریہ_2);
    }
  }

  منطقي serialize (hb_subset_layout_context_t *c,
                  بنانشانی feature_اشاریہ,
                  ثابت Feature *f, ثابت ٹیگ *ٹیگ_2)
  {
    TRACE_SERIALIZE (هذا);
    hb_serialize_context_t *s = c->subset_context->serializer;
    إذا (unlikely (!s->extend_min (هذا))) return_trace (خطأ);

    uint32_t *new_feature_idx;
    إذا (!c->feature_map_w_duplicates->has (feature_اشاریہ, &new_feature_idx))
      return_trace (خطأ);

    إذا (!s->check_سونپو (feature_اشاریہ_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خطأ);

    s->push ();
    منطقي ret = f->subset (c->subset_context, c, ٹیگ_2);
    إذا (ret) s->جوڑو_link (feature, s->نکالو_pack ());
    وإلا s->نکالو_discard ();

    return_trace (ret);
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_اشاریہ;
    إذا (!c->feature_map_w_duplicates->has (feature_اشاریہ_2, &new_feature_اشاریہ))
      return_trace (خطأ);

    خودکار *باہرلا = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->feature_اشاریہ_2 = *new_feature_اشاریہ;
    hb_pair_t<ثابت فراغ*, ثابت فراغ*> *original;
    إذا (!c->feature_idx_ٹیگ_map->has (feature_اشاریہ_2, &original))
      return_trace (خطأ);
    ثابت ٹیگ *ٹیگ_2 = reinterpret_cast<ثابت ٹیگ*> (original->second);
    return_trace (باہرلا->feature.serialize_subset (c->subset_context, feature, ادھار, c, ٹیگ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت ریکارڈ_sanitize_گھیرا_t گھیرا = {feature_list->get_ٹیگ (feature_اشاریہ_2), ادھار};
    return_trace (feature.sanitize (c, ادھار, &گھیرا));
  }

  محمي:
  HBUINT16		feature_اشاریہ_2;
  Offset32_منزل<Feature>	feature;
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية Feature_جدول_Substitution
{
  فراغ collect_feature_indices (hb_سیٹ_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارڈ &ریکارڈ : substitutions)
      feature_indices->جوڑو (ریکارڈ.feature_اشاریہ_2);
  }

  ثابت Feature *لبھو_substitute (بنانشانی صحيح feature_اشاریہ) ثابت
  {
    بنانشانی صحيح گنتی = substitutions.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      ثابت Feature_جدول_Substitution_ریکارڈ &ریکارڈ = substitutions.arrayZ[i];
      إذا (ریکارڈ.feature_اشاریہ_2 == feature_اشاریہ)
	أرجع &(هذا+ریکارڈ.feature);
    }
    أرجع خالی_سوچک;
  }

  فراغ collect_lookups (ثابت hb_سیٹ_t *feature_indexes,
			hb_سیٹ_t       *lookup_indexes /*باہرلا*/) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_جدول_Substitution_ریکارڈ::feature_اشاریہ_2)
    | hb_apply ([هذا, lookup_indexes] (ثابت Feature_جدول_Substitution_ریکارڈ& r)
		{ r.collect_lookups (هذا, lookup_indexes); })
    ;
  }

  سانچا <قسم_ناں ٹیگ_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_اشاریہ_map,
                         ٹیگ_Getter get_ٹیگ,
                         hb_سیٹ_t *nameids_منزل_retain) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارڈ &ریکارڈ : substitutions)
      إذا (feature_اشاریہ_map->has (ریکارڈ.feature_اشاریہ_2))
        (هذا+ریکارڈ.feature).collect_name_ids (get_ٹیگ (ریکارڈ.feature_اشاریہ_2), nameids_منزل_retain);
  }

  فراغ گھیرا_features (ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t       *feature_indexes /*باہرلا*/) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارڈ& ریکارڈ : substitutions)
      ریکارڈ.گھیرا_features (هذا, lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت hb_map_t *feature_اشاریہ_map) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارڈ& ریکارڈ : substitutions)
    {
      إذا (feature_اشاریہ_map->has (ریکارڈ.feature_اشاریہ_2)) أرجع سچ;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_ریکارڈ& ریکارڈ : substitutions)
      ریکارڈ.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_ریکارڈ_feature_idxes,
                                                          c->feature_indices, هذا);
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقي داخل_کرو_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->نسخہ.major = نسخہ.major;
    باہرلا->نسخہ.minor = نسخہ.minor;

    إذا (داخل_کرو_catch_all)
    {
      لكل (بنانشانی feature_اشاریہ : *(l->catch_all_ریکارڈ_feature_idxes))
      {
        hb_pair_t<ثابت فراغ*, ثابت فراغ*> *p;
        إذا (!l->feature_idx_ٹیگ_map->has (feature_اشاریہ, &p))
          return_trace (خطأ);
        خودکار *o = باہرلا->substitutions.serialize_append (c->serializer);
        إذا (!o->serialize (l, feature_اشاریہ,
                           reinterpret_cast<ثابت Feature*> (p->پہلا),
                           reinterpret_cast<ثابت ٹیگ*> (p->second)))
          return_trace (خطأ);
      }
      return_trace (سچ);
    }

    + substitutions.iter ()
    | hb_apply (subset_ریکارڈ_array (l, &(باہرلا->substitutions), هذا))
    ;

    return_trace (منطقي (باہرلا->substitutions));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  substitutions.sanitize (c, هذا, feature_list));
  }

  محمي:
  Fixed_نسخہ<>	نسخہ;	/* Version--0x00010000u */
  Array16_دا<Feature_جدول_Substitution_ریکارڈ>
			substitutions;
  عام:
  DEFINE_ناپ_ARRAY (6, substitutions);
};

بنية Feature_Variation_ریکارڈ
{
  یار بنية FeatureVariations;

  فراغ collect_lookups (ثابت فراغ     *ادھار,
			ثابت hb_سیٹ_t *feature_indexes,
			hb_سیٹ_t       *lookup_indexes /*باہرلا*/) ثابت
  {
    أرجع (ادھار+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  سانچا <قسم_ناں ٹیگ_Getter>
  فراغ collect_name_ids (ثابت فراغ *ادھار,
                         ثابت hb_map_t *feature_اشاریہ_map,
                         ٹیگ_Getter get_ٹیگ,
                         hb_سیٹ_t *nameids_منزل_retain) ثابت
  {
    (ادھار+substitutions).collect_name_ids (feature_اشاریہ_map, get_ٹیگ, nameids_منزل_retain);
  }

  فراغ گھیرا_features (ثابت فراغ     *ادھار,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t       *feature_indexes /*باہرلا*/) ثابت
  {
    (ادھار+substitutions).گھیرا_features (lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت فراغ *ادھار, ثابت hb_map_t *feature_اشاریہ_map) ثابت
  {
    أرجع (ادھار+substitutions).intersects_features (feature_اشاریہ_map);
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت فراغ *ادھار) ثابت
  {
    (ادھار+conditions).keep_with_variations (c);
    إذا (c->apply && !c->variation_applied)
    {
      (ادھار+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = سچ; // set variations only once
    }
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *ادھار,
               منطقي داخل_کرو_catch_all = خطأ) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    hb_سیٹ_t *retained_conditions = c->feature_ریکارڈ_cond_idx_map ?
        c->feature_ریکارڈ_cond_idx_map->get (c->cur_feature_var_ریکارڈ_idx).get () : خالی_سوچک;
    إذا (!داخل_کرو_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      باہرلا->conditions = 0;
    وإلا
      باہرلا->conditions.serialize_subset (c->subset_context, conditions, ادھار, c, داخل_کرو_catch_all);
    باہرلا->substitutions.serialize_subset (c->subset_context, substitutions, ادھار, c, داخل_کرو_catch_all);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, ادھار) &&
		  substitutions.sanitize (c, ادھار, feature_list));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *ادھار) ثابت
  { (ادھار+conditions).collect_variation_indices (c); }

  محمي:
  Offset32_منزل<Condition_سیٹ>
			conditions;
  Offset32_منزل<Feature_جدول_Substitution>
			substitutions;
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية Lookup_اشاریہ_List
{
  فراغ جوڑو_lookup_indexes_منزل (hb_سیٹ_t *lookup_indexes) ثابت
  { lookupIndices.جوڑو_indexes_منزل (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookupIndices.intersects (lookup_indexes); }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
      return_trace (خطأ);

    خودکار it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_اشاریہ_map)
    | hb_map (l->lookup_اشاریہ_map)
    ;
    باہرلا->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقي (باہرلا->lookupIndices));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (lookupIndices.sanitize (c));
  }

  محمي:
  اشاریہ_Array	lookupIndices;
  عام:
  DEFINE_ناپ_ARRAY_SIZED (2, lookupIndices);
};

بنية Lookup_Condition_ریکارڈ
{
  بنانشانی get_condition_وتھ (ثابت فراغ *ادھار, ثابت فراغ *variations) ثابت
  { أرجع (ثابت حرف *) &(ادھار+condition) - (ثابت حرف *) variations; }

  سانچا <قسم_ناں Instancer>
  منطقي evaluate (ثابت فراغ *ادھار,
		 ثابت صحيح *coords,
		 بنانشانی صحيح coord_len,
		 Instancer *instancer) ثابت
  { أرجع (ادھار+condition).evaluate (coords, coord_len, instancer); }

  منطقي evaluate (ثابت فراغ *ادھار,
                 ثابت فراغ *variations,
                 hb_sorted_array_t<ثابت بنانشانی> conditions) ثابت
  {
    أرجع !condition || hb_bsearch (get_condition_وتھ (ادھار, variations),
                                     conditions.arrayZ, conditions.لمائی,
                                     حجم (بنانشانی),
                                     _hb_cmp_operator<بنانشانی, بنانشانی>);
  }

  منطقي collect_condition (ثابت فراغ *ادھار,
                          ثابت فراغ *variations,
                          ثابت صحيح *coords,
                          بنانشانی صحيح coord_len,
                          شے_Var_Store_Instancer *instancer,
                          hb_سیٹ_t *ویکھیا,
                          hb_سمتیہ_t<بنانشانی> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    إذا (!condition) أرجع سچ;

    بنانشانی وتھ = get_condition_وتھ (ادھار, variations);
    إذا (ویکھیا->has (وتھ)) أرجع سچ;
    ویکھیا->جوڑو (وتھ);
    إذا (unlikely (ویکھیا->وچ_error ())) أرجع خطأ;

    إذا (evaluate (ادھار, coords, coord_len, instancer))
      conditions->push (وتھ);
    أرجع !conditions->وچ_error ();
  }

  فراغ جوڑو_lookup_indexes_منزل (ثابت فراغ *ادھار,
			      hb_سیٹ_t *lookup_indexes) ثابت
  { (ادھار+lookupIndices).جوڑو_lookup_indexes_منزل (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت فراغ *ادھار,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع (ادھار+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *ادھار) ثابت
  {
    إذا (unlikely (!(ادھار+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_اقدار)))
      c->layout_variation_indices->err ();
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت فراغ *ادھار) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    باہرلا->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_نتیجہ_t نتیجہ =
        (ادھار + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    سچ);
    إذا (نتیجہ != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->نکالو_discard ();
      return_trace (خطأ);
    }
    c->serializer->جوڑو_link (باہرلا->condition, c->serializer->نکالو_pack ());

    return_trace (باہرلا->lookupIndices.serialize_subset (c, lookupIndices, ادھار, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (condition.sanitize (c, ادھار) &&
		  lookupIndices.sanitize (c, ادھار));
  }

  محمي:
  Offset32_منزل<Condition>		condition;
  Offset32_منزل<Lookup_اشاریہ_List>	lookupIndices;
  عام:
  DEFINE_ناپ_STATIC (8);
};

بنية Feature_Lookups_جدول
{
  منطقي adds_default_lookups () ثابت { أرجع flags & 0x0001u; }

  منطقي collect_conditions (ثابت فراغ *variations,
                           ثابت صحيح *coords,
                           بنانشانی صحيح coord_len,
                           شے_Var_Store_Instancer *instancer,
                           hb_سیٹ_t *ویکھیا,
                           hb_سمتیہ_t<بنانشانی> *conditions) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
      إذا (unlikely (!ریکارڈ.collect_condition (هذا, variations, coords, coord_len,
                                               instancer, ویکھیا, conditions)))
	أرجع خطأ;
    أرجع سچ;
  }

  فراغ collect_lookups (ثابت فراغ *variations,
                       hb_sorted_array_t<ثابت بنانشانی> conditions,
                       hb_سیٹ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
      إذا (ریکارڈ.evaluate (هذا, variations, conditions))
	ریکارڈ.جوڑو_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  سانچا <قسم_ناں Instancer>
  فراغ collect_lookups (ثابت صحيح *coords,
			بنانشانی صحيح coord_len,
			Instancer *instancer,
			hb_سیٹ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
      إذا (ریکارڈ.evaluate (هذا, coords, coord_len, instancer))
	ریکارڈ.جوڑو_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  فراغ collect_lookups (hb_سیٹ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
      ریکارڈ.جوڑو_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
      إذا (ریکارڈ.intersects_lookup_indexes (هذا, lookup_indexes))
	أرجع سچ;
    أرجع خطأ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
      ریکارڈ.collect_variation_indices (c, هذا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا)))
      return_trace (خطأ);

    باہرلا->نسخہ.major = نسخہ.major;
    باہرلا->نسخہ.minor = نسخہ.minor;
    باہرلا->flags = flags;

    لكل (ثابت Lookup_Condition_ریکارڈ &ریکارڈ : ریکارڈاں)
    {
      خودکار snap = c->serializer->snapshot ();
      إذا (ریکارڈ.subset (c, l, هذا))
	باہرلا->ریکارڈاں.len++;
      وإلا
	c->serializer->revert (snap);
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  ریکارڈاں.sanitize (c, هذا));
  }

  محمي:
  Fixed_نسخہ<>	نسخہ;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_دا<Lookup_Condition_ریکارڈ>
		ریکارڈاں;
  عام:
  DEFINE_ناپ_ARRAY (10, ریکارڈاں);
};

بنية Lookup_Variation_ریکارڈ
{
  یار بنية FeatureVariations;

  بنانشانی get_feature_اشاریہ () ثابت { أرجع feature_اشاریہ_2; }

  فراغ collect_lookups (ثابت فراغ *ادھار,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت بنانشانی> conditions,
                       hb_سیٹ_t *lookup_indexes) ثابت
  {
    ثابت Feature_Lookups_جدول &feature_lookups = ادھار+featureLookups;
    إذا (feature_lookups.adds_default_lookups ())
      feature.جوڑو_lookup_indexes_منزل (lookup_indexes);
    feature_lookups.collect_lookups (ادھار, conditions, lookup_indexes);
  }

  فراغ collect_lookups (ثابت فراغ *ادھار,
			ثابت hb_سیٹ_t *feature_indexes,
			hb_سیٹ_t *lookup_indexes) ثابت
  {
    إذا (feature_indexes->has (feature_اشاریہ_2))
      (ادھار+featureLookups).collect_lookups (lookup_indexes);
  }

  فراغ گھیرا_features (ثابت فراغ *ادھار,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سیٹ_t *feature_indexes) ثابت
  {
    إذا ((ادھار+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جوڑو (feature_اشاریہ_2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *ادھار) ثابت
  { (ادھار+featureLookups).collect_variation_indices (c); }

  منطقي subset (hb_subset_layout_context_t *l, ثابت فراغ *ادھار) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_اشاریہ;
    إذا (!l->feature_map_w_duplicates->has (feature_اشاریہ_2, &new_feature_اشاریہ))
      return_trace (خطأ);

    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا ||
		  !c->serializer->check_سونپو (باہرلا->feature_اشاریہ_2,
						*new_feature_اشاریہ,
						HB_SERIALIZE_ERROR_INT_حد_توں_ودھ)))
      return_trace (خطأ);

    return_trace (باہرلا->featureLookups.serialize_subset (c, featureLookups, ادھار, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureLookups.sanitize (c, ادھار));
  }

  محمي:
  HBUINT16			feature_اشاریہ_2;
  Offset32_منزل<Feature_Lookups_جدول>	featureLookups;
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية FeatureVariations
{
  ساكن constexpr بنانشانی NOT_لبھیا_اشاریہ = 0xFFFFFFFFu;

  ثابت Array32_دا<Lookup_Variation_ریکارڈ> &get_lookup_variation_ریکارڈاں () ثابت
  {
    إذا (نسخہ.منزل_int () < 0x00010001u)
      أرجع Null (Array32_دا<Lookup_Variation_ریکارڈ>);
    أرجع StructAfter<Array32_دا<Lookup_Variation_ریکارڈ>> (var_ریکارڈاں);
  }

  بنانشانی ریکارڈ_گنتی () ثابت
  {
    أرجع var_ریکارڈاں.len;
  }

  منطقي لبھو_اشاریہ (ثابت صحيح *coords, بنانشانی صحيح coord_len,
		   بنانشانی صحيح *اشاریہ_2,
		   شے_Var_Store_Instancer *instancer) ثابت
  {
    بنانشانی صحيح گنتی = var_ریکارڈاں.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      ثابت Feature_Variation_ریکارڈ &ریکارڈ = var_ریکارڈاں.arrayZ[i];
      إذا ((هذا+ریکارڈ.conditions).evaluate (coords, coord_len, instancer))
      {
	*اشاریہ_2 = i;
	أرجع سچ;
      }
    }
    *اشاریہ_2 = NOT_لبھیا_اشاریہ;
    أرجع خطأ;
  }

  ثابت Feature *لبھو_substitute (بنانشانی صحيح variations_اشاریہ,
				  بنانشانی صحيح feature_اشاریہ) ثابت
  {
    ثابت Feature_Variation_ریکارڈ &ریکارڈ = var_ریکارڈاں[variations_اشاریہ];
    أرجع (هذا+ریکارڈ.substitutions).لبھو_substitute (feature_اشاریہ);
  }

  منطقي collect_lookup_conditions (ثابت صحيح *coords,
                                  بنانشانی صحيح coord_len,
                                  شے_Var_Store_Instancer *instancer,
                                  hb_سمتیہ_t<بنانشانی> *state) ثابت
  {
    hb_سیٹ_t ویکھیا_tables, ویکھیا_conditions;
    لكل (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_ریکارڈاں ())
    {
      بنانشانی وتھ = ریکارڈ.featureLookups;
      إذا (ویکھیا_tables.has (وتھ)) استمر;
      ویکھیا_tables.جوڑو (وتھ);
      إذا (unlikely (ویکھیا_tables.وچ_error ())) أرجع خطأ;

      ثابت Feature_Lookups_جدول &feature_lookups = هذا+ریکارڈ.featureLookups;
      إذا (unlikely (!feature_lookups.collect_conditions (هذا, coords, coord_len,
                                                         instancer, &ویکھیا_conditions,
                                                         state)))
	أرجع خطأ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<بنانشانی, بنانشانی>);
    أرجع سچ;
  }

  منطقي get_lookup_variations (بنانشانی feature_اشاریہ,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت بنانشانی> conditions,
                              hb_سیٹ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_ریکارڈاں ())
    {
      إذا (ریکارڈ.feature_اشاریہ_2 != feature_اشاریہ) استمر;

      ریکارڈ.collect_lookups (هذا, feature, conditions, lookup_indexes);
      أرجع سچ;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    بنانشانی صحيح گنتی = var_ریکارڈاں.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      c->cur_ریکارڈ_idx = i;
      var_ریکارڈاں[i].collect_feature_substitutes_with_variations (c, هذا);
      إذا (c->universal)
        اكسر;
    }
    إذا (c->universal || c->ریکارڈ_cond_idx_map->is_empty ())
      c->catch_all_ریکارڈ_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  فراغ collect_lookups (ثابت hb_سیٹ_t *feature_indexes,
			ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
			hb_سیٹ_t       *lookup_indexes /*باہرلا*/) ثابت
  {
    بنانشانی گنتی = var_ریکارڈاں.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (feature_ریکارڈ_cond_idx_map &&
          !feature_ریکارڈ_cond_idx_map->has (i))
        استمر;
      var_ریکارڈاں[i].collect_lookups (هذا, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  سانچا <قسم_ناں ٹیگ_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_اشاریہ_map,
                         ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
                         ٹیگ_Getter get_ٹیگ,
                         hb_سیٹ_t *nameids_منزل_retain) ثابت
  {
    لكل (بنانشانی i = 0; i < var_ریکارڈاں.len; i++)
    {
      إذا (feature_ریکارڈ_cond_idx_map && !feature_ریکارڈ_cond_idx_map->has (i))
        استمر;
      var_ریکارڈاں[i].collect_name_ids (هذا, feature_اشاریہ_map, get_ٹیگ, nameids_منزل_retain);
    }
  }

  فراغ collect_lookup_variation_lookups (ثابت hb_سیٹ_t *feature_indexes,
					 hb_سیٹ_t *lookup_indexes /*باہرلا*/) ثابت
  {
    لكل (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_ریکارڈاں ())
      ریکارڈ.collect_lookups (هذا, feature_indexes, lookup_indexes);
  }

  فراغ گھیرا_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<بنانشانی, hb::shared_ptr<hb_سیٹ_t>> *feature_ریکارڈ_cond_idx_map,
			 hb_سیٹ_t       *feature_indexes /*باہرلا*/) ثابت
  {
    بنانشانی صحيح گنتی = var_ریکارڈاں.len;
    لكل (بنانشانی صحيح i = 0; i < گنتی; i++)
    {
      إذا (feature_ریکارڈ_cond_idx_map != خالی_سوچک &&
          !feature_ریکارڈ_cond_idx_map->has (i))
        استمر;
      var_ریکارڈاں[i].گھیرا_features (هذا, lookup_indexes, feature_indexes);
    }

    لكل (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_ریکارڈاں ())
      ریکارڈ.گھیرا_features (هذا, lookup_indexes, feature_indexes);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Feature_Variation_ریکارڈ &ریکارڈ : var_ریکارڈاں)
      ریکارڈ.collect_variation_indices (c, هذا);
    لكل (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_ریکارڈاں ())
      ریکارڈ.collect_variation_indices (c, هذا);
  }

  فراغ collect_feature_indices (hb_سیٹ_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_Variation_ریکارڈ &ریکارڈ : var_ریکارڈاں)
      (هذا+ریکارڈ.substitutions).collect_feature_indices (feature_indices);
  }

  منطقي has_lookup_variations (بنانشانی feature_اشاریہ) ثابت
  {
    لكل (ثابت Lookup_Variation_ریکارڈ &ریکارڈ : get_lookup_variation_ریکارڈاں ())
    {
      إذا (ریکارڈ.feature_اشاریہ_2 == feature_اشاریہ)
	أرجع سچ;
      إذا (ریکارڈ.feature_اشاریہ_2 > feature_اشاریہ)
	اكسر;
    }
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!باہرلا || !c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    باہرلا->نسخہ.major = نسخہ.major;
    باہرلا->نسخہ.minor = نسخہ.minor;

    صحيح keep_اتے_منزل = -1;
    لكل (صحيح i = var_ریکارڈاں.len - 1; i >= 0; i--) {
      إذا (var_ریکارڈاں[i].intersects_features (هذا, l->feature_map_w_duplicates)) {
        keep_اتے_منزل = i;
        اكسر;
      }
    }

    بنانشانی گنتی = (بنانشانی) (keep_اتے_منزل + 1);
    لكل (بنانشانی i = 0; i < گنتی; i++)
    {
      إذا (l->feature_ریکارڈ_cond_idx_map != خالی_سوچک &&
          !l->feature_ریکارڈ_cond_idx_map->has (i))
        استمر;

      l->cur_feature_var_ریکارڈ_idx = i;
      subset_ریکارڈ_array (l, &(باہرلا->var_ریکارڈاں), هذا) (var_ریکارڈاں[i]);
    }

    إذا (باہرلا->var_ریکارڈاں.len && !l->catch_all_ریکارڈ_feature_idxes->is_empty ())
    {
      منطقي داخل_کرو_catch_all_ریکارڈ = سچ;
      subset_ریکارڈ_array (l, &(باہرلا->var_ریکارڈاں), هذا, داخل_کرو_catch_all_ریکارڈ) (var_ریکارڈاں[0]);
    }

    Array32_دا<Lookup_Variation_ریکارڈ> *باہرلا_lookup_ریکارڈاں = خالی_سوچک;
    إذا (نسخہ.minor >= 1)
    {
      باہرلا_lookup_ریکارڈاں = &StructAfter<Array32_دا<Lookup_Variation_ریکارڈ>> (باہرلا->var_ریکارڈاں);
      إذا (unlikely (!c->serializer->extend_min (باہرلا_lookup_ریکارڈاں)))
	return_trace (خطأ);

      + get_lookup_variation_ریکارڈاں ().iter ()
      | hb_apply (subset_ریکارڈ_array (l, باہرلا_lookup_ریکارڈاں, هذا))
      ;
    }

    return_trace (منطقي (باہرلا->var_ریکارڈاں) ||
		  (باہرلا_lookup_ریکارڈاں && منطقي (*باہرلا_lookup_ریکارڈاں)));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخہ.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخہ.major == 1) &&
		  var_ریکارڈاں.sanitize (c, هذا, feature_list) &&
		  (نسخہ.minor < 1 ||
		   get_lookup_variation_ریکارڈاں ().sanitize (c, هذا)));
  }

  محمي:
  Fixed_نسخہ<>	نسخہ;	/* Version--0x00010000u */
  Array32_دا<Feature_Variation_ریکارڈ>
			var_ریکارڈاں;
  عام:
  DEFINE_ناپ_ARRAY_SIZED (8, var_ریکارڈاں);
};


/*
 * Device Tables
 */

بنية HintingDevice
{
  یار بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->x_ppem, font->x_scale); }

  hb_position_t get_y_delta (hb_font_t *font) ثابت
  { أرجع get_delta (font->y_ppem, font->y_scale); }

  عام:

  size_t get_ناپ () ثابت
  {
    بنانشانی صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3 || شروع_ناپ > سرا_ناپ)) أرجع 3 * HBUINT16::static_ناپ;
    أرجع HBUINT16::static_ناپ * (4 + ((سرا_ناپ - شروع_ناپ) >> (4 - f)));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && c->check_range (هذا, هذا->get_ناپ ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed<HintingDevice> (هذا));
  }

  خاص:

  صحيح get_delta (بنانشانی صحيح ppem, صحيح scale) ثابت
  {
    إذا (!ppem) أرجع 0;

    صحيح pixels = get_delta_pixels (ppem);

    إذا (!pixels) أرجع 0;

    أرجع (صحيح) (pixels * (int64_t) scale / ppem);
  }
  صحيح get_delta_pixels (بنانشانی صحيح ppem_ناپ) ثابت
  {
    بنانشانی صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3))
      أرجع 0;

    إذا (ppem_ناپ < شروع_ناپ || ppem_ناپ > سرا_ناپ)
      أرجع 0;

    بنانشانی صحيح s = ppem_ناپ - شروع_ناپ;

    بنانشانی صحيح byte = delta_قدر_Z[s >> (4 - f)];
    بنانشانی صحيح بٹ = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    بنانشانی صحيح نقاب = (0xFFFFu >> (16 - (1 << f)));

    صحيح delta = بٹ & نقاب;

    إذا ((بنانشانی صحيح) delta >= ((نقاب + 1) >> 1))
      delta -= نقاب + 1;

    أرجع delta;
  }

  محمي:
  HBUINT16	شروع_ناپ;		/* Smallest size to correct--in ppem */
  HBUINT16	سرا_ناپ;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_دا<HBUINT16>
		delta_قدر_Z;		/* Array of compressed data */
  عام:
  DEFINE_ناپ_ARRAY (6, delta_قدر_Z);
};

بنية VariationDevice
{
  یار بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  عائم get_x_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!layout_variation_idx_delta_map) return_trace (خالی_سوچک);

    hb_pair_t<بنانشانی, صحيح> *v;
    إذا (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (خالی_سوچک);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    إذا (hb_پہلا (*v) == HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
      return_trace (خالی_سوچک);

    c->شروع_zerocopy (هذا->static_ناپ);
    خودکار *باہرلا = c->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خالی_سوچک);

    إذا (!c->check_سونپو (باہرلا->varIdx, hb_پہلا (*v), HB_SERIALIZE_ERROR_INT_حد_توں_ودھ))
      return_trace (خالی_سوچک);
    return_trace (باہرلا);
  }

  فراغ collect_variation_اشاریہ (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->جوڑو (varIdx); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:

  عائم get_delta (hb_font_t *font,
		   ثابت شے_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    أرجع store.get_delta (varIdx, font->coords, font->عدد_coords, store_cache);
  }

  محمي:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية Device_سرخی
{
  محمي:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  عام:
  HBUINT16		format;		/* Format identifier */
  عام:
  DEFINE_ناپ_STATIC (6);
};

بنية Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  عائم get_x_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_x_delta_f (font, store, store_cache);
    }
#جے_انت
    أرجع get_x_delta (font, store, store_cache);
  }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت شے_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
#جے_بنا_تعریف HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_y_delta_f (font, store, store_cache);
    }
#جے_انت
    أرجع get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store=Null (شے_Variation_Store),
			     hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    بدل (u.b.format)
    {
#جے_بنا_تعریف HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      أرجع u.hinting.get_x_delta (font);
#جے_انت
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_x_delta (font, store, store_cache);
#جے_انت
    افتراضي:
      أرجع 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت شے_Variation_Store &store=Null (شے_Variation_Store),
			     hb_scalar_cache_t *store_cache = خالی_سوچک) ثابت
  {
    بدل (u.b.format)
    {
    حالة 1: حالة 2: حالة 3:
#جے_بنا_تعریف HB_NO_HINTING
      hb_barrier ();
      أرجع u.hinting.get_y_delta (font);
#جے_انت
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_y_delta (font, store, store_cache);
#جے_انت
    افتراضي:
      أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.b.format.sanitize (c)) return_trace (خطأ);
    بدل (u.b.format) {
#جے_بنا_تعریف HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#جے_انت
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#جے_انت
    افتراضي:
      return_trace (سچ);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map=خالی_سوچک) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.b.format) {
#جے_بنا_تعریف HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#جے_انت
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#جے_انت
    افتراضي:
      return_trace (خالی_سوچک);
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.b.format) {
#جے_بنا_تعریف HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      أرجع;
#جے_انت
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      u.variation.collect_variation_اشاریہ (c);
      أرجع;
#جے_انت
    افتراضي:
      أرجع;
    }
  }

  بنانشانی get_variation_اشاریہ () ثابت
  {
    بدل (u.b.format) {
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.varIdx;
#جے_انت
    افتراضي:
      أرجع HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ;
    }
  }

  منطقي is_variation_device () ثابت
  {
    بدل (u.b.format) {
#جے_بنا_تعریف HB_NO_VAR
    حالة 0x8000:
      أرجع سچ;
#جے_انت
    افتراضي:
      أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  Device_سرخی		b;
  HintingDevice		hinting;
#جے_بنا_تعریف HB_NO_VAR
  VariationDevice	variation;
#جے_انت
  } u;
  عام:
  DEFINE_ناپ_UNION (6, b);
};


} /* namespace OT */


#جے_انت /* HB_OT_LAYOUT_COMMON_HH */

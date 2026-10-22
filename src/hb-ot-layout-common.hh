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

#جيڪڏهن_وصف_نه_ٿيل HB_OT_LAYOUT_COMMON_HH
#وصف_ڪريو HB_OT_LAYOUT_COMMON_HH

#ضمن "hb.hh"
#ضمن "hb-ot-layout.hh"
#ضمن "hb-open-type.hh"
#ضمن "hb-set.hh"
#ضمن "hb-bimap.hh"
#ضمن "hb-cache.hh"
#ضمن "hb-ot-dual.hh"

استعمال hb_ot_layout_mapping_cache_t = hb_cache_t<16, 8, 8>;
static_assert (حجم (hb_ot_layout_mapping_cache_t) == 512, "");

استعمال hb_ot_layout_ٻٽو_cache_t = hb_cache_t<14, 1, 8>;
static_assert (حجم (hb_ot_layout_ٻٽو_cache_t) == 256, "");

#ضمن "OT/Layout/Common/Coverage.hh"

// TODO(garretrieger): cleanup these after migration.
استعمال OT::Layout::Common::Coverage;
استعمال OT::Layout::Common::Range_رڪارڊ;
استعمال OT::Layout::SmallTypes;
استعمال OT::Layout::MediumTypes;


نطاق OT {

سانچو<قسم_نالو Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it);

ساكن منطقي ClassDef_remap_and_serialize (
    hb_serialize_context_t *c,
    ثابت hb_سيٽ_t &klasses,
    منطقي use_class_zero,
    hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
    hb_map_t *klass_map /*IN/OUT*/);

بنية شي_Var_Store_Instancer;

بنية hb_collect_feature_substitutes_with_var_context_t
{
  ثابت hb_map_t *axes_اشاريو_ٽيگ_map;
  ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> *axes_location;
  hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *رڪارڊ_cond_idx_map;
  hb_hashmap_t<بنانشان, ثابت Feature*> *feature_substitutes_map;
  hb_سيٽ_t& catch_all_رڪارڊ_feature_idxes;

  // not stored in subset_plan
  hb_سيٽ_t *feature_indices;
  منطقي apply;
  منطقي variation_applied;
  منطقي universal;
  بنانشان cur_رڪارڊ_idx;
  hb_hashmap_t<hb::shared_ptr<hb_map_t>, بنانشان> *conditionset_map;
  شي_Var_Store_Instancer *instancer;
  منطقي all_axes_pinned;
};

بنية hb_prune_langsys_context_t
{
  hb_prune_langsys_context_t (ثابت فراغ         *جدول_2,
                              hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> *script_langsys_map_,
                              ثابت hb_map_t     *duplicate_feature_map_,
                              hb_سيٽ_t           *new_collected_feature_indexes_)
      :جدول (جدول_2),
      script_langsys_map (script_langsys_map_),
      duplicate_feature_map (duplicate_feature_map_),
      new_feature_indexes (new_collected_feature_indexes_),
      script_ڳڻپ (0),langsys_feature_ڳڻپ (0) {}

  منطقي گهمو_Script ()
  { أرجع script_ڳڻپ++ < HB_MAX_SCRIPTS; }

  منطقي گهمو_Langsys (بنانشان feature_ڳڻپ_2)
  {
    langsys_feature_ڳڻپ += feature_ڳڻپ_2;
    أرجع langsys_feature_ڳڻپ < HB_MAX_LANGSYS_FEATURE_ڳڻپ;
  }

  عام:
  ثابت فراغ *جدول;
  hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> *script_langsys_map;
  ثابت hb_map_t     *duplicate_feature_map;
  hb_سيٽ_t           *new_feature_indexes;

  خاص:
  بنانشان script_ڳڻپ;
  بنانشان langsys_feature_ڳڻپ;
};

بنية hb_subset_layout_context_t :
  hb_dispatch_context_t<hb_subset_layout_context_t, hb_خالي_جايون_t, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { أرجع "SUBSET_LAYOUT"; }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  منطقي گهمو_Script ()
  {
    أرجع script_ڳڻپ++ < HB_MAX_SCRIPTS;
  }

  منطقي گهمو_Lang_Sys ()
  {
    أرجع langsys_ڳڻپ++ < HB_MAX_LANGSYS;
  }

  منطقي گهمو_Feature_اشاريو (صحيح ڳڻپ)
  {
    feature_اشاريو_ڳڻپ += ڳڻپ;
    أرجع feature_اشاريو_ڳڻپ < HB_MAX_FEATURE_INDICES;
  }

  منطقي گهمو_Lookup_اشاريو()
  {
    lookup_اشاريو_ڳڻپ++;
    أرجع lookup_اشاريو_ڳڻپ < HB_MAX_LOOKUP_گهمو_ڳڻپ;
  }

  hb_subset_context_t *subset_context;
  ثابت hb_ٽيگ_t جدول_ٽيگ_2;
  ثابت hb_map_t *lookup_اشاريو_map;
  ثابت hb_hashmap_t<بنانشان, hb::unique_ptr<hb_سيٽ_t>> *script_langsys_map;
  ثابت hb_map_t *feature_اشاريو_map;
  ثابت hb_map_t *feature_map_w_duplicates;
  ثابت hb_hashmap_t<بنانشان, ثابت Feature*> *feature_substitutes_map;
  ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *lookup_variations;
  hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map;
  ثابت hb_سيٽ_t *catch_all_رڪارڊ_feature_idxes;
  ثابت hb_hashmap_t<بنانشان, hb_pair_t<ثابت فراغ*, ثابت فراغ*>> *feature_idx_ٽيگ_map;

  بنانشان cur_script_اشاريو;
  بنانشان cur_feature_اشاريو;
  بنانشان cur_feature_var_رڪارڊ_idx;

  hb_subset_layout_context_t (hb_subset_context_t *c_,
			      hb_ٽيگ_t ٽيگ_4) :
				subset_context (c_),
				جدول_ٽيگ_2 (ٽيگ_4),
				cur_script_اشاريو (0xFFFFu),
				cur_feature_اشاريو (اشاريو::NOT_مليو_اشاريو),
				cur_feature_var_رڪارڊ_idx (0u),
				script_ڳڻپ (0),
				langsys_ڳڻپ (0),
				feature_اشاريو_ڳڻپ (0),
				lookup_اشاريو_ڳڻپ (0)
  {
    إذا (ٽيگ_4 == HB_OT_ٽيگ_GSUB)
    {
      lookup_اشاريو_map = &c_->plan->gsub_lookups;
      script_langsys_map = &c_->plan->gsub_langsys;
      feature_اشاريو_map = &c_->plan->gsub_features;
      feature_map_w_duplicates = &c_->plan->gsub_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gsub_feature_substitutes_map;
      lookup_variations = &c_->plan->gsub_lookup_variations;
      feature_رڪارڊ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? خالي_اشارو : &c_->plan->gsub_feature_رڪارڊ_cond_idx_map;
      catch_all_رڪارڊ_feature_idxes = &c_->plan->gsub_old_features;
      feature_idx_ٽيگ_map = &c_->plan->gsub_old_feature_idx_ٽيگ_map;
    }
    وإلا
    {
      lookup_اشاريو_map = &c_->plan->gpos_lookups;
      script_langsys_map = &c_->plan->gpos_langsys;
      feature_اشاريو_map = &c_->plan->gpos_features;
      feature_map_w_duplicates = &c_->plan->gpos_features_w_duplicates;
      feature_substitutes_map = &c_->plan->gpos_feature_substitutes_map;
      lookup_variations = &c_->plan->gpos_lookup_variations;
      feature_رڪارڊ_cond_idx_map = c_->plan->user_axes_location.is_empty () ? خالي_اشارو : &c_->plan->gpos_feature_رڪارڊ_cond_idx_map;
      catch_all_رڪارڊ_feature_idxes = &c_->plan->gpos_old_features;
      feature_idx_ٽيگ_map = &c_->plan->gpos_old_feature_idx_ٽيگ_map;
    }
  }

  خاص:
  بنانشان script_ڳڻپ;
  بنانشان langsys_ڳڻپ;
  بنانشان feature_اشاريو_ڳڻپ;
  بنانشان lookup_اشاريو_ڳڻپ;
};

بنية شي_Variation_Store;
بنية hb_collect_variation_indices_context_t :
       hb_dispatch_context_t<hb_collect_variation_indices_context_t>
{
  سانچو <قسم_نالو T>
  return_t dispatch (ثابت T &obj) { obj.collect_variation_indices (هذا); أرجع hb_خالي_جايون_t (); }
  ساكن return_t default_return_قدر () { أرجع hb_خالي_جايون_t (); }

  hb_سيٽ_t *layout_variation_indices;
  ثابت hb_سيٽ_t *glyph_سيٽ;
  ثابت hb_map_t *gpos_lookups;
  hb_hashmap_t<uintptr_t, hb_pair_t<بنانشان, صحيح>> *condition_قدرون = خالي_اشارو;

  hb_collect_variation_indices_context_t (hb_سيٽ_t *layout_variation_indices_,
					  ثابت hb_سيٽ_t *glyph_سيٽ_2,
					  ثابت hb_map_t *gpos_lookups_) :
					layout_variation_indices (layout_variation_indices_),
					glyph_سيٽ (glyph_سيٽ_2),
					gpos_lookups (gpos_lookups_) {}
};

سانچو<قسم_نالو OutputArray>
بنية subset_وٿي_array_t
{
  subset_وٿي_array_t (hb_subset_context_t *subset_context_,
			 OutputArray& ٻاهريون_4,
			 ثابت فراغ *بنياد_2) : subset_context (subset_context_),
					      ٻاهريون (ٻاهريون_4), بنياد (بنياد_2) {}

  سانچو <قسم_نالو T>
  منطقي عامل () (T&& وٿي)
  {
    خودڪار snap = subset_context->serializer->snapshot ();
    خودڪار *o = ٻاهريون.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, وٿي, بنياد);
    إذا (!ret)
    {
      ٻاهريون.ڪڍو ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &ٻاهريون;
  ثابت فراغ *بنياد;
};


سانچو<قسم_نالو OutputArray, قسم_نالو Arg>
بنية subset_وٿي_array_arg_t
{
  subset_وٿي_array_arg_t (hb_subset_context_t *subset_context_,
			     OutputArray& ٻاهريون_4,
			     ثابت فراغ *بنياد_2,
			     Arg &&arg_) : subset_context (subset_context_), ٻاهريون (ٻاهريون_4),
					  بنياد (بنياد_2), arg (arg_) {}

  سانچو <قسم_نالو T>
  منطقي عامل () (T&& وٿي)
  {
    خودڪار snap = subset_context->serializer->snapshot ();
    خودڪار *o = ٻاهريون.serialize_append (subset_context->serializer);
    إذا (unlikely (!o)) أرجع خطأ;
    منطقي ret = o->serialize_subset (subset_context, وٿي, بنياد, arg);
    إذا (!ret)
    {
      ٻاهريون.ڪڍو ();
      subset_context->serializer->revert (snap);
    }
    أرجع ret;
  }

  خاص:
  hb_subset_context_t *subset_context;
  OutputArray &ٻاهريون;
  ثابت فراغ *بنياد;
  Arg &&arg;
};

/*
 * Helper to subset an array of offsets. Subsets the thing pointed to by each offset
 * and discards the offset in the array if the subset operation results in an empty
 * thing.
 */
بنية
{
  سانچو<قسم_نالو OutputArray>
  subset_وٿي_array_t<OutputArray>
  عامل () (hb_subset_context_t *subset_context, OutputArray& ٻاهريون,
	       ثابت فراغ *بنياد) ثابت
  { أرجع subset_وٿي_array_t<OutputArray> (subset_context, ٻاهريون, بنياد); }

  /* Variant with one extra argument passed to serialize_subset */
  سانچو<قسم_نالو OutputArray, قسم_نالو Arg>
  subset_وٿي_array_arg_t<OutputArray, Arg>
  عامل () (hb_subset_context_t *subset_context, OutputArray& ٻاهريون,
	       ثابت فراغ *بنياد, Arg &&arg) ثابت
  { أرجع subset_وٿي_array_arg_t<OutputArray, Arg> (subset_context, ٻاهريون, بنياد, arg); }
}
HB_FUNCOBJ (subset_وٿي_array);

سانچو<قسم_نالو OutputArray>
بنية subset_رڪارڊ_array_t
{
  subset_رڪارڊ_array_t (hb_subset_layout_context_t *c_, OutputArray* ٻاهريون_4,
			 ثابت فراغ *بنياد_2) : subset_layout_context (c_),
					      ٻاهريون (ٻاهريون_4), بنياد (بنياد_2) {}

  سانچو <قسم_نالو T>
  فراغ
  عامل () (T&& رڪارڊ)
  {
    خودڪار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = رڪارڊ.subset (subset_layout_context, بنياد);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا ٻاهريون->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *ٻاهريون;
  ثابت فراغ *بنياد;
};

سانچو<قسم_نالو OutputArray, قسم_نالو Arg>
بنية subset_رڪارڊ_array_arg_t
{
  subset_رڪارڊ_array_arg_t (hb_subset_layout_context_t *c_, OutputArray* ٻاهريون_4,
			     ثابت فراغ *بنياد_2,
			     Arg &&arg_) : subset_layout_context (c_),
					   ٻاهريون (ٻاهريون_4), بنياد (بنياد_2), arg (arg_) {}

  سانچو <قسم_نالو T>
  فراغ
  عامل () (T&& رڪارڊ)
  {
    خودڪار snap = subset_layout_context->subset_context->serializer->snapshot ();
    منطقي ret = رڪارڊ.subset (subset_layout_context, بنياد, arg);
    إذا (!ret) subset_layout_context->subset_context->serializer->revert (snap);
    وإلا ٻاهريون->len++;
  }

  خاص:
  hb_subset_layout_context_t *subset_layout_context;
  OutputArray *ٻاهريون;
  ثابت فراغ *بنياد;
  Arg &&arg;
};

/*
 * Helper to subset a RecordList/record array. Subsets each Record in the array and
 * discards the record if the subset operation returns false.
 */
بنية
{
  سانچو<قسم_نالو OutputArray>
  subset_رڪارڊ_array_t<OutputArray>
  عامل () (hb_subset_layout_context_t *c, OutputArray* ٻاهريون,
	       ثابت فراغ *بنياد) ثابت
  { أرجع subset_رڪارڊ_array_t<OutputArray> (c, ٻاهريون, بنياد); }

  /* Variant with one extra argument passed to subset */
  سانچو<قسم_نالو OutputArray, قسم_نالو Arg>
  subset_رڪارڊ_array_arg_t<OutputArray, Arg>
  عامل () (hb_subset_layout_context_t *c, OutputArray* ٻاهريون,
               ثابت فراغ *بنياد, Arg &&arg) ثابت
  { أرجع subset_رڪارڊ_array_arg_t<OutputArray, Arg> (c, ٻاهريون, بنياد, arg); }
}
HB_FUNCOBJ (subset_رڪارڊ_array);


سانچو<قسم_نالو OutputArray>
بنية serialize_math_رڪارڊ_array_t
{
  serialize_math_رڪارڊ_array_t (hb_serialize_context_t *serialize_context_,
                         OutputArray& ٻاهريون_4,
                         ثابت فراغ *بنياد_2,
                         ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map_) :
                                              serialize_context (serialize_context_),
                                              ٻاهريون (ٻاهريون_4), بنياد (بنياد_2), variation_map (variation_map_) {}

  سانچو <قسم_نالو T>
  منطقي عامل () (T&& رڪارڊ)
  {
    إذا (!serialize_context->copy (رڪارڊ, بنياد, variation_map)) أرجع خطأ;
    ٻاهريون.len++;
    أرجع سچ;
  }

  خاص:
  hb_serialize_context_t *serialize_context;
  OutputArray &ٻاهريون;
  ثابت فراغ *بنياد;
  ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map;
};

/*
 * Helper to serialize an array of MATH records.
 */
بنية
{
  سانچو<قسم_نالو OutputArray>
  serialize_math_رڪارڊ_array_t<OutputArray>
  عامل () (hb_serialize_context_t *serialize_context, OutputArray& ٻاهريون,
               ثابت فراغ *بنياد,
               ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *variation_map) ثابت
  { أرجع serialize_math_رڪارڊ_array_t<OutputArray> (serialize_context, ٻاهريون, بنياد, variation_map); }

}
HB_FUNCOBJ (serialize_math_رڪارڊ_array);

/*
 *
 * OpenType Layout Common Table Formats
 *
 */


/*
 * Script, ScriptList, LangSys, Feature, FeatureList, Lookup, LookupList
 */

بنية اشاريو_Array : Array16_جو<اشاريو>
{
  منطقي intersects (ثابت hb_map_t *indexes) ثابت
  { أرجع hb_any (*هذا, indexes); }

  سانچو <قسم_نالو Iterator,
	    hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_serialize_context_t *c,
		  hb_subset_layout_context_t *l,
		  Iterator it)
  {
    إذا (!it) أرجع;
    إذا (unlikely (!c->extend_min ((*هذا)))) أرجع;

    لكل (ثابت خودڪار _ : it)
    {
      إذا (!l->گهمو_Lookup_اشاريو()) اكسر;

      اشاريو i;
      i = _;
      c->copy (i);
      هذا->len++;
    }
  }

  بنانشان صحيح get_indexes (بنانشان صحيح شروعات_وٿي,
			    بنانشان صحيح *ڳڻپ_2 /* IN/OUT */,
			    بنانشان صحيح *_indexes /*ٻاهريون*/) ثابت
  {
    إذا (ڳڻپ_2 && _indexes)
    {
      + هذا->as_array ().sub_array (شروعات_وٿي, ڳڻپ_2)
      | hb_sink (hb_array (_indexes, *ڳڻپ_2))
      ;
    }
    أرجع هذا->len;
  }

  فراغ جوڙيو_indexes_منزل (hb_سيٽ_t* output /*ٻاهريون*/) ثابت
  {
    output->جوڙيو_array (as_array ());
  }
};


/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#size */
بنية Feature_Params_سائيز
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

    إذا (!design_سائيز)
      return_trace (خطأ);
    وإلا إذا (subfamilyID == 0 &&
	     subfamilyNameID == 0 &&
	     range_شروعات == 0 &&
	     range_پڇاڙي == 0)
      return_trace (سچ);
    وإلا إذا (design_سائيز < range_شروعات ||
	     design_سائيز > range_پڇاڙي ||
	     subfamilyNameID < 256 ||
	     subfamilyNameID > 32767)
      return_trace (خطأ);
    وإلا
      return_trace (سچ);
  }

  فراغ collect_name_ids (hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  { nameids_منزل_retain->جوڙيو (subfamilyNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  HBUINT16	design_سائيز;	/* Represents the design size in 720/inch
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
  HBUINT16	range_شروعات;	/* Large end of the recommended usage range
				 * (inclusive), stored in 720/inch units
				 * (decipoints). */
  HBUINT16	range_پڇاڙي;	/* Small end of the recommended usage range
				   (exclusive), stored in 720/inch units
				 * (decipoints). */
  عام:
  DEFINE_سائيز_STATIC (10);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_pt#ssxx */
بنية Feature_Params_Stylistic_سيٽ
{
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    /* Right now minorVersion is at zero.  Which means, any table supports
     * the uiNameID field. */
    return_trace (c->check_struct (هذا));
  }

  فراغ collect_name_ids (hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  { nameids_منزل_retain->جوڙيو (uiNameID); }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    return_trace ((منطقي) c->serializer->embed (*هذا));
  }

  HBUINT16	نسخو;	/* (set to 0): This corresponds to a “minor”
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
  DEFINE_سائيز_STATIC (4);
};

/* https://docs.microsoft.com/en-us/typography/opentype/spec/features_ae#cv01-cv99 */
بنية FeatureParamsCharacterVariants
{
  بنانشان
  get_characters (بنانشان شروعات_وٿي, بنانشان *char_ڳڻپ, hb_codepoint_t *chars) ثابت
  {
    إذا (char_ڳڻپ && chars)
    {
      + characters.as_array ().sub_array (شروعات_وٿي, char_ڳڻپ)
      | hb_sink (hb_array (chars, *char_ڳڻپ))
      ;
    }
    أرجع characters.len;
  }

  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (characters.len, HBUINT24::static_سائيز, min_سائيز); }

  فراغ collect_name_ids (hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  {
    إذا (featUILableNameID) nameids_منزل_retain->جوڙيو (featUILableNameID);
    إذا (feat_UITooltip_متن_Name_ID) nameids_منزل_retain->جوڙيو (feat_UITooltip_متن_Name_ID);
    إذا (sample_متن_Name_ID) nameids_منزل_retain->جوڙيو (sample_متن_Name_ID);

    إذا (!پهريون_Param_UILabel_Name_ID || !انگ_Named_Parameters || انگ_Named_Parameters >= 0x7FFF)
      أرجع;

    بنانشان آخري_name_id = (بنانشان) پهريون_Param_UILabel_Name_ID + (بنانشان) انگ_Named_Parameters - 1;
    nameids_منزل_retain->جوڙيو_range (پهريون_Param_UILabel_Name_ID, آخري_name_id);
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
  HBUINT16	انگ_Named_Parameters;	/* Number of named parameters. (May
					 * be zero.) */
  NameID	پهريون_Param_UILabel_Name_ID;/* The first ‘name’ table name ID
					 * used to specify strings for
					 * user-interface labels for the
					 * feature parameters. (Must be zero
					 * if numParameters is zero.) */
  Array16_جو<HBUINT24>
		characters;		/* Array of the Unicode Scalar Value
					 * of the characters for which this
					 * feature provides glyph variants.
					 * (May be zero.) */
  عام:
  DEFINE_سائيز_ARRAY (14, characters);
};

بنية FeatureParams
{
  منطقي sanitize (hb_sanitize_context_t *c, hb_ٽيگ_t ٽيگ_2) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع سچ;
#جيڪڏهن_پورو
    TRACE_SANITIZE (هذا);
    إذا (ٽيگ_2 == HB_ٽيگ ('s','i','z','e') && hb_barrier ())
      return_trace (u.سائيز.sanitize (c));
    إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سيٽ.sanitize (c));
    إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.sanitize (c));
    return_trace (سچ);
  }

  فراغ collect_name_ids (hb_ٽيگ_t ٽيگ_2, hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع;
#جيڪڏهن_پورو
    إذا (ٽيگ_2 == HB_ٽيگ ('s','i','z','e') && hb_barrier ())
      أرجع (u.سائيز.collect_name_ids (nameids_منزل_retain));
    إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('s','s','\0','\0')) /* ssXX */
      أرجع (u.stylistic_سيٽ.collect_name_ids (nameids_منزل_retain));
    إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('c','v','\0','\0')) /* cvXX */
      أرجع (u.characterVariants.collect_name_ids (nameids_منزل_retain));
  }

  منطقي subset (hb_subset_context_t *c, ثابت ٽيگ* ٽيگ_2) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_LAYOUT_FEATURE_PARAMS
    أرجع خطأ;
#جيڪڏهن_پورو
    TRACE_SUBSET (هذا);
    إذا (!ٽيگ_2) return_trace (خطأ);
    إذا (*ٽيگ_2 == HB_ٽيگ ('s','i','z','e') && hb_barrier ())
      return_trace (u.سائيز.subset (c));
    إذا ((*ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('s','s','\0','\0')) /* ssXX */
      return_trace (u.stylistic_سيٽ.subset (c));
    إذا ((*ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('c','v','\0','\0')) /* cvXX */
      return_trace (u.characterVariants.subset (c));
    return_trace (خطأ);
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_LAYOUT_FEATURE_PARAMS
  ثابت Feature_Params_سائيز& get_سائيز_params (hb_ٽيگ_t ٽيگ_2) ثابت
  {
    إذا (ٽيگ_2 == HB_ٽيگ ('s','i','z','e') && hb_barrier ())
      أرجع u.سائيز;
    أرجع Null (Feature_Params_سائيز);
  }
  ثابت Feature_Params_Stylistic_سيٽ& get_stylistic_سيٽ_params (hb_ٽيگ_t ٽيگ_2) ثابت
  {
    إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('s','s','\0','\0')) /* ssXX */
      أرجع u.stylistic_سيٽ;
    أرجع Null (Feature_Params_Stylistic_سيٽ);
  }
  ثابت FeatureParamsCharacterVariants& get_character_variants_params (hb_ٽيگ_t ٽيگ_2) ثابت
  {
    إذا ((ٽيگ_2 & 0xFFFF0000u) == HB_ٽيگ ('c','v','\0','\0')) /* cvXX */
      أرجع u.characterVariants;
    أرجع Null (FeatureParamsCharacterVariants);
  }
#جيڪڏهن_پورو

  خاص:
  اتحاد {
  Feature_Params_سائيز			سائيز;
  Feature_Params_Stylistic_سيٽ		stylistic_سيٽ;
  FeatureParamsCharacterVariants	characterVariants;
  } u;
  عام:
  DEFINE_سائيز_MIN (0);
};

بنية رڪارڊ_sanitize_گهيرو_t {
  hb_ٽيگ_t ٽيگ_2;
  ثابت فراغ *list_بنياد;
};

بنية Feature
{
  بنانشان صحيح get_lookup_ڳڻپ () ثابت
  { أرجع lookup_اشاريو_2.len; }
  hb_ٽيگ_t get_lookup_اشاريو (بنانشان صحيح i) ثابت
  { أرجع lookup_اشاريو_2[i]; }
  بنانشان صحيح get_lookup_indexes (بنانشان صحيح شروعات_اشاريو,
				   بنانشان صحيح *lookup_ڳڻپ /* IN/OUT */,
				   بنانشان صحيح *lookup_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  { أرجع lookup_اشاريو_2.get_indexes (شروعات_اشاريو, lookup_ڳڻپ, lookup_ٽيگ_فهرست); }
  فراغ جوڙيو_lookup_indexes_منزل (hb_سيٽ_t *lookup_indexes) ثابت
  { lookup_اشاريو_2.جوڙيو_indexes_منزل (lookup_indexes); }

  ثابت FeatureParams &get_feature_params () ثابت
  { أرجع هذا+featureParams; }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookup_اشاريو_2.intersects (lookup_indexes); }

  فراغ collect_name_ids (hb_ٽيگ_t ٽيگ_2, hb_سيٽ_t *nameids_منزل_retain /*ٻاهريون*/) ثابت
  {
    إذا (featureParams)
      get_feature_params ().collect_name_ids (ٽيگ_2, nameids_منزل_retain);
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت ٽيگ                   *ٽيگ_2 = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->featureParams.serialize_subset (c, featureParams, هذا, ٽيگ_2);

    hb::shared_ptr<hb_سيٽ_t> *lookup_variations;
    إذا (l->cur_feature_اشاريو != اشاريو::NOT_مليو_اشاريو &&
	l->lookup_variations->has (l->cur_feature_اشاريو, &lookup_variations))
    {
      خودڪار it =
      + hb_iter (**lookup_variations)
      | hb_filter (l->lookup_اشاريو_map)
      | hb_map (l->lookup_اشاريو_map)
      ;
      ٻاهريون->lookup_اشاريو_2.serialize (c->serializer, l, it);
    }
    وإلا
    {
      خودڪار it =
      + hb_iter (lookup_اشاريو_2)
      | hb_filter (l->lookup_اشاريو_map)
      | hb_map (l->lookup_اشاريو_map)
      ;
      ٻاهريون->lookup_اشاريو_2.serialize (c->serializer, l, it);
    }
    // The decision to keep or drop this feature is already made before we get here
    // so always retain it.
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رڪارڊ_sanitize_گهيرو_t *گهيرو = خالي_اشارو) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureParams.sanitize (c, هذا, گهيرو ? گهيرو->ٽيگ_2 : HB_ٽيگ_NONE) &&
		  lookup_اشاريو_2.sanitize (c));
  }

  Offset16_منزل<FeatureParams>
		 featureParams;	/* Offset to Feature Parameters table (if one
				 * has been defined for the feature), relative
				 * to the beginning of the Feature Table; = Null
				 * if not required */
  اشاريو_Array	 lookup_اشاريو_2;	/* Array of LookupList indices */
  عام:
  DEFINE_سائيز_ARRAY_SIZED (4, lookup_اشاريو_2);
};

سانچو <قسم_نالو Type>
بنية رڪارڊ_2
{
  صحيح cmp (hb_ٽيگ_t a) ثابت { أرجع ٽيگ_2.cmp (a); }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *بنياد, ثابت فراغ *f_sub = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (!f_sub)
      return_trace (ٻاهريون->وٿي.serialize_subset (c->subset_context, وٿي, بنياد, c, &ٽيگ_2));

    ثابت Feature& f = *reinterpret_cast<ثابت Feature *> (f_sub);
    خودڪار *s = c->subset_context->serializer;
    s->push ();

    ٻاهريون->وٿي = 0;
    منطقي ret = f.subset (c->subset_context, c, &ٽيگ_2);
    إذا (ret)
      s->جوڙيو_link (ٻاهريون->وٿي, s->ڪڍو_pack ());
    وإلا
      s->ڪڍو_discard ();

    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    ثابت رڪارڊ_sanitize_گهيرو_t گهيرو = {ٽيگ_2, بنياد};
    return_trace (c->check_struct (هذا) &&
		  وٿي.sanitize (c, بنياد, &گهيرو));
  }

  ٽيگ           ٽيگ_2;            /* 4-byte Tag identifier */
  Offset16_منزل<Type>
                وٿي;         /* Offset from beginning of object holding
                                 * the Record */
  عام:
  DEFINE_سائيز_STATIC (6);
};

سانچو <قسم_نالو Type>
بنية رڪارڊ_Array_جو : Sorted_Array16_جو<رڪارڊ_2<Type>>
{
  ثابت Offset16_منزل<Type>& get_وٿي (بنانشان صحيح i) ثابت
  { أرجع (*هذا)[i].وٿي; }
  Offset16_منزل<Type>& get_وٿي (بنانشان صحيح i)
  { أرجع (*هذا)[i].وٿي; }
  ثابت ٽيگ& get_ٽيگ (بنانشان صحيح i) ثابت
  { أرجع (*هذا)[i].ٽيگ_2; }
  بنانشان صحيح get_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
                         بنانشان صحيح *رڪارڊ_ڳڻپ /* IN/OUT */,
                         hb_ٽيگ_t     *رڪارڊ_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  {
    إذا (رڪارڊ_ڳڻپ && رڪارڊ_ٽيگ_فهرست)
    {
      + هذا->as_array ().sub_array (شروعات_وٿي, رڪارڊ_ڳڻپ)
      | hb_map (&رڪارڊ_2<Type>::ٽيگ_2)
      | hb_sink (hb_array (رڪارڊ_ٽيگ_فهرست, *رڪارڊ_ڳڻپ))
      ;
    }
    أرجع هذا->len;
  }
  منطقي ڳوليو_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  {
    أرجع هذا->bfind (ٽيگ_2, اشاريو_2, HB_NOT_مليو_STORE, اشاريو::NOT_مليو_اشاريو);
  }
};

سانچو <قسم_نالو Type>
بنية رڪارڊ_List_جو : رڪارڊ_Array_جو<Type>
{
  ثابت Type& عامل [] (بنانشان صحيح i) ثابت
  { أرجع هذا+هذا->get_وٿي (i); }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    + هذا->iter ()
    | hb_apply (subset_رڪارڊ_array (l, ٻاهريون, هذا))
    ;
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (رڪارڊ_Array_جو<Type>::sanitize (c, هذا));
  }
};

بنية رڪارڊ_List_جو_Feature : رڪارڊ_List_جو<Feature>
{
  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->feature_اشاريو_map, hb_پهريون)
    | hb_apply ([l, ٻاهريون, هذا] (ثابت hb_pair_t<بنانشان, ثابت رڪارڊ_2<Feature>&>& _)
                {
                  ثابت Feature *f_sub = خالي_اشارو;
                  ثابت Feature **f = خالي_اشارو;
                  إذا (l->feature_substitutes_map->has (_.پهريون, &f))
                    f_sub = *f;

                  l->cur_feature_اشاريو = _.پهريون;
                  subset_رڪارڊ_array (l, ٻاهريون, هذا, f_sub) (_.second);
                  l->cur_feature_اشاريو = اشاريو::NOT_مليو_اشاريو;
                })
    ;

    return_trace (سچ);
  }
};

تعريفنوع رڪارڊ_List_جو<Feature> FeatureList;


بنية LangSys
{
  بنانشان صحيح get_feature_ڳڻپ () ثابت
  { أرجع feature_اشاريو_2.len; }
  hb_ٽيگ_t get_feature_اشاريو (بنانشان صحيح i) ثابت
  { أرجع feature_اشاريو_2[i]; }
  بنانشان صحيح get_feature_indexes (بنانشان صحيح شروعات_وٿي,
				    بنانشان صحيح *feature_ڳڻپ_2 /* IN/OUT */,
				    بنانشان صحيح *feature_indexes /*ٻاهريون*/) ثابت
  { أرجع feature_اشاريو_2.get_indexes (شروعات_وٿي, feature_ڳڻپ_2, feature_indexes); }
  فراغ جوڙيو_feature_indexes_منزل (hb_سيٽ_t *feature_indexes) ثابت
  { feature_اشاريو_2.جوڙيو_indexes_منزل (feature_indexes); }

  منطقي has_required_feature () ثابت { أرجع req_Feature_اشاريو != 0xFFFFu; }
  بنانشان صحيح get_required_feature_اشاريو () ثابت
  {
    إذا (req_Feature_اشاريو == 0xFFFFu)
      أرجع اشاريو::NOT_مليو_اشاريو;
   أرجع req_Feature_اشاريو;
  }

  LangSys* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  منطقي compare (ثابت LangSys& o, ثابت hb_map_t *feature_اشاريو_map) ثابت
  {
    إذا (req_Feature_اشاريو != o.req_Feature_اشاريو)
      أرجع خطأ;

    خودڪار iter =
    + hb_iter (feature_اشاريو_2)
    | hb_filter (feature_اشاريو_map)
    | hb_map (feature_اشاريو_map)
    ;

    خودڪار o_iter =
    + hb_iter (o.feature_اشاريو_2)
    | hb_filter (feature_اشاريو_map)
    | hb_map (feature_اشاريو_map)
    ;

    لكل (; iter && o_iter; iter++, o_iter++)
    {
      بنانشان a = *iter;
      بنانشان b = *o_iter;
      إذا (a != b) أرجع خطأ;
    }

    إذا (iter || o_iter) أرجع خطأ;

    أرجع سچ;
  }

  فراغ collect_features (hb_prune_langsys_context_t *c) ثابت
  {
    إذا (!has_required_feature () && !get_feature_ڳڻپ ()) أرجع;
    إذا (has_required_feature () &&
        c->duplicate_feature_map->has (req_Feature_اشاريو))
      c->new_feature_indexes->جوڙيو (get_required_feature_اشاريو ());

    + hb_iter (feature_اشاريو_2)
    | hb_filter (c->duplicate_feature_map)
    | hb_sink (c->new_feature_indexes)
    ;
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
	       ثابت ٽيگ                  *ٽيگ_2 = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ثابت uint32_t *v;
    ٻاهريون->req_Feature_اشاريو = l->feature_map_w_duplicates->has (req_Feature_اشاريو, &v) ? *v : 0xFFFFu;

    إذا (!l->گهمو_Feature_اشاريو (feature_اشاريو_2.len))
      return_trace (خطأ);

    خودڪار it =
    + hb_iter (feature_اشاريو_2)
    | hb_filter (l->feature_map_w_duplicates)
    | hb_map (l->feature_map_w_duplicates)
    ;

    منطقي ret = منطقي (it);
    ٻاهريون->feature_اشاريو_2.serialize (c->serializer, l, it);
    return_trace (ret);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رڪارڊ_sanitize_گهيرو_t * = خالي_اشارو) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && feature_اشاريو_2.sanitize (c));
  }

  Offset16	lookup_ترتيب_Z;	/* = Null (reserved for an offset to a
				 * reordering table) */
  HBUINT16	req_Feature_اشاريو;/* Index of a feature required for this
				 * language system--if no required features
				 * = 0xFFFFu */
  اشاريو_Array	feature_اشاريو_2;	/* Array of indices into the FeatureList */
  عام:
  DEFINE_سائيز_ARRAY_SIZED (6, feature_اشاريو_2);
};
DECLARE_NULL_NAMESPACE_بائيٽون (OT, LangSys);

بنية Script
{
  بنانشان صحيح get_lang_sys_ڳڻپ () ثابت
  { أرجع langSys.len; }
  ثابت ٽيگ& get_lang_sys_ٽيگ (بنانشان صحيح i) ثابت
  { أرجع langSys.get_ٽيگ (i); }
  بنانشان صحيح get_lang_sys_ٽيگ_فهرست (بنانشان صحيح شروعات_وٿي,
				  بنانشان صحيح *lang_sys_ڳڻپ /* IN/OUT */,
				  hb_ٽيگ_t     *lang_sys_ٽيگ_فهرست /*ٻاهريون*/) ثابت
  { أرجع langSys.get_ٽيگ_فهرست (شروعات_وٿي, lang_sys_ڳڻپ, lang_sys_ٽيگ_فهرست); }
  ثابت LangSys& get_lang_sys (بنانشان صحيح i) ثابت
  {
    إذا (i == اشاريو::NOT_مليو_اشاريو) أرجع get_default_lang_sys ();
    أرجع هذا+langSys[i].وٿي;
  }
  منطقي ڳوليو_lang_sys_اشاريو (hb_ٽيگ_t ٽيگ_2, بنانشان صحيح *اشاريو_2) ثابت
  { أرجع langSys.ڳوليو_اشاريو (ٽيگ_2, اشاريو_2); }

  منطقي has_default_lang_sys () ثابت           { أرجع defaultLangSys != 0; }
  ثابت LangSys& get_default_lang_sys () ثابت { أرجع هذا+defaultLangSys; }

  فراغ prune_langsys (hb_prune_langsys_context_t *c,
                      بنانشان script_اشاريو) ثابت
  {
    إذا (!has_default_lang_sys () && !get_lang_sys_ڳڻپ ()) أرجع;
    إذا (!c->گهمو_Script ()) أرجع;

    إذا (!c->script_langsys_map->has (script_اشاريو))
    {
      إذا (unlikely (!c->script_langsys_map->سيٽ (script_اشاريو, hb::unique_ptr<hb_سيٽ_t> {hb_سيٽ_create ()})))
	أرجع;
    }

    إذا (has_default_lang_sys ())
    {
      //only collect features from non-redundant langsys
      ثابت LangSys& d = get_default_lang_sys ();
      إذا (c->گهمو_Langsys (d.get_feature_ڳڻپ ())) {
        d.collect_features (c);
      }

      لكل (خودڪار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.وٿي;
        إذا (!c->گهمو_Langsys (l.get_feature_ڳڻپ ())) استمر;
        إذا (l.compare (d, c->duplicate_feature_map)) استمر;

        l.collect_features (c);
        c->script_langsys_map->get (script_اشاريو)->جوڙيو (_.پهريون);
      }
    }
    وإلا
    {
      لكل (خودڪار _ : + hb_enumerate (langSys))
      {
        ثابت LangSys& l = هذا+_.second.وٿي;
        إذا (!c->گهمو_Langsys (l.get_feature_ڳڻپ ())) استمر;
        l.collect_features (c);
        c->script_langsys_map->get (script_اشاريو)->جوڙيو (_.پهريون);
      }
    }
  }

  منطقي subset (hb_subset_context_t         *c,
	       hb_subset_layout_context_t  *l,
	       ثابت ٽيگ                   *ٽيگ_2) ثابت
  {
    TRACE_SUBSET (هذا);
    إذا (!l->گهمو_Script ()) return_trace (خطأ);
    إذا (ٽيگ_2 && !c->plan->layout_scripts.has (*ٽيگ_2))
      أرجع خطأ;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    منطقي defaultLang = خطأ;
    إذا (has_default_lang_sys ())
    {
      c->serializer->push ();
      ثابت LangSys& ls = هذا+defaultLangSys;
      منطقي ret = ls.subset (c, l);
      إذا (!ret && ٽيگ_2 && *ٽيگ_2 != HB_ٽيگ ('D', 'F', 'L', 'T'))
      {
	c->serializer->ڪڍو_discard ();
	ٻاهريون->defaultLangSys = 0;
      }
      وإلا
      {
	c->serializer->جوڙيو_link (ٻاهريون->defaultLangSys, c->serializer->ڪڍو_pack ());
	defaultLang = سچ;
      }
    }

    ثابت hb_سيٽ_t *سرگرم_langsys = l->script_langsys_map->get (l->cur_script_اشاريو);
    إذا (سرگرم_langsys)
    {
      + hb_enumerate (langSys)
      | hb_filter (سرگرم_langsys, hb_پهريون)
      | hb_map (hb_ٻيو)
      | hb_filter ([=] (ثابت رڪارڊ_2<LangSys>& رڪارڊ) {أرجع l->گهمو_Lang_Sys (); })
      | hb_apply (subset_رڪارڊ_array (l, &(ٻاهريون->langSys), هذا))
      ;
    }

    return_trace (منطقي (ٻاهريون->langSys.len) || defaultLang || l->جدول_ٽيگ_2 == HB_OT_ٽيگ_GSUB);
  }

  منطقي sanitize (hb_sanitize_context_t *c,
		 ثابت رڪارڊ_sanitize_گهيرو_t * = خالي_اشارو) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (defaultLangSys.sanitize (c, هذا) && langSys.sanitize (c, هذا));
  }

  محمي:
  Offset16_منزل<LangSys>
		defaultLangSys;	/* Offset to DefaultLangSys table--from
				 * beginning of Script table--may be Null */
  رڪارڊ_Array_جو<LangSys>
		langSys;	/* Array of LangSysRecords--listed
				 * alphabetically by LangSysTag */
  عام:
  DEFINE_سائيز_ARRAY_SIZED (4, langSys);
};

بنية رڪارڊ_List_جو_Script : رڪارڊ_List_جو<Script>
{
  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    لكل (خودڪار _ : + hb_enumerate (*هذا))
    {
      خودڪار snap = c->serializer->snapshot ();
      l->cur_script_اشاريو = _.پهريون;
      منطقي ret = _.second.subset (l, هذا);
      إذا (!ret) c->serializer->revert (snap);
      وإلا ٻاهريون->len++;
    }

    return_trace (سچ);
  }
};

تعريفنوع رڪارڊ_List_جو_Script ScriptList;



بنية LookupFlag : HBUINT16
{
  تعداد Flags {
    ساڄو_منزل_کاٻو		= 0x0001u,
    Ignore_بنياد_Glyphs	= 0x0002u,
    IgnoreLigatures	= 0x0004u,
    IgnoreMarks		= 0x0008u,
    IgnoreFlags		= 0x000Eu,
    Use_Mark_Filtering_سيٽ	= 0x0010u,
    Reserved		= 0x00E0u,
    MarkAttachmentType	= 0xFF00u
  };
  عام:
  DEFINE_سائيز_STATIC (2);
};

} /* namespace OT */
/* This has to be outside the namespace. */
HB_MARK_AS_FLAG_T (OT::LookupFlag::Flags);
نطاق OT {

بنية Lookup
{
  بنانشان صحيح get_subtable_ڳڻپ () ثابت { أرجع sub_جدول.len; }

  سانچو <قسم_نالو TSub_جدول>
  ثابت Array16_جو_Offset16_منزل<TSub_جدول>& get_subtables () ثابت
  { أرجع reinterpret_cast<ثابت Array16_جو_Offset16_منزل<TSub_جدول> &> (sub_جدول); }
  سانچو <قسم_نالو TSub_جدول>
  Array16_جو_Offset16_منزل<TSub_جدول>& get_subtables ()
  { أرجع reinterpret_cast<Array16_جو_Offset16_منزل<TSub_جدول> &> (sub_جدول); }

  سانچو <قسم_نالو TSub_جدول>
  ثابت TSub_جدول& get_subtable (بنانشان صحيح i) ثابت
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }
  سانچو <قسم_نالو TSub_جدول>
  TSub_جدول& get_subtable (بنانشان صحيح i)
  { أرجع هذا+get_subtables<TSub_جدول> ()[i]; }

  size_t get_سائيز () ثابت
  {
    ثابت HBUINT16 &mark_Filtering_سيٽ = StructAfter<ثابت HBUINT16> (sub_جدول);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سيٽ)
      أرجع (ثابت حرف *) &StructAfter<ثابت حرف> (mark_Filtering_سيٽ) - (ثابت حرف *) هذا;
    أرجع (ثابت حرف *) &mark_Filtering_سيٽ - (ثابت حرف *) هذا;
  }

  بنانشان صحيح get_type () ثابت { أرجع lookupType; }

  /* lookup_props is a 32-bit integer where the lower 16-bit is LookupFlag and
   * higher 16-bit is mark-filtering-set if the lookup uses one.
   * Not to be confused with glyph_props which is very similar. */
  uint32_t get_props () ثابت
  {
    بنانشان صحيح flag = lookupFlag;
    إذا (unlikely (flag & LookupFlag::Use_Mark_Filtering_سيٽ))
    {
      ثابت HBUINT16 &mark_Filtering_سيٽ = StructAfter<HBUINT16> (sub_جدول);
      flag += (mark_Filtering_سيٽ << 16);
    }
    أرجع flag;
  }

  سانچو <قسم_نالو TSub_جدول, قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
  {
    بنانشان صحيح lookup_type = get_type ();
    TRACE_DISPATCH (هذا, lookup_type);
    بنانشان صحيح ڳڻپ = get_subtable_ڳڻپ ();
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++) {
      قسم_نالو context_t::return_t r = get_subtable<TSub_جدول> (i).dispatch (c, lookup_type, ds...);
      إذا (c->stop_sublookup_iteration (r))
	return_trace (r);
    }
    return_trace (c->default_return_قدر ());
  }

  منطقي serialize (hb_serialize_context_t *c,
		  بنانشان صحيح lookup_type,
		  uint32_t lookup_props,
		  بنانشان صحيح انگ_subtables)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    lookupType = lookup_type;
    lookupFlag = lookup_props & 0xFFFFu;
    إذا (unlikely (!sub_جدول.serialize (c, انگ_subtables))) return_trace (خطأ);
    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سيٽ)
    {
      إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
      HBUINT16 &mark_Filtering_سيٽ = StructAfter<HBUINT16> (sub_جدول);
      mark_Filtering_سيٽ = lookup_props >> 16;
    }
    return_trace (سچ);
  }

  سانچو <قسم_نالو TSub_جدول>
  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->lookupType = lookupType;
    ٻاهريون->lookupFlag = lookupFlag;

    ثابت hb_سيٽ_t *glyphset = c->plan->glyphset_gsub ();
    بنانشان صحيح lookup_type = get_type ();
    + hb_iter (get_subtables <TSub_جدول> ())
    | hb_filter ([هذا, glyphset, lookup_type] (ثابت Offset16_منزل<TSub_جدول> &_) { أرجع (هذا+_).intersects (glyphset, lookup_type); })
    | hb_apply (subset_وٿي_array (c, ٻاهريون->get_subtables<TSub_جدول> (), هذا, lookup_type))
    ;

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سيٽ)
    {
      ثابت HBUINT16 &mark_Filtering_سيٽ = StructAfter<HBUINT16> (sub_جدول);
      hb_codepoint_t *idx;
      إذا (!c->plan->استعمال_ٿيل_mark_sets_map.has (mark_Filtering_سيٽ, &idx))
      {
        بنانشان new_flag = lookupFlag;
        new_flag &= ~LookupFlag::Use_Mark_Filtering_سيٽ;
        // https://github.com/harfbuzz/harfbuzz/issues/5499
        // If we remove UseMarkFilteringSet flag because the set is now empty,
        // we need to add IgnoreMarks flag, otherwise the lookup will not
        // ignore any marks, which changes the behavior.
        new_flag |= LookupFlag::IgnoreMarks;
        ٻاهريون->lookupFlag = new_flag;
      }
      وإلا
      {
        إذا (unlikely (!c->serializer->extend (ٻاهريون))) return_trace (خطأ);
        HBUINT16 &ٻاهريون_Mark_Filtering_سيٽ = StructAfter<HBUINT16> (ٻاهريون->sub_جدول);
        ٻاهريون_Mark_Filtering_سيٽ = *idx;
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

  سانچو <قسم_نالو TSub_جدول>
  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!(c->check_struct (هذا) && sub_جدول.sanitize (c))) return_trace (خطأ);
    hb_barrier ();

    بنانشان subtables = get_subtable_ڳڻپ ();
    إذا (unlikely (!c->گهمو_subtables (subtables))) return_trace (خطأ);

    إذا (lookupFlag & LookupFlag::Use_Mark_Filtering_سيٽ)
    {
      ثابت HBUINT16 &mark_Filtering_سيٽ = StructAfter<HBUINT16> (sub_جدول);
      إذا (!mark_Filtering_سيٽ.sanitize (c)) return_trace (خطأ);
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
      بنانشان صحيح type = get_subtable<TSub_جدول> (0).u.extension.get_type ();
      لكل (بنانشان صحيح i = 1; i < subtables; i++)
	إذا (get_subtable<TSub_جدول> (i).u.extension.get_type () != type)
	  return_trace (خطأ);
    }
    return_trace (سچ);
  }

  محمي:
  HBUINT16	lookupType;		/* Different enumerations for GSUB and GPOS */
  HBUINT16	lookupFlag;		/* Lookup qualifiers */
  Array16_جو<Offset16>
		sub_جدول;		/* Array of SubTables */
/*HBUINT16	markFilteringSetX[HB_VAR_ARRAY];*//* Index (base 0) into GDEF mark glyph sets
					 * structure. This field is only present if bit
					 * UseMarkFilteringSet of lookup flags is set. */
  عام:
  DEFINE_سائيز_ARRAY (6, sub_جدول);
};

سانچو <قسم_نالو Types>
استعمال LookupList = List16_جو_وٿي_منزل<Lookup, قسم_نالو Types::HBLUINT>;

سانچو <قسم_نالو TLookup, قسم_نالو وٿي_Type>
بنية Lookup_وٿي_List : List16_جو_وٿي_منزل<TLookup, وٿي_Type>
{
  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    + hb_enumerate (*هذا)
    | hb_filter (l->lookup_اشاريو_map, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_apply (subset_وٿي_array (c, *ٻاهريون, هذا))
    ;
    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace ((List16_جو_وٿي_منزل<TLookup, وٿي_Type>::sanitize (c, هذا)));
  }
};


/*
 * Coverage Table
 */


ساكن منطقي ClassDef_remap_and_serialize (hb_serialize_context_t *c,
					  ثابت hb_سيٽ_t &klasses,
                                          منطقي use_class_zero,
                                          hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> &glyph_and_klass, /* IN/OUT */
					  hb_map_t *klass_map /*IN/OUT*/)
{
  إذا (!klass_map)
    أرجع ClassDef_serialize (c, glyph_and_klass.iter ());

  /* any glyph not assigned a class value falls into Class zero (0),
   * if any glyph assigned to class 0, remapping must start with 0->0*/
  إذا (!use_class_zero)
    klass_map->سيٽ (0, 0);

  بنانشان idx = klass_map->has (0) ? 1 : 0;
  لكل (ثابت بنانشان k: klasses)
  {
    إذا (klass_map->has (k)) استمر;
    klass_map->سيٽ (k, idx);
    idx++;
  }


  إذا (unlikely (!c->propagate_error (glyph_and_klass, klasses, klass_map)))
    أرجع خطأ;

  لكل (بنانشان i = 0; i < glyph_and_klass.ڊيگهائي; i++)
  {
    hb_codepoint_t klass = glyph_and_klass[i].second;
    glyph_and_klass[i].second = klass_map->get (klass);
  }

  أرجع ClassDef_serialize (c, glyph_and_klass.iter ());
}

/*
 * Class Definition Table
 */

سانچو <قسم_نالو Types>
بنية ClassDefFormat1_3
{
  دوست بنية ClassDef;

  خاص:
  بنانشان صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع class_قدر[(بنانشان صحيح) (glyph_id - شروعات_Glyph)];
  }

  بنانشان get_population () ثابت
  {
    أرجع class_قدر.len;
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_sorted_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::سائيز == 2 ? 1 : 3;
      شروعات_Glyph = 0;
      class_قدر.len = 0;
      return_trace (سچ);
    }

    hb_codepoint_t glyph_min = (*it).پهريون;
    hb_codepoint_t glyph_max = + it
			       | hb_map (hb_پهريون)
			       | hb_reduce (hb_max, 0u);
    بنانشان glyph_ڳڻپ = glyph_max - glyph_min + 1;

    شروعات_Glyph = glyph_min;
    إذا (unlikely (!class_قدر.serialize (c, glyph_ڳڻپ))) return_trace (خطأ);
    لكل (ثابت hb_pair_t<hb_codepoint_t, uint32_t> gid_klass_pair : + it)
    {
      بنانشان idx = gid_klass_pair.پهريون - glyph_min;
      class_قدر[idx] = gid_klass_pair.second;
    }
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالي_اشارو /*OUT*/,
               منطقي keep_خالي_جايون_جدول = سچ,
               منطقي use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);

    hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سيٽ_t orig_klasses;

    hb_codepoint_t شروعات = شروعات_Glyph;
    hb_codepoint_t پڇاڙي   = شروعات + class_قدر.len;

    لكل (ثابت hb_codepoint_t gid : + hb_range (شروعات, پڇاڙي))
    {
      hb_codepoint_t new_gid = c->plan->map_gsub_glyph (gid);
      إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
      إذا (glyph_filter && !glyph_filter->has(gid)) استمر;

      بنانشان klass = class_قدر[gid - شروعات];
      إذا (!klass) استمر;

      glyph_and_klass.push (hb_pair (new_gid, klass));
      orig_klasses.جوڙيو (klass);
    }

    إذا (use_class_zero)
    {
      ثابت hb_سيٽ_t& glyphset = *c->plan->glyphset_gsub ();
      بنانشان glyph_ڳڻپ = glyph_filter
			     ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
			     : glyphset.get_population ();
      use_class_zero = glyph_ڳڻپ <= glyph_and_klass.ڊيگهائي;
    }
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_خالي_جايون_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && class_قدر.sanitize (c));
  }

  بنانشان لاڳت () ثابت { أرجع 1; }

  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_coverage (سيٽ_t *glyphs) ثابت
  {
    بنانشان صحيح شروعات = 0;
    بنانشان صحيح ڳڻپ = class_قدر.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (class_قدر[i])
	استمر;

      إذا (شروعات != i)
	إذا (unlikely (!glyphs->جوڙيو_range (شروعات_Glyph + شروعات, شروعات_Glyph + i)))
	  أرجع خطأ;

      شروعات = i + 1;
    }
    إذا (شروعات != ڳڻپ)
      إذا (unlikely (!glyphs->جوڙيو_range (شروعات_Glyph + شروعات, شروعات_Glyph + ڳڻپ)))
	أرجع خطأ;

    أرجع سچ;
  }

  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_class (سيٽ_t *glyphs, بنانشان klass) ثابت
  {
    بنانشان صحيح ڳڻپ = class_قدر.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (class_قدر[i] == klass) glyphs->جوڙيو (شروعات_Glyph + i);
    أرجع سچ;
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    hb_codepoint_t شروعات = شروعات_Glyph;
    hb_codepoint_t پڇاڙي = شروعات_Glyph + class_قدر.len;
    لكل (hb_codepoint_t iter = شروعات_Glyph - 1;
	 glyphs->next (&iter) && iter < پڇاڙي;)
      إذا (class_قدر[iter - شروعات]) أرجع سچ;
    أرجع خطأ;
  }
  منطقي intersects_class (ثابت hb_سيٽ_t *glyphs, بنانشان klass) ثابت
  {
    بنانشان صحيح ڳڻپ = class_قدر.len;
    إذا (klass == 0)
    {
      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سيٽ_قدر_INVALID;
      إذا (!glyphs->next (&g)) أرجع خطأ;
      إذا (g < شروعات_Glyph) أرجع سچ;
      g = شروعات_Glyph + ڳڻپ - 1;
      إذا (glyphs->next (&g)) أرجع سچ;
      /* Fall through. */
    }
    /* TODO Speed up, using set overlap first? */
    /* TODO(iter) Rewrite as dagger. */
    ثابت قسم_نالو Types::HBUINT *arr = class_قدر.arrayZ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (arr[i] == klass && glyphs->has (شروعات_Glyph + i))
	أرجع سچ;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_سيٽ_t *glyphs, بنانشان klass, hb_سيٽ_t *intersect_glyphs) ثابت
  {
    بنانشان ڳڻپ = class_قدر.len;
    إذا (klass == 0)
    {
      بنانشان شروعات_glyph = شروعات_Glyph;
      لكل (uint32_t g = HB_سيٽ_قدر_INVALID;
	   glyphs->next (&g) && g < شروعات_glyph;)
	intersect_glyphs->جوڙيو (g);

      لكل (uint32_t g = شروعات_Glyph + ڳڻپ - 1;
	   glyphs-> next (&g);)
	intersect_glyphs->جوڙيو (g);

      أرجع;
    }

    لكل (بنانشان i = 0; i < ڳڻپ; i++)
      إذا (class_قدر[i] == klass && glyphs->has (شروعات_Glyph + i))
	intersect_glyphs->جوڙيو (شروعات_Glyph + i);

#إذا 0
    /* The following implementation is faster asymptotically, but slower
     * in practice. */
    بنانشان شروعات_glyph = شروعات_Glyph;
    بنانشان پڇاڙي_glyph = شروعات_glyph + ڳڻپ;
    لكل (بنانشان g = شروعات_Glyph - 1;
	 glyphs->next (&g) && g < پڇاڙي_glyph;)
      إذا (class_قدر.arrayZ[g - شروعات_glyph] == klass)
        intersect_glyphs->جوڙيو (g);
#جيڪڏهن_پورو
  }

  فراغ intersected_classes (ثابت hb_سيٽ_t *glyphs, hb_سيٽ_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: `end_glyph` below
     * underflows when startGlyph is zero, and neither comparison can then be
     * true. */
    إذا (!class_قدر.len)
    {
      intersect_classes->جوڙيو (0);
      أرجع;
    }

    hb_codepoint_t پڇاڙي_glyph = شروعات_Glyph + class_قدر.len - 1;
    إذا (glyphs->get_min () < شروعات_Glyph ||
        glyphs->get_max () > پڇاڙي_glyph)
      intersect_classes->جوڙيو (0);

    لكل (ثابت خودڪار& _ : + hb_enumerate (class_قدر))
    {
      hb_codepoint_t g = شروعات_Glyph + _.پهريون;
      إذا (glyphs->has (g))
        intersect_classes->جوڙيو (_.second);
    }
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 1 */
  قسم_نالو Types::HBGlyphID
		 شروعات_Glyph;	/* First GlyphID of the classValueArray */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::HBUINT>
		class_قدر;	/* Array of Class Values--one per GlyphID */
  عام:
  DEFINE_سائيز_ARRAY (2 + 2 * Types::سائيز, class_قدر);
};

سانچو <قسم_نالو Types>
بنية ClassDefFormat2_4
{
  دوست بنية ClassDef;

  خاص:
  بنانشان صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    أرجع range_رڪارڊ.bsearch (glyph_id).value;
  }

  بنانشان get_population () ثابت
  {
    قسم_نالو Types::large_int ret = 0;
    لكل (ثابت خودڪار &r : range_رڪارڊ)
      ret += r.get_population ();
    أرجع ret > UINT_MAX ? UINT_MAX : (بنانشان) ret;
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_sorted_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    إذا (unlikely (!it))
    {
      classFormat = Types::سائيز == 2 ? 2 : 4;
      range_رڪارڊ.len = 0;
      return_trace (سچ);
    }

    بنانشان unsorted = خطأ;
    بنانشان انگ_ranges = 1;
    hb_codepoint_t prev_gid = (*it).پهريون;
    بنانشان prev_klass = (*it).second;

    Range_رڪارڊ<Types> range_rec;
    range_rec.پهريون = prev_gid;
    range_rec.آخري = prev_gid;
    range_rec.value = prev_klass;

    خودڪار *رڪارڊ = c->copy (range_rec);
    إذا (unlikely (!رڪارڊ)) return_trace (خطأ);

    لكل (ثابت خودڪار gid_klass_pair : + (++it))
    {
      hb_codepoint_t cur_gid = gid_klass_pair.پهريون;
      بنانشان cur_klass = gid_klass_pair.second;

      إذا (cur_gid != prev_gid + 1 ||
	  cur_klass != prev_klass)
      {

	إذا (unlikely (cur_gid < prev_gid))
	  unsorted = سچ;

	إذا (unlikely (!رڪارڊ)) اكسر;
	رڪارڊ->آخري = prev_gid;
	انگ_ranges++;

	range_rec.پهريون = cur_gid;
	range_rec.آخري = cur_gid;
	range_rec.value = cur_klass;

	رڪارڊ = c->copy (range_rec);
      }

      prev_klass = cur_klass;
      prev_gid = cur_gid;
    }

    إذا (unlikely (c->اندريون_error ())) return_trace (خطأ);

    إذا (likely (رڪارڊ)) رڪارڊ->آخري = prev_gid;
    range_رڪارڊ.len = انگ_ranges;

    إذا (unlikely (unsorted))
      range_رڪارڊ.as_array ().qsort (Range_رڪارڊ<Types>::cmp_range);

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالي_اشارو /*OUT*/,
               منطقي keep_خالي_جايون_جدول = سچ,
               منطقي use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyph_سيٽ = *c->plan->glyphset_gsub ();

    hb_sorted_ويڪٽر_t<hb_codepoint_pair_t> glyph_and_klass;
    hb_سيٽ_t orig_klasses;

    إذا (glyph_سيٽ.get_population () * hb_ٻٽو_انگ_storage ((بنانشان) range_رڪارڊ.len)
	< get_population ())
    {
      لكل (hb_codepoint_t g : glyph_سيٽ)
      {
	بنانشان klass = get_class (g);
	إذا (!klass) استمر;
	hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
	إذا (glyph_filter && !glyph_filter->has (g)) استمر;
	glyph_and_klass.push (hb_pair (new_gid, klass));
	orig_klasses.جوڙيو (klass);
      }
    }
    وإلا
    {
      بنانشان انگ_ذريعو_glyphs = c->plan->ذريعو->get_انگ_glyphs ();
      لكل (خودڪار &range : range_رڪارڊ)
      {
	بنانشان klass = range.value;
	إذا (!klass) استمر;
	hb_codepoint_t شروعات = range.پهريون;
	hb_codepoint_t پڇاڙي   = hb_min (range.آخري + 1, انگ_ذريعو_glyphs);
	لكل (hb_codepoint_t g = شروعات; g < پڇاڙي; g++)
	{
	  hb_codepoint_t new_gid = c->plan->map_gsub_glyph (g);
	  إذا (new_gid == HB_MAP_قدر_INVALID) استمر;
	  إذا (glyph_filter && !glyph_filter->has (g)) استمر;

	  glyph_and_klass.push (hb_pair (new_gid, klass));
	  orig_klasses.جوڙيو (klass);
	}
      }
    }

    ثابت hb_سيٽ_t& glyphset = *c->plan->glyphset_gsub ();
    بنانشان glyph_ڳڻپ = glyph_filter
                           ? hb_len (hb_iter (glyphset) | hb_filter (glyph_filter))
                           : glyphset.get_population ();
    use_class_zero = use_class_zero && glyph_ڳڻپ <= glyph_and_klass.ڊيگهائي;
    إذا (!ClassDef_remap_and_serialize (c->serializer,
                                       orig_klasses,
                                       use_class_zero,
                                       glyph_and_klass,
                                       klass_map))
      return_trace (خطأ);
    return_trace (keep_خالي_جايون_جدول || (منطقي) glyph_and_klass);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (range_رڪارڊ.sanitize (c));
  }

  بنانشان لاڳت () ثابت { أرجع hb_ٻٽو_انگ_storage ((بنانشان) range_رڪارڊ.len); /* bsearch cost */ }

  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_coverage (سيٽ_t *glyphs) ثابت
  {
    لكل (خودڪار &range : range_رڪارڊ)
      إذا (range.value)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    أرجع سچ;
  }

  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_class (سيٽ_t *glyphs, بنانشان صحيح klass) ثابت
  {
    لكل (خودڪار &range : range_رڪارڊ)
    {
      إذا (range.value == klass)
	إذا (unlikely (!range.collect_coverage (glyphs)))
	  أرجع خطأ;
    }
    أرجع سچ;
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    إذا (range_رڪارڊ.len > glyphs->get_population () * hb_ٻٽو_انگ_storage ((بنانشان) range_رڪارڊ.len))
    {
      لكل (خودڪار g : *glyphs)
        إذا (get_class (g))
	  أرجع سچ;
      أرجع خطأ;
    }

    أرجع hb_any (+ hb_iter (range_رڪارڊ)
                   | hb_map ([glyphs] (ثابت Range_رڪارڊ<Types> &range) { أرجع range.intersects (*glyphs) && range.value; }));
  }
  منطقي intersects_class (ثابت hb_سيٽ_t *glyphs, بنانشان klass) ثابت
  {
    إذا (klass == 0)
    {
      /* An empty ClassDef assigns class zero to every glyph, so it intersects
       * class zero as long as there is any glyph at all.  The loop below is a
       * no-op when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
       * guard after it would then wrongly report no intersection. */
      إذا (!range_رڪارڊ.len)
        أرجع !glyphs->is_empty ();

      /* Match if there's any glyph that is not listed! */
      hb_codepoint_t g = HB_سيٽ_قدر_INVALID;
      hb_codepoint_t آخري = HB_سيٽ_قدر_INVALID;
      خودڪار it = hb_iter (range_رڪارڊ);
      لكل (خودڪار &range : it)
      {
        إذا (it->پهريون == آخري + 1)
	{
	  it++;
	  استمر;
	}

	إذا (!glyphs->next (&g))
	  اكسر;
	إذا (g < range.پهريون)
	  أرجع سچ;
	g = range.آخري;
	آخري = g;
      }
      إذا (g != HB_سيٽ_قدر_INVALID && glyphs->next (&g))
	أرجع سچ;
      /* Fall through. */
    }
    لكل (ثابت خودڪار &range : range_رڪارڊ)
      إذا (range.value == klass && range.intersects (*glyphs))
	أرجع سچ;
    أرجع خطأ;
  }

  فراغ intersected_class_glyphs (ثابت hb_سيٽ_t *glyphs, بنانشان klass, hb_سيٽ_t *intersect_glyphs) ثابت
  {
    إذا (klass == 0)
    {
      hb_codepoint_t g = HB_سيٽ_قدر_INVALID;
      لكل (خودڪار &range : range_رڪارڊ)
      {
	إذا (!glyphs->next (&g))
	  اذهب پورو;
	طالما (g < range.پهريون)
	{
	  intersect_glyphs->جوڙيو (g);
	  إذا (!glyphs->next (&g))
	    اذهب پورو;
        }
        g = range.آخري;
      }
      طالما (glyphs->next (&g))
	intersect_glyphs->جوڙيو (g);
      پورو:

      أرجع;
    }

    بنانشان ڳڻپ = range_رڪارڊ.len;
    إذا (ڳڻپ > glyphs->get_population () * hb_ٻٽو_انگ_storage (ڳڻپ))
    {
      لكل (خودڪار g : *glyphs)
      {
        بنانشان i;
        إذا (range_رڪارڊ.as_array ().bfind (g, &i) &&
	    range_رڪارڊ.arrayZ[i].value == klass)
	  intersect_glyphs->جوڙيو (g);
      }
      أرجع;
    }

    لكل (خودڪار &range : range_رڪارڊ)
    {
      إذا (range.value != klass) استمر;

      بنانشان پڇاڙي = range.آخري + 1;
      لكل (hb_codepoint_t g = range.پهريون - 1;
	   glyphs->next (&g) && g < پڇاڙي;)
	intersect_glyphs->جوڙيو (g);
    }
  }

  فراغ intersected_classes (ثابت hb_سيٽ_t *glyphs, hb_سيٽ_t *intersect_classes) ثابت
  {
    إذا (glyphs->is_empty ()) أرجع;

    /* An empty ClassDef assigns class zero to every glyph, so every glyph in
     * `glyphs` intersects class zero.  Handle it up front: the loops below are
     * no-ops when there are no ranges, and the `g != HB_SET_VALUE_INVALID`
     * guard would then wrongly skip adding class zero. */
    إذا (!range_رڪارڊ.len)
    {
      intersect_classes->جوڙيو (0);
      أرجع;
    }

    hb_codepoint_t g = HB_سيٽ_قدر_INVALID;
    لكل (خودڪار &range : range_رڪارڊ)
    {
      إذا (!glyphs->next (&g))
        اكسر;
      إذا (g < range.پهريون)
      {
        intersect_classes->جوڙيو (0);
        اكسر;
      }
      g = range.آخري;
    }
    إذا (g != HB_سيٽ_قدر_INVALID && glyphs->next (&g))
      intersect_classes->جوڙيو (0);

    لكل (ثابت خودڪار& range : range_رڪارڊ)
      إذا (range.intersects (*glyphs))
        intersect_classes->جوڙيو (range.value);
  }

  محمي:
  HBUINT16	classFormat;	/* Format identifier--format = 2 */
  قسم_نالو Types::سانچو Sorted_Array_جو<Range_رڪارڊ<Types>>
		range_رڪارڊ;	/* Array of glyph ranges--ordered by
				 * Start GlyphID */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::سائيز, range_رڪارڊ);
};

بنية ClassDef
{
  /* Has interface. */
  بنانشان عامل [] (hb_codepoint_t k) ثابت { أرجع get (k); }
  منطقي has (hb_codepoint_t k) ثابت { أرجع (*هذا)[k]; }
  /* Projection. */
  hb_codepoint_t عامل () (hb_codepoint_t k) ثابت { أرجع get (k); }

  بنانشان صحيح get (hb_codepoint_t k) ثابت { أرجع get_class (k); }
  بنانشان صحيح get_class (hb_codepoint_t glyph_id) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_class (glyph_id);
    حالة 2: hb_barrier (); أرجع u.format2.get_class (glyph_id);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_class (glyph_id);
    حالة 4: hb_barrier (); أرجع u.format4.get_class (glyph_id);
#جيڪڏهن_پورو
    افتراضي:أرجع 0;
    }
  }
  بنانشان صحيح get_class (hb_codepoint_t glyph_id,
			  hb_ot_layout_mapping_cache_t *cache) ثابت
  {
    بنانشان klass;
    إذا (cache && cache->get (glyph_id, &klass)) أرجع klass;
    klass = get_class (glyph_id);
    إذا (cache) cache->سيٽ (glyph_id, klass);
    أرجع klass;
  }

  بنانشان get_population () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.get_population ();
    حالة 2: hb_barrier (); أرجع u.format2.get_population ();
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.get_population ();
    حالة 4: hb_barrier (); أرجع u.format4.get_population ();
#جيڪڏهن_پورو
    افتراضي:أرجع NOT_COVERED;
    }
  }

  سانچو<قسم_نالو Iterator,
	   hb_requires (hb_is_sorted_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c, Iterator it_with_class_zero)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    خودڪار glyphs = + it_with_class_zero | hb_filter (hb_ٻيو);

    بنانشان format = 2;
    hb_codepoint_t glyph_max = 0;
    بنانشان class_max = 0;
    إذا (likely (glyphs))
    {
      hb_codepoint_t glyph_min = (*glyphs).پهريون;
      glyph_max = glyph_min;

      بنانشان انگ_ranges = 1;
      hb_codepoint_t prev_gid = glyph_min;
      بنانشان prev_klass = (*glyphs).second;

      لكل (ثابت خودڪار gid_klass_pair : + glyphs)
      {
	hb_codepoint_t cur_gid = gid_klass_pair.پهريون;
	بنانشان cur_klass = gid_klass_pair.second;
	إذا (cur_klass > class_max) class_max = cur_klass;
	إذا (cur_gid == glyph_min) استمر;
        إذا (cur_gid > glyph_max) glyph_max = cur_gid;
	إذا (cur_gid != prev_gid + 1 ||
	    cur_klass != prev_klass)
	  انگ_ranges++;

	prev_gid = cur_gid;
	prev_klass = cur_klass;
      }

      uint64_t بهترين_سائيز = UINT64_MAX;
      بنانشان glyph_وسعت = glyph_max - glyph_min + 1;
      خودڪار consider = [&] (بنانشان candidate, uint64_t سائيز)
      {
	إذا (سائيز >= بهترين_سائيز) أرجع;
	بهترين_سائيز = سائيز;
	format = candidate;
      };

      إذا (glyph_max <= 0xFFFFu && glyph_وسعت <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (1, 6 + (uint64_t) HBUINT16::static_سائيز * glyph_وسعت);
      إذا (glyph_max <= 0xFFFFu && انگ_ranges <= 0xFFFFu && class_max <= 0xFFFFu)
	consider (2, 4 + (uint64_t) انگ_ranges *
		     (2 * HBGlyphID16::static_سائيز + HBUINT16::static_سائيز));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
      إذا (glyph_max <= 0xFFFFFFu && glyph_وسعت <= 0xFFFFFFu && class_max <= 0xFFFFFFu)
	consider (3, 8 + (uint64_t) HBUINT24::static_سائيز * glyph_وسعت);
      إذا (glyph_max <= 0xFFFFFFu && انگ_ranges <= 0xFFFFFFu && class_max <= 0xFFFFu)
	consider (4, 5 + (uint64_t) انگ_ranges *
		     (2 * HBGlyphID24::static_سائيز + HBUINT16::static_سائيز));
#جيڪڏهن_پورو
      إذا (unlikely (بهترين_سائيز == UINT64_MAX))
      {
	c->check_success (خطأ, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي);
	return_trace (خطأ);
      }
    }

    u.format.v = format;

    بدل (u.format.v)
    {
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, + it_with_class_zero | hb_filter (hb_ٻيو)));
    حالة 2: hb_barrier (); return_trace (u.format2.serialize (c, + it_with_class_zero | hb_filter (hb_ٻيو)));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.serialize (c, + it_with_class_zero | hb_filter (hb_ٻيو)));
    حالة 4: hb_barrier (); return_trace (u.format4.serialize (c, + it_with_class_zero | hb_filter (hb_ٻيو)));
#جيڪڏهن_پورو
    افتراضي:return_trace (خطأ);
    }
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_map_t *klass_map = خالي_اشارو /*OUT*/,
               منطقي keep_خالي_جايون_جدول = سچ,
               منطقي use_class_zero = سچ,
               ثابت Coverage* glyph_filter = خالي_اشارو) ثابت
  {
    TRACE_SUBSET (هذا);
    بدل (u.format.v) {
    حالة 1: hb_barrier (); return_trace (u.format1.subset (c, klass_map, keep_خالي_جايون_جدول, use_class_zero, glyph_filter));
    حالة 2: hb_barrier (); return_trace (u.format2.subset (c, klass_map, keep_خالي_جايون_جدول, use_class_zero, glyph_filter));
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.subset (c, klass_map, keep_خالي_جايون_جدول, use_class_zero, glyph_filter));
    حالة 4: hb_barrier (); return_trace (u.format4.subset (c, klass_map, keep_خالي_جايون_جدول, use_class_zero, glyph_filter));
#جيڪڏهن_پورو
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
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); return_trace (u.format3.sanitize (c));
    حالة 4: hb_barrier (); return_trace (u.format4.sanitize (c));
#جيڪڏهن_پورو
    افتراضي:return_trace (سچ);
    }
  }

  بنانشان لاڳت () ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.لاڳت ();
    حالة 2: hb_barrier (); أرجع u.format2.لاڳت ();
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.لاڳت ();
    حالة 4: hb_barrier (); أرجع u.format4.لاڳت ();
#جيڪڏهن_پورو
    افتراضي:أرجع 0u;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_coverage (سيٽ_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_coverage (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.collect_coverage (glyphs);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_coverage (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.collect_coverage (glyphs);
#جيڪڏهن_پورو
    افتراضي:أرجع خطأ;
    }
  }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  سانچو <قسم_نالو سيٽ_t>
  منطقي collect_class (سيٽ_t *glyphs, بنانشان صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.collect_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.collect_class (glyphs, klass);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.collect_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.collect_class (glyphs, klass);
#جيڪڏهن_پورو
    افتراضي:أرجع خطأ;
    }
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects (glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersects (glyphs);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects (glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersects (glyphs);
#جيڪڏهن_پورو
    افتراضي:أرجع خطأ;
    }
  }
  منطقي intersects_class (ثابت hb_سيٽ_t *glyphs, بنانشان صحيح klass) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersects_class (glyphs, klass);
    حالة 2: hb_barrier (); أرجع u.format2.intersects_class (glyphs, klass);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersects_class (glyphs, klass);
    حالة 4: hb_barrier (); أرجع u.format4.intersects_class (glyphs, klass);
#جيڪڏهن_پورو
    افتراضي:أرجع خطأ;
    }
  }

  فراغ intersected_class_glyphs (ثابت hb_سيٽ_t *glyphs, بنانشان klass, hb_سيٽ_t *intersect_glyphs) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_class_glyphs (glyphs, klass, intersect_glyphs);
#جيڪڏهن_پورو
    افتراضي:أرجع;
    }
  }

  فراغ intersected_classes (ثابت hb_سيٽ_t *glyphs, hb_سيٽ_t *intersect_classes) ثابت
  {
    بدل (u.format.v) {
    حالة 1: hb_barrier (); أرجع u.format1.intersected_classes (glyphs, intersect_classes);
    حالة 2: hb_barrier (); أرجع u.format2.intersected_classes (glyphs, intersect_classes);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    حالة 3: hb_barrier (); أرجع u.format3.intersected_classes (glyphs, intersect_classes);
    حالة 4: hb_barrier (); أرجع u.format4.intersected_classes (glyphs, intersect_classes);
#جيڪڏهن_پورو
    افتراضي:أرجع;
    }
  }


  محمي:
  اتحاد {
  بنية { HBUINT16 v; }	format;		/* Format identifier */
  ClassDefFormat1_3<SmallTypes>	format1;
  ClassDefFormat2_4<SmallTypes>	format2;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  ClassDefFormat1_3<MediumTypes>format3;
  ClassDefFormat2_4<MediumTypes>format4;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_UNION (2, format.v);
};

سانچو<قسم_نالو Iterator>
ساكن مضمن منطقي ClassDef_serialize (hb_serialize_context_t *c,
				       Iterator it)
{ أرجع (c->شروعات_embed<ClassDef> ()->serialize (c, it)); }


/*
 * Item Variation Store
 */

/* ported from fonttools (class _Encoding) */
بنية delta_صف_encoding_t
{
  /* each byte represents a region, value is one of 0/1/2/4, which means bytes
   * needed for this region */
  بنية chars_t : hb_ويڪٽر_t<uint8_t>
  {
    صحيح cmp (ثابت chars_t& other) ثابت
    {
      أرجع as_array ().cmp (other.as_array ());
    }

    hb_pair_t<بنانشان, بنانشان> get_ويڪر ()
    {
      بنانشان ويڪر = 0;
      بنانشان columns = 0;
      لكل (بنانشان i = 0; i < ڊيگهائي; i++)
      {
	بنانشان v = arrayZ[i];
	ويڪر += v;
	columns += (v != 0);
      }
      أرجع hb_pair (ويڪر, columns);
    }

    HB_HOT
    hb_pair_t<بنانشان, بنانشان> combine_ويڪر (ثابت chars_t& other) ثابت
    {
      بنانشان combined_ويڪر = 0;
      بنانشان combined_columns = 0;
      لكل (بنانشان i = 0; i < ڊيگهائي; i++)
      {
        بنانشان v = hb_max (arrayZ[i], other.arrayZ[i]);
	combined_ويڪر += v;
	combined_columns += (v != 0);
      }
      أرجع hb_pair (combined_ويڪر, combined_columns);
    }
  };

  hb_pair_t<بنانشان, بنانشان> combine_ويڪر (ثابت delta_صف_encoding_t& other_encoding) ثابت { أرجع chars.combine_ويڪر (other_encoding.chars); }

  // Actual data

  chars_t chars;
  بنانشان ويڪر = 0;
  بنانشان overhead = 0;
  hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح>*> شيون;

  delta_صف_encoding_t () = افتراضي;
  delta_صف_encoding_t (hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح>*> &&صفون, بنانشان انگ_کانن_ڳڻپ)
  {
    أكد (صفون);

    شيون = std::move (صفون);

    إذا (unlikely (!chars.resize (انگ_کانن_ڳڻپ)))
      أرجع;

    calculate_chars ();
  }

  فراغ ملايو (ثابت delta_صف_encoding_t& other)
  {
    شيون.alloc (شيون.ڊيگهائي + other.شيون.ڊيگهائي);
    لكل (خودڪار &صف : other.شيون)
      جوڙيو_صف (صف);

    // Merge chars
    أكد (chars.ڊيگهائي == other.chars.ڊيگهائي);
    لكل (بنانشان i = 0; i < chars.ڊيگهائي; i++)
      chars.arrayZ[i] = hb_max (chars.arrayZ[i], other.chars.arrayZ[i]);
    chars_بدليو ();
  }

  فراغ chars_بدليو ()
  {
    خودڪار _ = chars.get_ويڪر ();
    ويڪر = _.پهريون;
    overhead = get_chars_overhead (_.second);
  }

  فراغ calculate_chars ()
  {
    أكد (شيون);

    منطقي long_لفظ = خطأ;

    لكل (خودڪار &صف : شيون)
    {
      أكد (صف->ڊيگهائي == chars.ڊيگهائي);

      /* 0/1/2 byte encoding */
      لكل (بنانشان i = 0; i < صف->ڊيگهائي; i++)
      {
	صحيح v =  صف->arrayZ[i];
	إذا (v == 0)
	  استمر;
	وإلا إذا (v > 32767 || v < -32768)
	{
	  long_لفظ = سچ;
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 4);
	}
	وإلا إذا (v > 127 || v < -128)
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 2);
	وإلا
	  chars.arrayZ[i] = hb_max (chars.arrayZ[i], 1);
      }
    }

    إذا (long_لفظ)
    {
      // Convert 1s to 2s
      لكل (خودڪار &v : chars)
	إذا (v == 1)
	  v = 2;
    }

    chars_بدليو ();
  }

  منطقي is_empty () ثابت
  { أرجع !شيون; }

  منطقي اندريون_error () ثابت
  { أرجع chars.اندريون_error () || شيون.اندريون_error (); }

  ساكن مضمن بنانشان get_chars_overhead (بنانشان انگ_columns)
  {
    بنانشان c = 4 + 6; // 4 bytes for LOffset, 6 bytes for VarData header
    أرجع c + انگ_columns * 2;
  }

  بنانشان get_gain (بنانشان additional_بائيٽون_في_صفون = 1) ثابت
  {
    صحيح ڳڻپ = شيون.ڊيگهائي;
    أرجع hb_max (0, (صحيح) overhead - ڳڻپ * (صحيح) additional_بائيٽون_في_صفون);
  }

  صحيح gain_from_merging (ثابت delta_صف_encoding_t& other_encoding) ثابت
  {
    // Back of the envelope calculations to reject early.
    نشانوارو additional_بائيٽون_في_صفون = other_encoding.ويڪر - ويڪر;
    إذا (additional_بائيٽون_في_صفون > 0)
    {
      إذا (get_gain (additional_بائيٽون_في_صفون) == 0)
        أرجع 0;
    }
    وإلا
    {
      إذا (other_encoding.get_gain (-additional_بائيٽون_في_صفون) == 0)
	أرجع 0;
    }

    خودڪار pair = combine_ويڪر (other_encoding);
    بنانشان combined_ويڪر = pair.پهريون;
    بنانشان combined_columns = pair.second;

    صحيح combined_gain = (صحيح) overhead + (صحيح) other_encoding.overhead;
    combined_gain -= (combined_ويڪر - (صحيح) ويڪر) * شيون.ڊيگهائي;
    combined_gain -= (combined_ويڪر - (صحيح) other_encoding.ويڪر) * other_encoding.شيون.ڊيگهائي;
    combined_gain -= get_chars_overhead (combined_columns);

    أرجع combined_gain;
  }

  منطقي جوڙيو_صف (ثابت hb_ويڪٽر_t<صحيح>* صف)
  { أرجع شيون.push (صف); }

  ساكن صحيح cmp (ثابت فراغ *pa, ثابت فراغ *pb)
  {
    ثابت delta_صف_encoding_t *a = (ثابت delta_صف_encoding_t *)pa;
    ثابت delta_صف_encoding_t *b = (ثابت delta_صف_encoding_t *)pb;

    إذا (a->ويڪر != b->ويڪر)
      أرجع (صحيح) a->ويڪر - (صحيح) b->ويڪر;

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

    صحيح شروعات = شروعات_Coord.منزل_int (), پڇاڙي = پڇاڙي_Coord.منزل_int ();

    /* TODO Move these to sanitize(). */
    إذا (unlikely (شروعات > peak || peak > پڇاڙي))
      أرجع 1.f;
    إذا (unlikely (شروعات < 0 && پڇاڙي > 0))
      أرجع 1.f;

    إذا (coord <= شروعات || پڇاڙي <= coord)
      أرجع 0.f;

    /* Interpolate */
    إذا (coord < peak)
      أرجع عائم (coord - شروعات) / (peak - شروعات);
    وإلا
      أرجع عائم (پڇاڙي - coord) / (پڇاڙي - peak);
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
  F2DOT14	شروعات_Coord;
  F2DOT14	peakCoord;
  F2DOT14	پڇاڙي_Coord;
  عام:
  DEFINE_سائيز_STATIC (6);
};
بنية SparseVarRegionAxis
{
  عائم evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len) ثابت
  {
    بنانشان i = axis_اشاريو_2;
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
    خودڪار *ٻاهريون = c->embed (src);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    إذا (axis_map)
    {
      إذا (unlikely (!axis_map->has (ٻاهريون->axis_اشاريو_2)))
      {
	c->err (HB_SERIALIZE_ERROR_OTHER);
	return_trace (خطأ);
      }
      إذا (unlikely (!c->check_سونپيو (ٻاهريون->axis_اشاريو_2, axis_map->get (ٻاهريون->axis_اشاريو_2),
				    HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  عام:
  HBUINT16 axis_اشاريو_2;
  VarRegionAxis axis;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية hb_scalar_cache_t
{
  خاص:
  ساكن constexpr بنانشان STATIC_ڊيگهائي = 128;
  ساكن constexpr عائم INVALID = NAN;

  عام:
  hb_scalar_cache_t () {}

  hb_scalar_cache_t (ثابت hb_scalar_cache_t&) = ميٽيو;
  hb_scalar_cache_t (hb_scalar_cache_t&&) = ميٽيو;
  hb_scalar_cache_t& عامل= (ثابت hb_scalar_cache_t&) = ميٽيو;
  hb_scalar_cache_t& عامل= (hb_scalar_cache_t&&) = ميٽيو;

  ساكن hb_scalar_cache_t *create (بنانشان صحيح ڳڻپ,
				    hb_scalar_cache_t *scratch_cache = خالي_اشارو)
  {
    إذا (!ڳڻپ) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    إذا (scratch_cache && ڳڻپ <= STATIC_ڊيگهائي)
    {
      scratch_cache->ڊيگهائي = ڳڻپ;
      scratch_cache->clear ();
      أرجع scratch_cache;
    }

    خودڪار *cache = (hb_scalar_cache_t *) hb_malloc (حجم (hb_scalar_cache_t) - حجم (static_قدرون) + حجم (static_قدرون[0]) * ڳڻپ);
    إذا (unlikely (!cache)) أرجع (hb_scalar_cache_t *) &Null(hb_scalar_cache_t);

    cache->ڊيگهائي = ڳڻپ;
    cache->clear ();

    أرجع cache;
  }

  ساكن فراغ destroy (hb_scalar_cache_t *cache,
		       hb_scalar_cache_t *scratch_cache = خالي_اشارو)
  {
    إذا (cache != &Null(hb_scalar_cache_t) && cache != scratch_cache)
      hb_free (cache);
  }

  فراغ clear ()
  {
    خودڪار *values = &static_قدرون[0];
    بنانشان i = 0;
#جيڪڏهن_وصف_نه_ٿيل HB_OPTIMIZE_سائيز
    لكل (; i + 3 < ڊيگهائي; i += 4)
    {
      values[i + 0] = INVALID;
      values[i + 1] = INVALID;
      values[i + 2] = INVALID;
      values[i + 3] = INVALID;
    }
#جيڪڏهن_پورو
    لكل (; i < ڊيگهائي; i++)
      values[i] = INVALID;
  }

  HB_ALWAYS_INLINE
  منطقي get (بنانشان i, عائم *value) ثابت
  {
    إذا (unlikely (i >= ڊيگهائي))
    {
      *value = 0.f;
      أرجع سچ;
    }
    خودڪار *values = &static_قدرون[0];
    خودڪار *cached_قدر = &values[i];
    عائم v = *cached_قدر;
    إذا (std::isnan (v))
      أرجع خطأ;
    *value = v;
    أرجع سچ;
  }

  HB_ALWAYS_INLINE
  فراغ سيٽ (بنانشان i, عائم value)
  {
    إذا (unlikely (i >= ڊيگهائي)) أرجع;
    خودڪار *values = &static_قدرون[0];
    خودڪار *cached_قدر = &values[i];
    *cached_قدر = value;
  }

  عام:
  بنانشان ڊيگهائي;
  خاص:
  بدلجندڙ hb_atomic_t<عائم> static_قدرون[STATIC_ڊيگهائي];
};

بنية VarRegionList
{
  خاص:
  عائم evaluate_impl (بنانشان صحيح region_اشاريو,
		       ثابت صحيح *coords, بنانشان صحيح coord_len) ثابت
  {
    ثابت VarRegionAxis *axes = axesZ.arrayZ + (region_اشاريو * axis_ڳڻپ_2);
    عائم v = 1.f;

    بنانشان صحيح ڳڻپ = axis_ڳڻپ_2;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      صحيح coord = i < coord_len ? coords[i] : 0;
      عائم ضرب_ڪندڙ = axes[i].evaluate (coord);
      إذا (ضرب_ڪندڙ == 0.f)
      {
	v = 0.f;
	اكسر;
      }
      v *= ضرب_ڪندڙ;
    }

    أرجع v;
  }

  عام:
  HB_ALWAYS_INLINE
  عائم evaluate (بنانشان صحيح region_اشاريو,
		  ثابت صحيح *coords, بنانشان صحيح coord_len,
		  hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    إذا (unlikely (region_اشاريو >= region_ڳڻپ))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_اشاريو, &v))
      أرجع v;

    v = evaluate_impl (region_اشاريو, coords, coord_len);

    إذا (cache)
      cache->سيٽ (region_اشاريو, v);
    أرجع v;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  axesZ.sanitize (c, axis_ڳڻپ_2 * region_ڳڻپ));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  ثابت hb_ويڪٽر_t<hb_ٽيگ_t>& axis_ٽيگ_فهرست,
                  ثابت hb_ويڪٽر_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*>& regions)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان axis_ڳڻپ = axis_ٽيگ_فهرست.ڊيگهائي;
    بنانشان region_ڳڻپ_2 = regions.ڊيگهائي;
    إذا (!axis_ڳڻپ || !region_ڳڻپ_2) return_trace (خطأ);
    إذا (unlikely (hb_unsigned_mul_overflows (axis_ڳڻپ * region_ڳڻپ_2,
                                             VarRegionAxis::static_سائيز))) return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    axis_ڳڻپ_2 = axis_ڳڻپ;
    region_ڳڻپ = region_ڳڻپ_2;

    لكل (بنانشان r = 0; r < region_ڳڻپ_2; r++)
    {
      ثابت خودڪار& region = regions[r];
      لكل (بنانشان i = 0; i < axis_ڳڻپ; i++)
      {
        hb_ٽيگ_t ٽيگ_2 = axis_ٽيگ_فهرست.arrayZ[i];
        VarRegionAxis var_region_rec;
        Triple *coords;
        إذا (region->has (ٽيگ_2, &coords))
        {
          var_region_rec.شروعات_Coord.سيٽ_float (coords->minimum);
          var_region_rec.peakCoord.سيٽ_float (coords->وچ);
          var_region_rec.پڇاڙي_Coord.سيٽ_float (coords->وڌ_کان_وڌ);
        }
        وإلا
        {
          var_region_rec.شروعات_Coord.سيٽ_int (0);
          var_region_rec.peakCoord.سيٽ_int (0);
          var_region_rec.پڇاڙي_Coord.سيٽ_int (0);
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
    axis_ڳڻپ_2 = src->axis_ڳڻپ_2;
    region_ڳڻپ = region_map.get_population ();
    إذا (unlikely (hb_unsigned_mul_overflows (axis_ڳڻپ_2 * region_ڳڻپ,
					     VarRegionAxis::static_سائيز))) return_trace (خطأ);
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);
    بنانشان صحيح region_ڳڻپ_2 = src->region_ڳڻپ;
    لكل (بنانشان صحيح r = 0; r < region_ڳڻپ; r++)
    {
      بنانشان صحيح backward = region_map.backward (r);
      إذا (backward >= region_ڳڻپ_2) return_trace (خطأ);
      hb_memcpy (&axesZ[axis_ڳڻپ_2 * r], &src->axesZ[axis_ڳڻپ_2 * backward], VarRegionAxis::static_سائيز * axis_ڳڻپ_2);
    }

    return_trace (سچ);
  }

  منطقي get_var_region (بنانشان region_اشاريو,
                       ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
                       hb_hashmap_t<hb_ٽيگ_t, Triple>& axis_tuples /*ٻاهريون*/) ثابت
  {
    إذا (region_اشاريو >= region_ڳڻپ) أرجع خطأ;
    ثابت VarRegionAxis* axis_region = axesZ.arrayZ + (region_اشاريو * axis_ڳڻپ_2);
    لكل (بنانشان i = 0; i < axis_ڳڻپ_2; i++)
    {
      hb_ٽيگ_t *axis_ٽيگ_2;
      إذا (!axes_old_اشاريو_ٽيگ_map.has (i, &axis_ٽيگ_2))
        أرجع خطأ;

      عائم min_val = axis_region->شروعات_Coord.منزل_float ();
      عائم def_val = axis_region->peakCoord.منزل_float ();
      عائم max_val = axis_region->پڇاڙي_Coord.منزل_float ();

      إذا (def_val != 0.f)
        axis_tuples.سيٽ (*axis_ٽيگ_2, Triple ((مزدوج) min_val, (مزدوج) def_val, (مزدوج) max_val));
      axis_region++;
    }
    أرجع !axis_tuples.اندريون_error ();
  }

  منطقي get_var_regions (ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map,
                        hb_ويڪٽر_t<hb_hashmap_t<hb_ٽيگ_t, Triple>>& regions /*ٻاهريون*/) ثابت
  {
    إذا (!regions.alloc (region_ڳڻپ))
      أرجع خطأ;

    لكل (بنانشان i = 0; i < region_ڳڻپ; i++)
    {
      hb_hashmap_t<hb_ٽيگ_t, Triple> axis_tuples;
      إذا (!get_var_region (i, axes_old_اشاريو_ٽيگ_map, axis_tuples))
        أرجع خطأ;
      regions.push (std::move (axis_tuples));
    }
    أرجع !regions.اندريون_error ();
  }

  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_جوڙيو_saturate (min_سائيز,
				     hb_unsigned_mul_saturate (VarRegionAxis::static_سائيز,
							       axis_ڳڻپ_2, region_ڳڻپ)); }

  عام:
  HBUINT16	axis_ڳڻپ_2;
  HBUINT15	region_ڳڻپ;
  محمي:
  Unsized_Array_جو<VarRegionAxis>
		axesZ;
  عام:
  DEFINE_سائيز_ARRAY (4, axesZ);
};

/* Whether a variation region is unreachable given per-axis reachable
 * final-coordinate ranges, in F2Dot14 units (avar2 partial-instancing
 * culling; see hb_subset_plan_t::avar2_reachable_ranges). A region is dead
 * if some axis's tent evaluates to zero over the axis's entire reachable
 * range. Mirrors the runtime evaluation: invalid tents evaluate as
 * constant 1 and never kill a region. */
ساكن مضمن منطقي
_hb_avar2_region_is_dead (ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> &axis_tuples,
			  ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> &reachable_ranges)
{
  لكل (خودڪار _ : axis_tuples)
  {
    Triple *range;
    إذا (!reachable_ranges.has (_.پهريون, &range)) استمر;
    ثابت Triple &tent = _.second;
    صحيح شروعات = (صحيح) roundf ((عائم) tent.minimum * 16384.f);
    صحيح peak  = (صحيح) roundf ((عائم) tent.وچ * 16384.f);
    صحيح پڇاڙي   = (صحيح) roundf ((عائم) tent.وڌ_کان_وڌ * 16384.f);
    إذا (!peak) استمر;
    إذا (شروعات > peak || peak > پڇاڙي) استمر;
    إذا (شروعات < 0 && پڇاڙي > 0) استمر;
    صحيح lo = (صحيح) roundf ((عائم) range->minimum * 16384.f);
    صحيح hi = (صحيح) roundf ((عائم) range->وڌ_کان_وڌ * 16384.f);
    /* Strict comparisons: the scalar is 1 (not 0) at v == peak, so a tent
     * whose peak sits exactly on the range boundary is still live there.
     * (Reachable ranges are padded by one F2Dot14 unit at their producer,
     * which already covers this; strictness keeps the predicate correct on
     * its own terms.) */
    إذا (hi < شروعات || lo > پڇاڙي)
      أرجع سچ;
  }
  أرجع خطأ;
}

بنية SparseVariationRegion
{
  عائم evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len) ثابت
  {
    عائم v = 1.f;
    بنانشان صحيح ڳڻپ = axes.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      عائم ضرب_ڪندڙ = (هذا+axes[i]).evaluate (coords, coord_len);
      إذا (ضرب_ڪندڙ == 0.f)
	أرجع 0.;
      v *= ضرب_ڪندڙ;
    }
    أرجع v;
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت SparseVariationRegion *src,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!axes.serialize (c, (بنانشان) src->axes.len))) return_trace (خطأ);
    لكل (بنانشان i = 0; i < axes.len; i++)
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
  Array16_جو<Offset32_منزل<SparseVarRegionAxis>> axes;
  عام:
  DEFINE_سائيز_ARRAY (2, axes);
};

بنية SparseVarRegionList
{
  HB_ALWAYS_INLINE
  عائم evaluate (بنانشان صحيح region_اشاريو,
		  ثابت صحيح *coords, بنانشان صحيح coord_len,
		  hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    إذا (unlikely (region_اشاريو >= regions.len))
      أرجع 0.;

    عائم v;
    إذا (cache && cache->get (region_اشاريو, &v))
      أرجع v;

    ثابت SparseVariationRegion &region = هذا+regions[region_اشاريو];

    v = region.evaluate (coords, coord_len);
    إذا (cache)
      cache->سيٽ (region_اشاريو, v);

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

    لكل (بنانشان i = 0; i < regions.len; i++)
    {
      بنانشان old_اشاريو = region_map.backward (i);
      إذا (unlikely (old_اشاريو >= src->regions.len ||
		    !regions[i].serialize_serialize (c,
					     &(src+src->regions[old_اشاريو]),
					     axis_map)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  عام:
  Array32_جو<Offset32_منزل<SparseVariationRegion>>
		regions;
  عام:
  DEFINE_سائيز_ARRAY (4, regions);
};


بنية Var_معلومات
{
  بنانشان صحيح get_شي_ڳڻپ () ثابت
  { أرجع شي_ڳڻپ; }

  بنانشان صحيح get_region_اشاريو_ڳڻپ () ثابت
  { أرجع regionIndices.len; }

  بنانشان get_region_اشاريو (بنانشان i) ثابت
  { أرجع i >= regionIndices.len ? -1 : regionIndices[i]; }

  size_t get_صف_سائيز () ثابت
  { أرجع (لفظ_ڳڻپ () + regionIndices.len) * (long_لفظ_2 () ? 2 : 1); }

  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_جوڙيو_saturate (min_سائيز - regionIndices.min_سائيز,
				     regionIndices.get_سائيز (),
				     hb_unsigned_mul_saturate (شي_ڳڻپ, get_صف_سائيز ())); }

  سانچو <قسم_نالو Float = عائم>
  Float _get_delta (بنانشان صحيح inner,
		    ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		    ثابت VarRegionList &regions,
		    hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    إذا (unlikely (inner >= شي_ڳڻپ))
      أرجع 0.;
    منطقي is_long = long_لفظ_2 ();
    بنانشان صحيح ڳڻپ = regionIndices.len;
    بنانشان لفظ_ڳڻپ_2 = لفظ_ڳڻپ ();
    بنانشان صحيح scount = is_long ? ڳڻپ : لفظ_ڳڻپ_2;
    بنانشان صحيح lcount = is_long ? لفظ_ڳڻپ_2 : 0;

    ثابت HBUINT8 *بائيٽون = get_delta_بائيٽون ();
    ثابت HBUINT8 *صف = بائيٽون + inner * get_صف_سائيز ();

    Float delta = 0;
    بنانشان صحيح i = 0;

    ثابت HBINT32 *lcursor = reinterpret_cast<ثابت HBINT32 *> (صف);
    لكل (; i < lcount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ڳڻپ, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *lcursor;
      lcursor++;
    }
    ثابت HBINT16 *scursor = reinterpret_cast<ثابت HBINT16 *> (lcursor);
    لكل (; i < scount; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ڳڻپ, cache);
      إذا (scalar)
       delta += (Float) scalar * (صحيح) *scursor;
      scursor++;
    }
    ثابت HBINT8 *bcursor = reinterpret_cast<ثابت HBINT8 *> (scursor);
    لكل (; i < ڳڻپ; i++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ڳڻپ, cache);
      إذا (scalar)
        delta += (Float) scalar * (صحيح) *bcursor;
      bcursor++;
    }

    أرجع delta;
  }

  سانچو <قسم_نالو Float = عائم>
  HB_ALWAYS_INLINE
  Float get_delta (بنانشان صحيح inner,
		   ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		   ثابت VarRegionList &regions,
		   hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    بنانشان صحيح ڳڻپ = regionIndices.len;
    إذا (!ڳڻپ) أرجع Float (0); // This is quite common, so optimize it.
    أرجع _get_delta<Float> (inner, coords, coord_ڳڻپ, regions, cache);
  }

  فراغ get_region_scalars (ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
			   ثابت VarRegionList &regions,
			   عائم *scalars /*OUT */,
			   بنانشان صحيح انگ_scalars) ثابت
  {
    بنانشان ڳڻپ = hb_min (انگ_scalars, regionIndices.len);
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      scalars[i] = regions.evaluate (regionIndices.arrayZ[i], coords, coord_ڳڻپ);
    لكل (بنانشان صحيح i = ڳڻپ; i < انگ_scalars; i++)
      scalars[i] = 0.f;
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  regionIndices.sanitize (c) &&
		  hb_barrier () &&
		  لفظ_ڳڻپ () <= regionIndices.len &&
		  c->check_range (get_delta_بائيٽون (),
				  شي_ڳڻپ,
				  get_صف_سائيز ()));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_ويڪٽر_t<ثابت hb_ويڪٽر_t<صحيح>*>& صفون)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان صف_ڳڻپ_3 = صفون.ڊيگهائي;
    إذا (!صف_ڳڻپ_3) {
      // Nothing to serialize, will be empty.
      أرجع خطأ;
    }

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!c->check_سونپيو (شي_ڳڻپ, صف_ڳڻپ_3,
                                    HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
      return_trace (خطأ);

    صحيح min_حد = has_long ? -32768 : -128;
    صحيح max_حد = has_long ? +32767 : +127;
    تعداد delta_سائيز_t { kZero=0, k_Non_لفظ, k_لفظ };
    hb_ويڪٽر_t<delta_سائيز_t> delta_sz;
    بنانشان انگ_regions = صفون[0]->ڊيگهائي;
    إذا (!delta_sz.resize (انگ_regions))
      return_trace (خطأ);

    بنانشان لفظ_ڳڻپ_2 = 0;
    لكل (بنانشان r = 0; r < انگ_regions; r++)
    {
      لكل (بنانشان i = 0; i < صف_ڳڻپ_3; i++)
      {
        صحيح delta = صفون[i]->arrayZ[r];
        إذا (delta < min_حد || delta > max_حد)
        {
          delta_sz[r] = k_لفظ;
          لفظ_ڳڻپ_2++;
          اكسر;
        }
        وإلا إذا (delta != 0)
        {
          delta_sz[r] = k_Non_لفظ;
        }
      }
    }

    /* reorder regions: words and then non-words*/
    بنانشان لفظ_اشاريو = 0;
    بنانشان non_لفظ_اشاريو = لفظ_ڳڻپ_2;
    hb_map_t ri_map;
    لكل (بنانشان r = 0; r < انگ_regions; r++)
    {
      إذا (!delta_sz[r]) استمر;
      بنانشان new_r = (delta_sz[r] == k_لفظ)? لفظ_اشاريو++ : non_لفظ_اشاريو++;
      إذا (!ri_map.سيٽ (new_r, r))
        return_trace (خطأ);
    }

    لفظ_سائيز_ڳڻپ = لفظ_ڳڻپ_2 | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    بنانشان ri_ڳڻپ = ri_map.get_population ();
    regionIndices.len = ri_ڳڻپ;
    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (بنانشان r = 0; r < ri_ڳڻپ; r++)
    {
      hb_codepoint_t *idx;
      إذا (!ri_map.has (r, &idx))
        return_trace (خطأ);
      regionIndices[r] = *idx;
    }

    HBUINT8 *delta_بائيٽون = get_delta_بائيٽون ();
    بنانشان صف_سائيز = get_صف_سائيز ();
    لكل (بنانشان صحيح i = 0; i < صف_ڳڻپ_3; i++)
    {
      لكل (بنانشان صحيح r = 0; r < ri_ڳڻپ; r++)
      {
        صحيح delta = صفون[i]->arrayZ[ri_map[r]];
        سيٽ_شي_delta_fast (i, r, delta, delta_بائيٽون, صف_سائيز);
      }
    }
    return_trace (سچ);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Var_معلومات *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map,
		  ثابت hb_سيٽ_t *culled_regions = خالي_اشارو)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    شي_ڳڻپ = inner_map.get_ايندڙ_قدر ();

    /* Optimize word count */
    بنانشان ri_ڳڻپ = src->regionIndices.len;
    تعداد delta_سائيز_t { kZero=0, k_Non_لفظ, k_لفظ };
    hb_ويڪٽر_t<delta_سائيز_t> delta_sz;
    hb_ويڪٽر_t<بنانشان صحيح> ri_map;	/* maps new index to old index */
    delta_sz.resize (ri_ڳڻپ);
    ri_map.resize (ri_ڳڻپ);
    بنانشان صحيح new_لفظ_ڳڻپ = 0;
    بنانشان صحيح r;

    ثابت HBUINT8 *src_delta_بائيٽون = src->get_delta_بائيٽون ();
    بنانشان src_صف_سائيز = src->get_صف_سائيز ();
    بنانشان src_لفظ_ڳڻپ = src->لفظ_ڳڻپ ();
    منطقي     src_long_لفظ = src->long_لفظ_2 ();

    منطقي has_long = خطأ;
    إذا (src_long_لفظ)
    {
      لكل (r = 0; r < src_لفظ_ڳڻپ; r++)
      {
        لكل (بنانشان old_gid : inner_map.ڪنجيون())
	{
	  int32_t delta = src->get_شي_delta_fast (old_gid, r, src_delta_بائيٽون, src_صف_سائيز);
	  إذا (delta < -32768 || 32767 < delta)
	  {
	    has_long = سچ;
	    اكسر;
	  }
        }
      }
    }

    نشانوارو min_حد = has_long ? -32768 : -128;
    نشانوارو max_حد = has_long ? +32767 : +127;
    لكل (r = 0; r < ri_ڳڻپ; r++)
    {
      منطقي short_circuit = src_long_لفظ == has_long && src_لفظ_ڳڻپ <= r;

      delta_sz[r] = kZero;
      إذا (culled_regions && culled_regions->has (src->regionIndices[r]))
	استمر; /* unreachable region: drop its column */
      لكل (بنانشان old_gid : inner_map.ڪنجيون())
      {
	int32_t delta = src->get_شي_delta_fast (old_gid, r, src_delta_بائيٽون, src_صف_سائيز);
	إذا (delta < min_حد || max_حد < delta)
	{
	  delta_sz[r] = k_لفظ;
	  new_لفظ_ڳڻپ++;
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

    بنانشان صحيح لفظ_اشاريو = 0;
    بنانشان صحيح non_لفظ_اشاريو = new_لفظ_ڳڻپ;
    بنانشان صحيح new_ri_ڳڻپ = 0;
    لكل (r = 0; r < ri_ڳڻپ; r++)
      إذا (delta_sz[r])
      {
	بنانشان new_r = (delta_sz[r] == k_لفظ)? لفظ_اشاريو++ : non_لفظ_اشاريو++;
	ri_map[new_r] = r;
	new_ri_ڳڻپ++;
      }

    لفظ_سائيز_ڳڻپ = new_لفظ_ڳڻپ | (has_long ? 0x8000u /* LONG_WORDS */ : 0);

    regionIndices.len = new_ri_ڳڻپ;

    إذا (unlikely (!c->extend (هذا))) return_trace (خطأ);

    لكل (r = 0; r < new_ri_ڳڻپ; r++)
      regionIndices[r] = region_map[src->regionIndices[ri_map[r]]];

    HBUINT8 *delta_بائيٽون = get_delta_بائيٽون ();
    بنانشان صف_سائيز = get_صف_سائيز ();
    بنانشان ڳڻپ = شي_ڳڻپ;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      بنانشان صحيح old = inner_map.backward (i);
      لكل (بنانشان صحيح r = 0; r < new_ri_ڳڻپ; r++)
	سيٽ_شي_delta_fast (i, r,
			     src->get_شي_delta_fast (old, ri_map[r],
						       src_delta_بائيٽون, src_صف_سائيز),
			     delta_بائيٽون, صف_سائيز);
    }

    return_trace (سچ);
  }

  فراغ collect_region_refs (hb_سيٽ_t &region_indices, ثابت hb_inc_bimap_t &inner_map) ثابت
  {
    ثابت HBUINT8 *delta_بائيٽون = get_delta_بائيٽون ();
    بنانشان صف_سائيز = get_صف_سائيز ();

    لكل (بنانشان صحيح r = 0; r < regionIndices.len; r++)
    {
      بنانشان صحيح region = regionIndices.arrayZ[r];
      إذا (region_indices.has (region)) استمر;
      لكل (hb_codepoint_t old_gid : inner_map.ڪنجيون())
	إذا (get_شي_delta_fast (old_gid, r, delta_بائيٽون, صف_سائيز) != 0)
	{
	  region_indices.جوڙيو (region);
	  اكسر;
	}
    }
  }

  عام:
  ثابت HBUINT8 *get_delta_بائيٽون () ثابت
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  محمي:
  HBUINT8 *get_delta_بائيٽون ()
  { أرجع &StructAfter<HBUINT8> (regionIndices); }

  عام:
  int32_t get_شي_delta_fast (بنانشان صحيح شي, بنانشان صحيح region,
			       ثابت HBUINT8 *delta_بائيٽون, بنانشان صف_سائيز) ثابت
  {
    إذا (unlikely (شي >= شي_ڳڻپ || region >= regionIndices.len)) أرجع 0;

    ثابت HBINT8 *p = (ثابت HBINT8 *) delta_بائيٽون + شي * صف_سائيز;
    بنانشان لفظ_ڳڻپ_2 = لفظ_ڳڻپ ();
    منطقي is_long = long_لفظ_2 ();
    إذا (is_long)
    {
      إذا (region < لفظ_ڳڻپ_2)
	أرجع ((ثابت HBINT32 *) p)[region];
      وإلا
	أرجع ((ثابت HBINT16 *)(p + HBINT32::static_سائيز * لفظ_ڳڻپ_2))[region - لفظ_ڳڻپ_2];
    }
    وإلا
    {
      إذا (region < لفظ_ڳڻپ_2)
	أرجع ((ثابت HBINT16 *) p)[region];
      وإلا
	أرجع (p + HBINT16::static_سائيز * لفظ_ڳڻپ_2)[region - لفظ_ڳڻپ_2];
    }
  }
  int32_t get_شي_delta (بنانشان صحيح شي, بنانشان صحيح region) ثابت
  {
     أرجع get_شي_delta_fast (شي, region,
				 get_delta_بائيٽون (),
				 get_صف_سائيز ());
  }

  محمي:
  فراغ سيٽ_شي_delta_fast (بنانشان صحيح شي, بنانشان صحيح region, int32_t delta,
			    HBUINT8 *delta_بائيٽون, بنانشان صف_سائيز)
  {
    HBINT8 *p = (HBINT8 *) delta_بائيٽون + شي * صف_سائيز;
    بنانشان لفظ_ڳڻپ_2 = لفظ_ڳڻپ ();
    منطقي is_long = long_لفظ_2 ();
    إذا (is_long)
    {
      إذا (region < لفظ_ڳڻپ_2)
	((HBINT32 *) p)[region] = delta;
      وإلا
	((HBINT16 *)(p + HBINT32::static_سائيز * لفظ_ڳڻپ_2))[region - لفظ_ڳڻپ_2] = delta;
    }
    وإلا
    {
      إذا (region < لفظ_ڳڻپ_2)
	((HBINT16 *) p)[region] = delta;
      وإلا
	(p + HBINT16::static_سائيز * لفظ_ڳڻپ_2)[region - لفظ_ڳڻپ_2] = delta;
    }
  }
  فراغ سيٽ_شي_delta (بنانشان صحيح شي, بنانشان صحيح region, int32_t delta)
  {
    سيٽ_شي_delta_fast (شي, region, delta,
			 get_delta_بائيٽون (),
			 get_صف_سائيز ());
  }

  منطقي long_لفظ_2 () ثابت { أرجع لفظ_سائيز_ڳڻپ & 0x8000u /* LONG_WORDS */; }
  بنانشان لفظ_ڳڻپ () ثابت { أرجع لفظ_سائيز_ڳڻپ & 0x7FFFu /* WORD_DELTA_COUNT_MASK */; }

  محمي:
  HBUINT16		شي_ڳڻپ;
  HBUINT16		لفظ_سائيز_ڳڻپ;
  Array16_جو<HBUINT16>	regionIndices;
/*UnsizedArrayOf<HBUINT8>bytesX;*/
  عام:
  DEFINE_سائيز_ARRAY (6, regionIndices);
};

بنية Multi_Var_معلومات
{
  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_جوڙيو_saturate (min_سائيز - regionIndices.min_سائيز,
				     regionIndices.get_سائيز ()); }

  ثابت TupleList &get_delta_sets () ثابت
  { أرجع هذا+StructAfter<decltype (deltaSetsX)> (regionIndices); }

  فراغ get_delta (بنانشان صحيح inner,
		  ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		  ثابت SparseVarRegionList &regions,
		  hb_array_t<مزدوج> ٻاهريون,
		  hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    خودڪار &deltaSets = get_delta_sets ();

    خودڪار قدرون_iter = deltaSets.fetcher (inner);
    بنانشان region_ڳڻپ = regionIndices.len;
    بنانشان skip = 0;
    لكل (بنانشان region_اشاريو_2 = 0; region_اشاريو_2 < region_ڳڻپ; region_اشاريو_2++)
    {
      عائم scalar = regions.evaluate (regionIndices.arrayZ[region_اشاريو_2],
				       coords, coord_ڳڻپ,
				       cache);
      // We skip lazily. Helps with the tail end.
      إذا (scalar == 0.0f)
        skip += ٻاهريون.ڊيگهائي;
      وإلا
      {
        إذا (skip)
	{
	  قدرون_iter.skip (skip);
	  skip = 0;
	}
	قدرون_iter.جوڙيو_منزل_2 (ٻاهريون, scalar);
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

  بنانشان get_شي_ڳڻپ () ثابت
  { أرجع get_delta_sets ().ڳڻپ; }

  فراغ collect_region_refs (hb_سيٽ_t &region_indices) ثابت
  {
    لكل (بنانشان region : regionIndices)
      region_indices.جوڙيو (region);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_Var_معلومات *src,
		  ثابت hb_inc_bimap_t &inner_map,
		  ثابت hb_inc_bimap_t &region_map)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان region_ڳڻپ_2 = src->regionIndices.len;
    بنانشان سرو_سائيز = HBUINT8::static_سائيز +
			   Array16_جو<HBUINT16>::min_سائيز +
			   region_ڳڻپ_2 * HBUINT16::static_سائيز +
			   HBUINT32::static_سائيز;
    إذا (unlikely (!c->extend_سائيز (هذا, سرو_سائيز, خطأ)))
      return_trace (خطأ);
    format = 1;
    regionIndices.len = region_ڳڻپ_2;
    لكل (بنانشان i = 0; i < regionIndices.len; i++)
    {
      بنانشان old_region = src->regionIndices[i];
      إذا (unlikely (!region_map.has (old_region)))
	return_trace (خطأ);
      regionIndices[i] = region_map.get (old_region);
    }

    ثابت خودڪار &src_delta_sets = src->get_delta_sets ();
    hb_ويڪٽر_t<hb_ubytes_t> delta_sets;
    بنانشان معلومات_سائيز = 0;
    إذا (unlikely (!delta_sets.alloc_exact (inner_map.get_population ())))
      return_trace (خطأ);
    لكل (بنانشان i = 0; i < inner_map.get_population (); i++)
    {
      بنانشان old_inner = inner_map.backward (i);
      إذا (unlikely (old_inner >= src_delta_sets.ڳڻپ)) return_trace (خطأ);
      hb_ubytes_t بائيٽون = static_cast<ثابت CFF2_اشاريو &> (src_delta_sets)[old_inner];
      معلومات_سائيز = hb_unsigned_جوڙيو_saturate (معلومات_سائيز, بائيٽون.ڊيگهائي);
      delta_sets.push (بائيٽون);
    }
    إذا (unlikely (معلومات_سائيز == UINT_MAX || delta_sets.اندريون_error ()))
      return_trace (خطأ);

    خودڪار &ٻاهريون_delta_sets = StructAfter<decltype (deltaSetsX)> (regionIndices);
    return_trace (ٻاهريون_delta_sets.serialize_serialize (c, delta_sets.iter (), &معلومات_سائيز));
  }

  محمي:
  HBUINT8	      format; // 1
  Array16_جو<HBUINT16> regionIndices;
  Offset32_منزل<TupleList> deltaSetsX;
  عام:
  DEFINE_سائيز_MIN (7);
};

بنية شي_Variation_Store
{
  دوست بنية شي_variations_t;

  hb_scalar_cache_t *create_cache () ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#جيڪڏهن_پورو
    أرجع hb_scalar_cache_t::create ((هذا+regions).region_ڳڻپ);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache)
  {
    hb_scalar_cache_t::destroy (cache);
  }

  خاص:
  سانچو <قسم_نالو Float = عائم>
  Float get_delta (بنانشان صحيح outer, بنانشان صحيح inner,
		   ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		   hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع Float (0);
#جيڪڏهن_پورو

    إذا (unlikely (outer >= معلومات_Sets.len))
      أرجع Float (0);

    أرجع (هذا+معلومات_Sets[outer]).get_delta<Float> (inner,
					     coords, coord_ڳڻپ,
					     هذا+regions,
					     cache);
  }

  عام:
  منطقي has_delta_سيٽ (بنانشان صحيح اشاريو_2) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع خطأ;
#جيڪڏهن_پورو

    بنانشان صحيح outer = اشاريو_2 >> 16;
    بنانشان صحيح inner = اشاريو_2 & 0xFFFF;
    أرجع outer < معلومات_Sets.len &&
	   inner < (هذا+معلومات_Sets[outer]).get_شي_ڳڻپ ();
  }

  سانچو <قسم_نالو Float = عائم>
  Float get_delta (بنانشان صحيح اشاريو_2,
		   ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		   hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    بنانشان صحيح outer = اشاريو_2 >> 16;
    بنانشان صحيح inner = اشاريو_2 & 0xFFFF;
    أرجع get_delta<Float> (outer, inner, coords, coord_ڳڻپ, cache);
  }
  سانچو <قسم_نالو Float = عائم>
  Float get_delta (بنانشان صحيح اشاريو_2,
		   hb_array_t<ثابت صحيح> coords,
		   hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    أرجع get_delta<Float> (اشاريو_2,
		      coords.arrayZ, coords.ڊيگهائي,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع سچ;
#جيڪڏهن_پورو

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  معلومات_Sets.sanitize (c, هذا));
  }

  منطقي serialize (hb_serialize_context_t *c,
                  منطقي has_long,
                  ثابت hb_ويڪٽر_t<hb_ٽيگ_t>& axis_ٽيگ_فهرست,
                  ثابت hb_ويڪٽر_t<ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>*>& region_list,
                  ثابت hb_ويڪٽر_t<delta_صف_encoding_t>& vardata_encodings)
  {
    TRACE_SERIALIZE (هذا);
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    return_trace (خطأ);
#جيڪڏهن_پورو
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    format = 1;
    إذا (!regions.serialize_serialize (c, axis_ٽيگ_فهرست, region_list))
      return_trace (خطأ);

    بنانشان انگ_var_معلومات = vardata_encodings.ڊيگهائي;
    إذا (!انگ_var_معلومات) return_trace (خطأ);
    إذا (unlikely (!c->check_سونپيو (معلومات_Sets.len, انگ_var_معلومات,
                                    HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
      return_trace (خطأ);

    إذا (unlikely (!c->extend (معلومات_Sets))) return_trace (خطأ);
    لكل (بنانشان i = 0; i < انگ_var_معلومات; i++)
      إذا (!معلومات_Sets[i].serialize_serialize (c, has_long, vardata_encodings[i].شيون))
        return_trace (خطأ);

    return_trace (سچ);
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت شي_Variation_Store *src,
		  ثابت hb_array_t <ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_سيٽ_t *culled_regions = خالي_اشارو)
  {
    TRACE_SERIALIZE (هذا);
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    return_trace (خطأ);
#جيڪڏهن_پورو

    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    بنانشان صحيح سيٽ_ڳڻپ = 0;
    لكل (بنانشان صحيح i = 0; i < inner_maps.ڊيگهائي; i++)
      إذا (inner_maps[i].get_population ())
	سيٽ_ڳڻپ++;

    format = 1;

    ثابت خودڪار &src_regions = src+src->regions;

    hb_سيٽ_t region_indices;
    لكل (بنانشان صحيح i = 0; i < inner_maps.ڊيگهائي; i++)
      (src+src->معلومات_Sets[i]).collect_region_refs (region_indices, inner_maps[i]);

    إذا (region_indices.اندريون_error ())
      return_trace (خطأ);

    region_indices.del_range ((src_regions).region_ڳڻپ, hb_سيٽ_t::INVALID);

    إذا (culled_regions)
      region_indices.ڪٽيو (*culled_regions);

    /* TODO use constructor when our data-structures support that. */
    hb_inc_bimap_t region_map;
    + hb_iter (region_indices)
    | hb_apply ([&region_map] (بنانشان _) { region_map.جوڙيو(_); })
    ;
    إذا (region_map.اندريون_error())
      return_trace (خطأ);

    إذا (unlikely (!regions.serialize_serialize (c, &src_regions, region_map)))
      return_trace (خطأ);

    معلومات_Sets.len = سيٽ_ڳڻپ;
    إذا (unlikely (!c->extend (معلومات_Sets))) return_trace (خطأ);

    /* TODO: The following code could be simplified when
     * List16OfOffset16To::subset () can take a custom param to be passed to VarData::serialize () */
    بنانشان صحيح سيٽ_اشاريو = 0;
    لكل (بنانشان صحيح i = 0; i < inner_maps.ڊيگهائي; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!معلومات_Sets[سيٽ_اشاريو++]
		     .serialize_serialize (c, &(src+src->معلومات_Sets[i]), inner_maps[i], region_map,
					   culled_regions)))
	return_trace (خطأ);
    }

    return_trace (سچ);
  }

  شي_Variation_Store *copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->شروعات_embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خالي_اشارو);

    hb_ويڪٽر_t <hb_inc_bimap_t> inner_maps;
    بنانشان ڳڻپ = معلومات_Sets.len;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      hb_inc_bimap_t *map = inner_maps.push ();
      إذا (unlikely (!c->propagate_error(inner_maps)))
        return_trace(خالي_اشارو);
      خودڪار &معلومات = هذا+معلومات_Sets[i];

      بنانشان شي_ڳڻپ = معلومات.get_شي_ڳڻپ ();
      لكل (بنانشان j = 0; j < شي_ڳڻپ; j++)
	map->جوڙيو (j);
    }

    إذا (unlikely (!ٻاهريون->serialize (c, هذا, inner_maps))) return_trace (خالي_اشارو);

    return_trace (ٻاهريون);
  }

  /* Collect regions that a partial avar2 instance can never reach, per the
   * plan's reachable final-coordinate ranges. */
  منطقي collect_dead_regions (ثابت hb_map_t &axes_old_اشاريو_ٽيگ_map,
			     ثابت hb_hashmap_t<hb_ٽيگ_t, Triple> &reachable_ranges,
			     hb_سيٽ_t &dead_regions /*ٻاهريون*/) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (!reachable_ranges.get_population ()) أرجع سچ;
    ثابت VarRegionList &region_list = هذا+regions;
    بنانشان ڳڻپ = region_list.region_ڳڻپ;
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      hb_hashmap_t<hb_ٽيگ_t, Triple> axis_tuples;
      إذا (!region_list.get_var_region (i, axes_old_اشاريو_ٽيگ_map, axis_tuples))
	أرجع خطأ;
      إذا (_hb_avar2_region_is_dead (axis_tuples, reachable_ranges))
	dead_regions.جوڙيو (i);
    }
    أرجع !dead_regions.اندريون_error ();
#وإلا
    أرجع سچ;
#جيڪڏهن_پورو
  }

  منطقي subset (hb_subset_context_t *c, ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps) ثابت
  {
    TRACE_SUBSET (هذا);
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    return_trace (خطأ);
#جيڪڏهن_پورو

    شي_Variation_Store *varstore_مفرد = c->serializer->شروعات_embed<شي_Variation_Store> ();
    إذا (unlikely (!varstore_مفرد)) return_trace (خطأ);

    /* avar2 partial instancing: cull unreachable regions. */
    hb_سيٽ_t dead_regions;
    إذا (c->plan->has_avar2)
      إذا (!collect_dead_regions (c->plan->axes_old_اشاريو_ٽيگ_map,
				 c->plan->avar2_reachable_ranges,
				 dead_regions))
	return_trace (خطأ);

    varstore_مفرد->serialize (c->serializer, هذا, inner_maps,
			       dead_regions.get_population () ? &dead_regions : خالي_اشارو);

    return_trace (
        !c->serializer->اندريون_error()
        && varstore_مفرد->معلومات_Sets);
  }

  بنانشان صحيح get_region_اشاريو_ڳڻپ (بنانشان صحيح major) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع 0;
#جيڪڏهن_پورو
    أرجع (هذا+معلومات_Sets[major]).get_region_اشاريو_ڳڻپ ();
  }

  فراغ get_region_scalars (بنانشان صحيح major,
			   ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
			   عائم *scalars /*OUT*/,
			   بنانشان صحيح انگ_scalars) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    لكل (بنانشان i = 0; i < انگ_scalars; i++)
      scalars[i] = 0.f;
    أرجع;
#جيڪڏهن_پورو

    (هذا+معلومات_Sets[major]).get_region_scalars (coords, coord_ڳڻپ,
					       هذا+regions,
					       &scalars[0], انگ_scalars);
  }

  بنانشان صحيح get_sub_جدول_ڳڻپ () ثابت
   {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
     أرجع 0;
#جيڪڏهن_پورو
     أرجع معلومات_Sets.len;
   }

  ثابت Var_معلومات& get_sub_جدول (بنانشان i) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
     أرجع Null (Var_معلومات);
#جيڪڏهن_پورو
     أرجع هذا+معلومات_Sets[i];
  }

  ثابت VarRegionList& get_region_list () ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
     أرجع Null (VarRegionList);
#جيڪڏهن_پورو
     أرجع هذا+regions;
  }

  محمي:
  HBUINT16				format;
  Offset32_منزل<VarRegionList>		regions;
  Array16_جو_Offset32_منزل<Var_معلومات>		معلومات_Sets;
  عام:
  DEFINE_سائيز_ARRAY_SIZED (8, معلومات_Sets);
};

بنية Multi_شي_Variation_Store
{
  hb_scalar_cache_t *create_cache (hb_scalar_cache_t *static_cache = خالي_اشارو) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع hb_scalar_cache_t::create (0);
#جيڪڏهن_پورو
    خودڪار &r = هذا+regions;
    بنانشان ڳڻپ = r.regions.len;

    أرجع hb_scalar_cache_t::create (ڳڻپ, static_cache);
  }

  ساكن فراغ destroy_cache (hb_scalar_cache_t *cache,
			     hb_scalar_cache_t *static_cache = خالي_اشارو)
  {
    hb_scalar_cache_t::destroy (cache, static_cache);
  }

  خاص:
  فراغ get_delta (بنانشان صحيح outer, بنانشان صحيح inner,
		  ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		  hb_array_t<مزدوج> ٻاهريون,
		  hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع;
#جيڪڏهن_پورو

    إذا (unlikely (outer >= معلومات_Sets.len))
      أرجع;

    أرجع (هذا+معلومات_Sets[outer]).get_delta (inner,
					     coords, coord_ڳڻپ,
					     هذا+regions,
					     ٻاهريون,
					     cache);
  }

  عام:
  فراغ get_delta (بنانشان صحيح اشاريو_2,
		  ثابت صحيح *coords, بنانشان صحيح coord_ڳڻپ,
		  hb_array_t<مزدوج> ٻاهريون,
		  hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    بنانشان صحيح outer = اشاريو_2 >> 16;
    بنانشان صحيح inner = اشاريو_2 & 0xFFFF;
    get_delta (outer, inner, coords, coord_ڳڻپ, ٻاهريون, cache);
  }
  فراغ get_delta (بنانشان صحيح اشاريو_2,
		  hb_array_t<ثابت صحيح> coords,
		  hb_array_t<مزدوج> ٻاهريون,
		  hb_scalar_cache_t *cache = خالي_اشارو) ثابت
  {
    أرجع get_delta (اشاريو_2,
		      coords.arrayZ, coords.ڊيگهائي,
		      ٻاهريون,
		      cache);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
#جيڪڏهن_وصف_ٿيل HB_NO_VAR
    أرجع سچ;
#جيڪڏهن_پورو

    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
		  format == 1 &&
		  regions.sanitize (c, هذا) &&
		  معلومات_Sets.sanitize (c, هذا));
  }

  منطقي create_subset_plan (ثابت hb_سيٽ_t &var_indices,
			   hb_ويڪٽر_t<hb_inc_bimap_t> *inner_maps,
			   hb_map_t *varidx_map) ثابت
  {
    إذا (unlikely (!inner_maps->resize (معلومات_Sets.len))) أرجع خطأ;
    لكل (hb_codepoint_t var_idx : var_indices)
    {
      بنانشان outer = var_idx >> 16;
      بنانشان inner = var_idx & 0xFFFFu;
      إذا (unlikely (outer >= معلومات_Sets.len ||
		    inner >= (هذا+معلومات_Sets[outer]).get_شي_ڳڻپ ()))
	أرجع خطأ;
      (*inner_maps)[outer].جوڙيو (inner);
    }

    بنانشان new_outer = 0;
    لكل (بنانشان outer = 0; outer < inner_maps->ڊيگهائي; outer++)
    {
      ثابت hb_inc_bimap_t &inner_map = (*inner_maps)[outer];
      إذا (!inner_map.get_population ()) استمر;
      إذا (unlikely (inner_map.اندريون_error ())) أرجع خطأ;
      لكل (بنانشان new_inner = 0;
	   new_inner < inner_map.get_population ();
	   new_inner++)
      {
	بنانشان old_inner = inner_map.backward (new_inner);
	varidx_map->سيٽ ((outer << 16) | old_inner,
			(new_outer << 16) | new_inner);
      }
      new_outer++;
    }
    أرجع !inner_maps->اندريون_error () && !varidx_map->اندريون_error ();
  }

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Multi_شي_Variation_Store *src,
		  ثابت hb_array_t<ثابت hb_inc_bimap_t> &inner_maps,
		  ثابت hb_map_t *axis_map)
  {
    TRACE_SERIALIZE (هذا);
    إذا (unlikely (!c->extend_min (هذا) ||
		  inner_maps.ڊيگهائي < src->معلومات_Sets.len))
      return_trace (خطأ);
    format = 1;

    ثابت خودڪار &src_regions = src+src->regions;
    hb_سيٽ_t region_indices;
    بنانشان سيٽ_ڳڻپ = 0;
    لكل (بنانشان i = 0; i < src->معلومات_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      سيٽ_ڳڻپ++;
      (src+src->معلومات_Sets[i]).collect_region_refs (region_indices);
    }
    region_indices.del_range (src_regions.regions.len, hb_سيٽ_t::INVALID);
    hb_inc_bimap_t region_map;
    region_map.جوڙيو_سيٽ (&region_indices);
    إذا (unlikely (region_indices.اندريون_error () || region_map.اندريون_error () ||
		  !regions.serialize_serialize (c, &src_regions, region_map, axis_map)))
      return_trace (خطأ);

    معلومات_Sets.len = سيٽ_ڳڻپ;
    إذا (unlikely (!c->extend (معلومات_Sets))) return_trace (خطأ);
    بنانشان سيٽ_اشاريو = 0;
    لكل (بنانشان i = 0; i < src->معلومات_Sets.len; i++)
    {
      إذا (!inner_maps[i].get_population ()) استمر;
      إذا (unlikely (!معلومات_Sets[سيٽ_اشاريو++].serialize_serialize (
			    c, &(src+src->معلومات_Sets[i]), inner_maps[i], region_map)))
	return_trace (خطأ);
    }
    return_trace (سچ);
  }

  محمي:
  HBUINT16				format; // 1
  Offset32_منزل<SparseVarRegionList>	regions;
  Array16_جو_Offset32_منزل<Multi_Var_معلومات>	معلومات_Sets;
  عام:
  DEFINE_سائيز_ARRAY_SIZED (8, معلومات_Sets);
};

سانچو <قسم_نالو Map_ڳڻپ_T>
بنية Delta_سيٽ_اشاريو_Map_Format01
{
  دوست بنية Delta_سيٽ_اشاريو_Map;

  size_t get_سائيز () ثابت
  { أرجع hb_unsigned_mul_جوڙيو_saturate (map_ڳڻپ_2, get_ويڪر (), min_سائيز); }

  خاص:
  Delta_سيٽ_اشاريو_Map_Format01* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (هذا));
  }

  سانچو <قسم_نالو T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    بنانشان صحيح ويڪر = plan.get_ويڪر ();
    بنانشان صحيح inner_ٻٽو_انگ_ڳڻپ = plan.get_inner_ٻٽو_انگ_ڳڻپ ();
    ثابت hb_array_t<ثابت uint32_t> output_map = plan.get_output_map ();

    TRACE_SERIALIZE (هذا);
    إذا (unlikely (output_map.ڊيگهائي && ((((inner_ٻٽو_انگ_ڳڻپ-1)&~0xF)!=0) || (((ويڪر-1)&~0x3)!=0))))
      return_trace (خطأ);
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);

    داخلا_Format = ((ويڪر-1)<<4)|(inner_ٻٽو_انگ_ڳڻپ-1);
    map_ڳڻپ_2 = output_map.ڊيگهائي;
    HBUINT8 *p = c->allocate_سائيز<HBUINT8> (ويڪر * output_map.ڊيگهائي);
    إذا (unlikely (!p)) return_trace (خطأ);
    لكل (بنانشان صحيح i = 0; i < output_map.ڊيگهائي; i++)
    {
      بنانشان صحيح v = output_map.arrayZ[i];
      إذا (v)
      {
	بنانشان صحيح outer = v >> 16;
	بنانشان صحيح inner = v & 0xFFFF;
	بنانشان صحيح u = (outer << inner_ٻٽو_انگ_ڳڻپ) | inner;
	لكل (بنانشان صحيح w = ويڪر; w > 0;)
	{
	  p[--w] = u;
	  u >>= 8;
	}
      }
      p += ويڪر;
    }
    return_trace (سچ);
  }

  HB_ALWAYS_INLINE
  uint32_t map (بنانشان صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    /* If count is zero, pass value unchanged.  This takes
     * care of direct mapping for advance map. */
    إذا (!map_ڳڻپ_2)
      أرجع v;
    أرجع _map (v);
  }

  HB_HOT
  uint32_t _map (بنانشان صحيح v) ثابت /* Returns 16.16 outer.inner. */
  {
    إذا (v >= map_ڳڻپ_2)
      v = map_ڳڻپ_2 - 1;

    بنانشان صحيح u = 0;
    { /* Fetch it. */
      بنانشان صحيح w = get_ويڪر ();
      ثابت HBUINT8 *p = map_معلومات_Z.arrayZ + w * v;
      لكل (; w; w--)
        u = (u << 8) + *p++;
    }

    { /* Repack it. */
      بنانشان صحيح n = get_inner_ٻٽو_انگ_ڳڻپ ();
      بنانشان صحيح outer = u >> n;
      بنانشان صحيح inner = u & ((1 << n) - 1);
      u = (outer<<16) | inner;
    }

    أرجع u;
  }

  بنانشان get_map_ڳڻپ () ثابت       { أرجع map_ڳڻپ_2; }
  بنانشان get_ويڪر () ثابت           { أرجع ((داخلا_Format >> 4) & 3) + 1; }
  بنانشان get_inner_ٻٽو_انگ_ڳڻپ () ثابت { أرجع (داخلا_Format & 0xF) + 1; }


  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  hb_barrier () &&
                  c->check_range (map_معلومات_Z.arrayZ,
                                  map_ڳڻپ_2,
                                  get_ويڪر ()));
  }

  محمي:
  HBUINT8       format;         /* Format identifier--format = 0 */
  HBUINT8       داخلا_Format;    /* A packed field that describes the compressed
                                 * representation of delta-set indices. */
  Map_ڳڻپ_T     map_ڳڻپ_2;       /* The number of mapping entries. */
  Unsized_Array_جو<HBUINT8>
                map_معلومات_Z;       /* The delta-set index mapping data. */

  عام:
  DEFINE_سائيز_ARRAY (2+Map_ڳڻپ_T::static_سائيز, map_معلومات_Z);
};

بنية Delta_سيٽ_اشاريو_Map
{
  سانچو <قسم_نالو T>
  منطقي serialize (hb_serialize_context_t *c, ثابت T &plan)
  {
    TRACE_SERIALIZE (هذا);
    بنانشان ڊيگهائي = plan.get_output_map ().ڊيگهائي;
    u.format.v = ڊيگهائي <= 0xFFFF ? 0 : 1;
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (u.format0.serialize (c, plan));
    حالة 1: hb_barrier (); return_trace (u.format1.serialize (c, plan));
    افتراضي:return_trace (خطأ);
    }
  }

  uint32_t map (بنانشان v) ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع (u.format0.map (v));
    حالة 1: hb_barrier (); أرجع (u.format1.map (v));
    افتراضي:أرجع v;
    }
  }

  بنانشان get_map_ڳڻپ () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_map_ڳڻپ ();
    حالة 1: hb_barrier (); أرجع u.format1.get_map_ڳڻپ ();
    افتراضي:أرجع 0;
    }
  }

  بنانشان get_ويڪر () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_ويڪر ();
    حالة 1: hb_barrier (); أرجع u.format1.get_ويڪر ();
    افتراضي:أرجع 0;
    }
  }

  بنانشان get_inner_ٻٽو_انگ_ڳڻپ () ثابت
  {
    بدل (u.format.v) {
    حالة 0: hb_barrier (); أرجع u.format0.get_inner_ٻٽو_انگ_ڳڻپ ();
    حالة 1: hb_barrier (); أرجع u.format1.get_inner_ٻٽو_انگ_ڳڻپ ();
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

  Delta_سيٽ_اشاريو_Map* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.format.v) {
    حالة 0: hb_barrier (); return_trace (reinterpret_cast<Delta_سيٽ_اشاريو_Map *> (u.format0.copy (c)));
    حالة 1: hb_barrier (); return_trace (reinterpret_cast<Delta_سيٽ_اشاريو_Map *> (u.format1.copy (c)));
    افتراضي:return_trace (خالي_اشارو);
    }
  }

  محمي:
  اتحاد {
  بنية { HBUINT8 v; }             format;         /* Format identifier */
  Delta_سيٽ_اشاريو_Map_Format01<HBUINT16> format0;
  Delta_سيٽ_اشاريو_Map_Format01<HBUINT32> format1;
  } u;
  عام:
  DEFINE_سائيز_UNION (1, format.v);
};


بنية شي_Var_Store_Instancer
{
  شي_Var_Store_Instancer (ثابت شي_Variation_Store *varStore_,
			 ثابت Delta_سيٽ_اشاريو_Map *varIdxMap,
			 hb_array_t<ثابت صحيح> coords,
			 hb_scalar_cache_t *cache = خالي_اشارو) :
    varStore (varStore_), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(شي_Variation_Store);
    /* Like hb_font_t::has_nonzero_coords, treat explicit all-zero coordinates
     * as the default instance, which uses unadjusted values (OFF 7.1). */
    إذا (!hb_any (coords))
      هذا->coords = {};
  }

  عامل منطقي () ثابت { أرجع varStore && منطقي (coords); }

  عائم عامل[] (uint32_t varIdx) ثابت
  { أرجع (*هذا) (varIdx); }

  عائم عامل() (uint32_t varIdx, بنانشان ننڍو وٿي = 0) ثابت
  { أرجع get_delta (varIdx, وٿي); }

  مزدوج get_condition_delta (uint32_t varIdx) ثابت
  { أرجع get_delta<مزدوج> (varIdx); }

  سانچو <قسم_نالو Float = عائم>
  Float get_delta (uint32_t varIdx, بنانشان ننڍو وٿي = 0) ثابت
  {
   /* Ordinary stores do not apply deltas at the default instance. */
   إذا (!coords || varIdx == VarIdx::NO_VARIATION)
     أرجع Float (0);

    varIdx += وٿي;
    إذا (varIdxMap)
      varIdx = varIdxMap->map (varIdx);
    أرجع varStore->get_delta<Float> (varIdx, coords, cache);
  }

  ثابت شي_Variation_Store *varStore;
  ثابت Delta_سيٽ_اشاريو_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};

بنية Multi_شي_Var_Store_Instancer
{
  Multi_شي_Var_Store_Instancer (ثابت Multi_شي_Variation_Store *varStore,
			      ثابت Delta_سيٽ_اشاريو_Map *varIdxMap,
			      hb_array_t<ثابت صحيح> coords,
			      hb_scalar_cache_t *cache = خالي_اشارو) :
    varStore (varStore), varIdxMap (varIdxMap), coords (coords), cache (cache)
  {
    إذا (!varStore)
      varStore = &Null(Multi_شي_Variation_Store);
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

  فراغ عامل() (hb_array_t<مزدوج> ٻاهريون, uint32_t varIdx, بنانشان ننڍو وٿي = 0) ثابت
  {
    /* Static VARC fonts can have constant deltas even with empty coordinates. */
    إذا (varIdx != VarIdx::NO_VARIATION)
    {
      varIdx += وٿي;
      إذا (varIdxMap)
	varIdx = varIdxMap->map (varIdx);
      varStore->get_delta (varIdx, coords, ٻاهريون, cache);
    }
    وإلا
      لكل (بنانشان i = 0; i < ٻاهريون.ڊيگهائي; i++)
        ٻاهريون.arrayZ[i] = 0.;
  }

  ثابت Multi_شي_Variation_Store *varStore;
  ثابت Delta_سيٽ_اشاريو_Map *varIdxMap;
  hb_array_t<ثابت صحيح> coords;
  hb_scalar_cache_t *cache;
};


/*
 * Feature Variations
 */
تعداد Cond_with_Var_flag_t
{
  KEEP_COND_WITH_VAR = 0,
  KEEP_رڪارڊ_WITH_VAR = 1,
  DROP_COND_WITH_VAR = 2,
  DROP_رڪارڊ_WITH_VAR = 3,
};

تعداد lookup_condition_subset_نتيجو_t
{
  LOOKUP_CONDITION_SUBSET_ERROR,
  LOOKUP_CONDITION_SUBSET_FALSE,
  LOOKUP_CONDITION_SUBSET_TRUE,
  LOOKUP_CONDITION_SUBSET_KEEP,
};

بنية Condition;

سانچو <قسم_نالو Instancer>
ساكن منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بنانشان صحيح coord_len,
				Instancer *instancer);

بنية ConditionAxisRange
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ثابت hb_map_t *اشاريو_map = &c->plan->axes_اشاريو_map;
    إذا (اشاريو_map->is_empty ()) return_trace (سچ);

    ثابت hb_map_t& axes_old_اشاريو_ٽيگ_map = c->plan->axes_old_اشاريو_ٽيگ_map;
    hb_codepoint_t *axis_ٽيگ_2;
    إذا (!axes_old_اشاريو_ٽيگ_map.has (axis_اشاريو_2, &axis_ٽيگ_2) ||
        !اشاريو_map->has (axis_اشاريو_2))
      return_trace (خطأ);

    ثابت hb_hashmap_t<hb_ٽيگ_t, Triple>& normalized_axes_location = c->plan->axes_location;
    Triple axis_حد_بندي{-1.0, 0.0, 1.0};
    Triple *normalized_حد_بندي;
    إذا (normalized_axes_location.has (*axis_ٽيگ_2, &normalized_حد_بندي))
      axis_حد_بندي = *normalized_حد_بندي;

    ثابت hb_hashmap_t<hb_ٽيگ_t, TripleDistances>& axes_triple_distances = c->plan->axes_triple_distances;
    TripleDistances axis_triple_distances{1.0, 1.0};
    TripleDistances *triple_dists;
    إذا (axes_triple_distances.has (*axis_ٽيگ_2, &triple_dists))
      axis_triple_distances = *triple_dists;

    عائم normalized_min = renormalize_قدر ((مزدوج) filter_Range_Min_قدر.منزل_float (), axis_حد_بندي, axis_triple_distances, خطأ);
    عائم normalized_max = renormalize_قدر ((مزدوج) filter_Range_Max_قدر.منزل_float (), axis_حد_بندي, axis_triple_distances, خطأ);
    ٻاهريون->filter_Range_Min_قدر.سيٽ_float (normalized_min);
    ٻاهريون->filter_Range_Max_قدر.سيٽ_float (normalized_max);

    return_trace (c->serializer->check_سونپيو (ٻاهريون->axis_اشاريو_2, اشاريو_map->get (axis_اشاريو_2),
                                               HB_SERIALIZE_ERROR_INT_حد_کان_مٿي));
  }

  خاص:
  Cond_with_Var_flag_t keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                             hb_map_t *condition_map /*ٻاهريون*/) ثابت
  {
    //invalid axis index, drop the entire record
    إذا (!c->axes_اشاريو_ٽيگ_map->has (axis_اشاريو_2))
      أرجع DROP_رڪارڊ_WITH_VAR;

    hb_ٽيگ_t axis_ٽيگ_2 = c->axes_اشاريو_ٽيگ_map->get (axis_اشاريو_2);

    Triple axis_range (-1.0, 0.0, 1.0);
    Triple *axis_حد_بندي;
    منطقي axis_سيٽ_by_user = خطأ;
    إذا (c->axes_location->has (axis_ٽيگ_2, &axis_حد_بندي))
    {
      axis_range = *axis_حد_بندي;
      axis_سيٽ_by_user = سچ;
    }

    عائم axis_min_val = axis_range.minimum;
    عائم axis_default_val = axis_range.وچ;
    عائم axis_max_val = axis_range.وڌ_کان_وڌ;

    عائم filter_min_val = filter_Range_Min_قدر.منزل_float ();
    عائم filter_max_val = filter_Range_Max_قدر.منزل_float ();

    إذا (axis_default_val < filter_min_val ||
        axis_default_val > filter_max_val)
      c->apply = خطأ;

    //condition not met, drop the entire record
    إذا (axis_min_val > filter_max_val || axis_max_val < filter_min_val ||
        filter_min_val > filter_max_val)
      أرجع DROP_رڪارڊ_WITH_VAR;

    //condition met and axis pinned, drop the condition
    إذا (axis_سيٽ_by_user && axis_range.is_نقطو ())
      أرجع DROP_COND_WITH_VAR;

    إذا (filter_max_val != axis_max_val || filter_min_val != axis_min_val)
    {
      // add axisIndex->value into the hashmap so we can check if the record is
      // unique with variations
      uint16_t int_filter_max_val = (uint16_t) filter_Range_Max_قدر.منزل_int ();
      uint16_t int_filter_min_val = (uint16_t) filter_Range_Min_قدر.منزل_int ();
      hb_codepoint_t val = (int_filter_max_val << 16) + int_filter_min_val;

      condition_map->سيٽ (axis_اشاريو_2, val);
      أرجع KEEP_COND_WITH_VAR;
    }
    أرجع KEEP_رڪارڊ_WITH_VAR;
  }

  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
		 Instancer *instancer HB_اڻ_استعمال_ٿيل) ثابت
  {
    صحيح coord = axis_اشاريو_2 < coord_len ? coords[axis_اشاريو_2] : 0;
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
  HBUINT16	axis_اشاريو_2;
  F2DOT14	filter_Range_Min_قدر;
  F2DOT14	filter_Range_Max_قدر;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Condition_قدر
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    إذا (varIdx == VarIdx::NO_VARIATION)
      return_trace (سچ);

    hb_pair_t<بنانشان, صحيح> *condition_قدر;
    إذا (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) هذا, &condition_قدر))
    {
      ٻاهريون->default_قدر_2 = condition_قدر->second;
      ٻاهريون->varIdx = condition_قدر->پهريون;
      return_trace (سچ);
    }

    hb_pair_t<بنانشان, صحيح> *new_varidx_delta;
    إذا (!c->plan->layout_variation_idx_delta_map.has (varIdx, &new_varidx_delta))
      return_trace (خطأ);

    إذا (unlikely (!c->serializer->check_سونپيو (
		ٻاهريون->default_قدر_2,
		default_قدر_2 + hb_ٻيو (*new_varidx_delta),
		HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
      return_trace (خطأ);
    ٻاهريون->varIdx = hb_پهريون (*new_varidx_delta);
    return_trace (سچ);
  }

  خاص:
  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
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
  DEFINE_سائيز_STATIC (8);
};

بنية ConditionAnd
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
      return_trace (خطأ);
    ٻاهريون->format = format;

    لكل (ثابت خودڪار &وٿي : conditions)
    {
      إذا (وٿي.is_null ())
      {
	خودڪار *ٻاهريون_وٿي = ٻاهريون->conditions.serialize_append (c->serializer);
	إذا (unlikely (!ٻاهريون_وٿي)) return_trace (خطأ);
	*ٻاهريون_وٿي = 0;
      }
      وإلا إذا (unlikely (!subset_وٿي_array (c, ٻاهريون->conditions, هذا) (وٿي)))
	return_trace (خطأ);
    }

    return_trace (سچ);
  }

  خاص:
  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بنانشان صحيح ڳڻپ = conditions.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
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
  Array8_جو_Offset24_منزل<بنية Condition>	conditions;
  عام:
  DEFINE_سائيز_ARRAY (3, conditions);
};

بنية ConditionOr
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
      return_trace (خطأ);
    ٻاهريون->format = format;

    لكل (ثابت خودڪار &وٿي : conditions)
    {
      إذا (وٿي.is_null ())
      {
	خودڪار *ٻاهريون_وٿي = ٻاهريون->conditions.serialize_append (c->serializer);
	إذا (unlikely (!ٻاهريون_وٿي)) return_trace (خطأ);
	*ٻاهريون_وٿي = 0;
      }
      وإلا إذا (unlikely (!subset_وٿي_array (c, ٻاهريون->conditions, هذا) (وٿي)))
	return_trace (خطأ);
    }

    return_trace (سچ);
  }

  خاص:
  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
		 Instancer *instancer) ثابت
  {
    بنانشان صحيح ڳڻپ = conditions.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
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
  Array8_جو_Offset24_منزل<بنية Condition>	conditions;
  عام:
  DEFINE_سائيز_ARRAY (3, conditions);
};

بنية ConditionNegate
{
  دوست بنية Condition;

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
    إذا (condition.is_null ())
    {
      ٻاهريون->condition = 0;
      return_trace (سچ);
    }
    return_trace (ٻاهريون->condition.serialize_subset (c, condition, هذا));
  }

  خاص:
  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
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
  DEFINE_سائيز_STATIC (5);
};

بنية Condition
{
  دوست بنية Condition_سيٽ;

  ساكن منطقي serialize_constant (hb_serialize_context_t *c, منطقي value)
  {
    خودڪار *ٻاهريون = c->شروعات_embed<Condition_قدر> ();
    إذا (unlikely (!ٻاهريون || !c->extend_min (ٻاهريون))) أرجع خطأ;
    ٻاهريون->format = 2;
    ٻاهريون->default_قدر_2 = value ? 1 : 0;
    ٻاهريون->varIdx = VarIdx::NO_VARIATION;
    أرجع سچ;
  }

  lookup_condition_subset_نتيجو_t subset_lookup_condition (
				      hb_subset_context_t *c,
				      بنانشان اونهائي = HB_MAX_NESTING_LEVEL,
				      منطقي materialize_true = خطأ) ثابت;

  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
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
                                             hb_map_t *condition_map /*ٻاهريون*/,
                                             بنانشان اونهائي = HB_MAX_NESTING_LEVEL) ثابت
  {
    إذا (unlikely (!اونهائي))
    {
      c->apply = خطأ;
      أرجع DROP_رڪارڊ_WITH_VAR;
    }

    بدل (u.format.v) {
    حالة 0: أرجع KEEP_رڪارڊ_WITH_VAR;
    حالة 1: hb_barrier (); أرجع u.format1.keep_with_variations (c, condition_map);
    حالة 2: حالة 3: حالة 4: حالة 5: اكسر;
    افتراضي: c->apply = خطأ; أرجع DROP_رڪارڊ_WITH_VAR;
    }

    hb_barrier ();
    منطقي applies = evaluate (c->instancer->coords.arrayZ,
			     c->instancer->coords.ڊيگهائي, c->instancer);
    c->apply &= applies;
    إذا (c->all_axes_pinned)
      أرجع applies ? DROP_COND_WITH_VAR : DROP_رڪارڊ_WITH_VAR;

    إذا (u.format.v == 2)
      أرجع u.format2.varIdx == VarIdx::NO_VARIATION ||
             !c->instancer->varStore->has_delta_سيٽ (u.format2.varIdx) ?
             (applies ? KEEP_رڪارڊ_WITH_VAR : DROP_رڪارڊ_WITH_VAR) :
             KEEP_COND_WITH_VAR;

    خودڪار classify_ٻار = [c, condition_map, اونهائي] (ثابت Condition &ٻار)
    {
      خودڪار ٻار_context = *c;
      ٻار_context.apply = سچ;
      أرجع ٻار.keep_with_variations (&ٻار_context, condition_map, اونهائي - 1);
    };

    إذا (u.format.v == 5)
    {
      خودڪار نتيجو = classify_ٻار (&u.format5 + u.format5.condition);
      أرجع نتيجو == DROP_رڪارڊ_WITH_VAR ? KEEP_رڪارڊ_WITH_VAR :
             نتيجو == KEEP_COND_WITH_VAR ? KEEP_COND_WITH_VAR :
             DROP_رڪارڊ_WITH_VAR;
    }

    منطقي is_and = u.format.v == 3;
    منطقي variable = خطأ;
    ثابت خودڪار &conditions = is_and ? u.format3.conditions : u.format4.conditions;
    ثابت فراغ *بنياد = is_and ? (ثابت فراغ *) &u.format3 : (ثابت فراغ *) &u.format4;
    لكل (ثابت خودڪار &وٿي : conditions)
    {
      خودڪار نتيجو = classify_ٻار (بنياد + وٿي);
      إذا (is_and && نتيجو == DROP_رڪارڊ_WITH_VAR)
        أرجع DROP_رڪارڊ_WITH_VAR;
      إذا (!is_and && (نتيجو == DROP_COND_WITH_VAR || نتيجو == KEEP_رڪارڊ_WITH_VAR))
        أرجع KEEP_رڪارڊ_WITH_VAR;
      variable |= نتيجو == KEEP_COND_WITH_VAR;
    }
    أرجع variable ? KEEP_COND_WITH_VAR :
           is_and ? KEEP_رڪارڊ_WITH_VAR : DROP_رڪارڊ_WITH_VAR;
  }

  سانچو <قسم_نالو context_t, قسم_نالو ...Ts>
  قسم_نالو context_t::return_t dispatch (context_t *c, Ts&&... ds) ثابت
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

    إذا (unlikely (!c->check_شروعات_recursion (HB_MAX_NESTING_LEVEL)))
      return_trace (c->no_dispatch_return_قدر ());

    return_trace (c->پڇاڙي_recursion (هذا->dispatch (c)));
  }

  منطقي collect_var_indices (hb_سيٽ_t *var_indices,
			    بنانشان اونهائي = HB_MAX_NESTING_LEVEL,
                            hb_hashmap_t<uintptr_t, hb_pair_t<بنانشان, صحيح>> *condition_قدرون = خالي_اشارو) ثابت;

  منطقي serialize (hb_serialize_context_t *c,
		  ثابت Condition *src,
		  ثابت hb_map_t &varidx_map,
		  ثابت hb_map_t *axis_map = خالي_اشارو);

  خاص:
  lookup_condition_subset_نتيجو_t subset_lookup_condition_impl (
				      hb_subset_context_t *c,
				      بنانشان اونهائي) ثابت;

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
  DEFINE_سائيز_UNION (2, format.v);
};

سانچو <قسم_نالو Instancer>
منطقي
_hb_recurse_condition_evaluate (ثابت بنية Condition &condition,
				ثابت صحيح *coords,
				بنانشان صحيح coord_len,
				Instancer *instancer)
{
  أرجع condition.evaluate (coords, coord_len, instancer);
}

بنية ConditionList
{
  ثابت Condition& عامل[] (بنانشان i) ثابت
  { أرجع هذا+conditions[i]; }

  بنانشان get_ڳڻپ () ثابت { أرجع conditions.len; }

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
    لكل (بنانشان i = 0; i < conditions.len; i++)
    {
      بنانشان old_اشاريو = condition_map.backward (i);
      إذا (unlikely (old_اشاريو >= src->conditions.len ||
		    !conditions[i].serialize_serialize (
			c, &(src+src->conditions[old_اشاريو]), varidx_map, axis_map)))
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
  Array32_جو_Offset32_منزل<Condition> conditions;
  عام:
  DEFINE_سائيز_ARRAY (4, conditions);
};

مضمن منطقي
Condition::collect_var_indices (hb_سيٽ_t *var_indices, بنانشان اونهائي,
                               hb_hashmap_t<uintptr_t, hb_pair_t<بنانشان, صحيح>> *condition_قدرون) ثابت
{
  إذا (unlikely (!اونهائي)) أرجع خطأ;
  بدل (u.format.v)
  {
    حالة 0:
    حالة 1:
      أرجع سچ;
    حالة 2:
      إذا (u.format2.varIdx != VarIdx::NO_VARIATION)
      {
        var_indices->جوڙيو (u.format2.varIdx);
        إذا (condition_قدرون &&
            !condition_قدرون->سيٽ ((uintptr_t) &u.format2,
                                    hb_pair (بنانشان (u.format2.varIdx), صحيح (u.format2.default_قدر_2))))
          أرجع خطأ;
      }
      أرجع !var_indices->اندريون_error ();
    حالة 3:
      لكل (ثابت خودڪار &وٿي : u.format3.conditions)
	إذا (unlikely (!(&u.format3+وٿي).collect_var_indices (var_indices,
							     اونهائي - 1, condition_قدرون)))
	  أرجع خطأ;
      أرجع سچ;
    حالة 4:
      لكل (ثابت خودڪار &وٿي : u.format4.conditions)
	إذا (unlikely (!(&u.format4+وٿي).collect_var_indices (var_indices,
							     اونهائي - 1, condition_قدرون)))
	  أرجع خطأ;
      أرجع سچ;
    حالة 5:
      أرجع (&u.format5+u.format5.condition).collect_var_indices (var_indices,
								  اونهائي - 1, condition_قدرون);
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
      خودڪار *ٻاهريون = c->شروعات_embed<Condition_قدر> ();
      إذا (unlikely (!ٻاهريون || !c->extend_min (ٻاهريون))) return_trace (خطأ);
      ٻاهريون->format = 2;
      ٻاهريون->default_قدر_2 = 1;
      ٻاهريون->varIdx = VarIdx::NO_VARIATION;
      return_trace (سچ);
    }
    حالة 1:
    {
      خودڪار *ٻاهريون = c->embed (&src->u.format1);
      إذا (unlikely (!ٻاهريون)) return_trace (خطأ);
      إذا (axis_map)
      {
	إذا (unlikely (!axis_map->has (ٻاهريون->axis_اشاريو_2)))
	{
	  c->err (HB_SERIALIZE_ERROR_OTHER);
	  return_trace (خطأ);
	}
	إذا (unlikely (!c->check_سونپيو (ٻاهريون->axis_اشاريو_2, axis_map->get (ٻاهريون->axis_اشاريو_2),
				      HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
	  return_trace (خطأ);
      }
      return_trace (سچ);
    }
    حالة 2:
    {
      خودڪار *ٻاهريون = c->embed (&src->u.format2);
      إذا (unlikely (!ٻاهريون ||
		    (src->u.format2.varIdx != VarIdx::NO_VARIATION &&
		     !varidx_map.has (src->u.format2.varIdx))))
	return_trace (خطأ);
      ٻاهريون->varIdx = src->u.format2.varIdx == VarIdx::NO_VARIATION ?
		    src->u.format2.varIdx : varidx_map.get (src->u.format2.varIdx);
      return_trace (سچ);
    }
    حالة 3:
    {
      خودڪار *ٻاهريون = c->شروعات_embed<ConditionAnd> ();
      إذا (unlikely (!c->extend_min (ٻاهريون))) return_trace (خطأ);
      ٻاهريون->format = 3;
      ٻاهريون->conditions.len = src->u.format3.conditions.len;
      إذا (unlikely (!c->extend (ٻاهريون->conditions))) return_trace (خطأ);
      لكل (بنانشان i = 0; i < ٻاهريون->conditions.len; i++)
	إذا (unlikely (!ٻاهريون->conditions[i].serialize_serialize (
			c, &(&src->u.format3+src->u.format3.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (سچ);
    }
    حالة 4:
    {
      خودڪار *ٻاهريون = c->شروعات_embed<ConditionOr> ();
      إذا (unlikely (!c->extend_min (ٻاهريون))) return_trace (خطأ);
      ٻاهريون->format = 4;
      ٻاهريون->conditions.len = src->u.format4.conditions.len;
      إذا (unlikely (!c->extend (ٻاهريون->conditions))) return_trace (خطأ);
      لكل (بنانشان i = 0; i < ٻاهريون->conditions.len; i++)
	إذا (unlikely (!ٻاهريون->conditions[i].serialize_serialize (
			c, &(&src->u.format4+src->u.format4.conditions[i]),
			varidx_map, axis_map)))
	  return_trace (خطأ);
      return_trace (سچ);
    }
    حالة 5:
    {
      خودڪار *ٻاهريون = c->embed (&src->u.format5);
      إذا (unlikely (!ٻاهريون ||
		    !ٻاهريون->condition.serialize_serialize (
			c, &(&src->u.format5+src->u.format5.condition),
			varidx_map, axis_map)))
	return_trace (خطأ);
      return_trace (سچ);
    }
    افتراضي:
      return_trace (خطأ);
  }
}

مضمن lookup_condition_subset_نتيجو_t
Condition::subset_lookup_condition (hb_subset_context_t *c,
				    بنانشان اونهائي,
				    منطقي materialize_true) ثابت
{
  خودڪار snap = c->serializer->snapshot ();
  lookup_condition_subset_نتيجو_t نتيجو =
      subset_lookup_condition_impl (c, اونهائي);
  إذا (نتيجو == LOOKUP_CONDITION_SUBSET_KEEP)
    أرجع نتيجو;

  c->serializer->revert (snap);
  إذا (نتيجو != LOOKUP_CONDITION_SUBSET_TRUE || !materialize_true)
    أرجع نتيجو;

  أرجع serialize_constant (c->serializer, سچ) ?
         LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
}

مضمن lookup_condition_subset_نتيجو_t
Condition::subset_lookup_condition_impl (hb_subset_context_t *c,
					 بنانشان اونهائي) ثابت
{
  إذا (unlikely (!اونهائي))
    أرجع LOOKUP_CONDITION_SUBSET_ERROR;

  خودڪار subset_وٿي = [c, اونهائي] (Array8_جو_Offset24_منزل<Condition> &ٻاهريون,
				   ثابت Condition &condition)
      -> lookup_condition_subset_نتيجو_t
  {
    خودڪار snap = c->serializer->snapshot ();
    خودڪار *وٿي = ٻاهريون.serialize_append (c->serializer);
    إذا (unlikely (!وٿي))
      أرجع LOOKUP_CONDITION_SUBSET_ERROR;

    c->serializer->push ();
    lookup_condition_subset_نتيجو_t نتيجو =
        condition.subset_lookup_condition (c, اونهائي - 1);
    إذا (نتيجو == LOOKUP_CONDITION_SUBSET_KEEP)
      c->serializer->جوڙيو_link (*وٿي, c->serializer->ڪڍو_pack ());
    وإلا
    {
      c->serializer->ڪڍو_discard ();
      ٻاهريون.ڪڍو ();
      c->serializer->revert (snap);
    }
    أرجع نتيجو;
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

      hb_codepoint_t *axis_ٽيگ_2;
      إذا (!c->plan->axes_old_اشاريو_ٽيگ_map.has (src.axis_اشاريو_2, &axis_ٽيگ_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      Triple axis_حد_بندي {-1.f, 0.f, 1.f};
      Triple *normalized_حد_بندي;
      إذا (c->plan->axes_location.has (*axis_ٽيگ_2, &normalized_حد_بندي))
        axis_حد_بندي = *normalized_حد_بندي;

      عائم filter_min = src.filter_Range_Min_قدر.منزل_float ();
      عائم filter_max = src.filter_Range_Max_قدر.منزل_float ();
      إذا (filter_min > filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد_بندي.is_نقطو ())
        أرجع filter_min <= axis_حد_بندي.وچ &&
	       axis_حد_بندي.وچ <= filter_max ?
	       LOOKUP_CONDITION_SUBSET_TRUE :
	       LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (axis_حد_بندي.وڌ_کان_وڌ < filter_min ||
	  filter_max < axis_حد_بندي.minimum)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (filter_min <= axis_حد_بندي.minimum &&
	  axis_حد_بندي.وڌ_کان_وڌ <= filter_max)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      إذا (!c->plan->axes_اشاريو_map.has (src.axis_اشاريو_2))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      خودڪار *ٻاهريون = c->serializer->embed (&src);
      إذا (unlikely (!ٻاهريون))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      TripleDistances axis_distances {1.f, 1.f};
      TripleDistances *distances;
      إذا (c->plan->axes_triple_distances.has (*axis_ٽيگ_2, &distances))
        axis_distances = *distances;

      ٻاهريون->filter_Range_Min_قدر.سيٽ_float (
	  renormalize_قدر (filter_min, axis_حد_بندي, axis_distances, خطأ));
      ٻاهريون->filter_Range_Max_قدر.سيٽ_float (
	  renormalize_قدر (filter_max, axis_حد_بندي, axis_distances, خطأ));
      إذا (unlikely (!c->serializer->check_سونپيو (
		ٻاهريون->axis_اشاريو_2,
		c->plan->axes_اشاريو_map.get (src.axis_اشاريو_2),
		HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 2:
    {
      ثابت Condition_قدر &src = u.format2;
      خودڪار *ٻاهريون = c->serializer->embed (&src);
      إذا (unlikely (!ٻاهريون))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;

      إذا (src.varIdx == VarIdx::NO_VARIATION)
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      hb_pair_t<بنانشان, صحيح> *condition_قدر;
      إذا (c->plan->layout_condition_idx_قدر_map.has ((uintptr_t) &src, &condition_قدر))
      {
        ٻاهريون->default_قدر_2 = condition_قدر->second;
        ٻاهريون->varIdx = condition_قدر->پهريون;
        إذا (ٻاهريون->varIdx == VarIdx::NO_VARIATION)
          أرجع ٻاهريون->default_قدر_2 > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                                        LOOKUP_CONDITION_SUBSET_FALSE;
        أرجع LOOKUP_CONDITION_SUBSET_KEEP;
      }

      hb_pair_t<بنانشان, صحيح> *new_varidx_delta;
      إذا (!c->plan->layout_variation_idx_delta_map.has (src.varIdx,
							&new_varidx_delta))
        أرجع c->plan->user_axes_location.is_empty () ?
               LOOKUP_CONDITION_SUBSET_KEEP :
               ((صحيح) src.default_قدر_2 > 0 ?
		LOOKUP_CONDITION_SUBSET_TRUE :
		LOOKUP_CONDITION_SUBSET_FALSE);

      إذا (!c->plan->user_axes_location.is_empty () &&
          hb_پهريون (*new_varidx_delta) == VarIdx::NO_VARIATION)
      {
        // Conditions becoming constant depend on the unrounded sign, not
        // the integer delta used to instance positioning values.
        مزدوج value = (صحيح) src.default_قدر_2 +
                      c->plan->layout_variation_delta (src.varIdx);
        أرجع value > 0 ? LOOKUP_CONDITION_SUBSET_TRUE :
                          LOOKUP_CONDITION_SUBSET_FALSE;
      }

      صحيح default_قدر = (صحيح) src.default_قدر_2 +
			  hb_ٻيو (*new_varidx_delta);
      إذا (unlikely (!c->serializer->check_سونپيو (
		ٻاهريون->default_قدر_2,
		default_قدر,
		HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      ٻاهريون->varIdx = hb_پهريون (*new_varidx_delta);

      أرجع LOOKUP_CONDITION_SUBSET_KEEP;
    }

    حالة 3:
    {
      ثابت ConditionAnd &src = u.format3;
      خودڪار *ٻاهريون = c->serializer->شروعات_embed (&src);
      إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      ٻاهريون->format = 3;

      لكل (ثابت خودڪار &وٿي : src.conditions)
      {
        lookup_condition_subset_نتيجو_t نتيجو =
            subset_وٿي (ٻاهريون->conditions, &src + وٿي);
        إذا (نتيجو == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتيجو == LOOKUP_CONDITION_SUBSET_FALSE)
          أرجع نتيجو;
      }
      أرجع ٻاهريون->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_TRUE;
    }

    حالة 4:
    {
      ثابت ConditionOr &src = u.format4;
      خودڪار *ٻاهريون = c->serializer->شروعات_embed (&src);
      إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      ٻاهريون->format = 4;

      لكل (ثابت خودڪار &وٿي : src.conditions)
      {
        lookup_condition_subset_نتيجو_t نتيجو =
            subset_وٿي (ٻاهريون->conditions, &src + وٿي);
        إذا (نتيجو == LOOKUP_CONDITION_SUBSET_ERROR ||
	    نتيجو == LOOKUP_CONDITION_SUBSET_TRUE)
          أرجع نتيجو;
      }
      أرجع ٻاهريون->conditions ?
	     LOOKUP_CONDITION_SUBSET_KEEP :
	     LOOKUP_CONDITION_SUBSET_FALSE;
    }

    حالة 5:
    {
      ثابت ConditionNegate &src = u.format5;
      خودڪار *ٻاهريون = c->serializer->embed (&src);
      إذا (unlikely (!ٻاهريون))
        أرجع LOOKUP_CONDITION_SUBSET_ERROR;
      ٻاهريون->condition = 0;

      c->serializer->push ();
      lookup_condition_subset_نتيجو_t نتيجو =
          (&src + src.condition).subset_lookup_condition (c, اونهائي - 1);
      إذا (نتيجو == LOOKUP_CONDITION_SUBSET_KEEP)
      {
        c->serializer->جوڙيو_link (ٻاهريون->condition,
				 c->serializer->ڪڍو_pack ());
        أرجع نتيجو;
      }

      c->serializer->ڪڍو_discard ();
      إذا (نتيجو == LOOKUP_CONDITION_SUBSET_TRUE)
        أرجع LOOKUP_CONDITION_SUBSET_FALSE;
      إذا (نتيجو == LOOKUP_CONDITION_SUBSET_FALSE)
        أرجع LOOKUP_CONDITION_SUBSET_TRUE;
      أرجع نتيجو;
    }

    افتراضي:
      أرجع LOOKUP_CONDITION_SUBSET_FALSE;
  }
}

بنية Condition_سيٽ
{
  منطقي evaluate (ثابت صحيح *coords, بنانشان صحيح coord_len,
		 شي_Var_Store_Instancer *instancer) ثابت
  {
    بنانشان صحيح ڳڻپ = conditions.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
      إذا (!(هذا+conditions.arrayZ[i]).evaluate (coords, coord_len, instancer))
	أرجع خطأ;
    أرجع سچ;
  }

  فراغ keep_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    hb_map_t *condition_map = hb_map_create ();
    إذا (unlikely (!condition_map)) أرجع;
    hb::shared_ptr<hb_map_t> p {condition_map};

    hb_سيٽ_t *cond_سيٽ = hb_سيٽ_create ();
    إذا (unlikely (!cond_سيٽ)) أرجع;
    hb::shared_ptr<hb_سيٽ_t> s {cond_سيٽ};

    c->apply = سچ;
    منطقي should_keep = خطأ;
    /* The range map cannot identify value or boolean expressions uniquely. */
    منطقي can_deduplicate = سچ;
    بنانشان انگ_kept_cond = 0, cond_idx = 0;
    لكل (ثابت خودڪار& وٿي : conditions)
    {
      ثابت Condition &condition = هذا+وٿي;
      can_deduplicate &= condition.u.format.v <= 1;
      Cond_with_Var_flag_t ret = condition.keep_with_variations (c, condition_map);
      // condition is not met or condition out of range, drop the entire record
      إذا (ret == DROP_رڪارڊ_WITH_VAR)
      {
        c->apply = خطأ;
        أرجع;
      }

      إذا (ret == KEEP_COND_WITH_VAR)
      {
        should_keep = سچ;
        cond_سيٽ->جوڙيو (cond_idx);
        انگ_kept_cond++;
      }

      إذا (ret == KEEP_رڪارڊ_WITH_VAR)
        should_keep = سچ;

      cond_idx++;
    }

    /* Satisfied pinned conditions still terminate first-match processing.
     * Keep their universal record when other axes remain variable. */
    c->universal = انگ_kept_cond == 0;
    إذا (!should_keep && c->all_axes_pinned) أرجع;

    //check if condition_set is unique with variations
    إذا (can_deduplicate && c->conditionset_map->has (p))
      //duplicate found, drop the entire record
      أرجع;

    إذا (can_deduplicate)
      c->conditionset_map->سيٽ (p, 1);
    c->رڪارڊ_cond_idx_map->سيٽ (c->cur_رڪارڊ_idx, s);
  }

  منطقي subset (hb_subset_context_t *c,
               hb_subset_layout_context_t *l,
               منطقي وجهو_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    إذا (وجهو_catch_all) return_trace (سچ);

    hb_سيٽ_t *retained_cond_سيٽ = خالي_اشارو;
    إذا (l->feature_رڪارڊ_cond_idx_map != خالي_اشارو)
      retained_cond_سيٽ = l->feature_رڪارڊ_cond_idx_map->get (l->cur_feature_var_رڪارڊ_idx);

    بنانشان صحيح ڳڻپ = conditions.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (retained_cond_سيٽ != خالي_اشارو && !retained_cond_سيٽ->has (i))
        استمر;
      ثابت Condition &condition = هذا+conditions[i];
      إذا (condition.u.format.v == 1)
      {
        subset_وٿي_array (c, ٻاهريون->conditions, هذا) (conditions[i]);
        استمر;
      }

      خودڪار *وٿي = ٻاهريون->conditions.serialize_append (c->serializer);
      إذا (unlikely (!وٿي)) return_trace (خطأ);
      c->serializer->push ();
      خودڪار نتيجو = condition.subset_lookup_condition (c, HB_MAX_NESTING_LEVEL, سچ);
      إذا (نتيجو == LOOKUP_CONDITION_SUBSET_FALSE)
        /* A false condition must not become a null (true) ConditionSet. */
        نتيجو = Condition::serialize_constant (c->serializer, خطأ) ?
                 LOOKUP_CONDITION_SUBSET_KEEP : LOOKUP_CONDITION_SUBSET_ERROR;
      إذا (unlikely (نتيجو == LOOKUP_CONDITION_SUBSET_ERROR))
      {
        c->serializer->ڪڍو_discard ();
        c->serializer->check_success (خطأ);
        return_trace (خطأ);
      }
      c->serializer->جوڙيو_link (*وٿي, c->serializer->ڪڍو_pack ());
    }

    return_trace (منطقي (ٻاهريون->conditions));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت خودڪار &وٿي : conditions)
      إذا (unlikely (!(هذا+وٿي).collect_var_indices (c->layout_variation_indices,
                                                       HB_MAX_NESTING_LEVEL,
                                                       c->condition_قدرون)))
	c->layout_variation_indices->err ();
  }

  محمي:
  Array16_جو_Offset32_منزل<Condition>	conditions;
  عام:
  DEFINE_سائيز_ARRAY (2, conditions);
};

بنية Feature_جدول_Substitution_رڪارڊ
{
  دوست بنية Feature_جدول_Substitution;

  فراغ collect_lookups (ثابت فراغ *بنياد, hb_سيٽ_t *lookup_indexes /*ٻاهريون*/) ثابت
  {
    أرجع (بنياد+feature).جوڙيو_lookup_indexes_منزل (lookup_indexes);
  }

  فراغ گهيرو_features (ثابت فراغ *بنياد,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سيٽ_t       *feature_indexes /*ٻاهريون*/) ثابت
  {
    إذا ((بنياد+feature).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جوڙيو (feature_اشاريو_2);
  }

  فراغ collect_feature_substitutes_with_variations (hb_hashmap_t<بنانشان, ثابت Feature*> *feature_substitutes_map,
                                                    hb_سيٽ_t& catch_all_رڪارڊ_feature_idxes,
                                                    ثابت hb_سيٽ_t *feature_indices,
                                                    ثابت فراغ *بنياد) ثابت
  {
    إذا (feature_indices->has (feature_اشاريو_2))
    {
      feature_substitutes_map->سيٽ (feature_اشاريو_2, &(بنياد+feature));
      catch_all_رڪارڊ_feature_idxes.جوڙيو (feature_اشاريو_2);
    }
  }

  منطقي serialize (hb_subset_layout_context_t *c,
                  بنانشان feature_اشاريو,
                  ثابت Feature *f, ثابت ٽيگ *ٽيگ_2)
  {
    TRACE_SERIALIZE (هذا);
    hb_serialize_context_t *s = c->subset_context->serializer;
    إذا (unlikely (!s->extend_min (هذا))) return_trace (خطأ);

    uint32_t *new_feature_idx;
    إذا (!c->feature_map_w_duplicates->has (feature_اشاريو, &new_feature_idx))
      return_trace (خطأ);

    إذا (!s->check_سونپيو (feature_اشاريو_2, *new_feature_idx, HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خطأ);

    s->push ();
    منطقي ret = f->subset (c->subset_context, c, ٽيگ_2);
    إذا (ret) s->جوڙيو_link (feature, s->ڪڍو_pack ());
    وإلا s->ڪڍو_discard ();

    return_trace (ret);
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_اشاريو;
    إذا (!c->feature_map_w_duplicates->has (feature_اشاريو_2, &new_feature_اشاريو))
      return_trace (خطأ);

    خودڪار *ٻاهريون = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->feature_اشاريو_2 = *new_feature_اشاريو;
    hb_pair_t<ثابت فراغ*, ثابت فراغ*> *original;
    إذا (!c->feature_idx_ٽيگ_map->has (feature_اشاريو_2, &original))
      return_trace (خطأ);
    ثابت ٽيگ *ٽيگ_2 = reinterpret_cast<ثابت ٽيگ*> (original->second);
    return_trace (ٻاهريون->feature.serialize_subset (c->subset_context, feature, بنياد, c, ٽيگ_2));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!c->check_struct (هذا))) return_trace (خطأ);
    hb_barrier ();
    /* The substitute is read with the tag of the feature it replaces. */
    ثابت رڪارڊ_sanitize_گهيرو_t گهيرو = {feature_list->get_ٽيگ (feature_اشاريو_2), بنياد};
    return_trace (feature.sanitize (c, بنياد, &گهيرو));
  }

  محمي:
  HBUINT16		feature_اشاريو_2;
  Offset32_منزل<Feature>	feature;
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية Feature_جدول_Substitution
{
  فراغ collect_feature_indices (hb_سيٽ_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رڪارڊ &رڪارڊ : substitutions)
      feature_indices->جوڙيو (رڪارڊ.feature_اشاريو_2);
  }

  ثابت Feature *ڳوليو_substitute (بنانشان صحيح feature_اشاريو) ثابت
  {
    بنانشان صحيح ڳڻپ = substitutions.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      ثابت Feature_جدول_Substitution_رڪارڊ &رڪارڊ = substitutions.arrayZ[i];
      إذا (رڪارڊ.feature_اشاريو_2 == feature_اشاريو)
	أرجع &(هذا+رڪارڊ.feature);
    }
    أرجع خالي_اشارو;
  }

  فراغ collect_lookups (ثابت hb_سيٽ_t *feature_indexes,
			hb_سيٽ_t       *lookup_indexes /*ٻاهريون*/) ثابت
  {
    + hb_iter (substitutions)
    | hb_filter (feature_indexes, &Feature_جدول_Substitution_رڪارڊ::feature_اشاريو_2)
    | hb_apply ([هذا, lookup_indexes] (ثابت Feature_جدول_Substitution_رڪارڊ& r)
		{ r.collect_lookups (هذا, lookup_indexes); })
    ;
  }

  سانچو <قسم_نالو ٽيگ_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_اشاريو_map,
                         ٽيگ_Getter get_ٽيگ,
                         hb_سيٽ_t *nameids_منزل_retain) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رڪارڊ &رڪارڊ : substitutions)
      إذا (feature_اشاريو_map->has (رڪارڊ.feature_اشاريو_2))
        (هذا+رڪارڊ.feature).collect_name_ids (get_ٽيگ (رڪارڊ.feature_اشاريو_2), nameids_منزل_retain);
  }

  فراغ گهيرو_features (ثابت hb_map_t *lookup_indexes,
			 hb_سيٽ_t       *feature_indexes /*ٻاهريون*/) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رڪارڊ& رڪارڊ : substitutions)
      رڪارڊ.گهيرو_features (هذا, lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت hb_map_t *feature_اشاريو_map) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رڪارڊ& رڪارڊ : substitutions)
    {
      إذا (feature_اشاريو_map->has (رڪارڊ.feature_اشاريو_2)) أرجع سچ;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    لكل (ثابت Feature_جدول_Substitution_رڪارڊ& رڪارڊ : substitutions)
      رڪارڊ.collect_feature_substitutes_with_variations (c->feature_substitutes_map,
                                                          c->catch_all_رڪارڊ_feature_idxes,
                                                          c->feature_indices, هذا);
  }

  منطقي subset (hb_subset_context_t        *c,
	       hb_subset_layout_context_t *l,
               منطقي وجهو_catch_all) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->نسخو.major = نسخو.major;
    ٻاهريون->نسخو.minor = نسخو.minor;

    إذا (وجهو_catch_all)
    {
      لكل (بنانشان feature_اشاريو : *(l->catch_all_رڪارڊ_feature_idxes))
      {
        hb_pair_t<ثابت فراغ*, ثابت فراغ*> *p;
        إذا (!l->feature_idx_ٽيگ_map->has (feature_اشاريو, &p))
          return_trace (خطأ);
        خودڪار *o = ٻاهريون->substitutions.serialize_append (c->serializer);
        إذا (!o->serialize (l, feature_اشاريو,
                           reinterpret_cast<ثابت Feature*> (p->پهريون),
                           reinterpret_cast<ثابت ٽيگ*> (p->second)))
          return_trace (خطأ);
      }
      return_trace (سچ);
    }

    + substitutions.iter ()
    | hb_apply (subset_رڪارڊ_array (l, &(ٻاهريون->substitutions), هذا))
    ;

    return_trace (منطقي (ٻاهريون->substitutions));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  substitutions.sanitize (c, هذا, feature_list));
  }

  محمي:
  Fixed_نسخو<>	نسخو;	/* Version--0x00010000u */
  Array16_جو<Feature_جدول_Substitution_رڪارڊ>
			substitutions;
  عام:
  DEFINE_سائيز_ARRAY (6, substitutions);
};

بنية Feature_Variation_رڪارڊ
{
  دوست بنية FeatureVariations;

  فراغ collect_lookups (ثابت فراغ     *بنياد,
			ثابت hb_سيٽ_t *feature_indexes,
			hb_سيٽ_t       *lookup_indexes /*ٻاهريون*/) ثابت
  {
    أرجع (بنياد+substitutions).collect_lookups (feature_indexes, lookup_indexes);
  }

  سانچو <قسم_نالو ٽيگ_Getter>
  فراغ collect_name_ids (ثابت فراغ *بنياد,
                         ثابت hb_map_t *feature_اشاريو_map,
                         ٽيگ_Getter get_ٽيگ,
                         hb_سيٽ_t *nameids_منزل_retain) ثابت
  {
    (بنياد+substitutions).collect_name_ids (feature_اشاريو_map, get_ٽيگ, nameids_منزل_retain);
  }

  فراغ گهيرو_features (ثابت فراغ     *بنياد,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سيٽ_t       *feature_indexes /*ٻاهريون*/) ثابت
  {
    (بنياد+substitutions).گهيرو_features (lookup_indexes, feature_indexes);
  }

  منطقي intersects_features (ثابت فراغ *بنياد, ثابت hb_map_t *feature_اشاريو_map) ثابت
  {
    أرجع (بنياد+substitutions).intersects_features (feature_اشاريو_map);
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c,
                                                    ثابت فراغ *بنياد) ثابت
  {
    (بنياد+conditions).keep_with_variations (c);
    إذا (c->apply && !c->variation_applied)
    {
      (بنياد+substitutions).collect_feature_substitutes_with_variations (c);
      c->variation_applied = سچ; // set variations only once
    }
  }

  منطقي subset (hb_subset_layout_context_t *c, ثابت فراغ *بنياد,
               منطقي وجهو_catch_all = خطأ) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->subset_context->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    hb_سيٽ_t *retained_conditions = c->feature_رڪارڊ_cond_idx_map ?
        c->feature_رڪارڊ_cond_idx_map->get (c->cur_feature_var_رڪارڊ_idx).get () : خالي_اشارو;
    إذا (!وجهو_catch_all && retained_conditions && retained_conditions->is_empty ())
      /* A null ConditionSet is the unconditional first-match record. */
      ٻاهريون->conditions = 0;
    وإلا
      ٻاهريون->conditions.serialize_subset (c->subset_context, conditions, بنياد, c, وجهو_catch_all);
    ٻاهريون->substitutions.serialize_subset (c->subset_context, substitutions, بنياد, c, وجهو_catch_all);

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد,
		 ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (conditions.sanitize (c, بنياد) &&
		  substitutions.sanitize (c, بنياد, feature_list));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *بنياد) ثابت
  { (بنياد+conditions).collect_variation_indices (c); }

  محمي:
  Offset32_منزل<Condition_سيٽ>
			conditions;
  Offset32_منزل<Feature_جدول_Substitution>
			substitutions;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Lookup_اشاريو_List
{
  فراغ جوڙيو_lookup_indexes_منزل (hb_سيٽ_t *lookup_indexes) ثابت
  { lookupIndices.جوڙيو_indexes_منزل (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع lookupIndices.intersects (lookup_indexes); }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
      return_trace (خطأ);

    خودڪار it =
    + hb_iter (lookupIndices)
    | hb_filter (l->lookup_اشاريو_map)
    | hb_map (l->lookup_اشاريو_map)
    ;
    ٻاهريون->lookupIndices.serialize (c->serializer, l, it);
    return_trace (منطقي (ٻاهريون->lookupIndices));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (lookupIndices.sanitize (c));
  }

  محمي:
  اشاريو_Array	lookupIndices;
  عام:
  DEFINE_سائيز_ARRAY_SIZED (2, lookupIndices);
};

بنية Lookup_Condition_رڪارڊ
{
  بنانشان get_condition_وٿي (ثابت فراغ *بنياد, ثابت فراغ *variations) ثابت
  { أرجع (ثابت حرف *) &(بنياد+condition) - (ثابت حرف *) variations; }

  سانچو <قسم_نالو Instancer>
  منطقي evaluate (ثابت فراغ *بنياد,
		 ثابت صحيح *coords,
		 بنانشان صحيح coord_len,
		 Instancer *instancer) ثابت
  { أرجع (بنياد+condition).evaluate (coords, coord_len, instancer); }

  منطقي evaluate (ثابت فراغ *بنياد,
                 ثابت فراغ *variations,
                 hb_sorted_array_t<ثابت بنانشان> conditions) ثابت
  {
    أرجع !condition || hb_bsearch (get_condition_وٿي (بنياد, variations),
                                     conditions.arrayZ, conditions.ڊيگهائي,
                                     حجم (بنانشان),
                                     _hb_cmp_operator<بنانشان, بنانشان>);
  }

  منطقي collect_condition (ثابت فراغ *بنياد,
                          ثابت فراغ *variations,
                          ثابت صحيح *coords,
                          بنانشان صحيح coord_len,
                          شي_Var_Store_Instancer *instancer,
                          hb_سيٽ_t *گهميل,
                          hb_ويڪٽر_t<بنانشان> *conditions) ثابت
  {
    /* Null conditions always apply and need no key state. */
    إذا (!condition) أرجع سچ;

    بنانشان وٿي = get_condition_وٿي (بنياد, variations);
    إذا (گهميل->has (وٿي)) أرجع سچ;
    گهميل->جوڙيو (وٿي);
    إذا (unlikely (گهميل->اندريون_error ())) أرجع خطأ;

    إذا (evaluate (بنياد, coords, coord_len, instancer))
      conditions->push (وٿي);
    أرجع !conditions->اندريون_error ();
  }

  فراغ جوڙيو_lookup_indexes_منزل (ثابت فراغ *بنياد,
			      hb_سيٽ_t *lookup_indexes) ثابت
  { (بنياد+lookupIndices).جوڙيو_lookup_indexes_منزل (lookup_indexes); }

  منطقي intersects_lookup_indexes (ثابت فراغ *بنياد,
				   ثابت hb_map_t *lookup_indexes) ثابت
  { أرجع (بنياد+lookupIndices).intersects_lookup_indexes (lookup_indexes); }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *بنياد) ثابت
  {
    إذا (unlikely (!(بنياد+condition).collect_var_indices (c->layout_variation_indices,
                                                        HB_MAX_NESTING_LEVEL,
                                                        c->condition_قدرون)))
      c->layout_variation_indices->err ();
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l,
	       ثابت فراغ *بنياد) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    ٻاهريون->condition = 0;
    c->serializer->push ();
    lookup_condition_subset_نتيجو_t نتيجو =
        (بنياد + condition).subset_lookup_condition (c,
					    HB_MAX_NESTING_LEVEL,
					    سچ);
    إذا (نتيجو != LOOKUP_CONDITION_SUBSET_KEEP)
    {
      c->serializer->ڪڍو_discard ();
      return_trace (خطأ);
    }
    c->serializer->جوڙيو_link (ٻاهريون->condition, c->serializer->ڪڍو_pack ());

    return_trace (ٻاهريون->lookupIndices.serialize_subset (c, lookupIndices, بنياد, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (condition.sanitize (c, بنياد) &&
		  lookupIndices.sanitize (c, بنياد));
  }

  محمي:
  Offset32_منزل<Condition>		condition;
  Offset32_منزل<Lookup_اشاريو_List>	lookupIndices;
  عام:
  DEFINE_سائيز_STATIC (8);
};

بنية Feature_Lookups_جدول
{
  منطقي adds_default_lookups () ثابت { أرجع flags & 0x0001u; }

  منطقي collect_conditions (ثابت فراغ *variations,
                           ثابت صحيح *coords,
                           بنانشان صحيح coord_len,
                           شي_Var_Store_Instancer *instancer,
                           hb_سيٽ_t *گهميل,
                           hb_ويڪٽر_t<بنانشان> *conditions) ثابت
  {
    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
      إذا (unlikely (!رڪارڊ.collect_condition (هذا, variations, coords, coord_len,
                                               instancer, گهميل, conditions)))
	أرجع خطأ;
    أرجع سچ;
  }

  فراغ collect_lookups (ثابت فراغ *variations,
                       hb_sorted_array_t<ثابت بنانشان> conditions,
                       hb_سيٽ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
      إذا (رڪارڊ.evaluate (هذا, variations, conditions))
	رڪارڊ.جوڙيو_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  سانچو <قسم_نالو Instancer>
  فراغ collect_lookups (ثابت صحيح *coords,
			بنانشان صحيح coord_len,
			Instancer *instancer,
			hb_سيٽ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
      إذا (رڪارڊ.evaluate (هذا, coords, coord_len, instancer))
	رڪارڊ.جوڙيو_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  فراغ collect_lookups (hb_سيٽ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
      رڪارڊ.جوڙيو_lookup_indexes_منزل (هذا, lookup_indexes);
  }

  منطقي intersects_lookup_indexes (ثابت hb_map_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
      إذا (رڪارڊ.intersects_lookup_indexes (هذا, lookup_indexes))
	أرجع سچ;
    أرجع خطأ;
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
      رڪارڊ.collect_variation_indices (c, هذا);
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون)))
      return_trace (خطأ);

    ٻاهريون->نسخو.major = نسخو.major;
    ٻاهريون->نسخو.minor = نسخو.minor;
    ٻاهريون->flags = flags;

    لكل (ثابت Lookup_Condition_رڪارڊ &رڪارڊ : رڪارڊون)
    {
      خودڪار snap = c->serializer->snapshot ();
      إذا (رڪارڊ.subset (c, l, هذا))
	ٻاهريون->رڪارڊون.len++;
      وإلا
	c->serializer->revert (snap);
    }

    return_trace (سچ);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  رڪارڊون.sanitize (c, هذا));
  }

  محمي:
  Fixed_نسخو<>	نسخو;	/* Version--0x00010000u */
  HBUINT16	flags;
  Array32_جو<Lookup_Condition_رڪارڊ>
		رڪارڊون;
  عام:
  DEFINE_سائيز_ARRAY (10, رڪارڊون);
};

بنية Lookup_Variation_رڪارڊ
{
  دوست بنية FeatureVariations;

  بنانشان get_feature_اشاريو () ثابت { أرجع feature_اشاريو_2; }

  فراغ collect_lookups (ثابت فراغ *بنياد,
                       ثابت Feature &feature,
                       hb_sorted_array_t<ثابت بنانشان> conditions,
                       hb_سيٽ_t *lookup_indexes) ثابت
  {
    ثابت Feature_Lookups_جدول &feature_lookups = بنياد+featureLookups;
    إذا (feature_lookups.adds_default_lookups ())
      feature.جوڙيو_lookup_indexes_منزل (lookup_indexes);
    feature_lookups.collect_lookups (بنياد, conditions, lookup_indexes);
  }

  فراغ collect_lookups (ثابت فراغ *بنياد,
			ثابت hb_سيٽ_t *feature_indexes,
			hb_سيٽ_t *lookup_indexes) ثابت
  {
    إذا (feature_indexes->has (feature_اشاريو_2))
      (بنياد+featureLookups).collect_lookups (lookup_indexes);
  }

  فراغ گهيرو_features (ثابت فراغ *بنياد,
			 ثابت hb_map_t *lookup_indexes,
			 hb_سيٽ_t *feature_indexes) ثابت
  {
    إذا ((بنياد+featureLookups).intersects_lookup_indexes (lookup_indexes))
      feature_indexes->جوڙيو (feature_اشاريو_2);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
				  ثابت فراغ *بنياد) ثابت
  { (بنياد+featureLookups).collect_variation_indices (c); }

  منطقي subset (hb_subset_layout_context_t *l, ثابت فراغ *بنياد) ثابت
  {
    hb_subset_context_t *c = l->subset_context;
    TRACE_SUBSET (هذا);
    uint32_t *new_feature_اشاريو;
    إذا (!l->feature_map_w_duplicates->has (feature_اشاريو_2, &new_feature_اشاريو))
      return_trace (خطأ);

    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون ||
		  !c->serializer->check_سونپيو (ٻاهريون->feature_اشاريو_2,
						*new_feature_اشاريو,
						HB_SERIALIZE_ERROR_INT_حد_کان_مٿي)))
      return_trace (خطأ);

    return_trace (ٻاهريون->featureLookups.serialize_subset (c, featureLookups, بنياد, l));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت فراغ *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  featureLookups.sanitize (c, بنياد));
  }

  محمي:
  HBUINT16			feature_اشاريو_2;
  Offset32_منزل<Feature_Lookups_جدول>	featureLookups;
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية FeatureVariations
{
  ساكن constexpr بنانشان NOT_مليو_اشاريو = 0xFFFFFFFFu;

  ثابت Array32_جو<Lookup_Variation_رڪارڊ> &get_lookup_variation_رڪارڊون () ثابت
  {
    إذا (نسخو.منزل_int () < 0x00010001u)
      أرجع Null (Array32_جو<Lookup_Variation_رڪارڊ>);
    أرجع StructAfter<Array32_جو<Lookup_Variation_رڪارڊ>> (var_رڪارڊون);
  }

  بنانشان رڪارڊ_ڳڻپ () ثابت
  {
    أرجع var_رڪارڊون.len;
  }

  منطقي ڳوليو_اشاريو (ثابت صحيح *coords, بنانشان صحيح coord_len,
		   بنانشان صحيح *اشاريو_2,
		   شي_Var_Store_Instancer *instancer) ثابت
  {
    بنانشان صحيح ڳڻپ = var_رڪارڊون.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      ثابت Feature_Variation_رڪارڊ &رڪارڊ = var_رڪارڊون.arrayZ[i];
      إذا ((هذا+رڪارڊ.conditions).evaluate (coords, coord_len, instancer))
      {
	*اشاريو_2 = i;
	أرجع سچ;
      }
    }
    *اشاريو_2 = NOT_مليو_اشاريو;
    أرجع خطأ;
  }

  ثابت Feature *ڳوليو_substitute (بنانشان صحيح variations_اشاريو,
				  بنانشان صحيح feature_اشاريو) ثابت
  {
    ثابت Feature_Variation_رڪارڊ &رڪارڊ = var_رڪارڊون[variations_اشاريو];
    أرجع (هذا+رڪارڊ.substitutions).ڳوليو_substitute (feature_اشاريو);
  }

  منطقي collect_lookup_conditions (ثابت صحيح *coords,
                                  بنانشان صحيح coord_len,
                                  شي_Var_Store_Instancer *instancer,
                                  hb_ويڪٽر_t<بنانشان> *state) ثابت
  {
    hb_سيٽ_t گهميل_tables, گهميل_conditions;
    لكل (ثابت Lookup_Variation_رڪارڊ &رڪارڊ : get_lookup_variation_رڪارڊون ())
    {
      بنانشان وٿي = رڪارڊ.featureLookups;
      إذا (گهميل_tables.has (وٿي)) استمر;
      گهميل_tables.جوڙيو (وٿي);
      إذا (unlikely (گهميل_tables.اندريون_error ())) أرجع خطأ;

      ثابت Feature_Lookups_جدول &feature_lookups = هذا+رڪارڊ.featureLookups;
      إذا (unlikely (!feature_lookups.collect_conditions (هذا, coords, coord_len,
                                                         instancer, &گهميل_conditions,
                                                         state)))
	أرجع خطأ;
    }

    /* Key size depends on distinct conditions, not expanded lookup sets.
     * Sorting makes it independent of their first traversal order. */
    state->qsort (_hb_cmp_operator<بنانشان, بنانشان>);
    أرجع سچ;
  }

  منطقي get_lookup_variations (بنانشان feature_اشاريو,
                              ثابت Feature &feature,
                              hb_sorted_array_t<ثابت بنانشان> conditions,
                              hb_سيٽ_t *lookup_indexes) ثابت
  {
    لكل (ثابت Lookup_Variation_رڪارڊ &رڪارڊ : get_lookup_variation_رڪارڊون ())
    {
      إذا (رڪارڊ.feature_اشاريو_2 != feature_اشاريو) استمر;

      رڪارڊ.collect_lookups (هذا, feature, conditions, lookup_indexes);
      أرجع سچ;
    }
    أرجع خطأ;
  }

  فراغ collect_feature_substitutes_with_variations (hb_collect_feature_substitutes_with_var_context_t *c) ثابت
  {
    بنانشان صحيح ڳڻپ = var_رڪارڊون.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      c->cur_رڪارڊ_idx = i;
      var_رڪارڊون[i].collect_feature_substitutes_with_variations (c, هذا);
      إذا (c->universal)
        اكسر;
    }
    إذا (c->universal || c->رڪارڊ_cond_idx_map->is_empty ())
      c->catch_all_رڪارڊ_feature_idxes.reset ();
  }

  FeatureVariations* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed (*هذا));
  }

  فراغ collect_lookups (ثابت hb_سيٽ_t *feature_indexes,
			ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map,
			hb_سيٽ_t       *lookup_indexes /*ٻاهريون*/) ثابت
  {
    بنانشان ڳڻپ = var_رڪارڊون.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (feature_رڪارڊ_cond_idx_map &&
          !feature_رڪارڊ_cond_idx_map->has (i))
        استمر;
      var_رڪارڊون[i].collect_lookups (هذا, feature_indexes, lookup_indexes);
    }

    collect_lookup_variation_lookups (feature_indexes, lookup_indexes);
  }

  سانچو <قسم_نالو ٽيگ_Getter>
  فراغ collect_name_ids (ثابت hb_map_t *feature_اشاريو_map,
                         ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map,
                         ٽيگ_Getter get_ٽيگ,
                         hb_سيٽ_t *nameids_منزل_retain) ثابت
  {
    لكل (بنانشان i = 0; i < var_رڪارڊون.len; i++)
    {
      إذا (feature_رڪارڊ_cond_idx_map && !feature_رڪارڊ_cond_idx_map->has (i))
        استمر;
      var_رڪارڊون[i].collect_name_ids (هذا, feature_اشاريو_map, get_ٽيگ, nameids_منزل_retain);
    }
  }

  فراغ collect_lookup_variation_lookups (ثابت hb_سيٽ_t *feature_indexes,
					 hb_سيٽ_t *lookup_indexes /*ٻاهريون*/) ثابت
  {
    لكل (ثابت Lookup_Variation_رڪارڊ &رڪارڊ : get_lookup_variation_رڪارڊون ())
      رڪارڊ.collect_lookups (هذا, feature_indexes, lookup_indexes);
  }

  فراغ گهيرو_features (ثابت hb_map_t *lookup_indexes,
			 ثابت hb_hashmap_t<بنانشان, hb::shared_ptr<hb_سيٽ_t>> *feature_رڪارڊ_cond_idx_map,
			 hb_سيٽ_t       *feature_indexes /*ٻاهريون*/) ثابت
  {
    بنانشان صحيح ڳڻپ = var_رڪارڊون.len;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ; i++)
    {
      إذا (feature_رڪارڊ_cond_idx_map != خالي_اشارو &&
          !feature_رڪارڊ_cond_idx_map->has (i))
        استمر;
      var_رڪارڊون[i].گهيرو_features (هذا, lookup_indexes, feature_indexes);
    }

    لكل (ثابت Lookup_Variation_رڪارڊ &رڪارڊ : get_lookup_variation_رڪارڊون ())
      رڪارڊ.گهيرو_features (هذا, lookup_indexes, feature_indexes);
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (ثابت Feature_Variation_رڪارڊ &رڪارڊ : var_رڪارڊون)
      رڪارڊ.collect_variation_indices (c, هذا);
    لكل (ثابت Lookup_Variation_رڪارڊ &رڪارڊ : get_lookup_variation_رڪارڊون ())
      رڪارڊ.collect_variation_indices (c, هذا);
  }

  فراغ collect_feature_indices (hb_سيٽ_t *feature_indices) ثابت
  {
    لكل (ثابت Feature_Variation_رڪارڊ &رڪارڊ : var_رڪارڊون)
      (هذا+رڪارڊ.substitutions).collect_feature_indices (feature_indices);
  }

  منطقي has_lookup_variations (بنانشان feature_اشاريو) ثابت
  {
    لكل (ثابت Lookup_Variation_رڪارڊ &رڪارڊ : get_lookup_variation_رڪارڊون ())
    {
      إذا (رڪارڊ.feature_اشاريو_2 == feature_اشاريو)
	أرجع سچ;
      إذا (رڪارڊ.feature_اشاريو_2 > feature_اشاريو)
	اكسر;
    }
    أرجع خطأ;
  }

  منطقي subset (hb_subset_context_t *c,
	       hb_subset_layout_context_t *l) ثابت
  {
    TRACE_SUBSET (هذا);
    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!ٻاهريون || !c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    ٻاهريون->نسخو.major = نسخو.major;
    ٻاهريون->نسخو.minor = نسخو.minor;

    صحيح keep_مٿي_منزل = -1;
    لكل (صحيح i = var_رڪارڊون.len - 1; i >= 0; i--) {
      إذا (var_رڪارڊون[i].intersects_features (هذا, l->feature_map_w_duplicates)) {
        keep_مٿي_منزل = i;
        اكسر;
      }
    }

    بنانشان ڳڻپ = (بنانشان) (keep_مٿي_منزل + 1);
    لكل (بنانشان i = 0; i < ڳڻپ; i++)
    {
      إذا (l->feature_رڪارڊ_cond_idx_map != خالي_اشارو &&
          !l->feature_رڪارڊ_cond_idx_map->has (i))
        استمر;

      l->cur_feature_var_رڪارڊ_idx = i;
      subset_رڪارڊ_array (l, &(ٻاهريون->var_رڪارڊون), هذا) (var_رڪارڊون[i]);
    }

    إذا (ٻاهريون->var_رڪارڊون.len && !l->catch_all_رڪارڊ_feature_idxes->is_empty ())
    {
      منطقي وجهو_catch_all_رڪارڊ = سچ;
      subset_رڪارڊ_array (l, &(ٻاهريون->var_رڪارڊون), هذا, وجهو_catch_all_رڪارڊ) (var_رڪارڊون[0]);
    }

    Array32_جو<Lookup_Variation_رڪارڊ> *ٻاهريون_lookup_رڪارڊون = خالي_اشارو;
    إذا (نسخو.minor >= 1)
    {
      ٻاهريون_lookup_رڪارڊون = &StructAfter<Array32_جو<Lookup_Variation_رڪارڊ>> (ٻاهريون->var_رڪارڊون);
      إذا (unlikely (!c->serializer->extend_min (ٻاهريون_lookup_رڪارڊون)))
	return_trace (خطأ);

      + get_lookup_variation_رڪارڊون ().iter ()
      | hb_apply (subset_رڪارڊ_array (l, ٻاهريون_lookup_رڪارڊون, هذا))
      ;
    }

    return_trace (منطقي (ٻاهريون->var_رڪارڊون) ||
		  (ٻاهريون_lookup_رڪارڊون && منطقي (*ٻاهريون_lookup_رڪارڊون)));
  }

  منطقي sanitize (hb_sanitize_context_t *c, ثابت FeatureList *feature_list) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (نسخو.sanitize (c) &&
		  hb_barrier () &&
		  likely (نسخو.major == 1) &&
		  var_رڪارڊون.sanitize (c, هذا, feature_list) &&
		  (نسخو.minor < 1 ||
		   get_lookup_variation_رڪارڊون ().sanitize (c, هذا)));
  }

  محمي:
  Fixed_نسخو<>	نسخو;	/* Version--0x00010000u */
  Array32_جو<Feature_Variation_رڪارڊ>
			var_رڪارڊون;
  عام:
  DEFINE_سائيز_ARRAY_SIZED (8, var_رڪارڊون);
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

  size_t get_سائيز () ثابت
  {
    بنانشان صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3 || شروعات_سائيز > پڇاڙي_سائيز)) أرجع 3 * HBUINT16::static_سائيز;
    أرجع HBUINT16::static_سائيز * (4 + ((پڇاڙي_سائيز - شروعات_سائيز) >> (4 - f)));
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) && c->check_range (هذا, هذا->get_سائيز ()));
  }

  HintingDevice* copy (hb_serialize_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);
    return_trace (c->embed<HintingDevice> (هذا));
  }

  خاص:

  صحيح get_delta (بنانشان صحيح ppem, صحيح scale) ثابت
  {
    إذا (!ppem) أرجع 0;

    صحيح pixels = get_delta_pixels (ppem);

    إذا (!pixels) أرجع 0;

    أرجع (صحيح) (pixels * (int64_t) scale / ppem);
  }
  صحيح get_delta_pixels (بنانشان صحيح ppem_سائيز) ثابت
  {
    بنانشان صحيح f = deltaFormat;
    إذا (unlikely (f < 1 || f > 3))
      أرجع 0;

    إذا (ppem_سائيز < شروعات_سائيز || ppem_سائيز > پڇاڙي_سائيز)
      أرجع 0;

    بنانشان صحيح s = ppem_سائيز - شروعات_سائيز;

    بنانشان صحيح byte = delta_قدر_Z[s >> (4 - f)];
    بنانشان صحيح بٽ = (byte >> (16 - (((s & ((1 << (4 - f)) - 1)) + 1) << f)));
    بنانشان صحيح نقاب = (0xFFFFu >> (16 - (1 << f)));

    صحيح delta = بٽ & نقاب;

    إذا ((بنانشان صحيح) delta >= ((نقاب + 1) >> 1))
      delta -= نقاب + 1;

    أرجع delta;
  }

  محمي:
  HBUINT16	شروعات_سائيز;		/* Smallest size to correct--in ppem */
  HBUINT16	پڇاڙي_سائيز;		/* Largest size to correct--in ppem */
  HBUINT16	deltaFormat;		/* Format of DeltaValue array data: 1, 2, or 3
					 * 1	Signed 2-bit value, 8 values per uint16
					 * 2	Signed 4-bit value, 4 values per uint16
					 * 3	Signed 8-bit value, 2 values per uint16
					 */
  Unsized_Array_جو<HBUINT16>
		delta_قدر_Z;		/* Array of compressed data */
  عام:
  DEFINE_سائيز_ARRAY (6, delta_قدر_Z);
};

بنية VariationDevice
{
  دوست بنية Device;

  خاص:

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت شي_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_x (get_delta (font, store, store_cache)); }

  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت شي_Variation_Store &store,
			     hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  { أرجع !font->has_nonzero_coords ? 0 : font->em_scalef_y (get_delta (font, store, store_cache)); }

  عائم get_x_delta_f (hb_font_t *font,
		       ثابت شي_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_x (get_delta (font, store, store_cache)); }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت شي_Variation_Store &store,
		       hb_scalar_cache_t *store_cache) ثابت
  { أرجع !font->has_nonzero_coords ? 0.f : font->em_fscalef_y (get_delta (font, store, store_cache)); }

  VariationDevice* copy (hb_serialize_context_t *c,
                         ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    TRACE_SERIALIZE (هذا);
    إذا (!layout_variation_idx_delta_map) return_trace (خالي_اشارو);

    hb_pair_t<بنانشان, صحيح> *v;
    إذا (!layout_variation_idx_delta_map->has (varIdx, &v))
      return_trace (خالي_اشارو);
    /* The varidx was mapped to "no variations" during instancing (e.g. the
     * instantiated row is all zeros). Drop the device instead of emitting one
     * with an out-of-range varidx. */
    إذا (hb_پهريون (*v) == HB_OT_LAYOUT_NO_VARIATIONS_اشاريو)
      return_trace (خالي_اشارو);

    c->شروعات_zerocopy (هذا->static_سائيز);
    خودڪار *ٻاهريون = c->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خالي_اشارو);

    إذا (!c->check_سونپيو (ٻاهريون->varIdx, hb_پهريون (*v), HB_SERIALIZE_ERROR_INT_حد_کان_مٿي))
      return_trace (خالي_اشارو);
    return_trace (ٻاهريون);
  }

  فراغ collect_variation_اشاريو (hb_collect_variation_indices_context_t *c) ثابت
  { c->layout_variation_indices->جوڙيو (varIdx); }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا));
  }

  خاص:

  عائم get_delta (hb_font_t *font,
		   ثابت شي_Variation_Store &store,
		   hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    أرجع store.get_delta (varIdx, font->coords, font->انگ_coords, store_cache);
  }

  محمي:
  VarIdx	varIdx;		/* Variation index */
  HBUINT16	deltaFormat;	/* Format identifier for this table: 0x0x8000 */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية Device_سرو
{
  محمي:
  HBUINT16		reserved1;
  HBUINT16		reserved2;
  عام:
  HBUINT16		format;		/* Format identifier */
  عام:
  DEFINE_سائيز_STATIC (6);
};

بنية Device
{
  // Anchors retain fractional coordinates until their difference is rounded.
  // Hinting adjustments and value records keep their integer semantics.
  عائم get_x_delta_f (hb_font_t *font,
		       ثابت شي_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_x_delta_f (font, store, store_cache);
    }
#جيڪڏهن_پورو
    أرجع get_x_delta (font, store, store_cache);
  }

  عائم get_y_delta_f (hb_font_t *font,
		       ثابت شي_Variation_Store &store,
		       hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    إذا (u.b.format == 0x8000)
    {
      hb_barrier ();
      أرجع u.variation.get_y_delta_f (font, store, store_cache);
    }
#جيڪڏهن_پورو
    أرجع get_y_delta (font, store, store_cache);
  }

  hb_position_t get_x_delta (hb_font_t *font,
			     ثابت شي_Variation_Store &store=Null (شي_Variation_Store),
			     hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    بدل (u.b.format)
    {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      أرجع u.hinting.get_x_delta (font);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_x_delta (font, store, store_cache);
#جيڪڏهن_پورو
    افتراضي:
      أرجع 0;
    }
  }
  hb_position_t get_y_delta (hb_font_t *font,
			     ثابت شي_Variation_Store &store=Null (شي_Variation_Store),
			     hb_scalar_cache_t *store_cache = خالي_اشارو) ثابت
  {
    بدل (u.b.format)
    {
    حالة 1: حالة 2: حالة 3:
#جيڪڏهن_وصف_نه_ٿيل HB_NO_HINTING
      hb_barrier ();
      أرجع u.hinting.get_y_delta (font);
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.get_y_delta (font, store, store_cache);
#جيڪڏهن_پورو
    افتراضي:
      أرجع 0;
    }
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (!u.b.format.sanitize (c)) return_trace (خطأ);
    بدل (u.b.format) {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_HINTING
    حالة 1: حالة 2: حالة 3:
      hb_barrier ();
      return_trace (u.hinting.sanitize (c));
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (u.variation.sanitize (c));
#جيڪڏهن_پورو
    افتراضي:
      return_trace (سچ);
    }
  }

  Device* copy (hb_serialize_context_t *c,
                ثابت hb_hashmap_t<بنانشان, hb_pair_t<بنانشان, صحيح>> *layout_variation_idx_delta_map=خالي_اشارو) ثابت
  {
    TRACE_SERIALIZE (هذا);
    بدل (u.b.format) {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.hinting.copy (c)));
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      return_trace (reinterpret_cast<Device *> (u.variation.copy (c, layout_variation_idx_delta_map)));
#جيڪڏهن_پورو
    افتراضي:
      return_trace (خالي_اشارو);
    }
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    بدل (u.b.format) {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_HINTING
    حالة 1:
    حالة 2:
    حالة 3:
      أرجع;
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      u.variation.collect_variation_اشاريو (c);
      أرجع;
#جيڪڏهن_پورو
    افتراضي:
      أرجع;
    }
  }

  بنانشان get_variation_اشاريو () ثابت
  {
    بدل (u.b.format) {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      hb_barrier ();
      أرجع u.variation.varIdx;
#جيڪڏهن_پورو
    افتراضي:
      أرجع HB_OT_LAYOUT_NO_VARIATIONS_اشاريو;
    }
  }

  منطقي is_variation_device () ثابت
  {
    بدل (u.b.format) {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
    حالة 0x8000:
      أرجع سچ;
#جيڪڏهن_پورو
    افتراضي:
      أرجع خطأ;
    }
  }

  محمي:
  اتحاد {
  Device_سرو		b;
  HintingDevice		hinting;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_VAR
  VariationDevice	variation;
#جيڪڏهن_پورو
  } u;
  عام:
  DEFINE_سائيز_UNION (6, b);
};


} /* namespace OT */


#جيڪڏهن_پورو /* HB_OT_LAYOUT_COMMON_HH */

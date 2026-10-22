#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#شامل "MarkArray.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

سانچہ <قسم_نام Types>
ساخت Mark_بنیاد_Pos_Format1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                بنیاد_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_گنتی_2;             /* Number of classes defined for marks */
  قسم_نام Types::سانچہ LOffset_مقصد<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  قسم_نام Types::سانچہ LOffset_مقصد<Anchor_میٹرکس<Types>>
                بنیاد_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عوامی:
  DEFINE_سائز_STATIC (4 + 4 * Types::LOffset::static_سائز);

    منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  markCoverage.sanitize (c, یہ) &&
                  بنیاد_Coverage.sanitize (c, یہ) &&
                  markArray.sanitize (c, یہ) &&
                  بنیاد_Array.sanitize (c, یہ, (غیرعلامتی صحیح) class_گنتی_2));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس (یہ+markCoverage).intersects (glyphs) &&
           (یہ+بنیاد_Coverage).intersects (glyphs);
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (یہ+markArray).collect_variation_indices (c, یہ+markCoverage);

    hb_map_t klass_mapping;
    Markclass_احاطہ_and_remap_indexes (یہ+markCoverage, یہ+markArray, *c->glyph_سیٹ, &klass_mapping);

    hb_sorted_سمتیہ_t<غیرعلامتی> بنیاد_indexes;
    collect_anchor_indexes (یہ+بنیاد_Coverage, (یہ+بنیاد_Array).صفیں,
                            class_گنتی_2, *c->glyph_سیٹ, klass_mapping,
                            بنیاد_indexes);
    (یہ+بنیاد_Array).collect_variation_indices (c, بنیاد_indexes.iter ());
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+markCoverage).collect_coverage (c->داخلہ))) واپس;
    اگر (unlikely (!(یہ+بنیاد_Coverage).collect_coverage (c->داخلہ))) واپس;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (یہ+بنیاد_Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+markCoverage; }

  ساکن اندرونی منطقی accept (hb_بفر_t *بفر, غیرعلامتی idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    واپس !_hb_glyph_info_multiplied (&بفر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&بفر->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&بفر->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&بفر->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&بفر->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&بفر->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&بفر->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&بفر->info[idx - 1]) + 1
	    );
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;
    غیرعلامتی صحیح mark_اشاریہ = (یہ+markCoverage).get_coverage  (بفر->cur().codepoint);
    اگر (likely (mark_اشاریہ == NOT_COVERED)) return_trace (جھوٹ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.سیٹ_lookup_props (LookupFlag::IgnoreMarks);

    اگر (c->آخری_بنیاد_until > بفر->idx)
    {
      c->آخری_بنیاد_until = 0;
      c->آخری_بنیاد = -1;
    }
    غیرعلامتی j;
    برائے (j = بفر->idx; j > c->آخری_بنیاد_until; j--)
    {
      خودکار مطابقت = skippy_iter.مطابقت (بفر->info[j - 1]);
      اگر (مطابقت == skippy_iter.مطابقت_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	اگر (!accept (بفر, j - 1) &&
	    NOT_COVERED == (یہ+بنیاد_Coverage).get_coverage  (بفر->info[j - 1].codepoint))
	  مطابقت = skippy_iter.SKIP;
      }
      اگر (مطابقت == skippy_iter.مطابقت_2)
      {
	c->آخری_بنیاد = (علامتی) j - 1;
	توڑو;
      }
    }
    c->آخری_بنیاد_until = بفر->idx;
    اگر (c->آخری_بنیاد == -1)
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (0, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    غیرعلامتی idx = (غیرعلامتی) c->آخری_بنیاد;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    غیرعلامتی صحیح بنیاد_اشاریہ = (یہ+بنیاد_Coverage).get_coverage  (بفر->info[idx].codepoint);
    اگر (بنیاد_اشاریہ == NOT_COVERED)
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    return_trace ((یہ+markArray).apply (c, mark_اشاریہ, بنیاد_اشاریہ, یہ+بنیاد_Array, class_گنتی_2, idx));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    hb_map_t klass_mapping;
    Markclass_احاطہ_and_remap_indexes (یہ+markCoverage, یہ+markArray, glyphset, &klass_mapping);

    اگر (!klass_mapping.get_population ()) return_trace (جھوٹ);
    برآمد->class_گنتی_2 = klass_mapping.get_population ();

    خودکار mark_iter =
    + hb_zip (یہ+markCoverage, یہ+markArray)
    | hb_filter (glyphset, hb_پہلا)
    ;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    اگر (!برآمد->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (جھوٹ);

    اگر (unlikely (!برآمد->markArray.serialize_subset (c, markArray, یہ,
						    (یہ+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (جھوٹ);

    new_coverage.reset ();
    hb_sorted_سمتیہ_t<غیرعلامتی> بنیاد_indexes;
    ثابت خودکار &بنیاد_array = یہ+بنیاد_Array;
    collect_subset_anchor_indexes (یہ+بنیاد_Coverage, بنیاد_array.صفیں,
                                   class_گنتی_2, بنیاد_array.میٹرکس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, بنیاد_indexes);

    اگر (!new_coverage) return_trace (جھوٹ);
    اگر (!برآمد->بنیاد_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (جھوٹ);

    return_trace (برآمد->بنیاد_Array.serialize_subset (c, بنیاد_Array, یہ,
						   new_coverage.لمبائی,
						   بنیاد_indexes.iter ()));
  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */

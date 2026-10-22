#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#شامل "MarkMarkPosFormat1.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

سانچہ <قسم_نام Types>
ساخت MarkMarkPosFormat1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_گنتی_2;             /* Number of defined mark classes */
  قسم_نام Types::سانچہ LOffset_مقصد<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  قسم_نام Types::سانچہ LOffset_مقصد<Anchor_میٹرکس<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  عوامی:
  DEFINE_سائز_STATIC (4 + 4 * Types::LOffset::static_سائز);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  mark1Coverage.sanitize (c, یہ) &&
                  mark2Coverage.sanitize (c, یہ) &&
                  mark1Array.sanitize (c, یہ) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, یہ, (غیرعلامتی صحیح) class_گنتی_2));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس (یہ+mark1Coverage).intersects (glyphs) &&
           (یہ+mark2Coverage).intersects (glyphs);
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (یہ+mark1Array).collect_variation_indices (c, یہ+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_احاطہ_and_remap_indexes (یہ+mark1Coverage, یہ+mark1Array, *c->glyph_سیٹ, &klass_mapping);

    hb_sorted_سمتیہ_t<غیرعلامتی> mark2_indexes;
    collect_anchor_indexes (یہ+mark2Coverage, (یہ+mark2Array).صفیں,
                            class_گنتی_2, *c->glyph_سیٹ, klass_mapping,
                            mark2_indexes);
    (یہ+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+mark1Coverage).collect_coverage (c->داخلہ))) واپس;
    اگر (unlikely (!(یہ+mark2Coverage).collect_coverage (c->داخلہ))) واپس;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (یہ+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+mark1Coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;
    غیرعلامتی صحیح mark1_اشاریہ = (یہ+mark1Coverage).get_coverage  (بفر->cur().codepoint);
    اگر (likely (mark1_اشاریہ == NOT_COVERED)) return_trace (جھوٹ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    skippy_iter.سیٹ_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    غیرعلامتی unsafe_from;
    اگر (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (unsafe_from, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    اگر (likely (!_hb_glyph_info_is_mark (&بفر->info[skippy_iter.idx])))
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    غیرعلامتی صحیح j = skippy_iter.idx;

    غیرعلامتی صحیح id1 = _hb_glyph_info_get_lig_id (&بفر->cur());
    غیرعلامتی صحیح id2 = _hb_glyph_info_get_lig_id (&بفر->info[j]);
    غیرعلامتی صحیح comp1 = _hb_glyph_info_get_lig_comp (&بفر->cur());
    غیرعلامتی صحیح comp2 = _hb_glyph_info_get_lig_comp (&بفر->info[j]);

    اگر (likely (id1 == id2))
    {
      اگر (id1 == 0) /* Marks belonging to the same base. */
        جاؤ good;
      ورنہ اگر (comp1 == comp2) /* Marks belonging to the same ligature component. */
        جاؤ good;
    }
    ورنہ
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      اگر ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        جاؤ good;
    }

    /* Didn't match. */
    بفر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
    return_trace (جھوٹ);

    good:
    غیرعلامتی صحیح mark2_اشاریہ = (یہ+mark2Coverage).get_coverage  (بفر->info[j].codepoint);
    اگر (mark2_اشاریہ == NOT_COVERED)
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    return_trace ((یہ+mark1Array).apply (c, mark1_اشاریہ, mark2_اشاریہ, یہ+mark2Array, class_گنتی_2, j));
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
    Markclass_احاطہ_and_remap_indexes (یہ+mark1Coverage, یہ+mark1Array, glyphset, &klass_mapping);

    اگر (!klass_mapping.get_population ()) return_trace (جھوٹ);
    برآمد->class_گنتی_2 = klass_mapping.get_population ();

    خودکار mark1_iter =
    + hb_zip (یہ+mark1Coverage, یہ+mark1Array)
    | hb_filter (glyphset, hb_پہلا)
    ;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    اگر (!برآمد->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (جھوٹ);

    اگر (unlikely (!برآمد->mark1Array.serialize_subset (c, mark1Array, یہ,
						     (یہ+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (جھوٹ);

    new_coverage.reset ();
    hb_sorted_سمتیہ_t<غیرعلامتی> mark2_indexes;
    ثابت خودکار &mark2_array = یہ+mark2Array;
    collect_subset_anchor_indexes (یہ+mark2Coverage, mark2_array.صفیں,
                                   class_گنتی_2, mark2_array.میٹرکس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    اگر (!new_coverage) return_trace (جھوٹ);
    اگر (!برآمد->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (جھوٹ);

    return_trace (برآمد->mark2Array.serialize_subset (c, mark2Array, یہ,
						    new_coverage.لمبائی,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */

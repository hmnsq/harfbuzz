#اگر_تعریف_نشده OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ضمن "MarkMarkPosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

قالب <نام_نوع Types>
بنية MarkMarkPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_شمار_2;             /* Number of defined mark classes */
  نام_نوع Types::قالب LOffset_مقصد<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  نام_نوع Types::قالب LOffset_مقصد<Anchor_ماتریس<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  عام:
  DEFINE_اندازه_STATIC (4 + 4 * Types::LOffset::static_اندازه);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  mark1Coverage.sanitize (c, هذا) &&
                  mark2Coverage.sanitize (c, هذا) &&
                  mark1Array.sanitize (c, هذا) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, هذا, (بیعلامت صحيح) class_شمار_2));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  {
    أرجع (هذا+mark1Coverage).intersects (glyphs) &&
           (هذا+mark2Coverage).intersects (glyphs);
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mark1Array).collect_variation_indices (c, هذا+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_بستگی_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, *c->glyph_مجموعه, &klass_mapping);

    hb_sorted_وکتور_t<بیعلامت> mark2_indexes;
    collect_anchor_indexes (هذا+mark2Coverage, (هذا+mark2Array).ردیفها,
                            class_شمار_2, *c->glyph_مجموعه, klass_mapping,
                            mark2_indexes);
    (هذا+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+mark1Coverage).collect_coverage (c->درون_داد))) أرجع;
    إذا (unlikely (!(هذا+mark2Coverage).collect_coverage (c->درون_داد))) أرجع;
  }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  { (هذا+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+mark1Coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;
    بیعلامت صحيح mark1_شاخص = (هذا+mark1Coverage).get_coverage  (میانگیر->cur().codepoint);
    إذا (likely (mark1_شاخص == NOT_COVERED)) return_trace (خطأ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    خودکار &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (میانگیر->idx);
    skippy_iter.مجموعه_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    بیعلامت unsafe_from;
    إذا (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (unsafe_from, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    إذا (likely (!_hb_glyph_info_is_mark (&میانگیر->info[skippy_iter.idx])))
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    بیعلامت صحيح j = skippy_iter.idx;

    بیعلامت صحيح id1 = _hb_glyph_info_get_lig_id (&میانگیر->cur());
    بیعلامت صحيح id2 = _hb_glyph_info_get_lig_id (&میانگیر->info[j]);
    بیعلامت صحيح comp1 = _hb_glyph_info_get_lig_comp (&میانگیر->cur());
    بیعلامت صحيح comp2 = _hb_glyph_info_get_lig_comp (&میانگیر->info[j]);

    إذا (likely (id1 == id2))
    {
      إذا (id1 == 0) /* Marks belonging to the same base. */
        اذهب good;
      وإلا إذا (comp1 == comp2) /* Marks belonging to the same ligature component. */
        اذهب good;
    }
    وإلا
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      إذا ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        اذهب good;
    }

    /* Didn't match. */
    میانگیر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, میانگیر->idx + 1);
    return_trace (خطأ);

    good:
    بیعلامت صحيح mark2_شاخص = (هذا+mark2Coverage).get_coverage  (میانگیر->info[j].codepoint);
    إذا (mark2_شاخص == NOT_COVERED)
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+mark1Array).apply (c, mark1_شاخص, mark2_شاخص, هذا+mark2Array, class_شمار_2, j));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);
    بیرون->format = format;

    hb_map_t klass_mapping;
    Markclass_بستگی_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    بیرون->class_شمار_2 = klass_mapping.get_population ();

    خودکار mark1_iter =
    + hb_zip (هذا+mark1Coverage, هذا+mark1Array)
    | hb_filter (glyphset, hb_اولی)
    ;

    hb_sorted_وکتور_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_اولی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!بیرون->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!بیرون->mark1Array.serialize_subset (c, mark1Array, هذا,
						     (هذا+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_وکتور_t<بیعلامت> mark2_indexes;
    ثابت خودکار &mark2_array = هذا+mark2Array;
    collect_subset_anchor_indexes (هذا+mark2Coverage, mark2_array.ردیفها,
                                   class_شمار_2, mark2_array.ماتریس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!بیرون->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (بیرون->mark2Array.serialize_subset (c, mark2Array, هذا,
						    new_coverage.درازی,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */

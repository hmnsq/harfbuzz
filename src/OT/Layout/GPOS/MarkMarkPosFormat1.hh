#که_نه_تعریف_شوی OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ضمن "MarkMarkPosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

کينډۍ <ډول_نوم Types>
بنية MarkMarkPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_شمېر_2;             /* Number of defined mark classes */
  ډول_نوم Types::کينډۍ LOffset_منزل<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  ډول_نوم Types::کينډۍ LOffset_منزل<Anchor_ماتریکس<Types, HBUINT16>>
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
                  mark2Array.sanitize (c, هذا, (بینښه صحيح) class_شمېر_2));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  {
    أرجع (هذا+mark1Coverage).intersects (glyphs) &&
           (هذا+mark2Coverage).intersects (glyphs);
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mark1Array).collect_variation_indices (c, هذا+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_تړون_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, *c->glyph_سېټ, &klass_mapping);

    hb_sorted_ویکټور_t<بینښه> mark2_indexes;
    collect_anchor_indexes (هذا+mark2Coverage, (هذا+mark2Array).لیکې,
                            class_شمېر_2, *c->glyph_سېټ, klass_mapping,
                            mark2_indexes);
    (هذا+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+mark1Coverage).collect_coverage (c->ننوت))) أرجع;
    إذا (unlikely (!(هذا+mark2Coverage).collect_coverage (c->ننوت))) أرجع;
  }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  { (هذا+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+mark1Coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بینښه صحيح mark1_شاخص = (هذا+mark1Coverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark1_شاخص == NOT_COVERED)) return_trace (خطأ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    خپلکاری &skippy_iter = c->iter_ننوت;
    skippy_iter.reset_fast (بفر->idx);
    skippy_iter.سېټ_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    بینښه unsafe_from;
    إذا (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (unsafe_from, بفر->idx + 1);
      return_trace (خطأ);
    }

    إذا (likely (!_hb_glyph_info_is_mark (&بفر->info[skippy_iter.idx])))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    بینښه صحيح j = skippy_iter.idx;

    بینښه صحيح id1 = _hb_glyph_info_get_lig_id (&بفر->cur());
    بینښه صحيح id2 = _hb_glyph_info_get_lig_id (&بفر->info[j]);
    بینښه صحيح comp1 = _hb_glyph_info_get_lig_comp (&بفر->cur());
    بینښه صحيح comp2 = _hb_glyph_info_get_lig_comp (&بفر->info[j]);

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
    بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
    return_trace (خطأ);

    good:
    بینښه صحيح mark2_شاخص = (هذا+mark2Coverage).get_coverage  (بفر->info[j].codepoint);
    إذا (mark2_شاخص == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+mark1Array).apply (c, mark1_شاخص, mark2_شاخص, هذا+mark2Array, class_شمېر_2, j));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);
    بهر->format = format;

    hb_map_t klass_mapping;
    Markclass_تړون_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    بهر->class_شمېر_2 = klass_mapping.get_population ();

    خپلکاری mark1_iter =
    + hb_zip (هذا+mark1Coverage, هذا+mark1Array)
    | hb_filter (glyphset, hb_لومړی)
    ;

    hb_sorted_ویکټور_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_لومړی)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!بهر->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!بهر->mark1Array.serialize_subset (c, mark1Array, هذا,
						     (هذا+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_ویکټور_t<بینښه> mark2_indexes;
    ثابت خپلکاری &mark2_array = هذا+mark2Array;
    collect_subset_anchor_indexes (هذا+mark2Coverage, mark2_array.لیکې,
                                   class_شمېر_2, mark2_array.ماتریکس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!بهر->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (بهر->mark2Array.serialize_subset (c, mark2Array, هذا,
						    new_coverage.اوږدتیا,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */

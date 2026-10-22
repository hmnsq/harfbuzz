#جے_بنا_تعریف OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ضمن "MarkMarkPosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچا <قسم_ناں Types>
بنية MarkMarkPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_گنتی_2;             /* Number of defined mark classes */
  قسم_ناں Types::سانچا LOffset_منزل<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  قسم_ناں Types::سانچا LOffset_منزل<Anchor_میٹرکس<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  عام:
  DEFINE_ناپ_STATIC (4 + 4 * Types::LOffset::static_ناپ);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  mark1Coverage.sanitize (c, هذا) &&
                  mark2Coverage.sanitize (c, هذا) &&
                  mark1Array.sanitize (c, هذا) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, هذا, (بنانشانی صحيح) class_گنتی_2));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    أرجع (هذا+mark1Coverage).intersects (glyphs) &&
           (هذا+mark2Coverage).intersects (glyphs);
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mark1Array).collect_variation_indices (c, هذا+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_گھیرا_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, *c->glyph_سیٹ, &klass_mapping);

    hb_sorted_سمتیہ_t<بنانشانی> mark2_indexes;
    collect_anchor_indexes (هذا+mark2Coverage, (هذا+mark2Array).صفاں,
                            class_گنتی_2, *c->glyph_سیٹ, klass_mapping,
                            mark2_indexes);
    (هذا+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+mark1Coverage).collect_coverage (c->داخلہ))) أرجع;
    إذا (unlikely (!(هذا+mark2Coverage).collect_coverage (c->داخلہ))) أرجع;
  }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (هذا+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+mark1Coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشانی صحيح mark1_اشاریہ = (هذا+mark1Coverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark1_اشاریہ == NOT_COVERED)) return_trace (خطأ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    skippy_iter.سیٹ_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    بنانشانی unsafe_from;
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

    بنانشانی صحيح j = skippy_iter.idx;

    بنانشانی صحيح id1 = _hb_glyph_info_get_lig_id (&بفر->cur());
    بنانشانی صحيح id2 = _hb_glyph_info_get_lig_id (&بفر->info[j]);
    بنانشانی صحيح comp1 = _hb_glyph_info_get_lig_comp (&بفر->cur());
    بنانشانی صحيح comp2 = _hb_glyph_info_get_lig_comp (&بفر->info[j]);

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
    بنانشانی صحيح mark2_اشاریہ = (هذا+mark2Coverage).get_coverage  (بفر->info[j].codepoint);
    إذا (mark2_اشاریہ == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+mark1Array).apply (c, mark1_اشاریہ, mark2_اشاریہ, هذا+mark2Array, class_گنتی_2, j));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);
    باہرلا->format = format;

    hb_map_t klass_mapping;
    Markclass_گھیرا_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    باہرلا->class_گنتی_2 = klass_mapping.get_population ();

    خودکار mark1_iter =
    + hb_zip (هذا+mark1Coverage, هذا+mark1Array)
    | hb_filter (glyphset, hb_پہلا)
    ;

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_پہلا)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!باہرلا->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!باہرلا->mark1Array.serialize_subset (c, mark1Array, هذا,
						     (هذا+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_سمتیہ_t<بنانشانی> mark2_indexes;
    ثابت خودکار &mark2_array = هذا+mark2Array;
    collect_subset_anchor_indexes (هذا+mark2Coverage, mark2_array.صفاں,
                                   class_گنتی_2, mark2_array.میٹرکس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!باہرلا->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (باہرلا->mark2Array.serialize_subset (c, mark2Array, هذا,
						    new_coverage.لمائی,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */

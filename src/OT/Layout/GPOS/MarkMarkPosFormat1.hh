#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ضمن "MarkMarkPosFormat1.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچو <قسم_نالو Types>
بنية MarkMarkPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_ڳڻپ_2;             /* Number of defined mark classes */
  قسم_نالو Types::سانچو LOffset_منزل<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  قسم_نالو Types::سانچو LOffset_منزل<Anchor_ميٽرڪس<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  عام:
  DEFINE_سائيز_STATIC (4 + 4 * Types::LOffset::static_سائيز);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  mark1Coverage.sanitize (c, هذا) &&
                  mark2Coverage.sanitize (c, هذا) &&
                  mark1Array.sanitize (c, هذا) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, هذا, (بنانشان صحيح) class_ڳڻپ_2));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    أرجع (هذا+mark1Coverage).intersects (glyphs) &&
           (هذا+mark2Coverage).intersects (glyphs);
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+mark1Array).collect_variation_indices (c, هذا+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_گهيرو_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, *c->glyph_سيٽ, &klass_mapping);

    hb_sorted_ويڪٽر_t<بنانشان> mark2_indexes;
    collect_anchor_indexes (هذا+mark2Coverage, (هذا+mark2Array).صفون,
                            class_ڳڻپ_2, *c->glyph_سيٽ, klass_mapping,
                            mark2_indexes);
    (هذا+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+mark1Coverage).collect_coverage (c->داخلا_2))) أرجع;
    إذا (unlikely (!(هذا+mark2Coverage).collect_coverage (c->داخلا_2))) أرجع;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  { (هذا+mark2Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+mark1Coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشان صحيح mark1_اشاريو = (هذا+mark1Coverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark1_اشاريو == NOT_COVERED)) return_trace (خطأ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    خودڪار &skippy_iter = c->iter_داخلا;
    skippy_iter.reset_fast (بفر->idx);
    skippy_iter.سيٽ_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    بنانشان unsafe_from;
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

    بنانشان صحيح j = skippy_iter.idx;

    بنانشان صحيح id1 = _hb_glyph_info_get_lig_id (&بفر->cur());
    بنانشان صحيح id2 = _hb_glyph_info_get_lig_id (&بفر->info[j]);
    بنانشان صحيح comp1 = _hb_glyph_info_get_lig_comp (&بفر->cur());
    بنانشان صحيح comp2 = _hb_glyph_info_get_lig_comp (&بفر->info[j]);

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
    بنانشان صحيح mark2_اشاريو = (هذا+mark2Coverage).get_coverage  (بفر->info[j].codepoint);
    إذا (mark2_اشاريو == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+mark1Array).apply (c, mark1_اشاريو, mark2_اشاريو, هذا+mark2Array, class_ڳڻپ_2, j));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    hb_map_t klass_mapping;
    Markclass_گهيرو_and_remap_indexes (هذا+mark1Coverage, هذا+mark1Array, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    ٻاهريون->class_ڳڻپ_2 = klass_mapping.get_population ();

    خودڪار mark1_iter =
    + hb_zip (هذا+mark1Coverage, هذا+mark1Array)
    | hb_filter (glyphset, hb_پهريون)
    ;

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!ٻاهريون->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!ٻاهريون->mark1Array.serialize_subset (c, mark1Array, هذا,
						     (هذا+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_ويڪٽر_t<بنانشان> mark2_indexes;
    ثابت خودڪار &mark2_array = هذا+mark2Array;
    collect_subset_anchor_indexes (هذا+mark2Coverage, mark2_array.صفون,
                                   class_ڳڻپ_2, mark2_array.ميٽرڪس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!ٻاهريون->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (ٻاهريون->mark2Array.serialize_subset (c, mark2Array, هذا,
						    new_coverage.ڊيگهائي,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */

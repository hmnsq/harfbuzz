#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ضمن "MarkArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچو <قسم_نالو Types>
بنية Mark_بنياد_Pos_Format1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                بنياد_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_ڳڻپ_2;             /* Number of classes defined for marks */
  قسم_نالو Types::سانچو LOffset_منزل<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  قسم_نالو Types::سانچو LOffset_منزل<Anchor_ميٽرڪس<Types>>
                بنياد_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  عام:
  DEFINE_سائيز_STATIC (4 + 4 * Types::LOffset::static_سائيز);

    منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  بنياد_Coverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  بنياد_Array.sanitize (c, هذا, (بنانشان صحيح) class_ڳڻپ_2));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+بنياد_Coverage).intersects (glyphs);
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_گهيرو_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_سيٽ, &klass_mapping);

    hb_sorted_ويڪٽر_t<بنانشان> بنياد_indexes;
    collect_anchor_indexes (هذا+بنياد_Coverage, (هذا+بنياد_Array).صفون,
                            class_ڳڻپ_2, *c->glyph_سيٽ, klass_mapping,
                            بنياد_indexes);
    (هذا+بنياد_Array).collect_variation_indices (c, بنياد_indexes.iter ());
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->داخلا_2))) أرجع;
    إذا (unlikely (!(هذا+بنياد_Coverage).collect_coverage (c->داخلا_2))) أرجع;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  { (هذا+بنياد_Coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_بفر_t *بفر, بنانشان idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    أرجع !_hb_glyph_info_multiplied (&بفر->info[idx]) ||
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

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشان صحيح mark_اشاريو = (هذا+markCoverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark_اشاريو == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

    خودڪار &skippy_iter = c->iter_داخلا;
    skippy_iter.سيٽ_lookup_props (LookupFlag::IgnoreMarks);

    إذا (c->آخري_بنياد_until > بفر->idx)
    {
      c->آخري_بنياد_until = 0;
      c->آخري_بنياد = -1;
    }
    بنانشان j;
    لكل (j = بفر->idx; j > c->آخري_بنياد_until; j--)
    {
      خودڪار ميلاپ = skippy_iter.ميلاپ (بفر->info[j - 1]);
      إذا (ميلاپ == skippy_iter.ميلاپ_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	إذا (!accept (بفر, j - 1) &&
	    NOT_COVERED == (هذا+بنياد_Coverage).get_coverage  (بفر->info[j - 1].codepoint))
	  ميلاپ = skippy_iter.SKIP;
      }
      إذا (ميلاپ == skippy_iter.ميلاپ_2)
      {
	c->آخري_بنياد = (نشانوارو) j - 1;
	اكسر;
      }
    }
    c->آخري_بنياد_until = بفر->idx;
    إذا (c->آخري_بنياد == -1)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (0, بفر->idx + 1);
      return_trace (خطأ);
    }

    بنانشان idx = (بنانشان) c->آخري_بنياد;

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    بنانشان صحيح بنياد_اشاريو = (هذا+بنياد_Coverage).get_coverage  (بفر->info[idx].codepoint);
    إذا (بنياد_اشاريو == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    return_trace ((هذا+markArray).apply (c, mark_اشاريو, بنياد_اشاريو, هذا+بنياد_Array, class_ڳڻپ_2, idx));
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
    Markclass_گهيرو_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    ٻاهريون->class_ڳڻپ_2 = klass_mapping.get_population ();

    خودڪار mark_iter =
    + hb_zip (هذا+markCoverage, هذا+markArray)
    | hb_filter (glyphset, hb_پهريون)
    ;

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_پهريون)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    إذا (!ٻاهريون->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    إذا (unlikely (!ٻاهريون->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    new_coverage.reset ();
    hb_sorted_ويڪٽر_t<بنانشان> بنياد_indexes;
    ثابت خودڪار &بنياد_array = هذا+بنياد_Array;
    collect_subset_anchor_indexes (هذا+بنياد_Coverage, بنياد_array.صفون,
                                   class_ڳڻپ_2, بنياد_array.ميٽرڪس_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, بنياد_indexes);

    إذا (!new_coverage) return_trace (خطأ);
    إذا (!ٻاهريون->بنياد_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (خطأ);

    return_trace (ٻاهريون->بنياد_Array.serialize_subset (c, بنياد_Array, هذا,
						   new_coverage.ڊيگهائي,
						   بنياد_indexes.iter ()));
  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */

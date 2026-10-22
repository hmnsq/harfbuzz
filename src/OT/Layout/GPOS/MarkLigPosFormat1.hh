#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ضمن "LigatureArray.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {


سانچو <قسم_نالو Types>
بنية MarkLigPosFormat1_2
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_ڳڻپ_2;             /* Number of defined mark classes */
  قسم_نالو Types::سانچو LOffset_منزل<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  قسم_نالو Types::سانچو LOffset_منزل<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  عام:
  DEFINE_سائيز_STATIC (4 + 4 * Types::LOffset::static_سائيز);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
                  markCoverage.sanitize (c, هذا) &&
                  ligatureCoverage.sanitize (c, هذا) &&
                  markArray.sanitize (c, هذا) &&
                  ligatureArray.sanitize (c, هذا, (بنانشان صحيح) class_ڳڻپ_2));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  {
    أرجع (هذا+markCoverage).intersects (glyphs) &&
           (هذا+ligatureCoverage).intersects (glyphs);
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (هذا+markArray).collect_variation_indices (c, هذا+markCoverage);

    hb_map_t klass_mapping;
    Markclass_گهيرو_and_remap_indexes (هذا+markCoverage, هذا+markArray, *c->glyph_سيٽ, &klass_mapping);

    (هذا+ligatureArray).collect_variation_indices (c, هذا+ligatureCoverage,
                                                   class_ڳڻپ_2, klass_mapping);
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+markCoverage).collect_coverage (c->داخلا_2))) أرجع;
    إذا (unlikely (!(هذا+ligatureCoverage).collect_coverage (c->داخلا_2))) أرجع;
  }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  { (هذا+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+markCoverage; }

  ساكن مضمن منطقي accept (hb_بفر_t *بفر, بنانشان idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    أرجع !_hb_glyph_info_multiplied (&بفر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&بفر->info[idx]);
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;
    بنانشان صحيح mark_اشاريو = (هذا+markCoverage).get_coverage  (بفر->cur().codepoint);
    إذا (likely (mark_اشاريو == NOT_COVERED)) return_trace (خطأ);

    /* Now we search backwards for a non-mark glyph */

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
	    NOT_COVERED == (هذا+ligatureCoverage).get_coverage  (بفر->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    بنانشان صحيح lig_اشاريو = (هذا+ligatureCoverage).get_coverage  (بفر->info[idx].codepoint);
    إذا (lig_اشاريو == NOT_COVERED)
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    ثابت LigatureArray<Types>& lig_array = هذا+ligatureArray;
    ثابت Anchor_ميٽرڪس<Types, HBUINT16>& lig_attach = lig_array[lig_اشاريو];

    /* Find component to attach to */
    بنانشان صحيح comp_ڳڻپ = lig_attach.صفون;
    إذا (unlikely (!comp_ڳڻپ))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (خطأ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    بنانشان صحيح comp_اشاريو;
    بنانشان صحيح lig_id = _hb_glyph_info_get_lig_id (&بفر->info[idx]);
    بنانشان صحيح mark_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    بنانشان صحيح mark_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
    إذا (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_اشاريو = hb_min (comp_ڳڻپ, _hb_glyph_info_get_lig_comp (&بفر->cur())) - 1;
    وإلا
      comp_اشاريو = comp_ڳڻپ - 1;

    return_trace ((هذا+markArray).apply (c, mark_اشاريو, comp_اشاريو, lig_attach, class_ڳڻپ_2, idx));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);
    ٻاهريون->format = format;

    hb_map_t klass_mapping;
    Markclass_گهيرو_and_remap_indexes (هذا+markCoverage, هذا+markArray, glyphset, &klass_mapping);

    إذا (!klass_mapping.get_population ()) return_trace (خطأ);
    ٻاهريون->class_ڳڻپ_2 = klass_mapping.get_population ();

    خودڪار mark_iter =
    + hb_zip (هذا+markCoverage, hb_range ((بنانشان) (هذا+markArray).len))
    | hb_filter (glyphset, hb_پهريون)
    ;

    خودڪار new_mark_coverage =
    + mark_iter
    | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
    | hb_map_retains_ترتيب_ڏيڻ (glyph_map)
    ;

    إذا (!ٻاهريون->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (خطأ);

    إذا (unlikely (!ٻاهريون->markArray.serialize_subset (c, markArray, هذا,
						    (هذا+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (خطأ);

    hb_sorted_ويڪٽر_t<hb_codepoint_t> new_lig_coverage;
    إذا (!ٻاهريون->ligatureArray.serialize_subset (c, ligatureArray, هذا,
					      hb_iter (هذا+ligatureCoverage),
					      class_ڳڻپ_2, &klass_mapping, new_lig_coverage))
      return_trace (خطأ);

    return_trace (ٻاهريون->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */

#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#شامل "LigatureArray.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {


سانچہ <قسم_نام Types>
ساخت MarkLigPosFormat1_2
{
  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_گنتی_2;             /* Number of defined mark classes */
  قسم_نام Types::سانچہ LOffset_مقصد<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  قسم_نام Types::سانچہ LOffset_مقصد<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  عوامی:
  DEFINE_سائز_STATIC (4 + 4 * Types::LOffset::static_سائز);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
                  markCoverage.sanitize (c, یہ) &&
                  ligatureCoverage.sanitize (c, یہ) &&
                  markArray.sanitize (c, یہ) &&
                  ligatureArray.sanitize (c, یہ, (غیرعلامتی صحیح) class_گنتی_2));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  {
    واپس (یہ+markCoverage).intersects (glyphs) &&
           (یہ+ligatureCoverage).intersects (glyphs);
  }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    (یہ+markArray).collect_variation_indices (c, یہ+markCoverage);

    hb_map_t klass_mapping;
    Markclass_احاطہ_and_remap_indexes (یہ+markCoverage, یہ+markArray, *c->glyph_سیٹ, &klass_mapping);

    (یہ+ligatureArray).collect_variation_indices (c, یہ+ligatureCoverage,
                                                   class_گنتی_2, klass_mapping);
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(یہ+markCoverage).collect_coverage (c->داخلہ))) واپس;
    اگر (unlikely (!(یہ+ligatureCoverage).collect_coverage (c->داخلہ))) واپس;
  }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (یہ+ligatureCoverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+markCoverage; }

  ساکن اندرونی منطقی accept (hb_بفر_t *بفر, غیرعلامتی idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    واپس !_hb_glyph_info_multiplied (&بفر->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&بفر->info[idx]);
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;
    غیرعلامتی صحیح mark_اشاریہ = (یہ+markCoverage).get_coverage  (بفر->cur().codepoint);
    اگر (likely (mark_اشاریہ == NOT_COVERED)) return_trace (جھوٹ);

    /* Now we search backwards for a non-mark glyph */

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
	    NOT_COVERED == (یہ+ligatureCoverage).get_coverage  (بفر->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    غیرعلامتی صحیح lig_اشاریہ = (یہ+ligatureCoverage).get_coverage  (بفر->info[idx].codepoint);
    اگر (lig_اشاریہ == NOT_COVERED)
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    ثابت LigatureArray<Types>& lig_array = یہ+ligatureArray;
    ثابت Anchor_میٹرکس<Types, HBUINT16>& lig_attach = lig_array[lig_اشاریہ];

    /* Find component to attach to */
    غیرعلامتی صحیح comp_گنتی = lig_attach.صفیں;
    اگر (unlikely (!comp_گنتی))
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (idx, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    غیرعلامتی صحیح comp_اشاریہ;
    غیرعلامتی صحیح lig_id = _hb_glyph_info_get_lig_id (&بفر->info[idx]);
    غیرعلامتی صحیح mark_id = _hb_glyph_info_get_lig_id (&بفر->cur());
    غیرعلامتی صحیح mark_comp = _hb_glyph_info_get_lig_comp (&بفر->cur());
    اگر (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_اشاریہ = hb_min (comp_گنتی, _hb_glyph_info_get_lig_comp (&بفر->cur())) - 1;
    ورنہ
      comp_اشاریہ = comp_گنتی - 1;

    return_trace ((یہ+markArray).apply (c, mark_اشاریہ, comp_اشاریہ, lig_attach, class_گنتی_2, idx));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);
    برآمد->format = format;

    hb_map_t klass_mapping;
    Markclass_احاطہ_and_remap_indexes (یہ+markCoverage, یہ+markArray, glyphset, &klass_mapping);

    اگر (!klass_mapping.get_population ()) return_trace (جھوٹ);
    برآمد->class_گنتی_2 = klass_mapping.get_population ();

    خودکار mark_iter =
    + hb_zip (یہ+markCoverage, hb_range ((غیرعلامتی) (یہ+markArray).len))
    | hb_filter (glyphset, hb_پہلا)
    ;

    خودکار new_mark_coverage =
    + mark_iter
    | hb_map_retains_ترتیب_کاری (hb_پہلا)
    | hb_map_retains_ترتیب_کاری (glyph_map)
    ;

    اگر (!برآمد->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (جھوٹ);

    اگر (unlikely (!برآمد->markArray.serialize_subset (c, markArray, یہ,
						    (یہ+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (جھوٹ);

    hb_sorted_سمتیہ_t<hb_codepoint_t> new_lig_coverage;
    اگر (!برآمد->ligatureArray.serialize_subset (c, ligatureArray, یہ,
					      hb_iter (یہ+ligatureCoverage),
					      class_گنتی_2, &klass_mapping, new_lig_coverage))
      return_trace (جھوٹ);

    return_trace (برآمد->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "LigatureArray.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {


ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ MarkLigPosFormat1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                markCoverage;           /* Offset to Mark Coverage table--from
                                         * beginning of MarkLigPos subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                ligatureCoverage;       /* Offset to Ligature Coverage
                                         * table--from beginning of MarkLigPos
                                         * subtable */
  HBUINT16      class_ਗਿਣਤੀ_2;             /* Number of defined mark classes */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkLigPos subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<LigatureArray<Types>>
                ligatureArray;          /* Offset to LigatureArray table--from
                                         * beginning of MarkLigPos subtable */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 4 * Types::LOffset::static_ਮਾਪ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  markCoverage.sanitize (c, ਇਹ) &&
                  ligatureCoverage.sanitize (c, ਇਹ) &&
                  markArray.sanitize (c, ਇਹ) &&
                  ligatureArray.sanitize (c, ਇਹ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) class_ਗਿਣਤੀ_2));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ (ਇਹ+markCoverage).intersects (glyphs) &&
           (ਇਹ+ligatureCoverage).intersects (glyphs);
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    (ਇਹ+markArray).collect_variation_indices (c, ਇਹ+markCoverage);

    hb_map_t klass_mapping;
    Markclass_ਘੇਰਾ_and_remap_indexes (ਇਹ+markCoverage, ਇਹ+markArray, *c->glyph_ਸੈੱਟ, &klass_mapping);

    (ਇਹ+ligatureArray).collect_variation_indices (c, ਇਹ+ligatureCoverage,
                                                   class_ਗਿਣਤੀ_2, klass_mapping);
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+markCoverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    ਜੇ (unlikely (!(ਇਹ+ligatureCoverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  { (ਇਹ+ligatureCoverage).collect_coverage (digest); }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+markCoverage; }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ accept (hb_ਬਫਰ_t *ਬਫਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence,
     * which might have been ligated into a preceding ligature, and in that
     * case the mark should attach to that ligature.
     * https://github.com/harfbuzz/harfbuzz/issues/4969
     * Reject others... */
    ਵਾਪਸ !_hb_glyph_info_multiplied (&ਬਫਰ->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[idx]);
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_ਸੂਚਕਾਂਕ = (ਇਹ+markCoverage).get_coverage  (ਬਫਰ->cur().codepoint);
    ਜੇ (likely (mark_ਸੂਚਕਾਂਕ == NOT_COVERED)) return_trace (ਝੂਠ);

    /* Now we search backwards for a non-mark glyph */

    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_ਨਿਵੇਸ਼;
    skippy_iter.ਸੈੱਟ_lookup_props (LookupFlag::IgnoreMarks);

    ਜੇ (c->ਆਖਰੀ_ਅਧਾਰ_until > ਬਫਰ->idx)
    {
      c->ਆਖਰੀ_ਅਧਾਰ_until = 0;
      c->ਆਖਰੀ_ਅਧਾਰ = -1;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ j;
    ਲਈ (j = ਬਫਰ->idx; j > c->ਆਖਰੀ_ਅਧਾਰ_until; j--)
    {
      ਸਵੈਚਾਲਿਤ ਮੇਲ = skippy_iter.ਮੇਲ (ਬਫਰ->info[j - 1]);
      ਜੇ (ਮੇਲ == skippy_iter.ਮੇਲ_2)
      {
        // https://github.com/harfbuzz/harfbuzz/issues/4124
	ਜੇ (!accept (ਬਫਰ, j - 1) &&
	    NOT_COVERED == (ਇਹ+ligatureCoverage).get_coverage  (ਬਫਰ->info[j - 1].codepoint))
	  ਮੇਲ = skippy_iter.SKIP;
      }
      ਜੇ (ਮੇਲ == skippy_iter.ਮੇਲ_2)
      {
	c->ਆਖਰੀ_ਅਧਾਰ = (ਨਿਸ਼ਾਨੀਵਾਲਾ) j - 1;
	ਰੋਕੋ;
      }
    }
    c->ਆਖਰੀ_ਅਧਾਰ_until = ਬਫਰ->idx;
    ਜੇ (c->ਆਖਰੀ_ਅਧਾਰ == -1)
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (0, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = (ਬਿਨਾਨਿਸ਼ਾਨੀ) c->ਆਖਰੀ_ਅਧਾਰ;

    /* Checking that matched glyph is actually a ligature by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_ligature (&buffer->info[idx])) { return_trace (false); }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_ਸੂਚਕਾਂਕ = (ਇਹ+ligatureCoverage).get_coverage  (ਬਫਰ->info[idx].codepoint);
    ਜੇ (lig_ਸੂਚਕਾਂਕ == NOT_COVERED)
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (idx, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    ਅਚਲ LigatureArray<Types>& lig_array = ਇਹ+ligatureArray;
    ਅਚਲ Anchor_ਮੈਟ੍ਰਿਕਸ<Types, HBUINT16>& lig_attach = lig_array[lig_ਸੂਚਕਾਂਕ];

    /* Find component to attach to */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp_ਗਿਣਤੀ = lig_attach.ਪੰਕਤੀਆਂ;
    ਜੇ (unlikely (!comp_ਗਿਣਤੀ))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (idx, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    /* We must now check whether the ligature ID of the current mark glyph
     * is identical to the ligature ID of the found ligature.  If yes, we
     * can directly use the component index.  If not, we attach the mark
     * glyph to the last component of the ligature. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp_ਸੂਚਕਾਂਕ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ lig_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->info[idx]);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_id = _hb_glyph_info_get_lig_id (&ਬਫਰ->cur());
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_comp = _hb_glyph_info_get_lig_comp (&ਬਫਰ->cur());
    ਜੇ (lig_id && lig_id == mark_id && mark_comp > 0)
      comp_ਸੂਚਕਾਂਕ = hb_min (comp_ਗਿਣਤੀ, _hb_glyph_info_get_lig_comp (&ਬਫਰ->cur())) - 1;
    ਨਹੀਂਤਾਂ
      comp_ਸੂਚਕਾਂਕ = comp_ਗਿਣਤੀ - 1;

    return_trace ((ਇਹ+markArray).apply (c, mark_ਸੂਚਕਾਂਕ, comp_ਸੂਚਕਾਂਕ, lig_attach, class_ਗਿਣਤੀ_2, idx));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = c->plan->glyph_map_gsub;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    hb_map_t klass_mapping;
    Markclass_ਘੇਰਾ_and_remap_indexes (ਇਹ+markCoverage, ਇਹ+markArray, glyphset, &klass_mapping);

    ਜੇ (!klass_mapping.get_population ()) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->class_ਗਿਣਤੀ_2 = klass_mapping.get_population ();

    ਸਵੈਚਾਲਿਤ mark_iter =
    + hb_zip (ਇਹ+markCoverage, hb_range ((ਬਿਨਾਨਿਸ਼ਾਨੀ) (ਇਹ+markArray).len))
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    ;

    ਸਵੈਚਾਲਿਤ new_mark_coverage =
    + mark_iter
    | hb_map_retains_ਛਾਂਟੀ (hb_ਪਹਿਲਾ)
    | hb_map_retains_ਛਾਂਟੀ (glyph_map)
    ;

    ਜੇ (!ਬਾਹਰਲਾ->markCoverage.serialize_serialize (c->serializer, new_mark_coverage))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!ਬਾਹਰਲਾ->markArray.serialize_subset (c, markArray, ਇਹ,
						    (ਇਹ+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ਝੂਠ);

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_lig_coverage;
    ਜੇ (!ਬਾਹਰਲਾ->ligatureArray.serialize_subset (c, ligatureArray, ਇਹ,
					      hb_iter (ਇਹ+ligatureCoverage),
					      class_ਗਿਣਤੀ_2, &klass_mapping, new_lig_coverage))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->ligatureCoverage.serialize_serialize (c->serializer, new_lig_coverage.iter ()));
  }

};

}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_MARKLIGPOSFORMAT1_HH */

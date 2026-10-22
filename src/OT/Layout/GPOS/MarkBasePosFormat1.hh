#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "MarkArray.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ Mark_ਅਧਾਰ_Pos_Format1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                markCoverage;           /* Offset to MarkCoverage table--from
                                         * beginning of MarkBasePos subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                ਅਧਾਰ_Coverage;           /* Offset to BaseCoverage table--from
                                         * beginning of MarkBasePos subtable */
  HBUINT16      class_ਗਿਣਤੀ_2;             /* Number of classes defined for marks */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<MarkArray<Types>>
                markArray;              /* Offset to MarkArray table--from
                                         * beginning of MarkBasePos subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Anchor_ਮੈਟ੍ਰਿਕਸ<Types>>
                ਅਧਾਰ_Array;              /* Offset to BaseArray table--from
                                         * beginning of MarkBasePos subtable */

  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 4 * Types::LOffset::static_ਮਾਪ);

    ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  markCoverage.sanitize (c, ਇਹ) &&
                  ਅਧਾਰ_Coverage.sanitize (c, ਇਹ) &&
                  markArray.sanitize (c, ਇਹ) &&
                  ਅਧਾਰ_Array.sanitize (c, ਇਹ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) class_ਗਿਣਤੀ_2));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ (ਇਹ+markCoverage).intersects (glyphs) &&
           (ਇਹ+ਅਧਾਰ_Coverage).intersects (glyphs);
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    (ਇਹ+markArray).collect_variation_indices (c, ਇਹ+markCoverage);

    hb_map_t klass_mapping;
    Markclass_ਘੇਰਾ_and_remap_indexes (ਇਹ+markCoverage, ਇਹ+markArray, *c->glyph_ਸੈੱਟ, &klass_mapping);

    hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਅਧਾਰ_indexes;
    collect_anchor_indexes (ਇਹ+ਅਧਾਰ_Coverage, (ਇਹ+ਅਧਾਰ_Array).ਪੰਕਤੀਆਂ,
                            class_ਗਿਣਤੀ_2, *c->glyph_ਸੈੱਟ, klass_mapping,
                            ਅਧਾਰ_indexes);
    (ਇਹ+ਅਧਾਰ_Array).collect_variation_indices (c, ਅਧਾਰ_indexes.iter ());
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+markCoverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    ਜੇ (unlikely (!(ਇਹ+ਅਧਾਰ_Coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  { (ਇਹ+ਅਧਾਰ_Coverage).collect_coverage (digest); }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+markCoverage; }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ accept (hb_ਬਫਰ_t *ਬਫਰ, ਬਿਨਾਨਿਸ਼ਾਨੀ idx)
  {
    /* We only want to attach to the first of a MultipleSubst sequence.
     * https://github.com/harfbuzz/harfbuzz/issues/740
     * Reject others...
     * ...but stop if we find a mark in the MultipleSubst sequence:
     * https://github.com/harfbuzz/harfbuzz/issues/1020 */
    ਵਾਪਸ !_hb_glyph_info_multiplied (&ਬਫਰ->info[idx]) ||
	   0 == _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[idx]) ||
	   (idx == 0 ||
	    _hb_glyph_info_is_mark (&ਬਫਰ->info[idx - 1]) ||
	    !_hb_glyph_info_multiplied (&ਬਫਰ->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_id (&ਬਫਰ->info[idx]) !=
	    _hb_glyph_info_get_lig_id (&ਬਫਰ->info[idx - 1]) ||
	    _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[idx]) !=
	    _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[idx - 1]) + 1
	    );
  }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_ਸੂਚਕਾਂਕ = (ਇਹ+markCoverage).get_coverage  (ਬਫਰ->cur().codepoint);
    ਜੇ (likely (mark_ਸੂਚਕਾਂਕ == NOT_COVERED)) return_trace (ਝੂਠ);

    /* Now we search backwards for a non-mark glyph.
     * We don't use skippy_iter.prev() to avoid O(n^2) behavior. */

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
	    NOT_COVERED == (ਇਹ+ਅਧਾਰ_Coverage).get_coverage  (ਬਫਰ->info[j - 1].codepoint))
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

    /* Checking that matched glyph is actually a base glyph by GDEF is too strong; disabled */
    //if (!_hb_glyph_info_is_base_glyph (&buffer->info[idx])) { return_trace (false); }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅਧਾਰ_ਸੂਚਕਾਂਕ = (ਇਹ+ਅਧਾਰ_Coverage).get_coverage  (ਬਫਰ->info[idx].codepoint);
    ਜੇ (ਅਧਾਰ_ਸੂਚਕਾਂਕ == NOT_COVERED)
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (idx, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    return_trace ((ਇਹ+markArray).apply (c, mark_ਸੂਚਕਾਂਕ, ਅਧਾਰ_ਸੂਚਕਾਂਕ, ਇਹ+ਅਧਾਰ_Array, class_ਗਿਣਤੀ_2, idx));
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();
    ਅਚਲ hb_map_t &glyph_map = *c->plan->glyph_map;

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (*ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->format = format;

    hb_map_t klass_mapping;
    Markclass_ਘੇਰਾ_and_remap_indexes (ਇਹ+markCoverage, ਇਹ+markArray, glyphset, &klass_mapping);

    ਜੇ (!klass_mapping.get_population ()) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->class_ਗਿਣਤੀ_2 = klass_mapping.get_population ();

    ਸਵੈਚਾਲਿਤ mark_iter =
    + hb_zip (ਇਹ+markCoverage, ਇਹ+markArray)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    ;

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + mark_iter
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਜੇ (!ਬਾਹਰਲਾ->markCoverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!ਬਾਹਰਲਾ->markArray.serialize_subset (c, markArray, ਇਹ,
						    (ਇਹ+markCoverage).iter (),
						    &klass_mapping)))
      return_trace (ਝੂਠ);

    new_coverage.reset ();
    hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਅਧਾਰ_indexes;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &ਅਧਾਰ_array = ਇਹ+ਅਧਾਰ_Array;
    collect_subset_anchor_indexes (ਇਹ+ਅਧਾਰ_Coverage, ਅਧਾਰ_array.ਪੰਕਤੀਆਂ,
                                   class_ਗਿਣਤੀ_2, ਅਧਾਰ_array.ਮੈਟ੍ਰਿਕਸ_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, ਅਧਾਰ_indexes);

    ਜੇ (!new_coverage) return_trace (ਝੂਠ);
    ਜੇ (!ਬਾਹਰਲਾ->ਅਧਾਰ_Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->ਅਧਾਰ_Array.serialize_subset (c, ਅਧਾਰ_Array, ਇਹ,
						   new_coverage.ਲੰਮਾਈ,
						   ਅਧਾਰ_indexes.iter ()));
  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_MARKBASEPOSFORMAT1_HH */

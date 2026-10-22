#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "MarkMarkPosFormat1.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ MarkMarkPosFormat1_2
{
  ਸੁਰੱਖਿਅਤ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                mark1Coverage;          /* Offset to Combining Mark1 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Coverage>
                mark2Coverage;          /* Offset to Combining Mark2 Coverage
                                         * table--from beginning of MarkMarkPos
                                         * subtable */
  HBUINT16      class_ਗਿਣਤੀ_2;             /* Number of defined mark classes */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<MarkArray<Types>>
                mark1Array;             /* Offset to Mark1Array table--from
                                         * beginning of MarkMarkPos subtable */
  ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ LOffset_ਮੰਜ਼ਿਲ<Anchor_ਮੈਟ੍ਰਿਕਸ<Types, HBUINT16>>
                mark2Array;             /* Offset to Mark2Array table--from
                                         * beginning of MarkMarkPos subtable */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_STATIC (4 + 4 * Types::LOffset::static_ਮਾਪ);

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
                  mark1Coverage.sanitize (c, ਇਹ) &&
                  mark2Coverage.sanitize (c, ਇਹ) &&
                  mark1Array.sanitize (c, ਇਹ) &&
		  hb_barrier () &&
                  mark2Array.sanitize (c, ਇਹ, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) class_ਗਿਣਤੀ_2));
  }

  ਤਾਰਕਿਕ intersects (ਅਚਲ hb_ਸੈੱਟ_t *glyphs) ਅਚਲ
  {
    ਵਾਪਸ (ਇਹ+mark1Coverage).intersects (glyphs) &&
           (ਇਹ+mark2Coverage).intersects (glyphs);
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_ਘੇਰਾ_lookups_context_t *c) ਅਚਲ {}

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    (ਇਹ+mark1Array).collect_variation_indices (c, ਇਹ+mark1Coverage);

    hb_map_t klass_mapping;
    Markclass_ਘੇਰਾ_and_remap_indexes (ਇਹ+mark1Coverage, ਇਹ+mark1Array, *c->glyph_ਸੈੱਟ, &klass_mapping);

    hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> mark2_indexes;
    collect_anchor_indexes (ਇਹ+mark2Coverage, (ਇਹ+mark2Array).ਪੰਕਤੀਆਂ,
                            class_ਗਿਣਤੀ_2, *c->glyph_ਸੈੱਟ, klass_mapping,
                            mark2_indexes);
    (ਇਹ+mark2Array).collect_variation_indices (c, mark2_indexes.iter ());
  }

  ਖਾਲੀ collect_glyphs (hb_collect_glyphs_context_t *c) ਅਚਲ
  {
    ਜੇ (unlikely (!(ਇਹ+mark1Coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
    ਜੇ (unlikely (!(ਇਹ+mark2Coverage).collect_coverage (c->ਨਿਵੇਸ਼))) ਵਾਪਸ;
  }

  ਖਾਲੀ collect_ਦੂਜਾ_glyphs (hb_ਸੈੱਟ_digest_t *digest) ਅਚਲ
  { (ਇਹ+mark2Coverage).collect_coverage (digest); }

  ਅਚਲ Coverage &get_coverage () ਅਚਲ { ਵਾਪਸ ਇਹ+mark1Coverage; }

  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark1_ਸੂਚਕਾਂਕ = (ਇਹ+mark1Coverage).get_coverage  (ਬਫਰ->cur().codepoint);
    ਜੇ (likely (mark1_ਸੂਚਕਾਂਕ == NOT_COVERED)) return_trace (ਝੂਠ);

    /* now we search backwards for a suitable mark glyph until a non-mark glyph */
    ਸਵੈਚਾਲਿਤ &skippy_iter = c->iter_ਨਿਵੇਸ਼;
    skippy_iter.reset_fast (ਬਫਰ->idx);
    skippy_iter.ਸੈੱਟ_lookup_props (c->lookup_props & ~(uint32_t)LookupFlag::IgnoreFlags);
    ਬਿਨਾਨਿਸ਼ਾਨੀ unsafe_from;
    ਜੇ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (unsafe_from, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    ਜੇ (likely (!_hb_glyph_info_is_mark (&ਬਫਰ->info[skippy_iter.idx])))
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (skippy_iter.idx, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = skippy_iter.idx;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ id1 = _hb_glyph_info_get_lig_id (&ਬਫਰ->cur());
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ id2 = _hb_glyph_info_get_lig_id (&ਬਫਰ->info[j]);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp1 = _hb_glyph_info_get_lig_comp (&ਬਫਰ->cur());
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ comp2 = _hb_glyph_info_get_lig_comp (&ਬਫਰ->info[j]);

    ਜੇ (likely (id1 == id2))
    {
      ਜੇ (id1 == 0) /* Marks belonging to the same base. */
        ਜਾਓ good;
      ਨਹੀਂਤਾਂ ਜੇ (comp1 == comp2) /* Marks belonging to the same ligature component. */
        ਜਾਓ good;
    }
    ਨਹੀਂਤਾਂ
    {
      /* If ligature ids don't match, it may be the case that one of the marks
       * itself is a ligature.  In which case match. */
      ਜੇ ((id1 > 0 && !comp1) || (id2 > 0 && !comp2))
        ਜਾਓ good;
    }

    /* Didn't match. */
    ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (skippy_iter.idx, ਬਫਰ->idx + 1);
    return_trace (ਝੂਠ);

    good:
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark2_ਸੂਚਕਾਂਕ = (ਇਹ+mark2Coverage).get_coverage  (ਬਫਰ->info[j].codepoint);
    ਜੇ (mark2_ਸੂਚਕਾਂਕ == NOT_COVERED)
    {
      ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_concat_from_outbuffer (skippy_iter.idx, ਬਫਰ->idx + 1);
      return_trace (ਝੂਠ);
    }

    return_trace ((ਇਹ+mark1Array).apply (c, mark1_ਸੂਚਕਾਂਕ, mark2_ਸੂਚਕਾਂਕ, ਇਹ+mark2Array, class_ਗਿਣਤੀ_2, j));
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
    Markclass_ਘੇਰਾ_and_remap_indexes (ਇਹ+mark1Coverage, ਇਹ+mark1Array, glyphset, &klass_mapping);

    ਜੇ (!klass_mapping.get_population ()) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->class_ਗਿਣਤੀ_2 = klass_mapping.get_population ();

    ਸਵੈਚਾਲਿਤ mark1_iter =
    + hb_zip (ਇਹ+mark1Coverage, ਇਹ+mark1Array)
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    ;

    hb_sorted_ਸਦਿਸ਼_t<hb_codepoint_t> new_coverage;
    + mark1_iter
    | hb_map (hb_ਪਹਿਲਾ)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;

    ਜੇ (!ਬਾਹਰਲਾ->mark1Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ਝੂਠ);

    ਜੇ (unlikely (!ਬਾਹਰਲਾ->mark1Array.serialize_subset (c, mark1Array, ਇਹ,
						     (ਇਹ+mark1Coverage).iter (),
						     &klass_mapping)))
      return_trace (ਝੂਠ);

    new_coverage.reset ();
    hb_sorted_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> mark2_indexes;
    ਅਚਲ ਸਵੈਚਾਲਿਤ &mark2_array = ਇਹ+mark2Array;
    collect_subset_anchor_indexes (ਇਹ+mark2Coverage, mark2_array.ਪੰਕਤੀਆਂ,
                                   class_ਗਿਣਤੀ_2, mark2_array.ਮੈਟ੍ਰਿਕਸ_Z.arrayZ,
                                   glyphset, glyph_map, klass_mapping,
                                   new_coverage, mark2_indexes);

    ਜੇ (!new_coverage) return_trace (ਝੂਠ);
    ਜੇ (!ਬਾਹਰਲਾ->mark2Coverage.serialize_serialize (c->serializer, new_coverage.iter ()))
      return_trace (ਝੂਠ);

    return_trace (ਬਾਹਰਲਾ->mark2Array.serialize_subset (c, mark2Array, ਇਹ,
						    new_coverage.ਲੰਮਾਈ,
						    mark2_indexes.iter ()));

  }
};


}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_MARKMARKPOSFORMAT1_HH */

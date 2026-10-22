#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_MARKARRAY_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_MARKARRAY_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "AnchorMatrix.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "MarkRecord.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਸਥਿਰ ਸਤਰਅੰਦਰ hb_position_t
resolve_cross_ਵਿੱਥ (ਅਚਲ hb_glyph_position_t *pos,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len,
		      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_pos,
		      hb_direction_t direction)
{
  ਤਾਰਕਿਕ horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t ਵਿੱਥ = horizontal ? pos[glyph_pos].y_ਵਿੱਥ_2 : pos[glyph_pos].x_ਵਿੱਥ_2;
  ਜਦੋਂ (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    ਪੂਰਨ chain = pos[glyph_pos].attach_chain();
    ਜੇ (!chain)
      ਰੋਕੋ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪਾ = (ਪੂਰਨ) glyph_pos + chain;
    ਜੇ (unlikely (ਮਾਪਾ >= len))
      ਰੋਕੋ;
    glyph_pos = ਮਾਪਾ;
    ਵਿੱਥ = hb_saturate_ਜੋੜੋ (ਵਿੱਥ,
			      horizontal ? pos[glyph_pos].y_ਵਿੱਥ_2 : pos[glyph_pos].x_ਵਿੱਥ_2);
  }
  ਵਾਪਸ ਵਿੱਥ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ਅਚਲ Anchor &mark_anchor,
                       ਅਚਲ Anchor &glyph_anchor,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ glyph_pos)
{
  hb_ਬਫਰ_t *ਬਫਰ = c->ਬਫਰ;
  ਦਸ਼ਮਲਵ mark_x, mark_y, ਅਧਾਰ_x, ਅਧਾਰ_y;

  ਬਫਰ->unsafe_ਮੰਜ਼ਿਲ_break (glyph_pos, ਬਫਰ->idx + 1);
  mark_anchor.get_anchor (c, ਬਫਰ->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, ਬਫਰ->info[glyph_pos].codepoint, &ਅਧਾਰ_x, &ਅਧਾਰ_y);

  ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
  {
    c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->ਬਫਰ->idx, glyph_pos);
  }

  hb_position_t ਅਧਾਰ_ਵਿੱਥ = resolve_cross_ਵਿੱਥ (ਬਫਰ->pos, ਬਫਰ->len, glyph_pos,
						      ਬਫਰ->props.direction);

  hb_glyph_position_t &mark = ਬਫਰ->cur_pos();
  mark.attach_chain() = (ਪੂਰਨ) glyph_pos - (ਪੂਰਨ) ਬਫਰ->idx;
  ਜੇ (mark.attach_chain() != (ਪੂਰਨ) glyph_pos - (ਪੂਰਨ) ਬਫਰ->idx)
  {
    mark.attach_chain() = 0;
    ਜਾਓ ਉਛਾਲ;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_ਵਿੱਥ_2 = roundf (ਅਧਾਰ_x - mark_x);
  mark.y_ਵਿੱਥ_2 = roundf (ਅਧਾਰ_y - mark_y);
  ਜੇ (HB_DIRECTION_IS_HORIZONTAL (ਬਫਰ->props.direction))
    mark.y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (mark.y_ਵਿੱਥ_2, ਅਧਾਰ_ਵਿੱਥ);
  ਨਹੀਂਤਾਂ
    mark.x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (mark.x_ਵਿੱਥ_2, ਅਧਾਰ_ਵਿੱਥ);
  ਬਫਰ->scratch_flags |= HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  ਜੇ (HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ && c->ਬਫਰ->messaging ())
  {
    c->ਬਫਰ->ਸੁਨੇਹਾ (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->ਬਫਰ->idx, glyph_pos);
  }

ਉਛਾਲ:
  ਬਫਰ->idx++;
  ਵਾਪਸ ਸੱਚ;
}

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ਅਚਲ Coverage &coverage,
                                    ਅਚਲ ਖਾਲੀ *ਅਧਾਰ,
                                    glyph_ਅੰਕੜੇ_t anchors,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ)
{
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : hb_zip (coverage, hb_range (ਗਿਣਤੀ_2)))
  {
    ਜੇ (!c->glyph_ਸੈੱਟ->has (_.ਪਹਿਲਾ)) ਜਾਰੀ;
    glyph_ਅੰਕੜੇ_t anchor = anchors;
    anchor.ਅੰਕੜੇ = (ਅਚਲ ਅੱਖਰ *) anchors.ਅੰਕੜੇ + _.second * ਰਿਕਾਰਡ_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਵਿੱਥ = anchor[0];
    ਜੇ (ਵਿੱਥ)
      Struct_At_ਵਿੱਥ<ਅਚਲ Anchor> (ਅਧਾਰ, ਵਿੱਥ).collect_variation_indices (c);
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
ਬਣਤਰ MarkArray : Types::ਸਾਂਚਾ Array_ਦਾ<Mark_ਰਿਕਾਰਡ<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਕਿਸਮ_ਨਾਂ Types::ਸਾਂਚਾ Array_ਦਾ<Mark_ਰਿਕਾਰਡ<Types>> Super;

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (Super::sanitize (c, ਇਹ));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ Coverage &coverage) ਅਚਲ
  {
    ਜੇ (!ਇਹ->len) ਵਾਪਸ;
    collect_mark_variation_indices (c, coverage, ਇਹ,
                                   &ਇਹ->arrayZ[0].markAnchor,
                                   ਇਹ->len, Mark_ਰਿਕਾਰਡ<Types>::static_ਮਾਪ);
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Anchor_ਮੈਟ੍ਰਿਕਸ_Type>
  ਤਾਰਕਿਕ apply (hb_ot_apply_context_t *c,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_ਸੂਚਕਾਂਕ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਸੂਚਕਾਂਕ,
              ਅਚਲ Anchor_ਮੈਟ੍ਰਿਕਸ_Type &anchors, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ class_ਗਿਣਤੀ,
              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_pos) ਅਚਲ
  {
    TRACE_APPLY (ਇਹ);
    ਅਚਲ Mark_ਰਿਕਾਰਡ<Types> &ਰਿਕਾਰਡ = Super::ਸੰਚਾਲਕ[](mark_ਸੂਚਕਾਂਕ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ mark_class = ਰਿਕਾਰਡ.klass;

    ਅਚਲ Anchor& mark_anchor = ਇਹ + ਰਿਕਾਰਡ.markAnchor;
    ਤਾਰਕਿਕ ਮਿਲਿਆ;
    ਅਚਲ Anchor& glyph_anchor = anchors.get_anchor (c, glyph_ਸੂਚਕਾਂਕ, mark_class, class_ਗਿਣਤੀ, &ਮਿਲਿਆ);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    ਜੇ (unlikely (!ਮਿਲਿਆ)) return_trace (ਝੂਠ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ਤਾਰਕਿਕ subset (hb_subset_context_t *c,
               Iterator             coverage,
               ਅਚਲ hb_map_t      *klass_mapping) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਅਚਲ hb_ਸੈੱਟ_t &glyphset = *c->plan->glyphset_gsub ();

    ਸਵੈਚਾਲਿਤ* ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed (ਇਹ);
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    ਸਵੈਚਾਲਿਤ mark_iter =
    + hb_zip (coverage, ਇਹ->iter ())
    | hb_filter (glyphset, hb_ਪਹਿਲਾ)
    | hb_map (hb_ਦੂਜਾ)
    ;

    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ new_ਲੰਮਾਈ = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ& mark_ਰਿਕਾਰਡ : mark_iter) {
      ret |= mark_ਰਿਕਾਰਡ.subset (c, ਇਹ, klass_mapping);
      new_ਲੰਮਾਈ++;
    }

    ਜੇ (unlikely (!c->serializer->check_ਸੌਂਪੋ (ਬਾਹਰਲਾ->len, new_ਲੰਮਾਈ,
                                                HB_SERIALIZE_ERROR_ARRAY_ਉਛਾਲ)))
      return_trace (ਝੂਠ);

    return_trace (ret);
  }
};

HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ Markclass_ਘੇਰਾ_and_remap_indexes (ਅਚਲ Coverage &mark_coverage,
                                    ਅਚਲ ਖਾਲੀ *mark_ਰਿਕਾਰਡਾਂ,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2,
                                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਰਿਕਾਰਡ_ਮਾਪ,
                                    ਅਚਲ hb_ਸੈੱਟ_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_ਸੈੱਟ_t orig_classes;
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ _ : hb_zip (mark_coverage, hb_range (ਗਿਣਤੀ_2)))
    ਜੇ (glyphset.has (_.ਪਹਿਲਾ))
    {
      ਅਚਲ ਸਵੈਚਾਲਿਤ *klass = reinterpret_cast<ਅਚਲ HBUINT16 *>
        ((ਅਚਲ ਅੱਖਰ *) mark_ਰਿਕਾਰਡਾਂ + _.second * ਰਿਕਾਰਡ_ਮਾਪ);
      orig_classes.ਜੋੜੋ (*klass);
    }

  ਬਿਨਾਨਿਸ਼ਾਨੀ idx = 0;
  ਲਈ (ਸਵੈਚਾਲਿਤ klass : orig_classes.iter ())
  {
    ਜੇ (klass_mapping->has (klass)) ਜਾਰੀ;
    klass_mapping->ਸੈੱਟ (klass, idx);
    idx++;
  }
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Types>
HB_ਅੰਦਰੂਨੀ ਸਤਰਅੰਦਰ
ਖਾਲੀ Markclass_ਘੇਰਾ_and_remap_indexes (ਅਚਲ Coverage     &mark_coverage,
                                          ਅਚਲ MarkArray<Types> &mark_array,
                                          ਅਚਲ hb_ਸੈੱਟ_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_ਘੇਰਾ_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_ਰਿਕਾਰਡ<Types>::static_ਮਾਪ,
                                       glyphset, klass_mapping);
}

}
}
}

#ਜੇ_ਅੰਤ /* OT_LAYOUT_GPOS_MARKARRAY_HH */

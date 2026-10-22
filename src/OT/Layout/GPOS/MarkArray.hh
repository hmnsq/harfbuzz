#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#અન્તર્ભાવ "AnchorMatrix.hh"
#અન્તર્ભાવ "MarkRecord.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

સ્થિર અંતઃરેખ hb_position_t
resolve_cross_offset (અચલ hb_glyph_position_t *pos,
		      unsigned પૂર્ણાંક len,
		      unsigned પૂર્ણાંક glyph_pos,
		      hb_direction_t direction)
{
  તાર્કિક horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  જ્યારે (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    પૂર્ણાંક chain = pos[glyph_pos].attach_chain();
    જો (!chain)
      તોડો;
    unsigned પૂર્ણાંક parent = (પૂર્ણાંક) glyph_pos + chain;
    જો (unlikely (parent >= len))
      તોડો;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  પાછા offset;
}

સ્થિર અંતઃરેખ તાર્કિક
apply_mark_attachment (hb_ot_apply_context_t *c,
                       અચલ Anchor &mark_anchor,
                       અચલ Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  દશાંશ mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (પૂર્ણાંક) glyph_pos - (પૂર્ણાંક) buffer->idx;
  જો (mark.attach_chain() != (પૂર્ણાંક) glyph_pos - (પૂર્ણાંક) buffer->idx)
  {
    mark.attach_chain() = 0;
    જાઓ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  જો (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  નહીંતર
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  પાછા સાચું;
}

HB_INTERNAL અંતઃરેખ
શૂન્ય collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    અચલ Coverage &coverage,
                                    અચલ શૂન્ય *base,
                                    glyph_data_t anchors,
                                    unsigned ગણતરી,
                                    unsigned record_size)
{
  માટે (અચલ auto _ : hb_zip (coverage, hb_range (ગણતરી)))
  {
    જો (!c->glyph_set->has (_.first)) ચાલુ;
    glyph_data_t anchor = anchors;
    anchor.data = (અચલ અક્ષર *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    જો (offset)
      StructAtOffset<અચલ Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
સંરચના MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  પ્રકારવ્યાખ્યા typename Types::template ArrayOf<MarkRecord<Types>> Super;

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (Super::sanitize (c, આ));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ Coverage &coverage) અચલ
  {
    જો (!આ->len) પાછા;
    collect_mark_variation_indices (c, coverage, આ,
                                   &આ->arrayZ[0].markAnchor,
                                   આ->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  તાર્કિક apply (hb_ot_apply_context_t *c,
              unsigned પૂર્ણાંક mark_index, unsigned પૂર્ણાંક glyph_index,
              અચલ AnchorMatrixType &anchors, unsigned પૂર્ણાંક class_ગણતરી,
              unsigned પૂર્ણાંક glyph_pos) અચલ
  {
    TRACE_APPLY (આ);
    અચલ MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned પૂર્ણાંક mark_class = record.klass;

    અચલ Anchor& mark_anchor = આ + record.markAnchor;
    તાર્કિક found;
    અચલ Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_ગણતરી, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    જો (unlikely (!found)) return_trace (ખોટું);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  તાર્કિક subset (hb_subset_context_t *c,
               Iterator             coverage,
               અચલ hb_map_t      *klass_mapping) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (આ);
    જો (unlikely (!c->serializer->extend_min (out))) return_trace (ખોટું);

    auto mark_iter =
    + hb_zip (coverage, આ->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    તાર્કિક ret = ખોટું;
    unsigned new_length = 0;
    માટે (અચલ auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, આ, klass_mapping);
      new_length++;
    }

    જો (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (ખોટું);

    return_trace (ret);
  }
};

HB_INTERNAL અંતઃરેખ
શૂન્ય Markclass_closure_and_remap_indexes (અચલ Coverage &mark_coverage,
                                    અચલ શૂન્ય *mark_records,
                                    unsigned ગણતરી,
                                    unsigned record_size,
                                    અચલ hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  માટે (અચલ auto _ : hb_zip (mark_coverage, hb_range (ગણતરી)))
    જો (glyphset.has (_.first))
    {
      અચલ auto *klass = reinterpret_cast<અચલ HBUINT16 *>
        ((અચલ અક્ષર *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  માટે (auto klass : orig_classes.iter ())
  {
    જો (klass_mapping->has (klass)) ચાલુ;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL અંતઃરેખ
શૂન્ય Markclass_closure_and_remap_indexes (અચલ Coverage     &mark_coverage,
                                          અચલ MarkArray<Types> &mark_array,
                                          અચલ hb_set_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_closure_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       MarkRecord<Types>::static_size,
                                       glyphset, klass_mapping);
}

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKARRAY_HH */

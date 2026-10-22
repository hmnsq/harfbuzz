#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#অন্তর্ভাব "AnchorMatrix.hh"
#অন্তর্ভাব "MarkRecord.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

স্থির অন্তঃস্থ hb_position_t
resolve_cross_offset (ধ্রুব hb_glyph_position_t *pos,
		      unsigned পূর্ণাঙ্ক len,
		      unsigned পূর্ণাঙ্ক glyph_pos,
		      hb_direction_t direction)
{
  তার্কিক horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  যাবত্ (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    পূর্ণাঙ্ক chain = pos[glyph_pos].attach_chain();
    যদি (!chain)
      ভঙ্গ;
    unsigned পূর্ণাঙ্ক parent = (পূর্ণাঙ্ক) glyph_pos + chain;
    যদি (unlikely (parent >= len))
      ভঙ্গ;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  প্রত্যাগম offset;
}

স্থির অন্তঃস্থ তার্কিক
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ধ্রুব Anchor &mark_anchor,
                       ধ্রুব Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  ভিন্ন mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (পূর্ণাঙ্ক) glyph_pos - (পূর্ণাঙ্ক) buffer->idx;
  যদি (mark.attach_chain() != (পূর্ণাঙ্ক) glyph_pos - (পূর্ণাঙ্ক) buffer->idx)
  {
    mark.attach_chain() = 0;
    গচ্ছ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  যদি (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  অন্যথা
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  প্রত্যাগম সত্য;
}

HB_INTERNAL অন্তঃস্থ
শূন্য collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ধ্রুব Coverage &coverage,
                                    ধ্রুব শূন্য *base,
                                    glyph_data_t anchors,
                                    unsigned গুনতি,
                                    unsigned record_size)
{
  প্রতি (ধ্রুব auto _ : hb_zip (coverage, hb_range (গুনতি)))
  {
    যদি (!c->glyph_set->has (_.first)) অনুবর্ত;
    glyph_data_t anchor = anchors;
    anchor.data = (ধ্রুব বর্ণ *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    যদি (offset)
      StructAtOffset<ধ্রুব Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
সংরচনা MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  প্রকারনির্দেশ typename Types::template ArrayOf<MarkRecord<Types>> Super;

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (Super::sanitize (c, এতত্));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুব Coverage &coverage) ধ্রুব
  {
    যদি (!এতত্->len) প্রত্যাগম;
    collect_mark_variation_indices (c, coverage, এতত্,
                                   &এতত্->arrayZ[0].markAnchor,
                                   এতত্->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  তার্কিক apply (hb_ot_apply_context_t *c,
              unsigned পূর্ণাঙ্ক mark_index, unsigned পূর্ণাঙ্ক glyph_index,
              ধ্রুব AnchorMatrixType &anchors, unsigned পূর্ণাঙ্ক class_গুনতি,
              unsigned পূর্ণাঙ্ক glyph_pos) ধ্রুব
  {
    TRACE_APPLY (এতত্);
    ধ্রুব MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned পূর্ণাঙ্ক mark_class = record.klass;

    ধ্রুব Anchor& mark_anchor = এতত্ + record.markAnchor;
    তার্কিক found;
    ধ্রুব Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_গুনতি, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    যদি (unlikely (!found)) return_trace (অসত্য);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  তার্কিক subset (hb_subset_context_t *c,
               Iterator             coverage,
               ধ্রুব hb_map_t      *klass_mapping) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    ধ্রুব hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (এতত্);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    auto mark_iter =
    + hb_zip (coverage, এতত্->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    তার্কিক ret = অসত্য;
    unsigned new_length = 0;
    প্রতি (ধ্রুব auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, এতত্, klass_mapping);
      new_length++;
    }

    যদি (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (অসত্য);

    return_trace (ret);
  }
};

HB_INTERNAL অন্তঃস্থ
শূন্য Markclass_closure_and_remap_indexes (ধ্রুব Coverage &mark_coverage,
                                    ধ্রুব শূন্য *mark_records,
                                    unsigned গুনতি,
                                    unsigned record_size,
                                    ধ্রুব hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  প্রতি (ধ্রুব auto _ : hb_zip (mark_coverage, hb_range (গুনতি)))
    যদি (glyphset.has (_.first))
    {
      ধ্রুব auto *klass = reinterpret_cast<ধ্রুব HBUINT16 *>
        ((ধ্রুব বর্ণ *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  প্রতি (auto klass : orig_classes.iter ())
  {
    যদি (klass_mapping->has (klass)) অনুবর্ত;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL অন্তঃস্থ
শূন্য Markclass_closure_and_remap_indexes (ধ্রুব Coverage     &mark_coverage,
                                          ধ্রুব MarkArray<Types> &mark_array,
                                          ধ্রুব hb_set_t     &glyphset,
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

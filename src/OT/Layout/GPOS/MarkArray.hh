#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#ଅନ୍ତର୍ଭାଵ "AnchorMatrix.hh"
#ଅନ୍ତର୍ଭାଵ "MarkRecord.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ସ୍ଥିର ଧାଡ଼ିଭିତରେ hb_position_t
resolve_cross_offset (ଅଚଳ hb_glyph_position_t *pos,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
		      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_pos,
		      hb_direction_t direction)
{
  ତାର୍କିକ horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  ଯେତେବେଳେ (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା chain = pos[glyph_pos].attach_chain();
    ଯଦି (!chain)
      ଭାଙ୍ଗ;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) glyph_pos + chain;
    ଯଦି (unlikely (parent >= len))
      ଭାଙ୍ଗ;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  ଫେରନ୍ତୁ offset;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ଅଚଳ Anchor &mark_anchor,
                       ଅଚଳ Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  ଦଶମିକ mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) glyph_pos - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) buffer->idx;
  ଯଦି (mark.attach_chain() != (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) glyph_pos - (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) buffer->idx)
  {
    mark.attach_chain() = 0;
    ଯାଅ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  ଯଦି (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  ଅନ୍ୟଥା
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  ଫେରନ୍ତୁ ସତ୍ୟ;
}

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ଅଚଳ Coverage &coverage,
                                    ଅଚଳ ଶୂନ୍ୟ *base,
                                    glyph_data_t anchors,
                                    unsigned ଗଣନା_2,
                                    unsigned record_size)
{
  ପାଇଁ (ଅଚଳ auto _ : hb_zip (coverage, hb_range (ଗଣନା_2)))
  {
    ଯଦି (!c->glyph_set->has (_.first)) ଜାରି;
    glyph_data_t anchor = anchors;
    anchor.data = (ଅଚଳ ଅକ୍ଷର *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    ଯଦି (offset)
      StructAtOffset<ଅଚଳ Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
ସଂରଚନା MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  ପ୍ରକାରବ୍ୟାଖ୍ୟା typename Types::template ArrayOf<MarkRecord<Types>> Super;

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (Super::sanitize (c, ଏହି));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ Coverage &coverage) ଅଚଳ
  {
    ଯଦି (!ଏହି->len) ଫେରନ୍ତୁ;
    collect_mark_variation_indices (c, coverage, ଏହି,
                                   &ଏହି->arrayZ[0].markAnchor,
                                   ଏହି->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  ତାର୍କିକ apply (hb_ot_apply_context_t *c,
              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_index, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_index,
              ଅଚଳ AnchorMatrixType &anchors, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା class_ଗଣନା,
              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_pos) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    ଅଚଳ MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_class = record.klass;

    ଅଚଳ Anchor& mark_anchor = ଏହି + record.markAnchor;
    ତାର୍କିକ found;
    ଅଚଳ Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_ଗଣନା, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    ଯଦି (unlikely (!found)) return_trace (ମିଥ୍ୟା);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ତାର୍କିକ subset (hb_subset_context_t *c,
               Iterator             coverage,
               ଅଚଳ hb_map_t      *klass_mapping) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (ଏହି);
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    auto mark_iter =
    + hb_zip (coverage, ଏହି->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    ତାର୍କିକ ret = ମିଥ୍ୟା;
    unsigned new_length = 0;
    ପାଇଁ (ଅଚଳ auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, ଏହି, klass_mapping);
      new_length++;
    }

    ଯଦି (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (ମିଥ୍ୟା);

    return_trace (ret);
  }
};

HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ Markclass_closure_and_remap_indexes (ଅଚଳ Coverage &mark_coverage,
                                    ଅଚଳ ଶୂନ୍ୟ *mark_records,
                                    unsigned ଗଣନା_2,
                                    unsigned record_size,
                                    ଅଚଳ hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  ପାଇଁ (ଅଚଳ auto _ : hb_zip (mark_coverage, hb_range (ଗଣନା_2)))
    ଯଦି (glyphset.has (_.first))
    {
      ଅଚଳ auto *klass = reinterpret_cast<ଅଚଳ HBUINT16 *>
        ((ଅଚଳ ଅକ୍ଷର *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  ପାଇଁ (auto klass : orig_classes.iter ())
  {
    ଯଦି (klass_mapping->has (klass)) ଜାରି;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ Markclass_closure_and_remap_indexes (ଅଚଳ Coverage     &mark_coverage,
                                          ଅଚଳ MarkArray<Types> &mark_array,
                                          ଅଚଳ hb_set_t     &glyphset,
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

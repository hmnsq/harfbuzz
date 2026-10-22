#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#अन्तर्भाव "AnchorMatrix.hh"
#अन्तर्भाव "MarkRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

स्थिर अन्तःस्थ hb_position_t
resolve_cross_offset (ध्रुव hb_glyph_position_t *pos,
		      unsigned पूर्णाङ्क len,
		      unsigned पूर्णाङ्क glyph_pos,
		      hb_direction_t direction)
{
  तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  जाव (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    पूर्णाङ्क chain = pos[glyph_pos].attach_chain();
    जइ (!chain)
      भङ्ग;
    unsigned पूर्णाङ्क parent = (पूर्णाङ्क) glyph_pos + chain;
    जइ (unlikely (parent >= len))
      भङ्ग;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  प्रत्यागम offset;
}

स्थिर अन्तःस्थ तार्किक
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ध्रुव Anchor &mark_anchor,
                       ध्रुव Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  भिन्न mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (पूर्णाङ्क) glyph_pos - (पूर्णाङ्क) buffer->idx;
  जइ (mark.attach_chain() != (पूर्णाङ्क) glyph_pos - (पूर्णाङ्क) buffer->idx)
  {
    mark.attach_chain() = 0;
    गच्छ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  जइ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  अण्णहा
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  प्रत्यागम सच्च;
}

HB_INTERNAL अन्तःस्थ
शून्य collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ध्रुव Coverage &coverage,
                                    ध्रुव शून्य *base,
                                    glyph_data_t anchors,
                                    unsigned गणनम्,
                                    unsigned record_size)
{
  प्रति (ध्रुव auto _ : hb_zip (coverage, hb_range (गणनम्)))
  {
    जइ (!c->glyph_set->has (_.first)) अनुवर्त;
    glyph_data_t anchor = anchors;
    anchor.data = (ध्रुव वर्ण *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    जइ (offset)
      StructAtOffset<ध्रुव Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
संरचना MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  प्रकारनिर्देश typename Types::template ArrayOf<MarkRecord<Types>> Super;

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (Super::sanitize (c, एतत्));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव Coverage &coverage) ध्रुव
  {
    जइ (!एतत्->len) प्रत्यागम;
    collect_mark_variation_indices (c, coverage, एतत्,
                                   &एतत्->arrayZ[0].markAnchor,
                                   एतत्->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  तार्किक apply (hb_ot_apply_context_t *c,
              unsigned पूर्णाङ्क mark_index, unsigned पूर्णाङ्क glyph_index,
              ध्रुव AnchorMatrixType &anchors, unsigned पूर्णाङ्क class_गणनम्,
              unsigned पूर्णाङ्क glyph_pos) ध्रुव
  {
    TRACE_APPLY (एतत्);
    ध्रुव MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned पूर्णाङ्क mark_class = record.klass;

    ध्रुव Anchor& mark_anchor = एतत् + record.markAnchor;
    तार्किक found;
    ध्रुव Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_गणनम्, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    जइ (unlikely (!found)) return_trace (मिच्छ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               Iterator             coverage,
               ध्रुव hb_map_t      *klass_mapping) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (एतत्);
    जइ (unlikely (!c->serializer->extend_min (out))) return_trace (मिच्छ);

    auto mark_iter =
    + hb_zip (coverage, एतत्->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    तार्किक ret = मिच्छ;
    unsigned new_length = 0;
    प्रति (ध्रुव auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, एतत्, klass_mapping);
      new_length++;
    }

    जइ (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (मिच्छ);

    return_trace (ret);
  }
};

HB_INTERNAL अन्तःस्थ
शून्य Markclass_closure_and_remap_indexes (ध्रुव Coverage &mark_coverage,
                                    ध्रुव शून्य *mark_records,
                                    unsigned गणनम्,
                                    unsigned record_size,
                                    ध्रुव hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  प्रति (ध्रुव auto _ : hb_zip (mark_coverage, hb_range (गणनम्)))
    जइ (glyphset.has (_.first))
    {
      ध्रुव auto *klass = reinterpret_cast<ध्रुव HBUINT16 *>
        ((ध्रुव वर्ण *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  प्रति (auto klass : orig_classes.iter ())
  {
    जइ (klass_mapping->has (klass)) अनुवर्त;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL अन्तःस्थ
शून्य Markclass_closure_and_remap_indexes (ध्रुव Coverage     &mark_coverage,
                                          ध्रुव MarkArray<Types> &mark_array,
                                          ध्रुव hb_set_t     &glyphset,
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

#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#அந்தர்பாவ "AnchorMatrix.hh"
#அந்தர்பாவ "MarkRecord.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

நிலையான உள்வரி hb_position_t
resolve_cross_offset (மாறிலி hb_glyph_position_t *pos,
		      unsigned முழு len,
		      unsigned முழு glyph_pos,
		      hb_direction_t direction)
{
  தருக்க horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  போது (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    முழு chain = pos[glyph_pos].attach_chain();
    எனில் (!chain)
      நிறுத்து;
    unsigned முழு parent = (முழு) glyph_pos + chain;
    எனில் (unlikely (parent >= len))
      நிறுத்து;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  திரும்பு offset;
}

நிலையான உள்வரி தருக்க
apply_mark_attachment (hb_ot_apply_context_t *c,
                       மாறிலி Anchor &mark_anchor,
                       மாறிலி Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  தசம mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (முழு) glyph_pos - (முழு) buffer->idx;
  எனில் (mark.attach_chain() != (முழு) glyph_pos - (முழு) buffer->idx)
  {
    mark.attach_chain() = 0;
    செல் overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  எனில் (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  இல்லையெனில்
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  திரும்பு உண்மை;
}

HB_INTERNAL உள்வரி
வெறுமை collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    மாறிலி Coverage &coverage,
                                    மாறிலி வெறுமை *base,
                                    glyph_data_t anchors,
                                    unsigned மொத்தம்,
                                    unsigned record_size)
{
  க்கு (மாறிலி auto _ : hb_zip (coverage, hb_range (மொத்தம்)))
  {
    எனில் (!c->glyph_set->has (_.first)) தொடர்;
    glyph_data_t anchor = anchors;
    anchor.data = (மாறிலி எழுத்து *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    எனில் (offset)
      StructAtOffset<மாறிலி Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
கட்டமைப்பு MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  வகைவரை typename Types::template ArrayOf<MarkRecord<Types>> Super;

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (Super::sanitize (c, இது));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி Coverage &coverage) மாறிலி
  {
    எனில் (!இது->len) திரும்பு;
    collect_mark_variation_indices (c, coverage, இது,
                                   &இது->arrayZ[0].markAnchor,
                                   இது->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  தருக்க apply (hb_ot_apply_context_t *c,
              unsigned முழு mark_index, unsigned முழு glyph_index,
              மாறிலி AnchorMatrixType &anchors, unsigned முழு class_மொத்தம்,
              unsigned முழு glyph_pos) மாறிலி
  {
    TRACE_APPLY (இது);
    மாறிலி MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned முழு mark_class = record.klass;

    மாறிலி Anchor& mark_anchor = இது + record.markAnchor;
    தருக்க found;
    மாறிலி Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_மொத்தம், &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    எனில் (unlikely (!found)) return_trace (பொய்);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  தருக்க subset (hb_subset_context_t *c,
               Iterator             coverage,
               மாறிலி hb_map_t      *klass_mapping) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (இது);
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    auto mark_iter =
    + hb_zip (coverage, இது->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    தருக்க ret = பொய்;
    unsigned new_length = 0;
    க்கு (மாறிலி auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, இது, klass_mapping);
      new_length++;
    }

    எனில் (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (பொய்);

    return_trace (ret);
  }
};

HB_INTERNAL உள்வரி
வெறுமை Markclass_closure_and_remap_indexes (மாறிலி Coverage &mark_coverage,
                                    மாறிலி வெறுமை *mark_records,
                                    unsigned மொத்தம்,
                                    unsigned record_size,
                                    மாறிலி hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  க்கு (மாறிலி auto _ : hb_zip (mark_coverage, hb_range (மொத்தம்)))
    எனில் (glyphset.has (_.first))
    {
      மாறிலி auto *klass = reinterpret_cast<மாறிலி HBUINT16 *>
        ((மாறிலி எழுத்து *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  க்கு (auto klass : orig_classes.iter ())
  {
    எனில் (klass_mapping->has (klass)) தொடர்;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL உள்வரி
வெறுமை Markclass_closure_and_remap_indexes (மாறிலி Coverage     &mark_coverage,
                                          மாறிலி MarkArray<Types> &mark_array,
                                          மாறிலி hb_set_t     &glyphset,
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

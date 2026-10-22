#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#شامل "AnchorMatrix.hh"
#شامل "MarkRecord.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

ایستا درونخطی hb_position_t
resolve_cross_offset (ثابت hb_glyph_position_t *pos,
		      unsigned صحیح len,
		      unsigned صحیح glyph_pos,
		      hb_direction_t direction)
{
  منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  وقتی (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحیح chain = pos[glyph_pos].attach_chain();
    اگر (!chain)
      بشکن;
    unsigned صحیح parent = (صحیح) glyph_pos + chain;
    اگر (unlikely (parent >= len))
      بشکن;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  بازگشت offset;
}

ایستا درونخطی منطقی
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  اعشاری mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (صحیح) glyph_pos - (صحیح) buffer->idx;
  اگر (mark.attach_chain() != (صحیح) glyph_pos - (صحیح) buffer->idx)
  {
    mark.attach_chain() = 0;
    برو overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  اگر (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  وگرنه
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  بازگشت درست;
}

HB_INTERNAL درونخطی
تهی collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت تهی *base,
                                    glyph_data_t anchors,
                                    unsigned شمار,
                                    unsigned record_size)
{
  برای (ثابت auto _ : hb_zip (coverage, hb_range (شمار)))
  {
    اگر (!c->glyph_set->has (_.first)) ادامه;
    glyph_data_t anchor = anchors;
    anchor.data = (ثابت نویسه *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    اگر (offset)
      StructAtOffset<ثابت Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
ساختار MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  تعریفنوع typename Types::template ArrayOf<MarkRecord<Types>> Super;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (Super::sanitize (c, این));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage) ثابت
  {
    اگر (!این->len) بازگشت;
    collect_mark_variation_indices (c, coverage, این,
                                   &این->arrayZ[0].markAnchor,
                                   این->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  منطقی apply (hb_ot_apply_context_t *c,
              unsigned صحیح mark_index, unsigned صحیح glyph_index,
              ثابت AnchorMatrixType &anchors, unsigned صحیح class_شمار,
              unsigned صحیح glyph_pos) ثابت
  {
    TRACE_APPLY (این);
    ثابت MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned صحیح mark_class = record.klass;

    ثابت Anchor& mark_anchor = این + record.markAnchor;
    منطقی found;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_شمار, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    اگر (unlikely (!found)) return_trace (نادرست);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقی subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (این);
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    auto mark_iter =
    + hb_zip (coverage, این->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    منطقی ret = نادرست;
    unsigned new_length = 0;
    برای (ثابت auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, این, klass_mapping);
      new_length++;
    }

    اگر (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (نادرست);

    return_trace (ret);
  }
};

HB_INTERNAL درونخطی
تهی Markclass_closure_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت تهی *mark_records,
                                    unsigned شمار,
                                    unsigned record_size,
                                    ثابت hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  برای (ثابت auto _ : hb_zip (mark_coverage, hb_range (شمار)))
    اگر (glyphset.has (_.first))
    {
      ثابت auto *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت نویسه *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  برای (auto klass : orig_classes.iter ())
  {
    اگر (klass_mapping->has (klass)) ادامه;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL درونخطی
تهی Markclass_closure_and_remap_indexes (ثابت Coverage     &mark_coverage,
                                          ثابت MarkArray<Types> &mark_array,
                                          ثابت hb_set_t     &glyphset,
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

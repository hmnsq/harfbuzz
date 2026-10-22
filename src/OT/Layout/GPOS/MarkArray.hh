#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#ضمن "AnchorMatrix.hh"
#ضمن "MarkRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

ساكن مضمن hb_position_t
resolve_cross_offset (ثابت hb_glyph_position_t *pos,
		      unsigned صحيح len,
		      unsigned صحيح glyph_pos,
		      hb_direction_t direction)
{
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  طالما (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحيح chain = pos[glyph_pos].attach_chain();
    𐤀𐤌 (!chain)
      اكسر;
    unsigned صحيح parent = (صحيح) glyph_pos + chain;
    𐤀𐤌 (unlikely (parent >= len))
      اكسر;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  أرجع offset;
}

ساكن مضمن منطقي
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  عائم mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  𐤀𐤌 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (صحيح) glyph_pos - (صحيح) buffer->idx;
  𐤀𐤌 (mark.attach_chain() != (صحيح) glyph_pos - (صحيح) buffer->idx)
  {
    mark.attach_chain() = 0;
    اذهب overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  𐤀𐤌 (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  وإلا
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  𐤀𐤌 (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  أرجع true;
}

HB_INTERNAL مضمن
فراغ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت فراغ *base,
                                    glyph_data_t anchors,
                                    unsigned count,
                                    unsigned record_size)
{
  لكل (ثابت auto _ : hb_zip (coverage, hb_range (count)))
  {
    𐤀𐤌 (!c->glyph_set->has (_.first)) استمر;
    glyph_data_t anchor = anchors;
    anchor.data = (ثابت حرف *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    𐤀𐤌 (offset)
      StructAtOffset<ثابت Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
بنية MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  تعريفنوع typename Types::template ArrayOf<MarkRecord<Types>> Super;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage) ثابت
  {
    𐤀𐤌 (!هذا->len) أرجع;
    collect_mark_variation_indices (c, coverage, هذا,
                                   &هذا->arrayZ[0].markAnchor,
                                   هذا->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  منطقي apply (hb_ot_apply_context_t *c,
              unsigned صحيح mark_index, unsigned صحيح glyph_index,
              ثابت AnchorMatrixType &anchors, unsigned صحيح class_count,
              unsigned صحيح glyph_pos) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned صحيح mark_class = record.klass;

    ثابت Anchor& mark_anchor = هذا + record.markAnchor;
    منطقي found;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_count, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    𐤀𐤌 (unlikely (!found)) return_trace (خطأ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (هذا);
    𐤀𐤌 (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    auto mark_iter =
    + hb_zip (coverage, هذا->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    منطقي ret = خطأ;
    unsigned new_length = 0;
    لكل (ثابت auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, هذا, klass_mapping);
      new_length++;
    }

    𐤀𐤌 (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (خطأ);

    return_trace (ret);
  }
};

HB_INTERNAL مضمن
فراغ Markclass_closure_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت فراغ *mark_records,
                                    unsigned count,
                                    unsigned record_size,
                                    ثابت hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  لكل (ثابت auto _ : hb_zip (mark_coverage, hb_range (count)))
    𐤀𐤌 (glyphset.has (_.first))
    {
      ثابت auto *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت حرف *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  لكل (auto klass : orig_classes.iter ())
  {
    𐤀𐤌 (klass_mapping->has (klass)) استمر;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL مضمن
فراغ Markclass_closure_and_remap_indexes (ثابت Coverage     &mark_coverage,
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

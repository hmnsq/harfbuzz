#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#अन्तर्भाव "AnchorMatrix.hh"
#अन्तर्भाव "MarkRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

स्थिर अन्तःस्थ hb_position_t
resolve_cross_offset (अचर hb_glyph_position_t *pos,
		      अचिन्हित पूर्णाङ्क len,
		      अचिन्हित पूर्णाङ्क glyph_pos,
		      hb_direction_t direction)
{
  तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  जबसम्म (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    पूर्णाङ्क chain = pos[glyph_pos].attach_chain();
    यदि (!chain)
      रोक;
    अचिन्हित पूर्णाङ्क parent = (पूर्णाङ्क) glyph_pos + chain;
    यदि (unlikely (parent >= len))
      रोक;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  फर्काऊ offset;
}

स्थिर अन्तःस्थ तार्किक
apply_mark_attachment (hb_ot_apply_context_t *c,
                       अचर Anchor &mark_anchor,
                       अचर Anchor &glyph_anchor,
                       अचिन्हित glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  भिन्न mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (पूर्णाङ्क) glyph_pos - (पूर्णाङ्क) buffer->idx;
  यदि (mark.attach_chain() != (पूर्णाङ्क) glyph_pos - (पूर्णाङ्क) buffer->idx)
  {
    mark.attach_chain() = 0;
    गच्छ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  यदि (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  अन्यथा
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  फर्काऊ सत्य;
}

HB_INTERNAL अन्तःस्थ
शून्य collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    अचर Coverage &coverage,
                                    अचर शून्य *base,
                                    glyph_data_t anchors,
                                    अचिन्हित गन्ती,
                                    अचिन्हित record_size)
{
  लागि (अचर स्वतः _ : hb_zip (coverage, hb_range (गन्ती)))
  {
    यदि (!c->glyph_set->has (_.first)) जारी;
    glyph_data_t anchor = anchors;
    anchor.data = (अचर वर्ण *) anchors.data + _.second * record_size;
    अचिन्हित offset = anchor[0];
    यदि (offset)
      StructAtOffset<अचर Anchor> (base, offset).collect_variation_indices (c);
  }
}

ढाँचा <typename Types>
संरचना MarkArray : Types::ढाँचा ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  प्रकारनिर्देश typename Types::ढाँचा ArrayOf<MarkRecord<Types>> Super;

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (Super::sanitize (c, एतत्));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर Coverage &coverage) अचर
  {
    यदि (!एतत्->len) फर्काऊ;
    collect_mark_variation_indices (c, coverage, एतत्,
                                   &एतत्->arrayZ[0].markAnchor,
                                   एतत्->len, MarkRecord<Types>::static_size);
  }

  ढाँचा <typename AnchorMatrixType>
  तार्किक apply (hb_ot_apply_context_t *c,
              अचिन्हित पूर्णाङ्क mark_index, अचिन्हित पूर्णाङ्क glyph_index,
              अचर AnchorMatrixType &anchors, अचिन्हित पूर्णाङ्क class_गन्ती,
              अचिन्हित पूर्णाङ्क glyph_pos) अचर
  {
    TRACE_APPLY (एतत्);
    अचर MarkRecord<Types> &record = Super::चालक[](mark_index);
    अचिन्हित पूर्णाङ्क mark_class = record.klass;

    अचर Anchor& mark_anchor = एतत् + record.markAnchor;
    तार्किक found;
    अचर Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_गन्ती, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    यदि (unlikely (!found)) return_trace (असत्य);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               Iterator             coverage,
               अचर hb_map_t      *klass_mapping) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    स्वतः* out = c->serializer->start_embed (एतत्);
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    स्वतः mark_iter =
    + hb_zip (coverage, एतत्->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    तार्किक ret = असत्य;
    अचिन्हित new_length = 0;
    लागि (अचर स्वतः& mark_record : mark_iter) {
      ret |= mark_record.subset (c, एतत्, klass_mapping);
      new_length++;
    }

    यदि (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (असत्य);

    return_trace (ret);
  }
};

HB_INTERNAL अन्तःस्थ
शून्य Markclass_closure_and_remap_indexes (अचर Coverage &mark_coverage,
                                    अचर शून्य *mark_records,
                                    अचिन्हित गन्ती,
                                    अचिन्हित record_size,
                                    अचर hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  लागि (अचर स्वतः _ : hb_zip (mark_coverage, hb_range (गन्ती)))
    यदि (glyphset.has (_.first))
    {
      अचर स्वतः *klass = reinterpret_cast<अचर HBUINT16 *>
        ((अचर वर्ण *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  अचिन्हित idx = 0;
  लागि (स्वतः klass : orig_classes.iter ())
  {
    यदि (klass_mapping->has (klass)) जारी;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

ढाँचा <typename Types>
HB_INTERNAL अन्तःस्थ
शून्य Markclass_closure_and_remap_indexes (अचर Coverage     &mark_coverage,
                                          अचर MarkArray<Types> &mark_array,
                                          अचर hb_set_t     &glyphset,
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

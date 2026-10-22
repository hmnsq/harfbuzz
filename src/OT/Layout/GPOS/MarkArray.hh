#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#अन्तर्भाव "AnchorMatrix.hh"
#अन्तर्भाव "MarkRecord.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

स्थिर ओळीत hb_position_t
resolve_cross_offset (स्थिरांक hb_glyph_position_t *pos,
		      अचिन्हित पूर्णांक len,
		      अचिन्हित पूर्णांक glyph_pos,
		      hb_direction_t direction)
{
  तार्किक horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  पर्यंत (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    पूर्णांक chain = pos[glyph_pos].attach_chain();
    जर (!chain)
      थांबा;
    अचिन्हित पूर्णांक parent = (पूर्णांक) glyph_pos + chain;
    जर (unlikely (parent >= len))
      थांबा;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  परत offset;
}

स्थिर ओळीत तार्किक
apply_mark_attachment (hb_ot_apply_context_t *c,
                       स्थिरांक Anchor &mark_anchor,
                       स्थिरांक Anchor &glyph_anchor,
                       अचिन्हित glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  दशांश mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (पूर्णांक) glyph_pos - (पूर्णांक) buffer->idx;
  जर (mark.attach_chain() != (पूर्णांक) glyph_pos - (पूर्णांक) buffer->idx)
  {
    mark.attach_chain() = 0;
    जा overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  जर (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  अन्यथा
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  परत सत्य;
}

HB_INTERNAL ओळीत
शून्य collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    स्थिरांक Coverage &coverage,
                                    स्थिरांक शून्य *base,
                                    glyph_data_t anchors,
                                    अचिन्हित मोजणी,
                                    अचिन्हित record_size)
{
  साठी (स्थिरांक स्वतः _ : hb_zip (coverage, hb_range (मोजणी)))
  {
    जर (!c->glyph_set->has (_.first)) सुरूठेवा;
    glyph_data_t anchor = anchors;
    anchor.data = (स्थिरांक अक्षर *) anchors.data + _.second * record_size;
    अचिन्हित offset = anchor[0];
    जर (offset)
      StructAtOffset<स्थिरांक Anchor> (base, offset).collect_variation_indices (c);
  }
}

ढाँचा <typename Types>
संरचना MarkArray : Types::ढाँचा ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  प्रकारव्याख्या typename Types::ढाँचा ArrayOf<MarkRecord<Types>> Super;

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (Super::sanitize (c, हे));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिरांक Coverage &coverage) स्थिरांक
  {
    जर (!हे->len) परत;
    collect_mark_variation_indices (c, coverage, हे,
                                   &हे->arrayZ[0].markAnchor,
                                   हे->len, MarkRecord<Types>::static_size);
  }

  ढाँचा <typename AnchorMatrixType>
  तार्किक apply (hb_ot_apply_context_t *c,
              अचिन्हित पूर्णांक mark_index, अचिन्हित पूर्णांक glyph_index,
              स्थिरांक AnchorMatrixType &anchors, अचिन्हित पूर्णांक class_मोजणी,
              अचिन्हित पूर्णांक glyph_pos) स्थिरांक
  {
    TRACE_APPLY (हे);
    स्थिरांक MarkRecord<Types> &record = Super::चालक[](mark_index);
    अचिन्हित पूर्णांक mark_class = record.klass;

    स्थिरांक Anchor& mark_anchor = हे + record.markAnchor;
    तार्किक found;
    स्थिरांक Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_मोजणी, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    जर (unlikely (!found)) return_trace (असत्य);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  ढाँचा <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  तार्किक subset (hb_subset_context_t *c,
               Iterator             coverage,
               स्थिरांक hb_map_t      *klass_mapping) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    स्वतः* out = c->serializer->start_embed (हे);
    जर (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    स्वतः mark_iter =
    + hb_zip (coverage, हे->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    तार्किक ret = असत्य;
    अचिन्हित new_length = 0;
    साठी (स्थिरांक स्वतः& mark_record : mark_iter) {
      ret |= mark_record.subset (c, हे, klass_mapping);
      new_length++;
    }

    जर (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (असत्य);

    return_trace (ret);
  }
};

HB_INTERNAL ओळीत
शून्य Markclass_closure_and_remap_indexes (स्थिरांक Coverage &mark_coverage,
                                    स्थिरांक शून्य *mark_records,
                                    अचिन्हित मोजणी,
                                    अचिन्हित record_size,
                                    स्थिरांक hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  साठी (स्थिरांक स्वतः _ : hb_zip (mark_coverage, hb_range (मोजणी)))
    जर (glyphset.has (_.first))
    {
      स्थिरांक स्वतः *klass = reinterpret_cast<स्थिरांक HBUINT16 *>
        ((स्थिरांक अक्षर *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  अचिन्हित idx = 0;
  साठी (स्वतः klass : orig_classes.iter ())
  {
    जर (klass_mapping->has (klass)) सुरूठेवा;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

ढाँचा <typename Types>
HB_INTERNAL ओळीत
शून्य Markclass_closure_and_remap_indexes (स्थिरांक Coverage     &mark_coverage,
                                          स्थिरांक MarkArray<Types> &mark_array,
                                          स्थिरांक hb_set_t     &glyphset,
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

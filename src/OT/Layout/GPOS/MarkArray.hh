#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#ಅನ್ತರ್ಭಾವ "AnchorMatrix.hh"
#ಅನ್ತರ್ಭಾವ "MarkRecord.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ hb_position_t
resolve_cross_offset (ಧ್ರುವ hb_glyph_position_t *pos,
		      unsigned ಪೂರ್ಣಾಙ್ಕ len,
		      unsigned ಪೂರ್ಣಾಙ್ಕ glyph_pos,
		      hb_direction_t direction)
{
  ತಾರ್ಕಿಕ horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  ಯಾವತ್ (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    ಪೂರ್ಣಾಙ್ಕ chain = pos[glyph_pos].attach_chain();
    ಯದಿ (!chain)
      ಭಙ್ಗ;
    unsigned ಪೂರ್ಣಾಙ್ಕ parent = (ಪೂರ್ಣಾಙ್ಕ) glyph_pos + chain;
    ಯದಿ (unlikely (parent >= len))
      ಭಙ್ಗ;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  ಪ್ರತ್ಯಾಗಮ offset;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ಧ್ರುವ Anchor &mark_anchor,
                       ಧ್ರುವ Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  ಭಿನ್ನ mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (ಪೂರ್ಣಾಙ್ಕ) glyph_pos - (ಪೂರ್ಣಾಙ್ಕ) buffer->idx;
  ಯದಿ (mark.attach_chain() != (ಪೂರ್ಣಾಙ್ಕ) glyph_pos - (ಪೂರ್ಣಾಙ್ಕ) buffer->idx)
  {
    mark.attach_chain() = 0;
    ಗಚ್ಛ overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  ಅನ್ಯಥಾ
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ಧ್ರುವ Coverage &coverage,
                                    ಧ್ರುವ ಶೂನ್ಯ *base,
                                    glyph_data_t anchors,
                                    unsigned ಎಣಿಕೆ,
                                    unsigned record_size)
{
  ಪ್ರತಿ (ಧ್ರುವ auto _ : hb_zip (coverage, hb_range (ಎಣಿಕೆ)))
  {
    ಯದಿ (!c->glyph_set->has (_.first)) ಅನುವರ್ತ;
    glyph_data_t anchor = anchors;
    anchor.data = (ಧ್ರುವ ವರ್ಣ *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    ಯದಿ (offset)
      StructAtOffset<ಧ್ರುವ Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
ಸಂರಚನಾ MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  ಪ್ರಕಾರನಿರ್ದೇಶ typename Types::template ArrayOf<MarkRecord<Types>> Super;

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (Super::sanitize (c, ಏತತ್));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ Coverage &coverage) ಧ್ರುವ
  {
    ಯದಿ (!ಏತತ್->len) ಪ್ರತ್ಯಾಗಮ;
    collect_mark_variation_indices (c, coverage, ಏತತ್,
                                   &ಏತತ್->arrayZ[0].markAnchor,
                                   ಏತತ್->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c,
              unsigned ಪೂರ್ಣಾಙ್ಕ mark_index, unsigned ಪೂರ್ಣಾಙ್ಕ glyph_index,
              ಧ್ರುವ AnchorMatrixType &anchors, unsigned ಪೂರ್ಣಾಙ್ಕ class_ಎಣಿಕೆ,
              unsigned ಪೂರ್ಣಾಙ್ಕ glyph_pos) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    ಧ್ರುವ MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned ಪೂರ್ಣಾಙ್ಕ mark_class = record.klass;

    ಧ್ರುವ Anchor& mark_anchor = ಏತತ್ + record.markAnchor;
    ತಾರ್ಕಿಕ found;
    ಧ್ರುವ Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_ಎಣಿಕೆ, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    ಯದಿ (unlikely (!found)) return_trace (ಅಸತ್ಯ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c,
               Iterator             coverage,
               ಧ್ರುವ hb_map_t      *klass_mapping) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (ಏತತ್);
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    auto mark_iter =
    + hb_zip (coverage, ಏತತ್->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    unsigned new_length = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, ಏತತ್, klass_mapping);
      new_length++;
    }

    ಯದಿ (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (ಅಸತ್ಯ);

    return_trace (ret);
  }
};

HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ Markclass_closure_and_remap_indexes (ಧ್ರುವ Coverage &mark_coverage,
                                    ಧ್ರುವ ಶೂನ್ಯ *mark_records,
                                    unsigned ಎಣಿಕೆ,
                                    unsigned record_size,
                                    ಧ್ರುವ hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  ಪ್ರತಿ (ಧ್ರುವ auto _ : hb_zip (mark_coverage, hb_range (ಎಣಿಕೆ)))
    ಯದಿ (glyphset.has (_.first))
    {
      ಧ್ರುವ auto *klass = reinterpret_cast<ಧ್ರುವ HBUINT16 *>
        ((ಧ್ರುವ ವರ್ಣ *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  ಪ್ರತಿ (auto klass : orig_classes.iter ())
  {
    ಯದಿ (klass_mapping->has (klass)) ಅನುವರ್ತ;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ Markclass_closure_and_remap_indexes (ಧ್ರುವ Coverage     &mark_coverage,
                                          ಧ್ರುವ MarkArray<Types> &mark_array,
                                          ಧ್ರುವ hb_set_t     &glyphset,
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

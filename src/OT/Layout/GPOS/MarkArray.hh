#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#അന്തര്ഭാവ "AnchorMatrix.hh"
#അന്തര്ഭാവ "MarkRecord.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

സ്ഥിതം വരിയിൽ hb_position_t
resolve_cross_offset (സ്ഥിരം hb_glyph_position_t *pos,
		      unsigned പൂർണ്ണസംഖ്യ len,
		      unsigned പൂർണ്ണസംഖ്യ glyph_pos,
		      hb_direction_t direction)
{
  തർക്കം horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  ആകുമ്പോൾ (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    പൂർണ്ണസംഖ്യ chain = pos[glyph_pos].attach_chain();
    എങ്കിൽ (!chain)
      നിർത്തുക;
    unsigned പൂർണ്ണസംഖ്യ parent = (പൂർണ്ണസംഖ്യ) glyph_pos + chain;
    എങ്കിൽ (unlikely (parent >= len))
      നിർത്തുക;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  തിരികെ offset;
}

സ്ഥിതം വരിയിൽ തർക്കം
apply_mark_attachment (hb_ot_apply_context_t *c,
                       സ്ഥിരം Anchor &mark_anchor,
                       സ്ഥിരം Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_buffer_t *buffer = c->buffer;
  ദശാംശം mark_x, mark_y, base_x, base_y;

  buffer->unsafe_to_break (glyph_pos, buffer->idx + 1);
  mark_anchor.get_anchor (c, buffer->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, buffer->info[glyph_pos].codepoint, &base_x, &base_y);

  എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (buffer->pos, buffer->len, glyph_pos,
						      buffer->props.direction);

  hb_glyph_position_t &mark = buffer->cur_pos();
  mark.attach_chain() = (പൂർണ്ണസംഖ്യ) glyph_pos - (പൂർണ്ണസംഖ്യ) buffer->idx;
  എങ്കിൽ (mark.attach_chain() != (പൂർണ്ണസംഖ്യ) glyph_pos - (പൂർണ്ണസംഖ്യ) buffer->idx)
  {
    mark.attach_chain() = 0;
    പോകുക overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (buffer->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  അല്ലെങ്കിൽ
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->buffer->idx, glyph_pos);
  }

overflow:
  buffer->idx++;
  തിരികെ സത്യം;
}

HB_INTERNAL വരിയിൽ
ശൂന്യം collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    സ്ഥിരം Coverage &coverage,
                                    സ്ഥിരം ശൂന്യം *base,
                                    glyph_data_t anchors,
                                    unsigned എണ്ണം,
                                    unsigned record_size)
{
  വേണ്ടി (സ്ഥിരം auto _ : hb_zip (coverage, hb_range (എണ്ണം)))
  {
    എങ്കിൽ (!c->glyph_set->has (_.first)) തുടരുക;
    glyph_data_t anchor = anchors;
    anchor.data = (സ്ഥിരം അക്ഷരം *) anchors.data + _.second * record_size;
    unsigned offset = anchor[0];
    എങ്കിൽ (offset)
      StructAtOffset<സ്ഥിരം Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
ഘടന MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  തരനിർവചനം typename Types::template ArrayOf<MarkRecord<Types>> Super;

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (Super::sanitize (c, ഇത്));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം Coverage &coverage) സ്ഥിരം
  {
    എങ്കിൽ (!ഇത്->len) തിരികെ;
    collect_mark_variation_indices (c, coverage, ഇത്,
                                   &ഇത്->arrayZ[0].markAnchor,
                                   ഇത്->len, MarkRecord<Types>::static_size);
  }

  template <typename AnchorMatrixType>
  തർക്കം apply (hb_ot_apply_context_t *c,
              unsigned പൂർണ്ണസംഖ്യ mark_index, unsigned പൂർണ്ണസംഖ്യ glyph_index,
              സ്ഥിരം AnchorMatrixType &anchors, unsigned പൂർണ്ണസംഖ്യ class_എണ്ണം,
              unsigned പൂർണ്ണസംഖ്യ glyph_pos) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    സ്ഥിരം MarkRecord<Types> &record = Super::operator[](mark_index);
    unsigned പൂർണ്ണസംഖ്യ mark_class = record.klass;

    സ്ഥിരം Anchor& mark_anchor = ഇത് + record.markAnchor;
    തർക്കം found;
    സ്ഥിരം Anchor& glyph_anchor = anchors.get_anchor (c, glyph_index, mark_class, class_എണ്ണം, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    എങ്കിൽ (unlikely (!found)) return_trace (അസത്യം);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  തർക്കം subset (hb_subset_context_t *c,
               Iterator             coverage,
               സ്ഥിരം hb_map_t      *klass_mapping) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (ഇത്);
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    auto mark_iter =
    + hb_zip (coverage, ഇത്->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    തർക്കം ret = അസത്യം;
    unsigned new_length = 0;
    വേണ്ടി (സ്ഥിരം auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, ഇത്, klass_mapping);
      new_length++;
    }

    എങ്കിൽ (unlikely (!c->serializer->check_assign (out->len, new_length,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (അസത്യം);

    return_trace (ret);
  }
};

HB_INTERNAL വരിയിൽ
ശൂന്യം Markclass_closure_and_remap_indexes (സ്ഥിരം Coverage &mark_coverage,
                                    സ്ഥിരം ശൂന്യം *mark_records,
                                    unsigned എണ്ണം,
                                    unsigned record_size,
                                    സ്ഥിരം hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  വേണ്ടി (സ്ഥിരം auto _ : hb_zip (mark_coverage, hb_range (എണ്ണം)))
    എങ്കിൽ (glyphset.has (_.first))
    {
      സ്ഥിരം auto *klass = reinterpret_cast<സ്ഥിരം HBUINT16 *>
        ((സ്ഥിരം അക്ഷരം *) mark_records + _.second * record_size);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  വേണ്ടി (auto klass : orig_classes.iter ())
  {
    എങ്കിൽ (klass_mapping->has (klass)) തുടരുക;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL വരിയിൽ
ശൂന്യം Markclass_closure_and_remap_indexes (സ്ഥിരം Coverage     &mark_coverage,
                                          സ്ഥിരം MarkArray<Types> &mark_array,
                                          സ്ഥിരം hb_set_t     &glyphset,
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

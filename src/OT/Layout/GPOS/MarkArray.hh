#ifndef OT_LAYOUT_GPOS_MARKARRAY_HH
#define OT_LAYOUT_GPOS_MARKARRAY_HH

#অন্তর্ভাব "AnchorMatrix.hh"
#অন্তর্ভাব "MarkRecord.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

স্থির অন্তঃরেখ hb_position_t
resolve_cross_offset (ধ্রুবক hb_glyph_position_t *pos,
		      unsigned পূর্ণসংখ্যা len,
		      unsigned পূর্ণসংখ্যা glyph_pos,
		      hb_direction_t direction)
{
  যৌক্তিক horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t offset = horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset;
  যখন (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    পূর্ণসংখ্যা chain = pos[glyph_pos].attach_chain();
    যদি (!chain)
      বিরতি;
    unsigned পূর্ণসংখ্যা parent = (পূর্ণসংখ্যা) glyph_pos + chain;
    যদি (unlikely (parent >= len))
      বিরতি;
    glyph_pos = parent;
    offset = hb_saturate_add (offset,
			      horizontal ? pos[glyph_pos].y_offset : pos[glyph_pos].x_offset);
  }
  ফেরত offset;
}

স্থির অন্তঃরেখ যৌক্তিক
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ধ্রুবক Anchor &mark_anchor,
                       ধ্রুবক Anchor &glyph_anchor,
                       unsigned glyph_pos)
{
  hb_বাফার_t *বাফার = c->বাফার;
  দশমিক mark_x, mark_y, base_x, base_y;

  বাফার->unsafe_to_break (glyph_pos, বাফার->idx + 1);
  mark_anchor.get_anchor (c, বাফার->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, বাফার->info[glyph_pos].codepoint, &base_x, &base_y);

  যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
  {
    c->বাফার->message (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->বাফার->idx, glyph_pos);
  }

  hb_position_t base_offset = resolve_cross_offset (বাফার->pos, বাফার->len, glyph_pos,
						      বাফার->props.direction);

  hb_glyph_position_t &mark = বাফার->cur_pos();
  mark.attach_chain() = (পূর্ণসংখ্যা) glyph_pos - (পূর্ণসংখ্যা) বাফার->idx;
  যদি (mark.attach_chain() != (পূর্ণসংখ্যা) glyph_pos - (পূর্ণসংখ্যা) বাফার->idx)
  {
    mark.attach_chain() = 0;
    যাও overflow;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_offset = roundf (base_x - mark_x);
  mark.y_offset = roundf (base_y - mark_y);
  যদি (HB_DIRECTION_IS_HORIZONTAL (বাফার->props.direction))
    mark.y_offset = hb_saturate_add (mark.y_offset, base_offset);
  অন্যথা
    mark.x_offset = hb_saturate_add (mark.x_offset, base_offset);
  বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
  {
    c->বাফার->message (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->বাফার->idx, glyph_pos);
  }

overflow:
  বাফার->idx++;
  ফেরত সত্য;
}

HB_INTERNAL অন্তঃরেখ
শূন্য collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ধ্রুবক Coverage &coverage,
                                    ধ্রুবক শূন্য *base,
                                    glyph_তথ্য_t anchors,
                                    unsigned গুনতি,
                                    unsigned record_আকার)
{
  জন্য (ধ্রুবক auto _ : hb_zip (coverage, hb_range (গুনতি)))
  {
    যদি (!c->glyph_set->has (_.first)) চালু;
    glyph_তথ্য_t anchor = anchors;
    anchor.তথ্য = (ধ্রুবক অক্ষর *) anchors.তথ্য + _.second * record_আকার;
    unsigned offset = anchor[0];
    যদি (offset)
      StructAtOffset<ধ্রুবক Anchor> (base, offset).collect_variation_indices (c);
  }
}

template <typename Types>
কাঠামো MarkArray : Types::template ArrayOf<MarkRecord<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  প্রকারলেখ typename Types::template ArrayOf<MarkRecord<Types>> Super;

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (Super::sanitize (c, এই));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক Coverage &coverage) ধ্রুবক
  {
    যদি (!এই->len) ফেরত;
    collect_mark_variation_indices (c, coverage, এই,
                                   &এই->arrayZ[0].markAnchor,
                                   এই->len, MarkRecord<Types>::static_আকার);
  }

  template <typename AnchorMatrixType>
  যৌক্তিক apply (hb_ot_apply_context_t *c,
              unsigned পূর্ণসংখ্যা mark_সূচক, unsigned পূর্ণসংখ্যা glyph_সূচক,
              ধ্রুবক AnchorMatrixType &anchors, unsigned পূর্ণসংখ্যা class_গুনতি,
              unsigned পূর্ণসংখ্যা glyph_pos) ধ্রুবক
  {
    TRACE_APPLY (এই);
    ধ্রুবক MarkRecord<Types> &record = Super::operator[](mark_সূচক);
    unsigned পূর্ণসংখ্যা mark_class = record.klass;

    ধ্রুবক Anchor& mark_anchor = এই + record.markAnchor;
    যৌক্তিক found;
    ধ্রুবক Anchor& glyph_anchor = anchors.get_anchor (c, glyph_সূচক, mark_class, class_গুনতি, &found);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    যদি (unlikely (!found)) return_trace (মিথ্যা);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  template <typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  যৌক্তিক subset (hb_subset_context_t *c,
               Iterator             coverage,
               ধ্রুবক hb_map_t      *klass_mapping) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();

    auto* out = c->serializer->start_embed (এই);
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    auto mark_iter =
    + hb_zip (coverage, এই->iter ())
    | hb_filter (glyphset, hb_first)
    | hb_map (hb_second)
    ;

    যৌক্তিক ret = মিথ্যা;
    unsigned new_দৈর্ঘ্য = 0;
    জন্য (ধ্রুবক auto& mark_record : mark_iter) {
      ret |= mark_record.subset (c, এই, klass_mapping);
      new_দৈর্ঘ্য++;
    }

    যদি (unlikely (!c->serializer->check_assign (out->len, new_দৈর্ঘ্য,
                                                HB_SERIALIZE_ERROR_ARRAY_OVERFLOW)))
      return_trace (মিথ্যা);

    return_trace (ret);
  }
};

HB_INTERNAL অন্তঃরেখ
শূন্য Markclass_closure_and_remap_indexes (ধ্রুবক Coverage &mark_coverage,
                                    ধ্রুবক শূন্য *mark_records,
                                    unsigned গুনতি,
                                    unsigned record_আকার,
                                    ধ্রুবক hb_set_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_set_t orig_classes;
  জন্য (ধ্রুবক auto _ : hb_zip (mark_coverage, hb_range (গুনতি)))
    যদি (glyphset.has (_.first))
    {
      ধ্রুবক auto *klass = reinterpret_cast<ধ্রুবক HBUINT16 *>
        ((ধ্রুবক অক্ষর *) mark_records + _.second * record_আকার);
      orig_classes.add (*klass);
    }

  unsigned idx = 0;
  জন্য (auto klass : orig_classes.iter ())
  {
    যদি (klass_mapping->has (klass)) চালু;
    klass_mapping->set (klass, idx);
    idx++;
  }
}

template <typename Types>
HB_INTERNAL অন্তঃরেখ
শূন্য Markclass_closure_and_remap_indexes (ধ্রুবক Coverage     &mark_coverage,
                                          ধ্রুবক MarkArray<Types> &mark_array,
                                          ধ্রুবক hb_set_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_closure_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       MarkRecord<Types>::static_আকার,
                                       glyphset, klass_mapping);
}

}
}
}

#endif /* OT_LAYOUT_GPOS_MARKARRAY_HH */

#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#شامل "Anchor.hh"
#شامل "../../../hb-limits.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

template <typename Types>
ساختار CursivePosFormat1_2;

template <typename Types>
ساختار EntryExitRecord
{
  template <typename Types2> friend ساختار CursivePosFormat1_2;

  منطقی sanitize (hb_sanitize_context_t *c, ثابت ساختار CursivePosFormat1_2<Types> *base) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ساختار CursivePosFormat1_2<Types> *src_base) ثابت
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت ساختار CursivePosFormat1_2<Types> *src_base) ثابت
  {
    TRACE_SERIALIZE (این);
    auto *out = c->serializer->embed (این);
    اگر (unlikely (!out)) return_trace (نادرست);

    منطقی ret = نادرست;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  محافظتشده:
  typename Types::template OffsetTo<Anchor, ساختار CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, ساختار CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عمومی:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

ایستا درونخطی تهی
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned صحیح len,
                              unsigned صحیح i,
                              hb_direction_t direction,
                              unsigned صحیح new_parent,
                              unsigned nesting_level = 0)
{
  اگر (nesting_level > HB_MAX_NESTING_LEVEL)
    بازگشت;

  صحیح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  اگر (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    بازگشت;

  pos[i].attach_chain() = 0;

  unsigned صحیح j = (صحیح) i + chain;

  اگر (unlikely (j >= len))
    بازگشت;

  /* Stop if we see new parent in the chain. */
  اگر (j == new_parent)
    بازگشت;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  اگر (unlikely (reversed_chain != -chain))
    بازگشت;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  وگرنه
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ایستا درونخطی منطقی
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &entry_anchor,
                          unsigned i)
{
  hb_buffer_t *buffer = c->buffer;
  unsigned صحیح j = buffer->idx;

  اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  اعشاری entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  انتخاب (c->direction) {
    حالت HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      بشکن;
    حالت HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      بشکن;
    حالت HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      بشکن;
    حالت HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      بشکن;
    حالت HB_DIRECTION_INVALID:
    پیشفرض:
      بشکن;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned صحیح child  = i;
  unsigned صحیح parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  اگر  (!(c->lookup_props & LookupFlag::راست_To_چپ))
  {
    unsigned صحیح k = child;
    child = parent;
    parent = k;
    x_offset = -x_offset;
    y_offset = -y_offset;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  reverse_cursive_minor_offset (pos, buffer->len, child, c->direction, parent);

  pos[child].attach_chain() = (صحیح) parent - (صحیح) child;
  اگر (pos[child].attach_chain() != (صحیح) parent - (صحیح) child)
  {
    pos[child].attach_chain() = 0;
    برو overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  اگر (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  وگرنه
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  اگر (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    وگرنه
	pos[parent].x_offset = 0;
  }

  اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  بازگشت درست;
}

template <typename Types>
ساختار CursivePosFormat1_2
{

  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    اگر (unlikely (!coverage.sanitize (c, این)))
      return_trace (نادرست);

    اگر (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    وگرنه
      return_trace (entryExitRecord.sanitize (c, این));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (این+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ثابت EntryExitRecord<Types>& record) { record.collect_variation_indices (c, این); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت; }

  تهی collect_second_glyphs (hb_set_digest_t *digest) ثابت
  { (این+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    hb_buffer_t *buffer = c->buffer;

    ثابت EntryExitRecord<Types> &this_record = entryExitRecord[(این+coverage).get_coverage  (buffer->cur().codepoint)];
    اگر (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, این))) return_trace (نادرست);
    hb_barrier ();

    auto &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (buffer->idx);
    unsigned unsafe_from;
    اگر (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (نادرست);
    }

    ثابت EntryExitRecord<Types> &prev_record = entryExitRecord[(این+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    اگر (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, این)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (نادرست);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            این+prev_record.exitAnchor,
                                            این+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  تهی serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت ساختار CursivePosFormat1_2<Types> *src_base)
  {
    اگر (unlikely (!c->serializer->extend_min ((*این)))) بازگشت;
    این->format = src_base->format;
    این->entryExitRecord.len = it.len ();

    برای (ثابت EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*این);

    auto it =
    + hb_zip (این+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ثابت EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت EntryExitRecord<Types>&>
                              { بازگشت hb_pair (glyph_map[p.first], p.second);})
    ;

    منطقی ret = منطقی (it);
    out->serialize (c, it, این);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

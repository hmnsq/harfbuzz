#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#অন্তর্ভাব "Anchor.hh"
#অন্তর্ভাব "../../../hb-limits.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

template <typename Types>
কাঠামো CursivePosFormat1_2;

template <typename Types>
কাঠামো EntryExitRecord
{
  template <typename Types2> friend কাঠামো CursivePosFormat1_2;

  যৌক্তিক sanitize (hb_sanitize_context_t *c, ধ্রুবক কাঠামো CursivePosFormat1_2<Types> *base) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক কাঠামো CursivePosFormat1_2<Types> *src_base) ধ্রুবক
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  যৌক্তিক subset (hb_subset_context_t *c,
	       ধ্রুবক কাঠামো CursivePosFormat1_2<Types> *src_base) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    auto *out = c->serializer->embed (এই);
    যদি (unlikely (!out)) return_trace (মিথ্যা);

    যৌক্তিক ret = মিথ্যা;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  সুরক্ষিত:
  typename Types::template OffsetTo<Anchor, কাঠামো CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::template OffsetTo<Anchor, কাঠামো CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  সর্বজনীন:
  DEFINE_আকার_STATIC (2 * Types::আকার_2);
};

স্থির অন্তঃরেখ শূন্য
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              unsigned পূর্ণসংখ্যা len,
                              unsigned পূর্ণসংখ্যা i,
                              hb_direction_t direction,
                              unsigned পূর্ণসংখ্যা new_parent,
                              unsigned nesting_level = 0)
{
  যদি (nesting_level > HB_MAX_NESTING_LEVEL)
    ফেরত;

  পূর্ণসংখ্যা chain = pos[i].attach_chain(), type = pos[i].attach_type();
  যদি (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    ফেরত;

  pos[i].attach_chain() = 0;

  unsigned পূর্ণসংখ্যা j = (পূর্ণসংখ্যা) i + chain;

  যদি (unlikely (j >= len))
    ফেরত;

  /* Stop if we see new parent in the chain. */
  যদি (j == new_parent)
    ফেরত;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  যদি (unlikely (reversed_chain != -chain))
    ফেরত;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  যদি (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  অন্যথা
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


স্থির অন্তঃরেখ যৌক্তিক
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ধ্রুবক Anchor &exit_anchor,
                          ধ্রুবক Anchor &entry_anchor,
                          unsigned i)
{
  hb_বাফার_t *বাফার = c->বাফার;
  unsigned পূর্ণসংখ্যা j = বাফার->idx;

  যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
  {
    c->বাফার->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  বাফার->unsafe_to_break (i, j + 1);
  দশমিক entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, বাফার->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, বাফার->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = বাফার->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  নির্বাচন (c->direction) {
    ক্ষেত্র HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      বিরতি;
    ক্ষেত্র HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      বিরতি;
    ক্ষেত্র HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      বিরতি;
    ক্ষেত্র HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      বিরতি;
    ক্ষেত্র HB_DIRECTION_INVALID:
    পূর্বনির্ধারিত:
      বিরতি;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  unsigned পূর্ণসংখ্যা child  = i;
  unsigned পূর্ণসংখ্যা parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  যদি  (!(c->lookup_props & LookupFlag::ডান_To_বাম))
  {
    unsigned পূর্ণসংখ্যা k = child;
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
  reverse_cursive_minor_offset (pos, বাফার->len, child, c->direction, parent);

  pos[child].attach_chain() = (পূর্ণসংখ্যা) parent - (পূর্ণসংখ্যা) child;
  যদি (pos[child].attach_chain() != (পূর্ণসংখ্যা) parent - (পূর্ণসংখ্যা) child)
  {
    pos[child].attach_chain() = 0;
    যাও overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  বাফার->scratch_flags |= HB_বাফার_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  যদি (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  অন্যথা
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  যদি (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    যদি (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    অন্যথা
	pos[parent].x_offset = 0;
  }

  যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
  {
    c->বাফার->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  বাফার->idx++;
  ফেরত সত্য;
}

template <typename Types>
কাঠামো CursivePosFormat1_2
{

  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::template ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার, entryExitRecord);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    যদি (unlikely (!coverage.sanitize (c, এই)))
      return_trace (মিথ্যা);

    যদি (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    অন্যথা
      return_trace (entryExitRecord.sanitize (c, এই));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত (এই+coverage).intersects (glyphs); }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুবক
  {
    + hb_zip (এই+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (ধ্রুবক EntryExitRecord<Types>& record) { record.collect_variation_indices (c, এই); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  { যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত; }

  শূন্য collect_second_glyphs (hb_set_digest_t *digest) ধ্রুবক
  { (এই+coverage).collect_coverage (digest); }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    hb_বাফার_t *বাফার = c->বাফার;

    ধ্রুবক EntryExitRecord<Types> &this_record = entryExitRecord[(এই+coverage).get_coverage  (বাফার->cur().codepoint)];
    যদি (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, এই))) return_trace (মিথ্যা);
    hb_barrier ();

    auto &skippy_iter = c->iter_প্রবেশ;
    skippy_iter.reset_fast (বাফার->idx);
    unsigned unsafe_from;
    যদি (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      বাফার->unsafe_to_concat_from_outbuffer (unsafe_from, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }

    ধ্রুবক EntryExitRecord<Types> &পূর্ব_record = entryExitRecord[(এই+coverage).get_coverage  (বাফার->info[skippy_iter.idx].codepoint)];
    যদি (!পূর্ব_record.exitAnchor ||
	unlikely (!পূর্ব_record.exitAnchor.sanitize (&c->sanitizer, এই)))
    {
      বাফার->unsafe_to_concat_from_outbuffer (skippy_iter.idx, বাফার->idx + 1);
      return_trace (মিথ্যা);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            এই+পূর্ব_record.exitAnchor,
                                            এই+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  template <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  শূন্য serialize (hb_subset_context_t *c,
                  Iterator it,
                  ধ্রুবক কাঠামো CursivePosFormat1_2<Types> *src_base)
  {
    যদি (unlikely (!c->serializer->extend_min ((*এই)))) ফেরত;
    এই->format = src_base->format;
    এই->entryExitRecord.len = it.len ();

    জন্য (ধ্রুবক EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    auto glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto *out = c->serializer->start_embed (*এই);

    auto it =
    + hb_zip (এই+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ধ্রুবক EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ধ্রুবক EntryExitRecord<Types>&>
                              { ফেরত hb_pair (glyph_map[p.first], p.second);})
    ;

    যৌক্তিক ret = যৌক্তিক (it);
    out->serialize (c, it, এই);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

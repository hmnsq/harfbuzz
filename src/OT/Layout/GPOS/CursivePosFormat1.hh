#ifndef OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#define OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#अन्तर्भाव "Anchor.hh"
#अन्तर्भाव "../../../hb-limits.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

ढाँचा <typename Types>
संरचना CursivePosFormat1_2;

ढाँचा <typename Types>
संरचना EntryExitRecord
{
  ढाँचा <typename Types2> मित्र संरचना CursivePosFormat1_2;

  तार्किक sanitize (hb_sanitize_context_t *c, स्थिरांक संरचना CursivePosFormat1_2<Types> *base) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिरांक संरचना CursivePosFormat1_2<Types> *src_base) स्थिरांक
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  तार्किक subset (hb_subset_context_t *c,
	       स्थिरांक संरचना CursivePosFormat1_2<Types> *src_base) स्थिरांक
  {
    TRACE_SERIALIZE (हे);
    स्वतः *out = c->serializer->embed (हे);
    जर (unlikely (!out)) return_trace (असत्य);

    तार्किक ret = असत्य;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  संरक्षित:
  typename Types::ढाँचा OffsetTo<Anchor, संरचना CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::ढाँचा OffsetTo<Anchor, संरचना CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  सार्वजनिक:
  DEFINE_SIZE_STATIC (2 * Types::size);
};

स्थिर ओळीत शून्य
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              अचिन्हित पूर्णांक len,
                              अचिन्हित पूर्णांक i,
                              hb_direction_t direction,
                              अचिन्हित पूर्णांक new_parent,
                              अचिन्हित nesting_level = 0)
{
  जर (nesting_level > HB_MAX_NESTING_LEVEL)
    परत;

  पूर्णांक chain = pos[i].attach_chain(), type = pos[i].attach_type();
  जर (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    परत;

  pos[i].attach_chain() = 0;

  अचिन्हित पूर्णांक j = (पूर्णांक) i + chain;

  जर (unlikely (j >= len))
    परत;

  /* Stop if we see new parent in the chain. */
  जर (j == new_parent)
    परत;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  जर (unlikely (reversed_chain != -chain))
    परत;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  जर (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  अन्यथा
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


स्थिर ओळीत तार्किक
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          स्थिरांक Anchor &exit_anchor,
                          स्थिरांक Anchor &entry_anchor,
                          अचिन्हित i)
{
  hb_buffer_t *buffer = c->buffer;
  अचिन्हित पूर्णांक j = buffer->idx;

  जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  buffer->unsafe_to_break (i, j + 1);
  दशांश entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, buffer->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, buffer->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = buffer->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  निवड (c->direction) {
    स्थिती HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      थांबा;
    स्थिती HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      थांबा;
    स्थिती HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      थांबा;
    स्थिती HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      थांबा;
    स्थिती HB_DIRECTION_INVALID:
    पूर्वनिर्धारित:
      थांबा;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  अचिन्हित पूर्णांक child  = i;
  अचिन्हित पूर्णांक parent = j;
  hb_position_t x_offset = (hb_position_t) roundf (entry_x - exit_x);
  hb_position_t y_offset = (hb_position_t) roundf (entry_y - exit_y);
  जर  (!(c->lookup_props & LookupFlag::उजवा_To_डावा))
  {
    अचिन्हित पूर्णांक k = child;
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

  pos[child].attach_chain() = (पूर्णांक) parent - (पूर्णांक) child;
  जर (pos[child].attach_chain() != (पूर्णांक) parent - (पूर्णांक) child)
  {
    pos[child].attach_chain() = 0;
    जा overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  buffer->scratch_flags |= HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  जर (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  अन्यथा
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  जर (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    जर (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    अन्यथा
	pos[parent].x_offset = 0;
  }

  जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
  {
    c->buffer->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  buffer->idx++;
  परत सत्य;
}

ढाँचा <typename Types>
संरचना CursivePosFormat1_2
{

  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::ढाँचा ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, entryExitRecord);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    जर (unlikely (!coverage.sanitize (c, हे)))
      return_trace (असत्य);

    जर (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    अन्यथा
      return_trace (entryExitRecord.sanitize (c, हे));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत (हे+coverage).intersects (glyphs); }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिरांक
  {
    + hb_zip (हे+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (स्थिरांक EntryExitRecord<Types>& record) { record.collect_variation_indices (c, हे); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  { जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत; }

  शून्य collect_second_glyphs (hb_set_digest_t *digest) स्थिरांक
  { (हे+coverage).collect_coverage (digest); }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    hb_buffer_t *buffer = c->buffer;

    स्थिरांक EntryExitRecord<Types> &this_record = entryExitRecord[(हे+coverage).get_coverage  (buffer->cur().codepoint)];
    जर (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, हे))) return_trace (असत्य);
    hb_barrier ();

    स्वतः &skippy_iter = c->iter_आदान;
    skippy_iter.reset_fast (buffer->idx);
    अचिन्हित unsafe_from;
    जर (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      buffer->unsafe_to_concat_from_outbuffer (unsafe_from, buffer->idx + 1);
      return_trace (असत्य);
    }

    स्थिरांक EntryExitRecord<Types> &prev_record = entryExitRecord[(हे+coverage).get_coverage  (buffer->info[skippy_iter.idx].codepoint)];
    जर (!prev_record.exitAnchor ||
	unlikely (!prev_record.exitAnchor.sanitize (&c->sanitizer, हे)))
    {
      buffer->unsafe_to_concat_from_outbuffer (skippy_iter.idx, buffer->idx + 1);
      return_trace (असत्य);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            हे+prev_record.exitAnchor,
                                            हे+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  शून्य serialize (hb_subset_context_t *c,
                  Iterator it,
                  स्थिरांक संरचना CursivePosFormat1_2<Types> *src_base)
  {
    जर (unlikely (!c->serializer->extend_min ((*हे)))) परत;
    हे->format = src_base->format;
    हे->entryExitRecord.len = it.len ();

    साठी (स्थिरांक EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*हे);

    स्वतः it =
    + hb_zip (हे+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, स्थिरांक EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, स्थिरांक EntryExitRecord<Types>&>
                              { परत hb_pair (glyph_map[p.first], p.second);})
    ;

    तार्किक ret = तार्किक (it);
    out->serialize (c, it, हे);
    return_trace (ret);
  }
};


}
}
}

#endif /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

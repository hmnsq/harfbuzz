#अगर_अघोषित OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#घोषणा OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#समावेश "Anchor.hh"
#समावेश "../../../hb-limits.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

ढाँचा <typename Types>
काष्ठा CursivePosFormat1_2;

ढाँचा <typename Types>
काष्ठा EntryExitRecord
{
  ढाँचा <typename Types2> मित्र काष्ठा CursivePosFormat1_2;

  bool sanitize (hb_sanitize_context_t *c, स्थिर काष्ठा CursivePosFormat1_2<Types> *base) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (entryAnchor.sanitize (c, base) && exitAnchor.sanitize (c, base));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर काष्ठा CursivePosFormat1_2<Types> *src_base) स्थिर
  {
    (src_base+entryAnchor).collect_variation_indices (c);
    (src_base+exitAnchor).collect_variation_indices (c);
  }

  bool subset (hb_subset_context_t *c,
	       स्थिर काष्ठा CursivePosFormat1_2<Types> *src_base) स्थिर
  {
    TRACE_SERIALIZE (यह);
    स्वतः *out = c->serializer->embed (यह);
    अगर (unlikely (!out)) return_trace (false);

    bool ret = false;
    ret |= out->entryAnchor.serialize_subset (c, entryAnchor, src_base);
    ret |= out->exitAnchor.serialize_subset (c, exitAnchor, src_base);
    return_trace (ret);
  }

  रक्षित:
  typename Types::ढाँचा OffsetTo<Anchor, काष्ठा CursivePosFormat1_2<Types>>
                entryAnchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  typename Types::ढाँचा OffsetTo<Anchor, काष्ठा CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  खुला:
  DEFINE_आकार_STATIC (2 * Types::आकार);
};

अटल अंतरभूत व्योम
reverse_cursive_minor_offset (hb_glyph_position_t *pos,
                              अचिन्हित पूर्णांक len,
                              अचिन्हित पूर्णांक i,
                              hb_direction_t direction,
                              अचिन्हित पूर्णांक new_parent,
                              अचिन्हित nesting_level = 0)
{
  अगर (nesting_level > HB_MAX_NESTING_LEVEL)
    वापस;

  पूर्णांक chain = pos[i].attach_chain(), type = pos[i].attach_type();
  अगर (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    वापस;

  pos[i].attach_chain() = 0;

  अचिन्हित पूर्णांक j = (पूर्णांक) i + chain;

  अगर (unlikely (j >= len))
    वापस;

  /* Stop if we see new parent in the chain. */
  अगर (j == new_parent)
    वापस;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  अगर (unlikely (reversed_chain != -chain))
    वापस;

  reverse_cursive_minor_offset (pos, len, j, direction, new_parent, nesting_level + 1);

  अगर (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_offset = hb_saturate_neg (pos[i].y_offset);
  अन्यथा
    pos[j].x_offset = hb_saturate_neg (pos[i].x_offset);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


अटल अंतरभूत bool
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          स्थिर Anchor &exit_anchor,
                          स्थिर Anchor &entry_anchor,
                          अचिन्हित i)
{
  hb_बफर_t *बफर = c->बफर;
  अचिन्हित पूर्णांक j = बफर->idx;

  अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
  {
    c->बफर->message (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  बफर->unsafe_to_break (i, j + 1);
  भग्न entry_x, entry_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, बफर->info[i].codepoint, &exit_x, &exit_y);
  entry_anchor.get_anchor (c, बफर->info[j].codepoint, &entry_x, &entry_y);

  hb_glyph_position_t *pos = बफर->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  चयन (c->direction) {
    हाल HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_offset = hb_saturate_sub (pos[j].x_offset, d);
      अवरोध;
    हाल HB_DIRECTION_RTL:
      d = hb_saturate_add ((hb_position_t) roundf (exit_x), pos[i].x_offset);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, d);

      pos[j].x_advance = hb_saturate_add ((hb_position_t) roundf (entry_x), pos[j].x_offset);
      अवरोध;
    हाल HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);

      d = hb_saturate_add ((hb_position_t) roundf (entry_y), pos[j].y_offset);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_offset = hb_saturate_sub (pos[j].y_offset, d);
      अवरोध;
    हाल HB_DIRECTION_BTT:
      d = hb_saturate_add ((hb_position_t) roundf (exit_y), pos[i].y_offset);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, d);

      pos[j].y_advance = (hb_position_t) roundf (entry_y);
      अवरोध;
    हाल HB_DIRECTION_INVALID:
    शेष:
      अवरोध;
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
  अगर  (!(c->lookup_props & LookupFlag::दायाँ_To_बायाँ))
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
  reverse_cursive_minor_offset (pos, बफर->len, child, c->direction, parent);

  pos[child].attach_chain() = (पूर्णांक) parent - (पूर्णांक) child;
  अगर (pos[child].attach_chain() != (पूर्णांक) parent - (पूर्णांक) child)
  {
    pos[child].attach_chain() = 0;
    जाओ overflow;
  }
  pos[child].attach_type() = ATTACH_TYPE_CURSIVE;
  बफर->scratch_flags |= HB_बफर_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  अगर (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[child].y_offset = y_offset;
  अन्यथा
    pos[child].x_offset = x_offset;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  अगर (unlikely (pos[parent].attach_chain() == -pos[child].attach_chain()))
  {
    pos[parent].attach_chain() = 0;
    अगर (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[parent].y_offset = 0;
    अन्यथा
	pos[parent].x_offset = 0;
  }

  अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
  {
    c->बफर->message (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

overflow:
  बफर->idx++;
  वापस true;
}

ढाँचा <typename Types>
काष्ठा CursivePosFormat1_2
{

  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  typename Types::ढाँचा ArrayOf<EntryExitRecord<Types>>
                entryExitRecord;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, entryExitRecord);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    अगर (unlikely (!coverage.sanitize (c, यह)))
      return_trace (false);

    अगर (c->lazy_some_gpos)
      return_trace (entryExitRecord.sanitize_shallow (c));
    अन्यथा
      return_trace (entryExitRecord.sanitize (c, यह));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c) स्थिर
  {
    + hb_zip (यह+coverage, entryExitRecord)
    | hb_filter (c->glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_apply ([&] (स्थिर EntryExitRecord<Types>& record) { record.collect_variation_indices (c, यह); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  { अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस; }

  व्योम collect_second_glyphs (hb_set_digest_t *digest) स्थिर
  { (यह+coverage).collect_coverage (digest); }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    hb_बफर_t *बफर = c->बफर;

    स्थिर EntryExitRecord<Types> &this_record = entryExitRecord[(यह+coverage).get_coverage  (बफर->cur().codepoint)];
    अगर (!this_record.entryAnchor ||
	unlikely (!this_record.entryAnchor.sanitize (&c->sanitizer, यह))) return_trace (false);
    hb_barrier ();

    स्वतः &skippy_iter = c->iter_निवेश;
    skippy_iter.reset_fast (बफर->idx);
    अचिन्हित unsafe_from;
    अगर (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      बफर->unsafe_to_concat_from_outbuffer (unsafe_from, बफर->idx + 1);
      return_trace (false);
    }

    स्थिर EntryExitRecord<Types> &पूर्व_record = entryExitRecord[(यह+coverage).get_coverage  (बफर->info[skippy_iter.idx].codepoint)];
    अगर (!पूर्व_record.exitAnchor ||
	unlikely (!पूर्व_record.exitAnchor.sanitize (&c->sanitizer, यह)))
    {
      बफर->unsafe_to_concat_from_outbuffer (skippy_iter.idx, बफर->idx + 1);
      return_trace (false);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            यह+पूर्व_record.exitAnchor,
                                            यह+this_record.entryAnchor,
                                            skippy_iter.idx));
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  व्योम serialize (hb_subset_context_t *c,
                  Iterator it,
                  स्थिर काष्ठा CursivePosFormat1_2<Types> *src_base)
  {
    अगर (unlikely (!c->serializer->extend_min ((*यह)))) वापस;
    यह->format = src_base->format;
    यह->entryExitRecord.len = it.len ();

    क्रम (स्थिर EntryExitRecord<Types>& entry_record : + it
                                                      | hb_map (hb_second))
      entry_record.subset (c, src_base);

    स्वतः glyphs =
    + it
    | hb_map_retains_sorting (hb_first)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);

    स्वतः it =
    + hb_zip (यह+coverage, entryExitRecord)
    | hb_filter (glyphset, hb_first)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, स्थिर EntryExitRecord<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, स्थिर EntryExitRecord<Types>&>
                              { वापस hb_pair (glyph_map[p.first], p.second);})
    ;

    bool ret = bool (it);
    out->serialize (c, it, यह);
    return_trace (ret);
  }
};


}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#నిర్వచించు OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#అన్తర్భావ "Anchor.hh"
#అన్తర్భావ "../../../hb-limits.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

మూస <రకం_పేరు Types>
నిర్మాణం CursivePosFormat1_2;

మూస <రకం_పేరు Types>
నిర్మాణం నమోదు_Exit_రికార్డు
{
  మూస <రకం_పేరు Types2> మిత్ర నిర్మాణం CursivePosFormat1_2;

  తార్కిక sanitize (hb_sanitize_context_t *c, స్థిరరాశి నిర్మాణం CursivePosFormat1_2<Types> *పునాది) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (నమోదు_Anchor.sanitize (c, పునాది) && exitAnchor.sanitize (c, పునాది));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి నిర్మాణం CursivePosFormat1_2<Types> *src_పునాది) స్థిరరాశి
  {
    (src_పునాది+నమోదు_Anchor).collect_variation_indices (c);
    (src_పునాది+exitAnchor).collect_variation_indices (c);
  }

  తార్కిక subset (hb_subset_context_t *c,
	       స్థిరరాశి నిర్మాణం CursivePosFormat1_2<Types> *src_పునాది) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    స్వయంచాలక *బహిర్గతం = c->serializer->embed (ఇది);
    ఒకవేళ (unlikely (!బహిర్గతం)) return_trace (అబద్ధం);

    తార్కిక ret = అబద్ధం;
    ret |= బహిర్గతం->నమోదు_Anchor.serialize_subset (c, నమోదు_Anchor, src_పునాది);
    ret |= బహిర్గతం->exitAnchor.serialize_subset (c, exitAnchor, src_పునాది);
    return_trace (ret);
  }

  రక్షిత:
  రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Anchor, నిర్మాణం CursivePosFormat1_2<Types>>
                నమోదు_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  రకం_పేరు Types::మూస స్థానభ్రంశం_గమ్యం<Anchor, నిర్మాణం CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  బహిరంగ:
  DEFINE_కొలత_STATIC (2 * Types::కొలత);
};

స్థిర అంతఃవరుస శూన్య
తిరగేయి_cursive_minor_స్థానభ్రంశం (hb_glyph_position_t *pos,
                              గుర్తులేని పూర్ణాంకం len,
                              గుర్తులేని పూర్ణాంకం i,
                              hb_direction_t direction,
                              గుర్తులేని పూర్ణాంకం new_మాతృ,
                              గుర్తులేని nesting_level = 0)
{
  ఒకవేళ (nesting_level > HB_MAX_NESTING_LEVEL)
    తిరిగి;

  పూర్ణాంకం chain = pos[i].attach_chain(), type = pos[i].attach_type();
  ఒకవేళ (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    తిరిగి;

  pos[i].attach_chain() = 0;

  గుర్తులేని పూర్ణాంకం j = (పూర్ణాంకం) i + chain;

  ఒకవేళ (unlikely (j >= len))
    తిరిగి;

  /* Stop if we see new parent in the chain. */
  ఒకవేళ (j == new_మాతృ)
    తిరిగి;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  ఒకవేళ (unlikely (reversed_chain != -chain))
    తిరిగి;

  తిరగేయి_cursive_minor_స్థానభ్రంశం (pos, len, j, direction, new_మాతృ, nesting_level + 1);

  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_స్థానభ్రంశం_2 = hb_saturate_neg (pos[i].y_స్థానభ్రంశం_2);
  లేకపోతే
    pos[j].x_స్థానభ్రంశం_2 = hb_saturate_neg (pos[i].x_స్థానభ్రంశం_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


స్థిర అంతఃవరుస తార్కిక
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          స్థిరరాశి Anchor &exit_anchor,
                          స్థిరరాశి Anchor &నమోదు_anchor,
                          గుర్తులేని i)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;
  గుర్తులేని పూర్ణాంకం j = బఫరు->idx;

  ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
  {
    c->బఫరు->సందేశం (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  బఫరు->unsafe_గమ్యం_break (i, j + 1);
  దశాంశం నమోదు_x, నమోదు_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, బఫరు->info[i].codepoint, &exit_x, &exit_y);
  నమోదు_anchor.get_anchor (c, బఫరు->info[j].codepoint, &నమోదు_x, &నమోదు_y);

  hb_glyph_position_t *pos = బఫరు->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  ఎంపిక (c->direction) {
    స్థితి HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_కూడు ((hb_position_t) roundf (exit_x), pos[i].x_స్థానభ్రంశం_2);

      d = hb_saturate_కూడు ((hb_position_t) roundf (నమోదు_x), pos[j].x_స్థానభ్రంశం_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_స్థానభ్రంశం_2 = hb_saturate_sub (pos[j].x_స్థానభ్రంశం_2, d);
      ఆపు;
    స్థితి HB_DIRECTION_RTL:
      d = hb_saturate_కూడు ((hb_position_t) roundf (exit_x), pos[i].x_స్థానభ్రంశం_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].x_స్థానభ్రంశం_2, d);

      pos[j].x_advance = hb_saturate_కూడు ((hb_position_t) roundf (నమోదు_x), pos[j].x_స్థానభ్రంశం_2);
      ఆపు;
    స్థితి HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_కూడు ((hb_position_t) roundf (exit_y), pos[i].y_స్థానభ్రంశం_2);

      d = hb_saturate_కూడు ((hb_position_t) roundf (నమోదు_y), pos[j].y_స్థానభ్రంశం_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_స్థానభ్రంశం_2 = hb_saturate_sub (pos[j].y_స్థానభ్రంశం_2, d);
      ఆపు;
    స్థితి HB_DIRECTION_BTT:
      d = hb_saturate_కూడు ((hb_position_t) roundf (exit_y), pos[i].y_స్థానభ్రంశం_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_స్థానభ్రంశం_2 = hb_saturate_sub (pos[i].y_స్థానభ్రంశం_2, d);

      pos[j].y_advance = (hb_position_t) roundf (నమోదు_y);
      ఆపు;
    స్థితి HB_DIRECTION_INVALID:
    అప్రమేయ:
      ఆపు;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  గుర్తులేని పూర్ణాంకం బిడ్డ  = i;
  గుర్తులేని పూర్ణాంకం మాతృ = j;
  hb_position_t x_స్థానభ్రంశం_2 = (hb_position_t) roundf (నమోదు_x - exit_x);
  hb_position_t y_స్థానభ్రంశం_2 = (hb_position_t) roundf (నమోదు_y - exit_y);
  ఒకవేళ  (!(c->lookup_props & LookupFlag::కుడి_గమ్యం_ఎడమ))
  {
    గుర్తులేని పూర్ణాంకం k = బిడ్డ;
    బిడ్డ = మాతృ;
    మాతృ = k;
    x_స్థానభ్రంశం_2 = -x_స్థానభ్రంశం_2;
    y_స్థానభ్రంశం_2 = -y_స్థానభ్రంశం_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  తిరగేయి_cursive_minor_స్థానభ్రంశం (pos, బఫరు->len, బిడ్డ, c->direction, మాతృ);

  pos[బిడ్డ].attach_chain() = (పూర్ణాంకం) మాతృ - (పూర్ణాంకం) బిడ్డ;
  ఒకవేళ (pos[బిడ్డ].attach_chain() != (పూర్ణాంకం) మాతృ - (పూర్ణాంకం) బిడ్డ)
  {
    pos[బిడ్డ].attach_chain() = 0;
    వెళ్ళు అధికప్రవాహం;
  }
  pos[బిడ్డ].attach_type() = ATTACH_TYPE_CURSIVE;
  బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[బిడ్డ].y_స్థానభ్రంశం_2 = y_స్థానభ్రంశం_2;
  లేకపోతే
    pos[బిడ్డ].x_స్థానభ్రంశం_2 = x_స్థానభ్రంశం_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  ఒకవేళ (unlikely (pos[మాతృ].attach_chain() == -pos[బిడ్డ].attach_chain()))
  {
    pos[మాతృ].attach_chain() = 0;
    ఒకవేళ (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[మాతృ].y_స్థానభ్రంశం_2 = 0;
    లేకపోతే
	pos[మాతృ].x_స్థానభ్రంశం_2 = 0;
  }

  ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
  {
    c->బఫరు->సందేశం (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

అధికప్రవాహం:
  బఫరు->idx++;
  తిరిగి నిజం;
}

మూస <రకం_పేరు Types>
నిర్మాణం CursivePosFormat1_2
{

  రక్షిత:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  రకం_పేరు Types::మూస LOffset_గమ్యం<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  రకం_పేరు Types::మూస Array_యొక్క<నమోదు_Exit_రికార్డు<Types>>
                నమోదు_Exit_రికార్డు_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (2 + Types::LOffset::static_కొలత + Types::HBUINT::static_కొలత, నమోదు_Exit_రికార్డు_2);

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    ఒకవేళ (unlikely (!coverage.sanitize (c, ఇది)))
      return_trace (అబద్ధం);

    ఒకవేళ (c->lazy_some_gpos)
      return_trace (నమోదు_Exit_రికార్డు_2.sanitize_shallow (c));
    లేకపోతే
      return_trace (నమోదు_Exit_రికార్డు_2.sanitize (c, ఇది));
  }

  తార్కిక intersects (స్థిరరాశి hb_సమితి_t *glyphs) స్థిరరాశి
  { తిరిగి (ఇది+coverage).intersects (glyphs); }

  శూన్య సంవృతం_lookups (hb_సంవృతం_lookups_context_t *c) స్థిరరాశి {}

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c) స్థిరరాశి
  {
    + hb_zip (ఇది+coverage, నమోదు_Exit_రికార్డు_2)
    | hb_filter (c->glyph_సమితి, hb_మొదటి)
    | hb_map (hb_రెండవ)
    | hb_apply ([&] (స్థిరరాశి నమోదు_Exit_రికార్డు<Types>& రికార్డు) { రికార్డు.collect_variation_indices (c, ఇది); })
    ;
  }

  శూన్య collect_glyphs (hb_collect_glyphs_context_t *c) స్థిరరాశి
  { ఒకవేళ (unlikely (!(ఇది+coverage).collect_coverage (c->ఆదానం))) తిరిగి; }

  శూన్య collect_రెండవ_glyphs (hb_సమితి_digest_t *digest) స్థిరరాశి
  { (ఇది+coverage).collect_coverage (digest); }

  స్థిరరాశి Coverage &get_coverage () స్థిరరాశి { తిరిగి ఇది+coverage; }

  తార్కిక apply (hb_ot_apply_context_t *c) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    hb_బఫరు_t *బఫరు = c->బఫరు;

    స్థిరరాశి నమోదు_Exit_రికార్డు<Types> &this_రికార్డు = నమోదు_Exit_రికార్డు_2[(ఇది+coverage).get_coverage  (బఫరు->cur().codepoint)];
    ఒకవేళ (!this_రికార్డు.నమోదు_Anchor ||
	unlikely (!this_రికార్డు.నమోదు_Anchor.sanitize (&c->sanitizer, ఇది))) return_trace (అబద్ధం);
    hb_barrier ();

    స్వయంచాలక &skippy_iter = c->iter_ఆదానం;
    skippy_iter.reset_fast (బఫరు->idx);
    గుర్తులేని unsafe_from;
    ఒకవేళ (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (unsafe_from, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }

    స్థిరరాశి నమోదు_Exit_రికార్డు<Types> &prev_రికార్డు = నమోదు_Exit_రికార్డు_2[(ఇది+coverage).get_coverage  (బఫరు->info[skippy_iter.idx].codepoint)];
    ఒకవేళ (!prev_రికార్డు.exitAnchor ||
	unlikely (!prev_రికార్డు.exitAnchor.sanitize (&c->sanitizer, ఇది)))
    {
      బఫరు->unsafe_గమ్యం_concat_from_outbuffer (skippy_iter.idx, బఫరు->idx + 1);
      return_trace (అబద్ధం);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            ఇది+prev_రికార్డు.exitAnchor,
                                            ఇది+this_రికార్డు.నమోదు_Anchor,
                                            skippy_iter.idx));
  }

  మూస <రకం_పేరు Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  శూన్య serialize (hb_subset_context_t *c,
                  Iterator it,
                  స్థిరరాశి నిర్మాణం CursivePosFormat1_2<Types> *src_పునాది)
  {
    ఒకవేళ (unlikely (!c->serializer->extend_min ((*ఇది)))) తిరిగి;
    ఇది->format = src_పునాది->format;
    ఇది->నమోదు_Exit_రికార్డు_2.len = it.len ();

    కొరకు (స్థిరరాశి నమోదు_Exit_రికార్డు<Types>& నమోదు_రికార్డు : + it
                                                      | hb_map (hb_రెండవ))
      నమోదు_రికార్డు.subset (c, src_పునాది);

    స్వయంచాలక glyphs =
    + it
    | hb_map_retains_క్రమీకరణ (hb_మొదటి)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  తార్కిక subset (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();
    స్థిరరాశి hb_map_t &glyph_map = *c->plan->glyph_map;

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed (*ఇది);

    స్వయంచాలక it =
    + hb_zip (ఇది+coverage, నమోదు_Exit_రికార్డు_2)
    | hb_filter (glyphset, hb_మొదటి)
    | hb_map_retains_క్రమీకరణ ([&] (hb_pair_t<hb_codepoint_t, స్థిరరాశి నమోదు_Exit_రికార్డు<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, స్థిరరాశి నమోదు_Exit_రికార్డు<Types>&>
                              { తిరిగి hb_pair (glyph_map[p.మొదటి], p.second);})
    ;

    తార్కిక ret = తార్కిక (it);
    బహిర్గతం->serialize (c, it, ఇది);
    return_trace (ret);
  }
};


}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

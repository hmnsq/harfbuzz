#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_MARKARRAY_HH
#నిర్వచించు OT_LAYOUT_GPOS_MARKARRAY_HH

#అన్తర్భావ "AnchorMatrix.hh"
#అన్తర్భావ "MarkRecord.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

స్థిర అంతఃవరుస hb_position_t
resolve_cross_స్థానభ్రంశం (స్థిరరాశి hb_glyph_position_t *pos,
		      గుర్తులేని పూర్ణాంకం len,
		      గుర్తులేని పూర్ణాంకం glyph_pos,
		      hb_direction_t direction)
{
  తార్కిక horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t స్థానభ్రంశం = horizontal ? pos[glyph_pos].y_స్థానభ్రంశం_2 : pos[glyph_pos].x_స్థానభ్రంశం_2;
  అయితే (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    పూర్ణాంకం chain = pos[glyph_pos].attach_chain();
    ఒకవేళ (!chain)
      ఆపు;
    గుర్తులేని పూర్ణాంకం మాతృ = (పూర్ణాంకం) glyph_pos + chain;
    ఒకవేళ (unlikely (మాతృ >= len))
      ఆపు;
    glyph_pos = మాతృ;
    స్థానభ్రంశం = hb_saturate_కూడు (స్థానభ్రంశం,
			      horizontal ? pos[glyph_pos].y_స్థానభ్రంశం_2 : pos[glyph_pos].x_స్థానభ్రంశం_2);
  }
  తిరిగి స్థానభ్రంశం;
}

స్థిర అంతఃవరుస తార్కిక
apply_mark_attachment (hb_ot_apply_context_t *c,
                       స్థిరరాశి Anchor &mark_anchor,
                       స్థిరరాశి Anchor &glyph_anchor,
                       గుర్తులేని glyph_pos)
{
  hb_బఫరు_t *బఫరు = c->బఫరు;
  దశాంశం mark_x, mark_y, పునాది_x, పునాది_y;

  బఫరు->unsafe_గమ్యం_break (glyph_pos, బఫరు->idx + 1);
  mark_anchor.get_anchor (c, బఫరు->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, బఫరు->info[glyph_pos].codepoint, &పునాది_x, &పునాది_y);

  ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
  {
    c->బఫరు->సందేశం (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->బఫరు->idx, glyph_pos);
  }

  hb_position_t పునాది_స్థానభ్రంశం = resolve_cross_స్థానభ్రంశం (బఫరు->pos, బఫరు->len, glyph_pos,
						      బఫరు->props.direction);

  hb_glyph_position_t &mark = బఫరు->cur_pos();
  mark.attach_chain() = (పూర్ణాంకం) glyph_pos - (పూర్ణాంకం) బఫరు->idx;
  ఒకవేళ (mark.attach_chain() != (పూర్ణాంకం) glyph_pos - (పూర్ణాంకం) బఫరు->idx)
  {
    mark.attach_chain() = 0;
    వెళ్ళు అధికప్రవాహం;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_స్థానభ్రంశం_2 = roundf (పునాది_x - mark_x);
  mark.y_స్థానభ్రంశం_2 = roundf (పునాది_y - mark_y);
  ఒకవేళ (HB_DIRECTION_IS_HORIZONTAL (బఫరు->props.direction))
    mark.y_స్థానభ్రంశం_2 = hb_saturate_కూడు (mark.y_స్థానభ్రంశం_2, పునాది_స్థానభ్రంశం);
  లేకపోతే
    mark.x_స్థానభ్రంశం_2 = hb_saturate_కూడు (mark.x_స్థానభ్రంశం_2, పునాది_స్థానభ్రంశం);
  బఫరు->scratch_flags |= HB_బఫరు_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  ఒకవేళ (HB_బఫరు_సందేశం_మరిన్ని && c->బఫరు->messaging ())
  {
    c->బఫరు->సందేశం (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->బఫరు->idx, glyph_pos);
  }

అధికప్రవాహం:
  బఫరు->idx++;
  తిరిగి నిజం;
}

HB_అంతర్గత అంతఃవరుస
శూన్య collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    స్థిరరాశి Coverage &coverage,
                                    స్థిరరాశి శూన్య *పునాది,
                                    glyph_దత్తాంశం_t anchors,
                                    గుర్తులేని లెక్క,
                                    గుర్తులేని రికార్డు_కొలత)
{
  కొరకు (స్థిరరాశి స్వయంచాలక _ : hb_zip (coverage, hb_range (లెక్క)))
  {
    ఒకవేళ (!c->glyph_సమితి->has (_.మొదటి)) కొనసాగు;
    glyph_దత్తాంశం_t anchor = anchors;
    anchor.దత్తాంశం = (స్థిరరాశి అక్షరం *) anchors.దత్తాంశం + _.second * రికార్డు_కొలత;
    గుర్తులేని స్థానభ్రంశం = anchor[0];
    ఒకవేళ (స్థానభ్రంశం)
      Struct_At_స్థానభ్రంశం<స్థిరరాశి Anchor> (పునాది, స్థానభ్రంశం).collect_variation_indices (c);
  }
}

మూస <రకం_పేరు Types>
నిర్మాణం MarkArray : Types::మూస Array_యొక్క<Mark_రికార్డు<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  రకనిర్వచనం రకం_పేరు Types::మూస Array_యొక్క<Mark_రికార్డు<Types>> Super;

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (Super::sanitize (c, ఇది));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి Coverage &coverage) స్థిరరాశి
  {
    ఒకవేళ (!ఇది->len) తిరిగి;
    collect_mark_variation_indices (c, coverage, ఇది,
                                   &ఇది->arrayZ[0].markAnchor,
                                   ఇది->len, Mark_రికార్డు<Types>::static_కొలత);
  }

  మూస <రకం_పేరు Anchor_మాతృక_Type>
  తార్కిక apply (hb_ot_apply_context_t *c,
              గుర్తులేని పూర్ణాంకం mark_సూచీ, గుర్తులేని పూర్ణాంకం glyph_సూచీ,
              స్థిరరాశి Anchor_మాతృక_Type &anchors, గుర్తులేని పూర్ణాంకం class_లెక్క,
              గుర్తులేని పూర్ణాంకం glyph_pos) స్థిరరాశి
  {
    TRACE_APPLY (ఇది);
    స్థిరరాశి Mark_రికార్డు<Types> &రికార్డు = Super::క్రియాకారకం[](mark_సూచీ);
    గుర్తులేని పూర్ణాంకం mark_class = రికార్డు.klass;

    స్థిరరాశి Anchor& mark_anchor = ఇది + రికార్డు.markAnchor;
    తార్కిక దొరికింది;
    స్థిరరాశి Anchor& glyph_anchor = anchors.get_anchor (c, glyph_సూచీ, mark_class, class_లెక్క, &దొరికింది);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    ఒకవేళ (unlikely (!దొరికింది)) return_trace (అబద్ధం);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  మూస <రకం_పేరు Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  తార్కిక subset (hb_subset_context_t *c,
               Iterator             coverage,
               స్థిరరాశి hb_map_t      *klass_mapping) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    స్థిరరాశి hb_సమితి_t &glyphset = *c->plan->glyphset_gsub ();

    స్వయంచాలక* బహిర్గతం = c->serializer->ప్రారంభం_embed (ఇది);
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    స్వయంచాలక mark_iter =
    + hb_zip (coverage, ఇది->iter ())
    | hb_filter (glyphset, hb_మొదటి)
    | hb_map (hb_రెండవ)
    ;

    తార్కిక ret = అబద్ధం;
    గుర్తులేని new_నిడివి = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక& mark_రికార్డు : mark_iter) {
      ret |= mark_రికార్డు.subset (c, ఇది, klass_mapping);
      new_నిడివి++;
    }

    ఒకవేళ (unlikely (!c->serializer->check_కేటాయించు (బహిర్గతం->len, new_నిడివి,
                                                HB_SERIALIZE_ERROR_ARRAY_అధికప్రవాహం)))
      return_trace (అబద్ధం);

    return_trace (ret);
  }
};

HB_అంతర్గత అంతఃవరుస
శూన్య Markclass_సంవృతం_and_remap_indexes (స్థిరరాశి Coverage &mark_coverage,
                                    స్థిరరాశి శూన్య *mark_రికార్డులు,
                                    గుర్తులేని లెక్క,
                                    గుర్తులేని రికార్డు_కొలత,
                                    స్థిరరాశి hb_సమితి_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_సమితి_t orig_classes;
  కొరకు (స్థిరరాశి స్వయంచాలక _ : hb_zip (mark_coverage, hb_range (లెక్క)))
    ఒకవేళ (glyphset.has (_.మొదటి))
    {
      స్థిరరాశి స్వయంచాలక *klass = reinterpret_cast<స్థిరరాశి HBUINT16 *>
        ((స్థిరరాశి అక్షరం *) mark_రికార్డులు + _.second * రికార్డు_కొలత);
      orig_classes.కూడు (*klass);
    }

  గుర్తులేని idx = 0;
  కొరకు (స్వయంచాలక klass : orig_classes.iter ())
  {
    ఒకవేళ (klass_mapping->has (klass)) కొనసాగు;
    klass_mapping->సమితి (klass, idx);
    idx++;
  }
}

మూస <రకం_పేరు Types>
HB_అంతర్గత అంతఃవరుస
శూన్య Markclass_సంవృతం_and_remap_indexes (స్థిరరాశి Coverage     &mark_coverage,
                                          స్థిరరాశి MarkArray<Types> &mark_array,
                                          స్థిరరాశి hb_సమితి_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_సంవృతం_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_రికార్డు<Types>::static_కొలత,
                                       glyphset, klass_mapping);
}

}
}
}

#అయితే_ముగింపు /* OT_LAYOUT_GPOS_MARKARRAY_HH */

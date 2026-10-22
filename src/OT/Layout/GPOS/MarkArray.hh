#جے_بنا_تعریف OT_LAYOUT_GPOS_MARKARRAY_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKARRAY_HH

#ضمن "AnchorMatrix.hh"
#ضمن "MarkRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

ساكن مضمن hb_position_t
resolve_cross_وتھ (ثابت hb_glyph_position_t *pos,
		      بنانشانی صحيح len,
		      بنانشانی صحيح glyph_pos,
		      hb_direction_t direction)
{
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t وتھ = horizontal ? pos[glyph_pos].y_وتھ_2 : pos[glyph_pos].x_وتھ_2;
  طالما (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحيح chain = pos[glyph_pos].attach_chain();
    إذا (!chain)
      اكسر;
    بنانشانی صحيح ماپا = (صحيح) glyph_pos + chain;
    إذا (unlikely (ماپا >= len))
      اكسر;
    glyph_pos = ماپا;
    وتھ = hb_saturate_جوڑو (وتھ,
			      horizontal ? pos[glyph_pos].y_وتھ_2 : pos[glyph_pos].x_وتھ_2);
  }
  أرجع وتھ;
}

ساكن مضمن منطقي
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       بنانشانی glyph_pos)
{
  hb_بفر_t *بفر = c->بفر;
  عائم mark_x, mark_y, ادھار_x, ادھار_y;

  بفر->unsafe_منزل_break (glyph_pos, بفر->idx + 1);
  mark_anchor.get_anchor (c, بفر->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, بفر->info[glyph_pos].codepoint, &ادھار_x, &ادھار_y);

  إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
  {
    c->بفر->سنیہا (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

  hb_position_t ادھار_وتھ = resolve_cross_وتھ (بفر->pos, بفر->len, glyph_pos,
						      بفر->props.direction);

  hb_glyph_position_t &mark = بفر->cur_pos();
  mark.attach_chain() = (صحيح) glyph_pos - (صحيح) بفر->idx;
  إذا (mark.attach_chain() != (صحيح) glyph_pos - (صحيح) بفر->idx)
  {
    mark.attach_chain() = 0;
    اذهب حد_توں_ودھ;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_وتھ_2 = roundf (ادھار_x - mark_x);
  mark.y_وتھ_2 = roundf (ادھار_y - mark_y);
  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    mark.y_وتھ_2 = hb_saturate_جوڑو (mark.y_وتھ_2, ادھار_وتھ);
  وإلا
    mark.x_وتھ_2 = hb_saturate_جوڑو (mark.x_وتھ_2, ادھار_وتھ);
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
  {
    c->بفر->سنیہا (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

حد_توں_ودھ:
  بفر->idx++;
  أرجع سچ;
}

HB_اندرونی مضمن
فراغ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت فراغ *ادھار,
                                    glyph_انکڑے_t anchors,
                                    بنانشانی گنتی,
                                    بنانشانی ریکارڈ_ناپ)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (گنتی)))
  {
    إذا (!c->glyph_سیٹ->has (_.پہلا)) استمر;
    glyph_انکڑے_t anchor = anchors;
    anchor.انکڑے = (ثابت حرف *) anchors.انکڑے + _.second * ریکارڈ_ناپ;
    بنانشانی وتھ = anchor[0];
    إذا (وتھ)
      Struct_At_وتھ<ثابت Anchor> (ادھار, وتھ).collect_variation_indices (c);
  }
}

سانچا <قسم_ناں Types>
بنية MarkArray : Types::سانچا Array_دا<Mark_ریکارڈ<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  تعريفنوع قسم_ناں Types::سانچا Array_دا<Mark_ریکارڈ<Types>> Super;

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (Super::sanitize (c, هذا));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage) ثابت
  {
    إذا (!هذا->len) أرجع;
    collect_mark_variation_indices (c, coverage, هذا,
                                   &هذا->arrayZ[0].markAnchor,
                                   هذا->len, Mark_ریکارڈ<Types>::static_ناپ);
  }

  سانچا <قسم_ناں Anchor_میٹرکس_Type>
  منطقي apply (hb_ot_apply_context_t *c,
              بنانشانی صحيح mark_اشاریہ, بنانشانی صحيح glyph_اشاریہ,
              ثابت Anchor_میٹرکس_Type &anchors, بنانشانی صحيح class_گنتی,
              بنانشانی صحيح glyph_pos) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت Mark_ریکارڈ<Types> &ریکارڈ = Super::عامل[](mark_اشاریہ);
    بنانشانی صحيح mark_class = ریکارڈ.klass;

    ثابت Anchor& mark_anchor = هذا + ریکارڈ.markAnchor;
    منطقي لبھیا;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_اشاریہ, mark_class, class_گنتی, &لبھیا);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    إذا (unlikely (!لبھیا)) return_trace (خطأ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  سانچا <قسم_ناں Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();

    خودکار* باہرلا = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    خودکار mark_iter =
    + hb_zip (coverage, هذا->iter ())
    | hb_filter (glyphset, hb_پہلا)
    | hb_map (hb_دوجا)
    ;

    منطقي ret = خطأ;
    بنانشانی new_لمائی = 0;
    لكل (ثابت خودکار& mark_ریکارڈ : mark_iter) {
      ret |= mark_ریکارڈ.subset (c, هذا, klass_mapping);
      new_لمائی++;
    }

    إذا (unlikely (!c->serializer->check_سونپو (باہرلا->len, new_لمائی,
                                                HB_SERIALIZE_ERROR_ARRAY_حد_توں_ودھ)))
      return_trace (خطأ);

    return_trace (ret);
  }
};

HB_اندرونی مضمن
فراغ Markclass_گھیرا_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت فراغ *mark_ریکارڈاں,
                                    بنانشانی گنتی,
                                    بنانشانی ریکارڈ_ناپ,
                                    ثابت hb_سیٹ_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_سیٹ_t orig_classes;
  لكل (ثابت خودکار _ : hb_zip (mark_coverage, hb_range (گنتی)))
    إذا (glyphset.has (_.پہلا))
    {
      ثابت خودکار *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت حرف *) mark_ریکارڈاں + _.second * ریکارڈ_ناپ);
      orig_classes.جوڑو (*klass);
    }

  بنانشانی idx = 0;
  لكل (خودکار klass : orig_classes.iter ())
  {
    إذا (klass_mapping->has (klass)) استمر;
    klass_mapping->سیٹ (klass, idx);
    idx++;
  }
}

سانچا <قسم_ناں Types>
HB_اندرونی مضمن
فراغ Markclass_گھیرا_and_remap_indexes (ثابت Coverage     &mark_coverage,
                                          ثابت MarkArray<Types> &mark_array,
                                          ثابت hb_سیٹ_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_گھیرا_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_ریکارڈ<Types>::static_ناپ,
                                       glyphset, klass_mapping);
}

}
}
}

#جے_انت /* OT_LAYOUT_GPOS_MARKARRAY_HH */

#که_نه_تعریف_شوی OT_LAYOUT_GPOS_MARKARRAY_HH
#تعریف_کړه OT_LAYOUT_GPOS_MARKARRAY_HH

#ضمن "AnchorMatrix.hh"
#ضمن "MarkRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

ساكن مضمن hb_position_t
resolve_cross_دننه_واټن (ثابت hb_glyph_position_t *pos,
		      بینښه صحيح len,
		      بینښه صحيح glyph_pos,
		      hb_direction_t direction)
{
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t دننه_واټن = horizontal ? pos[glyph_pos].y_دننه_واټن_2 : pos[glyph_pos].x_دننه_واټن_2;
  طالما (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحيح chain = pos[glyph_pos].attach_chain();
    إذا (!chain)
      اكسر;
    بینښه صحيح پلار = (صحيح) glyph_pos + chain;
    إذا (unlikely (پلار >= len))
      اكسر;
    glyph_pos = پلار;
    دننه_واټن = hb_saturate_جمع_کړه (دننه_واټن,
			      horizontal ? pos[glyph_pos].y_دننه_واټن_2 : pos[glyph_pos].x_دننه_واټن_2);
  }
  أرجع دننه_واټن;
}

ساكن مضمن منطقي
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       بینښه glyph_pos)
{
  hb_بفر_t *بفر = c->بفر;
  عائم mark_x, mark_y, بنسټ_x, بنسټ_y;

  بفر->unsafe_منزل_break (glyph_pos, بفر->idx + 1);
  mark_anchor.get_anchor (c, بفر->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, بفر->info[glyph_pos].codepoint, &بنسټ_x, &بنسټ_y);

  إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

  hb_position_t بنسټ_دننه_واټن = resolve_cross_دننه_واټن (بفر->pos, بفر->len, glyph_pos,
						      بفر->props.direction);

  hb_glyph_position_t &mark = بفر->cur_pos();
  mark.attach_chain() = (صحيح) glyph_pos - (صحيح) بفر->idx;
  إذا (mark.attach_chain() != (صحيح) glyph_pos - (صحيح) بفر->idx)
  {
    mark.attach_chain() = 0;
    اذهب ډکېدنه;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_دننه_واټن_2 = roundf (بنسټ_x - mark_x);
  mark.y_دننه_واټن_2 = roundf (بنسټ_y - mark_y);
  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    mark.y_دننه_واټن_2 = hb_saturate_جمع_کړه (mark.y_دننه_واټن_2, بنسټ_دننه_واټن);
  وإلا
    mark.x_دننه_واټن_2 = hb_saturate_جمع_کړه (mark.x_دننه_واټن_2, بنسټ_دننه_واټن);
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

ډکېدنه:
  بفر->idx++;
  أرجع ریښتیا;
}

HB_دننی مضمن
فراغ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت فراغ *بنسټ,
                                    glyph_معلومات_t anchors,
                                    بینښه شمېر,
                                    بینښه ریکارډ_اندازه)
{
  لكل (ثابت خپلکاری _ : hb_zip (coverage, hb_range (شمېر)))
  {
    إذا (!c->glyph_سېټ->has (_.لومړی)) استمر;
    glyph_معلومات_t anchor = anchors;
    anchor.معلومات = (ثابت حرف *) anchors.معلومات + _.second * ریکارډ_اندازه;
    بینښه دننه_واټن = anchor[0];
    إذا (دننه_واټن)
      Struct_At_دننه_واټن<ثابت Anchor> (بنسټ, دننه_واټن).collect_variation_indices (c);
  }
}

کينډۍ <ډول_نوم Types>
بنية MarkArray : Types::کينډۍ Array_د<Mark_ریکارډ<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  تعريفنوع ډول_نوم Types::کينډۍ Array_د<Mark_ریکارډ<Types>> Super;

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
                                   هذا->len, Mark_ریکارډ<Types>::static_اندازه);
  }

  کينډۍ <ډول_نوم Anchor_ماتریکس_Type>
  منطقي apply (hb_ot_apply_context_t *c,
              بینښه صحيح mark_شاخص, بینښه صحيح glyph_شاخص,
              ثابت Anchor_ماتریکس_Type &anchors, بینښه صحيح class_شمېر,
              بینښه صحيح glyph_pos) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت Mark_ریکارډ<Types> &ریکارډ = Super::چلوونکی[](mark_شاخص);
    بینښه صحيح mark_class = ریکارډ.klass;

    ثابت Anchor& mark_anchor = هذا + ریکارډ.markAnchor;
    منطقي وموندل;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_شاخص, mark_class, class_شمېر, &وموندل);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    إذا (unlikely (!وموندل)) return_trace (خطأ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  کينډۍ <ډول_نوم Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();

    خپلکاری* بهر = c->serializer->پیل_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    خپلکاری mark_iter =
    + hb_zip (coverage, هذا->iter ())
    | hb_filter (glyphset, hb_لومړی)
    | hb_map (hb_دوهم)
    ;

    منطقي ret = خطأ;
    بینښه new_اوږدتیا = 0;
    لكل (ثابت خپلکاری& mark_ریکارډ : mark_iter) {
      ret |= mark_ریکارډ.subset (c, هذا, klass_mapping);
      new_اوږدتیا++;
    }

    إذا (unlikely (!c->serializer->check_ټاکل (بهر->len, new_اوږدتیا,
                                                HB_SERIALIZE_ERROR_ARRAY_ډکېدنه)))
      return_trace (خطأ);

    return_trace (ret);
  }
};

HB_دننی مضمن
فراغ Markclass_تړون_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت فراغ *mark_ریکارډونه,
                                    بینښه شمېر,
                                    بینښه ریکارډ_اندازه,
                                    ثابت hb_سېټ_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_سېټ_t orig_classes;
  لكل (ثابت خپلکاری _ : hb_zip (mark_coverage, hb_range (شمېر)))
    إذا (glyphset.has (_.لومړی))
    {
      ثابت خپلکاری *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت حرف *) mark_ریکارډونه + _.second * ریکارډ_اندازه);
      orig_classes.جمع_کړه (*klass);
    }

  بینښه idx = 0;
  لكل (خپلکاری klass : orig_classes.iter ())
  {
    إذا (klass_mapping->has (klass)) استمر;
    klass_mapping->سېټ (klass, idx);
    idx++;
  }
}

کينډۍ <ډول_نوم Types>
HB_دننی مضمن
فراغ Markclass_تړون_and_remap_indexes (ثابت Coverage     &mark_coverage,
                                          ثابت MarkArray<Types> &mark_array,
                                          ثابت hb_سېټ_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_تړون_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_ریکارډ<Types>::static_اندازه,
                                       glyphset, klass_mapping);
}

}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_MARKARRAY_HH */

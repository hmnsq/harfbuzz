#اگر_تعریف_نشده OT_LAYOUT_GPOS_MARKARRAY_HH
#تعریف_کن OT_LAYOUT_GPOS_MARKARRAY_HH

#ضمن "AnchorMatrix.hh"
#ضمن "MarkRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

ساكن مضمن hb_position_t
resolve_cross_جابه_جایی (ثابت hb_glyph_position_t *pos,
		      بیعلامت صحيح len,
		      بیعلامت صحيح glyph_pos,
		      hb_direction_t direction)
{
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t جابه_جایی = horizontal ? pos[glyph_pos].y_جابه_جایی_2 : pos[glyph_pos].x_جابه_جایی_2;
  طالما (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحيح chain = pos[glyph_pos].attach_chain();
    إذا (!chain)
      اكسر;
    بیعلامت صحيح والد = (صحيح) glyph_pos + chain;
    إذا (unlikely (والد >= len))
      اكسر;
    glyph_pos = والد;
    جابه_جایی = hb_saturate_جمع_کن (جابه_جایی,
			      horizontal ? pos[glyph_pos].y_جابه_جایی_2 : pos[glyph_pos].x_جابه_جایی_2);
  }
  أرجع جابه_جایی;
}

ساكن مضمن منطقي
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       بیعلامت glyph_pos)
{
  hb_میانگیر_t *میانگیر = c->میانگیر;
  عائم mark_x, mark_y, پایه_x, پایه_y;

  میانگیر->unsafe_مقصد_break (glyph_pos, میانگیر->idx + 1);
  mark_anchor.get_anchor (c, میانگیر->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, میانگیر->info[glyph_pos].codepoint, &پایه_x, &پایه_y);

  إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
  {
    c->میانگیر->پیام (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->میانگیر->idx, glyph_pos);
  }

  hb_position_t پایه_جابه_جایی = resolve_cross_جابه_جایی (میانگیر->pos, میانگیر->len, glyph_pos,
						      میانگیر->props.direction);

  hb_glyph_position_t &mark = میانگیر->cur_pos();
  mark.attach_chain() = (صحيح) glyph_pos - (صحيح) میانگیر->idx;
  إذا (mark.attach_chain() != (صحيح) glyph_pos - (صحيح) میانگیر->idx)
  {
    mark.attach_chain() = 0;
    اذهب سرریز;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_جابه_جایی_2 = roundf (پایه_x - mark_x);
  mark.y_جابه_جایی_2 = roundf (پایه_y - mark_y);
  إذا (HB_DIRECTION_IS_HORIZONTAL (میانگیر->props.direction))
    mark.y_جابه_جایی_2 = hb_saturate_جمع_کن (mark.y_جابه_جایی_2, پایه_جابه_جایی);
  وإلا
    mark.x_جابه_جایی_2 = hb_saturate_جمع_کن (mark.x_جابه_جایی_2, پایه_جابه_جایی);
  میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
  {
    c->میانگیر->پیام (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->میانگیر->idx, glyph_pos);
  }

سرریز:
  میانگیر->idx++;
  أرجع درست;
}

HB_داخلی مضمن
فراغ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت فراغ *پایه,
                                    glyph_داده_t anchors,
                                    بیعلامت شمار,
                                    بیعلامت رکورد_اندازه)
{
  لكل (ثابت خودکار _ : hb_zip (coverage, hb_range (شمار)))
  {
    إذا (!c->glyph_مجموعه->has (_.اولی)) استمر;
    glyph_داده_t anchor = anchors;
    anchor.داده = (ثابت حرف *) anchors.داده + _.second * رکورد_اندازه;
    بیعلامت جابه_جایی = anchor[0];
    إذا (جابه_جایی)
      Struct_At_جابه_جایی<ثابت Anchor> (پایه, جابه_جایی).collect_variation_indices (c);
  }
}

قالب <نام_نوع Types>
بنية MarkArray : Types::قالب Array_از<Mark_رکورد<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  تعريفنوع نام_نوع Types::قالب Array_از<Mark_رکورد<Types>> Super;

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
                                   هذا->len, Mark_رکورد<Types>::static_اندازه);
  }

  قالب <نام_نوع Anchor_ماتریس_Type>
  منطقي apply (hb_ot_apply_context_t *c,
              بیعلامت صحيح mark_شاخص, بیعلامت صحيح glyph_شاخص,
              ثابت Anchor_ماتریس_Type &anchors, بیعلامت صحيح class_شمار,
              بیعلامت صحيح glyph_pos) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت Mark_رکورد<Types> &رکورد = Super::عملگر[](mark_شاخص);
    بیعلامت صحيح mark_class = رکورد.klass;

    ثابت Anchor& mark_anchor = هذا + رکورد.markAnchor;
    منطقي یافت;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_شاخص, mark_class, class_شمار, &یافت);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    إذا (unlikely (!یافت)) return_trace (خطأ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  قالب <نام_نوع Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();

    خودکار* بیرون = c->serializer->شروع_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    خودکار mark_iter =
    + hb_zip (coverage, هذا->iter ())
    | hb_filter (glyphset, hb_اولی)
    | hb_map (hb_دومی)
    ;

    منطقي ret = خطأ;
    بیعلامت new_درازی = 0;
    لكل (ثابت خودکار& mark_رکورد : mark_iter) {
      ret |= mark_رکورد.subset (c, هذا, klass_mapping);
      new_درازی++;
    }

    إذا (unlikely (!c->serializer->check_تخصیص_بده (بیرون->len, new_درازی,
                                                HB_SERIALIZE_ERROR_ARRAY_سرریز)))
      return_trace (خطأ);

    return_trace (ret);
  }
};

HB_داخلی مضمن
فراغ Markclass_بستگی_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت فراغ *mark_رکوردها,
                                    بیعلامت شمار,
                                    بیعلامت رکورد_اندازه,
                                    ثابت hb_مجموعه_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_مجموعه_t orig_classes;
  لكل (ثابت خودکار _ : hb_zip (mark_coverage, hb_range (شمار)))
    إذا (glyphset.has (_.اولی))
    {
      ثابت خودکار *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت حرف *) mark_رکوردها + _.second * رکورد_اندازه);
      orig_classes.جمع_کن (*klass);
    }

  بیعلامت idx = 0;
  لكل (خودکار klass : orig_classes.iter ())
  {
    إذا (klass_mapping->has (klass)) استمر;
    klass_mapping->مجموعه (klass, idx);
    idx++;
  }
}

قالب <نام_نوع Types>
HB_داخلی مضمن
فراغ Markclass_بستگی_and_remap_indexes (ثابت Coverage     &mark_coverage,
                                          ثابت MarkArray<Types> &mark_array,
                                          ثابت hb_مجموعه_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_بستگی_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_رکورد<Types>::static_اندازه,
                                       glyphset, klass_mapping);
}

}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_MARKARRAY_HH */

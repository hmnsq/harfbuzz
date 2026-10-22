#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_MARKARRAY_HH
#تعریف_کرو OT_LAYOUT_GPOS_MARKARRAY_HH

#شامل "AnchorMatrix.hh"
#شامل "MarkRecord.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

ساکن اندرونی hb_position_t
resolve_cross_انحراف (ثابت hb_glyph_position_t *pos,
		      غیرعلامتی صحیح len,
		      غیرعلامتی صحیح glyph_pos,
		      hb_direction_t direction)
{
  منطقی horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t انحراف = horizontal ? pos[glyph_pos].y_انحراف_2 : pos[glyph_pos].x_انحراف_2;
  جب (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحیح chain = pos[glyph_pos].attach_chain();
    اگر (!chain)
      توڑو;
    غیرعلامتی صحیح والد = (صحیح) glyph_pos + chain;
    اگر (unlikely (والد >= len))
      توڑو;
    glyph_pos = والد;
    انحراف = hb_saturate_جوڑو (انحراف,
			      horizontal ? pos[glyph_pos].y_انحراف_2 : pos[glyph_pos].x_انحراف_2);
  }
  واپس انحراف;
}

ساکن اندرونی منطقی
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       غیرعلامتی glyph_pos)
{
  hb_بفر_t *بفر = c->بفر;
  اعشاری mark_x, mark_y, بنیاد_x, بنیاد_y;

  بفر->unsafe_مقصد_break (glyph_pos, بفر->idx + 1);
  mark_anchor.get_anchor (c, بفر->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, بفر->info[glyph_pos].codepoint, &بنیاد_x, &بنیاد_y);

  اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

  hb_position_t بنیاد_انحراف = resolve_cross_انحراف (بفر->pos, بفر->len, glyph_pos,
						      بفر->props.direction);

  hb_glyph_position_t &mark = بفر->cur_pos();
  mark.attach_chain() = (صحیح) glyph_pos - (صحیح) بفر->idx;
  اگر (mark.attach_chain() != (صحیح) glyph_pos - (صحیح) بفر->idx)
  {
    mark.attach_chain() = 0;
    جاؤ حد_سے_تجاوز;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_انحراف_2 = roundf (بنیاد_x - mark_x);
  mark.y_انحراف_2 = roundf (بنیاد_y - mark_y);
  اگر (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    mark.y_انحراف_2 = hb_saturate_جوڑو (mark.y_انحراف_2, بنیاد_انحراف);
  ورنہ
    mark.x_انحراف_2 = hb_saturate_جوڑو (mark.x_انحراف_2, بنیاد_انحراف);
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

حد_سے_تجاوز:
  بفر->idx++;
  واپس سچ;
}

HB_اندرونی اندرونی
خالی collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت خالی *بنیاد,
                                    glyph_ڈیٹا_t anchors,
                                    غیرعلامتی گنتی,
                                    غیرعلامتی ریکارڈ_سائز)
{
  برائے (ثابت خودکار _ : hb_zip (coverage, hb_range (گنتی)))
  {
    اگر (!c->glyph_سیٹ->has (_.پہلا)) جاری;
    glyph_ڈیٹا_t anchor = anchors;
    anchor.ڈیٹا = (ثابت حرف *) anchors.ڈیٹا + _.second * ریکارڈ_سائز;
    غیرعلامتی انحراف = anchor[0];
    اگر (انحراف)
      Struct_At_انحراف<ثابت Anchor> (بنیاد, انحراف).collect_variation_indices (c);
  }
}

سانچہ <قسم_نام Types>
ساخت MarkArray : Types::سانچہ Array_کا<Mark_ریکارڈ<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  قسمتعریف قسم_نام Types::سانچہ Array_کا<Mark_ریکارڈ<Types>> Super;

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (Super::sanitize (c, یہ));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت Coverage &coverage) ثابت
  {
    اگر (!یہ->len) واپس;
    collect_mark_variation_indices (c, coverage, یہ,
                                   &یہ->arrayZ[0].markAnchor,
                                   یہ->len, Mark_ریکارڈ<Types>::static_سائز);
  }

  سانچہ <قسم_نام Anchor_میٹرکس_Type>
  منطقی apply (hb_ot_apply_context_t *c,
              غیرعلامتی صحیح mark_اشاریہ, غیرعلامتی صحیح glyph_اشاریہ,
              ثابت Anchor_میٹرکس_Type &anchors, غیرعلامتی صحیح class_گنتی,
              غیرعلامتی صحیح glyph_pos) ثابت
  {
    TRACE_APPLY (یہ);
    ثابت Mark_ریکارڈ<Types> &ریکارڈ = Super::عامل[](mark_اشاریہ);
    غیرعلامتی صحیح mark_class = ریکارڈ.klass;

    ثابت Anchor& mark_anchor = یہ + ریکارڈ.markAnchor;
    منطقی ملا;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_اشاریہ, mark_class, class_گنتی, &ملا);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    اگر (unlikely (!ملا)) return_trace (جھوٹ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  سانچہ <قسم_نام Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقی subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();

    خودکار* برآمد = c->serializer->آغاز_embed (یہ);
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    خودکار mark_iter =
    + hb_zip (coverage, یہ->iter ())
    | hb_filter (glyphset, hb_پہلا)
    | hb_map (hb_دوسرا)
    ;

    منطقی ret = جھوٹ;
    غیرعلامتی new_لمبائی = 0;
    برائے (ثابت خودکار& mark_ریکارڈ : mark_iter) {
      ret |= mark_ریکارڈ.subset (c, یہ, klass_mapping);
      new_لمبائی++;
    }

    اگر (unlikely (!c->serializer->check_تفویض (برآمد->len, new_لمبائی,
                                                HB_SERIALIZE_ERROR_ARRAY_حد_سے_تجاوز)))
      return_trace (جھوٹ);

    return_trace (ret);
  }
};

HB_اندرونی اندرونی
خالی Markclass_احاطہ_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت خالی *mark_اندراجات,
                                    غیرعلامتی گنتی,
                                    غیرعلامتی ریکارڈ_سائز,
                                    ثابت hb_سیٹ_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_سیٹ_t orig_classes;
  برائے (ثابت خودکار _ : hb_zip (mark_coverage, hb_range (گنتی)))
    اگر (glyphset.has (_.پہلا))
    {
      ثابت خودکار *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت حرف *) mark_اندراجات + _.second * ریکارڈ_سائز);
      orig_classes.جوڑو (*klass);
    }

  غیرعلامتی idx = 0;
  برائے (خودکار klass : orig_classes.iter ())
  {
    اگر (klass_mapping->has (klass)) جاری;
    klass_mapping->سیٹ (klass, idx);
    idx++;
  }
}

سانچہ <قسم_نام Types>
HB_اندرونی اندرونی
خالی Markclass_احاطہ_and_remap_indexes (ثابت Coverage     &mark_coverage,
                                          ثابت MarkArray<Types> &mark_array,
                                          ثابت hb_سیٹ_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_احاطہ_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_ریکارڈ<Types>::static_سائز,
                                       glyphset, klass_mapping);
}

}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_MARKARRAY_HH */

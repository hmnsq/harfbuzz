#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_MARKARRAY_HH
#وصف_ڪريو OT_LAYOUT_GPOS_MARKARRAY_HH

#ضمن "AnchorMatrix.hh"
#ضمن "MarkRecord.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

ساكن مضمن hb_position_t
resolve_cross_وٿي (ثابت hb_glyph_position_t *pos,
		      بنانشان صحيح len,
		      بنانشان صحيح glyph_pos,
		      hb_direction_t direction)
{
  منطقي horizontal = HB_DIRECTION_IS_HORIZONTAL (direction);
  hb_position_t وٿي = horizontal ? pos[glyph_pos].y_وٿي_2 : pos[glyph_pos].x_وٿي_2;
  طالما (pos[glyph_pos].attach_type() & ATTACH_TYPE_CURSIVE)
  {
    صحيح chain = pos[glyph_pos].attach_chain();
    إذا (!chain)
      اكسر;
    بنانشان صحيح والد = (صحيح) glyph_pos + chain;
    إذا (unlikely (والد >= len))
      اكسر;
    glyph_pos = والد;
    وٿي = hb_saturate_جوڙيو (وٿي,
			      horizontal ? pos[glyph_pos].y_وٿي_2 : pos[glyph_pos].x_وٿي_2);
  }
  أرجع وٿي;
}

ساكن مضمن منطقي
apply_mark_attachment (hb_ot_apply_context_t *c,
                       ثابت Anchor &mark_anchor,
                       ثابت Anchor &glyph_anchor,
                       بنانشان glyph_pos)
{
  hb_بفر_t *بفر = c->بفر;
  عائم mark_x, mark_y, بنياد_x, بنياد_y;

  بفر->unsafe_منزل_break (glyph_pos, بفر->idx + 1);
  mark_anchor.get_anchor (c, بفر->cur().codepoint, &mark_x, &mark_y);
  glyph_anchor.get_anchor (c, بفر->info[glyph_pos].codepoint, &بنياد_x, &بنياد_y);

  إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
  {
    c->بفر->پيغام (c->font,
			  "attaching mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

  hb_position_t بنياد_وٿي = resolve_cross_وٿي (بفر->pos, بفر->len, glyph_pos,
						      بفر->props.direction);

  hb_glyph_position_t &mark = بفر->cur_pos();
  mark.attach_chain() = (صحيح) glyph_pos - (صحيح) بفر->idx;
  إذا (mark.attach_chain() != (صحيح) glyph_pos - (صحيح) بفر->idx)
  {
    mark.attach_chain() = 0;
    اذهب حد_کان_مٿي;
  }
  mark.attach_type() = ATTACH_TYPE_MARK;
  mark.x_وٿي_2 = roundf (بنياد_x - mark_x);
  mark.y_وٿي_2 = roundf (بنياد_y - mark_y);
  إذا (HB_DIRECTION_IS_HORIZONTAL (بفر->props.direction))
    mark.y_وٿي_2 = hb_saturate_جوڙيو (mark.y_وٿي_2, بنياد_وٿي);
  وإلا
    mark.x_وٿي_2 = hb_saturate_جوڙيو (mark.x_وٿي_2, بنياد_وٿي);
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;

  إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
  {
    c->بفر->پيغام (c->font,
			  "attached mark glyph at %u to glyph at %u",
			  c->بفر->idx, glyph_pos);
  }

حد_کان_مٿي:
  بفر->idx++;
  أرجع سچ;
}

HB_اندروني مضمن
فراغ collect_mark_variation_indices (hb_collect_variation_indices_context_t *c,
                                    ثابت Coverage &coverage,
                                    ثابت فراغ *بنياد,
                                    glyph_معلومات_t anchors,
                                    بنانشان ڳڻپ,
                                    بنانشان رڪارڊ_سائيز)
{
  لكل (ثابت خودڪار _ : hb_zip (coverage, hb_range (ڳڻپ)))
  {
    إذا (!c->glyph_سيٽ->has (_.پهريون)) استمر;
    glyph_معلومات_t anchor = anchors;
    anchor.معلومات = (ثابت حرف *) anchors.معلومات + _.second * رڪارڊ_سائيز;
    بنانشان وٿي = anchor[0];
    إذا (وٿي)
      Struct_At_وٿي<ثابت Anchor> (بنياد, وٿي).collect_variation_indices (c);
  }
}

سانچو <قسم_نالو Types>
بنية MarkArray : Types::سانچو Array_جو<Mark_رڪارڊ<Types>>
                                                /* Array of MarkRecords--in Coverage order */
{
  تعريفنوع قسم_نالو Types::سانچو Array_جو<Mark_رڪارڊ<Types>> Super;

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
                                   هذا->len, Mark_رڪارڊ<Types>::static_سائيز);
  }

  سانچو <قسم_نالو Anchor_ميٽرڪس_Type>
  منطقي apply (hb_ot_apply_context_t *c,
              بنانشان صحيح mark_اشاريو, بنانشان صحيح glyph_اشاريو,
              ثابت Anchor_ميٽرڪس_Type &anchors, بنانشان صحيح class_ڳڻپ,
              بنانشان صحيح glyph_pos) ثابت
  {
    TRACE_APPLY (هذا);
    ثابت Mark_رڪارڊ<Types> &رڪارڊ = Super::عامل[](mark_اشاريو);
    بنانشان صحيح mark_class = رڪارڊ.klass;

    ثابت Anchor& mark_anchor = هذا + رڪارڊ.markAnchor;
    منطقي مليو;
    ثابت Anchor& glyph_anchor = anchors.get_anchor (c, glyph_اشاريو, mark_class, class_ڳڻپ, &مليو);
    /* If this subtable doesn't have an anchor for this base and this class,
     * return false such that the subsequent subtables have a chance at it. */
    إذا (unlikely (!مليو)) return_trace (خطأ);

    return_trace (apply_mark_attachment (c, mark_anchor, glyph_anchor, glyph_pos));
  }

  سانچو <قسم_نالو Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  منطقي subset (hb_subset_context_t *c,
               Iterator             coverage,
               ثابت hb_map_t      *klass_mapping) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();

    خودڪار* ٻاهريون = c->serializer->شروعات_embed (هذا);
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    خودڪار mark_iter =
    + hb_zip (coverage, هذا->iter ())
    | hb_filter (glyphset, hb_پهريون)
    | hb_map (hb_ٻيو)
    ;

    منطقي ret = خطأ;
    بنانشان new_ڊيگهائي = 0;
    لكل (ثابت خودڪار& mark_رڪارڊ : mark_iter) {
      ret |= mark_رڪارڊ.subset (c, هذا, klass_mapping);
      new_ڊيگهائي++;
    }

    إذا (unlikely (!c->serializer->check_سونپيو (ٻاهريون->len, new_ڊيگهائي,
                                                HB_SERIALIZE_ERROR_ARRAY_حد_کان_مٿي)))
      return_trace (خطأ);

    return_trace (ret);
  }
};

HB_اندروني مضمن
فراغ Markclass_گهيرو_and_remap_indexes (ثابت Coverage &mark_coverage,
                                    ثابت فراغ *mark_رڪارڊون,
                                    بنانشان ڳڻپ,
                                    بنانشان رڪارڊ_سائيز,
                                    ثابت hb_سيٽ_t &glyphset,
                                    hb_map_t *klass_mapping)
{
  hb_سيٽ_t orig_classes;
  لكل (ثابت خودڪار _ : hb_zip (mark_coverage, hb_range (ڳڻپ)))
    إذا (glyphset.has (_.پهريون))
    {
      ثابت خودڪار *klass = reinterpret_cast<ثابت HBUINT16 *>
        ((ثابت حرف *) mark_رڪارڊون + _.second * رڪارڊ_سائيز);
      orig_classes.جوڙيو (*klass);
    }

  بنانشان idx = 0;
  لكل (خودڪار klass : orig_classes.iter ())
  {
    إذا (klass_mapping->has (klass)) استمر;
    klass_mapping->سيٽ (klass, idx);
    idx++;
  }
}

سانچو <قسم_نالو Types>
HB_اندروني مضمن
فراغ Markclass_گهيرو_and_remap_indexes (ثابت Coverage     &mark_coverage,
                                          ثابت MarkArray<Types> &mark_array,
                                          ثابت hb_سيٽ_t     &glyphset,
                                          hb_map_t*           klass_mapping /* INOUT */)
{
  Markclass_گهيرو_and_remap_indexes (mark_coverage,
                                       mark_array.arrayZ, mark_array.len,
                                       Mark_رڪارڊ<Types>::static_سائيز,
                                       glyphset, klass_mapping);
}

}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_MARKARRAY_HH */

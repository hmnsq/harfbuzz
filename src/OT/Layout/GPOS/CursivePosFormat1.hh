#اگر_تعریف_نشده OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#تعریف_کن OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ضمن "Anchor.hh"
#ضمن "../../../hb-limits.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

قالب <نام_نوع Types>
بنية CursivePosFormat1_2;

قالب <نام_نوع Types>
بنية مدخل_Exit_رکورد
{
  قالب <نام_نوع Types2> دوست بنية CursivePosFormat1_2;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنية CursivePosFormat1_2<Types> *پایه) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (مدخل_Anchor.sanitize (c, پایه) && exitAnchor.sanitize (c, پایه));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بنية CursivePosFormat1_2<Types> *src_پایه) ثابت
  {
    (src_پایه+مدخل_Anchor).collect_variation_indices (c);
    (src_پایه+exitAnchor).collect_variation_indices (c);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت بنية CursivePosFormat1_2<Types> *src_پایه) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *بیرون = c->serializer->embed (هذا);
    إذا (unlikely (!بیرون)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= بیرون->مدخل_Anchor.serialize_subset (c, مدخل_Anchor, src_پایه);
    ret |= بیرون->exitAnchor.serialize_subset (c, exitAnchor, src_پایه);
    return_trace (ret);
  }

  محمي:
  نام_نوع Types::قالب جابه_جایی_مقصد<Anchor, بنية CursivePosFormat1_2<Types>>
                مدخل_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  نام_نوع Types::قالب جابه_جایی_مقصد<Anchor, بنية CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عام:
  DEFINE_اندازه_STATIC (2 * Types::اندازه);
};

ساكن مضمن فراغ
معکوس_cursive_minor_جابه_جایی (hb_glyph_position_t *pos,
                              بیعلامت صحيح len,
                              بیعلامت صحيح i,
                              hb_direction_t direction,
                              بیعلامت صحيح new_والد,
                              بیعلامت nesting_level = 0)
{
  إذا (nesting_level > HB_MAX_NESTING_LEVEL)
    أرجع;

  صحيح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  إذا (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    أرجع;

  pos[i].attach_chain() = 0;

  بیعلامت صحيح j = (صحيح) i + chain;

  إذا (unlikely (j >= len))
    أرجع;

  /* Stop if we see new parent in the chain. */
  إذا (j == new_والد)
    أرجع;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  إذا (unlikely (reversed_chain != -chain))
    أرجع;

  معکوس_cursive_minor_جابه_جایی (pos, len, j, direction, new_والد, nesting_level + 1);

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_جابه_جایی_2 = hb_saturate_neg (pos[i].y_جابه_جایی_2);
  وإلا
    pos[j].x_جابه_جایی_2 = hb_saturate_neg (pos[i].x_جابه_جایی_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ساكن مضمن منطقي
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &مدخل_anchor,
                          بیعلامت i)
{
  hb_میانگیر_t *میانگیر = c->میانگیر;
  بیعلامت صحيح j = میانگیر->idx;

  إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
  {
    c->میانگیر->پیام (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  میانگیر->unsafe_مقصد_break (i, j + 1);
  عائم مدخل_x, مدخل_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, میانگیر->info[i].codepoint, &exit_x, &exit_y);
  مدخل_anchor.get_anchor (c, میانگیر->info[j].codepoint, &مدخل_x, &مدخل_y);

  hb_glyph_position_t *pos = میانگیر->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  بدل (c->direction) {
    حالة HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_جمع_کن ((hb_position_t) roundf (exit_x), pos[i].x_جابه_جایی_2);

      d = hb_saturate_جمع_کن ((hb_position_t) roundf (مدخل_x), pos[j].x_جابه_جایی_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_جابه_جایی_2 = hb_saturate_sub (pos[j].x_جابه_جایی_2, d);
      اكسر;
    حالة HB_DIRECTION_RTL:
      d = hb_saturate_جمع_کن ((hb_position_t) roundf (exit_x), pos[i].x_جابه_جایی_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_جابه_جایی_2 = hb_saturate_sub (pos[i].x_جابه_جایی_2, d);

      pos[j].x_advance = hb_saturate_جمع_کن ((hb_position_t) roundf (مدخل_x), pos[j].x_جابه_جایی_2);
      اكسر;
    حالة HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_جمع_کن ((hb_position_t) roundf (exit_y), pos[i].y_جابه_جایی_2);

      d = hb_saturate_جمع_کن ((hb_position_t) roundf (مدخل_y), pos[j].y_جابه_جایی_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_جابه_جایی_2 = hb_saturate_sub (pos[j].y_جابه_جایی_2, d);
      اكسر;
    حالة HB_DIRECTION_BTT:
      d = hb_saturate_جمع_کن ((hb_position_t) roundf (exit_y), pos[i].y_جابه_جایی_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_جابه_جایی_2 = hb_saturate_sub (pos[i].y_جابه_جایی_2, d);

      pos[j].y_advance = (hb_position_t) roundf (مدخل_y);
      اكسر;
    حالة HB_DIRECTION_INVALID:
    افتراضي:
      اكسر;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  بیعلامت صحيح فرزند  = i;
  بیعلامت صحيح والد = j;
  hb_position_t x_جابه_جایی_2 = (hb_position_t) roundf (مدخل_x - exit_x);
  hb_position_t y_جابه_جایی_2 = (hb_position_t) roundf (مدخل_y - exit_y);
  إذا  (!(c->lookup_props & LookupFlag::راست_مقصد_چپ))
  {
    بیعلامت صحيح k = فرزند;
    فرزند = والد;
    والد = k;
    x_جابه_جایی_2 = -x_جابه_جایی_2;
    y_جابه_جایی_2 = -y_جابه_جایی_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  معکوس_cursive_minor_جابه_جایی (pos, میانگیر->len, فرزند, c->direction, والد);

  pos[فرزند].attach_chain() = (صحيح) والد - (صحيح) فرزند;
  إذا (pos[فرزند].attach_chain() != (صحيح) والد - (صحيح) فرزند)
  {
    pos[فرزند].attach_chain() = 0;
    اذهب سرریز;
  }
  pos[فرزند].attach_type() = ATTACH_TYPE_CURSIVE;
  میانگیر->scratch_flags |= HB_میانگیر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[فرزند].y_جابه_جایی_2 = y_جابه_جایی_2;
  وإلا
    pos[فرزند].x_جابه_جایی_2 = x_جابه_جایی_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  إذا (unlikely (pos[والد].attach_chain() == -pos[فرزند].attach_chain()))
  {
    pos[والد].attach_chain() = 0;
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[والد].y_جابه_جایی_2 = 0;
    وإلا
	pos[والد].x_جابه_جایی_2 = 0;
  }

  إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
  {
    c->میانگیر->پیام (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

سرریز:
  میانگیر->idx++;
  أرجع درست;
}

قالب <نام_نوع Types>
بنية CursivePosFormat1_2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  نام_نوع Types::قالب Array_از<مدخل_Exit_رکورد<Types>>
                مدخل_Exit_رکورد_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, مدخل_Exit_رکورد_2);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!coverage.sanitize (c, هذا)))
      return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (مدخل_Exit_رکورد_2.sanitize_shallow (c));
    وإلا
      return_trace (مدخل_Exit_رکورد_2.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, مدخل_Exit_رکورد_2)
    | hb_filter (c->glyph_مجموعه, hb_اولی)
    | hb_map (hb_دومی)
    | hb_apply ([&] (ثابت مدخل_Exit_رکورد<Types>& رکورد) { رکورد.collect_variation_indices (c, هذا); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع; }

  فراغ collect_دومی_glyphs (hb_مجموعه_digest_t *digest) ثابت
  { (هذا+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_میانگیر_t *میانگیر = c->میانگیر;

    ثابت مدخل_Exit_رکورد<Types> &this_رکورد = مدخل_Exit_رکورد_2[(هذا+coverage).get_coverage  (میانگیر->cur().codepoint)];
    إذا (!this_رکورد.مدخل_Anchor ||
	unlikely (!this_رکورد.مدخل_Anchor.sanitize (&c->sanitizer, هذا))) return_trace (خطأ);
    hb_barrier ();

    خودکار &skippy_iter = c->iter_درون_داد;
    skippy_iter.reset_fast (میانگیر->idx);
    بیعلامت unsafe_from;
    إذا (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (unsafe_from, میانگیر->idx + 1);
      return_trace (خطأ);
    }

    ثابت مدخل_Exit_رکورد<Types> &prev_رکورد = مدخل_Exit_رکورد_2[(هذا+coverage).get_coverage  (میانگیر->info[skippy_iter.idx].codepoint)];
    إذا (!prev_رکورد.exitAnchor ||
	unlikely (!prev_رکورد.exitAnchor.sanitize (&c->sanitizer, هذا)))
    {
      میانگیر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, میانگیر->idx + 1);
      return_trace (خطأ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            هذا+prev_رکورد.exitAnchor,
                                            هذا+this_رکورد.مدخل_Anchor,
                                            skippy_iter.idx));
  }

  قالب <نام_نوع Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت بنية CursivePosFormat1_2<Types> *src_پایه)
  {
    إذا (unlikely (!c->serializer->extend_min ((*هذا)))) أرجع;
    هذا->format = src_پایه->format;
    هذا->مدخل_Exit_رکورد_2.len = it.len ();

    لكل (ثابت مدخل_Exit_رکورد<Types>& مدخل_رکورد : + it
                                                      | hb_map (hb_دومی))
      مدخل_رکورد.subset (c, src_پایه);

    خودکار glyphs =
    + it
    | hb_map_retains_ترتیبدهی (hb_اولی)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);

    خودکار it =
    + hb_zip (هذا+coverage, مدخل_Exit_رکورد_2)
    | hb_filter (glyphset, hb_اولی)
    | hb_map_retains_ترتیبدهی ([&] (hb_pair_t<hb_codepoint_t, ثابت مدخل_Exit_رکورد<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت مدخل_Exit_رکورد<Types>&>
                              { أرجع hb_pair (glyph_map[p.اولی], p.second);})
    ;

    منطقي ret = منطقي (it);
    بیرون->serialize (c, it, هذا);
    return_trace (ret);
  }
};


}
}
}

#پایان_اگر /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

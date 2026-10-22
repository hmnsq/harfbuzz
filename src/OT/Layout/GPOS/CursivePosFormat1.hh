#که_نه_تعریف_شوی OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#تعریف_کړه OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ضمن "Anchor.hh"
#ضمن "../../../hb-limits.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

کينډۍ <ډول_نوم Types>
بنية CursivePosFormat1_2;

کينډۍ <ډول_نوم Types>
بنية ثبت_Exit_ریکارډ
{
  کينډۍ <ډول_نوم Types2> ملګری بنية CursivePosFormat1_2;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنية CursivePosFormat1_2<Types> *بنسټ) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (ثبت_Anchor.sanitize (c, بنسټ) && exitAnchor.sanitize (c, بنسټ));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بنية CursivePosFormat1_2<Types> *src_بنسټ) ثابت
  {
    (src_بنسټ+ثبت_Anchor).collect_variation_indices (c);
    (src_بنسټ+exitAnchor).collect_variation_indices (c);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت بنية CursivePosFormat1_2<Types> *src_بنسټ) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری *بهر = c->serializer->embed (هذا);
    إذا (unlikely (!بهر)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= بهر->ثبت_Anchor.serialize_subset (c, ثبت_Anchor, src_بنسټ);
    ret |= بهر->exitAnchor.serialize_subset (c, exitAnchor, src_بنسټ);
    return_trace (ret);
  }

  محمي:
  ډول_نوم Types::کينډۍ دننه_واټن_منزل<Anchor, بنية CursivePosFormat1_2<Types>>
                ثبت_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  ډول_نوم Types::کينډۍ دننه_واټن_منزل<Anchor, بنية CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عام:
  DEFINE_اندازه_STATIC (2 * Types::اندازه);
};

ساكن مضمن فراغ
سرچپه_cursive_minor_دننه_واټن (hb_glyph_position_t *pos,
                              بینښه صحيح len,
                              بینښه صحيح i,
                              hb_direction_t direction,
                              بینښه صحيح new_پلار,
                              بینښه nesting_level = 0)
{
  إذا (nesting_level > HB_MAX_NESTING_LEVEL)
    أرجع;

  صحيح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  إذا (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    أرجع;

  pos[i].attach_chain() = 0;

  بینښه صحيح j = (صحيح) i + chain;

  إذا (unlikely (j >= len))
    أرجع;

  /* Stop if we see new parent in the chain. */
  إذا (j == new_پلار)
    أرجع;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  إذا (unlikely (reversed_chain != -chain))
    أرجع;

  سرچپه_cursive_minor_دننه_واټن (pos, len, j, direction, new_پلار, nesting_level + 1);

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_دننه_واټن_2 = hb_saturate_neg (pos[i].y_دننه_واټن_2);
  وإلا
    pos[j].x_دننه_واټن_2 = hb_saturate_neg (pos[i].x_دننه_واټن_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ساكن مضمن منطقي
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &ثبت_anchor,
                          بینښه i)
{
  hb_بفر_t *بفر = c->بفر;
  بینښه صحيح j = بفر->idx;

  إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  بفر->unsafe_منزل_break (i, j + 1);
  عائم ثبت_x, ثبت_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, بفر->info[i].codepoint, &exit_x, &exit_y);
  ثبت_anchor.get_anchor (c, بفر->info[j].codepoint, &ثبت_x, &ثبت_y);

  hb_glyph_position_t *pos = بفر->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  بدل (c->direction) {
    حالة HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_جمع_کړه ((hb_position_t) roundf (exit_x), pos[i].x_دننه_واټن_2);

      d = hb_saturate_جمع_کړه ((hb_position_t) roundf (ثبت_x), pos[j].x_دننه_واټن_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_دننه_واټن_2 = hb_saturate_sub (pos[j].x_دننه_واټن_2, d);
      اكسر;
    حالة HB_DIRECTION_RTL:
      d = hb_saturate_جمع_کړه ((hb_position_t) roundf (exit_x), pos[i].x_دننه_واټن_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_دننه_واټن_2 = hb_saturate_sub (pos[i].x_دننه_واټن_2, d);

      pos[j].x_advance = hb_saturate_جمع_کړه ((hb_position_t) roundf (ثبت_x), pos[j].x_دننه_واټن_2);
      اكسر;
    حالة HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_جمع_کړه ((hb_position_t) roundf (exit_y), pos[i].y_دننه_واټن_2);

      d = hb_saturate_جمع_کړه ((hb_position_t) roundf (ثبت_y), pos[j].y_دننه_واټن_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_دننه_واټن_2 = hb_saturate_sub (pos[j].y_دننه_واټن_2, d);
      اكسر;
    حالة HB_DIRECTION_BTT:
      d = hb_saturate_جمع_کړه ((hb_position_t) roundf (exit_y), pos[i].y_دننه_واټن_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_دننه_واټن_2 = hb_saturate_sub (pos[i].y_دننه_واټن_2, d);

      pos[j].y_advance = (hb_position_t) roundf (ثبت_y);
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
  بینښه صحيح ماشوم  = i;
  بینښه صحيح پلار = j;
  hb_position_t x_دننه_واټن_2 = (hb_position_t) roundf (ثبت_x - exit_x);
  hb_position_t y_دننه_واټن_2 = (hb_position_t) roundf (ثبت_y - exit_y);
  إذا  (!(c->lookup_props & LookupFlag::ښی_منزل_کیڼ))
  {
    بینښه صحيح k = ماشوم;
    ماشوم = پلار;
    پلار = k;
    x_دننه_واټن_2 = -x_دننه_واټن_2;
    y_دننه_واټن_2 = -y_دننه_واټن_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  سرچپه_cursive_minor_دننه_واټن (pos, بفر->len, ماشوم, c->direction, پلار);

  pos[ماشوم].attach_chain() = (صحيح) پلار - (صحيح) ماشوم;
  إذا (pos[ماشوم].attach_chain() != (صحيح) پلار - (صحيح) ماشوم)
  {
    pos[ماشوم].attach_chain() = 0;
    اذهب ډکېدنه;
  }
  pos[ماشوم].attach_type() = ATTACH_TYPE_CURSIVE;
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[ماشوم].y_دننه_واټن_2 = y_دننه_واټن_2;
  وإلا
    pos[ماشوم].x_دننه_واټن_2 = x_دننه_واټن_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  إذا (unlikely (pos[پلار].attach_chain() == -pos[ماشوم].attach_chain()))
  {
    pos[پلار].attach_chain() = 0;
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[پلار].y_دننه_واټن_2 = 0;
    وإلا
	pos[پلار].x_دننه_واټن_2 = 0;
  }

  إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

ډکېدنه:
  بفر->idx++;
  أرجع ریښتیا;
}

کينډۍ <ډول_نوم Types>
بنية CursivePosFormat1_2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  ډول_نوم Types::کينډۍ Array_د<ثبت_Exit_ریکارډ<Types>>
                ثبت_Exit_ریکارډ_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, ثبت_Exit_ریکارډ_2);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!coverage.sanitize (c, هذا)))
      return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (ثبت_Exit_ریکارډ_2.sanitize_shallow (c));
    وإلا
      return_trace (ثبت_Exit_ریکارډ_2.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, ثبت_Exit_ریکارډ_2)
    | hb_filter (c->glyph_سېټ, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_apply ([&] (ثابت ثبت_Exit_ریکارډ<Types>& ریکارډ) { ریکارډ.collect_variation_indices (c, هذا); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع; }

  فراغ collect_دوهم_glyphs (hb_سېټ_digest_t *digest) ثابت
  { (هذا+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;

    ثابت ثبت_Exit_ریکارډ<Types> &this_ریکارډ = ثبت_Exit_ریکارډ_2[(هذا+coverage).get_coverage  (بفر->cur().codepoint)];
    إذا (!this_ریکارډ.ثبت_Anchor ||
	unlikely (!this_ریکارډ.ثبت_Anchor.sanitize (&c->sanitizer, هذا))) return_trace (خطأ);
    hb_barrier ();

    خپلکاری &skippy_iter = c->iter_ننوت;
    skippy_iter.reset_fast (بفر->idx);
    بینښه unsafe_from;
    إذا (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (unsafe_from, بفر->idx + 1);
      return_trace (خطأ);
    }

    ثابت ثبت_Exit_ریکارډ<Types> &prev_ریکارډ = ثبت_Exit_ریکارډ_2[(هذا+coverage).get_coverage  (بفر->info[skippy_iter.idx].codepoint)];
    إذا (!prev_ریکارډ.exitAnchor ||
	unlikely (!prev_ریکارډ.exitAnchor.sanitize (&c->sanitizer, هذا)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            هذا+prev_ریکارډ.exitAnchor,
                                            هذا+this_ریکارډ.ثبت_Anchor,
                                            skippy_iter.idx));
  }

  کينډۍ <ډول_نوم Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت بنية CursivePosFormat1_2<Types> *src_بنسټ)
  {
    إذا (unlikely (!c->serializer->extend_min ((*هذا)))) أرجع;
    هذا->format = src_بنسټ->format;
    هذا->ثبت_Exit_ریکارډ_2.len = it.len ();

    لكل (ثابت ثبت_Exit_ریکارډ<Types>& ثبت_ریکارډ : + it
                                                      | hb_map (hb_دوهم))
      ثبت_ریکارډ.subset (c, src_بنسټ);

    خپلکاری glyphs =
    + it
    | hb_map_retains_ترتیبونه (hb_لومړی)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);

    خپلکاری it =
    + hb_zip (هذا+coverage, ثبت_Exit_ریکارډ_2)
    | hb_filter (glyphset, hb_لومړی)
    | hb_map_retains_ترتیبونه ([&] (hb_pair_t<hb_codepoint_t, ثابت ثبت_Exit_ریکارډ<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت ثبت_Exit_ریکارډ<Types>&>
                              { أرجع hb_pair (glyph_map[p.لومړی], p.second);})
    ;

    منطقي ret = منطقي (it);
    بهر->serialize (c, it, هذا);
    return_trace (ret);
  }
};


}
}
}

#د_که_پای /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

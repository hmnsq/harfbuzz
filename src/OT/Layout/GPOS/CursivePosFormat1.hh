#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#شامل "Anchor.hh"
#شامل "../../../hb-limits.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

سانچہ <قسم_نام Types>
ساخت CursivePosFormat1_2;

سانچہ <قسم_نام Types>
ساخت اندراج_Exit_ریکارڈ
{
  سانچہ <قسم_نام Types2> دوست ساخت CursivePosFormat1_2;

  منطقی sanitize (hb_sanitize_context_t *c, ثابت ساخت CursivePosFormat1_2<Types> *بنیاد) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (اندراج_Anchor.sanitize (c, بنیاد) && exitAnchor.sanitize (c, بنیاد));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ساخت CursivePosFormat1_2<Types> *src_بنیاد) ثابت
  {
    (src_بنیاد+اندراج_Anchor).collect_variation_indices (c);
    (src_بنیاد+exitAnchor).collect_variation_indices (c);
  }

  منطقی subset (hb_subset_context_t *c,
	       ثابت ساخت CursivePosFormat1_2<Types> *src_بنیاد) ثابت
  {
    TRACE_SERIALIZE (یہ);
    خودکار *برآمد = c->serializer->embed (یہ);
    اگر (unlikely (!برآمد)) return_trace (جھوٹ);

    منطقی ret = جھوٹ;
    ret |= برآمد->اندراج_Anchor.serialize_subset (c, اندراج_Anchor, src_بنیاد);
    ret |= برآمد->exitAnchor.serialize_subset (c, exitAnchor, src_بنیاد);
    return_trace (ret);
  }

  محفوظ:
  قسم_نام Types::سانچہ انحراف_مقصد<Anchor, ساخت CursivePosFormat1_2<Types>>
                اندراج_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  قسم_نام Types::سانچہ انحراف_مقصد<Anchor, ساخت CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عوامی:
  DEFINE_سائز_STATIC (2 * Types::سائز);
};

ساکن اندرونی خالی
الٹ_cursive_minor_انحراف (hb_glyph_position_t *pos,
                              غیرعلامتی صحیح len,
                              غیرعلامتی صحیح i,
                              hb_direction_t direction,
                              غیرعلامتی صحیح new_والد,
                              غیرعلامتی nesting_level = 0)
{
  اگر (nesting_level > HB_MAX_NESTING_LEVEL)
    واپس;

  صحیح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  اگر (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    واپس;

  pos[i].attach_chain() = 0;

  غیرعلامتی صحیح j = (صحیح) i + chain;

  اگر (unlikely (j >= len))
    واپس;

  /* Stop if we see new parent in the chain. */
  اگر (j == new_والد)
    واپس;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  اگر (unlikely (reversed_chain != -chain))
    واپس;

  الٹ_cursive_minor_انحراف (pos, len, j, direction, new_والد, nesting_level + 1);

  اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_انحراف_2 = hb_saturate_neg (pos[i].y_انحراف_2);
  ورنہ
    pos[j].x_انحراف_2 = hb_saturate_neg (pos[i].x_انحراف_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ساکن اندرونی منطقی
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &اندراج_anchor,
                          غیرعلامتی i)
{
  hb_بفر_t *بفر = c->بفر;
  غیرعلامتی صحیح j = بفر->idx;

  اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  بفر->unsafe_مقصد_break (i, j + 1);
  اعشاری اندراج_x, اندراج_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, بفر->info[i].codepoint, &exit_x, &exit_y);
  اندراج_anchor.get_anchor (c, بفر->info[j].codepoint, &اندراج_x, &اندراج_y);

  hb_glyph_position_t *pos = بفر->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  انتخاب (c->direction) {
    حالت HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_جوڑو ((hb_position_t) roundf (exit_x), pos[i].x_انحراف_2);

      d = hb_saturate_جوڑو ((hb_position_t) roundf (اندراج_x), pos[j].x_انحراف_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_انحراف_2 = hb_saturate_sub (pos[j].x_انحراف_2, d);
      توڑو;
    حالت HB_DIRECTION_RTL:
      d = hb_saturate_جوڑو ((hb_position_t) roundf (exit_x), pos[i].x_انحراف_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_انحراف_2 = hb_saturate_sub (pos[i].x_انحراف_2, d);

      pos[j].x_advance = hb_saturate_جوڑو ((hb_position_t) roundf (اندراج_x), pos[j].x_انحراف_2);
      توڑو;
    حالت HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_جوڑو ((hb_position_t) roundf (exit_y), pos[i].y_انحراف_2);

      d = hb_saturate_جوڑو ((hb_position_t) roundf (اندراج_y), pos[j].y_انحراف_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_انحراف_2 = hb_saturate_sub (pos[j].y_انحراف_2, d);
      توڑو;
    حالت HB_DIRECTION_BTT:
      d = hb_saturate_جوڑو ((hb_position_t) roundf (exit_y), pos[i].y_انحراف_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_انحراف_2 = hb_saturate_sub (pos[i].y_انحراف_2, d);

      pos[j].y_advance = (hb_position_t) roundf (اندراج_y);
      توڑو;
    حالت HB_DIRECTION_INVALID:
    طےشدہ:
      توڑو;
  }

  /* Cross-direction adjustment */

  /* We attach child to parent (think graph theory and rooted trees whereas
   * the root stays on baseline and each node aligns itself against its
   * parent.
   *
   * Optimize things for the case of RightToLeft, as that's most common in
   * Arabic. */
  غیرعلامتی صحیح بچہ  = i;
  غیرعلامتی صحیح والد = j;
  hb_position_t x_انحراف_2 = (hb_position_t) roundf (اندراج_x - exit_x);
  hb_position_t y_انحراف_2 = (hb_position_t) roundf (اندراج_y - exit_y);
  اگر  (!(c->lookup_props & LookupFlag::دایاں_مقصد_بایاں))
  {
    غیرعلامتی صحیح k = بچہ;
    بچہ = والد;
    والد = k;
    x_انحراف_2 = -x_انحراف_2;
    y_انحراف_2 = -y_انحراف_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  الٹ_cursive_minor_انحراف (pos, بفر->len, بچہ, c->direction, والد);

  pos[بچہ].attach_chain() = (صحیح) والد - (صحیح) بچہ;
  اگر (pos[بچہ].attach_chain() != (صحیح) والد - (صحیح) بچہ)
  {
    pos[بچہ].attach_chain() = 0;
    جاؤ حد_سے_تجاوز;
  }
  pos[بچہ].attach_type() = ATTACH_TYPE_CURSIVE;
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  اگر (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[بچہ].y_انحراف_2 = y_انحراف_2;
  ورنہ
    pos[بچہ].x_انحراف_2 = x_انحراف_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  اگر (unlikely (pos[والد].attach_chain() == -pos[بچہ].attach_chain()))
  {
    pos[والد].attach_chain() = 0;
    اگر (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[والد].y_انحراف_2 = 0;
    ورنہ
	pos[والد].x_انحراف_2 = 0;
  }

  اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
  {
    c->بفر->پیغام (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

حد_سے_تجاوز:
  بفر->idx++;
  واپس سچ;
}

سانچہ <قسم_نام Types>
ساخت CursivePosFormat1_2
{

  محفوظ:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نام Types::سانچہ LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  قسم_نام Types::سانچہ Array_کا<اندراج_Exit_ریکارڈ<Types>>
                اندراج_Exit_ریکارڈ_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عوامی:
  DEFINE_سائز_ARRAY (2 + Types::LOffset::static_سائز + Types::HBUINT::static_سائز, اندراج_Exit_ریکارڈ_2);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    اگر (unlikely (!coverage.sanitize (c, یہ)))
      return_trace (جھوٹ);

    اگر (c->lazy_some_gpos)
      return_trace (اندراج_Exit_ریکارڈ_2.sanitize_shallow (c));
    ورنہ
      return_trace (اندراج_Exit_ریکارڈ_2.sanitize (c, یہ));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس (یہ+coverage).intersects (glyphs); }

  خالی احاطہ_lookups (hb_احاطہ_lookups_context_t *c) ثابت {}

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (یہ+coverage, اندراج_Exit_ریکارڈ_2)
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوسرا)
    | hb_apply ([&] (ثابت اندراج_Exit_ریکارڈ<Types>& ریکارڈ) { ریکارڈ.collect_variation_indices (c, یہ); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { اگر (unlikely (!(یہ+coverage).collect_coverage (c->داخلہ))) واپس; }

  خالی collect_دوسرا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (یہ+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { واپس یہ+coverage; }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    hb_بفر_t *بفر = c->بفر;

    ثابت اندراج_Exit_ریکارڈ<Types> &this_ریکارڈ = اندراج_Exit_ریکارڈ_2[(یہ+coverage).get_coverage  (بفر->cur().codepoint)];
    اگر (!this_ریکارڈ.اندراج_Anchor ||
	unlikely (!this_ریکارڈ.اندراج_Anchor.sanitize (&c->sanitizer, یہ))) return_trace (جھوٹ);
    hb_barrier ();

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    غیرعلامتی unsafe_from;
    اگر (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (unsafe_from, بفر->idx + 1);
      return_trace (جھوٹ);
    }

    ثابت اندراج_Exit_ریکارڈ<Types> &پچھلا_ریکارڈ = اندراج_Exit_ریکارڈ_2[(یہ+coverage).get_coverage  (بفر->info[skippy_iter.idx].codepoint)];
    اگر (!پچھلا_ریکارڈ.exitAnchor ||
	unlikely (!پچھلا_ریکارڈ.exitAnchor.sanitize (&c->sanitizer, یہ)))
    {
      بفر->unsafe_مقصد_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (جھوٹ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            یہ+پچھلا_ریکارڈ.exitAnchor,
                                            یہ+this_ریکارڈ.اندراج_Anchor,
                                            skippy_iter.idx));
  }

  سانچہ <قسم_نام Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  خالی serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت ساخت CursivePosFormat1_2<Types> *src_بنیاد)
  {
    اگر (unlikely (!c->serializer->extend_min ((*یہ)))) واپس;
    یہ->format = src_بنیاد->format;
    یہ->اندراج_Exit_ریکارڈ_2.len = it.len ();

    برائے (ثابت اندراج_Exit_ریکارڈ<Types>& اندراج_ریکارڈ : + it
                                                      | hb_map (hb_دوسرا))
      اندراج_ریکارڈ.subset (c, src_بنیاد);

    خودکار glyphs =
    + it
    | hb_map_retains_ترتیب_کاری (hb_پہلا)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);

    خودکار it =
    + hb_zip (یہ+coverage, اندراج_Exit_ریکارڈ_2)
    | hb_filter (glyphset, hb_پہلا)
    | hb_map_retains_ترتیب_کاری ([&] (hb_pair_t<hb_codepoint_t, ثابت اندراج_Exit_ریکارڈ<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت اندراج_Exit_ریکارڈ<Types>&>
                              { واپس hb_pair (glyph_map[p.پہلا], p.second);})
    ;

    منطقی ret = منطقی (it);
    برآمد->serialize (c, it, یہ);
    return_trace (ret);
  }
};


}
}
}

#اگر_ختم /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

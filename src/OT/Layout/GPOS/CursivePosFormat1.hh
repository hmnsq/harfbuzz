#جے_بنا_تعریف OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#تعریف_کرو OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ضمن "Anchor.hh"
#ضمن "../../../hb-limits.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچا <قسم_ناں Types>
بنية CursivePosFormat1_2;

سانچا <قسم_ناں Types>
بنية اندراج_Exit_ریکارڈ
{
  سانچا <قسم_ناں Types2> یار بنية CursivePosFormat1_2;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنية CursivePosFormat1_2<Types> *ادھار) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (اندراج_Anchor.sanitize (c, ادھار) && exitAnchor.sanitize (c, ادھار));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بنية CursivePosFormat1_2<Types> *src_ادھار) ثابت
  {
    (src_ادھار+اندراج_Anchor).collect_variation_indices (c);
    (src_ادھار+exitAnchor).collect_variation_indices (c);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت بنية CursivePosFormat1_2<Types> *src_ادھار) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودکار *باہرلا = c->serializer->embed (هذا);
    إذا (unlikely (!باہرلا)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= باہرلا->اندراج_Anchor.serialize_subset (c, اندراج_Anchor, src_ادھار);
    ret |= باہرلا->exitAnchor.serialize_subset (c, exitAnchor, src_ادھار);
    return_trace (ret);
  }

  محمي:
  قسم_ناں Types::سانچا وتھ_منزل<Anchor, بنية CursivePosFormat1_2<Types>>
                اندراج_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  قسم_ناں Types::سانچا وتھ_منزل<Anchor, بنية CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عام:
  DEFINE_ناپ_STATIC (2 * Types::ناپ);
};

ساكن مضمن فراغ
الٹ_cursive_minor_وتھ (hb_glyph_position_t *pos,
                              بنانشانی صحيح len,
                              بنانشانی صحيح i,
                              hb_direction_t direction,
                              بنانشانی صحيح new_ماپا,
                              بنانشانی nesting_level = 0)
{
  إذا (nesting_level > HB_MAX_NESTING_LEVEL)
    أرجع;

  صحيح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  إذا (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    أرجع;

  pos[i].attach_chain() = 0;

  بنانشانی صحيح j = (صحيح) i + chain;

  إذا (unlikely (j >= len))
    أرجع;

  /* Stop if we see new parent in the chain. */
  إذا (j == new_ماپا)
    أرجع;

  int16_t reversed_chain = -chain;
  /* The old edge was cleared above; if the reversed distance truncates,
   * keep it detached instead of storing a poisoned chain.
   */
  إذا (unlikely (reversed_chain != -chain))
    أرجع;

  الٹ_cursive_minor_وتھ (pos, len, j, direction, new_ماپا, nesting_level + 1);

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_وتھ_2 = hb_saturate_neg (pos[i].y_وتھ_2);
  وإلا
    pos[j].x_وتھ_2 = hb_saturate_neg (pos[i].x_وتھ_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ساكن مضمن منطقي
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &اندراج_anchor,
                          بنانشانی i)
{
  hb_بفر_t *بفر = c->بفر;
  بنانشانی صحيح j = بفر->idx;

  إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
  {
    c->بفر->سنیہا (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  بفر->unsafe_منزل_break (i, j + 1);
  عائم اندراج_x, اندراج_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, بفر->info[i].codepoint, &exit_x, &exit_y);
  اندراج_anchor.get_anchor (c, بفر->info[j].codepoint, &اندراج_x, &اندراج_y);

  hb_glyph_position_t *pos = بفر->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  بدل (c->direction) {
    حالة HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_جوڑو ((hb_position_t) roundf (exit_x), pos[i].x_وتھ_2);

      d = hb_saturate_جوڑو ((hb_position_t) roundf (اندراج_x), pos[j].x_وتھ_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_وتھ_2 = hb_saturate_sub (pos[j].x_وتھ_2, d);
      اكسر;
    حالة HB_DIRECTION_RTL:
      d = hb_saturate_جوڑو ((hb_position_t) roundf (exit_x), pos[i].x_وتھ_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_وتھ_2 = hb_saturate_sub (pos[i].x_وتھ_2, d);

      pos[j].x_advance = hb_saturate_جوڑو ((hb_position_t) roundf (اندراج_x), pos[j].x_وتھ_2);
      اكسر;
    حالة HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_جوڑو ((hb_position_t) roundf (exit_y), pos[i].y_وتھ_2);

      d = hb_saturate_جوڑو ((hb_position_t) roundf (اندراج_y), pos[j].y_وتھ_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_وتھ_2 = hb_saturate_sub (pos[j].y_وتھ_2, d);
      اكسر;
    حالة HB_DIRECTION_BTT:
      d = hb_saturate_جوڑو ((hb_position_t) roundf (exit_y), pos[i].y_وتھ_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_وتھ_2 = hb_saturate_sub (pos[i].y_وتھ_2, d);

      pos[j].y_advance = (hb_position_t) roundf (اندراج_y);
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
  بنانشانی صحيح بچہ  = i;
  بنانشانی صحيح ماپا = j;
  hb_position_t x_وتھ_2 = (hb_position_t) roundf (اندراج_x - exit_x);
  hb_position_t y_وتھ_2 = (hb_position_t) roundf (اندراج_y - exit_y);
  إذا  (!(c->lookup_props & LookupFlag::دایاں_منزل_بایاں))
  {
    بنانشانی صحيح k = بچہ;
    بچہ = ماپا;
    ماپا = k;
    x_وتھ_2 = -x_وتھ_2;
    y_وتھ_2 = -y_وتھ_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  الٹ_cursive_minor_وتھ (pos, بفر->len, بچہ, c->direction, ماپا);

  pos[بچہ].attach_chain() = (صحيح) ماپا - (صحيح) بچہ;
  إذا (pos[بچہ].attach_chain() != (صحيح) ماپا - (صحيح) بچہ)
  {
    pos[بچہ].attach_chain() = 0;
    اذهب حد_توں_ودھ;
  }
  pos[بچہ].attach_type() = ATTACH_TYPE_CURSIVE;
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[بچہ].y_وتھ_2 = y_وتھ_2;
  وإلا
    pos[بچہ].x_وتھ_2 = x_وتھ_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  إذا (unlikely (pos[ماپا].attach_chain() == -pos[بچہ].attach_chain()))
  {
    pos[ماپا].attach_chain() = 0;
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[ماپا].y_وتھ_2 = 0;
    وإلا
	pos[ماپا].x_وتھ_2 = 0;
  }

  إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
  {
    c->بفر->سنیہا (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

حد_توں_ودھ:
  بفر->idx++;
  أرجع سچ;
}

سانچا <قسم_ناں Types>
بنية CursivePosFormat1_2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  قسم_ناں Types::سانچا Array_دا<اندراج_Exit_ریکارڈ<Types>>
                اندراج_Exit_ریکارڈ_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, اندراج_Exit_ریکارڈ_2);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!coverage.sanitize (c, هذا)))
      return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (اندراج_Exit_ریکارڈ_2.sanitize_shallow (c));
    وإلا
      return_trace (اندراج_Exit_ریکارڈ_2.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, اندراج_Exit_ریکارڈ_2)
    | hb_filter (c->glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_apply ([&] (ثابت اندراج_Exit_ریکارڈ<Types>& ریکارڈ) { ریکارڈ.collect_variation_indices (c, هذا); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع; }

  فراغ collect_دوجا_glyphs (hb_سیٹ_digest_t *digest) ثابت
  { (هذا+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;

    ثابت اندراج_Exit_ریکارڈ<Types> &this_ریکارڈ = اندراج_Exit_ریکارڈ_2[(هذا+coverage).get_coverage  (بفر->cur().codepoint)];
    إذا (!this_ریکارڈ.اندراج_Anchor ||
	unlikely (!this_ریکارڈ.اندراج_Anchor.sanitize (&c->sanitizer, هذا))) return_trace (خطأ);
    hb_barrier ();

    خودکار &skippy_iter = c->iter_داخلہ;
    skippy_iter.reset_fast (بفر->idx);
    بنانشانی unsafe_from;
    إذا (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (unsafe_from, بفر->idx + 1);
      return_trace (خطأ);
    }

    ثابت اندراج_Exit_ریکارڈ<Types> &prev_ریکارڈ = اندراج_Exit_ریکارڈ_2[(هذا+coverage).get_coverage  (بفر->info[skippy_iter.idx].codepoint)];
    إذا (!prev_ریکارڈ.exitAnchor ||
	unlikely (!prev_ریکارڈ.exitAnchor.sanitize (&c->sanitizer, هذا)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            هذا+prev_ریکارڈ.exitAnchor,
                                            هذا+this_ریکارڈ.اندراج_Anchor,
                                            skippy_iter.idx));
  }

  سانچا <قسم_ناں Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت بنية CursivePosFormat1_2<Types> *src_ادھار)
  {
    إذا (unlikely (!c->serializer->extend_min ((*هذا)))) أرجع;
    هذا->format = src_ادھار->format;
    هذا->اندراج_Exit_ریکارڈ_2.len = it.len ();

    لكل (ثابت اندراج_Exit_ریکارڈ<Types>& اندراج_ریکارڈ : + it
                                                      | hb_map (hb_دوجا))
      اندراج_ریکارڈ.subset (c, src_ادھار);

    خودکار glyphs =
    + it
    | hb_map_retains_چھانٹی (hb_پہلا)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);

    خودکار it =
    + hb_zip (هذا+coverage, اندراج_Exit_ریکارڈ_2)
    | hb_filter (glyphset, hb_پہلا)
    | hb_map_retains_چھانٹی ([&] (hb_pair_t<hb_codepoint_t, ثابت اندراج_Exit_ریکارڈ<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت اندراج_Exit_ریکارڈ<Types>&>
                              { أرجع hb_pair (glyph_map[p.پہلا], p.second);})
    ;

    منطقي ret = منطقي (it);
    باہرلا->serialize (c, it, هذا);
    return_trace (ret);
  }
};


}
}
}

#جے_انت /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

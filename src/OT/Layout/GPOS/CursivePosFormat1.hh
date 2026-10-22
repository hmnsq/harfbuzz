#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH
#وصف_ڪريو OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH

#ضمن "Anchor.hh"
#ضمن "../../../hb-limits.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

سانچو <قسم_نالو Types>
بنية CursivePosFormat1_2;

سانچو <قسم_نالو Types>
بنية داخلا_Exit_رڪارڊ
{
  سانچو <قسم_نالو Types2> دوست بنية CursivePosFormat1_2;

  منطقي sanitize (hb_sanitize_context_t *c, ثابت بنية CursivePosFormat1_2<Types> *بنياد) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (داخلا_Anchor.sanitize (c, بنياد) && exitAnchor.sanitize (c, بنياد));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بنية CursivePosFormat1_2<Types> *src_بنياد) ثابت
  {
    (src_بنياد+داخلا_Anchor).collect_variation_indices (c);
    (src_بنياد+exitAnchor).collect_variation_indices (c);
  }

  منطقي subset (hb_subset_context_t *c,
	       ثابت بنية CursivePosFormat1_2<Types> *src_بنياد) ثابت
  {
    TRACE_SERIALIZE (هذا);
    خودڪار *ٻاهريون = c->serializer->embed (هذا);
    إذا (unlikely (!ٻاهريون)) return_trace (خطأ);

    منطقي ret = خطأ;
    ret |= ٻاهريون->داخلا_Anchor.serialize_subset (c, داخلا_Anchor, src_بنياد);
    ret |= ٻاهريون->exitAnchor.serialize_subset (c, exitAnchor, src_بنياد);
    return_trace (ret);
  }

  محمي:
  قسم_نالو Types::سانچو وٿي_منزل<Anchor, بنية CursivePosFormat1_2<Types>>
                داخلا_Anchor;            /* Offset to EntryAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  قسم_نالو Types::سانچو وٿي_منزل<Anchor, بنية CursivePosFormat1_2<Types>>
                exitAnchor;             /* Offset to ExitAnchor table--from
                                         * beginning of CursivePos
                                         * subtable--may be NULL */
  عام:
  DEFINE_سائيز_STATIC (2 * Types::سائيز);
};

ساكن مضمن فراغ
ابتو_cursive_minor_وٿي (hb_glyph_position_t *pos,
                              بنانشان صحيح len,
                              بنانشان صحيح i,
                              hb_direction_t direction,
                              بنانشان صحيح new_والد,
                              بنانشان nesting_level = 0)
{
  إذا (nesting_level > HB_MAX_NESTING_LEVEL)
    أرجع;

  صحيح chain = pos[i].attach_chain(), type = pos[i].attach_type();
  إذا (likely (!chain || 0 == (type & ATTACH_TYPE_CURSIVE)))
    أرجع;

  pos[i].attach_chain() = 0;

  بنانشان صحيح j = (صحيح) i + chain;

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

  ابتو_cursive_minor_وٿي (pos, len, j, direction, new_والد, nesting_level + 1);

  إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
    pos[j].y_وٿي_2 = hb_saturate_neg (pos[i].y_وٿي_2);
  وإلا
    pos[j].x_وٿي_2 = hb_saturate_neg (pos[i].x_وٿي_2);

  pos[j].attach_chain() = reversed_chain;
  pos[j].attach_type() = type;
}


ساكن مضمن منطقي
apply_cursive_attachment (hb_ot_apply_context_t *c,
                          ثابت Anchor &exit_anchor,
                          ثابت Anchor &داخلا_anchor,
                          بنانشان i)
{
  hb_بفر_t *بفر = c->بفر;
  بنانشان صحيح j = بفر->idx;

  إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
  {
    c->بفر->پيغام (c->font,
			  "cursive attaching glyph at %u to glyph at %u",
			  i, j);
  }

  بفر->unsafe_منزل_break (i, j + 1);
  عائم داخلا_x, داخلا_y, exit_x, exit_y;
  exit_anchor.get_anchor (c, بفر->info[i].codepoint, &exit_x, &exit_y);
  داخلا_anchor.get_anchor (c, بفر->info[j].codepoint, &داخلا_x, &داخلا_y);

  hb_glyph_position_t *pos = بفر->pos;

  hb_position_t d;
  /* Main-direction adjustment */
  بدل (c->direction) {
    حالة HB_DIRECTION_LTR:
      pos[i].x_advance = hb_saturate_جوڙيو ((hb_position_t) roundf (exit_x), pos[i].x_وٿي_2);

      d = hb_saturate_جوڙيو ((hb_position_t) roundf (داخلا_x), pos[j].x_وٿي_2);
      pos[j].x_advance = hb_saturate_sub (pos[j].x_advance, d);
      pos[j].x_وٿي_2 = hb_saturate_sub (pos[j].x_وٿي_2, d);
      اكسر;
    حالة HB_DIRECTION_RTL:
      d = hb_saturate_جوڙيو ((hb_position_t) roundf (exit_x), pos[i].x_وٿي_2);
      pos[i].x_advance = hb_saturate_sub (pos[i].x_advance, d);
      pos[i].x_وٿي_2 = hb_saturate_sub (pos[i].x_وٿي_2, d);

      pos[j].x_advance = hb_saturate_جوڙيو ((hb_position_t) roundf (داخلا_x), pos[j].x_وٿي_2);
      اكسر;
    حالة HB_DIRECTION_TTB:
      pos[i].y_advance = hb_saturate_جوڙيو ((hb_position_t) roundf (exit_y), pos[i].y_وٿي_2);

      d = hb_saturate_جوڙيو ((hb_position_t) roundf (داخلا_y), pos[j].y_وٿي_2);
      pos[j].y_advance = hb_saturate_sub (pos[j].y_advance, d);
      pos[j].y_وٿي_2 = hb_saturate_sub (pos[j].y_وٿي_2, d);
      اكسر;
    حالة HB_DIRECTION_BTT:
      d = hb_saturate_جوڙيو ((hb_position_t) roundf (exit_y), pos[i].y_وٿي_2);
      pos[i].y_advance = hb_saturate_sub (pos[i].y_advance, d);
      pos[i].y_وٿي_2 = hb_saturate_sub (pos[i].y_وٿي_2, d);

      pos[j].y_advance = (hb_position_t) roundf (داخلا_y);
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
  بنانشان صحيح ٻار  = i;
  بنانشان صحيح والد = j;
  hb_position_t x_وٿي_2 = (hb_position_t) roundf (داخلا_x - exit_x);
  hb_position_t y_وٿي_2 = (hb_position_t) roundf (داخلا_y - exit_y);
  إذا  (!(c->lookup_props & LookupFlag::ساڄو_منزل_کاٻو))
  {
    بنانشان صحيح k = ٻار;
    ٻار = والد;
    والد = k;
    x_وٿي_2 = -x_وٿي_2;
    y_وٿي_2 = -y_وٿي_2;
  }

  /* If child was already connected to someone else, walk through its old
   * chain and reverse the link direction, such that the whole tree of its
   * previous connection now attaches to new parent.  Watch out for case
   * where new parent is on the path from old chain...
   */
  ابتو_cursive_minor_وٿي (pos, بفر->len, ٻار, c->direction, والد);

  pos[ٻار].attach_chain() = (صحيح) والد - (صحيح) ٻار;
  إذا (pos[ٻار].attach_chain() != (صحيح) والد - (صحيح) ٻار)
  {
    pos[ٻار].attach_chain() = 0;
    اذهب حد_کان_مٿي;
  }
  pos[ٻار].attach_type() = ATTACH_TYPE_CURSIVE;
  بفر->scratch_flags |= HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT;
  إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
    pos[ٻار].y_وٿي_2 = y_وٿي_2;
  وإلا
    pos[ٻار].x_وٿي_2 = x_وٿي_2;

  /* If parent was attached to child, separate them.
   * https://github.com/harfbuzz/harfbuzz/issues/2469
   */
  إذا (unlikely (pos[والد].attach_chain() == -pos[ٻار].attach_chain()))
  {
    pos[والد].attach_chain() = 0;
    إذا (likely (HB_DIRECTION_IS_HORIZONTAL (c->direction)))
	pos[والد].y_وٿي_2 = 0;
    وإلا
	pos[والد].x_وٿي_2 = 0;
  }

  إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
  {
    c->بفر->پيغام (c->font,
			  "cursive attached glyph at %u to glyph at %u",
			  i, j);
  }

حد_کان_مٿي:
  بفر->idx++;
  أرجع سچ;
}

سانچو <قسم_نالو Types>
بنية CursivePosFormat1_2
{

  محمي:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from beginning of subtable */
  قسم_نالو Types::سانچو Array_جو<داخلا_Exit_رڪارڊ<Types>>
                داخلا_Exit_رڪارڊ_2;        /* Array of EntryExit records--in
                                         * Coverage Index order */
  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, داخلا_Exit_رڪارڊ_2);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    إذا (unlikely (!coverage.sanitize (c, هذا)))
      return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (داخلا_Exit_رڪارڊ_2.sanitize_shallow (c));
    وإلا
      return_trace (داخلا_Exit_رڪارڊ_2.sanitize (c, هذا));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    + hb_zip (هذا+coverage, داخلا_Exit_رڪارڊ_2)
    | hb_filter (c->glyph_سيٽ, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_apply ([&] (ثابت داخلا_Exit_رڪارڊ<Types>& رڪارڊ) { رڪارڊ.collect_variation_indices (c, هذا); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع; }

  فراغ collect_ٻيو_glyphs (hb_سيٽ_digest_t *digest) ثابت
  { (هذا+coverage).collect_coverage (digest); }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    hb_بفر_t *بفر = c->بفر;

    ثابت داخلا_Exit_رڪارڊ<Types> &this_رڪارڊ = داخلا_Exit_رڪارڊ_2[(هذا+coverage).get_coverage  (بفر->cur().codepoint)];
    إذا (!this_رڪارڊ.داخلا_Anchor ||
	unlikely (!this_رڪارڊ.داخلا_Anchor.sanitize (&c->sanitizer, هذا))) return_trace (خطأ);
    hb_barrier ();

    خودڪار &skippy_iter = c->iter_داخلا;
    skippy_iter.reset_fast (بفر->idx);
    بنانشان unsafe_from;
    إذا (unlikely (!skippy_iter.prev (&unsafe_from)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (unsafe_from, بفر->idx + 1);
      return_trace (خطأ);
    }

    ثابت داخلا_Exit_رڪارڊ<Types> &prev_رڪارڊ = داخلا_Exit_رڪارڊ_2[(هذا+coverage).get_coverage  (بفر->info[skippy_iter.idx].codepoint)];
    إذا (!prev_رڪارڊ.exitAnchor ||
	unlikely (!prev_رڪارڊ.exitAnchor.sanitize (&c->sanitizer, هذا)))
    {
      بفر->unsafe_منزل_concat_from_outbuffer (skippy_iter.idx, بفر->idx + 1);
      return_trace (خطأ);
    }
    hb_barrier ();

    return_trace (apply_cursive_attachment (c,
                                            هذا+prev_رڪارڊ.exitAnchor,
                                            هذا+this_رڪارڊ.داخلا_Anchor,
                                            skippy_iter.idx));
  }

  سانچو <قسم_نالو Iterator,
            hb_requires (hb_is_iterator (Iterator))>
  فراغ serialize (hb_subset_context_t *c,
                  Iterator it,
                  ثابت بنية CursivePosFormat1_2<Types> *src_بنياد)
  {
    إذا (unlikely (!c->serializer->extend_min ((*هذا)))) أرجع;
    هذا->format = src_بنياد->format;
    هذا->داخلا_Exit_رڪارڊ_2.len = it.len ();

    لكل (ثابت داخلا_Exit_رڪارڊ<Types>& داخلا_رڪارڊ : + it
                                                      | hb_map (hb_ٻيو))
      داخلا_رڪارڊ.subset (c, src_بنياد);

    خودڪار glyphs =
    + it
    | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
    ;

    coverage.serialize_serialize (c->serializer, glyphs);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);

    خودڪار it =
    + hb_zip (هذا+coverage, داخلا_Exit_رڪارڊ_2)
    | hb_filter (glyphset, hb_پهريون)
    | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_pair_t<hb_codepoint_t, ثابت داخلا_Exit_رڪارڊ<Types>&> p)
                              -> hb_pair_t<hb_codepoint_t, ثابت داخلا_Exit_رڪارڊ<Types>&>
                              { أرجع hb_pair (glyph_map[p.پهريون], p.second);})
    ;

    منطقي ret = منطقي (it);
    ٻاهريون->serialize (c, it, هذا);
    return_trace (ret);
  }
};


}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GPOS_CURSIVEPOSFORMAT1_HH */

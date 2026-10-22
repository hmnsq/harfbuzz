#که_نه_تعریف_شوی OT_LAYOUT_GSUB_ALTERNATESET_HH
#تعریف_کړه OT_LAYOUT_GSUB_ALTERNATESET_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية Alternate_سېټ
{
  محمي:
  Array16_د<ډول_نوم Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  عام:
  DEFINE_اندازه_ARRAY (2, alternates);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (alternates.sanitize (c));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع hb_any (alternates, glyphs); }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  { c->output->جمع_کړه_array (alternates.arrayZ, alternates.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t سرچینه) ثابت
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_معلومات->جمع_کړه_gsub_lookup (سرچینه, c->lookup_شاخص, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جمع_کړه_array (alternates.arrayZ, alternates.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شمېر = alternates.len;

    إذا (unlikely (!شمېر)) return_trace (خطأ);

    hb_ماسک_t glyph_ماسک = c->بفر->cur().ماسک;
    hb_ماسک_t lookup_ماسک = c->lookup_ماسک;

    /* Note: This breaks badly if two features enabled this lookup together. */
    بینښه صحيح ښويدنه = hb_ctz (lookup_ماسک);
    بینښه صحيح alt_شاخص = ((lookup_ماسک & glyph_ماسک) >> ښويدنه);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    إذا (alt_شاخص == HB_OT_MAP_MAX_ارزښت && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->بفر->unsafe_منزل_break (0, c->بفر->len);
      alt_شاخص = c->random_شمېره () % شمېر + 1;
    }

    إذا (unlikely (alt_شاخص > شمېر || alt_شاخص == 0)) return_trace (خطأ);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (alternates[alt_شاخص - 1]);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (ریښتیا);
  }

  بینښه
  get_alternates (بینښه        پیل_دننه_واټن,
                  بینښه       *alternate_شمېر  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    إذا (alternates.len && alternate_شمېر && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (پیل_دننه_واټن, alternate_شمېر)
      | hb_sink (hb_array (alternate_glyphs, *alternate_شمېر))
      ;
    }
    أرجع alternates.len;
  }

  فراغ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_شمېر /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<بینښه, hb_codepoint_t> _) { أرجع hb_pair (gid + (_.لومړی << 24), _.second); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جمع_کړه (p.لومړی, p.second,
						    alternate_شمېر, alternate_glyphs); })
    ;
  }

  کينډۍ <ډول_نوم Iterator,
            hb_requires (hb_is_سرچینه_د (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (alternates.serialize (c, alts));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    خپلکاری *بهر = c->serializer->پیل_embed (*هذا);
    return_trace (بهر->serialize (c->serializer, it) &&
                  بهر->alternates);
  }
};

}
}
}


#د_که_پای /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

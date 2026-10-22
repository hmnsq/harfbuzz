#اگر_تعریف_نشده OT_LAYOUT_GSUB_ALTERNATESET_HH
#تعریف_کن OT_LAYOUT_GSUB_ALTERNATESET_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية Alternate_مجموعه
{
  محمي:
  Array16_از<نام_نوع Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  عام:
  DEFINE_اندازه_ARRAY (2, alternates);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (alternates.sanitize (c));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع hb_any (alternates, glyphs); }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  { c->output->جمع_کن_array (alternates.arrayZ, alternates.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t منبع) ثابت
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_داده->جمع_کن_gsub_lookup (منبع, c->lookup_شاخص, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جمع_کن_array (alternates.arrayZ, alternates.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شمار = alternates.len;

    إذا (unlikely (!شمار)) return_trace (خطأ);

    hb_ماسک_t glyph_ماسک = c->میانگیر->cur().ماسک;
    hb_ماسک_t lookup_ماسک = c->lookup_ماسک;

    /* Note: This breaks badly if two features enabled this lookup together. */
    بیعلامت صحيح لغزش = hb_ctz (lookup_ماسک);
    بیعلامت صحيح alt_شاخص = ((lookup_ماسک & glyph_ماسک) >> لغزش);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    إذا (alt_شاخص == HB_OT_MAP_MAX_مقدار && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->میانگیر->unsafe_مقصد_break (0, c->میانگیر->len);
      alt_شاخص = c->random_عدد () % شمار + 1;
    }

    إذا (unlikely (alt_شاخص > شمار || alt_شاخص == 0)) return_trace (خطأ);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->sync_so_far ();
      c->میانگیر->پیام (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->میانگیر->idx);
    }

    c->replace_glyph (alternates[alt_شاخص - 1]);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->میانگیر->idx - 1u);
    }

    return_trace (درست);
  }

  بیعلامت
  get_alternates (بیعلامت        شروع_جابه_جایی,
                  بیعلامت       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    إذا (alternates.len && alternate_شمار && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (شروع_جابه_جایی, alternate_شمار)
      | hb_sink (hb_array (alternate_glyphs, *alternate_شمار))
      ;
    }
    أرجع alternates.len;
  }

  فراغ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_شمار /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<بیعلامت, hb_codepoint_t> _) { أرجع hb_pair (gid + (_.اولی << 24), _.second); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جمع_کن (p.اولی, p.second,
						    alternate_شمار, alternate_glyphs); })
    ;
  }

  قالب <نام_نوع Iterator,
            hb_requires (hb_is_منبع_از (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (alternates.serialize (c, alts));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    خودکار *بیرون = c->serializer->شروع_embed (*هذا);
    return_trace (بیرون->serialize (c->serializer, it) &&
                  بیرون->alternates);
  }
};

}
}
}


#پایان_اگر /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

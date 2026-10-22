#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_ALTERNATESET_HH
#وصف_ڪريو OT_LAYOUT_GSUB_ALTERNATESET_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية Alternate_سيٽ
{
  محمي:
  Array16_جو<قسم_نالو Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  عام:
  DEFINE_سائيز_ARRAY (2, alternates);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (alternates.sanitize (c));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع hb_any (alternates, glyphs); }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  { c->output->جوڙيو_array (alternates.arrayZ, alternates.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t ذريعو) ثابت
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_معلومات->جوڙيو_gsub_lookup (ذريعو, c->lookup_اشاريو, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جوڙيو_array (alternates.arrayZ, alternates.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح ڳڻپ = alternates.len;

    إذا (unlikely (!ڳڻپ)) return_trace (خطأ);

    hb_نقاب_t glyph_نقاب = c->بفر->cur().نقاب;
    hb_نقاب_t lookup_نقاب = c->lookup_نقاب;

    /* Note: This breaks badly if two features enabled this lookup together. */
    بنانشان صحيح سرڪايو = hb_ctz (lookup_نقاب);
    بنانشان صحيح alt_اشاريو = ((lookup_نقاب & glyph_نقاب) >> سرڪايو);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    إذا (alt_اشاريو == HB_OT_MAP_MAX_قدر && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->بفر->unsafe_منزل_break (0, c->بفر->len);
      alt_اشاريو = c->random_عدد () % ڳڻپ + 1;
    }

    إذا (unlikely (alt_اشاريو > ڳڻپ || alt_اشاريو == 0)) return_trace (خطأ);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (alternates[alt_اشاريو - 1]);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  بنانشان
  get_alternates (بنانشان        شروعات_وٿي,
                  بنانشان       *alternate_ڳڻپ  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    إذا (alternates.len && alternate_ڳڻپ && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (شروعات_وٿي, alternate_ڳڻپ)
      | hb_sink (hb_array (alternate_glyphs, *alternate_ڳڻپ))
      ;
    }
    أرجع alternates.len;
  }

  فراغ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_ڳڻپ /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<بنانشان, hb_codepoint_t> _) { أرجع hb_pair (gid + (_.پهريون << 24), _.second); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جوڙيو (p.پهريون, p.second,
						    alternate_ڳڻپ, alternate_glyphs); })
    ;
  }

  سانچو <قسم_نالو Iterator,
            hb_requires (hb_is_ذريعو_جو (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (alternates.serialize (c, alts));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    خودڪار *ٻاهريون = c->serializer->شروعات_embed (*هذا);
    return_trace (ٻاهريون->serialize (c->serializer, it) &&
                  ٻاهريون->alternates);
  }
};

}
}
}


#جيڪڏهن_پورو /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

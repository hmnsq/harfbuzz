#جے_بنا_تعریف OT_LAYOUT_GSUB_ALTERNATESET_HH
#تعریف_کرو OT_LAYOUT_GSUB_ALTERNATESET_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية Alternate_سیٹ
{
  محمي:
  Array16_دا<قسم_ناں Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  عام:
  DEFINE_ناپ_ARRAY (2, alternates);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (alternates.sanitize (c));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع hb_any (alternates, glyphs); }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  { c->output->جوڑو_array (alternates.arrayZ, alternates.len); }

  فراغ depend (hb_depend_context_t *c, hb_codepoint_t سروت) ثابت
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_انکڑے->جوڑو_gsub_lookup (سروت, c->lookup_اشاریہ, target); })
    ;
  }

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جوڑو_array (alternates.arrayZ, alternates.len); }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح گنتی = alternates.len;

    إذا (unlikely (!گنتی)) return_trace (خطأ);

    hb_نقاب_t glyph_نقاب = c->بفر->cur().نقاب;
    hb_نقاب_t lookup_نقاب = c->lookup_نقاب;

    /* Note: This breaks badly if two features enabled this lookup together. */
    بنانشانی صحيح کھسکاؤ = hb_ctz (lookup_نقاب);
    بنانشانی صحيح alt_اشاریہ = ((lookup_نقاب & glyph_نقاب) >> کھسکاؤ);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    إذا (alt_اشاریہ == HB_OT_MAP_MAX_قدر && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->بفر->unsafe_منزل_break (0, c->بفر->len);
      alt_اشاریہ = c->random_نمبر () % گنتی + 1;
    }

    إذا (unlikely (alt_اشاریہ > گنتی || alt_اشاریہ == 0)) return_trace (خطأ);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->سنیہا (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (alternates[alt_اشاریہ - 1]);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->سنیہا (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  بنانشانی
  get_alternates (بنانشانی        شروع_وتھ,
                  بنانشانی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    إذا (alternates.len && alternate_گنتی && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (شروع_وتھ, alternate_گنتی)
      | hb_sink (hb_array (alternate_glyphs, *alternate_گنتی))
      ;
    }
    أرجع alternates.len;
  }

  فراغ
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_گنتی /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<بنانشانی, hb_codepoint_t> _) { أرجع hb_pair (gid + (_.پہلا << 24), _.second); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جوڑو (p.پہلا, p.second,
						    alternate_گنتی, alternate_glyphs); })
    ;
  }

  سانچا <قسم_ناں Iterator,
            hb_requires (hb_is_سروت_دا (Iterator, hb_codepoint_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (هذا);
    return_trace (alternates.serialize (c, alts));
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    خودکار *باہرلا = c->serializer->شروع_embed (*هذا);
    return_trace (باہرلا->serialize (c->serializer, it) &&
                  باہرلا->alternates);
  }
};

}
}
}


#جے_انت /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

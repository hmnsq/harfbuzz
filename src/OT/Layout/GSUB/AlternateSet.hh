#اگر_غیر_تعریف_شدہ OT_LAYOUT_GSUB_ALTERNATESET_HH
#تعریف_کرو OT_LAYOUT_GSUB_ALTERNATESET_HH

#شامل "Common.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GSUB_impl {

سانچہ <قسم_نام Types>
ساخت Alternate_سیٹ
{
  محفوظ:
  Array16_کا<قسم_نام Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  عوامی:
  DEFINE_سائز_ARRAY (2, alternates);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (alternates.sanitize (c));
  }

  منطقی intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { واپس hb_any (alternates, glyphs); }

  خالی احاطہ (hb_احاطہ_context_t *c) ثابت
  { c->output->جوڑو_array (alternates.arrayZ, alternates.len); }

  خالی depend (hb_depend_context_t *c, hb_codepoint_t ماخذ) ثابت
  {
    + hb_iter (alternates)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_ڈیٹا->جوڑو_gsub_lookup (ماخذ, c->lookup_اشاریہ, target); })
    ;
  }

  خالی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->جوڑو_array (alternates.arrayZ, alternates.len); }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (یہ);
    غیرعلامتی صحیح گنتی = alternates.len;

    اگر (unlikely (!گنتی)) return_trace (جھوٹ);

    hb_نقاب_t glyph_نقاب = c->بفر->cur().نقاب;
    hb_نقاب_t lookup_نقاب = c->lookup_نقاب;

    /* Note: This breaks badly if two features enabled this lookup together. */
    غیرعلامتی صحیح سرکاؤ = hb_ctz (lookup_نقاب);
    غیرعلامتی صحیح alt_اشاریہ = ((lookup_نقاب & glyph_نقاب) >> سرکاؤ);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    اگر (alt_اشاریہ == HB_OT_MAP_MAX_قدر && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->بفر->unsafe_مقصد_break (0, c->بفر->len);
      alt_اشاریہ = c->random_نمبر () % گنتی + 1;
    }

    اگر (unlikely (alt_اشاریہ > گنتی || alt_اشاریہ == 0)) return_trace (جھوٹ);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (alternates[alt_اشاریہ - 1]);

    اگر (HB_بفر_پیغام_مزید && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  غیرعلامتی
  get_alternates (غیرعلامتی        آغاز_انحراف,
                  غیرعلامتی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    اگر (alternates.len && alternate_گنتی && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (آغاز_انحراف, alternate_گنتی)
      | hb_sink (hb_array (alternate_glyphs, *alternate_گنتی))
      ;
    }
    واپس alternates.len;
  }

  خالی
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_گنتی /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<غیرعلامتی, hb_codepoint_t> _) { واپس hb_pair (gid + (_.پہلا << 24), _.second); })
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> خالی
		{ hb_collect_glyph_alternates_جوڑو (p.پہلا, p.second,
						    alternate_گنتی, alternate_glyphs); })
    ;
  }

  سانچہ <قسم_نام Iterator,
            hb_requires (hb_is_ماخذ_کا (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (یہ);
    return_trace (alternates.serialize (c, alts));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (یہ);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    خودکار *برآمد = c->serializer->آغاز_embed (*یہ);
    return_trace (برآمد->serialize (c->serializer, it) &&
                  برآمد->alternates);
  }
};

}
}
}


#اگر_ختم /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

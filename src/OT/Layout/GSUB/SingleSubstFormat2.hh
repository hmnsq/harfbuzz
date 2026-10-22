#اگر_تعریف_نشده OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#تعریف_کن OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

قالب <نام_نوع Types>
بنية SingleSubstFormat2_4
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  نام_نوع Types::قالب LOffset_مقصد<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  نام_نوع Types::قالب Array_از<نام_نوع Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_مجموعه_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    خودکار &cov = هذا+coverage;
    خودکار &glyph_مجموعه = c->والد_فعال_glyphs ();

    // Filter by active glyphs like closure does
    إذا (substitute.len > glyph_مجموعه.get_population () * 4)
    {
      لكل (خودکار g : glyph_مجموعه)
      {
	بیعلامت i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->depend_داده->جمع_کن_gsub_lookup (g, c->lookup_شاخص, substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_مجموعه, hb_اولی)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_داده->جمع_کن_gsub_lookup (_.اولی, c->lookup_شاخص, _.second); })
    ;
  }

  فراغ بستگی (hb_بستگی_context_t *c) ثابت
  {
    خودکار &cov = هذا+coverage;
    خودکار &glyph_مجموعه = c->والد_فعال_glyphs ();

    إذا (substitute.len > glyph_مجموعه.get_population () * 4)
    {
      لكل (خودکار g : glyph_مجموعه)
      {
	بیعلامت i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->output->جمع_کن (substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_مجموعه, hb_اولی)
    | hb_map (hb_دومی)
    | hb_sink (c->output)
    ;
  }

  فراغ بستگی_lookups (hb_بستگی_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->درون_داد))) أرجع;
    + hb_zip (هذا+coverage, substitute)
    | hb_map (hb_دومی)
    | hb_sink (c->output)
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بیعلامت
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        بیعلامت        شروع_جابه_جایی,
                        بیعلامت       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (likely (شاخص_2 == NOT_COVERED))
    {
      إذا (alternate_شمار)
        *alternate_شمار = 0;
      أرجع 0;
    }

    إذا (alternate_شمار && *alternate_شمار && alternate_glyphs)
    {
      glyph_id = substitute[شاخص_2];

      *alternate_glyphs = glyph_id;
      *alternate_شمار = 1;
    }

    أرجع 1;
  }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_شمار /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_zip (هذا+coverage, substitute)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جمع_کن (p.اولی, p.second,
						    alternate_شمار, alternate_glyphs); })
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بیعلامت صحيح شاخص_2 = (هذا+coverage).get_coverage (c->میانگیر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (شاخص_2 >= substitute.len)) return_trace (خطأ);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->sync_so_far ();
      c->میانگیر->پیام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->میانگیر->idx);
    }

    c->replace_glyph (substitute[شاخص_2]);

    إذا (HB_میانگیر_پیام_بیشتر && c->میانگیر->messaging ())
    {
      c->میانگیر->پیام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->میانگیر->idx - 1u);
    }

    return_trace (درست);
  }

  قالب<نام_نوع Iterator,
           hb_requires (hb_is_sorted_منبع_از (Iterator,
                                                hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خودکار substitutes =
      + it
      | hb_map (hb_دومی)
      ;
    خودکار glyphs =
      + it
      | hb_map_retains_ترتیبدهی (hb_اولی)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!substitute.serialize (c, substitutes))) return_trace (خطأ);
    إذا (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (خطأ);
    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_مجموعه_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_اولی)
    | hb_filter (glyphset, hb_دومی)
    | hb_map_retains_ترتیبدهی ([&] (hb_pair_t<hb_codepoint_t, ثابت نام_نوع Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.اولی], glyph_map[p.second]); })
    ;

    منطقي ret = منطقي (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#پایان_اگر /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

#که_نه_تعریف_شوی OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#تعریف_کړه OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

کينډۍ <ډول_نوم Types>
بنية SingleSubstFormat2_4
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  ډول_نوم Types::کينډۍ LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  ډول_نوم Types::کينډۍ Array_د<ډول_نوم Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  عام:
  DEFINE_اندازه_ARRAY (2 + Types::LOffset::static_اندازه + Types::HBUINT::static_اندازه, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سېټ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    خپلکاری &cov = هذا+coverage;
    خپلکاری &glyph_سېټ = c->پلار_فعال_glyphs ();

    // Filter by active glyphs like closure does
    إذا (substitute.len > glyph_سېټ.get_population () * 4)
    {
      لكل (خپلکاری g : glyph_سېټ)
      {
	بینښه i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->depend_معلومات->جمع_کړه_gsub_lookup (g, c->lookup_شاخص, substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سېټ, hb_لومړی)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_معلومات->جمع_کړه_gsub_lookup (_.لومړی, c->lookup_شاخص, _.second); })
    ;
  }

  فراغ تړون (hb_تړون_context_t *c) ثابت
  {
    خپلکاری &cov = هذا+coverage;
    خپلکاری &glyph_سېټ = c->پلار_فعال_glyphs ();

    إذا (substitute.len > glyph_سېټ.get_population () * 4)
    {
      لكل (خپلکاری g : glyph_سېټ)
      {
	بینښه i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->output->جمع_کړه (substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سېټ, hb_لومړی)
    | hb_map (hb_دوهم)
    | hb_sink (c->output)
    ;
  }

  فراغ تړون_lookups (hb_تړون_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->ننوت))) أرجع;
    + hb_zip (هذا+coverage, substitute)
    | hb_map (hb_دوهم)
    | hb_sink (c->output)
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بینښه
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        بینښه        پیل_دننه_واټن,
                        بینښه       *alternate_شمېر  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (likely (شاخص_2 == NOT_COVERED))
    {
      إذا (alternate_شمېر)
        *alternate_شمېر = 0;
      أرجع 0;
    }

    إذا (alternate_شمېر && *alternate_شمېر && alternate_glyphs)
    {
      glyph_id = substitute[شاخص_2];

      *alternate_glyphs = glyph_id;
      *alternate_شمېر = 1;
    }

    أرجع 1;
  }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_شمېر /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_zip (هذا+coverage, substitute)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جمع_کړه (p.لومړی, p.second,
						    alternate_شمېر, alternate_glyphs); })
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بینښه صحيح شاخص_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (شاخص_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (شاخص_2 >= substitute.len)) return_trace (خطأ);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پیغام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (substitute[شاخص_2]);

    إذا (HB_بفر_پیغام_نور && c->بفر->messaging ())
    {
      c->بفر->پیغام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (ریښتیا);
  }

  کينډۍ<ډول_نوم Iterator,
           hb_requires (hb_is_sorted_سرچینه_د (Iterator,
                                                hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خپلکاری substitutes =
      + it
      | hb_map (hb_دوهم)
      ;
    خپلکاری glyphs =
      + it
      | hb_map_retains_ترتیبونه (hb_لومړی)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!substitute.serialize (c, substitutes))) return_trace (خطأ);
    إذا (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (خطأ);
    return_trace (ریښتیا);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سېټ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خپلکاری it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_لومړی)
    | hb_filter (glyphset, hb_دوهم)
    | hb_map_retains_ترتیبونه ([&] (hb_pair_t<hb_codepoint_t, ثابت ډول_نوم Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.لومړی], glyph_map[p.second]); })
    ;

    منطقي ret = منطقي (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#د_که_پای /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

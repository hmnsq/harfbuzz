#جے_بنا_تعریف OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#تعریف_کرو OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچا <قسم_ناں Types>
بنية SingleSubstFormat2_4
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_ناں Types::سانچا LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_ناں Types::سانچا Array_دا<قسم_ناں Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  عام:
  DEFINE_ناپ_ARRAY (2 + Types::LOffset::static_ناپ + Types::HBUINT::static_ناپ, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سیٹ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    خودکار &cov = هذا+coverage;
    خودکار &glyph_سیٹ = c->ماپا_سرگرم_glyphs ();

    // Filter by active glyphs like closure does
    إذا (substitute.len > glyph_سیٹ.get_population () * 4)
    {
      لكل (خودکار g : glyph_سیٹ)
      {
	بنانشانی i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->depend_انکڑے->جوڑو_gsub_lookup (g, c->lookup_اشاریہ, substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سیٹ, hb_پہلا)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_انکڑے->جوڑو_gsub_lookup (_.پہلا, c->lookup_اشاریہ, _.second); })
    ;
  }

  فراغ گھیرا (hb_گھیرا_context_t *c) ثابت
  {
    خودکار &cov = هذا+coverage;
    خودکار &glyph_سیٹ = c->ماپا_سرگرم_glyphs ();

    إذا (substitute.len > glyph_سیٹ.get_population () * 4)
    {
      لكل (خودکار g : glyph_سیٹ)
      {
	بنانشانی i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->output->جوڑو (substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سیٹ, hb_پہلا)
    | hb_map (hb_دوجا)
    | hb_sink (c->output)
    ;
  }

  فراغ گھیرا_lookups (hb_گھیرا_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلہ))) أرجع;
    + hb_zip (هذا+coverage, substitute)
    | hb_map (hb_دوجا)
    | hb_sink (c->output)
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بنانشانی
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        بنانشانی        شروع_وتھ,
                        بنانشانی       *alternate_گنتی  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (likely (اشاریہ_2 == NOT_COVERED))
    {
      إذا (alternate_گنتی)
        *alternate_گنتی = 0;
      أرجع 0;
    }

    إذا (alternate_گنتی && *alternate_گنتی && alternate_glyphs)
    {
      glyph_id = substitute[اشاریہ_2];

      *alternate_glyphs = glyph_id;
      *alternate_گنتی = 1;
    }

    أرجع 1;
  }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_گنتی /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_zip (هذا+coverage, substitute)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جوڑو (p.پہلا, p.second,
						    alternate_گنتی, alternate_glyphs); })
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشانی صحيح اشاریہ_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاریہ_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (اشاریہ_2 >= substitute.len)) return_trace (خطأ);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->سنیہا (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (substitute[اشاریہ_2]);

    إذا (HB_بفر_سنیہا_ہور && c->بفر->messaging ())
    {
      c->بفر->سنیہا (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  سانچا<قسم_ناں Iterator,
           hb_requires (hb_is_sorted_سروت_دا (Iterator,
                                                hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خودکار substitutes =
      + it
      | hb_map (hb_دوجا)
      ;
    خودکار glyphs =
      + it
      | hb_map_retains_چھانٹی (hb_پہلا)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!substitute.serialize (c, substitutes))) return_trace (خطأ);
    إذا (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سیٹ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودکار it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_پہلا)
    | hb_filter (glyphset, hb_دوجا)
    | hb_map_retains_چھانٹی ([&] (hb_pair_t<hb_codepoint_t, ثابت قسم_ناں Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.پہلا], glyph_map[p.second]); })
    ;

    منطقي ret = منطقي (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#جے_انت /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

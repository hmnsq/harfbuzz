#جيڪڏهن_وصف_نه_ٿيل OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#وصف_ڪريو OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#ضمن "Common.hh"

نطاق OT {
نطاق Layout {
نطاق GSUB_impl {

سانچو <قسم_نالو Types>
بنية SingleSubstFormat2_4
{
  محمي:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  قسم_نالو Types::سانچو LOffset_منزل<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  قسم_نالو Types::سانچو Array_جو<قسم_نالو Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  عام:
  DEFINE_سائيز_ARRAY (2 + Types::LOffset::static_سائيز + Types::HBUINT::static_سائيز, substitute);

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (coverage.sanitize (c, هذا) && substitute.sanitize (c));
  }

  منطقي intersects (ثابت hb_سيٽ_t *glyphs) ثابت
  { أرجع (هذا+coverage).intersects (glyphs); }

  منطقي may_have_non_1to1 () ثابت
  { أرجع خطأ; }

  فراغ depend (hb_depend_context_t *c) ثابت
  {
    خودڪار &cov = هذا+coverage;
    خودڪار &glyph_سيٽ = c->والد_سرگرم_glyphs ();

    // Filter by active glyphs like closure does
    إذا (substitute.len > glyph_سيٽ.get_population () * 4)
    {
      لكل (خودڪار g : glyph_سيٽ)
      {
	بنانشان i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->depend_معلومات->جوڙيو_gsub_lookup (g, c->lookup_اشاريو, substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سيٽ, hb_پهريون)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_معلومات->جوڙيو_gsub_lookup (_.پهريون, c->lookup_اشاريو, _.second); })
    ;
  }

  فراغ گهيرو (hb_گهيرو_context_t *c) ثابت
  {
    خودڪار &cov = هذا+coverage;
    خودڪار &glyph_سيٽ = c->والد_سرگرم_glyphs ();

    إذا (substitute.len > glyph_سيٽ.get_population () * 4)
    {
      لكل (خودڪار g : glyph_سيٽ)
      {
	بنانشان i = cov.get_coverage (g);
	إذا (i == NOT_COVERED || i >= substitute.len)
	  استمر;
	c->output->جوڙيو (substitute.arrayZ[i]);
      }

      أرجع;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_سيٽ, hb_پهريون)
    | hb_map (hb_ٻيو)
    | hb_sink (c->output)
    ;
  }

  فراغ گهيرو_lookups (hb_گهيرو_lookups_context_t *c) ثابت {}

  فراغ collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    إذا (unlikely (!(هذا+coverage).collect_coverage (c->داخلا_2))) أرجع;
    + hb_zip (هذا+coverage, substitute)
    | hb_map (hb_ٻيو)
    | hb_sink (c->output)
    ;
  }

  ثابت Coverage &get_coverage () ثابت { أرجع هذا+coverage; }

  منطقي would_apply (hb_would_apply_context_t *c) ثابت
  { أرجع c->len == 1 && (هذا+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  بنانشان
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        بنانشان        شروعات_وٿي,
                        بنانشان       *alternate_ڳڻپ  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (glyph_id);
    إذا (likely (اشاريو_2 == NOT_COVERED))
    {
      إذا (alternate_ڳڻپ)
        *alternate_ڳڻپ = 0;
      أرجع 0;
    }

    إذا (alternate_ڳڻپ && *alternate_ڳڻپ && alternate_glyphs)
    {
      glyph_id = substitute[اشاريو_2];

      *alternate_glyphs = glyph_id;
      *alternate_ڳڻپ = 1;
    }

    أرجع 1;
  }

  فراغ
  collect_glyph_alternates (hb_map_t  *alternate_ڳڻپ /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_zip (هذا+coverage, substitute)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> فراغ
		{ hb_collect_glyph_alternates_جوڙيو (p.پهريون, p.second,
						    alternate_ڳڻپ, alternate_glyphs); })
    ;
  }

  منطقي apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (هذا);
    بنانشان صحيح اشاريو_2 = (هذا+coverage).get_coverage (c->بفر->cur().codepoint);
    إذا (اشاريو_2 == NOT_COVERED) return_trace (خطأ);

    إذا (unlikely (اشاريو_2 >= substitute.len)) return_trace (خطأ);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->sync_so_far ();
      c->بفر->پيغام (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->بفر->idx);
    }

    c->replace_glyph (substitute[اشاريو_2]);

    إذا (HB_بفر_پيغام_وڌيڪ && c->بفر->messaging ())
    {
      c->بفر->پيغام (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->بفر->idx - 1u);
    }

    return_trace (سچ);
  }

  سانچو<قسم_نالو Iterator,
           hb_requires (hb_is_sorted_ذريعو_جو (Iterator,
                                                hb_codepoint_pair_t))>
  منطقي serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (هذا);
    خودڪار substitutes =
      + it
      | hb_map (hb_ٻيو)
      ;
    خودڪار glyphs =
      + it
      | hb_map_retains_ترتيب_ڏيڻ (hb_پهريون)
      ;
    إذا (unlikely (!c->extend_min (هذا))) return_trace (خطأ);
    إذا (unlikely (!substitute.serialize (c, substitutes))) return_trace (خطأ);
    إذا (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (خطأ);
    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (هذا);
    ثابت hb_سيٽ_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    خودڪار it =
    + hb_zip (هذا+coverage, substitute)
    | hb_filter (glyphset, hb_پهريون)
    | hb_filter (glyphset, hb_ٻيو)
    | hb_map_retains_ترتيب_ڏيڻ ([&] (hb_pair_t<hb_codepoint_t, ثابت قسم_نالو Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { أرجع hb_pair (glyph_map[p.پهريون], glyph_map[p.second]); })
    ;

    منطقي ret = منطقي (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#جيڪڏهن_پورو /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

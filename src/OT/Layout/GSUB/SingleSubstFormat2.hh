#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#অন্তর্ভাব "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
কাঠামো SingleSubstFormat2_4
{
  সুরক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  সর্বজনীন:
  DEFINE_আকার_ARRAY (2 + Types::LOffset::static_আকার + Types::HBUINT::static_আকার, substitute);

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (coverage.sanitize (c, এই) && substitute.sanitize (c));
  }

  যৌক্তিক intersects (ধ্রুবক hb_set_t *glyphs) ধ্রুবক
  { ফেরত (এই+coverage).intersects (glyphs); }

  যৌক্তিক may_have_non_1to1 () ধ্রুবক
  { ফেরত মিথ্যা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুবক
  {
    auto &cov = এই+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    যদি (substitute.len > glyph_set.get_population () * 4)
    {
      জন্য (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	যদি (i == NOT_COVERED || i >= substitute.len)
	  চালু;
	c->depend_তথ্য->add_gsub_lookup (g, c->lookup_সূচক, substitute.arrayZ[i]);
      }

      ফেরত;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (ধ্রুবক hb_codepoint_pair_t &_) { c->depend_তথ্য->add_gsub_lookup (_.first, c->lookup_সূচক, _.second); })
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুবক
  {
    auto &cov = এই+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    যদি (substitute.len > glyph_set.get_population () * 4)
    {
      জন্য (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	যদি (i == NOT_COVERED || i >= substitute.len)
	  চালু;
	c->output->add (substitute.arrayZ[i]);
      }

      ফেরত;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুবক {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুবক
  {
    যদি (unlikely (!(এই+coverage).collect_coverage (c->প্রবেশ))) ফেরত;
    + hb_zip (এই+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ধ্রুবক Coverage &get_coverage () ধ্রুবক { ফেরত এই+coverage; }

  যৌক্তিক would_apply (hb_would_apply_context_t *c) ধ্রুবক
  { ফেরত c->len == 1 && (এই+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (glyph_id);
    যদি (likely (সূচক_2 == NOT_COVERED))
    {
      যদি (alternate_গুনতি)
        *alternate_গুনতি = 0;
      ফেরত 0;
    }

    যদি (alternate_গুনতি && *alternate_গুনতি && alternate_glyphs)
    {
      glyph_id = substitute[সূচক_2];

      *alternate_glyphs = glyph_id;
      *alternate_গুনতি = 1;
    }

    ফেরত 1;
  }

  শূন্য
  collect_glyph_alternates (hb_map_t  *alternate_গুনতি /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুবক
  {
    + hb_zip (এই+coverage, substitute)
    | hb_apply ([&] (ধ্রুবক hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> শূন্য
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_গুনতি, alternate_glyphs); })
    ;
  }

  যৌক্তিক apply (hb_ot_apply_context_t *c) ধ্রুবক
  {
    TRACE_APPLY (এই);
    unsigned পূর্ণসংখ্যা সূচক_2 = (এই+coverage).get_coverage (c->বাফার->cur().codepoint);
    যদি (সূচক_2 == NOT_COVERED) return_trace (মিথ্যা);

    যদি (unlikely (সূচক_2 >= substitute.len)) return_trace (মিথ্যা);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->sync_so_far ();
      c->বাফার->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->বাফার->idx);
    }

    c->replace_glyph (substitute[সূচক_2]);

    যদি (HB_বাফার_MESSAGE_MORE && c->বাফার->messaging ())
    {
      c->বাফার->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->বাফার->idx - 1u);
    }

    return_trace (সত্য);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  যৌক্তিক serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (এই);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    যদি (unlikely (!c->extend_min (এই))) return_trace (মিথ্যা);
    যদি (unlikely (!substitute.serialize (c, substitutes))) return_trace (মিথ্যা);
    যদি (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (মিথ্যা);
    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    ধ্রুবক hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুবক hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (এই+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ধ্রুবক typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ফেরত hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    যৌক্তিক ret = যৌক্তিক (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

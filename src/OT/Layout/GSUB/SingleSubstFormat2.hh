#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#অন্তর্ভাৱ "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা SingleSubstFormat2_4
{
  রক্ষিত:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (coverage.sanitize (c, এতত্) && substitute.sanitize (c));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  { ঘূৰাই (এতত্+coverage).intersects (glyphs); }

  তার্কিক may_have_non_1to1 () ধ্রুৱ
  { ঘূৰাই মিছা; }

  শূন্য depend (hb_depend_context_t *c) ধ্রুৱ
  {
    auto &cov = এতত্+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    যদি (substitute.len > glyph_set.get_population () * 4)
    {
      বাবে (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	যদি (i == NOT_COVERED || i >= substitute.len)
	  অনুৱর্ত;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      ঘূৰাই;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (ধ্রুৱ hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  {
    auto &cov = এতত্+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    যদি (substitute.len > glyph_set.get_population () * 4)
    {
      বাবে (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	যদি (i == NOT_COVERED || i >= substitute.len)
	  অনুৱর্ত;
	c->output->add (substitute.arrayZ[i]);
      }

      ঘূৰাই;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  শূন্য closure_lookups (hb_closure_lookups_context_t *c) ধ্রুৱ {}

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  {
    যদি (unlikely (!(এতত্+coverage).collect_coverage (c->প্ৰবেশ))) ঘূৰাই;
    + hb_zip (এতত্+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ধ্রুৱ Coverage &get_coverage () ধ্রুৱ { ঘূৰাই এতত্+coverage; }

  তার্কিক would_apply (hb_would_apply_context_t *c) ধ্রুৱ
  { ঘূৰাই c->len == 1 && (এতত্+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_গুনতি  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (glyph_id);
    যদি (likely (index == NOT_COVERED))
    {
      যদি (alternate_গুনতি)
        *alternate_গুনতি = 0;
      ঘূৰাই 0;
    }

    যদি (alternate_গুনতি && *alternate_গুনতি && alternate_glyphs)
    {
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_গুনতি = 1;
    }

    ঘূৰাই 1;
  }

  শূন্য
  collect_glyph_alternates (hb_map_t  *alternate_গুনতি /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ধ্রুৱ
  {
    + hb_zip (এতত্+coverage, substitute)
    | hb_apply ([&] (ধ্রুৱ hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> শূন্য
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_গুনতি, alternate_glyphs); })
    ;
  }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক index = (এতত্+coverage).get_coverage (c->buffer->cur().codepoint);
    যদি (index == NOT_COVERED) return_trace (মিছা);

    যদি (unlikely (index >= substitute.len)) return_trace (মিছা);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (সঁচা);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (এতত্);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    যদি (unlikely (!c->extend_min (এতত্))) return_trace (মিছা);
    যদি (unlikely (!substitute.serialize (c, substitutes))) return_trace (মিছা);
    যদি (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (মিছা);
    return_trace (সঁচা);
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (এতত্+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ধ্রুৱ typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ঘূৰাই hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    তার্কিক ret = তার্কিক (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار SingleSubstFormat2_4
{
  محافظتشده:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  عمومی:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (coverage.sanitize (c, این) && substitute.sanitize (c));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت (این+coverage).intersects (glyphs); }

  منطقی may_have_non_1to1 () ثابت
  { بازگشت نادرست; }

  تهی depend (hb_depend_context_t *c) ثابت
  {
    auto &cov = این+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    اگر (substitute.len > glyph_set.get_population () * 4)
    {
      برای (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	اگر (i == NOT_COVERED || i >= substitute.len)
	  ادامه;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      بازگشت;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (ثابت hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  تهی closure (hb_closure_context_t *c) ثابت
  {
    auto &cov = این+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    اگر (substitute.len > glyph_set.get_population () * 4)
    {
      برای (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	اگر (i == NOT_COVERED || i >= substitute.len)
	  ادامه;
	c->output->add (substitute.arrayZ[i]);
      }

      بازگشت;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  تهی closure_lookups (hb_closure_lookups_context_t *c) ثابت {}

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  {
    اگر (unlikely (!(این+coverage).collect_coverage (c->درون_داد))) بازگشت;
    + hb_zip (این+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  ثابت Coverage &get_coverage () ثابت { بازگشت این+coverage; }

  منطقی would_apply (hb_would_apply_context_t *c) ثابت
  { بازگشت c->len == 1 && (این+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_شمار  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) ثابت
  {
    unsigned صحیح index = (این+coverage).get_coverage (glyph_id);
    اگر (likely (index == NOT_COVERED))
    {
      اگر (alternate_شمار)
        *alternate_شمار = 0;
      بازگشت 0;
    }

    اگر (alternate_شمار && *alternate_شمار && alternate_glyphs)
    {
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_شمار = 1;
    }

    بازگشت 1;
  }

  تهی
  collect_glyph_alternates (hb_map_t  *alternate_شمار /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) ثابت
  {
    + hb_zip (این+coverage, substitute)
    | hb_apply ([&] (ثابت hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> تهی
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_شمار, alternate_glyphs); })
    ;
  }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح index = (این+coverage).get_coverage (c->buffer->cur().codepoint);
    اگر (index == NOT_COVERED) return_trace (نادرست);

    اگر (unlikely (index >= substitute.len)) return_trace (نادرست);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (درست);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (این);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    اگر (unlikely (!c->extend_min (این))) return_trace (نادرست);
    اگر (unlikely (!substitute.serialize (c, substitutes))) return_trace (نادرست);
    اگر (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (نادرست);
    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (این+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, ثابت typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { بازگشت hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    منطقی ret = منطقی (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

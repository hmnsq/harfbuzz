#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना SingleSubstFormat2_4
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (coverage.sanitize (c, एतत्) && substitute.sanitize (c));
  }

  तार्किक intersects (अचर hb_set_t *glyphs) अचर
  { फर्काऊ (एतत्+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () अचर
  { फर्काऊ असत्य; }

  शून्य depend (hb_depend_context_t *c) अचर
  {
    स्वतः &cov = एतत्+coverage;
    स्वतः &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    यदि (substitute.len > glyph_set.get_population () * 4)
    {
      लागि (स्वतः g : glyph_set)
      {
	अचिन्हित i = cov.get_coverage (g);
	यदि (i == NOT_COVERED || i >= substitute.len)
	  जारी;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      फर्काऊ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (अचर hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  शून्य closure (hb_closure_context_t *c) अचर
  {
    स्वतः &cov = एतत्+coverage;
    स्वतः &glyph_set = c->parent_active_glyphs ();

    यदि (substitute.len > glyph_set.get_population () * 4)
    {
      लागि (स्वतः g : glyph_set)
      {
	अचिन्हित i = cov.get_coverage (g);
	यदि (i == NOT_COVERED || i >= substitute.len)
	  जारी;
	c->output->add (substitute.arrayZ[i]);
      }

      फर्काऊ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) अचर {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) अचर
  {
    यदि (unlikely (!(एतत्+coverage).collect_coverage (c->आगत))) फर्काऊ;
    + hb_zip (एतत्+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  अचर Coverage &get_coverage () अचर { फर्काऊ एतत्+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) अचर
  { फर्काऊ c->len == 1 && (एतत्+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_गन्ती  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) अचर
  {
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (glyph_id);
    यदि (likely (index == NOT_COVERED))
    {
      यदि (alternate_गन्ती)
        *alternate_गन्ती = 0;
      फर्काऊ 0;
    }

    यदि (alternate_गन्ती && *alternate_गन्ती && alternate_glyphs)
    {
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_गन्ती = 1;
    }

    फर्काऊ 1;
  }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_गन्ती /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) अचर
  {
    + hb_zip (एतत्+coverage, substitute)
    | hb_apply ([&] (अचर hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गन्ती, alternate_glyphs); })
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) अचर
  {
    TRACE_APPLY (एतत्);
    अचिन्हित पूर्णाङ्क index = (एतत्+coverage).get_coverage (c->buffer->cur().codepoint);
    यदि (index == NOT_COVERED) return_trace (असत्य);

    यदि (unlikely (index >= substitute.len)) return_trace (असत्य);

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    यदि (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (सत्य);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (एतत्);
    स्वतः substitutes =
      + it
      | hb_map (hb_second)
      ;
    स्वतः glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    यदि (unlikely (!c->extend_min (एतत्))) return_trace (असत्य);
    यदि (unlikely (!substitute.serialize (c, substitutes))) return_trace (असत्य);
    यदि (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    TRACE_SUBSET (एतत्);
    अचर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    अचर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
    + hb_zip (एतत्+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, अचर typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { फर्काऊ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    तार्किक ret = तार्किक (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

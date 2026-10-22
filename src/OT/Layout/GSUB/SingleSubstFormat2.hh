#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना SingleSubstFormat2_4
{
  संरक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (coverage.sanitize (c, हे) && substitute.sanitize (c));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत (हे+coverage).intersects (glyphs); }

  तार्किक may_have_non_1to1 () स्थिरांक
  { परत असत्य; }

  शून्य depend (hb_depend_context_t *c) स्थिरांक
  {
    स्वतः &cov = हे+coverage;
    स्वतः &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    जर (substitute.len > glyph_set.get_population () * 4)
    {
      साठी (स्वतः g : glyph_set)
      {
	अचिन्हित i = cov.get_coverage (g);
	जर (i == NOT_COVERED || i >= substitute.len)
	  सुरूठेवा;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      परत;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (स्थिरांक hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  {
    स्वतः &cov = हे+coverage;
    स्वतः &glyph_set = c->parent_active_glyphs ();

    जर (substitute.len > glyph_set.get_population () * 4)
    {
      साठी (स्वतः g : glyph_set)
      {
	अचिन्हित i = cov.get_coverage (g);
	जर (i == NOT_COVERED || i >= substitute.len)
	  सुरूठेवा;
	c->output->add (substitute.arrayZ[i]);
      }

      परत;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  शून्य closure_lookups (hb_closure_lookups_context_t *c) स्थिरांक {}

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  {
    जर (unlikely (!(हे+coverage).collect_coverage (c->आदान))) परत;
    + hb_zip (हे+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  स्थिरांक Coverage &get_coverage () स्थिरांक { परत हे+coverage; }

  तार्किक would_apply (hb_would_apply_context_t *c) स्थिरांक
  { परत c->len == 1 && (हे+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_मोजणी  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिरांक
  {
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (glyph_id);
    जर (likely (index == NOT_COVERED))
    {
      जर (alternate_मोजणी)
        *alternate_मोजणी = 0;
      परत 0;
    }

    जर (alternate_मोजणी && *alternate_मोजणी && alternate_glyphs)
    {
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_मोजणी = 1;
    }

    परत 1;
  }

  शून्य
  collect_glyph_alternates (hb_map_t  *alternate_मोजणी /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिरांक
  {
    + hb_zip (हे+coverage, substitute)
    | hb_apply ([&] (स्थिरांक hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> शून्य
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_मोजणी, alternate_glyphs); })
    ;
  }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक index = (हे+coverage).get_coverage (c->buffer->cur().codepoint);
    जर (index == NOT_COVERED) return_trace (असत्य);

    जर (unlikely (index >= substitute.len)) return_trace (असत्य);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
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
    TRACE_SERIALIZE (हे);
    स्वतः substitutes =
      + it
      | hb_map (hb_second)
      ;
    स्वतः glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    जर (unlikely (!c->extend_min (हे))) return_trace (असत्य);
    जर (unlikely (!substitute.serialize (c, substitutes))) return_trace (असत्य);
    जर (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (असत्य);
    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
    + hb_zip (हे+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, स्थिरांक typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { परत hb_pair (glyph_map[p.first], glyph_map[p.second]); })
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

#अगर_अघोषित OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#घोषणा OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा SingleSubstFormat2_4
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, substitute);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && substitute.sanitize (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  bool may_have_non_1to1 () स्थिर
  { वापस false; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    स्वतः &cov = यह+coverage;
    स्वतः &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    अगर (substitute.len > glyph_set.get_population () * 4)
    {
      क्रम (स्वतः g : glyph_set)
      {
	अचिन्हित i = cov.get_coverage (g);
	अगर (i == NOT_COVERED || i >= substitute.len)
	  जारी;
	c->depend_तथ्य->add_gsub_lookup (g, c->lookup_सूचकांक, substitute.arrayZ[i]);
      }

      वापस;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (स्थिर hb_codepoint_pair_t &_) { c->depend_तथ्य->add_gsub_lookup (_.first, c->lookup_सूचकांक, _.second); })
    ;
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    स्वतः &cov = यह+coverage;
    स्वतः &glyph_set = c->parent_active_glyphs ();

    अगर (substitute.len > glyph_set.get_population () * 4)
    {
      क्रम (स्वतः g : glyph_set)
      {
	अचिन्हित i = cov.get_coverage (g);
	अगर (i == NOT_COVERED || i >= substitute.len)
	  जारी;
	c->output->add (substitute.arrayZ[i]);
      }

      वापस;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;
    + hb_zip (यह+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  { वापस c->len == 1 && (यह+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_गिनती  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिर
  {
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (glyph_id);
    अगर (likely (सूचकांक_2 == NOT_COVERED))
    {
      अगर (alternate_गिनती)
        *alternate_गिनती = 0;
      वापस 0;
    }

    अगर (alternate_गिनती && *alternate_गिनती && alternate_glyphs)
    {
      glyph_id = substitute[सूचकांक_2];

      *alternate_glyphs = glyph_id;
      *alternate_गिनती = 1;
    }

    वापस 1;
  }

  व्योम
  collect_glyph_alternates (hb_map_t  *alternate_गिनती /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिर
  {
    + hb_zip (यह+coverage, substitute)
    | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> व्योम
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गिनती, alternate_glyphs); })
    ;
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    अगर (unlikely (सूचकांक_2 >= substitute.len)) return_trace (false);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->बफर->idx);
    }

    c->replace_glyph (substitute[सूचकांक_2]);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->बफर->idx - 1u);
    }

    return_trace (true);
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (यह);
    स्वतः substitutes =
      + it
      | hb_map (hb_second)
      ;
    स्वतः glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    अगर (unlikely (!substitute.serialize (c, substitutes))) return_trace (false);
    अगर (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (false);
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
    + hb_zip (यह+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, स्थिर typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { वापस hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    bool ret = bool (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#पूर्ण_अगर /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

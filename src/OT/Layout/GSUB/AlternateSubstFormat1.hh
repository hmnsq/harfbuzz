#अगर_अघोषित OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH
#घोषणा OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH

#समावेश "AlternateSet.hh"
#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा AlternateSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<AlternateSet<Types>>>
                alternateSet;           /* Array of AlternateSet tables
                                         * ordered by Coverage Index */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, alternateSet);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && alternateSet.sanitize (c, यह));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  bool may_have_non_1to1 () स्थिर
  { वापस false; }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (यह+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, स्थिर typename Types::ढाँचा OffsetTo<AlternateSet<Types>>&> &_)
                {
                  स्थिर AlternateSet<Types>& as = यह+_.second;
                  as.depend (c, _.first);
                })
    ;
  }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    + hb_zip (यह+coverage, alternateSet)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर AlternateSet<Types> &_) { _.closure (c); })
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;
    + hb_zip (यह+coverage, alternateSet)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर AlternateSet<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  { वापस c->len == 1 && (यह+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  अचिन्हित
  get_glyph_alternates (hb_codepoint_t  gid,
                        अचिन्हित        start_offset,
                        अचिन्हित       *alternate_गिनती  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिर
  { वापस (यह+alternateSet[(यह+coverage).get_coverage (gid)])
           .get_alternates (start_offset, alternate_गिनती, alternate_glyphs); }

  व्योम
  collect_glyph_alternates (hb_map_t  *alternate_गिनती /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिर
  {
    + hb_iter (alternateSet)
    | hb_map (hb_add (यह))
    | hb_zip (यह+coverage)
    | hb_apply ([&] (स्थिर hb_pair_t<स्थिर AlternateSet<Types> &, hb_codepoint_t> _) {
		  _.first.collect_alternates (_.second, alternate_गिनती, alternate_glyphs);
		})
    ;
  }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    return_trace ((यह+alternateSet[सूचकांक_2]).apply (c));
  }

  ढाँचा <typename GlyphID>
  bool serialize (hb_serialize_context_t *c,
                  hb_sorted_array_t<GlyphID> glyphs,
                  hb_array_t<स्थिर अचिन्हित पूर्णांक> alternate_len_list,
                  hb_array_t<GlyphID> alternate_glyphs_list)
  {
    TRACE_SERIALIZE (यह);
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);
    अगर (unlikely (!alternateSet.serialize (c, glyphs.लंबाई))) return_trace (false);
    क्रम (अचिन्हित पूर्णांक i = 0; i < glyphs.लंबाई; i++)
    {
      अचिन्हित पूर्णांक alternate_len = alternate_len_list[i];
      अगर (unlikely (!alternateSet[i]
                        .serialize_serialize (c, alternate_glyphs_list.sub_array (0, alternate_len))))
        return_trace (false);
      alternate_glyphs_list += alternate_len;
    }
    return_trace (coverage.serialize_serialize (c, glyphs));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः *out = c->serializer->start_embed (*यह);
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);
    out->format = format;

    hb_sorted_vector_t<hb_codepoint_t> new_coverage;
    + hb_zip (यह+coverage, alternateSet)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->alternateSet, यह), hb_second)
    | hb_map (hb_first)
    | hb_map (glyph_map)
    | hb_sink (new_coverage)
    ;
    out->coverage.serialize_serialize (c->serializer, new_coverage.iter ());
    return_trace (bool (new_coverage));
  }
};

}
}
}

#पूर्ण_अगर  /* OT_LAYOUT_GSUB_ALTERNATESUBSTFORMAT1_HH */

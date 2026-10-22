#अगर_अघोषित OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH
#घोषणा OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH

#समावेश "Common.hh"
#समावेश "Sequence.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा MultipleSubstFormat1_2
{
  रक्षित:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::ढाँचा LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::ढाँचा ArrayOf<typename Types::ढाँचा OffsetTo<Sequence<Types>>>
                sequence;               /* Array of Sequence tables
                                         * ordered by Coverage Index */
  खुला:
  DEFINE_आकार_ARRAY (2 + Types::LOffset::static_आकार + Types::HBUINT::static_आकार, sequence);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (coverage.sanitize (c, यह) && sequence.sanitize (c, यह));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस (यह+coverage).intersects (glyphs); }

  bool may_have_non_1to1 () स्थिर
  { वापस true; }

  व्योम closure (hb_closure_context_t *c) स्थिर
  {
    + hb_zip (यह+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर Sequence<Types> &_) { _.closure (c); })
    ;
  }

  व्योम depend (hb_depend_context_t *c) स्थिर
  {
    // Filter by parent_active_glyphs like closure does
    + hb_zip (यह+coverage, sequence)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, स्थिर typename Types::ढाँचा OffsetTo<Sequence<Types>>&> &_)
                {
                  स्थिर Sequence<Types>& s = यह+_.second;
                  s.depend (c, _.first);
                })
    ;
  }

  व्योम closure_lookups (hb_closure_lookups_context_t *c) स्थिर {}

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  {
    अगर (unlikely (!(यह+coverage).collect_coverage (c->निवेश))) वापस;
    + hb_zip (यह+coverage, sequence)
    | hb_map (hb_second)
    | hb_map (hb_add (यह))
    | hb_apply ([c] (स्थिर Sequence<Types> &_) { _.collect_glyphs (c); })
    ;
  }

  स्थिर Coverage &get_coverage () स्थिर { वापस यह+coverage; }

  bool would_apply (hb_would_apply_context_t *c) स्थिर
  { वापस c->len == 1 && (यह+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);

    अचिन्हित पूर्णांक सूचकांक_2 = (यह+coverage).get_coverage (c->बफर->cur().codepoint);
    अगर (सूचकांक_2 == NOT_COVERED) return_trace (false);

    return_trace ((यह+sequence[सूचकांक_2]).apply (c));
  }

  ढाँचा<typename Iterator,
           hb_requires (hb_is_sorted_iterator (Iterator))>
  bool serialize (hb_serialize_context_t *c,
		  Iterator it)
  {
    TRACE_SERIALIZE (यह);
    स्वतः sequences =
      + it
      | hb_map (hb_second)
      ;
    स्वतः glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    अगर (unlikely (!c->extend_min (यह))) return_trace (false);

    अगर (unlikely (!sequence.serialize (c, sequences.लंबाई))) return_trace (false);

    क्रम (स्वतः& pair : hb_zip (sequences, sequence))
    {
      अगर (unlikely (!pair.second
		    .serialize_serialize (c, pair.first)))
        return_trace (false);
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
    + hb_zip (यह+coverage, sequence)
    | hb_filter (glyphset, hb_first)
    | hb_filter (subset_offset_array (c, out->sequence, यह), hb_second)
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


#पूर्ण_अगर /* OT_LAYOUT_GSUB_MULTIPLESUBSTFORMAT1_HH */

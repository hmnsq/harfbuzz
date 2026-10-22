#ifndef OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH
#define OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH

#include "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

template <typename Types>
struct ReverseChainSingleSubstFormat1_2
{
  protected:
  HBUINT16      format;                 /* Format identifier--format = 1 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of table */
  Array16Of<typename Types::template OffsetTo<Coverage>>
                backtrack;              /* Array of coverage tables
                                         * in backtracking sequence, in glyph
                                         * sequence order */
  /* Format 2 widens the backtrack and lookahead coverage offsets but
   * keeps their counts 16-bit. */
  Array16Of<typename Types::template OffsetTo<Coverage>>
                lookaheadX;             /* Array of coverage tables
                                         * in lookahead sequence, in glyph
                                         * sequence order */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substituteX;            /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */
  public:
  DEFINE_SIZE_MIN (6 + Types::LOffset::static_size + Types::HBUINT::static_size);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    もし (!(coverage.sanitize (c, this) && backtrack.sanitize (c, this)))
      return_trace (false);
    hb_barrier ();
    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    もし (!lookahead.sanitize (c, this))
      return_trace (false);
    hb_barrier ();
    const auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    return_trace (substitute.sanitize (c));
  }

  bool intersects (const hb_set_t *glyphs) const
  {
    もし (!(this+coverage).intersects (glyphs))
      戻る false;

    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);

    unsigned 整数 count;

    count = backtrack.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (!(this+backtrack[i]).intersects (glyphs))
        戻る false;

    count = lookahead.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (!(this+lookahead[i]).intersects (glyphs))
        戻る false;

    戻る true;
  }

  bool may_have_non_1to1 () const
  { 戻る false; }

  void depend (hb_depend_context_t *c) const {
    // Filter by intersects and parent_active_glyphs like closure does
    もし (!intersects (c->glyphs)) 戻る;

    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    const auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    /* Build context set from backtrack and lookahead coverages.
     * ReverseChainSingleSubst has contextual requirements - the substitution
     * only fires when backtrack and lookahead glyphs are present. */
    hb_set_t context_glyphs;

    /* Add glyphs from backtrack coverages */
    unsigned 整数 count = backtrack.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      hb_set_t back_glyphs;
      (this+backtrack[i]).intersect_set (*c->glyphs, back_glyphs);
      もし (back_glyphs.get_population () == 1)
        context_glyphs.add (back_glyphs.get_min ());
      否则 もし (back_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (back_glyphs);
        もし (unlikely (set_idx == HB_CODEPOINT_INVALID))
          戻る;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Add glyphs from lookahead coverages */
    count = lookahead.len;
    ために (unsigned 整数 i = 0; i < count; i++)
    {
      hb_set_t look_glyphs;
      (this+lookahead[i]).intersect_set (*c->glyphs, look_glyphs);
      もし (look_glyphs.get_population () == 1)
        context_glyphs.add (look_glyphs.get_min ());
      否则 もし (look_glyphs.get_population () > 1)
      {
        hb_codepoint_t set_idx = c->depend_data->find_or_create_context_set (look_glyphs);
        もし (unlikely (set_idx == HB_CODEPOINT_INVALID))
          戻る;
        context_glyphs.add (HB_DEPEND_CONTEXT_SET_FLAG | set_idx);
      }
    }

    /* Allocate context set and save/restore around edge creation */
    もし (unlikely (context_glyphs.in_error ()))
    {
      c->depend_data->fail ();
      戻る;
    }
    hb_codepoint_t context_set_idx = context_glyphs.is_empty ()
      ? HB_CODEPOINT_INVALID
      : c->depend_data->find_or_create_context_set (context_glyphs);
    もし (unlikely (!context_glyphs.is_empty () &&
                  context_set_idx == HB_CODEPOINT_INVALID))
      戻る;

    hb_codepoint_t saved_context = c->depend_data->current_context_set_index;
    c->depend_data->current_context_set_index = context_set_idx;

    + hb_zip (this+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_apply ([&] (const hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;

    c->depend_data->current_context_set_index = saved_context;
  }

  void closure (hb_closure_context_t *c) const
  {
    もし (!intersects (c->glyphs)) 戻る;

    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    const auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    + hb_zip (this+coverage, substitute)
    | hb_filter (c->parent_active_glyphs (), hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    もし (unlikely (!(this+coverage).collect_coverage (c->input))) 戻る;

    unsigned 整数 count;

    count = backtrack.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (unlikely (!(this+backtrack[i]).collect_coverage (c->before))) 戻る;

    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    count = lookahead.len;
    ために (unsigned 整数 i = 0; i < count; i++)
      もし (unlikely (!(this+lookahead[i]).collect_coverage (c->after))) 戻る;

    const auto &substitute = StructAfter<decltype (substituteX)> (lookahead);
    count = substitute.len;
    c->output->add_array (substitute.arrayZ, substitute.len);
  }

  const Coverage &get_coverage () const { 戻る this+coverage; }

  bool would_apply (hb_would_apply_context_t *c) const
  { 戻る c->len == 1 && (this+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    unsigned 整数 index = (this+coverage).get_coverage (c->buffer->cur ().codepoint);
    もし (index == NOT_COVERED) return_trace (false);

    もし (unlikely (c->nesting_level_left != HB_MAX_NESTING_LEVEL))
      return_trace (false); /* No chaining to this type */

    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    const auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    もし (unlikely (index >= substitute.len)) return_trace (false);

    unsigned 整数 start_index = 0, end_index = 0;
    もし (match_backtrack (c,
                         backtrack.len, backtrack.arrayZ,
                         match_coverage, this,
                         &start_index) &&
        match_lookahead (c,
                         lookahead.len, lookahead.arrayZ,
                         match_coverage, this,
                         c->buffer->idx + 1, &end_index))
    {
      c->buffer->unsafe_to_break_from_outbuffer (start_index, end_index);

      もし (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replacing glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph_inplace (substitute[index]);

      もし (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (reverse chaining substitution)",
			    c->buffer->idx);
      }

      /* Note: We DON'T decrease buffer->idx.  The main loop does it
       * for us.  This is useful for preventing surprises if someone
       * calls us through a Context lookup. */
      return_trace (true);
    }
    否则
    {
      c->buffer->unsafe_to_concat_from_outbuffer (start_index, end_index);
      return_trace (false);
    }
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize_coverage_offset_array (hb_subset_context_t *c, Iterator it) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    もし (unlikely (!out->serialize (c->serializer, 0u))) return_trace (false);

    ために (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      もし (unlikely (!o) || !o->serialize_subset (c, offset, this))
        return_trace (false);
    }

    return_trace (true);
  }

  template<typename Iterator, typename BacktrackIterator, typename LookaheadIterator,
           hb_requires (hb_is_sorted_source_of (Iterator, hb_codepoint_pair_t)),
           hb_requires (hb_is_iterator (BacktrackIterator)),
           hb_requires (hb_is_iterator (LookaheadIterator))>
  bool serialize (hb_subset_context_t *c,
                  Iterator coverage_subst_iter,
                  BacktrackIterator backtrack_iter,
                  LookaheadIterator lookahead_iter) const
  {
    TRACE_SERIALIZE (this);

    auto *out = c->serializer->start_embed (this);
    もし (unlikely (!c->serializer->embed (this->format))) return_trace (false);
    もし (unlikely (!c->serializer->embed (this->coverage))) return_trace (false);

    もし (!serialize_coverage_offset_array (c, backtrack_iter)) return_trace (false);
    もし (!serialize_lookahead_coverage_offset_array (c, lookahead_iter)) return_trace (false);

    auto *substitute_out = c->serializer->start_embed<typename Types::template ArrayOf<typename Types::HBGlyphID>> ();
    auto substitutes =
    + coverage_subst_iter
    | hb_map (hb_second)
    ;

    auto glyphs =
    + coverage_subst_iter
    | hb_map_retains_sorting (hb_first)
    ;
    もし (unlikely (! c->serializer->check_success (substitute_out->serialize (c->serializer, substitutes))))
        return_trace (false);

    もし (unlikely (!out->coverage.serialize_serialize (c->serializer, glyphs)))
      return_trace (false);
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    const auto &lookahead = StructAfter<decltype (lookaheadX)> (backtrack);
    const auto &substitute = StructAfter<decltype (substituteX)> (lookahead);

    auto it =
    + hb_zip (this+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, const typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { 戻る hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    return_trace (bool (it) && serialize (c, it, backtrack.iter (), lookahead.iter ()));
  }

  template<typename Iterator,
           hb_requires (hb_is_iterator (Iterator))>
  bool serialize_lookahead_coverage_offset_array (hb_subset_context_t *c, Iterator it) const
  {
    TRACE_SERIALIZE (this);
    auto *out = c->serializer->start_embed<Array16Of<typename Types::template OffsetTo<Coverage>>> ();
    もし (unlikely (!out->serialize (c->serializer, 0u))) return_trace (false);

    ために (auto& offset : it) {
      auto *o = out->serialize_append (c->serializer);
      もし (unlikely (!o) || !o->serialize_subset (c, offset, this))
        return_trace (false);
    }

    return_trace (true);
  }
};

}
}
}

#endif  /* HB_OT_LAYOUT_GSUB_REVERSECHAINSINGLESUBSTFORMAT1_HH */

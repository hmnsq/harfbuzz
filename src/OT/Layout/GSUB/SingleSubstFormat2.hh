#ifndef OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH
#define OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH

#include "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

template <typename Types>
sa_nracanaa SingleSubstFormat2_4
{
  rak_sita:
  HBUINT16      format;                 /* Format identifier--format = 2 */
  typename Types::template LOffsetTo<Coverage>
                coverage;               /* Offset to Coverage table--from
                                         * beginning of Substitution table */
  typename Types::template ArrayOf<typename Types::HBGlyphID>
                substitute;             /* Array of substitute
                                         * GlyphIDs--ordered by Coverage Index */

  public:
  DEFINE_SIZE_ARRAY (2 + Types::LOffset::static_size + Types::HBUINT::static_size, substitute);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (coverage.sanitize (c, this) && substitute.sanitize (c));
  }

  bool intersects (const hb_set_t *glyphs) const
  { ᬯᬮᬶ (this+coverage).intersects (glyphs); }

  bool may_have_non_1to1 () const
  { ᬯᬮᬶ false; }

  void depend (hb_depend_context_t *c) const
  {
    auto &cov = this+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    // Filter by active glyphs like closure does
    ᬬᬾᬦ᭄ (substitute.len > glyph_set.get_population () * 4)
    {
      for (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	ᬬᬾᬦ᭄ (i == NOT_COVERED || i >= substitute.len)
	  continue;
	c->depend_data->add_gsub_lookup (g, c->lookup_index, substitute.arrayZ[i]);
      }

      ᬯᬮᬶ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_apply ([&] (const hb_codepoint_pair_t &_) { c->depend_data->add_gsub_lookup (_.first, c->lookup_index, _.second); })
    ;
  }

  void closure (hb_closure_context_t *c) const
  {
    auto &cov = this+coverage;
    auto &glyph_set = c->parent_active_glyphs ();

    ᬬᬾᬦ᭄ (substitute.len > glyph_set.get_population () * 4)
    {
      for (auto g : glyph_set)
      {
	unsigned i = cov.get_coverage (g);
	ᬬᬾᬦ᭄ (i == NOT_COVERED || i >= substitute.len)
	  continue;
	c->output->add (substitute.arrayZ[i]);
      }

      ᬯᬮᬶ;
    }

    + hb_zip (cov, substitute)
    | hb_filter (glyph_set, hb_first)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  void closure_lookups (hb_closure_lookups_context_t *c) const {}

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  {
    ᬬᬾᬦ᭄ (unlikely (!(this+coverage).collect_coverage (c->input))) ᬯᬮᬶ;
    + hb_zip (this+coverage, substitute)
    | hb_map (hb_second)
    | hb_sink (c->output)
    ;
  }

  const Coverage &get_coverage () const { ᬯᬮᬶ this+coverage; }

  bool would_apply (hb_would_apply_context_t *c) const
  { ᬯᬮᬶ c->len == 1 && (this+coverage).get_coverage (c->glyphs[0]) != NOT_COVERED; }

  unsigned
  get_glyph_alternates (hb_codepoint_t  glyph_id,
                        unsigned        start_offset,
                        unsigned       *alternate_count  /* IN/OUT.  May be NULL. */,
                        hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) const
  {
    unsigned puur_naa_nka index = (this+coverage).get_coverage (glyph_id);
    ᬬᬾᬦ᭄ (likely (index == NOT_COVERED))
    {
      ᬬᬾᬦ᭄ (alternate_count)
        *alternate_count = 0;
      ᬯᬮᬶ 0;
    }

    ᬬᬾᬦ᭄ (alternate_count && *alternate_count && alternate_glyphs)
    {
      glyph_id = substitute[index];

      *alternate_glyphs = glyph_id;
      *alternate_count = 1;
    }

    ᬯᬮᬶ 1;
  }

  void
  collect_glyph_alternates (hb_map_t  *alternate_count /* IN/OUT */,
			    hb_map_t  *alternate_glyphs /* IN/OUT */) const
  {
    + hb_zip (this+coverage, substitute)
    | hb_apply ([&] (const hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> void
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_count, alternate_glyphs); })
    ;
  }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    unsigned puur_naa_nka index = (this+coverage).get_coverage (c->buffer->cur().codepoint);
    ᬬᬾᬦ᭄ (index == NOT_COVERED) return_trace (false);

    ᬬᬾᬦ᭄ (unlikely (index >= substitute.len)) return_trace (false);

    ᬬᬾᬦ᭄ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (single substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (substitute[index]);

    ᬬᬾᬦ᭄ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (single substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (true);
  }

  template<typename Iterator,
           hb_requires (hb_is_sorted_source_of (Iterator,
                                                hb_codepoint_pair_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator it)
  {
    TRACE_SERIALIZE (this);
    auto substitutes =
      + it
      | hb_map (hb_second)
      ;
    auto glyphs =
      + it
      | hb_map_retains_sorting (hb_first)
      ;
    ᬬᬾᬦ᭄ (unlikely (!c->extend_min (this))) return_trace (false);
    ᬬᬾᬦ᭄ (unlikely (!substitute.serialize (c, substitutes))) return_trace (false);
    ᬬᬾᬦ᭄ (unlikely (!coverage.serialize_serialize (c, glyphs))) return_trace (false);
    return_trace (true);
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
    + hb_zip (this+coverage, substitute)
    | hb_filter (glyphset, hb_first)
    | hb_filter (glyphset, hb_second)
    | hb_map_retains_sorting ([&] (hb_pair_t<hb_codepoint_t, const typename Types::HBGlyphID &> p) -> hb_codepoint_pair_t
                              { ᬯᬮᬶ hb_pair (glyph_map[p.first], glyph_map[p.second]); })
    ;

    bool ret = bool (it);
    SingleSubst_serialize (c->serializer, it);
    return_trace (ret);
  }
};

}
}
}

#endif /* OT_LAYOUT_GSUB_SINGLESUBSTFORMAT2_HH */

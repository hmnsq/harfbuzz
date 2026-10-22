#ifndef OT_LAYOUT_GSUB_ALTERNATESET_HH
#define OT_LAYOUT_GSUB_ALTERNATESET_HH

#include "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

template <typename Types>
struct AlternateSet
{
  protected:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  public:
  DEFINE_SIZE_ARRAY (2, alternates);

  bool sanitize (hb_sanitize_context_t *c) const
  {
    TRACE_SANITIZE (this);
    return_trace (alternates.sanitize (c));
  }

  bool intersects (const hb_set_t *glyphs) const
  { return hb_any (alternates, glyphs); }

  void closure (hb_closure_context_t *c) const
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  void depend (hb_depend_context_t *c, hb_codepoint_t source) const
  {
    + hb_iter (alternates)
    | hb_apply ([&] (const hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  void collect_glyphs (hb_collect_glyphs_context_t *c) const
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  bool apply (hb_ot_apply_context_t *c) const
  {
    TRACE_APPLY (this);
    unsigned int बिलांग = alternates.len;

    if (unlikely (!बिलांग)) return_trace (false);

    hb_mask_t glyph_mask = c->buffer->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    unsigned int shift = hb_ctz (lookup_mask);
    unsigned int alt_index = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    if (alt_index == HB_OT_MAP_MAX_हालागा && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->buffer->unsafe_to_break (0, c->buffer->len);
      alt_index = c->random_नुमेरो () % बिलांग + 1;
    }

    if (unlikely (alt_index > बिलांग || alt_index == 0)) return_trace (false);

    if (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->buffer->idx);
    }

    c->replace_glyph (alternates[alt_index - 1]);

    if (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->buffer->idx - 1u);
    }

    return_trace (true);
  }

  unsigned
  get_alternates (unsigned        start_offset,
                  unsigned       *alternate_बिलांग  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) const
  {
    if (alternates.len && alternate_बिलांग && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_बिलांग)
      | hb_sink (hb_array (alternate_glyphs, *alternate_बिलांग))
      ;
    }
    return alternates.len;
  }

  void
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_बिलांग /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) const
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<unsigned, hb_codepoint_t> _) { return hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (const hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> void
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_बिलांग, alternate_glyphs); })
    ;
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (this);
    return_trace (alternates.serialize (c, alts));
  }

  bool subset (hb_subset_context_t *c) const
  {
    TRACE_SUBSET (this);
    const hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    const hb_map_t &glyph_map = *c->plan->glyph_map;

    auto it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    auto *out = c->serializer->start_embed (*this);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#endif /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

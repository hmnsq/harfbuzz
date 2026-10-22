#अगर_अघोषित OT_LAYOUT_GSUB_ALTERNATESET_HH
#घोषणा OT_LAYOUT_GSUB_ALTERNATESET_HH

#समावेश "Common.hh"

namespace OT {
namespace Layout {
namespace GSUB_impl {

ढाँचा <typename Types>
काष्ठा AlternateSet
{
  रक्षित:
  Array16Of<typename Types::HBGlyphID>
                alternates;             /* Array of alternate GlyphIDs--in
                                         * arbitrary order */
  खुला:
  DEFINE_आकार_ARRAY (2, alternates);

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (alternates.sanitize (c));
  }

  bool intersects (स्थिर hb_set_t *glyphs) स्थिर
  { वापस hb_any (alternates, glyphs); }

  व्योम closure (hb_closure_context_t *c) स्थिर
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  व्योम depend (hb_depend_context_t *c, hb_codepoint_t source) स्थिर
  {
    + hb_iter (alternates)
    | hb_apply ([&] (स्थिर hb_codepoint_t &target) { c->depend_तथ्य->add_gsub_lookup (source, c->lookup_सूचकांक, target); })
    ;
  }

  व्योम collect_glyphs (hb_collect_glyphs_context_t *c) स्थिर
  { c->output->add_array (alternates.arrayZ, alternates.len); }

  bool apply (hb_ot_apply_context_t *c) स्थिर
  {
    TRACE_APPLY (यह);
    अचिन्हित पूर्णांक गिनती = alternates.len;

    अगर (unlikely (!गिनती)) return_trace (false);

    hb_mask_t glyph_mask = c->बफर->cur().mask;
    hb_mask_t lookup_mask = c->lookup_mask;

    /* Note: This breaks badly if two features enabled this lookup together. */
    अचिन्हित पूर्णांक shift = hb_ctz (lookup_mask);
    अचिन्हित पूर्णांक alt_सूचकांक = ((lookup_mask & glyph_mask) >> shift);

    /* If alt_index is MAX_VALUE, randomize feature if it is the rand feature. */
    अगर (alt_सूचकांक == HB_OT_MAP_MAX_मूल्य && c->random)
    {
      /* Maybe we can do better than unsafe-to-break all; but since we are
       * changing random state, it would be hard to track that.  Good 'nough. */
      c->बफर->unsafe_to_break (0, c->बफर->len);
      alt_सूचकांक = c->random_संख्या () % गिनती + 1;
    }

    अगर (unlikely (alt_सूचकांक > गिनती || alt_सूचकांक == 0)) return_trace (false);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->sync_so_far ();
      c->बफर->message (c->font,
			  "replacing glyph at %u (alternate substitution)",
			  c->बफर->idx);
    }

    c->replace_glyph (alternates[alt_सूचकांक - 1]);

    अगर (HB_बफर_MESSAGE_MORE && c->बफर->messaging ())
    {
      c->बफर->message (c->font,
			  "replaced glyph at %u (alternate substitution)",
			  c->बफर->idx - 1u);
    }

    return_trace (true);
  }

  अचिन्हित
  get_alternates (अचिन्हित        start_offset,
                  अचिन्हित       *alternate_गिनती  /* IN/OUT.  May be NULL. */,
                  hb_codepoint_t *alternate_glyphs /* OUT.     May be NULL. */) स्थिर
  {
    अगर (alternates.len && alternate_गिनती && alternate_glyphs)
    {
      + alternates.as_array ().sub_array (start_offset, alternate_गिनती)
      | hb_sink (hb_array (alternate_glyphs, *alternate_गिनती))
      ;
    }
    वापस alternates.len;
  }

  व्योम
  collect_alternates (hb_codepoint_t gid,
		      hb_map_t  *alternate_गिनती /* IN/OUT */,
		      hb_map_t  *alternate_glyphs /* IN/OUT */) स्थिर
  {
    + hb_enumerate (alternates)
    | hb_map ([gid] (hb_pair_t<अचिन्हित, hb_codepoint_t> _) { वापस hb_pair (gid + (_.first << 24), _.second); })
    | hb_apply ([&] (स्थिर hb_pair_t<hb_codepoint_t, hb_codepoint_t> &p) -> व्योम
		{ _hb_collect_glyph_alternates_add (p.first, p.second,
						    alternate_गिनती, alternate_glyphs); })
    ;
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  bool serialize (hb_serialize_context_t *c,
                  Iterator alts)
  {
    TRACE_SERIALIZE (यह);
    return_trace (alternates.serialize (c, alts));
  }

  bool subset (hb_subset_context_t *c) स्थिर
  {
    TRACE_SUBSET (यह);
    स्थिर hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिर hb_map_t &glyph_map = *c->plan->glyph_map;

    स्वतः it =
      + hb_iter (alternates)
      | hb_filter (glyphset)
      | hb_map (glyph_map)
      ;

    स्वतः *out = c->serializer->start_embed (*यह);
    return_trace (out->serialize (c->serializer, it) &&
                  out->alternates);
  }
};

}
}
}


#पूर्ण_अगर /* OT_LAYOUT_GSUB_ALTERNATESET_HH */

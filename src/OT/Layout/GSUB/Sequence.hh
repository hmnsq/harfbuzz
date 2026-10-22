#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#അന്തര്ഭാവ "Common.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GSUB_impl {

template <typename Types>
ഘടന Sequence
{
  സംരക്ഷിത:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  പൊതു:
  DEFINE_SIZE_ARRAY (2, substitute);

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (substitute.sanitize (c));
  }

  തർക്കം intersects (സ്ഥിരം hb_set_t *glyphs) സ്ഥിരം
  { തിരികെ hb_all (substitute, glyphs); }

  ശൂന്യം closure (hb_closure_context_t *c) സ്ഥിരം
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  ശൂന്യം depend (hb_depend_context_t *c, hb_codepoint_t source) സ്ഥിരം
  {
    + hb_iter (substitute)
    | hb_apply ([&] (സ്ഥിരം hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  ശൂന്യം collect_glyphs (hb_collect_glyphs_context_t *c) സ്ഥിരം
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  തർക്കം apply (hb_ot_apply_context_t *c) സ്ഥിരം
  {
    TRACE_APPLY (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ എണ്ണം = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    എങ്കിൽ (unlikely (എണ്ണം == 1))
    {
      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (സത്യം);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    അല്ലെങ്കിൽ എങ്കിൽ (unlikely (എണ്ണം == 0))
    {
      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (സത്യം);
    }

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned പൂർണ്ണസംഖ്യ klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      എങ്കിൽ (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    എങ്കിൽ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      അക്ഷരം buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      അക്ഷരം *p = buf;

      വേണ്ടി (unsigned i = c->buffer->idx - എണ്ണം; i < c->buffer->idx; i++)
      {
	എങ്കിൽ (buf < p && വലുപ്പം(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, വലുപ്പം(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (സത്യം);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  തർക്കം serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (ഇത്);
    return_trace (substitute.serialize (c, subst));
  }

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    സ്ഥിരം hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    സ്ഥിരം hb_map_t &glyph_map = *c->plan->glyph_map;

    എങ്കിൽ (!intersects (&glyphset)) return_trace (അസത്യം);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*ഇത്);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

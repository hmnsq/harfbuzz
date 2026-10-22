#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#અન્તર્ભાવ "Common.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GSUB_impl {

template <typename Types>
સંરચના Sequence
{
  સુરક્ષિત:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  જાહેર:
  DEFINE_SIZE_ARRAY (2, substitute);

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (substitute.sanitize (c));
  }

  તાર્કિક intersects (અચલ hb_set_t *glyphs) અચલ
  { પાછા hb_all (substitute, glyphs); }

  શૂન્ય closure (hb_closure_context_t *c) અચલ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  શૂન્ય depend (hb_depend_context_t *c, hb_codepoint_t source) અચલ
  {
    + hb_iter (substitute)
    | hb_apply ([&] (અચલ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  શૂન્ય collect_glyphs (hb_collect_glyphs_context_t *c) અચલ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  તાર્કિક apply (hb_ot_apply_context_t *c) અચલ
  {
    TRACE_APPLY (આ);
    unsigned પૂર્ણાંક ગણતરી = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    જો (unlikely (ગણતરી == 1))
    {
      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (સાચું);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    નહીંતર જો (unlikely (ગણતરી == 0))
    {
      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (સાચું);
    }

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned પૂર્ણાંક klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      જો (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    જો (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      અક્ષર buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      અક્ષર *p = buf;

      માટે (unsigned i = c->buffer->idx - ગણતરી; i < c->buffer->idx; i++)
      {
	જો (buf < p && કદ(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, કદ(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (સાચું);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  તાર્કિક serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (આ);
    return_trace (substitute.serialize (c, subst));
  }

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    TRACE_SUBSET (આ);
    અચલ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    અચલ hb_map_t &glyph_map = *c->plan->glyph_map;

    જો (!intersects (&glyphset)) return_trace (ખોટું);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*આ);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

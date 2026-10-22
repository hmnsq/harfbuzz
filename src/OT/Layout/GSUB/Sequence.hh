#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

template <typename Types>
संरचना Sequence
{
  रक्षित:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, substitute);

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (substitute.sanitize (c));
  }

  तार्किक intersects (ध्रुव hb_set_t *glyphs) ध्रुव
  { प्रत्यागम hb_all (substitute, glyphs); }

  शून्य closure (hb_closure_context_t *c) ध्रुव
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t source) ध्रुव
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ध्रुव hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) ध्रुव
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  तार्किक apply (hb_ot_apply_context_t *c) ध्रुव
  {
    TRACE_APPLY (एतत्);
    unsigned पूर्णाङ्क गणनम् = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    जइ (unlikely (गणनम् == 1))
    {
      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (सच्च);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    अण्णहा जइ (unlikely (गणनम् == 0))
    {
      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (सच्च);
    }

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned पूर्णाङ्क klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      जइ (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    जइ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      वर्ण buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      वर्ण *p = buf;

      प्रति (unsigned i = c->buffer->idx - गणनम्; i < c->buffer->idx; i++)
      {
	जइ (buf < p && परिमाण(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, परिमाण(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (सच्च);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (एतत्);
    return_trace (substitute.serialize (c, subst));
  }

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    ध्रुव hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ध्रुव hb_map_t &glyph_map = *c->plan->glyph_map;

    जइ (!intersects (&glyphset)) return_trace (मिच्छ);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*एतत्);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

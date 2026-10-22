#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#ಅನ್ತರ್ಭಾವ "Common.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GSUB_impl {

template <typename Types>
ಸಂರಚನಾ Sequence
{
  ರಕ್ಷಿತ:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (2, substitute);

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (substitute.sanitize (c));
  }

  ತಾರ್ಕಿಕ intersects (ಧ್ರುವ hb_set_t *glyphs) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ hb_all (substitute, glyphs); }

  ಶೂನ್ಯ closure (hb_closure_context_t *c) ಧ್ರುವ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  ಶೂನ್ಯ depend (hb_depend_context_t *c, hb_codepoint_t source) ಧ್ರುವ
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ಧ್ರುವ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  ಶೂನ್ಯ collect_glyphs (hb_collect_glyphs_context_t *c) ಧ್ರುವ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  ತಾರ್ಕಿಕ apply (hb_ot_apply_context_t *c) ಧ್ರುವ
  {
    TRACE_APPLY (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    ಯದಿ (unlikely (ಎಣಿಕೆ == 1))
    {
      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (ಸತ್ಯ);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    ಅನ್ಯಥಾ ಯದಿ (unlikely (ಎಣಿಕೆ == 0))
    {
      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (ಸತ್ಯ);
    }

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned ಪೂರ್ಣಾಙ್ಕ klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      ಯದಿ (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    ಯದಿ (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      ವರ್ಣ buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      ವರ್ಣ *p = buf;

      ಪ್ರತಿ (unsigned i = c->buffer->idx - ಎಣಿಕೆ; i < c->buffer->idx; i++)
      {
	ಯದಿ (buf < p && ಪರಿಮಾಣ(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, ಪರಿಮಾಣ(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (ಸತ್ಯ);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  ತಾರ್ಕಿಕ serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (ಏತತ್);
    return_trace (substitute.serialize (c, subst));
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    ಧ್ರುವ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ಧ್ರುವ hb_map_t &glyph_map = *c->plan->glyph_map;

    ಯದಿ (!intersects (&glyphset)) return_trace (ಅಸತ್ಯ);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*ಏತತ್);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

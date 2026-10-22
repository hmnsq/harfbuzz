#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#ଅନ୍ତର୍ଭାଵ "Common.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GSUB_impl {

template <typename Types>
ସଂରଚନା Sequence
{
  ସୁରକ୍ଷିତ:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (2, substitute);

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (substitute.sanitize (c));
  }

  ତାର୍କିକ intersects (ଅଚଳ hb_set_t *glyphs) ଅଚଳ
  { ଫେରନ୍ତୁ hb_all (substitute, glyphs); }

  ଶୂନ୍ୟ closure (hb_closure_context_t *c) ଅଚଳ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  ଶୂନ୍ୟ depend (hb_depend_context_t *c, hb_codepoint_t source) ଅଚଳ
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ଅଚଳ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  ଶୂନ୍ୟ collect_glyphs (hb_collect_glyphs_context_t *c) ଅଚଳ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  ତାର୍କିକ apply (hb_ot_apply_context_t *c) ଅଚଳ
  {
    TRACE_APPLY (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    ଯଦି (unlikely (ଗଣନା_2 == 1))
    {
      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (ସତ୍ୟ);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    ଅନ୍ୟଥା ଯଦି (unlikely (ଗଣନା_2 == 0))
    {
      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (ସତ୍ୟ);
    }

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      ଯଦି (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    ଯଦି (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      ଅକ୍ଷର buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      ଅକ୍ଷର *p = buf;

      ପାଇଁ (unsigned i = c->buffer->idx - ଗଣନା_2; i < c->buffer->idx; i++)
      {
	ଯଦି (buf < p && ଆକାର(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, ଆକାର(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (ସତ୍ୟ);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  ତାର୍କିକ serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (ଏହି);
    return_trace (substitute.serialize (c, subst));
  }

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    ଅଚଳ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ଅଚଳ hb_map_t &glyph_map = *c->plan->glyph_map;

    ଯଦି (!intersects (&glyphset)) return_trace (ମିଥ୍ୟା);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*ଏହି);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

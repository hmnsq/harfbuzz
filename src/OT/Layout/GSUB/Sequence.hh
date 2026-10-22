#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#அந்தர்பாவ "Common.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GSUB_impl {

template <typename Types>
கட்டமைப்பு Sequence
{
  பாதுகாக்கப்பட்ட:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  பொது:
  DEFINE_SIZE_ARRAY (2, substitute);

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (substitute.sanitize (c));
  }

  தருக்க intersects (மாறிலி hb_set_t *glyphs) மாறிலி
  { திரும்பு hb_all (substitute, glyphs); }

  வெறுமை closure (hb_closure_context_t *c) மாறிலி
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  வெறுமை depend (hb_depend_context_t *c, hb_codepoint_t source) மாறிலி
  {
    + hb_iter (substitute)
    | hb_apply ([&] (மாறிலி hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  வெறுமை collect_glyphs (hb_collect_glyphs_context_t *c) மாறிலி
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  தருக்க apply (hb_ot_apply_context_t *c) மாறிலி
  {
    TRACE_APPLY (இது);
    unsigned முழு மொத்தம் = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    எனில் (unlikely (மொத்தம் == 1))
    {
      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (உண்மை);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    இல்லையெனில் எனில் (unlikely (மொத்தம் == 0))
    {
      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (உண்மை);
    }

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned முழு klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      எனில் (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    எனில் (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      எழுத்து buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      எழுத்து *p = buf;

      க்கு (unsigned i = c->buffer->idx - மொத்தம்; i < c->buffer->idx; i++)
      {
	எனில் (buf < p && அளவு(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, அளவு(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (உண்மை);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  தருக்க serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (இது);
    return_trace (substitute.serialize (c, subst));
  }

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SUBSET (இது);
    மாறிலி hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    மாறிலி hb_map_t &glyph_map = *c->plan->glyph_map;

    எனில் (!intersects (&glyphset)) return_trace (பொய்);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*இது);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

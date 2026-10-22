#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#অন্তর্ভাৱ "Common.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GSUB_impl {

template <typename Types>
সংরচনা Sequence
{
  রক্ষিত:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  সার্ৱজনিক:
  DEFINE_SIZE_ARRAY (2, substitute);

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    return_trace (substitute.sanitize (c));
  }

  তার্কিক intersects (ধ্রুৱ hb_set_t *glyphs) ধ্রুৱ
  { ঘূৰাই hb_all (substitute, glyphs); }

  শূন্য closure (hb_closure_context_t *c) ধ্রুৱ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  শূন্য depend (hb_depend_context_t *c, hb_codepoint_t source) ধ্রুৱ
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ধ্রুৱ hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  শূন্য collect_glyphs (hb_collect_glyphs_context_t *c) ধ্রুৱ
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  তার্কিক apply (hb_ot_apply_context_t *c) ধ্রুৱ
  {
    TRACE_APPLY (এতত্);
    unsigned পূর্ণাঙ্ক গুনতি = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    যদি (unlikely (গুনতি == 1))
    {
      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (সঁচা);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    অন্যথা যদি (unlikely (গুনতি == 0))
    {
      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (সঁচা);
    }

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned পূর্ণাঙ্ক klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      যদি (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    যদি (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      ৱর্ণ buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      ৱর্ণ *p = buf;

      বাবে (unsigned i = c->buffer->idx - গুনতি; i < c->buffer->idx; i++)
      {
	যদি (buf < p && পরিমাণ(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, পরিমাণ(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (সঁচা);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  তার্কিক serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (এতত্);
    return_trace (substitute.serialize (c, subst));
  }

  তার্কিক subset (hb_subset_context_t *c) ধ্রুৱ
  {
    TRACE_SUBSET (এতত্);
    ধ্রুৱ hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ধ্রুৱ hb_map_t &glyph_map = *c->plan->glyph_map;

    যদি (!intersects (&glyphset)) return_trace (মিছা);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*এতত্);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

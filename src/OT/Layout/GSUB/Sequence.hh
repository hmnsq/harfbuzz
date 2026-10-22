#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#अन्तर्भाव "Common.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GSUB_impl {

ढाँचा <typename Types>
संरचना Sequence
{
  संरक्षित:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (2, substitute);

  तार्किक sanitize (hb_sanitize_context_t *c) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    return_trace (substitute.sanitize (c));
  }

  तार्किक intersects (स्थिरांक hb_set_t *glyphs) स्थिरांक
  { परत hb_all (substitute, glyphs); }

  शून्य closure (hb_closure_context_t *c) स्थिरांक
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  शून्य depend (hb_depend_context_t *c, hb_codepoint_t source) स्थिरांक
  {
    + hb_iter (substitute)
    | hb_apply ([&] (स्थिरांक hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  शून्य collect_glyphs (hb_collect_glyphs_context_t *c) स्थिरांक
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  तार्किक apply (hb_ot_apply_context_t *c) स्थिरांक
  {
    TRACE_APPLY (हे);
    अचिन्हित पूर्णांक मोजणी = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    जर (unlikely (मोजणी == 1))
    {
      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (सत्य);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    अन्यथा जर (unlikely (मोजणी == 0))
    {
      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (सत्य);
    }

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    अचिन्हित पूर्णांक klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    अचिन्हित lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      जर (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    जर (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      अक्षर buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      अक्षर *p = buf;

      साठी (अचिन्हित i = c->buffer->idx - मोजणी; i < c->buffer->idx; i++)
      {
	जर (buf < p && आकार(buf) - 1u > अचिन्हित (p - buf))
	  *p++ = ',';
	snprintf (p, आकार(buf) - (p - buf), "%u", i);
	p += म_माप(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (सत्य);
  }

  ढाँचा <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  तार्किक serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (हे);
    return_trace (substitute.serialize (c, subst));
  }

  तार्किक subset (hb_subset_context_t *c) स्थिरांक
  {
    TRACE_SUBSET (हे);
    स्थिरांक hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    स्थिरांक hb_map_t &glyph_map = *c->plan->glyph_map;

    जर (!intersects (&glyphset)) return_trace (असत्य);

    स्वतः it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    स्वतः *out = c->serializer->start_embed (*हे);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

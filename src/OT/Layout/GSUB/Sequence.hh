#ifndef OT_LAYOUT_GSUB_SEQUENCE_HH
#define OT_LAYOUT_GSUB_SEQUENCE_HH

#شامل "Common.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GSUB_impl {

template <typename Types>
ساختار Sequence
{
  محافظتشده:
  Array16Of<typename Types::HBGlyphID>
                substitute;             /* String of GlyphIDs to substitute */
  عمومی:
  DEFINE_SIZE_ARRAY (2, substitute);

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (substitute.sanitize (c));
  }

  منطقی intersects (ثابت hb_set_t *glyphs) ثابت
  { بازگشت hb_all (substitute, glyphs); }

  تهی closure (hb_closure_context_t *c) ثابت
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  تهی depend (hb_depend_context_t *c, hb_codepoint_t source) ثابت
  {
    + hb_iter (substitute)
    | hb_apply ([&] (ثابت hb_codepoint_t &target) { c->depend_data->add_gsub_lookup (source, c->lookup_index, target); })
    ;
  }

  تهی collect_glyphs (hb_collect_glyphs_context_t *c) ثابت
  { c->output->add_array (substitute.arrayZ, substitute.len); }

  منطقی apply (hb_ot_apply_context_t *c) ثابت
  {
    TRACE_APPLY (این);
    unsigned صحیح شمار = substitute.len;

    /* Special-case to make it in-place and not consider this
     * as a "multiplied" substitution. */
    اگر (unlikely (شمار == 1))
    {
      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "replacing glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->replace_glyph (substitute.arrayZ[0]);

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->message (c->font,
			    "replaced glyph at %u (multiple substitution)",
			    c->buffer->idx - 1u);
      }

      return_trace (درست);
    }
    /* Spec disallows this, but Uniscribe allows it.
     * https://github.com/harfbuzz/harfbuzz/issues/253 */
    وگرنه اگر (unlikely (شمار == 0))
    {
      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleting glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      c->buffer->delete_glyph ();

      اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
      {
	c->buffer->sync_so_far ();
	c->buffer->message (c->font,
			    "deleted glyph at %u (multiple substitution)",
			    c->buffer->idx);
      }

      return_trace (درست);
    }

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();
      c->buffer->message (c->font,
			  "multiplying glyph at %u",
			  c->buffer->idx);
    }

    unsigned صحیح klass = _hb_glyph_info_is_ligature (&c->buffer->cur()) ?
                         HB_OT_LAYOUT_GLYPH_PROPS_BASE_GLYPH : 0;
    unsigned lig_id = _hb_glyph_info_get_lig_id (&c->buffer->cur());

    برای (unsigned صحیح i = 0; i < شمار; i++)
    {
      /* If is attached to a ligature, don't disturb that.
       * https://github.com/harfbuzz/harfbuzz/issues/3069 */
      اگر (!lig_id)
        _hb_glyph_info_set_lig_props_for_component (&c->buffer->cur(), i);
      c->output_glyph_for_component (substitute.arrayZ[i], klass);
    }
    c->buffer->skip_glyph ();

    اگر (HB_BUFFER_MESSAGE_MORE && c->buffer->messaging ())
    {
      c->buffer->sync_so_far ();

      نویسه buf[HB_MAX_CONTEXT_LENGTH * 16] = {0};
      نویسه *p = buf;

      برای (unsigned i = c->buffer->idx - شمار; i < c->buffer->idx; i++)
      {
	اگر (buf < p && اندازه(buf) - 1u > unsigned (p - buf))
	  *p++ = ',';
	snprintf (p, اندازه(buf) - (p - buf), "%u", i);
	p += strlen(p);
      }

      c->buffer->message (c->font,
			  "multiplied glyphs at %s",
			  buf);
    }

    return_trace (درست);
  }

  template <typename Iterator,
            hb_requires (hb_is_source_of (Iterator, hb_codepoint_t))>
  منطقی serialize (hb_serialize_context_t *c,
                  Iterator subst)
  {
    TRACE_SERIALIZE (این);
    return_trace (substitute.serialize (c, subst));
  }

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    TRACE_SUBSET (این);
    ثابت hb_set_t &glyphset = *c->plan->glyphset_gsub ();
    ثابت hb_map_t &glyph_map = *c->plan->glyph_map;

    اگر (!intersects (&glyphset)) return_trace (نادرست);

    auto it =
    + hb_iter (substitute)
    | hb_map (glyph_map)
    ;

    auto *out = c->serializer->start_embed (*این);
    return_trace (out->serialize (c->serializer, it));
  }
};


}
}
}


#endif /* OT_LAYOUT_GSUB_SEQUENCE_HH */

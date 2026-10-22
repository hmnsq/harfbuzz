#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#അന്തര്ഭാവ "../../../hb-ot-layout-common.hh"
#അന്തര്ഭാവ "../../../hb-ot-layout-gsubgpos.hh"
#അന്തര്ഭാവ "Common.hh"
#അന്തര്ഭാവ "PosLookup.hh"

നാമാകാശ OT {

using Layout::GPOS_impl::PosLookup;

നാമാകാശ Layout {

സ്ഥിതം ശൂന്യം
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned പൂർണ്ണസംഖ്യ len,
                              unsigned പൂർണ്ണസംഖ്യ i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

ഘടന GPOS : GSUBGPOS
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  സ്ഥിരം PosLookup& get_lookup (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം
  { തിരികെ static_cast<സ്ഥിരം PosLookup &> (GSUBGPOS::get_lookup (i)); }

  സ്ഥിതം വരിയിൽ ശൂന്യം position_start (hb_font_t *font, hb_buffer_t *buffer);
  സ്ഥിതം വരിയിൽ ശൂന്യം position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  സ്ഥിതം വരിയിൽ ശൂന്യം position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  തർക്കം subset (hb_subset_context_t *c) സ്ഥിരം
  {
    hb_subset_layout_context_t l (c, tableTag);
    തിരികെ GSUBGPOS::subset<PosLookup> (&l);
  }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL തർക്കം is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) സ്ഥിരം;

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c) സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < GSUBGPOS::get_lookup_എണ്ണം (); i++)
    {
      എങ്കിൽ (!c->gpos_lookups->has (i)) തുടരുക;
      സ്ഥിരം PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  ശൂന്യം closure_lookups (hb_face_t      *face,
                        സ്ഥിരം hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) സ്ഥിരം
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  തരനിർവചനം GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


സ്ഥിതം ശൂന്യം
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned പൂർണ്ണസംഖ്യ len,
                              unsigned പൂർണ്ണസംഖ്യ i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  പൂർണ്ണസംഖ്യ chain = pos[i].attach_chain();
  പൂർണ്ണസംഖ്യ type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned പൂർണ്ണസംഖ്യ j = (പൂർണ്ണസംഖ്യ) i + chain;

  എങ്കിൽ (unlikely (j >= len))
    തിരികെ;

  എങ്കിൽ (unlikely (!nesting_level))
    തിരികെ;

  എങ്കിൽ (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  പ്രതിജ്ഞാ (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  എങ്കിൽ (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    അല്ലെങ്കിൽ
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  അല്ലെങ്കിൽ /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    എങ്കിൽ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    അല്ലെങ്കിൽ
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    എങ്കിൽ (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      എങ്കിൽ (HB_DIRECTION_IS_FORWARD (direction))
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      അല്ലെങ്കിൽ
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    അല്ലെങ്കിൽ // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      എങ്കിൽ (HB_DIRECTION_IS_FORWARD (direction))
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      അല്ലെങ്കിൽ
	വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

ശൂന്യം
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned പൂർണ്ണസംഖ്യ എണ്ണം = buffer->len;
  വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

ശൂന്യം
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

ശൂന്യം
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned പൂർണ്ണസംഖ്യ len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  എങ്കിൽ (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    എങ്കിൽ (HB_DIRECTION_IS_FORWARD (direction))
    {
      വേണ്ടി (unsigned i = 0; i < len; i++)
	എങ്കിൽ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } അല്ലെങ്കിൽ {
      വേണ്ടി (unsigned i = len; i-- > 0; )
	എങ്കിൽ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  എങ്കിൽ (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    വേണ്ടി (unsigned i = 0; i < len; i++)
      എങ്കിൽ (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((ഇരട്ട) pos[i].x_offset +
						      (ഇരട്ട) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

ഘടന GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#અન્તર્ભાવ "../../../hb-ot-layout-common.hh"
#અન્તર્ભાવ "../../../hb-ot-layout-gsubgpos.hh"
#અન્તર્ભાવ "Common.hh"
#અન્તર્ભાવ "PosLookup.hh"

નામાકાશ OT {

using Layout::GPOS_impl::PosLookup;

નામાકાશ Layout {

સ્થિર શૂન્ય
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned પૂર્ણાંક len,
                              unsigned પૂર્ણાંક i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

સંરચના GPOS : GSUBGPOS
{
  સ્થિર constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  અચલ PosLookup& get_lookup (unsigned પૂર્ણાંક i) અચલ
  { પાછા static_cast<અચલ PosLookup &> (GSUBGPOS::get_lookup (i)); }

  સ્થિર અંતઃરેખ શૂન્ય position_start (hb_font_t *font, hb_buffer_t *buffer);
  સ્થિર અંતઃરેખ શૂન્ય position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  સ્થિર અંતઃરેખ શૂન્ય position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  તાર્કિક subset (hb_subset_context_t *c) અચલ
  {
    hb_subset_layout_context_t l (c, tableTag);
    પાછા GSUBGPOS::subset<PosLookup> (&l);
  }

  તાર્કિક sanitize (hb_sanitize_context_t *c) અચલ
  {
    TRACE_SANITIZE (આ);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL તાર્કિક is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) અચલ;

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c) અચલ
  {
    માટે (unsigned i = 0; i < GSUBGPOS::get_lookup_ગણતરી (); i++)
    {
      જો (!c->gpos_lookups->has (i)) ચાલુ;
      અચલ PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  શૂન્ય closure_lookups (hb_face_t      *face,
                        અચલ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) અચલ
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  પ્રકારવ્યાખ્યા GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


સ્થિર શૂન્ય
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned પૂર્ણાંક len,
                              unsigned પૂર્ણાંક i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  પૂર્ણાંક chain = pos[i].attach_chain();
  પૂર્ણાંક type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned પૂર્ણાંક j = (પૂર્ણાંક) i + chain;

  જો (unlikely (j >= len))
    પાછા;

  જો (unlikely (!nesting_level))
    પાછા;

  જો (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  પ્રતિજ્ઞા (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  જો (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    જો (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    નહીંતર
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  નહીંતર /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    જો (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    નહીંતર
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    જો (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      જો (HB_DIRECTION_IS_FORWARD (direction))
	માટે (unsigned પૂર્ણાંક k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      નહીંતર
	માટે (unsigned પૂર્ણાંક k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    નહીંતર // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      જો (HB_DIRECTION_IS_FORWARD (direction))
	માટે (unsigned પૂર્ણાંક k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      નહીંતર
	માટે (unsigned પૂર્ણાંક k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

શૂન્ય
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned પૂર્ણાંક ગણતરી = buffer->len;
  માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

શૂન્ય
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

શૂન્ય
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned પૂર્ણાંક len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  જો (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    જો (HB_DIRECTION_IS_FORWARD (direction))
    {
      માટે (unsigned i = 0; i < len; i++)
	જો (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } નહીંતર {
      માટે (unsigned i = len; i-- > 0; )
	જો (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  જો (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    માટે (unsigned i = 0; i < len; i++)
      જો (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((દ્વિગુણ) pos[i].x_offset +
						      (દ્વિગુણ) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

સંરચના GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-common.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-gsubgpos.hh"
#ಅನ್ತರ್ಭಾವ "Common.hh"
#ಅನ್ತರ್ಭಾವ "PosLookup.hh"

ನಾಮಾಕಾಶ OT {

using Layout::GPOS_impl::PosLookup;

ನಾಮಾಕಾಶ Layout {

ಸ್ಥಿರ ಶೂನ್ಯ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned ಪೂರ್ಣಾಙ್ಕ len,
                              unsigned ಪೂರ್ಣಾಙ್ಕ i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

ಸಂರಚನಾ GPOS : GSUBGPOS
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  ಧ್ರುವ PosLookup& get_lookup (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ static_cast<ಧ್ರುವ PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ position_start (hb_font_t *font, hb_buffer_t *buffer);
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c) ಧ್ರುವ
  {
    hb_subset_layout_context_t l (c, tableTag);
    ಪ್ರತ್ಯಾಗಮ GSUBGPOS::subset<PosLookup> (&l);
  }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL ತಾರ್ಕಿಕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ಧ್ರುವ;

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c) ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < GSUBGPOS::get_lookup_ಎಣಿಕೆ (); i++)
    {
      ಯದಿ (!c->gpos_lookups->has (i)) ಅನುವರ್ತ;
      ಧ್ರುವ PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  ಶೂನ್ಯ closure_lookups (hb_face_t      *face,
                        ಧ್ರುವ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ಧ್ರುವ
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  ಪ್ರಕಾರನಿರ್ದೇಶ GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ಸ್ಥಿರ ಶೂನ್ಯ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned ಪೂರ್ಣಾಙ್ಕ len,
                              unsigned ಪೂರ್ಣಾಙ್ಕ i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  ಪೂರ್ಣಾಙ್ಕ chain = pos[i].attach_chain();
  ಪೂರ್ಣಾಙ್ಕ type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned ಪೂರ್ಣಾಙ್ಕ j = (ಪೂರ್ಣಾಙ್ಕ) i + chain;

  ಯದಿ (unlikely (j >= len))
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (unlikely (!nesting_level))
    ಪ್ರತ್ಯಾಗಮ;

  ಯದಿ (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  ಪ್ರತಿಜ್ಞಾ (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  ಯದಿ (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    ಅನ್ಯಥಾ
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  ಅನ್ಯಥಾ /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    ಯದಿ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    ಅನ್ಯಥಾ
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    ಯದಿ (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      ಯದಿ (HB_DIRECTION_IS_FORWARD (direction))
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      ಅನ್ಯಥಾ
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    ಅನ್ಯಥಾ // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      ಯದಿ (HB_DIRECTION_IS_FORWARD (direction))
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      ಅನ್ಯಥಾ
	ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

ಶೂನ್ಯ
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = buffer->len;
  ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

ಶೂನ್ಯ
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

ಶೂನ್ಯ
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned ಪೂರ್ಣಾಙ್ಕ len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  ಯದಿ (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    ಯದಿ (HB_DIRECTION_IS_FORWARD (direction))
    {
      ಪ್ರತಿ (unsigned i = 0; i < len; i++)
	ಯದಿ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } ಅನ್ಯಥಾ {
      ಪ್ರತಿ (unsigned i = len; i-- > 0; )
	ಯದಿ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  ಯದಿ (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    ಪ್ರತಿ (unsigned i = 0; i < len; i++)
      ಯದಿ (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((ದ್ವಿಗುಣ) pos[i].x_offset +
						      (ದ್ವಿಗುಣ) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

ಸಂರಚನಾ GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

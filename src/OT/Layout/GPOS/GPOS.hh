#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#அந்தர்பாவ "../../../hb-ot-layout-common.hh"
#அந்தர்பாவ "../../../hb-ot-layout-gsubgpos.hh"
#அந்தர்பாவ "Common.hh"
#அந்தர்பாவ "PosLookup.hh"

நாமாகாஶ OT {

using Layout::GPOS_impl::PosLookup;

நாமாகாஶ Layout {

நிலையான வெறுமை
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned முழு len,
                              unsigned முழு i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

கட்டமைப்பு GPOS : GSUBGPOS
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  மாறிலி PosLookup& get_lookup (unsigned முழு i) மாறிலி
  { திரும்பு static_cast<மாறிலி PosLookup &> (GSUBGPOS::get_lookup (i)); }

  நிலையான உள்வரி வெறுமை position_start (hb_font_t *font, hb_buffer_t *buffer);
  நிலையான உள்வரி வெறுமை position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  நிலையான உள்வரி வெறுமை position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  தருக்க subset (hb_subset_context_t *c) மாறிலி
  {
    hb_subset_layout_context_t l (c, tableTag);
    திரும்பு GSUBGPOS::subset<PosLookup> (&l);
  }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL தருக்க is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) மாறிலி;

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c) மாறிலி
  {
    க்கு (unsigned i = 0; i < GSUBGPOS::get_lookup_மொத்தம் (); i++)
    {
      எனில் (!c->gpos_lookups->has (i)) தொடர்;
      மாறிலி PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  வெறுமை closure_lookups (hb_face_t      *face,
                        மாறிலி hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) மாறிலி
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  வகைவரை GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


நிலையான வெறுமை
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned முழு len,
                              unsigned முழு i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  முழு chain = pos[i].attach_chain();
  முழு type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned முழு j = (முழு) i + chain;

  எனில் (unlikely (j >= len))
    திரும்பு;

  எனில் (unlikely (!nesting_level))
    திரும்பு;

  எனில் (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  ப்ரதிஜ்ஞா (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  எனில் (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    எனில் (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    இல்லையெனில்
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  இல்லையெனில் /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    எனில் (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    இல்லையெனில்
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    எனில் (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      எனில் (HB_DIRECTION_IS_FORWARD (direction))
	க்கு (unsigned முழு k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      இல்லையெனில்
	க்கு (unsigned முழு k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    இல்லையெனில் // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      எனில் (HB_DIRECTION_IS_FORWARD (direction))
	க்கு (unsigned முழு k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      இல்லையெனில்
	க்கு (unsigned முழு k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

வெறுமை
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned முழு மொத்தம் = buffer->len;
  க்கு (unsigned முழு i = 0; i < மொத்தம்; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

வெறுமை
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

வெறுமை
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned முழு len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  எனில் (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    எனில் (HB_DIRECTION_IS_FORWARD (direction))
    {
      க்கு (unsigned i = 0; i < len; i++)
	எனில் (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } இல்லையெனில் {
      க்கு (unsigned i = len; i-- > 0; )
	எனில் (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  எனில் (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    க்கு (unsigned i = 0; i < len; i++)
      எனில் (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((இரட்டை) pos[i].x_offset +
						      (இரட்டை) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

கட்டமைப்பு GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

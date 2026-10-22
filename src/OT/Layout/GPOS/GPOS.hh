#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-common.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-gsubgpos.hh"
#ଅନ୍ତର୍ଭାଵ "Common.hh"
#ଅନ୍ତର୍ଭାଵ "PosLookup.hh"

ନାମାକାଶ OT {

using Layout::GPOS_impl::PosLookup;

ନାମାକାଶ Layout {

ସ୍ଥିର ଶୂନ୍ୟ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

ସଂରଚନା GPOS : GSUBGPOS
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  ଅଚଳ PosLookup& get_lookup (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ
  { ଫେରନ୍ତୁ static_cast<ଅଚଳ PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ position_start (hb_font_t *font, hb_buffer_t *buffer);
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  ତାର୍କିକ subset (hb_subset_context_t *c) ଅଚଳ
  {
    hb_subset_layout_context_t l (c, tableTag);
    ଫେରନ୍ତୁ GSUBGPOS::subset<PosLookup> (&l);
  }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL ତାର୍କିକ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ଅଚଳ;

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c) ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < GSUBGPOS::get_lookup_ଗଣନା (); i++)
    {
      ଯଦି (!c->gpos_lookups->has (i)) ଜାରି;
      ଅଚଳ PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  ଶୂନ୍ୟ closure_lookups (hb_face_t      *face,
                        ଅଚଳ hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ଅଚଳ
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  ପ୍ରକାରବ୍ୟାଖ୍ୟା GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ସ୍ଥିର ଶୂନ୍ୟ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len,
                              unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା chain = pos[i].attach_chain();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) i + chain;

  ଯଦି (unlikely (j >= len))
    ଫେରନ୍ତୁ;

  ଯଦି (unlikely (!nesting_level))
    ଫେରନ୍ତୁ;

  ଯଦି (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  ପ୍ରତିଜ୍ଞା (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  ଯଦି (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    ଯଦି (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    ଅନ୍ୟଥା
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  ଅନ୍ୟଥା /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    ଯଦି (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    ଅନ୍ୟଥା
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    ଯଦି (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      ଯଦି (HB_DIRECTION_IS_FORWARD (direction))
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      ଅନ୍ୟଥା
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    ଅନ୍ୟଥା // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      ଯଦି (HB_DIRECTION_IS_FORWARD (direction))
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      ଅନ୍ୟଥା
	ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

ଶୂନ୍ୟ
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = buffer->len;
  ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

ଶୂନ୍ୟ
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

ଶୂନ୍ୟ
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  ଯଦି (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    ଯଦି (HB_DIRECTION_IS_FORWARD (direction))
    {
      ପାଇଁ (unsigned i = 0; i < len; i++)
	ଯଦି (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } ଅନ୍ୟଥା {
      ପାଇଁ (unsigned i = len; i-- > 0; )
	ଯଦି (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  ଯଦି (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    ପାଇଁ (unsigned i = 0; i < len; i++)
      ଯଦି (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((ଦ୍ୱିଗୁଣ) pos[i].x_offset +
						      (ଦ୍ୱିଗୁଣ) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

ସଂରଚନା GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

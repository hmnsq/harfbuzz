#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#অন্তর্ভাব "../../../hb-ot-layout-common.hh"
#অন্তর্ভাব "../../../hb-ot-layout-gsubgpos.hh"
#অন্তর্ভাব "Common.hh"
#অন্তর্ভাব "PosLookup.hh"

নামাকাশ OT {

using Layout::GPOS_impl::PosLookup;

নামাকাশ Layout {

স্থির শূন্য
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned পূর্ণাঙ্ক len,
                              unsigned পূর্ণাঙ্ক i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

সংরচনা GPOS : GSUBGPOS
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  ধ্রুব PosLookup& get_lookup (unsigned পূর্ণাঙ্ক i) ধ্রুব
  { প্রত্যাগম static_cast<ধ্রুব PosLookup &> (GSUBGPOS::get_lookup (i)); }

  স্থির অন্তঃস্থ শূন্য position_start (hb_font_t *font, hb_buffer_t *buffer);
  স্থির অন্তঃস্থ শূন্য position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  স্থির অন্তঃস্থ শূন্য position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  তার্কিক subset (hb_subset_context_t *c) ধ্রুব
  {
    hb_subset_layout_context_t l (c, tableTag);
    প্রত্যাগম GSUBGPOS::subset<PosLookup> (&l);
  }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL তার্কিক is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ধ্রুব;

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c) ধ্রুব
  {
    প্রতি (unsigned i = 0; i < GSUBGPOS::get_lookup_গুনতি (); i++)
    {
      যদি (!c->gpos_lookups->has (i)) অনুবর্ত;
      ধ্রুব PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  শূন্য closure_lookups (hb_face_t      *face,
                        ধ্রুব hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ধ্রুব
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  প্রকারনির্দেশ GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


স্থির শূন্য
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned পূর্ণাঙ্ক len,
                              unsigned পূর্ণাঙ্ক i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  পূর্ণাঙ্ক chain = pos[i].attach_chain();
  পূর্ণাঙ্ক type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned পূর্ণাঙ্ক j = (পূর্ণাঙ্ক) i + chain;

  যদি (unlikely (j >= len))
    প্রত্যাগম;

  যদি (unlikely (!nesting_level))
    প্রত্যাগম;

  যদি (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  প্রতিজ্ঞা (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  যদি (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    যদি (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    অন্যথা
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  অন্যথা /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    যদি (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    অন্যথা
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    যদি (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      যদি (HB_DIRECTION_IS_FORWARD (direction))
	প্রতি (unsigned পূর্ণাঙ্ক k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      অন্যথা
	প্রতি (unsigned পূর্ণাঙ্ক k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    অন্যথা // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      যদি (HB_DIRECTION_IS_FORWARD (direction))
	প্রতি (unsigned পূর্ণাঙ্ক k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      অন্যথা
	প্রতি (unsigned পূর্ণাঙ্ক k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

শূন্য
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned পূর্ণাঙ্ক গুনতি = buffer->len;
  প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

শূন্য
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

শূন্য
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned পূর্ণাঙ্ক len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  যদি (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    যদি (HB_DIRECTION_IS_FORWARD (direction))
    {
      প্রতি (unsigned i = 0; i < len; i++)
	যদি (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } অন্যথা {
      প্রতি (unsigned i = len; i-- > 0; )
	যদি (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  যদি (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    প্রতি (unsigned i = 0; i < len; i++)
      যদি (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((দ্বিগুণ) pos[i].x_offset +
						      (দ্বিগুণ) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

সংরচনা GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

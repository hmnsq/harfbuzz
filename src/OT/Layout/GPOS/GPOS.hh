#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#अन्तर्भाव "../../../hb-ot-layout-common.hh"
#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"
#अन्तर्भाव "Common.hh"
#अन्तर्भाव "PosLookup.hh"

नामाकाश OT {

using Layout::GPOS_impl::PosLookup;

नामाकाश Layout {

स्थिर शून्य
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned पूर्णाङ्क len,
                              unsigned पूर्णाङ्क i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

संरचना GPOS : GSUBGPOS
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  ध्रुव PosLookup& get_lookup (unsigned पूर्णाङ्क i) ध्रुव
  { प्रत्यागम static_cast<ध्रुव PosLookup &> (GSUBGPOS::get_lookup (i)); }

  स्थिर अन्तःस्थ शून्य position_start (hb_font_t *font, hb_buffer_t *buffer);
  स्थिर अन्तःस्थ शून्य position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  स्थिर अन्तःस्थ शून्य position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  तार्किक subset (hb_subset_context_t *c) ध्रुव
  {
    hb_subset_layout_context_t l (c, tableTag);
    प्रत्यागम GSUBGPOS::subset<PosLookup> (&l);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ध्रुव;

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) ध्रुव
  {
    प्रति (unsigned i = 0; i < GSUBGPOS::get_lookup_गणनम् (); i++)
    {
      जइ (!c->gpos_lookups->has (i)) अनुवर्त;
      ध्रुव PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  शून्य closure_lookups (hb_face_t      *face,
                        ध्रुव hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ध्रुव
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  प्रकारनिर्देश GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


स्थिर शून्य
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned पूर्णाङ्क len,
                              unsigned पूर्णाङ्क i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  पूर्णाङ्क chain = pos[i].attach_chain();
  पूर्णाङ्क type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned पूर्णाङ्क j = (पूर्णाङ्क) i + chain;

  जइ (unlikely (j >= len))
    प्रत्यागम;

  जइ (unlikely (!nesting_level))
    प्रत्यागम;

  जइ (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  प्रतिज्ञा (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  जइ (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    जइ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    अण्णहा
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  अण्णहा /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    जइ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    अण्णहा
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    जइ (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      जइ (HB_DIRECTION_IS_FORWARD (direction))
	प्रति (unsigned पूर्णाङ्क k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      अण्णहा
	प्रति (unsigned पूर्णाङ्क k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    अण्णहा // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      जइ (HB_DIRECTION_IS_FORWARD (direction))
	प्रति (unsigned पूर्णाङ्क k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      अण्णहा
	प्रति (unsigned पूर्णाङ्क k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

शून्य
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned पूर्णाङ्क गणनम् = buffer->len;
  प्रति (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

शून्य
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

शून्य
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned पूर्णाङ्क len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  जइ (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    जइ (HB_DIRECTION_IS_FORWARD (direction))
    {
      प्रति (unsigned i = 0; i < len; i++)
	जइ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } अण्णहा {
      प्रति (unsigned i = len; i-- > 0; )
	जइ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  जइ (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    प्रति (unsigned i = 0; i < len; i++)
      जइ (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((द्विगुण) pos[i].x_offset +
						      (द्विगुण) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

संरचना GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

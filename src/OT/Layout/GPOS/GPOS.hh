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
                              अचिन्हित पूर्णाङ्क len,
                              अचिन्हित पूर्णाङ्क i,
                              hb_direction_t direction,
                              अचिन्हित nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

संरचना GPOS : GSUBGPOS
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  अचर PosLookup& get_lookup (अचिन्हित पूर्णाङ्क i) अचर
  { फर्काऊ static_cast<अचर PosLookup &> (GSUBGPOS::get_lookup (i)); }

  स्थिर अन्तःस्थ शून्य position_start (hb_font_t *font, hb_buffer_t *buffer);
  स्थिर अन्तःस्थ शून्य position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  स्थिर अन्तःस्थ शून्य position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  तार्किक subset (hb_subset_context_t *c) अचर
  {
    hb_subset_layout_context_t l (c, tableTag);
    फर्काऊ GSUBGPOS::subset<PosLookup> (&l);
  }

  तार्किक sanitize (hb_sanitize_context_t *c) अचर
  {
    TRACE_SANITIZE (एतत्);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL तार्किक is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) अचर;

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c) अचर
  {
    लागि (अचिन्हित i = 0; i < GSUBGPOS::get_lookup_गन्ती (); i++)
    {
      यदि (!c->gpos_lookups->has (i)) जारी;
      अचर PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  शून्य closure_lookups (hb_face_t      *face,
                        अचर hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) अचर
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  प्रकारनिर्देश GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


स्थिर शून्य
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              अचिन्हित पूर्णाङ्क len,
                              अचिन्हित पूर्णाङ्क i,
                              hb_direction_t direction,
                              अचिन्हित nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  पूर्णाङ्क chain = pos[i].attach_chain();
  पूर्णाङ्क type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  अचिन्हित पूर्णाङ्क j = (पूर्णाङ्क) i + chain;

  यदि (unlikely (j >= len))
    फर्काऊ;

  यदि (unlikely (!nesting_level))
    फर्काऊ;

  यदि (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  प्रतिज्ञा (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  यदि (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    यदि (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    अन्यथा
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  अन्यथा /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    यदि (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    अन्यथा
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    यदि (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      यदि (HB_DIRECTION_IS_FORWARD (direction))
	लागि (अचिन्हित पूर्णाङ्क k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      अन्यथा
	लागि (अचिन्हित पूर्णाङ्क k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    अन्यथा // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      यदि (HB_DIRECTION_IS_FORWARD (direction))
	लागि (अचिन्हित पूर्णाङ्क k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      अन्यथा
	लागि (अचिन्हित पूर्णाङ्क k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

शून्य
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  अचिन्हित पूर्णाङ्क गन्ती = buffer->len;
  लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++)
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

  अचिन्हित पूर्णाङ्क len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  यदि (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    स्वतः *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    यदि (HB_DIRECTION_IS_FORWARD (direction))
    {
      लागि (अचिन्हित i = 0; i < len; i++)
	यदि (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } अन्यथा {
      लागि (अचिन्हित i = len; i-- > 0; )
	यदि (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  यदि (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    लागि (अचिन्हित i = 0; i < len; i++)
      यदि (unlikely (pos[i].y_offset))
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

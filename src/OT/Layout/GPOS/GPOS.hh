#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "PosLookup.hh"

نطاق OT {

using Layout::GPOS_impl::PosLookup;

نطاق Layout {

ساكن فراغ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned صحيح len,
                              unsigned صحيح i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

بنية GPOS : GSUBGPOS
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  ثابت PosLookup& get_lookup (unsigned صحيح i) ثابت
  { أرجع static_cast<ثابت PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ساكن مضمن فراغ position_start (hb_font_t *font, hb_buffer_t *buffer);
  ساكن مضمن فراغ position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  ساكن مضمن فراغ position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, tableTag);
    أرجع GSUBGPOS::subset<PosLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (unsigned i = 0; i < GSUBGPOS::get_lookup_ژماردن (); i++)
    {
      إذا (!c->gpos_lookups->has (i)) استمر;
      ثابت PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  فراغ closure_lookups (hb_face_t      *face,
                        ثابت hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ساكن فراغ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned صحيح len,
                              unsigned صحيح i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  صحيح chain = pos[i].attach_chain();
  صحيح type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned صحيح j = (صحيح) i + chain;

  إذا (unlikely (j >= len))
    أرجع;

  إذا (unlikely (!nesting_level))
    أرجع;

  إذا (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  أكد (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  إذا (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    وإلا
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  وإلا /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    وإلا
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    إذا (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      إذا (HB_DIRECTION_IS_FORWARD (direction))
	لكل (unsigned صحيح k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      وإلا
	لكل (unsigned صحيح k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    وإلا // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      إذا (HB_DIRECTION_IS_FORWARD (direction))
	لكل (unsigned صحيح k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      وإلا
	لكل (unsigned صحيح k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

فراغ
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned صحيح ژماردن = buffer->len;
  لكل (unsigned صحيح i = 0; i < ژماردن; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

فراغ
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

فراغ
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned صحيح len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  إذا (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    إذا (HB_DIRECTION_IS_FORWARD (direction))
    {
      لكل (unsigned i = 0; i < len; i++)
	إذا (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } وإلا {
      لكل (unsigned i = len; i-- > 0; )
	إذا (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  إذا (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    لكل (unsigned i = 0; i < len; i++)
      إذا (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((مزدوج) pos[i].x_offset +
						      (مزدوج) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

بنية GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

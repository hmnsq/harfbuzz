#ifndef OT_LAYOUT_GPOS_GPOS_HH
#define OT_LAYOUT_GPOS_GPOS_HH

#شامل "../../../hb-ot-layout-common.hh"
#شامل "../../../hb-ot-layout-gsubgpos.hh"
#شامل "Common.hh"
#شامل "PosLookup.hh"

فضاینام OT {

using Layout::GPOS_impl::PosLookup;

فضاینام Layout {

ایستا تهی
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned صحیح len,
                              unsigned صحیح i,
                              hb_direction_t direction,
                              unsigned nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

ساختار GPOS : GSUBGPOS
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_GPOS;

  using Lookup = PosLookup;

  ثابت PosLookup& get_lookup (unsigned صحیح i) ثابت
  { بازگشت static_cast<ثابت PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ایستا درونخطی تهی position_start (hb_font_t *font, hb_buffer_t *buffer);
  ایستا درونخطی تهی position_finish_advances (hb_font_t *font, hb_buffer_t *buffer);
  ایستا درونخطی تهی position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer);

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, tableTag);
    بازگشت GSUBGPOS::subset<PosLookup> (&l);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_INTERNAL منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برای (unsigned i = 0; i < GSUBGPOS::get_lookup_شمار (); i++)
    {
      اگر (!c->gpos_lookups->has (i)) ادامه;
      ثابت PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  تهی closure_lookups (hb_face_t      *face,
                        ثابت hb_set_t *glyphs,
                        hb_set_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::closure_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  تعریفنوع GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ایستا تهی
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              unsigned صحیح len,
                              unsigned صحیح i,
                              hb_direction_t direction,
                              unsigned nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  صحیح chain = pos[i].attach_chain();
  صحیح type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  unsigned صحیح j = (صحیح) i + chain;

  اگر (unlikely (j >= len))
    بازگشت;

  اگر (unlikely (!nesting_level))
    بازگشت;

  اگر (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  أكد (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  اگر (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);
    وگرنه
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
  }
  وگرنه /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[j].x_offset);
    وگرنه
      pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[j].y_offset);

    // i is the position of the mark; j is the base.
    اگر (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      اگر (HB_DIRECTION_IS_FORWARD (direction))
	برای (unsigned صحیح k = j; k < i; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
      وگرنه
	برای (unsigned صحیح k = j + 1; k < i + 1; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
    }
    وگرنه // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      اگر (HB_DIRECTION_IS_FORWARD (direction))
	برای (unsigned صحیح k = i; k < j; k++) {
	  pos[i].x_offset = hb_saturate_add (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_add (pos[i].y_offset, pos[k].y_advance);
	}
      وگرنه
	برای (unsigned صحیح k = i + 1; k < j + 1; k++) {
	  pos[i].x_offset = hb_saturate_sub (pos[i].x_offset, pos[k].x_advance);
	  pos[i].y_offset = hb_saturate_sub (pos[i].y_offset, pos[k].y_advance);
	}
    }
  }
}

تهی
GPOS::position_start (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer)
{
  unsigned صحیح شمار = buffer->len;
  برای (unsigned صحیح i = 0; i < شمار; i++)
    buffer->pos[i].attach_chain() = buffer->pos[i].attach_type() = 0;
}

تهی
GPOS::position_finish_advances (hb_font_t *font HB_UNUSED, hb_buffer_t *buffer HB_UNUSED)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

تهی
GPOS::position_finish_offsets (hb_font_t *font, hb_buffer_t *buffer)
{
  _hb_buffer_assert_gsubgpos_vars (buffer);

  unsigned صحیح len;
  hb_glyph_position_t *pos = hb_buffer_get_glyph_positions (buffer, &len);
  hb_direction_t direction = buffer->props.direction;

  /* Handle attachments */
  اگر (buffer->scratch_flags & HB_BUFFER_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    auto *pos = buffer->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    اگر (HB_DIRECTION_IS_FORWARD (direction))
    {
      برای (unsigned i = 0; i < len; i++)
	اگر (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } وگرنه {
      برای (unsigned i = len; i-- > 0; )
	اگر (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  اگر (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    برای (unsigned i = 0; i < len; i++)
      اگر (unlikely (pos[i].y_offset))
        pos[i].x_offset = hb_clamp_to<hb_position_t> ((دوگانه) pos[i].x_offset +
						      (دوگانه) roundf (font->slant_xy * pos[i].y_offset));
  }
}

}

ساختار GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#endif  /* OT_LAYOUT_GPOS_GPOS_HH */

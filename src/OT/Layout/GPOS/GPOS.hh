#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_GPOS_HH
#تعریف_کرو OT_LAYOUT_GPOS_GPOS_HH

#شامل "../../../hb-ot-layout-common.hh"
#شامل "../../../hb-ot-layout-gsubgpos.hh"
#شامل "Common.hh"
#شامل "PosLookup.hh"

نامخلا OT {

استعمال Layout::GPOS_impl::PosLookup;

نامخلا Layout {

ساکن خالی
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              غیرعلامتی صحیح len,
                              غیرعلامتی صحیح i,
                              hb_direction_t direction,
                              غیرعلامتی nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

ساخت GPOS : GSUBGPOS
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_GPOS;

  استعمال Lookup = PosLookup;

  ثابت PosLookup& get_lookup (غیرعلامتی صحیح i) ثابت
  { واپس static_cast<ثابت PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ساکن اندرونی خالی position_آغاز (hb_font_t *font, hb_بفر_t *بفر);
  ساکن اندرونی خالی position_اختتام_advances (hb_font_t *font, hb_بفر_t *بفر);
  ساکن اندرونی خالی position_اختتام_offsets (hb_font_t *font, hb_بفر_t *بفر);

  منطقی subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_لیبل);
    واپس GSUBGPOS::subset<PosLookup> (&l);
  }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_اندرونی منطقی is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    برائے (غیرعلامتی i = 0; i < GSUBGPOS::get_lookup_گنتی (); i++)
    {
      اگر (!c->gpos_lookups->has (i)) جاری;
      ثابت PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  خالی احاطہ_lookups (hb_face_t      *face,
                        ثابت hb_سیٹ_t *glyphs,
                        hb_سیٹ_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::احاطہ_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  قسمتعریف GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ساکن خالی
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              غیرعلامتی صحیح len,
                              غیرعلامتی صحیح i,
                              hb_direction_t direction,
                              غیرعلامتی nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  صحیح chain = pos[i].attach_chain();
  صحیح type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  غیرعلامتی صحیح j = (صحیح) i + chain;

  اگر (unlikely (j >= len))
    واپس;

  اگر (unlikely (!nesting_level))
    واپس;

  اگر (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  أكد (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  اگر (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_انحراف_2 = hb_saturate_جوڑو (pos[i].y_انحراف_2, pos[j].y_انحراف_2);
    ورنہ
      pos[i].x_انحراف_2 = hb_saturate_جوڑو (pos[i].x_انحراف_2, pos[j].x_انحراف_2);
  }
  ورنہ /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    اگر (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_انحراف_2 = hb_saturate_جوڑو (pos[i].x_انحراف_2, pos[j].x_انحراف_2);
    ورنہ
      pos[i].y_انحراف_2 = hb_saturate_جوڑو (pos[i].y_انحراف_2, pos[j].y_انحراف_2);

    // i is the position of the mark; j is the base.
    اگر (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      اگر (HB_DIRECTION_IS_FORWARD (direction))
	برائے (غیرعلامتی صحیح k = j; k < i; k++) {
	  pos[i].x_انحراف_2 = hb_saturate_sub (pos[i].x_انحراف_2, pos[k].x_advance);
	  pos[i].y_انحراف_2 = hb_saturate_sub (pos[i].y_انحراف_2, pos[k].y_advance);
	}
      ورنہ
	برائے (غیرعلامتی صحیح k = j + 1; k < i + 1; k++) {
	  pos[i].x_انحراف_2 = hb_saturate_جوڑو (pos[i].x_انحراف_2, pos[k].x_advance);
	  pos[i].y_انحراف_2 = hb_saturate_جوڑو (pos[i].y_انحراف_2, pos[k].y_advance);
	}
    }
    ورنہ // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      اگر (HB_DIRECTION_IS_FORWARD (direction))
	برائے (غیرعلامتی صحیح k = i; k < j; k++) {
	  pos[i].x_انحراف_2 = hb_saturate_جوڑو (pos[i].x_انحراف_2, pos[k].x_advance);
	  pos[i].y_انحراف_2 = hb_saturate_جوڑو (pos[i].y_انحراف_2, pos[k].y_advance);
	}
      ورنہ
	برائے (غیرعلامتی صحیح k = i + 1; k < j + 1; k++) {
	  pos[i].x_انحراف_2 = hb_saturate_sub (pos[i].x_انحراف_2, pos[k].x_advance);
	  pos[i].y_انحراف_2 = hb_saturate_sub (pos[i].y_انحراف_2, pos[k].y_advance);
	}
    }
  }
}

خالی
GPOS::position_آغاز (hb_font_t *font HB_غیر_مستعمل, hb_بفر_t *بفر)
{
  غیرعلامتی صحیح گنتی = بفر->len;
  برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++)
    بفر->pos[i].attach_chain() = بفر->pos[i].attach_type() = 0;
}

خالی
GPOS::position_اختتام_advances (hb_font_t *font HB_غیر_مستعمل, hb_بفر_t *بفر HB_غیر_مستعمل)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

خالی
GPOS::position_اختتام_offsets (hb_font_t *font, hb_بفر_t *بفر)
{
  hb_بفر_assert_gsubgpos_vars (بفر);

  غیرعلامتی صحیح len;
  hb_glyph_position_t *pos = hb_بفر_get_glyph_positions (بفر, &len);
  hb_direction_t direction = بفر->props.direction;

  /* Handle attachments */
  اگر (بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    خودکار *pos = بفر->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    اگر (HB_DIRECTION_IS_FORWARD (direction))
    {
      برائے (غیرعلامتی i = 0; i < len; i++)
	اگر (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } ورنہ {
      برائے (غیرعلامتی i = len; i-- > 0; )
	اگر (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  اگر (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    برائے (غیرعلامتی i = 0; i < len; i++)
      اگر (unlikely (pos[i].y_انحراف_2))
        pos[i].x_انحراف_2 = hb_clamp_مقصد<hb_position_t> ((دوہرا) pos[i].x_انحراف_2 +
						      (دوہرا) roundf (font->slant_xy * pos[i].y_انحراف_2));
  }
}

}

ساخت GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#اگر_ختم  /* OT_LAYOUT_GPOS_GPOS_HH */

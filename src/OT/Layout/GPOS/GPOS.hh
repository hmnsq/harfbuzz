#که_نه_تعریف_شوی OT_LAYOUT_GPOS_GPOS_HH
#تعریف_کړه OT_LAYOUT_GPOS_GPOS_HH

#ضمن "../../../hb-ot-layout-common.hh"
#ضمن "../../../hb-ot-layout-gsubgpos.hh"
#ضمن "Common.hh"
#ضمن "PosLookup.hh"

نطاق OT {

کارول Layout::GPOS_impl::PosLookup;

نطاق Layout {

ساكن فراغ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              بینښه صحيح len,
                              بینښه صحيح i,
                              hb_direction_t direction,
                              بینښه nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

بنية GPOS : GSUBGPOS
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_GPOS;

  کارول Lookup = PosLookup;

  ثابت PosLookup& get_lookup (بینښه صحيح i) ثابت
  { أرجع static_cast<ثابت PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ساكن مضمن فراغ position_پیل (hb_font_t *font, hb_بفر_t *بفر);
  ساكن مضمن فراغ position_پای_advances (hb_font_t *font, hb_بفر_t *بفر);
  ساكن مضمن فراغ position_پای_offsets (hb_font_t *font, hb_بفر_t *بفر);

  منطقي subset (hb_subset_context_t *c) ثابت
  {
    hb_subset_layout_context_t l (c, جدول_نښان);
    أرجع GSUBGPOS::subset<PosLookup> (&l);
  }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_دننی منطقي is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ثابت;

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c) ثابت
  {
    لكل (بینښه i = 0; i < GSUBGPOS::get_lookup_شمېر (); i++)
    {
      إذا (!c->gpos_lookups->has (i)) استمر;
      ثابت PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  فراغ تړون_lookups (hb_face_t      *face,
                        ثابت hb_سېټ_t *glyphs,
                        hb_سېټ_t       *lookup_indexes /* IN/OUT */) ثابت
  { GSUBGPOS::تړون_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  تعريفنوع GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ساكن فراغ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              بینښه صحيح len,
                              بینښه صحيح i,
                              hb_direction_t direction,
                              بینښه nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  صحيح chain = pos[i].attach_chain();
  صحيح type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  بینښه صحيح j = (صحيح) i + chain;

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
      pos[i].y_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].y_دننه_واټن_2, pos[j].y_دننه_واټن_2);
    وإلا
      pos[i].x_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].x_دننه_واټن_2, pos[j].x_دننه_واټن_2);
  }
  وإلا /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    إذا (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].x_دننه_واټن_2, pos[j].x_دننه_واټن_2);
    وإلا
      pos[i].y_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].y_دننه_واټن_2, pos[j].y_دننه_واټن_2);

    // i is the position of the mark; j is the base.
    إذا (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      إذا (HB_DIRECTION_IS_FORWARD (direction))
	لكل (بینښه صحيح k = j; k < i; k++) {
	  pos[i].x_دننه_واټن_2 = hb_saturate_sub (pos[i].x_دننه_واټن_2, pos[k].x_advance);
	  pos[i].y_دننه_واټن_2 = hb_saturate_sub (pos[i].y_دننه_واټن_2, pos[k].y_advance);
	}
      وإلا
	لكل (بینښه صحيح k = j + 1; k < i + 1; k++) {
	  pos[i].x_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].x_دننه_واټن_2, pos[k].x_advance);
	  pos[i].y_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].y_دننه_واټن_2, pos[k].y_advance);
	}
    }
    وإلا // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      إذا (HB_DIRECTION_IS_FORWARD (direction))
	لكل (بینښه صحيح k = i; k < j; k++) {
	  pos[i].x_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].x_دننه_واټن_2, pos[k].x_advance);
	  pos[i].y_دننه_واټن_2 = hb_saturate_جمع_کړه (pos[i].y_دننه_واټن_2, pos[k].y_advance);
	}
      وإلا
	لكل (بینښه صحيح k = i + 1; k < j + 1; k++) {
	  pos[i].x_دننه_واټن_2 = hb_saturate_sub (pos[i].x_دننه_واټن_2, pos[k].x_advance);
	  pos[i].y_دننه_واټن_2 = hb_saturate_sub (pos[i].y_دننه_واټن_2, pos[k].y_advance);
	}
    }
  }
}

فراغ
GPOS::position_پیل (hb_font_t *font HB_ناکارول_شوی, hb_بفر_t *بفر)
{
  بینښه صحيح شمېر = بفر->len;
  لكل (بینښه صحيح i = 0; i < شمېر; i++)
    بفر->pos[i].attach_chain() = بفر->pos[i].attach_type() = 0;
}

فراغ
GPOS::position_پای_advances (hb_font_t *font HB_ناکارول_شوی, hb_بفر_t *بفر HB_ناکارول_شوی)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

فراغ
GPOS::position_پای_offsets (hb_font_t *font, hb_بفر_t *بفر)
{
  hb_بفر_assert_gsubgpos_vars (بفر);

  بینښه صحيح len;
  hb_glyph_position_t *pos = hb_بفر_get_glyph_positions (بفر, &len);
  hb_direction_t direction = بفر->props.direction;

  /* Handle attachments */
  إذا (بفر->scratch_flags & HB_بفر_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    خپلکاری *pos = بفر->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    إذا (HB_DIRECTION_IS_FORWARD (direction))
    {
      لكل (بینښه i = 0; i < len; i++)
	إذا (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } وإلا {
      لكل (بینښه i = len; i-- > 0; )
	إذا (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  إذا (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    لكل (بینښه i = 0; i < len; i++)
      إذا (unlikely (pos[i].y_دننه_واټن_2))
        pos[i].x_دننه_واټن_2 = hb_clamp_منزل<hb_position_t> ((مزدوج) pos[i].x_دننه_واټن_2 +
						      (مزدوج) roundf (font->slant_xy * pos[i].y_دننه_واټن_2));
  }
}

}

بنية GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#د_که_پای  /* OT_LAYOUT_GPOS_GPOS_HH */

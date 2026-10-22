#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_GPOS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_GPOS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-gsubgpos.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "Common.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "PosLookup.hh"

ਨਾਮਾਕਾਸ਼ OT {

ਵਰਤੋਂ Layout::GPOS_impl::PosLookup;

ਨਾਮਾਕਾਸ਼ Layout {

ਸਥਿਰ ਖਾਲੀ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,
                              hb_direction_t direction,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level = HB_MAX_NESTING_LEVEL);

/*
 * GPOS -- Glyph Positioning
 * https://docs.microsoft.com/en-us/typography/opentype/spec/gpos
 */

ਬਣਤਰ GPOS : GSUBGPOS
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_GPOS;

  ਵਰਤੋਂ Lookup = PosLookup;

  ਅਚਲ PosLookup& get_lookup (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ
  { ਵਾਪਸ static_cast<ਅਚਲ PosLookup &> (GSUBGPOS::get_lookup (i)); }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ position_ਸ਼ੁਰੂ (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ);
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ position_ਅੰਤ_advances (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ);
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ position_ਅੰਤ_offsets (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ);

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c) ਅਚਲ
  {
    hb_subset_layout_context_t l (c, ਸਾਰਣੀ_ਟੈਗ);
    ਵਾਪਸ GSUBGPOS::subset<PosLookup> (&l);
  }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (GSUBGPOS::sanitize<PosLookup> (c));
  }

  HB_ਅੰਦਰੂਨੀ ਤਾਰਕਿਕ is_blocklisted (hb_blob_t *blob,
                                   hb_face_t *face) ਅਚਲ;

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c) ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < GSUBGPOS::get_lookup_ਗਿਣਤੀ (); i++)
    {
      ਜੇ (!c->gpos_lookups->has (i)) ਜਾਰੀ;
      ਅਚਲ PosLookup &l = get_lookup (i);
      l.dispatch (c);
    }
  }

  ਖਾਲੀ ਘੇਰਾ_lookups (hb_face_t      *face,
                        ਅਚਲ hb_ਸੈੱਟ_t *glyphs,
                        hb_ਸੈੱਟ_t       *lookup_indexes /* IN/OUT */) ਅਚਲ
  { GSUBGPOS::ਘੇਰਾ_lookups<PosLookup> (face, glyphs, lookup_indexes); }

  ਕਿਸਮਪਰਿਭਾਸ਼ਾ GSUBGPOS::accelerator_t<GPOS> accelerator_t;
};


ਸਥਿਰ ਖਾਲੀ
propagate_attachment_offsets (hb_glyph_position_t *pos,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i,
                              hb_direction_t direction,
                              ਬਿਨਾਨਿਸ਼ਾਨੀ nesting_level)
{
  /* Adjusts offsets of attached glyphs (both cursive and mark) to accumulate
   * offset of glyph they are attached to. */
  ਪੂਰਨ chain = pos[i].attach_chain();
  ਪੂਰਨ type = pos[i].attach_type();

  pos[i].attach_chain() = 0;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = (ਪੂਰਨ) i + chain;

  ਜੇ (unlikely (j >= len))
    ਵਾਪਸ;

  ਜੇ (unlikely (!nesting_level))
    ਵਾਪਸ;

  ਜੇ (pos[j].attach_chain())
    propagate_attachment_offsets (pos, len, j, direction, nesting_level - 1);

  ਪ੍ਰਤਿਜ੍ਞਾ (!!(type & GPOS_impl::ATTACH_TYPE_MARK) ^ !!(type & GPOS_impl::ATTACH_TYPE_CURSIVE));

  ਜੇ (type & GPOS_impl::ATTACH_TYPE_CURSIVE)
  {
    ਜੇ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].y_ਵਿੱਥ_2, pos[j].y_ਵਿੱਥ_2);
    ਨਹੀਂਤਾਂ
      pos[i].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].x_ਵਿੱਥ_2, pos[j].x_ਵਿੱਥ_2);
  }
  ਨਹੀਂਤਾਂ /*if (type & GPOS_impl::ATTACH_TYPE_MARK)*/
  {
    ਜੇ (HB_DIRECTION_IS_HORIZONTAL (direction))
      pos[i].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].x_ਵਿੱਥ_2, pos[j].x_ਵਿੱਥ_2);
    ਨਹੀਂਤਾਂ
      pos[i].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].y_ਵਿੱਥ_2, pos[j].y_ਵਿੱਥ_2);

    // i is the position of the mark; j is the base.
    ਜੇ (j < i)
    {
      /* This is the common case: mark follows base.
       * And currently the only way in OpenType. */
      ਜੇ (HB_DIRECTION_IS_FORWARD (direction))
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = j; k < i; k++) {
	  pos[i].x_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].x_ਵਿੱਥ_2, pos[k].x_advance);
	  pos[i].y_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].y_ਵਿੱਥ_2, pos[k].y_advance);
	}
      ਨਹੀਂਤਾਂ
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = j + 1; k < i + 1; k++) {
	  pos[i].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].x_ਵਿੱਥ_2, pos[k].x_advance);
	  pos[i].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].y_ਵਿੱਥ_2, pos[k].y_advance);
	}
    }
    ਨਹੀਂਤਾਂ // j > i
    {
      /* This can happen with `kerx`: a mark attaching
       * to a base after it in the logical order. */
      ਜੇ (HB_DIRECTION_IS_FORWARD (direction))
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = i; k < j; k++) {
	  pos[i].x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].x_ਵਿੱਥ_2, pos[k].x_advance);
	  pos[i].y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (pos[i].y_ਵਿੱਥ_2, pos[k].y_advance);
	}
      ਨਹੀਂਤਾਂ
	ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ k = i + 1; k < j + 1; k++) {
	  pos[i].x_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].x_ਵਿੱਥ_2, pos[k].x_advance);
	  pos[i].y_ਵਿੱਥ_2 = hb_saturate_sub (pos[i].y_ਵਿੱਥ_2, pos[k].y_advance);
	}
    }
  }
}

ਖਾਲੀ
GPOS::position_ਸ਼ੁਰੂ (hb_font_t *font HB_ਅਣਵਰਤਿਆ, hb_ਬਫਰ_t *ਬਫਰ)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2 = ਬਫਰ->len;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++)
    ਬਫਰ->pos[i].attach_chain() = ਬਫਰ->pos[i].attach_type() = 0;
}

ਖਾਲੀ
GPOS::position_ਅੰਤ_advances (hb_font_t *font HB_ਅਣਵਰਤਿਆ, hb_ਬਫਰ_t *ਬਫਰ HB_ਅਣਵਰਤਿਆ)
{
  //_hb_buffer_assert_gsubgpos_vars (buffer);
}

ਖਾਲੀ
GPOS::position_ਅੰਤ_offsets (hb_font_t *font, hb_ਬਫਰ_t *ਬਫਰ)
{
  hb_ਬਫਰ_assert_gsubgpos_vars (ਬਫਰ);

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len;
  hb_glyph_position_t *pos = hb_ਬਫਰ_get_glyph_positions (ਬਫਰ, &len);
  hb_direction_t direction = ਬਫਰ->props.direction;

  /* Handle attachments */
  ਜੇ (ਬਫਰ->scratch_flags & HB_ਬਫਰ_SCRATCH_FLAG_HAS_GPOS_ATTACHMENT)
  {
    ਸਵੈਚਾਲਿਤ *pos = ਬਫਰ->pos;
    // https://github.com/harfbuzz/harfbuzz/issues/5514
    ਜੇ (HB_DIRECTION_IS_FORWARD (direction))
    {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
	ਜੇ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    } ਨਹੀਂਤਾਂ {
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = len; i-- > 0; )
	ਜੇ (pos[i].attach_chain())
	  propagate_attachment_offsets (pos, len, i, direction);
    }
  }

  ਜੇ (unlikely (font->slant_xy) &&
      HB_DIRECTION_IS_HORIZONTAL (direction))
  {
    /* Slanting shaping results is only supported for horizontal text,
     * as it gets weird otherwise. */
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < len; i++)
      ਜੇ (unlikely (pos[i].y_ਵਿੱਥ_2))
        pos[i].x_ਵਿੱਥ_2 = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((ਦੁੱਗਣਾ) pos[i].x_ਵਿੱਥ_2 +
						      (ਦੁੱਗਣਾ) roundf (font->slant_xy * pos[i].y_ਵਿੱਥ_2));
  }
}

}

ਬਣਤਰ GPOS_accelerator_t : Layout::GPOS::accelerator_t {
  GPOS_accelerator_t (hb_face_t *face) : Layout::GPOS::accelerator_t (face) {}
};

}

#ਜੇ_ਅੰਤ  /* OT_LAYOUT_GPOS_GPOS_HH */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_LAYOUT_GPOS_VALUEFORMAT_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-ot-layout-gsubgpos.hh"

ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ Layout {
ਨਾਮਾਕਾਸ਼ GPOS_impl {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ HBUINT16 ਮੁੱਲ;

ਬਣਤਰ ਮੁੱਲ_ਅਧਾਰ {}; // Dummy base class tag for OffsetTo<Value> bases.

ਕਿਸਮਪਰਿਭਾਸ਼ਾ Unsized_Array_ਦਾ<ਮੁੱਲ> ਮੁੱਲ_ਰਿਕਾਰਡ;

ਬਣਤਰ ਮੁੱਲ_Format : HBUINT16
{
  ਗਿਣਤੀ Flags {
    xPlacement  = 0x0001u,      /* Includes horizontal adjustment for placement */
    yPlacement  = 0x0002u,      /* Includes vertical adjustment for placement */
    xAdvance    = 0x0004u,      /* Includes horizontal adjustment for advance */
    yAdvance    = 0x0008u,      /* Includes vertical adjustment for advance */
    xPlaDevice  = 0x0010u,      /* Includes horizontal Device table for placement */
    yPlaDevice  = 0x0020u,      /* Includes vertical Device table for placement */
    xAdvDevice  = 0x0040u,      /* Includes horizontal Device table for advance */
    yAdvDevice  = 0x0080u,      /* Includes vertical Device table for advance */
    ignored     = 0x0F00u,      /* Was used in TrueType Open for MM fonts */
    reserved    = 0xF000u,      /* For future use */

    devices     = 0x00F0u       /* Mask for having any Device table */
  };

/* All fields are options.  Only those available advance the value pointer. */
#ਜੇ 0
  HBINT16               xPlacement;     /* Horizontal adjustment for
                                         * placement--in design units */
  HBINT16               yPlacement;     /* Vertical adjustment for
                                         * placement--in design units */
  HBINT16               xAdvance;       /* Horizontal adjustment for
                                         * advance--in design units (only used
                                         * for horizontal writing) */
  HBINT16               yAdvance;       /* Vertical adjustment for advance--in
                                         * design units (only used for vertical
                                         * writing) */
  Offset16_ਮੰਜ਼ਿਲ<Device>    xPlaDevice;     /* Offset to Device table for
                                         * horizontal placement--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<Device>    yPlaDevice;     /* Offset to Device table for vertical
                                         * placement--measured from beginning
                                         * of PosTable (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<Device>    xAdvDevice;     /* Offset to Device table for
                                         * horizontal advance--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_ਮੰਜ਼ਿਲ<Device>    yAdvDevice;     /* Offset to Device table for vertical
                                         * advance--measured from beginning of
                                         * PosTable (may be NULL) */
#ਜੇ_ਅੰਤ

  ਅੰਕ_Type& ਸੰਚਾਲਕ = (uint16_t i) { v = i; ਵਾਪਸ *ਇਹ; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_len () ਅਚਲ  { ਵਾਪਸ hb_popcount8 ((uint8_t) *ਇਹ); }
  size_t get_ਮਾਪ () ਅਚਲ { ਵਾਪਸ get_len () * ਮੁੱਲ::static_ਮਾਪ; }

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> get_device_ਸਾਰਣੀ_indices () ਅਚਲ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> ਨਤੀਜਾ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ format = *ਇਹ;

    ਜੇ (format & xPlacement) i++;
    ਜੇ (format & yPlacement) i++;
    ਜੇ (format & xAdvance)   i++;
    ਜੇ (format & yAdvance)   i++;

    ਜੇ (format & xPlaDevice) ਨਤੀਜਾ.push (i++);
    ਜੇ (format & yPlaDevice) ਨਤੀਜਾ.push (i++);
    ਜੇ (format & xAdvDevice) ਨਤੀਜਾ.push (i++);
    ਜੇ (format & yAdvDevice) ਨਤੀਜਾ.push (i++);

    ਵਾਪਸ ਨਤੀਜਾ;
  }

  ਤਾਰਕਿਕ apply_ਮੁੱਲ (hb_ot_apply_context_t *c,
                    ਅਚਲ ਮੁੱਲ_ਅਧਾਰ       *ਅਧਾਰ,
                    ਅਚਲ ਮੁੱਲ           *values,
                    hb_glyph_position_t   &glyph_pos) ਅਚਲ
  {
    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format = *ਇਹ;
    ਜੇ (!format) ਵਾਪਸ ret;

    hb_font_t *font = c->font;
    ਤਾਰਕਿਕ horizontal =
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#ਨਹੀਂਤਾਂ
      ਸੱਚ
#ਜੇ_ਅੰਤ
      ;

    ਜੇ (format & xPlacement)
      glyph_pos.x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (glyph_pos.x_ਵਿੱਥ_2, font->em_scale_x (get_short (values++, &ret)));
    ਜੇ (format & yPlacement)
      glyph_pos.y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (glyph_pos.y_ਵਿੱਥ_2, font->em_scale_y (get_short (values++, &ret)));
    ਜੇ (format & xAdvance) {
      ਜੇ (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_ਜੋੜੋ (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    ਜੇ (format & yAdvance) {
      ਜੇ (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    ਜੇ (!has_device ()) ਵਾਪਸ ret;

    ਤਾਰਕਿਕ use_x_device = font->x_ppem || font->has_nonzero_coords;
    ਤਾਰਕਿਕ use_y_device = font->y_ppem || font->has_nonzero_coords;

    ਜੇ (!use_x_device && !use_y_device) ਵਾਪਸ ret;

    ਅਚਲ ਵਸਤੂ_Variation_Store &store = c->var_store;
    ਸਵੈਚਾਲਿਤ *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    ਜੇ (format & xPlaDevice)
    {
      ਜੇ (use_x_device)
	glyph_pos.x_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (glyph_pos.x_ਵਿੱਥ_2, get_device (values, &ret, ਅਧਾਰ, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ਜੇ (format & yPlaDevice)
    {
      ਜੇ (use_y_device)
	glyph_pos.y_ਵਿੱਥ_2 = hb_saturate_ਜੋੜੋ (glyph_pos.y_ਵਿੱਥ_2, get_device (values, &ret, ਅਧਾਰ, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ਜੇ (format & xAdvDevice)
    {
      ਜੇ (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_ਜੋੜੋ (glyph_pos.x_advance, get_device (values, &ret, ਅਧਾਰ, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ਜੇ (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      ਜੇ (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, ਅਧਾਰ, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ਵਾਪਸ ret;
  }

  ਖਾਲੀ position_single (hb_font_t *font,
                        hb_blob_t *ਸਾਰਣੀ_blob,
                        hb_direction_t direction,
                        ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                        ਅਚਲ ਮੁੱਲ *values,
                        hb_glyph_position_t &pos) ਅਚਲ
  {
    /* This is ugly... */
    hb_ਬਫਰ_t ਬਫਰ {};
    ਬਫਰ.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &ਬਫਰ, ਸਾਰਣੀ_blob);

    apply_ਮੁੱਲ (&c, ਅਧਾਰ, values, pos);
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_effective_format (ਅਚਲ ਮੁੱਲ *values, ਤਾਰਕਿਕ strip_hints, ਤਾਰਕਿਕ strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                                     ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format = *ਇਹ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      ਜੇ (format & flag)
      {
        ਜੇ (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          ਜਾਰੀ;
        }
        ਜੇ (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, ਅਧਾਰ, varidx_delta_map);
          ਜਾਰੀ;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        ਜੇ (strip_ਖਾਲੀ_ਥਾਵਾਂ) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ਵਾਪਸ format;
  }

  ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_effective_format (Iterator it, ਤਾਰਕਿਕ strip_hints, ਤਾਰਕਿਕ strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                                     ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map) ਅਚਲ {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_format = 0;

    ਲਈ (ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_ਖਾਲੀ_ਥਾਵਾਂ, ਅਧਾਰ, varidx_delta_map);

    ਵਾਪਸ new_format;
  }

  ਖਾਲੀ copy_ਮੁੱਲਾਂ (hb_serialize_context_t *c,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_format,
                    ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                    ਅਚਲ ਮੁੱਲ *values,
                    ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format = *ਇਹ;
    ਜੇ (!format) ਵਾਪਸ;

    HBINT16 *x_placement = ਖਾਲੀ_ਸੂਚਕ, *y_placement = ਖਾਲੀ_ਸੂਚਕ, *x_adv = ਖਾਲੀ_ਸੂਚਕ, *y_adv = ਖਾਲੀ_ਸੂਚਕ;
    ਜੇ ((format | new_format) & xPlacement)
      x_placement = copy_ਮੁੱਲ (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (ਮੁੱਲ));
    ਜੇ ((format | new_format) & yPlacement)
      y_placement = copy_ਮੁੱਲ (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (ਮੁੱਲ));
    ਜੇ ((format | new_format) & xAdvance)
      x_adv = copy_ਮੁੱਲ (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (ਮੁੱਲ));
    ਜੇ ((format | new_format) & yAdvance)
      y_adv = copy_ਮੁੱਲ (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (ਮੁੱਲ));

    ਜੇ (!has_device ())
      ਵਾਪਸ;

    ਜੇ (format & xPlaDevice)
    {
      ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_ਮੁੱਲ (x_placement, ਅਧਾਰ, values, layout_variation_idx_delta_map);
      copy_device (c, ਅਧਾਰ, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    ਜੇ (format & yPlaDevice)
    {
      ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_ਮੁੱਲ (y_placement, ਅਧਾਰ, values, layout_variation_idx_delta_map);
      copy_device (c, ਅਧਾਰ, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    ਜੇ (format & xAdvDevice)
    {
      ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_ਮੁੱਲ (x_adv, ਅਧਾਰ, values, layout_variation_idx_delta_map);
      copy_device (c, ਅਧਾਰ, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    ਜੇ (format & yAdvDevice)
    {
      ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_ਮੁੱਲ (y_adv, ਅਧਾਰ, values, layout_variation_idx_delta_map);
      copy_device (c, ਅਧਾਰ, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_ਮੁੱਲ (hb_serialize_context_t *c,
                       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_format,
                       Flags flag,
                       ਮੁੱਲ value) ਅਚਲ
  {
    // Filter by new format.
    ਜੇ (!(new_format & flag)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  ਖਾਲੀ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                                  ਅਚਲ hb_array_t<ਅਚਲ ਮੁੱਲ>& values) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ format = *ਇਹ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0;
    ਜੇ (format & xPlacement) i++;
    ਜੇ (format & yPlacement) i++;
    ਜੇ (format & xAdvance) i++;
    ਜੇ (format & yAdvance) i++;
    ਜੇ (format & xPlaDevice)
    {
      (ਅਧਾਰ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ਜੇ (format & ਮੁੱਲ_Format::yPlaDevice)
    {
      (ਅਧਾਰ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ਜੇ (format & ਮੁੱਲ_Format::xAdvDevice)
    {
      (ਅਧਾਰ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ਜੇ (format & ਮੁੱਲ_Format::yAdvDevice)
    {
      (ਅਧਾਰ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  ਨਿਜੀ:
  ਤਾਰਕਿਕ sanitize_ਮੁੱਲ_devices (hb_sanitize_context_t *c, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ, ਅਚਲ ਮੁੱਲ *values) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format = *ਇਹ;

    ਜੇ (format & xPlacement) values++;
    ਜੇ (format & yPlacement) values++;
    ਜੇ (format & xAdvance)   values++;
    ਜੇ (format & yAdvance)   values++;

    ਜੇ ((format & xPlaDevice) && !get_device (values++).sanitize (c, ਅਧਾਰ)) ਵਾਪਸ ਝੂਠ;
    ਜੇ ((format & yPlaDevice) && !get_device (values++).sanitize (c, ਅਧਾਰ)) ਵਾਪਸ ਝੂਠ;
    ਜੇ ((format & xAdvDevice) && !get_device (values++).sanitize (c, ਅਧਾਰ)) ਵਾਪਸ ਝੂਠ;
    ਜੇ ((format & yAdvDevice) && !get_device (values++).sanitize (c, ਅਧਾਰ)) ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ Offset16_ਮੰਜ਼ਿਲ<Device, ਮੁੱਲ_ਅਧਾਰ>& get_device (ਮੁੱਲ* value)
  {
    ਵਾਪਸ *static_cast<Offset16_ਮੰਜ਼ਿਲ<Device, ਮੁੱਲ_ਅਧਾਰ> *> (value);
  }
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Device, ਮੁੱਲ_ਅਧਾਰ>& get_device (ਅਚਲ ਮੁੱਲ* value)
  {
    ਵਾਪਸ *static_cast<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Device, ਮੁੱਲ_ਅਧਾਰ> *> (value);
  }
  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ Device& get_device (ਅਚਲ ਮੁੱਲ* value,
					  ਤਾਰਕਿਕ *worked,
					  ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
					  hb_sanitize_context_t &c)
  {
    ਜੇ (worked) *worked |= ਤਾਰਕਿਕ (*value);
    ਸਵੈਚਾਲਿਤ &ਵਿੱਥ = *static_cast<ਅਚਲ Offset16_ਮੰਜ਼ਿਲ<Device> *> (value);

    ਜੇ (unlikely (!ਵਿੱਥ.sanitize (&c, ਅਧਾਰ)))
      ਵਾਪਸ Null(Device);
    hb_barrier ();

    ਵਾਪਸ ਅਧਾਰ + ਵਿੱਥ;
  }

  ਖਾਲੀ ਜੋੜੋ_delta_ਮੰਜ਼ਿਲ_ਮੁੱਲ (HBINT16 *value,
                           ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                           ਅਚਲ ਮੁੱਲ *src_ਮੁੱਲ,
                           ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map) ਅਚਲ
  {
    ਜੇ (!value) ਵਾਪਸ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ varidx = (ਅਧਾਰ + get_device (src_ਮੁੱਲ)).get_variation_ਸੂਚਕਾਂਕ ();
    hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *varidx_delta;
    ਜੇ (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ਵਾਪਸ;

    *value += hb_ਦੂਜਾ (*varidx_delta);
  }

  ਤਾਰਕਿਕ copy_device (hb_serialize_context_t *c,
                    ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                    ਅਚਲ ਮੁੱਲ *src_ਮੁੱਲ,
                    ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *layout_variation_idx_delta_map,
                    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ new_format, Flags flag) ਅਚਲ
  {
    // Filter by new format.
    ਜੇ (!(new_format & flag)) ਵਾਪਸ ਸੱਚ;

    ਮੁੱਲ       *dst_ਮੁੱਲ = c->copy (*src_ਮੁੱਲ);

    ਜੇ (!dst_ਮੁੱਲ) ਵਾਪਸ ਝੂਠ;
    ਜੇ (*dst_ਮੁੱਲ == 0) ਵਾਪਸ ਸੱਚ;

    *dst_ਮੁੱਲ = 0;
    c->push ();
    ਜੇ ((ਅਧਾਰ + get_device (src_ਮੁੱਲ)).copy (c, layout_variation_idx_delta_map))
    {
      c->ਜੋੜੋ_link (*dst_ਮੁੱਲ, c->ਕੱਢੋ_pack ());
      ਵਾਪਸ ਸੱਚ;
    }
    ਨਹੀਂਤਾਂ
    {
      c->ਕੱਢੋ_discard ();
      ਵਾਪਸ ਝੂਠ;
    }
  }

  ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ HBINT16& get_short (ਅਚਲ ਮੁੱਲ* value, ਤਾਰਕਿਕ *worked=ਖਾਲੀ_ਸੂਚਕ)
  {
    ਜੇ (worked) *worked |= ਤਾਰਕਿਕ (*value);
    ਵਾਪਸ *reinterpret_cast<ਅਚਲ HBINT16 *> (value);
  }

  ਜਨਤਕ:

  ਤਾਰਕਿਕ has_device () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ format = *ਇਹ;
    ਵਾਪਸ (format & devices) != 0;
  }

  ਤਾਰਕਿਕ sanitize_ਮੁੱਲ (hb_sanitize_context_t *c, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ, ਅਚਲ ਮੁੱਲ *values) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਜੇ (unlikely (!c->check_range (values, get_ਮਾਪ ()))) return_trace (ਝੂਠ);

    ਜੇ (c->lazy_some_gpos)
      return_trace (ਸੱਚ);

    return_trace (!has_device () || sanitize_ਮੁੱਲ_devices (c, ਅਧਾਰ, values));
  }

  ਤਾਰਕਿਕ sanitize_ਮੁੱਲਾਂ (hb_sanitize_context_t *c, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ, ਅਚਲ ਮੁੱਲ *values, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = get_ਮਾਪ ();

    ਜੇ (!c->check_range (values, ਗਿਣਤੀ_2, ਮਾਪ)) return_trace (ਝੂਠ);

    ਜੇ (c->lazy_some_gpos)
      return_trace (ਸੱਚ);

    hb_barrier ();
    return_trace (sanitize_ਮੁੱਲਾਂ_stride_unsafe (c, ਅਧਾਰ, values, ਗਿਣਤੀ_2, ਮਾਪ));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  ਤਾਰਕਿਕ sanitize_ਮੁੱਲਾਂ_stride_unsafe (hb_sanitize_context_t *c, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ, ਅਚਲ ਮੁੱਲ *values, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਗਿਣਤੀ_2, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ stride) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);

    ਜੇ (!has_device ()) return_trace (ਸੱਚ);

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2; i++) {
      ਜੇ (!sanitize_ਮੁੱਲ_devices (c, ਅਧਾਰ, values))
        return_trace (ਝੂਠ);
      values = &Struct_At_ਵਿੱਥ<ਅਚਲ ਮੁੱਲ> (values, stride);
    }

    return_trace (ਸੱਚ);
  }

 ਨਿਜੀ:

  ਖਾਲੀ should_drop (ਮੁੱਲ value, Flags flag, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ* format) ਅਚਲ
  {
    ਜੇ (value) ਵਾਪਸ;
    *format = *format & ~flag;
  }

  ਖਾਲੀ update_var_flag (ਅਚਲ ਮੁੱਲ* value, Flags flag,
                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ* format, ਅਚਲ ਮੁੱਲ_ਅਧਾਰ *ਅਧਾਰ,
                        ਅਚਲ hb_hashmap_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ>> *varidx_delta_map) ਅਚਲ
  {
    ਜੇ (*value)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ varidx = (ਅਧਾਰ + get_device (value)).get_variation_ਸੂਚਕਾਂਕ ();
      hb_pair_t<ਬਿਨਾਨਿਸ਼ਾਨੀ, ਪੂਰਨ> *varidx_delta;
      ਜੇ (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        ਜੇ (varidx_delta->second)
          *format |= flag >> 4;
        ਜੇ (varidx_delta->ਪਹਿਲਾ != HB_OT_LAYOUT_NO_VARIATIONS_ਸੂਚਕਾਂਕ)
          ਵਾਪਸ;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#ਜੇ_ਅੰਤ  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

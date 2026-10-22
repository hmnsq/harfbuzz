#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ಅನ್ತರ್ಭಾವ "../../../hb-ot-layout-gsubgpos.hh"

ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ Layout {
ನಾಮಾಕಾಶ GPOS_impl {

ಪ್ರಕಾರನಿರ್ದೇಶ HBUINT16 ಮೌಲ್ಯ;

ಸಂರಚನಾ ಮೌಲ್ಯ_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

ಪ್ರಕಾರನಿರ್ದೇಶ UnsizedArrayOf<ಮೌಲ್ಯ> ಮೌಲ್ಯ_Record;

ಸಂರಚನಾ ಮೌಲ್ಯ_Format : HBUINT16
{
  ಗಣನಾ Flags {
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
#ಯದಿ 0
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
  Offset16To<Device>    xPlaDevice;     /* Offset to Device table for
                                         * horizontal placement--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16To<Device>    yPlaDevice;     /* Offset to Device table for vertical
                                         * placement--measured from beginning
                                         * of PosTable (may be NULL) */
  Offset16To<Device>    xAdvDevice;     /* Offset to Device table for
                                         * horizontal advance--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16To<Device>    yAdvDevice;     /* Offset to Device table for vertical
                                         * advance--measured from beginning of
                                         * PosTable (may be NULL) */
#endif

  ಅಂಕೆ_Type& operator = (uint16_t i) { v = i; ಪ್ರತ್ಯಾಗಮ *ಏತತ್; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned ಪೂರ್ಣಾಙ್ಕ get_len () ಧ್ರುವ  { ಪ್ರತ್ಯಾಗಮ hb_popcount8 ((uint8_t) *ಏತತ್); }
  size_t get_size () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ get_len () * ಮೌಲ್ಯ::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ಧ್ರುವ {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *ಏತತ್;

    ಯದಿ (format & xPlacement) i++;
    ಯದಿ (format & yPlacement) i++;
    ಯದಿ (format & xAdvance)   i++;
    ಯದಿ (format & yAdvance)   i++;

    ಯದಿ (format & xPlaDevice) result.push (i++);
    ಯದಿ (format & yPlaDevice) result.push (i++);
    ಯದಿ (format & xAdvDevice) result.push (i++);
    ಯದಿ (format & yAdvDevice) result.push (i++);

    ಪ್ರತ್ಯಾಗಮ result;
  }

  ತಾರ್ಕಿಕ apply_ಮೌಲ್ಯ (hb_ot_apply_context_t *c,
                    ಧ್ರುವ ಮೌಲ್ಯ_Base       *base,
                    ಧ್ರುವ ಮೌಲ್ಯ           *values,
                    hb_glyph_position_t   &glyph_pos) ಧ್ರುವ
  {
    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    unsigned ಪೂರ್ಣಾಙ್ಕ format = *ಏತತ್;
    ಯದಿ (!format) ಪ್ರತ್ಯಾಗಮ ret;

    hb_font_t *font = c->font;
    ತಾರ್ಕಿಕ horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#ಅನ್ಯಥಾ
      ಸತ್ಯ
#endif
      ;

    ಯದಿ (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    ಯದಿ (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    ಯದಿ (format & xAdvance) {
      ಯದಿ (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    ಯದಿ (format & yAdvance) {
      ಯದಿ (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    ಯದಿ (!has_device ()) ಪ್ರತ್ಯಾಗಮ ret;

    ತಾರ್ಕಿಕ use_x_device = font->x_ppem || font->has_nonzero_coords;
    ತಾರ್ಕಿಕ use_y_device = font->y_ppem || font->has_nonzero_coords;

    ಯದಿ (!use_x_device && !use_y_device) ಪ್ರತ್ಯಾಗಮ ret;

    ಧ್ರುವ ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    ಯದಿ (format & xPlaDevice)
    {
      ಯದಿ (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ಯದಿ (format & yPlaDevice)
    {
      ಯದಿ (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ಯದಿ (format & xAdvDevice)
    {
      ಯದಿ (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ಯದಿ (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      ಯದಿ (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ಪ್ರತ್ಯಾಗಮ ret;
  }

  ಶೂನ್ಯ position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                        ಧ್ರುವ ಮೌಲ್ಯ *values,
                        hb_glyph_position_t &pos) ಧ್ರುವ
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_ಮೌಲ್ಯ (&c, base, values, pos);
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ get_effective_format (ಧ್ರುವ ಮೌಲ್ಯ *values, ತಾರ್ಕಿಕ strip_hints, ತಾರ್ಕಿಕ strip_empty, ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                                     ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ format = *ಏತತ್;
    ಪ್ರತಿ (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      ಯದಿ (format & flag)
      {
        ಯದಿ (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          ಅನುವರ್ತ;
        }
        ಯದಿ (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          ಅನುವರ್ತ;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        ಯದಿ (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ಪ್ರತ್ಯಾಗಮ format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned ಪೂರ್ಣಾಙ್ಕ get_effective_format (Iterator it, ತಾರ್ಕಿಕ strip_hints, ತಾರ್ಕಿಕ strip_empty, ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                                     ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map) ಧ್ರುವ {
    unsigned ಪೂರ್ಣಾಙ್ಕ new_format = 0;

    ಪ್ರತಿ (ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    ಪ್ರತ್ಯಾಗಮ new_format;
  }

  ಶೂನ್ಯ copy_ಮೌಲ್ಯಗಳು (hb_serialize_context_t *c,
                    unsigned ಪೂರ್ಣಾಙ್ಕ new_format,
                    ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                    ಧ್ರುವ ಮೌಲ್ಯ *values,
                    ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ format = *ಏತತ್;
    ಯದಿ (!format) ಪ್ರತ್ಯಾಗಮ;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    ಯದಿ ((format | new_format) & xPlacement)
      x_placement = copy_ಮೌಲ್ಯ (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (ಮೌಲ್ಯ));
    ಯದಿ ((format | new_format) & yPlacement)
      y_placement = copy_ಮೌಲ್ಯ (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (ಮೌಲ್ಯ));
    ಯದಿ ((format | new_format) & xAdvance)
      x_adv = copy_ಮೌಲ್ಯ (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (ಮೌಲ್ಯ));
    ಯದಿ ((format | new_format) & yAdvance)
      y_adv = copy_ಮೌಲ್ಯ (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (ಮೌಲ್ಯ));

    ಯದಿ (!has_device ())
      ಪ್ರತ್ಯಾಗಮ;

    ಯದಿ (format & xPlaDevice)
    {
      add_delta_to_ಮೌಲ್ಯ (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    ಯದಿ (format & yPlaDevice)
    {
      add_delta_to_ಮೌಲ್ಯ (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    ಯದಿ (format & xAdvDevice)
    {
      add_delta_to_ಮೌಲ್ಯ (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    ಯದಿ (format & yAdvDevice)
    {
      add_delta_to_ಮೌಲ್ಯ (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_ಮೌಲ್ಯ (hb_serialize_context_t *c,
                       unsigned ಪೂರ್ಣಾಙ್ಕ new_format,
                       Flags flag,
                       ಮೌಲ್ಯ value) ಧ್ರುವ
  {
    // Filter by new format.
    ಯದಿ (!(new_format & flag)) ಪ್ರತ್ಯಾಗಮ nullptr;
    ಪ್ರತ್ಯಾಗಮ reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  ಶೂನ್ಯ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                                  ಧ್ರುವ hb_array_t<ಧ್ರುವ ಮೌಲ್ಯ>& values) ಧ್ರುವ
  {
    unsigned format = *ಏತತ್;
    unsigned i = 0;
    ಯದಿ (format & xPlacement) i++;
    ಯದಿ (format & yPlacement) i++;
    ಯದಿ (format & xAdvance) i++;
    ಯದಿ (format & yAdvance) i++;
    ಯದಿ (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ಯದಿ (format & ಮೌಲ್ಯ_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ಯದಿ (format & ಮೌಲ್ಯ_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ಯದಿ (format & ಮೌಲ್ಯ_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  ಗುಪ್ತ:
  ತಾರ್ಕಿಕ sanitize_ಮೌಲ್ಯ_devices (hb_sanitize_context_t *c, ಧ್ರುವ ಮೌಲ್ಯ_Base *base, ಧ್ರುವ ಮೌಲ್ಯ *values) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ format = *ಏತತ್;

    ಯದಿ (format & xPlacement) values++;
    ಯದಿ (format & yPlacement) values++;
    ಯದಿ (format & xAdvance)   values++;
    ಯದಿ (format & yAdvance)   values++;

    ಯದಿ ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ Offset16To<Device, ಮೌಲ್ಯ_Base>& get_device (ಮೌಲ್ಯ* value)
  {
    ಪ್ರತ್ಯಾಗಮ *static_cast<Offset16To<Device, ಮೌಲ್ಯ_Base> *> (value);
  }
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ Offset16To<Device, ಮೌಲ್ಯ_Base>& get_device (ಧ್ರುವ ಮೌಲ್ಯ* value)
  {
    ಪ್ರತ್ಯಾಗಮ *static_cast<ಧ್ರುವ Offset16To<Device, ಮೌಲ್ಯ_Base> *> (value);
  }
  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ Device& get_device (ಧ್ರುವ ಮೌಲ್ಯ* value,
					  ತಾರ್ಕಿಕ *worked,
					  ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
					  hb_sanitize_context_t &c)
  {
    ಯದಿ (worked) *worked |= ತಾರ್ಕಿಕ (*value);
    auto &offset = *static_cast<ಧ್ರುವ Offset16To<Device> *> (value);

    ಯದಿ (unlikely (!offset.sanitize (&c, base)))
      ಪ್ರತ್ಯಾಗಮ Null(Device);
    hb_barrier ();

    ಪ್ರತ್ಯಾಗಮ base + offset;
  }

  ಶೂನ್ಯ add_delta_to_ಮೌಲ್ಯ (HBINT16 *value,
                           ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                           ಧ್ರುವ ಮೌಲ್ಯ *src_ಮೌಲ್ಯ,
                           ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map) ಧ್ರುವ
  {
    ಯದಿ (!value) ಪ್ರತ್ಯಾಗಮ;
    unsigned varidx = (base + get_device (src_ಮೌಲ್ಯ)).get_variation_index ();
    hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *varidx_delta;
    ಯದಿ (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ಪ್ರತ್ಯಾಗಮ;

    *value += hb_second (*varidx_delta);
  }

  ತಾರ್ಕಿಕ copy_device (hb_serialize_context_t *c,
                    ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                    ಧ್ರುವ ಮೌಲ್ಯ *src_ಮೌಲ್ಯ,
                    ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *layout_variation_idx_delta_map,
                    unsigned ಪೂರ್ಣಾಙ್ಕ new_format, Flags flag) ಧ್ರುವ
  {
    // Filter by new format.
    ಯದಿ (!(new_format & flag)) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಮೌಲ್ಯ       *dst_ಮೌಲ್ಯ = c->copy (*src_ಮೌಲ್ಯ);

    ಯದಿ (!dst_ಮೌಲ್ಯ) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (*dst_ಮೌಲ್ಯ == 0) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    *dst_ಮೌಲ್ಯ = 0;
    c->push ();
    ಯದಿ ((base + get_device (src_ಮೌಲ್ಯ)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_ಮೌಲ್ಯ, c->ತೆಗೆ_pack ());
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಅನ್ಯಥಾ
    {
      c->ತೆಗೆ_discard ();
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
  }

  ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ HBINT16& get_short (ಧ್ರುವ ಮೌಲ್ಯ* value, ತಾರ್ಕಿಕ *worked=nullptr)
  {
    ಯದಿ (worked) *worked |= ತಾರ್ಕಿಕ (*value);
    ಪ್ರತ್ಯಾಗಮ *reinterpret_cast<ಧ್ರುವ HBINT16 *> (value);
  }

  ಸಾರ್ವಜನಿಕ:

  ತಾರ್ಕಿಕ has_device () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ format = *ಏತತ್;
    ಪ್ರತ್ಯಾಗಮ (format & devices) != 0;
  }

  ತಾರ್ಕಿಕ sanitize_ಮೌಲ್ಯ (hb_sanitize_context_t *c, ಧ್ರುವ ಮೌಲ್ಯ_Base *base, ಧ್ರುವ ಮೌಲ್ಯ *values) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    ಯದಿ (unlikely (!c->check_range (values, get_size ()))) return_trace (ಅಸತ್ಯ);

    ಯದಿ (c->lazy_some_gpos)
      return_trace (ಸತ್ಯ);

    return_trace (!has_device () || sanitize_ಮೌಲ್ಯ_devices (c, base, values));
  }

  ತಾರ್ಕಿಕ sanitize_ಮೌಲ್ಯಗಳು (hb_sanitize_context_t *c, ಧ್ರುವ ಮೌಲ್ಯ_Base *base, ಧ್ರುವ ಮೌಲ್ಯ *values, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    unsigned size = get_size ();

    ಯದಿ (!c->check_range (values, ಎಣಿಕೆ, size)) return_trace (ಅಸತ್ಯ);

    ಯದಿ (c->lazy_some_gpos)
      return_trace (ಸತ್ಯ);

    hb_barrier ();
    return_trace (sanitize_ಮೌಲ್ಯಗಳು_stride_unsafe (c, base, values, ಎಣಿಕೆ, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  ತಾರ್ಕಿಕ sanitize_ಮೌಲ್ಯಗಳು_stride_unsafe (hb_sanitize_context_t *c, ಧ್ರುವ ಮೌಲ್ಯ_Base *base, ಧ್ರುವ ಮೌಲ್ಯ *values, unsigned ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ, unsigned ಪೂರ್ಣಾಙ್ಕ stride) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);

    ಯದಿ (!has_device ()) return_trace (ಸತ್ಯ);

    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ; i++) {
      ಯದಿ (!sanitize_ಮೌಲ್ಯ_devices (c, base, values))
        return_trace (ಅಸತ್ಯ);
      values = &StructAtOffset<ಧ್ರುವ ಮೌಲ್ಯ> (values, stride);
    }

    return_trace (ಸತ್ಯ);
  }

 ಗುಪ್ತ:

  ಶೂನ್ಯ should_drop (ಮೌಲ್ಯ value, Flags flag, unsigned ಪೂರ್ಣಾಙ್ಕ* format) ಧ್ರುವ
  {
    ಯದಿ (value) ಪ್ರತ್ಯಾಗಮ;
    *format = *format & ~flag;
  }

  ಶೂನ್ಯ update_var_flag (ಧ್ರುವ ಮೌಲ್ಯ* value, Flags flag,
                        unsigned ಪೂರ್ಣಾಙ್ಕ* format, ಧ್ರುವ ಮೌಲ್ಯ_Base *base,
                        ಧ್ರುವ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ>> *varidx_delta_map) ಧ್ರುವ
  {
    ಯದಿ (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, ಪೂರ್ಣಾಙ್ಕ> *varidx_delta;
      ಯದಿ (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        ಯದಿ (varidx_delta->second)
          *format |= flag >> 4;
        ಯದಿ (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ಪ್ರತ್ಯಾಗಮ;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

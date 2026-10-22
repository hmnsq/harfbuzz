#اگر_غیر_تعریف_شدہ OT_LAYOUT_GPOS_VALUEFORMAT_HH
#تعریف_کرو OT_LAYOUT_GPOS_VALUEFORMAT_HH

#شامل "../../../hb-ot-layout-gsubgpos.hh"

نامخلا OT {
نامخلا Layout {
نامخلا GPOS_impl {

قسمتعریف HBUINT16 قدر;

ساخت قدر_بنیاد {}; // Dummy base class tag for OffsetTo<Value> bases.

قسمتعریف Unsized_Array_کا<قدر> قدر_ریکارڈ;

ساخت قدر_Format : HBUINT16
{
  شمار Flags {
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
#اگر 0
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
  Offset16_مقصد<Device>    xPlaDevice;     /* Offset to Device table for
                                         * horizontal placement--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_مقصد<Device>    yPlaDevice;     /* Offset to Device table for vertical
                                         * placement--measured from beginning
                                         * of PosTable (may be NULL) */
  Offset16_مقصد<Device>    xAdvDevice;     /* Offset to Device table for
                                         * horizontal advance--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_مقصد<Device>    yAdvDevice;     /* Offset to Device table for vertical
                                         * advance--measured from beginning of
                                         * PosTable (may be NULL) */
#اگر_ختم

  عدد_Type& عامل = (uint16_t i) { v = i; واپس *یہ; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  غیرعلامتی صحیح get_len () ثابت  { واپس hb_popcount8 ((uint8_t) *یہ); }
  size_t get_سائز () ثابت { واپس get_len () * قدر::static_سائز; }

  hb_سمتیہ_t<غیرعلامتی> get_device_جدول_indices () ثابت {
    غیرعلامتی i = 0;
    hb_سمتیہ_t<غیرعلامتی> نتیجہ;
    غیرعلامتی format = *یہ;

    اگر (format & xPlacement) i++;
    اگر (format & yPlacement) i++;
    اگر (format & xAdvance)   i++;
    اگر (format & yAdvance)   i++;

    اگر (format & xPlaDevice) نتیجہ.push (i++);
    اگر (format & yPlaDevice) نتیجہ.push (i++);
    اگر (format & xAdvDevice) نتیجہ.push (i++);
    اگر (format & yAdvDevice) نتیجہ.push (i++);

    واپس نتیجہ;
  }

  منطقی apply_قدر (hb_ot_apply_context_t *c,
                    ثابت قدر_بنیاد       *بنیاد,
                    ثابت قدر           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقی ret = جھوٹ;
    غیرعلامتی صحیح format = *یہ;
    اگر (!format) واپس ret;

    hb_font_t *font = c->font;
    منطقی horizontal =
#اگر_غیر_تعریف_شدہ HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#ورنہ
      سچ
#اگر_ختم
      ;

    اگر (format & xPlacement)
      glyph_pos.x_انحراف_2 = hb_saturate_جوڑو (glyph_pos.x_انحراف_2, font->em_scale_x (get_short (values++, &ret)));
    اگر (format & yPlacement)
      glyph_pos.y_انحراف_2 = hb_saturate_جوڑو (glyph_pos.y_انحراف_2, font->em_scale_y (get_short (values++, &ret)));
    اگر (format & xAdvance) {
      اگر (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_جوڑو (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    اگر (format & yAdvance) {
      اگر (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    اگر (!has_device ()) واپس ret;

    منطقی use_x_device = font->x_ppem || font->has_nonzero_coords;
    منطقی use_y_device = font->y_ppem || font->has_nonzero_coords;

    اگر (!use_x_device && !use_y_device) واپس ret;

    ثابت شے_Variation_Store &store = c->var_store;
    خودکار *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    اگر (format & xPlaDevice)
    {
      اگر (use_x_device)
	glyph_pos.x_انحراف_2 = hb_saturate_جوڑو (glyph_pos.x_انحراف_2, get_device (values, &ret, بنیاد, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    اگر (format & yPlaDevice)
    {
      اگر (use_y_device)
	glyph_pos.y_انحراف_2 = hb_saturate_جوڑو (glyph_pos.y_انحراف_2, get_device (values, &ret, بنیاد, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    اگر (format & xAdvDevice)
    {
      اگر (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_جوڑو (glyph_pos.x_advance, get_device (values, &ret, بنیاد, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    اگر (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      اگر (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, بنیاد, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    واپس ret;
  }

  خالی position_single (hb_font_t *font,
                        hb_blob_t *جدول_blob,
                        hb_direction_t direction,
                        ثابت قدر_بنیاد *بنیاد,
                        ثابت قدر *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_بفر_t بفر {};
    بفر.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &بفر, جدول_blob);

    apply_قدر (&c, بنیاد, values, pos);
  }

  غیرعلامتی صحیح get_effective_format (ثابت قدر *values, منطقی strip_hints, منطقی strip_خالی_خانے, ثابت قدر_بنیاد *بنیاد,
                                     ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map) ثابت
  {
    غیرعلامتی صحیح format = *یہ;
    برائے (غیرعلامتی flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      اگر (format & flag)
      {
        اگر (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          جاری;
        }
        اگر (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, بنیاد, varidx_delta_map);
          جاری;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        اگر (strip_خالی_خانے) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    واپس format;
  }

  سانچہ<قسم_نام Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  غیرعلامتی صحیح get_effective_format (Iterator it, منطقی strip_hints, منطقی strip_خالی_خانے, ثابت قدر_بنیاد *بنیاد,
                                     ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map) ثابت {
    غیرعلامتی صحیح new_format = 0;

    برائے (ثابت hb_array_t<ثابت قدر>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_خالی_خانے, بنیاد, varidx_delta_map);

    واپس new_format;
  }

  خالی copy_اقدار (hb_serialize_context_t *c,
                    غیرعلامتی صحیح new_format,
                    ثابت قدر_بنیاد *بنیاد,
                    ثابت قدر *values,
                    ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map) ثابت
  {
    غیرعلامتی صحیح format = *یہ;
    اگر (!format) واپس;

    HBINT16 *x_placement = خالی_اشاریہ, *y_placement = خالی_اشاریہ, *x_adv = خالی_اشاریہ, *y_adv = خالی_اشاریہ;
    اگر ((format | new_format) & xPlacement)
      x_placement = copy_قدر (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (قدر));
    اگر ((format | new_format) & yPlacement)
      y_placement = copy_قدر (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (قدر));
    اگر ((format | new_format) & xAdvance)
      x_adv = copy_قدر (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (قدر));
    اگر ((format | new_format) & yAdvance)
      y_adv = copy_قدر (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (قدر));

    اگر (!has_device ())
      واپس;

    اگر (format & xPlaDevice)
    {
      جوڑو_delta_مقصد_قدر (x_placement, بنیاد, values, layout_variation_idx_delta_map);
      copy_device (c, بنیاد, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    اگر (format & yPlaDevice)
    {
      جوڑو_delta_مقصد_قدر (y_placement, بنیاد, values, layout_variation_idx_delta_map);
      copy_device (c, بنیاد, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    اگر (format & xAdvDevice)
    {
      جوڑو_delta_مقصد_قدر (x_adv, بنیاد, values, layout_variation_idx_delta_map);
      copy_device (c, بنیاد, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    اگر (format & yAdvDevice)
    {
      جوڑو_delta_مقصد_قدر (y_adv, بنیاد, values, layout_variation_idx_delta_map);
      copy_device (c, بنیاد, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_قدر (hb_serialize_context_t *c,
                       غیرعلامتی صحیح new_format,
                       Flags flag,
                       قدر value) ثابت
  {
    // Filter by new format.
    اگر (!(new_format & flag)) واپس خالی_اشاریہ;
    واپس reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  خالی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_بنیاد *بنیاد,
                                  ثابت hb_array_t<ثابت قدر>& values) ثابت
  {
    غیرعلامتی format = *یہ;
    غیرعلامتی i = 0;
    اگر (format & xPlacement) i++;
    اگر (format & yPlacement) i++;
    اگر (format & xAdvance) i++;
    اگر (format & yAdvance) i++;
    اگر (format & xPlaDevice)
    {
      (بنیاد + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    اگر (format & قدر_Format::yPlaDevice)
    {
      (بنیاد + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    اگر (format & قدر_Format::xAdvDevice)
    {
      (بنیاد + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    اگر (format & قدر_Format::yAdvDevice)
    {
      (بنیاد + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  نجی:
  منطقی sanitize_قدر_devices (hb_sanitize_context_t *c, ثابت قدر_بنیاد *بنیاد, ثابت قدر *values) ثابت
  {
    غیرعلامتی صحیح format = *یہ;

    اگر (format & xPlacement) values++;
    اگر (format & yPlacement) values++;
    اگر (format & xAdvance)   values++;
    اگر (format & yAdvance)   values++;

    اگر ((format & xPlaDevice) && !get_device (values++).sanitize (c, بنیاد)) واپس جھوٹ;
    اگر ((format & yPlaDevice) && !get_device (values++).sanitize (c, بنیاد)) واپس جھوٹ;
    اگر ((format & xAdvDevice) && !get_device (values++).sanitize (c, بنیاد)) واپس جھوٹ;
    اگر ((format & yAdvDevice) && !get_device (values++).sanitize (c, بنیاد)) واپس جھوٹ;

    واپس سچ;
  }

  ساکن اندرونی Offset16_مقصد<Device, قدر_بنیاد>& get_device (قدر* value)
  {
    واپس *static_cast<Offset16_مقصد<Device, قدر_بنیاد> *> (value);
  }
  ساکن اندرونی ثابت Offset16_مقصد<Device, قدر_بنیاد>& get_device (ثابت قدر* value)
  {
    واپس *static_cast<ثابت Offset16_مقصد<Device, قدر_بنیاد> *> (value);
  }
  ساکن اندرونی ثابت Device& get_device (ثابت قدر* value,
					  منطقی *worked,
					  ثابت قدر_بنیاد *بنیاد,
					  hb_sanitize_context_t &c)
  {
    اگر (worked) *worked |= منطقی (*value);
    خودکار &انحراف = *static_cast<ثابت Offset16_مقصد<Device> *> (value);

    اگر (unlikely (!انحراف.sanitize (&c, بنیاد)))
      واپس Null(Device);
    hb_barrier ();

    واپس بنیاد + انحراف;
  }

  خالی جوڑو_delta_مقصد_قدر (HBINT16 *value,
                           ثابت قدر_بنیاد *بنیاد,
                           ثابت قدر *src_قدر,
                           ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map) ثابت
  {
    اگر (!value) واپس;
    غیرعلامتی varidx = (بنیاد + get_device (src_قدر)).get_variation_اشاریہ ();
    hb_pair_t<غیرعلامتی, صحیح> *varidx_delta;
    اگر (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) واپس;

    *value += hb_دوسرا (*varidx_delta);
  }

  منطقی copy_device (hb_serialize_context_t *c,
                    ثابت قدر_بنیاد *بنیاد,
                    ثابت قدر *src_قدر,
                    ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *layout_variation_idx_delta_map,
                    غیرعلامتی صحیح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    اگر (!(new_format & flag)) واپس سچ;

    قدر       *dst_قدر = c->copy (*src_قدر);

    اگر (!dst_قدر) واپس جھوٹ;
    اگر (*dst_قدر == 0) واپس سچ;

    *dst_قدر = 0;
    c->push ();
    اگر ((بنیاد + get_device (src_قدر)).copy (c, layout_variation_idx_delta_map))
    {
      c->جوڑو_link (*dst_قدر, c->نکالو_pack ());
      واپس سچ;
    }
    ورنہ
    {
      c->نکالو_discard ();
      واپس جھوٹ;
    }
  }

  ساکن اندرونی ثابت HBINT16& get_short (ثابت قدر* value, منطقی *worked=خالی_اشاریہ)
  {
    اگر (worked) *worked |= منطقی (*value);
    واپس *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عوامی:

  منطقی has_device () ثابت
  {
    غیرعلامتی صحیح format = *یہ;
    واپس (format & devices) != 0;
  }

  منطقی sanitize_قدر (hb_sanitize_context_t *c, ثابت قدر_بنیاد *بنیاد, ثابت قدر *values) ثابت
  {
    TRACE_SANITIZE (یہ);

    اگر (unlikely (!c->check_range (values, get_سائز ()))) return_trace (جھوٹ);

    اگر (c->lazy_some_gpos)
      return_trace (سچ);

    return_trace (!has_device () || sanitize_قدر_devices (c, بنیاد, values));
  }

  منطقی sanitize_اقدار (hb_sanitize_context_t *c, ثابت قدر_بنیاد *بنیاد, ثابت قدر *values, غیرعلامتی صحیح گنتی) ثابت
  {
    TRACE_SANITIZE (یہ);
    غیرعلامتی سائز = get_سائز ();

    اگر (!c->check_range (values, گنتی, سائز)) return_trace (جھوٹ);

    اگر (c->lazy_some_gpos)
      return_trace (سچ);

    hb_barrier ();
    return_trace (sanitize_اقدار_stride_unsafe (c, بنیاد, values, گنتی, سائز));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقی sanitize_اقدار_stride_unsafe (hb_sanitize_context_t *c, ثابت قدر_بنیاد *بنیاد, ثابت قدر *values, غیرعلامتی صحیح گنتی, غیرعلامتی صحیح stride) ثابت
  {
    TRACE_SANITIZE (یہ);

    اگر (!has_device ()) return_trace (سچ);

    برائے (غیرعلامتی صحیح i = 0; i < گنتی; i++) {
      اگر (!sanitize_قدر_devices (c, بنیاد, values))
        return_trace (جھوٹ);
      values = &Struct_At_انحراف<ثابت قدر> (values, stride);
    }

    return_trace (سچ);
  }

 نجی:

  خالی should_drop (قدر value, Flags flag, غیرعلامتی صحیح* format) ثابت
  {
    اگر (value) واپس;
    *format = *format & ~flag;
  }

  خالی update_var_flag (ثابت قدر* value, Flags flag,
                        غیرعلامتی صحیح* format, ثابت قدر_بنیاد *بنیاد,
                        ثابت hb_hashmap_t<غیرعلامتی, hb_pair_t<غیرعلامتی, صحیح>> *varidx_delta_map) ثابت
  {
    اگر (*value)
    {
      غیرعلامتی varidx = (بنیاد + get_device (value)).get_variation_اشاریہ ();
      hb_pair_t<غیرعلامتی, صحیح> *varidx_delta;
      اگر (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        اگر (varidx_delta->second)
          *format |= flag >> 4;
        اگر (varidx_delta->پہلا != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
          واپس;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#اگر_ختم  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

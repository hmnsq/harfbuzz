#که_نه_تعریف_شوی OT_LAYOUT_GPOS_VALUEFORMAT_HH
#تعریف_کړه OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

تعريفنوع HBUINT16 ارزښت;

بنية ارزښت_بنسټ {}; // Dummy base class tag for OffsetTo<Value> bases.

تعريفنوع Unsized_Array_د<ارزښت> ارزښت_ریکارډ;

بنية ارزښت_Format : HBUINT16
{
  تعداد Flags {
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
#إذا 0
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
  Offset16_منزل<Device>    xPlaDevice;     /* Offset to Device table for
                                         * horizontal placement--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_منزل<Device>    yPlaDevice;     /* Offset to Device table for vertical
                                         * placement--measured from beginning
                                         * of PosTable (may be NULL) */
  Offset16_منزل<Device>    xAdvDevice;     /* Offset to Device table for
                                         * horizontal advance--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_منزل<Device>    yAdvDevice;     /* Offset to Device table for vertical
                                         * advance--measured from beginning of
                                         * PosTable (may be NULL) */
#د_که_پای

  عدد_Type& چلوونکی = (uint16_t i) { v = i; أرجع *هذا; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  بینښه صحيح get_len () ثابت  { أرجع hb_popcount8 ((uint8_t) *هذا); }
  size_t get_اندازه () ثابت { أرجع get_len () * ارزښت::static_اندازه; }

  hb_ویکټور_t<بینښه> get_device_جدول_indices () ثابت {
    بینښه i = 0;
    hb_ویکټور_t<بینښه> پایله;
    بینښه format = *هذا;

    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance)   i++;
    إذا (format & yAdvance)   i++;

    إذا (format & xPlaDevice) پایله.push (i++);
    إذا (format & yPlaDevice) پایله.push (i++);
    إذا (format & xAdvDevice) پایله.push (i++);
    إذا (format & yAdvDevice) پایله.push (i++);

    أرجع پایله;
  }

  منطقي apply_ارزښت (hb_ot_apply_context_t *c,
                    ثابت ارزښت_بنسټ       *بنسټ,
                    ثابت ارزښت           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقي ret = خطأ;
    بینښه صحيح format = *هذا;
    إذا (!format) أرجع ret;

    hb_font_t *font = c->font;
    منطقي horizontal =
#که_نه_تعریف_شوی HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#وإلا
      ریښتیا
#د_که_پای
      ;

    إذا (format & xPlacement)
      glyph_pos.x_دننه_واټن_2 = hb_saturate_جمع_کړه (glyph_pos.x_دننه_واټن_2, font->em_scale_x (get_short (values++, &ret)));
    إذا (format & yPlacement)
      glyph_pos.y_دننه_واټن_2 = hb_saturate_جمع_کړه (glyph_pos.y_دننه_واټن_2, font->em_scale_y (get_short (values++, &ret)));
    إذا (format & xAdvance) {
      إذا (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_جمع_کړه (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    إذا (format & yAdvance) {
      إذا (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    إذا (!has_device ()) أرجع ret;

    منطقي use_x_device = font->x_ppem || font->has_nonzero_coords;
    منطقي use_y_device = font->y_ppem || font->has_nonzero_coords;

    إذا (!use_x_device && !use_y_device) أرجع ret;

    ثابت توکی_Variation_Store &store = c->var_store;
    خپلکاری *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    إذا (format & xPlaDevice)
    {
      إذا (use_x_device)
	glyph_pos.x_دننه_واټن_2 = hb_saturate_جمع_کړه (glyph_pos.x_دننه_واټن_2, get_device (values, &ret, بنسټ, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yPlaDevice)
    {
      إذا (use_y_device)
	glyph_pos.y_دننه_واټن_2 = hb_saturate_جمع_کړه (glyph_pos.y_دننه_واټن_2, get_device (values, &ret, بنسټ, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    إذا (format & xAdvDevice)
    {
      إذا (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_جمع_کړه (glyph_pos.x_advance, get_device (values, &ret, بنسټ, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      إذا (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, بنسټ, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    أرجع ret;
  }

  فراغ position_single (hb_font_t *font,
                        hb_blob_t *جدول_blob,
                        hb_direction_t direction,
                        ثابت ارزښت_بنسټ *بنسټ,
                        ثابت ارزښت *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_بفر_t بفر {};
    بفر.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &بفر, جدول_blob);

    apply_ارزښت (&c, بنسټ, values, pos);
  }

  بینښه صحيح get_effective_format (ثابت ارزښت *values, منطقي strip_hints, منطقي strip_تشه, ثابت ارزښت_بنسټ *بنسټ,
                                     ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map) ثابت
  {
    بینښه صحيح format = *هذا;
    لكل (بینښه flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      إذا (format & flag)
      {
        إذا (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          استمر;
        }
        إذا (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, بنسټ, varidx_delta_map);
          استمر;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        إذا (strip_تشه) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    أرجع format;
  }

  کينډۍ<ډول_نوم Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بینښه صحيح get_effective_format (Iterator it, منطقي strip_hints, منطقي strip_تشه, ثابت ارزښت_بنسټ *بنسټ,
                                     ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map) ثابت {
    بینښه صحيح new_format = 0;

    لكل (ثابت hb_array_t<ثابت ارزښت>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_تشه, بنسټ, varidx_delta_map);

    أرجع new_format;
  }

  فراغ copy_ارزښتونه (hb_serialize_context_t *c,
                    بینښه صحيح new_format,
                    ثابت ارزښت_بنسټ *بنسټ,
                    ثابت ارزښت *values,
                    ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    بینښه صحيح format = *هذا;
    إذا (!format) أرجع;

    HBINT16 *x_placement = تش_اشاره, *y_placement = تش_اشاره, *x_adv = تش_اشاره, *y_adv = تش_اشاره;
    إذا ((format | new_format) & xPlacement)
      x_placement = copy_ارزښت (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (ارزښت));
    إذا ((format | new_format) & yPlacement)
      y_placement = copy_ارزښت (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (ارزښت));
    إذا ((format | new_format) & xAdvance)
      x_adv = copy_ارزښت (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (ارزښت));
    إذا ((format | new_format) & yAdvance)
      y_adv = copy_ارزښت (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (ارزښت));

    إذا (!has_device ())
      أرجع;

    إذا (format & xPlaDevice)
    {
      جمع_کړه_delta_منزل_ارزښت (x_placement, بنسټ, values, layout_variation_idx_delta_map);
      copy_device (c, بنسټ, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    إذا (format & yPlaDevice)
    {
      جمع_کړه_delta_منزل_ارزښت (y_placement, بنسټ, values, layout_variation_idx_delta_map);
      copy_device (c, بنسټ, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    إذا (format & xAdvDevice)
    {
      جمع_کړه_delta_منزل_ارزښت (x_adv, بنسټ, values, layout_variation_idx_delta_map);
      copy_device (c, بنسټ, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    إذا (format & yAdvDevice)
    {
      جمع_کړه_delta_منزل_ارزښت (y_adv, بنسټ, values, layout_variation_idx_delta_map);
      copy_device (c, بنسټ, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_ارزښت (hb_serialize_context_t *c,
                       بینښه صحيح new_format,
                       Flags flag,
                       ارزښت value) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع تش_اشاره;
    أرجع reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ارزښت_بنسټ *بنسټ,
                                  ثابت hb_array_t<ثابت ارزښت>& values) ثابت
  {
    بینښه format = *هذا;
    بینښه i = 0;
    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance) i++;
    إذا (format & yAdvance) i++;
    إذا (format & xPlaDevice)
    {
      (بنسټ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & ارزښت_Format::yPlaDevice)
    {
      (بنسټ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & ارزښت_Format::xAdvDevice)
    {
      (بنسټ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & ارزښت_Format::yAdvDevice)
    {
      (بنسټ + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  خاص:
  منطقي sanitize_ارزښت_devices (hb_sanitize_context_t *c, ثابت ارزښت_بنسټ *بنسټ, ثابت ارزښت *values) ثابت
  {
    بینښه صحيح format = *هذا;

    إذا (format & xPlacement) values++;
    إذا (format & yPlacement) values++;
    إذا (format & xAdvance)   values++;
    إذا (format & yAdvance)   values++;

    إذا ((format & xPlaDevice) && !get_device (values++).sanitize (c, بنسټ)) أرجع خطأ;
    إذا ((format & yPlaDevice) && !get_device (values++).sanitize (c, بنسټ)) أرجع خطأ;
    إذا ((format & xAdvDevice) && !get_device (values++).sanitize (c, بنسټ)) أرجع خطأ;
    إذا ((format & yAdvDevice) && !get_device (values++).sanitize (c, بنسټ)) أرجع خطأ;

    أرجع ریښتیا;
  }

  ساكن مضمن Offset16_منزل<Device, ارزښت_بنسټ>& get_device (ارزښت* value)
  {
    أرجع *static_cast<Offset16_منزل<Device, ارزښت_بنسټ> *> (value);
  }
  ساكن مضمن ثابت Offset16_منزل<Device, ارزښت_بنسټ>& get_device (ثابت ارزښت* value)
  {
    أرجع *static_cast<ثابت Offset16_منزل<Device, ارزښت_بنسټ> *> (value);
  }
  ساكن مضمن ثابت Device& get_device (ثابت ارزښت* value,
					  منطقي *worked,
					  ثابت ارزښت_بنسټ *بنسټ,
					  hb_sanitize_context_t &c)
  {
    إذا (worked) *worked |= منطقي (*value);
    خپلکاری &دننه_واټن = *static_cast<ثابت Offset16_منزل<Device> *> (value);

    إذا (unlikely (!دننه_واټن.sanitize (&c, بنسټ)))
      أرجع Null(Device);
    hb_barrier ();

    أرجع بنسټ + دننه_واټن;
  }

  فراغ جمع_کړه_delta_منزل_ارزښت (HBINT16 *value,
                           ثابت ارزښت_بنسټ *بنسټ,
                           ثابت ارزښت *src_ارزښت,
                           ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    إذا (!value) أرجع;
    بینښه varidx = (بنسټ + get_device (src_ارزښت)).get_variation_شاخص ();
    hb_pair_t<بینښه, صحيح> *varidx_delta;
    إذا (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) أرجع;

    *value += hb_دوهم (*varidx_delta);
  }

  منطقي copy_device (hb_serialize_context_t *c,
                    ثابت ارزښت_بنسټ *بنسټ,
                    ثابت ارزښت *src_ارزښت,
                    ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *layout_variation_idx_delta_map,
                    بینښه صحيح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع ریښتیا;

    ارزښت       *dst_ارزښت = c->copy (*src_ارزښت);

    إذا (!dst_ارزښت) أرجع خطأ;
    إذا (*dst_ارزښت == 0) أرجع ریښتیا;

    *dst_ارزښت = 0;
    c->push ();
    إذا ((بنسټ + get_device (src_ارزښت)).copy (c, layout_variation_idx_delta_map))
    {
      c->جمع_کړه_link (*dst_ارزښت, c->راوباسه_pack ());
      أرجع ریښتیا;
    }
    وإلا
    {
      c->راوباسه_discard ();
      أرجع خطأ;
    }
  }

  ساكن مضمن ثابت HBINT16& get_short (ثابت ارزښت* value, منطقي *worked=تش_اشاره)
  {
    إذا (worked) *worked |= منطقي (*value);
    أرجع *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عام:

  منطقي has_device () ثابت
  {
    بینښه صحيح format = *هذا;
    أرجع (format & devices) != 0;
  }

  منطقي sanitize_ارزښت (hb_sanitize_context_t *c, ثابت ارزښت_بنسټ *بنسټ, ثابت ارزښت *values) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_range (values, get_اندازه ()))) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (ریښتیا);

    return_trace (!has_device () || sanitize_ارزښت_devices (c, بنسټ, values));
  }

  منطقي sanitize_ارزښتونه (hb_sanitize_context_t *c, ثابت ارزښت_بنسټ *بنسټ, ثابت ارزښت *values, بینښه صحيح شمېر) ثابت
  {
    TRACE_SANITIZE (هذا);
    بینښه اندازه = get_اندازه ();

    إذا (!c->check_range (values, شمېر, اندازه)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (ریښتیا);

    hb_barrier ();
    return_trace (sanitize_ارزښتونه_stride_unsafe (c, بنسټ, values, شمېر, اندازه));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقي sanitize_ارزښتونه_stride_unsafe (hb_sanitize_context_t *c, ثابت ارزښت_بنسټ *بنسټ, ثابت ارزښت *values, بینښه صحيح شمېر, بینښه صحيح stride) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!has_device ()) return_trace (ریښتیا);

    لكل (بینښه صحيح i = 0; i < شمېر; i++) {
      إذا (!sanitize_ارزښت_devices (c, بنسټ, values))
        return_trace (خطأ);
      values = &Struct_At_دننه_واټن<ثابت ارزښت> (values, stride);
    }

    return_trace (ریښتیا);
  }

 خاص:

  فراغ should_drop (ارزښت value, Flags flag, بینښه صحيح* format) ثابت
  {
    إذا (value) أرجع;
    *format = *format & ~flag;
  }

  فراغ update_var_flag (ثابت ارزښت* value, Flags flag,
                        بینښه صحيح* format, ثابت ارزښت_بنسټ *بنسټ,
                        ثابت hb_hashmap_t<بینښه, hb_pair_t<بینښه, صحيح>> *varidx_delta_map) ثابت
  {
    إذا (*value)
    {
      بینښه varidx = (بنسټ + get_device (value)).get_variation_شاخص ();
      hb_pair_t<بینښه, صحيح> *varidx_delta;
      إذا (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        إذا (varidx_delta->second)
          *format |= flag >> 4;
        إذا (varidx_delta->لومړی != HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
          أرجع;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#د_که_پای  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

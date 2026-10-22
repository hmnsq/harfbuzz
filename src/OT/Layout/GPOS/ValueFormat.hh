#اگر_تعریف_نشده OT_LAYOUT_GPOS_VALUEFORMAT_HH
#تعریف_کن OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

تعريفنوع HBUINT16 مقدار;

بنية مقدار_پایه {}; // Dummy base class tag for OffsetTo<Value> bases.

تعريفنوع Unsized_Array_از<مقدار> مقدار_رکورد;

بنية مقدار_Format : HBUINT16
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
#پایان_اگر

  رقم_Type& عملگر = (uint16_t i) { v = i; أرجع *هذا; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  بیعلامت صحيح get_len () ثابت  { أرجع hb_popcount8 ((uint8_t) *هذا); }
  size_t get_اندازه () ثابت { أرجع get_len () * مقدار::static_اندازه; }

  hb_وکتور_t<بیعلامت> get_device_جدول_indices () ثابت {
    بیعلامت i = 0;
    hb_وکتور_t<بیعلامت> نتیجه;
    بیعلامت format = *هذا;

    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance)   i++;
    إذا (format & yAdvance)   i++;

    إذا (format & xPlaDevice) نتیجه.push (i++);
    إذا (format & yPlaDevice) نتیجه.push (i++);
    إذا (format & xAdvDevice) نتیجه.push (i++);
    إذا (format & yAdvDevice) نتیجه.push (i++);

    أرجع نتیجه;
  }

  منطقي apply_مقدار (hb_ot_apply_context_t *c,
                    ثابت مقدار_پایه       *پایه,
                    ثابت مقدار           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقي ret = خطأ;
    بیعلامت صحيح format = *هذا;
    إذا (!format) أرجع ret;

    hb_font_t *font = c->font;
    منطقي horizontal =
#اگر_تعریف_نشده HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#وإلا
      درست
#پایان_اگر
      ;

    إذا (format & xPlacement)
      glyph_pos.x_جابه_جایی_2 = hb_saturate_جمع_کن (glyph_pos.x_جابه_جایی_2, font->em_scale_x (get_short (values++, &ret)));
    إذا (format & yPlacement)
      glyph_pos.y_جابه_جایی_2 = hb_saturate_جمع_کن (glyph_pos.y_جابه_جایی_2, font->em_scale_y (get_short (values++, &ret)));
    إذا (format & xAdvance) {
      إذا (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_جمع_کن (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
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

    ثابت قلم_Variation_Store &store = c->var_store;
    خودکار *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    إذا (format & xPlaDevice)
    {
      إذا (use_x_device)
	glyph_pos.x_جابه_جایی_2 = hb_saturate_جمع_کن (glyph_pos.x_جابه_جایی_2, get_device (values, &ret, پایه, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yPlaDevice)
    {
      إذا (use_y_device)
	glyph_pos.y_جابه_جایی_2 = hb_saturate_جمع_کن (glyph_pos.y_جابه_جایی_2, get_device (values, &ret, پایه, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    إذا (format & xAdvDevice)
    {
      إذا (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_جمع_کن (glyph_pos.x_advance, get_device (values, &ret, پایه, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      إذا (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, پایه, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    أرجع ret;
  }

  فراغ position_single (hb_font_t *font,
                        hb_blob_t *جدول_blob,
                        hb_direction_t direction,
                        ثابت مقدار_پایه *پایه,
                        ثابت مقدار *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_میانگیر_t میانگیر {};
    میانگیر.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &میانگیر, جدول_blob);

    apply_مقدار (&c, پایه, values, pos);
  }

  بیعلامت صحيح get_effective_format (ثابت مقدار *values, منطقي strip_hints, منطقي strip_خالی, ثابت مقدار_پایه *پایه,
                                     ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map) ثابت
  {
    بیعلامت صحيح format = *هذا;
    لكل (بیعلامت flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
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
          update_var_flag (values++, (Flags) flag, &format, پایه, varidx_delta_map);
          استمر;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        إذا (strip_خالی) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    أرجع format;
  }

  قالب<نام_نوع Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بیعلامت صحيح get_effective_format (Iterator it, منطقي strip_hints, منطقي strip_خالی, ثابت مقدار_پایه *پایه,
                                     ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map) ثابت {
    بیعلامت صحيح new_format = 0;

    لكل (ثابت hb_array_t<ثابت مقدار>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_خالی, پایه, varidx_delta_map);

    أرجع new_format;
  }

  فراغ copy_مقادیر (hb_serialize_context_t *c,
                    بیعلامت صحيح new_format,
                    ثابت مقدار_پایه *پایه,
                    ثابت مقدار *values,
                    ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    بیعلامت صحيح format = *هذا;
    إذا (!format) أرجع;

    HBINT16 *x_placement = اشاره_تهی, *y_placement = اشاره_تهی, *x_adv = اشاره_تهی, *y_adv = اشاره_تهی;
    إذا ((format | new_format) & xPlacement)
      x_placement = copy_مقدار (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (مقدار));
    إذا ((format | new_format) & yPlacement)
      y_placement = copy_مقدار (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (مقدار));
    إذا ((format | new_format) & xAdvance)
      x_adv = copy_مقدار (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (مقدار));
    إذا ((format | new_format) & yAdvance)
      y_adv = copy_مقدار (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (مقدار));

    إذا (!has_device ())
      أرجع;

    إذا (format & xPlaDevice)
    {
      جمع_کن_delta_مقصد_مقدار (x_placement, پایه, values, layout_variation_idx_delta_map);
      copy_device (c, پایه, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    إذا (format & yPlaDevice)
    {
      جمع_کن_delta_مقصد_مقدار (y_placement, پایه, values, layout_variation_idx_delta_map);
      copy_device (c, پایه, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    إذا (format & xAdvDevice)
    {
      جمع_کن_delta_مقصد_مقدار (x_adv, پایه, values, layout_variation_idx_delta_map);
      copy_device (c, پایه, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    إذا (format & yAdvDevice)
    {
      جمع_کن_delta_مقصد_مقدار (y_adv, پایه, values, layout_variation_idx_delta_map);
      copy_device (c, پایه, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_مقدار (hb_serialize_context_t *c,
                       بیعلامت صحيح new_format,
                       Flags flag,
                       مقدار value) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع اشاره_تهی;
    أرجع reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_پایه *پایه,
                                  ثابت hb_array_t<ثابت مقدار>& values) ثابت
  {
    بیعلامت format = *هذا;
    بیعلامت i = 0;
    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance) i++;
    إذا (format & yAdvance) i++;
    إذا (format & xPlaDevice)
    {
      (پایه + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & مقدار_Format::yPlaDevice)
    {
      (پایه + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & مقدار_Format::xAdvDevice)
    {
      (پایه + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & مقدار_Format::yAdvDevice)
    {
      (پایه + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  خاص:
  منطقي sanitize_مقدار_devices (hb_sanitize_context_t *c, ثابت مقدار_پایه *پایه, ثابت مقدار *values) ثابت
  {
    بیعلامت صحيح format = *هذا;

    إذا (format & xPlacement) values++;
    إذا (format & yPlacement) values++;
    إذا (format & xAdvance)   values++;
    إذا (format & yAdvance)   values++;

    إذا ((format & xPlaDevice) && !get_device (values++).sanitize (c, پایه)) أرجع خطأ;
    إذا ((format & yPlaDevice) && !get_device (values++).sanitize (c, پایه)) أرجع خطأ;
    إذا ((format & xAdvDevice) && !get_device (values++).sanitize (c, پایه)) أرجع خطأ;
    إذا ((format & yAdvDevice) && !get_device (values++).sanitize (c, پایه)) أرجع خطأ;

    أرجع درست;
  }

  ساكن مضمن Offset16_مقصد<Device, مقدار_پایه>& get_device (مقدار* value)
  {
    أرجع *static_cast<Offset16_مقصد<Device, مقدار_پایه> *> (value);
  }
  ساكن مضمن ثابت Offset16_مقصد<Device, مقدار_پایه>& get_device (ثابت مقدار* value)
  {
    أرجع *static_cast<ثابت Offset16_مقصد<Device, مقدار_پایه> *> (value);
  }
  ساكن مضمن ثابت Device& get_device (ثابت مقدار* value,
					  منطقي *worked,
					  ثابت مقدار_پایه *پایه,
					  hb_sanitize_context_t &c)
  {
    إذا (worked) *worked |= منطقي (*value);
    خودکار &جابه_جایی = *static_cast<ثابت Offset16_مقصد<Device> *> (value);

    إذا (unlikely (!جابه_جایی.sanitize (&c, پایه)))
      أرجع Null(Device);
    hb_barrier ();

    أرجع پایه + جابه_جایی;
  }

  فراغ جمع_کن_delta_مقصد_مقدار (HBINT16 *value,
                           ثابت مقدار_پایه *پایه,
                           ثابت مقدار *src_مقدار,
                           ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    إذا (!value) أرجع;
    بیعلامت varidx = (پایه + get_device (src_مقدار)).get_variation_شاخص ();
    hb_pair_t<بیعلامت, صحيح> *varidx_delta;
    إذا (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) أرجع;

    *value += hb_دومی (*varidx_delta);
  }

  منطقي copy_device (hb_serialize_context_t *c,
                    ثابت مقدار_پایه *پایه,
                    ثابت مقدار *src_مقدار,
                    ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *layout_variation_idx_delta_map,
                    بیعلامت صحيح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع درست;

    مقدار       *dst_مقدار = c->copy (*src_مقدار);

    إذا (!dst_مقدار) أرجع خطأ;
    إذا (*dst_مقدار == 0) أرجع درست;

    *dst_مقدار = 0;
    c->push ();
    إذا ((پایه + get_device (src_مقدار)).copy (c, layout_variation_idx_delta_map))
    {
      c->جمع_کن_link (*dst_مقدار, c->برداشت_pack ());
      أرجع درست;
    }
    وإلا
    {
      c->برداشت_discard ();
      أرجع خطأ;
    }
  }

  ساكن مضمن ثابت HBINT16& get_short (ثابت مقدار* value, منطقي *worked=اشاره_تهی)
  {
    إذا (worked) *worked |= منطقي (*value);
    أرجع *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عام:

  منطقي has_device () ثابت
  {
    بیعلامت صحيح format = *هذا;
    أرجع (format & devices) != 0;
  }

  منطقي sanitize_مقدار (hb_sanitize_context_t *c, ثابت مقدار_پایه *پایه, ثابت مقدار *values) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_range (values, get_اندازه ()))) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (درست);

    return_trace (!has_device () || sanitize_مقدار_devices (c, پایه, values));
  }

  منطقي sanitize_مقادیر (hb_sanitize_context_t *c, ثابت مقدار_پایه *پایه, ثابت مقدار *values, بیعلامت صحيح شمار) ثابت
  {
    TRACE_SANITIZE (هذا);
    بیعلامت اندازه = get_اندازه ();

    إذا (!c->check_range (values, شمار, اندازه)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (درست);

    hb_barrier ();
    return_trace (sanitize_مقادیر_stride_unsafe (c, پایه, values, شمار, اندازه));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقي sanitize_مقادیر_stride_unsafe (hb_sanitize_context_t *c, ثابت مقدار_پایه *پایه, ثابت مقدار *values, بیعلامت صحيح شمار, بیعلامت صحيح stride) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!has_device ()) return_trace (درست);

    لكل (بیعلامت صحيح i = 0; i < شمار; i++) {
      إذا (!sanitize_مقدار_devices (c, پایه, values))
        return_trace (خطأ);
      values = &Struct_At_جابه_جایی<ثابت مقدار> (values, stride);
    }

    return_trace (درست);
  }

 خاص:

  فراغ should_drop (مقدار value, Flags flag, بیعلامت صحيح* format) ثابت
  {
    إذا (value) أرجع;
    *format = *format & ~flag;
  }

  فراغ update_var_flag (ثابت مقدار* value, Flags flag,
                        بیعلامت صحيح* format, ثابت مقدار_پایه *پایه,
                        ثابت hb_hashmap_t<بیعلامت, hb_pair_t<بیعلامت, صحيح>> *varidx_delta_map) ثابت
  {
    إذا (*value)
    {
      بیعلامت varidx = (پایه + get_device (value)).get_variation_شاخص ();
      hb_pair_t<بیعلامت, صحيح> *varidx_delta;
      إذا (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        إذا (varidx_delta->second)
          *format |= flag >> 4;
        إذا (varidx_delta->اولی != HB_OT_LAYOUT_NO_VARIATIONS_شاخص)
          أرجع;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#پایان_اگر  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

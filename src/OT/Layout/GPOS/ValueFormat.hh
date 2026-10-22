#جے_بنا_تعریف OT_LAYOUT_GPOS_VALUEFORMAT_HH
#تعریف_کرو OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

تعريفنوع HBUINT16 قدر;

بنية قدر_ادھار {}; // Dummy base class tag for OffsetTo<Value> bases.

تعريفنوع Unsized_Array_دا<قدر> قدر_ریکارڈ;

بنية قدر_Format : HBUINT16
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
#جے_انت

  عدد_Type& عامل = (uint16_t i) { v = i; أرجع *هذا; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  بنانشانی صحيح get_len () ثابت  { أرجع hb_popcount8 ((uint8_t) *هذا); }
  size_t get_ناپ () ثابت { أرجع get_len () * قدر::static_ناپ; }

  hb_سمتیہ_t<بنانشانی> get_device_جدول_indices () ثابت {
    بنانشانی i = 0;
    hb_سمتیہ_t<بنانشانی> نتیجہ;
    بنانشانی format = *هذا;

    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance)   i++;
    إذا (format & yAdvance)   i++;

    إذا (format & xPlaDevice) نتیجہ.push (i++);
    إذا (format & yPlaDevice) نتیجہ.push (i++);
    إذا (format & xAdvDevice) نتیجہ.push (i++);
    إذا (format & yAdvDevice) نتیجہ.push (i++);

    أرجع نتیجہ;
  }

  منطقي apply_قدر (hb_ot_apply_context_t *c,
                    ثابت قدر_ادھار       *ادھار,
                    ثابت قدر           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقي ret = خطأ;
    بنانشانی صحيح format = *هذا;
    إذا (!format) أرجع ret;

    hb_font_t *font = c->font;
    منطقي horizontal =
#جے_بنا_تعریف HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#وإلا
      سچ
#جے_انت
      ;

    إذا (format & xPlacement)
      glyph_pos.x_وتھ_2 = hb_saturate_جوڑو (glyph_pos.x_وتھ_2, font->em_scale_x (get_short (values++, &ret)));
    إذا (format & yPlacement)
      glyph_pos.y_وتھ_2 = hb_saturate_جوڑو (glyph_pos.y_وتھ_2, font->em_scale_y (get_short (values++, &ret)));
    إذا (format & xAdvance) {
      إذا (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_جوڑو (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
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

    ثابت شے_Variation_Store &store = c->var_store;
    خودکار *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    إذا (format & xPlaDevice)
    {
      إذا (use_x_device)
	glyph_pos.x_وتھ_2 = hb_saturate_جوڑو (glyph_pos.x_وتھ_2, get_device (values, &ret, ادھار, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yPlaDevice)
    {
      إذا (use_y_device)
	glyph_pos.y_وتھ_2 = hb_saturate_جوڑو (glyph_pos.y_وتھ_2, get_device (values, &ret, ادھار, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    إذا (format & xAdvDevice)
    {
      إذا (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_جوڑو (glyph_pos.x_advance, get_device (values, &ret, ادھار, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      إذا (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, ادھار, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    أرجع ret;
  }

  فراغ position_single (hb_font_t *font,
                        hb_blob_t *جدول_blob,
                        hb_direction_t direction,
                        ثابت قدر_ادھار *ادھار,
                        ثابت قدر *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_بفر_t بفر {};
    بفر.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &بفر, جدول_blob);

    apply_قدر (&c, ادھار, values, pos);
  }

  بنانشانی صحيح get_effective_format (ثابت قدر *values, منطقي strip_hints, منطقي strip_خالی_تھاواں, ثابت قدر_ادھار *ادھار,
                                     ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map) ثابت
  {
    بنانشانی صحيح format = *هذا;
    لكل (بنانشانی flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
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
          update_var_flag (values++, (Flags) flag, &format, ادھار, varidx_delta_map);
          استمر;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        إذا (strip_خالی_تھاواں) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    أرجع format;
  }

  سانچا<قسم_ناں Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  بنانشانی صحيح get_effective_format (Iterator it, منطقي strip_hints, منطقي strip_خالی_تھاواں, ثابت قدر_ادھار *ادھار,
                                     ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map) ثابت {
    بنانشانی صحيح new_format = 0;

    لكل (ثابت hb_array_t<ثابت قدر>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_خالی_تھاواں, ادھار, varidx_delta_map);

    أرجع new_format;
  }

  فراغ copy_اقدار (hb_serialize_context_t *c,
                    بنانشانی صحيح new_format,
                    ثابت قدر_ادھار *ادھار,
                    ثابت قدر *values,
                    ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    بنانشانی صحيح format = *هذا;
    إذا (!format) أرجع;

    HBINT16 *x_placement = خالی_سوچک, *y_placement = خالی_سوچک, *x_adv = خالی_سوچک, *y_adv = خالی_سوچک;
    إذا ((format | new_format) & xPlacement)
      x_placement = copy_قدر (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (قدر));
    إذا ((format | new_format) & yPlacement)
      y_placement = copy_قدر (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (قدر));
    إذا ((format | new_format) & xAdvance)
      x_adv = copy_قدر (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (قدر));
    إذا ((format | new_format) & yAdvance)
      y_adv = copy_قدر (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (قدر));

    إذا (!has_device ())
      أرجع;

    إذا (format & xPlaDevice)
    {
      جوڑو_delta_منزل_قدر (x_placement, ادھار, values, layout_variation_idx_delta_map);
      copy_device (c, ادھار, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    إذا (format & yPlaDevice)
    {
      جوڑو_delta_منزل_قدر (y_placement, ادھار, values, layout_variation_idx_delta_map);
      copy_device (c, ادھار, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    إذا (format & xAdvDevice)
    {
      جوڑو_delta_منزل_قدر (x_adv, ادھار, values, layout_variation_idx_delta_map);
      copy_device (c, ادھار, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    إذا (format & yAdvDevice)
    {
      جوڑو_delta_منزل_قدر (y_adv, ادھار, values, layout_variation_idx_delta_map);
      copy_device (c, ادھار, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_قدر (hb_serialize_context_t *c,
                       بنانشانی صحيح new_format,
                       Flags flag,
                       قدر value) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع خالی_سوچک;
    أرجع reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت قدر_ادھار *ادھار,
                                  ثابت hb_array_t<ثابت قدر>& values) ثابت
  {
    بنانشانی format = *هذا;
    بنانشانی i = 0;
    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance) i++;
    إذا (format & yAdvance) i++;
    إذا (format & xPlaDevice)
    {
      (ادھار + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & قدر_Format::yPlaDevice)
    {
      (ادھار + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & قدر_Format::xAdvDevice)
    {
      (ادھار + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & قدر_Format::yAdvDevice)
    {
      (ادھار + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  خاص:
  منطقي sanitize_قدر_devices (hb_sanitize_context_t *c, ثابت قدر_ادھار *ادھار, ثابت قدر *values) ثابت
  {
    بنانشانی صحيح format = *هذا;

    إذا (format & xPlacement) values++;
    إذا (format & yPlacement) values++;
    إذا (format & xAdvance)   values++;
    إذا (format & yAdvance)   values++;

    إذا ((format & xPlaDevice) && !get_device (values++).sanitize (c, ادھار)) أرجع خطأ;
    إذا ((format & yPlaDevice) && !get_device (values++).sanitize (c, ادھار)) أرجع خطأ;
    إذا ((format & xAdvDevice) && !get_device (values++).sanitize (c, ادھار)) أرجع خطأ;
    إذا ((format & yAdvDevice) && !get_device (values++).sanitize (c, ادھار)) أرجع خطأ;

    أرجع سچ;
  }

  ساكن مضمن Offset16_منزل<Device, قدر_ادھار>& get_device (قدر* value)
  {
    أرجع *static_cast<Offset16_منزل<Device, قدر_ادھار> *> (value);
  }
  ساكن مضمن ثابت Offset16_منزل<Device, قدر_ادھار>& get_device (ثابت قدر* value)
  {
    أرجع *static_cast<ثابت Offset16_منزل<Device, قدر_ادھار> *> (value);
  }
  ساكن مضمن ثابت Device& get_device (ثابت قدر* value,
					  منطقي *worked,
					  ثابت قدر_ادھار *ادھار,
					  hb_sanitize_context_t &c)
  {
    إذا (worked) *worked |= منطقي (*value);
    خودکار &وتھ = *static_cast<ثابت Offset16_منزل<Device> *> (value);

    إذا (unlikely (!وتھ.sanitize (&c, ادھار)))
      أرجع Null(Device);
    hb_barrier ();

    أرجع ادھار + وتھ;
  }

  فراغ جوڑو_delta_منزل_قدر (HBINT16 *value,
                           ثابت قدر_ادھار *ادھار,
                           ثابت قدر *src_قدر,
                           ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    إذا (!value) أرجع;
    بنانشانی varidx = (ادھار + get_device (src_قدر)).get_variation_اشاریہ ();
    hb_pair_t<بنانشانی, صحيح> *varidx_delta;
    إذا (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) أرجع;

    *value += hb_دوجا (*varidx_delta);
  }

  منطقي copy_device (hb_serialize_context_t *c,
                    ثابت قدر_ادھار *ادھار,
                    ثابت قدر *src_قدر,
                    ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *layout_variation_idx_delta_map,
                    بنانشانی صحيح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع سچ;

    قدر       *dst_قدر = c->copy (*src_قدر);

    إذا (!dst_قدر) أرجع خطأ;
    إذا (*dst_قدر == 0) أرجع سچ;

    *dst_قدر = 0;
    c->push ();
    إذا ((ادھار + get_device (src_قدر)).copy (c, layout_variation_idx_delta_map))
    {
      c->جوڑو_link (*dst_قدر, c->نکالو_pack ());
      أرجع سچ;
    }
    وإلا
    {
      c->نکالو_discard ();
      أرجع خطأ;
    }
  }

  ساكن مضمن ثابت HBINT16& get_short (ثابت قدر* value, منطقي *worked=خالی_سوچک)
  {
    إذا (worked) *worked |= منطقي (*value);
    أرجع *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عام:

  منطقي has_device () ثابت
  {
    بنانشانی صحيح format = *هذا;
    أرجع (format & devices) != 0;
  }

  منطقي sanitize_قدر (hb_sanitize_context_t *c, ثابت قدر_ادھار *ادھار, ثابت قدر *values) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_range (values, get_ناپ ()))) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (سچ);

    return_trace (!has_device () || sanitize_قدر_devices (c, ادھار, values));
  }

  منطقي sanitize_اقدار (hb_sanitize_context_t *c, ثابت قدر_ادھار *ادھار, ثابت قدر *values, بنانشانی صحيح گنتی) ثابت
  {
    TRACE_SANITIZE (هذا);
    بنانشانی ناپ = get_ناپ ();

    إذا (!c->check_range (values, گنتی, ناپ)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (سچ);

    hb_barrier ();
    return_trace (sanitize_اقدار_stride_unsafe (c, ادھار, values, گنتی, ناپ));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقي sanitize_اقدار_stride_unsafe (hb_sanitize_context_t *c, ثابت قدر_ادھار *ادھار, ثابت قدر *values, بنانشانی صحيح گنتی, بنانشانی صحيح stride) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!has_device ()) return_trace (سچ);

    لكل (بنانشانی صحيح i = 0; i < گنتی; i++) {
      إذا (!sanitize_قدر_devices (c, ادھار, values))
        return_trace (خطأ);
      values = &Struct_At_وتھ<ثابت قدر> (values, stride);
    }

    return_trace (سچ);
  }

 خاص:

  فراغ should_drop (قدر value, Flags flag, بنانشانی صحيح* format) ثابت
  {
    إذا (value) أرجع;
    *format = *format & ~flag;
  }

  فراغ update_var_flag (ثابت قدر* value, Flags flag,
                        بنانشانی صحيح* format, ثابت قدر_ادھار *ادھار,
                        ثابت hb_hashmap_t<بنانشانی, hb_pair_t<بنانشانی, صحيح>> *varidx_delta_map) ثابت
  {
    إذا (*value)
    {
      بنانشانی varidx = (ادھار + get_device (value)).get_variation_اشاریہ ();
      hb_pair_t<بنانشانی, صحيح> *varidx_delta;
      إذا (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        إذا (varidx_delta->second)
          *format |= flag >> 4;
        إذا (varidx_delta->پہلا != HB_OT_LAYOUT_NO_VARIATIONS_اشاریہ)
          أرجع;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#جے_انت  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#شامل "../../../hb-ot-layout-gsubgpos.hh"

فضاینام OT {
فضاینام Layout {
فضاینام GPOS_impl {

تعریفنوع HBUINT16 مقدار;

ساختار مقدار_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

تعریفنوع UnsizedArrayOf<مقدار> مقدار_Record;

ساختار مقدار_Format : HBUINT16
{
  شمارش Flags {
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

  رقم_Type& operator = (uint16_t i) { v = i; بازگشت *این; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned صحیح get_len () ثابت  { بازگشت hb_popcount8 ((uint8_t) *این); }
  size_t get_size () ثابت { بازگشت get_len () * مقدار::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ثابت {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *این;

    اگر (format & xPlacement) i++;
    اگر (format & yPlacement) i++;
    اگر (format & xAdvance)   i++;
    اگر (format & yAdvance)   i++;

    اگر (format & xPlaDevice) result.push (i++);
    اگر (format & yPlaDevice) result.push (i++);
    اگر (format & xAdvDevice) result.push (i++);
    اگر (format & yAdvDevice) result.push (i++);

    بازگشت result;
  }

  منطقی apply_مقدار (hb_ot_apply_context_t *c,
                    ثابت مقدار_Base       *base,
                    ثابت مقدار           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقی ret = نادرست;
    unsigned صحیح format = *این;
    اگر (!format) بازگشت ret;

    hb_font_t *font = c->font;
    منطقی horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#وگرنه
      درست
#endif
      ;

    اگر (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    اگر (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    اگر (format & xAdvance) {
      اگر (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    اگر (format & yAdvance) {
      اگر (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    اگر (!has_device ()) بازگشت ret;

    منطقی use_x_device = font->x_ppem || font->has_nonzero_coords;
    منطقی use_y_device = font->y_ppem || font->has_nonzero_coords;

    اگر (!use_x_device && !use_y_device) بازگشت ret;

    ثابت ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    اگر (format & xPlaDevice)
    {
      اگر (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    اگر (format & yPlaDevice)
    {
      اگر (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    اگر (format & xAdvDevice)
    {
      اگر (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    اگر (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      اگر (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    بازگشت ret;
  }

  تهی position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ثابت مقدار_Base *base,
                        ثابت مقدار *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_مقدار (&c, base, values, pos);
  }

  unsigned صحیح get_effective_format (ثابت مقدار *values, منطقی strip_hints, منطقی strip_empty, ثابت مقدار_Base *base,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map) ثابت
  {
    unsigned صحیح format = *این;
    برای (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      اگر (format & flag)
      {
        اگر (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          ادامه;
        }
        اگر (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          ادامه;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        اگر (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    بازگشت format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned صحیح get_effective_format (Iterator it, منطقی strip_hints, منطقی strip_empty, ثابت مقدار_Base *base,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map) ثابت {
    unsigned صحیح new_format = 0;

    برای (ثابت hb_array_t<ثابت مقدار>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    بازگشت new_format;
  }

  تهی copy_مقادیر (hb_serialize_context_t *c,
                    unsigned صحیح new_format,
                    ثابت مقدار_Base *base,
                    ثابت مقدار *values,
                    ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map) ثابت
  {
    unsigned صحیح format = *این;
    اگر (!format) بازگشت;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    اگر ((format | new_format) & xPlacement)
      x_placement = copy_مقدار (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (مقدار));
    اگر ((format | new_format) & yPlacement)
      y_placement = copy_مقدار (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (مقدار));
    اگر ((format | new_format) & xAdvance)
      x_adv = copy_مقدار (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (مقدار));
    اگر ((format | new_format) & yAdvance)
      y_adv = copy_مقدار (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (مقدار));

    اگر (!has_device ())
      بازگشت;

    اگر (format & xPlaDevice)
    {
      add_delta_to_مقدار (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    اگر (format & yPlaDevice)
    {
      add_delta_to_مقدار (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    اگر (format & xAdvDevice)
    {
      add_delta_to_مقدار (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    اگر (format & yAdvDevice)
    {
      add_delta_to_مقدار (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_مقدار (hb_serialize_context_t *c,
                       unsigned صحیح new_format,
                       Flags flag,
                       مقدار value) ثابت
  {
    // Filter by new format.
    اگر (!(new_format & flag)) بازگشت nullptr;
    بازگشت reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  تهی collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت مقدار_Base *base,
                                  ثابت hb_array_t<ثابت مقدار>& values) ثابت
  {
    unsigned format = *این;
    unsigned i = 0;
    اگر (format & xPlacement) i++;
    اگر (format & yPlacement) i++;
    اگر (format & xAdvance) i++;
    اگر (format & yAdvance) i++;
    اگر (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    اگر (format & مقدار_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    اگر (format & مقدار_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    اگر (format & مقدار_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  خصوصی:
  منطقی sanitize_مقدار_devices (hb_sanitize_context_t *c, ثابت مقدار_Base *base, ثابت مقدار *values) ثابت
  {
    unsigned صحیح format = *این;

    اگر (format & xPlacement) values++;
    اگر (format & yPlacement) values++;
    اگر (format & xAdvance)   values++;
    اگر (format & yAdvance)   values++;

    اگر ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) بازگشت نادرست;
    اگر ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) بازگشت نادرست;
    اگر ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) بازگشت نادرست;
    اگر ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) بازگشت نادرست;

    بازگشت درست;
  }

  ایستا درونخطی Offset16To<Device, مقدار_Base>& get_device (مقدار* value)
  {
    بازگشت *static_cast<Offset16To<Device, مقدار_Base> *> (value);
  }
  ایستا درونخطی ثابت Offset16To<Device, مقدار_Base>& get_device (ثابت مقدار* value)
  {
    بازگشت *static_cast<ثابت Offset16To<Device, مقدار_Base> *> (value);
  }
  ایستا درونخطی ثابت Device& get_device (ثابت مقدار* value,
					  منطقی *worked,
					  ثابت مقدار_Base *base,
					  hb_sanitize_context_t &c)
  {
    اگر (worked) *worked |= منطقی (*value);
    auto &offset = *static_cast<ثابت Offset16To<Device> *> (value);

    اگر (unlikely (!offset.sanitize (&c, base)))
      بازگشت Null(Device);
    hb_barrier ();

    بازگشت base + offset;
  }

  تهی add_delta_to_مقدار (HBINT16 *value,
                           ثابت مقدار_Base *base,
                           ثابت مقدار *src_مقدار,
                           ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map) ثابت
  {
    اگر (!value) بازگشت;
    unsigned varidx = (base + get_device (src_مقدار)).get_variation_index ();
    hb_pair_t<unsigned, صحیح> *varidx_delta;
    اگر (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) بازگشت;

    *value += hb_second (*varidx_delta);
  }

  منطقی copy_device (hb_serialize_context_t *c,
                    ثابت مقدار_Base *base,
                    ثابت مقدار *src_مقدار,
                    ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *layout_variation_idx_delta_map,
                    unsigned صحیح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    اگر (!(new_format & flag)) بازگشت درست;

    مقدار       *dst_مقدار = c->copy (*src_مقدار);

    اگر (!dst_مقدار) بازگشت نادرست;
    اگر (*dst_مقدار == 0) بازگشت درست;

    *dst_مقدار = 0;
    c->push ();
    اگر ((base + get_device (src_مقدار)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_مقدار, c->برداشت_pack ());
      بازگشت درست;
    }
    وگرنه
    {
      c->برداشت_discard ();
      بازگشت نادرست;
    }
  }

  ایستا درونخطی ثابت HBINT16& get_short (ثابت مقدار* value, منطقی *worked=nullptr)
  {
    اگر (worked) *worked |= منطقی (*value);
    بازگشت *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عمومی:

  منطقی has_device () ثابت
  {
    unsigned صحیح format = *این;
    بازگشت (format & devices) != 0;
  }

  منطقی sanitize_مقدار (hb_sanitize_context_t *c, ثابت مقدار_Base *base, ثابت مقدار *values) ثابت
  {
    TRACE_SANITIZE (این);

    اگر (unlikely (!c->check_range (values, get_size ()))) return_trace (نادرست);

    اگر (c->lazy_some_gpos)
      return_trace (درست);

    return_trace (!has_device () || sanitize_مقدار_devices (c, base, values));
  }

  منطقی sanitize_مقادیر (hb_sanitize_context_t *c, ثابت مقدار_Base *base, ثابت مقدار *values, unsigned صحیح شمار) ثابت
  {
    TRACE_SANITIZE (این);
    unsigned size = get_size ();

    اگر (!c->check_range (values, شمار, size)) return_trace (نادرست);

    اگر (c->lazy_some_gpos)
      return_trace (درست);

    hb_barrier ();
    return_trace (sanitize_مقادیر_stride_unsafe (c, base, values, شمار, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقی sanitize_مقادیر_stride_unsafe (hb_sanitize_context_t *c, ثابت مقدار_Base *base, ثابت مقدار *values, unsigned صحیح شمار, unsigned صحیح stride) ثابت
  {
    TRACE_SANITIZE (این);

    اگر (!has_device ()) return_trace (درست);

    برای (unsigned صحیح i = 0; i < شمار; i++) {
      اگر (!sanitize_مقدار_devices (c, base, values))
        return_trace (نادرست);
      values = &StructAtOffset<ثابت مقدار> (values, stride);
    }

    return_trace (درست);
  }

 خصوصی:

  تهی should_drop (مقدار value, Flags flag, unsigned صحیح* format) ثابت
  {
    اگر (value) بازگشت;
    *format = *format & ~flag;
  }

  تهی update_var_flag (ثابت مقدار* value, Flags flag,
                        unsigned صحیح* format, ثابت مقدار_Base *base,
                        ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحیح>> *varidx_delta_map) ثابت
  {
    اگر (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, صحیح> *varidx_delta;
      اگر (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        اگر (varidx_delta->second)
          *format |= flag >> 4;
        اگر (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          بازگشت;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

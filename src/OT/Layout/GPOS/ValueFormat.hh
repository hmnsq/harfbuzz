#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

تعريفنوع HBUINT16 Value;

بنية ValueBase {}; // Dummy base class tag for OffsetTo<Value> bases.

تعريفنوع UnsizedArrayOf<Value> ValueRecord;

بنية ValueFormat : HBUINT16
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
#אם 0
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

  NumType& operator = (uint16_t i) { v = i; החזר *هذا; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned صحيح get_len () ثابت  { החזר hb_popcount8 ((uint8_t) *هذا); }
  size_t get_size () ثابت { החזר get_len () * Value::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ثابت {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *هذا;

    אם (format & xPlacement) i++;
    אם (format & yPlacement) i++;
    אם (format & xAdvance)   i++;
    אם (format & yAdvance)   i++;

    אם (format & xPlaDevice) result.push (i++);
    אם (format & yPlaDevice) result.push (i++);
    אם (format & xAdvDevice) result.push (i++);
    אם (format & yAdvDevice) result.push (i++);

    החזר result;
  }

  منطقي apply_value (hb_ot_apply_context_t *c,
                    ثابت ValueBase       *base,
                    ثابت Value           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقي ret = שקר;
    unsigned صحيح format = *هذا;
    אם (!format) החזר ret;

    hb_font_t *font = c->font;
    منطقي horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#אחרת
      אמת
#endif
      ;

    אם (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    אם (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    אם (format & xAdvance) {
      אם (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    אם (format & yAdvance) {
      אם (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    אם (!has_device ()) החזר ret;

    منطقي use_x_device = font->x_ppem || font->has_nonzero_coords;
    منطقي use_y_device = font->y_ppem || font->has_nonzero_coords;

    אם (!use_x_device && !use_y_device) החזר ret;

    ثابت ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    אם (format & xPlaDevice)
    {
      אם (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    אם (format & yPlaDevice)
    {
      אם (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    אם (format & xAdvDevice)
    {
      אם (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    אם (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      אם (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    החזר ret;
  }

  فراغ position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ثابت ValueBase *base,
                        ثابت Value *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_value (&c, base, values, pos);
  }

  unsigned صحيح get_effective_format (ثابت Value *values, منطقي strip_hints, منطقي strip_empty, ثابت ValueBase *base,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned صحيح format = *هذا;
    עבור (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      אם (format & flag)
      {
        אם (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          استمر;
        }
        אם (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          استمر;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        אם (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    החזר format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned صحيح get_effective_format (Iterator it, منطقي strip_hints, منطقي strip_empty, ثابت ValueBase *base,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت {
    unsigned صحيح new_format = 0;

    עבור (ثابت hb_array_t<ثابت Value>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    החזר new_format;
  }

  فراغ copy_values (hb_serialize_context_t *c,
                    unsigned صحيح new_format,
                    ثابت ValueBase *base,
                    ثابت Value *values,
                    ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    unsigned صحيح format = *هذا;
    אם (!format) החזר;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    אם ((format | new_format) & xPlacement)
      x_placement = copy_value (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (Value));
    אם ((format | new_format) & yPlacement)
      y_placement = copy_value (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (Value));
    אם ((format | new_format) & xAdvance)
      x_adv = copy_value (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (Value));
    אם ((format | new_format) & yAdvance)
      y_adv = copy_value (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (Value));

    אם (!has_device ())
      החזר;

    אם (format & xPlaDevice)
    {
      add_delta_to_value (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    אם (format & yPlaDevice)
    {
      add_delta_to_value (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    אם (format & xAdvDevice)
    {
      add_delta_to_value (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    אם (format & yAdvDevice)
    {
      add_delta_to_value (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_value (hb_serialize_context_t *c,
                       unsigned صحيح new_format,
                       Flags flag,
                       Value value) ثابت
  {
    // Filter by new format.
    אם (!(new_format & flag)) החזר nullptr;
    החזר reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت ValueBase *base,
                                  ثابت hb_array_t<ثابت Value>& values) ثابت
  {
    unsigned format = *هذا;
    unsigned i = 0;
    אם (format & xPlacement) i++;
    אם (format & yPlacement) i++;
    אם (format & xAdvance) i++;
    אם (format & yAdvance) i++;
    אם (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    אם (format & ValueFormat::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    אם (format & ValueFormat::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    אם (format & ValueFormat::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  خاص:
  منطقي sanitize_value_devices (hb_sanitize_context_t *c, ثابت ValueBase *base, ثابت Value *values) ثابت
  {
    unsigned صحيح format = *هذا;

    אם (format & xPlacement) values++;
    אם (format & yPlacement) values++;
    אם (format & xAdvance)   values++;
    אם (format & yAdvance)   values++;

    אם ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) החזר שקר;
    אם ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) החזר שקר;
    אם ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) החזר שקר;
    אם ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) החזר שקר;

    החזר אמת;
  }

  ساكن مضمن Offset16To<Device, ValueBase>& get_device (Value* value)
  {
    החזר *static_cast<Offset16To<Device, ValueBase> *> (value);
  }
  ساكن مضمن ثابت Offset16To<Device, ValueBase>& get_device (ثابت Value* value)
  {
    החזר *static_cast<ثابت Offset16To<Device, ValueBase> *> (value);
  }
  ساكن مضمن ثابت Device& get_device (ثابت Value* value,
					  منطقي *worked,
					  ثابت ValueBase *base,
					  hb_sanitize_context_t &c)
  {
    אם (worked) *worked |= منطقي (*value);
    auto &offset = *static_cast<ثابت Offset16To<Device> *> (value);

    אם (unlikely (!offset.sanitize (&c, base)))
      החזר Null(Device);
    hb_barrier ();

    החזר base + offset;
  }

  فراغ add_delta_to_value (HBINT16 *value,
                           ثابت ValueBase *base,
                           ثابت Value *src_value,
                           ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    אם (!value) החזר;
    unsigned varidx = (base + get_device (src_value)).get_variation_index ();
    hb_pair_t<unsigned, صحيح> *varidx_delta;
    אם (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) החזר;

    *value += hb_second (*varidx_delta);
  }

  منطقي copy_device (hb_serialize_context_t *c,
                    ثابت ValueBase *base,
                    ثابت Value *src_value,
                    ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map,
                    unsigned صحيح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    אם (!(new_format & flag)) החזר אמת;

    Value       *dst_value = c->copy (*src_value);

    אם (!dst_value) החזר שקר;
    אם (*dst_value == 0) החזר אמת;

    *dst_value = 0;
    c->push ();
    אם ((base + get_device (src_value)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_value, c->pop_pack ());
      החזר אמת;
    }
    אחרת
    {
      c->pop_discard ();
      החזר שקר;
    }
  }

  ساكن مضمن ثابت HBINT16& get_short (ثابت Value* value, منطقي *worked=nullptr)
  {
    אם (worked) *worked |= منطقي (*value);
    החזר *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عام:

  منطقي has_device () ثابت
  {
    unsigned صحيح format = *هذا;
    החזר (format & devices) != 0;
  }

  منطقي sanitize_value (hb_sanitize_context_t *c, ثابت ValueBase *base, ثابت Value *values) ثابت
  {
    TRACE_SANITIZE (هذا);

    אם (unlikely (!c->check_range (values, get_size ()))) return_trace (שקר);

    אם (c->lazy_some_gpos)
      return_trace (אמת);

    return_trace (!has_device () || sanitize_value_devices (c, base, values));
  }

  منطقي sanitize_values (hb_sanitize_context_t *c, ثابت ValueBase *base, ثابت Value *values, unsigned صحيح count) ثابت
  {
    TRACE_SANITIZE (هذا);
    unsigned size = get_size ();

    אם (!c->check_range (values, count, size)) return_trace (שקר);

    אם (c->lazy_some_gpos)
      return_trace (אמת);

    hb_barrier ();
    return_trace (sanitize_values_stride_unsafe (c, base, values, count, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقي sanitize_values_stride_unsafe (hb_sanitize_context_t *c, ثابت ValueBase *base, ثابت Value *values, unsigned صحيح count, unsigned صحيح stride) ثابت
  {
    TRACE_SANITIZE (هذا);

    אם (!has_device ()) return_trace (אמת);

    עבור (unsigned صحيح i = 0; i < count; i++) {
      אם (!sanitize_value_devices (c, base, values))
        return_trace (שקר);
      values = &StructAtOffset<ثابت Value> (values, stride);
    }

    return_trace (אמת);
  }

 خاص:

  فراغ should_drop (Value value, Flags flag, unsigned صحيح* format) ثابت
  {
    אם (value) החזר;
    *format = *format & ~flag;
  }

  فراغ update_var_flag (ثابت Value* value, Flags flag,
                        unsigned صحيح* format, ثابت ValueBase *base,
                        ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    אם (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, صحيح> *varidx_delta;
      אם (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        אם (varidx_delta->second)
          *format |= flag >> 4;
        אם (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          החזר;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

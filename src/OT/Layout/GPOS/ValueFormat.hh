#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ضمن "../../../hb-ot-layout-gsubgpos.hh"

نطاق OT {
نطاق Layout {
نطاق GPOS_impl {

تعريفنوع HBUINT16 بەها;

بنية بەها_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

تعريفنوع UnsizedArrayOf<بەها> بەها_Record;

بنية بەها_Format : HBUINT16
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

  ژمارە_Type& operator = (uint16_t i) { v = i; أرجع *هذا; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned صحيح get_len () ثابت  { أرجع hb_popcount8 ((uint8_t) *هذا); }
  size_t get_size () ثابت { أرجع get_len () * بەها::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ثابت {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *هذا;

    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance)   i++;
    إذا (format & yAdvance)   i++;

    إذا (format & xPlaDevice) result.push (i++);
    إذا (format & yPlaDevice) result.push (i++);
    إذا (format & xAdvDevice) result.push (i++);
    إذا (format & yAdvDevice) result.push (i++);

    أرجع result;
  }

  منطقي apply_بەها (hb_ot_apply_context_t *c,
                    ثابت بەها_Base       *base,
                    ثابت بەها           *values,
                    hb_glyph_position_t   &glyph_pos) ثابت
  {
    منطقي ret = خطأ;
    unsigned صحيح format = *هذا;
    إذا (!format) أرجع ret;

    hb_font_t *font = c->font;
    منطقي horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#وإلا
      true
#endif
      ;

    إذا (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    إذا (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    إذا (format & xAdvance) {
      إذا (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
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

    ثابت ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    إذا (format & xPlaDevice)
    {
      إذا (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yPlaDevice)
    {
      إذا (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    إذا (format & xAdvDevice)
    {
      إذا (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    إذا (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      إذا (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    أرجع ret;
  }

  فراغ position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ثابت بەها_Base *base,
                        ثابت بەها *values,
                        hb_glyph_position_t &pos) ثابت
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_بەها (&c, base, values, pos);
  }

  unsigned صحيح get_effective_format (ثابت بەها *values, منطقي strip_hints, منطقي strip_empty, ثابت بەها_Base *base,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    unsigned صحيح format = *هذا;
    لكل (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
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
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          استمر;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        إذا (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    أرجع format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned صحيح get_effective_format (Iterator it, منطقي strip_hints, منطقي strip_empty, ثابت بەها_Base *base,
                                     ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت {
    unsigned صحيح new_format = 0;

    لكل (ثابت hb_array_t<ثابت بەها>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    أرجع new_format;
  }

  فراغ copy_بەهاکان (hb_serialize_context_t *c,
                    unsigned صحيح new_format,
                    ثابت بەها_Base *base,
                    ثابت بەها *values,
                    ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    unsigned صحيح format = *هذا;
    إذا (!format) أرجع;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    إذا ((format | new_format) & xPlacement)
      x_placement = copy_بەها (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (بەها));
    إذا ((format | new_format) & yPlacement)
      y_placement = copy_بەها (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (بەها));
    إذا ((format | new_format) & xAdvance)
      x_adv = copy_بەها (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (بەها));
    إذا ((format | new_format) & yAdvance)
      y_adv = copy_بەها (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (بەها));

    إذا (!has_device ())
      أرجع;

    إذا (format & xPlaDevice)
    {
      add_delta_to_بەها (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    إذا (format & yPlaDevice)
    {
      add_delta_to_بەها (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    إذا (format & xAdvDevice)
    {
      add_delta_to_بەها (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    إذا (format & yAdvDevice)
    {
      add_delta_to_بەها (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_بەها (hb_serialize_context_t *c,
                       unsigned صحيح new_format,
                       Flags flag,
                       بەها value) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع nullptr;
    أرجع reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  فراغ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ثابت بەها_Base *base,
                                  ثابت hb_array_t<ثابت بەها>& values) ثابت
  {
    unsigned format = *هذا;
    unsigned i = 0;
    إذا (format & xPlacement) i++;
    إذا (format & yPlacement) i++;
    إذا (format & xAdvance) i++;
    إذا (format & yAdvance) i++;
    إذا (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & بەها_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & بەها_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    إذا (format & بەها_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  خاص:
  منطقي sanitize_بەها_devices (hb_sanitize_context_t *c, ثابت بەها_Base *base, ثابت بەها *values) ثابت
  {
    unsigned صحيح format = *هذا;

    إذا (format & xPlacement) values++;
    إذا (format & yPlacement) values++;
    إذا (format & xAdvance)   values++;
    إذا (format & yAdvance)   values++;

    إذا ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) أرجع خطأ;
    إذا ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) أرجع خطأ;
    إذا ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) أرجع خطأ;
    إذا ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) أرجع خطأ;

    أرجع true;
  }

  ساكن مضمن Offset16To<Device, بەها_Base>& get_device (بەها* value)
  {
    أرجع *static_cast<Offset16To<Device, بەها_Base> *> (value);
  }
  ساكن مضمن ثابت Offset16To<Device, بەها_Base>& get_device (ثابت بەها* value)
  {
    أرجع *static_cast<ثابت Offset16To<Device, بەها_Base> *> (value);
  }
  ساكن مضمن ثابت Device& get_device (ثابت بەها* value,
					  منطقي *worked,
					  ثابت بەها_Base *base,
					  hb_sanitize_context_t &c)
  {
    إذا (worked) *worked |= منطقي (*value);
    auto &offset = *static_cast<ثابت Offset16To<Device> *> (value);

    إذا (unlikely (!offset.sanitize (&c, base)))
      أرجع Null(Device);
    hb_barrier ();

    أرجع base + offset;
  }

  فراغ add_delta_to_بەها (HBINT16 *value,
                           ثابت بەها_Base *base,
                           ثابت بەها *src_بەها,
                           ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map) ثابت
  {
    إذا (!value) أرجع;
    unsigned varidx = (base + get_device (src_بەها)).get_variation_index ();
    hb_pair_t<unsigned, صحيح> *varidx_delta;
    إذا (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) أرجع;

    *value += hb_second (*varidx_delta);
  }

  منطقي copy_device (hb_serialize_context_t *c,
                    ثابت بەها_Base *base,
                    ثابت بەها *src_بەها,
                    ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *layout_variation_idx_delta_map,
                    unsigned صحيح new_format, Flags flag) ثابت
  {
    // Filter by new format.
    إذا (!(new_format & flag)) أرجع true;

    بەها       *dst_بەها = c->copy (*src_بەها);

    إذا (!dst_بەها) أرجع خطأ;
    إذا (*dst_بەها == 0) أرجع true;

    *dst_بەها = 0;
    c->push ();
    إذا ((base + get_device (src_بەها)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_بەها, c->دەرهێنان_pack ());
      أرجع true;
    }
    وإلا
    {
      c->دەرهێنان_discard ();
      أرجع خطأ;
    }
  }

  ساكن مضمن ثابت HBINT16& get_short (ثابت بەها* value, منطقي *worked=nullptr)
  {
    إذا (worked) *worked |= منطقي (*value);
    أرجع *reinterpret_cast<ثابت HBINT16 *> (value);
  }

  عام:

  منطقي has_device () ثابت
  {
    unsigned صحيح format = *هذا;
    أرجع (format & devices) != 0;
  }

  منطقي sanitize_بەها (hb_sanitize_context_t *c, ثابت بەها_Base *base, ثابت بەها *values) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (unlikely (!c->check_range (values, get_size ()))) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (true);

    return_trace (!has_device () || sanitize_بەها_devices (c, base, values));
  }

  منطقي sanitize_بەهاکان (hb_sanitize_context_t *c, ثابت بەها_Base *base, ثابت بەها *values, unsigned صحيح ژماردن) ثابت
  {
    TRACE_SANITIZE (هذا);
    unsigned size = get_size ();

    إذا (!c->check_range (values, ژماردن, size)) return_trace (خطأ);

    إذا (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    return_trace (sanitize_بەهاکان_stride_unsafe (c, base, values, ژماردن, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  منطقي sanitize_بەهاکان_stride_unsafe (hb_sanitize_context_t *c, ثابت بەها_Base *base, ثابت بەها *values, unsigned صحيح ژماردن, unsigned صحيح stride) ثابت
  {
    TRACE_SANITIZE (هذا);

    إذا (!has_device ()) return_trace (true);

    لكل (unsigned صحيح i = 0; i < ژماردن; i++) {
      إذا (!sanitize_بەها_devices (c, base, values))
        return_trace (خطأ);
      values = &StructAtOffset<ثابت بەها> (values, stride);
    }

    return_trace (true);
  }

 خاص:

  فراغ should_drop (بەها value, Flags flag, unsigned صحيح* format) ثابت
  {
    إذا (value) أرجع;
    *format = *format & ~flag;
  }

  فراغ update_var_flag (ثابت بەها* value, Flags flag,
                        unsigned صحيح* format, ثابت بەها_Base *base,
                        ثابت hb_hashmap_t<unsigned, hb_pair_t<unsigned, صحيح>> *varidx_delta_map) ثابت
  {
    إذا (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, صحيح> *varidx_delta;
      إذا (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        إذا (varidx_delta->second)
          *format |= flag >> 4;
        إذا (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          أرجع;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

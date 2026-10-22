#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#அந்தர்பாவ "../../../hb-ot-layout-gsubgpos.hh"

நாமாகாஶ OT {
நாமாகாஶ Layout {
நாமாகாஶ GPOS_impl {

வகைவரை HBUINT16 மதிப்பு;

கட்டமைப்பு மதிப்பு_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

வகைவரை UnsizedArrayOf<மதிப்பு> மதிப்பு_Record;

கட்டமைப்பு மதிப்பு_Format : HBUINT16
{
  எண்ணிக்கை Flags {
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
#எனில் 0
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

  இலக்கம்_Type& operator = (uint16_t i) { v = i; திரும்பு *இது; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned முழு get_len () மாறிலி  { திரும்பு hb_popcount8 ((uint8_t) *இது); }
  size_t get_size () மாறிலி { திரும்பு get_len () * மதிப்பு::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () மாறிலி {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *இது;

    எனில் (format & xPlacement) i++;
    எனில் (format & yPlacement) i++;
    எனில் (format & xAdvance)   i++;
    எனில் (format & yAdvance)   i++;

    எனில் (format & xPlaDevice) result.push (i++);
    எனில் (format & yPlaDevice) result.push (i++);
    எனில் (format & xAdvDevice) result.push (i++);
    எனில் (format & yAdvDevice) result.push (i++);

    திரும்பு result;
  }

  தருக்க apply_மதிப்பு (hb_ot_apply_context_t *c,
                    மாறிலி மதிப்பு_Base       *base,
                    மாறிலி மதிப்பு           *values,
                    hb_glyph_position_t   &glyph_pos) மாறிலி
  {
    தருக்க ret = பொய்;
    unsigned முழு format = *இது;
    எனில் (!format) திரும்பு ret;

    hb_font_t *font = c->font;
    தருக்க horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#இல்லையெனில்
      உண்மை
#endif
      ;

    எனில் (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    எனில் (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    எனில் (format & xAdvance) {
      எனில் (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    எனில் (format & yAdvance) {
      எனில் (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    எனில் (!has_device ()) திரும்பு ret;

    தருக்க use_x_device = font->x_ppem || font->has_nonzero_coords;
    தருக்க use_y_device = font->y_ppem || font->has_nonzero_coords;

    எனில் (!use_x_device && !use_y_device) திரும்பு ret;

    மாறிலி ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    எனில் (format & xPlaDevice)
    {
      எனில் (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    எனில் (format & yPlaDevice)
    {
      எனில் (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    எனில் (format & xAdvDevice)
    {
      எனில் (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    எனில் (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      எனில் (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    திரும்பு ret;
  }

  வெறுமை position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        மாறிலி மதிப்பு_Base *base,
                        மாறிலி மதிப்பு *values,
                        hb_glyph_position_t &pos) மாறிலி
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_மதிப்பு (&c, base, values, pos);
  }

  unsigned முழு get_effective_format (மாறிலி மதிப்பு *values, தருக்க strip_hints, தருக்க strip_empty, மாறிலி மதிப்பு_Base *base,
                                     மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map) மாறிலி
  {
    unsigned முழு format = *இது;
    க்கு (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      எனில் (format & flag)
      {
        எனில் (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          தொடர்;
        }
        எனில் (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          தொடர்;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        எனில் (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    திரும்பு format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned முழு get_effective_format (Iterator it, தருக்க strip_hints, தருக்க strip_empty, மாறிலி மதிப்பு_Base *base,
                                     மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map) மாறிலி {
    unsigned முழு new_format = 0;

    க்கு (மாறிலி hb_array_t<மாறிலி மதிப்பு>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    திரும்பு new_format;
  }

  வெறுமை copy_மதிப்புகள் (hb_serialize_context_t *c,
                    unsigned முழு new_format,
                    மாறிலி மதிப்பு_Base *base,
                    மாறிலி மதிப்பு *values,
                    மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map) மாறிலி
  {
    unsigned முழு format = *இது;
    எனில் (!format) திரும்பு;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    எனில் ((format | new_format) & xPlacement)
      x_placement = copy_மதிப்பு (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (மதிப்பு));
    எனில் ((format | new_format) & yPlacement)
      y_placement = copy_மதிப்பு (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (மதிப்பு));
    எனில் ((format | new_format) & xAdvance)
      x_adv = copy_மதிப்பு (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (மதிப்பு));
    எனில் ((format | new_format) & yAdvance)
      y_adv = copy_மதிப்பு (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (மதிப்பு));

    எனில் (!has_device ())
      திரும்பு;

    எனில் (format & xPlaDevice)
    {
      add_delta_to_மதிப்பு (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    எனில் (format & yPlaDevice)
    {
      add_delta_to_மதிப்பு (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    எனில் (format & xAdvDevice)
    {
      add_delta_to_மதிப்பு (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    எனில் (format & yAdvDevice)
    {
      add_delta_to_மதிப்பு (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_மதிப்பு (hb_serialize_context_t *c,
                       unsigned முழு new_format,
                       Flags flag,
                       மதிப்பு value) மாறிலி
  {
    // Filter by new format.
    எனில் (!(new_format & flag)) திரும்பு nullptr;
    திரும்பு reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  வெறுமை collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  மாறிலி மதிப்பு_Base *base,
                                  மாறிலி hb_array_t<மாறிலி மதிப்பு>& values) மாறிலி
  {
    unsigned format = *இது;
    unsigned i = 0;
    எனில் (format & xPlacement) i++;
    எனில் (format & yPlacement) i++;
    எனில் (format & xAdvance) i++;
    எனில் (format & yAdvance) i++;
    எனில் (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    எனில் (format & மதிப்பு_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    எனில் (format & மதிப்பு_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    எனில் (format & மதிப்பு_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  தனிப்பட்ட:
  தருக்க sanitize_மதிப்பு_devices (hb_sanitize_context_t *c, மாறிலி மதிப்பு_Base *base, மாறிலி மதிப்பு *values) மாறிலி
  {
    unsigned முழு format = *இது;

    எனில் (format & xPlacement) values++;
    எனில் (format & yPlacement) values++;
    எனில் (format & xAdvance)   values++;
    எனில் (format & yAdvance)   values++;

    எனில் ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) திரும்பு பொய்;
    எனில் ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) திரும்பு பொய்;
    எனில் ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) திரும்பு பொய்;
    எனில் ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) திரும்பு பொய்;

    திரும்பு உண்மை;
  }

  நிலையான உள்வரி Offset16To<Device, மதிப்பு_Base>& get_device (மதிப்பு* value)
  {
    திரும்பு *static_cast<Offset16To<Device, மதிப்பு_Base> *> (value);
  }
  நிலையான உள்வரி மாறிலி Offset16To<Device, மதிப்பு_Base>& get_device (மாறிலி மதிப்பு* value)
  {
    திரும்பு *static_cast<மாறிலி Offset16To<Device, மதிப்பு_Base> *> (value);
  }
  நிலையான உள்வரி மாறிலி Device& get_device (மாறிலி மதிப்பு* value,
					  தருக்க *worked,
					  மாறிலி மதிப்பு_Base *base,
					  hb_sanitize_context_t &c)
  {
    எனில் (worked) *worked |= தருக்க (*value);
    auto &offset = *static_cast<மாறிலி Offset16To<Device> *> (value);

    எனில் (unlikely (!offset.sanitize (&c, base)))
      திரும்பு Null(Device);
    hb_barrier ();

    திரும்பு base + offset;
  }

  வெறுமை add_delta_to_மதிப்பு (HBINT16 *value,
                           மாறிலி மதிப்பு_Base *base,
                           மாறிலி மதிப்பு *src_மதிப்பு,
                           மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map) மாறிலி
  {
    எனில் (!value) திரும்பு;
    unsigned varidx = (base + get_device (src_மதிப்பு)).get_variation_index ();
    hb_pair_t<unsigned, முழு> *varidx_delta;
    எனில் (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) திரும்பு;

    *value += hb_second (*varidx_delta);
  }

  தருக்க copy_device (hb_serialize_context_t *c,
                    மாறிலி மதிப்பு_Base *base,
                    மாறிலி மதிப்பு *src_மதிப்பு,
                    மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *layout_variation_idx_delta_map,
                    unsigned முழு new_format, Flags flag) மாறிலி
  {
    // Filter by new format.
    எனில் (!(new_format & flag)) திரும்பு உண்மை;

    மதிப்பு       *dst_மதிப்பு = c->copy (*src_மதிப்பு);

    எனில் (!dst_மதிப்பு) திரும்பு பொய்;
    எனில் (*dst_மதிப்பு == 0) திரும்பு உண்மை;

    *dst_மதிப்பு = 0;
    c->push ();
    எனில் ((base + get_device (src_மதிப்பு)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_மதிப்பு, c->எடு_pack ());
      திரும்பு உண்மை;
    }
    இல்லையெனில்
    {
      c->எடு_discard ();
      திரும்பு பொய்;
    }
  }

  நிலையான உள்வரி மாறிலி HBINT16& get_short (மாறிலி மதிப்பு* value, தருக்க *worked=nullptr)
  {
    எனில் (worked) *worked |= தருக்க (*value);
    திரும்பு *reinterpret_cast<மாறிலி HBINT16 *> (value);
  }

  பொது:

  தருக்க has_device () மாறிலி
  {
    unsigned முழு format = *இது;
    திரும்பு (format & devices) != 0;
  }

  தருக்க sanitize_மதிப்பு (hb_sanitize_context_t *c, மாறிலி மதிப்பு_Base *base, மாறிலி மதிப்பு *values) மாறிலி
  {
    TRACE_SANITIZE (இது);

    எனில் (unlikely (!c->check_range (values, get_size ()))) return_trace (பொய்);

    எனில் (c->lazy_some_gpos)
      return_trace (உண்மை);

    return_trace (!has_device () || sanitize_மதிப்பு_devices (c, base, values));
  }

  தருக்க sanitize_மதிப்புகள் (hb_sanitize_context_t *c, மாறிலி மதிப்பு_Base *base, மாறிலி மதிப்பு *values, unsigned முழு மொத்தம்) மாறிலி
  {
    TRACE_SANITIZE (இது);
    unsigned size = get_size ();

    எனில் (!c->check_range (values, மொத்தம், size)) return_trace (பொய்);

    எனில் (c->lazy_some_gpos)
      return_trace (உண்மை);

    hb_barrier ();
    return_trace (sanitize_மதிப்புகள்_stride_unsafe (c, base, values, மொத்தம், size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  தருக்க sanitize_மதிப்புகள்_stride_unsafe (hb_sanitize_context_t *c, மாறிலி மதிப்பு_Base *base, மாறிலி மதிப்பு *values, unsigned முழு மொத்தம், unsigned முழு stride) மாறிலி
  {
    TRACE_SANITIZE (இது);

    எனில் (!has_device ()) return_trace (உண்மை);

    க்கு (unsigned முழு i = 0; i < மொத்தம்; i++) {
      எனில் (!sanitize_மதிப்பு_devices (c, base, values))
        return_trace (பொய்);
      values = &StructAtOffset<மாறிலி மதிப்பு> (values, stride);
    }

    return_trace (உண்மை);
  }

 தனிப்பட்ட:

  வெறுமை should_drop (மதிப்பு value, Flags flag, unsigned முழு* format) மாறிலி
  {
    எனில் (value) திரும்பு;
    *format = *format & ~flag;
  }

  வெறுமை update_var_flag (மாறிலி மதிப்பு* value, Flags flag,
                        unsigned முழு* format, மாறிலி மதிப்பு_Base *base,
                        மாறிலி hb_hashmap_t<unsigned, hb_pair_t<unsigned, முழு>> *varidx_delta_map) மாறிலி
  {
    எனில் (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, முழு> *varidx_delta;
      எனில் (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        எனில் (varidx_delta->second)
          *format |= flag >> 4;
        எனில் (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          திரும்பு;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

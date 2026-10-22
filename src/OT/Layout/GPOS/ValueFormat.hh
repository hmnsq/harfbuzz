#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

प्रकारनिर्देश HBUINT16 मूल्य;

संरचना मूल्य_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

प्रकारनिर्देश UnsizedArrayOf<मूल्य> मूल्य_Record;

संरचना मूल्य_Format : HBUINT16
{
  गणना Flags {
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
#यदि 0
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

  अंक_Type& operator = (uint16_t i) { v = i; प्रत्यागम *एतत्; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned पूर्णाङ्क get_len () ध्रुव  { प्रत्यागम hb_popcount8 ((uint8_t) *एतत्); }
  size_t get_size () ध्रुव { प्रत्यागम get_len () * मूल्य::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ध्रुव {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *एतत्;

    यदि (format & xPlacement) i++;
    यदि (format & yPlacement) i++;
    यदि (format & xAdvance)   i++;
    यदि (format & yAdvance)   i++;

    यदि (format & xPlaDevice) result.push (i++);
    यदि (format & yPlaDevice) result.push (i++);
    यदि (format & xAdvDevice) result.push (i++);
    यदि (format & yAdvDevice) result.push (i++);

    प्रत्यागम result;
  }

  तार्किक apply_मूल्य (hb_ot_apply_context_t *c,
                    ध्रुव मूल्य_Base       *base,
                    ध्रुव मूल्य           *values,
                    hb_glyph_position_t   &glyph_pos) ध्रुव
  {
    तार्किक ret = असत्य;
    unsigned पूर्णाङ्क format = *एतत्;
    यदि (!format) प्रत्यागम ret;

    hb_font_t *font = c->font;
    तार्किक horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#अन्यथा
      सत्य
#endif
      ;

    यदि (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    यदि (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    यदि (format & xAdvance) {
      यदि (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    यदि (format & yAdvance) {
      यदि (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    यदि (!has_device ()) प्रत्यागम ret;

    तार्किक use_x_device = font->x_ppem || font->has_nonzero_coords;
    तार्किक use_y_device = font->y_ppem || font->has_nonzero_coords;

    यदि (!use_x_device && !use_y_device) प्रत्यागम ret;

    ध्रुव ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    यदि (format & xPlaDevice)
    {
      यदि (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    यदि (format & yPlaDevice)
    {
      यदि (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    यदि (format & xAdvDevice)
    {
      यदि (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    यदि (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      यदि (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    प्रत्यागम ret;
  }

  शून्य position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ध्रुव मूल्य_Base *base,
                        ध्रुव मूल्य *values,
                        hb_glyph_position_t &pos) ध्रुव
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_मूल्य (&c, base, values, pos);
  }

  unsigned पूर्णाङ्क get_effective_format (ध्रुव मूल्य *values, तार्किक strip_hints, तार्किक strip_empty, ध्रुव मूल्य_Base *base,
                                     ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;
    प्रति (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      यदि (format & flag)
      {
        यदि (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          अनुवर्त;
        }
        यदि (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          अनुवर्त;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        यदि (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    प्रत्यागम format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned पूर्णाङ्क get_effective_format (Iterator it, तार्किक strip_hints, तार्किक strip_empty, ध्रुव मूल्य_Base *base,
                                     ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव {
    unsigned पूर्णाङ्क new_format = 0;

    प्रति (ध्रुव hb_array_t<ध्रुव मूल्य>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    प्रत्यागम new_format;
  }

  शून्य copy_मूल्यावली (hb_serialize_context_t *c,
                    unsigned पूर्णाङ्क new_format,
                    ध्रुव मूल्य_Base *base,
                    ध्रुव मूल्य *values,
                    ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map) ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;
    यदि (!format) प्रत्यागम;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    यदि ((format | new_format) & xPlacement)
      x_placement = copy_मूल्य (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (मूल्य));
    यदि ((format | new_format) & yPlacement)
      y_placement = copy_मूल्य (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (मूल्य));
    यदि ((format | new_format) & xAdvance)
      x_adv = copy_मूल्य (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (मूल्य));
    यदि ((format | new_format) & yAdvance)
      y_adv = copy_मूल्य (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (मूल्य));

    यदि (!has_device ())
      प्रत्यागम;

    यदि (format & xPlaDevice)
    {
      add_delta_to_मूल्य (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    यदि (format & yPlaDevice)
    {
      add_delta_to_मूल्य (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    यदि (format & xAdvDevice)
    {
      add_delta_to_मूल्य (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    यदि (format & yAdvDevice)
    {
      add_delta_to_मूल्य (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_मूल्य (hb_serialize_context_t *c,
                       unsigned पूर्णाङ्क new_format,
                       Flags flag,
                       मूल्य value) ध्रुव
  {
    // Filter by new format.
    यदि (!(new_format & flag)) प्रत्यागम nullptr;
    प्रत्यागम reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मूल्य_Base *base,
                                  ध्रुव hb_array_t<ध्रुव मूल्य>& values) ध्रुव
  {
    unsigned format = *एतत्;
    unsigned i = 0;
    यदि (format & xPlacement) i++;
    यदि (format & yPlacement) i++;
    यदि (format & xAdvance) i++;
    यदि (format & yAdvance) i++;
    यदि (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    यदि (format & मूल्य_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    यदि (format & मूल्य_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    यदि (format & मूल्य_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  गुप्त:
  तार्किक sanitize_मूल्य_devices (hb_sanitize_context_t *c, ध्रुव मूल्य_Base *base, ध्रुव मूल्य *values) ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;

    यदि (format & xPlacement) values++;
    यदि (format & yPlacement) values++;
    यदि (format & xAdvance)   values++;
    यदि (format & yAdvance)   values++;

    यदि ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) प्रत्यागम असत्य;
    यदि ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) प्रत्यागम असत्य;
    यदि ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) प्रत्यागम असत्य;
    यदि ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) प्रत्यागम असत्य;

    प्रत्यागम सत्य;
  }

  स्थिर अन्तःस्थ Offset16To<Device, मूल्य_Base>& get_device (मूल्य* value)
  {
    प्रत्यागम *static_cast<Offset16To<Device, मूल्य_Base> *> (value);
  }
  स्थिर अन्तःस्थ ध्रुव Offset16To<Device, मूल्य_Base>& get_device (ध्रुव मूल्य* value)
  {
    प्रत्यागम *static_cast<ध्रुव Offset16To<Device, मूल्य_Base> *> (value);
  }
  स्थिर अन्तःस्थ ध्रुव Device& get_device (ध्रुव मूल्य* value,
					  तार्किक *worked,
					  ध्रुव मूल्य_Base *base,
					  hb_sanitize_context_t &c)
  {
    यदि (worked) *worked |= तार्किक (*value);
    auto &offset = *static_cast<ध्रुव Offset16To<Device> *> (value);

    यदि (unlikely (!offset.sanitize (&c, base)))
      प्रत्यागम Null(Device);
    hb_barrier ();

    प्रत्यागम base + offset;
  }

  शून्य add_delta_to_मूल्य (HBINT16 *value,
                           ध्रुव मूल्य_Base *base,
                           ध्रुव मूल्य *src_मूल्य,
                           ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map) ध्रुव
  {
    यदि (!value) प्रत्यागम;
    unsigned varidx = (base + get_device (src_मूल्य)).get_variation_index ();
    hb_pair_t<unsigned, पूर्णाङ्क> *varidx_delta;
    यदि (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) प्रत्यागम;

    *value += hb_second (*varidx_delta);
  }

  तार्किक copy_device (hb_serialize_context_t *c,
                    ध्रुव मूल्य_Base *base,
                    ध्रुव मूल्य *src_मूल्य,
                    ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map,
                    unsigned पूर्णाङ्क new_format, Flags flag) ध्रुव
  {
    // Filter by new format.
    यदि (!(new_format & flag)) प्रत्यागम सत्य;

    मूल्य       *dst_मूल्य = c->copy (*src_मूल्य);

    यदि (!dst_मूल्य) प्रत्यागम असत्य;
    यदि (*dst_मूल्य == 0) प्रत्यागम सत्य;

    *dst_मूल्य = 0;
    c->push ();
    यदि ((base + get_device (src_मूल्य)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_मूल्य, c->निकालो_pack ());
      प्रत्यागम सत्य;
    }
    अन्यथा
    {
      c->निकालो_discard ();
      प्रत्यागम असत्य;
    }
  }

  स्थिर अन्तःस्थ ध्रुव HBINT16& get_short (ध्रुव मूल्य* value, तार्किक *worked=nullptr)
  {
    यदि (worked) *worked |= तार्किक (*value);
    प्रत्यागम *reinterpret_cast<ध्रुव HBINT16 *> (value);
  }

  सार्वजनिक:

  तार्किक has_device () ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;
    प्रत्यागम (format & devices) != 0;
  }

  तार्किक sanitize_मूल्य (hb_sanitize_context_t *c, ध्रुव मूल्य_Base *base, ध्रुव मूल्य *values) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    यदि (unlikely (!c->check_range (values, get_size ()))) return_trace (असत्य);

    यदि (c->lazy_some_gpos)
      return_trace (सत्य);

    return_trace (!has_device () || sanitize_मूल्य_devices (c, base, values));
  }

  तार्किक sanitize_मूल्यावली (hb_sanitize_context_t *c, ध्रुव मूल्य_Base *base, ध्रुव मूल्य *values, unsigned पूर्णाङ्क गिनती) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    unsigned size = get_size ();

    यदि (!c->check_range (values, गिनती, size)) return_trace (असत्य);

    यदि (c->lazy_some_gpos)
      return_trace (सत्य);

    hb_barrier ();
    return_trace (sanitize_मूल्यावली_stride_unsafe (c, base, values, गिनती, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  तार्किक sanitize_मूल्यावली_stride_unsafe (hb_sanitize_context_t *c, ध्रुव मूल्य_Base *base, ध्रुव मूल्य *values, unsigned पूर्णाङ्क गिनती, unsigned पूर्णाङ्क stride) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    यदि (!has_device ()) return_trace (सत्य);

    प्रति (unsigned पूर्णाङ्क i = 0; i < गिनती; i++) {
      यदि (!sanitize_मूल्य_devices (c, base, values))
        return_trace (असत्य);
      values = &StructAtOffset<ध्रुव मूल्य> (values, stride);
    }

    return_trace (सत्य);
  }

 गुप्त:

  शून्य should_drop (मूल्य value, Flags flag, unsigned पूर्णाङ्क* format) ध्रुव
  {
    यदि (value) प्रत्यागम;
    *format = *format & ~flag;
  }

  शून्य update_var_flag (ध्रुव मूल्य* value, Flags flag,
                        unsigned पूर्णाङ्क* format, ध्रुव मूल्य_Base *base,
                        ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    यदि (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, पूर्णाङ्क> *varidx_delta;
      यदि (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        यदि (varidx_delta->second)
          *format |= flag >> 4;
        यदि (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          प्रत्यागम;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

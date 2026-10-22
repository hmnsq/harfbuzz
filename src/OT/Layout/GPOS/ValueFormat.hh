#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

प्रकारनिर्देश HBUINT16 मानम्;

संरचना मानम्_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

प्रकारनिर्देश UnsizedArrayOf<मानम्> मानम्_Record;

संरचना मानम्_Format : HBUINT16
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
#सचे 0
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

  अङ्कः_Type& operator = (uint16_t i) { v = i; पटिनिवत्त *एतत्; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned पूर्णाङ्क get_len () ध्रुव  { पटिनिवत्त hb_popcount8 ((uint8_t) *एतत्); }
  size_t get_size () ध्रुव { पटिनिवत्त get_len () * मानम्::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ध्रुव {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *एतत्;

    सचे (format & xPlacement) i++;
    सचे (format & yPlacement) i++;
    सचे (format & xAdvance)   i++;
    सचे (format & yAdvance)   i++;

    सचे (format & xPlaDevice) result.push (i++);
    सचे (format & yPlaDevice) result.push (i++);
    सचे (format & xAdvDevice) result.push (i++);
    सचे (format & yAdvDevice) result.push (i++);

    पटिनिवत्त result;
  }

  तार्किक apply_मानम् (hb_ot_apply_context_t *c,
                    ध्रुव मानम्_Base       *base,
                    ध्रुव मानम्           *values,
                    hb_glyph_position_t   &glyph_pos) ध्रुव
  {
    तार्किक ret = मिच्छा;
    unsigned पूर्णाङ्क format = *एतत्;
    सचे (!format) पटिनिवत्त ret;

    hb_font_t *font = c->font;
    तार्किक horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#अञ्ञथा
      सच्च
#endif
      ;

    सचे (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    सचे (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    सचे (format & xAdvance) {
      सचे (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    सचे (format & yAdvance) {
      सचे (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    सचे (!has_device ()) पटिनिवत्त ret;

    तार्किक use_x_device = font->x_ppem || font->has_nonzero_coords;
    तार्किक use_y_device = font->y_ppem || font->has_nonzero_coords;

    सचे (!use_x_device && !use_y_device) पटिनिवत्त ret;

    ध्रुव ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    सचे (format & xPlaDevice)
    {
      सचे (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    सचे (format & yPlaDevice)
    {
      सचे (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    सचे (format & xAdvDevice)
    {
      सचे (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    सचे (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      सचे (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    पटिनिवत्त ret;
  }

  शून्य position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ध्रुव मानम्_Base *base,
                        ध्रुव मानम् *values,
                        hb_glyph_position_t &pos) ध्रुव
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_मानम् (&c, base, values, pos);
  }

  unsigned पूर्णाङ्क get_effective_format (ध्रुव मानम् *values, तार्किक strip_hints, तार्किक strip_empty, ध्रुव मानम्_Base *base,
                                     ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;
    अत्थाय (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      सचे (format & flag)
      {
        सचे (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          अनुवर्त;
        }
        सचे (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          अनुवर्त;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        सचे (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    पटिनिवत्त format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned पूर्णाङ्क get_effective_format (Iterator it, तार्किक strip_hints, तार्किक strip_empty, ध्रुव मानम्_Base *base,
                                     ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव {
    unsigned पूर्णाङ्क new_format = 0;

    अत्थाय (ध्रुव hb_array_t<ध्रुव मानम्>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    पटिनिवत्त new_format;
  }

  शून्य copy_मानानि (hb_serialize_context_t *c,
                    unsigned पूर्णाङ्क new_format,
                    ध्रुव मानम्_Base *base,
                    ध्रुव मानम् *values,
                    ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map) ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;
    सचे (!format) पटिनिवत्त;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    सचे ((format | new_format) & xPlacement)
      x_placement = copy_मानम् (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (मानम्));
    सचे ((format | new_format) & yPlacement)
      y_placement = copy_मानम् (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (मानम्));
    सचे ((format | new_format) & xAdvance)
      x_adv = copy_मानम् (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (मानम्));
    सचे ((format | new_format) & yAdvance)
      y_adv = copy_मानम् (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (मानम्));

    सचे (!has_device ())
      पटिनिवत्त;

    सचे (format & xPlaDevice)
    {
      add_delta_to_मानम् (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    सचे (format & yPlaDevice)
    {
      add_delta_to_मानम् (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    सचे (format & xAdvDevice)
    {
      add_delta_to_मानम् (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    सचे (format & yAdvDevice)
    {
      add_delta_to_मानम् (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_मानम् (hb_serialize_context_t *c,
                       unsigned पूर्णाङ्क new_format,
                       Flags flag,
                       मानम् value) ध्रुव
  {
    // Filter by new format.
    सचे (!(new_format & flag)) पटिनिवत्त nullptr;
    पटिनिवत्त reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ध्रुव मानम्_Base *base,
                                  ध्रुव hb_array_t<ध्रुव मानम्>& values) ध्रुव
  {
    unsigned format = *एतत्;
    unsigned i = 0;
    सचे (format & xPlacement) i++;
    सचे (format & yPlacement) i++;
    सचे (format & xAdvance) i++;
    सचे (format & yAdvance) i++;
    सचे (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    सचे (format & मानम्_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    सचे (format & मानम्_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    सचे (format & मानम्_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  गुप्त:
  तार्किक sanitize_मानम्_devices (hb_sanitize_context_t *c, ध्रुव मानम्_Base *base, ध्रुव मानम् *values) ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;

    सचे (format & xPlacement) values++;
    सचे (format & yPlacement) values++;
    सचे (format & xAdvance)   values++;
    सचे (format & yAdvance)   values++;

    सचे ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) पटिनिवत्त मिच्छा;
    सचे ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) पटिनिवत्त मिच्छा;
    सचे ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) पटिनिवत्त मिच्छा;
    सचे ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) पटिनिवत्त मिच्छा;

    पटिनिवत्त सच्च;
  }

  स्थिर अन्तःस्थ Offset16To<Device, मानम्_Base>& get_device (मानम्* value)
  {
    पटिनिवत्त *static_cast<Offset16To<Device, मानम्_Base> *> (value);
  }
  स्थिर अन्तःस्थ ध्रुव Offset16To<Device, मानम्_Base>& get_device (ध्रुव मानम्* value)
  {
    पटिनिवत्त *static_cast<ध्रुव Offset16To<Device, मानम्_Base> *> (value);
  }
  स्थिर अन्तःस्थ ध्रुव Device& get_device (ध्रुव मानम्* value,
					  तार्किक *worked,
					  ध्रुव मानम्_Base *base,
					  hb_sanitize_context_t &c)
  {
    सचे (worked) *worked |= तार्किक (*value);
    auto &offset = *static_cast<ध्रुव Offset16To<Device> *> (value);

    सचे (unlikely (!offset.sanitize (&c, base)))
      पटिनिवत्त Null(Device);
    hb_barrier ();

    पटिनिवत्त base + offset;
  }

  शून्य add_delta_to_मानम् (HBINT16 *value,
                           ध्रुव मानम्_Base *base,
                           ध्रुव मानम् *src_मानम्,
                           ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map) ध्रुव
  {
    सचे (!value) पटिनिवत्त;
    unsigned varidx = (base + get_device (src_मानम्)).get_variation_index ();
    hb_pair_t<unsigned, पूर्णाङ्क> *varidx_delta;
    सचे (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) पटिनिवत्त;

    *value += hb_second (*varidx_delta);
  }

  तार्किक copy_device (hb_serialize_context_t *c,
                    ध्रुव मानम्_Base *base,
                    ध्रुव मानम् *src_मानम्,
                    ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *layout_variation_idx_delta_map,
                    unsigned पूर्णाङ्क new_format, Flags flag) ध्रुव
  {
    // Filter by new format.
    सचे (!(new_format & flag)) पटिनिवत्त सच्च;

    मानम्       *dst_मानम् = c->copy (*src_मानम्);

    सचे (!dst_मानम्) पटिनिवत्त मिच्छा;
    सचे (*dst_मानम् == 0) पटिनिवत्त सच्च;

    *dst_मानम् = 0;
    c->push ();
    सचे ((base + get_device (src_मानम्)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_मानम्, c->निष्कासय_pack ());
      पटिनिवत्त सच्च;
    }
    अञ्ञथा
    {
      c->निष्कासय_discard ();
      पटिनिवत्त मिच्छा;
    }
  }

  स्थिर अन्तःस्थ ध्रुव HBINT16& get_short (ध्रुव मानम्* value, तार्किक *worked=nullptr)
  {
    सचे (worked) *worked |= तार्किक (*value);
    पटिनिवत्त *reinterpret_cast<ध्रुव HBINT16 *> (value);
  }

  सार्वजनिक:

  तार्किक has_device () ध्रुव
  {
    unsigned पूर्णाङ्क format = *एतत्;
    पटिनिवत्त (format & devices) != 0;
  }

  तार्किक sanitize_मानम् (hb_sanitize_context_t *c, ध्रुव मानम्_Base *base, ध्रुव मानम् *values) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    सचे (unlikely (!c->check_range (values, get_size ()))) return_trace (मिच्छा);

    सचे (c->lazy_some_gpos)
      return_trace (सच्च);

    return_trace (!has_device () || sanitize_मानम्_devices (c, base, values));
  }

  तार्किक sanitize_मानानि (hb_sanitize_context_t *c, ध्रुव मानम्_Base *base, ध्रुव मानम् *values, unsigned पूर्णाङ्क गणनम्) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    unsigned size = get_size ();

    सचे (!c->check_range (values, गणनम्, size)) return_trace (मिच्छा);

    सचे (c->lazy_some_gpos)
      return_trace (सच्च);

    hb_barrier ();
    return_trace (sanitize_मानानि_stride_unsafe (c, base, values, गणनम्, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  तार्किक sanitize_मानानि_stride_unsafe (hb_sanitize_context_t *c, ध्रुव मानम्_Base *base, ध्रुव मानम् *values, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride) ध्रुव
  {
    TRACE_SANITIZE (एतत्);

    सचे (!has_device ()) return_trace (सच्च);

    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++) {
      सचे (!sanitize_मानम्_devices (c, base, values))
        return_trace (मिच्छा);
      values = &StructAtOffset<ध्रुव मानम्> (values, stride);
    }

    return_trace (सच्च);
  }

 गुप्त:

  शून्य should_drop (मानम् value, Flags flag, unsigned पूर्णाङ्क* format) ध्रुव
  {
    सचे (value) पटिनिवत्त;
    *format = *format & ~flag;
  }

  शून्य update_var_flag (ध्रुव मानम्* value, Flags flag,
                        unsigned पूर्णाङ्क* format, ध्रुव मानम्_Base *base,
                        ध्रुव hb_hashmap_t<unsigned, hb_pair_t<unsigned, पूर्णाङ्क>> *varidx_delta_map) ध्रुव
  {
    सचे (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, पूर्णाङ्क> *varidx_delta;
      सचे (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        सचे (varidx_delta->second)
          *format |= flag >> 4;
        सचे (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          पटिनिवत्त;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

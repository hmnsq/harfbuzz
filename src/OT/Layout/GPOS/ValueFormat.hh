#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

प्रकारनिर्देश HBUINT16 मान;

संरचना मान_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

प्रकारनिर्देश UnsizedArrayOf<मान> मान_Record;

संरचना मान_Format : HBUINT16
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

  अङ्क_Type& चालक = (uint16_t i) { v = i; फर्काऊ *एतत्; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  अचिन्हित पूर्णाङ्क get_len () अचर  { फर्काऊ hb_popcount8 ((uint8_t) *एतत्); }
  माप_प्रकार get_size () अचर { फर्काऊ get_len () * मान::static_size; }

  hb_vector_t<अचिन्हित> get_device_table_indices () अचर {
    अचिन्हित i = 0;
    hb_vector_t<अचिन्हित> result;
    अचिन्हित format = *एतत्;

    यदि (format & xPlacement) i++;
    यदि (format & yPlacement) i++;
    यदि (format & xAdvance)   i++;
    यदि (format & yAdvance)   i++;

    यदि (format & xPlaDevice) result.push (i++);
    यदि (format & yPlaDevice) result.push (i++);
    यदि (format & xAdvDevice) result.push (i++);
    यदि (format & yAdvDevice) result.push (i++);

    फर्काऊ result;
  }

  तार्किक apply_मान (hb_ot_apply_context_t *c,
                    अचर मान_Base       *base,
                    अचर मान           *values,
                    hb_glyph_position_t   &glyph_pos) अचर
  {
    तार्किक ret = असत्य;
    अचिन्हित पूर्णाङ्क format = *एतत्;
    यदि (!format) फर्काऊ ret;

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

    यदि (!has_device ()) फर्काऊ ret;

    तार्किक use_x_device = font->x_ppem || font->has_nonzero_coords;
    तार्किक use_y_device = font->y_ppem || font->has_nonzero_coords;

    यदि (!use_x_device && !use_y_device) फर्काऊ ret;

    अचर ItemVariationStore &store = c->var_store;
    स्वतः *cache = c->var_store_cache;

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
    फर्काऊ ret;
  }

  शून्य position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        अचर मान_Base *base,
                        अचर मान *values,
                        hb_glyph_position_t &pos) अचर
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_मान (&c, base, values, pos);
  }

  अचिन्हित पूर्णाङ्क get_effective_format (अचर मान *values, तार्किक strip_hints, तार्किक strip_empty, अचर मान_Base *base,
                                     अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map) अचर
  {
    अचिन्हित पूर्णाङ्क format = *एतत्;
    लागि (अचिन्हित flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      यदि (format & flag)
      {
        यदि (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          जारी;
        }
        यदि (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          जारी;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        यदि (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    फर्काऊ format;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित पूर्णाङ्क get_effective_format (Iterator it, तार्किक strip_hints, तार्किक strip_empty, अचर मान_Base *base,
                                     अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map) अचर {
    अचिन्हित पूर्णाङ्क new_format = 0;

    लागि (अचर hb_array_t<अचर मान>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    फर्काऊ new_format;
  }

  शून्य copy_मानहरू (hb_serialize_context_t *c,
                    अचिन्हित पूर्णाङ्क new_format,
                    अचर मान_Base *base,
                    अचर मान *values,
                    अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map) अचर
  {
    अचिन्हित पूर्णाङ्क format = *एतत्;
    यदि (!format) फर्काऊ;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    यदि ((format | new_format) & xPlacement)
      x_placement = copy_मान (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (मान));
    यदि ((format | new_format) & yPlacement)
      y_placement = copy_मान (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (मान));
    यदि ((format | new_format) & xAdvance)
      x_adv = copy_मान (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (मान));
    यदि ((format | new_format) & yAdvance)
      y_adv = copy_मान (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (मान));

    यदि (!has_device ())
      फर्काऊ;

    यदि (format & xPlaDevice)
    {
      add_delta_to_मान (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    यदि (format & yPlaDevice)
    {
      add_delta_to_मान (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    यदि (format & xAdvDevice)
    {
      add_delta_to_मान (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    यदि (format & yAdvDevice)
    {
      add_delta_to_मान (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_मान (hb_serialize_context_t *c,
                       अचिन्हित पूर्णाङ्क new_format,
                       Flags flag,
                       मान value) अचर
  {
    // Filter by new format.
    यदि (!(new_format & flag)) फर्काऊ nullptr;
    फर्काऊ reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  अचर मान_Base *base,
                                  अचर hb_array_t<अचर मान>& values) अचर
  {
    अचिन्हित format = *एतत्;
    अचिन्हित i = 0;
    यदि (format & xPlacement) i++;
    यदि (format & yPlacement) i++;
    यदि (format & xAdvance) i++;
    यदि (format & yAdvance) i++;
    यदि (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    यदि (format & मान_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    यदि (format & मान_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    यदि (format & मान_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  गुप्त:
  तार्किक sanitize_मान_devices (hb_sanitize_context_t *c, अचर मान_Base *base, अचर मान *values) अचर
  {
    अचिन्हित पूर्णाङ्क format = *एतत्;

    यदि (format & xPlacement) values++;
    यदि (format & yPlacement) values++;
    यदि (format & xAdvance)   values++;
    यदि (format & yAdvance)   values++;

    यदि ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) फर्काऊ असत्य;
    यदि ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) फर्काऊ असत्य;
    यदि ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) फर्काऊ असत्य;
    यदि ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) फर्काऊ असत्य;

    फर्काऊ सत्य;
  }

  स्थिर अन्तःस्थ Offset16To<Device, मान_Base>& get_device (मान* value)
  {
    फर्काऊ *static_cast<Offset16To<Device, मान_Base> *> (value);
  }
  स्थिर अन्तःस्थ अचर Offset16To<Device, मान_Base>& get_device (अचर मान* value)
  {
    फर्काऊ *static_cast<अचर Offset16To<Device, मान_Base> *> (value);
  }
  स्थिर अन्तःस्थ अचर Device& get_device (अचर मान* value,
					  तार्किक *worked,
					  अचर मान_Base *base,
					  hb_sanitize_context_t &c)
  {
    यदि (worked) *worked |= तार्किक (*value);
    स्वतः &offset = *static_cast<अचर Offset16To<Device> *> (value);

    यदि (unlikely (!offset.sanitize (&c, base)))
      फर्काऊ Null(Device);
    hb_barrier ();

    फर्काऊ base + offset;
  }

  शून्य add_delta_to_मान (HBINT16 *value,
                           अचर मान_Base *base,
                           अचर मान *src_मान,
                           अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map) अचर
  {
    यदि (!value) फर्काऊ;
    अचिन्हित varidx = (base + get_device (src_मान)).get_variation_index ();
    hb_pair_t<अचिन्हित, पूर्णाङ्क> *varidx_delta;
    यदि (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) फर्काऊ;

    *value += hb_second (*varidx_delta);
  }

  तार्किक copy_device (hb_serialize_context_t *c,
                    अचर मान_Base *base,
                    अचर मान *src_मान,
                    अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *layout_variation_idx_delta_map,
                    अचिन्हित पूर्णाङ्क new_format, Flags flag) अचर
  {
    // Filter by new format.
    यदि (!(new_format & flag)) फर्काऊ सत्य;

    मान       *dst_मान = c->copy (*src_मान);

    यदि (!dst_मान) फर्काऊ असत्य;
    यदि (*dst_मान == 0) फर्काऊ सत्य;

    *dst_मान = 0;
    c->push ();
    यदि ((base + get_device (src_मान)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_मान, c->झिक_pack ());
      फर्काऊ सत्य;
    }
    अन्यथा
    {
      c->झिक_discard ();
      फर्काऊ असत्य;
    }
  }

  स्थिर अन्तःस्थ अचर HBINT16& get_short (अचर मान* value, तार्किक *worked=nullptr)
  {
    यदि (worked) *worked |= तार्किक (*value);
    फर्काऊ *reinterpret_cast<अचर HBINT16 *> (value);
  }

  सार्वजनिक:

  तार्किक has_device () अचर
  {
    अचिन्हित पूर्णाङ्क format = *एतत्;
    फर्काऊ (format & devices) != 0;
  }

  तार्किक sanitize_मान (hb_sanitize_context_t *c, अचर मान_Base *base, अचर मान *values) अचर
  {
    TRACE_SANITIZE (एतत्);

    यदि (unlikely (!c->check_range (values, get_size ()))) return_trace (असत्य);

    यदि (c->lazy_some_gpos)
      return_trace (सत्य);

    return_trace (!has_device () || sanitize_मान_devices (c, base, values));
  }

  तार्किक sanitize_मानहरू (hb_sanitize_context_t *c, अचर मान_Base *base, अचर मान *values, अचिन्हित पूर्णाङ्क गन्ती) अचर
  {
    TRACE_SANITIZE (एतत्);
    अचिन्हित size = get_size ();

    यदि (!c->check_range (values, गन्ती, size)) return_trace (असत्य);

    यदि (c->lazy_some_gpos)
      return_trace (सत्य);

    hb_barrier ();
    return_trace (sanitize_मानहरू_stride_unsafe (c, base, values, गन्ती, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  तार्किक sanitize_मानहरू_stride_unsafe (hb_sanitize_context_t *c, अचर मान_Base *base, अचर मान *values, अचिन्हित पूर्णाङ्क गन्ती, अचिन्हित पूर्णाङ्क stride) अचर
  {
    TRACE_SANITIZE (एतत्);

    यदि (!has_device ()) return_trace (सत्य);

    लागि (अचिन्हित पूर्णाङ्क i = 0; i < गन्ती; i++) {
      यदि (!sanitize_मान_devices (c, base, values))
        return_trace (असत्य);
      values = &StructAtOffset<अचर मान> (values, stride);
    }

    return_trace (सत्य);
  }

 गुप्त:

  शून्य should_drop (मान value, Flags flag, अचिन्हित पूर्णाङ्क* format) अचर
  {
    यदि (value) फर्काऊ;
    *format = *format & ~flag;
  }

  शून्य update_var_flag (अचर मान* value, Flags flag,
                        अचिन्हित पूर्णाङ्क* format, अचर मान_Base *base,
                        अचर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णाङ्क>> *varidx_delta_map) अचर
  {
    यदि (*value)
    {
      अचिन्हित varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<अचिन्हित, पूर्णाङ्क> *varidx_delta;
      यदि (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        यदि (varidx_delta->second)
          *format |= flag >> 4;
        यदि (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          फर्काऊ;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

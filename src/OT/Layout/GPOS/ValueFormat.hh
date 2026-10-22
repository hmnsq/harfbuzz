#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#अन्तर्भाव "../../../hb-ot-layout-gsubgpos.hh"

नामाकाश OT {
नामाकाश Layout {
नामाकाश GPOS_impl {

प्रकारव्याख्या HBUINT16 मूल्य;

संरचना मूल्य_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

प्रकारव्याख्या UnsizedArrayOf<मूल्य> मूल्य_Record;

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
#जर 0
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

  अंक_Type& चालक = (uint16_t i) { v = i; परत *हे; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  अचिन्हित पूर्णांक get_len () स्थिरांक  { परत hb_popcount8 ((uint8_t) *हे); }
  माप_प्रकार get_size () स्थिरांक { परत get_len () * मूल्य::static_size; }

  hb_vector_t<अचिन्हित> get_device_table_indices () स्थिरांक {
    अचिन्हित i = 0;
    hb_vector_t<अचिन्हित> result;
    अचिन्हित format = *हे;

    जर (format & xPlacement) i++;
    जर (format & yPlacement) i++;
    जर (format & xAdvance)   i++;
    जर (format & yAdvance)   i++;

    जर (format & xPlaDevice) result.push (i++);
    जर (format & yPlaDevice) result.push (i++);
    जर (format & xAdvDevice) result.push (i++);
    जर (format & yAdvDevice) result.push (i++);

    परत result;
  }

  तार्किक apply_मूल्य (hb_ot_apply_context_t *c,
                    स्थिरांक मूल्य_Base       *base,
                    स्थिरांक मूल्य           *values,
                    hb_glyph_position_t   &glyph_pos) स्थिरांक
  {
    तार्किक ret = असत्य;
    अचिन्हित पूर्णांक format = *हे;
    जर (!format) परत ret;

    hb_font_t *font = c->font;
    तार्किक horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#अन्यथा
      सत्य
#endif
      ;

    जर (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    जर (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    जर (format & xAdvance) {
      जर (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    जर (format & yAdvance) {
      जर (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    जर (!has_device ()) परत ret;

    तार्किक use_x_device = font->x_ppem || font->has_nonzero_coords;
    तार्किक use_y_device = font->y_ppem || font->has_nonzero_coords;

    जर (!use_x_device && !use_y_device) परत ret;

    स्थिरांक ItemVariationStore &store = c->var_store;
    स्वतः *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    जर (format & xPlaDevice)
    {
      जर (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    जर (format & yPlaDevice)
    {
      जर (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    जर (format & xAdvDevice)
    {
      जर (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    जर (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      जर (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    परत ret;
  }

  शून्य position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        स्थिरांक मूल्य_Base *base,
                        स्थिरांक मूल्य *values,
                        hb_glyph_position_t &pos) स्थिरांक
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_मूल्य (&c, base, values, pos);
  }

  अचिन्हित पूर्णांक get_effective_format (स्थिरांक मूल्य *values, तार्किक strip_hints, तार्किक strip_empty, स्थिरांक मूल्य_Base *base,
                                     स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिरांक
  {
    अचिन्हित पूर्णांक format = *हे;
    साठी (अचिन्हित flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      जर (format & flag)
      {
        जर (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          सुरूठेवा;
        }
        जर (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          सुरूठेवा;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        जर (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    परत format;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित पूर्णांक get_effective_format (Iterator it, तार्किक strip_hints, तार्किक strip_empty, स्थिरांक मूल्य_Base *base,
                                     स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिरांक {
    अचिन्हित पूर्णांक new_format = 0;

    साठी (स्थिरांक hb_array_t<स्थिरांक मूल्य>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    परत new_format;
  }

  शून्य copy_मूल्ये (hb_serialize_context_t *c,
                    अचिन्हित पूर्णांक new_format,
                    स्थिरांक मूल्य_Base *base,
                    स्थिरांक मूल्य *values,
                    स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map) स्थिरांक
  {
    अचिन्हित पूर्णांक format = *हे;
    जर (!format) परत;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    जर ((format | new_format) & xPlacement)
      x_placement = copy_मूल्य (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (मूल्य));
    जर ((format | new_format) & yPlacement)
      y_placement = copy_मूल्य (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (मूल्य));
    जर ((format | new_format) & xAdvance)
      x_adv = copy_मूल्य (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (मूल्य));
    जर ((format | new_format) & yAdvance)
      y_adv = copy_मूल्य (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (मूल्य));

    जर (!has_device ())
      परत;

    जर (format & xPlaDevice)
    {
      add_delta_to_मूल्य (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    जर (format & yPlaDevice)
    {
      add_delta_to_मूल्य (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    जर (format & xAdvDevice)
    {
      add_delta_to_मूल्य (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    जर (format & yAdvDevice)
    {
      add_delta_to_मूल्य (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_मूल्य (hb_serialize_context_t *c,
                       अचिन्हित पूर्णांक new_format,
                       Flags flag,
                       मूल्य value) स्थिरांक
  {
    // Filter by new format.
    जर (!(new_format & flag)) परत nullptr;
    परत reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  शून्य collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिरांक मूल्य_Base *base,
                                  स्थिरांक hb_array_t<स्थिरांक मूल्य>& values) स्थिरांक
  {
    अचिन्हित format = *हे;
    अचिन्हित i = 0;
    जर (format & xPlacement) i++;
    जर (format & yPlacement) i++;
    जर (format & xAdvance) i++;
    जर (format & yAdvance) i++;
    जर (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    जर (format & मूल्य_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    जर (format & मूल्य_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    जर (format & मूल्य_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  खाजगी:
  तार्किक sanitize_मूल्य_devices (hb_sanitize_context_t *c, स्थिरांक मूल्य_Base *base, स्थिरांक मूल्य *values) स्थिरांक
  {
    अचिन्हित पूर्णांक format = *हे;

    जर (format & xPlacement) values++;
    जर (format & yPlacement) values++;
    जर (format & xAdvance)   values++;
    जर (format & yAdvance)   values++;

    जर ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) परत असत्य;
    जर ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) परत असत्य;
    जर ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) परत असत्य;
    जर ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) परत असत्य;

    परत सत्य;
  }

  स्थिर ओळीत Offset16To<Device, मूल्य_Base>& get_device (मूल्य* value)
  {
    परत *static_cast<Offset16To<Device, मूल्य_Base> *> (value);
  }
  स्थिर ओळीत स्थिरांक Offset16To<Device, मूल्य_Base>& get_device (स्थिरांक मूल्य* value)
  {
    परत *static_cast<स्थिरांक Offset16To<Device, मूल्य_Base> *> (value);
  }
  स्थिर ओळीत स्थिरांक Device& get_device (स्थिरांक मूल्य* value,
					  तार्किक *worked,
					  स्थिरांक मूल्य_Base *base,
					  hb_sanitize_context_t &c)
  {
    जर (worked) *worked |= तार्किक (*value);
    स्वतः &offset = *static_cast<स्थिरांक Offset16To<Device> *> (value);

    जर (unlikely (!offset.sanitize (&c, base)))
      परत Null(Device);
    hb_barrier ();

    परत base + offset;
  }

  शून्य add_delta_to_मूल्य (HBINT16 *value,
                           स्थिरांक मूल्य_Base *base,
                           स्थिरांक मूल्य *src_मूल्य,
                           स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map) स्थिरांक
  {
    जर (!value) परत;
    अचिन्हित varidx = (base + get_device (src_मूल्य)).get_variation_index ();
    hb_pair_t<अचिन्हित, पूर्णांक> *varidx_delta;
    जर (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) परत;

    *value += hb_second (*varidx_delta);
  }

  तार्किक copy_device (hb_serialize_context_t *c,
                    स्थिरांक मूल्य_Base *base,
                    स्थिरांक मूल्य *src_मूल्य,
                    स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                    अचिन्हित पूर्णांक new_format, Flags flag) स्थिरांक
  {
    // Filter by new format.
    जर (!(new_format & flag)) परत सत्य;

    मूल्य       *dst_मूल्य = c->copy (*src_मूल्य);

    जर (!dst_मूल्य) परत असत्य;
    जर (*dst_मूल्य == 0) परत सत्य;

    *dst_मूल्य = 0;
    c->push ();
    जर ((base + get_device (src_मूल्य)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_मूल्य, c->काढा_pack ());
      परत सत्य;
    }
    अन्यथा
    {
      c->काढा_discard ();
      परत असत्य;
    }
  }

  स्थिर ओळीत स्थिरांक HBINT16& get_short (स्थिरांक मूल्य* value, तार्किक *worked=nullptr)
  {
    जर (worked) *worked |= तार्किक (*value);
    परत *reinterpret_cast<स्थिरांक HBINT16 *> (value);
  }

  सार्वजनिक:

  तार्किक has_device () स्थिरांक
  {
    अचिन्हित पूर्णांक format = *हे;
    परत (format & devices) != 0;
  }

  तार्किक sanitize_मूल्य (hb_sanitize_context_t *c, स्थिरांक मूल्य_Base *base, स्थिरांक मूल्य *values) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    जर (unlikely (!c->check_range (values, get_size ()))) return_trace (असत्य);

    जर (c->lazy_some_gpos)
      return_trace (सत्य);

    return_trace (!has_device () || sanitize_मूल्य_devices (c, base, values));
  }

  तार्किक sanitize_मूल्ये (hb_sanitize_context_t *c, स्थिरांक मूल्य_Base *base, स्थिरांक मूल्य *values, अचिन्हित पूर्णांक मोजणी) स्थिरांक
  {
    TRACE_SANITIZE (हे);
    अचिन्हित size = get_size ();

    जर (!c->check_range (values, मोजणी, size)) return_trace (असत्य);

    जर (c->lazy_some_gpos)
      return_trace (सत्य);

    hb_barrier ();
    return_trace (sanitize_मूल्ये_stride_unsafe (c, base, values, मोजणी, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  तार्किक sanitize_मूल्ये_stride_unsafe (hb_sanitize_context_t *c, स्थिरांक मूल्य_Base *base, स्थिरांक मूल्य *values, अचिन्हित पूर्णांक मोजणी, अचिन्हित पूर्णांक stride) स्थिरांक
  {
    TRACE_SANITIZE (हे);

    जर (!has_device ()) return_trace (सत्य);

    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी; i++) {
      जर (!sanitize_मूल्य_devices (c, base, values))
        return_trace (असत्य);
      values = &StructAtOffset<स्थिरांक मूल्य> (values, stride);
    }

    return_trace (सत्य);
  }

 खाजगी:

  शून्य should_drop (मूल्य value, Flags flag, अचिन्हित पूर्णांक* format) स्थिरांक
  {
    जर (value) परत;
    *format = *format & ~flag;
  }

  शून्य update_var_flag (स्थिरांक मूल्य* value, Flags flag,
                        अचिन्हित पूर्णांक* format, स्थिरांक मूल्य_Base *base,
                        स्थिरांक hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिरांक
  {
    जर (*value)
    {
      अचिन्हित varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<अचिन्हित, पूर्णांक> *varidx_delta;
      जर (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        जर (varidx_delta->second)
          *format |= flag >> 4;
        जर (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          परत;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

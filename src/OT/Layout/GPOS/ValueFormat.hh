#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#અન્તર્ભાવ "../../../hb-ot-layout-gsubgpos.hh"

નામાકાશ OT {
નામાકાશ Layout {
નામાકાશ GPOS_impl {

પ્રકારવ્યાખ્યા HBUINT16 કિંમત;

સંરચના કિંમત_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

પ્રકારવ્યાખ્યા UnsizedArrayOf<કિંમત> કિંમત_Record;

સંરચના કિંમત_Format : HBUINT16
{
  ગણના Flags {
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
#જો 0
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

  અંક_Type& operator = (uint16_t i) { v = i; પાછા *આ; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned પૂર્ણાંક get_len () અચલ  { પાછા hb_popcount8 ((uint8_t) *આ); }
  size_t get_size () અચલ { પાછા get_len () * કિંમત::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () અચલ {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *આ;

    જો (format & xPlacement) i++;
    જો (format & yPlacement) i++;
    જો (format & xAdvance)   i++;
    જો (format & yAdvance)   i++;

    જો (format & xPlaDevice) result.push (i++);
    જો (format & yPlaDevice) result.push (i++);
    જો (format & xAdvDevice) result.push (i++);
    જો (format & yAdvDevice) result.push (i++);

    પાછા result;
  }

  તાર્કિક apply_કિંમત (hb_ot_apply_context_t *c,
                    અચલ કિંમત_Base       *base,
                    અચલ કિંમત           *values,
                    hb_glyph_position_t   &glyph_pos) અચલ
  {
    તાર્કિક ret = ખોટું;
    unsigned પૂર્ણાંક format = *આ;
    જો (!format) પાછા ret;

    hb_font_t *font = c->font;
    તાર્કિક horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#નહીંતર
      સાચું
#endif
      ;

    જો (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    જો (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    જો (format & xAdvance) {
      જો (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    જો (format & yAdvance) {
      જો (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    જો (!has_device ()) પાછા ret;

    તાર્કિક use_x_device = font->x_ppem || font->has_nonzero_coords;
    તાર્કિક use_y_device = font->y_ppem || font->has_nonzero_coords;

    જો (!use_x_device && !use_y_device) પાછા ret;

    અચલ ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    જો (format & xPlaDevice)
    {
      જો (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    જો (format & yPlaDevice)
    {
      જો (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    જો (format & xAdvDevice)
    {
      જો (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    જો (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      જો (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    પાછા ret;
  }

  શૂન્ય position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        અચલ કિંમત_Base *base,
                        અચલ કિંમત *values,
                        hb_glyph_position_t &pos) અચલ
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_કિંમત (&c, base, values, pos);
  }

  unsigned પૂર્ણાંક get_effective_format (અચલ કિંમત *values, તાર્કિક strip_hints, તાર્કિક strip_empty, અચલ કિંમત_Base *base,
                                     અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map) અચલ
  {
    unsigned પૂર્ણાંક format = *આ;
    માટે (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      જો (format & flag)
      {
        જો (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          ચાલુ;
        }
        જો (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          ચાલુ;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        જો (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    પાછા format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned પૂર્ણાંક get_effective_format (Iterator it, તાર્કિક strip_hints, તાર્કિક strip_empty, અચલ કિંમત_Base *base,
                                     અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map) અચલ {
    unsigned પૂર્ણાંક new_format = 0;

    માટે (અચલ hb_array_t<અચલ કિંમત>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    પાછા new_format;
  }

  શૂન્ય copy_કિંમતો (hb_serialize_context_t *c,
                    unsigned પૂર્ણાંક new_format,
                    અચલ કિંમત_Base *base,
                    અચલ કિંમત *values,
                    અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map) અચલ
  {
    unsigned પૂર્ણાંક format = *આ;
    જો (!format) પાછા;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    જો ((format | new_format) & xPlacement)
      x_placement = copy_કિંમત (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (કિંમત));
    જો ((format | new_format) & yPlacement)
      y_placement = copy_કિંમત (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (કિંમત));
    જો ((format | new_format) & xAdvance)
      x_adv = copy_કિંમત (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (કિંમત));
    જો ((format | new_format) & yAdvance)
      y_adv = copy_કિંમત (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (કિંમત));

    જો (!has_device ())
      પાછા;

    જો (format & xPlaDevice)
    {
      add_delta_to_કિંમત (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    જો (format & yPlaDevice)
    {
      add_delta_to_કિંમત (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    જો (format & xAdvDevice)
    {
      add_delta_to_કિંમત (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    જો (format & yAdvDevice)
    {
      add_delta_to_કિંમત (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_કિંમત (hb_serialize_context_t *c,
                       unsigned પૂર્ણાંક new_format,
                       Flags flag,
                       કિંમત value) અચલ
  {
    // Filter by new format.
    જો (!(new_format & flag)) પાછા nullptr;
    પાછા reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  શૂન્ય collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  અચલ કિંમત_Base *base,
                                  અચલ hb_array_t<અચલ કિંમત>& values) અચલ
  {
    unsigned format = *આ;
    unsigned i = 0;
    જો (format & xPlacement) i++;
    જો (format & yPlacement) i++;
    જો (format & xAdvance) i++;
    જો (format & yAdvance) i++;
    જો (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    જો (format & કિંમત_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    જો (format & કિંમત_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    જો (format & કિંમત_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  ખાનગી:
  તાર્કિક sanitize_કિંમત_devices (hb_sanitize_context_t *c, અચલ કિંમત_Base *base, અચલ કિંમત *values) અચલ
  {
    unsigned પૂર્ણાંક format = *આ;

    જો (format & xPlacement) values++;
    જો (format & yPlacement) values++;
    જો (format & xAdvance)   values++;
    જો (format & yAdvance)   values++;

    જો ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) પાછા ખોટું;
    જો ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) પાછા ખોટું;
    જો ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) પાછા ખોટું;
    જો ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) પાછા ખોટું;

    પાછા સાચું;
  }

  સ્થિર અંતઃરેખ Offset16To<Device, કિંમત_Base>& get_device (કિંમત* value)
  {
    પાછા *static_cast<Offset16To<Device, કિંમત_Base> *> (value);
  }
  સ્થિર અંતઃરેખ અચલ Offset16To<Device, કિંમત_Base>& get_device (અચલ કિંમત* value)
  {
    પાછા *static_cast<અચલ Offset16To<Device, કિંમત_Base> *> (value);
  }
  સ્થિર અંતઃરેખ અચલ Device& get_device (અચલ કિંમત* value,
					  તાર્કિક *worked,
					  અચલ કિંમત_Base *base,
					  hb_sanitize_context_t &c)
  {
    જો (worked) *worked |= તાર્કિક (*value);
    auto &offset = *static_cast<અચલ Offset16To<Device> *> (value);

    જો (unlikely (!offset.sanitize (&c, base)))
      પાછા Null(Device);
    hb_barrier ();

    પાછા base + offset;
  }

  શૂન્ય add_delta_to_કિંમત (HBINT16 *value,
                           અચલ કિંમત_Base *base,
                           અચલ કિંમત *src_કિંમત,
                           અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map) અચલ
  {
    જો (!value) પાછા;
    unsigned varidx = (base + get_device (src_કિંમત)).get_variation_index ();
    hb_pair_t<unsigned, પૂર્ણાંક> *varidx_delta;
    જો (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) પાછા;

    *value += hb_second (*varidx_delta);
  }

  તાર્કિક copy_device (hb_serialize_context_t *c,
                    અચલ કિંમત_Base *base,
                    અચલ કિંમત *src_કિંમત,
                    અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *layout_variation_idx_delta_map,
                    unsigned પૂર્ણાંક new_format, Flags flag) અચલ
  {
    // Filter by new format.
    જો (!(new_format & flag)) પાછા સાચું;

    કિંમત       *dst_કિંમત = c->copy (*src_કિંમત);

    જો (!dst_કિંમત) પાછા ખોટું;
    જો (*dst_કિંમત == 0) પાછા સાચું;

    *dst_કિંમત = 0;
    c->push ();
    જો ((base + get_device (src_કિંમત)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_કિંમત, c->કાઢો_pack ());
      પાછા સાચું;
    }
    નહીંતર
    {
      c->કાઢો_discard ();
      પાછા ખોટું;
    }
  }

  સ્થિર અંતઃરેખ અચલ HBINT16& get_short (અચલ કિંમત* value, તાર્કિક *worked=nullptr)
  {
    જો (worked) *worked |= તાર્કિક (*value);
    પાછા *reinterpret_cast<અચલ HBINT16 *> (value);
  }

  જાહેર:

  તાર્કિક has_device () અચલ
  {
    unsigned પૂર્ણાંક format = *આ;
    પાછા (format & devices) != 0;
  }

  તાર્કિક sanitize_કિંમત (hb_sanitize_context_t *c, અચલ કિંમત_Base *base, અચલ કિંમત *values) અચલ
  {
    TRACE_SANITIZE (આ);

    જો (unlikely (!c->check_range (values, get_size ()))) return_trace (ખોટું);

    જો (c->lazy_some_gpos)
      return_trace (સાચું);

    return_trace (!has_device () || sanitize_કિંમત_devices (c, base, values));
  }

  તાર્કિક sanitize_કિંમતો (hb_sanitize_context_t *c, અચલ કિંમત_Base *base, અચલ કિંમત *values, unsigned પૂર્ણાંક ગણતરી) અચલ
  {
    TRACE_SANITIZE (આ);
    unsigned size = get_size ();

    જો (!c->check_range (values, ગણતરી, size)) return_trace (ખોટું);

    જો (c->lazy_some_gpos)
      return_trace (સાચું);

    hb_barrier ();
    return_trace (sanitize_કિંમતો_stride_unsafe (c, base, values, ગણતરી, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  તાર્કિક sanitize_કિંમતો_stride_unsafe (hb_sanitize_context_t *c, અચલ કિંમત_Base *base, અચલ કિંમત *values, unsigned પૂર્ણાંક ગણતરી, unsigned પૂર્ણાંક stride) અચલ
  {
    TRACE_SANITIZE (આ);

    જો (!has_device ()) return_trace (સાચું);

    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી; i++) {
      જો (!sanitize_કિંમત_devices (c, base, values))
        return_trace (ખોટું);
      values = &StructAtOffset<અચલ કિંમત> (values, stride);
    }

    return_trace (સાચું);
  }

 ખાનગી:

  શૂન્ય should_drop (કિંમત value, Flags flag, unsigned પૂર્ણાંક* format) અચલ
  {
    જો (value) પાછા;
    *format = *format & ~flag;
  }

  શૂન્ય update_var_flag (અચલ કિંમત* value, Flags flag,
                        unsigned પૂર્ણાંક* format, અચલ કિંમત_Base *base,
                        અચલ hb_hashmap_t<unsigned, hb_pair_t<unsigned, પૂર્ણાંક>> *varidx_delta_map) અચલ
  {
    જો (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, પૂર્ણાંક> *varidx_delta;
      જો (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        જો (varidx_delta->second)
          *format |= flag >> 4;
        જો (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          પાછા;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

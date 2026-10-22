#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#അന്തര്ഭാവ "../../../hb-ot-layout-gsubgpos.hh"

നാമാകാശ OT {
നാമാകാശ Layout {
നാമാകാശ GPOS_impl {

തരനിർവചനം HBUINT16 മൂല്യം;

ഘടന മൂല്യം_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

തരനിർവചനം UnsizedArrayOf<മൂല്യം> മൂല്യം_Record;

ഘടന മൂല്യം_Format : HBUINT16
{
  എണ്ണൽ Flags {
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
#എങ്കിൽ 0
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

  അക്കം_Type& operator = (uint16_t i) { v = i; തിരികെ *ഇത്; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned പൂർണ്ണസംഖ്യ get_len () സ്ഥിരം  { തിരികെ hb_popcount8 ((uint8_t) *ഇത്); }
  size_t get_size () സ്ഥിരം { തിരികെ get_len () * മൂല്യം::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () സ്ഥിരം {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *ഇത്;

    എങ്കിൽ (format & xPlacement) i++;
    എങ്കിൽ (format & yPlacement) i++;
    എങ്കിൽ (format & xAdvance)   i++;
    എങ്കിൽ (format & yAdvance)   i++;

    എങ്കിൽ (format & xPlaDevice) result.push (i++);
    എങ്കിൽ (format & yPlaDevice) result.push (i++);
    എങ്കിൽ (format & xAdvDevice) result.push (i++);
    എങ്കിൽ (format & yAdvDevice) result.push (i++);

    തിരികെ result;
  }

  തർക്കം apply_മൂല്യം (hb_ot_apply_context_t *c,
                    സ്ഥിരം മൂല്യം_Base       *base,
                    സ്ഥിരം മൂല്യം           *values,
                    hb_glyph_position_t   &glyph_pos) സ്ഥിരം
  {
    തർക്കം ret = അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ format = *ഇത്;
    എങ്കിൽ (!format) തിരികെ ret;

    hb_font_t *font = c->font;
    തർക്കം horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#അല്ലെങ്കിൽ
      സത്യം
#endif
      ;

    എങ്കിൽ (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    എങ്കിൽ (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    എങ്കിൽ (format & xAdvance) {
      എങ്കിൽ (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    എങ്കിൽ (format & yAdvance) {
      എങ്കിൽ (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    എങ്കിൽ (!has_device ()) തിരികെ ret;

    തർക്കം use_x_device = font->x_ppem || font->has_nonzero_coords;
    തർക്കം use_y_device = font->y_ppem || font->has_nonzero_coords;

    എങ്കിൽ (!use_x_device && !use_y_device) തിരികെ ret;

    സ്ഥിരം ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    എങ്കിൽ (format & xPlaDevice)
    {
      എങ്കിൽ (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    എങ്കിൽ (format & yPlaDevice)
    {
      എങ്കിൽ (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    എങ്കിൽ (format & xAdvDevice)
    {
      എങ്കിൽ (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    എങ്കിൽ (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      എങ്കിൽ (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    തിരികെ ret;
  }

  ശൂന്യം position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        സ്ഥിരം മൂല്യം_Base *base,
                        സ്ഥിരം മൂല്യം *values,
                        hb_glyph_position_t &pos) സ്ഥിരം
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_മൂല്യം (&c, base, values, pos);
  }

  unsigned പൂർണ്ണസംഖ്യ get_effective_format (സ്ഥിരം മൂല്യം *values, തർക്കം strip_hints, തർക്കം strip_empty, സ്ഥിരം മൂല്യം_Base *base,
                                     സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ format = *ഇത്;
    വേണ്ടി (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      എങ്കിൽ (format & flag)
      {
        എങ്കിൽ (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          തുടരുക;
        }
        എങ്കിൽ (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          തുടരുക;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        എങ്കിൽ (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    തിരികെ format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned പൂർണ്ണസംഖ്യ get_effective_format (Iterator it, തർക്കം strip_hints, തർക്കം strip_empty, സ്ഥിരം മൂല്യം_Base *base,
                                     സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map) സ്ഥിരം {
    unsigned പൂർണ്ണസംഖ്യ new_format = 0;

    വേണ്ടി (സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    തിരികെ new_format;
  }

  ശൂന്യം copy_മൂല്യങ്ങൾ (hb_serialize_context_t *c,
                    unsigned പൂർണ്ണസംഖ്യ new_format,
                    സ്ഥിരം മൂല്യം_Base *base,
                    സ്ഥിരം മൂല്യം *values,
                    സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ format = *ഇത്;
    എങ്കിൽ (!format) തിരികെ;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    എങ്കിൽ ((format | new_format) & xPlacement)
      x_placement = copy_മൂല്യം (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (മൂല്യം));
    എങ്കിൽ ((format | new_format) & yPlacement)
      y_placement = copy_മൂല്യം (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (മൂല്യം));
    എങ്കിൽ ((format | new_format) & xAdvance)
      x_adv = copy_മൂല്യം (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (മൂല്യം));
    എങ്കിൽ ((format | new_format) & yAdvance)
      y_adv = copy_മൂല്യം (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (മൂല്യം));

    എങ്കിൽ (!has_device ())
      തിരികെ;

    എങ്കിൽ (format & xPlaDevice)
    {
      add_delta_to_മൂല്യം (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    എങ്കിൽ (format & yPlaDevice)
    {
      add_delta_to_മൂല്യം (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    എങ്കിൽ (format & xAdvDevice)
    {
      add_delta_to_മൂല്യം (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    എങ്കിൽ (format & yAdvDevice)
    {
      add_delta_to_മൂല്യം (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_മൂല്യം (hb_serialize_context_t *c,
                       unsigned പൂർണ്ണസംഖ്യ new_format,
                       Flags flag,
                       മൂല്യം value) സ്ഥിരം
  {
    // Filter by new format.
    എങ്കിൽ (!(new_format & flag)) തിരികെ nullptr;
    തിരികെ reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  ശൂന്യം collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  സ്ഥിരം മൂല്യം_Base *base,
                                  സ്ഥിരം hb_array_t<സ്ഥിരം മൂല്യം>& values) സ്ഥിരം
  {
    unsigned format = *ഇത്;
    unsigned i = 0;
    എങ്കിൽ (format & xPlacement) i++;
    എങ്കിൽ (format & yPlacement) i++;
    എങ്കിൽ (format & xAdvance) i++;
    എങ്കിൽ (format & yAdvance) i++;
    എങ്കിൽ (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    എങ്കിൽ (format & മൂല്യം_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    എങ്കിൽ (format & മൂല്യം_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    എങ്കിൽ (format & മൂല്യം_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  സ്വകാര്യ:
  തർക്കം sanitize_മൂല്യം_devices (hb_sanitize_context_t *c, സ്ഥിരം മൂല്യം_Base *base, സ്ഥിരം മൂല്യം *values) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ format = *ഇത്;

    എങ്കിൽ (format & xPlacement) values++;
    എങ്കിൽ (format & yPlacement) values++;
    എങ്കിൽ (format & xAdvance)   values++;
    എങ്കിൽ (format & yAdvance)   values++;

    എങ്കിൽ ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) തിരികെ അസത്യം;
    എങ്കിൽ ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) തിരികെ അസത്യം;
    എങ്കിൽ ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) തിരികെ അസത്യം;
    എങ്കിൽ ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) തിരികെ അസത്യം;

    തിരികെ സത്യം;
  }

  സ്ഥിതം വരിയിൽ Offset16To<Device, മൂല്യം_Base>& get_device (മൂല്യം* value)
  {
    തിരികെ *static_cast<Offset16To<Device, മൂല്യം_Base> *> (value);
  }
  സ്ഥിതം വരിയിൽ സ്ഥിരം Offset16To<Device, മൂല്യം_Base>& get_device (സ്ഥിരം മൂല്യം* value)
  {
    തിരികെ *static_cast<സ്ഥിരം Offset16To<Device, മൂല്യം_Base> *> (value);
  }
  സ്ഥിതം വരിയിൽ സ്ഥിരം Device& get_device (സ്ഥിരം മൂല്യം* value,
					  തർക്കം *worked,
					  സ്ഥിരം മൂല്യം_Base *base,
					  hb_sanitize_context_t &c)
  {
    എങ്കിൽ (worked) *worked |= തർക്കം (*value);
    auto &offset = *static_cast<സ്ഥിരം Offset16To<Device> *> (value);

    എങ്കിൽ (unlikely (!offset.sanitize (&c, base)))
      തിരികെ Null(Device);
    hb_barrier ();

    തിരികെ base + offset;
  }

  ശൂന്യം add_delta_to_മൂല്യം (HBINT16 *value,
                           സ്ഥിരം മൂല്യം_Base *base,
                           സ്ഥിരം മൂല്യം *src_മൂല്യം,
                           സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map) സ്ഥിരം
  {
    എങ്കിൽ (!value) തിരികെ;
    unsigned varidx = (base + get_device (src_മൂല്യം)).get_variation_index ();
    hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *varidx_delta;
    എങ്കിൽ (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) തിരികെ;

    *value += hb_second (*varidx_delta);
  }

  തർക്കം copy_device (hb_serialize_context_t *c,
                    സ്ഥിരം മൂല്യം_Base *base,
                    സ്ഥിരം മൂല്യം *src_മൂല്യം,
                    സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *layout_variation_idx_delta_map,
                    unsigned പൂർണ്ണസംഖ്യ new_format, Flags flag) സ്ഥിരം
  {
    // Filter by new format.
    എങ്കിൽ (!(new_format & flag)) തിരികെ സത്യം;

    മൂല്യം       *dst_മൂല്യം = c->copy (*src_മൂല്യം);

    എങ്കിൽ (!dst_മൂല്യം) തിരികെ അസത്യം;
    എങ്കിൽ (*dst_മൂല്യം == 0) തിരികെ സത്യം;

    *dst_മൂല്യം = 0;
    c->push ();
    എങ്കിൽ ((base + get_device (src_മൂല്യം)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_മൂല്യം, c->എടുക്കുക_pack ());
      തിരികെ സത്യം;
    }
    അല്ലെങ്കിൽ
    {
      c->എടുക്കുക_discard ();
      തിരികെ അസത്യം;
    }
  }

  സ്ഥിതം വരിയിൽ സ്ഥിരം HBINT16& get_short (സ്ഥിരം മൂല്യം* value, തർക്കം *worked=nullptr)
  {
    എങ്കിൽ (worked) *worked |= തർക്കം (*value);
    തിരികെ *reinterpret_cast<സ്ഥിരം HBINT16 *> (value);
  }

  പൊതു:

  തർക്കം has_device () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ format = *ഇത്;
    തിരികെ (format & devices) != 0;
  }

  തർക്കം sanitize_മൂല്യം (hb_sanitize_context_t *c, സ്ഥിരം മൂല്യം_Base *base, സ്ഥിരം മൂല്യം *values) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    എങ്കിൽ (unlikely (!c->check_range (values, get_size ()))) return_trace (അസത്യം);

    എങ്കിൽ (c->lazy_some_gpos)
      return_trace (സത്യം);

    return_trace (!has_device () || sanitize_മൂല്യം_devices (c, base, values));
  }

  തർക്കം sanitize_മൂല്യങ്ങൾ (hb_sanitize_context_t *c, സ്ഥിരം മൂല്യം_Base *base, സ്ഥിരം മൂല്യം *values, unsigned പൂർണ്ണസംഖ്യ എണ്ണം) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    unsigned size = get_size ();

    എങ്കിൽ (!c->check_range (values, എണ്ണം, size)) return_trace (അസത്യം);

    എങ്കിൽ (c->lazy_some_gpos)
      return_trace (സത്യം);

    hb_barrier ();
    return_trace (sanitize_മൂല്യങ്ങൾ_stride_unsafe (c, base, values, എണ്ണം, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  തർക്കം sanitize_മൂല്യങ്ങൾ_stride_unsafe (hb_sanitize_context_t *c, സ്ഥിരം മൂല്യം_Base *base, സ്ഥിരം മൂല്യം *values, unsigned പൂർണ്ണസംഖ്യ എണ്ണം, unsigned പൂർണ്ണസംഖ്യ stride) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);

    എങ്കിൽ (!has_device ()) return_trace (സത്യം);

    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം; i++) {
      എങ്കിൽ (!sanitize_മൂല്യം_devices (c, base, values))
        return_trace (അസത്യം);
      values = &StructAtOffset<സ്ഥിരം മൂല്യം> (values, stride);
    }

    return_trace (സത്യം);
  }

 സ്വകാര്യ:

  ശൂന്യം should_drop (മൂല്യം value, Flags flag, unsigned പൂർണ്ണസംഖ്യ* format) സ്ഥിരം
  {
    എങ്കിൽ (value) തിരികെ;
    *format = *format & ~flag;
  }

  ശൂന്യം update_var_flag (സ്ഥിരം മൂല്യം* value, Flags flag,
                        unsigned പൂർണ്ണസംഖ്യ* format, സ്ഥിരം മൂല്യം_Base *base,
                        സ്ഥിരം hb_hashmap_t<unsigned, hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ>> *varidx_delta_map) സ്ഥിരം
  {
    എങ്കിൽ (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, പൂർണ്ണസംഖ്യ> *varidx_delta;
      എങ്കിൽ (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        എങ്കിൽ (varidx_delta->second)
          *format |= flag >> 4;
        എങ്കിൽ (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          തിരികെ;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

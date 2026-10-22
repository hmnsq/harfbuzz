#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#ଅନ୍ତର୍ଭାଵ "../../../hb-ot-layout-gsubgpos.hh"

ନାମାକାଶ OT {
ନାମାକାଶ Layout {
ନାମାକାଶ GPOS_impl {

ପ୍ରକାରବ୍ୟାଖ୍ୟା HBUINT16 ମୂଲ୍ୟ;

ସଂରଚନା ମୂଲ୍ୟ_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

ପ୍ରକାରବ୍ୟାଖ୍ୟା UnsizedArrayOf<ମୂଲ୍ୟ> ମୂଲ୍ୟ_Record;

ସଂରଚନା ମୂଲ୍ୟ_Format : HBUINT16
{
  ଗଣନା Flags {
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
#ଯଦି 0
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

  ଅଙ୍କ_Type& operator = (uint16_t i) { v = i; ଫେରନ୍ତୁ *ଏହି; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_len () ଅଚଳ  { ଫେରନ୍ତୁ hb_popcount8 ((uint8_t) *ଏହି); }
  size_t get_size () ଅଚଳ { ଫେରନ୍ତୁ get_len () * ମୂଲ୍ୟ::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ଅଚଳ {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *ଏହି;

    ଯଦି (format & xPlacement) i++;
    ଯଦି (format & yPlacement) i++;
    ଯଦି (format & xAdvance)   i++;
    ଯଦି (format & yAdvance)   i++;

    ଯଦି (format & xPlaDevice) result.push (i++);
    ଯଦି (format & yPlaDevice) result.push (i++);
    ଯଦି (format & xAdvDevice) result.push (i++);
    ଯଦି (format & yAdvDevice) result.push (i++);

    ଫେରନ୍ତୁ result;
  }

  ତାର୍କିକ apply_ମୂଲ୍ୟ (hb_ot_apply_context_t *c,
                    ଅଚଳ ମୂଲ୍ୟ_Base       *base,
                    ଅଚଳ ମୂଲ୍ୟ           *values,
                    hb_glyph_position_t   &glyph_pos) ଅଚଳ
  {
    ତାର୍କିକ ret = ମିଥ୍ୟା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = *ଏହି;
    ଯଦି (!format) ଫେରନ୍ତୁ ret;

    hb_font_t *font = c->font;
    ତାର୍କିକ horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#ଅନ୍ୟଥା
      ସତ୍ୟ
#endif
      ;

    ଯଦି (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    ଯଦି (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    ଯଦି (format & xAdvance) {
      ଯଦି (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    ଯଦି (format & yAdvance) {
      ଯଦି (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    ଯଦି (!has_device ()) ଫେରନ୍ତୁ ret;

    ତାର୍କିକ use_x_device = font->x_ppem || font->has_nonzero_coords;
    ତାର୍କିକ use_y_device = font->y_ppem || font->has_nonzero_coords;

    ଯଦି (!use_x_device && !use_y_device) ଫେରନ୍ତୁ ret;

    ଅଚଳ ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    ଯଦି (format & xPlaDevice)
    {
      ଯଦି (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ଯଦି (format & yPlaDevice)
    {
      ଯଦି (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ଯଦି (format & xAdvDevice)
    {
      ଯଦି (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ଯଦି (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      ଯଦି (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ଫେରନ୍ତୁ ret;
  }

  ଶୂନ୍ୟ position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ଅଚଳ ମୂଲ୍ୟ_Base *base,
                        ଅଚଳ ମୂଲ୍ୟ *values,
                        hb_glyph_position_t &pos) ଅଚଳ
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_ମୂଲ୍ୟ (&c, base, values, pos);
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_effective_format (ଅଚଳ ମୂଲ୍ୟ *values, ତାର୍କିକ strip_hints, ତାର୍କିକ strip_empty, ଅଚଳ ମୂଲ୍ୟ_Base *base,
                                     ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = *ଏହି;
    ପାଇଁ (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      ଯଦି (format & flag)
      {
        ଯଦି (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          ଜାରି;
        }
        ଯଦି (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          ଜାରି;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        ଯଦି (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ଫେରନ୍ତୁ format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_effective_format (Iterator it, ତାର୍କିକ strip_hints, ତାର୍କିକ strip_empty, ଅଚଳ ମୂଲ୍ୟ_Base *base,
                                     ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map) ଅଚଳ {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_format = 0;

    ପାଇଁ (ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    ଫେରନ୍ତୁ new_format;
  }

  ଶୂନ୍ୟ copy_ମୂଲ୍ୟଗୁଡ଼ିକ (hb_serialize_context_t *c,
                    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_format,
                    ଅଚଳ ମୂଲ୍ୟ_Base *base,
                    ଅଚଳ ମୂଲ୍ୟ *values,
                    ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = *ଏହି;
    ଯଦି (!format) ଫେରନ୍ତୁ;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    ଯଦି ((format | new_format) & xPlacement)
      x_placement = copy_ମୂଲ୍ୟ (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (ମୂଲ୍ୟ));
    ଯଦି ((format | new_format) & yPlacement)
      y_placement = copy_ମୂଲ୍ୟ (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (ମୂଲ୍ୟ));
    ଯଦି ((format | new_format) & xAdvance)
      x_adv = copy_ମୂଲ୍ୟ (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (ମୂଲ୍ୟ));
    ଯଦି ((format | new_format) & yAdvance)
      y_adv = copy_ମୂଲ୍ୟ (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (ମୂଲ୍ୟ));

    ଯଦି (!has_device ())
      ଫେରନ୍ତୁ;

    ଯଦି (format & xPlaDevice)
    {
      add_delta_to_ମୂଲ୍ୟ (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    ଯଦି (format & yPlaDevice)
    {
      add_delta_to_ମୂଲ୍ୟ (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    ଯଦି (format & xAdvDevice)
    {
      add_delta_to_ମୂଲ୍ୟ (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    ଯଦି (format & yAdvDevice)
    {
      add_delta_to_ମୂଲ୍ୟ (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_ମୂଲ୍ୟ (hb_serialize_context_t *c,
                       unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_format,
                       Flags flag,
                       ମୂଲ୍ୟ value) ଅଚଳ
  {
    // Filter by new format.
    ଯଦି (!(new_format & flag)) ଫେରନ୍ତୁ nullptr;
    ଫେରନ୍ତୁ reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  ଶୂନ୍ୟ collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ଅଚଳ ମୂଲ୍ୟ_Base *base,
                                  ଅଚଳ hb_array_t<ଅଚଳ ମୂଲ୍ୟ>& values) ଅଚଳ
  {
    unsigned format = *ଏହି;
    unsigned i = 0;
    ଯଦି (format & xPlacement) i++;
    ଯଦି (format & yPlacement) i++;
    ଯଦି (format & xAdvance) i++;
    ଯଦି (format & yAdvance) i++;
    ଯଦି (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ଯଦି (format & ମୂଲ୍ୟ_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ଯଦି (format & ମୂଲ୍ୟ_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ଯଦି (format & ମୂଲ୍ୟ_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  ବ୍ୟକ୍ତିଗତ:
  ତାର୍କିକ sanitize_ମୂଲ୍ୟ_devices (hb_sanitize_context_t *c, ଅଚଳ ମୂଲ୍ୟ_Base *base, ଅଚଳ ମୂଲ୍ୟ *values) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = *ଏହି;

    ଯଦି (format & xPlacement) values++;
    ଯଦି (format & yPlacement) values++;
    ଯଦି (format & xAdvance)   values++;
    ଯଦି (format & yAdvance)   values++;

    ଯଦି ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ Offset16To<Device, ମୂଲ୍ୟ_Base>& get_device (ମୂଲ୍ୟ* value)
  {
    ଫେରନ୍ତୁ *static_cast<Offset16To<Device, ମୂଲ୍ୟ_Base> *> (value);
  }
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ Offset16To<Device, ମୂଲ୍ୟ_Base>& get_device (ଅଚଳ ମୂଲ୍ୟ* value)
  {
    ଫେରନ୍ତୁ *static_cast<ଅଚଳ Offset16To<Device, ମୂଲ୍ୟ_Base> *> (value);
  }
  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ Device& get_device (ଅଚଳ ମୂଲ୍ୟ* value,
					  ତାର୍କିକ *worked,
					  ଅଚଳ ମୂଲ୍ୟ_Base *base,
					  hb_sanitize_context_t &c)
  {
    ଯଦି (worked) *worked |= ତାର୍କିକ (*value);
    auto &offset = *static_cast<ଅଚଳ Offset16To<Device> *> (value);

    ଯଦି (unlikely (!offset.sanitize (&c, base)))
      ଫେରନ୍ତୁ Null(Device);
    hb_barrier ();

    ଫେରନ୍ତୁ base + offset;
  }

  ଶୂନ୍ୟ add_delta_to_ମୂଲ୍ୟ (HBINT16 *value,
                           ଅଚଳ ମୂଲ୍ୟ_Base *base,
                           ଅଚଳ ମୂଲ୍ୟ *src_ମୂଲ୍ୟ,
                           ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map) ଅଚଳ
  {
    ଯଦି (!value) ଫେରନ୍ତୁ;
    unsigned varidx = (base + get_device (src_ମୂଲ୍ୟ)).get_variation_index ();
    hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *varidx_delta;
    ଯଦି (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ଫେରନ୍ତୁ;

    *value += hb_second (*varidx_delta);
  }

  ତାର୍କିକ copy_device (hb_serialize_context_t *c,
                    ଅଚଳ ମୂଲ୍ୟ_Base *base,
                    ଅଚଳ ମୂଲ୍ୟ *src_ମୂଲ୍ୟ,
                    ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *layout_variation_idx_delta_map,
                    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_format, Flags flag) ଅଚଳ
  {
    // Filter by new format.
    ଯଦି (!(new_format & flag)) ଫେରନ୍ତୁ ସତ୍ୟ;

    ମୂଲ୍ୟ       *dst_ମୂଲ୍ୟ = c->copy (*src_ମୂଲ୍ୟ);

    ଯଦି (!dst_ମୂଲ୍ୟ) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (*dst_ମୂଲ୍ୟ == 0) ଫେରନ୍ତୁ ସତ୍ୟ;

    *dst_ମୂଲ୍ୟ = 0;
    c->push ();
    ଯଦି ((base + get_device (src_ମୂଲ୍ୟ)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_ମୂଲ୍ୟ, c->କାଢ଼_pack ());
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଅନ୍ୟଥା
    {
      c->କାଢ଼_discard ();
      ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
  }

  ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ HBINT16& get_short (ଅଚଳ ମୂଲ୍ୟ* value, ତାର୍କିକ *worked=nullptr)
  {
    ଯଦି (worked) *worked |= ତାର୍କିକ (*value);
    ଫେରନ୍ତୁ *reinterpret_cast<ଅଚଳ HBINT16 *> (value);
  }

  ସର୍ବସାଧାରଣ:

  ତାର୍କିକ has_device () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା format = *ଏହି;
    ଫେରନ୍ତୁ (format & devices) != 0;
  }

  ତାର୍କିକ sanitize_ମୂଲ୍ୟ (hb_sanitize_context_t *c, ଅଚଳ ମୂଲ୍ୟ_Base *base, ଅଚଳ ମୂଲ୍ୟ *values) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    ଯଦି (unlikely (!c->check_range (values, get_size ()))) return_trace (ମିଥ୍ୟା);

    ଯଦି (c->lazy_some_gpos)
      return_trace (ସତ୍ୟ);

    return_trace (!has_device () || sanitize_ମୂଲ୍ୟ_devices (c, base, values));
  }

  ତାର୍କିକ sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ (hb_sanitize_context_t *c, ଅଚଳ ମୂଲ୍ୟ_Base *base, ଅଚଳ ମୂଲ୍ୟ *values, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    unsigned size = get_size ();

    ଯଦି (!c->check_range (values, ଗଣନା_2, size)) return_trace (ମିଥ୍ୟା);

    ଯଦି (c->lazy_some_gpos)
      return_trace (ସତ୍ୟ);

    hb_barrier ();
    return_trace (sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ_stride_unsafe (c, base, values, ଗଣନା_2, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  ତାର୍କିକ sanitize_ମୂଲ୍ୟଗୁଡ଼ିକ_stride_unsafe (hb_sanitize_context_t *c, ଅଚଳ ମୂଲ୍ୟ_Base *base, ଅଚଳ ମୂଲ୍ୟ *values, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା stride) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);

    ଯଦି (!has_device ()) return_trace (ସତ୍ୟ);

    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2; i++) {
      ଯଦି (!sanitize_ମୂଲ୍ୟ_devices (c, base, values))
        return_trace (ମିଥ୍ୟା);
      values = &StructAtOffset<ଅଚଳ ମୂଲ୍ୟ> (values, stride);
    }

    return_trace (ସତ୍ୟ);
  }

 ବ୍ୟକ୍ତିଗତ:

  ଶୂନ୍ୟ should_drop (ମୂଲ୍ୟ value, Flags flag, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା* format) ଅଚଳ
  {
    ଯଦି (value) ଫେରନ୍ତୁ;
    *format = *format & ~flag;
  }

  ଶୂନ୍ୟ update_var_flag (ଅଚଳ ମୂଲ୍ୟ* value, Flags flag,
                        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା* format, ଅଚଳ ମୂଲ୍ୟ_Base *base,
                        ଅଚଳ hb_hashmap_t<unsigned, hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>> *varidx_delta_map) ଅଚଳ
  {
    ଯଦି (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, ପୂର୍ଣ୍ଣସଂଖ୍ୟା> *varidx_delta;
      ଯଦି (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        ଯଦି (varidx_delta->second)
          *format |= flag >> 4;
        ଯଦି (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ଫେରନ୍ତୁ;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

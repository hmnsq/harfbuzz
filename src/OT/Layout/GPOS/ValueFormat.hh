#నిర్వచితం_కాకపోతే OT_LAYOUT_GPOS_VALUEFORMAT_HH
#నిర్వచించు OT_LAYOUT_GPOS_VALUEFORMAT_HH

#అన్తర్భావ "../../../hb-ot-layout-gsubgpos.hh"

నామాకాశ OT {
నామాకాశ Layout {
నామాకాశ GPOS_impl {

రకనిర్వచనం HBUINT16 విలువ;

నిర్మాణం విలువ_పునాది {}; // Dummy base class tag for OffsetTo<Value> bases.

రకనిర్వచనం Unsized_Array_యొక్క<విలువ> విలువ_రికార్డు;

నిర్మాణం విలువ_Format : HBUINT16
{
  గణన Flags {
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
#ఒకవేళ 0
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
  Offset16_గమ్యం<Device>    xPlaDevice;     /* Offset to Device table for
                                         * horizontal placement--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_గమ్యం<Device>    yPlaDevice;     /* Offset to Device table for vertical
                                         * placement--measured from beginning
                                         * of PosTable (may be NULL) */
  Offset16_గమ్యం<Device>    xAdvDevice;     /* Offset to Device table for
                                         * horizontal advance--measured from
                                         * beginning of PosTable (may be NULL) */
  Offset16_గమ్యం<Device>    yAdvDevice;     /* Offset to Device table for vertical
                                         * advance--measured from beginning of
                                         * PosTable (may be NULL) */
#అయితే_ముగింపు

  అంకె_Type& క్రియాకారకం = (uint16_t i) { v = i; తిరిగి *ఇది; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  గుర్తులేని పూర్ణాంకం get_len () స్థిరరాశి  { తిరిగి hb_popcount8 ((uint8_t) *ఇది); }
  size_t get_కొలత () స్థిరరాశి { తిరిగి get_len () * విలువ::static_కొలత; }

  hb_సదిశ_t<గుర్తులేని> get_device_పట్టిక_indices () స్థిరరాశి {
    గుర్తులేని i = 0;
    hb_సదిశ_t<గుర్తులేని> ఫలితం;
    గుర్తులేని format = *ఇది;

    ఒకవేళ (format & xPlacement) i++;
    ఒకవేళ (format & yPlacement) i++;
    ఒకవేళ (format & xAdvance)   i++;
    ఒకవేళ (format & yAdvance)   i++;

    ఒకవేళ (format & xPlaDevice) ఫలితం.push (i++);
    ఒకవేళ (format & yPlaDevice) ఫలితం.push (i++);
    ఒకవేళ (format & xAdvDevice) ఫలితం.push (i++);
    ఒకవేళ (format & yAdvDevice) ఫలితం.push (i++);

    తిరిగి ఫలితం;
  }

  తార్కిక apply_విలువ (hb_ot_apply_context_t *c,
                    స్థిరరాశి విలువ_పునాది       *పునాది,
                    స్థిరరాశి విలువ           *values,
                    hb_glyph_position_t   &glyph_pos) స్థిరరాశి
  {
    తార్కిక ret = అబద్ధం;
    గుర్తులేని పూర్ణాంకం format = *ఇది;
    ఒకవేళ (!format) తిరిగి ret;

    hb_font_t *font = c->font;
    తార్కిక horizontal =
#నిర్వచితం_కాకపోతే HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#లేకపోతే
      నిజం
#అయితే_ముగింపు
      ;

    ఒకవేళ (format & xPlacement)
      glyph_pos.x_స్థానభ్రంశం_2 = hb_saturate_కూడు (glyph_pos.x_స్థానభ్రంశం_2, font->em_scale_x (get_short (values++, &ret)));
    ఒకవేళ (format & yPlacement)
      glyph_pos.y_స్థానభ్రంశం_2 = hb_saturate_కూడు (glyph_pos.y_స్థానభ్రంశం_2, font->em_scale_y (get_short (values++, &ret)));
    ఒకవేళ (format & xAdvance) {
      ఒకవేళ (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_కూడు (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    ఒకవేళ (format & yAdvance) {
      ఒకవేళ (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    ఒకవేళ (!has_device ()) తిరిగి ret;

    తార్కిక use_x_device = font->x_ppem || font->has_nonzero_coords;
    తార్కిక use_y_device = font->y_ppem || font->has_nonzero_coords;

    ఒకవేళ (!use_x_device && !use_y_device) తిరిగి ret;

    స్థిరరాశి అంశం_Variation_Store &store = c->var_store;
    స్వయంచాలక *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    ఒకవేళ (format & xPlaDevice)
    {
      ఒకవేళ (use_x_device)
	glyph_pos.x_స్థానభ్రంశం_2 = hb_saturate_కూడు (glyph_pos.x_స్థానభ్రంశం_2, get_device (values, &ret, పునాది, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ఒకవేళ (format & yPlaDevice)
    {
      ఒకవేళ (use_y_device)
	glyph_pos.y_స్థానభ్రంశం_2 = hb_saturate_కూడు (glyph_pos.y_స్థానభ్రంశం_2, get_device (values, &ret, పునాది, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ఒకవేళ (format & xAdvDevice)
    {
      ఒకవేళ (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_కూడు (glyph_pos.x_advance, get_device (values, &ret, పునాది, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ఒకవేళ (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      ఒకవేళ (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, పునాది, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    తిరిగి ret;
  }

  శూన్య position_single (hb_font_t *font,
                        hb_blob_t *పట్టిక_blob,
                        hb_direction_t direction,
                        స్థిరరాశి విలువ_పునాది *పునాది,
                        స్థిరరాశి విలువ *values,
                        hb_glyph_position_t &pos) స్థిరరాశి
  {
    /* This is ugly... */
    hb_బఫరు_t బఫరు {};
    బఫరు.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &బఫరు, పట్టిక_blob);

    apply_విలువ (&c, పునాది, values, pos);
  }

  గుర్తులేని పూర్ణాంకం get_effective_format (స్థిరరాశి విలువ *values, తార్కిక strip_hints, తార్కిక strip_ఖాళీలు, స్థిరరాశి విలువ_పునాది *పునాది,
                                     స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం format = *ఇది;
    కొరకు (గుర్తులేని flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      ఒకవేళ (format & flag)
      {
        ఒకవేళ (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          కొనసాగు;
        }
        ఒకవేళ (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, పునాది, varidx_delta_map);
          కొనసాగు;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        ఒకవేళ (strip_ఖాళీలు) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    తిరిగి format;
  }

  మూస<రకం_పేరు Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  గుర్తులేని పూర్ణాంకం get_effective_format (Iterator it, తార్కిక strip_hints, తార్కిక strip_ఖాళీలు, స్థిరరాశి విలువ_పునాది *పునాది,
                                     స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map) స్థిరరాశి {
    గుర్తులేని పూర్ణాంకం new_format = 0;

    కొరకు (స్థిరరాశి hb_array_t<స్థిరరాశి విలువ>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_ఖాళీలు, పునాది, varidx_delta_map);

    తిరిగి new_format;
  }

  శూన్య copy_విలువలు (hb_serialize_context_t *c,
                    గుర్తులేని పూర్ణాంకం new_format,
                    స్థిరరాశి విలువ_పునాది *పునాది,
                    స్థిరరాశి విలువ *values,
                    స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం format = *ఇది;
    ఒకవేళ (!format) తిరిగి;

    HBINT16 *x_placement = శూన్యసూచిక, *y_placement = శూన్యసూచిక, *x_adv = శూన్యసూచిక, *y_adv = శూన్యసూచిక;
    ఒకవేళ ((format | new_format) & xPlacement)
      x_placement = copy_విలువ (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (విలువ));
    ఒకవేళ ((format | new_format) & yPlacement)
      y_placement = copy_విలువ (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (విలువ));
    ఒకవేళ ((format | new_format) & xAdvance)
      x_adv = copy_విలువ (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (విలువ));
    ఒకవేళ ((format | new_format) & yAdvance)
      y_adv = copy_విలువ (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (విలువ));

    ఒకవేళ (!has_device ())
      తిరిగి;

    ఒకవేళ (format & xPlaDevice)
    {
      కూడు_delta_గమ్యం_విలువ (x_placement, పునాది, values, layout_variation_idx_delta_map);
      copy_device (c, పునాది, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    ఒకవేళ (format & yPlaDevice)
    {
      కూడు_delta_గమ్యం_విలువ (y_placement, పునాది, values, layout_variation_idx_delta_map);
      copy_device (c, పునాది, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    ఒకవేళ (format & xAdvDevice)
    {
      కూడు_delta_గమ్యం_విలువ (x_adv, పునాది, values, layout_variation_idx_delta_map);
      copy_device (c, పునాది, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    ఒకవేళ (format & yAdvDevice)
    {
      కూడు_delta_గమ్యం_విలువ (y_adv, పునాది, values, layout_variation_idx_delta_map);
      copy_device (c, పునాది, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_విలువ (hb_serialize_context_t *c,
                       గుర్తులేని పూర్ణాంకం new_format,
                       Flags flag,
                       విలువ value) స్థిరరాశి
  {
    // Filter by new format.
    ఒకవేళ (!(new_format & flag)) తిరిగి శూన్యసూచిక;
    తిరిగి reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  శూన్య collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  స్థిరరాశి విలువ_పునాది *పునాది,
                                  స్థిరరాశి hb_array_t<స్థిరరాశి విలువ>& values) స్థిరరాశి
  {
    గుర్తులేని format = *ఇది;
    గుర్తులేని i = 0;
    ఒకవేళ (format & xPlacement) i++;
    ఒకవేళ (format & yPlacement) i++;
    ఒకవేళ (format & xAdvance) i++;
    ఒకవేళ (format & yAdvance) i++;
    ఒకవేళ (format & xPlaDevice)
    {
      (పునాది + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ఒకవేళ (format & విలువ_Format::yPlaDevice)
    {
      (పునాది + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ఒకవేళ (format & విలువ_Format::xAdvDevice)
    {
      (పునాది + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ఒకవేళ (format & విలువ_Format::yAdvDevice)
    {
      (పునాది + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  అంతరంగ:
  తార్కిక sanitize_విలువ_devices (hb_sanitize_context_t *c, స్థిరరాశి విలువ_పునాది *పునాది, స్థిరరాశి విలువ *values) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం format = *ఇది;

    ఒకవేళ (format & xPlacement) values++;
    ఒకవేళ (format & yPlacement) values++;
    ఒకవేళ (format & xAdvance)   values++;
    ఒకవేళ (format & yAdvance)   values++;

    ఒకవేళ ((format & xPlaDevice) && !get_device (values++).sanitize (c, పునాది)) తిరిగి అబద్ధం;
    ఒకవేళ ((format & yPlaDevice) && !get_device (values++).sanitize (c, పునాది)) తిరిగి అబద్ధం;
    ఒకవేళ ((format & xAdvDevice) && !get_device (values++).sanitize (c, పునాది)) తిరిగి అబద్ధం;
    ఒకవేళ ((format & yAdvDevice) && !get_device (values++).sanitize (c, పునాది)) తిరిగి అబద్ధం;

    తిరిగి నిజం;
  }

  స్థిర అంతఃవరుస Offset16_గమ్యం<Device, విలువ_పునాది>& get_device (విలువ* value)
  {
    తిరిగి *static_cast<Offset16_గమ్యం<Device, విలువ_పునాది> *> (value);
  }
  స్థిర అంతఃవరుస స్థిరరాశి Offset16_గమ్యం<Device, విలువ_పునాది>& get_device (స్థిరరాశి విలువ* value)
  {
    తిరిగి *static_cast<స్థిరరాశి Offset16_గమ్యం<Device, విలువ_పునాది> *> (value);
  }
  స్థిర అంతఃవరుస స్థిరరాశి Device& get_device (స్థిరరాశి విలువ* value,
					  తార్కిక *worked,
					  స్థిరరాశి విలువ_పునాది *పునాది,
					  hb_sanitize_context_t &c)
  {
    ఒకవేళ (worked) *worked |= తార్కిక (*value);
    స్వయంచాలక &స్థానభ్రంశం = *static_cast<స్థిరరాశి Offset16_గమ్యం<Device> *> (value);

    ఒకవేళ (unlikely (!స్థానభ్రంశం.sanitize (&c, పునాది)))
      తిరిగి Null(Device);
    hb_barrier ();

    తిరిగి పునాది + స్థానభ్రంశం;
  }

  శూన్య కూడు_delta_గమ్యం_విలువ (HBINT16 *value,
                           స్థిరరాశి విలువ_పునాది *పునాది,
                           స్థిరరాశి విలువ *src_విలువ,
                           స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map) స్థిరరాశి
  {
    ఒకవేళ (!value) తిరిగి;
    గుర్తులేని varidx = (పునాది + get_device (src_విలువ)).get_variation_సూచీ ();
    hb_pair_t<గుర్తులేని, పూర్ణాంకం> *varidx_delta;
    ఒకవేళ (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) తిరిగి;

    *value += hb_రెండవ (*varidx_delta);
  }

  తార్కిక copy_device (hb_serialize_context_t *c,
                    స్థిరరాశి విలువ_పునాది *పునాది,
                    స్థిరరాశి విలువ *src_విలువ,
                    స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *layout_variation_idx_delta_map,
                    గుర్తులేని పూర్ణాంకం new_format, Flags flag) స్థిరరాశి
  {
    // Filter by new format.
    ఒకవేళ (!(new_format & flag)) తిరిగి నిజం;

    విలువ       *dst_విలువ = c->copy (*src_విలువ);

    ఒకవేళ (!dst_విలువ) తిరిగి అబద్ధం;
    ఒకవేళ (*dst_విలువ == 0) తిరిగి నిజం;

    *dst_విలువ = 0;
    c->push ();
    ఒకవేళ ((పునాది + get_device (src_విలువ)).copy (c, layout_variation_idx_delta_map))
    {
      c->కూడు_link (*dst_విలువ, c->తీయి_pack ());
      తిరిగి నిజం;
    }
    లేకపోతే
    {
      c->తీయి_discard ();
      తిరిగి అబద్ధం;
    }
  }

  స్థిర అంతఃవరుస స్థిరరాశి HBINT16& get_short (స్థిరరాశి విలువ* value, తార్కిక *worked=శూన్యసూచిక)
  {
    ఒకవేళ (worked) *worked |= తార్కిక (*value);
    తిరిగి *reinterpret_cast<స్థిరరాశి HBINT16 *> (value);
  }

  బహిరంగ:

  తార్కిక has_device () స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం format = *ఇది;
    తిరిగి (format & devices) != 0;
  }

  తార్కిక sanitize_విలువ (hb_sanitize_context_t *c, స్థిరరాశి విలువ_పునాది *పునాది, స్థిరరాశి విలువ *values) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    ఒకవేళ (unlikely (!c->check_range (values, get_కొలత ()))) return_trace (అబద్ధం);

    ఒకవేళ (c->lazy_some_gpos)
      return_trace (నిజం);

    return_trace (!has_device () || sanitize_విలువ_devices (c, పునాది, values));
  }

  తార్కిక sanitize_విలువలు (hb_sanitize_context_t *c, స్థిరరాశి విలువ_పునాది *పునాది, స్థిరరాశి విలువ *values, గుర్తులేని పూర్ణాంకం లెక్క) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    గుర్తులేని కొలత = get_కొలత ();

    ఒకవేళ (!c->check_range (values, లెక్క, కొలత)) return_trace (అబద్ధం);

    ఒకవేళ (c->lazy_some_gpos)
      return_trace (నిజం);

    hb_barrier ();
    return_trace (sanitize_విలువలు_stride_unsafe (c, పునాది, values, లెక్క, కొలత));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  తార్కిక sanitize_విలువలు_stride_unsafe (hb_sanitize_context_t *c, స్థిరరాశి విలువ_పునాది *పునాది, స్థిరరాశి విలువ *values, గుర్తులేని పూర్ణాంకం లెక్క, గుర్తులేని పూర్ణాంకం stride) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);

    ఒకవేళ (!has_device ()) return_trace (నిజం);

    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < లెక్క; i++) {
      ఒకవేళ (!sanitize_విలువ_devices (c, పునాది, values))
        return_trace (అబద్ధం);
      values = &Struct_At_స్థానభ్రంశం<స్థిరరాశి విలువ> (values, stride);
    }

    return_trace (నిజం);
  }

 అంతరంగ:

  శూన్య should_drop (విలువ value, Flags flag, గుర్తులేని పూర్ణాంకం* format) స్థిరరాశి
  {
    ఒకవేళ (value) తిరిగి;
    *format = *format & ~flag;
  }

  శూన్య update_var_flag (స్థిరరాశి విలువ* value, Flags flag,
                        గుర్తులేని పూర్ణాంకం* format, స్థిరరాశి విలువ_పునాది *పునాది,
                        స్థిరరాశి hb_hashmap_t<గుర్తులేని, hb_pair_t<గుర్తులేని, పూర్ణాంకం>> *varidx_delta_map) స్థిరరాశి
  {
    ఒకవేళ (*value)
    {
      గుర్తులేని varidx = (పునాది + get_device (value)).get_variation_సూచీ ();
      hb_pair_t<గుర్తులేని, పూర్ణాంకం> *varidx_delta;
      ఒకవేళ (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        ఒకవేళ (varidx_delta->second)
          *format |= flag >> 4;
        ఒకవేళ (varidx_delta->మొదటి != HB_OT_LAYOUT_NO_VARIATIONS_సూచీ)
          తిరిగి;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#అయితే_ముగింపు  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

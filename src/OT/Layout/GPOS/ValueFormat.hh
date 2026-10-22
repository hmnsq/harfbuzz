#अगर_अघोषित OT_LAYOUT_GPOS_VALUEFORMAT_HH
#घोषणा OT_LAYOUT_GPOS_VALUEFORMAT_HH

#समावेश "../../../hb-ot-layout-gsubgpos.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

प्रकार HBUINT16 मूल्य;

काष्ठा मूल्य_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

प्रकार UnsizedArrayOf<मूल्य> मूल्य_Record;

काष्ठा मूल्य_Format : HBUINT16
{
  क्रमागत Flags {
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
#अगर 0
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
#पूर्ण_अगर

  अंक_Type& चालक = (uint16_t i) { v = i; वापस *यह; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  अचिन्हित पूर्णांक get_len () स्थिर  { वापस hb_popcount8 ((uint8_t) *यह); }
  माप_प्रकार get_आकार () स्थिर { वापस get_len () * मूल्य::static_आकार; }

  hb_vector_t<अचिन्हित> get_device_table_indices () स्थिर {
    अचिन्हित i = 0;
    hb_vector_t<अचिन्हित> परिणाम;
    अचिन्हित format = *यह;

    अगर (format & xPlacement) i++;
    अगर (format & yPlacement) i++;
    अगर (format & xAdvance)   i++;
    अगर (format & yAdvance)   i++;

    अगर (format & xPlaDevice) परिणाम.push (i++);
    अगर (format & yPlaDevice) परिणाम.push (i++);
    अगर (format & xAdvDevice) परिणाम.push (i++);
    अगर (format & yAdvDevice) परिणाम.push (i++);

    वापस परिणाम;
  }

  bool apply_मूल्य (hb_ot_apply_context_t *c,
                    स्थिर मूल्य_Base       *base,
                    स्थिर मूल्य           *values,
                    hb_glyph_position_t   &glyph_pos) स्थिर
  {
    bool ret = false;
    अचिन्हित पूर्णांक format = *यह;
    अगर (!format) वापस ret;

    hb_font_t *font = c->font;
    bool horizontal =
#अगर_अघोषित HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#अन्यथा
      true
#पूर्ण_अगर
      ;

    अगर (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    अगर (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    अगर (format & xAdvance) {
      अगर (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    अगर (format & yAdvance) {
      अगर (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    अगर (!has_device ()) वापस ret;

    bool use_x_device = font->x_ppem || font->has_nonzero_coords;
    bool use_y_device = font->y_ppem || font->has_nonzero_coords;

    अगर (!use_x_device && !use_y_device) वापस ret;

    स्थिर ItemVariationStore &store = c->var_store;
    स्वतः *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    अगर (format & xPlaDevice)
    {
      अगर (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    अगर (format & yPlaDevice)
    {
      अगर (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    अगर (format & xAdvDevice)
    {
      अगर (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    अगर (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      अगर (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    वापस ret;
  }

  व्योम position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        स्थिर मूल्य_Base *base,
                        स्थिर मूल्य *values,
                        hb_glyph_position_t &pos) स्थिर
  {
    /* This is ugly... */
    hb_बफर_t बफर {};
    बफर.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &बफर, table_blob);

    apply_मूल्य (&c, base, values, pos);
  }

  अचिन्हित पूर्णांक get_effective_format (स्थिर मूल्य *values, bool strip_hints, bool strip_empty, स्थिर मूल्य_Base *base,
                                     स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिर
  {
    अचिन्हित पूर्णांक format = *यह;
    क्रम (अचिन्हित flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      अगर (format & flag)
      {
        अगर (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          जारी;
        }
        अगर (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          जारी;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        अगर (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    वापस format;
  }

  ढाँचा<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  अचिन्हित पूर्णांक get_effective_format (Iterator it, bool strip_hints, bool strip_empty, स्थिर मूल्य_Base *base,
                                     स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिर {
    अचिन्हित पूर्णांक new_format = 0;

    क्रम (स्थिर hb_array_t<स्थिर मूल्य>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    वापस new_format;
  }

  व्योम copy_मूल्यावली (hb_serialize_context_t *c,
                    अचिन्हित पूर्णांक new_format,
                    स्थिर मूल्य_Base *base,
                    स्थिर मूल्य *values,
                    स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map) स्थिर
  {
    अचिन्हित पूर्णांक format = *यह;
    अगर (!format) वापस;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    अगर ((format | new_format) & xPlacement)
      x_placement = copy_मूल्य (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (मूल्य));
    अगर ((format | new_format) & yPlacement)
      y_placement = copy_मूल्य (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (मूल्य));
    अगर ((format | new_format) & xAdvance)
      x_adv = copy_मूल्य (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (मूल्य));
    अगर ((format | new_format) & yAdvance)
      y_adv = copy_मूल्य (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (मूल्य));

    अगर (!has_device ())
      वापस;

    अगर (format & xPlaDevice)
    {
      add_delta_to_मूल्य (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    अगर (format & yPlaDevice)
    {
      add_delta_to_मूल्य (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    अगर (format & xAdvDevice)
    {
      add_delta_to_मूल्य (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    अगर (format & yAdvDevice)
    {
      add_delta_to_मूल्य (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_मूल्य (hb_serialize_context_t *c,
                       अचिन्हित पूर्णांक new_format,
                       Flags flag,
                       मूल्य value) स्थिर
  {
    // Filter by new format.
    अगर (!(new_format & flag)) वापस nullptr;
    वापस reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  व्योम collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  स्थिर मूल्य_Base *base,
                                  स्थिर hb_array_t<स्थिर मूल्य>& values) स्थिर
  {
    अचिन्हित format = *यह;
    अचिन्हित i = 0;
    अगर (format & xPlacement) i++;
    अगर (format & yPlacement) i++;
    अगर (format & xAdvance) i++;
    अगर (format & yAdvance) i++;
    अगर (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    अगर (format & मूल्य_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    अगर (format & मूल्य_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    अगर (format & मूल्य_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  निजी:
  bool sanitize_मूल्य_devices (hb_sanitize_context_t *c, स्थिर मूल्य_Base *base, स्थिर मूल्य *values) स्थिर
  {
    अचिन्हित पूर्णांक format = *यह;

    अगर (format & xPlacement) values++;
    अगर (format & yPlacement) values++;
    अगर (format & xAdvance)   values++;
    अगर (format & yAdvance)   values++;

    अगर ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) वापस false;
    अगर ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) वापस false;
    अगर ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) वापस false;
    अगर ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) वापस false;

    वापस true;
  }

  अटल अंतरभूत Offset16To<Device, मूल्य_Base>& get_device (मूल्य* value)
  {
    वापस *static_cast<Offset16To<Device, मूल्य_Base> *> (value);
  }
  अटल अंतरभूत स्थिर Offset16To<Device, मूल्य_Base>& get_device (स्थिर मूल्य* value)
  {
    वापस *static_cast<स्थिर Offset16To<Device, मूल्य_Base> *> (value);
  }
  अटल अंतरभूत स्थिर Device& get_device (स्थिर मूल्य* value,
					  bool *worked,
					  स्थिर मूल्य_Base *base,
					  hb_sanitize_context_t &c)
  {
    अगर (worked) *worked |= bool (*value);
    स्वतः &offset = *static_cast<स्थिर Offset16To<Device> *> (value);

    अगर (unlikely (!offset.sanitize (&c, base)))
      वापस Null(Device);
    hb_barrier ();

    वापस base + offset;
  }

  व्योम add_delta_to_मूल्य (HBINT16 *value,
                           स्थिर मूल्य_Base *base,
                           स्थिर मूल्य *src_मूल्य,
                           स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map) स्थिर
  {
    अगर (!value) वापस;
    अचिन्हित varidx = (base + get_device (src_मूल्य)).get_variation_सूचकांक ();
    hb_pair_t<अचिन्हित, पूर्णांक> *varidx_delta;
    अगर (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) वापस;

    *value += hb_second (*varidx_delta);
  }

  bool copy_device (hb_serialize_context_t *c,
                    स्थिर मूल्य_Base *base,
                    स्थिर मूल्य *src_मूल्य,
                    स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *layout_variation_idx_delta_map,
                    अचिन्हित पूर्णांक new_format, Flags flag) स्थिर
  {
    // Filter by new format.
    अगर (!(new_format & flag)) वापस true;

    मूल्य       *dst_मूल्य = c->copy (*src_मूल्य);

    अगर (!dst_मूल्य) वापस false;
    अगर (*dst_मूल्य == 0) वापस true;

    *dst_मूल्य = 0;
    c->push ();
    अगर ((base + get_device (src_मूल्य)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_मूल्य, c->निकालो_pack ());
      वापस true;
    }
    अन्यथा
    {
      c->निकालो_discard ();
      वापस false;
    }
  }

  अटल अंतरभूत स्थिर HBINT16& get_short (स्थिर मूल्य* value, bool *worked=nullptr)
  {
    अगर (worked) *worked |= bool (*value);
    वापस *reinterpret_cast<स्थिर HBINT16 *> (value);
  }

  खुला:

  bool has_device () स्थिर
  {
    अचिन्हित पूर्णांक format = *यह;
    वापस (format & devices) != 0;
  }

  bool sanitize_मूल्य (hb_sanitize_context_t *c, स्थिर मूल्य_Base *base, स्थिर मूल्य *values) स्थिर
  {
    TRACE_SANITIZE (यह);

    अगर (unlikely (!c->check_range (values, get_आकार ()))) return_trace (false);

    अगर (c->lazy_some_gpos)
      return_trace (true);

    return_trace (!has_device () || sanitize_मूल्य_devices (c, base, values));
  }

  bool sanitize_मूल्यावली (hb_sanitize_context_t *c, स्थिर मूल्य_Base *base, स्थिर मूल्य *values, अचिन्हित पूर्णांक गिनती) स्थिर
  {
    TRACE_SANITIZE (यह);
    अचिन्हित आकार = get_आकार ();

    अगर (!c->check_range (values, गिनती, आकार)) return_trace (false);

    अगर (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    return_trace (sanitize_मूल्यावली_stride_unsafe (c, base, values, गिनती, आकार));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  bool sanitize_मूल्यावली_stride_unsafe (hb_sanitize_context_t *c, स्थिर मूल्य_Base *base, स्थिर मूल्य *values, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride) स्थिर
  {
    TRACE_SANITIZE (यह);

    अगर (!has_device ()) return_trace (true);

    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++) {
      अगर (!sanitize_मूल्य_devices (c, base, values))
        return_trace (false);
      values = &StructAtOffset<स्थिर मूल्य> (values, stride);
    }

    return_trace (true);
  }

 निजी:

  व्योम should_drop (मूल्य value, Flags flag, अचिन्हित पूर्णांक* format) स्थिर
  {
    अगर (value) वापस;
    *format = *format & ~flag;
  }

  व्योम update_var_flag (स्थिर मूल्य* value, Flags flag,
                        अचिन्हित पूर्णांक* format, स्थिर मूल्य_Base *base,
                        स्थिर hb_hashmap_t<अचिन्हित, hb_pair_t<अचिन्हित, पूर्णांक>> *varidx_delta_map) स्थिर
  {
    अगर (*value)
    {
      अचिन्हित varidx = (base + get_device (value)).get_variation_सूचकांक ();
      hb_pair_t<अचिन्हित, पूर्णांक> *varidx_delta;
      अगर (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        अगर (varidx_delta->second)
          *format |= flag >> 4;
        अगर (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_सूचकांक)
          वापस;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#पूर्ण_अगर  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

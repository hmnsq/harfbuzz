#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#include "../../../hb-ot-layout-gsubgpos.hh"

namespace OT {
namespace Layout {
namespace GPOS_impl {

typedef HBUINT16 Value;

sa_nracanaa ValueBase {}; // Dummy base class tag for OffsetTo<Value> bases.

typedef UnsizedArrayOf<Value> ValueRecord;

sa_nracanaa ValueFormat : HBUINT16
{
  ga_nanaa Flags {
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
#ᱡᱩᱫᱤ 0
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

  NumType& operator = (uint16_t i) { v = i; ᱨᱩᱣᱟᱹᱲ *this; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned puur_naa_nka get_len () const  { ᱨᱩᱣᱟᱹᱲ hb_popcount8 ((uint8_t) *this); }
  size_t get_size () const { ᱨᱩᱣᱟᱹᱲ get_len () * Value::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () const {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *this;

    ᱡᱩᱫᱤ (format & xPlacement) i++;
    ᱡᱩᱫᱤ (format & yPlacement) i++;
    ᱡᱩᱫᱤ (format & xAdvance)   i++;
    ᱡᱩᱫᱤ (format & yAdvance)   i++;

    ᱡᱩᱫᱤ (format & xPlaDevice) result.push (i++);
    ᱡᱩᱫᱤ (format & yPlaDevice) result.push (i++);
    ᱡᱩᱫᱤ (format & xAdvDevice) result.push (i++);
    ᱡᱩᱫᱤ (format & yAdvDevice) result.push (i++);

    ᱨᱩᱣᱟᱹᱲ result;
  }

  bool apply_value (hb_ot_apply_context_t *c,
                    const ValueBase       *base,
                    const Value           *values,
                    hb_glyph_position_t   &glyph_pos) const
  {
    bool ret = false;
    unsigned puur_naa_nka format = *this;
    ᱡᱩᱫᱤ (!format) ᱨᱩᱣᱟᱹᱲ ret;

    hb_font_t *font = c->font;
    bool horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#ᱵᱟᱝ
      true
#endif
      ;

    ᱡᱩᱫᱤ (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    ᱡᱩᱫᱤ (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    ᱡᱩᱫᱤ (format & xAdvance) {
      ᱡᱩᱫᱤ (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    ᱡᱩᱫᱤ (format & yAdvance) {
      ᱡᱩᱫᱤ (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    ᱡᱩᱫᱤ (!has_device ()) ᱨᱩᱣᱟᱹᱲ ret;

    bool use_x_device = font->x_ppem || font->has_nonzero_coords;
    bool use_y_device = font->y_ppem || font->has_nonzero_coords;

    ᱡᱩᱫᱤ (!use_x_device && !use_y_device) ᱨᱩᱣᱟᱹᱲ ret;

    const ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    ᱡᱩᱫᱤ (format & xPlaDevice)
    {
      ᱡᱩᱫᱤ (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ᱡᱩᱫᱤ (format & yPlaDevice)
    {
      ᱡᱩᱫᱤ (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ᱡᱩᱫᱤ (format & xAdvDevice)
    {
      ᱡᱩᱫᱤ (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    ᱡᱩᱫᱤ (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      ᱡᱩᱫᱤ (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ᱨᱩᱣᱟᱹᱲ ret;
  }

  void position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        const ValueBase *base,
                        const Value *values,
                        hb_glyph_position_t &pos) const
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_value (&c, base, values, pos);
  }

  unsigned puur_naa_nka get_effective_format (const Value *values, bool strip_hints, bool strip_empty, const ValueBase *base,
                                     const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *varidx_delta_map) const
  {
    unsigned puur_naa_nka format = *this;
    for (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      ᱡᱩᱫᱤ (format & flag)
      {
        ᱡᱩᱫᱤ (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          continue;
        }
        ᱡᱩᱫᱤ (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          continue;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        ᱡᱩᱫᱤ (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ᱨᱩᱣᱟᱹᱲ format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned puur_naa_nka get_effective_format (Iterator it, bool strip_hints, bool strip_empty, const ValueBase *base,
                                     const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *varidx_delta_map) const {
    unsigned puur_naa_nka new_format = 0;

    for (const hb_array_t<const Value>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    ᱨᱩᱣᱟᱹᱲ new_format;
  }

  void copy_values (hb_serialize_context_t *c,
                    unsigned puur_naa_nka new_format,
                    const ValueBase *base,
                    const Value *values,
                    const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *layout_variation_idx_delta_map) const
  {
    unsigned puur_naa_nka format = *this;
    ᱡᱩᱫᱤ (!format) ᱨᱩᱣᱟᱹᱲ;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    ᱡᱩᱫᱤ ((format | new_format) & xPlacement)
      x_placement = copy_value (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (Value));
    ᱡᱩᱫᱤ ((format | new_format) & yPlacement)
      y_placement = copy_value (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (Value));
    ᱡᱩᱫᱤ ((format | new_format) & xAdvance)
      x_adv = copy_value (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (Value));
    ᱡᱩᱫᱤ ((format | new_format) & yAdvance)
      y_adv = copy_value (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (Value));

    ᱡᱩᱫᱤ (!has_device ())
      ᱨᱩᱣᱟᱹᱲ;

    ᱡᱩᱫᱤ (format & xPlaDevice)
    {
      add_delta_to_value (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    ᱡᱩᱫᱤ (format & yPlaDevice)
    {
      add_delta_to_value (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    ᱡᱩᱫᱤ (format & xAdvDevice)
    {
      add_delta_to_value (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    ᱡᱩᱫᱤ (format & yAdvDevice)
    {
      add_delta_to_value (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_value (hb_serialize_context_t *c,
                       unsigned puur_naa_nka new_format,
                       Flags flag,
                       Value value) const
  {
    // Filter by new format.
    ᱡᱩᱫᱤ (!(new_format & flag)) ᱨᱩᱣᱟᱹᱲ nullptr;
    ᱨᱩᱣᱟᱹᱲ reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueBase *base,
                                  const hb_array_t<const Value>& values) const
  {
    unsigned format = *this;
    unsigned i = 0;
    ᱡᱩᱫᱤ (format & xPlacement) i++;
    ᱡᱩᱫᱤ (format & yPlacement) i++;
    ᱡᱩᱫᱤ (format & xAdvance) i++;
    ᱡᱩᱫᱤ (format & yAdvance) i++;
    ᱡᱩᱫᱤ (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ᱡᱩᱫᱤ (format & ValueFormat::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ᱡᱩᱫᱤ (format & ValueFormat::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    ᱡᱩᱫᱤ (format & ValueFormat::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  private:
  bool sanitize_value_devices (hb_sanitize_context_t *c, const ValueBase *base, const Value *values) const
  {
    unsigned puur_naa_nka format = *this;

    ᱡᱩᱫᱤ (format & xPlacement) values++;
    ᱡᱩᱫᱤ (format & yPlacement) values++;
    ᱡᱩᱫᱤ (format & xAdvance)   values++;
    ᱡᱩᱫᱤ (format & yAdvance)   values++;

    ᱡᱩᱫᱤ ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) ᱨᱩᱣᱟᱹᱲ false;
    ᱡᱩᱫᱤ ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) ᱨᱩᱣᱟᱹᱲ false;
    ᱡᱩᱫᱤ ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) ᱨᱩᱣᱟᱹᱲ false;
    ᱡᱩᱫᱤ ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) ᱨᱩᱣᱟᱹᱲ false;

    ᱨᱩᱣᱟᱹᱲ true;
  }

  static anta_hstha Offset16To<Device, ValueBase>& get_device (Value* value)
  {
    ᱨᱩᱣᱟᱹᱲ *static_cast<Offset16To<Device, ValueBase> *> (value);
  }
  static anta_hstha const Offset16To<Device, ValueBase>& get_device (const Value* value)
  {
    ᱨᱩᱣᱟᱹᱲ *static_cast<const Offset16To<Device, ValueBase> *> (value);
  }
  static anta_hstha const Device& get_device (const Value* value,
					  bool *worked,
					  const ValueBase *base,
					  hb_sanitize_context_t &c)
  {
    ᱡᱩᱫᱤ (worked) *worked |= bool (*value);
    auto &offset = *static_cast<const Offset16To<Device> *> (value);

    ᱡᱩᱫᱤ (unlikely (!offset.sanitize (&c, base)))
      ᱨᱩᱣᱟᱹᱲ Null(Device);
    hb_barrier ();

    ᱨᱩᱣᱟᱹᱲ base + offset;
  }

  void add_delta_to_value (HBINT16 *value,
                           const ValueBase *base,
                           const Value *src_value,
                           const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *layout_variation_idx_delta_map) const
  {
    ᱡᱩᱫᱤ (!value) ᱨᱩᱣᱟᱹᱲ;
    unsigned varidx = (base + get_device (src_value)).get_variation_index ();
    hb_pair_t<unsigned, puur_naa_nka> *varidx_delta;
    ᱡᱩᱫᱤ (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ᱨᱩᱣᱟᱹᱲ;

    *value += hb_second (*varidx_delta);
  }

  bool copy_device (hb_serialize_context_t *c,
                    const ValueBase *base,
                    const Value *src_value,
                    const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *layout_variation_idx_delta_map,
                    unsigned puur_naa_nka new_format, Flags flag) const
  {
    // Filter by new format.
    ᱡᱩᱫᱤ (!(new_format & flag)) ᱨᱩᱣᱟᱹᱲ true;

    Value       *dst_value = c->copy (*src_value);

    ᱡᱩᱫᱤ (!dst_value) ᱨᱩᱣᱟᱹᱲ false;
    ᱡᱩᱫᱤ (*dst_value == 0) ᱨᱩᱣᱟᱹᱲ true;

    *dst_value = 0;
    c->push ();
    ᱡᱩᱫᱤ ((base + get_device (src_value)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_value, c->pop_pack ());
      ᱨᱩᱣᱟᱹᱲ true;
    }
    ᱵᱟᱝ
    {
      c->pop_discard ();
      ᱨᱩᱣᱟᱹᱲ false;
    }
  }

  static anta_hstha const HBINT16& get_short (const Value* value, bool *worked=nullptr)
  {
    ᱡᱩᱫᱤ (worked) *worked |= bool (*value);
    ᱨᱩᱣᱟᱹᱲ *reinterpret_cast<const HBINT16 *> (value);
  }

  public:

  bool has_device () const
  {
    unsigned puur_naa_nka format = *this;
    ᱨᱩᱣᱟᱹᱲ (format & devices) != 0;
  }

  bool sanitize_value (hb_sanitize_context_t *c, const ValueBase *base, const Value *values) const
  {
    TRACE_SANITIZE (this);

    ᱡᱩᱫᱤ (unlikely (!c->check_range (values, get_size ()))) return_trace (false);

    ᱡᱩᱫᱤ (c->lazy_some_gpos)
      return_trace (true);

    return_trace (!has_device () || sanitize_value_devices (c, base, values));
  }

  bool sanitize_values (hb_sanitize_context_t *c, const ValueBase *base, const Value *values, unsigned puur_naa_nka count) const
  {
    TRACE_SANITIZE (this);
    unsigned size = get_size ();

    ᱡᱩᱫᱤ (!c->check_range (values, count, size)) return_trace (false);

    ᱡᱩᱫᱤ (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    return_trace (sanitize_values_stride_unsafe (c, base, values, count, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  bool sanitize_values_stride_unsafe (hb_sanitize_context_t *c, const ValueBase *base, const Value *values, unsigned puur_naa_nka count, unsigned puur_naa_nka stride) const
  {
    TRACE_SANITIZE (this);

    ᱡᱩᱫᱤ (!has_device ()) return_trace (true);

    for (unsigned puur_naa_nka i = 0; i < count; i++) {
      ᱡᱩᱫᱤ (!sanitize_value_devices (c, base, values))
        return_trace (false);
      values = &StructAtOffset<const Value> (values, stride);
    }

    return_trace (true);
  }

 private:

  void should_drop (Value value, Flags flag, unsigned puur_naa_nka* format) const
  {
    ᱡᱩᱫᱤ (value) ᱨᱩᱣᱟᱹᱲ;
    *format = *format & ~flag;
  }

  void update_var_flag (const Value* value, Flags flag,
                        unsigned puur_naa_nka* format, const ValueBase *base,
                        const hb_hashmap_t<unsigned, hb_pair_t<unsigned, puur_naa_nka>> *varidx_delta_map) const
  {
    ᱡᱩᱫᱤ (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, puur_naa_nka> *varidx_delta;
      ᱡᱩᱫᱤ (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        ᱡᱩᱫᱤ (varidx_delta->second)
          *format |= flag >> 4;
        ᱡᱩᱫᱤ (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ᱨᱩᱣᱟᱹᱲ;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

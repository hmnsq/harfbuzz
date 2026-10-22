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
#གལ་སྲིད 0
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

  NumType& operator = (uint16_t i) { v = i; ལོག *this; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned ཧྲིལ get_len () const  { ལོག hb_popcount8 ((uint8_t) *this); }
  size_t get_size () const { ལོག get_len () * Value::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () const {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *this;

    གལ་སྲིད (format & xPlacement) i++;
    གལ་སྲིད (format & yPlacement) i++;
    གལ་སྲིད (format & xAdvance)   i++;
    གལ་སྲིད (format & yAdvance)   i++;

    གལ་སྲིད (format & xPlaDevice) result.push (i++);
    གལ་སྲིད (format & yPlaDevice) result.push (i++);
    གལ་སྲིད (format & xAdvDevice) result.push (i++);
    གལ་སྲིད (format & yAdvDevice) result.push (i++);

    ལོག result;
  }

  bool apply_value (hb_ot_apply_context_t *c,
                    const ValueBase       *base,
                    const Value           *values,
                    hb_glyph_position_t   &glyph_pos) const
  {
    bool ret = false;
    unsigned ཧྲིལ format = *this;
    གལ་སྲིད (!format) ལོག ret;

    hb_font_t *font = c->font;
    bool horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#དེ་མིན
      true
#endif
      ;

    གལ་སྲིད (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    གལ་སྲིད (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    གལ་སྲིད (format & xAdvance) {
      གལ་སྲིད (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    གལ་སྲིད (format & yAdvance) {
      གལ་སྲིད (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    གལ་སྲིད (!has_device ()) ལོག ret;

    bool use_x_device = font->x_ppem || font->has_nonzero_coords;
    bool use_y_device = font->y_ppem || font->has_nonzero_coords;

    གལ་སྲིད (!use_x_device && !use_y_device) ལོག ret;

    const ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    གལ་སྲིད (format & xPlaDevice)
    {
      གལ་སྲིད (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    གལ་སྲིད (format & yPlaDevice)
    {
      གལ་སྲིད (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    གལ་སྲིད (format & xAdvDevice)
    {
      གལ་སྲིད (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    གལ་སྲིད (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      གལ་སྲིད (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ལོག ret;
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

  unsigned ཧྲིལ get_effective_format (const Value *values, bool strip_hints, bool strip_empty, const ValueBase *base,
                                     const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *varidx_delta_map) const
  {
    unsigned ཧྲིལ format = *this;
    དོན་ལུ (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      གལ་སྲིད (format & flag)
      {
        གལ་སྲིད (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          continue;
        }
        གལ་སྲིད (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          continue;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        གལ་སྲིད (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ལོག format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned ཧྲིལ get_effective_format (Iterator it, bool strip_hints, bool strip_empty, const ValueBase *base,
                                     const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *varidx_delta_map) const {
    unsigned ཧྲིལ new_format = 0;

    དོན་ལུ (const hb_array_t<const Value>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    ལོག new_format;
  }

  void copy_values (hb_serialize_context_t *c,
                    unsigned ཧྲིལ new_format,
                    const ValueBase *base,
                    const Value *values,
                    const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *layout_variation_idx_delta_map) const
  {
    unsigned ཧྲིལ format = *this;
    གལ་སྲིད (!format) ལོག;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    གལ་སྲིད ((format | new_format) & xPlacement)
      x_placement = copy_value (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (Value));
    གལ་སྲིད ((format | new_format) & yPlacement)
      y_placement = copy_value (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (Value));
    གལ་སྲིད ((format | new_format) & xAdvance)
      x_adv = copy_value (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (Value));
    གལ་སྲིད ((format | new_format) & yAdvance)
      y_adv = copy_value (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (Value));

    གལ་སྲིད (!has_device ())
      ལོག;

    གལ་སྲིད (format & xPlaDevice)
    {
      add_delta_to_value (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    གལ་སྲིད (format & yPlaDevice)
    {
      add_delta_to_value (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    གལ་སྲིད (format & xAdvDevice)
    {
      add_delta_to_value (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    གལ་སྲིད (format & yAdvDevice)
    {
      add_delta_to_value (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_value (hb_serialize_context_t *c,
                       unsigned ཧྲིལ new_format,
                       Flags flag,
                       Value value) const
  {
    // Filter by new format.
    གལ་སྲིད (!(new_format & flag)) ལོག nullptr;
    ལོག reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  void collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  const ValueBase *base,
                                  const hb_array_t<const Value>& values) const
  {
    unsigned format = *this;
    unsigned i = 0;
    གལ་སྲིད (format & xPlacement) i++;
    གལ་སྲིད (format & yPlacement) i++;
    གལ་སྲིད (format & xAdvance) i++;
    གལ་སྲིད (format & yAdvance) i++;
    གལ་སྲིད (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    གལ་སྲིད (format & ValueFormat::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    གལ་སྲིད (format & ValueFormat::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    གལ་སྲིད (format & ValueFormat::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  private:
  bool sanitize_value_devices (hb_sanitize_context_t *c, const ValueBase *base, const Value *values) const
  {
    unsigned ཧྲིལ format = *this;

    གལ་སྲིད (format & xPlacement) values++;
    གལ་སྲིད (format & yPlacement) values++;
    གལ་སྲིད (format & xAdvance)   values++;
    གལ་སྲིད (format & yAdvance)   values++;

    གལ་སྲིད ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) ལོག false;
    གལ་སྲིད ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) ལོག false;
    གལ་སྲིད ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) ལོག false;
    གལ་སྲིད ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) ལོག false;

    ལོག true;
  }

  static anta_hstha Offset16To<Device, ValueBase>& get_device (Value* value)
  {
    ལོག *static_cast<Offset16To<Device, ValueBase> *> (value);
  }
  static anta_hstha const Offset16To<Device, ValueBase>& get_device (const Value* value)
  {
    ལོག *static_cast<const Offset16To<Device, ValueBase> *> (value);
  }
  static anta_hstha const Device& get_device (const Value* value,
					  bool *worked,
					  const ValueBase *base,
					  hb_sanitize_context_t &c)
  {
    གལ་སྲིད (worked) *worked |= bool (*value);
    auto &offset = *static_cast<const Offset16To<Device> *> (value);

    གལ་སྲིད (unlikely (!offset.sanitize (&c, base)))
      ལོག Null(Device);
    hb_barrier ();

    ལོག base + offset;
  }

  void add_delta_to_value (HBINT16 *value,
                           const ValueBase *base,
                           const Value *src_value,
                           const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *layout_variation_idx_delta_map) const
  {
    གལ་སྲིད (!value) ལོག;
    unsigned varidx = (base + get_device (src_value)).get_variation_index ();
    hb_pair_t<unsigned, ཧྲིལ> *varidx_delta;
    གལ་སྲིད (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ལོག;

    *value += hb_second (*varidx_delta);
  }

  bool copy_device (hb_serialize_context_t *c,
                    const ValueBase *base,
                    const Value *src_value,
                    const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *layout_variation_idx_delta_map,
                    unsigned ཧྲིལ new_format, Flags flag) const
  {
    // Filter by new format.
    གལ་སྲིད (!(new_format & flag)) ལོག true;

    Value       *dst_value = c->copy (*src_value);

    གལ་སྲིད (!dst_value) ལོག false;
    གལ་སྲིད (*dst_value == 0) ལོག true;

    *dst_value = 0;
    c->push ();
    གལ་སྲིད ((base + get_device (src_value)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_value, c->pop_pack ());
      ལོག true;
    }
    དེ་མིན
    {
      c->pop_discard ();
      ལོག false;
    }
  }

  static anta_hstha const HBINT16& get_short (const Value* value, bool *worked=nullptr)
  {
    གལ་སྲིད (worked) *worked |= bool (*value);
    ལོག *reinterpret_cast<const HBINT16 *> (value);
  }

  public:

  bool has_device () const
  {
    unsigned ཧྲིལ format = *this;
    ལོག (format & devices) != 0;
  }

  bool sanitize_value (hb_sanitize_context_t *c, const ValueBase *base, const Value *values) const
  {
    TRACE_SANITIZE (this);

    གལ་སྲིད (unlikely (!c->check_range (values, get_size ()))) return_trace (false);

    གལ་སྲིད (c->lazy_some_gpos)
      return_trace (true);

    return_trace (!has_device () || sanitize_value_devices (c, base, values));
  }

  bool sanitize_values (hb_sanitize_context_t *c, const ValueBase *base, const Value *values, unsigned ཧྲིལ count) const
  {
    TRACE_SANITIZE (this);
    unsigned size = get_size ();

    གལ་སྲིད (!c->check_range (values, count, size)) return_trace (false);

    གལ་སྲིད (c->lazy_some_gpos)
      return_trace (true);

    hb_barrier ();
    return_trace (sanitize_values_stride_unsafe (c, base, values, count, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  bool sanitize_values_stride_unsafe (hb_sanitize_context_t *c, const ValueBase *base, const Value *values, unsigned ཧྲིལ count, unsigned ཧྲིལ stride) const
  {
    TRACE_SANITIZE (this);

    གལ་སྲིད (!has_device ()) return_trace (true);

    དོན་ལུ (unsigned ཧྲིལ i = 0; i < count; i++) {
      གལ་སྲིད (!sanitize_value_devices (c, base, values))
        return_trace (false);
      values = &StructAtOffset<const Value> (values, stride);
    }

    return_trace (true);
  }

 private:

  void should_drop (Value value, Flags flag, unsigned ཧྲིལ* format) const
  {
    གལ་སྲིད (value) ལོག;
    *format = *format & ~flag;
  }

  void update_var_flag (const Value* value, Flags flag,
                        unsigned ཧྲིལ* format, const ValueBase *base,
                        const hb_hashmap_t<unsigned, hb_pair_t<unsigned, ཧྲིལ>> *varidx_delta_map) const
  {
    གལ་སྲིད (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, ཧྲིལ> *varidx_delta;
      གལ་སྲིད (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        གལ་སྲིད (varidx_delta->second)
          *format |= flag >> 4;
        གལ་སྲིད (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ལོག;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

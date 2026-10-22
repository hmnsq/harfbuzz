#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#অন্তর্ভাৱ "../../../hb-ot-layout-gsubgpos.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

প্রকারনির্দেশ HBUINT16 মান;

সংরচনা মান_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

প্রকারনির্দেশ UnsizedArrayOf<মান> মান_Record;

সংরচনা মান_Format : HBUINT16
{
  গণনা Flags {
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
#যদি 0
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

  অঙ্ক_Type& operator = (uint16_t i) { v = i; ঘূৰাই *এতত্; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned পূর্ণাঙ্ক get_len () ধ্রুৱ  { ঘূৰাই hb_popcount8 ((uint8_t) *এতত্); }
  size_t get_size () ধ্রুৱ { ঘূৰাই get_len () * মান::static_size; }

  hb_vector_t<unsigned> get_device_table_indices () ধ্রুৱ {
    unsigned i = 0;
    hb_vector_t<unsigned> result;
    unsigned format = *এতত্;

    যদি (format & xPlacement) i++;
    যদি (format & yPlacement) i++;
    যদি (format & xAdvance)   i++;
    যদি (format & yAdvance)   i++;

    যদি (format & xPlaDevice) result.push (i++);
    যদি (format & yPlaDevice) result.push (i++);
    যদি (format & xAdvDevice) result.push (i++);
    যদি (format & yAdvDevice) result.push (i++);

    ঘূৰাই result;
  }

  তার্কিক apply_মান (hb_ot_apply_context_t *c,
                    ধ্রুৱ মান_Base       *base,
                    ধ্রুৱ মান           *values,
                    hb_glyph_position_t   &glyph_pos) ধ্রুৱ
  {
    তার্কিক ret = মিছা;
    unsigned পূর্ণাঙ্ক format = *এতত্;
    যদি (!format) ঘূৰাই ret;

    hb_font_t *font = c->font;
    তার্কিক horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#অন্যথা
      সঁচা
#endif
      ;

    যদি (format & xPlacement)
      glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, font->em_scale_x (get_short (values++, &ret)));
    যদি (format & yPlacement)
      glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, font->em_scale_y (get_short (values++, &ret)));
    যদি (format & xAdvance) {
      যদি (likely (horizontal))
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, font->em_scale_x (get_short (values, &ret)));
      values++;
    }
    /* y_advance values grow downward but font-space grows upward, hence negation */
    যদি (format & yAdvance) {
      যদি (unlikely (!horizontal))
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, font->em_scale_y (get_short (values, &ret)));
      values++;
    }

    যদি (!has_device ()) ঘূৰাই ret;

    তার্কিক use_x_device = font->x_ppem || font->has_nonzero_coords;
    তার্কিক use_y_device = font->y_ppem || font->has_nonzero_coords;

    যদি (!use_x_device && !use_y_device) ঘূৰাই ret;

    ধ্রুৱ ItemVariationStore &store = c->var_store;
    auto *cache = c->var_store_cache;

    /* pixel -> fractional pixel */
    যদি (format & xPlaDevice)
    {
      যদি (use_x_device)
	glyph_pos.x_offset = hb_saturate_add (glyph_pos.x_offset, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    যদি (format & yPlaDevice)
    {
      যদি (use_y_device)
	glyph_pos.y_offset = hb_saturate_add (glyph_pos.y_offset, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    যদি (format & xAdvDevice)
    {
      যদি (horizontal && use_x_device)
	glyph_pos.x_advance = hb_saturate_add (glyph_pos.x_advance, get_device (values, &ret, base, c->sanitizer).get_x_delta (font, store, cache));
      values++;
    }
    যদি (format & yAdvDevice)
    {
      /* y_advance values grow downward but font-space grows upward, hence negation */
      যদি (!horizontal && use_y_device)
	glyph_pos.y_advance = hb_saturate_sub (glyph_pos.y_advance, get_device (values, &ret, base, c->sanitizer).get_y_delta (font, store, cache));
      values++;
    }
    ঘূৰাই ret;
  }

  শূন্য position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ধ্রুৱ মান_Base *base,
                        ধ্রুৱ মান *values,
                        hb_glyph_position_t &pos) ধ্রুৱ
  {
    /* This is ugly... */
    hb_buffer_t buffer {};
    buffer.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &buffer, table_blob);

    apply_মান (&c, base, values, pos);
  }

  unsigned পূর্ণাঙ্ক get_effective_format (ধ্রুৱ মান *values, তার্কিক strip_hints, তার্কিক strip_empty, ধ্রুৱ মান_Base *base,
                                     ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক format = *এতত্;
    বাবে (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      যদি (format & flag)
      {
        যদি (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          অনুৱর্ত;
        }
        যদি (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          অনুৱর্ত;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        যদি (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ঘূৰাই format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned পূর্ণাঙ্ক get_effective_format (Iterator it, তার্কিক strip_hints, তার্কিক strip_empty, ধ্রুৱ মান_Base *base,
                                     ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map) ধ্রুৱ {
    unsigned পূর্ণাঙ্ক new_format = 0;

    বাবে (ধ্রুৱ hb_array_t<ধ্রুৱ মান>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    ঘূৰাই new_format;
  }

  শূন্য copy_মানসমূহ (hb_serialize_context_t *c,
                    unsigned পূর্ণাঙ্ক new_format,
                    ধ্রুৱ মান_Base *base,
                    ধ্রুৱ মান *values,
                    ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক format = *এতত্;
    যদি (!format) ঘূৰাই;

    HBINT16 *x_placement = nullptr, *y_placement = nullptr, *x_adv = nullptr, *y_adv = nullptr;
    যদি ((format | new_format) & xPlacement)
      x_placement = copy_মান (c, new_format, xPlacement, format & xPlacement ? *values++ : Null (মান));
    যদি ((format | new_format) & yPlacement)
      y_placement = copy_মান (c, new_format, yPlacement, format & yPlacement ? *values++ : Null (মান));
    যদি ((format | new_format) & xAdvance)
      x_adv = copy_মান (c, new_format, xAdvance, format & xAdvance ? *values++ : Null (মান));
    যদি ((format | new_format) & yAdvance)
      y_adv = copy_মান (c, new_format, yAdvance, format & yAdvance ? *values++ : Null (মান));

    যদি (!has_device ())
      ঘূৰাই;

    যদি (format & xPlaDevice)
    {
      add_delta_to_মান (x_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xPlaDevice);
    }

    যদি (format & yPlaDevice)
    {
      add_delta_to_মান (y_placement, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yPlaDevice);
    }

    যদি (format & xAdvDevice)
    {
      add_delta_to_মান (x_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, xAdvDevice);
    }

    যদি (format & yAdvDevice)
    {
      add_delta_to_মান (y_adv, base, values, layout_variation_idx_delta_map);
      copy_device (c, base, values++, layout_variation_idx_delta_map, new_format, yAdvDevice);
    }
  }

  HBINT16* copy_মান (hb_serialize_context_t *c,
                       unsigned পূর্ণাঙ্ক new_format,
                       Flags flag,
                       মান value) ধ্রুৱ
  {
    // Filter by new format.
    যদি (!(new_format & flag)) ঘূৰাই nullptr;
    ঘূৰাই reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুৱ মান_Base *base,
                                  ধ্রুৱ hb_array_t<ধ্রুৱ মান>& values) ধ্রুৱ
  {
    unsigned format = *এতত্;
    unsigned i = 0;
    যদি (format & xPlacement) i++;
    যদি (format & yPlacement) i++;
    যদি (format & xAdvance) i++;
    যদি (format & yAdvance) i++;
    যদি (format & xPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    যদি (format & মান_Format::yPlaDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    যদি (format & মান_Format::xAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }

    যদি (format & মান_Format::yAdvDevice)
    {
      (base + get_device (&(values[i]))).collect_variation_indices (c);
      i++;
    }
  }

  গুপ্ত:
  তার্কিক sanitize_মান_devices (hb_sanitize_context_t *c, ধ্রুৱ মান_Base *base, ধ্রুৱ মান *values) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক format = *এতত্;

    যদি (format & xPlacement) values++;
    যদি (format & yPlacement) values++;
    যদি (format & xAdvance)   values++;
    যদি (format & yAdvance)   values++;

    যদি ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) ঘূৰাই মিছা;
    যদি ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) ঘূৰাই মিছা;
    যদি ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) ঘূৰাই মিছা;
    যদি ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) ঘূৰাই মিছা;

    ঘূৰাই সঁচা;
  }

  স্থির অন্তঃস্থ Offset16To<Device, মান_Base>& get_device (মান* value)
  {
    ঘূৰাই *static_cast<Offset16To<Device, মান_Base> *> (value);
  }
  স্থির অন্তঃস্থ ধ্রুৱ Offset16To<Device, মান_Base>& get_device (ধ্রুৱ মান* value)
  {
    ঘূৰাই *static_cast<ধ্রুৱ Offset16To<Device, মান_Base> *> (value);
  }
  স্থির অন্তঃস্থ ধ্রুৱ Device& get_device (ধ্রুৱ মান* value,
					  তার্কিক *worked,
					  ধ্রুৱ মান_Base *base,
					  hb_sanitize_context_t &c)
  {
    যদি (worked) *worked |= তার্কিক (*value);
    auto &offset = *static_cast<ধ্রুৱ Offset16To<Device> *> (value);

    যদি (unlikely (!offset.sanitize (&c, base)))
      ঘূৰাই Null(Device);
    hb_barrier ();

    ঘূৰাই base + offset;
  }

  শূন্য add_delta_to_মান (HBINT16 *value,
                           ধ্রুৱ মান_Base *base,
                           ধ্রুৱ মান *src_মান,
                           ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map) ধ্রুৱ
  {
    যদি (!value) ঘূৰাই;
    unsigned varidx = (base + get_device (src_মান)).get_variation_index ();
    hb_pair_t<unsigned, পূর্ণাঙ্ক> *varidx_delta;
    যদি (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ঘূৰাই;

    *value += hb_second (*varidx_delta);
  }

  তার্কিক copy_device (hb_serialize_context_t *c,
                    ধ্রুৱ মান_Base *base,
                    ধ্রুৱ মান *src_মান,
                    ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *layout_variation_idx_delta_map,
                    unsigned পূর্ণাঙ্ক new_format, Flags flag) ধ্রুৱ
  {
    // Filter by new format.
    যদি (!(new_format & flag)) ঘূৰাই সঁচা;

    মান       *dst_মান = c->copy (*src_মান);

    যদি (!dst_মান) ঘূৰাই মিছা;
    যদি (*dst_মান == 0) ঘূৰাই সঁচা;

    *dst_মান = 0;
    c->push ();
    যদি ((base + get_device (src_মান)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_মান, c->বেৰ_কৰো_pack ());
      ঘূৰাই সঁচা;
    }
    অন্যথা
    {
      c->বেৰ_কৰো_discard ();
      ঘূৰাই মিছা;
    }
  }

  স্থির অন্তঃস্থ ধ্রুৱ HBINT16& get_short (ধ্রুৱ মান* value, তার্কিক *worked=nullptr)
  {
    যদি (worked) *worked |= তার্কিক (*value);
    ঘূৰাই *reinterpret_cast<ধ্রুৱ HBINT16 *> (value);
  }

  সার্ৱজনিক:

  তার্কিক has_device () ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক format = *এতত্;
    ঘূৰাই (format & devices) != 0;
  }

  তার্কিক sanitize_মান (hb_sanitize_context_t *c, ধ্রুৱ মান_Base *base, ধ্রুৱ মান *values) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);

    যদি (unlikely (!c->check_range (values, get_size ()))) return_trace (মিছা);

    যদি (c->lazy_some_gpos)
      return_trace (সঁচা);

    return_trace (!has_device () || sanitize_মান_devices (c, base, values));
  }

  তার্কিক sanitize_মানসমূহ (hb_sanitize_context_t *c, ধ্রুৱ মান_Base *base, ধ্রুৱ মান *values, unsigned পূর্ণাঙ্ক গুনতি) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);
    unsigned size = get_size ();

    যদি (!c->check_range (values, গুনতি, size)) return_trace (মিছা);

    যদি (c->lazy_some_gpos)
      return_trace (সঁচা);

    hb_barrier ();
    return_trace (sanitize_মানসমূহ_stride_unsafe (c, base, values, গুনতি, size));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  তার্কিক sanitize_মানসমূহ_stride_unsafe (hb_sanitize_context_t *c, ধ্রুৱ মান_Base *base, ধ্রুৱ মান *values, unsigned পূর্ণাঙ্ক গুনতি, unsigned পূর্ণাঙ্ক stride) ধ্রুৱ
  {
    TRACE_SANITIZE (এতত্);

    যদি (!has_device ()) return_trace (সঁচা);

    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি; i++) {
      যদি (!sanitize_মান_devices (c, base, values))
        return_trace (মিছা);
      values = &StructAtOffset<ধ্রুৱ মান> (values, stride);
    }

    return_trace (সঁচা);
  }

 গুপ্ত:

  শূন্য should_drop (মান value, Flags flag, unsigned পূর্ণাঙ্ক* format) ধ্রুৱ
  {
    যদি (value) ঘূৰাই;
    *format = *format & ~flag;
  }

  শূন্য update_var_flag (ধ্রুৱ মান* value, Flags flag,
                        unsigned পূর্ণাঙ্ক* format, ধ্রুৱ মান_Base *base,
                        ধ্রুৱ hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণাঙ্ক>> *varidx_delta_map) ধ্রুৱ
  {
    যদি (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_index ();
      hb_pair_t<unsigned, পূর্ণাঙ্ক> *varidx_delta;
      যদি (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        যদি (varidx_delta->second)
          *format |= flag >> 4;
        যদি (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_INDEX)
          ঘূৰাই;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

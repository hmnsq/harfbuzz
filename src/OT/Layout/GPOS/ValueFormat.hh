#ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH
#define OT_LAYOUT_GPOS_VALUEFORMAT_HH

#অন্তর্ভাব "../../../hb-ot-layout-gsubgpos.hh"

নামাকাশ OT {
নামাকাশ Layout {
নামাকাশ GPOS_impl {

প্রকারলেখ HBUINT16 মান;

কাঠামো মান_Base {}; // Dummy base class tag for OffsetTo<Value> bases.

প্রকারলেখ UnsizedArrayOf<মান> মান_Record;

কাঠামো মান_Format : HBUINT16
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

  অঙ্ক_Type& operator = (uint16_t i) { v = i; ফেরত *এই; }

  // Note: spec says skip 2 bytes per bit in the valueformat. But reports
  // from Microsoft developers indicate that only the fields that are
  // currently defined are counted. We don't expect any new fields to
  // be added to ValueFormat. As such, we use the faster hb_popcount8
  // that only processes the lowest 8 bits.
  unsigned পূর্ণসংখ্যা get_len () ধ্রুবক  { ফেরত hb_popcount8 ((uint8_t) *এই); }
  size_t get_আকার () ধ্রুবক { ফেরত get_len () * মান::static_আকার; }

  hb_vector_t<unsigned> get_device_table_indices () ধ্রুবক {
    unsigned i = 0;
    hb_vector_t<unsigned> ফলাফল;
    unsigned format = *এই;

    যদি (format & xPlacement) i++;
    যদি (format & yPlacement) i++;
    যদি (format & xAdvance)   i++;
    যদি (format & yAdvance)   i++;

    যদি (format & xPlaDevice) ফলাফল.push (i++);
    যদি (format & yPlaDevice) ফলাফল.push (i++);
    যদি (format & xAdvDevice) ফলাফল.push (i++);
    যদি (format & yAdvDevice) ফলাফল.push (i++);

    ফেরত ফলাফল;
  }

  যৌক্তিক apply_মান (hb_ot_apply_context_t *c,
                    ধ্রুবক মান_Base       *base,
                    ধ্রুবক মান           *values,
                    hb_glyph_position_t   &glyph_pos) ধ্রুবক
  {
    যৌক্তিক ret = মিথ্যা;
    unsigned পূর্ণসংখ্যা format = *এই;
    যদি (!format) ফেরত ret;

    hb_font_t *font = c->font;
    যৌক্তিক horizontal =
#ifndef HB_NO_VERTICAL
      HB_DIRECTION_IS_HORIZONTAL (c->direction)
#অন্যথা
      সত্য
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

    যদি (!has_device ()) ফেরত ret;

    যৌক্তিক use_x_device = font->x_ppem || font->has_nonzero_coords;
    যৌক্তিক use_y_device = font->y_ppem || font->has_nonzero_coords;

    যদি (!use_x_device && !use_y_device) ফেরত ret;

    ধ্রুবক ItemVariationStore &store = c->var_store;
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
    ফেরত ret;
  }

  শূন্য position_single (hb_font_t *font,
                        hb_blob_t *table_blob,
                        hb_direction_t direction,
                        ধ্রুবক মান_Base *base,
                        ধ্রুবক মান *values,
                        hb_glyph_position_t &pos) ধ্রুবক
  {
    /* This is ugly... */
    hb_বাফার_t বাফার {};
    বাফার.props.direction = direction;
    hb_ot_apply_context_t c (1, font, &বাফার, table_blob);

    apply_মান (&c, base, values, pos);
  }

  unsigned পূর্ণসংখ্যা get_effective_format (ধ্রুবক মান *values, যৌক্তিক strip_hints, যৌক্তিক strip_empty, ধ্রুবক মান_Base *base,
                                     ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা format = *এই;
    জন্য (unsigned flag = xPlacement; flag <= yAdvDevice; flag = flag << 1) {
      যদি (format & flag)
      {
        যদি (strip_hints && flag >= xPlaDevice)
        {
          format = format & ~flag;
          values++;
          চালু;
        }
        যদি (varidx_delta_map && flag >= xPlaDevice)
        {
          update_var_flag (values++, (Flags) flag, &format, base, varidx_delta_map);
          চালু;
        }
        /* do not strip empty when instancing, cause we don't know whether the new
         * default value is 0 or not */
        যদি (strip_empty) should_drop (*values, (Flags) flag, &format);
        values++;
      }
    }

    ফেরত format;
  }

  template<typename Iterator,
      hb_requires (hb_is_iterator (Iterator))>
  unsigned পূর্ণসংখ্যা get_effective_format (Iterator it, যৌক্তিক strip_hints, যৌক্তিক strip_empty, ধ্রুবক মান_Base *base,
                                     ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map) ধ্রুবক {
    unsigned পূর্ণসংখ্যা new_format = 0;

    জন্য (ধ্রুবক hb_array_t<ধ্রুবক মান>& values : it)
      new_format = new_format | get_effective_format (&values, strip_hints, strip_empty, base, varidx_delta_map);

    ফেরত new_format;
  }

  শূন্য copy_মানসমূহ (hb_serialize_context_t *c,
                    unsigned পূর্ণসংখ্যা new_format,
                    ধ্রুবক মান_Base *base,
                    ধ্রুবক মান *values,
                    ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা format = *এই;
    যদি (!format) ফেরত;

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
      ফেরত;

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
                       unsigned পূর্ণসংখ্যা new_format,
                       Flags flag,
                       মান value) ধ্রুবক
  {
    // Filter by new format.
    যদি (!(new_format & flag)) ফেরত nullptr;
    ফেরত reinterpret_cast<HBINT16 *> (c->copy (value));
  }

  শূন্য collect_variation_indices (hb_collect_variation_indices_context_t *c,
                                  ধ্রুবক মান_Base *base,
                                  ধ্রুবক hb_array_t<ধ্রুবক মান>& values) ধ্রুবক
  {
    unsigned format = *এই;
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

  ব্যক্তিগত:
  যৌক্তিক sanitize_মান_devices (hb_sanitize_context_t *c, ধ্রুবক মান_Base *base, ধ্রুবক মান *values) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা format = *এই;

    যদি (format & xPlacement) values++;
    যদি (format & yPlacement) values++;
    যদি (format & xAdvance)   values++;
    যদি (format & yAdvance)   values++;

    যদি ((format & xPlaDevice) && !get_device (values++).sanitize (c, base)) ফেরত মিথ্যা;
    যদি ((format & yPlaDevice) && !get_device (values++).sanitize (c, base)) ফেরত মিথ্যা;
    যদি ((format & xAdvDevice) && !get_device (values++).sanitize (c, base)) ফেরত মিথ্যা;
    যদি ((format & yAdvDevice) && !get_device (values++).sanitize (c, base)) ফেরত মিথ্যা;

    ফেরত সত্য;
  }

  স্থির অন্তঃরেখ Offset16To<Device, মান_Base>& get_device (মান* value)
  {
    ফেরত *static_cast<Offset16To<Device, মান_Base> *> (value);
  }
  স্থির অন্তঃরেখ ধ্রুবক Offset16To<Device, মান_Base>& get_device (ধ্রুবক মান* value)
  {
    ফেরত *static_cast<ধ্রুবক Offset16To<Device, মান_Base> *> (value);
  }
  স্থির অন্তঃরেখ ধ্রুবক Device& get_device (ধ্রুবক মান* value,
					  যৌক্তিক *worked,
					  ধ্রুবক মান_Base *base,
					  hb_sanitize_context_t &c)
  {
    যদি (worked) *worked |= যৌক্তিক (*value);
    auto &offset = *static_cast<ধ্রুবক Offset16To<Device> *> (value);

    যদি (unlikely (!offset.sanitize (&c, base)))
      ফেরত Null(Device);
    hb_barrier ();

    ফেরত base + offset;
  }

  শূন্য add_delta_to_মান (HBINT16 *value,
                           ধ্রুবক মান_Base *base,
                           ধ্রুবক মান *src_মান,
                           ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map) ধ্রুবক
  {
    যদি (!value) ফেরত;
    unsigned varidx = (base + get_device (src_মান)).get_variation_সূচক ();
    hb_pair_t<unsigned, পূর্ণসংখ্যা> *varidx_delta;
    যদি (!layout_variation_idx_delta_map->has (varidx, &varidx_delta)) ফেরত;

    *value += hb_second (*varidx_delta);
  }

  যৌক্তিক copy_device (hb_serialize_context_t *c,
                    ধ্রুবক মান_Base *base,
                    ধ্রুবক মান *src_মান,
                    ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *layout_variation_idx_delta_map,
                    unsigned পূর্ণসংখ্যা new_format, Flags flag) ধ্রুবক
  {
    // Filter by new format.
    যদি (!(new_format & flag)) ফেরত সত্য;

    মান       *dst_মান = c->copy (*src_মান);

    যদি (!dst_মান) ফেরত মিথ্যা;
    যদি (*dst_মান == 0) ফেরত সত্য;

    *dst_মান = 0;
    c->push ();
    যদি ((base + get_device (src_মান)).copy (c, layout_variation_idx_delta_map))
    {
      c->add_link (*dst_মান, c->বের_করো_pack ());
      ফেরত সত্য;
    }
    অন্যথা
    {
      c->বের_করো_discard ();
      ফেরত মিথ্যা;
    }
  }

  স্থির অন্তঃরেখ ধ্রুবক HBINT16& get_short (ধ্রুবক মান* value, যৌক্তিক *worked=nullptr)
  {
    যদি (worked) *worked |= যৌক্তিক (*value);
    ফেরত *reinterpret_cast<ধ্রুবক HBINT16 *> (value);
  }

  সর্বজনীন:

  যৌক্তিক has_device () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা format = *এই;
    ফেরত (format & devices) != 0;
  }

  যৌক্তিক sanitize_মান (hb_sanitize_context_t *c, ধ্রুবক মান_Base *base, ধ্রুবক মান *values) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    যদি (unlikely (!c->check_range (values, get_আকার ()))) return_trace (মিথ্যা);

    যদি (c->lazy_some_gpos)
      return_trace (সত্য);

    return_trace (!has_device () || sanitize_মান_devices (c, base, values));
  }

  যৌক্তিক sanitize_মানসমূহ (hb_sanitize_context_t *c, ধ্রুবক মান_Base *base, ধ্রুবক মান *values, unsigned পূর্ণসংখ্যা গুনতি) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    unsigned আকার_2 = get_আকার ();

    যদি (!c->check_range (values, গুনতি, আকার_2)) return_trace (মিথ্যা);

    যদি (c->lazy_some_gpos)
      return_trace (সত্য);

    hb_barrier ();
    return_trace (sanitize_মানসমূহ_stride_unsafe (c, base, values, গুনতি, আকার_2));
  }

  /* Just sanitize referenced Device tables.  Doesn't check the values themselves. */
  যৌক্তিক sanitize_মানসমূহ_stride_unsafe (hb_sanitize_context_t *c, ধ্রুবক মান_Base *base, ধ্রুবক মান *values, unsigned পূর্ণসংখ্যা গুনতি, unsigned পূর্ণসংখ্যা stride) ধ্রুবক
  {
    TRACE_SANITIZE (এই);

    যদি (!has_device ()) return_trace (সত্য);

    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি; i++) {
      যদি (!sanitize_মান_devices (c, base, values))
        return_trace (মিথ্যা);
      values = &StructAtOffset<ধ্রুবক মান> (values, stride);
    }

    return_trace (সত্য);
  }

 ব্যক্তিগত:

  শূন্য should_drop (মান value, Flags flag, unsigned পূর্ণসংখ্যা* format) ধ্রুবক
  {
    যদি (value) ফেরত;
    *format = *format & ~flag;
  }

  শূন্য update_var_flag (ধ্রুবক মান* value, Flags flag,
                        unsigned পূর্ণসংখ্যা* format, ধ্রুবক মান_Base *base,
                        ধ্রুবক hb_hashmap_t<unsigned, hb_pair_t<unsigned, পূর্ণসংখ্যা>> *varidx_delta_map) ধ্রুবক
  {
    যদি (*value)
    {
      unsigned varidx = (base + get_device (value)).get_variation_সূচক ();
      hb_pair_t<unsigned, পূর্ণসংখ্যা> *varidx_delta;
      যদি (varidx_delta_map->has (varidx, &varidx_delta))
      {
        /* A device-only value needs a base field for its instanced delta. */
        যদি (varidx_delta->second)
          *format |= flag >> 4;
        যদি (varidx_delta->first != HB_OT_LAYOUT_NO_VARIATIONS_সূচক)
          ফেরত;
      }
    }
    *format = *format & ~flag;
  }
};

}
}
}

#endif  // #ifndef OT_LAYOUT_GPOS_VALUEFORMAT_HH

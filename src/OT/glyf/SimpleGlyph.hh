#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_SIMPLEGLYPH_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_SIMPLEGLYPH_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ glyf_impl {


ਬਣਤਰ SimpleGlyph
{
  ਗਿਣਤੀ simple_glyph_flag_t
  {
    FLAG_ON_ਵਕਰ       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ਅਚਲ Glyph_ਸਿਰਲੇਖ &ਸਿਰਲੇਖ;
  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ;
  SimpleGlyph (ਅਚਲ Glyph_ਸਿਰਲੇਖ &ਸਿਰਲੇਖ_2, hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ_2) :
    ਸਿਰਲੇਖ (ਸਿਰਲੇਖ_2), ਬਾਈਟਾਂ (ਬਾਈਟਾਂ_2) {}

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instruction_len_ਵਿੱਥ () ਅਚਲ
  { ਵਾਪਸ Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ + 2 * ਸਿਰਲੇਖ.ਸੰਖਿਆ_ਦਾ_Contours; }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਲੰਮਾਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instruction_len) ਅਚਲ
  { ਵਾਪਸ instruction_len_ਵਿੱਥ () + 2 + instruction_len; }

  ਤਾਰਕਿਕ has_instructions_ਲੰਮਾਈ () ਅਚਲ
  {
    ਵਾਪਸ instruction_len_ਵਿੱਥ () + 2 <= ਬਾਈਟਾਂ.ਲੰਮਾਈ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instructions_ਲੰਮਾਈ () ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instruction_ਲੰਮਾਈ_ਵਿੱਥ = instruction_len_ਵਿੱਥ ();
    ਜੇ (unlikely (instruction_ਲੰਮਾਈ_ਵਿੱਥ + 2 > ਬਾਈਟਾਂ.ਲੰਮਾਈ)) ਵਾਪਸ 0;

    ਅਚਲ HBUINT16 &instruction_ਲੰਮਾਈ = Struct_At_ਵਿੱਥ<HBUINT16> (&ਬਾਈਟਾਂ, instruction_ਲੰਮਾਈ_ਵਿੱਥ);
    /* Out of bounds of the current glyph */
    ਜੇ (unlikely (ਲੰਮਾਈ (instruction_ਲੰਮਾਈ) > ਬਾਈਟਾਂ.ਲੰਮਾਈ)) ਵਾਪਸ 0;
    ਵਾਪਸ instruction_ਲੰਮਾਈ;
  }

  ਅਚਲ hb_ਬਾਈਟਾਂ_t trim_padding () ਅਚਲ
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ਅਚਲ uint8_t *glyph = (uint8_t*) ਬਾਈਟਾਂ.arrayZ;
    ਅਚਲ uint8_t *glyph_ਸਿਰਾ = glyph + ਬਾਈਟਾਂ.ਲੰਮਾਈ;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_ਵਿੱਥ ();

    ਜੇ (unlikely (glyph + 2 >= glyph_ਸਿਰਾ)) ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_coordinates = Struct_At_ਵਿੱਥ<HBUINT16> (glyph - 2, 0) + 1;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_instructions = Struct_At_ਵਿੱਥ<HBUINT16> (glyph, 0);

    glyph += 2 + ਅੰਕ_instructions;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coord_ਬਾਈਟਾਂ = 0;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ coords_with_flags = 0;
    ਜਦੋਂ (glyph < glyph_ਸਿਰਾ)
    {
      uint8_t flag = *glyph;
      glyph++;

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ repeat = 1;
      ਜੇ (flag & FLAG_REPEAT)
      {
	ਜੇ (unlikely (glyph >= glyph_ਸਿਰਾ)) ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ x_ਬਾਈਟਾਂ, y_ਬਾਈਟਾਂ;
      x_ਬਾਈਟਾਂ = y_ਬਾਈਟਾਂ = 0;
      ਜੇ (flag & FLAG_X_SHORT) x_ਬਾਈਟਾਂ = 1;
      ਨਹੀਂਤਾਂ ਜੇ ((flag & FLAG_X_SAME) == 0) x_ਬਾਈਟਾਂ = 2;

      ਜੇ (flag & FLAG_Y_SHORT) y_ਬਾਈਟਾਂ = 1;
      ਨਹੀਂਤਾਂ ਜੇ ((flag & FLAG_Y_SAME) == 0) y_ਬਾਈਟਾਂ = 2;

      coord_ਬਾਈਟਾਂ += (x_ਬਾਈਟਾਂ + y_ਬਾਈਟਾਂ) * repeat;
      coords_with_flags += repeat;
      ਜੇ (coords_with_flags >= ਅੰਕ_coordinates) ਰੋਕੋ;
    }

    ਜੇ (unlikely (coords_with_flags != ਅੰਕ_coordinates)) ਵਾਪਸ hb_ਬਾਈਟਾਂ_t ();
    ਵਾਪਸ ਬਾਈਟਾਂ.sub_array (0, ਬਾਈਟਾਂ.ਲੰਮਾਈ + coord_ਬਾਈਟਾਂ - (glyph_ਸਿਰਾ - glyph));
  }

  /* zero instruction length */
  ਖਾਲੀ drop_hints ()
  {
    ਜੇ (!has_instructions_ਲੰਮਾਈ ()) ਵਾਪਸ;
    Glyph_ਸਿਰਲੇਖ &glyph_ਸਿਰਲੇਖ = const_cast<Glyph_ਸਿਰਲੇਖ &> (ਸਿਰਲੇਖ);
    (HBUINT16 &) Struct_At_ਵਿੱਥ<HBUINT16> (&glyph_ਸਿਰਲੇਖ, instruction_len_ਵਿੱਥ ()) = 0;
  }

  ਖਾਲੀ drop_hints_ਬਾਈਟਾਂ (hb_ਬਾਈਟਾਂ_t &dest_ਸ਼ੁਰੂ, hb_ਬਾਈਟਾਂ_t &dest_ਸਿਰਾ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instructions_len = instructions_ਲੰਮਾਈ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਲੰਮਾਈ = ਲੰਮਾਈ (instructions_len);
    dest_ਸ਼ੁਰੂ = ਬਾਈਟਾਂ.sub_array (0, glyph_ਲੰਮਾਈ - instructions_len);
    dest_ਸਿਰਾ = ਬਾਈਟਾਂ.sub_array (glyph_ਲੰਮਾਈ, ਬਾਈਟਾਂ.ਲੰਮਾਈ - glyph_ਲੰਮਾਈ);
  }

  ਖਾਲੀ ਸੈੱਟ_overlaps_flag ()
  {
    ਜੇ (unlikely (!ਸਿਰਲੇਖ.ਸੰਖਿਆ_ਦਾ_Contours)) ਵਾਪਸ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ flags_ਵਿੱਥ = ਲੰਮਾਈ (instructions_ਲੰਮਾਈ ());
    ਜੇ (unlikely (flags_ਵਿੱਥ + 1 > ਬਾਈਟਾਂ.ਲੰਮਾਈ)) ਵਾਪਸ;

    HBUINT8 &ਪਹਿਲਾ_flag = (HBUINT8 &) Struct_At_ਵਿੱਥ<HBUINT16> (&ਬਾਈਟਾਂ, flags_ਵਿੱਥ);
    ਪਹਿਲਾ_flag = (uint8_t) ਪਹਿਲਾ_flag | FLAG_OVERLAP_SIMPLE;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ read_flags (ਅਚਲ HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_ਬਿੰਦੂ_t> points_ /* IN/OUT */,
			  ਅਚਲ HBUINT8 *ਸਿਰਾ,
			  ਤਾਰਕਿਕ extended)
  {
    ਸਵੈਚਾਲਿਤ *points = points_.arrayZ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = points_.ਲੰਮਾਈ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_2;)
    {
      ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
      uint8_t flag = *p++;
      ਜੇ (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ਜੇ (flag & FLAG_REPEAT)
      {
	ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ repeat_ਗਿਣਤੀ = *p++;
	ਬਿਨਾਨਿਸ਼ਾਨੀ stop = hb_min (i + repeat_ਗਿਣਤੀ, ਗਿਣਤੀ_2);
	ਲਈ (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਸਥਿਰ ਤਾਰਕਿਕ read_points (ਅਚਲ HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_ਬਿੰਦੂ_t> points_ /* IN/OUT */,
			   ਅਚਲ HBUINT8 *ਸਿਰਾ,
			   ਦਸ਼ਮਲਵ contour_ਬਿੰਦੂ_t::*m,
			   ਅਚਲ simple_glyph_flag_t short_flag,
			   ਅਚਲ simple_glyph_flag_t same_flag)
  {
    ਪੂਰਨ v = 0;

    ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : points_)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ flag = ਬਿੰਦੂ.flag;
      ਜੇ (flag & short_flag)
      {
	ਜੇ (unlikely (p + 1 > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	v += (ਤਾਰਕਿਕ(flag & same_flag) * 2 - 1) * *p++;
      }
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (!(flag & same_flag))
	{
	  ਜੇ (unlikely (p + HBINT16::static_ਮਾਪ > ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;
	  v += *(ਅਚਲ HBINT16 *) p;
	  p += HBINT16::static_ਮਾਪ;
	}
      }
      ਬਿੰਦੂ.*m = v;
    }
    ਵਾਪਸ ਸੱਚ;
  }

  ਤਾਰਕਿਕ get_contour_points (contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &points /*ਬਾਹਰਲਾ*/,
			   ਤਾਰਕਿਕ extended,
			   ਤਾਰਕਿਕ phantom_only = ਝੂਠ,
			   int64_t *budget = ਖਾਲੀ_ਸੂਚਕ) ਅਚਲ
  {
    ਅਚਲ HBUINT16 *ਸਿਰਾ_Pts_ਦਾ_Contours = &StructAfter<HBUINT16> (ਸਿਰਲੇਖ);
    ਪੂਰਨ ਅੰਕ_contours = ਸਿਰਲੇਖ.ਸੰਖਿਆ_ਦਾ_Contours;
    ਪ੍ਰਤਿਜ੍ਞਾ (ਅੰਕ_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ਜੇ (unlikely (!ਬਾਈਟਾਂ.check_range (&ਸਿਰਾ_Pts_ਦਾ_Contours[ਅੰਕ_contours]))) ਵਾਪਸ ਝੂਠ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_points = ਸਿਰਾ_Pts_ਦਾ_Contours[ਅੰਕ_contours - 1] + 1;
    ਜੇ (unlikely (ਅੰਕ_points < (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਅੰਕ_contours)) ਵਾਪਸ ਝੂਠ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ਜੇ (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, ਅੰਕ_points))) ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਲੰਮਾਈ = points.ਲੰਮਾਈ;
    points.alloc (points.ਲੰਮਾਈ + ਅੰਕ_points + 4); // Allocate for phantom points, to avoid a possible copy
    ਜੇ (unlikely (!points.resize_dirty (points.ਲੰਮਾਈ + ਅੰਕ_points))) ਵਾਪਸ ਝੂਠ;
    ਸਵੈਚਾਲਿਤ points_ = points.as_array ().sub_array (old_ਲੰਮਾਈ);
    ਜੇ (!phantom_only)
      hb_memset (points_.arrayZ, 0, ਆਕਾਰ (contour_ਬਿੰਦੂ_t) * ਅੰਕ_points);
    ਜੇ (phantom_only) ਵਾਪਸ ਸੱਚ;

    ਲਈ (ਪੂਰਨ i = 0; i < ਅੰਕ_contours; i++)
      points_[ਸਿਰਾ_Pts_ਦਾ_Contours[i]].is_ਸਿਰਾ_ਬਿੰਦੂ = ਸੱਚ;

    /* Skip instructions */
    ਅਚਲ HBUINT8 *p = &Struct_At_ਵਿੱਥ<HBUINT8> (&ਸਿਰਾ_Pts_ਦਾ_Contours[ਅੰਕ_contours + 1],
						 ਸਿਰਾ_Pts_ਦਾ_Contours[ਅੰਕ_contours]);

    ਜੇ (unlikely ((ਅਚਲ ਅੱਖਰ *) p < ਬਾਈਟਾਂ.arrayZ)) ਵਾਪਸ ਝੂਠ; /* Unlikely overflow */
    ਅਚਲ HBUINT8 *ਸਿਰਾ = (ਅਚਲ HBUINT8 *) (ਬਾਈਟਾਂ.arrayZ + ਬਾਈਟਾਂ.ਲੰਮਾਈ);
    ਜੇ (unlikely (p >= ਸਿਰਾ)) ਵਾਪਸ ਝੂਠ;

    /* Read x & y coordinates */
    ਵਾਪਸ read_flags (p, points_, ਸਿਰਾ, extended)
        && read_points (p, points_, ਸਿਰਾ, &contour_ਬਿੰਦੂ_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, ਸਿਰਾ, &contour_ਬਿੰਦੂ_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ਸਥਿਰ ਖਾਲੀ encode_coord (ਪੂਰਨ value,
                            ਬਿਨਾਨਿਸ਼ਾਨੀ &flag,
                            ਅਚਲ simple_glyph_flag_t short_flag,
                            ਅਚਲ simple_glyph_flag_t same_flag,
                            hb_ਸਦਿਸ਼_t<uint8_t> &coords /*ਬਾਹਰਲਾ*/)
  {
    ਜੇ (value == 0)
    {
      flag |= same_flag;
    }
    ਨਹੀਂਤਾਂ ਜੇ (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ਜੇ (value > 0) flag |= same_flag;
      ਨਹੀਂਤਾਂ value = -value;

      coords.arrayZ[coords.ਲੰਮਾਈ++] = (uint8_t) value;
    }
    ਨਹੀਂਤਾਂ
    {
      int16_t val = value;
      coords.arrayZ[coords.ਲੰਮਾਈ++] = val >> 8;
      coords.arrayZ[coords.ਲੰਮਾਈ++] = val & 0xff;
    }
  }

  ਸਥਿਰ ਖਾਲੀ encode_flag (ਬਿਨਾਨਿਸ਼ਾਨੀ flag,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ &repeat,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ lastflag,
                           hb_ਸਦਿਸ਼_t<uint8_t> &flags /*ਬਾਹਰਲਾ*/)
  {
    ਜੇ (flag == lastflag && repeat != 255)
    {
      repeat++;
      ਜੇ (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.ਲੰਮਾਈ++] = flag;
      }
      ਨਹੀਂਤਾਂ
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ len = flags.ਲੰਮਾਈ;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    ਨਹੀਂਤਾਂ
    {
      repeat = 0;
      flags.arrayZ[flags.ਲੰਮਾਈ++] = flag;
    }
  }

  ਤਾਰਕਿਕ compile_ਬਾਈਟਾਂ_with_deltas (ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &all_points,
                                  ਤਾਰਕਿਕ no_hinting,
                                  hb_ਬਾਈਟਾਂ_t &dest_ਬਾਈਟਾਂ /*ਬਾਹਰਲਾ*/)
  {
    ਜੇ (ਸਿਰਲੇਖ.ਸੰਖਿਆ_ਦਾ_Contours == 0 || all_points.ਲੰਮਾਈ <= 4)
    {
      dest_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t ();
      ਵਾਪਸ ਸੱਚ;
    }
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_points = all_points.ਲੰਮਾਈ - 4;

    hb_ਸਦਿਸ਼_t<uint8_t> flags, x_coords, y_coords;
    ਜੇ (unlikely (!flags.alloc_exact (ਅੰਕ_points))) ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely (!x_coords.alloc_exact (2*ਅੰਕ_points))) ਵਾਪਸ ਝੂਠ;
    ਜੇ (unlikely (!y_coords.alloc_exact (2*ਅੰਕ_points))) ਵਾਪਸ ਝੂਠ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ lastflag = 255, repeat = 0;
    ਪੂਰਨ prev_x = 0, prev_y = 0;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_points; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_ਵਕਰ | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      ਪੂਰਨ cur_x = roundf (all_points.arrayZ[i].x);
      ਪੂਰਨ cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ len_before_instrs = 2 * ਸਿਰਲੇਖ.ਸੰਖਿਆ_ਦਾ_Contours + 2;
    ਬਿਨਾਨਿਸ਼ਾਨੀ len_instrs = instructions_ਲੰਮਾਈ ();
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_len = len_before_instrs + flags.ਲੰਮਾਈ + x_coords.ਲੰਮਾਈ + y_coords.ਲੰਮਾਈ;

    ਜੇ (!no_hinting)
      ਕੁੱਲ_len += len_instrs;

    ਅੱਖਰ *p = (ਅੱਖਰ *) hb_malloc (ਕੁੱਲ_len);
    ਜੇ (unlikely (!p)) ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਅੱਖਰ *src = ਬਾਈਟਾਂ.arrayZ + Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ;
    ਅੱਖਰ *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ਜੇ (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.ਲੰਮਾਈ);
    cur += flags.ਲੰਮਾਈ;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.ਲੰਮਾਈ);
    cur += x_coords.ਲੰਮਾਈ;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.ਲੰਮਾਈ);

    dest_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t (p, ਕੁੱਲ_len);
    ਵਾਪਸ ਸੱਚ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_SIMPLEGLYPH_HH */

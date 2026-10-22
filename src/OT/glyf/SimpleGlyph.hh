#اگر_تعریف_نشده OT_GLYF_SIMPLEGLYPH_HH
#تعریف_کن OT_GLYF_SIMPLEGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {
نطاق glyf_impl {


بنية SimpleGlyph
{
  تعداد simple_glyph_flag_t
  {
    FLAG_ON_منحنی       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ثابت Glyph_سرآیند &سرآیند;
  hb_بایتها_t بایتها;
  SimpleGlyph (ثابت Glyph_سرآیند &سرآیند_2, hb_بایتها_t بایتها_2) :
    سرآیند (سرآیند_2), بایتها (بایتها_2) {}

  بیعلامت صحيح instruction_len_جابه_جایی () ثابت
  { أرجع Glyph_سرآیند::static_اندازه + 2 * سرآیند.عدد_از_Contours; }

  بیعلامت صحيح درازی (بیعلامت صحيح instruction_len) ثابت
  { أرجع instruction_len_جابه_جایی () + 2 + instruction_len; }

  منطقي has_instructions_درازی () ثابت
  {
    أرجع instruction_len_جابه_جایی () + 2 <= بایتها.درازی;
  }

  بیعلامت صحيح instructions_درازی () ثابت
  {
    بیعلامت صحيح instruction_درازی_جابه_جایی = instruction_len_جابه_جایی ();
    إذا (unlikely (instruction_درازی_جابه_جایی + 2 > بایتها.درازی)) أرجع 0;

    ثابت HBUINT16 &instruction_درازی = Struct_At_جابه_جایی<HBUINT16> (&بایتها, instruction_درازی_جابه_جایی);
    /* Out of bounds of the current glyph */
    إذا (unlikely (درازی (instruction_درازی) > بایتها.درازی)) أرجع 0;
    أرجع instruction_درازی;
  }

  ثابت hb_بایتها_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) بایتها.arrayZ;
    ثابت uint8_t *glyph_انتها = glyph + بایتها.درازی;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_جابه_جایی ();

    إذا (unlikely (glyph + 2 >= glyph_انتها)) أرجع hb_بایتها_t ();
    بیعلامت صحيح رقم_coordinates = Struct_At_جابه_جایی<HBUINT16> (glyph - 2, 0) + 1;
    بیعلامت صحيح رقم_instructions = Struct_At_جابه_جایی<HBUINT16> (glyph, 0);

    glyph += 2 + رقم_instructions;

    بیعلامت صحيح coord_بایتها = 0;
    بیعلامت صحيح coords_with_flags = 0;
    طالما (glyph < glyph_انتها)
    {
      uint8_t flag = *glyph;
      glyph++;

      بیعلامت صحيح repeat = 1;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (glyph >= glyph_انتها)) أرجع hb_بایتها_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      بیعلامت صحيح x_بایتها, y_بایتها;
      x_بایتها = y_بایتها = 0;
      إذا (flag & FLAG_X_SHORT) x_بایتها = 1;
      وإلا إذا ((flag & FLAG_X_SAME) == 0) x_بایتها = 2;

      إذا (flag & FLAG_Y_SHORT) y_بایتها = 1;
      وإلا إذا ((flag & FLAG_Y_SAME) == 0) y_بایتها = 2;

      coord_بایتها += (x_بایتها + y_بایتها) * repeat;
      coords_with_flags += repeat;
      إذا (coords_with_flags >= رقم_coordinates) اكسر;
    }

    إذا (unlikely (coords_with_flags != رقم_coordinates)) أرجع hb_بایتها_t ();
    أرجع بایتها.sub_array (0, بایتها.درازی + coord_بایتها - (glyph_انتها - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    إذا (!has_instructions_درازی ()) أرجع;
    Glyph_سرآیند &glyph_سرآیند = const_cast<Glyph_سرآیند &> (سرآیند);
    (HBUINT16 &) Struct_At_جابه_جایی<HBUINT16> (&glyph_سرآیند, instruction_len_جابه_جایی ()) = 0;
  }

  فراغ drop_hints_بایتها (hb_بایتها_t &dest_شروع, hb_بایتها_t &dest_انتها) ثابت
  {
    بیعلامت صحيح instructions_len = instructions_درازی ();
    بیعلامت صحيح glyph_درازی = درازی (instructions_len);
    dest_شروع = بایتها.sub_array (0, glyph_درازی - instructions_len);
    dest_انتها = بایتها.sub_array (glyph_درازی, بایتها.درازی - glyph_درازی);
  }

  فراغ مجموعه_overlaps_flag ()
  {
    إذا (unlikely (!سرآیند.عدد_از_Contours)) أرجع;

    بیعلامت flags_جابه_جایی = درازی (instructions_درازی ());
    إذا (unlikely (flags_جابه_جایی + 1 > بایتها.درازی)) أرجع;

    HBUINT8 &اولی_flag = (HBUINT8 &) Struct_At_جابه_جایی<HBUINT16> (&بایتها, flags_جابه_جایی);
    اولی_flag = (uint8_t) اولی_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساكن منطقي read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_نقطه_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *انتها,
			  منطقي extended)
  {
    خودکار *points = points_.arrayZ;
    بیعلامت شمار = points_.درازی;
    لكل (بیعلامت صحيح i = 0; i < شمار;)
    {
      إذا (unlikely (p + 1 > انتها)) أرجع خطأ;
      uint8_t flag = *p++;
      إذا (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (p + 1 > انتها)) أرجع خطأ;
	بیعلامت صحيح repeat_شمار = *p++;
	بیعلامت stop = hb_min (i + repeat_شمار, شمار);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    أرجع درست;
  }

  ساكن منطقي read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_نقطه_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *انتها,
			   عائم contour_نقطه_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحيح v = 0;

    لكل (خودکار &نقطه : points_)
    {
      بیعلامت flag = نقطه.flag;
      إذا (flag & short_flag)
      {
	إذا (unlikely (p + 1 > انتها)) أرجع خطأ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      وإلا
      {
	إذا (!(flag & same_flag))
	{
	  إذا (unlikely (p + HBINT16::static_اندازه > انتها)) أرجع خطأ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_اندازه;
	}
      }
      نقطه.*m = v;
    }
    أرجع درست;
  }

  منطقي get_contour_points (contour_نقطه_وکتور_t &points /*بیرون*/,
			   منطقي extended,
			   منطقي phantom_only = خطأ,
			   int64_t *budget = اشاره_تهی) ثابت
  {
    ثابت HBUINT16 *انتها_Pts_از_Contours = &StructAfter<HBUINT16> (سرآیند);
    صحيح رقم_contours = سرآیند.عدد_از_Contours;
    أكد (رقم_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    إذا (unlikely (!بایتها.check_range (&انتها_Pts_از_Contours[رقم_contours]))) أرجع خطأ;
    بیعلامت صحيح رقم_points = انتها_Pts_از_Contours[رقم_contours - 1] + 1;
    إذا (unlikely (رقم_points < (بیعلامت) رقم_contours)) أرجع خطأ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    إذا (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, رقم_points))) أرجع خطأ;

    بیعلامت old_درازی = points.درازی;
    points.alloc (points.درازی + رقم_points + 4); // Allocate for phantom points, to avoid a possible copy
    إذا (unlikely (!points.resize_dirty (points.درازی + رقم_points))) أرجع خطأ;
    خودکار points_ = points.as_array ().sub_array (old_درازی);
    إذا (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_نقطه_t) * رقم_points);
    إذا (phantom_only) أرجع درست;

    لكل (صحيح i = 0; i < رقم_contours; i++)
      points_[انتها_Pts_از_Contours[i]].is_انتها_نقطه = درست;

    /* Skip instructions */
    ثابت HBUINT8 *p = &Struct_At_جابه_جایی<HBUINT8> (&انتها_Pts_از_Contours[رقم_contours + 1],
						 انتها_Pts_از_Contours[رقم_contours]);

    إذا (unlikely ((ثابت حرف *) p < بایتها.arrayZ)) أرجع خطأ; /* Unlikely overflow */
    ثابت HBUINT8 *انتها = (ثابت HBUINT8 *) (بایتها.arrayZ + بایتها.درازی);
    إذا (unlikely (p >= انتها)) أرجع خطأ;

    /* Read x & y coordinates */
    أرجع read_flags (p, points_, انتها, extended)
        && read_points (p, points_, انتها, &contour_نقطه_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, انتها, &contour_نقطه_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ساكن فراغ encode_coord (صحيح value,
                            بیعلامت &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_وکتور_t<uint8_t> &coords /*بیرون*/)
  {
    إذا (value == 0)
    {
      flag |= same_flag;
    }
    وإلا إذا (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      إذا (value > 0) flag |= same_flag;
      وإلا value = -value;

      coords.arrayZ[coords.درازی++] = (uint8_t) value;
    }
    وإلا
    {
      int16_t val = value;
      coords.arrayZ[coords.درازی++] = val >> 8;
      coords.arrayZ[coords.درازی++] = val & 0xff;
    }
  }

  ساكن فراغ encode_flag (بیعلامت flag,
                           بیعلامت &repeat,
                           بیعلامت lastflag,
                           hb_وکتور_t<uint8_t> &flags /*بیرون*/)
  {
    إذا (flag == lastflag && repeat != 255)
    {
      repeat++;
      إذا (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.درازی++] = flag;
      }
      وإلا
      {
        بیعلامت len = flags.درازی;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    وإلا
    {
      repeat = 0;
      flags.arrayZ[flags.درازی++] = flag;
    }
  }

  منطقي compile_بایتها_with_deltas (ثابت contour_نقطه_وکتور_t &all_points,
                                  منطقي no_hinting,
                                  hb_بایتها_t &dest_بایتها /*بیرون*/)
  {
    إذا (سرآیند.عدد_از_Contours == 0 || all_points.درازی <= 4)
    {
      dest_بایتها = hb_بایتها_t ();
      أرجع درست;
    }
    بیعلامت رقم_points = all_points.درازی - 4;

    hb_وکتور_t<uint8_t> flags, x_coords, y_coords;
    إذا (unlikely (!flags.alloc_exact (رقم_points))) أرجع خطأ;
    إذا (unlikely (!x_coords.alloc_exact (2*رقم_points))) أرجع خطأ;
    إذا (unlikely (!y_coords.alloc_exact (2*رقم_points))) أرجع خطأ;

    بیعلامت lastflag = 255, repeat = 0;
    صحيح prev_x = 0, prev_y = 0;

    لكل (بیعلامت i = 0; i < رقم_points; i++)
    {
      بیعلامت flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_منحنی | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحيح cur_x = roundf (all_points.arrayZ[i].x);
      صحيح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    بیعلامت len_before_instrs = 2 * سرآیند.عدد_از_Contours + 2;
    بیعلامت len_instrs = instructions_درازی ();
    بیعلامت مجموع_len = len_before_instrs + flags.درازی + x_coords.درازی + y_coords.درازی;

    إذا (!no_hinting)
      مجموع_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (مجموع_len);
    إذا (unlikely (!p)) أرجع خطأ;

    ثابت حرف *src = بایتها.arrayZ + Glyph_سرآیند::static_اندازه;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    إذا (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.درازی);
    cur += flags.درازی;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.درازی);
    cur += x_coords.درازی;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.درازی);

    dest_بایتها = hb_بایتها_t (p, مجموع_len);
    أرجع درست;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#پایان_اگر /* OT_GLYF_SIMPLEGLYPH_HH */

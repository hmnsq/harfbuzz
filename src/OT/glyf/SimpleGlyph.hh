#جے_بنا_تعریف OT_GLYF_SIMPLEGLYPH_HH
#تعریف_کرو OT_GLYF_SIMPLEGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {
نطاق glyf_impl {


بنية SimpleGlyph
{
  تعداد simple_glyph_flag_t
  {
    FLAG_ON_ونگ       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ثابت Glyph_سرخی &سرخی;
  hb_بائٹاں_t بائٹاں;
  SimpleGlyph (ثابت Glyph_سرخی &سرخی_2, hb_بائٹاں_t بائٹاں_2) :
    سرخی (سرخی_2), بائٹاں (بائٹاں_2) {}

  بنانشانی صحيح instruction_len_وتھ () ثابت
  { أرجع Glyph_سرخی::static_ناپ + 2 * سرخی.نمبر_دا_Contours; }

  بنانشانی صحيح لمائی (بنانشانی صحيح instruction_len) ثابت
  { أرجع instruction_len_وتھ () + 2 + instruction_len; }

  منطقي has_instructions_لمائی () ثابت
  {
    أرجع instruction_len_وتھ () + 2 <= بائٹاں.لمائی;
  }

  بنانشانی صحيح instructions_لمائی () ثابت
  {
    بنانشانی صحيح instruction_لمائی_وتھ = instruction_len_وتھ ();
    إذا (unlikely (instruction_لمائی_وتھ + 2 > بائٹاں.لمائی)) أرجع 0;

    ثابت HBUINT16 &instruction_لمائی = Struct_At_وتھ<HBUINT16> (&بائٹاں, instruction_لمائی_وتھ);
    /* Out of bounds of the current glyph */
    إذا (unlikely (لمائی (instruction_لمائی) > بائٹاں.لمائی)) أرجع 0;
    أرجع instruction_لمائی;
  }

  ثابت hb_بائٹاں_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) بائٹاں.arrayZ;
    ثابت uint8_t *glyph_سرا = glyph + بائٹاں.لمائی;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_وتھ ();

    إذا (unlikely (glyph + 2 >= glyph_سرا)) أرجع hb_بائٹاں_t ();
    بنانشانی صحيح عدد_coordinates = Struct_At_وتھ<HBUINT16> (glyph - 2, 0) + 1;
    بنانشانی صحيح عدد_instructions = Struct_At_وتھ<HBUINT16> (glyph, 0);

    glyph += 2 + عدد_instructions;

    بنانشانی صحيح coord_بائٹاں = 0;
    بنانشانی صحيح coords_with_flags = 0;
    طالما (glyph < glyph_سرا)
    {
      uint8_t flag = *glyph;
      glyph++;

      بنانشانی صحيح repeat = 1;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (glyph >= glyph_سرا)) أرجع hb_بائٹاں_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      بنانشانی صحيح x_بائٹاں, y_بائٹاں;
      x_بائٹاں = y_بائٹاں = 0;
      إذا (flag & FLAG_X_SHORT) x_بائٹاں = 1;
      وإلا إذا ((flag & FLAG_X_SAME) == 0) x_بائٹاں = 2;

      إذا (flag & FLAG_Y_SHORT) y_بائٹاں = 1;
      وإلا إذا ((flag & FLAG_Y_SAME) == 0) y_بائٹاں = 2;

      coord_بائٹاں += (x_بائٹاں + y_بائٹاں) * repeat;
      coords_with_flags += repeat;
      إذا (coords_with_flags >= عدد_coordinates) اكسر;
    }

    إذا (unlikely (coords_with_flags != عدد_coordinates)) أرجع hb_بائٹاں_t ();
    أرجع بائٹاں.sub_array (0, بائٹاں.لمائی + coord_بائٹاں - (glyph_سرا - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    إذا (!has_instructions_لمائی ()) أرجع;
    Glyph_سرخی &glyph_سرخی = const_cast<Glyph_سرخی &> (سرخی);
    (HBUINT16 &) Struct_At_وتھ<HBUINT16> (&glyph_سرخی, instruction_len_وتھ ()) = 0;
  }

  فراغ drop_hints_بائٹاں (hb_بائٹاں_t &dest_شروع, hb_بائٹاں_t &dest_سرا) ثابت
  {
    بنانشانی صحيح instructions_len = instructions_لمائی ();
    بنانشانی صحيح glyph_لمائی = لمائی (instructions_len);
    dest_شروع = بائٹاں.sub_array (0, glyph_لمائی - instructions_len);
    dest_سرا = بائٹاں.sub_array (glyph_لمائی, بائٹاں.لمائی - glyph_لمائی);
  }

  فراغ سیٹ_overlaps_flag ()
  {
    إذا (unlikely (!سرخی.نمبر_دا_Contours)) أرجع;

    بنانشانی flags_وتھ = لمائی (instructions_لمائی ());
    إذا (unlikely (flags_وتھ + 1 > بائٹاں.لمائی)) أرجع;

    HBUINT8 &پہلا_flag = (HBUINT8 &) Struct_At_وتھ<HBUINT16> (&بائٹاں, flags_وتھ);
    پہلا_flag = (uint8_t) پہلا_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساكن منطقي read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_نقطہ_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *سرا,
			  منطقي extended)
  {
    خودکار *points = points_.arrayZ;
    بنانشانی گنتی = points_.لمائی;
    لكل (بنانشانی صحيح i = 0; i < گنتی;)
    {
      إذا (unlikely (p + 1 > سرا)) أرجع خطأ;
      uint8_t flag = *p++;
      إذا (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (p + 1 > سرا)) أرجع خطأ;
	بنانشانی صحيح repeat_گنتی = *p++;
	بنانشانی stop = hb_min (i + repeat_گنتی, گنتی);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    أرجع سچ;
  }

  ساكن منطقي read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_نقطہ_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *سرا,
			   عائم contour_نقطہ_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحيح v = 0;

    لكل (خودکار &نقطہ : points_)
    {
      بنانشانی flag = نقطہ.flag;
      إذا (flag & short_flag)
      {
	إذا (unlikely (p + 1 > سرا)) أرجع خطأ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      وإلا
      {
	إذا (!(flag & same_flag))
	{
	  إذا (unlikely (p + HBINT16::static_ناپ > سرا)) أرجع خطأ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_ناپ;
	}
      }
      نقطہ.*m = v;
    }
    أرجع سچ;
  }

  منطقي get_contour_points (contour_نقطہ_سمتیہ_t &points /*باہرلا*/,
			   منطقي extended,
			   منطقي phantom_only = خطأ,
			   int64_t *budget = خالی_سوچک) ثابت
  {
    ثابت HBUINT16 *سرا_Pts_دا_Contours = &StructAfter<HBUINT16> (سرخی);
    صحيح عدد_contours = سرخی.نمبر_دا_Contours;
    أكد (عدد_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    إذا (unlikely (!بائٹاں.check_range (&سرا_Pts_دا_Contours[عدد_contours]))) أرجع خطأ;
    بنانشانی صحيح عدد_points = سرا_Pts_دا_Contours[عدد_contours - 1] + 1;
    إذا (unlikely (عدد_points < (بنانشانی) عدد_contours)) أرجع خطأ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    إذا (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, عدد_points))) أرجع خطأ;

    بنانشانی old_لمائی = points.لمائی;
    points.alloc (points.لمائی + عدد_points + 4); // Allocate for phantom points, to avoid a possible copy
    إذا (unlikely (!points.resize_dirty (points.لمائی + عدد_points))) أرجع خطأ;
    خودکار points_ = points.as_array ().sub_array (old_لمائی);
    إذا (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_نقطہ_t) * عدد_points);
    إذا (phantom_only) أرجع سچ;

    لكل (صحيح i = 0; i < عدد_contours; i++)
      points_[سرا_Pts_دا_Contours[i]].is_سرا_نقطہ = سچ;

    /* Skip instructions */
    ثابت HBUINT8 *p = &Struct_At_وتھ<HBUINT8> (&سرا_Pts_دا_Contours[عدد_contours + 1],
						 سرا_Pts_دا_Contours[عدد_contours]);

    إذا (unlikely ((ثابت حرف *) p < بائٹاں.arrayZ)) أرجع خطأ; /* Unlikely overflow */
    ثابت HBUINT8 *سرا = (ثابت HBUINT8 *) (بائٹاں.arrayZ + بائٹاں.لمائی);
    إذا (unlikely (p >= سرا)) أرجع خطأ;

    /* Read x & y coordinates */
    أرجع read_flags (p, points_, سرا, extended)
        && read_points (p, points_, سرا, &contour_نقطہ_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, سرا, &contour_نقطہ_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ساكن فراغ encode_coord (صحيح value,
                            بنانشانی &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_سمتیہ_t<uint8_t> &coords /*باہرلا*/)
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

      coords.arrayZ[coords.لمائی++] = (uint8_t) value;
    }
    وإلا
    {
      int16_t val = value;
      coords.arrayZ[coords.لمائی++] = val >> 8;
      coords.arrayZ[coords.لمائی++] = val & 0xff;
    }
  }

  ساكن فراغ encode_flag (بنانشانی flag,
                           بنانشانی &repeat,
                           بنانشانی lastflag,
                           hb_سمتیہ_t<uint8_t> &flags /*باہرلا*/)
  {
    إذا (flag == lastflag && repeat != 255)
    {
      repeat++;
      إذا (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.لمائی++] = flag;
      }
      وإلا
      {
        بنانشانی len = flags.لمائی;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    وإلا
    {
      repeat = 0;
      flags.arrayZ[flags.لمائی++] = flag;
    }
  }

  منطقي compile_بائٹاں_with_deltas (ثابت contour_نقطہ_سمتیہ_t &all_points,
                                  منطقي no_hinting,
                                  hb_بائٹاں_t &dest_بائٹاں /*باہرلا*/)
  {
    إذا (سرخی.نمبر_دا_Contours == 0 || all_points.لمائی <= 4)
    {
      dest_بائٹاں = hb_بائٹاں_t ();
      أرجع سچ;
    }
    بنانشانی عدد_points = all_points.لمائی - 4;

    hb_سمتیہ_t<uint8_t> flags, x_coords, y_coords;
    إذا (unlikely (!flags.alloc_exact (عدد_points))) أرجع خطأ;
    إذا (unlikely (!x_coords.alloc_exact (2*عدد_points))) أرجع خطأ;
    إذا (unlikely (!y_coords.alloc_exact (2*عدد_points))) أرجع خطأ;

    بنانشانی lastflag = 255, repeat = 0;
    صحيح prev_x = 0, prev_y = 0;

    لكل (بنانشانی i = 0; i < عدد_points; i++)
    {
      بنانشانی flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_ونگ | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحيح cur_x = roundf (all_points.arrayZ[i].x);
      صحيح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    بنانشانی len_before_instrs = 2 * سرخی.نمبر_دا_Contours + 2;
    بنانشانی len_instrs = instructions_لمائی ();
    بنانشانی کل_len = len_before_instrs + flags.لمائی + x_coords.لمائی + y_coords.لمائی;

    إذا (!no_hinting)
      کل_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (کل_len);
    إذا (unlikely (!p)) أرجع خطأ;

    ثابت حرف *src = بائٹاں.arrayZ + Glyph_سرخی::static_ناپ;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    إذا (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.لمائی);
    cur += flags.لمائی;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.لمائی);
    cur += x_coords.لمائی;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.لمائی);

    dest_بائٹاں = hb_بائٹاں_t (p, کل_len);
    أرجع سچ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_SIMPLEGLYPH_HH */

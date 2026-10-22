#که_نه_تعریف_شوی OT_GLYF_SIMPLEGLYPH_HH
#تعریف_کړه OT_GLYF_SIMPLEGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {
نطاق glyf_impl {


بنية SimpleGlyph
{
  تعداد simple_glyph_flag_t
  {
    FLAG_ON_منحني       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ثابت Glyph_سرلیک &سرلیک;
  hb_بایټونه_t بایټونه;
  SimpleGlyph (ثابت Glyph_سرلیک &سرلیک_2, hb_بایټونه_t بایټونه_2) :
    سرلیک (سرلیک_2), بایټونه (بایټونه_2) {}

  بینښه صحيح instruction_len_دننه_واټن () ثابت
  { أرجع Glyph_سرلیک::static_اندازه + 2 * سرلیک.شمېره_د_Contours; }

  بینښه صحيح اوږدتیا (بینښه صحيح instruction_len) ثابت
  { أرجع instruction_len_دننه_واټن () + 2 + instruction_len; }

  منطقي has_instructions_اوږدتیا () ثابت
  {
    أرجع instruction_len_دننه_واټن () + 2 <= بایټونه.اوږدتیا;
  }

  بینښه صحيح instructions_اوږدتیا () ثابت
  {
    بینښه صحيح instruction_اوږدتیا_دننه_واټن = instruction_len_دننه_واټن ();
    إذا (unlikely (instruction_اوږدتیا_دننه_واټن + 2 > بایټونه.اوږدتیا)) أرجع 0;

    ثابت HBUINT16 &instruction_اوږدتیا = Struct_At_دننه_واټن<HBUINT16> (&بایټونه, instruction_اوږدتیا_دننه_واټن);
    /* Out of bounds of the current glyph */
    إذا (unlikely (اوږدتیا (instruction_اوږدتیا) > بایټونه.اوږدتیا)) أرجع 0;
    أرجع instruction_اوږدتیا;
  }

  ثابت hb_بایټونه_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) بایټونه.arrayZ;
    ثابت uint8_t *glyph_پای_ټکی = glyph + بایټونه.اوږدتیا;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_دننه_واټن ();

    إذا (unlikely (glyph + 2 >= glyph_پای_ټکی)) أرجع hb_بایټونه_t ();
    بینښه صحيح عدد_coordinates = Struct_At_دننه_واټن<HBUINT16> (glyph - 2, 0) + 1;
    بینښه صحيح عدد_instructions = Struct_At_دننه_واټن<HBUINT16> (glyph, 0);

    glyph += 2 + عدد_instructions;

    بینښه صحيح coord_بایټونه = 0;
    بینښه صحيح coords_with_flags = 0;
    طالما (glyph < glyph_پای_ټکی)
    {
      uint8_t flag = *glyph;
      glyph++;

      بینښه صحيح repeat = 1;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (glyph >= glyph_پای_ټکی)) أرجع hb_بایټونه_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      بینښه صحيح x_بایټونه, y_بایټونه;
      x_بایټونه = y_بایټونه = 0;
      إذا (flag & FLAG_X_SHORT) x_بایټونه = 1;
      وإلا إذا ((flag & FLAG_X_SAME) == 0) x_بایټونه = 2;

      إذا (flag & FLAG_Y_SHORT) y_بایټونه = 1;
      وإلا إذا ((flag & FLAG_Y_SAME) == 0) y_بایټونه = 2;

      coord_بایټونه += (x_بایټونه + y_بایټونه) * repeat;
      coords_with_flags += repeat;
      إذا (coords_with_flags >= عدد_coordinates) اكسر;
    }

    إذا (unlikely (coords_with_flags != عدد_coordinates)) أرجع hb_بایټونه_t ();
    أرجع بایټونه.sub_array (0, بایټونه.اوږدتیا + coord_بایټونه - (glyph_پای_ټکی - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    إذا (!has_instructions_اوږدتیا ()) أرجع;
    Glyph_سرلیک &glyph_سرلیک = const_cast<Glyph_سرلیک &> (سرلیک);
    (HBUINT16 &) Struct_At_دننه_واټن<HBUINT16> (&glyph_سرلیک, instruction_len_دننه_واټن ()) = 0;
  }

  فراغ drop_hints_بایټونه (hb_بایټونه_t &dest_پیل, hb_بایټونه_t &dest_پای_ټکی) ثابت
  {
    بینښه صحيح instructions_len = instructions_اوږدتیا ();
    بینښه صحيح glyph_اوږدتیا = اوږدتیا (instructions_len);
    dest_پیل = بایټونه.sub_array (0, glyph_اوږدتیا - instructions_len);
    dest_پای_ټکی = بایټونه.sub_array (glyph_اوږدتیا, بایټونه.اوږدتیا - glyph_اوږدتیا);
  }

  فراغ سېټ_overlaps_flag ()
  {
    إذا (unlikely (!سرلیک.شمېره_د_Contours)) أرجع;

    بینښه flags_دننه_واټن = اوږدتیا (instructions_اوږدتیا ());
    إذا (unlikely (flags_دننه_واټن + 1 > بایټونه.اوږدتیا)) أرجع;

    HBUINT8 &لومړی_flag = (HBUINT8 &) Struct_At_دننه_واټن<HBUINT16> (&بایټونه, flags_دننه_واټن);
    لومړی_flag = (uint8_t) لومړی_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساكن منطقي read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_ټکی_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *پای_ټکی,
			  منطقي extended)
  {
    خپلکاری *points = points_.arrayZ;
    بینښه شمېر = points_.اوږدتیا;
    لكل (بینښه صحيح i = 0; i < شمېر;)
    {
      إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;
      uint8_t flag = *p++;
      إذا (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;
	بینښه صحيح repeat_شمېر = *p++;
	بینښه stop = hb_min (i + repeat_شمېر, شمېر);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    أرجع ریښتیا;
  }

  ساكن منطقي read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_ټکی_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *پای_ټکی,
			   عائم contour_ټکی_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحيح v = 0;

    لكل (خپلکاری &ټکی : points_)
    {
      بینښه flag = ټکی.flag;
      إذا (flag & short_flag)
      {
	إذا (unlikely (p + 1 > پای_ټکی)) أرجع خطأ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      وإلا
      {
	إذا (!(flag & same_flag))
	{
	  إذا (unlikely (p + HBINT16::static_اندازه > پای_ټکی)) أرجع خطأ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_اندازه;
	}
      }
      ټکی.*m = v;
    }
    أرجع ریښتیا;
  }

  منطقي get_contour_points (contour_ټکی_ویکټور_t &points /*بهر*/,
			   منطقي extended,
			   منطقي phantom_only = خطأ,
			   int64_t *budget = تش_اشاره) ثابت
  {
    ثابت HBUINT16 *پای_ټکی_Pts_د_Contours = &StructAfter<HBUINT16> (سرلیک);
    صحيح عدد_contours = سرلیک.شمېره_د_Contours;
    أكد (عدد_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    إذا (unlikely (!بایټونه.check_range (&پای_ټکی_Pts_د_Contours[عدد_contours]))) أرجع خطأ;
    بینښه صحيح عدد_points = پای_ټکی_Pts_د_Contours[عدد_contours - 1] + 1;
    إذا (unlikely (عدد_points < (بینښه) عدد_contours)) أرجع خطأ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    إذا (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, عدد_points))) أرجع خطأ;

    بینښه old_اوږدتیا = points.اوږدتیا;
    points.alloc (points.اوږدتیا + عدد_points + 4); // Allocate for phantom points, to avoid a possible copy
    إذا (unlikely (!points.resize_dirty (points.اوږدتیا + عدد_points))) أرجع خطأ;
    خپلکاری points_ = points.as_array ().sub_array (old_اوږدتیا);
    إذا (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_ټکی_t) * عدد_points);
    إذا (phantom_only) أرجع ریښتیا;

    لكل (صحيح i = 0; i < عدد_contours; i++)
      points_[پای_ټکی_Pts_د_Contours[i]].is_پای_ټکی_ټکی = ریښتیا;

    /* Skip instructions */
    ثابت HBUINT8 *p = &Struct_At_دننه_واټن<HBUINT8> (&پای_ټکی_Pts_د_Contours[عدد_contours + 1],
						 پای_ټکی_Pts_د_Contours[عدد_contours]);

    إذا (unlikely ((ثابت حرف *) p < بایټونه.arrayZ)) أرجع خطأ; /* Unlikely overflow */
    ثابت HBUINT8 *پای_ټکی = (ثابت HBUINT8 *) (بایټونه.arrayZ + بایټونه.اوږدتیا);
    إذا (unlikely (p >= پای_ټکی)) أرجع خطأ;

    /* Read x & y coordinates */
    أرجع read_flags (p, points_, پای_ټکی, extended)
        && read_points (p, points_, پای_ټکی, &contour_ټکی_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, پای_ټکی, &contour_ټکی_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ساكن فراغ encode_coord (صحيح value,
                            بینښه &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_ویکټور_t<uint8_t> &coords /*بهر*/)
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

      coords.arrayZ[coords.اوږدتیا++] = (uint8_t) value;
    }
    وإلا
    {
      int16_t val = value;
      coords.arrayZ[coords.اوږدتیا++] = val >> 8;
      coords.arrayZ[coords.اوږدتیا++] = val & 0xff;
    }
  }

  ساكن فراغ encode_flag (بینښه flag,
                           بینښه &repeat,
                           بینښه lastflag,
                           hb_ویکټور_t<uint8_t> &flags /*بهر*/)
  {
    إذا (flag == lastflag && repeat != 255)
    {
      repeat++;
      إذا (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.اوږدتیا++] = flag;
      }
      وإلا
      {
        بینښه len = flags.اوږدتیا;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    وإلا
    {
      repeat = 0;
      flags.arrayZ[flags.اوږدتیا++] = flag;
    }
  }

  منطقي compile_بایټونه_with_deltas (ثابت contour_ټکی_ویکټور_t &all_points,
                                  منطقي no_hinting,
                                  hb_بایټونه_t &dest_بایټونه /*بهر*/)
  {
    إذا (سرلیک.شمېره_د_Contours == 0 || all_points.اوږدتیا <= 4)
    {
      dest_بایټونه = hb_بایټونه_t ();
      أرجع ریښتیا;
    }
    بینښه عدد_points = all_points.اوږدتیا - 4;

    hb_ویکټور_t<uint8_t> flags, x_coords, y_coords;
    إذا (unlikely (!flags.alloc_exact (عدد_points))) أرجع خطأ;
    إذا (unlikely (!x_coords.alloc_exact (2*عدد_points))) أرجع خطأ;
    إذا (unlikely (!y_coords.alloc_exact (2*عدد_points))) أرجع خطأ;

    بینښه lastflag = 255, repeat = 0;
    صحيح prev_x = 0, prev_y = 0;

    لكل (بینښه i = 0; i < عدد_points; i++)
    {
      بینښه flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_منحني | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحيح cur_x = roundf (all_points.arrayZ[i].x);
      صحيح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    بینښه len_before_instrs = 2 * سرلیک.شمېره_د_Contours + 2;
    بینښه len_instrs = instructions_اوږدتیا ();
    بینښه ټول_len = len_before_instrs + flags.اوږدتیا + x_coords.اوږدتیا + y_coords.اوږدتیا;

    إذا (!no_hinting)
      ټول_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (ټول_len);
    إذا (unlikely (!p)) أرجع خطأ;

    ثابت حرف *src = بایټونه.arrayZ + Glyph_سرلیک::static_اندازه;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    إذا (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.اوږدتیا);
    cur += flags.اوږدتیا;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.اوږدتیا);
    cur += x_coords.اوږدتیا;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.اوږدتیا);

    dest_بایټونه = hb_بایټونه_t (p, ټول_len);
    أرجع ریښتیا;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#د_که_پای /* OT_GLYF_SIMPLEGLYPH_HH */

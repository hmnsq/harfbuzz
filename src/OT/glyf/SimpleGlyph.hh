#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_SIMPLEGLYPH_HH
#وصف_ڪريو OT_GLYF_SIMPLEGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {
نطاق glyf_impl {


بنية SimpleGlyph
{
  تعداد simple_glyph_flag_t
  {
    FLAG_ON_وڪر       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ثابت Glyph_سرو &سرو;
  hb_بائيٽون_t بائيٽون;
  SimpleGlyph (ثابت Glyph_سرو &سرو_2, hb_بائيٽون_t بائيٽون_2) :
    سرو (سرو_2), بائيٽون (بائيٽون_2) {}

  بنانشان صحيح instruction_len_وٿي () ثابت
  { أرجع Glyph_سرو::static_سائيز + 2 * سرو.عدد_جو_Contours; }

  بنانشان صحيح ڊيگهائي (بنانشان صحيح instruction_len) ثابت
  { أرجع instruction_len_وٿي () + 2 + instruction_len; }

  منطقي has_instructions_ڊيگهائي () ثابت
  {
    أرجع instruction_len_وٿي () + 2 <= بائيٽون.ڊيگهائي;
  }

  بنانشان صحيح instructions_ڊيگهائي () ثابت
  {
    بنانشان صحيح instruction_ڊيگهائي_وٿي = instruction_len_وٿي ();
    إذا (unlikely (instruction_ڊيگهائي_وٿي + 2 > بائيٽون.ڊيگهائي)) أرجع 0;

    ثابت HBUINT16 &instruction_ڊيگهائي = Struct_At_وٿي<HBUINT16> (&بائيٽون, instruction_ڊيگهائي_وٿي);
    /* Out of bounds of the current glyph */
    إذا (unlikely (ڊيگهائي (instruction_ڊيگهائي) > بائيٽون.ڊيگهائي)) أرجع 0;
    أرجع instruction_ڊيگهائي;
  }

  ثابت hb_بائيٽون_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) بائيٽون.arrayZ;
    ثابت uint8_t *glyph_پڇاڙي = glyph + بائيٽون.ڊيگهائي;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_وٿي ();

    إذا (unlikely (glyph + 2 >= glyph_پڇاڙي)) أرجع hb_بائيٽون_t ();
    بنانشان صحيح انگ_coordinates = Struct_At_وٿي<HBUINT16> (glyph - 2, 0) + 1;
    بنانشان صحيح انگ_instructions = Struct_At_وٿي<HBUINT16> (glyph, 0);

    glyph += 2 + انگ_instructions;

    بنانشان صحيح coord_بائيٽون = 0;
    بنانشان صحيح coords_with_flags = 0;
    طالما (glyph < glyph_پڇاڙي)
    {
      uint8_t flag = *glyph;
      glyph++;

      بنانشان صحيح repeat = 1;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (glyph >= glyph_پڇاڙي)) أرجع hb_بائيٽون_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      بنانشان صحيح x_بائيٽون, y_بائيٽون;
      x_بائيٽون = y_بائيٽون = 0;
      إذا (flag & FLAG_X_SHORT) x_بائيٽون = 1;
      وإلا إذا ((flag & FLAG_X_SAME) == 0) x_بائيٽون = 2;

      إذا (flag & FLAG_Y_SHORT) y_بائيٽون = 1;
      وإلا إذا ((flag & FLAG_Y_SAME) == 0) y_بائيٽون = 2;

      coord_بائيٽون += (x_بائيٽون + y_بائيٽون) * repeat;
      coords_with_flags += repeat;
      إذا (coords_with_flags >= انگ_coordinates) اكسر;
    }

    إذا (unlikely (coords_with_flags != انگ_coordinates)) أرجع hb_بائيٽون_t ();
    أرجع بائيٽون.sub_array (0, بائيٽون.ڊيگهائي + coord_بائيٽون - (glyph_پڇاڙي - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    إذا (!has_instructions_ڊيگهائي ()) أرجع;
    Glyph_سرو &glyph_سرو = const_cast<Glyph_سرو &> (سرو);
    (HBUINT16 &) Struct_At_وٿي<HBUINT16> (&glyph_سرو, instruction_len_وٿي ()) = 0;
  }

  فراغ drop_hints_بائيٽون (hb_بائيٽون_t &dest_شروعات, hb_بائيٽون_t &dest_پڇاڙي) ثابت
  {
    بنانشان صحيح instructions_len = instructions_ڊيگهائي ();
    بنانشان صحيح glyph_ڊيگهائي = ڊيگهائي (instructions_len);
    dest_شروعات = بائيٽون.sub_array (0, glyph_ڊيگهائي - instructions_len);
    dest_پڇاڙي = بائيٽون.sub_array (glyph_ڊيگهائي, بائيٽون.ڊيگهائي - glyph_ڊيگهائي);
  }

  فراغ سيٽ_overlaps_flag ()
  {
    إذا (unlikely (!سرو.عدد_جو_Contours)) أرجع;

    بنانشان flags_وٿي = ڊيگهائي (instructions_ڊيگهائي ());
    إذا (unlikely (flags_وٿي + 1 > بائيٽون.ڊيگهائي)) أرجع;

    HBUINT8 &پهريون_flag = (HBUINT8 &) Struct_At_وٿي<HBUINT16> (&بائيٽون, flags_وٿي);
    پهريون_flag = (uint8_t) پهريون_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساكن منطقي read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_نقطو_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *پڇاڙي,
			  منطقي extended)
  {
    خودڪار *points = points_.arrayZ;
    بنانشان ڳڻپ = points_.ڊيگهائي;
    لكل (بنانشان صحيح i = 0; i < ڳڻپ;)
    {
      إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;
      uint8_t flag = *p++;
      إذا (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;
	بنانشان صحيح repeat_ڳڻپ = *p++;
	بنانشان stop = hb_min (i + repeat_ڳڻپ, ڳڻپ);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    أرجع سچ;
  }

  ساكن منطقي read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_نقطو_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *پڇاڙي,
			   عائم contour_نقطو_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحيح v = 0;

    لكل (خودڪار &نقطو : points_)
    {
      بنانشان flag = نقطو.flag;
      إذا (flag & short_flag)
      {
	إذا (unlikely (p + 1 > پڇاڙي)) أرجع خطأ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      وإلا
      {
	إذا (!(flag & same_flag))
	{
	  إذا (unlikely (p + HBINT16::static_سائيز > پڇاڙي)) أرجع خطأ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_سائيز;
	}
      }
      نقطو.*m = v;
    }
    أرجع سچ;
  }

  منطقي get_contour_points (contour_نقطو_ويڪٽر_t &points /*ٻاهريون*/,
			   منطقي extended,
			   منطقي phantom_only = خطأ,
			   int64_t *budget = خالي_اشارو) ثابت
  {
    ثابت HBUINT16 *پڇاڙي_Pts_جو_Contours = &StructAfter<HBUINT16> (سرو);
    صحيح انگ_contours = سرو.عدد_جو_Contours;
    أكد (انگ_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    إذا (unlikely (!بائيٽون.check_range (&پڇاڙي_Pts_جو_Contours[انگ_contours]))) أرجع خطأ;
    بنانشان صحيح انگ_points = پڇاڙي_Pts_جو_Contours[انگ_contours - 1] + 1;
    إذا (unlikely (انگ_points < (بنانشان) انگ_contours)) أرجع خطأ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    إذا (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, انگ_points))) أرجع خطأ;

    بنانشان old_ڊيگهائي = points.ڊيگهائي;
    points.alloc (points.ڊيگهائي + انگ_points + 4); // Allocate for phantom points, to avoid a possible copy
    إذا (unlikely (!points.resize_dirty (points.ڊيگهائي + انگ_points))) أرجع خطأ;
    خودڪار points_ = points.as_array ().sub_array (old_ڊيگهائي);
    إذا (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_نقطو_t) * انگ_points);
    إذا (phantom_only) أرجع سچ;

    لكل (صحيح i = 0; i < انگ_contours; i++)
      points_[پڇاڙي_Pts_جو_Contours[i]].is_پڇاڙي_نقطو = سچ;

    /* Skip instructions */
    ثابت HBUINT8 *p = &Struct_At_وٿي<HBUINT8> (&پڇاڙي_Pts_جو_Contours[انگ_contours + 1],
						 پڇاڙي_Pts_جو_Contours[انگ_contours]);

    إذا (unlikely ((ثابت حرف *) p < بائيٽون.arrayZ)) أرجع خطأ; /* Unlikely overflow */
    ثابت HBUINT8 *پڇاڙي = (ثابت HBUINT8 *) (بائيٽون.arrayZ + بائيٽون.ڊيگهائي);
    إذا (unlikely (p >= پڇاڙي)) أرجع خطأ;

    /* Read x & y coordinates */
    أرجع read_flags (p, points_, پڇاڙي, extended)
        && read_points (p, points_, پڇاڙي, &contour_نقطو_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, پڇاڙي, &contour_نقطو_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ساكن فراغ encode_coord (صحيح value,
                            بنانشان &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_ويڪٽر_t<uint8_t> &coords /*ٻاهريون*/)
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

      coords.arrayZ[coords.ڊيگهائي++] = (uint8_t) value;
    }
    وإلا
    {
      int16_t val = value;
      coords.arrayZ[coords.ڊيگهائي++] = val >> 8;
      coords.arrayZ[coords.ڊيگهائي++] = val & 0xff;
    }
  }

  ساكن فراغ encode_flag (بنانشان flag,
                           بنانشان &repeat,
                           بنانشان lastflag,
                           hb_ويڪٽر_t<uint8_t> &flags /*ٻاهريون*/)
  {
    إذا (flag == lastflag && repeat != 255)
    {
      repeat++;
      إذا (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.ڊيگهائي++] = flag;
      }
      وإلا
      {
        بنانشان len = flags.ڊيگهائي;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    وإلا
    {
      repeat = 0;
      flags.arrayZ[flags.ڊيگهائي++] = flag;
    }
  }

  منطقي compile_بائيٽون_with_deltas (ثابت contour_نقطو_ويڪٽر_t &all_points,
                                  منطقي no_hinting,
                                  hb_بائيٽون_t &dest_بائيٽون /*ٻاهريون*/)
  {
    إذا (سرو.عدد_جو_Contours == 0 || all_points.ڊيگهائي <= 4)
    {
      dest_بائيٽون = hb_بائيٽون_t ();
      أرجع سچ;
    }
    بنانشان انگ_points = all_points.ڊيگهائي - 4;

    hb_ويڪٽر_t<uint8_t> flags, x_coords, y_coords;
    إذا (unlikely (!flags.alloc_exact (انگ_points))) أرجع خطأ;
    إذا (unlikely (!x_coords.alloc_exact (2*انگ_points))) أرجع خطأ;
    إذا (unlikely (!y_coords.alloc_exact (2*انگ_points))) أرجع خطأ;

    بنانشان lastflag = 255, repeat = 0;
    صحيح prev_x = 0, prev_y = 0;

    لكل (بنانشان i = 0; i < انگ_points; i++)
    {
      بنانشان flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_وڪر | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحيح cur_x = roundf (all_points.arrayZ[i].x);
      صحيح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    بنانشان len_before_instrs = 2 * سرو.عدد_جو_Contours + 2;
    بنانشان len_instrs = instructions_ڊيگهائي ();
    بنانشان ڪل_len = len_before_instrs + flags.ڊيگهائي + x_coords.ڊيگهائي + y_coords.ڊيگهائي;

    إذا (!no_hinting)
      ڪل_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (ڪل_len);
    إذا (unlikely (!p)) أرجع خطأ;

    ثابت حرف *src = بائيٽون.arrayZ + Glyph_سرو::static_سائيز;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    إذا (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.ڊيگهائي);
    cur += flags.ڊيگهائي;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.ڊيگهائي);
    cur += x_coords.ڊيگهائي;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.ڊيگهائي);

    dest_بائيٽون = hb_بائيٽون_t (p, ڪل_len);
    أرجع سچ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_SIMPLEGLYPH_HH */

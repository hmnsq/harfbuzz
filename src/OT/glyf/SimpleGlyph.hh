#اگر_غیر_تعریف_شدہ OT_GLYF_SIMPLEGLYPH_HH
#تعریف_کرو OT_GLYF_SIMPLEGLYPH_HH


#شامل "../../hb-open-type.hh"


نامخلا OT {
نامخلا glyf_impl {


ساخت SimpleGlyph
{
  شمار simple_glyph_flag_t
  {
    FLAG_ON_خم       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ثابت Glyph_سرخی &سرخی;
  hb_بائٹیں_t بائٹیں;
  SimpleGlyph (ثابت Glyph_سرخی &سرخی_2, hb_بائٹیں_t بائٹیں_2) :
    سرخی (سرخی_2), بائٹیں (بائٹیں_2) {}

  غیرعلامتی صحیح instruction_len_انحراف () ثابت
  { واپس Glyph_سرخی::static_سائز + 2 * سرخی.نمبر_کا_Contours; }

  غیرعلامتی صحیح لمبائی (غیرعلامتی صحیح instruction_len) ثابت
  { واپس instruction_len_انحراف () + 2 + instruction_len; }

  منطقی has_instructions_لمبائی () ثابت
  {
    واپس instruction_len_انحراف () + 2 <= بائٹیں.لمبائی;
  }

  غیرعلامتی صحیح instructions_لمبائی () ثابت
  {
    غیرعلامتی صحیح instruction_لمبائی_انحراف = instruction_len_انحراف ();
    اگر (unlikely (instruction_لمبائی_انحراف + 2 > بائٹیں.لمبائی)) واپس 0;

    ثابت HBUINT16 &instruction_لمبائی = Struct_At_انحراف<HBUINT16> (&بائٹیں, instruction_لمبائی_انحراف);
    /* Out of bounds of the current glyph */
    اگر (unlikely (لمبائی (instruction_لمبائی) > بائٹیں.لمبائی)) واپس 0;
    واپس instruction_لمبائی;
  }

  ثابت hb_بائٹیں_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) بائٹیں.arrayZ;
    ثابت uint8_t *glyph_انتہا = glyph + بائٹیں.لمبائی;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_انحراف ();

    اگر (unlikely (glyph + 2 >= glyph_انتہا)) واپس hb_بائٹیں_t ();
    غیرعلامتی صحیح عدد_coordinates = Struct_At_انحراف<HBUINT16> (glyph - 2, 0) + 1;
    غیرعلامتی صحیح عدد_instructions = Struct_At_انحراف<HBUINT16> (glyph, 0);

    glyph += 2 + عدد_instructions;

    غیرعلامتی صحیح coord_بائٹیں = 0;
    غیرعلامتی صحیح coords_with_flags = 0;
    جب (glyph < glyph_انتہا)
    {
      uint8_t flag = *glyph;
      glyph++;

      غیرعلامتی صحیح repeat = 1;
      اگر (flag & FLAG_REPEAT)
      {
	اگر (unlikely (glyph >= glyph_انتہا)) واپس hb_بائٹیں_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      غیرعلامتی صحیح x_بائٹیں, y_بائٹیں;
      x_بائٹیں = y_بائٹیں = 0;
      اگر (flag & FLAG_X_SHORT) x_بائٹیں = 1;
      ورنہ اگر ((flag & FLAG_X_SAME) == 0) x_بائٹیں = 2;

      اگر (flag & FLAG_Y_SHORT) y_بائٹیں = 1;
      ورنہ اگر ((flag & FLAG_Y_SAME) == 0) y_بائٹیں = 2;

      coord_بائٹیں += (x_بائٹیں + y_بائٹیں) * repeat;
      coords_with_flags += repeat;
      اگر (coords_with_flags >= عدد_coordinates) توڑو;
    }

    اگر (unlikely (coords_with_flags != عدد_coordinates)) واپس hb_بائٹیں_t ();
    واپس بائٹیں.sub_array (0, بائٹیں.لمبائی + coord_بائٹیں - (glyph_انتہا - glyph));
  }

  /* zero instruction length */
  خالی drop_hints ()
  {
    اگر (!has_instructions_لمبائی ()) واپس;
    Glyph_سرخی &glyph_سرخی = const_cast<Glyph_سرخی &> (سرخی);
    (HBUINT16 &) Struct_At_انحراف<HBUINT16> (&glyph_سرخی, instruction_len_انحراف ()) = 0;
  }

  خالی drop_hints_بائٹیں (hb_بائٹیں_t &dest_آغاز, hb_بائٹیں_t &dest_انتہا) ثابت
  {
    غیرعلامتی صحیح instructions_len = instructions_لمبائی ();
    غیرعلامتی صحیح glyph_لمبائی = لمبائی (instructions_len);
    dest_آغاز = بائٹیں.sub_array (0, glyph_لمبائی - instructions_len);
    dest_انتہا = بائٹیں.sub_array (glyph_لمبائی, بائٹیں.لمبائی - glyph_لمبائی);
  }

  خالی سیٹ_overlaps_flag ()
  {
    اگر (unlikely (!سرخی.نمبر_کا_Contours)) واپس;

    غیرعلامتی flags_انحراف = لمبائی (instructions_لمبائی ());
    اگر (unlikely (flags_انحراف + 1 > بائٹیں.لمبائی)) واپس;

    HBUINT8 &پہلا_flag = (HBUINT8 &) Struct_At_انحراف<HBUINT16> (&بائٹیں, flags_انحراف);
    پہلا_flag = (uint8_t) پہلا_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساکن منطقی read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_نقطہ_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *انتہا,
			  منطقی extended)
  {
    خودکار *points = points_.arrayZ;
    غیرعلامتی گنتی = points_.لمبائی;
    برائے (غیرعلامتی صحیح i = 0; i < گنتی;)
    {
      اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;
      uint8_t flag = *p++;
      اگر (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      اگر (flag & FLAG_REPEAT)
      {
	اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;
	غیرعلامتی صحیح repeat_گنتی = *p++;
	غیرعلامتی stop = hb_min (i + repeat_گنتی, گنتی);
	برائے (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    واپس سچ;
  }

  ساکن منطقی read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_نقطہ_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *انتہا,
			   اعشاری contour_نقطہ_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحیح v = 0;

    برائے (خودکار &نقطہ : points_)
    {
      غیرعلامتی flag = نقطہ.flag;
      اگر (flag & short_flag)
      {
	اگر (unlikely (p + 1 > انتہا)) واپس جھوٹ;
	v += (منطقی(flag & same_flag) * 2 - 1) * *p++;
      }
      ورنہ
      {
	اگر (!(flag & same_flag))
	{
	  اگر (unlikely (p + HBINT16::static_سائز > انتہا)) واپس جھوٹ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_سائز;
	}
      }
      نقطہ.*m = v;
    }
    واپس سچ;
  }

  منطقی get_contour_points (contour_نقطہ_سمتیہ_t &points /*برآمد*/,
			   منطقی extended,
			   منطقی phantom_only = جھوٹ,
			   int64_t *budget = خالی_اشاریہ) ثابت
  {
    ثابت HBUINT16 *انتہا_Pts_کا_Contours = &StructAfter<HBUINT16> (سرخی);
    صحیح عدد_contours = سرخی.نمبر_کا_Contours;
    أكد (عدد_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    اگر (unlikely (!بائٹیں.check_range (&انتہا_Pts_کا_Contours[عدد_contours]))) واپس جھوٹ;
    غیرعلامتی صحیح عدد_points = انتہا_Pts_کا_Contours[عدد_contours - 1] + 1;
    اگر (unlikely (عدد_points < (غیرعلامتی) عدد_contours)) واپس جھوٹ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    اگر (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, عدد_points))) واپس جھوٹ;

    غیرعلامتی old_لمبائی = points.لمبائی;
    points.alloc (points.لمبائی + عدد_points + 4); // Allocate for phantom points, to avoid a possible copy
    اگر (unlikely (!points.resize_dirty (points.لمبائی + عدد_points))) واپس جھوٹ;
    خودکار points_ = points.as_array ().sub_array (old_لمبائی);
    اگر (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_نقطہ_t) * عدد_points);
    اگر (phantom_only) واپس سچ;

    برائے (صحیح i = 0; i < عدد_contours; i++)
      points_[انتہا_Pts_کا_Contours[i]].is_انتہا_نقطہ = سچ;

    /* Skip instructions */
    ثابت HBUINT8 *p = &Struct_At_انحراف<HBUINT8> (&انتہا_Pts_کا_Contours[عدد_contours + 1],
						 انتہا_Pts_کا_Contours[عدد_contours]);

    اگر (unlikely ((ثابت حرف *) p < بائٹیں.arrayZ)) واپس جھوٹ; /* Unlikely overflow */
    ثابت HBUINT8 *انتہا = (ثابت HBUINT8 *) (بائٹیں.arrayZ + بائٹیں.لمبائی);
    اگر (unlikely (p >= انتہا)) واپس جھوٹ;

    /* Read x & y coordinates */
    واپس read_flags (p, points_, انتہا, extended)
        && read_points (p, points_, انتہا, &contour_نقطہ_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, انتہا, &contour_نقطہ_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ساکن خالی encode_coord (صحیح value,
                            غیرعلامتی &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_سمتیہ_t<uint8_t> &coords /*برآمد*/)
  {
    اگر (value == 0)
    {
      flag |= same_flag;
    }
    ورنہ اگر (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      اگر (value > 0) flag |= same_flag;
      ورنہ value = -value;

      coords.arrayZ[coords.لمبائی++] = (uint8_t) value;
    }
    ورنہ
    {
      int16_t val = value;
      coords.arrayZ[coords.لمبائی++] = val >> 8;
      coords.arrayZ[coords.لمبائی++] = val & 0xff;
    }
  }

  ساکن خالی encode_flag (غیرعلامتی flag,
                           غیرعلامتی &repeat,
                           غیرعلامتی lastflag,
                           hb_سمتیہ_t<uint8_t> &flags /*برآمد*/)
  {
    اگر (flag == lastflag && repeat != 255)
    {
      repeat++;
      اگر (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.لمبائی++] = flag;
      }
      ورنہ
      {
        غیرعلامتی len = flags.لمبائی;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    ورنہ
    {
      repeat = 0;
      flags.arrayZ[flags.لمبائی++] = flag;
    }
  }

  منطقی compile_بائٹیں_with_deltas (ثابت contour_نقطہ_سمتیہ_t &all_points,
                                  منطقی no_hinting,
                                  hb_بائٹیں_t &dest_بائٹیں /*برآمد*/)
  {
    اگر (سرخی.نمبر_کا_Contours == 0 || all_points.لمبائی <= 4)
    {
      dest_بائٹیں = hb_بائٹیں_t ();
      واپس سچ;
    }
    غیرعلامتی عدد_points = all_points.لمبائی - 4;

    hb_سمتیہ_t<uint8_t> flags, x_coords, y_coords;
    اگر (unlikely (!flags.alloc_exact (عدد_points))) واپس جھوٹ;
    اگر (unlikely (!x_coords.alloc_exact (2*عدد_points))) واپس جھوٹ;
    اگر (unlikely (!y_coords.alloc_exact (2*عدد_points))) واپس جھوٹ;

    غیرعلامتی lastflag = 255, repeat = 0;
    صحیح پچھلا_x = 0, پچھلا_y = 0;

    برائے (غیرعلامتی i = 0; i < عدد_points; i++)
    {
      غیرعلامتی flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_خم | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحیح cur_x = roundf (all_points.arrayZ[i].x);
      صحیح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - پچھلا_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - پچھلا_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      پچھلا_x = cur_x;
      پچھلا_y = cur_y;
      lastflag = flag;
    }

    غیرعلامتی len_before_instrs = 2 * سرخی.نمبر_کا_Contours + 2;
    غیرعلامتی len_instrs = instructions_لمبائی ();
    غیرعلامتی کل_len = len_before_instrs + flags.لمبائی + x_coords.لمبائی + y_coords.لمبائی;

    اگر (!no_hinting)
      کل_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (کل_len);
    اگر (unlikely (!p)) واپس جھوٹ;

    ثابت حرف *src = بائٹیں.arrayZ + Glyph_سرخی::static_سائز;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    اگر (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.لمبائی);
    cur += flags.لمبائی;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.لمبائی);
    cur += x_coords.لمبائی;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.لمبائی);

    dest_بائٹیں = hb_بائٹیں_t (p, کل_len);
    واپس سچ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_SIMPLEGLYPH_HH */

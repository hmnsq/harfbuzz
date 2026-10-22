#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#ضمن "../../hb-open-type.hh"


نطاق OT {
نطاق glyf_impl {


بنية SimpleGlyph
{
  تعداد simple_glyph_flag_t
  {
    FLAG_ON_CURVE       = 0x01,
    FLAG_X_SHORT        = 0x02,
    FLAG_Y_SHORT        = 0x04,
    FLAG_REPEAT         = 0x08,
    FLAG_X_SAME         = 0x10,
    FLAG_Y_SAME         = 0x20,
    FLAG_OVERLAP_SIMPLE = 0x40,
    FLAG_CUBIC          = 0x80
  };

  ثابت GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (ثابت GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned صحيح instruction_len_offset () ثابت
  { أرجع GlyphHeader::static_size + 2 * header.رقم_Of_Contours; }

  unsigned صحيح length (unsigned صحيح instruction_len) ثابت
  { أرجع instruction_len_offset () + 2 + instruction_len; }

  منطقي has_instructions_length () ثابت
  {
    أرجع instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned صحيح instructions_length () ثابت
  {
    unsigned صحيح instruction_length_offset = instruction_len_offset ();
    إذا (unlikely (instruction_length_offset + 2 > bytes.length)) أرجع 0;

    ثابت HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    إذا (unlikely (length (instructionLength) > bytes.length)) أرجع 0;
    أرجع instructionLength;
  }

  ثابت hb_bytes_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ثابت uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    إذا (unlikely (glyph + 2 >= glyph_end)) أرجع hb_bytes_t ();
    unsigned صحيح رقم_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned صحيح رقم_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + رقم_instructions;

    unsigned صحيح coord_bytes = 0;
    unsigned صحيح coords_with_flags = 0;
    طالما (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned صحيح repeat = 1;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (glyph >= glyph_end)) أرجع hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned صحيح xBytes, yBytes;
      xBytes = yBytes = 0;
      إذا (flag & FLAG_X_SHORT) xBytes = 1;
      وإلا إذا ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      إذا (flag & FLAG_Y_SHORT) yBytes = 1;
      وإلا إذا ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      إذا (coords_with_flags >= رقم_coordinates) اكسر;
    }

    إذا (unlikely (coords_with_flags != رقم_coordinates)) أرجع hb_bytes_t ();
    أرجع bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    إذا (!has_instructions_length ()) أرجع;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  فراغ drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ثابت
  {
    unsigned صحيح instructions_len = instructions_length ();
    unsigned صحيح glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  فراغ set_overlaps_flag ()
  {
    إذا (unlikely (!header.رقم_Of_Contours)) أرجع;

    unsigned flags_offset = length (instructions_length ());
    إذا (unlikely (flags_offset + 1 > bytes.length)) أرجع;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساكن منطقي read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *end,
			  منطقي extended)
  {
    auto *points = points_.arrayZ;
    unsigned عدد = points_.length;
    لكل (unsigned صحيح i = 0; i < عدد;)
    {
      إذا (unlikely (p + 1 > end)) أرجع خطأ;
      uint8_t flag = *p++;
      إذا (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      إذا (flag & FLAG_REPEAT)
      {
	إذا (unlikely (p + 1 > end)) أرجع خطأ;
	unsigned صحيح repeat_عدد = *p++;
	unsigned stop = hb_min (i + repeat_عدد, عدد);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    أرجع true;
  }

  ساكن منطقي read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *end,
			   عائم contour_point_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحيح v = 0;

    لكل (auto &point : points_)
    {
      unsigned flag = point.flag;
      إذا (flag & short_flag)
      {
	إذا (unlikely (p + 1 > end)) أرجع خطأ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      وإلا
      {
	إذا (!(flag & same_flag))
	{
	  إذا (unlikely (p + HBINT16::static_size > end)) أرجع خطأ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    أرجع true;
  }

  منطقي get_contour_points (contour_point_vector_t &points /* OUT */,
			   منطقي extended,
			   منطقي phantom_only = خطأ,
			   int64_t *budget = nullptr) ثابت
  {
    ثابت HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    صحيح رقم_contours = header.رقم_Of_Contours;
    أكد (رقم_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    إذا (unlikely (!bytes.check_range (&endPtsOfContours[رقم_contours]))) أرجع خطأ;
    unsigned صحيح رقم_points = endPtsOfContours[رقم_contours - 1] + 1;
    إذا (unlikely (رقم_points < (unsigned) رقم_contours)) أرجع خطأ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    إذا (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, رقم_points))) أرجع خطأ;

    unsigned old_length = points.length;
    points.alloc (points.length + رقم_points + 4); // Allocate for phantom points, to avoid a possible copy
    إذا (unlikely (!points.resize_dirty (points.length + رقم_points))) أرجع خطأ;
    auto points_ = points.as_array ().sub_array (old_length);
    إذا (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_point_t) * رقم_points);
    إذا (phantom_only) أرجع true;

    لكل (صحيح i = 0; i < رقم_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = true;

    /* Skip instructions */
    ثابت HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[رقم_contours + 1],
						 endPtsOfContours[رقم_contours]);

    إذا (unlikely ((ثابت حرف *) p < bytes.arrayZ)) أرجع خطأ; /* Unlikely overflow */
    ثابت HBUINT8 *end = (ثابت HBUINT8 *) (bytes.arrayZ + bytes.length);
    إذا (unlikely (p >= end)) أرجع خطأ;

    /* Read x & y coordinates */
    أرجع read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ساكن فراغ encode_coord (صحيح value,
                            unsigned &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
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

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    وإلا
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  ساكن فراغ encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    إذا (flag == lastflag && repeat != 255)
    {
      repeat++;
      إذا (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      وإلا
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    وإلا
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  منطقي compile_bytes_with_deltas (ثابت contour_point_vector_t &all_points,
                                  منطقي no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    إذا (header.رقم_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      أرجع true;
    }
    unsigned رقم_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    إذا (unlikely (!flags.alloc_exact (رقم_points))) أرجع خطأ;
    إذا (unlikely (!x_coords.alloc_exact (2*رقم_points))) أرجع خطأ;
    إذا (unlikely (!y_coords.alloc_exact (2*رقم_points))) أرجع خطأ;

    unsigned lastflag = 255, repeat = 0;
    صحيح prev_x = 0, prev_y = 0;

    لكل (unsigned i = 0; i < رقم_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحيح cur_x = roundf (all_points.arrayZ[i].x);
      صحيح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.رقم_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    إذا (!no_hinting)
      total_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (total_len);
    إذا (unlikely (!p)) أرجع خطأ;

    ثابت حرف *src = bytes.arrayZ + GlyphHeader::static_size;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    إذا (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.length);
    cur += flags.length;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.length);
    cur += x_coords.length;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.length);

    dest_bytes = hb_bytes_t (p, total_len);
    أرجع true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

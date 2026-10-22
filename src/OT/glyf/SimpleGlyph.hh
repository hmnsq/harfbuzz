#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#شامل "../../hb-open-type.hh"


فضاینام OT {
فضاینام glyf_impl {


ساختار SimpleGlyph
{
  شمارش simple_glyph_flag_t
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

  unsigned صحیح instruction_len_offset () ثابت
  { بازگشت GlyphHeader::static_size + 2 * header.عدد_Of_Contours; }

  unsigned صحیح length (unsigned صحیح instruction_len) ثابت
  { بازگشت instruction_len_offset () + 2 + instruction_len; }

  منطقی has_instructions_length () ثابت
  {
    بازگشت instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned صحیح instructions_length () ثابت
  {
    unsigned صحیح instruction_length_offset = instruction_len_offset ();
    اگر (unlikely (instruction_length_offset + 2 > bytes.length)) بازگشت 0;

    ثابت HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    اگر (unlikely (length (instructionLength) > bytes.length)) بازگشت 0;
    بازگشت instructionLength;
  }

  ثابت hb_bytes_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ثابت uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    اگر (unlikely (glyph + 2 >= glyph_end)) بازگشت hb_bytes_t ();
    unsigned صحیح رقم_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned صحیح رقم_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + رقم_instructions;

    unsigned صحیح coord_bytes = 0;
    unsigned صحیح coords_with_flags = 0;
    وقتی (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned صحیح repeat = 1;
      اگر (flag & FLAG_REPEAT)
      {
	اگر (unlikely (glyph >= glyph_end)) بازگشت hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned صحیح xBytes, yBytes;
      xBytes = yBytes = 0;
      اگر (flag & FLAG_X_SHORT) xBytes = 1;
      وگرنه اگر ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      اگر (flag & FLAG_Y_SHORT) yBytes = 1;
      وگرنه اگر ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      اگر (coords_with_flags >= رقم_coordinates) بشکن;
    }

    اگر (unlikely (coords_with_flags != رقم_coordinates)) بازگشت hb_bytes_t ();
    بازگشت bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  تهی drop_hints ()
  {
    اگر (!has_instructions_length ()) بازگشت;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  تهی drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ثابت
  {
    unsigned صحیح instructions_len = instructions_length ();
    unsigned صحیح glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  تهی set_overlaps_flag ()
  {
    اگر (unlikely (!header.عدد_Of_Contours)) بازگشت;

    unsigned flags_offset = length (instructions_length ());
    اگر (unlikely (flags_offset + 1 > bytes.length)) بازگشت;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  ایستا منطقی read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *end,
			  منطقی extended)
  {
    auto *points = points_.arrayZ;
    unsigned شمار = points_.length;
    برای (unsigned صحیح i = 0; i < شمار;)
    {
      اگر (unlikely (p + 1 > end)) بازگشت نادرست;
      uint8_t flag = *p++;
      اگر (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      اگر (flag & FLAG_REPEAT)
      {
	اگر (unlikely (p + 1 > end)) بازگشت نادرست;
	unsigned صحیح repeat_شمار = *p++;
	unsigned stop = hb_min (i + repeat_شمار, شمار);
	برای (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    بازگشت درست;
  }

  ایستا منطقی read_points (ثابت HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ثابت HBUINT8 *end,
			   اعشاری contour_point_t::*m,
			   ثابت simple_glyph_flag_t short_flag,
			   ثابت simple_glyph_flag_t same_flag)
  {
    صحیح v = 0;

    برای (auto &point : points_)
    {
      unsigned flag = point.flag;
      اگر (flag & short_flag)
      {
	اگر (unlikely (p + 1 > end)) بازگشت نادرست;
	v += (منطقی(flag & same_flag) * 2 - 1) * *p++;
      }
      وگرنه
      {
	اگر (!(flag & same_flag))
	{
	  اگر (unlikely (p + HBINT16::static_size > end)) بازگشت نادرست;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    بازگشت درست;
  }

  منطقی get_contour_points (contour_point_vector_t &points /* OUT */,
			   منطقی extended,
			   منطقی phantom_only = نادرست,
			   int64_t *budget = nullptr) ثابت
  {
    ثابت HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    صحیح رقم_contours = header.عدد_Of_Contours;
    أكد (رقم_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    اگر (unlikely (!bytes.check_range (&endPtsOfContours[رقم_contours]))) بازگشت نادرست;
    unsigned صحیح رقم_points = endPtsOfContours[رقم_contours - 1] + 1;
    اگر (unlikely (رقم_points < (unsigned) رقم_contours)) بازگشت نادرست;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    اگر (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, رقم_points))) بازگشت نادرست;

    unsigned old_length = points.length;
    points.alloc (points.length + رقم_points + 4); // Allocate for phantom points, to avoid a possible copy
    اگر (unlikely (!points.resize_dirty (points.length + رقم_points))) بازگشت نادرست;
    auto points_ = points.as_array ().sub_array (old_length);
    اگر (!phantom_only)
      hb_memset (points_.arrayZ, 0, اندازه (contour_point_t) * رقم_points);
    اگر (phantom_only) بازگشت درست;

    برای (صحیح i = 0; i < رقم_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = درست;

    /* Skip instructions */
    ثابت HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[رقم_contours + 1],
						 endPtsOfContours[رقم_contours]);

    اگر (unlikely ((ثابت نویسه *) p < bytes.arrayZ)) بازگشت نادرست; /* Unlikely overflow */
    ثابت HBUINT8 *end = (ثابت HBUINT8 *) (bytes.arrayZ + bytes.length);
    اگر (unlikely (p >= end)) بازگشت نادرست;

    /* Read x & y coordinates */
    بازگشت read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ایستا تهی encode_coord (صحیح value,
                            unsigned &flag,
                            ثابت simple_glyph_flag_t short_flag,
                            ثابت simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    اگر (value == 0)
    {
      flag |= same_flag;
    }
    وگرنه اگر (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      اگر (value > 0) flag |= same_flag;
      وگرنه value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    وگرنه
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  ایستا تهی encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    اگر (flag == lastflag && repeat != 255)
    {
      repeat++;
      اگر (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      وگرنه
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    وگرنه
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  منطقی compile_bytes_with_deltas (ثابت contour_point_vector_t &all_points,
                                  منطقی no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    اگر (header.عدد_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      بازگشت درست;
    }
    unsigned رقم_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    اگر (unlikely (!flags.alloc_exact (رقم_points))) بازگشت نادرست;
    اگر (unlikely (!x_coords.alloc_exact (2*رقم_points))) بازگشت نادرست;
    اگر (unlikely (!y_coords.alloc_exact (2*رقم_points))) بازگشت نادرست;

    unsigned lastflag = 255, repeat = 0;
    صحیح prev_x = 0, prev_y = 0;

    برای (unsigned i = 0; i < رقم_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      صحیح cur_x = roundf (all_points.arrayZ[i].x);
      صحیح cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.عدد_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    اگر (!no_hinting)
      total_len += len_instrs;

    نویسه *p = (نویسه *) hb_malloc (total_len);
    اگر (unlikely (!p)) بازگشت نادرست;

    ثابت نویسه *src = bytes.arrayZ + GlyphHeader::static_size;
    نویسه *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    اگر (!no_hinting)
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
    بازگشت درست;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#अन्तर्भाव "../../hb-open-type.hh"


नामाकाश OT {
नामाकाश glyf_impl {


संरचना SimpleGlyph
{
  गणना simple_glyph_flag_t
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

  स्थिरांक GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (स्थिरांक GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  अचिन्हित पूर्णांक instruction_len_offset () स्थिरांक
  { परत GlyphHeader::static_size + 2 * header.संख्या_Of_Contours; }

  अचिन्हित पूर्णांक length (अचिन्हित पूर्णांक instruction_len) स्थिरांक
  { परत instruction_len_offset () + 2 + instruction_len; }

  तार्किक has_instructions_length () स्थिरांक
  {
    परत instruction_len_offset () + 2 <= bytes.length;
  }

  अचिन्हित पूर्णांक instructions_length () स्थिरांक
  {
    अचिन्हित पूर्णांक instruction_length_offset = instruction_len_offset ();
    जर (unlikely (instruction_length_offset + 2 > bytes.length)) परत 0;

    स्थिरांक HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    जर (unlikely (length (instructionLength) > bytes.length)) परत 0;
    परत instructionLength;
  }

  स्थिरांक hb_bytes_t trim_padding () स्थिरांक
  {
    /* based on FontTools _g_l_y_f.py::trim */
    स्थिरांक uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    स्थिरांक uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    जर (unlikely (glyph + 2 >= glyph_end)) परत hb_bytes_t ();
    अचिन्हित पूर्णांक अंक_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    अचिन्हित पूर्णांक अंक_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + अंक_instructions;

    अचिन्हित पूर्णांक coord_bytes = 0;
    अचिन्हित पूर्णांक coords_with_flags = 0;
    पर्यंत (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      अचिन्हित पूर्णांक repeat = 1;
      जर (flag & FLAG_REPEAT)
      {
	जर (unlikely (glyph >= glyph_end)) परत hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      अचिन्हित पूर्णांक xBytes, yBytes;
      xBytes = yBytes = 0;
      जर (flag & FLAG_X_SHORT) xBytes = 1;
      अन्यथा जर ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      जर (flag & FLAG_Y_SHORT) yBytes = 1;
      अन्यथा जर ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      जर (coords_with_flags >= अंक_coordinates) थांबा;
    }

    जर (unlikely (coords_with_flags != अंक_coordinates)) परत hb_bytes_t ();
    परत bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  शून्य drop_hints ()
  {
    जर (!has_instructions_length ()) परत;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  शून्य drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) स्थिरांक
  {
    अचिन्हित पूर्णांक instructions_len = instructions_length ();
    अचिन्हित पूर्णांक glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  शून्य set_overlaps_flag ()
  {
    जर (unlikely (!header.संख्या_Of_Contours)) परत;

    अचिन्हित flags_offset = length (instructions_length ());
    जर (unlikely (flags_offset + 1 > bytes.length)) परत;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  स्थिर तार्किक read_flags (स्थिरांक HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  स्थिरांक HBUINT8 *end,
			  तार्किक extended)
  {
    स्वतः *points = points_.arrayZ;
    अचिन्हित मोजणी = points_.length;
    साठी (अचिन्हित पूर्णांक i = 0; i < मोजणी;)
    {
      जर (unlikely (p + 1 > end)) परत असत्य;
      uint8_t flag = *p++;
      जर (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      जर (flag & FLAG_REPEAT)
      {
	जर (unlikely (p + 1 > end)) परत असत्य;
	अचिन्हित पूर्णांक repeat_मोजणी = *p++;
	अचिन्हित stop = hb_min (i + repeat_मोजणी, मोजणी);
	साठी (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    परत सत्य;
  }

  स्थिर तार्किक read_points (स्थिरांक HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   स्थिरांक HBUINT8 *end,
			   दशांश contour_point_t::*m,
			   स्थिरांक simple_glyph_flag_t short_flag,
			   स्थिरांक simple_glyph_flag_t same_flag)
  {
    पूर्णांक v = 0;

    साठी (स्वतः &point : points_)
    {
      अचिन्हित flag = point.flag;
      जर (flag & short_flag)
      {
	जर (unlikely (p + 1 > end)) परत असत्य;
	v += (तार्किक(flag & same_flag) * 2 - 1) * *p++;
      }
      अन्यथा
      {
	जर (!(flag & same_flag))
	{
	  जर (unlikely (p + HBINT16::static_size > end)) परत असत्य;
	  v += *(स्थिरांक HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    परत सत्य;
  }

  तार्किक get_contour_points (contour_point_vector_t &points /* OUT */,
			   तार्किक extended,
			   तार्किक phantom_only = असत्य,
			   int64_t *budget = nullptr) स्थिरांक
  {
    स्थिरांक HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    पूर्णांक अंक_contours = header.संख्या_Of_Contours;
    प्रतिज्ञा (अंक_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    जर (unlikely (!bytes.check_range (&endPtsOfContours[अंक_contours]))) परत असत्य;
    अचिन्हित पूर्णांक अंक_points = endPtsOfContours[अंक_contours - 1] + 1;
    जर (unlikely (अंक_points < (अचिन्हित) अंक_contours)) परत असत्य;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    जर (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, अंक_points))) परत असत्य;

    अचिन्हित old_length = points.length;
    points.alloc (points.length + अंक_points + 4); // Allocate for phantom points, to avoid a possible copy
    जर (unlikely (!points.resize_dirty (points.length + अंक_points))) परत असत्य;
    स्वतः points_ = points.as_array ().sub_array (old_length);
    जर (!phantom_only)
      hb_memset (points_.arrayZ, 0, आकार (contour_point_t) * अंक_points);
    जर (phantom_only) परत सत्य;

    साठी (पूर्णांक i = 0; i < अंक_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = सत्य;

    /* Skip instructions */
    स्थिरांक HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[अंक_contours + 1],
						 endPtsOfContours[अंक_contours]);

    जर (unlikely ((स्थिरांक अक्षर *) p < bytes.arrayZ)) परत असत्य; /* Unlikely overflow */
    स्थिरांक HBUINT8 *end = (स्थिरांक HBUINT8 *) (bytes.arrayZ + bytes.length);
    जर (unlikely (p >= end)) परत असत्य;

    /* Read x & y coordinates */
    परत read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  स्थिर शून्य encode_coord (पूर्णांक value,
                            अचिन्हित &flag,
                            स्थिरांक simple_glyph_flag_t short_flag,
                            स्थिरांक simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    जर (value == 0)
    {
      flag |= same_flag;
    }
    अन्यथा जर (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      जर (value > 0) flag |= same_flag;
      अन्यथा value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    अन्यथा
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  स्थिर शून्य encode_flag (अचिन्हित flag,
                           अचिन्हित &repeat,
                           अचिन्हित lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    जर (flag == lastflag && repeat != 255)
    {
      repeat++;
      जर (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      अन्यथा
      {
        अचिन्हित len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    अन्यथा
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  तार्किक compile_bytes_with_deltas (स्थिरांक contour_point_vector_t &all_points,
                                  तार्किक no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    जर (header.संख्या_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      परत सत्य;
    }
    अचिन्हित अंक_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    जर (unlikely (!flags.alloc_exact (अंक_points))) परत असत्य;
    जर (unlikely (!x_coords.alloc_exact (2*अंक_points))) परत असत्य;
    जर (unlikely (!y_coords.alloc_exact (2*अंक_points))) परत असत्य;

    अचिन्हित lastflag = 255, repeat = 0;
    पूर्णांक prev_x = 0, prev_y = 0;

    साठी (अचिन्हित i = 0; i < अंक_points; i++)
    {
      अचिन्हित flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      पूर्णांक cur_x = roundf (all_points.arrayZ[i].x);
      पूर्णांक cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    अचिन्हित len_before_instrs = 2 * header.संख्या_Of_Contours + 2;
    अचिन्हित len_instrs = instructions_length ();
    अचिन्हित total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    जर (!no_hinting)
      total_len += len_instrs;

    अक्षर *p = (अक्षर *) hb_malloc (total_len);
    जर (unlikely (!p)) परत असत्य;

    स्थिरांक अक्षर *src = bytes.arrayZ + GlyphHeader::static_size;
    अक्षर *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    जर (!no_hinting)
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
    परत सत्य;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

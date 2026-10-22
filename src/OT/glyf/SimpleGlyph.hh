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

  ध्रुव GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (ध्रुव GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned पूर्णाङ्क instruction_len_offset () ध्रुव
  { प्रत्यागम GlyphHeader::static_size + 2 * header.संख्या_Of_Contours; }

  unsigned पूर्णाङ्क length (unsigned पूर्णाङ्क instruction_len) ध्रुव
  { प्रत्यागम instruction_len_offset () + 2 + instruction_len; }

  तार्किक has_instructions_length () ध्रुव
  {
    प्रत्यागम instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned पूर्णाङ्क instructions_length () ध्रुव
  {
    unsigned पूर्णाङ्क instruction_length_offset = instruction_len_offset ();
    यदि (unlikely (instruction_length_offset + 2 > bytes.length)) प्रत्यागम 0;

    ध्रुव HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    यदि (unlikely (length (instructionLength) > bytes.length)) प्रत्यागम 0;
    प्रत्यागम instructionLength;
  }

  ध्रुव hb_bytes_t trim_padding () ध्रुव
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ध्रुव uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ध्रुव uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    यदि (unlikely (glyph + 2 >= glyph_end)) प्रत्यागम hb_bytes_t ();
    unsigned पूर्णाङ्क अंक_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned पूर्णाङ्क अंक_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + अंक_instructions;

    unsigned पूर्णाङ्क coord_bytes = 0;
    unsigned पूर्णाङ्क coords_with_flags = 0;
    यावत् (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned पूर्णाङ्क repeat = 1;
      यदि (flag & FLAG_REPEAT)
      {
	यदि (unlikely (glyph >= glyph_end)) प्रत्यागम hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned पूर्णाङ्क xBytes, yBytes;
      xBytes = yBytes = 0;
      यदि (flag & FLAG_X_SHORT) xBytes = 1;
      अन्यथा यदि ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      यदि (flag & FLAG_Y_SHORT) yBytes = 1;
      अन्यथा यदि ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      यदि (coords_with_flags >= अंक_coordinates) भङ्ग;
    }

    यदि (unlikely (coords_with_flags != अंक_coordinates)) प्रत्यागम hb_bytes_t ();
    प्रत्यागम bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  शून्य drop_hints ()
  {
    यदि (!has_instructions_length ()) प्रत्यागम;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  शून्य drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ध्रुव
  {
    unsigned पूर्णाङ्क instructions_len = instructions_length ();
    unsigned पूर्णाङ्क glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  शून्य set_overlaps_flag ()
  {
    यदि (unlikely (!header.संख्या_Of_Contours)) प्रत्यागम;

    unsigned flags_offset = length (instructions_length ());
    यदि (unlikely (flags_offset + 1 > bytes.length)) प्रत्यागम;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  स्थिर तार्किक read_flags (ध्रुव HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ध्रुव HBUINT8 *end,
			  तार्किक extended)
  {
    auto *points = points_.arrayZ;
    unsigned मोजणी = points_.length;
    प्रति (unsigned पूर्णाङ्क i = 0; i < मोजणी;)
    {
      यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;
      uint8_t flag = *p++;
      यदि (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      यदि (flag & FLAG_REPEAT)
      {
	यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;
	unsigned पूर्णाङ्क repeat_मोजणी = *p++;
	unsigned stop = hb_min (i + repeat_मोजणी, मोजणी);
	प्रति (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    प्रत्यागम सत्य;
  }

  स्थिर तार्किक read_points (ध्रुव HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ध्रुव HBUINT8 *end,
			   भिन्न contour_point_t::*m,
			   ध्रुव simple_glyph_flag_t short_flag,
			   ध्रुव simple_glyph_flag_t same_flag)
  {
    पूर्णाङ्क v = 0;

    प्रति (auto &point : points_)
    {
      unsigned flag = point.flag;
      यदि (flag & short_flag)
      {
	यदि (unlikely (p + 1 > end)) प्रत्यागम असत्य;
	v += (तार्किक(flag & same_flag) * 2 - 1) * *p++;
      }
      अन्यथा
      {
	यदि (!(flag & same_flag))
	{
	  यदि (unlikely (p + HBINT16::static_size > end)) प्रत्यागम असत्य;
	  v += *(ध्रुव HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    प्रत्यागम सत्य;
  }

  तार्किक get_contour_points (contour_point_vector_t &points /* OUT */,
			   तार्किक extended,
			   तार्किक phantom_only = असत्य,
			   int64_t *budget = nullptr) ध्रुव
  {
    ध्रुव HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    पूर्णाङ्क अंक_contours = header.संख्या_Of_Contours;
    प्रतिज्ञा (अंक_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    यदि (unlikely (!bytes.check_range (&endPtsOfContours[अंक_contours]))) प्रत्यागम असत्य;
    unsigned पूर्णाङ्क अंक_points = endPtsOfContours[अंक_contours - 1] + 1;
    यदि (unlikely (अंक_points < (unsigned) अंक_contours)) प्रत्यागम असत्य;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    यदि (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, अंक_points))) प्रत्यागम असत्य;

    unsigned old_length = points.length;
    points.alloc (points.length + अंक_points + 4); // Allocate for phantom points, to avoid a possible copy
    यदि (unlikely (!points.resize_dirty (points.length + अंक_points))) प्रत्यागम असत्य;
    auto points_ = points.as_array ().sub_array (old_length);
    यदि (!phantom_only)
      hb_memset (points_.arrayZ, 0, परिमाण (contour_point_t) * अंक_points);
    यदि (phantom_only) प्रत्यागम सत्य;

    प्रति (पूर्णाङ्क i = 0; i < अंक_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = सत्य;

    /* Skip instructions */
    ध्रुव HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[अंक_contours + 1],
						 endPtsOfContours[अंक_contours]);

    यदि (unlikely ((ध्रुव वर्ण *) p < bytes.arrayZ)) प्रत्यागम असत्य; /* Unlikely overflow */
    ध्रुव HBUINT8 *end = (ध्रुव HBUINT8 *) (bytes.arrayZ + bytes.length);
    यदि (unlikely (p >= end)) प्रत्यागम असत्य;

    /* Read x & y coordinates */
    प्रत्यागम read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  स्थिर शून्य encode_coord (पूर्णाङ्क value,
                            unsigned &flag,
                            ध्रुव simple_glyph_flag_t short_flag,
                            ध्रुव simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    यदि (value == 0)
    {
      flag |= same_flag;
    }
    अन्यथा यदि (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      यदि (value > 0) flag |= same_flag;
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

  स्थिर शून्य encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    यदि (flag == lastflag && repeat != 255)
    {
      repeat++;
      यदि (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      अन्यथा
      {
        unsigned len = flags.length;
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

  तार्किक compile_bytes_with_deltas (ध्रुव contour_point_vector_t &all_points,
                                  तार्किक no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    यदि (header.संख्या_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      प्रत्यागम सत्य;
    }
    unsigned अंक_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    यदि (unlikely (!flags.alloc_exact (अंक_points))) प्रत्यागम असत्य;
    यदि (unlikely (!x_coords.alloc_exact (2*अंक_points))) प्रत्यागम असत्य;
    यदि (unlikely (!y_coords.alloc_exact (2*अंक_points))) प्रत्यागम असत्य;

    unsigned lastflag = 255, repeat = 0;
    पूर्णाङ्क prev_x = 0, prev_y = 0;

    प्रति (unsigned i = 0; i < अंक_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      पूर्णाङ्क cur_x = roundf (all_points.arrayZ[i].x);
      पूर्णाङ्क cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.संख्या_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    यदि (!no_hinting)
      total_len += len_instrs;

    वर्ण *p = (वर्ण *) hb_malloc (total_len);
    यदि (unlikely (!p)) प्रत्यागम असत्य;

    ध्रुव वर्ण *src = bytes.arrayZ + GlyphHeader::static_size;
    वर्ण *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    यदि (!no_hinting)
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
    प्रत्यागम सत्य;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

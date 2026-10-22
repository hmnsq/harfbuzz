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
  { पटिनिवत्त GlyphHeader::static_size + 2 * header.सङ्ख्या_Of_Contours; }

  unsigned पूर्णाङ्क length (unsigned पूर्णाङ्क instruction_len) ध्रुव
  { पटिनिवत्त instruction_len_offset () + 2 + instruction_len; }

  तार्किक has_instructions_length () ध्रुव
  {
    पटिनिवत्त instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned पूर्णाङ्क instructions_length () ध्रुव
  {
    unsigned पूर्णाङ्क instruction_length_offset = instruction_len_offset ();
    सचे (unlikely (instruction_length_offset + 2 > bytes.length)) पटिनिवत्त 0;

    ध्रुव HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    सचे (unlikely (length (instructionLength) > bytes.length)) पटिनिवत्त 0;
    पटिनिवत्त instructionLength;
  }

  ध्रुव hb_bytes_t trim_padding () ध्रुव
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ध्रुव uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ध्रुव uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    सचे (unlikely (glyph + 2 >= glyph_end)) पटिनिवत्त hb_bytes_t ();
    unsigned पूर्णाङ्क अङ्कः_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned पूर्णाङ्क अङ्कः_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + अङ्कः_instructions;

    unsigned पूर्णाङ्क coord_bytes = 0;
    unsigned पूर्णाङ्क coords_with_flags = 0;
    याव (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned पूर्णाङ्क repeat = 1;
      सचे (flag & FLAG_REPEAT)
      {
	सचे (unlikely (glyph >= glyph_end)) पटिनिवत्त hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned पूर्णाङ्क xBytes, yBytes;
      xBytes = yBytes = 0;
      सचे (flag & FLAG_X_SHORT) xBytes = 1;
      अञ्ञथा सचे ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      सचे (flag & FLAG_Y_SHORT) yBytes = 1;
      अञ्ञथा सचे ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      सचे (coords_with_flags >= अङ्कः_coordinates) भङ्ग;
    }

    सचे (unlikely (coords_with_flags != अङ्कः_coordinates)) पटिनिवत्त hb_bytes_t ();
    पटिनिवत्त bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  शून्य drop_hints ()
  {
    सचे (!has_instructions_length ()) पटिनिवत्त;
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
    सचे (unlikely (!header.सङ्ख्या_Of_Contours)) पटिनिवत्त;

    unsigned flags_offset = length (instructions_length ());
    सचे (unlikely (flags_offset + 1 > bytes.length)) पटिनिवत्त;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  स्थिर तार्किक read_flags (ध्रुव HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ध्रुव HBUINT8 *end,
			  तार्किक extended)
  {
    auto *points = points_.arrayZ;
    unsigned गणनम् = points_.length;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्;)
    {
      सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;
      uint8_t flag = *p++;
      सचे (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      सचे (flag & FLAG_REPEAT)
      {
	सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;
	unsigned पूर्णाङ्क repeat_गणनम् = *p++;
	unsigned stop = hb_min (i + repeat_गणनम्, गणनम्);
	अत्थाय (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    पटिनिवत्त सच्च;
  }

  स्थिर तार्किक read_points (ध्रुव HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ध्रुव HBUINT8 *end,
			   भिन्न contour_point_t::*m,
			   ध्रुव simple_glyph_flag_t short_flag,
			   ध्रुव simple_glyph_flag_t same_flag)
  {
    पूर्णाङ्क v = 0;

    अत्थाय (auto &point : points_)
    {
      unsigned flag = point.flag;
      सचे (flag & short_flag)
      {
	सचे (unlikely (p + 1 > end)) पटिनिवत्त मिच्छा;
	v += (तार्किक(flag & same_flag) * 2 - 1) * *p++;
      }
      अञ्ञथा
      {
	सचे (!(flag & same_flag))
	{
	  सचे (unlikely (p + HBINT16::static_size > end)) पटिनिवत्त मिच्छा;
	  v += *(ध्रुव HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    पटिनिवत्त सच्च;
  }

  तार्किक get_contour_points (contour_point_vector_t &points /* OUT */,
			   तार्किक extended,
			   तार्किक phantom_only = मिच्छा,
			   int64_t *budget = nullptr) ध्रुव
  {
    ध्रुव HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    पूर्णाङ्क अङ्कः_contours = header.सङ्ख्या_Of_Contours;
    प्रतिज्ञा (अङ्कः_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    सचे (unlikely (!bytes.check_range (&endPtsOfContours[अङ्कः_contours]))) पटिनिवत्त मिच्छा;
    unsigned पूर्णाङ्क अङ्कः_points = endPtsOfContours[अङ्कः_contours - 1] + 1;
    सचे (unlikely (अङ्कः_points < (unsigned) अङ्कः_contours)) पटिनिवत्त मिच्छा;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    सचे (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, अङ्कः_points))) पटिनिवत्त मिच्छा;

    unsigned old_length = points.length;
    points.alloc (points.length + अङ्कः_points + 4); // Allocate for phantom points, to avoid a possible copy
    सचे (unlikely (!points.resize_dirty (points.length + अङ्कः_points))) पटिनिवत्त मिच्छा;
    auto points_ = points.as_array ().sub_array (old_length);
    सचे (!phantom_only)
      hb_memset (points_.arrayZ, 0, परिमाण (contour_point_t) * अङ्कः_points);
    सचे (phantom_only) पटिनिवत्त सच्च;

    अत्थाय (पूर्णाङ्क i = 0; i < अङ्कः_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = सच्च;

    /* Skip instructions */
    ध्रुव HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[अङ्कः_contours + 1],
						 endPtsOfContours[अङ्कः_contours]);

    सचे (unlikely ((ध्रुव वर्ण *) p < bytes.arrayZ)) पटिनिवत्त मिच्छा; /* Unlikely overflow */
    ध्रुव HBUINT8 *end = (ध्रुव HBUINT8 *) (bytes.arrayZ + bytes.length);
    सचे (unlikely (p >= end)) पटिनिवत्त मिच्छा;

    /* Read x & y coordinates */
    पटिनिवत्त read_flags (p, points_, end, extended)
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
    सचे (value == 0)
    {
      flag |= same_flag;
    }
    अञ्ञथा सचे (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      सचे (value > 0) flag |= same_flag;
      अञ्ञथा value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    अञ्ञथा
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
    सचे (flag == lastflag && repeat != 255)
    {
      repeat++;
      सचे (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      अञ्ञथा
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    अञ्ञथा
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  तार्किक compile_bytes_with_deltas (ध्रुव contour_point_vector_t &all_points,
                                  तार्किक no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    सचे (header.सङ्ख्या_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      पटिनिवत्त सच्च;
    }
    unsigned अङ्कः_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    सचे (unlikely (!flags.alloc_exact (अङ्कः_points))) पटिनिवत्त मिच्छा;
    सचे (unlikely (!x_coords.alloc_exact (2*अङ्कः_points))) पटिनिवत्त मिच्छा;
    सचे (unlikely (!y_coords.alloc_exact (2*अङ्कः_points))) पटिनिवत्त मिच्छा;

    unsigned lastflag = 255, repeat = 0;
    पूर्णाङ्क prev_x = 0, prev_y = 0;

    अत्थाय (unsigned i = 0; i < अङ्कः_points; i++)
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

    unsigned len_before_instrs = 2 * header.सङ्ख्या_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    सचे (!no_hinting)
      total_len += len_instrs;

    वर्ण *p = (वर्ण *) hb_malloc (total_len);
    सचे (unlikely (!p)) पटिनिवत्त मिच्छा;

    ध्रुव वर्ण *src = bytes.arrayZ + GlyphHeader::static_size;
    वर्ण *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    सचे (!no_hinting)
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
    पटिनिवत्त सच्च;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

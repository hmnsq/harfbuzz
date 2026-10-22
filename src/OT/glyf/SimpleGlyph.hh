#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"


ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ glyf_impl {


ಸಂರಚನಾ SimpleGlyph
{
  ಗಣನಾ simple_glyph_flag_t
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

  ಧ್ರುವ GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (ಧ್ರುವ GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned ಪೂರ್ಣಾಙ್ಕ instruction_len_offset () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ GlyphHeader::static_size + 2 * header.ಸಂಖ್ಯೆ_Of_Contours; }

  unsigned ಪೂರ್ಣಾಙ್ಕ length (unsigned ಪೂರ್ಣಾಙ್ಕ instruction_len) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ instruction_len_offset () + 2 + instruction_len; }

  ತಾರ್ಕಿಕ has_instructions_length () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned ಪೂರ್ಣಾಙ್ಕ instructions_length () ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ instruction_length_offset = instruction_len_offset ();
    ಯದಿ (unlikely (instruction_length_offset + 2 > bytes.length)) ಪ್ರತ್ಯಾಗಮ 0;

    ಧ್ರುವ HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    ಯದಿ (unlikely (length (instructionLength) > bytes.length)) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ instructionLength;
  }

  ಧ್ರುವ hb_bytes_t trim_padding () ಧ್ರುವ
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ಧ್ರುವ uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ಧ್ರುವ uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    ಯದಿ (unlikely (glyph + 2 >= glyph_end)) ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + ಅಂಕೆ_instructions;

    unsigned ಪೂರ್ಣಾಙ್ಕ coord_bytes = 0;
    unsigned ಪೂರ್ಣಾಙ್ಕ coords_with_flags = 0;
    ಯಾವತ್ (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned ಪೂರ್ಣಾಙ್ಕ repeat = 1;
      ಯದಿ (flag & FLAG_REPEAT)
      {
	ಯದಿ (unlikely (glyph >= glyph_end)) ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned ಪೂರ್ಣಾಙ್ಕ xBytes, yBytes;
      xBytes = yBytes = 0;
      ಯದಿ (flag & FLAG_X_SHORT) xBytes = 1;
      ಅನ್ಯಥಾ ಯದಿ ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      ಯದಿ (flag & FLAG_Y_SHORT) yBytes = 1;
      ಅನ್ಯಥಾ ಯದಿ ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      ಯದಿ (coords_with_flags >= ಅಂಕೆ_coordinates) ಭಙ್ಗ;
    }

    ಯದಿ (unlikely (coords_with_flags != ಅಂಕೆ_coordinates)) ಪ್ರತ್ಯಾಗಮ hb_bytes_t ();
    ಪ್ರತ್ಯಾಗಮ bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  ಶೂನ್ಯ drop_hints ()
  {
    ಯದಿ (!has_instructions_length ()) ಪ್ರತ್ಯಾಗಮ;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  ಶೂನ್ಯ drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ instructions_len = instructions_length ();
    unsigned ಪೂರ್ಣಾಙ್ಕ glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  ಶೂನ್ಯ set_overlaps_flag ()
  {
    ಯದಿ (unlikely (!header.ಸಂಖ್ಯೆ_Of_Contours)) ಪ್ರತ್ಯಾಗಮ;

    unsigned flags_offset = length (instructions_length ());
    ಯದಿ (unlikely (flags_offset + 1 > bytes.length)) ಪ್ರತ್ಯಾಗಮ;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ read_flags (ಧ್ರುವ HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ಧ್ರುವ HBUINT8 *end,
			  ತಾರ್ಕಿಕ extended)
  {
    auto *points = points_.arrayZ;
    unsigned ಎಣಿಕೆ = points_.length;
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ;)
    {
      ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      uint8_t flag = *p++;
      ಯದಿ (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ಯದಿ (flag & FLAG_REPEAT)
      {
	ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	unsigned ಪೂರ್ಣಾಙ್ಕ repeat_ಎಣಿಕೆ = *p++;
	unsigned stop = hb_min (i + repeat_ಎಣಿಕೆ, ಎಣಿಕೆ);
	ಪ್ರತಿ (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ಸ್ಥಿರ ತಾರ್ಕಿಕ read_points (ಧ್ರುವ HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ಧ್ರುವ HBUINT8 *end,
			   ಭಿನ್ನ contour_point_t::*m,
			   ಧ್ರುವ simple_glyph_flag_t short_flag,
			   ಧ್ರುವ simple_glyph_flag_t same_flag)
  {
    ಪೂರ್ಣಾಙ್ಕ v = 0;

    ಪ್ರತಿ (auto &point : points_)
    {
      unsigned flag = point.flag;
      ಯದಿ (flag & short_flag)
      {
	ಯದಿ (unlikely (p + 1 > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	v += (ತಾರ್ಕಿಕ(flag & same_flag) * 2 - 1) * *p++;
      }
      ಅನ್ಯಥಾ
      {
	ಯದಿ (!(flag & same_flag))
	{
	  ಯದಿ (unlikely (p + HBINT16::static_size > end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
	  v += *(ಧ್ರುವ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  ತಾರ್ಕಿಕ get_contour_points (contour_point_vector_t &points /* OUT */,
			   ತಾರ್ಕಿಕ extended,
			   ತಾರ್ಕಿಕ phantom_only = ಅಸತ್ಯ,
			   int64_t *budget = nullptr) ಧ್ರುವ
  {
    ಧ್ರುವ HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_contours = header.ಸಂಖ್ಯೆ_Of_Contours;
    ಪ್ರತಿಜ್ಞಾ (ಅಂಕೆ_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ಯದಿ (unlikely (!bytes.check_range (&endPtsOfContours[ಅಂಕೆ_contours]))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_points = endPtsOfContours[ಅಂಕೆ_contours - 1] + 1;
    ಯದಿ (unlikely (ಅಂಕೆ_points < (unsigned) ಅಂಕೆ_contours)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ಯದಿ (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, ಅಂಕೆ_points))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned old_length = points.length;
    points.alloc (points.length + ಅಂಕೆ_points + 4); // Allocate for phantom points, to avoid a possible copy
    ಯದಿ (unlikely (!points.resize_dirty (points.length + ಅಂಕೆ_points))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    auto points_ = points.as_array ().sub_array (old_length);
    ಯದಿ (!phantom_only)
      hb_memset (points_.arrayZ, 0, ಪರಿಮಾಣ (contour_point_t) * ಅಂಕೆ_points);
    ಯದಿ (phantom_only) ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಅಂಕೆ_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = ಸತ್ಯ;

    /* Skip instructions */
    ಧ್ರುವ HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[ಅಂಕೆ_contours + 1],
						 endPtsOfContours[ಅಂಕೆ_contours]);

    ಯದಿ (unlikely ((ಧ್ರುವ ವರ್ಣ *) p < bytes.arrayZ)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; /* Unlikely overflow */
    ಧ್ರುವ HBUINT8 *end = (ಧ್ರುವ HBUINT8 *) (bytes.arrayZ + bytes.length);
    ಯದಿ (unlikely (p >= end)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    /* Read x & y coordinates */
    ಪ್ರತ್ಯಾಗಮ read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ encode_coord (ಪೂರ್ಣಾಙ್ಕ value,
                            unsigned &flag,
                            ಧ್ರುವ simple_glyph_flag_t short_flag,
                            ಧ್ರುವ simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    ಯದಿ (value == 0)
    {
      flag |= same_flag;
    }
    ಅನ್ಯಥಾ ಯದಿ (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ಯದಿ (value > 0) flag |= same_flag;
      ಅನ್ಯಥಾ value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    ಅನ್ಯಥಾ
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    ಯದಿ (flag == lastflag && repeat != 255)
    {
      repeat++;
      ಯದಿ (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      ಅನ್ಯಥಾ
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    ಅನ್ಯಥಾ
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  ತಾರ್ಕಿಕ compile_bytes_with_deltas (ಧ್ರುವ contour_point_vector_t &all_points,
                                  ತಾರ್ಕಿಕ no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ಯದಿ (header.ಸಂಖ್ಯೆ_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    unsigned ಅಂಕೆ_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    ಯದಿ (unlikely (!flags.alloc_exact (ಅಂಕೆ_points))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely (!x_coords.alloc_exact (2*ಅಂಕೆ_points))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (unlikely (!y_coords.alloc_exact (2*ಅಂಕೆ_points))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    unsigned lastflag = 255, repeat = 0;
    ಪೂರ್ಣಾಙ್ಕ prev_x = 0, prev_y = 0;

    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      ಪೂರ್ಣಾಙ್ಕ cur_x = roundf (all_points.arrayZ[i].x);
      ಪೂರ್ಣಾಙ್ಕ cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.ಸಂಖ್ಯೆ_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    ಯದಿ (!no_hinting)
      total_len += len_instrs;

    ವರ್ಣ *p = (ವರ್ಣ *) hb_malloc (total_len);
    ಯದಿ (unlikely (!p)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ ವರ್ಣ *src = bytes.arrayZ + GlyphHeader::static_size;
    ವರ್ಣ *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ಯದಿ (!no_hinting)
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
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

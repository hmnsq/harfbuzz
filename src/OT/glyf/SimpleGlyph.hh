#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"


ନାମାକାଶ OT {
ନାମାକାଶ glyf_impl {


ସଂରଚନା SimpleGlyph
{
  ଗଣନା simple_glyph_flag_t
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

  ଅଚଳ GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (ଅଚଳ GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instruction_len_offset () ଅଚଳ
  { ଫେରନ୍ତୁ GlyphHeader::static_size + 2 * header.ସଂଖ୍ୟା_Of_Contours; }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା length (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instruction_len) ଅଚଳ
  { ଫେରନ୍ତୁ instruction_len_offset () + 2 + instruction_len; }

  ତାର୍କିକ has_instructions_length () ଅଚଳ
  {
    ଫେରନ୍ତୁ instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instructions_length () ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instruction_length_offset = instruction_len_offset ();
    ଯଦି (unlikely (instruction_length_offset + 2 > bytes.length)) ଫେରନ୍ତୁ 0;

    ଅଚଳ HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    ଯଦି (unlikely (length (instructionLength) > bytes.length)) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ instructionLength;
  }

  ଅଚଳ hb_bytes_t trim_padding () ଅଚଳ
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ଅଚଳ uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ଅଚଳ uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    ଯଦି (unlikely (glyph + 2 >= glyph_end)) ଫେରନ୍ତୁ hb_bytes_t ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + ଅଙ୍କ_instructions;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coord_bytes = 0;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords_with_flags = 0;
    ଯେତେବେଳେ (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା repeat = 1;
      ଯଦି (flag & FLAG_REPEAT)
      {
	ଯଦି (unlikely (glyph >= glyph_end)) ଫେରନ୍ତୁ hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା xBytes, yBytes;
      xBytes = yBytes = 0;
      ଯଦି (flag & FLAG_X_SHORT) xBytes = 1;
      ଅନ୍ୟଥା ଯଦି ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      ଯଦି (flag & FLAG_Y_SHORT) yBytes = 1;
      ଅନ୍ୟଥା ଯଦି ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      ଯଦି (coords_with_flags >= ଅଙ୍କ_coordinates) ଭାଙ୍ଗ;
    }

    ଯଦି (unlikely (coords_with_flags != ଅଙ୍କ_coordinates)) ଫେରନ୍ତୁ hb_bytes_t ();
    ଫେରନ୍ତୁ bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  ଶୂନ୍ୟ drop_hints ()
  {
    ଯଦି (!has_instructions_length ()) ଫେରନ୍ତୁ;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  ଶୂନ୍ୟ drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instructions_len = instructions_length ();
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  ଶୂନ୍ୟ set_overlaps_flag ()
  {
    ଯଦି (unlikely (!header.ସଂଖ୍ୟା_Of_Contours)) ଫେରନ୍ତୁ;

    unsigned flags_offset = length (instructions_length ());
    ଯଦି (unlikely (flags_offset + 1 > bytes.length)) ଫେରନ୍ତୁ;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  ସ୍ଥିର ତାର୍କିକ read_flags (ଅଚଳ HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ଅଚଳ HBUINT8 *end,
			  ତାର୍କିକ extended)
  {
    auto *points = points_.arrayZ;
    unsigned ଗଣନା_2 = points_.length;
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_2;)
    {
      ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
      uint8_t flag = *p++;
      ଯଦି (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ଯଦି (flag & FLAG_REPEAT)
      {
	ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା repeat_ଗଣନା = *p++;
	unsigned stop = hb_min (i + repeat_ଗଣନା, ଗଣନା_2);
	ପାଇଁ (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ସ୍ଥିର ତାର୍କିକ read_points (ଅଚଳ HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ଅଚଳ HBUINT8 *end,
			   ଦଶମିକ contour_point_t::*m,
			   ଅଚଳ simple_glyph_flag_t short_flag,
			   ଅଚଳ simple_glyph_flag_t same_flag)
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା v = 0;

    ପାଇଁ (auto &point : points_)
    {
      unsigned flag = point.flag;
      ଯଦି (flag & short_flag)
      {
	ଯଦି (unlikely (p + 1 > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	v += (ତାର୍କିକ(flag & same_flag) * 2 - 1) * *p++;
      }
      ଅନ୍ୟଥା
      {
	ଯଦି (!(flag & same_flag))
	{
	  ଯଦି (unlikely (p + HBINT16::static_size > end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;
	  v += *(ଅଚଳ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  ତାର୍କିକ get_contour_points (contour_point_vector_t &points /* OUT */,
			   ତାର୍କିକ extended,
			   ତାର୍କିକ phantom_only = ମିଥ୍ୟା,
			   int64_t *budget = nullptr) ଅଚଳ
  {
    ଅଚଳ HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_contours = header.ସଂଖ୍ୟା_Of_Contours;
    ପ୍ରତିଜ୍ଞା (ଅଙ୍କ_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ଯଦି (unlikely (!bytes.check_range (&endPtsOfContours[ଅଙ୍କ_contours]))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_points = endPtsOfContours[ଅଙ୍କ_contours - 1] + 1;
    ଯଦି (unlikely (ଅଙ୍କ_points < (unsigned) ଅଙ୍କ_contours)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ଯଦି (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, ଅଙ୍କ_points))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned old_length = points.length;
    points.alloc (points.length + ଅଙ୍କ_points + 4); // Allocate for phantom points, to avoid a possible copy
    ଯଦି (unlikely (!points.resize_dirty (points.length + ଅଙ୍କ_points))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    auto points_ = points.as_array ().sub_array (old_length);
    ଯଦି (!phantom_only)
      hb_memset (points_.arrayZ, 0, ଆକାର (contour_point_t) * ଅଙ୍କ_points);
    ଯଦି (phantom_only) ଫେରନ୍ତୁ ସତ୍ୟ;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଅଙ୍କ_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = ସତ୍ୟ;

    /* Skip instructions */
    ଅଚଳ HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[ଅଙ୍କ_contours + 1],
						 endPtsOfContours[ଅଙ୍କ_contours]);

    ଯଦି (unlikely ((ଅଚଳ ଅକ୍ଷର *) p < bytes.arrayZ)) ଫେରନ୍ତୁ ମିଥ୍ୟା; /* Unlikely overflow */
    ଅଚଳ HBUINT8 *end = (ଅଚଳ HBUINT8 *) (bytes.arrayZ + bytes.length);
    ଯଦି (unlikely (p >= end)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    /* Read x & y coordinates */
    ଫେରନ୍ତୁ read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  ସ୍ଥିର ଶୂନ୍ୟ encode_coord (ପୂର୍ଣ୍ଣସଂଖ୍ୟା value,
                            unsigned &flag,
                            ଅଚଳ simple_glyph_flag_t short_flag,
                            ଅଚଳ simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    ଯଦି (value == 0)
    {
      flag |= same_flag;
    }
    ଅନ୍ୟଥା ଯଦି (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ଯଦି (value > 0) flag |= same_flag;
      ଅନ୍ୟଥା value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    ଅନ୍ୟଥା
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    ଯଦି (flag == lastflag && repeat != 255)
    {
      repeat++;
      ଯଦି (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      ଅନ୍ୟଥା
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    ଅନ୍ୟଥା
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  ତାର୍କିକ compile_bytes_with_deltas (ଅଚଳ contour_point_vector_t &all_points,
                                  ତାର୍କିକ no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ଯଦି (header.ସଂଖ୍ୟା_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    unsigned ଅଙ୍କ_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    ଯଦି (unlikely (!flags.alloc_exact (ଅଙ୍କ_points))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (!x_coords.alloc_exact (2*ଅଙ୍କ_points))) ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (unlikely (!y_coords.alloc_exact (2*ଅଙ୍କ_points))) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    unsigned lastflag = 255, repeat = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା prev_x = 0, prev_y = 0;

    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur_x = roundf (all_points.arrayZ[i].x);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.ସଂଖ୍ୟା_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    ଯଦି (!no_hinting)
      total_len += len_instrs;

    ଅକ୍ଷର *p = (ଅକ୍ଷର *) hb_malloc (total_len);
    ଯଦି (unlikely (!p)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ ଅକ୍ଷର *src = bytes.arrayZ + GlyphHeader::static_size;
    ଅକ୍ଷର *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ଯଦି (!no_hinting)
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
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

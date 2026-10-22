#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#অন্তর্ভাৱ "../../hb-open-type.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


সংরচনা SimpleGlyph
{
  গণনা simple_glyph_flag_t
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

  ধ্রুৱ GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (ধ্রুৱ GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned পূর্ণাঙ্ক instruction_len_offset () ধ্রুৱ
  { ঘূৰাই GlyphHeader::static_size + 2 * header.সংখ্যা_Of_Contours; }

  unsigned পূর্ণাঙ্ক length (unsigned পূর্ণাঙ্ক instruction_len) ধ্রুৱ
  { ঘূৰাই instruction_len_offset () + 2 + instruction_len; }

  তার্কিক has_instructions_length () ধ্রুৱ
  {
    ঘূৰাই instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned পূর্ণাঙ্ক instructions_length () ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক instruction_length_offset = instruction_len_offset ();
    যদি (unlikely (instruction_length_offset + 2 > bytes.length)) ঘূৰাই 0;

    ধ্রুৱ HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    যদি (unlikely (length (instructionLength) > bytes.length)) ঘূৰাই 0;
    ঘূৰাই instructionLength;
  }

  ধ্রুৱ hb_bytes_t trim_padding () ধ্রুৱ
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ধ্রুৱ uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ধ্রুৱ uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    যদি (unlikely (glyph + 2 >= glyph_end)) ঘূৰাই hb_bytes_t ();
    unsigned পূর্ণাঙ্ক অঙ্ক_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned পূর্ণাঙ্ক অঙ্ক_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + অঙ্ক_instructions;

    unsigned পূর্ণাঙ্ক coord_bytes = 0;
    unsigned পূর্ণাঙ্ক coords_with_flags = 0;
    যেতিয়ালৈ (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned পূর্ণাঙ্ক repeat = 1;
      যদি (flag & FLAG_REPEAT)
      {
	যদি (unlikely (glyph >= glyph_end)) ঘূৰাই hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned পূর্ণাঙ্ক xBytes, yBytes;
      xBytes = yBytes = 0;
      যদি (flag & FLAG_X_SHORT) xBytes = 1;
      অন্যথা যদি ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      যদি (flag & FLAG_Y_SHORT) yBytes = 1;
      অন্যথা যদি ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      যদি (coords_with_flags >= অঙ্ক_coordinates) ভঙ্গ;
    }

    যদি (unlikely (coords_with_flags != অঙ্ক_coordinates)) ঘূৰাই hb_bytes_t ();
    ঘূৰাই bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  শূন্য drop_hints ()
  {
    যদি (!has_instructions_length ()) ঘূৰাই;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  শূন্য drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক instructions_len = instructions_length ();
    unsigned পূর্ণাঙ্ক glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  শূন্য set_overlaps_flag ()
  {
    যদি (unlikely (!header.সংখ্যা_Of_Contours)) ঘূৰাই;

    unsigned flags_offset = length (instructions_length ());
    যদি (unlikely (flags_offset + 1 > bytes.length)) ঘূৰাই;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  স্থির তার্কিক read_flags (ধ্রুৱ HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ধ্রুৱ HBUINT8 *end,
			  তার্কিক extended)
  {
    auto *points = points_.arrayZ;
    unsigned গুনতি = points_.length;
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < গুনতি;)
    {
      যদি (unlikely (p + 1 > end)) ঘূৰাই মিছা;
      uint8_t flag = *p++;
      যদি (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      যদি (flag & FLAG_REPEAT)
      {
	যদি (unlikely (p + 1 > end)) ঘূৰাই মিছা;
	unsigned পূর্ণাঙ্ক repeat_গুনতি = *p++;
	unsigned stop = hb_min (i + repeat_গুনতি, গুনতি);
	বাবে (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ঘূৰাই সঁচা;
  }

  স্থির তার্কিক read_points (ধ্রুৱ HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ধ্রুৱ HBUINT8 *end,
			   ভিন্ন contour_point_t::*m,
			   ধ্রুৱ simple_glyph_flag_t short_flag,
			   ধ্রুৱ simple_glyph_flag_t same_flag)
  {
    পূর্ণাঙ্ক v = 0;

    বাবে (auto &point : points_)
    {
      unsigned flag = point.flag;
      যদি (flag & short_flag)
      {
	যদি (unlikely (p + 1 > end)) ঘূৰাই মিছা;
	v += (তার্কিক(flag & same_flag) * 2 - 1) * *p++;
      }
      অন্যথা
      {
	যদি (!(flag & same_flag))
	{
	  যদি (unlikely (p + HBINT16::static_size > end)) ঘূৰাই মিছা;
	  v += *(ধ্রুৱ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    ঘূৰাই সঁচা;
  }

  তার্কিক get_contour_points (contour_point_vector_t &points /* OUT */,
			   তার্কিক extended,
			   তার্কিক phantom_only = মিছা,
			   int64_t *budget = nullptr) ধ্রুৱ
  {
    ধ্রুৱ HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    পূর্ণাঙ্ক অঙ্ক_contours = header.সংখ্যা_Of_Contours;
    প্রতিজ্ঞা (অঙ্ক_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    যদি (unlikely (!bytes.check_range (&endPtsOfContours[অঙ্ক_contours]))) ঘূৰাই মিছা;
    unsigned পূর্ণাঙ্ক অঙ্ক_points = endPtsOfContours[অঙ্ক_contours - 1] + 1;
    যদি (unlikely (অঙ্ক_points < (unsigned) অঙ্ক_contours)) ঘূৰাই মিছা;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    যদি (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, অঙ্ক_points))) ঘূৰাই মিছা;

    unsigned old_length = points.length;
    points.alloc (points.length + অঙ্ক_points + 4); // Allocate for phantom points, to avoid a possible copy
    যদি (unlikely (!points.resize_dirty (points.length + অঙ্ক_points))) ঘূৰাই মিছা;
    auto points_ = points.as_array ().sub_array (old_length);
    যদি (!phantom_only)
      hb_memset (points_.arrayZ, 0, পরিমাণ (contour_point_t) * অঙ্ক_points);
    যদি (phantom_only) ঘূৰাই সঁচা;

    বাবে (পূর্ণাঙ্ক i = 0; i < অঙ্ক_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = সঁচা;

    /* Skip instructions */
    ধ্রুৱ HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[অঙ্ক_contours + 1],
						 endPtsOfContours[অঙ্ক_contours]);

    যদি (unlikely ((ধ্রুৱ ৱর্ণ *) p < bytes.arrayZ)) ঘূৰাই মিছা; /* Unlikely overflow */
    ধ্রুৱ HBUINT8 *end = (ধ্রুৱ HBUINT8 *) (bytes.arrayZ + bytes.length);
    যদি (unlikely (p >= end)) ঘূৰাই মিছা;

    /* Read x & y coordinates */
    ঘূৰাই read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  স্থির শূন্য encode_coord (পূর্ণাঙ্ক value,
                            unsigned &flag,
                            ধ্রুৱ simple_glyph_flag_t short_flag,
                            ধ্রুৱ simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    যদি (value == 0)
    {
      flag |= same_flag;
    }
    অন্যথা যদি (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      যদি (value > 0) flag |= same_flag;
      অন্যথা value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    অন্যথা
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  স্থির শূন্য encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    যদি (flag == lastflag && repeat != 255)
    {
      repeat++;
      যদি (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      অন্যথা
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    অন্যথা
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  তার্কিক compile_bytes_with_deltas (ধ্রুৱ contour_point_vector_t &all_points,
                                  তার্কিক no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    যদি (header.সংখ্যা_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      ঘূৰাই সঁচা;
    }
    unsigned অঙ্ক_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    যদি (unlikely (!flags.alloc_exact (অঙ্ক_points))) ঘূৰাই মিছা;
    যদি (unlikely (!x_coords.alloc_exact (2*অঙ্ক_points))) ঘূৰাই মিছা;
    যদি (unlikely (!y_coords.alloc_exact (2*অঙ্ক_points))) ঘূৰাই মিছা;

    unsigned lastflag = 255, repeat = 0;
    পূর্ণাঙ্ক prev_x = 0, prev_y = 0;

    বাবে (unsigned i = 0; i < অঙ্ক_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      পূর্ণাঙ্ক cur_x = roundf (all_points.arrayZ[i].x);
      পূর্ণাঙ্ক cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.সংখ্যা_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    যদি (!no_hinting)
      total_len += len_instrs;

    ৱর্ণ *p = (ৱর্ণ *) hb_malloc (total_len);
    যদি (unlikely (!p)) ঘূৰাই মিছা;

    ধ্রুৱ ৱর্ণ *src = bytes.arrayZ + GlyphHeader::static_size;
    ৱর্ণ *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    যদি (!no_hinting)
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
    ঘূৰাই সঁচা;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

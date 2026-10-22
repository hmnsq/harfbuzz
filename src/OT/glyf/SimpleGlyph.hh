#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#অন্তর্ভাব "../../hb-open-type.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


কাঠামো SimpleGlyph
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

  ধ্রুবক GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (ধ্রুবক GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned পূর্ণসংখ্যা instruction_len_offset () ধ্রুবক
  { ফেরত GlyphHeader::static_আকার + 2 * header.সংখ্যা_Of_Contours; }

  unsigned পূর্ণসংখ্যা দৈর্ঘ্য (unsigned পূর্ণসংখ্যা instruction_len) ধ্রুবক
  { ফেরত instruction_len_offset () + 2 + instruction_len; }

  যৌক্তিক has_instructions_দৈর্ঘ্য () ধ্রুবক
  {
    ফেরত instruction_len_offset () + 2 <= bytes.দৈর্ঘ্য;
  }

  unsigned পূর্ণসংখ্যা instructions_দৈর্ঘ্য () ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা instruction_দৈর্ঘ্য_offset = instruction_len_offset ();
    যদি (unlikely (instruction_দৈর্ঘ্য_offset + 2 > bytes.দৈর্ঘ্য)) ফেরত 0;

    ধ্রুবক HBUINT16 &instruction_দৈর্ঘ্য = StructAtOffset<HBUINT16> (&bytes, instruction_দৈর্ঘ্য_offset);
    /* Out of bounds of the current glyph */
    যদি (unlikely (দৈর্ঘ্য (instruction_দৈর্ঘ্য) > bytes.দৈর্ঘ্য)) ফেরত 0;
    ফেরত instruction_দৈর্ঘ্য;
  }

  ধ্রুবক hb_bytes_t trim_padding () ধ্রুবক
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ধ্রুবক uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ধ্রুবক uint8_t *glyph_end = glyph + bytes.দৈর্ঘ্য;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    যদি (unlikely (glyph + 2 >= glyph_end)) ফেরত hb_bytes_t ();
    unsigned পূর্ণসংখ্যা অঙ্ক_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned পূর্ণসংখ্যা অঙ্ক_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + অঙ্ক_instructions;

    unsigned পূর্ণসংখ্যা coord_bytes = 0;
    unsigned পূর্ণসংখ্যা coords_with_flags = 0;
    যখন (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned পূর্ণসংখ্যা repeat = 1;
      যদি (flag & FLAG_REPEAT)
      {
	যদি (unlikely (glyph >= glyph_end)) ফেরত hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned পূর্ণসংখ্যা xBytes, yBytes;
      xBytes = yBytes = 0;
      যদি (flag & FLAG_X_SHORT) xBytes = 1;
      অন্যথা যদি ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      যদি (flag & FLAG_Y_SHORT) yBytes = 1;
      অন্যথা যদি ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      যদি (coords_with_flags >= অঙ্ক_coordinates) বিরতি;
    }

    যদি (unlikely (coords_with_flags != অঙ্ক_coordinates)) ফেরত hb_bytes_t ();
    ফেরত bytes.sub_array (0, bytes.দৈর্ঘ্য + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  শূন্য drop_hints ()
  {
    যদি (!has_instructions_দৈর্ঘ্য ()) ফেরত;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  শূন্য drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা instructions_len = instructions_দৈর্ঘ্য ();
    unsigned পূর্ণসংখ্যা glyph_দৈর্ঘ্য = দৈর্ঘ্য (instructions_len);
    dest_start = bytes.sub_array (0, glyph_দৈর্ঘ্য - instructions_len);
    dest_end = bytes.sub_array (glyph_দৈর্ঘ্য, bytes.দৈর্ঘ্য - glyph_দৈর্ঘ্য);
  }

  শূন্য set_overlaps_flag ()
  {
    যদি (unlikely (!header.সংখ্যা_Of_Contours)) ফেরত;

    unsigned flags_offset = দৈর্ঘ্য (instructions_দৈর্ঘ্য ());
    যদি (unlikely (flags_offset + 1 > bytes.দৈর্ঘ্য)) ফেরত;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  স্থির যৌক্তিক read_flags (ধ্রুবক HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ধ্রুবক HBUINT8 *end,
			  যৌক্তিক extended)
  {
    auto *points = points_.arrayZ;
    unsigned গুনতি = points_.দৈর্ঘ্য;
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < গুনতি;)
    {
      যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;
      uint8_t flag = *p++;
      যদি (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      যদি (flag & FLAG_REPEAT)
      {
	যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;
	unsigned পূর্ণসংখ্যা repeat_গুনতি = *p++;
	unsigned stop = hb_min (i + repeat_গুনতি, গুনতি);
	জন্য (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ফেরত সত্য;
  }

  স্থির যৌক্তিক read_points (ধ্রুবক HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   ধ্রুবক HBUINT8 *end,
			   দশমিক contour_point_t::*m,
			   ধ্রুবক simple_glyph_flag_t short_flag,
			   ধ্রুবক simple_glyph_flag_t same_flag)
  {
    পূর্ণসংখ্যা v = 0;

    জন্য (auto &point : points_)
    {
      unsigned flag = point.flag;
      যদি (flag & short_flag)
      {
	যদি (unlikely (p + 1 > end)) ফেরত মিথ্যা;
	v += (যৌক্তিক(flag & same_flag) * 2 - 1) * *p++;
      }
      অন্যথা
      {
	যদি (!(flag & same_flag))
	{
	  যদি (unlikely (p + HBINT16::static_আকার > end)) ফেরত মিথ্যা;
	  v += *(ধ্রুবক HBINT16 *) p;
	  p += HBINT16::static_আকার;
	}
      }
      point.*m = v;
    }
    ফেরত সত্য;
  }

  যৌক্তিক get_contour_points (contour_point_vector_t &points /* OUT */,
			   যৌক্তিক extended,
			   যৌক্তিক phantom_only = মিথ্যা,
			   int64_t *budget = nullptr) ধ্রুবক
  {
    ধ্রুবক HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    পূর্ণসংখ্যা অঙ্ক_contours = header.সংখ্যা_Of_Contours;
    প্রতিজ্ঞা (অঙ্ক_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    যদি (unlikely (!bytes.check_range (&endPtsOfContours[অঙ্ক_contours]))) ফেরত মিথ্যা;
    unsigned পূর্ণসংখ্যা অঙ্ক_points = endPtsOfContours[অঙ্ক_contours - 1] + 1;
    যদি (unlikely (অঙ্ক_points < (unsigned) অঙ্ক_contours)) ফেরত মিথ্যা;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    যদি (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, অঙ্ক_points))) ফেরত মিথ্যা;

    unsigned old_দৈর্ঘ্য = points.দৈর্ঘ্য;
    points.alloc (points.দৈর্ঘ্য + অঙ্ক_points + 4); // Allocate for phantom points, to avoid a possible copy
    যদি (unlikely (!points.resize_dirty (points.দৈর্ঘ্য + অঙ্ক_points))) ফেরত মিথ্যা;
    auto points_ = points.as_array ().sub_array (old_দৈর্ঘ্য);
    যদি (!phantom_only)
      hb_memset (points_.arrayZ, 0, আকার (contour_point_t) * অঙ্ক_points);
    যদি (phantom_only) ফেরত সত্য;

    জন্য (পূর্ণসংখ্যা i = 0; i < অঙ্ক_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = সত্য;

    /* Skip instructions */
    ধ্রুবক HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[অঙ্ক_contours + 1],
						 endPtsOfContours[অঙ্ক_contours]);

    যদি (unlikely ((ধ্রুবক অক্ষর *) p < bytes.arrayZ)) ফেরত মিথ্যা; /* Unlikely overflow */
    ধ্রুবক HBUINT8 *end = (ধ্রুবক HBUINT8 *) (bytes.arrayZ + bytes.দৈর্ঘ্য);
    যদি (unlikely (p >= end)) ফেরত মিথ্যা;

    /* Read x & y coordinates */
    ফেরত read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  স্থির শূন্য encode_coord (পূর্ণসংখ্যা value,
                            unsigned &flag,
                            ধ্রুবক simple_glyph_flag_t short_flag,
                            ধ্রুবক simple_glyph_flag_t same_flag,
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

      coords.arrayZ[coords.দৈর্ঘ্য++] = (uint8_t) value;
    }
    অন্যথা
    {
      int16_t val = value;
      coords.arrayZ[coords.দৈর্ঘ্য++] = val >> 8;
      coords.arrayZ[coords.দৈর্ঘ্য++] = val & 0xff;
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
        flags.arrayZ[flags.দৈর্ঘ্য++] = flag;
      }
      অন্যথা
      {
        unsigned len = flags.দৈর্ঘ্য;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    অন্যথা
    {
      repeat = 0;
      flags.arrayZ[flags.দৈর্ঘ্য++] = flag;
    }
  }

  যৌক্তিক compile_bytes_with_deltas (ধ্রুবক contour_point_vector_t &all_points,
                                  যৌক্তিক no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    যদি (header.সংখ্যা_Of_Contours == 0 || all_points.দৈর্ঘ্য <= 4)
    {
      dest_bytes = hb_bytes_t ();
      ফেরত সত্য;
    }
    unsigned অঙ্ক_points = all_points.দৈর্ঘ্য - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    যদি (unlikely (!flags.alloc_exact (অঙ্ক_points))) ফেরত মিথ্যা;
    যদি (unlikely (!x_coords.alloc_exact (2*অঙ্ক_points))) ফেরত মিথ্যা;
    যদি (unlikely (!y_coords.alloc_exact (2*অঙ্ক_points))) ফেরত মিথ্যা;

    unsigned lastflag = 255, repeat = 0;
    পূর্ণসংখ্যা পূর্ব_x = 0, পূর্ব_y = 0;

    জন্য (unsigned i = 0; i < অঙ্ক_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      পূর্ণসংখ্যা cur_x = roundf (all_points.arrayZ[i].x);
      পূর্ণসংখ্যা cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - পূর্ব_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - পূর্ব_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      পূর্ব_x = cur_x;
      পূর্ব_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.সংখ্যা_Of_Contours + 2;
    unsigned len_instrs = instructions_দৈর্ঘ্য ();
    unsigned যোগ_len = len_before_instrs + flags.দৈর্ঘ্য + x_coords.দৈর্ঘ্য + y_coords.দৈর্ঘ্য;

    যদি (!no_hinting)
      যোগ_len += len_instrs;

    অক্ষর *p = (অক্ষর *) hb_malloc (যোগ_len);
    যদি (unlikely (!p)) ফেরত মিথ্যা;

    ধ্রুবক অক্ষর *src = bytes.arrayZ + GlyphHeader::static_আকার;
    অক্ষর *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    যদি (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.দৈর্ঘ্য);
    cur += flags.দৈর্ঘ্য;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.দৈর্ঘ্য);
    cur += x_coords.দৈর্ঘ্য;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.দৈর্ঘ্য);

    dest_bytes = hb_bytes_t (p, যোগ_len);
    ফেরত সত্য;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

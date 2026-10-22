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
  { ܦܢܐ GlyphHeader::static_size + 2 * header.numberOfContours; }

  unsigned صحيح length (unsigned صحيح instruction_len) ثابت
  { ܦܢܐ instruction_len_offset () + 2 + instruction_len; }

  منطقي has_instructions_length () ثابت
  {
    ܦܢܐ instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned صحيح instructions_length () ثابت
  {
    unsigned صحيح instruction_length_offset = instruction_len_offset ();
    ܐܢ (unlikely (instruction_length_offset + 2 > bytes.length)) ܦܢܐ 0;

    ثابت HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    ܐܢ (unlikely (length (instructionLength) > bytes.length)) ܦܢܐ 0;
    ܦܢܐ instructionLength;
  }

  ثابت hb_bytes_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ثابت uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    ܐܢ (unlikely (glyph + 2 >= glyph_end)) ܦܢܐ hb_bytes_t ();
    unsigned صحيح num_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned صحيح num_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + num_instructions;

    unsigned صحيح coord_bytes = 0;
    unsigned صحيح coords_with_flags = 0;
    ܟܡܐ (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned صحيح repeat = 1;
      ܐܢ (flag & FLAG_REPEAT)
      {
	ܐܢ (unlikely (glyph >= glyph_end)) ܦܢܐ hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned صحيح xBytes, yBytes;
      xBytes = yBytes = 0;
      ܐܢ (flag & FLAG_X_SHORT) xBytes = 1;
      ܐܚܪܢܐ ܐܢ ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      ܐܢ (flag & FLAG_Y_SHORT) yBytes = 1;
      ܐܚܪܢܐ ܐܢ ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      ܐܢ (coords_with_flags >= num_coordinates) اكسر;
    }

    ܐܢ (unlikely (coords_with_flags != num_coordinates)) ܦܢܐ hb_bytes_t ();
    ܦܢܐ bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    ܐܢ (!has_instructions_length ()) ܦܢܐ;
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
    ܐܢ (unlikely (!header.numberOfContours)) ܦܢܐ;

    unsigned flags_offset = length (instructions_length ());
    ܐܢ (unlikely (flags_offset + 1 > bytes.length)) ܦܢܐ;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  ساكن منطقي read_flags (ثابت HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  ثابت HBUINT8 *end,
			  منطقي extended)
  {
    auto *points = points_.arrayZ;
    unsigned count = points_.length;
    لكل (unsigned صحيح i = 0; i < count;)
    {
      ܐܢ (unlikely (p + 1 > end)) ܦܢܐ ܕܓܠܐ;
      uint8_t flag = *p++;
      ܐܢ (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ܐܢ (flag & FLAG_REPEAT)
      {
	ܐܢ (unlikely (p + 1 > end)) ܦܢܐ ܕܓܠܐ;
	unsigned صحيح repeat_count = *p++;
	unsigned stop = hb_min (i + repeat_count, count);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ܦܢܐ ܫܪܪܐ;
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
      ܐܢ (flag & short_flag)
      {
	ܐܢ (unlikely (p + 1 > end)) ܦܢܐ ܕܓܠܐ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      ܐܚܪܢܐ
      {
	ܐܢ (!(flag & same_flag))
	{
	  ܐܢ (unlikely (p + HBINT16::static_size > end)) ܦܢܐ ܕܓܠܐ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    ܦܢܐ ܫܪܪܐ;
  }

  منطقي get_contour_points (contour_point_vector_t &points /* OUT */,
			   منطقي extended,
			   منطقي phantom_only = ܕܓܠܐ,
			   int64_t *budget = nullptr) ثابت
  {
    ثابت HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    صحيح num_contours = header.numberOfContours;
    أكد (num_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ܐܢ (unlikely (!bytes.check_range (&endPtsOfContours[num_contours]))) ܦܢܐ ܕܓܠܐ;
    unsigned صحيح num_points = endPtsOfContours[num_contours - 1] + 1;
    ܐܢ (unlikely (num_points < (unsigned) num_contours)) ܦܢܐ ܕܓܠܐ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ܐܢ (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, num_points))) ܦܢܐ ܕܓܠܐ;

    unsigned old_length = points.length;
    points.alloc (points.length + num_points + 4); // Allocate for phantom points, to avoid a possible copy
    ܐܢ (unlikely (!points.resize_dirty (points.length + num_points))) ܦܢܐ ܕܓܠܐ;
    auto points_ = points.as_array ().sub_array (old_length);
    ܐܢ (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_point_t) * num_points);
    ܐܢ (phantom_only) ܦܢܐ ܫܪܪܐ;

    لكل (صحيح i = 0; i < num_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = ܫܪܪܐ;

    /* Skip instructions */
    ثابت HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[num_contours + 1],
						 endPtsOfContours[num_contours]);

    ܐܢ (unlikely ((ثابت حرف *) p < bytes.arrayZ)) ܦܢܐ ܕܓܠܐ; /* Unlikely overflow */
    ثابت HBUINT8 *end = (ثابت HBUINT8 *) (bytes.arrayZ + bytes.length);
    ܐܢ (unlikely (p >= end)) ܦܢܐ ܕܓܠܐ;

    /* Read x & y coordinates */
    ܦܢܐ read_flags (p, points_, end, extended)
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
    ܐܢ (value == 0)
    {
      flag |= same_flag;
    }
    ܐܚܪܢܐ ܐܢ (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ܐܢ (value > 0) flag |= same_flag;
      ܐܚܪܢܐ value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    ܐܚܪܢܐ
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
    ܐܢ (flag == lastflag && repeat != 255)
    {
      repeat++;
      ܐܢ (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      ܐܚܪܢܐ
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    ܐܚܪܢܐ
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  منطقي compile_bytes_with_deltas (ثابت contour_point_vector_t &all_points,
                                  منطقي no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ܐܢ (header.numberOfContours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      ܦܢܐ ܫܪܪܐ;
    }
    unsigned num_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    ܐܢ (unlikely (!flags.alloc_exact (num_points))) ܦܢܐ ܕܓܠܐ;
    ܐܢ (unlikely (!x_coords.alloc_exact (2*num_points))) ܦܢܐ ܕܓܠܐ;
    ܐܢ (unlikely (!y_coords.alloc_exact (2*num_points))) ܦܢܐ ܕܓܠܐ;

    unsigned lastflag = 255, repeat = 0;
    صحيح prev_x = 0, prev_y = 0;

    لكل (unsigned i = 0; i < num_points; i++)
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

    unsigned len_before_instrs = 2 * header.numberOfContours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    ܐܢ (!no_hinting)
      total_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (total_len);
    ܐܢ (unlikely (!p)) ܦܢܐ ܕܓܠܐ;

    ثابت حرف *src = bytes.arrayZ + GlyphHeader::static_size;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ܐܢ (!no_hinting)
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
    ܦܢܐ ܫܪܪܐ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

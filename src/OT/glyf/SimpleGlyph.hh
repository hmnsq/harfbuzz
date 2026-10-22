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
  { أرجع GlyphHeader::static_size + 2 * header.numberOfContours; }

  unsigned صحيح length (unsigned صحيح instruction_len) ثابت
  { أرجع instruction_len_offset () + 2 + instruction_len; }

  منطقي has_instructions_length () ثابت
  {
    أرجع instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned صحيح instructions_length () ثابت
  {
    unsigned صحيح instruction_length_offset = instruction_len_offset ();
    𐡄𐡍 (unlikely (instruction_length_offset + 2 > bytes.length)) أرجع 0;

    ثابت HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    𐡄𐡍 (unlikely (length (instructionLength) > bytes.length)) أرجع 0;
    أرجع instructionLength;
  }

  ثابت hb_bytes_t trim_padding () ثابت
  {
    /* based on FontTools _g_l_y_f.py::trim */
    ثابت uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    ثابت uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    𐡄𐡍 (unlikely (glyph + 2 >= glyph_end)) أرجع hb_bytes_t ();
    unsigned صحيح num_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned صحيح num_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + num_instructions;

    unsigned صحيح coord_bytes = 0;
    unsigned صحيح coords_with_flags = 0;
    طالما (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned صحيح repeat = 1;
      𐡄𐡍 (flag & FLAG_REPEAT)
      {
	𐡄𐡍 (unlikely (glyph >= glyph_end)) أرجع hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned صحيح xBytes, yBytes;
      xBytes = yBytes = 0;
      𐡄𐡍 (flag & FLAG_X_SHORT) xBytes = 1;
      وإلا 𐡄𐡍 ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      𐡄𐡍 (flag & FLAG_Y_SHORT) yBytes = 1;
      وإلا 𐡄𐡍 ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      𐡄𐡍 (coords_with_flags >= num_coordinates) اكسر;
    }

    𐡄𐡍 (unlikely (coords_with_flags != num_coordinates)) أرجع hb_bytes_t ();
    أرجع bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  فراغ drop_hints ()
  {
    𐡄𐡍 (!has_instructions_length ()) أرجع;
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
    𐡄𐡍 (unlikely (!header.numberOfContours)) أرجع;

    unsigned flags_offset = length (instructions_length ());
    𐡄𐡍 (unlikely (flags_offset + 1 > bytes.length)) أرجع;

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
      𐡄𐡍 (unlikely (p + 1 > end)) أرجع خطأ;
      uint8_t flag = *p++;
      𐡄𐡍 (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      𐡄𐡍 (flag & FLAG_REPEAT)
      {
	𐡄𐡍 (unlikely (p + 1 > end)) أرجع خطأ;
	unsigned صحيح repeat_count = *p++;
	unsigned stop = hb_min (i + repeat_count, count);
	لكل (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    أرجع 𐡒𐡔𐡈;
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
      𐡄𐡍 (flag & short_flag)
      {
	𐡄𐡍 (unlikely (p + 1 > end)) أرجع خطأ;
	v += (منطقي(flag & same_flag) * 2 - 1) * *p++;
      }
      وإلا
      {
	𐡄𐡍 (!(flag & same_flag))
	{
	  𐡄𐡍 (unlikely (p + HBINT16::static_size > end)) أرجع خطأ;
	  v += *(ثابت HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    أرجع 𐡒𐡔𐡈;
  }

  منطقي get_contour_points (contour_point_vector_t &points /* OUT */,
			   منطقي extended,
			   منطقي phantom_only = خطأ,
			   int64_t *budget = nullptr) ثابت
  {
    ثابت HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    صحيح num_contours = header.numberOfContours;
    أكد (num_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    𐡄𐡍 (unlikely (!bytes.check_range (&endPtsOfContours[num_contours]))) أرجع خطأ;
    unsigned صحيح num_points = endPtsOfContours[num_contours - 1] + 1;
    𐡄𐡍 (unlikely (num_points < (unsigned) num_contours)) أرجع خطأ;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    𐡄𐡍 (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, num_points))) أرجع خطأ;

    unsigned old_length = points.length;
    points.alloc (points.length + num_points + 4); // Allocate for phantom points, to avoid a possible copy
    𐡄𐡍 (unlikely (!points.resize_dirty (points.length + num_points))) أرجع خطأ;
    auto points_ = points.as_array ().sub_array (old_length);
    𐡄𐡍 (!phantom_only)
      hb_memset (points_.arrayZ, 0, حجم (contour_point_t) * num_points);
    𐡄𐡍 (phantom_only) أرجع 𐡒𐡔𐡈;

    لكل (صحيح i = 0; i < num_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = 𐡒𐡔𐡈;

    /* Skip instructions */
    ثابت HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[num_contours + 1],
						 endPtsOfContours[num_contours]);

    𐡄𐡍 (unlikely ((ثابت حرف *) p < bytes.arrayZ)) أرجع خطأ; /* Unlikely overflow */
    ثابت HBUINT8 *end = (ثابت HBUINT8 *) (bytes.arrayZ + bytes.length);
    𐡄𐡍 (unlikely (p >= end)) أرجع خطأ;

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
    𐡄𐡍 (value == 0)
    {
      flag |= same_flag;
    }
    وإلا 𐡄𐡍 (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      𐡄𐡍 (value > 0) flag |= same_flag;
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
    𐡄𐡍 (flag == lastflag && repeat != 255)
    {
      repeat++;
      𐡄𐡍 (repeat == 1)
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
    𐡄𐡍 (header.numberOfContours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      أرجع 𐡒𐡔𐡈;
    }
    unsigned num_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    𐡄𐡍 (unlikely (!flags.alloc_exact (num_points))) أرجع خطأ;
    𐡄𐡍 (unlikely (!x_coords.alloc_exact (2*num_points))) أرجع خطأ;
    𐡄𐡍 (unlikely (!y_coords.alloc_exact (2*num_points))) أرجع خطأ;

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

    𐡄𐡍 (!no_hinting)
      total_len += len_instrs;

    حرف *p = (حرف *) hb_malloc (total_len);
    𐡄𐡍 (unlikely (!p)) أرجع خطأ;

    ثابت حرف *src = bytes.arrayZ + GlyphHeader::static_size;
    حرف *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    𐡄𐡍 (!no_hinting)
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
    أرجع 𐡒𐡔𐡈;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

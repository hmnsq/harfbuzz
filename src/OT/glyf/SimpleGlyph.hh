#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#அந்தர்பாவ "../../hb-open-type.hh"


நாமாகாஶ OT {
நாமாகாஶ glyf_impl {


கட்டமைப்பு SimpleGlyph
{
  எண்ணிக்கை simple_glyph_flag_t
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

  மாறிலி GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (மாறிலி GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned முழு instruction_len_offset () மாறிலி
  { திரும்பு GlyphHeader::static_size + 2 * header.எண்_Of_Contours; }

  unsigned முழு length (unsigned முழு instruction_len) மாறிலி
  { திரும்பு instruction_len_offset () + 2 + instruction_len; }

  தருக்க has_instructions_length () மாறிலி
  {
    திரும்பு instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned முழு instructions_length () மாறிலி
  {
    unsigned முழு instruction_length_offset = instruction_len_offset ();
    எனில் (unlikely (instruction_length_offset + 2 > bytes.length)) திரும்பு 0;

    மாறிலி HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    எனில் (unlikely (length (instructionLength) > bytes.length)) திரும்பு 0;
    திரும்பு instructionLength;
  }

  மாறிலி hb_bytes_t trim_padding () மாறிலி
  {
    /* based on FontTools _g_l_y_f.py::trim */
    மாறிலி uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    மாறிலி uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    எனில் (unlikely (glyph + 2 >= glyph_end)) திரும்பு hb_bytes_t ();
    unsigned முழு இலக்கம்_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned முழு இலக்கம்_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + இலக்கம்_instructions;

    unsigned முழு coord_bytes = 0;
    unsigned முழு coords_with_flags = 0;
    போது (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned முழு repeat = 1;
      எனில் (flag & FLAG_REPEAT)
      {
	எனில் (unlikely (glyph >= glyph_end)) திரும்பு hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned முழு xBytes, yBytes;
      xBytes = yBytes = 0;
      எனில் (flag & FLAG_X_SHORT) xBytes = 1;
      இல்லையெனில் எனில் ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      எனில் (flag & FLAG_Y_SHORT) yBytes = 1;
      இல்லையெனில் எனில் ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      எனில் (coords_with_flags >= இலக்கம்_coordinates) நிறுத்து;
    }

    எனில் (unlikely (coords_with_flags != இலக்கம்_coordinates)) திரும்பு hb_bytes_t ();
    திரும்பு bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  வெறுமை drop_hints ()
  {
    எனில் (!has_instructions_length ()) திரும்பு;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  வெறுமை drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) மாறிலி
  {
    unsigned முழு instructions_len = instructions_length ();
    unsigned முழு glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  வெறுமை set_overlaps_flag ()
  {
    எனில் (unlikely (!header.எண்_Of_Contours)) திரும்பு;

    unsigned flags_offset = length (instructions_length ());
    எனில் (unlikely (flags_offset + 1 > bytes.length)) திரும்பு;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  நிலையான தருக்க read_flags (மாறிலி HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  மாறிலி HBUINT8 *end,
			  தருக்க extended)
  {
    auto *points = points_.arrayZ;
    unsigned மொத்தம் = points_.length;
    க்கு (unsigned முழு i = 0; i < மொத்தம்;)
    {
      எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;
      uint8_t flag = *p++;
      எனில் (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      எனில் (flag & FLAG_REPEAT)
      {
	எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;
	unsigned முழு repeat_மொத்தம் = *p++;
	unsigned stop = hb_min (i + repeat_மொத்தம், மொத்தம்);
	க்கு (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    திரும்பு உண்மை;
  }

  நிலையான தருக்க read_points (மாறிலி HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   மாறிலி HBUINT8 *end,
			   தசம contour_point_t::*m,
			   மாறிலி simple_glyph_flag_t short_flag,
			   மாறிலி simple_glyph_flag_t same_flag)
  {
    முழு v = 0;

    க்கு (auto &point : points_)
    {
      unsigned flag = point.flag;
      எனில் (flag & short_flag)
      {
	எனில் (unlikely (p + 1 > end)) திரும்பு பொய்;
	v += (தருக்க(flag & same_flag) * 2 - 1) * *p++;
      }
      இல்லையெனில்
      {
	எனில் (!(flag & same_flag))
	{
	  எனில் (unlikely (p + HBINT16::static_size > end)) திரும்பு பொய்;
	  v += *(மாறிலி HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    திரும்பு உண்மை;
  }

  தருக்க get_contour_points (contour_point_vector_t &points /* OUT */,
			   தருக்க extended,
			   தருக்க phantom_only = பொய்,
			   int64_t *budget = nullptr) மாறிலி
  {
    மாறிலி HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    முழு இலக்கம்_contours = header.எண்_Of_Contours;
    ப்ரதிஜ்ஞா (இலக்கம்_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    எனில் (unlikely (!bytes.check_range (&endPtsOfContours[இலக்கம்_contours]))) திரும்பு பொய்;
    unsigned முழு இலக்கம்_points = endPtsOfContours[இலக்கம்_contours - 1] + 1;
    எனில் (unlikely (இலக்கம்_points < (unsigned) இலக்கம்_contours)) திரும்பு பொய்;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    எனில் (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, இலக்கம்_points))) திரும்பு பொய்;

    unsigned old_length = points.length;
    points.alloc (points.length + இலக்கம்_points + 4); // Allocate for phantom points, to avoid a possible copy
    எனில் (unlikely (!points.resize_dirty (points.length + இலக்கம்_points))) திரும்பு பொய்;
    auto points_ = points.as_array ().sub_array (old_length);
    எனில் (!phantom_only)
      hb_memset (points_.arrayZ, 0, அளவு (contour_point_t) * இலக்கம்_points);
    எனில் (phantom_only) திரும்பு உண்மை;

    க்கு (முழு i = 0; i < இலக்கம்_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = உண்மை;

    /* Skip instructions */
    மாறிலி HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[இலக்கம்_contours + 1],
						 endPtsOfContours[இலக்கம்_contours]);

    எனில் (unlikely ((மாறிலி எழுத்து *) p < bytes.arrayZ)) திரும்பு பொய்; /* Unlikely overflow */
    மாறிலி HBUINT8 *end = (மாறிலி HBUINT8 *) (bytes.arrayZ + bytes.length);
    எனில் (unlikely (p >= end)) திரும்பு பொய்;

    /* Read x & y coordinates */
    திரும்பு read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  நிலையான வெறுமை encode_coord (முழு value,
                            unsigned &flag,
                            மாறிலி simple_glyph_flag_t short_flag,
                            மாறிலி simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    எனில் (value == 0)
    {
      flag |= same_flag;
    }
    இல்லையெனில் எனில் (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      எனில் (value > 0) flag |= same_flag;
      இல்லையெனில் value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    இல்லையெனில்
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  நிலையான வெறுமை encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    எனில் (flag == lastflag && repeat != 255)
    {
      repeat++;
      எனில் (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      இல்லையெனில்
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    இல்லையெனில்
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  தருக்க compile_bytes_with_deltas (மாறிலி contour_point_vector_t &all_points,
                                  தருக்க no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    எனில் (header.எண்_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      திரும்பு உண்மை;
    }
    unsigned இலக்கம்_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    எனில் (unlikely (!flags.alloc_exact (இலக்கம்_points))) திரும்பு பொய்;
    எனில் (unlikely (!x_coords.alloc_exact (2*இலக்கம்_points))) திரும்பு பொய்;
    எனில் (unlikely (!y_coords.alloc_exact (2*இலக்கம்_points))) திரும்பு பொய்;

    unsigned lastflag = 255, repeat = 0;
    முழு prev_x = 0, prev_y = 0;

    க்கு (unsigned i = 0; i < இலக்கம்_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      முழு cur_x = roundf (all_points.arrayZ[i].x);
      முழு cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.எண்_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    எனில் (!no_hinting)
      total_len += len_instrs;

    எழுத்து *p = (எழுத்து *) hb_malloc (total_len);
    எனில் (unlikely (!p)) திரும்பு பொய்;

    மாறிலி எழுத்து *src = bytes.arrayZ + GlyphHeader::static_size;
    எழுத்து *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    எனில் (!no_hinting)
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
    திரும்பு உண்மை;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

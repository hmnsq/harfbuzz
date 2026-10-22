#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"


നാമാകാശ OT {
നാമാകാശ glyf_impl {


ഘടന SimpleGlyph
{
  എണ്ണൽ simple_glyph_flag_t
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

  സ്ഥിരം GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (സ്ഥിരം GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned പൂർണ്ണസംഖ്യ instruction_len_offset () സ്ഥിരം
  { തിരികെ GlyphHeader::static_size + 2 * header.സംഖ്യ_Of_Contours; }

  unsigned പൂർണ്ണസംഖ്യ length (unsigned പൂർണ്ണസംഖ്യ instruction_len) സ്ഥിരം
  { തിരികെ instruction_len_offset () + 2 + instruction_len; }

  തർക്കം has_instructions_length () സ്ഥിരം
  {
    തിരികെ instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned പൂർണ്ണസംഖ്യ instructions_length () സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ instruction_length_offset = instruction_len_offset ();
    എങ്കിൽ (unlikely (instruction_length_offset + 2 > bytes.length)) തിരികെ 0;

    സ്ഥിരം HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    എങ്കിൽ (unlikely (length (instructionLength) > bytes.length)) തിരികെ 0;
    തിരികെ instructionLength;
  }

  സ്ഥിരം hb_bytes_t trim_padding () സ്ഥിരം
  {
    /* based on FontTools _g_l_y_f.py::trim */
    സ്ഥിരം uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    സ്ഥിരം uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    എങ്കിൽ (unlikely (glyph + 2 >= glyph_end)) തിരികെ hb_bytes_t ();
    unsigned പൂർണ്ണസംഖ്യ അക്കം_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + അക്കം_instructions;

    unsigned പൂർണ്ണസംഖ്യ coord_bytes = 0;
    unsigned പൂർണ്ണസംഖ്യ coords_with_flags = 0;
    ആകുമ്പോൾ (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned പൂർണ്ണസംഖ്യ repeat = 1;
      എങ്കിൽ (flag & FLAG_REPEAT)
      {
	എങ്കിൽ (unlikely (glyph >= glyph_end)) തിരികെ hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned പൂർണ്ണസംഖ്യ xBytes, yBytes;
      xBytes = yBytes = 0;
      എങ്കിൽ (flag & FLAG_X_SHORT) xBytes = 1;
      അല്ലെങ്കിൽ എങ്കിൽ ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      എങ്കിൽ (flag & FLAG_Y_SHORT) yBytes = 1;
      അല്ലെങ്കിൽ എങ്കിൽ ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      എങ്കിൽ (coords_with_flags >= അക്കം_coordinates) നിർത്തുക;
    }

    എങ്കിൽ (unlikely (coords_with_flags != അക്കം_coordinates)) തിരികെ hb_bytes_t ();
    തിരികെ bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  ശൂന്യം drop_hints ()
  {
    എങ്കിൽ (!has_instructions_length ()) തിരികെ;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  ശൂന്യം drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ instructions_len = instructions_length ();
    unsigned പൂർണ്ണസംഖ്യ glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  ശൂന്യം set_overlaps_flag ()
  {
    എങ്കിൽ (unlikely (!header.സംഖ്യ_Of_Contours)) തിരികെ;

    unsigned flags_offset = length (instructions_length ());
    എങ്കിൽ (unlikely (flags_offset + 1 > bytes.length)) തിരികെ;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  സ്ഥിതം തർക്കം read_flags (സ്ഥിരം HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  സ്ഥിരം HBUINT8 *end,
			  തർക്കം extended)
  {
    auto *points = points_.arrayZ;
    unsigned എണ്ണം = points_.length;
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം;)
    {
      എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;
      uint8_t flag = *p++;
      എങ്കിൽ (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      എങ്കിൽ (flag & FLAG_REPEAT)
      {
	എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;
	unsigned പൂർണ്ണസംഖ്യ repeat_എണ്ണം = *p++;
	unsigned stop = hb_min (i + repeat_എണ്ണം, എണ്ണം);
	വേണ്ടി (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    തിരികെ സത്യം;
  }

  സ്ഥിതം തർക്കം read_points (സ്ഥിരം HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   സ്ഥിരം HBUINT8 *end,
			   ദശാംശം contour_point_t::*m,
			   സ്ഥിരം simple_glyph_flag_t short_flag,
			   സ്ഥിരം simple_glyph_flag_t same_flag)
  {
    പൂർണ്ണസംഖ്യ v = 0;

    വേണ്ടി (auto &point : points_)
    {
      unsigned flag = point.flag;
      എങ്കിൽ (flag & short_flag)
      {
	എങ്കിൽ (unlikely (p + 1 > end)) തിരികെ അസത്യം;
	v += (തർക്കം(flag & same_flag) * 2 - 1) * *p++;
      }
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (!(flag & same_flag))
	{
	  എങ്കിൽ (unlikely (p + HBINT16::static_size > end)) തിരികെ അസത്യം;
	  v += *(സ്ഥിരം HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    തിരികെ സത്യം;
  }

  തർക്കം get_contour_points (contour_point_vector_t &points /* OUT */,
			   തർക്കം extended,
			   തർക്കം phantom_only = അസത്യം,
			   int64_t *budget = nullptr) സ്ഥിരം
  {
    സ്ഥിരം HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    പൂർണ്ണസംഖ്യ അക്കം_contours = header.സംഖ്യ_Of_Contours;
    പ്രതിജ്ഞാ (അക്കം_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    എങ്കിൽ (unlikely (!bytes.check_range (&endPtsOfContours[അക്കം_contours]))) തിരികെ അസത്യം;
    unsigned പൂർണ്ണസംഖ്യ അക്കം_points = endPtsOfContours[അക്കം_contours - 1] + 1;
    എങ്കിൽ (unlikely (അക്കം_points < (unsigned) അക്കം_contours)) തിരികെ അസത്യം;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    എങ്കിൽ (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, അക്കം_points))) തിരികെ അസത്യം;

    unsigned old_length = points.length;
    points.alloc (points.length + അക്കം_points + 4); // Allocate for phantom points, to avoid a possible copy
    എങ്കിൽ (unlikely (!points.resize_dirty (points.length + അക്കം_points))) തിരികെ അസത്യം;
    auto points_ = points.as_array ().sub_array (old_length);
    എങ്കിൽ (!phantom_only)
      hb_memset (points_.arrayZ, 0, വലുപ്പം (contour_point_t) * അക്കം_points);
    എങ്കിൽ (phantom_only) തിരികെ സത്യം;

    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < അക്കം_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = സത്യം;

    /* Skip instructions */
    സ്ഥിരം HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[അക്കം_contours + 1],
						 endPtsOfContours[അക്കം_contours]);

    എങ്കിൽ (unlikely ((സ്ഥിരം അക്ഷരം *) p < bytes.arrayZ)) തിരികെ അസത്യം; /* Unlikely overflow */
    സ്ഥിരം HBUINT8 *end = (സ്ഥിരം HBUINT8 *) (bytes.arrayZ + bytes.length);
    എങ്കിൽ (unlikely (p >= end)) തിരികെ അസത്യം;

    /* Read x & y coordinates */
    തിരികെ read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  സ്ഥിതം ശൂന്യം encode_coord (പൂർണ്ണസംഖ്യ value,
                            unsigned &flag,
                            സ്ഥിരം simple_glyph_flag_t short_flag,
                            സ്ഥിരം simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    എങ്കിൽ (value == 0)
    {
      flag |= same_flag;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      എങ്കിൽ (value > 0) flag |= same_flag;
      അല്ലെങ്കിൽ value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    അല്ലെങ്കിൽ
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  സ്ഥിതം ശൂന്യം encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    എങ്കിൽ (flag == lastflag && repeat != 255)
    {
      repeat++;
      എങ്കിൽ (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      അല്ലെങ്കിൽ
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    അല്ലെങ്കിൽ
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  തർക്കം compile_bytes_with_deltas (സ്ഥിരം contour_point_vector_t &all_points,
                                  തർക്കം no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    എങ്കിൽ (header.സംഖ്യ_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      തിരികെ സത്യം;
    }
    unsigned അക്കം_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    എങ്കിൽ (unlikely (!flags.alloc_exact (അക്കം_points))) തിരികെ അസത്യം;
    എങ്കിൽ (unlikely (!x_coords.alloc_exact (2*അക്കം_points))) തിരികെ അസത്യം;
    എങ്കിൽ (unlikely (!y_coords.alloc_exact (2*അക്കം_points))) തിരികെ അസത്യം;

    unsigned lastflag = 255, repeat = 0;
    പൂർണ്ണസംഖ്യ prev_x = 0, prev_y = 0;

    വേണ്ടി (unsigned i = 0; i < അക്കം_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      പൂർണ്ണസംഖ്യ cur_x = roundf (all_points.arrayZ[i].x);
      പൂർണ്ണസംഖ്യ cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.സംഖ്യ_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    എങ്കിൽ (!no_hinting)
      total_len += len_instrs;

    അക്ഷരം *p = (അക്ഷരം *) hb_malloc (total_len);
    എങ്കിൽ (unlikely (!p)) തിരികെ അസത്യം;

    സ്ഥിരം അക്ഷരം *src = bytes.arrayZ + GlyphHeader::static_size;
    അക്ഷരം *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    എങ്കിൽ (!no_hinting)
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
    തിരികെ സത്യം;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

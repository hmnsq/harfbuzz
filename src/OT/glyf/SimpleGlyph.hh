#अगर_अघोषित OT_GLYF_SIMPLEGLYPH_HH
#घोषणा OT_GLYF_SIMPLEGLYPH_HH


#समावेश "../../hb-open-type.hh"


namespace OT {
namespace glyf_impl {


काष्ठा SimpleGlyph
{
  क्रमागत simple_glyph_flag_t
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

  स्थिर GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (स्थिर GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  अचिन्हित पूर्णांक instruction_len_offset () स्थिर
  { वापस GlyphHeader::static_आकार + 2 * header.संख्या_Of_Contours; }

  अचिन्हित पूर्णांक लंबाई (अचिन्हित पूर्णांक instruction_len) स्थिर
  { वापस instruction_len_offset () + 2 + instruction_len; }

  bool has_instructions_लंबाई () स्थिर
  {
    वापस instruction_len_offset () + 2 <= bytes.लंबाई;
  }

  अचिन्हित पूर्णांक instructions_लंबाई () स्थिर
  {
    अचिन्हित पूर्णांक instruction_लंबाई_offset = instruction_len_offset ();
    अगर (unlikely (instruction_लंबाई_offset + 2 > bytes.लंबाई)) वापस 0;

    स्थिर HBUINT16 &instruction_लंबाई = StructAtOffset<HBUINT16> (&bytes, instruction_लंबाई_offset);
    /* Out of bounds of the current glyph */
    अगर (unlikely (लंबाई (instruction_लंबाई) > bytes.लंबाई)) वापस 0;
    वापस instruction_लंबाई;
  }

  स्थिर hb_bytes_t trim_padding () स्थिर
  {
    /* based on FontTools _g_l_y_f.py::trim */
    स्थिर uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    स्थिर uint8_t *glyph_end = glyph + bytes.लंबाई;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    अगर (unlikely (glyph + 2 >= glyph_end)) वापस hb_bytes_t ();
    अचिन्हित पूर्णांक अंक_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    अचिन्हित पूर्णांक अंक_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + अंक_instructions;

    अचिन्हित पूर्णांक coord_bytes = 0;
    अचिन्हित पूर्णांक coords_with_flags = 0;
    जबतक (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      अचिन्हित पूर्णांक repeat = 1;
      अगर (flag & FLAG_REPEAT)
      {
	अगर (unlikely (glyph >= glyph_end)) वापस hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      अचिन्हित पूर्णांक xBytes, yBytes;
      xBytes = yBytes = 0;
      अगर (flag & FLAG_X_SHORT) xBytes = 1;
      अन्यथा अगर ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      अगर (flag & FLAG_Y_SHORT) yBytes = 1;
      अन्यथा अगर ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      अगर (coords_with_flags >= अंक_coordinates) अवरोध;
    }

    अगर (unlikely (coords_with_flags != अंक_coordinates)) वापस hb_bytes_t ();
    वापस bytes.sub_array (0, bytes.लंबाई + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  व्योम drop_hints ()
  {
    अगर (!has_instructions_लंबाई ()) वापस;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  व्योम drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) स्थिर
  {
    अचिन्हित पूर्णांक instructions_len = instructions_लंबाई ();
    अचिन्हित पूर्णांक glyph_लंबाई = लंबाई (instructions_len);
    dest_start = bytes.sub_array (0, glyph_लंबाई - instructions_len);
    dest_end = bytes.sub_array (glyph_लंबाई, bytes.लंबाई - glyph_लंबाई);
  }

  व्योम set_overlaps_flag ()
  {
    अगर (unlikely (!header.संख्या_Of_Contours)) वापस;

    अचिन्हित flags_offset = लंबाई (instructions_लंबाई ());
    अगर (unlikely (flags_offset + 1 > bytes.लंबाई)) वापस;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  अटल bool read_flags (स्थिर HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  स्थिर HBUINT8 *end,
			  bool extended)
  {
    स्वतः *points = points_.arrayZ;
    अचिन्हित गिनती = points_.लंबाई;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती;)
    {
      अगर (unlikely (p + 1 > end)) वापस false;
      uint8_t flag = *p++;
      अगर (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      अगर (flag & FLAG_REPEAT)
      {
	अगर (unlikely (p + 1 > end)) वापस false;
	अचिन्हित पूर्णांक repeat_गिनती = *p++;
	अचिन्हित stop = hb_min (i + repeat_गिनती, गिनती);
	क्रम (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    वापस true;
  }

  अटल bool read_points (स्थिर HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   स्थिर HBUINT8 *end,
			   भग्न contour_point_t::*m,
			   स्थिर simple_glyph_flag_t short_flag,
			   स्थिर simple_glyph_flag_t same_flag)
  {
    पूर्णांक v = 0;

    क्रम (स्वतः &point : points_)
    {
      अचिन्हित flag = point.flag;
      अगर (flag & short_flag)
      {
	अगर (unlikely (p + 1 > end)) वापस false;
	v += (bool(flag & same_flag) * 2 - 1) * *p++;
      }
      अन्यथा
      {
	अगर (!(flag & same_flag))
	{
	  अगर (unlikely (p + HBINT16::static_आकार > end)) वापस false;
	  v += *(स्थिर HBINT16 *) p;
	  p += HBINT16::static_आकार;
	}
      }
      point.*m = v;
    }
    वापस true;
  }

  bool get_contour_points (contour_point_vector_t &points /* OUT */,
			   bool extended,
			   bool phantom_only = false,
			   int64_t *budget = nullptr) स्थिर
  {
    स्थिर HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    पूर्णांक अंक_contours = header.संख्या_Of_Contours;
    निश्चित (अंक_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    अगर (unlikely (!bytes.check_range (&endPtsOfContours[अंक_contours]))) वापस false;
    अचिन्हित पूर्णांक अंक_points = endPtsOfContours[अंक_contours - 1] + 1;
    अगर (unlikely (अंक_points < (अचिन्हित) अंक_contours)) वापस false;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    अगर (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, अंक_points))) वापस false;

    अचिन्हित old_लंबाई = points.लंबाई;
    points.alloc (points.लंबाई + अंक_points + 4); // Allocate for phantom points, to avoid a possible copy
    अगर (unlikely (!points.resize_dirty (points.लंबाई + अंक_points))) वापस false;
    स्वतः points_ = points.as_array ().sub_array (old_लंबाई);
    अगर (!phantom_only)
      hb_memset (points_.arrayZ, 0, माप (contour_point_t) * अंक_points);
    अगर (phantom_only) वापस true;

    क्रम (पूर्णांक i = 0; i < अंक_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = true;

    /* Skip instructions */
    स्थिर HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[अंक_contours + 1],
						 endPtsOfContours[अंक_contours]);

    अगर (unlikely ((स्थिर अक्षर *) p < bytes.arrayZ)) वापस false; /* Unlikely overflow */
    स्थिर HBUINT8 *end = (स्थिर HBUINT8 *) (bytes.arrayZ + bytes.लंबाई);
    अगर (unlikely (p >= end)) वापस false;

    /* Read x & y coordinates */
    वापस read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  अटल व्योम encode_coord (पूर्णांक value,
                            अचिन्हित &flag,
                            स्थिर simple_glyph_flag_t short_flag,
                            स्थिर simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    अगर (value == 0)
    {
      flag |= same_flag;
    }
    अन्यथा अगर (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      अगर (value > 0) flag |= same_flag;
      अन्यथा value = -value;

      coords.arrayZ[coords.लंबाई++] = (uint8_t) value;
    }
    अन्यथा
    {
      int16_t val = value;
      coords.arrayZ[coords.लंबाई++] = val >> 8;
      coords.arrayZ[coords.लंबाई++] = val & 0xff;
    }
  }

  अटल व्योम encode_flag (अचिन्हित flag,
                           अचिन्हित &repeat,
                           अचिन्हित lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    अगर (flag == lastflag && repeat != 255)
    {
      repeat++;
      अगर (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.लंबाई++] = flag;
      }
      अन्यथा
      {
        अचिन्हित len = flags.लंबाई;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    अन्यथा
    {
      repeat = 0;
      flags.arrayZ[flags.लंबाई++] = flag;
    }
  }

  bool compile_bytes_with_deltas (स्थिर contour_point_vector_t &all_points,
                                  bool no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    अगर (header.संख्या_Of_Contours == 0 || all_points.लंबाई <= 4)
    {
      dest_bytes = hb_bytes_t ();
      वापस true;
    }
    अचिन्हित अंक_points = all_points.लंबाई - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    अगर (unlikely (!flags.alloc_exact (अंक_points))) वापस false;
    अगर (unlikely (!x_coords.alloc_exact (2*अंक_points))) वापस false;
    अगर (unlikely (!y_coords.alloc_exact (2*अंक_points))) वापस false;

    अचिन्हित lastflag = 255, repeat = 0;
    पूर्णांक पूर्व_x = 0, पूर्व_y = 0;

    क्रम (अचिन्हित i = 0; i < अंक_points; i++)
    {
      अचिन्हित flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      पूर्णांक cur_x = roundf (all_points.arrayZ[i].x);
      पूर्णांक cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - पूर्व_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - पूर्व_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      पूर्व_x = cur_x;
      पूर्व_y = cur_y;
      lastflag = flag;
    }

    अचिन्हित len_before_instrs = 2 * header.संख्या_Of_Contours + 2;
    अचिन्हित len_instrs = instructions_लंबाई ();
    अचिन्हित योग_len = len_before_instrs + flags.लंबाई + x_coords.लंबाई + y_coords.लंबाई;

    अगर (!no_hinting)
      योग_len += len_instrs;

    अक्षर *p = (अक्षर *) hb_malloc (योग_len);
    अगर (unlikely (!p)) वापस false;

    स्थिर अक्षर *src = bytes.arrayZ + GlyphHeader::static_आकार;
    अक्षर *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    अगर (!no_hinting)
    {
      hb_memcpy (cur, src, len_instrs);
      cur += len_instrs;
    }

    hb_memcpy (cur, flags.arrayZ, flags.लंबाई);
    cur += flags.लंबाई;

    hb_memcpy (cur, x_coords.arrayZ, x_coords.लंबाई);
    cur += x_coords.लंबाई;

    hb_memcpy (cur, y_coords.arrayZ, y_coords.लंबाई);

    dest_bytes = hb_bytes_t (p, योग_len);
    वापस true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_SIMPLEGLYPH_HH */

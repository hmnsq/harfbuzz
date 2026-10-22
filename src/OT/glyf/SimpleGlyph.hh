#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#અન્તર્ભાવ "../../hb-open-type.hh"


નામાકાશ OT {
નામાકાશ glyf_impl {


સંરચના SimpleGlyph
{
  ગણના simple_glyph_flag_t
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

  અચલ GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (અચલ GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned પૂર્ણાંક instruction_len_offset () અચલ
  { પાછા GlyphHeader::static_size + 2 * header.સંખ્યા_Of_Contours; }

  unsigned પૂર્ણાંક length (unsigned પૂર્ણાંક instruction_len) અચલ
  { પાછા instruction_len_offset () + 2 + instruction_len; }

  તાર્કિક has_instructions_length () અચલ
  {
    પાછા instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned પૂર્ણાંક instructions_length () અચલ
  {
    unsigned પૂર્ણાંક instruction_length_offset = instruction_len_offset ();
    જો (unlikely (instruction_length_offset + 2 > bytes.length)) પાછા 0;

    અચલ HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    જો (unlikely (length (instructionLength) > bytes.length)) પાછા 0;
    પાછા instructionLength;
  }

  અચલ hb_bytes_t trim_padding () અચલ
  {
    /* based on FontTools _g_l_y_f.py::trim */
    અચલ uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    અચલ uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    જો (unlikely (glyph + 2 >= glyph_end)) પાછા hb_bytes_t ();
    unsigned પૂર્ણાંક અંક_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned પૂર્ણાંક અંક_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + અંક_instructions;

    unsigned પૂર્ણાંક coord_bytes = 0;
    unsigned પૂર્ણાંક coords_with_flags = 0;
    જ્યારે (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned પૂર્ણાંક repeat = 1;
      જો (flag & FLAG_REPEAT)
      {
	જો (unlikely (glyph >= glyph_end)) પાછા hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned પૂર્ણાંક xBytes, yBytes;
      xBytes = yBytes = 0;
      જો (flag & FLAG_X_SHORT) xBytes = 1;
      નહીંતર જો ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      જો (flag & FLAG_Y_SHORT) yBytes = 1;
      નહીંતર જો ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      જો (coords_with_flags >= અંક_coordinates) તોડો;
    }

    જો (unlikely (coords_with_flags != અંક_coordinates)) પાછા hb_bytes_t ();
    પાછા bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  શૂન્ય drop_hints ()
  {
    જો (!has_instructions_length ()) પાછા;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  શૂન્ય drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) અચલ
  {
    unsigned પૂર્ણાંક instructions_len = instructions_length ();
    unsigned પૂર્ણાંક glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  શૂન્ય set_overlaps_flag ()
  {
    જો (unlikely (!header.સંખ્યા_Of_Contours)) પાછા;

    unsigned flags_offset = length (instructions_length ());
    જો (unlikely (flags_offset + 1 > bytes.length)) પાછા;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  સ્થિર તાર્કિક read_flags (અચલ HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  અચલ HBUINT8 *end,
			  તાર્કિક extended)
  {
    auto *points = points_.arrayZ;
    unsigned ગણતરી = points_.length;
    માટે (unsigned પૂર્ણાંક i = 0; i < ગણતરી;)
    {
      જો (unlikely (p + 1 > end)) પાછા ખોટું;
      uint8_t flag = *p++;
      જો (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      જો (flag & FLAG_REPEAT)
      {
	જો (unlikely (p + 1 > end)) પાછા ખોટું;
	unsigned પૂર્ણાંક repeat_ગણતરી = *p++;
	unsigned stop = hb_min (i + repeat_ગણતરી, ગણતરી);
	માટે (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    પાછા સાચું;
  }

  સ્થિર તાર્કિક read_points (અચલ HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   અચલ HBUINT8 *end,
			   દશાંશ contour_point_t::*m,
			   અચલ simple_glyph_flag_t short_flag,
			   અચલ simple_glyph_flag_t same_flag)
  {
    પૂર્ણાંક v = 0;

    માટે (auto &point : points_)
    {
      unsigned flag = point.flag;
      જો (flag & short_flag)
      {
	જો (unlikely (p + 1 > end)) પાછા ખોટું;
	v += (તાર્કિક(flag & same_flag) * 2 - 1) * *p++;
      }
      નહીંતર
      {
	જો (!(flag & same_flag))
	{
	  જો (unlikely (p + HBINT16::static_size > end)) પાછા ખોટું;
	  v += *(અચલ HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    પાછા સાચું;
  }

  તાર્કિક get_contour_points (contour_point_vector_t &points /* OUT */,
			   તાર્કિક extended,
			   તાર્કિક phantom_only = ખોટું,
			   int64_t *budget = nullptr) અચલ
  {
    અચલ HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    પૂર્ણાંક અંક_contours = header.સંખ્યા_Of_Contours;
    પ્રતિજ્ઞા (અંક_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    જો (unlikely (!bytes.check_range (&endPtsOfContours[અંક_contours]))) પાછા ખોટું;
    unsigned પૂર્ણાંક અંક_points = endPtsOfContours[અંક_contours - 1] + 1;
    જો (unlikely (અંક_points < (unsigned) અંક_contours)) પાછા ખોટું;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    જો (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, અંક_points))) પાછા ખોટું;

    unsigned old_length = points.length;
    points.alloc (points.length + અંક_points + 4); // Allocate for phantom points, to avoid a possible copy
    જો (unlikely (!points.resize_dirty (points.length + અંક_points))) પાછા ખોટું;
    auto points_ = points.as_array ().sub_array (old_length);
    જો (!phantom_only)
      hb_memset (points_.arrayZ, 0, કદ (contour_point_t) * અંક_points);
    જો (phantom_only) પાછા સાચું;

    માટે (પૂર્ણાંક i = 0; i < અંક_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = સાચું;

    /* Skip instructions */
    અચલ HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[અંક_contours + 1],
						 endPtsOfContours[અંક_contours]);

    જો (unlikely ((અચલ અક્ષર *) p < bytes.arrayZ)) પાછા ખોટું; /* Unlikely overflow */
    અચલ HBUINT8 *end = (અચલ HBUINT8 *) (bytes.arrayZ + bytes.length);
    જો (unlikely (p >= end)) પાછા ખોટું;

    /* Read x & y coordinates */
    પાછા read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  સ્થિર શૂન્ય encode_coord (પૂર્ણાંક value,
                            unsigned &flag,
                            અચલ simple_glyph_flag_t short_flag,
                            અચલ simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    જો (value == 0)
    {
      flag |= same_flag;
    }
    નહીંતર જો (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      જો (value > 0) flag |= same_flag;
      નહીંતર value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    નહીંતર
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  સ્થિર શૂન્ય encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    જો (flag == lastflag && repeat != 255)
    {
      repeat++;
      જો (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      નહીંતર
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    નહીંતર
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  તાર્કિક compile_bytes_with_deltas (અચલ contour_point_vector_t &all_points,
                                  તાર્કિક no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    જો (header.સંખ્યા_Of_Contours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      પાછા સાચું;
    }
    unsigned અંક_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    જો (unlikely (!flags.alloc_exact (અંક_points))) પાછા ખોટું;
    જો (unlikely (!x_coords.alloc_exact (2*અંક_points))) પાછા ખોટું;
    જો (unlikely (!y_coords.alloc_exact (2*અંક_points))) પાછા ખોટું;

    unsigned lastflag = 255, repeat = 0;
    પૂર્ણાંક prev_x = 0, prev_y = 0;

    માટે (unsigned i = 0; i < અંક_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      પૂર્ણાંક cur_x = roundf (all_points.arrayZ[i].x);
      પૂર્ણાંક cur_y = roundf (all_points.arrayZ[i].y);
      encode_coord (cur_x - prev_x, flag, FLAG_X_SHORT, FLAG_X_SAME, x_coords);
      encode_coord (cur_y - prev_y, flag, FLAG_Y_SHORT, FLAG_Y_SAME, y_coords);
      encode_flag (flag, repeat, lastflag, flags);

      prev_x = cur_x;
      prev_y = cur_y;
      lastflag = flag;
    }

    unsigned len_before_instrs = 2 * header.સંખ્યા_Of_Contours + 2;
    unsigned len_instrs = instructions_length ();
    unsigned total_len = len_before_instrs + flags.length + x_coords.length + y_coords.length;

    જો (!no_hinting)
      total_len += len_instrs;

    અક્ષર *p = (અક્ષર *) hb_malloc (total_len);
    જો (unlikely (!p)) પાછા ખોટું;

    અચલ અક્ષર *src = bytes.arrayZ + GlyphHeader::static_size;
    અક્ષર *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    જો (!no_hinting)
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
    પાછા સાચું;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#include "../../hb-open-type.hh"


namespace OT {
namespace glyf_impl {


struct SimpleGlyph
{
  enum simple_glyph_flag_t
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

  const GlyphHeader &header;
  hb_bytes_t bytes;
  SimpleGlyph (const GlyphHeader &header_, hb_bytes_t bytes_) :
    header (header_), bytes (bytes_) {}

  unsigned цяло instruction_len_offset () const
  { върни GlyphHeader::static_size + 2 * header.numberOfContours; }

  unsigned цяло length (unsigned цяло instruction_len) const
  { върни instruction_len_offset () + 2 + instruction_len; }

  bool has_instructions_length () const
  {
    върни instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned цяло instructions_length () const
  {
    unsigned цяло instruction_length_offset = instruction_len_offset ();
    ако (unlikely (instruction_length_offset + 2 > bytes.length)) върни 0;

    const HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    ако (unlikely (length (instructionLength) > bytes.length)) върни 0;
    върни instructionLength;
  }

  const hb_bytes_t trim_padding () const
  {
    /* based on FontTools _g_l_y_f.py::trim */
    const uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    const uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    ако (unlikely (glyph + 2 >= glyph_end)) върни hb_bytes_t ();
    unsigned цяло num_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned цяло num_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + num_instructions;

    unsigned цяло coord_bytes = 0;
    unsigned цяло coords_with_flags = 0;
    докато (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned цяло repeat = 1;
      ако (flag & FLAG_REPEAT)
      {
	ако (unlikely (glyph >= glyph_end)) върни hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned цяло xBytes, yBytes;
      xBytes = yBytes = 0;
      ако (flag & FLAG_X_SHORT) xBytes = 1;
      иначе ако ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      ако (flag & FLAG_Y_SHORT) yBytes = 1;
      иначе ако ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      ако (coords_with_flags >= num_coordinates) break;
    }

    ако (unlikely (coords_with_flags != num_coordinates)) върни hb_bytes_t ();
    върни bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  void drop_hints ()
  {
    ако (!has_instructions_length ()) върни;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  void drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) const
  {
    unsigned цяло instructions_len = instructions_length ();
    unsigned цяло glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  void set_overlaps_flag ()
  {
    ако (unlikely (!header.numberOfContours)) върни;

    unsigned flags_offset = length (instructions_length ());
    ако (unlikely (flags_offset + 1 > bytes.length)) върни;

    HBUINT8 &first_flag = (HBUINT8 &) StructAtOffset<HBUINT16> (&bytes, flags_offset);
    first_flag = (uint8_t) first_flag | FLAG_OVERLAP_SIMPLE;
  }

  static bool read_flags (const HBUINT8 *&p /* IN/OUT */,
			  hb_array_t<contour_point_t> points_ /* IN/OUT */,
			  const HBUINT8 *end,
			  bool extended)
  {
    auto *points = points_.arrayZ;
    unsigned count = points_.length;
    за (unsigned цяло i = 0; i < count;)
    {
      ако (unlikely (p + 1 > end)) върни false;
      uint8_t flag = *p++;
      ако (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ако (flag & FLAG_REPEAT)
      {
	ако (unlikely (p + 1 > end)) върни false;
	unsigned цяло repeat_count = *p++;
	unsigned stop = hb_min (i + repeat_count, count);
	за (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    върни true;
  }

  static bool read_points (const HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   const HBUINT8 *end,
			   float contour_point_t::*m,
			   const simple_glyph_flag_t short_flag,
			   const simple_glyph_flag_t same_flag)
  {
    цяло v = 0;

    за (auto &point : points_)
    {
      unsigned flag = point.flag;
      ако (flag & short_flag)
      {
	ако (unlikely (p + 1 > end)) върни false;
	v += (bool(flag & same_flag) * 2 - 1) * *p++;
      }
      иначе
      {
	ако (!(flag & same_flag))
	{
	  ако (unlikely (p + HBINT16::static_size > end)) върни false;
	  v += *(const HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    върни true;
  }

  bool get_contour_points (contour_point_vector_t &points /* OUT */,
			   bool extended,
			   bool phantom_only = false,
			   int64_t *budget = nullptr) const
  {
    const HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    цяло num_contours = header.numberOfContours;
    assert (num_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ако (unlikely (!bytes.check_range (&endPtsOfContours[num_contours]))) върни false;
    unsigned цяло num_points = endPtsOfContours[num_contours - 1] + 1;
    ако (unlikely (num_points < (unsigned) num_contours)) върни false;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ако (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, num_points))) върни false;

    unsigned old_length = points.length;
    points.alloc (points.length + num_points + 4); // Allocate for phantom points, to avoid a possible copy
    ако (unlikely (!points.resize_dirty (points.length + num_points))) върни false;
    auto points_ = points.as_array ().sub_array (old_length);
    ако (!phantom_only)
      hb_memset (points_.arrayZ, 0, sizeof (contour_point_t) * num_points);
    ако (phantom_only) върни true;

    за (цяло i = 0; i < num_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = true;

    /* Skip instructions */
    const HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[num_contours + 1],
						 endPtsOfContours[num_contours]);

    ако (unlikely ((const char *) p < bytes.arrayZ)) върни false; /* Unlikely overflow */
    const HBUINT8 *end = (const HBUINT8 *) (bytes.arrayZ + bytes.length);
    ако (unlikely (p >= end)) върни false;

    /* Read x & y coordinates */
    върни read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  static void encode_coord (цяло value,
                            unsigned &flag,
                            const simple_glyph_flag_t short_flag,
                            const simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    ако (value == 0)
    {
      flag |= same_flag;
    }
    иначе ако (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ако (value > 0) flag |= same_flag;
      иначе value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    иначе
    {
      int16_t val = value;
      coords.arrayZ[coords.length++] = val >> 8;
      coords.arrayZ[coords.length++] = val & 0xff;
    }
  }

  static void encode_flag (unsigned flag,
                           unsigned &repeat,
                           unsigned lastflag,
                           hb_vector_t<uint8_t> &flags /* OUT */)
  {
    ако (flag == lastflag && repeat != 255)
    {
      repeat++;
      ако (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      иначе
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    иначе
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  bool compile_bytes_with_deltas (const contour_point_vector_t &all_points,
                                  bool no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ако (header.numberOfContours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      върни true;
    }
    unsigned num_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    ако (unlikely (!flags.alloc_exact (num_points))) върни false;
    ако (unlikely (!x_coords.alloc_exact (2*num_points))) върни false;
    ако (unlikely (!y_coords.alloc_exact (2*num_points))) върни false;

    unsigned lastflag = 255, repeat = 0;
    цяло prev_x = 0, prev_y = 0;

    за (unsigned i = 0; i < num_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      цяло cur_x = roundf (all_points.arrayZ[i].x);
      цяло cur_y = roundf (all_points.arrayZ[i].y);
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

    ако (!no_hinting)
      total_len += len_instrs;

    char *p = (char *) hb_malloc (total_len);
    ако (unlikely (!p)) върни false;

    const char *src = bytes.arrayZ + GlyphHeader::static_size;
    char *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ако (!no_hinting)
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
    върни true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

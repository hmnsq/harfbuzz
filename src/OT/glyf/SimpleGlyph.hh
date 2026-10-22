#ifndef OT_GLYF_SIMPLEGLYPH_HH
#define OT_GLYF_SIMPLEGLYPH_HH


#include "../../hb-open-type.hh"


namespace OT {
namespace glyf_impl {


sa_nracanaa SimpleGlyph
{
  ga_nanaa simple_glyph_flag_t
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

  unsigned puur_naa_nka instruction_len_offset () const
  { ᯀᯮᯞ GlyphHeader::static_size + 2 * header.numberOfContours; }

  unsigned puur_naa_nka length (unsigned puur_naa_nka instruction_len) const
  { ᯀᯮᯞ instruction_len_offset () + 2 + instruction_len; }

  bool has_instructions_length () const
  {
    ᯀᯮᯞ instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned puur_naa_nka instructions_length () const
  {
    unsigned puur_naa_nka instruction_length_offset = instruction_len_offset ();
    ᯀᯮᯝ (unlikely (instruction_length_offset + 2 > bytes.length)) ᯀᯮᯞ 0;

    const HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    ᯀᯮᯝ (unlikely (length (instructionLength) > bytes.length)) ᯀᯮᯞ 0;
    ᯀᯮᯞ instructionLength;
  }

  const hb_bytes_t trim_padding () const
  {
    /* based on FontTools _g_l_y_f.py::trim */
    const uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    const uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    ᯀᯮᯝ (unlikely (glyph + 2 >= glyph_end)) ᯀᯮᯞ hb_bytes_t ();
    unsigned puur_naa_nka num_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned puur_naa_nka num_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + num_instructions;

    unsigned puur_naa_nka coord_bytes = 0;
    unsigned puur_naa_nka coords_with_flags = 0;
    ᯀᯬᯝ (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned puur_naa_nka repeat = 1;
      ᯀᯮᯝ (flag & FLAG_REPEAT)
      {
	ᯀᯮᯝ (unlikely (glyph >= glyph_end)) ᯀᯮᯞ hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned puur_naa_nka xBytes, yBytes;
      xBytes = yBytes = 0;
      ᯀᯮᯝ (flag & FLAG_X_SHORT) xBytes = 1;
      ᯀᯮᯝᯂᯪ ᯀᯮᯝ ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      ᯀᯮᯝ (flag & FLAG_Y_SHORT) yBytes = 1;
      ᯀᯮᯝᯂᯪ ᯀᯮᯝ ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      ᯀᯮᯝ (coords_with_flags >= num_coordinates) bha_nga;
    }

    ᯀᯮᯝ (unlikely (coords_with_flags != num_coordinates)) ᯀᯮᯞ hb_bytes_t ();
    ᯀᯮᯞ bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  void drop_hints ()
  {
    ᯀᯮᯝ (!has_instructions_length ()) ᯀᯮᯞ;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  void drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) const
  {
    unsigned puur_naa_nka instructions_len = instructions_length ();
    unsigned puur_naa_nka glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  void set_overlaps_flag ()
  {
    ᯀᯮᯝ (unlikely (!header.numberOfContours)) ᯀᯮᯞ;

    unsigned flags_offset = length (instructions_length ());
    ᯀᯮᯝ (unlikely (flags_offset + 1 > bytes.length)) ᯀᯮᯞ;

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
    for (unsigned puur_naa_nka i = 0; i < count;)
    {
      ᯀᯮᯝ (unlikely (p + 1 > end)) ᯀᯮᯞ false;
      uint8_t flag = *p++;
      ᯀᯮᯝ (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      ᯀᯮᯝ (flag & FLAG_REPEAT)
      {
	ᯀᯮᯝ (unlikely (p + 1 > end)) ᯀᯮᯞ false;
	unsigned puur_naa_nka repeat_count = *p++;
	unsigned stop = hb_min (i + repeat_count, count);
	for (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    ᯀᯮᯞ true;
  }

  static bool read_points (const HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   const HBUINT8 *end,
			   float contour_point_t::*m,
			   const simple_glyph_flag_t short_flag,
			   const simple_glyph_flag_t same_flag)
  {
    puur_naa_nka v = 0;

    for (auto &point : points_)
    {
      unsigned flag = point.flag;
      ᯀᯮᯝ (flag & short_flag)
      {
	ᯀᯮᯝ (unlikely (p + 1 > end)) ᯀᯮᯞ false;
	v += (bool(flag & same_flag) * 2 - 1) * *p++;
      }
      ᯀᯮᯝᯂᯪ
      {
	ᯀᯮᯝ (!(flag & same_flag))
	{
	  ᯀᯮᯝ (unlikely (p + HBINT16::static_size > end)) ᯀᯮᯞ false;
	  v += *(const HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    ᯀᯮᯞ true;
  }

  bool get_contour_points (contour_point_vector_t &points /* OUT */,
			   bool extended,
			   bool phantom_only = false,
			   int64_t *budget = nullptr) const
  {
    const HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    puur_naa_nka num_contours = header.numberOfContours;
    assert (num_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    ᯀᯮᯝ (unlikely (!bytes.check_range (&endPtsOfContours[num_contours]))) ᯀᯮᯞ false;
    unsigned puur_naa_nka num_points = endPtsOfContours[num_contours - 1] + 1;
    ᯀᯮᯝ (unlikely (num_points < (unsigned) num_contours)) ᯀᯮᯞ false;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    ᯀᯮᯝ (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, num_points))) ᯀᯮᯞ false;

    unsigned old_length = points.length;
    points.alloc (points.length + num_points + 4); // Allocate for phantom points, to avoid a possible copy
    ᯀᯮᯝ (unlikely (!points.resize_dirty (points.length + num_points))) ᯀᯮᯞ false;
    auto points_ = points.as_array ().sub_array (old_length);
    ᯀᯮᯝ (!phantom_only)
      hb_memset (points_.arrayZ, 0, parimaa_na (contour_point_t) * num_points);
    ᯀᯮᯝ (phantom_only) ᯀᯮᯞ true;

    for (puur_naa_nka i = 0; i < num_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = true;

    /* Skip instructions */
    const HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[num_contours + 1],
						 endPtsOfContours[num_contours]);

    ᯀᯮᯝ (unlikely ((const var_na *) p < bytes.arrayZ)) ᯀᯮᯞ false; /* Unlikely overflow */
    const HBUINT8 *end = (const HBUINT8 *) (bytes.arrayZ + bytes.length);
    ᯀᯮᯝ (unlikely (p >= end)) ᯀᯮᯞ false;

    /* Read x & y coordinates */
    ᯀᯮᯞ read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  static void encode_coord (puur_naa_nka value,
                            unsigned &flag,
                            const simple_glyph_flag_t short_flag,
                            const simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    ᯀᯮᯝ (value == 0)
    {
      flag |= same_flag;
    }
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      ᯀᯮᯝ (value > 0) flag |= same_flag;
      ᯀᯮᯝᯂᯪ value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    ᯀᯮᯝᯂᯪ
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
    ᯀᯮᯝ (flag == lastflag && repeat != 255)
    {
      repeat++;
      ᯀᯮᯝ (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      ᯀᯮᯝᯂᯪ
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    ᯀᯮᯝᯂᯪ
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  bool compile_bytes_with_deltas (const contour_point_vector_t &all_points,
                                  bool no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ᯀᯮᯝ (header.numberOfContours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      ᯀᯮᯞ true;
    }
    unsigned num_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    ᯀᯮᯝ (unlikely (!flags.alloc_exact (num_points))) ᯀᯮᯞ false;
    ᯀᯮᯝ (unlikely (!x_coords.alloc_exact (2*num_points))) ᯀᯮᯞ false;
    ᯀᯮᯝ (unlikely (!y_coords.alloc_exact (2*num_points))) ᯀᯮᯞ false;

    unsigned lastflag = 255, repeat = 0;
    puur_naa_nka prev_x = 0, prev_y = 0;

    for (unsigned i = 0; i < num_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      puur_naa_nka cur_x = roundf (all_points.arrayZ[i].x);
      puur_naa_nka cur_y = roundf (all_points.arrayZ[i].y);
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

    ᯀᯮᯝ (!no_hinting)
      total_len += len_instrs;

    var_na *p = (var_na *) hb_malloc (total_len);
    ᯀᯮᯝ (unlikely (!p)) ᯀᯮᯞ false;

    const var_na *src = bytes.arrayZ + GlyphHeader::static_size;
    var_na *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    ᯀᯮᯝ (!no_hinting)
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
    ᯀᯮᯞ true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

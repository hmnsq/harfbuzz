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

  unsigned ціле instruction_len_offset () const
  { повернути GlyphHeader::static_size + 2 * header.numberOfContours; }

  unsigned ціле length (unsigned ціле instruction_len) const
  { повернути instruction_len_offset () + 2 + instruction_len; }

  bool has_instructions_length () const
  {
    повернути instruction_len_offset () + 2 <= bytes.length;
  }

  unsigned ціле instructions_length () const
  {
    unsigned ціле instruction_length_offset = instruction_len_offset ();
    якщо (unlikely (instruction_length_offset + 2 > bytes.length)) повернути 0;

    const HBUINT16 &instructionLength = StructAtOffset<HBUINT16> (&bytes, instruction_length_offset);
    /* Out of bounds of the current glyph */
    якщо (unlikely (length (instructionLength) > bytes.length)) повернути 0;
    повернути instructionLength;
  }

  const hb_bytes_t trim_padding () const
  {
    /* based on FontTools _g_l_y_f.py::trim */
    const uint8_t *glyph = (uint8_t*) bytes.arrayZ;
    const uint8_t *glyph_end = glyph + bytes.length;
    /* simple glyph w/contours, possibly trimmable */
    glyph += instruction_len_offset ();

    якщо (unlikely (glyph + 2 >= glyph_end)) повернути hb_bytes_t ();
    unsigned ціле num_coordinates = StructAtOffset<HBUINT16> (glyph - 2, 0) + 1;
    unsigned ціле num_instructions = StructAtOffset<HBUINT16> (glyph, 0);

    glyph += 2 + num_instructions;

    unsigned ціле coord_bytes = 0;
    unsigned ціле coords_with_flags = 0;
    поки (glyph < glyph_end)
    {
      uint8_t flag = *glyph;
      glyph++;

      unsigned ціле repeat = 1;
      якщо (flag & FLAG_REPEAT)
      {
	якщо (unlikely (glyph >= glyph_end)) повернути hb_bytes_t ();
	repeat = *glyph + 1;
	glyph++;
      }

      unsigned ціле xBytes, yBytes;
      xBytes = yBytes = 0;
      якщо (flag & FLAG_X_SHORT) xBytes = 1;
      інакше якщо ((flag & FLAG_X_SAME) == 0) xBytes = 2;

      якщо (flag & FLAG_Y_SHORT) yBytes = 1;
      інакше якщо ((flag & FLAG_Y_SAME) == 0) yBytes = 2;

      coord_bytes += (xBytes + yBytes) * repeat;
      coords_with_flags += repeat;
      якщо (coords_with_flags >= num_coordinates) break;
    }

    якщо (unlikely (coords_with_flags != num_coordinates)) повернути hb_bytes_t ();
    повернути bytes.sub_array (0, bytes.length + coord_bytes - (glyph_end - glyph));
  }

  /* zero instruction length */
  void drop_hints ()
  {
    якщо (!has_instructions_length ()) повернути;
    GlyphHeader &glyph_header = const_cast<GlyphHeader &> (header);
    (HBUINT16 &) StructAtOffset<HBUINT16> (&glyph_header, instruction_len_offset ()) = 0;
  }

  void drop_hints_bytes (hb_bytes_t &dest_start, hb_bytes_t &dest_end) const
  {
    unsigned ціле instructions_len = instructions_length ();
    unsigned ціле glyph_length = length (instructions_len);
    dest_start = bytes.sub_array (0, glyph_length - instructions_len);
    dest_end = bytes.sub_array (glyph_length, bytes.length - glyph_length);
  }

  void set_overlaps_flag ()
  {
    якщо (unlikely (!header.numberOfContours)) повернути;

    unsigned flags_offset = length (instructions_length ());
    якщо (unlikely (flags_offset + 1 > bytes.length)) повернути;

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
    для (unsigned ціле i = 0; i < count;)
    {
      якщо (unlikely (p + 1 > end)) повернути false;
      uint8_t flag = *p++;
      якщо (!extended)
	flag &= ~FLAG_CUBIC;
      points[i++].flag = flag;
      якщо (flag & FLAG_REPEAT)
      {
	якщо (unlikely (p + 1 > end)) повернути false;
	unsigned ціле repeat_count = *p++;
	unsigned stop = hb_min (i + repeat_count, count);
	для (; i < stop; i++)
	  points[i].flag = flag;
      }
    }
    повернути true;
  }

  static bool read_points (const HBUINT8 *&p /* IN/OUT */,
			   hb_array_t<contour_point_t> points_ /* IN/OUT */,
			   const HBUINT8 *end,
			   float contour_point_t::*m,
			   const simple_glyph_flag_t short_flag,
			   const simple_glyph_flag_t same_flag)
  {
    ціле v = 0;

    для (auto &point : points_)
    {
      unsigned flag = point.flag;
      якщо (flag & short_flag)
      {
	якщо (unlikely (p + 1 > end)) повернути false;
	v += (bool(flag & same_flag) * 2 - 1) * *p++;
      }
      інакше
      {
	якщо (!(flag & same_flag))
	{
	  якщо (unlikely (p + HBINT16::static_size > end)) повернути false;
	  v += *(const HBINT16 *) p;
	  p += HBINT16::static_size;
	}
      }
      point.*m = v;
    }
    повернути true;
  }

  bool get_contour_points (contour_point_vector_t &points /* OUT */,
			   bool extended,
			   bool phantom_only = false,
			   int64_t *budget = nullptr) const
  {
    const HBUINT16 *endPtsOfContours = &StructAfter<HBUINT16> (header);
    ціле num_contours = header.numberOfContours;
    assert (num_contours > 0);
    /* One extra item at the end, for the instruction-count below. */
    якщо (unlikely (!bytes.check_range (&endPtsOfContours[num_contours]))) повернути false;
    unsigned ціле num_points = endPtsOfContours[num_contours - 1] + 1;
    якщо (unlikely (num_points < (unsigned) num_contours)) повернути false;

    /* Charge before allocating and reading, so that glyphs that fail
     * later (truncated data, composite caps) still pay for this work. */
    якщо (budget && unlikely (!hb_budget_spend (*budget, HB_BUDGET_1, num_points))) повернути false;

    unsigned old_length = points.length;
    points.alloc (points.length + num_points + 4); // Allocate for phantom points, to avoid a possible copy
    якщо (unlikely (!points.resize_dirty (points.length + num_points))) повернути false;
    auto points_ = points.as_array ().sub_array (old_length);
    якщо (!phantom_only)
      hb_memset (points_.arrayZ, 0, sizeof (contour_point_t) * num_points);
    якщо (phantom_only) повернути true;

    для (ціле i = 0; i < num_contours; i++)
      points_[endPtsOfContours[i]].is_end_point = true;

    /* Skip instructions */
    const HBUINT8 *p = &StructAtOffset<HBUINT8> (&endPtsOfContours[num_contours + 1],
						 endPtsOfContours[num_contours]);

    якщо (unlikely ((const char *) p < bytes.arrayZ)) повернути false; /* Unlikely overflow */
    const HBUINT8 *end = (const HBUINT8 *) (bytes.arrayZ + bytes.length);
    якщо (unlikely (p >= end)) повернути false;

    /* Read x & y coordinates */
    повернути read_flags (p, points_, end, extended)
        && read_points (p, points_, end, &contour_point_t::x,
			FLAG_X_SHORT, FLAG_X_SAME)
	&& read_points (p, points_, end, &contour_point_t::y,
			FLAG_Y_SHORT, FLAG_Y_SAME);
  }

  static void encode_coord (ціле value,
                            unsigned &flag,
                            const simple_glyph_flag_t short_flag,
                            const simple_glyph_flag_t same_flag,
                            hb_vector_t<uint8_t> &coords /* OUT */)
  {
    якщо (value == 0)
    {
      flag |= same_flag;
    }
    інакше якщо (value >= -255 && value <= 255)
    {
      flag |= short_flag;
      якщо (value > 0) flag |= same_flag;
      інакше value = -value;

      coords.arrayZ[coords.length++] = (uint8_t) value;
    }
    інакше
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
    якщо (flag == lastflag && repeat != 255)
    {
      repeat++;
      якщо (repeat == 1)
      {
        /* We know there's room. */
        flags.arrayZ[flags.length++] = flag;
      }
      інакше
      {
        unsigned len = flags.length;
        flags.arrayZ[len-2] = flag | FLAG_REPEAT;
        flags.arrayZ[len-1] = repeat;
      }
    }
    інакше
    {
      repeat = 0;
      flags.arrayZ[flags.length++] = flag;
    }
  }

  bool compile_bytes_with_deltas (const contour_point_vector_t &all_points,
                                  bool no_hinting,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    якщо (header.numberOfContours == 0 || all_points.length <= 4)
    {
      dest_bytes = hb_bytes_t ();
      повернути true;
    }
    unsigned num_points = all_points.length - 4;

    hb_vector_t<uint8_t> flags, x_coords, y_coords;
    якщо (unlikely (!flags.alloc_exact (num_points))) повернути false;
    якщо (unlikely (!x_coords.alloc_exact (2*num_points))) повернути false;
    якщо (unlikely (!y_coords.alloc_exact (2*num_points))) повернути false;

    unsigned lastflag = 255, repeat = 0;
    ціле prev_x = 0, prev_y = 0;

    для (unsigned i = 0; i < num_points; i++)
    {
      unsigned flag = all_points.arrayZ[i].flag;
      flag &= FLAG_ON_CURVE | FLAG_OVERLAP_SIMPLE | FLAG_CUBIC;

      ціле cur_x = roundf (all_points.arrayZ[i].x);
      ціле cur_y = roundf (all_points.arrayZ[i].y);
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

    якщо (!no_hinting)
      total_len += len_instrs;

    char *p = (char *) hb_malloc (total_len);
    якщо (unlikely (!p)) повернути false;

    const char *src = bytes.arrayZ + GlyphHeader::static_size;
    char *cur = p;
    hb_memcpy (p, src, len_before_instrs);

    cur += len_before_instrs;
    src += len_before_instrs;

    якщо (!no_hinting)
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
    повернути true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_SIMPLEGLYPH_HH */

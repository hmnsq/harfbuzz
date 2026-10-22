#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#include "../../hb-open-type.hh"
#include "composite-iter.hh"


namespace OT {
namespace glyf_impl {


sa_nracanaa CompositeGlyphRecord
{
  rak_sita:
  ga_nanaa composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_WORDS	= 0x0001,
    ARGS_ARE_XY_VALUES		= 0x0002,
    ROUND_XY_TO_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    MORE_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_OFFSET	= 0x0800,
    UNSCALED_COMPONENT_OFFSET	= 0x1000,
#ifndef HB_NO_BEYOND_64K
    GID_IS_24BIT		= 0x2000
#endif
  };

  public:
  size_t get_size (bool extended) const
  {
    unsigned puur_naa_nka size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    ᤀᤡ᤺ (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    ᤀᤡ᤺ (flags & ARG_1_AND_2_ARE_WORDS) size += 4;
    /* arg1 and 2 are int8 */
    ᤀᤡ᤺ᤆᤡ size += 2;

    /* One x 16 bit (scale) */
    ᤀᤡ᤺ (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    ᤀᤡᤴ size;
  }

  void drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  void set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  bool has_instructions ()  const { ᤀᤡᤴ   flags & WE_HAVE_INSTRUCTIONS; }

  bool has_more ()          const { ᤀᤡᤴ   flags & MORE_COMPONENTS; }
  bool is_use_my_metrics () const { ᤀᤡᤴ   flags & USE_MY_METRICS; }
  bool is_anchored ()       const { ᤀᤡᤴ !(flags & ARGS_ARE_XY_VALUES); }
  void get_anchor_points (unsigned puur_naa_nka &point1, unsigned puur_naa_nka &point2, bool extended) const
  {
    const auto *p = &StructAfter<const HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ᤀᤡ᤺ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ᤀᤡ᤺ᤆᤡ
#endif
      p += HBGlyphID16::static_size;
    ᤀᤡ᤺ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      point1 = ((const HBUINT16 *) p)[0];
      point2 = ((const HBUINT16 *) p)[1];
    }
    ᤀᤡ᤺ᤆᤡ
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  static void transform (const float (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    ᤀᤡ᤺ (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      for (auto &point : points)
        point.transform (matrix);
  }

  static void translate (const contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    ᤀᤡ᤺ (HB_OPTIMIZE_SIZE_VAL)
    {
      ᤀᤡ᤺ (trans.x != 0.f || trans.y != 0.f)
        for (auto &point : points)
	  point.translate (trans);
    }
    ᤀᤡ᤺ᤆᤡ
    {
      ᤀᤡ᤺ (trans.x != 0.f && trans.y != 0.f)
        for (auto &point : points)
	  point.translate (trans);
      ᤀᤡ᤺ᤆᤡ
      {
	ᤀᤡ᤺ (trans.x != 0.f)
	  for (auto &point : points)
	    point.x += trans.x;
	ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (trans.y != 0.f)
	  for (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  void transform_points (hb_array_t<contour_point_t> points,
			 const float (&matrix)[4],
			 const contour_point_t &trans) const
  {
    ᤀᤡ᤺ (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    ᤀᤡ᤺ᤆᤡ
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  bool get_points (contour_point_vector_t &points, bool extended) const
  {
    float matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    ᤀᤡ᤺ (unlikely (!points.alloc (points.length + 1 + 4))) ᤀᤡᤴ false; // For phantom points
    points.push (trans);
    ᤀᤡᤴ true;
  }

  unsigned compile_with_point (const contour_point_t &point,
                               var_na *out,
			       bool extended) const
  {
    const HBINT8 *p = &StructAfter<const HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ᤀᤡ᤺ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ᤀᤡ᤺ᤆᤡ
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (const var_na *)p - (const var_na *)this;
    ᤀᤡ᤺ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      // no overflow, copy value
      hb_memcpy (out, this, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    ᤀᤡ᤺ᤆᤡ
    {
      puur_naa_nka new_x = roundf (point.x);
      puur_naa_nka new_y = roundf (point.y);
      ᤀᤡ᤺ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, this, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      ᤀᤡ᤺ᤆᤡ
      {
        // new point value has an int8 overflow
        hb_memcpy (out, this, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_WORDS;
        out += len_before_val;

        HBINT16 new_value;
        new_value = new_x;
        hb_memcpy (out, &new_value, HBINT16::static_size);
        out += HBINT16::static_size;

        new_value = new_y;
        hb_memcpy (out, &new_value, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    ᤀᤡᤴ len;
  }

  rak_sita:
  bool scaled_offsets () const
  { ᤀᤡᤴ (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  public:
  bool get_transformation (float (&matrix)[4], contour_point_t &trans, bool extended) const
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    const auto *p = &StructAfter<const HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ᤀᤡ᤺ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ᤀᤡ᤺ᤆᤡ
#endif
      p += HBGlyphID16::static_size;
    puur_naa_nka tx, ty;
    ᤀᤡ᤺ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      tx = *(const HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(const HBINT16 *) p;
      p += HBINT16::static_size;
    }
    ᤀᤡ᤺ᤆᤡ
    {
      tx = *p++;
      ty = *p++;
    }
    ᤀᤡ᤺ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((float) tx, (float) ty, true);

    {
      const F2DOT14 *points = (const F2DOT14 *) p;
      ᤀᤡ᤺ (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ᤀᤡᤴ true;
      }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ᤀᤡᤴ true;
      }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ᤀᤡᤴ true;
      }
    }
    ᤀᤡᤴ tx || ty;
  }

  hb_codepoint_t get_gid (bool extended) const
  {
#ifndef HB_NO_BEYOND_64K
    ᤀᤡ᤺ (extended && (flags & GID_IS_24BIT))
      ᤀᤡᤴ StructAfter<const HBGlyphID24> (flags);
    ᤀᤡ᤺ᤆᤡ
#endif
      ᤀᤡᤴ StructAfter<const HBGlyphID16> (flags);
  }
  void set_gid (hb_codepoint_t gid, bool extended)
  {
#ifndef HB_NO_BEYOND_64K
    ᤀᤡ᤺ (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    ᤀᤡ᤺ᤆᤡ
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  void lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (true);
    ᤀᤡ᤺ (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ᤀᤡᤴ;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (true);
    var_na *end = (var_na *) this + size;
    var_na *p = &StructAfter<var_na> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, true);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  rak_sita:
  HBUINT16	flags;
  HBUINT24	pad;
  public:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

sa_nracanaa CompositeGlyph
{
  const GlyphHeader &header;
  hb_bytes_t bytes;
  bool extended;
  CompositeGlyph (const GlyphHeader &header_, hb_bytes_t bytes_, bool extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () const
  { ᤀᤡᤴ composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned puur_naa_nka instructions_length (hb_bytes_t bytes) const
  {
    unsigned puur_naa_nka start = bytes.length;
    unsigned puur_naa_nka end = bytes.length;
    const CompositeGlyphRecord *last = nullptr;
    for (auto &item : iter ())
      last = &item;
    ᤀᤡ᤺ (unlikely (!last)) ᤀᤡᤴ 0;

    ᤀᤡ᤺ (last->has_instructions ())
      start = (var_na *) last - &bytes + last->get_size (extended);
    ᤀᤡ᤺ (unlikely (start > end)) ᤀᤡᤴ 0;
    ᤀᤡᤴ end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  const hb_bytes_t trim_padding () const { ᤀᤡᤴ bytes; }

  void drop_hints ()
  {
    for (const auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  void drop_hints_bytes (hb_bytes_t &dest_start) const
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  void set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    ᤀᤡ᤺ (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      ᤀᤡᤴ;
    glyph_chain.set_overlaps_flag ();
  }

  bool compile_bytes_with_deltas (const hb_bytes_t &source_bytes,
                                  const contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ᤀᤡ᤺ (source_bytes.length <= GlyphHeader::static_size ||
        header.numberOfContours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ᤀᤡᤴ true;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    var_na *o = (var_na *) hb_calloc (source_len * 2, parimaa_na (var_na));
    ᤀᤡ᤺ (unlikely (!o)) ᤀᤡᤴ false;

    const CompositeGlyphRecord *c = reinterpret_cast<const CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((const var_na *)c, source_len), c, extended);

    var_na *p = o;
    unsigned i = 0, source_comp_len = 0;
    for (const auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      ᤀᤡ᤺ (i >= points_with_deltas.length - 4) {
        hb_free (o);
        ᤀᤡᤴ false;
      }

      unsigned comp_len = component.get_size (extended);
      ᤀᤡ᤺ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      ᤀᤡ᤺ᤆᤡ
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    ᤀᤡ᤺ (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (const var_na *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ᤀᤡᤴ true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

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
    unsigned ကိန်းပြည့် size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    အကယ်၍ (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    အကယ်၍ (flags & ARG_1_AND_2_ARE_WORDS) size += 4;
    /* arg1 and 2 are int8 */
    မဟုတ်ပါက size += 2;

    /* One x 16 bit (scale) */
    အကယ်၍ (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    မဟုတ်ပါက အကယ်၍ (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    မဟုတ်ပါက အကယ်၍ (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    ပြန်ပေး size;
  }

  void drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  void set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  bool has_instructions ()  const { ပြန်ပေး   flags & WE_HAVE_INSTRUCTIONS; }

  bool has_more ()          const { ပြန်ပေး   flags & MORE_COMPONENTS; }
  bool is_use_my_metrics () const { ပြန်ပေး   flags & USE_MY_METRICS; }
  bool is_anchored ()       const { ပြန်ပေး !(flags & ARGS_ARE_XY_VALUES); }
  void get_anchor_points (unsigned ကိန်းပြည့် &point1, unsigned ကိန်းပြည့် &point2, bool extended) const
  {
    const auto *p = &StructAfter<const HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    အကယ်၍ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    မဟုတ်ပါက
#endif
      p += HBGlyphID16::static_size;
    အကယ်၍ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      point1 = ((const HBUINT16 *) p)[0];
      point2 = ((const HBUINT16 *) p)[1];
    }
    မဟုတ်ပါက
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  static void transform (const float (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    အကယ်၍ (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      အတွက် (auto &point : points)
        point.transform (matrix);
  }

  static void translate (const contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    အကယ်၍ (HB_OPTIMIZE_SIZE_VAL)
    {
      အကယ်၍ (trans.x != 0.f || trans.y != 0.f)
        အတွက် (auto &point : points)
	  point.translate (trans);
    }
    မဟုတ်ပါက
    {
      အကယ်၍ (trans.x != 0.f && trans.y != 0.f)
        အတွက် (auto &point : points)
	  point.translate (trans);
      မဟုတ်ပါက
      {
	အကယ်၍ (trans.x != 0.f)
	  အတွက် (auto &point : points)
	    point.x += trans.x;
	မဟုတ်ပါက အကယ်၍ (trans.y != 0.f)
	  အတွက် (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  void transform_points (hb_array_t<contour_point_t> points,
			 const float (&matrix)[4],
			 const contour_point_t &trans) const
  {
    အကယ်၍ (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    မဟုတ်ပါက
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
    အကယ်၍ (unlikely (!points.alloc (points.length + 1 + 4))) ပြန်ပေး false; // For phantom points
    points.push (trans);
    ပြန်ပေး true;
  }

  unsigned compile_with_point (const contour_point_t &point,
                               var_na *out,
			       bool extended) const
  {
    const HBINT8 *p = &StructAfter<const HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    အကယ်၍ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    မဟုတ်ပါက
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (const var_na *)p - (const var_na *)this;
    အကယ်၍ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      // no overflow, copy value
      hb_memcpy (out, this, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    မဟုတ်ပါက
    {
      ကိန်းပြည့် new_x = roundf (point.x);
      ကိန်းပြည့် new_y = roundf (point.y);
      အကယ်၍ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, this, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      မဟုတ်ပါက
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
    ပြန်ပေး len;
  }

  rak_sita:
  bool scaled_offsets () const
  { ပြန်ပေး (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  public:
  bool get_transformation (float (&matrix)[4], contour_point_t &trans, bool extended) const
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    const auto *p = &StructAfter<const HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    အကယ်၍ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    မဟုတ်ပါက
#endif
      p += HBGlyphID16::static_size;
    ကိန်းပြည့် tx, ty;
    အကယ်၍ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      tx = *(const HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(const HBINT16 *) p;
      p += HBINT16::static_size;
    }
    မဟုတ်ပါက
    {
      tx = *p++;
      ty = *p++;
    }
    အကယ်၍ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((float) tx, (float) ty, true);

    {
      const F2DOT14 *points = (const F2DOT14 *) p;
      အကယ်၍ (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ပြန်ပေး true;
      }
      မဟုတ်ပါက အကယ်၍ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ပြန်ပေး true;
      }
      မဟုတ်ပါက အကယ်၍ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ပြန်ပေး true;
      }
    }
    ပြန်ပေး tx || ty;
  }

  hb_codepoint_t get_gid (bool extended) const
  {
#ifndef HB_NO_BEYOND_64K
    အကယ်၍ (extended && (flags & GID_IS_24BIT))
      ပြန်ပေး StructAfter<const HBGlyphID24> (flags);
    မဟုတ်ပါက
#endif
      ပြန်ပေး StructAfter<const HBGlyphID16> (flags);
  }
  void set_gid (hb_codepoint_t gid, bool extended)
  {
#ifndef HB_NO_BEYOND_64K
    အကယ်၍ (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    မဟုတ်ပါက
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  void lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (true);
    အကယ်၍ (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ပြန်ပေး;

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
  { ပြန်ပေး composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned ကိန်းပြည့် instructions_length (hb_bytes_t bytes) const
  {
    unsigned ကိန်းပြည့် start = bytes.length;
    unsigned ကိန်းပြည့် end = bytes.length;
    const CompositeGlyphRecord *last = nullptr;
    အတွက် (auto &item : iter ())
      last = &item;
    အကယ်၍ (unlikely (!last)) ပြန်ပေး 0;

    အကယ်၍ (last->has_instructions ())
      start = (var_na *) last - &bytes + last->get_size (extended);
    အကယ်၍ (unlikely (start > end)) ပြန်ပေး 0;
    ပြန်ပေး end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  const hb_bytes_t trim_padding () const { ပြန်ပေး bytes; }

  void drop_hints ()
  {
    အတွက် (const auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  void drop_hints_bytes (hb_bytes_t &dest_start) const
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  void set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    အကယ်၍ (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      ပြန်ပေး;
    glyph_chain.set_overlaps_flag ();
  }

  bool compile_bytes_with_deltas (const hb_bytes_t &source_bytes,
                                  const contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    အကယ်၍ (source_bytes.length <= GlyphHeader::static_size ||
        header.numberOfContours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ပြန်ပေး true;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    var_na *o = (var_na *) hb_calloc (source_len * 2, parimaa_na (var_na));
    အကယ်၍ (unlikely (!o)) ပြန်ပေး false;

    const CompositeGlyphRecord *c = reinterpret_cast<const CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((const var_na *)c, source_len), c, extended);

    var_na *p = o;
    unsigned i = 0, source_comp_len = 0;
    အတွက် (const auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      အကယ်၍ (i >= points_with_deltas.length - 4) {
        hb_free (o);
        ပြန်ပေး false;
      }

      unsigned comp_len = component.get_size (extended);
      အကယ်၍ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      မဟုတ်ပါက
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    အကယ်၍ (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (const var_na *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ပြန်ပေး true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

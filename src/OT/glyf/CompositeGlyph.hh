#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#شامل "../../hb-open-type.hh"
#شامل "composite-iter.hh"


فضاینام OT {
فضاینام glyf_impl {


ساختار CompositeGlyphRecord
{
  محافظتشده:
  شمارش composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_واژگان	= 0x0001,
    ARGS_ARE_XY_مقادیر		= 0x0002,
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

  عمومی:
  size_t get_size (منطقی extended) ثابت
  {
    unsigned صحیح size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    اگر (flags & ARG_1_AND_2_ARE_واژگان) size += 4;
    /* arg1 and 2 are int8 */
    وگرنه size += 2;

    /* One x 16 bit (scale) */
    اگر (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    وگرنه اگر (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    وگرنه اگر (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    بازگشت size;
  }

  تهی drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  تهی set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقی has_instructions ()  ثابت { بازگشت   flags & WE_HAVE_INSTRUCTIONS; }

  منطقی has_more ()          ثابت { بازگشت   flags & MORE_COMPONENTS; }
  منطقی is_use_my_metrics () ثابت { بازگشت   flags & USE_MY_METRICS; }
  منطقی is_anchored ()       ثابت { بازگشت !(flags & ARGS_ARE_XY_مقادیر); }
  تهی get_anchor_points (unsigned صحیح &point1, unsigned صحیح &point2, منطقی extended) ثابت
  {
    ثابت auto *p = &StructAfter<ثابت HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    وگرنه
#endif
      p += HBGlyphID16::static_size;
    اگر (flags & ARG_1_AND_2_ARE_واژگان)
    {
      point1 = ((ثابت HBUINT16 *) p)[0];
      point2 = ((ثابت HBUINT16 *) p)[1];
    }
    وگرنه
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ایستا تهی transform (ثابت اعشاری (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    اگر (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      برای (auto &point : points)
        point.transform (matrix);
  }

  ایستا تهی translate (ثابت contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    اگر (HB_OPTIMIZE_SIZE_VAL)
    {
      اگر (trans.x != 0.f || trans.y != 0.f)
        برای (auto &point : points)
	  point.translate (trans);
    }
    وگرنه
    {
      اگر (trans.x != 0.f && trans.y != 0.f)
        برای (auto &point : points)
	  point.translate (trans);
      وگرنه
      {
	اگر (trans.x != 0.f)
	  برای (auto &point : points)
	    point.x += trans.x;
	وگرنه اگر (trans.y != 0.f)
	  برای (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  تهی transform_points (hb_array_t<contour_point_t> points,
			 ثابت اعشاری (&matrix)[4],
			 ثابت contour_point_t &trans) ثابت
  {
    اگر (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    وگرنه
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  منطقی get_points (contour_point_vector_t &points, منطقی extended) ثابت
  {
    اعشاری matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    اگر (unlikely (!points.alloc (points.length + 1 + 4))) بازگشت نادرست; // For phantom points
    points.push (trans);
    بازگشت درست;
  }

  unsigned compile_with_point (ثابت contour_point_t &point,
                               نویسه *out,
			       منطقی extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    وگرنه
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (ثابت نویسه *)p - (ثابت نویسه *)این;
    اگر (flags & ARG_1_AND_2_ARE_واژگان)
    {
      // no overflow, copy value
      hb_memcpy (out, این, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    وگرنه
    {
      صحیح new_x = roundf (point.x);
      صحیح new_y = roundf (point.y);
      اگر (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, این, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      وگرنه
      {
        // new point value has an int8 overflow
        hb_memcpy (out, این, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_واژگان;
        out += len_before_val;

        HBINT16 new_مقدار;
        new_مقدار = new_x;
        hb_memcpy (out, &new_مقدار, HBINT16::static_size);
        out += HBINT16::static_size;

        new_مقدار = new_y;
        hb_memcpy (out, &new_مقدار, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    بازگشت len;
  }

  محافظتشده:
  منطقی scaled_offsets () ثابت
  { بازگشت (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  عمومی:
  منطقی get_transformation (اعشاری (&matrix)[4], contour_point_t &trans, منطقی extended) ثابت
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ثابت auto *p = &StructAfter<ثابت HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    وگرنه
#endif
      p += HBGlyphID16::static_size;
    صحیح tx, ty;
    اگر (flags & ARG_1_AND_2_ARE_واژگان)
    {
      tx = *(ثابت HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(ثابت HBINT16 *) p;
      p += HBINT16::static_size;
    }
    وگرنه
    {
      tx = *p++;
      ty = *p++;
    }
    اگر (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((اعشاری) tx, (اعشاری) ty, درست);

    {
      ثابت F2DOT14 *points = (ثابت F2DOT14 *) p;
      اگر (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	بازگشت درست;
      }
      وگرنه اگر (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	بازگشت درست;
      }
      وگرنه اگر (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	بازگشت درست;
      }
    }
    بازگشت tx || ty;
  }

  hb_codepoint_t get_gid (منطقی extended) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24BIT))
      بازگشت StructAfter<ثابت HBGlyphID24> (flags);
    وگرنه
#endif
      بازگشت StructAfter<ثابت HBGlyphID16> (flags);
  }
  تهی set_gid (hb_codepoint_t gid, منطقی extended)
  {
#ifndef HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    وگرنه
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  تهی lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (درست);
    اگر (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      بازگشت;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (درست);
    نویسه *end = (نویسه *) این + size;
    نویسه *p = &StructAfter<نویسه> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, درست);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  محافظتشده:
  HBUINT16	flags;
  HBUINT24	pad;
  عمومی:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

ساختار CompositeGlyph
{
  ثابت GlyphHeader &header;
  hb_bytes_t bytes;
  منطقی extended;
  CompositeGlyph (ثابت GlyphHeader &header_, hb_bytes_t bytes_, منطقی extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ثابت
  { بازگشت composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned صحیح instructions_length (hb_bytes_t bytes) ثابت
  {
    unsigned صحیح start = bytes.length;
    unsigned صحیح end = bytes.length;
    ثابت CompositeGlyphRecord *last = nullptr;
    برای (auto &item : iter ())
      last = &item;
    اگر (unlikely (!last)) بازگشت 0;

    اگر (last->has_instructions ())
      start = (نویسه *) last - &bytes + last->get_size (extended);
    اگر (unlikely (start > end)) بازگشت 0;
    بازگشت end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_bytes_t trim_padding () ثابت { بازگشت bytes; }

  تهی drop_hints ()
  {
    برای (ثابت auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  تهی drop_hints_bytes (hb_bytes_t &dest_start) ثابت
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  تهی set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    اگر (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      بازگشت;
    glyph_chain.set_overlaps_flag ();
  }

  منطقی compile_bytes_with_deltas (ثابت hb_bytes_t &source_bytes,
                                  ثابت contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    اگر (source_bytes.length <= GlyphHeader::static_size ||
        header.عدد_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      بازگشت درست;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    نویسه *o = (نویسه *) hb_calloc (source_len * 2, اندازه (نویسه));
    اگر (unlikely (!o)) بازگشت نادرست;

    ثابت CompositeGlyphRecord *c = reinterpret_cast<ثابت CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((ثابت نویسه *)c, source_len), c, extended);

    نویسه *p = o;
    unsigned i = 0, source_comp_len = 0;
    برای (ثابت auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      اگر (i >= points_with_deltas.length - 4) {
        hb_free (o);
        بازگشت نادرست;
      }

      unsigned comp_len = component.get_size (extended);
      اگر (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      وگرنه
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    اگر (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (ثابت نویسه *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    بازگشت درست;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

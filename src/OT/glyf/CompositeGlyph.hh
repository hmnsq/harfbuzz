#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#ಅನ್ತರ್ಭಾವ "../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "composite-iter.hh"


ನಾಮಾಕಾಶ OT {
ನಾಮಾಕಾಶ glyf_impl {


ಸಂರಚನಾ CompositeGlyphRecord
{
  ರಕ್ಷಿತ:
  ಗಣನಾ composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_ಪದಗಳು	= 0x0001,
    ARGS_ARE_XY_ಮೌಲ್ಯಗಳು		= 0x0002,
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

  ಸಾರ್ವಜನಿಕ:
  size_t get_size (ತಾರ್ಕಿಕ extended) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    ಯದಿ (flags & ARG_1_AND_2_ARE_ಪದಗಳು) size += 4;
    /* arg1 and 2 are int8 */
    ಅನ್ಯಥಾ size += 2;

    /* One x 16 bit (scale) */
    ಯದಿ (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    ಅನ್ಯಥಾ ಯದಿ (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    ಅನ್ಯಥಾ ಯದಿ (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    ಪ್ರತ್ಯಾಗಮ size;
  }

  ಶೂನ್ಯ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  ಶೂನ್ಯ set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  ತಾರ್ಕಿಕ has_instructions ()  ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   flags & WE_HAVE_INSTRUCTIONS; }

  ತಾರ್ಕಿಕ has_more ()          ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   flags & MORE_COMPONENTS; }
  ತಾರ್ಕಿಕ is_use_my_metrics () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ   flags & USE_MY_METRICS; }
  ತಾರ್ಕಿಕ is_anchored ()       ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ !(flags & ARGS_ARE_XY_ಮೌಲ್ಯಗಳು); }
  ಶೂನ್ಯ get_anchor_points (unsigned ಪೂರ್ಣಾಙ್ಕ &point1, unsigned ಪೂರ್ಣಾಙ್ಕ &point2, ತಾರ್ಕಿಕ extended) ಧ್ರುವ
  {
    ಧ್ರುವ auto *p = &StructAfter<ಧ್ರುವ HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ಅನ್ಯಥಾ
#endif
      p += HBGlyphID16::static_size;
    ಯದಿ (flags & ARG_1_AND_2_ARE_ಪದಗಳು)
    {
      point1 = ((ಧ್ರುವ HBUINT16 *) p)[0];
      point2 = ((ಧ್ರುವ HBUINT16 *) p)[1];
    }
    ಅನ್ಯಥಾ
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ಸ್ಥಿರ ಶೂನ್ಯ transform (ಧ್ರುವ ಭಿನ್ನ (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    ಯದಿ (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      ಪ್ರತಿ (auto &point : points)
        point.transform (matrix);
  }

  ಸ್ಥಿರ ಶೂನ್ಯ translate (ಧ್ರುವ contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    ಯದಿ (HB_OPTIMIZE_SIZE_VAL)
    {
      ಯದಿ (trans.x != 0.f || trans.y != 0.f)
        ಪ್ರತಿ (auto &point : points)
	  point.translate (trans);
    }
    ಅನ್ಯಥಾ
    {
      ಯದಿ (trans.x != 0.f && trans.y != 0.f)
        ಪ್ರತಿ (auto &point : points)
	  point.translate (trans);
      ಅನ್ಯಥಾ
      {
	ಯದಿ (trans.x != 0.f)
	  ಪ್ರತಿ (auto &point : points)
	    point.x += trans.x;
	ಅನ್ಯಥಾ ಯದಿ (trans.y != 0.f)
	  ಪ್ರತಿ (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  ಶೂನ್ಯ transform_points (hb_array_t<contour_point_t> points,
			 ಧ್ರುವ ಭಿನ್ನ (&matrix)[4],
			 ಧ್ರುವ contour_point_t &trans) ಧ್ರುವ
  {
    ಯದಿ (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    ಅನ್ಯಥಾ
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  ತಾರ್ಕಿಕ get_points (contour_point_vector_t &points, ತಾರ್ಕಿಕ extended) ಧ್ರುವ
  {
    ಭಿನ್ನ matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    ಯದಿ (unlikely (!points.alloc (points.length + 1 + 4))) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; // For phantom points
    points.push (trans);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }

  unsigned compile_with_point (ಧ್ರುವ contour_point_t &point,
                               ವರ್ಣ *out,
			       ತಾರ್ಕಿಕ extended) ಧ್ರುವ
  {
    ಧ್ರುವ HBINT8 *p = &StructAfter<ಧ್ರುವ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ಅನ್ಯಥಾ
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (ಧ್ರುವ ವರ್ಣ *)p - (ಧ್ರುವ ವರ್ಣ *)ಏತತ್;
    ಯದಿ (flags & ARG_1_AND_2_ARE_ಪದಗಳು)
    {
      // no overflow, copy value
      hb_memcpy (out, ಏತತ್, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ new_x = roundf (point.x);
      ಪೂರ್ಣಾಙ್ಕ new_y = roundf (point.y);
      ಯದಿ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, ಏತತ್, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      ಅನ್ಯಥಾ
      {
        // new point value has an int8 overflow
        hb_memcpy (out, ಏತತ್, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_ಪದಗಳು;
        out += len_before_val;

        HBINT16 new_ಮೌಲ್ಯ;
        new_ಮೌಲ್ಯ = new_x;
        hb_memcpy (out, &new_ಮೌಲ್ಯ, HBINT16::static_size);
        out += HBINT16::static_size;

        new_ಮೌಲ್ಯ = new_y;
        hb_memcpy (out, &new_ಮೌಲ್ಯ, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    ಪ್ರತ್ಯಾಗಮ len;
  }

  ರಕ್ಷಿತ:
  ತಾರ್ಕಿಕ scaled_offsets () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  ಸಾರ್ವಜನಿಕ:
  ತಾರ್ಕಿಕ get_transformation (ಭಿನ್ನ (&matrix)[4], contour_point_t &trans, ತಾರ್ಕಿಕ extended) ಧ್ರುವ
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ಧ್ರುವ auto *p = &StructAfter<ಧ್ರುವ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ಅನ್ಯಥಾ
#endif
      p += HBGlyphID16::static_size;
    ಪೂರ್ಣಾಙ್ಕ tx, ty;
    ಯದಿ (flags & ARG_1_AND_2_ARE_ಪದಗಳು)
    {
      tx = *(ಧ್ರುವ HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(ಧ್ರುವ HBINT16 *) p;
      p += HBINT16::static_size;
    }
    ಅನ್ಯಥಾ
    {
      tx = *p++;
      ty = *p++;
    }
    ಯದಿ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((ಭಿನ್ನ) tx, (ಭಿನ್ನ) ty, ಸತ್ಯ);

    {
      ಧ್ರುವ F2DOT14 *points = (ಧ್ರುವ F2DOT14 *) p;
      ಯದಿ (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
      ಅನ್ಯಥಾ ಯದಿ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
      ಅನ್ಯಥಾ ಯದಿ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
      }
    }
    ಪ್ರತ್ಯಾಗಮ tx || ty;
  }

  hb_codepoint_t get_gid (ತಾರ್ಕಿಕ extended) ಧ್ರುವ
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended && (flags & GID_IS_24BIT))
      ಪ್ರತ್ಯಾಗಮ StructAfter<ಧ್ರುವ HBGlyphID24> (flags);
    ಅನ್ಯಥಾ
#endif
      ಪ್ರತ್ಯಾಗಮ StructAfter<ಧ್ರುವ HBGlyphID16> (flags);
  }
  ಶೂನ್ಯ set_gid (hb_codepoint_t gid, ತಾರ್ಕಿಕ extended)
  {
#ifndef HB_NO_BEYOND_64K
    ಯದಿ (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    ಅನ್ಯಥಾ
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  ಶೂನ್ಯ lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (ಸತ್ಯ);
    ಯದಿ (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ಪ್ರತ್ಯಾಗಮ;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (ಸತ್ಯ);
    ವರ್ಣ *end = (ವರ್ಣ *) ಏತತ್ + size;
    ವರ್ಣ *p = &StructAfter<ವರ್ಣ> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, ಸತ್ಯ);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  ರಕ್ಷಿತ:
  HBUINT16	flags;
  HBUINT24	pad;
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

ಸಂರಚನಾ CompositeGlyph
{
  ಧ್ರುವ GlyphHeader &header;
  hb_bytes_t bytes;
  ತಾರ್ಕಿಕ extended;
  CompositeGlyph (ಧ್ರುವ GlyphHeader &header_, hb_bytes_t bytes_, ತಾರ್ಕಿಕ extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned ಪೂರ್ಣಾಙ್ಕ instructions_length (hb_bytes_t bytes) ಧ್ರುವ
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ start = bytes.length;
    unsigned ಪೂರ್ಣಾಙ್ಕ end = bytes.length;
    ಧ್ರುವ CompositeGlyphRecord *last = nullptr;
    ಪ್ರತಿ (auto &item : iter ())
      last = &item;
    ಯದಿ (unlikely (!last)) ಪ್ರತ್ಯಾಗಮ 0;

    ಯದಿ (last->has_instructions ())
      start = (ವರ್ಣ *) last - &bytes + last->get_size (extended);
    ಯದಿ (unlikely (start > end)) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ಧ್ರುವ hb_bytes_t trim_padding () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ bytes; }

  ಶೂನ್ಯ drop_hints ()
  {
    ಪ್ರತಿ (ಧ್ರುವ auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  ಶೂನ್ಯ drop_hints_bytes (hb_bytes_t &dest_start) ಧ್ರುವ
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  ಶೂನ್ಯ set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    ಯದಿ (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      ಪ್ರತ್ಯಾಗಮ;
    glyph_chain.set_overlaps_flag ();
  }

  ತಾರ್ಕಿಕ compile_bytes_with_deltas (ಧ್ರುವ hb_bytes_t &source_bytes,
                                  ಧ್ರುವ contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ಯದಿ (source_bytes.length <= GlyphHeader::static_size ||
        header.ಸಂಖ್ಯೆ_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    ವರ್ಣ *o = (ವರ್ಣ *) hb_calloc (source_len * 2, ಪರಿಮಾಣ (ವರ್ಣ));
    ಯದಿ (unlikely (!o)) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಧ್ರುವ CompositeGlyphRecord *c = reinterpret_cast<ಧ್ರುವ CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((ಧ್ರುವ ವರ್ಣ *)c, source_len), c, extended);

    ವರ್ಣ *p = o;
    unsigned i = 0, source_comp_len = 0;
    ಪ್ರತಿ (ಧ್ರುವ auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      ಯದಿ (i >= points_with_deltas.length - 4) {
        hb_free (o);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      unsigned comp_len = component.get_size (extended);
      ಯದಿ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      ಅನ್ಯಥಾ
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    ಯದಿ (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (ಧ್ರುವ ವರ್ಣ *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

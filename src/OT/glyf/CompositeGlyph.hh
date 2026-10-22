#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#अन्तर्भाव "../../hb-open-type.hh"
#अन्तर्भाव "composite-iter.hh"


नामाकाश OT {
नामाकाश glyf_impl {


संरचना CompositeGlyphRecord
{
  रक्षित:
  गणना composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_शब्दाः	= 0x0001,
    ARGS_ARE_XY_मानानि		= 0x0002,
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

  सार्वजनिक:
  size_t get_size (तार्किक extended) ध्रुव
  {
    unsigned पूर्णाङ्क size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    सचे (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    सचे (flags & ARG_1_AND_2_ARE_शब्दाः) size += 4;
    /* arg1 and 2 are int8 */
    अञ्ञथा size += 2;

    /* One x 16 bit (scale) */
    सचे (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    अञ्ञथा सचे (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    अञ्ञथा सचे (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    पटिनिवत्त size;
  }

  शून्य drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  शून्य set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  तार्किक has_instructions ()  ध्रुव { पटिनिवत्त   flags & WE_HAVE_INSTRUCTIONS; }

  तार्किक has_more ()          ध्रुव { पटिनिवत्त   flags & MORE_COMPONENTS; }
  तार्किक is_use_my_metrics () ध्रुव { पटिनिवत्त   flags & USE_MY_METRICS; }
  तार्किक is_anchored ()       ध्रुव { पटिनिवत्त !(flags & ARGS_ARE_XY_मानानि); }
  शून्य get_anchor_points (unsigned पूर्णाङ्क &point1, unsigned पूर्णाङ्क &point2, तार्किक extended) ध्रुव
  {
    ध्रुव auto *p = &StructAfter<ध्रुव HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    सचे (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अञ्ञथा
#endif
      p += HBGlyphID16::static_size;
    सचे (flags & ARG_1_AND_2_ARE_शब्दाः)
    {
      point1 = ((ध्रुव HBUINT16 *) p)[0];
      point2 = ((ध्रुव HBUINT16 *) p)[1];
    }
    अञ्ञथा
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  स्थिर शून्य transform (ध्रुव भिन्न (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    सचे (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      अत्थाय (auto &point : points)
        point.transform (matrix);
  }

  स्थिर शून्य translate (ध्रुव contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    सचे (HB_OPTIMIZE_SIZE_VAL)
    {
      सचे (trans.x != 0.f || trans.y != 0.f)
        अत्थाय (auto &point : points)
	  point.translate (trans);
    }
    अञ्ञथा
    {
      सचे (trans.x != 0.f && trans.y != 0.f)
        अत्थाय (auto &point : points)
	  point.translate (trans);
      अञ्ञथा
      {
	सचे (trans.x != 0.f)
	  अत्थाय (auto &point : points)
	    point.x += trans.x;
	अञ्ञथा सचे (trans.y != 0.f)
	  अत्थाय (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  शून्य transform_points (hb_array_t<contour_point_t> points,
			 ध्रुव भिन्न (&matrix)[4],
			 ध्रुव contour_point_t &trans) ध्रुव
  {
    सचे (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    अञ्ञथा
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  तार्किक get_points (contour_point_vector_t &points, तार्किक extended) ध्रुव
  {
    भिन्न matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    सचे (unlikely (!points.alloc (points.length + 1 + 4))) पटिनिवत्त मिच्छा; // For phantom points
    points.push (trans);
    पटिनिवत्त सच्च;
  }

  unsigned compile_with_point (ध्रुव contour_point_t &point,
                               वर्ण *out,
			       तार्किक extended) ध्रुव
  {
    ध्रुव HBINT8 *p = &StructAfter<ध्रुव HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    सचे (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अञ्ञथा
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (ध्रुव वर्ण *)p - (ध्रुव वर्ण *)एतत्;
    सचे (flags & ARG_1_AND_2_ARE_शब्दाः)
    {
      // no overflow, copy value
      hb_memcpy (out, एतत्, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    अञ्ञथा
    {
      पूर्णाङ्क new_x = roundf (point.x);
      पूर्णाङ्क new_y = roundf (point.y);
      सचे (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, एतत्, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      अञ्ञथा
      {
        // new point value has an int8 overflow
        hb_memcpy (out, एतत्, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_शब्दाः;
        out += len_before_val;

        HBINT16 new_मानम्;
        new_मानम् = new_x;
        hb_memcpy (out, &new_मानम्, HBINT16::static_size);
        out += HBINT16::static_size;

        new_मानम् = new_y;
        hb_memcpy (out, &new_मानम्, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    पटिनिवत्त len;
  }

  रक्षित:
  तार्किक scaled_offsets () ध्रुव
  { पटिनिवत्त (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  सार्वजनिक:
  तार्किक get_transformation (भिन्न (&matrix)[4], contour_point_t &trans, तार्किक extended) ध्रुव
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ध्रुव auto *p = &StructAfter<ध्रुव HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    सचे (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अञ्ञथा
#endif
      p += HBGlyphID16::static_size;
    पूर्णाङ्क tx, ty;
    सचे (flags & ARG_1_AND_2_ARE_शब्दाः)
    {
      tx = *(ध्रुव HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(ध्रुव HBINT16 *) p;
      p += HBINT16::static_size;
    }
    अञ्ञथा
    {
      tx = *p++;
      ty = *p++;
    }
    सचे (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((भिन्न) tx, (भिन्न) ty, सच्च);

    {
      ध्रुव F2DOT14 *points = (ध्रुव F2DOT14 *) p;
      सचे (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	पटिनिवत्त सच्च;
      }
      अञ्ञथा सचे (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	पटिनिवत्त सच्च;
      }
      अञ्ञथा सचे (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	पटिनिवत्त सच्च;
      }
    }
    पटिनिवत्त tx || ty;
  }

  hb_codepoint_t get_gid (तार्किक extended) ध्रुव
  {
#ifndef HB_NO_BEYOND_64K
    सचे (extended && (flags & GID_IS_24BIT))
      पटिनिवत्त StructAfter<ध्रुव HBGlyphID24> (flags);
    अञ्ञथा
#endif
      पटिनिवत्त StructAfter<ध्रुव HBGlyphID16> (flags);
  }
  शून्य set_gid (hb_codepoint_t gid, तार्किक extended)
  {
#ifndef HB_NO_BEYOND_64K
    सचे (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    अञ्ञथा
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  शून्य lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (सच्च);
    सचे (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      पटिनिवत्त;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (सच्च);
    वर्ण *end = (वर्ण *) एतत् + size;
    वर्ण *p = &StructAfter<वर्ण> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, सच्च);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  रक्षित:
  HBUINT16	flags;
  HBUINT24	pad;
  सार्वजनिक:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

संरचना CompositeGlyph
{
  ध्रुव GlyphHeader &header;
  hb_bytes_t bytes;
  तार्किक extended;
  CompositeGlyph (ध्रुव GlyphHeader &header_, hb_bytes_t bytes_, तार्किक extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ध्रुव
  { पटिनिवत्त composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned पूर्णाङ्क instructions_length (hb_bytes_t bytes) ध्रुव
  {
    unsigned पूर्णाङ्क start = bytes.length;
    unsigned पूर्णाङ्क end = bytes.length;
    ध्रुव CompositeGlyphRecord *last = nullptr;
    अत्थाय (auto &item : iter ())
      last = &item;
    सचे (unlikely (!last)) पटिनिवत्त 0;

    सचे (last->has_instructions ())
      start = (वर्ण *) last - &bytes + last->get_size (extended);
    सचे (unlikely (start > end)) पटिनिवत्त 0;
    पटिनिवत्त end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ध्रुव hb_bytes_t trim_padding () ध्रुव { पटिनिवत्त bytes; }

  शून्य drop_hints ()
  {
    अत्थाय (ध्रुव auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  शून्य drop_hints_bytes (hb_bytes_t &dest_start) ध्रुव
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  शून्य set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    सचे (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      पटिनिवत्त;
    glyph_chain.set_overlaps_flag ();
  }

  तार्किक compile_bytes_with_deltas (ध्रुव hb_bytes_t &source_bytes,
                                  ध्रुव contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    सचे (source_bytes.length <= GlyphHeader::static_size ||
        header.सङ्ख्या_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      पटिनिवत्त सच्च;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    वर्ण *o = (वर्ण *) hb_calloc (source_len * 2, परिमाण (वर्ण));
    सचे (unlikely (!o)) पटिनिवत्त मिच्छा;

    ध्रुव CompositeGlyphRecord *c = reinterpret_cast<ध्रुव CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((ध्रुव वर्ण *)c, source_len), c, extended);

    वर्ण *p = o;
    unsigned i = 0, source_comp_len = 0;
    अत्थाय (ध्रुव auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      सचे (i >= points_with_deltas.length - 4) {
        hb_free (o);
        पटिनिवत्त मिच्छा;
      }

      unsigned comp_len = component.get_size (extended);
      सचे (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      अञ्ञथा
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    सचे (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (ध्रुव वर्ण *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    पटिनिवत्त सच्च;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

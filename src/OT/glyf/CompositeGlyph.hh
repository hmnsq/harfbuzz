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
    ARG_1_AND_2_ARE_शब्दहरू	= 0x0001,
    ARGS_ARE_XY_मानहरू		= 0x0002,
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
  माप_प्रकार get_size (तार्किक extended) अचर
  {
    अचिन्हित पूर्णाङ्क size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    यदि (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    यदि (flags & ARG_1_AND_2_ARE_शब्दहरू) size += 4;
    /* arg1 and 2 are int8 */
    अन्यथा size += 2;

    /* One x 16 bit (scale) */
    यदि (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    अन्यथा यदि (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    अन्यथा यदि (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    फर्काऊ size;
  }

  शून्य drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  शून्य set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  तार्किक has_instructions ()  अचर { फर्काऊ   flags & WE_HAVE_INSTRUCTIONS; }

  तार्किक has_more ()          अचर { फर्काऊ   flags & MORE_COMPONENTS; }
  तार्किक is_use_my_metrics () अचर { फर्काऊ   flags & USE_MY_METRICS; }
  तार्किक is_anchored ()       अचर { फर्काऊ !(flags & ARGS_ARE_XY_मानहरू); }
  शून्य get_anchor_points (अचिन्हित पूर्णाङ्क &point1, अचिन्हित पूर्णाङ्क &point2, तार्किक extended) अचर
  {
    अचर स्वतः *p = &StructAfter<अचर HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    यदि (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अन्यथा
#endif
      p += HBGlyphID16::static_size;
    यदि (flags & ARG_1_AND_2_ARE_शब्दहरू)
    {
      point1 = ((अचर HBUINT16 *) p)[0];
      point2 = ((अचर HBUINT16 *) p)[1];
    }
    अन्यथा
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  स्थिर शून्य transform (अचर भिन्न (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    यदि (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      लागि (स्वतः &point : points)
        point.transform (matrix);
  }

  स्थिर शून्य translate (अचर contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    यदि (HB_OPTIMIZE_SIZE_VAL)
    {
      यदि (trans.x != 0.f || trans.y != 0.f)
        लागि (स्वतः &point : points)
	  point.translate (trans);
    }
    अन्यथा
    {
      यदि (trans.x != 0.f && trans.y != 0.f)
        लागि (स्वतः &point : points)
	  point.translate (trans);
      अन्यथा
      {
	यदि (trans.x != 0.f)
	  लागि (स्वतः &point : points)
	    point.x += trans.x;
	अन्यथा यदि (trans.y != 0.f)
	  लागि (स्वतः &point : points)
	    point.y += trans.y;
      }
    }
  }

  शून्य transform_points (hb_array_t<contour_point_t> points,
			 अचर भिन्न (&matrix)[4],
			 अचर contour_point_t &trans) अचर
  {
    यदि (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    अन्यथा
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  तार्किक get_points (contour_point_vector_t &points, तार्किक extended) अचर
  {
    भिन्न matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    यदि (unlikely (!points.alloc (points.length + 1 + 4))) फर्काऊ असत्य; // For phantom points
    points.push (trans);
    फर्काऊ सत्य;
  }

  अचिन्हित compile_with_point (अचर contour_point_t &point,
                               वर्ण *out,
			       तार्किक extended) अचर
  {
    अचर HBINT8 *p = &StructAfter<अचर HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    यदि (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अन्यथा
#endif
      p += HBGlyphID16::static_size;

    अचिन्हित len = get_size (extended);
    अचिन्हित len_before_val = (अचर वर्ण *)p - (अचर वर्ण *)एतत्;
    यदि (flags & ARG_1_AND_2_ARE_शब्दहरू)
    {
      // no overflow, copy value
      hb_memcpy (out, एतत्, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    अन्यथा
    {
      पूर्णाङ्क new_x = roundf (point.x);
      पूर्णाङ्क new_y = roundf (point.y);
      यदि (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, एतत्, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      अन्यथा
      {
        // new point value has an int8 overflow
        hb_memcpy (out, एतत्, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_शब्दहरू;
        out += len_before_val;

        HBINT16 new_मान;
        new_मान = new_x;
        hb_memcpy (out, &new_मान, HBINT16::static_size);
        out += HBINT16::static_size;

        new_मान = new_y;
        hb_memcpy (out, &new_मान, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    फर्काऊ len;
  }

  रक्षित:
  तार्किक scaled_offsets () अचर
  { फर्काऊ (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  सार्वजनिक:
  तार्किक get_transformation (भिन्न (&matrix)[4], contour_point_t &trans, तार्किक extended) अचर
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    अचर स्वतः *p = &StructAfter<अचर HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    यदि (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अन्यथा
#endif
      p += HBGlyphID16::static_size;
    पूर्णाङ्क tx, ty;
    यदि (flags & ARG_1_AND_2_ARE_शब्दहरू)
    {
      tx = *(अचर HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(अचर HBINT16 *) p;
      p += HBINT16::static_size;
    }
    अन्यथा
    {
      tx = *p++;
      ty = *p++;
    }
    यदि (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((भिन्न) tx, (भिन्न) ty, सत्य);

    {
      अचर F2DOT14 *points = (अचर F2DOT14 *) p;
      यदि (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	फर्काऊ सत्य;
      }
      अन्यथा यदि (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	फर्काऊ सत्य;
      }
      अन्यथा यदि (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	फर्काऊ सत्य;
      }
    }
    फर्काऊ tx || ty;
  }

  hb_codepoint_t get_gid (तार्किक extended) अचर
  {
#ifndef HB_NO_BEYOND_64K
    यदि (extended && (flags & GID_IS_24BIT))
      फर्काऊ StructAfter<अचर HBGlyphID24> (flags);
    अन्यथा
#endif
      फर्काऊ StructAfter<अचर HBGlyphID16> (flags);
  }
  शून्य set_gid (hb_codepoint_t gid, तार्किक extended)
  {
#ifndef HB_NO_BEYOND_64K
    यदि (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    अन्यथा
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  शून्य lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (सत्य);
    यदि (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      फर्काऊ;

    /* Lower the flag and move the rest of the struct down. */

    अचिन्हित size = get_size (सत्य);
    वर्ण *end = (वर्ण *) एतत् + size;
    वर्ण *p = &StructAfter<वर्ण> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, सत्य);

    स_हटाओ (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
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
  अचर GlyphHeader &header;
  hb_bytes_t bytes;
  तार्किक extended;
  CompositeGlyph (अचर GlyphHeader &header_, hb_bytes_t bytes_, तार्किक extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () अचर
  { फर्काऊ composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  अचिन्हित पूर्णाङ्क instructions_length (hb_bytes_t bytes) अचर
  {
    अचिन्हित पूर्णाङ्क start = bytes.length;
    अचिन्हित पूर्णाङ्क end = bytes.length;
    अचर CompositeGlyphRecord *last = nullptr;
    लागि (स्वतः &item : iter ())
      last = &item;
    यदि (unlikely (!last)) फर्काऊ 0;

    यदि (last->has_instructions ())
      start = (वर्ण *) last - &bytes + last->get_size (extended);
    यदि (unlikely (start > end)) फर्काऊ 0;
    फर्काऊ end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  अचर hb_bytes_t trim_padding () अचर { फर्काऊ bytes; }

  शून्य drop_hints ()
  {
    लागि (अचर स्वतः &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  शून्य drop_hints_bytes (hb_bytes_t &dest_start) अचर
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  शून्य set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    यदि (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      फर्काऊ;
    glyph_chain.set_overlaps_flag ();
  }

  तार्किक compile_bytes_with_deltas (अचर hb_bytes_t &source_bytes,
                                  अचर contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    यदि (source_bytes.length <= GlyphHeader::static_size ||
        header.सङ्ख्या_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      फर्काऊ सत्य;
    }

    अचिन्हित source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    वर्ण *o = (वर्ण *) hb_calloc (source_len * 2, परिमाण (वर्ण));
    यदि (unlikely (!o)) फर्काऊ असत्य;

    अचर CompositeGlyphRecord *c = reinterpret_cast<अचर CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    स्वतः it = composite_iter_t (hb_bytes_t ((अचर वर्ण *)c, source_len), c, extended);

    वर्ण *p = o;
    अचिन्हित i = 0, source_comp_len = 0;
    लागि (अचर स्वतः &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      यदि (i >= points_with_deltas.length - 4) {
        hb_free (o);
        फर्काऊ असत्य;
      }

      अचिन्हित comp_len = component.get_size (extended);
      यदि (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      अन्यथा
      {
        अचिन्हित new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    यदि (source_len > source_comp_len)
    {
      अचिन्हित instr_len = source_len - source_comp_len;
      hb_memcpy (p, (अचर वर्ण *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    अचिन्हित len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    फर्काऊ सत्य;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

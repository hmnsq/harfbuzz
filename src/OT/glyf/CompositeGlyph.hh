#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#अन्तर्भाव "../../hb-open-type.hh"
#अन्तर्भाव "composite-iter.hh"


नामाकाश OT {
नामाकाश glyf_impl {


संरचना CompositeGlyphRecord
{
  संरक्षित:
  गणना composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_शब्द	= 0x0001,
    ARGS_ARE_XY_मूल्ये		= 0x0002,
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
  माप_प्रकार get_size (तार्किक extended) स्थिरांक
  {
    अचिन्हित पूर्णांक size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    जर (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    जर (flags & ARG_1_AND_2_ARE_शब्द) size += 4;
    /* arg1 and 2 are int8 */
    अन्यथा size += 2;

    /* One x 16 bit (scale) */
    जर (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    अन्यथा जर (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    अन्यथा जर (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    परत size;
  }

  शून्य drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  शून्य set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  तार्किक has_instructions ()  स्थिरांक { परत   flags & WE_HAVE_INSTRUCTIONS; }

  तार्किक has_more ()          स्थिरांक { परत   flags & MORE_COMPONENTS; }
  तार्किक is_use_my_metrics () स्थिरांक { परत   flags & USE_MY_METRICS; }
  तार्किक is_anchored ()       स्थिरांक { परत !(flags & ARGS_ARE_XY_मूल्ये); }
  शून्य get_anchor_points (अचिन्हित पूर्णांक &point1, अचिन्हित पूर्णांक &point2, तार्किक extended) स्थिरांक
  {
    स्थिरांक स्वतः *p = &StructAfter<स्थिरांक HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    जर (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अन्यथा
#endif
      p += HBGlyphID16::static_size;
    जर (flags & ARG_1_AND_2_ARE_शब्द)
    {
      point1 = ((स्थिरांक HBUINT16 *) p)[0];
      point2 = ((स्थिरांक HBUINT16 *) p)[1];
    }
    अन्यथा
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  स्थिर शून्य transform (स्थिरांक दशांश (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    जर (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      साठी (स्वतः &point : points)
        point.transform (matrix);
  }

  स्थिर शून्य translate (स्थिरांक contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    जर (HB_OPTIMIZE_SIZE_VAL)
    {
      जर (trans.x != 0.f || trans.y != 0.f)
        साठी (स्वतः &point : points)
	  point.translate (trans);
    }
    अन्यथा
    {
      जर (trans.x != 0.f && trans.y != 0.f)
        साठी (स्वतः &point : points)
	  point.translate (trans);
      अन्यथा
      {
	जर (trans.x != 0.f)
	  साठी (स्वतः &point : points)
	    point.x += trans.x;
	अन्यथा जर (trans.y != 0.f)
	  साठी (स्वतः &point : points)
	    point.y += trans.y;
      }
    }
  }

  शून्य transform_points (hb_array_t<contour_point_t> points,
			 स्थिरांक दशांश (&matrix)[4],
			 स्थिरांक contour_point_t &trans) स्थिरांक
  {
    जर (scaled_offsets ())
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

  तार्किक get_points (contour_point_vector_t &points, तार्किक extended) स्थिरांक
  {
    दशांश matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    जर (unlikely (!points.alloc (points.length + 1 + 4))) परत असत्य; // For phantom points
    points.push (trans);
    परत सत्य;
  }

  अचिन्हित compile_with_point (स्थिरांक contour_point_t &point,
                               अक्षर *out,
			       तार्किक extended) स्थिरांक
  {
    स्थिरांक HBINT8 *p = &StructAfter<स्थिरांक HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    जर (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अन्यथा
#endif
      p += HBGlyphID16::static_size;

    अचिन्हित len = get_size (extended);
    अचिन्हित len_before_val = (स्थिरांक अक्षर *)p - (स्थिरांक अक्षर *)हे;
    जर (flags & ARG_1_AND_2_ARE_शब्द)
    {
      // no overflow, copy value
      hb_memcpy (out, हे, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    अन्यथा
    {
      पूर्णांक new_x = roundf (point.x);
      पूर्णांक new_y = roundf (point.y);
      जर (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, हे, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      अन्यथा
      {
        // new point value has an int8 overflow
        hb_memcpy (out, हे, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_शब्द;
        out += len_before_val;

        HBINT16 new_मूल्य;
        new_मूल्य = new_x;
        hb_memcpy (out, &new_मूल्य, HBINT16::static_size);
        out += HBINT16::static_size;

        new_मूल्य = new_y;
        hb_memcpy (out, &new_मूल्य, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    परत len;
  }

  संरक्षित:
  तार्किक scaled_offsets () स्थिरांक
  { परत (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  सार्वजनिक:
  तार्किक get_transformation (दशांश (&matrix)[4], contour_point_t &trans, तार्किक extended) स्थिरांक
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    स्थिरांक स्वतः *p = &StructAfter<स्थिरांक HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    जर (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    अन्यथा
#endif
      p += HBGlyphID16::static_size;
    पूर्णांक tx, ty;
    जर (flags & ARG_1_AND_2_ARE_शब्द)
    {
      tx = *(स्थिरांक HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(स्थिरांक HBINT16 *) p;
      p += HBINT16::static_size;
    }
    अन्यथा
    {
      tx = *p++;
      ty = *p++;
    }
    जर (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((दशांश) tx, (दशांश) ty, सत्य);

    {
      स्थिरांक F2DOT14 *points = (स्थिरांक F2DOT14 *) p;
      जर (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	परत सत्य;
      }
      अन्यथा जर (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	परत सत्य;
      }
      अन्यथा जर (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	परत सत्य;
      }
    }
    परत tx || ty;
  }

  hb_codepoint_t get_gid (तार्किक extended) स्थिरांक
  {
#ifndef HB_NO_BEYOND_64K
    जर (extended && (flags & GID_IS_24BIT))
      परत StructAfter<स्थिरांक HBGlyphID24> (flags);
    अन्यथा
#endif
      परत StructAfter<स्थिरांक HBGlyphID16> (flags);
  }
  शून्य set_gid (hb_codepoint_t gid, तार्किक extended)
  {
#ifndef HB_NO_BEYOND_64K
    जर (extended && (flags & GID_IS_24BIT))
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
    जर (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      परत;

    /* Lower the flag and move the rest of the struct down. */

    अचिन्हित size = get_size (सत्य);
    अक्षर *end = (अक्षर *) हे + size;
    अक्षर *p = &StructAfter<अक्षर> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, सत्य);

    स_हटाओ (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  संरक्षित:
  HBUINT16	flags;
  HBUINT24	pad;
  सार्वजनिक:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

संरचना CompositeGlyph
{
  स्थिरांक GlyphHeader &header;
  hb_bytes_t bytes;
  तार्किक extended;
  CompositeGlyph (स्थिरांक GlyphHeader &header_, hb_bytes_t bytes_, तार्किक extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () स्थिरांक
  { परत composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  अचिन्हित पूर्णांक instructions_length (hb_bytes_t bytes) स्थिरांक
  {
    अचिन्हित पूर्णांक start = bytes.length;
    अचिन्हित पूर्णांक end = bytes.length;
    स्थिरांक CompositeGlyphRecord *last = nullptr;
    साठी (स्वतः &item : iter ())
      last = &item;
    जर (unlikely (!last)) परत 0;

    जर (last->has_instructions ())
      start = (अक्षर *) last - &bytes + last->get_size (extended);
    जर (unlikely (start > end)) परत 0;
    परत end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  स्थिरांक hb_bytes_t trim_padding () स्थिरांक { परत bytes; }

  शून्य drop_hints ()
  {
    साठी (स्थिरांक स्वतः &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  शून्य drop_hints_bytes (hb_bytes_t &dest_start) स्थिरांक
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  शून्य set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    जर (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      परत;
    glyph_chain.set_overlaps_flag ();
  }

  तार्किक compile_bytes_with_deltas (स्थिरांक hb_bytes_t &source_bytes,
                                  स्थिरांक contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    जर (source_bytes.length <= GlyphHeader::static_size ||
        header.संख्या_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      परत सत्य;
    }

    अचिन्हित source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    अक्षर *o = (अक्षर *) hb_calloc (source_len * 2, आकार (अक्षर));
    जर (unlikely (!o)) परत असत्य;

    स्थिरांक CompositeGlyphRecord *c = reinterpret_cast<स्थिरांक CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    स्वतः it = composite_iter_t (hb_bytes_t ((स्थिरांक अक्षर *)c, source_len), c, extended);

    अक्षर *p = o;
    अचिन्हित i = 0, source_comp_len = 0;
    साठी (स्थिरांक स्वतः &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      जर (i >= points_with_deltas.length - 4) {
        hb_free (o);
        परत असत्य;
      }

      अचिन्हित comp_len = component.get_size (extended);
      जर (component.is_anchored ())
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
    जर (source_len > source_comp_len)
    {
      अचिन्हित instr_len = source_len - source_comp_len;
      hb_memcpy (p, (स्थिरांक अक्षर *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    अचिन्हित len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    परत सत्य;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

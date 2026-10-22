#अगर_अघोषित OT_GLYF_COMPOSITEGLYPH_HH
#घोषणा OT_GLYF_COMPOSITEGLYPH_HH


#समावेश "../../hb-open-type.hh"
#समावेश "composite-iter.hh"


namespace OT {
namespace glyf_impl {


काष्ठा CompositeGlyphRecord
{
  रक्षित:
  क्रमागत composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_शब्दगण	= 0x0001,
    ARGS_ARE_XY_मूल्यावली		= 0x0002,
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
#अगर_अघोषित HB_NO_BEYOND_64K
    GID_IS_24BIT		= 0x2000
#पूर्ण_अगर
  };

  खुला:
  माप_प्रकार get_आकार (bool extended) स्थिर
  {
    अचिन्हित पूर्णांक आकार = min_आकार;
    /* glyphIndex is 24bit instead of 16bit */
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended && (flags & GID_IS_24BIT)) आकार += HBGlyphID24::static_आकार - HBGlyphID16::static_आकार;
#पूर्ण_अगर
    /* arg1 and 2 are int16 */
    अगर (flags & ARG_1_AND_2_ARE_शब्दगण) आकार += 4;
    /* arg1 and 2 are int8 */
    अन्यथा आकार += 2;

    /* One x 16 bit (scale) */
    अगर (flags & WE_HAVE_A_SCALE) आकार += 2;
    /* Two x 16 bit (xscale, yscale) */
    अन्यथा अगर (flags & WE_HAVE_AN_X_AND_Y_SCALE) आकार += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    अन्यथा अगर (flags & WE_HAVE_A_TWO_BY_TWO) आकार += 8;

    वापस आकार;
  }

  व्योम drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  व्योम set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  bool has_instructions ()  स्थिर { वापस   flags & WE_HAVE_INSTRUCTIONS; }

  bool has_more ()          स्थिर { वापस   flags & MORE_COMPONENTS; }
  bool is_use_my_metrics () स्थिर { वापस   flags & USE_MY_METRICS; }
  bool is_anchored ()       स्थिर { वापस !(flags & ARGS_ARE_XY_मूल्यावली); }
  व्योम get_anchor_points (अचिन्हित पूर्णांक &point1, अचिन्हित पूर्णांक &point2, bool extended) स्थिर
  {
    स्थिर स्वतः *p = &StructAfter<स्थिर HBUINT8> (flags);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_आकार;
    अन्यथा
#पूर्ण_अगर
      p += HBGlyphID16::static_आकार;
    अगर (flags & ARG_1_AND_2_ARE_शब्दगण)
    {
      point1 = ((स्थिर HBUINT16 *) p)[0];
      point2 = ((स्थिर HBUINT16 *) p)[1];
    }
    अन्यथा
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  अटल व्योम transform (स्थिर भग्न (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    अगर (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      क्रम (स्वतः &point : points)
        point.transform (matrix);
  }

  अटल व्योम translate (स्थिर contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    अगर (HB_OPTIMIZE_आकार_VAL)
    {
      अगर (trans.x != 0.f || trans.y != 0.f)
        क्रम (स्वतः &point : points)
	  point.translate (trans);
    }
    अन्यथा
    {
      अगर (trans.x != 0.f && trans.y != 0.f)
        क्रम (स्वतः &point : points)
	  point.translate (trans);
      अन्यथा
      {
	अगर (trans.x != 0.f)
	  क्रम (स्वतः &point : points)
	    point.x += trans.x;
	अन्यथा अगर (trans.y != 0.f)
	  क्रम (स्वतः &point : points)
	    point.y += trans.y;
      }
    }
  }

  व्योम transform_points (hb_array_t<contour_point_t> points,
			 स्थिर भग्न (&matrix)[4],
			 स्थिर contour_point_t &trans) स्थिर
  {
    अगर (scaled_offsets ())
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

  bool get_points (contour_point_vector_t &points, bool extended) स्थिर
  {
    भग्न matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    अगर (unlikely (!points.alloc (points.लंबाई + 1 + 4))) वापस false; // For phantom points
    points.push (trans);
    वापस true;
  }

  अचिन्हित compile_with_point (स्थिर contour_point_t &point,
                               अक्षर *out,
			       bool extended) स्थिर
  {
    स्थिर HBINT8 *p = &StructAfter<स्थिर HBINT8> (flags);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_आकार;
    अन्यथा
#पूर्ण_अगर
      p += HBGlyphID16::static_आकार;

    अचिन्हित len = get_आकार (extended);
    अचिन्हित len_before_val = (स्थिर अक्षर *)p - (स्थिर अक्षर *)यह;
    अगर (flags & ARG_1_AND_2_ARE_शब्दगण)
    {
      // no overflow, copy value
      hb_memcpy (out, यह, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    अन्यथा
    {
      पूर्णांक new_x = roundf (point.x);
      पूर्णांक new_y = roundf (point.y);
      अगर (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, यह, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      अन्यथा
      {
        // new point value has an int8 overflow
        hb_memcpy (out, यह, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_शब्दगण;
        out += len_before_val;

        HBINT16 new_मूल्य;
        new_मूल्य = new_x;
        hb_memcpy (out, &new_मूल्य, HBINT16::static_आकार);
        out += HBINT16::static_आकार;

        new_मूल्य = new_y;
        hb_memcpy (out, &new_मूल्य, HBINT16::static_आकार);
        out += HBINT16::static_आकार;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    वापस len;
  }

  रक्षित:
  bool scaled_offsets () स्थिर
  { वापस (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  खुला:
  bool get_transformation (भग्न (&matrix)[4], contour_point_t &trans, bool extended) स्थिर
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    स्थिर स्वतः *p = &StructAfter<स्थिर HBINT8> (flags);
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_आकार;
    अन्यथा
#पूर्ण_अगर
      p += HBGlyphID16::static_आकार;
    पूर्णांक tx, ty;
    अगर (flags & ARG_1_AND_2_ARE_शब्दगण)
    {
      tx = *(स्थिर HBINT16 *) p;
      p += HBINT16::static_आकार;
      ty = *(स्थिर HBINT16 *) p;
      p += HBINT16::static_आकार;
    }
    अन्यथा
    {
      tx = *p++;
      ty = *p++;
    }
    अगर (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((भग्न) tx, (भग्न) ty, true);

    {
      स्थिर F2DOT14 *points = (स्थिर F2DOT14 *) p;
      अगर (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	वापस true;
      }
      अन्यथा अगर (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	वापस true;
      }
      अन्यथा अगर (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	वापस true;
      }
    }
    वापस tx || ty;
  }

  hb_codepoint_t get_gid (bool extended) स्थिर
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended && (flags & GID_IS_24BIT))
      वापस StructAfter<स्थिर HBGlyphID24> (flags);
    अन्यथा
#पूर्ण_अगर
      वापस StructAfter<स्थिर HBGlyphID16> (flags);
  }
  व्योम set_gid (hb_codepoint_t gid, bool extended)
  {
#अगर_अघोषित HB_NO_BEYOND_64K
    अगर (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    अन्यथा
#पूर्ण_अगर
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#अगर_अघोषित HB_NO_BEYOND_64K
  व्योम lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (true);
    अगर (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      वापस;

    /* Lower the flag and move the rest of the struct down. */

    अचिन्हित आकार = get_आकार (true);
    अक्षर *end = (अक्षर *) यह + आकार;
    अक्षर *p = &StructAfter<अक्षर> (flags);
    p += HBGlyphID24::static_आकार;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, true);

    स_हटाओ (p - HBGlyphID24::static_आकार + HBGlyphID16::static_आकार, p, end - p);
  }
#पूर्ण_अगर

  रक्षित:
  HBUINT16	flags;
  HBUINT24	pad;
  खुला:
  DEFINE_आकार_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

काष्ठा CompositeGlyph
{
  स्थिर GlyphHeader &header;
  hb_bytes_t bytes;
  bool extended;
  CompositeGlyph (स्थिर GlyphHeader &header_, hb_bytes_t bytes_, bool extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () स्थिर
  { वापस composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  अचिन्हित पूर्णांक instructions_लंबाई (hb_bytes_t bytes) स्थिर
  {
    अचिन्हित पूर्णांक start = bytes.लंबाई;
    अचिन्हित पूर्णांक end = bytes.लंबाई;
    स्थिर CompositeGlyphRecord *last = nullptr;
    क्रम (स्वतः &item : iter ())
      last = &item;
    अगर (unlikely (!last)) वापस 0;

    अगर (last->has_instructions ())
      start = (अक्षर *) last - &bytes + last->get_आकार (extended);
    अगर (unlikely (start > end)) वापस 0;
    वापस end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  स्थिर hb_bytes_t trim_padding () स्थिर { वापस bytes; }

  व्योम drop_hints ()
  {
    क्रम (स्थिर स्वतः &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  व्योम drop_hints_bytes (hb_bytes_t &dest_start) स्थिर
  { dest_start = bytes.sub_array (0, bytes.लंबाई - instructions_लंबाई (bytes)); }

  व्योम set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    अगर (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_आकार))
      वापस;
    glyph_chain.set_overlaps_flag ();
  }

  bool compile_bytes_with_deltas (स्थिर hb_bytes_t &source_bytes,
                                  स्थिर contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    अगर (source_bytes.लंबाई <= GlyphHeader::static_आकार ||
        header.संख्या_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      वापस true;
    }

    अचिन्हित source_len = source_bytes.लंबाई - GlyphHeader::static_आकार;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    अक्षर *o = (अक्षर *) hb_calloc (source_len * 2, माप (अक्षर));
    अगर (unlikely (!o)) वापस false;

    स्थिर CompositeGlyphRecord *c = reinterpret_cast<स्थिर CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_आकार);
    स्वतः it = composite_iter_t (hb_bytes_t ((स्थिर अक्षर *)c, source_len), c, extended);

    अक्षर *p = o;
    अचिन्हित i = 0, source_comp_len = 0;
    क्रम (स्थिर स्वतः &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      अगर (i >= points_with_deltas.लंबाई - 4) {
        hb_free (o);
        वापस false;
      }

      अचिन्हित comp_len = component.get_आकार (extended);
      अगर (component.is_anchored ())
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
    अगर (source_len > source_comp_len)
    {
      अचिन्हित instr_len = source_len - source_comp_len;
      hb_memcpy (p, (स्थिर अक्षर *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    अचिन्हित len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    वापस true;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#पूर्ण_अगर /* OT_GLYF_COMPOSITEGLYPH_HH */

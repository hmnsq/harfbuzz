#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#ضمن "../../hb-open-type.hh"
#ضمن "composite-iter.hh"


نطاق OT {
نطاق glyf_impl {


بنية CompositeGlyphRecord
{
  محمي:
  تعداد composite_glyph_flag_t
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

  عام:
  size_t get_size (منطقي extended) ثابت
  {
    unsigned صحيح size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    ܐܢ (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    ܐܢ (flags & ARG_1_AND_2_ARE_WORDS) size += 4;
    /* arg1 and 2 are int8 */
    ܐܚܪܢܐ size += 2;

    /* One x 16 bit (scale) */
    ܐܢ (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    ܐܚܪܢܐ ܐܢ (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    ܐܚܪܢܐ ܐܢ (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    ܦܢܐ size;
  }

  فراغ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  فراغ set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقي has_instructions ()  ثابت { ܦܢܐ   flags & WE_HAVE_INSTRUCTIONS; }

  منطقي has_more ()          ثابت { ܦܢܐ   flags & MORE_COMPONENTS; }
  منطقي is_use_my_metrics () ثابت { ܦܢܐ   flags & USE_MY_METRICS; }
  منطقي is_anchored ()       ثابت { ܦܢܐ !(flags & ARGS_ARE_XY_VALUES); }
  فراغ get_anchor_points (unsigned صحيح &point1, unsigned صحيح &point2, منطقي extended) ثابت
  {
    ثابت auto *p = &StructAfter<ثابت HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ܐܢ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ܐܚܪܢܐ
#endif
      p += HBGlyphID16::static_size;
    ܐܢ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      point1 = ((ثابت HBUINT16 *) p)[0];
      point2 = ((ثابت HBUINT16 *) p)[1];
    }
    ܐܚܪܢܐ
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ساكن فراغ transform (ثابت عائم (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    ܐܢ (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      لكل (auto &point : points)
        point.transform (matrix);
  }

  ساكن فراغ translate (ثابت contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    ܐܢ (HB_OPTIMIZE_SIZE_VAL)
    {
      ܐܢ (trans.x != 0.f || trans.y != 0.f)
        لكل (auto &point : points)
	  point.translate (trans);
    }
    ܐܚܪܢܐ
    {
      ܐܢ (trans.x != 0.f && trans.y != 0.f)
        لكل (auto &point : points)
	  point.translate (trans);
      ܐܚܪܢܐ
      {
	ܐܢ (trans.x != 0.f)
	  لكل (auto &point : points)
	    point.x += trans.x;
	ܐܚܪܢܐ ܐܢ (trans.y != 0.f)
	  لكل (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  فراغ transform_points (hb_array_t<contour_point_t> points,
			 ثابت عائم (&matrix)[4],
			 ثابت contour_point_t &trans) ثابت
  {
    ܐܢ (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    ܐܚܪܢܐ
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  منطقي get_points (contour_point_vector_t &points, منطقي extended) ثابت
  {
    عائم matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    ܐܢ (unlikely (!points.alloc (points.length + 1 + 4))) ܦܢܐ ܕܓܠܐ; // For phantom points
    points.push (trans);
    ܦܢܐ ܫܪܪܐ;
  }

  unsigned compile_with_point (ثابت contour_point_t &point,
                               حرف *out,
			       منطقي extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ܐܢ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ܐܚܪܢܐ
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (ثابت حرف *)p - (ثابت حرف *)هذا;
    ܐܢ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      // no overflow, copy value
      hb_memcpy (out, هذا, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    ܐܚܪܢܐ
    {
      صحيح new_x = roundf (point.x);
      صحيح new_y = roundf (point.y);
      ܐܢ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, هذا, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      ܐܚܪܢܐ
      {
        // new point value has an int8 overflow
        hb_memcpy (out, هذا, len_before_val);
        
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
    ܦܢܐ len;
  }

  محمي:
  منطقي scaled_offsets () ثابت
  { ܦܢܐ (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  عام:
  منطقي get_transformation (عائم (&matrix)[4], contour_point_t &trans, منطقي extended) ثابت
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ثابت auto *p = &StructAfter<ثابت HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ܐܢ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ܐܚܪܢܐ
#endif
      p += HBGlyphID16::static_size;
    صحيح tx, ty;
    ܐܢ (flags & ARG_1_AND_2_ARE_WORDS)
    {
      tx = *(ثابت HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(ثابت HBINT16 *) p;
      p += HBINT16::static_size;
    }
    ܐܚܪܢܐ
    {
      tx = *p++;
      ty = *p++;
    }
    ܐܢ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((عائم) tx, (عائم) ty, ܫܪܪܐ);

    {
      ثابت F2DOT14 *points = (ثابت F2DOT14 *) p;
      ܐܢ (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ܦܢܐ ܫܪܪܐ;
      }
      ܐܚܪܢܐ ܐܢ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ܦܢܐ ܫܪܪܐ;
      }
      ܐܚܪܢܐ ܐܢ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ܦܢܐ ܫܪܪܐ;
      }
    }
    ܦܢܐ tx || ty;
  }

  hb_codepoint_t get_gid (منطقي extended) ثابت
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (extended && (flags & GID_IS_24BIT))
      ܦܢܐ StructAfter<ثابت HBGlyphID24> (flags);
    ܐܚܪܢܐ
#endif
      ܦܢܐ StructAfter<ثابت HBGlyphID16> (flags);
  }
  فراغ set_gid (hb_codepoint_t gid, منطقي extended)
  {
#ifndef HB_NO_BEYOND_64K
    ܐܢ (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    ܐܚܪܢܐ
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  فراغ lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (ܫܪܪܐ);
    ܐܢ (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ܦܢܐ;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (ܫܪܪܐ);
    حرف *end = (حرف *) هذا + size;
    حرف *p = &StructAfter<حرف> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, ܫܪܪܐ);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  محمي:
  HBUINT16	flags;
  HBUINT24	pad;
  عام:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

بنية CompositeGlyph
{
  ثابت GlyphHeader &header;
  hb_bytes_t bytes;
  منطقي extended;
  CompositeGlyph (ثابت GlyphHeader &header_, hb_bytes_t bytes_, منطقي extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ثابت
  { ܦܢܐ composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned صحيح instructions_length (hb_bytes_t bytes) ثابت
  {
    unsigned صحيح start = bytes.length;
    unsigned صحيح end = bytes.length;
    ثابت CompositeGlyphRecord *last = nullptr;
    لكل (auto &item : iter ())
      last = &item;
    ܐܢ (unlikely (!last)) ܦܢܐ 0;

    ܐܢ (last->has_instructions ())
      start = (حرف *) last - &bytes + last->get_size (extended);
    ܐܢ (unlikely (start > end)) ܦܢܐ 0;
    ܦܢܐ end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_bytes_t trim_padding () ثابت { ܦܢܐ bytes; }

  فراغ drop_hints ()
  {
    لكل (ثابت auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  فراغ drop_hints_bytes (hb_bytes_t &dest_start) ثابت
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  فراغ set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    ܐܢ (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      ܦܢܐ;
    glyph_chain.set_overlaps_flag ();
  }

  منطقي compile_bytes_with_deltas (ثابت hb_bytes_t &source_bytes,
                                  ثابت contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ܐܢ (source_bytes.length <= GlyphHeader::static_size ||
        header.numberOfContours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ܦܢܐ ܫܪܪܐ;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    حرف *o = (حرف *) hb_calloc (source_len * 2, حجم (حرف));
    ܐܢ (unlikely (!o)) ܦܢܐ ܕܓܠܐ;

    ثابت CompositeGlyphRecord *c = reinterpret_cast<ثابت CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((ثابت حرف *)c, source_len), c, extended);

    حرف *p = o;
    unsigned i = 0, source_comp_len = 0;
    لكل (ثابت auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      ܐܢ (i >= points_with_deltas.length - 4) {
        hb_free (o);
        ܦܢܐ ܕܓܠܐ;
      }

      unsigned comp_len = component.get_size (extended);
      ܐܢ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      ܐܚܪܢܐ
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    ܐܢ (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (ثابت حرف *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ܦܢܐ ܫܪܪܐ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

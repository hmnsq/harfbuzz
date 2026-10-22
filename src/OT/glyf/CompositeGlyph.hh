#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#অন্তর্ভাব "../../hb-open-type.hh"
#অন্তর্ভাব "composite-iter.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


কাঠামো CompositeGlyphRecord
{
  সুরক্ষিত:
  গণনা composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_শব্দগুলি	= 0x0001,
    ARGS_ARE_XY_মানসমূহ		= 0x0002,
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

  সর্বজনীন:
  size_t get_আকার (যৌক্তিক extended) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা আকার_2 = min_আকার;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT)) আকার_2 += HBGlyphID24::static_আকার - HBGlyphID16::static_আকার;
#endif
    /* arg1 and 2 are int16 */
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি) আকার_2 += 4;
    /* arg1 and 2 are int8 */
    অন্যথা আকার_2 += 2;

    /* One x 16 bit (scale) */
    যদি (flags & WE_HAVE_A_SCALE) আকার_2 += 2;
    /* Two x 16 bit (xscale, yscale) */
    অন্যথা যদি (flags & WE_HAVE_AN_X_AND_Y_SCALE) আকার_2 += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    অন্যথা যদি (flags & WE_HAVE_A_TWO_BY_TWO) আকার_2 += 8;

    ফেরত আকার_2;
  }

  শূন্য drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  শূন্য set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  যৌক্তিক has_instructions ()  ধ্রুবক { ফেরত   flags & WE_HAVE_INSTRUCTIONS; }

  যৌক্তিক has_more ()          ধ্রুবক { ফেরত   flags & MORE_COMPONENTS; }
  যৌক্তিক is_use_my_metrics () ধ্রুবক { ফেরত   flags & USE_MY_METRICS; }
  যৌক্তিক is_anchored ()       ধ্রুবক { ফেরত !(flags & ARGS_ARE_XY_মানসমূহ); }
  শূন্য get_anchor_points (unsigned পূর্ণসংখ্যা &point1, unsigned পূর্ণসংখ্যা &point2, যৌক্তিক extended) ধ্রুবক
  {
    ধ্রুবক auto *p = &StructAfter<ধ্রুবক HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_আকার;
    অন্যথা
#endif
      p += HBGlyphID16::static_আকার;
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি)
    {
      point1 = ((ধ্রুবক HBUINT16 *) p)[0];
      point2 = ((ধ্রুবক HBUINT16 *) p)[1];
    }
    অন্যথা
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  স্থির শূন্য transform (ধ্রুবক দশমিক (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    যদি (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      জন্য (auto &point : points)
        point.transform (matrix);
  }

  স্থির শূন্য translate (ধ্রুবক contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    যদি (HB_OPTIMIZE_আকার_VAL)
    {
      যদি (trans.x != 0.f || trans.y != 0.f)
        জন্য (auto &point : points)
	  point.translate (trans);
    }
    অন্যথা
    {
      যদি (trans.x != 0.f && trans.y != 0.f)
        জন্য (auto &point : points)
	  point.translate (trans);
      অন্যথা
      {
	যদি (trans.x != 0.f)
	  জন্য (auto &point : points)
	    point.x += trans.x;
	অন্যথা যদি (trans.y != 0.f)
	  জন্য (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  শূন্য transform_points (hb_array_t<contour_point_t> points,
			 ধ্রুবক দশমিক (&matrix)[4],
			 ধ্রুবক contour_point_t &trans) ধ্রুবক
  {
    যদি (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    অন্যথা
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  যৌক্তিক get_points (contour_point_vector_t &points, যৌক্তিক extended) ধ্রুবক
  {
    দশমিক matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    যদি (unlikely (!points.alloc (points.দৈর্ঘ্য + 1 + 4))) ফেরত মিথ্যা; // For phantom points
    points.push (trans);
    ফেরত সত্য;
  }

  unsigned compile_with_point (ধ্রুবক contour_point_t &point,
                               অক্ষর *out,
			       যৌক্তিক extended) ধ্রুবক
  {
    ধ্রুবক HBINT8 *p = &StructAfter<ধ্রুবক HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_আকার;
    অন্যথা
#endif
      p += HBGlyphID16::static_আকার;

    unsigned len = get_আকার (extended);
    unsigned len_before_val = (ধ্রুবক অক্ষর *)p - (ধ্রুবক অক্ষর *)এই;
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি)
    {
      // no overflow, copy value
      hb_memcpy (out, এই, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    অন্যথা
    {
      পূর্ণসংখ্যা new_x = roundf (point.x);
      পূর্ণসংখ্যা new_y = roundf (point.y);
      যদি (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, এই, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      অন্যথা
      {
        // new point value has an int8 overflow
        hb_memcpy (out, এই, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_শব্দগুলি;
        out += len_before_val;

        HBINT16 new_মান;
        new_মান = new_x;
        hb_memcpy (out, &new_মান, HBINT16::static_আকার);
        out += HBINT16::static_আকার;

        new_মান = new_y;
        hb_memcpy (out, &new_মান, HBINT16::static_আকার);
        out += HBINT16::static_আকার;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    ফেরত len;
  }

  সুরক্ষিত:
  যৌক্তিক scaled_offsets () ধ্রুবক
  { ফেরত (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  সর্বজনীন:
  যৌক্তিক get_transformation (দশমিক (&matrix)[4], contour_point_t &trans, যৌক্তিক extended) ধ্রুবক
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ধ্রুবক auto *p = &StructAfter<ধ্রুবক HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_আকার;
    অন্যথা
#endif
      p += HBGlyphID16::static_আকার;
    পূর্ণসংখ্যা tx, ty;
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি)
    {
      tx = *(ধ্রুবক HBINT16 *) p;
      p += HBINT16::static_আকার;
      ty = *(ধ্রুবক HBINT16 *) p;
      p += HBINT16::static_আকার;
    }
    অন্যথা
    {
      tx = *p++;
      ty = *p++;
    }
    যদি (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((দশমিক) tx, (দশমিক) ty, সত্য);

    {
      ধ্রুবক F2DOT14 *points = (ধ্রুবক F2DOT14 *) p;
      যদি (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ফেরত সত্য;
      }
      অন্যথা যদি (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ফেরত সত্য;
      }
      অন্যথা যদি (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ফেরত সত্য;
      }
    }
    ফেরত tx || ty;
  }

  hb_codepoint_t get_gid (যৌক্তিক extended) ধ্রুবক
  {
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      ফেরত StructAfter<ধ্রুবক HBGlyphID24> (flags);
    অন্যথা
#endif
      ফেরত StructAfter<ধ্রুবক HBGlyphID16> (flags);
  }
  শূন্য set_gid (hb_codepoint_t gid, যৌক্তিক extended)
  {
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    অন্যথা
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  শূন্য lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (সত্য);
    যদি (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ফেরত;

    /* Lower the flag and move the rest of the struct down. */

    unsigned আকার_2 = get_আকার (সত্য);
    অক্ষর *end = (অক্ষর *) এই + আকার_2;
    অক্ষর *p = &StructAfter<অক্ষর> (flags);
    p += HBGlyphID24::static_আকার;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, সত্য);

    memmove (p - HBGlyphID24::static_আকার + HBGlyphID16::static_আকার, p, end - p);
  }
#endif

  সুরক্ষিত:
  HBUINT16	flags;
  HBUINT24	pad;
  সর্বজনীন:
  DEFINE_আকার_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

কাঠামো CompositeGlyph
{
  ধ্রুবক GlyphHeader &header;
  hb_bytes_t bytes;
  যৌক্তিক extended;
  CompositeGlyph (ধ্রুবক GlyphHeader &header_, hb_bytes_t bytes_, যৌক্তিক extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ধ্রুবক
  { ফেরত composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned পূর্ণসংখ্যা instructions_দৈর্ঘ্য (hb_bytes_t bytes) ধ্রুবক
  {
    unsigned পূর্ণসংখ্যা start = bytes.দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা end = bytes.দৈর্ঘ্য;
    ধ্রুবক CompositeGlyphRecord *last = nullptr;
    জন্য (auto &item : iter ())
      last = &item;
    যদি (unlikely (!last)) ফেরত 0;

    যদি (last->has_instructions ())
      start = (অক্ষর *) last - &bytes + last->get_আকার (extended);
    যদি (unlikely (start > end)) ফেরত 0;
    ফেরত end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ধ্রুবক hb_bytes_t trim_padding () ধ্রুবক { ফেরত bytes; }

  শূন্য drop_hints ()
  {
    জন্য (ধ্রুবক auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  শূন্য drop_hints_bytes (hb_bytes_t &dest_start) ধ্রুবক
  { dest_start = bytes.sub_array (0, bytes.দৈর্ঘ্য - instructions_দৈর্ঘ্য (bytes)); }

  শূন্য set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    যদি (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_আকার))
      ফেরত;
    glyph_chain.set_overlaps_flag ();
  }

  যৌক্তিক compile_bytes_with_deltas (ধ্রুবক hb_bytes_t &source_bytes,
                                  ধ্রুবক contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    যদি (source_bytes.দৈর্ঘ্য <= GlyphHeader::static_আকার ||
        header.সংখ্যা_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ফেরত সত্য;
    }

    unsigned source_len = source_bytes.দৈর্ঘ্য - GlyphHeader::static_আকার;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    অক্ষর *o = (অক্ষর *) hb_calloc (source_len * 2, আকার (অক্ষর));
    যদি (unlikely (!o)) ফেরত মিথ্যা;

    ধ্রুবক CompositeGlyphRecord *c = reinterpret_cast<ধ্রুবক CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_আকার);
    auto it = composite_iter_t (hb_bytes_t ((ধ্রুবক অক্ষর *)c, source_len), c, extended);

    অক্ষর *p = o;
    unsigned i = 0, source_comp_len = 0;
    জন্য (ধ্রুবক auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      যদি (i >= points_with_deltas.দৈর্ঘ্য - 4) {
        hb_free (o);
        ফেরত মিথ্যা;
      }

      unsigned comp_len = component.get_আকার (extended);
      যদি (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      অন্যথা
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    যদি (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (ধ্রুবক অক্ষর *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ফেরত সত্য;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

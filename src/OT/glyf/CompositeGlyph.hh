#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#অন্তর্ভাৱ "../../hb-open-type.hh"
#অন্তর্ভাৱ "composite-iter.hh"


নামাকাশ OT {
নামাকাশ glyf_impl {


সংরচনা CompositeGlyphRecord
{
  রক্ষিত:
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

  সার্ৱজনিক:
  size_t get_size (তার্কিক extended) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি) size += 4;
    /* arg1 and 2 are int8 */
    অন্যথা size += 2;

    /* One x 16 bit (scale) */
    যদি (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    অন্যথা যদি (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    অন্যথা যদি (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    ঘূৰাই size;
  }

  শূন্য drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  শূন্য set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  তার্কিক has_instructions ()  ধ্রুৱ { ঘূৰাই   flags & WE_HAVE_INSTRUCTIONS; }

  তার্কিক has_more ()          ধ্রুৱ { ঘূৰাই   flags & MORE_COMPONENTS; }
  তার্কিক is_use_my_metrics () ধ্রুৱ { ঘূৰাই   flags & USE_MY_METRICS; }
  তার্কিক is_anchored ()       ধ্রুৱ { ঘূৰাই !(flags & ARGS_ARE_XY_মানসমূহ); }
  শূন্য get_anchor_points (unsigned পূর্ণাঙ্ক &point1, unsigned পূর্ণাঙ্ক &point2, তার্কিক extended) ধ্রুৱ
  {
    ধ্রুৱ auto *p = &StructAfter<ধ্রুৱ HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    অন্যথা
#endif
      p += HBGlyphID16::static_size;
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি)
    {
      point1 = ((ধ্রুৱ HBUINT16 *) p)[0];
      point2 = ((ধ্রুৱ HBUINT16 *) p)[1];
    }
    অন্যথা
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  স্থির শূন্য transform (ধ্রুৱ ভিন্ন (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    যদি (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      বাবে (auto &point : points)
        point.transform (matrix);
  }

  স্থির শূন্য translate (ধ্রুৱ contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    যদি (HB_OPTIMIZE_SIZE_VAL)
    {
      যদি (trans.x != 0.f || trans.y != 0.f)
        বাবে (auto &point : points)
	  point.translate (trans);
    }
    অন্যথা
    {
      যদি (trans.x != 0.f && trans.y != 0.f)
        বাবে (auto &point : points)
	  point.translate (trans);
      অন্যথা
      {
	যদি (trans.x != 0.f)
	  বাবে (auto &point : points)
	    point.x += trans.x;
	অন্যথা যদি (trans.y != 0.f)
	  বাবে (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  শূন্য transform_points (hb_array_t<contour_point_t> points,
			 ধ্রুৱ ভিন্ন (&matrix)[4],
			 ধ্রুৱ contour_point_t &trans) ধ্রুৱ
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

  তার্কিক get_points (contour_point_vector_t &points, তার্কিক extended) ধ্রুৱ
  {
    ভিন্ন matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    যদি (unlikely (!points.alloc (points.length + 1 + 4))) ঘূৰাই মিছা; // For phantom points
    points.push (trans);
    ঘূৰাই সঁচা;
  }

  unsigned compile_with_point (ধ্রুৱ contour_point_t &point,
                               ৱর্ণ *out,
			       তার্কিক extended) ধ্রুৱ
  {
    ধ্রুৱ HBINT8 *p = &StructAfter<ধ্রুৱ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    অন্যথা
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (ধ্রুৱ ৱর্ণ *)p - (ধ্রুৱ ৱর্ণ *)এতত্;
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি)
    {
      // no overflow, copy value
      hb_memcpy (out, এতত্, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    অন্যথা
    {
      পূর্ণাঙ্ক new_x = roundf (point.x);
      পূর্ণাঙ্ক new_y = roundf (point.y);
      যদি (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, এতত্, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      অন্যথা
      {
        // new point value has an int8 overflow
        hb_memcpy (out, এতত্, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_শব্দগুলি;
        out += len_before_val;

        HBINT16 new_মান;
        new_মান = new_x;
        hb_memcpy (out, &new_মান, HBINT16::static_size);
        out += HBINT16::static_size;

        new_মান = new_y;
        hb_memcpy (out, &new_মান, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    ঘূৰাই len;
  }

  রক্ষিত:
  তার্কিক scaled_offsets () ধ্রুৱ
  { ঘূৰাই (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  সার্ৱজনিক:
  তার্কিক get_transformation (ভিন্ন (&matrix)[4], contour_point_t &trans, তার্কিক extended) ধ্রুৱ
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ধ্রুৱ auto *p = &StructAfter<ধ্রুৱ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    অন্যথা
#endif
      p += HBGlyphID16::static_size;
    পূর্ণাঙ্ক tx, ty;
    যদি (flags & ARG_1_AND_2_ARE_শব্দগুলি)
    {
      tx = *(ধ্রুৱ HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(ধ্রুৱ HBINT16 *) p;
      p += HBINT16::static_size;
    }
    অন্যথা
    {
      tx = *p++;
      ty = *p++;
    }
    যদি (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((ভিন্ন) tx, (ভিন্ন) ty, সঁচা);

    {
      ধ্রুৱ F2DOT14 *points = (ধ্রুৱ F2DOT14 *) p;
      যদি (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ঘূৰাই সঁচা;
      }
      অন্যথা যদি (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ঘূৰাই সঁচা;
      }
      অন্যথা যদি (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ঘূৰাই সঁচা;
      }
    }
    ঘূৰাই tx || ty;
  }

  hb_codepoint_t get_gid (তার্কিক extended) ধ্রুৱ
  {
#ifndef HB_NO_BEYOND_64K
    যদি (extended && (flags & GID_IS_24BIT))
      ঘূৰাই StructAfter<ধ্রুৱ HBGlyphID24> (flags);
    অন্যথা
#endif
      ঘূৰাই StructAfter<ধ্রুৱ HBGlyphID16> (flags);
  }
  শূন্য set_gid (hb_codepoint_t gid, তার্কিক extended)
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
    hb_codepoint_t gid = get_gid (সঁচা);
    যদি (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ঘূৰাই;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (সঁচা);
    ৱর্ণ *end = (ৱর্ণ *) এতত্ + size;
    ৱর্ণ *p = &StructAfter<ৱর্ণ> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, সঁচা);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  রক্ষিত:
  HBUINT16	flags;
  HBUINT24	pad;
  সার্ৱজনিক:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

সংরচনা CompositeGlyph
{
  ধ্রুৱ GlyphHeader &header;
  hb_bytes_t bytes;
  তার্কিক extended;
  CompositeGlyph (ধ্রুৱ GlyphHeader &header_, hb_bytes_t bytes_, তার্কিক extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ধ্রুৱ
  { ঘূৰাই composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned পূর্ণাঙ্ক instructions_length (hb_bytes_t bytes) ধ্রুৱ
  {
    unsigned পূর্ণাঙ্ক start = bytes.length;
    unsigned পূর্ণাঙ্ক end = bytes.length;
    ধ্রুৱ CompositeGlyphRecord *last = nullptr;
    বাবে (auto &item : iter ())
      last = &item;
    যদি (unlikely (!last)) ঘূৰাই 0;

    যদি (last->has_instructions ())
      start = (ৱর্ণ *) last - &bytes + last->get_size (extended);
    যদি (unlikely (start > end)) ঘূৰাই 0;
    ঘূৰাই end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ধ্রুৱ hb_bytes_t trim_padding () ধ্রুৱ { ঘূৰাই bytes; }

  শূন্য drop_hints ()
  {
    বাবে (ধ্রুৱ auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  শূন্য drop_hints_bytes (hb_bytes_t &dest_start) ধ্রুৱ
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  শূন্য set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    যদি (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      ঘূৰাই;
    glyph_chain.set_overlaps_flag ();
  }

  তার্কিক compile_bytes_with_deltas (ধ্রুৱ hb_bytes_t &source_bytes,
                                  ধ্রুৱ contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    যদি (source_bytes.length <= GlyphHeader::static_size ||
        header.সংখ্যা_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ঘূৰাই সঁচা;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    ৱর্ণ *o = (ৱর্ণ *) hb_calloc (source_len * 2, পরিমাণ (ৱর্ণ));
    যদি (unlikely (!o)) ঘূৰাই মিছা;

    ধ্রুৱ CompositeGlyphRecord *c = reinterpret_cast<ধ্রুৱ CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((ধ্রুৱ ৱর্ণ *)c, source_len), c, extended);

    ৱর্ণ *p = o;
    unsigned i = 0, source_comp_len = 0;
    বাবে (ধ্রুৱ auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      যদি (i >= points_with_deltas.length - 4) {
        hb_free (o);
        ঘূৰাই মিছা;
      }

      unsigned comp_len = component.get_size (extended);
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
      hb_memcpy (p, (ধ্রুৱ ৱর্ণ *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ঘূৰাই সঁচা;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

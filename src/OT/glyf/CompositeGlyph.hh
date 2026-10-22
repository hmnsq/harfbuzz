#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#ଅନ୍ତର୍ଭାଵ "../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "composite-iter.hh"


ନାମାକାଶ OT {
ନାମାକାଶ glyf_impl {


ସଂରଚନା CompositeGlyphRecord
{
  ସୁରକ୍ଷିତ:
  ଗଣନା composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_ଶବ୍ଦଗୁଡ଼ିକ	= 0x0001,
    ARGS_ARE_XY_ମୂଲ୍ୟଗୁଡ଼ିକ		= 0x0002,
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

  ସର୍ବସାଧାରଣ:
  size_t get_size (ତାର୍କିକ extended) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    ଯଦି (flags & ARG_1_AND_2_ARE_ଶବ୍ଦଗୁଡ଼ିକ) size += 4;
    /* arg1 and 2 are int8 */
    ଅନ୍ୟଥା size += 2;

    /* One x 16 bit (scale) */
    ଯଦି (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    ଅନ୍ୟଥା ଯଦି (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    ଅନ୍ୟଥା ଯଦି (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    ଫେରନ୍ତୁ size;
  }

  ଶୂନ୍ୟ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  ଶୂନ୍ୟ set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  ତାର୍କିକ has_instructions ()  ଅଚଳ { ଫେରନ୍ତୁ   flags & WE_HAVE_INSTRUCTIONS; }

  ତାର୍କିକ has_more ()          ଅଚଳ { ଫେରନ୍ତୁ   flags & MORE_COMPONENTS; }
  ତାର୍କିକ is_use_my_metrics () ଅଚଳ { ଫେରନ୍ତୁ   flags & USE_MY_METRICS; }
  ତାର୍କିକ is_anchored ()       ଅଚଳ { ଫେରନ୍ତୁ !(flags & ARGS_ARE_XY_ମୂଲ୍ୟଗୁଡ଼ିକ); }
  ଶୂନ୍ୟ get_anchor_points (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &point1, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା &point2, ତାର୍କିକ extended) ଅଚଳ
  {
    ଅଚଳ auto *p = &StructAfter<ଅଚଳ HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ଅନ୍ୟଥା
#endif
      p += HBGlyphID16::static_size;
    ଯଦି (flags & ARG_1_AND_2_ARE_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      point1 = ((ଅଚଳ HBUINT16 *) p)[0];
      point2 = ((ଅଚଳ HBUINT16 *) p)[1];
    }
    ଅନ୍ୟଥା
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ସ୍ଥିର ଶୂନ୍ୟ transform (ଅଚଳ ଦଶମିକ (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    ଯଦି (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      ପାଇଁ (auto &point : points)
        point.transform (matrix);
  }

  ସ୍ଥିର ଶୂନ୍ୟ translate (ଅଚଳ contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    ଯଦି (HB_OPTIMIZE_SIZE_VAL)
    {
      ଯଦି (trans.x != 0.f || trans.y != 0.f)
        ପାଇଁ (auto &point : points)
	  point.translate (trans);
    }
    ଅନ୍ୟଥା
    {
      ଯଦି (trans.x != 0.f && trans.y != 0.f)
        ପାଇଁ (auto &point : points)
	  point.translate (trans);
      ଅନ୍ୟଥା
      {
	ଯଦି (trans.x != 0.f)
	  ପାଇଁ (auto &point : points)
	    point.x += trans.x;
	ଅନ୍ୟଥା ଯଦି (trans.y != 0.f)
	  ପାଇଁ (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  ଶୂନ୍ୟ transform_points (hb_array_t<contour_point_t> points,
			 ଅଚଳ ଦଶମିକ (&matrix)[4],
			 ଅଚଳ contour_point_t &trans) ଅଚଳ
  {
    ଯଦି (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    ଅନ୍ୟଥା
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  ତାର୍କିକ get_points (contour_point_vector_t &points, ତାର୍କିକ extended) ଅଚଳ
  {
    ଦଶମିକ matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    ଯଦି (unlikely (!points.alloc (points.length + 1 + 4))) ଫେରନ୍ତୁ ମିଥ୍ୟା; // For phantom points
    points.push (trans);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }

  unsigned compile_with_point (ଅଚଳ contour_point_t &point,
                               ଅକ୍ଷର *out,
			       ତାର୍କିକ extended) ଅଚଳ
  {
    ଅଚଳ HBINT8 *p = &StructAfter<ଅଚଳ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ଅନ୍ୟଥା
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (ଅଚଳ ଅକ୍ଷର *)p - (ଅଚଳ ଅକ୍ଷର *)ଏହି;
    ଯଦି (flags & ARG_1_AND_2_ARE_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      // no overflow, copy value
      hb_memcpy (out, ଏହି, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_x = roundf (point.x);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_y = roundf (point.y);
      ଯଦି (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, ଏହି, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      ଅନ୍ୟଥା
      {
        // new point value has an int8 overflow
        hb_memcpy (out, ଏହି, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_ଶବ୍ଦଗୁଡ଼ିକ;
        out += len_before_val;

        HBINT16 new_ମୂଲ୍ୟ;
        new_ମୂଲ୍ୟ = new_x;
        hb_memcpy (out, &new_ମୂଲ୍ୟ, HBINT16::static_size);
        out += HBINT16::static_size;

        new_ମୂଲ୍ୟ = new_y;
        hb_memcpy (out, &new_ମୂଲ୍ୟ, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    ଫେରନ୍ତୁ len;
  }

  ସୁରକ୍ଷିତ:
  ତାର୍କିକ scaled_offsets () ଅଚଳ
  { ଫେରନ୍ତୁ (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  ସର୍ବସାଧାରଣ:
  ତାର୍କିକ get_transformation (ଦଶମିକ (&matrix)[4], contour_point_t &trans, ତାର୍କିକ extended) ଅଚଳ
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    ଅଚଳ auto *p = &StructAfter<ଅଚଳ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    ଅନ୍ୟଥା
#endif
      p += HBGlyphID16::static_size;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା tx, ty;
    ଯଦି (flags & ARG_1_AND_2_ARE_ଶବ୍ଦଗୁଡ଼ିକ)
    {
      tx = *(ଅଚଳ HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(ଅଚଳ HBINT16 *) p;
      p += HBINT16::static_size;
    }
    ଅନ୍ୟଥା
    {
      tx = *p++;
      ty = *p++;
    }
    ଯଦି (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((ଦଶମିକ) tx, (ଦଶମିକ) ty, ସତ୍ୟ);

    {
      ଅଚଳ F2DOT14 *points = (ଅଚଳ F2DOT14 *) p;
      ଯଦି (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
      ଅନ୍ୟଥା ଯଦି (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
      ଅନ୍ୟଥା ଯଦି (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	ଫେରନ୍ତୁ ସତ୍ୟ;
      }
    }
    ଫେରନ୍ତୁ tx || ty;
  }

  hb_codepoint_t get_gid (ତାର୍କିକ extended) ଅଚଳ
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended && (flags & GID_IS_24BIT))
      ଫେରନ୍ତୁ StructAfter<ଅଚଳ HBGlyphID24> (flags);
    ଅନ୍ୟଥା
#endif
      ଫେରନ୍ତୁ StructAfter<ଅଚଳ HBGlyphID16> (flags);
  }
  ଶୂନ୍ୟ set_gid (hb_codepoint_t gid, ତାର୍କିକ extended)
  {
#ifndef HB_NO_BEYOND_64K
    ଯଦି (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    ଅନ୍ୟଥା
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  ଶୂନ୍ୟ lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (ସତ୍ୟ);
    ଯଦି (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      ଫେରନ୍ତୁ;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (ସତ୍ୟ);
    ଅକ୍ଷର *end = (ଅକ୍ଷର *) ଏହି + size;
    ଅକ୍ଷର *p = &StructAfter<ଅକ୍ଷର> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, ସତ୍ୟ);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  ସୁରକ୍ଷିତ:
  HBUINT16	flags;
  HBUINT24	pad;
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

ସଂରଚନା CompositeGlyph
{
  ଅଚଳ GlyphHeader &header;
  hb_bytes_t bytes;
  ତାର୍କିକ extended;
  CompositeGlyph (ଅଚଳ GlyphHeader &header_, hb_bytes_t bytes_, ତାର୍କିକ extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () ଅଚଳ
  { ଫେରନ୍ତୁ composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା instructions_length (hb_bytes_t bytes) ଅଚଳ
  {
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = bytes.length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = bytes.length;
    ଅଚଳ CompositeGlyphRecord *last = nullptr;
    ପାଇଁ (auto &item : iter ())
      last = &item;
    ଯଦି (unlikely (!last)) ଫେରନ୍ତୁ 0;

    ଯଦି (last->has_instructions ())
      start = (ଅକ୍ଷର *) last - &bytes + last->get_size (extended);
    ଯଦି (unlikely (start > end)) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ଅଚଳ hb_bytes_t trim_padding () ଅଚଳ { ଫେରନ୍ତୁ bytes; }

  ଶୂନ୍ୟ drop_hints ()
  {
    ପାଇଁ (ଅଚଳ auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  ଶୂନ୍ୟ drop_hints_bytes (hb_bytes_t &dest_start) ଅଚଳ
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  ଶୂନ୍ୟ set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    ଯଦି (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      ଫେରନ୍ତୁ;
    glyph_chain.set_overlaps_flag ();
  }

  ତାର୍କିକ compile_bytes_with_deltas (ଅଚଳ hb_bytes_t &source_bytes,
                                  ଅଚଳ contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    ଯଦି (source_bytes.length <= GlyphHeader::static_size ||
        header.ସଂଖ୍ୟା_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    ଅକ୍ଷର *o = (ଅକ୍ଷର *) hb_calloc (source_len * 2, ଆକାର (ଅକ୍ଷର));
    ଯଦି (unlikely (!o)) ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଅଚଳ CompositeGlyphRecord *c = reinterpret_cast<ଅଚଳ CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((ଅଚଳ ଅକ୍ଷର *)c, source_len), c, extended);

    ଅକ୍ଷର *p = o;
    unsigned i = 0, source_comp_len = 0;
    ପାଇଁ (ଅଚଳ auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      ଯଦି (i >= points_with_deltas.length - 4) {
        hb_free (o);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      unsigned comp_len = component.get_size (extended);
      ଯଦି (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      ଅନ୍ୟଥା
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    ଯଦି (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (ଅଚଳ ଅକ୍ଷର *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    ଫେରନ୍ତୁ ସତ୍ୟ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

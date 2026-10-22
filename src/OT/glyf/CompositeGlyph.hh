#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#அந்தர்பாவ "../../hb-open-type.hh"
#அந்தர்பாவ "composite-iter.hh"


நாமாகாஶ OT {
நாமாகாஶ glyf_impl {


கட்டமைப்பு CompositeGlyphRecord
{
  பாதுகாக்கப்பட்ட:
  எண்ணிக்கை composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_சொற்கள்	= 0x0001,
    ARGS_ARE_XY_மதிப்புகள்		= 0x0002,
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

  பொது:
  size_t get_size (தருக்க extended) மாறிலி
  {
    unsigned முழு size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    எனில் (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    எனில் (flags & ARG_1_AND_2_ARE_சொற்கள்) size += 4;
    /* arg1 and 2 are int8 */
    இல்லையெனில் size += 2;

    /* One x 16 bit (scale) */
    எனில் (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    இல்லையெனில் எனில் (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    இல்லையெனில் எனில் (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    திரும்பு size;
  }

  வெறுமை drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  வெறுமை set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  தருக்க has_instructions ()  மாறிலி { திரும்பு   flags & WE_HAVE_INSTRUCTIONS; }

  தருக்க has_more ()          மாறிலி { திரும்பு   flags & MORE_COMPONENTS; }
  தருக்க is_use_my_metrics () மாறிலி { திரும்பு   flags & USE_MY_METRICS; }
  தருக்க is_anchored ()       மாறிலி { திரும்பு !(flags & ARGS_ARE_XY_மதிப்புகள்); }
  வெறுமை get_anchor_points (unsigned முழு &point1, unsigned முழு &point2, தருக்க extended) மாறிலி
  {
    மாறிலி auto *p = &StructAfter<மாறிலி HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    எனில் (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    இல்லையெனில்
#endif
      p += HBGlyphID16::static_size;
    எனில் (flags & ARG_1_AND_2_ARE_சொற்கள்)
    {
      point1 = ((மாறிலி HBUINT16 *) p)[0];
      point2 = ((மாறிலி HBUINT16 *) p)[1];
    }
    இல்லையெனில்
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  நிலையான வெறுமை transform (மாறிலி தசம (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    எனில் (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      க்கு (auto &point : points)
        point.transform (matrix);
  }

  நிலையான வெறுமை translate (மாறிலி contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    எனில் (HB_OPTIMIZE_SIZE_VAL)
    {
      எனில் (trans.x != 0.f || trans.y != 0.f)
        க்கு (auto &point : points)
	  point.translate (trans);
    }
    இல்லையெனில்
    {
      எனில் (trans.x != 0.f && trans.y != 0.f)
        க்கு (auto &point : points)
	  point.translate (trans);
      இல்லையெனில்
      {
	எனில் (trans.x != 0.f)
	  க்கு (auto &point : points)
	    point.x += trans.x;
	இல்லையெனில் எனில் (trans.y != 0.f)
	  க்கு (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  வெறுமை transform_points (hb_array_t<contour_point_t> points,
			 மாறிலி தசம (&matrix)[4],
			 மாறிலி contour_point_t &trans) மாறிலி
  {
    எனில் (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    இல்லையெனில்
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  தருக்க get_points (contour_point_vector_t &points, தருக்க extended) மாறிலி
  {
    தசம matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    எனில் (unlikely (!points.alloc (points.length + 1 + 4))) திரும்பு பொய்; // For phantom points
    points.push (trans);
    திரும்பு உண்மை;
  }

  unsigned compile_with_point (மாறிலி contour_point_t &point,
                               எழுத்து *out,
			       தருக்க extended) மாறிலி
  {
    மாறிலி HBINT8 *p = &StructAfter<மாறிலி HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    எனில் (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    இல்லையெனில்
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (மாறிலி எழுத்து *)p - (மாறிலி எழுத்து *)இது;
    எனில் (flags & ARG_1_AND_2_ARE_சொற்கள்)
    {
      // no overflow, copy value
      hb_memcpy (out, இது, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    இல்லையெனில்
    {
      முழு new_x = roundf (point.x);
      முழு new_y = roundf (point.y);
      எனில் (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, இது, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      இல்லையெனில்
      {
        // new point value has an int8 overflow
        hb_memcpy (out, இது, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_சொற்கள்;
        out += len_before_val;

        HBINT16 new_மதிப்பு;
        new_மதிப்பு = new_x;
        hb_memcpy (out, &new_மதிப்பு, HBINT16::static_size);
        out += HBINT16::static_size;

        new_மதிப்பு = new_y;
        hb_memcpy (out, &new_மதிப்பு, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    திரும்பு len;
  }

  பாதுகாக்கப்பட்ட:
  தருக்க scaled_offsets () மாறிலி
  { திரும்பு (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  பொது:
  தருக்க get_transformation (தசம (&matrix)[4], contour_point_t &trans, தருக்க extended) மாறிலி
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    மாறிலி auto *p = &StructAfter<மாறிலி HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    எனில் (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    இல்லையெனில்
#endif
      p += HBGlyphID16::static_size;
    முழு tx, ty;
    எனில் (flags & ARG_1_AND_2_ARE_சொற்கள்)
    {
      tx = *(மாறிலி HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(மாறிலி HBINT16 *) p;
      p += HBINT16::static_size;
    }
    இல்லையெனில்
    {
      tx = *p++;
      ty = *p++;
    }
    எனில் (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((தசம) tx, (தசம) ty, உண்மை);

    {
      மாறிலி F2DOT14 *points = (மாறிலி F2DOT14 *) p;
      எனில் (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	திரும்பு உண்மை;
      }
      இல்லையெனில் எனில் (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	திரும்பு உண்மை;
      }
      இல்லையெனில் எனில் (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	திரும்பு உண்மை;
      }
    }
    திரும்பு tx || ty;
  }

  hb_codepoint_t get_gid (தருக்க extended) மாறிலி
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (extended && (flags & GID_IS_24BIT))
      திரும்பு StructAfter<மாறிலி HBGlyphID24> (flags);
    இல்லையெனில்
#endif
      திரும்பு StructAfter<மாறிலி HBGlyphID16> (flags);
  }
  வெறுமை set_gid (hb_codepoint_t gid, தருக்க extended)
  {
#ifndef HB_NO_BEYOND_64K
    எனில் (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    இல்லையெனில்
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  வெறுமை lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (உண்மை);
    எனில் (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      திரும்பு;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (உண்மை);
    எழுத்து *end = (எழுத்து *) இது + size;
    எழுத்து *p = &StructAfter<எழுத்து> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, உண்மை);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  பாதுகாக்கப்பட்ட:
  HBUINT16	flags;
  HBUINT24	pad;
  பொது:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

கட்டமைப்பு CompositeGlyph
{
  மாறிலி GlyphHeader &header;
  hb_bytes_t bytes;
  தருக்க extended;
  CompositeGlyph (மாறிலி GlyphHeader &header_, hb_bytes_t bytes_, தருக்க extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () மாறிலி
  { திரும்பு composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned முழு instructions_length (hb_bytes_t bytes) மாறிலி
  {
    unsigned முழு start = bytes.length;
    unsigned முழு end = bytes.length;
    மாறிலி CompositeGlyphRecord *last = nullptr;
    க்கு (auto &item : iter ())
      last = &item;
    எனில் (unlikely (!last)) திரும்பு 0;

    எனில் (last->has_instructions ())
      start = (எழுத்து *) last - &bytes + last->get_size (extended);
    எனில் (unlikely (start > end)) திரும்பு 0;
    திரும்பு end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  மாறிலி hb_bytes_t trim_padding () மாறிலி { திரும்பு bytes; }

  வெறுமை drop_hints ()
  {
    க்கு (மாறிலி auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  வெறுமை drop_hints_bytes (hb_bytes_t &dest_start) மாறிலி
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  வெறுமை set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    எனில் (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      திரும்பு;
    glyph_chain.set_overlaps_flag ();
  }

  தருக்க compile_bytes_with_deltas (மாறிலி hb_bytes_t &source_bytes,
                                  மாறிலி contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    எனில் (source_bytes.length <= GlyphHeader::static_size ||
        header.எண்_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      திரும்பு உண்மை;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    எழுத்து *o = (எழுத்து *) hb_calloc (source_len * 2, அளவு (எழுத்து));
    எனில் (unlikely (!o)) திரும்பு பொய்;

    மாறிலி CompositeGlyphRecord *c = reinterpret_cast<மாறிலி CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((மாறிலி எழுத்து *)c, source_len), c, extended);

    எழுத்து *p = o;
    unsigned i = 0, source_comp_len = 0;
    க்கு (மாறிலி auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      எனில் (i >= points_with_deltas.length - 4) {
        hb_free (o);
        திரும்பு பொய்;
      }

      unsigned comp_len = component.get_size (extended);
      எனில் (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      இல்லையெனில்
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    எனில் (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (மாறிலி எழுத்து *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    திரும்பு உண்மை;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

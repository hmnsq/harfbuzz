#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#അന്തര്ഭാവ "../../hb-open-type.hh"
#അന്തര്ഭാവ "composite-iter.hh"


നാമാകാശ OT {
നാമാകാശ glyf_impl {


ഘടന CompositeGlyphRecord
{
  സംരക്ഷിത:
  എണ്ണൽ composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_വാക്കുകൾ	= 0x0001,
    ARGS_ARE_XY_മൂല്യങ്ങൾ		= 0x0002,
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

  പൊതു:
  size_t get_size (തർക്കം extended) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    എങ്കിൽ (flags & ARG_1_AND_2_ARE_വാക്കുകൾ) size += 4;
    /* arg1 and 2 are int8 */
    അല്ലെങ്കിൽ size += 2;

    /* One x 16 bit (scale) */
    എങ്കിൽ (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    അല്ലെങ്കിൽ എങ്കിൽ (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    അല്ലെങ്കിൽ എങ്കിൽ (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    തിരികെ size;
  }

  ശൂന്യം drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  ശൂന്യം set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  തർക്കം has_instructions ()  സ്ഥിരം { തിരികെ   flags & WE_HAVE_INSTRUCTIONS; }

  തർക്കം has_more ()          സ്ഥിരം { തിരികെ   flags & MORE_COMPONENTS; }
  തർക്കം is_use_my_metrics () സ്ഥിരം { തിരികെ   flags & USE_MY_METRICS; }
  തർക്കം is_anchored ()       സ്ഥിരം { തിരികെ !(flags & ARGS_ARE_XY_മൂല്യങ്ങൾ); }
  ശൂന്യം get_anchor_points (unsigned പൂർണ്ണസംഖ്യ &point1, unsigned പൂർണ്ണസംഖ്യ &point2, തർക്കം extended) സ്ഥിരം
  {
    സ്ഥിരം auto *p = &StructAfter<സ്ഥിരം HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    അല്ലെങ്കിൽ
#endif
      p += HBGlyphID16::static_size;
    എങ്കിൽ (flags & ARG_1_AND_2_ARE_വാക്കുകൾ)
    {
      point1 = ((സ്ഥിരം HBUINT16 *) p)[0];
      point2 = ((സ്ഥിരം HBUINT16 *) p)[1];
    }
    അല്ലെങ്കിൽ
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  സ്ഥിതം ശൂന്യം transform (സ്ഥിരം ദശാംശം (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    എങ്കിൽ (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      വേണ്ടി (auto &point : points)
        point.transform (matrix);
  }

  സ്ഥിതം ശൂന്യം translate (സ്ഥിരം contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    എങ്കിൽ (HB_OPTIMIZE_SIZE_VAL)
    {
      എങ്കിൽ (trans.x != 0.f || trans.y != 0.f)
        വേണ്ടി (auto &point : points)
	  point.translate (trans);
    }
    അല്ലെങ്കിൽ
    {
      എങ്കിൽ (trans.x != 0.f && trans.y != 0.f)
        വേണ്ടി (auto &point : points)
	  point.translate (trans);
      അല്ലെങ്കിൽ
      {
	എങ്കിൽ (trans.x != 0.f)
	  വേണ്ടി (auto &point : points)
	    point.x += trans.x;
	അല്ലെങ്കിൽ എങ്കിൽ (trans.y != 0.f)
	  വേണ്ടി (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  ശൂന്യം transform_points (hb_array_t<contour_point_t> points,
			 സ്ഥിരം ദശാംശം (&matrix)[4],
			 സ്ഥിരം contour_point_t &trans) സ്ഥിരം
  {
    എങ്കിൽ (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    അല്ലെങ്കിൽ
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  തർക്കം get_points (contour_point_vector_t &points, തർക്കം extended) സ്ഥിരം
  {
    ദശാംശം matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    എങ്കിൽ (unlikely (!points.alloc (points.length + 1 + 4))) തിരികെ അസത്യം; // For phantom points
    points.push (trans);
    തിരികെ സത്യം;
  }

  unsigned compile_with_point (സ്ഥിരം contour_point_t &point,
                               അക്ഷരം *out,
			       തർക്കം extended) സ്ഥിരം
  {
    സ്ഥിരം HBINT8 *p = &StructAfter<സ്ഥിരം HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    അല്ലെങ്കിൽ
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (സ്ഥിരം അക്ഷരം *)p - (സ്ഥിരം അക്ഷരം *)ഇത്;
    എങ്കിൽ (flags & ARG_1_AND_2_ARE_വാക്കുകൾ)
    {
      // no overflow, copy value
      hb_memcpy (out, ഇത്, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ new_x = roundf (point.x);
      പൂർണ്ണസംഖ്യ new_y = roundf (point.y);
      എങ്കിൽ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, ഇത്, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      അല്ലെങ്കിൽ
      {
        // new point value has an int8 overflow
        hb_memcpy (out, ഇത്, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_വാക്കുകൾ;
        out += len_before_val;

        HBINT16 new_മൂല്യം;
        new_മൂല്യം = new_x;
        hb_memcpy (out, &new_മൂല്യം, HBINT16::static_size);
        out += HBINT16::static_size;

        new_മൂല്യം = new_y;
        hb_memcpy (out, &new_മൂല്യം, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    തിരികെ len;
  }

  സംരക്ഷിത:
  തർക്കം scaled_offsets () സ്ഥിരം
  { തിരികെ (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  പൊതു:
  തർക്കം get_transformation (ദശാംശം (&matrix)[4], contour_point_t &trans, തർക്കം extended) സ്ഥിരം
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    സ്ഥിരം auto *p = &StructAfter<സ്ഥിരം HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    അല്ലെങ്കിൽ
#endif
      p += HBGlyphID16::static_size;
    പൂർണ്ണസംഖ്യ tx, ty;
    എങ്കിൽ (flags & ARG_1_AND_2_ARE_വാക്കുകൾ)
    {
      tx = *(സ്ഥിരം HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(സ്ഥിരം HBINT16 *) p;
      p += HBINT16::static_size;
    }
    അല്ലെങ്കിൽ
    {
      tx = *p++;
      ty = *p++;
    }
    എങ്കിൽ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((ദശാംശം) tx, (ദശാംശം) ty, സത്യം);

    {
      സ്ഥിരം F2DOT14 *points = (സ്ഥിരം F2DOT14 *) p;
      എങ്കിൽ (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	തിരികെ സത്യം;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	തിരികെ സത്യം;
      }
      അല്ലെങ്കിൽ എങ്കിൽ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	തിരികെ സത്യം;
      }
    }
    തിരികെ tx || ty;
  }

  hb_codepoint_t get_gid (തർക്കം extended) സ്ഥിരം
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended && (flags & GID_IS_24BIT))
      തിരികെ StructAfter<സ്ഥിരം HBGlyphID24> (flags);
    അല്ലെങ്കിൽ
#endif
      തിരികെ StructAfter<സ്ഥിരം HBGlyphID16> (flags);
  }
  ശൂന്യം set_gid (hb_codepoint_t gid, തർക്കം extended)
  {
#ifndef HB_NO_BEYOND_64K
    എങ്കിൽ (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    അല്ലെങ്കിൽ
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  ശൂന്യം lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (സത്യം);
    എങ്കിൽ (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      തിരികെ;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (സത്യം);
    അക്ഷരം *end = (അക്ഷരം *) ഇത് + size;
    അക്ഷരം *p = &StructAfter<അക്ഷരം> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, സത്യം);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  സംരക്ഷിത:
  HBUINT16	flags;
  HBUINT24	pad;
  പൊതു:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

ഘടന CompositeGlyph
{
  സ്ഥിരം GlyphHeader &header;
  hb_bytes_t bytes;
  തർക്കം extended;
  CompositeGlyph (സ്ഥിരം GlyphHeader &header_, hb_bytes_t bytes_, തർക്കം extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () സ്ഥിരം
  { തിരികെ composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned പൂർണ്ണസംഖ്യ instructions_length (hb_bytes_t bytes) സ്ഥിരം
  {
    unsigned പൂർണ്ണസംഖ്യ start = bytes.length;
    unsigned പൂർണ്ണസംഖ്യ end = bytes.length;
    സ്ഥിരം CompositeGlyphRecord *last = nullptr;
    വേണ്ടി (auto &item : iter ())
      last = &item;
    എങ്കിൽ (unlikely (!last)) തിരികെ 0;

    എങ്കിൽ (last->has_instructions ())
      start = (അക്ഷരം *) last - &bytes + last->get_size (extended);
    എങ്കിൽ (unlikely (start > end)) തിരികെ 0;
    തിരികെ end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  സ്ഥിരം hb_bytes_t trim_padding () സ്ഥിരം { തിരികെ bytes; }

  ശൂന്യം drop_hints ()
  {
    വേണ്ടി (സ്ഥിരം auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  ശൂന്യം drop_hints_bytes (hb_bytes_t &dest_start) സ്ഥിരം
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  ശൂന്യം set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    എങ്കിൽ (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      തിരികെ;
    glyph_chain.set_overlaps_flag ();
  }

  തർക്കം compile_bytes_with_deltas (സ്ഥിരം hb_bytes_t &source_bytes,
                                  സ്ഥിരം contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    എങ്കിൽ (source_bytes.length <= GlyphHeader::static_size ||
        header.സംഖ്യ_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      തിരികെ സത്യം;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    അക്ഷരം *o = (അക്ഷരം *) hb_calloc (source_len * 2, വലുപ്പം (അക്ഷരം));
    എങ്കിൽ (unlikely (!o)) തിരികെ അസത്യം;

    സ്ഥിരം CompositeGlyphRecord *c = reinterpret_cast<സ്ഥിരം CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((സ്ഥിരം അക്ഷരം *)c, source_len), c, extended);

    അക്ഷരം *p = o;
    unsigned i = 0, source_comp_len = 0;
    വേണ്ടി (സ്ഥിരം auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      എങ്കിൽ (i >= points_with_deltas.length - 4) {
        hb_free (o);
        തിരികെ അസത്യം;
      }

      unsigned comp_len = component.get_size (extended);
      എങ്കിൽ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      അല്ലെങ്കിൽ
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    എങ്കിൽ (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (സ്ഥിരം അക്ഷരം *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    തിരികെ സത്യം;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

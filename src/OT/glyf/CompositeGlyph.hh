#ifndef OT_GLYF_COMPOSITEGLYPH_HH
#define OT_GLYF_COMPOSITEGLYPH_HH


#અન્તર્ભાવ "../../hb-open-type.hh"
#અન્તર્ભાવ "composite-iter.hh"


નામાકાશ OT {
નામાકાશ glyf_impl {


સંરચના CompositeGlyphRecord
{
  સુરક્ષિત:
  ગણના composite_glyph_flag_t
  {
    ARG_1_AND_2_ARE_શબ્દો	= 0x0001,
    ARGS_ARE_XY_કિંમતો		= 0x0002,
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

  જાહેર:
  size_t get_size (તાર્કિક extended) અચલ
  {
    unsigned પૂર્ણાંક size = min_size;
    /* glyphIndex is 24bit instead of 16bit */
#ifndef HB_NO_BEYOND_64K
    જો (extended && (flags & GID_IS_24BIT)) size += HBGlyphID24::static_size - HBGlyphID16::static_size;
#endif
    /* arg1 and 2 are int16 */
    જો (flags & ARG_1_AND_2_ARE_શબ્દો) size += 4;
    /* arg1 and 2 are int8 */
    નહીંતર size += 2;

    /* One x 16 bit (scale) */
    જો (flags & WE_HAVE_A_SCALE) size += 2;
    /* Two x 16 bit (xscale, yscale) */
    નહીંતર જો (flags & WE_HAVE_AN_X_AND_Y_SCALE) size += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    નહીંતર જો (flags & WE_HAVE_A_TWO_BY_TWO) size += 8;

    પાછા size;
  }

  શૂન્ય drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  શૂન્ય set_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  તાર્કિક has_instructions ()  અચલ { પાછા   flags & WE_HAVE_INSTRUCTIONS; }

  તાર્કિક has_more ()          અચલ { પાછા   flags & MORE_COMPONENTS; }
  તાર્કિક is_use_my_metrics () અચલ { પાછા   flags & USE_MY_METRICS; }
  તાર્કિક is_anchored ()       અચલ { પાછા !(flags & ARGS_ARE_XY_કિંમતો); }
  શૂન્ય get_anchor_points (unsigned પૂર્ણાંક &point1, unsigned પૂર્ણાંક &point2, તાર્કિક extended) અચલ
  {
    અચલ auto *p = &StructAfter<અચલ HBUINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    જો (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    નહીંતર
#endif
      p += HBGlyphID16::static_size;
    જો (flags & ARG_1_AND_2_ARE_શબ્દો)
    {
      point1 = ((અચલ HBUINT16 *) p)[0];
      point2 = ((અચલ HBUINT16 *) p)[1];
    }
    નહીંતર
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  સ્થિર શૂન્ય transform (અચલ દશાંશ (&matrix)[4],
			 hb_array_t<contour_point_t> points)
  {
    જો (matrix[0] != 1.f || matrix[1] != 0.f ||
	matrix[2] != 0.f || matrix[3] != 1.f)
      માટે (auto &point : points)
        point.transform (matrix);
  }

  સ્થિર શૂન્ય translate (અચલ contour_point_t &trans,
			 hb_array_t<contour_point_t> points)
  {
    જો (HB_OPTIMIZE_SIZE_VAL)
    {
      જો (trans.x != 0.f || trans.y != 0.f)
        માટે (auto &point : points)
	  point.translate (trans);
    }
    નહીંતર
    {
      જો (trans.x != 0.f && trans.y != 0.f)
        માટે (auto &point : points)
	  point.translate (trans);
      નહીંતર
      {
	જો (trans.x != 0.f)
	  માટે (auto &point : points)
	    point.x += trans.x;
	નહીંતર જો (trans.y != 0.f)
	  માટે (auto &point : points)
	    point.y += trans.y;
      }
    }
  }

  શૂન્ય transform_points (hb_array_t<contour_point_t> points,
			 અચલ દશાંશ (&matrix)[4],
			 અચલ contour_point_t &trans) અચલ
  {
    જો (scaled_offsets ())
    {
      translate (trans, points);
      transform (matrix, points);
    }
    નહીંતર
    {
      transform (matrix, points);
      translate (trans, points);
    }
  }

  તાર્કિક get_points (contour_point_vector_t &points, તાર્કિક extended) અચલ
  {
    દશાંશ matrix[4];
    contour_point_t trans;
    get_transformation (matrix, trans, extended);
    જો (unlikely (!points.alloc (points.length + 1 + 4))) પાછા ખોટું; // For phantom points
    points.push (trans);
    પાછા સાચું;
  }

  unsigned compile_with_point (અચલ contour_point_t &point,
                               અક્ષર *out,
			       તાર્કિક extended) અચલ
  {
    અચલ HBINT8 *p = &StructAfter<અચલ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    જો (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    નહીંતર
#endif
      p += HBGlyphID16::static_size;

    unsigned len = get_size (extended);
    unsigned len_before_val = (અચલ અક્ષર *)p - (અચલ અક્ષર *)આ;
    જો (flags & ARG_1_AND_2_ARE_શબ્દો)
    {
      // no overflow, copy value
      hb_memcpy (out, આ, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (out + len_before_val);
      o[0] = roundf (point.x);
      o[1] = roundf (point.y);
    }
    નહીંતર
    {
      પૂર્ણાંક new_x = roundf (point.x);
      પૂર્ણાંક new_y = roundf (point.y);
      જો (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (out, આ, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (out + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      નહીંતર
      {
        // new point value has an int8 overflow
        hb_memcpy (out, આ, len_before_val);
        
        //update flags
        CompositeGlyphRecord *o = reinterpret_cast<CompositeGlyphRecord *> (out);
        o->flags = flags | ARG_1_AND_2_ARE_શબ્દો;
        out += len_before_val;

        HBINT16 new_કિંમત;
        new_કિંમત = new_x;
        hb_memcpy (out, &new_કિંમત, HBINT16::static_size);
        out += HBINT16::static_size;

        new_કિંમત = new_y;
        hb_memcpy (out, &new_કિંમત, HBINT16::static_size);
        out += HBINT16::static_size;

        hb_memcpy (out, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    પાછા len;
  }

  સુરક્ષિત:
  તાર્કિક scaled_offsets () અચલ
  { પાછા (flags & (SCALED_COMPONENT_OFFSET | UNSCALED_COMPONENT_OFFSET)) == SCALED_COMPONENT_OFFSET; }

  જાહેર:
  તાર્કિક get_transformation (દશાંશ (&matrix)[4], contour_point_t &trans, તાર્કિક extended) અચલ
  {
    matrix[0] = matrix[3] = 1.f;
    matrix[1] = matrix[2] = 0.f;

    અચલ auto *p = &StructAfter<અચલ HBINT8> (flags);
#ifndef HB_NO_BEYOND_64K
    જો (extended && (flags & GID_IS_24BIT))
      p += HBGlyphID24::static_size;
    નહીંતર
#endif
      p += HBGlyphID16::static_size;
    પૂર્ણાંક tx, ty;
    જો (flags & ARG_1_AND_2_ARE_શબ્દો)
    {
      tx = *(અચલ HBINT16 *) p;
      p += HBINT16::static_size;
      ty = *(અચલ HBINT16 *) p;
      p += HBINT16::static_size;
    }
    નહીંતર
    {
      tx = *p++;
      ty = *p++;
    }
    જો (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((દશાંશ) tx, (દશાંશ) ty, સાચું);

    {
      અચલ F2DOT14 *points = (અચલ F2DOT14 *) p;
      જો (flags & WE_HAVE_A_SCALE)
      {
	matrix[0] = matrix[3] = points[0].to_float ();
	પાછા સાચું;
      }
      નહીંતર જો (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	matrix[0] = points[0].to_float ();
	matrix[3] = points[1].to_float ();
	પાછા સાચું;
      }
      નહીંતર જો (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	matrix[0] = points[0].to_float ();
	matrix[1] = points[1].to_float ();
	matrix[2] = points[2].to_float ();
	matrix[3] = points[3].to_float ();
	પાછા સાચું;
      }
    }
    પાછા tx || ty;
  }

  hb_codepoint_t get_gid (તાર્કિક extended) અચલ
  {
#ifndef HB_NO_BEYOND_64K
    જો (extended && (flags & GID_IS_24BIT))
      પાછા StructAfter<અચલ HBGlyphID24> (flags);
    નહીંતર
#endif
      પાછા StructAfter<અચલ HBGlyphID16> (flags);
  }
  શૂન્ય set_gid (hb_codepoint_t gid, તાર્કિક extended)
  {
#ifndef HB_NO_BEYOND_64K
    જો (extended && (flags & GID_IS_24BIT))
      StructAfter<HBGlyphID24> (flags) = gid;
    નહીંતર
#endif
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ifndef HB_NO_BEYOND_64K
  શૂન્ય lower_gid_24_to_16 ()
  {
    hb_codepoint_t gid = get_gid (સાચું);
    જો (!(flags & GID_IS_24BIT) || gid > 0xFFFFu)
      પાછા;

    /* Lower the flag and move the rest of the struct down. */

    unsigned size = get_size (સાચું);
    અક્ષર *end = (અક્ષર *) આ + size;
    અક્ષર *p = &StructAfter<અક્ષર> (flags);
    p += HBGlyphID24::static_size;

    flags = flags & ~GID_IS_24BIT;
    set_gid (gid, સાચું);

    memmove (p - HBGlyphID24::static_size + HBGlyphID16::static_size, p, end - p);
  }
#endif

  સુરક્ષિત:
  HBUINT16	flags;
  HBUINT24	pad;
  જાહેર:
  DEFINE_SIZE_MIN (4);
};

using composite_iter_t = composite_iter_tmpl<CompositeGlyphRecord>;

સંરચના CompositeGlyph
{
  અચલ GlyphHeader &header;
  hb_bytes_t bytes;
  તાર્કિક extended;
  CompositeGlyph (અચલ GlyphHeader &header_, hb_bytes_t bytes_, તાર્કિક extended_) :
    header (header_), bytes (bytes_), extended (extended_) {}

  composite_iter_t iter () અચલ
  { પાછા composite_iter_t (bytes, &StructAfter<CompositeGlyphRecord, GlyphHeader> (header), extended); }

  unsigned પૂર્ણાંક instructions_length (hb_bytes_t bytes) અચલ
  {
    unsigned પૂર્ણાંક start = bytes.length;
    unsigned પૂર્ણાંક end = bytes.length;
    અચલ CompositeGlyphRecord *last = nullptr;
    માટે (auto &item : iter ())
      last = &item;
    જો (unlikely (!last)) પાછા 0;

    જો (last->has_instructions ())
      start = (અક્ષર *) last - &bytes + last->get_size (extended);
    જો (unlikely (start > end)) પાછા 0;
    પાછા end - start;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  અચલ hb_bytes_t trim_padding () અચલ { પાછા bytes; }

  શૂન્ય drop_hints ()
  {
    માટે (અચલ auto &_ : iter ())
      const_cast<CompositeGlyphRecord &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  શૂન્ય drop_hints_bytes (hb_bytes_t &dest_start) અચલ
  { dest_start = bytes.sub_array (0, bytes.length - instructions_length (bytes)); }

  શૂન્ય set_overlaps_flag ()
  {
    CompositeGlyphRecord& glyph_chain = const_cast<CompositeGlyphRecord &> (
	StructAfter<CompositeGlyphRecord, GlyphHeader> (header));
    જો (!bytes.check_range(&glyph_chain, CompositeGlyphRecord::min_size))
      પાછા;
    glyph_chain.set_overlaps_flag ();
  }

  તાર્કિક compile_bytes_with_deltas (અચલ hb_bytes_t &source_bytes,
                                  અચલ contour_point_vector_t &points_with_deltas,
                                  hb_bytes_t &dest_bytes /* OUT */)
  {
    જો (source_bytes.length <= GlyphHeader::static_size ||
        header.સંખ્યા_Of_Contours != -1)
    {
      dest_bytes = hb_bytes_t ();
      પાછા સાચું;
    }

    unsigned source_len = source_bytes.length - GlyphHeader::static_size;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    અક્ષર *o = (અક્ષર *) hb_calloc (source_len * 2, કદ (અક્ષર));
    જો (unlikely (!o)) પાછા ખોટું;

    અચલ CompositeGlyphRecord *c = reinterpret_cast<અચલ CompositeGlyphRecord *> (source_bytes.arrayZ + GlyphHeader::static_size);
    auto it = composite_iter_t (hb_bytes_t ((અચલ અક્ષર *)c, source_len), c, extended);

    અક્ષર *p = o;
    unsigned i = 0, source_comp_len = 0;
    માટે (અચલ auto &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      જો (i >= points_with_deltas.length - 4) {
        hb_free (o);
        પાછા ખોટું;
      }

      unsigned comp_len = component.get_size (extended);
      જો (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      નહીંતર
      {
        unsigned new_len = component.compile_with_point (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      source_comp_len += comp_len;
    }

    //copy instructions if any
    જો (source_len > source_comp_len)
    {
      unsigned instr_len = source_len - source_comp_len;
      hb_memcpy (p, (અચલ અક્ષર *)c + source_comp_len, instr_len);
      p += instr_len;
    }

    unsigned len = p - o;
    dest_bytes = hb_bytes_t (o, len);
    પાછા સાચું;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#endif /* OT_GLYF_COMPOSITEGLYPH_HH */

#నిర్వచితం_కాకపోతే OT_GLYF_COMPOSITEGLYPH_HH
#నిర్వచించు OT_GLYF_COMPOSITEGLYPH_HH


#అన్తర్భావ "../../hb-open-type.hh"
#అన్తర్భావ "composite-iter.hh"


నామాకాశ OT {
నామాకాశ glyf_impl {


నిర్మాణం సంయుక్త_Glyph_రికార్డు
{
  రక్షిత:
  గణన సంయుక్త_glyph_flag_t
  {
    ARG_1_AND_2_ARE_పదాలు	= 0x0001,
    వాదనలు_ARE_XY_విలువలు		= 0x0002,
    విడత_XY_గమ్యం_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    మరిన్ని_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_స్థానభ్రంశం	= 0x0800,
    UNSCALED_COMPONENT_స్థానభ్రంశం	= 0x1000,
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    GID_IS_24_బిట్టు		= 0x2000
#అయితే_ముగింపు
  };

  బహిరంగ:
  size_t get_కొలత (తార్కిక extended) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం కొలత = min_కొలత;
    /* glyphIndex is 24bit instead of 16bit */
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended && (flags & GID_IS_24_బిట్టు)) కొలత += HBGlyphID24::static_కొలత - HBGlyphID16::static_కొలత;
#అయితే_ముగింపు
    /* arg1 and 2 are int16 */
    ఒకవేళ (flags & ARG_1_AND_2_ARE_పదాలు) కొలత += 4;
    /* arg1 and 2 are int8 */
    లేకపోతే కొలత += 2;

    /* One x 16 bit (scale) */
    ఒకవేళ (flags & WE_HAVE_A_SCALE) కొలత += 2;
    /* Two x 16 bit (xscale, yscale) */
    లేకపోతే ఒకవేళ (flags & WE_HAVE_AN_X_AND_Y_SCALE) కొలత += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    లేకపోతే ఒకవేళ (flags & WE_HAVE_A_TWO_BY_TWO) కొలత += 8;

    తిరిగి కొలత;
  }

  శూన్య drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  శూన్య సమితి_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  తార్కిక has_instructions ()  స్థిరరాశి { తిరిగి   flags & WE_HAVE_INSTRUCTIONS; }

  తార్కిక has_మరిన్ని ()          స్థిరరాశి { తిరిగి   flags & మరిన్ని_COMPONENTS; }
  తార్కిక is_use_my_metrics () స్థిరరాశి { తిరిగి   flags & USE_MY_METRICS; }
  తార్కిక is_anchored ()       స్థిరరాశి { తిరిగి !(flags & వాదనలు_ARE_XY_విలువలు); }
  శూన్య get_anchor_points (గుర్తులేని పూర్ణాంకం &point1, గుర్తులేని పూర్ణాంకం &point2, తార్కిక extended) స్థిరరాశి
  {
    స్థిరరాశి స్వయంచాలక *p = &StructAfter<స్థిరరాశి HBUINT8> (flags);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended && (flags & GID_IS_24_బిట్టు))
      p += HBGlyphID24::static_కొలత;
    లేకపోతే
#అయితే_ముగింపు
      p += HBGlyphID16::static_కొలత;
    ఒకవేళ (flags & ARG_1_AND_2_ARE_పదాలు)
    {
      point1 = ((స్థిరరాశి HBUINT16 *) p)[0];
      point2 = ((స్థిరరాశి HBUINT16 *) p)[1];
    }
    లేకపోతే
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  స్థిర శూన్య transform (స్థిరరాశి దశాంశం (&మాతృక)[4],
			 hb_array_t<contour_బిందువు_t> points)
  {
    ఒకవేళ (మాతృక[0] != 1.f || మాతృక[1] != 0.f ||
	మాతృక[2] != 0.f || మాతృక[3] != 1.f)
      కొరకు (స్వయంచాలక &బిందువు : points)
        బిందువు.transform (మాతృక);
  }

  స్థిర శూన్య translate (స్థిరరాశి contour_బిందువు_t &trans,
			 hb_array_t<contour_బిందువు_t> points)
  {
    ఒకవేళ (HB_OPTIMIZE_కొలత_VAL)
    {
      ఒకవేళ (trans.x != 0.f || trans.y != 0.f)
        కొరకు (స్వయంచాలక &బిందువు : points)
	  బిందువు.translate (trans);
    }
    లేకపోతే
    {
      ఒకవేళ (trans.x != 0.f && trans.y != 0.f)
        కొరకు (స్వయంచాలక &బిందువు : points)
	  బిందువు.translate (trans);
      లేకపోతే
      {
	ఒకవేళ (trans.x != 0.f)
	  కొరకు (స్వయంచాలక &బిందువు : points)
	    బిందువు.x += trans.x;
	లేకపోతే ఒకవేళ (trans.y != 0.f)
	  కొరకు (స్వయంచాలక &బిందువు : points)
	    బిందువు.y += trans.y;
      }
    }
  }

  శూన్య transform_points (hb_array_t<contour_బిందువు_t> points,
			 స్థిరరాశి దశాంశం (&మాతృక)[4],
			 స్థిరరాశి contour_బిందువు_t &trans) స్థిరరాశి
  {
    ఒకవేళ (scaled_offsets ())
    {
      translate (trans, points);
      transform (మాతృక, points);
    }
    లేకపోతే
    {
      transform (మాతృక, points);
      translate (trans, points);
    }
  }

  తార్కిక get_points (contour_బిందువు_సదిశ_t &points, తార్కిక extended) స్థిరరాశి
  {
    దశాంశం మాతృక[4];
    contour_బిందువు_t trans;
    get_transformation (మాతృక, trans, extended);
    ఒకవేళ (unlikely (!points.alloc (points.నిడివి + 1 + 4))) తిరిగి అబద్ధం; // For phantom points
    points.push (trans);
    తిరిగి నిజం;
  }

  గుర్తులేని compile_with_బిందువు (స్థిరరాశి contour_బిందువు_t &బిందువు,
                               అక్షరం *బహిర్గతం,
			       తార్కిక extended) స్థిరరాశి
  {
    స్థిరరాశి HBINT8 *p = &StructAfter<స్థిరరాశి HBINT8> (flags);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended && (flags & GID_IS_24_బిట్టు))
      p += HBGlyphID24::static_కొలత;
    లేకపోతే
#అయితే_ముగింపు
      p += HBGlyphID16::static_కొలత;

    గుర్తులేని len = get_కొలత (extended);
    గుర్తులేని len_before_val = (స్థిరరాశి అక్షరం *)p - (స్థిరరాశి అక్షరం *)ఇది;
    ఒకవేళ (flags & ARG_1_AND_2_ARE_పదాలు)
    {
      // no overflow, copy value
      hb_memcpy (బహిర్గతం, ఇది, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (బహిర్గతం + len_before_val);
      o[0] = roundf (బిందువు.x);
      o[1] = roundf (బిందువు.y);
    }
    లేకపోతే
    {
      పూర్ణాంకం new_x = roundf (బిందువు.x);
      పూర్ణాంకం new_y = roundf (బిందువు.y);
      ఒకవేళ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (బహిర్గతం, ఇది, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (బహిర్గతం + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      లేకపోతే
      {
        // new point value has an int8 overflow
        hb_memcpy (బహిర్గతం, ఇది, len_before_val);
        
        //update flags
        సంయుక్త_Glyph_రికార్డు *o = reinterpret_cast<సంయుక్త_Glyph_రికార్డు *> (బహిర్గతం);
        o->flags = flags | ARG_1_AND_2_ARE_పదాలు;
        బహిర్గతం += len_before_val;

        HBINT16 new_విలువ;
        new_విలువ = new_x;
        hb_memcpy (బహిర్గతం, &new_విలువ, HBINT16::static_కొలత);
        బహిర్గతం += HBINT16::static_కొలత;

        new_విలువ = new_y;
        hb_memcpy (బహిర్గతం, &new_విలువ, HBINT16::static_కొలత);
        బహిర్గతం += HBINT16::static_కొలత;

        hb_memcpy (బహిర్గతం, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    తిరిగి len;
  }

  రక్షిత:
  తార్కిక scaled_offsets () స్థిరరాశి
  { తిరిగి (flags & (SCALED_COMPONENT_స్థానభ్రంశం | UNSCALED_COMPONENT_స్థానభ్రంశం)) == SCALED_COMPONENT_స్థానభ్రంశం; }

  బహిరంగ:
  తార్కిక get_transformation (దశాంశం (&మాతృక)[4], contour_బిందువు_t &trans, తార్కిక extended) స్థిరరాశి
  {
    మాతృక[0] = మాతృక[3] = 1.f;
    మాతృక[1] = మాతృక[2] = 0.f;

    స్థిరరాశి స్వయంచాలక *p = &StructAfter<స్థిరరాశి HBINT8> (flags);
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended && (flags & GID_IS_24_బిట్టు))
      p += HBGlyphID24::static_కొలత;
    లేకపోతే
#అయితే_ముగింపు
      p += HBGlyphID16::static_కొలత;
    పూర్ణాంకం tx, ty;
    ఒకవేళ (flags & ARG_1_AND_2_ARE_పదాలు)
    {
      tx = *(స్థిరరాశి HBINT16 *) p;
      p += HBINT16::static_కొలత;
      ty = *(స్థిరరాశి HBINT16 *) p;
      p += HBINT16::static_కొలత;
    }
    లేకపోతే
    {
      tx = *p++;
      ty = *p++;
    }
    ఒకవేళ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((దశాంశం) tx, (దశాంశం) ty, నిజం);

    {
      స్థిరరాశి F2DOT14 *points = (స్థిరరాశి F2DOT14 *) p;
      ఒకవేళ (flags & WE_HAVE_A_SCALE)
      {
	మాతృక[0] = మాతృక[3] = points[0].గమ్యం_float ();
	తిరిగి నిజం;
      }
      లేకపోతే ఒకవేళ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	మాతృక[0] = points[0].గమ్యం_float ();
	మాతృక[3] = points[1].గమ్యం_float ();
	తిరిగి నిజం;
      }
      లేకపోతే ఒకవేళ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	మాతృక[0] = points[0].గమ్యం_float ();
	మాతృక[1] = points[1].గమ్యం_float ();
	మాతృక[2] = points[2].గమ్యం_float ();
	మాతృక[3] = points[3].గమ్యం_float ();
	తిరిగి నిజం;
      }
    }
    తిరిగి tx || ty;
  }

  hb_codepoint_t get_gid (తార్కిక extended) స్థిరరాశి
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended && (flags & GID_IS_24_బిట్టు))
      తిరిగి StructAfter<స్థిరరాశి HBGlyphID24> (flags);
    లేకపోతే
#అయితే_ముగింపు
      తిరిగి StructAfter<స్థిరరాశి HBGlyphID16> (flags);
  }
  శూన్య సమితి_gid (hb_codepoint_t gid, తార్కిక extended)
  {
#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
    ఒకవేళ (extended && (flags & GID_IS_24_బిట్టు))
      StructAfter<HBGlyphID24> (flags) = gid;
    లేకపోతే
#అయితే_ముగింపు
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#నిర్వచితం_కాకపోతే HB_NO_BEYOND_64K
  శూన్య క్రింది_gid_24_గమ్యం_16 ()
  {
    hb_codepoint_t gid = get_gid (నిజం);
    ఒకవేళ (!(flags & GID_IS_24_బిట్టు) || gid > 0xFFFFu)
      తిరిగి;

    /* Lower the flag and move the rest of the struct down. */

    గుర్తులేని కొలత = get_కొలత (నిజం);
    అక్షరం *చివర = (అక్షరం *) ఇది + కొలత;
    అక్షరం *p = &StructAfter<అక్షరం> (flags);
    p += HBGlyphID24::static_కొలత;

    flags = flags & ~GID_IS_24_బిట్టు;
    సమితి_gid (gid, నిజం);

    memmove (p - HBGlyphID24::static_కొలత + HBGlyphID16::static_కొలత, p, చివర - p);
  }
#అయితే_ముగింపు

  రక్షిత:
  HBUINT16	flags;
  HBUINT24	pad;
  బహిరంగ:
  DEFINE_కొలత_MIN (4);
};

ఉపయోగించి సంయుక్త_iter_t = సంయుక్త_iter_tmpl<సంయుక్త_Glyph_రికార్డు>;

నిర్మాణం సంయుక్త_Glyph
{
  స్థిరరాశి Glyph_శీర్షిక &శీర్షిక;
  hb_బైట్లు_t బైట్లు;
  తార్కిక extended;
  సంయుక్త_Glyph (స్థిరరాశి Glyph_శీర్షిక &శీర్షిక_2, hb_బైట్లు_t బైట్లు_2, తార్కిక extended_) :
    శీర్షిక (శీర్షిక_2), బైట్లు (బైట్లు_2), extended (extended_) {}

  సంయుక్త_iter_t iter () స్థిరరాశి
  { తిరిగి సంయుక్త_iter_t (బైట్లు, &StructAfter<సంయుక్త_Glyph_రికార్డు, Glyph_శీర్షిక> (శీర్షిక), extended); }

  గుర్తులేని పూర్ణాంకం instructions_నిడివి (hb_బైట్లు_t బైట్లు) స్థిరరాశి
  {
    గుర్తులేని పూర్ణాంకం ప్రారంభం = బైట్లు.నిడివి;
    గుర్తులేని పూర్ణాంకం చివర = బైట్లు.నిడివి;
    స్థిరరాశి సంయుక్త_Glyph_రికార్డు *చివరి = శూన్యసూచిక;
    కొరకు (స్వయంచాలక &అంశం : iter ())
      చివరి = &అంశం;
    ఒకవేళ (unlikely (!చివరి)) తిరిగి 0;

    ఒకవేళ (చివరి->has_instructions ())
      ప్రారంభం = (అక్షరం *) చివరి - &బైట్లు + చివరి->get_కొలత (extended);
    ఒకవేళ (unlikely (ప్రారంభం > చివర)) తిరిగి 0;
    తిరిగి చివర - ప్రారంభం;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  స్థిరరాశి hb_బైట్లు_t trim_padding () స్థిరరాశి { తిరిగి బైట్లు; }

  శూన్య drop_hints ()
  {
    కొరకు (స్థిరరాశి స్వయంచాలక &_ : iter ())
      const_cast<సంయుక్త_Glyph_రికార్డు &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  శూన్య drop_hints_బైట్లు (hb_బైట్లు_t &dest_ప్రారంభం) స్థిరరాశి
  { dest_ప్రారంభం = బైట్లు.sub_array (0, బైట్లు.నిడివి - instructions_నిడివి (బైట్లు)); }

  శూన్య సమితి_overlaps_flag ()
  {
    సంయుక్త_Glyph_రికార్డు& glyph_chain = const_cast<సంయుక్త_Glyph_రికార్డు &> (
	StructAfter<సంయుక్త_Glyph_రికార్డు, Glyph_శీర్షిక> (శీర్షిక));
    ఒకవేళ (!బైట్లు.check_range(&glyph_chain, సంయుక్త_Glyph_రికార్డు::min_కొలత))
      తిరిగి;
    glyph_chain.సమితి_overlaps_flag ();
  }

  తార్కిక compile_బైట్లు_with_deltas (స్థిరరాశి hb_బైట్లు_t &ఆధారం_బైట్లు,
                                  స్థిరరాశి contour_బిందువు_సదిశ_t &points_with_deltas,
                                  hb_బైట్లు_t &dest_బైట్లు /*బహిర్గతం*/)
  {
    ఒకవేళ (ఆధారం_బైట్లు.నిడివి <= Glyph_శీర్షిక::static_కొలత ||
        శీర్షిక.సంఖ్య_యొక్క_Contours != -1)
    {
      dest_బైట్లు = hb_బైట్లు_t ();
      తిరిగి నిజం;
    }

    గుర్తులేని ఆధారం_len = ఆధారం_బైట్లు.నిడివి - Glyph_శీర్షిక::static_కొలత;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    అక్షరం *o = (అక్షరం *) hb_calloc (ఆధారం_len * 2, పరిమాణం (అక్షరం));
    ఒకవేళ (unlikely (!o)) తిరిగి అబద్ధం;

    స్థిరరాశి సంయుక్త_Glyph_రికార్డు *c = reinterpret_cast<స్థిరరాశి సంయుక్త_Glyph_రికార్డు *> (ఆధారం_బైట్లు.arrayZ + Glyph_శీర్షిక::static_కొలత);
    స్వయంచాలక it = సంయుక్త_iter_t (hb_బైట్లు_t ((స్థిరరాశి అక్షరం *)c, ఆధారం_len), c, extended);

    అక్షరం *p = o;
    గుర్తులేని i = 0, ఆధారం_comp_len = 0;
    కొరకు (స్థిరరాశి స్వయంచాలక &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      ఒకవేళ (i >= points_with_deltas.నిడివి - 4) {
        hb_free (o);
        తిరిగి అబద్ధం;
      }

      గుర్తులేని comp_len = component.get_కొలత (extended);
      ఒకవేళ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      లేకపోతే
      {
        గుర్తులేని new_len = component.compile_with_బిందువు (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      ఆధారం_comp_len += comp_len;
    }

    //copy instructions if any
    ఒకవేళ (ఆధారం_len > ఆధారం_comp_len)
    {
      గుర్తులేని instr_len = ఆధారం_len - ఆధారం_comp_len;
      hb_memcpy (p, (స్థిరరాశి అక్షరం *)c + ఆధారం_comp_len, instr_len);
      p += instr_len;
    }

    గుర్తులేని len = p - o;
    dest_బైట్లు = hb_బైట్లు_t (o, len);
    తిరిగి నిజం;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#అయితే_ముగింపు /* OT_GLYF_COMPOSITEGLYPH_HH */

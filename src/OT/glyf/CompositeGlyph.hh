#که_نه_تعریف_شوی OT_GLYF_COMPOSITEGLYPH_HH
#تعریف_کړه OT_GLYF_COMPOSITEGLYPH_HH


#ضمن "../../hb-open-type.hh"
#ضمن "composite-iter.hh"


نطاق OT {
نطاق glyf_impl {


بنية مرکب_Glyph_ریکارډ
{
  محمي:
  تعداد مرکب_glyph_flag_t
  {
    ARG_1_AND_2_ARE_ټکي	= 0x0001,
    دلیلونه_ARE_XY_ارزښتونه		= 0x0002,
    پړاو_XY_منزل_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    نور_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_دننه_واټن	= 0x0800,
    UNSCALED_COMPONENT_دننه_واټن	= 0x1000,
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    GID_IS_24_بټ		= 0x2000
#د_که_پای
  };

  عام:
  size_t get_اندازه (منطقي extended) ثابت
  {
    بینښه صحيح اندازه = min_اندازه;
    /* glyphIndex is 24bit instead of 16bit */
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بټ)) اندازه += HBGlyphID24::static_اندازه - HBGlyphID16::static_اندازه;
#د_که_پای
    /* arg1 and 2 are int16 */
    إذا (flags & ARG_1_AND_2_ARE_ټکي) اندازه += 4;
    /* arg1 and 2 are int8 */
    وإلا اندازه += 2;

    /* One x 16 bit (scale) */
    إذا (flags & WE_HAVE_A_SCALE) اندازه += 2;
    /* Two x 16 bit (xscale, yscale) */
    وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE) اندازه += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO) اندازه += 8;

    أرجع اندازه;
  }

  فراغ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  فراغ سېټ_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقي has_instructions ()  ثابت { أرجع   flags & WE_HAVE_INSTRUCTIONS; }

  منطقي has_نور ()          ثابت { أرجع   flags & نور_COMPONENTS; }
  منطقي is_use_my_metrics () ثابت { أرجع   flags & USE_MY_METRICS; }
  منطقي is_anchored ()       ثابت { أرجع !(flags & دلیلونه_ARE_XY_ارزښتونه); }
  فراغ get_anchor_points (بینښه صحيح &point1, بینښه صحيح &point2, منطقي extended) ثابت
  {
    ثابت خپلکاری *p = &StructAfter<ثابت HBUINT8> (flags);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بټ))
      p += HBGlyphID24::static_اندازه;
    وإلا
#د_که_پای
      p += HBGlyphID16::static_اندازه;
    إذا (flags & ARG_1_AND_2_ARE_ټکي)
    {
      point1 = ((ثابت HBUINT16 *) p)[0];
      point2 = ((ثابت HBUINT16 *) p)[1];
    }
    وإلا
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ساكن فراغ transform (ثابت عائم (&ماتریکس)[4],
			 hb_array_t<contour_ټکی_t> points)
  {
    إذا (ماتریکس[0] != 1.f || ماتریکس[1] != 0.f ||
	ماتریکس[2] != 0.f || ماتریکس[3] != 1.f)
      لكل (خپلکاری &ټکی : points)
        ټکی.transform (ماتریکس);
  }

  ساكن فراغ translate (ثابت contour_ټکی_t &trans,
			 hb_array_t<contour_ټکی_t> points)
  {
    إذا (HB_OPTIMIZE_اندازه_VAL)
    {
      إذا (trans.x != 0.f || trans.y != 0.f)
        لكل (خپلکاری &ټکی : points)
	  ټکی.translate (trans);
    }
    وإلا
    {
      إذا (trans.x != 0.f && trans.y != 0.f)
        لكل (خپلکاری &ټکی : points)
	  ټکی.translate (trans);
      وإلا
      {
	إذا (trans.x != 0.f)
	  لكل (خپلکاری &ټکی : points)
	    ټکی.x += trans.x;
	وإلا إذا (trans.y != 0.f)
	  لكل (خپلکاری &ټکی : points)
	    ټکی.y += trans.y;
      }
    }
  }

  فراغ transform_points (hb_array_t<contour_ټکی_t> points,
			 ثابت عائم (&ماتریکس)[4],
			 ثابت contour_ټکی_t &trans) ثابت
  {
    إذا (scaled_offsets ())
    {
      translate (trans, points);
      transform (ماتریکس, points);
    }
    وإلا
    {
      transform (ماتریکس, points);
      translate (trans, points);
    }
  }

  منطقي get_points (contour_ټکی_ویکټور_t &points, منطقي extended) ثابت
  {
    عائم ماتریکس[4];
    contour_ټکی_t trans;
    get_transformation (ماتریکس, trans, extended);
    إذا (unlikely (!points.alloc (points.اوږدتیا + 1 + 4))) أرجع خطأ; // For phantom points
    points.push (trans);
    أرجع ریښتیا;
  }

  بینښه compile_with_ټکی (ثابت contour_ټکی_t &ټکی,
                               حرف *بهر,
			       منطقي extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بټ))
      p += HBGlyphID24::static_اندازه;
    وإلا
#د_که_پای
      p += HBGlyphID16::static_اندازه;

    بینښه len = get_اندازه (extended);
    بینښه len_before_val = (ثابت حرف *)p - (ثابت حرف *)هذا;
    إذا (flags & ARG_1_AND_2_ARE_ټکي)
    {
      // no overflow, copy value
      hb_memcpy (بهر, هذا, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (بهر + len_before_val);
      o[0] = roundf (ټکی.x);
      o[1] = roundf (ټکی.y);
    }
    وإلا
    {
      صحيح new_x = roundf (ټکی.x);
      صحيح new_y = roundf (ټکی.y);
      إذا (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (بهر, هذا, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (بهر + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      وإلا
      {
        // new point value has an int8 overflow
        hb_memcpy (بهر, هذا, len_before_val);
        
        //update flags
        مرکب_Glyph_ریکارډ *o = reinterpret_cast<مرکب_Glyph_ریکارډ *> (بهر);
        o->flags = flags | ARG_1_AND_2_ARE_ټکي;
        بهر += len_before_val;

        HBINT16 new_ارزښت;
        new_ارزښت = new_x;
        hb_memcpy (بهر, &new_ارزښت, HBINT16::static_اندازه);
        بهر += HBINT16::static_اندازه;

        new_ارزښت = new_y;
        hb_memcpy (بهر, &new_ارزښت, HBINT16::static_اندازه);
        بهر += HBINT16::static_اندازه;

        hb_memcpy (بهر, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    أرجع len;
  }

  محمي:
  منطقي scaled_offsets () ثابت
  { أرجع (flags & (SCALED_COMPONENT_دننه_واټن | UNSCALED_COMPONENT_دننه_واټن)) == SCALED_COMPONENT_دننه_واټن; }

  عام:
  منطقي get_transformation (عائم (&ماتریکس)[4], contour_ټکی_t &trans, منطقي extended) ثابت
  {
    ماتریکس[0] = ماتریکس[3] = 1.f;
    ماتریکس[1] = ماتریکس[2] = 0.f;

    ثابت خپلکاری *p = &StructAfter<ثابت HBINT8> (flags);
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بټ))
      p += HBGlyphID24::static_اندازه;
    وإلا
#د_که_پای
      p += HBGlyphID16::static_اندازه;
    صحيح tx, ty;
    إذا (flags & ARG_1_AND_2_ARE_ټکي)
    {
      tx = *(ثابت HBINT16 *) p;
      p += HBINT16::static_اندازه;
      ty = *(ثابت HBINT16 *) p;
      p += HBINT16::static_اندازه;
    }
    وإلا
    {
      tx = *p++;
      ty = *p++;
    }
    إذا (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((عائم) tx, (عائم) ty, ریښتیا);

    {
      ثابت F2DOT14 *points = (ثابت F2DOT14 *) p;
      إذا (flags & WE_HAVE_A_SCALE)
      {
	ماتریکس[0] = ماتریکس[3] = points[0].منزل_float ();
	أرجع ریښتیا;
      }
      وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	ماتریکس[0] = points[0].منزل_float ();
	ماتریکس[3] = points[1].منزل_float ();
	أرجع ریښتیا;
      }
      وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	ماتریکس[0] = points[0].منزل_float ();
	ماتریکس[1] = points[1].منزل_float ();
	ماتریکس[2] = points[2].منزل_float ();
	ماتریکس[3] = points[3].منزل_float ();
	أرجع ریښتیا;
      }
    }
    أرجع tx || ty;
  }

  hb_codepoint_t get_gid (منطقي extended) ثابت
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بټ))
      أرجع StructAfter<ثابت HBGlyphID24> (flags);
    وإلا
#د_که_پای
      أرجع StructAfter<ثابت HBGlyphID16> (flags);
  }
  فراغ سېټ_gid (hb_codepoint_t gid, منطقي extended)
  {
#که_نه_تعریف_شوی HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بټ))
      StructAfter<HBGlyphID24> (flags) = gid;
    وإلا
#د_که_پای
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#که_نه_تعریف_شوی HB_NO_BEYOND_64K
  فراغ ښکتنی_gid_24_منزل_16 ()
  {
    hb_codepoint_t gid = get_gid (ریښتیا);
    إذا (!(flags & GID_IS_24_بټ) || gid > 0xFFFFu)
      أرجع;

    /* Lower the flag and move the rest of the struct down. */

    بینښه اندازه = get_اندازه (ریښتیا);
    حرف *پای_ټکی = (حرف *) هذا + اندازه;
    حرف *p = &StructAfter<حرف> (flags);
    p += HBGlyphID24::static_اندازه;

    flags = flags & ~GID_IS_24_بټ;
    سېټ_gid (gid, ریښتیا);

    memmove (p - HBGlyphID24::static_اندازه + HBGlyphID16::static_اندازه, p, پای_ټکی - p);
  }
#د_که_پای

  محمي:
  HBUINT16	flags;
  HBUINT24	pad;
  عام:
  DEFINE_اندازه_MIN (4);
};

کارول مرکب_iter_t = مرکب_iter_tmpl<مرکب_Glyph_ریکارډ>;

بنية مرکب_Glyph
{
  ثابت Glyph_سرلیک &سرلیک;
  hb_بایټونه_t بایټونه;
  منطقي extended;
  مرکب_Glyph (ثابت Glyph_سرلیک &سرلیک_2, hb_بایټونه_t بایټونه_2, منطقي extended_) :
    سرلیک (سرلیک_2), بایټونه (بایټونه_2), extended (extended_) {}

  مرکب_iter_t iter () ثابت
  { أرجع مرکب_iter_t (بایټونه, &StructAfter<مرکب_Glyph_ریکارډ, Glyph_سرلیک> (سرلیک), extended); }

  بینښه صحيح instructions_اوږدتیا (hb_بایټونه_t بایټونه) ثابت
  {
    بینښه صحيح پیل = بایټونه.اوږدتیا;
    بینښه صحيح پای_ټکی = بایټونه.اوږدتیا;
    ثابت مرکب_Glyph_ریکارډ *وروستی = تش_اشاره;
    لكل (خپلکاری &توکی : iter ())
      وروستی = &توکی;
    إذا (unlikely (!وروستی)) أرجع 0;

    إذا (وروستی->has_instructions ())
      پیل = (حرف *) وروستی - &بایټونه + وروستی->get_اندازه (extended);
    إذا (unlikely (پیل > پای_ټکی)) أرجع 0;
    أرجع پای_ټکی - پیل;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_بایټونه_t trim_padding () ثابت { أرجع بایټونه; }

  فراغ drop_hints ()
  {
    لكل (ثابت خپلکاری &_ : iter ())
      const_cast<مرکب_Glyph_ریکارډ &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  فراغ drop_hints_بایټونه (hb_بایټونه_t &dest_پیل) ثابت
  { dest_پیل = بایټونه.sub_array (0, بایټونه.اوږدتیا - instructions_اوږدتیا (بایټونه)); }

  فراغ سېټ_overlaps_flag ()
  {
    مرکب_Glyph_ریکارډ& glyph_chain = const_cast<مرکب_Glyph_ریکارډ &> (
	StructAfter<مرکب_Glyph_ریکارډ, Glyph_سرلیک> (سرلیک));
    إذا (!بایټونه.check_range(&glyph_chain, مرکب_Glyph_ریکارډ::min_اندازه))
      أرجع;
    glyph_chain.سېټ_overlaps_flag ();
  }

  منطقي compile_بایټونه_with_deltas (ثابت hb_بایټونه_t &سرچینه_بایټونه,
                                  ثابت contour_ټکی_ویکټور_t &points_with_deltas,
                                  hb_بایټونه_t &dest_بایټونه /*بهر*/)
  {
    إذا (سرچینه_بایټونه.اوږدتیا <= Glyph_سرلیک::static_اندازه ||
        سرلیک.شمېره_د_Contours != -1)
    {
      dest_بایټونه = hb_بایټونه_t ();
      أرجع ریښتیا;
    }

    بینښه سرچینه_len = سرچینه_بایټونه.اوږدتیا - Glyph_سرلیک::static_اندازه;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    حرف *o = (حرف *) hb_calloc (سرچینه_len * 2, حجم (حرف));
    إذا (unlikely (!o)) أرجع خطأ;

    ثابت مرکب_Glyph_ریکارډ *c = reinterpret_cast<ثابت مرکب_Glyph_ریکارډ *> (سرچینه_بایټونه.arrayZ + Glyph_سرلیک::static_اندازه);
    خپلکاری it = مرکب_iter_t (hb_بایټونه_t ((ثابت حرف *)c, سرچینه_len), c, extended);

    حرف *p = o;
    بینښه i = 0, سرچینه_comp_len = 0;
    لكل (ثابت خپلکاری &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      إذا (i >= points_with_deltas.اوږدتیا - 4) {
        hb_free (o);
        أرجع خطأ;
      }

      بینښه comp_len = component.get_اندازه (extended);
      إذا (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      وإلا
      {
        بینښه new_len = component.compile_with_ټکی (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      سرچینه_comp_len += comp_len;
    }

    //copy instructions if any
    إذا (سرچینه_len > سرچینه_comp_len)
    {
      بینښه instr_len = سرچینه_len - سرچینه_comp_len;
      hb_memcpy (p, (ثابت حرف *)c + سرچینه_comp_len, instr_len);
      p += instr_len;
    }

    بینښه len = p - o;
    dest_بایټونه = hb_بایټونه_t (o, len);
    أرجع ریښتیا;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#د_که_پای /* OT_GLYF_COMPOSITEGLYPH_HH */

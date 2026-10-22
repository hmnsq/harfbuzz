#اگر_تعریف_نشده OT_GLYF_COMPOSITEGLYPH_HH
#تعریف_کن OT_GLYF_COMPOSITEGLYPH_HH


#ضمن "../../hb-open-type.hh"
#ضمن "composite-iter.hh"


نطاق OT {
نطاق glyf_impl {


بنية مرکب_Glyph_رکورد
{
  محمي:
  تعداد مرکب_glyph_flag_t
  {
    ARG_1_AND_2_ARE_واژگان	= 0x0001,
    آرگومانها_ARE_XY_مقادیر		= 0x0002,
    دور_XY_مقصد_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    بیشتر_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_جابه_جایی	= 0x0800,
    UNSCALED_COMPONENT_جابه_جایی	= 0x1000,
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    GID_IS_24_بیت		= 0x2000
#پایان_اگر
  };

  عام:
  size_t get_اندازه (منطقي extended) ثابت
  {
    بیعلامت صحيح اندازه = min_اندازه;
    /* glyphIndex is 24bit instead of 16bit */
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بیت)) اندازه += HBGlyphID24::static_اندازه - HBGlyphID16::static_اندازه;
#پایان_اگر
    /* arg1 and 2 are int16 */
    إذا (flags & ARG_1_AND_2_ARE_واژگان) اندازه += 4;
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
  فراغ مجموعه_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقي has_instructions ()  ثابت { أرجع   flags & WE_HAVE_INSTRUCTIONS; }

  منطقي has_بیشتر ()          ثابت { أرجع   flags & بیشتر_COMPONENTS; }
  منطقي is_use_my_metrics () ثابت { أرجع   flags & USE_MY_METRICS; }
  منطقي is_anchored ()       ثابت { أرجع !(flags & آرگومانها_ARE_XY_مقادیر); }
  فراغ get_anchor_points (بیعلامت صحيح &point1, بیعلامت صحيح &point2, منطقي extended) ثابت
  {
    ثابت خودکار *p = &StructAfter<ثابت HBUINT8> (flags);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بیت))
      p += HBGlyphID24::static_اندازه;
    وإلا
#پایان_اگر
      p += HBGlyphID16::static_اندازه;
    إذا (flags & ARG_1_AND_2_ARE_واژگان)
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

  ساكن فراغ transform (ثابت عائم (&ماتریس)[4],
			 hb_array_t<contour_نقطه_t> points)
  {
    إذا (ماتریس[0] != 1.f || ماتریس[1] != 0.f ||
	ماتریس[2] != 0.f || ماتریس[3] != 1.f)
      لكل (خودکار &نقطه : points)
        نقطه.transform (ماتریس);
  }

  ساكن فراغ translate (ثابت contour_نقطه_t &trans,
			 hb_array_t<contour_نقطه_t> points)
  {
    إذا (HB_OPTIMIZE_اندازه_VAL)
    {
      إذا (trans.x != 0.f || trans.y != 0.f)
        لكل (خودکار &نقطه : points)
	  نقطه.translate (trans);
    }
    وإلا
    {
      إذا (trans.x != 0.f && trans.y != 0.f)
        لكل (خودکار &نقطه : points)
	  نقطه.translate (trans);
      وإلا
      {
	إذا (trans.x != 0.f)
	  لكل (خودکار &نقطه : points)
	    نقطه.x += trans.x;
	وإلا إذا (trans.y != 0.f)
	  لكل (خودکار &نقطه : points)
	    نقطه.y += trans.y;
      }
    }
  }

  فراغ transform_points (hb_array_t<contour_نقطه_t> points,
			 ثابت عائم (&ماتریس)[4],
			 ثابت contour_نقطه_t &trans) ثابت
  {
    إذا (scaled_offsets ())
    {
      translate (trans, points);
      transform (ماتریس, points);
    }
    وإلا
    {
      transform (ماتریس, points);
      translate (trans, points);
    }
  }

  منطقي get_points (contour_نقطه_وکتور_t &points, منطقي extended) ثابت
  {
    عائم ماتریس[4];
    contour_نقطه_t trans;
    get_transformation (ماتریس, trans, extended);
    إذا (unlikely (!points.alloc (points.درازی + 1 + 4))) أرجع خطأ; // For phantom points
    points.push (trans);
    أرجع درست;
  }

  بیعلامت compile_with_نقطه (ثابت contour_نقطه_t &نقطه,
                               حرف *بیرون,
			       منطقي extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بیت))
      p += HBGlyphID24::static_اندازه;
    وإلا
#پایان_اگر
      p += HBGlyphID16::static_اندازه;

    بیعلامت len = get_اندازه (extended);
    بیعلامت len_before_val = (ثابت حرف *)p - (ثابت حرف *)هذا;
    إذا (flags & ARG_1_AND_2_ARE_واژگان)
    {
      // no overflow, copy value
      hb_memcpy (بیرون, هذا, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (بیرون + len_before_val);
      o[0] = roundf (نقطه.x);
      o[1] = roundf (نقطه.y);
    }
    وإلا
    {
      صحيح new_x = roundf (نقطه.x);
      صحيح new_y = roundf (نقطه.y);
      إذا (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (بیرون, هذا, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (بیرون + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      وإلا
      {
        // new point value has an int8 overflow
        hb_memcpy (بیرون, هذا, len_before_val);
        
        //update flags
        مرکب_Glyph_رکورد *o = reinterpret_cast<مرکب_Glyph_رکورد *> (بیرون);
        o->flags = flags | ARG_1_AND_2_ARE_واژگان;
        بیرون += len_before_val;

        HBINT16 new_مقدار;
        new_مقدار = new_x;
        hb_memcpy (بیرون, &new_مقدار, HBINT16::static_اندازه);
        بیرون += HBINT16::static_اندازه;

        new_مقدار = new_y;
        hb_memcpy (بیرون, &new_مقدار, HBINT16::static_اندازه);
        بیرون += HBINT16::static_اندازه;

        hb_memcpy (بیرون, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    أرجع len;
  }

  محمي:
  منطقي scaled_offsets () ثابت
  { أرجع (flags & (SCALED_COMPONENT_جابه_جایی | UNSCALED_COMPONENT_جابه_جایی)) == SCALED_COMPONENT_جابه_جایی; }

  عام:
  منطقي get_transformation (عائم (&ماتریس)[4], contour_نقطه_t &trans, منطقي extended) ثابت
  {
    ماتریس[0] = ماتریس[3] = 1.f;
    ماتریس[1] = ماتریس[2] = 0.f;

    ثابت خودکار *p = &StructAfter<ثابت HBINT8> (flags);
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بیت))
      p += HBGlyphID24::static_اندازه;
    وإلا
#پایان_اگر
      p += HBGlyphID16::static_اندازه;
    صحيح tx, ty;
    إذا (flags & ARG_1_AND_2_ARE_واژگان)
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
    trans.init ((عائم) tx, (عائم) ty, درست);

    {
      ثابت F2DOT14 *points = (ثابت F2DOT14 *) p;
      إذا (flags & WE_HAVE_A_SCALE)
      {
	ماتریس[0] = ماتریس[3] = points[0].مقصد_float ();
	أرجع درست;
      }
      وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	ماتریس[0] = points[0].مقصد_float ();
	ماتریس[3] = points[1].مقصد_float ();
	أرجع درست;
      }
      وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	ماتریس[0] = points[0].مقصد_float ();
	ماتریس[1] = points[1].مقصد_float ();
	ماتریس[2] = points[2].مقصد_float ();
	ماتریس[3] = points[3].مقصد_float ();
	أرجع درست;
      }
    }
    أرجع tx || ty;
  }

  hb_codepoint_t get_gid (منطقي extended) ثابت
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بیت))
      أرجع StructAfter<ثابت HBGlyphID24> (flags);
    وإلا
#پایان_اگر
      أرجع StructAfter<ثابت HBGlyphID16> (flags);
  }
  فراغ مجموعه_gid (hb_codepoint_t gid, منطقي extended)
  {
#اگر_تعریف_نشده HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_بیت))
      StructAfter<HBGlyphID24> (flags) = gid;
    وإلا
#پایان_اگر
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#اگر_تعریف_نشده HB_NO_BEYOND_64K
  فراغ پایینی_gid_24_مقصد_16 ()
  {
    hb_codepoint_t gid = get_gid (درست);
    إذا (!(flags & GID_IS_24_بیت) || gid > 0xFFFFu)
      أرجع;

    /* Lower the flag and move the rest of the struct down. */

    بیعلامت اندازه = get_اندازه (درست);
    حرف *انتها = (حرف *) هذا + اندازه;
    حرف *p = &StructAfter<حرف> (flags);
    p += HBGlyphID24::static_اندازه;

    flags = flags & ~GID_IS_24_بیت;
    مجموعه_gid (gid, درست);

    memmove (p - HBGlyphID24::static_اندازه + HBGlyphID16::static_اندازه, p, انتها - p);
  }
#پایان_اگر

  محمي:
  HBUINT16	flags;
  HBUINT24	pad;
  عام:
  DEFINE_اندازه_MIN (4);
};

استفاده مرکب_iter_t = مرکب_iter_tmpl<مرکب_Glyph_رکورد>;

بنية مرکب_Glyph
{
  ثابت Glyph_سرآیند &سرآیند;
  hb_بایتها_t بایتها;
  منطقي extended;
  مرکب_Glyph (ثابت Glyph_سرآیند &سرآیند_2, hb_بایتها_t بایتها_2, منطقي extended_) :
    سرآیند (سرآیند_2), بایتها (بایتها_2), extended (extended_) {}

  مرکب_iter_t iter () ثابت
  { أرجع مرکب_iter_t (بایتها, &StructAfter<مرکب_Glyph_رکورد, Glyph_سرآیند> (سرآیند), extended); }

  بیعلامت صحيح instructions_درازی (hb_بایتها_t بایتها) ثابت
  {
    بیعلامت صحيح شروع = بایتها.درازی;
    بیعلامت صحيح انتها = بایتها.درازی;
    ثابت مرکب_Glyph_رکورد *آخرین = اشاره_تهی;
    لكل (خودکار &قلم : iter ())
      آخرین = &قلم;
    إذا (unlikely (!آخرین)) أرجع 0;

    إذا (آخرین->has_instructions ())
      شروع = (حرف *) آخرین - &بایتها + آخرین->get_اندازه (extended);
    إذا (unlikely (شروع > انتها)) أرجع 0;
    أرجع انتها - شروع;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_بایتها_t trim_padding () ثابت { أرجع بایتها; }

  فراغ drop_hints ()
  {
    لكل (ثابت خودکار &_ : iter ())
      const_cast<مرکب_Glyph_رکورد &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  فراغ drop_hints_بایتها (hb_بایتها_t &dest_شروع) ثابت
  { dest_شروع = بایتها.sub_array (0, بایتها.درازی - instructions_درازی (بایتها)); }

  فراغ مجموعه_overlaps_flag ()
  {
    مرکب_Glyph_رکورد& glyph_chain = const_cast<مرکب_Glyph_رکورد &> (
	StructAfter<مرکب_Glyph_رکورد, Glyph_سرآیند> (سرآیند));
    إذا (!بایتها.check_range(&glyph_chain, مرکب_Glyph_رکورد::min_اندازه))
      أرجع;
    glyph_chain.مجموعه_overlaps_flag ();
  }

  منطقي compile_بایتها_with_deltas (ثابت hb_بایتها_t &منبع_بایتها,
                                  ثابت contour_نقطه_وکتور_t &points_with_deltas,
                                  hb_بایتها_t &dest_بایتها /*بیرون*/)
  {
    إذا (منبع_بایتها.درازی <= Glyph_سرآیند::static_اندازه ||
        سرآیند.عدد_از_Contours != -1)
    {
      dest_بایتها = hb_بایتها_t ();
      أرجع درست;
    }

    بیعلامت منبع_len = منبع_بایتها.درازی - Glyph_سرآیند::static_اندازه;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    حرف *o = (حرف *) hb_calloc (منبع_len * 2, حجم (حرف));
    إذا (unlikely (!o)) أرجع خطأ;

    ثابت مرکب_Glyph_رکورد *c = reinterpret_cast<ثابت مرکب_Glyph_رکورد *> (منبع_بایتها.arrayZ + Glyph_سرآیند::static_اندازه);
    خودکار it = مرکب_iter_t (hb_بایتها_t ((ثابت حرف *)c, منبع_len), c, extended);

    حرف *p = o;
    بیعلامت i = 0, منبع_comp_len = 0;
    لكل (ثابت خودکار &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      إذا (i >= points_with_deltas.درازی - 4) {
        hb_free (o);
        أرجع خطأ;
      }

      بیعلامت comp_len = component.get_اندازه (extended);
      إذا (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      وإلا
      {
        بیعلامت new_len = component.compile_with_نقطه (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      منبع_comp_len += comp_len;
    }

    //copy instructions if any
    إذا (منبع_len > منبع_comp_len)
    {
      بیعلامت instr_len = منبع_len - منبع_comp_len;
      hb_memcpy (p, (ثابت حرف *)c + منبع_comp_len, instr_len);
      p += instr_len;
    }

    بیعلامت len = p - o;
    dest_بایتها = hb_بایتها_t (o, len);
    أرجع درست;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#پایان_اگر /* OT_GLYF_COMPOSITEGLYPH_HH */

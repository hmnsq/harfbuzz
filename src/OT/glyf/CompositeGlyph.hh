#جے_بنا_تعریف OT_GLYF_COMPOSITEGLYPH_HH
#تعریف_کرو OT_GLYF_COMPOSITEGLYPH_HH


#ضمن "../../hb-open-type.hh"
#ضمن "composite-iter.hh"


نطاق OT {
نطاق glyf_impl {


بنية مرکب_Glyph_ریکارڈ
{
  محمي:
  تعداد مرکب_glyph_flag_t
  {
    ARG_1_AND_2_ARE_الفاظ	= 0x0001,
    دلیلاں_ARE_XY_اقدار		= 0x0002,
    گیڑا_XY_منزل_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    ہور_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_وتھ	= 0x0800,
    UNSCALED_COMPONENT_وتھ	= 0x1000,
#جے_بنا_تعریف HB_NO_BEYOND_64K
    GID_IS_24_ثنائی_ہندسہ		= 0x2000
#جے_انت
  };

  عام:
  size_t get_ناپ (منطقي extended) ثابت
  {
    بنانشانی صحيح ناپ = min_ناپ;
    /* glyphIndex is 24bit instead of 16bit */
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ثنائی_ہندسہ)) ناپ += HBGlyphID24::static_ناپ - HBGlyphID16::static_ناپ;
#جے_انت
    /* arg1 and 2 are int16 */
    إذا (flags & ARG_1_AND_2_ARE_الفاظ) ناپ += 4;
    /* arg1 and 2 are int8 */
    وإلا ناپ += 2;

    /* One x 16 bit (scale) */
    إذا (flags & WE_HAVE_A_SCALE) ناپ += 2;
    /* Two x 16 bit (xscale, yscale) */
    وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE) ناپ += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO) ناپ += 8;

    أرجع ناپ;
  }

  فراغ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  فراغ سیٹ_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقي has_instructions ()  ثابت { أرجع   flags & WE_HAVE_INSTRUCTIONS; }

  منطقي has_ہور ()          ثابت { أرجع   flags & ہور_COMPONENTS; }
  منطقي is_use_my_metrics () ثابت { أرجع   flags & USE_MY_METRICS; }
  منطقي is_anchored ()       ثابت { أرجع !(flags & دلیلاں_ARE_XY_اقدار); }
  فراغ get_anchor_points (بنانشانی صحيح &point1, بنانشانی صحيح &point2, منطقي extended) ثابت
  {
    ثابت خودکار *p = &StructAfter<ثابت HBUINT8> (flags);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      p += HBGlyphID24::static_ناپ;
    وإلا
#جے_انت
      p += HBGlyphID16::static_ناپ;
    إذا (flags & ARG_1_AND_2_ARE_الفاظ)
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

  ساكن فراغ transform (ثابت عائم (&میٹرکس)[4],
			 hb_array_t<contour_نقطہ_t> points)
  {
    إذا (میٹرکس[0] != 1.f || میٹرکس[1] != 0.f ||
	میٹرکس[2] != 0.f || میٹرکس[3] != 1.f)
      لكل (خودکار &نقطہ : points)
        نقطہ.transform (میٹرکس);
  }

  ساكن فراغ translate (ثابت contour_نقطہ_t &trans,
			 hb_array_t<contour_نقطہ_t> points)
  {
    إذا (HB_OPTIMIZE_ناپ_VAL)
    {
      إذا (trans.x != 0.f || trans.y != 0.f)
        لكل (خودکار &نقطہ : points)
	  نقطہ.translate (trans);
    }
    وإلا
    {
      إذا (trans.x != 0.f && trans.y != 0.f)
        لكل (خودکار &نقطہ : points)
	  نقطہ.translate (trans);
      وإلا
      {
	إذا (trans.x != 0.f)
	  لكل (خودکار &نقطہ : points)
	    نقطہ.x += trans.x;
	وإلا إذا (trans.y != 0.f)
	  لكل (خودکار &نقطہ : points)
	    نقطہ.y += trans.y;
      }
    }
  }

  فراغ transform_points (hb_array_t<contour_نقطہ_t> points,
			 ثابت عائم (&میٹرکس)[4],
			 ثابت contour_نقطہ_t &trans) ثابت
  {
    إذا (scaled_offsets ())
    {
      translate (trans, points);
      transform (میٹرکس, points);
    }
    وإلا
    {
      transform (میٹرکس, points);
      translate (trans, points);
    }
  }

  منطقي get_points (contour_نقطہ_سمتیہ_t &points, منطقي extended) ثابت
  {
    عائم میٹرکس[4];
    contour_نقطہ_t trans;
    get_transformation (میٹرکس, trans, extended);
    إذا (unlikely (!points.alloc (points.لمائی + 1 + 4))) أرجع خطأ; // For phantom points
    points.push (trans);
    أرجع سچ;
  }

  بنانشانی compile_with_نقطہ (ثابت contour_نقطہ_t &نقطہ,
                               حرف *باہرلا,
			       منطقي extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      p += HBGlyphID24::static_ناپ;
    وإلا
#جے_انت
      p += HBGlyphID16::static_ناپ;

    بنانشانی len = get_ناپ (extended);
    بنانشانی len_before_val = (ثابت حرف *)p - (ثابت حرف *)هذا;
    إذا (flags & ARG_1_AND_2_ARE_الفاظ)
    {
      // no overflow, copy value
      hb_memcpy (باہرلا, هذا, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (باہرلا + len_before_val);
      o[0] = roundf (نقطہ.x);
      o[1] = roundf (نقطہ.y);
    }
    وإلا
    {
      صحيح new_x = roundf (نقطہ.x);
      صحيح new_y = roundf (نقطہ.y);
      إذا (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (باہرلا, هذا, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (باہرلا + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      وإلا
      {
        // new point value has an int8 overflow
        hb_memcpy (باہرلا, هذا, len_before_val);
        
        //update flags
        مرکب_Glyph_ریکارڈ *o = reinterpret_cast<مرکب_Glyph_ریکارڈ *> (باہرلا);
        o->flags = flags | ARG_1_AND_2_ARE_الفاظ;
        باہرلا += len_before_val;

        HBINT16 new_قدر;
        new_قدر = new_x;
        hb_memcpy (باہرلا, &new_قدر, HBINT16::static_ناپ);
        باہرلا += HBINT16::static_ناپ;

        new_قدر = new_y;
        hb_memcpy (باہرلا, &new_قدر, HBINT16::static_ناپ);
        باہرلا += HBINT16::static_ناپ;

        hb_memcpy (باہرلا, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    أرجع len;
  }

  محمي:
  منطقي scaled_offsets () ثابت
  { أرجع (flags & (SCALED_COMPONENT_وتھ | UNSCALED_COMPONENT_وتھ)) == SCALED_COMPONENT_وتھ; }

  عام:
  منطقي get_transformation (عائم (&میٹرکس)[4], contour_نقطہ_t &trans, منطقي extended) ثابت
  {
    میٹرکس[0] = میٹرکس[3] = 1.f;
    میٹرکس[1] = میٹرکس[2] = 0.f;

    ثابت خودکار *p = &StructAfter<ثابت HBINT8> (flags);
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      p += HBGlyphID24::static_ناپ;
    وإلا
#جے_انت
      p += HBGlyphID16::static_ناپ;
    صحيح tx, ty;
    إذا (flags & ARG_1_AND_2_ARE_الفاظ)
    {
      tx = *(ثابت HBINT16 *) p;
      p += HBINT16::static_ناپ;
      ty = *(ثابت HBINT16 *) p;
      p += HBINT16::static_ناپ;
    }
    وإلا
    {
      tx = *p++;
      ty = *p++;
    }
    إذا (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((عائم) tx, (عائم) ty, سچ);

    {
      ثابت F2DOT14 *points = (ثابت F2DOT14 *) p;
      إذا (flags & WE_HAVE_A_SCALE)
      {
	میٹرکس[0] = میٹرکس[3] = points[0].منزل_float ();
	أرجع سچ;
      }
      وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	میٹرکس[0] = points[0].منزل_float ();
	میٹرکس[3] = points[1].منزل_float ();
	أرجع سچ;
      }
      وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	میٹرکس[0] = points[0].منزل_float ();
	میٹرکس[1] = points[1].منزل_float ();
	میٹرکس[2] = points[2].منزل_float ();
	میٹرکس[3] = points[3].منزل_float ();
	أرجع سچ;
      }
    }
    أرجع tx || ty;
  }

  hb_codepoint_t get_gid (منطقي extended) ثابت
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      أرجع StructAfter<ثابت HBGlyphID24> (flags);
    وإلا
#جے_انت
      أرجع StructAfter<ثابت HBGlyphID16> (flags);
  }
  فراغ سیٹ_gid (hb_codepoint_t gid, منطقي extended)
  {
#جے_بنا_تعریف HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      StructAfter<HBGlyphID24> (flags) = gid;
    وإلا
#جے_انت
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#جے_بنا_تعریف HB_NO_BEYOND_64K
  فراغ ہیٹھلا_gid_24_منزل_16 ()
  {
    hb_codepoint_t gid = get_gid (سچ);
    إذا (!(flags & GID_IS_24_ثنائی_ہندسہ) || gid > 0xFFFFu)
      أرجع;

    /* Lower the flag and move the rest of the struct down. */

    بنانشانی ناپ = get_ناپ (سچ);
    حرف *سرا = (حرف *) هذا + ناپ;
    حرف *p = &StructAfter<حرف> (flags);
    p += HBGlyphID24::static_ناپ;

    flags = flags & ~GID_IS_24_ثنائی_ہندسہ;
    سیٹ_gid (gid, سچ);

    memmove (p - HBGlyphID24::static_ناپ + HBGlyphID16::static_ناپ, p, سرا - p);
  }
#جے_انت

  محمي:
  HBUINT16	flags;
  HBUINT24	pad;
  عام:
  DEFINE_ناپ_MIN (4);
};

ورتوں مرکب_iter_t = مرکب_iter_tmpl<مرکب_Glyph_ریکارڈ>;

بنية مرکب_Glyph
{
  ثابت Glyph_سرخی &سرخی;
  hb_بائٹاں_t بائٹاں;
  منطقي extended;
  مرکب_Glyph (ثابت Glyph_سرخی &سرخی_2, hb_بائٹاں_t بائٹاں_2, منطقي extended_) :
    سرخی (سرخی_2), بائٹاں (بائٹاں_2), extended (extended_) {}

  مرکب_iter_t iter () ثابت
  { أرجع مرکب_iter_t (بائٹاں, &StructAfter<مرکب_Glyph_ریکارڈ, Glyph_سرخی> (سرخی), extended); }

  بنانشانی صحيح instructions_لمائی (hb_بائٹاں_t بائٹاں) ثابت
  {
    بنانشانی صحيح شروع = بائٹاں.لمائی;
    بنانشانی صحيح سرا = بائٹاں.لمائی;
    ثابت مرکب_Glyph_ریکارڈ *آخری = خالی_سوچک;
    لكل (خودکار &شے : iter ())
      آخری = &شے;
    إذا (unlikely (!آخری)) أرجع 0;

    إذا (آخری->has_instructions ())
      شروع = (حرف *) آخری - &بائٹاں + آخری->get_ناپ (extended);
    إذا (unlikely (شروع > سرا)) أرجع 0;
    أرجع سرا - شروع;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_بائٹاں_t trim_padding () ثابت { أرجع بائٹاں; }

  فراغ drop_hints ()
  {
    لكل (ثابت خودکار &_ : iter ())
      const_cast<مرکب_Glyph_ریکارڈ &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  فراغ drop_hints_بائٹاں (hb_بائٹاں_t &dest_شروع) ثابت
  { dest_شروع = بائٹاں.sub_array (0, بائٹاں.لمائی - instructions_لمائی (بائٹاں)); }

  فراغ سیٹ_overlaps_flag ()
  {
    مرکب_Glyph_ریکارڈ& glyph_chain = const_cast<مرکب_Glyph_ریکارڈ &> (
	StructAfter<مرکب_Glyph_ریکارڈ, Glyph_سرخی> (سرخی));
    إذا (!بائٹاں.check_range(&glyph_chain, مرکب_Glyph_ریکارڈ::min_ناپ))
      أرجع;
    glyph_chain.سیٹ_overlaps_flag ();
  }

  منطقي compile_بائٹاں_with_deltas (ثابت hb_بائٹاں_t &سروت_بائٹاں,
                                  ثابت contour_نقطہ_سمتیہ_t &points_with_deltas,
                                  hb_بائٹاں_t &dest_بائٹاں /*باہرلا*/)
  {
    إذا (سروت_بائٹاں.لمائی <= Glyph_سرخی::static_ناپ ||
        سرخی.نمبر_دا_Contours != -1)
    {
      dest_بائٹاں = hb_بائٹاں_t ();
      أرجع سچ;
    }

    بنانشانی سروت_len = سروت_بائٹاں.لمائی - Glyph_سرخی::static_ناپ;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    حرف *o = (حرف *) hb_calloc (سروت_len * 2, حجم (حرف));
    إذا (unlikely (!o)) أرجع خطأ;

    ثابت مرکب_Glyph_ریکارڈ *c = reinterpret_cast<ثابت مرکب_Glyph_ریکارڈ *> (سروت_بائٹاں.arrayZ + Glyph_سرخی::static_ناپ);
    خودکار it = مرکب_iter_t (hb_بائٹاں_t ((ثابت حرف *)c, سروت_len), c, extended);

    حرف *p = o;
    بنانشانی i = 0, سروت_comp_len = 0;
    لكل (ثابت خودکار &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      إذا (i >= points_with_deltas.لمائی - 4) {
        hb_free (o);
        أرجع خطأ;
      }

      بنانشانی comp_len = component.get_ناپ (extended);
      إذا (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      وإلا
      {
        بنانشانی new_len = component.compile_with_نقطہ (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      سروت_comp_len += comp_len;
    }

    //copy instructions if any
    إذا (سروت_len > سروت_comp_len)
    {
      بنانشانی instr_len = سروت_len - سروت_comp_len;
      hb_memcpy (p, (ثابت حرف *)c + سروت_comp_len, instr_len);
      p += instr_len;
    }

    بنانشانی len = p - o;
    dest_بائٹاں = hb_بائٹاں_t (o, len);
    أرجع سچ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جے_انت /* OT_GLYF_COMPOSITEGLYPH_HH */

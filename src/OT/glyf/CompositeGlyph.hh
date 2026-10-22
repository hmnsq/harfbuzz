#اگر_غیر_تعریف_شدہ OT_GLYF_COMPOSITEGLYPH_HH
#تعریف_کرو OT_GLYF_COMPOSITEGLYPH_HH


#شامل "../../hb-open-type.hh"
#شامل "composite-iter.hh"


نامخلا OT {
نامخلا glyf_impl {


ساخت مرکب_Glyph_ریکارڈ
{
  محفوظ:
  شمار مرکب_glyph_flag_t
  {
    ARG_1_AND_2_ARE_الفاظ	= 0x0001,
    دلائل_ARE_XY_اقدار		= 0x0002,
    باری_XY_مقصد_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    مزید_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_انحراف	= 0x0800,
    UNSCALED_COMPONENT_انحراف	= 0x1000,
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    GID_IS_24_ثنائی_ہندسہ		= 0x2000
#اگر_ختم
  };

  عوامی:
  size_t get_سائز (منطقی extended) ثابت
  {
    غیرعلامتی صحیح سائز = min_سائز;
    /* glyphIndex is 24bit instead of 16bit */
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24_ثنائی_ہندسہ)) سائز += HBGlyphID24::static_سائز - HBGlyphID16::static_سائز;
#اگر_ختم
    /* arg1 and 2 are int16 */
    اگر (flags & ARG_1_AND_2_ARE_الفاظ) سائز += 4;
    /* arg1 and 2 are int8 */
    ورنہ سائز += 2;

    /* One x 16 bit (scale) */
    اگر (flags & WE_HAVE_A_SCALE) سائز += 2;
    /* Two x 16 bit (xscale, yscale) */
    ورنہ اگر (flags & WE_HAVE_AN_X_AND_Y_SCALE) سائز += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    ورنہ اگر (flags & WE_HAVE_A_TWO_BY_TWO) سائز += 8;

    واپس سائز;
  }

  خالی drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  خالی سیٹ_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقی has_instructions ()  ثابت { واپس   flags & WE_HAVE_INSTRUCTIONS; }

  منطقی has_مزید ()          ثابت { واپس   flags & مزید_COMPONENTS; }
  منطقی is_use_my_metrics () ثابت { واپس   flags & USE_MY_METRICS; }
  منطقی is_anchored ()       ثابت { واپس !(flags & دلائل_ARE_XY_اقدار); }
  خالی get_anchor_points (غیرعلامتی صحیح &point1, غیرعلامتی صحیح &point2, منطقی extended) ثابت
  {
    ثابت خودکار *p = &StructAfter<ثابت HBUINT8> (flags);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      p += HBGlyphID24::static_سائز;
    ورنہ
#اگر_ختم
      p += HBGlyphID16::static_سائز;
    اگر (flags & ARG_1_AND_2_ARE_الفاظ)
    {
      point1 = ((ثابت HBUINT16 *) p)[0];
      point2 = ((ثابت HBUINT16 *) p)[1];
    }
    ورنہ
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ساکن خالی transform (ثابت اعشاری (&میٹرکس)[4],
			 hb_array_t<contour_نقطہ_t> points)
  {
    اگر (میٹرکس[0] != 1.f || میٹرکس[1] != 0.f ||
	میٹرکس[2] != 0.f || میٹرکس[3] != 1.f)
      برائے (خودکار &نقطہ : points)
        نقطہ.transform (میٹرکس);
  }

  ساکن خالی translate (ثابت contour_نقطہ_t &trans,
			 hb_array_t<contour_نقطہ_t> points)
  {
    اگر (HB_OPTIMIZE_سائز_VAL)
    {
      اگر (trans.x != 0.f || trans.y != 0.f)
        برائے (خودکار &نقطہ : points)
	  نقطہ.translate (trans);
    }
    ورنہ
    {
      اگر (trans.x != 0.f && trans.y != 0.f)
        برائے (خودکار &نقطہ : points)
	  نقطہ.translate (trans);
      ورنہ
      {
	اگر (trans.x != 0.f)
	  برائے (خودکار &نقطہ : points)
	    نقطہ.x += trans.x;
	ورنہ اگر (trans.y != 0.f)
	  برائے (خودکار &نقطہ : points)
	    نقطہ.y += trans.y;
      }
    }
  }

  خالی transform_points (hb_array_t<contour_نقطہ_t> points,
			 ثابت اعشاری (&میٹرکس)[4],
			 ثابت contour_نقطہ_t &trans) ثابت
  {
    اگر (scaled_offsets ())
    {
      translate (trans, points);
      transform (میٹرکس, points);
    }
    ورنہ
    {
      transform (میٹرکس, points);
      translate (trans, points);
    }
  }

  منطقی get_points (contour_نقطہ_سمتیہ_t &points, منطقی extended) ثابت
  {
    اعشاری میٹرکس[4];
    contour_نقطہ_t trans;
    get_transformation (میٹرکس, trans, extended);
    اگر (unlikely (!points.alloc (points.لمبائی + 1 + 4))) واپس جھوٹ; // For phantom points
    points.push (trans);
    واپس سچ;
  }

  غیرعلامتی compile_with_نقطہ (ثابت contour_نقطہ_t &نقطہ,
                               حرف *برآمد,
			       منطقی extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      p += HBGlyphID24::static_سائز;
    ورنہ
#اگر_ختم
      p += HBGlyphID16::static_سائز;

    غیرعلامتی len = get_سائز (extended);
    غیرعلامتی len_before_val = (ثابت حرف *)p - (ثابت حرف *)یہ;
    اگر (flags & ARG_1_AND_2_ARE_الفاظ)
    {
      // no overflow, copy value
      hb_memcpy (برآمد, یہ, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (برآمد + len_before_val);
      o[0] = roundf (نقطہ.x);
      o[1] = roundf (نقطہ.y);
    }
    ورنہ
    {
      صحیح new_x = roundf (نقطہ.x);
      صحیح new_y = roundf (نقطہ.y);
      اگر (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (برآمد, یہ, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (برآمد + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      ورنہ
      {
        // new point value has an int8 overflow
        hb_memcpy (برآمد, یہ, len_before_val);
        
        //update flags
        مرکب_Glyph_ریکارڈ *o = reinterpret_cast<مرکب_Glyph_ریکارڈ *> (برآمد);
        o->flags = flags | ARG_1_AND_2_ARE_الفاظ;
        برآمد += len_before_val;

        HBINT16 new_قدر;
        new_قدر = new_x;
        hb_memcpy (برآمد, &new_قدر, HBINT16::static_سائز);
        برآمد += HBINT16::static_سائز;

        new_قدر = new_y;
        hb_memcpy (برآمد, &new_قدر, HBINT16::static_سائز);
        برآمد += HBINT16::static_سائز;

        hb_memcpy (برآمد, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    واپس len;
  }

  محفوظ:
  منطقی scaled_offsets () ثابت
  { واپس (flags & (SCALED_COMPONENT_انحراف | UNSCALED_COMPONENT_انحراف)) == SCALED_COMPONENT_انحراف; }

  عوامی:
  منطقی get_transformation (اعشاری (&میٹرکس)[4], contour_نقطہ_t &trans, منطقی extended) ثابت
  {
    میٹرکس[0] = میٹرکس[3] = 1.f;
    میٹرکس[1] = میٹرکس[2] = 0.f;

    ثابت خودکار *p = &StructAfter<ثابت HBINT8> (flags);
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      p += HBGlyphID24::static_سائز;
    ورنہ
#اگر_ختم
      p += HBGlyphID16::static_سائز;
    صحیح tx, ty;
    اگر (flags & ARG_1_AND_2_ARE_الفاظ)
    {
      tx = *(ثابت HBINT16 *) p;
      p += HBINT16::static_سائز;
      ty = *(ثابت HBINT16 *) p;
      p += HBINT16::static_سائز;
    }
    ورنہ
    {
      tx = *p++;
      ty = *p++;
    }
    اگر (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((اعشاری) tx, (اعشاری) ty, سچ);

    {
      ثابت F2DOT14 *points = (ثابت F2DOT14 *) p;
      اگر (flags & WE_HAVE_A_SCALE)
      {
	میٹرکس[0] = میٹرکس[3] = points[0].مقصد_float ();
	واپس سچ;
      }
      ورنہ اگر (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	میٹرکس[0] = points[0].مقصد_float ();
	میٹرکس[3] = points[1].مقصد_float ();
	واپس سچ;
      }
      ورنہ اگر (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	میٹرکس[0] = points[0].مقصد_float ();
	میٹرکس[1] = points[1].مقصد_float ();
	میٹرکس[2] = points[2].مقصد_float ();
	میٹرکس[3] = points[3].مقصد_float ();
	واپس سچ;
      }
    }
    واپس tx || ty;
  }

  hb_codepoint_t get_gid (منطقی extended) ثابت
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      واپس StructAfter<ثابت HBGlyphID24> (flags);
    ورنہ
#اگر_ختم
      واپس StructAfter<ثابت HBGlyphID16> (flags);
  }
  خالی سیٹ_gid (hb_codepoint_t gid, منطقی extended)
  {
#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
    اگر (extended && (flags & GID_IS_24_ثنائی_ہندسہ))
      StructAfter<HBGlyphID24> (flags) = gid;
    ورنہ
#اگر_ختم
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#اگر_غیر_تعریف_شدہ HB_NO_BEYOND_64K
  خالی زیریں_gid_24_مقصد_16 ()
  {
    hb_codepoint_t gid = get_gid (سچ);
    اگر (!(flags & GID_IS_24_ثنائی_ہندسہ) || gid > 0xFFFFu)
      واپس;

    /* Lower the flag and move the rest of the struct down. */

    غیرعلامتی سائز = get_سائز (سچ);
    حرف *انتہا = (حرف *) یہ + سائز;
    حرف *p = &StructAfter<حرف> (flags);
    p += HBGlyphID24::static_سائز;

    flags = flags & ~GID_IS_24_ثنائی_ہندسہ;
    سیٹ_gid (gid, سچ);

    memmove (p - HBGlyphID24::static_سائز + HBGlyphID16::static_سائز, p, انتہا - p);
  }
#اگر_ختم

  محفوظ:
  HBUINT16	flags;
  HBUINT24	pad;
  عوامی:
  DEFINE_سائز_MIN (4);
};

استعمال مرکب_iter_t = مرکب_iter_tmpl<مرکب_Glyph_ریکارڈ>;

ساخت مرکب_Glyph
{
  ثابت Glyph_سرخی &سرخی;
  hb_بائٹیں_t بائٹیں;
  منطقی extended;
  مرکب_Glyph (ثابت Glyph_سرخی &سرخی_2, hb_بائٹیں_t بائٹیں_2, منطقی extended_) :
    سرخی (سرخی_2), بائٹیں (بائٹیں_2), extended (extended_) {}

  مرکب_iter_t iter () ثابت
  { واپس مرکب_iter_t (بائٹیں, &StructAfter<مرکب_Glyph_ریکارڈ, Glyph_سرخی> (سرخی), extended); }

  غیرعلامتی صحیح instructions_لمبائی (hb_بائٹیں_t بائٹیں) ثابت
  {
    غیرعلامتی صحیح آغاز = بائٹیں.لمبائی;
    غیرعلامتی صحیح انتہا = بائٹیں.لمبائی;
    ثابت مرکب_Glyph_ریکارڈ *آخری = خالی_اشاریہ;
    برائے (خودکار &شے : iter ())
      آخری = &شے;
    اگر (unlikely (!آخری)) واپس 0;

    اگر (آخری->has_instructions ())
      آغاز = (حرف *) آخری - &بائٹیں + آخری->get_سائز (extended);
    اگر (unlikely (آغاز > انتہا)) واپس 0;
    واپس انتہا - آغاز;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_بائٹیں_t trim_padding () ثابت { واپس بائٹیں; }

  خالی drop_hints ()
  {
    برائے (ثابت خودکار &_ : iter ())
      const_cast<مرکب_Glyph_ریکارڈ &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  خالی drop_hints_بائٹیں (hb_بائٹیں_t &dest_آغاز) ثابت
  { dest_آغاز = بائٹیں.sub_array (0, بائٹیں.لمبائی - instructions_لمبائی (بائٹیں)); }

  خالی سیٹ_overlaps_flag ()
  {
    مرکب_Glyph_ریکارڈ& glyph_chain = const_cast<مرکب_Glyph_ریکارڈ &> (
	StructAfter<مرکب_Glyph_ریکارڈ, Glyph_سرخی> (سرخی));
    اگر (!بائٹیں.check_range(&glyph_chain, مرکب_Glyph_ریکارڈ::min_سائز))
      واپس;
    glyph_chain.سیٹ_overlaps_flag ();
  }

  منطقی compile_بائٹیں_with_deltas (ثابت hb_بائٹیں_t &ماخذ_بائٹیں,
                                  ثابت contour_نقطہ_سمتیہ_t &points_with_deltas,
                                  hb_بائٹیں_t &dest_بائٹیں /*برآمد*/)
  {
    اگر (ماخذ_بائٹیں.لمبائی <= Glyph_سرخی::static_سائز ||
        سرخی.نمبر_کا_Contours != -1)
    {
      dest_بائٹیں = hb_بائٹیں_t ();
      واپس سچ;
    }

    غیرعلامتی ماخذ_len = ماخذ_بائٹیں.لمبائی - Glyph_سرخی::static_سائز;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    حرف *o = (حرف *) hb_calloc (ماخذ_len * 2, حجم (حرف));
    اگر (unlikely (!o)) واپس جھوٹ;

    ثابت مرکب_Glyph_ریکارڈ *c = reinterpret_cast<ثابت مرکب_Glyph_ریکارڈ *> (ماخذ_بائٹیں.arrayZ + Glyph_سرخی::static_سائز);
    خودکار it = مرکب_iter_t (hb_بائٹیں_t ((ثابت حرف *)c, ماخذ_len), c, extended);

    حرف *p = o;
    غیرعلامتی i = 0, ماخذ_comp_len = 0;
    برائے (ثابت خودکار &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      اگر (i >= points_with_deltas.لمبائی - 4) {
        hb_free (o);
        واپس جھوٹ;
      }

      غیرعلامتی comp_len = component.get_سائز (extended);
      اگر (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      ورنہ
      {
        غیرعلامتی new_len = component.compile_with_نقطہ (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      ماخذ_comp_len += comp_len;
    }

    //copy instructions if any
    اگر (ماخذ_len > ماخذ_comp_len)
    {
      غیرعلامتی instr_len = ماخذ_len - ماخذ_comp_len;
      hb_memcpy (p, (ثابت حرف *)c + ماخذ_comp_len, instr_len);
      p += instr_len;
    }

    غیرعلامتی len = p - o;
    dest_بائٹیں = hb_بائٹیں_t (o, len);
    واپس سچ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#اگر_ختم /* OT_GLYF_COMPOSITEGLYPH_HH */

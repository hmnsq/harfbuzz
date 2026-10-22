#جيڪڏهن_وصف_نه_ٿيل OT_GLYF_COMPOSITEGLYPH_HH
#وصف_ڪريو OT_GLYF_COMPOSITEGLYPH_HH


#ضمن "../../hb-open-type.hh"
#ضمن "composite-iter.hh"


نطاق OT {
نطاق glyf_impl {


بنية مرکب_Glyph_رڪارڊ
{
  محمي:
  تعداد مرکب_glyph_flag_t
  {
    ARG_1_AND_2_ARE_لفظ	= 0x0001,
    دليلون_ARE_XY_قدرون		= 0x0002,
    ڦيرو_XY_منزل_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    وڌيڪ_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_وٿي	= 0x0800,
    UNSCALED_COMPONENT_وٿي	= 0x1000,
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    GID_IS_24_ٻٽو_انگ		= 0x2000
#جيڪڏهن_پورو
  };

  عام:
  size_t get_سائيز (منطقي extended) ثابت
  {
    بنانشان صحيح سائيز = min_سائيز;
    /* glyphIndex is 24bit instead of 16bit */
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ٻٽو_انگ)) سائيز += HBGlyphID24::static_سائيز - HBGlyphID16::static_سائيز;
#جيڪڏهن_پورو
    /* arg1 and 2 are int16 */
    إذا (flags & ARG_1_AND_2_ARE_لفظ) سائيز += 4;
    /* arg1 and 2 are int8 */
    وإلا سائيز += 2;

    /* One x 16 bit (scale) */
    إذا (flags & WE_HAVE_A_SCALE) سائيز += 2;
    /* Two x 16 bit (xscale, yscale) */
    وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE) سائيز += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO) سائيز += 8;

    أرجع سائيز;
  }

  فراغ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  فراغ سيٽ_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  منطقي has_instructions ()  ثابت { أرجع   flags & WE_HAVE_INSTRUCTIONS; }

  منطقي has_وڌيڪ ()          ثابت { أرجع   flags & وڌيڪ_COMPONENTS; }
  منطقي is_use_my_metrics () ثابت { أرجع   flags & USE_MY_METRICS; }
  منطقي is_anchored ()       ثابت { أرجع !(flags & دليلون_ARE_XY_قدرون); }
  فراغ get_anchor_points (بنانشان صحيح &point1, بنانشان صحيح &point2, منطقي extended) ثابت
  {
    ثابت خودڪار *p = &StructAfter<ثابت HBUINT8> (flags);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ٻٽو_انگ))
      p += HBGlyphID24::static_سائيز;
    وإلا
#جيڪڏهن_پورو
      p += HBGlyphID16::static_سائيز;
    إذا (flags & ARG_1_AND_2_ARE_لفظ)
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

  ساكن فراغ transform (ثابت عائم (&ميٽرڪس)[4],
			 hb_array_t<contour_نقطو_t> points)
  {
    إذا (ميٽرڪس[0] != 1.f || ميٽرڪس[1] != 0.f ||
	ميٽرڪس[2] != 0.f || ميٽرڪس[3] != 1.f)
      لكل (خودڪار &نقطو : points)
        نقطو.transform (ميٽرڪس);
  }

  ساكن فراغ translate (ثابت contour_نقطو_t &trans,
			 hb_array_t<contour_نقطو_t> points)
  {
    إذا (HB_OPTIMIZE_سائيز_VAL)
    {
      إذا (trans.x != 0.f || trans.y != 0.f)
        لكل (خودڪار &نقطو : points)
	  نقطو.translate (trans);
    }
    وإلا
    {
      إذا (trans.x != 0.f && trans.y != 0.f)
        لكل (خودڪار &نقطو : points)
	  نقطو.translate (trans);
      وإلا
      {
	إذا (trans.x != 0.f)
	  لكل (خودڪار &نقطو : points)
	    نقطو.x += trans.x;
	وإلا إذا (trans.y != 0.f)
	  لكل (خودڪار &نقطو : points)
	    نقطو.y += trans.y;
      }
    }
  }

  فراغ transform_points (hb_array_t<contour_نقطو_t> points,
			 ثابت عائم (&ميٽرڪس)[4],
			 ثابت contour_نقطو_t &trans) ثابت
  {
    إذا (scaled_offsets ())
    {
      translate (trans, points);
      transform (ميٽرڪس, points);
    }
    وإلا
    {
      transform (ميٽرڪس, points);
      translate (trans, points);
    }
  }

  منطقي get_points (contour_نقطو_ويڪٽر_t &points, منطقي extended) ثابت
  {
    عائم ميٽرڪس[4];
    contour_نقطو_t trans;
    get_transformation (ميٽرڪس, trans, extended);
    إذا (unlikely (!points.alloc (points.ڊيگهائي + 1 + 4))) أرجع خطأ; // For phantom points
    points.push (trans);
    أرجع سچ;
  }

  بنانشان compile_with_نقطو (ثابت contour_نقطو_t &نقطو,
                               حرف *ٻاهريون,
			       منطقي extended) ثابت
  {
    ثابت HBINT8 *p = &StructAfter<ثابت HBINT8> (flags);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ٻٽو_انگ))
      p += HBGlyphID24::static_سائيز;
    وإلا
#جيڪڏهن_پورو
      p += HBGlyphID16::static_سائيز;

    بنانشان len = get_سائيز (extended);
    بنانشان len_before_val = (ثابت حرف *)p - (ثابت حرف *)هذا;
    إذا (flags & ARG_1_AND_2_ARE_لفظ)
    {
      // no overflow, copy value
      hb_memcpy (ٻاهريون, هذا, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (ٻاهريون + len_before_val);
      o[0] = roundf (نقطو.x);
      o[1] = roundf (نقطو.y);
    }
    وإلا
    {
      صحيح new_x = roundf (نقطو.x);
      صحيح new_y = roundf (نقطو.y);
      إذا (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (ٻاهريون, هذا, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (ٻاهريون + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      وإلا
      {
        // new point value has an int8 overflow
        hb_memcpy (ٻاهريون, هذا, len_before_val);
        
        //update flags
        مرکب_Glyph_رڪارڊ *o = reinterpret_cast<مرکب_Glyph_رڪارڊ *> (ٻاهريون);
        o->flags = flags | ARG_1_AND_2_ARE_لفظ;
        ٻاهريون += len_before_val;

        HBINT16 new_قدر;
        new_قدر = new_x;
        hb_memcpy (ٻاهريون, &new_قدر, HBINT16::static_سائيز);
        ٻاهريون += HBINT16::static_سائيز;

        new_قدر = new_y;
        hb_memcpy (ٻاهريون, &new_قدر, HBINT16::static_سائيز);
        ٻاهريون += HBINT16::static_سائيز;

        hb_memcpy (ٻاهريون, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    أرجع len;
  }

  محمي:
  منطقي scaled_offsets () ثابت
  { أرجع (flags & (SCALED_COMPONENT_وٿي | UNSCALED_COMPONENT_وٿي)) == SCALED_COMPONENT_وٿي; }

  عام:
  منطقي get_transformation (عائم (&ميٽرڪس)[4], contour_نقطو_t &trans, منطقي extended) ثابت
  {
    ميٽرڪس[0] = ميٽرڪس[3] = 1.f;
    ميٽرڪس[1] = ميٽرڪس[2] = 0.f;

    ثابت خودڪار *p = &StructAfter<ثابت HBINT8> (flags);
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ٻٽو_انگ))
      p += HBGlyphID24::static_سائيز;
    وإلا
#جيڪڏهن_پورو
      p += HBGlyphID16::static_سائيز;
    صحيح tx, ty;
    إذا (flags & ARG_1_AND_2_ARE_لفظ)
    {
      tx = *(ثابت HBINT16 *) p;
      p += HBINT16::static_سائيز;
      ty = *(ثابت HBINT16 *) p;
      p += HBINT16::static_سائيز;
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
	ميٽرڪس[0] = ميٽرڪس[3] = points[0].منزل_float ();
	أرجع سچ;
      }
      وإلا إذا (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	ميٽرڪس[0] = points[0].منزل_float ();
	ميٽرڪس[3] = points[1].منزل_float ();
	أرجع سچ;
      }
      وإلا إذا (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	ميٽرڪس[0] = points[0].منزل_float ();
	ميٽرڪس[1] = points[1].منزل_float ();
	ميٽرڪس[2] = points[2].منزل_float ();
	ميٽرڪس[3] = points[3].منزل_float ();
	أرجع سچ;
      }
    }
    أرجع tx || ty;
  }

  hb_codepoint_t get_gid (منطقي extended) ثابت
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ٻٽو_انگ))
      أرجع StructAfter<ثابت HBGlyphID24> (flags);
    وإلا
#جيڪڏهن_پورو
      أرجع StructAfter<ثابت HBGlyphID16> (flags);
  }
  فراغ سيٽ_gid (hb_codepoint_t gid, منطقي extended)
  {
#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
    إذا (extended && (flags & GID_IS_24_ٻٽو_انگ))
      StructAfter<HBGlyphID24> (flags) = gid;
    وإلا
#جيڪڏهن_پورو
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#جيڪڏهن_وصف_نه_ٿيل HB_NO_BEYOND_64K
  فراغ هيٺاهون_gid_24_منزل_16 ()
  {
    hb_codepoint_t gid = get_gid (سچ);
    إذا (!(flags & GID_IS_24_ٻٽو_انگ) || gid > 0xFFFFu)
      أرجع;

    /* Lower the flag and move the rest of the struct down. */

    بنانشان سائيز = get_سائيز (سچ);
    حرف *پڇاڙي = (حرف *) هذا + سائيز;
    حرف *p = &StructAfter<حرف> (flags);
    p += HBGlyphID24::static_سائيز;

    flags = flags & ~GID_IS_24_ٻٽو_انگ;
    سيٽ_gid (gid, سچ);

    memmove (p - HBGlyphID24::static_سائيز + HBGlyphID16::static_سائيز, p, پڇاڙي - p);
  }
#جيڪڏهن_پورو

  محمي:
  HBUINT16	flags;
  HBUINT24	pad;
  عام:
  DEFINE_سائيز_MIN (4);
};

استعمال مرکب_iter_t = مرکب_iter_tmpl<مرکب_Glyph_رڪارڊ>;

بنية مرکب_Glyph
{
  ثابت Glyph_سرو &سرو;
  hb_بائيٽون_t بائيٽون;
  منطقي extended;
  مرکب_Glyph (ثابت Glyph_سرو &سرو_2, hb_بائيٽون_t بائيٽون_2, منطقي extended_) :
    سرو (سرو_2), بائيٽون (بائيٽون_2), extended (extended_) {}

  مرکب_iter_t iter () ثابت
  { أرجع مرکب_iter_t (بائيٽون, &StructAfter<مرکب_Glyph_رڪارڊ, Glyph_سرو> (سرو), extended); }

  بنانشان صحيح instructions_ڊيگهائي (hb_بائيٽون_t بائيٽون) ثابت
  {
    بنانشان صحيح شروعات = بائيٽون.ڊيگهائي;
    بنانشان صحيح پڇاڙي = بائيٽون.ڊيگهائي;
    ثابت مرکب_Glyph_رڪارڊ *آخري = خالي_اشارو;
    لكل (خودڪار &شي : iter ())
      آخري = &شي;
    إذا (unlikely (!آخري)) أرجع 0;

    إذا (آخري->has_instructions ())
      شروعات = (حرف *) آخري - &بائيٽون + آخري->get_سائيز (extended);
    إذا (unlikely (شروعات > پڇاڙي)) أرجع 0;
    أرجع پڇاڙي - شروعات;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ثابت hb_بائيٽون_t trim_padding () ثابت { أرجع بائيٽون; }

  فراغ drop_hints ()
  {
    لكل (ثابت خودڪار &_ : iter ())
      const_cast<مرکب_Glyph_رڪارڊ &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  فراغ drop_hints_بائيٽون (hb_بائيٽون_t &dest_شروعات) ثابت
  { dest_شروعات = بائيٽون.sub_array (0, بائيٽون.ڊيگهائي - instructions_ڊيگهائي (بائيٽون)); }

  فراغ سيٽ_overlaps_flag ()
  {
    مرکب_Glyph_رڪارڊ& glyph_chain = const_cast<مرکب_Glyph_رڪارڊ &> (
	StructAfter<مرکب_Glyph_رڪارڊ, Glyph_سرو> (سرو));
    إذا (!بائيٽون.check_range(&glyph_chain, مرکب_Glyph_رڪارڊ::min_سائيز))
      أرجع;
    glyph_chain.سيٽ_overlaps_flag ();
  }

  منطقي compile_بائيٽون_with_deltas (ثابت hb_بائيٽون_t &ذريعو_بائيٽون,
                                  ثابت contour_نقطو_ويڪٽر_t &points_with_deltas,
                                  hb_بائيٽون_t &dest_بائيٽون /*ٻاهريون*/)
  {
    إذا (ذريعو_بائيٽون.ڊيگهائي <= Glyph_سرو::static_سائيز ||
        سرو.عدد_جو_Contours != -1)
    {
      dest_بائيٽون = hb_بائيٽون_t ();
      أرجع سچ;
    }

    بنانشان ذريعو_len = ذريعو_بائيٽون.ڊيگهائي - Glyph_سرو::static_سائيز;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    حرف *o = (حرف *) hb_calloc (ذريعو_len * 2, حجم (حرف));
    إذا (unlikely (!o)) أرجع خطأ;

    ثابت مرکب_Glyph_رڪارڊ *c = reinterpret_cast<ثابت مرکب_Glyph_رڪارڊ *> (ذريعو_بائيٽون.arrayZ + Glyph_سرو::static_سائيز);
    خودڪار it = مرکب_iter_t (hb_بائيٽون_t ((ثابت حرف *)c, ذريعو_len), c, extended);

    حرف *p = o;
    بنانشان i = 0, ذريعو_comp_len = 0;
    لكل (ثابت خودڪار &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      إذا (i >= points_with_deltas.ڊيگهائي - 4) {
        hb_free (o);
        أرجع خطأ;
      }

      بنانشان comp_len = component.get_سائيز (extended);
      إذا (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      وإلا
      {
        بنانشان new_len = component.compile_with_نقطو (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      ذريعو_comp_len += comp_len;
    }

    //copy instructions if any
    إذا (ذريعو_len > ذريعو_comp_len)
    {
      بنانشان instr_len = ذريعو_len - ذريعو_comp_len;
      hb_memcpy (p, (ثابت حرف *)c + ذريعو_comp_len, instr_len);
      p += instr_len;
    }

    بنانشان len = p - o;
    dest_بائيٽون = hb_بائيٽون_t (o, len);
    أرجع سچ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#جيڪڏهن_پورو /* OT_GLYF_COMPOSITEGLYPH_HH */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_GLYF_COMPOSITEGLYPH_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_GLYF_COMPOSITEGLYPH_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "composite-iter.hh"


ਨਾਮਾਕਾਸ਼ OT {
ਨਾਮਾਕਾਸ਼ glyf_impl {


ਬਣਤਰ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ
{
  ਸੁਰੱਖਿਅਤ:
  ਗਿਣਤੀ ਸੰਯੁਕਤ_glyph_flag_t
  {
    ARG_1_AND_2_ARE_ਸ਼ਬਦ	= 0x0001,
    ਦਲੀਲਾਂ_ARE_XY_ਮੁੱਲਾਂ		= 0x0002,
    ਗੇੜਾ_XY_ਮੰਜ਼ਿਲ_GRID		= 0x0004,
    WE_HAVE_A_SCALE		= 0x0008,
    ਹੋਰ_COMPONENTS		= 0x0020,
    WE_HAVE_AN_X_AND_Y_SCALE	= 0x0040,
    WE_HAVE_A_TWO_BY_TWO	= 0x0080,
    WE_HAVE_INSTRUCTIONS	= 0x0100,
    USE_MY_METRICS		= 0x0200,
    OVERLAP_COMPOUND		= 0x0400,
    SCALED_COMPONENT_ਵਿੱਥ	= 0x0800,
    UNSCALED_COMPONENT_ਵਿੱਥ	= 0x1000,
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ		= 0x2000
#ਜੇ_ਅੰਤ
  };

  ਜਨਤਕ:
  size_t get_ਮਾਪ (ਤਾਰਕਿਕ extended) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਮਾਪ = min_ਮਾਪ;
    /* glyphIndex is 24bit instead of 16bit */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended && (flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ)) ਮਾਪ += HBGlyphID24::static_ਮਾਪ - HBGlyphID16::static_ਮਾਪ;
#ਜੇ_ਅੰਤ
    /* arg1 and 2 are int16 */
    ਜੇ (flags & ARG_1_AND_2_ARE_ਸ਼ਬਦ) ਮਾਪ += 4;
    /* arg1 and 2 are int8 */
    ਨਹੀਂਤਾਂ ਮਾਪ += 2;

    /* One x 16 bit (scale) */
    ਜੇ (flags & WE_HAVE_A_SCALE) ਮਾਪ += 2;
    /* Two x 16 bit (xscale, yscale) */
    ਨਹੀਂਤਾਂ ਜੇ (flags & WE_HAVE_AN_X_AND_Y_SCALE) ਮਾਪ += 4;
    /* Four x 16 bit (xscale, scale01, scale10, yscale) */
    ਨਹੀਂਤਾਂ ਜੇ (flags & WE_HAVE_A_TWO_BY_TWO) ਮਾਪ += 8;

    ਵਾਪਸ ਮਾਪ;
  }

  ਖਾਲੀ drop_instructions_flag ()  { flags = (uint16_t) flags & ~WE_HAVE_INSTRUCTIONS; }
  ਖਾਲੀ ਸੈੱਟ_overlaps_flag ()
  {
    flags = (uint16_t) flags | OVERLAP_COMPOUND;
  }

  ਤਾਰਕਿਕ has_instructions ()  ਅਚਲ { ਵਾਪਸ   flags & WE_HAVE_INSTRUCTIONS; }

  ਤਾਰਕਿਕ has_ਹੋਰ ()          ਅਚਲ { ਵਾਪਸ   flags & ਹੋਰ_COMPONENTS; }
  ਤਾਰਕਿਕ is_use_my_metrics () ਅਚਲ { ਵਾਪਸ   flags & USE_MY_METRICS; }
  ਤਾਰਕਿਕ is_anchored ()       ਅਚਲ { ਵਾਪਸ !(flags & ਦਲੀਲਾਂ_ARE_XY_ਮੁੱਲਾਂ); }
  ਖਾਲੀ get_anchor_points (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &point1, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ &point2, ਤਾਰਕਿਕ extended) ਅਚਲ
  {
    ਅਚਲ ਸਵੈਚਾਲਿਤ *p = &StructAfter<ਅਚਲ HBUINT8> (flags);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended && (flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ))
      p += HBGlyphID24::static_ਮਾਪ;
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      p += HBGlyphID16::static_ਮਾਪ;
    ਜੇ (flags & ARG_1_AND_2_ARE_ਸ਼ਬਦ)
    {
      point1 = ((ਅਚਲ HBUINT16 *) p)[0];
      point2 = ((ਅਚਲ HBUINT16 *) p)[1];
    }
    ਨਹੀਂਤਾਂ
    {
      point1 = p[0];
      point2 = p[1];
    }
  }

  ਸਥਿਰ ਖਾਲੀ transform (ਅਚਲ ਦਸ਼ਮਲਵ (&ਮੈਟ੍ਰਿਕਸ)[4],
			 hb_array_t<contour_ਬਿੰਦੂ_t> points)
  {
    ਜੇ (ਮੈਟ੍ਰਿਕਸ[0] != 1.f || ਮੈਟ੍ਰਿਕਸ[1] != 0.f ||
	ਮੈਟ੍ਰਿਕਸ[2] != 0.f || ਮੈਟ੍ਰਿਕਸ[3] != 1.f)
      ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : points)
        ਬਿੰਦੂ.transform (ਮੈਟ੍ਰਿਕਸ);
  }

  ਸਥਿਰ ਖਾਲੀ translate (ਅਚਲ contour_ਬਿੰਦੂ_t &trans,
			 hb_array_t<contour_ਬਿੰਦੂ_t> points)
  {
    ਜੇ (HB_OPTIMIZE_ਮਾਪ_VAL)
    {
      ਜੇ (trans.x != 0.f || trans.y != 0.f)
        ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : points)
	  ਬਿੰਦੂ.translate (trans);
    }
    ਨਹੀਂਤਾਂ
    {
      ਜੇ (trans.x != 0.f && trans.y != 0.f)
        ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : points)
	  ਬਿੰਦੂ.translate (trans);
      ਨਹੀਂਤਾਂ
      {
	ਜੇ (trans.x != 0.f)
	  ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : points)
	    ਬਿੰਦੂ.x += trans.x;
	ਨਹੀਂਤਾਂ ਜੇ (trans.y != 0.f)
	  ਲਈ (ਸਵੈਚਾਲਿਤ &ਬਿੰਦੂ : points)
	    ਬਿੰਦੂ.y += trans.y;
      }
    }
  }

  ਖਾਲੀ transform_points (hb_array_t<contour_ਬਿੰਦੂ_t> points,
			 ਅਚਲ ਦਸ਼ਮਲਵ (&ਮੈਟ੍ਰਿਕਸ)[4],
			 ਅਚਲ contour_ਬਿੰਦੂ_t &trans) ਅਚਲ
  {
    ਜੇ (scaled_offsets ())
    {
      translate (trans, points);
      transform (ਮੈਟ੍ਰਿਕਸ, points);
    }
    ਨਹੀਂਤਾਂ
    {
      transform (ਮੈਟ੍ਰਿਕਸ, points);
      translate (trans, points);
    }
  }

  ਤਾਰਕਿਕ get_points (contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &points, ਤਾਰਕਿਕ extended) ਅਚਲ
  {
    ਦਸ਼ਮਲਵ ਮੈਟ੍ਰਿਕਸ[4];
    contour_ਬਿੰਦੂ_t trans;
    get_transformation (ਮੈਟ੍ਰਿਕਸ, trans, extended);
    ਜੇ (unlikely (!points.alloc (points.ਲੰਮਾਈ + 1 + 4))) ਵਾਪਸ ਝੂਠ; // For phantom points
    points.push (trans);
    ਵਾਪਸ ਸੱਚ;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ compile_with_ਬਿੰਦੂ (ਅਚਲ contour_ਬਿੰਦੂ_t &ਬਿੰਦੂ,
                               ਅੱਖਰ *ਬਾਹਰਲਾ,
			       ਤਾਰਕਿਕ extended) ਅਚਲ
  {
    ਅਚਲ HBINT8 *p = &StructAfter<ਅਚਲ HBINT8> (flags);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended && (flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ))
      p += HBGlyphID24::static_ਮਾਪ;
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      p += HBGlyphID16::static_ਮਾਪ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = get_ਮਾਪ (extended);
    ਬਿਨਾਨਿਸ਼ਾਨੀ len_before_val = (ਅਚਲ ਅੱਖਰ *)p - (ਅਚਲ ਅੱਖਰ *)ਇਹ;
    ਜੇ (flags & ARG_1_AND_2_ARE_ਸ਼ਬਦ)
    {
      // no overflow, copy value
      hb_memcpy (ਬਾਹਰਲਾ, ਇਹ, len);

      HBINT16 *o = reinterpret_cast<HBINT16 *> (ਬਾਹਰਲਾ + len_before_val);
      o[0] = roundf (ਬਿੰਦੂ.x);
      o[1] = roundf (ਬਿੰਦੂ.y);
    }
    ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ new_x = roundf (ਬਿੰਦੂ.x);
      ਪੂਰਨ new_y = roundf (ਬਿੰਦੂ.y);
      ਜੇ (new_x <= 127 && new_x >= -128 &&
          new_y <= 127 && new_y >= -128)
      {
        hb_memcpy (ਬਾਹਰਲਾ, ਇਹ, len);
        HBINT8 *o = reinterpret_cast<HBINT8 *> (ਬਾਹਰਲਾ + len_before_val);
        o[0] = new_x;
        o[1] = new_y;
      }
      ਨਹੀਂਤਾਂ
      {
        // new point value has an int8 overflow
        hb_memcpy (ਬਾਹਰਲਾ, ਇਹ, len_before_val);
        
        //update flags
        ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ *o = reinterpret_cast<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ *> (ਬਾਹਰਲਾ);
        o->flags = flags | ARG_1_AND_2_ARE_ਸ਼ਬਦ;
        ਬਾਹਰਲਾ += len_before_val;

        HBINT16 new_ਮੁੱਲ;
        new_ਮੁੱਲ = new_x;
        hb_memcpy (ਬਾਹਰਲਾ, &new_ਮੁੱਲ, HBINT16::static_ਮਾਪ);
        ਬਾਹਰਲਾ += HBINT16::static_ਮਾਪ;

        new_ਮੁੱਲ = new_y;
        hb_memcpy (ਬਾਹਰਲਾ, &new_ਮੁੱਲ, HBINT16::static_ਮਾਪ);
        ਬਾਹਰਲਾ += HBINT16::static_ਮਾਪ;

        hb_memcpy (ਬਾਹਰਲਾ, p+2, len - len_before_val - 2);
        len += 2;
      }
    }
    ਵਾਪਸ len;
  }

  ਸੁਰੱਖਿਅਤ:
  ਤਾਰਕਿਕ scaled_offsets () ਅਚਲ
  { ਵਾਪਸ (flags & (SCALED_COMPONENT_ਵਿੱਥ | UNSCALED_COMPONENT_ਵਿੱਥ)) == SCALED_COMPONENT_ਵਿੱਥ; }

  ਜਨਤਕ:
  ਤਾਰਕਿਕ get_transformation (ਦਸ਼ਮਲਵ (&ਮੈਟ੍ਰਿਕਸ)[4], contour_ਬਿੰਦੂ_t &trans, ਤਾਰਕਿਕ extended) ਅਚਲ
  {
    ਮੈਟ੍ਰਿਕਸ[0] = ਮੈਟ੍ਰਿਕਸ[3] = 1.f;
    ਮੈਟ੍ਰਿਕਸ[1] = ਮੈਟ੍ਰਿਕਸ[2] = 0.f;

    ਅਚਲ ਸਵੈਚਾਲਿਤ *p = &StructAfter<ਅਚਲ HBINT8> (flags);
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended && (flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ))
      p += HBGlyphID24::static_ਮਾਪ;
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      p += HBGlyphID16::static_ਮਾਪ;
    ਪੂਰਨ tx, ty;
    ਜੇ (flags & ARG_1_AND_2_ARE_ਸ਼ਬਦ)
    {
      tx = *(ਅਚਲ HBINT16 *) p;
      p += HBINT16::static_ਮਾਪ;
      ty = *(ਅਚਲ HBINT16 *) p;
      p += HBINT16::static_ਮਾਪ;
    }
    ਨਹੀਂਤਾਂ
    {
      tx = *p++;
      ty = *p++;
    }
    ਜੇ (is_anchored ()) tx = ty = 0;

    /* set is_end_point flag to true, used by IUP delta optimization */
    trans.init ((ਦਸ਼ਮਲਵ) tx, (ਦਸ਼ਮਲਵ) ty, ਸੱਚ);

    {
      ਅਚਲ F2DOT14 *points = (ਅਚਲ F2DOT14 *) p;
      ਜੇ (flags & WE_HAVE_A_SCALE)
      {
	ਮੈਟ੍ਰਿਕਸ[0] = ਮੈਟ੍ਰਿਕਸ[3] = points[0].ਮੰਜ਼ਿਲ_float ();
	ਵਾਪਸ ਸੱਚ;
      }
      ਨਹੀਂਤਾਂ ਜੇ (flags & WE_HAVE_AN_X_AND_Y_SCALE)
      {
	ਮੈਟ੍ਰਿਕਸ[0] = points[0].ਮੰਜ਼ਿਲ_float ();
	ਮੈਟ੍ਰਿਕਸ[3] = points[1].ਮੰਜ਼ਿਲ_float ();
	ਵਾਪਸ ਸੱਚ;
      }
      ਨਹੀਂਤਾਂ ਜੇ (flags & WE_HAVE_A_TWO_BY_TWO)
      {
	ਮੈਟ੍ਰਿਕਸ[0] = points[0].ਮੰਜ਼ਿਲ_float ();
	ਮੈਟ੍ਰਿਕਸ[1] = points[1].ਮੰਜ਼ਿਲ_float ();
	ਮੈਟ੍ਰਿਕਸ[2] = points[2].ਮੰਜ਼ਿਲ_float ();
	ਮੈਟ੍ਰਿਕਸ[3] = points[3].ਮੰਜ਼ਿਲ_float ();
	ਵਾਪਸ ਸੱਚ;
      }
    }
    ਵਾਪਸ tx || ty;
  }

  hb_codepoint_t get_gid (ਤਾਰਕਿਕ extended) ਅਚਲ
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended && (flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ))
      ਵਾਪਸ StructAfter<ਅਚਲ HBGlyphID24> (flags);
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      ਵਾਪਸ StructAfter<ਅਚਲ HBGlyphID16> (flags);
  }
  ਖਾਲੀ ਸੈੱਟ_gid (hb_codepoint_t gid, ਤਾਰਕਿਕ extended)
  {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
    ਜੇ (extended && (flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ))
      StructAfter<HBGlyphID24> (flags) = gid;
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
      /* TODO assert? */
      StructAfter<HBGlyphID16> (flags) = gid;
  }

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_BEYOND_64K
  ਖਾਲੀ ਹੇਠਲਾ_gid_24_ਮੰਜ਼ਿਲ_16 ()
  {
    hb_codepoint_t gid = get_gid (ਸੱਚ);
    ਜੇ (!(flags & GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ) || gid > 0xFFFFu)
      ਵਾਪਸ;

    /* Lower the flag and move the rest of the struct down. */

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਮਾਪ = get_ਮਾਪ (ਸੱਚ);
    ਅੱਖਰ *ਸਿਰਾ = (ਅੱਖਰ *) ਇਹ + ਮਾਪ;
    ਅੱਖਰ *p = &StructAfter<ਅੱਖਰ> (flags);
    p += HBGlyphID24::static_ਮਾਪ;

    flags = flags & ~GID_IS_24_ਦੋਆਧਾਰੀ_ਅੰਕ;
    ਸੈੱਟ_gid (gid, ਸੱਚ);

    memmove (p - HBGlyphID24::static_ਮਾਪ + HBGlyphID16::static_ਮਾਪ, p, ਸਿਰਾ - p);
  }
#ਜੇ_ਅੰਤ

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	flags;
  HBUINT24	pad;
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_MIN (4);
};

ਵਰਤੋਂ ਸੰਯੁਕਤ_iter_t = ਸੰਯੁਕਤ_iter_tmpl<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ>;

ਬਣਤਰ ਸੰਯੁਕਤ_Glyph
{
  ਅਚਲ Glyph_ਸਿਰਲੇਖ &ਸਿਰਲੇਖ;
  hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ;
  ਤਾਰਕਿਕ extended;
  ਸੰਯੁਕਤ_Glyph (ਅਚਲ Glyph_ਸਿਰਲੇਖ &ਸਿਰਲੇਖ_2, hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ_2, ਤਾਰਕਿਕ extended_) :
    ਸਿਰਲੇਖ (ਸਿਰਲੇਖ_2), ਬਾਈਟਾਂ (ਬਾਈਟਾਂ_2), extended (extended_) {}

  ਸੰਯੁਕਤ_iter_t iter () ਅਚਲ
  { ਵਾਪਸ ਸੰਯੁਕਤ_iter_t (ਬਾਈਟਾਂ, &StructAfter<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ, Glyph_ਸਿਰਲੇਖ> (ਸਿਰਲੇਖ), extended); }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ instructions_ਲੰਮਾਈ (hb_ਬਾਈਟਾਂ_t ਬਾਈਟਾਂ) ਅਚਲ
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ = ਬਾਈਟਾਂ.ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਿਰਾ = ਬਾਈਟਾਂ.ਲੰਮਾਈ;
    ਅਚਲ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ *ਆਖਰੀ = ਖਾਲੀ_ਸੂਚਕ;
    ਲਈ (ਸਵੈਚਾਲਿਤ &ਵਸਤੂ : iter ())
      ਆਖਰੀ = &ਵਸਤੂ;
    ਜੇ (unlikely (!ਆਖਰੀ)) ਵਾਪਸ 0;

    ਜੇ (ਆਖਰੀ->has_instructions ())
      ਸ਼ੁਰੂ = (ਅੱਖਰ *) ਆਖਰੀ - &ਬਾਈਟਾਂ + ਆਖਰੀ->get_ਮਾਪ (extended);
    ਜੇ (unlikely (ਸ਼ੁਰੂ > ਸਿਰਾ)) ਵਾਪਸ 0;
    ਵਾਪਸ ਸਿਰਾ - ਸ਼ੁਰੂ;
  }

  /* Trimming for composites not implemented.
   * If removing hints it falls out of that. */
  ਅਚਲ hb_ਬਾਈਟਾਂ_t trim_padding () ਅਚਲ { ਵਾਪਸ ਬਾਈਟਾਂ; }

  ਖਾਲੀ drop_hints ()
  {
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : iter ())
      const_cast<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ &> (_).drop_instructions_flag ();
  }

  /* Chop instructions off the end */
  ਖਾਲੀ drop_hints_ਬਾਈਟਾਂ (hb_ਬਾਈਟਾਂ_t &dest_ਸ਼ੁਰੂ) ਅਚਲ
  { dest_ਸ਼ੁਰੂ = ਬਾਈਟਾਂ.sub_array (0, ਬਾਈਟਾਂ.ਲੰਮਾਈ - instructions_ਲੰਮਾਈ (ਬਾਈਟਾਂ)); }

  ਖਾਲੀ ਸੈੱਟ_overlaps_flag ()
  {
    ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ& glyph_chain = const_cast<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ &> (
	StructAfter<ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ, Glyph_ਸਿਰਲੇਖ> (ਸਿਰਲੇਖ));
    ਜੇ (!ਬਾਈਟਾਂ.check_range(&glyph_chain, ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ::min_ਮਾਪ))
      ਵਾਪਸ;
    glyph_chain.ਸੈੱਟ_overlaps_flag ();
  }

  ਤਾਰਕਿਕ compile_ਬਾਈਟਾਂ_with_deltas (ਅਚਲ hb_ਬਾਈਟਾਂ_t &ਸਰੋਤ_ਬਾਈਟਾਂ,
                                  ਅਚਲ contour_ਬਿੰਦੂ_ਸਦਿਸ਼_t &points_with_deltas,
                                  hb_ਬਾਈਟਾਂ_t &dest_ਬਾਈਟਾਂ /*ਬਾਹਰਲਾ*/)
  {
    ਜੇ (ਸਰੋਤ_ਬਾਈਟਾਂ.ਲੰਮਾਈ <= Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ ||
        ਸਿਰਲੇਖ.ਸੰਖਿਆ_ਦਾ_Contours != -1)
    {
      dest_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t ();
      ਵਾਪਸ ਸੱਚ;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਰੋਤ_len = ਸਰੋਤ_ਬਾਈਟਾਂ.ਲੰਮਾਈ - Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ;

    /* try to allocate more memories than source glyph bytes
     * in case that there might be an overflow for int8 value
     * and we would need to use int16 instead */
    ਅੱਖਰ *o = (ਅੱਖਰ *) hb_calloc (ਸਰੋਤ_len * 2, ਆਕਾਰ (ਅੱਖਰ));
    ਜੇ (unlikely (!o)) ਵਾਪਸ ਝੂਠ;

    ਅਚਲ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ *c = reinterpret_cast<ਅਚਲ ਸੰਯੁਕਤ_Glyph_ਰਿਕਾਰਡ *> (ਸਰੋਤ_ਬਾਈਟਾਂ.arrayZ + Glyph_ਸਿਰਲੇਖ::static_ਮਾਪ);
    ਸਵੈਚਾਲਿਤ it = ਸੰਯੁਕਤ_iter_t (hb_ਬਾਈਟਾਂ_t ((ਅਚਲ ਅੱਖਰ *)c, ਸਰੋਤ_len), c, extended);

    ਅੱਖਰ *p = o;
    ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0, ਸਰੋਤ_comp_len = 0;
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &component : it)
    {
      /* last 4 points in points_with_deltas are phantom points and should not be included */
      ਜੇ (i >= points_with_deltas.ਲੰਮਾਈ - 4) {
        hb_free (o);
        ਵਾਪਸ ਝੂਠ;
      }

      ਬਿਨਾਨਿਸ਼ਾਨੀ comp_len = component.get_ਮਾਪ (extended);
      ਜੇ (component.is_anchored ())
      {
        hb_memcpy (p, &component, comp_len);
        p += comp_len;
      }
      ਨਹੀਂਤਾਂ
      {
        ਬਿਨਾਨਿਸ਼ਾਨੀ new_len = component.compile_with_ਬਿੰਦੂ (points_with_deltas[i], p, extended);
        p += new_len;
      }
      i++;
      ਸਰੋਤ_comp_len += comp_len;
    }

    //copy instructions if any
    ਜੇ (ਸਰੋਤ_len > ਸਰੋਤ_comp_len)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ instr_len = ਸਰੋਤ_len - ਸਰੋਤ_comp_len;
      hb_memcpy (p, (ਅਚਲ ਅੱਖਰ *)c + ਸਰੋਤ_comp_len, instr_len);
      p += instr_len;
    }

    ਬਿਨਾਨਿਸ਼ਾਨੀ len = p - o;
    dest_ਬਾਈਟਾਂ = hb_ਬਾਈਟਾਂ_t (o, len);
    ਵਾਪਸ ਸੱਚ;
  }
};


} /* namespace glyf_impl */
} /* namespace OT */


#ਜੇ_ਅੰਤ /* OT_GLYF_COMPOSITEGLYPH_HH */

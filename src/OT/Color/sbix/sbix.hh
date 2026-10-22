/*
 * Copyright © 2018  Ebrahim Byagowi
 * Copyright © 2020  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Calder Kitagawa
 */

#اگر_تعریف_نشده OT_COLOR_SBIX_SBIX_HH
#تعریف_کن OT_COLOR_SBIX_SBIX_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#تعریف_کن HB_OT_برچسب_sbix HB_برچسب('s','b','i','x')


نطاق OT {


بنية SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, بیعلامت صحيح داده_درازی) ثابت
  {
    TRACE_SERIALIZE (هذا);
    SBIXGlyph* new_glyph = c->شروع_embed<SBIXGlyph> ();
    إذا (unlikely (!c->extend_min (new_glyph))) return_trace (اشاره_تهی);

    new_glyph->x_جابه_جایی = x_جابه_جایی;
    new_glyph->y_جابه_جایی = y_جابه_جایی;
    new_glyph->graphicType = graphicType;
    داده.copy (c, داده_درازی);
    return_trace (new_glyph);
  }

  HBINT16	x_جابه_جایی;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_جابه_جایی;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  برچسب		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_از<HBUINT8>
		داده;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  عام:
  DEFINE_اندازه_ARRAY (8, داده);
};

بنية SBIXStrike
{
  ساكن size_t get_اندازه (بیعلامت رقم_glyphs)
  { أرجع min_اندازه + رقم_glyphs * HBUINT32::static_اندازه; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_رقم_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (بیعلامت صحيح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_برچسب_t      file_type,
			     صحيح          *x_جابه_جایی_2,
			     صحيح          *y_جابه_جایی_2,
			     بیعلامت صحيح  رقم_glyphs,
			     بیعلامت صحيح *strike_ppem) ثابت
  {
    إذا (unlikely (!ppem)) أرجع hb_blob_get_خالی (); /* To get Null() object out of the way. */

    بیعلامت صحيح retry_شمار = 8;
    بیعلامت صحيح sbix_len = sbix_blob->درازی;
    بیعلامت صحيح strike_جابه_جایی = (ثابت حرف *) هذا - (ثابت حرف *) sbix_blob->داده;
    أكد (strike_جابه_جایی < sbix_len);

  retry:
    إذا (unlikely (glyph_id >= رقم_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_اندازه ||
		  (بیعلامت صحيح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_جابه_جایی))
      أرجع hb_blob_get_خالی ();

    بیعلامت صحيح glyph_جابه_جایی = strike_جابه_جایی + (بیعلامت صحيح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_اندازه;
    بیعلامت صحيح glyph_درازی = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_اندازه;

    ثابت SBIXGlyph *glyph = &(هذا+imageOffsetsZ[glyph_id]);

    إذا (glyph->graphicType == HB_برچسب ('d','u','p','e'))
    {
      إذا (glyph_درازی >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->داده);
	إذا (retry_شمار--)
	  اذهب retry;
      }
      أرجع hb_blob_get_خالی ();
    }

    إذا (unlikely (file_type != glyph->graphicType))
      أرجع hb_blob_get_خالی ();

    إذا (strike_ppem) *strike_ppem = ppem;
    إذا (x_جابه_جایی_2) *x_جابه_جایی_2 = glyph->x_جابه_جایی;
    إذا (y_جابه_جایی_2) *y_جابه_جایی_2 = glyph->y_جابه_جایی;
    أرجع hb_blob_create_sub_blob (sbix_blob, glyph_جابه_جایی, glyph_درازی);
  }

  منطقي subset (hb_subset_context_t *c, بیعلامت صحيح موجود_len) ثابت
  {
    TRACE_SUBSET (هذا);
    بیعلامت صحيح رقم_output_glyphs = c->plan->رقم_output_glyphs ();

    خودکار* بیرون = c->serializer->شروع_embed<SBIXStrike> ();
    خودکار snap = c->serializer->snapshot ();
    إذا (unlikely (!c->serializer->extend (بیرون, رقم_output_glyphs + 1))) return_trace (خطأ);
    بیرون->ppem = ppem;
    بیرون->resolution = resolution;
    HBUINT32 سر_2;
    سر_2 = get_اندازه (رقم_output_glyphs + 1);

    منطقي has_glyphs = خطأ;
    لكل (بیعلامت new_gid = 0; new_gid < رقم_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_اندازه) ||
		    (بیعلامت صحيح) imageOffsetsZ[old_gid + 1] > موجود_len)
      {
	بیرون->imageOffsetsZ[new_gid] = سر_2;
	استمر;
      }
      has_glyphs = درست;
      بیعلامت صحيح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      بیعلامت صحيح glyph_داده_درازی = delta - SBIXGlyph::min_اندازه;
      إذا (!(هذا+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_داده_درازی))
	return_trace (خطأ);
      بیرون->imageOffsetsZ[new_gid] = سر_2;
      سر_2 += delta;
    }
    إذا (has_glyphs)
      بیرون->imageOffsetsZ[رقم_output_glyphs] = سر_2;
    وإلا
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عام:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محمي:
  Unsized_Array_از<Offset32_مقصد<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عام:
  DEFINE_اندازه_ARRAY (4, imageOffsetsZ);
};

بنية sbix
{
  ساكن constexpr hb_برچسب_t جدول_برچسب = HB_OT_برچسب_sbix;

  منطقي has_داده () ثابت { أرجع نسخه; }

  ثابت SBIXStrike &get_strike (بیعلامت صحيح i) ثابت { أرجع هذا+strikes[i]; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<sbix> (face);
      رقم_glyphs = face->get_رقم_glyphs ();
    }
    ~accelerator_t () { جدول.destroy (); }

    منطقي has_داده () ثابت { أرجع جدول->has_داده (); }

    منطقي get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      منطقي                scale = درست) ثابت
    {
      /* We only support PNG right now, and following function checks type. */
      أرجع get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      صحيح            *x_جابه_جایی_2,
			      صحيح            *y_جابه_جایی_2,
			      بیعلامت صحيح   *موجود_ppem) ثابت
    {
      أرجع انتخاب_strike (font).get_glyph_blob (glyph_id, جدول.get_blob (),
						  HB_برچسب ('p','n','g',' '),
						  x_جابه_جایی_2, y_جابه_جایی_2,
						  رقم_glyphs, موجود_ppem);
    }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *داده) ثابت
    {
      إذا (!has_داده ())
        أرجع خطأ;

      صحيح x_جابه_جایی_2 = 0, y_جابه_جایی_2 = 0;
      بیعلامت صحيح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      إذا (!font->get_glyph_extents (glyph, &extents, خطأ))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph, &x_جابه_جایی_2, &y_جابه_جایی_2, &strike_ppem);
      إذا (hb_blob_is_immutable (blob))
        أرجع خطأ;

      منطقي ret = funcs->image (داده,
			       blob,
			       pixel_extents.عرض, -pixel_extents.ارتفاع,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:

    ثابت SBIXStrike &انتخاب_strike (hb_font_t *font) ثابت
    {
      بیعلامت شمار = جدول->strikes.len;
      إذا (unlikely (!شمار))
	أرجع Null (SBIXStrike);

      بیعلامت صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      إذا (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      بیعلامت صحيح بهترین_i = 0;
      بیعلامت صحيح بهترین_ppem = جدول->get_strike (0).ppem;

      لكل (بیعلامت صحيح i = 1; i < شمار; i++)
      {
	بیعلامت صحيح ppem = (جدول->get_strike (i)).ppem;
	إذا ((requested_ppem <= ppem && ppem < بهترین_ppem) ||
	    (requested_ppem > بهترین_ppem && ppem > بهترین_ppem))
	{
	  بهترین_i = i;
	  بهترین_ppem = ppem;
	}
      }

      أرجع جدول->get_strike (بهترین_i);
    }

    بنية PNGHeader
    {
      HBUINT8	signature[8];
      بنية
      {
	بنية
	{
	  HBUINT32	درازی;
	  برچسب		type;
	}		سرآیند;
	HBUINT32	عرض;
	HBUINT32	ارتفاع;
	HBUINT8		بیت_عمق;
	HBUINT8		colorType;
	HBUINT8		compression_روش;
	HBUINT8		filter_روش;
	HBUINT8		interlace_روش;
      } IHDR;

      عام:
      DEFINE_اندازه_STATIC (29);
    };

    منطقي get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقي                scale = درست) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      إذا (!has_داده ())
	أرجع خطأ;

      صحيح x_جابه_جایی_2 = 0, y_جابه_جایی_2 = 0;
      بیعلامت صحيح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_جابه_جایی_2, &y_جابه_جایی_2, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      إذا (png.IHDR.ارتفاع >= 65536 || png.IHDR.عرض >= 65536)
      {
	hb_blob_destroy (blob);
	أرجع خطأ;
      }

      extents->x_bearing = x_جابه_جایی_2;
      extents->y_bearing = hb_clamp_مقصد<hb_position_t> ((int64_t) (بیعلامت صحيح) png.IHDR.ارتفاع + y_جابه_جایی_2);
      extents->عرض     = (hb_position_t) (بیعلامت صحيح) png.IHDR.عرض;
      extents->ارتفاع    = -(hb_position_t) (بیعلامت صحيح) png.IHDR.ارتفاع;

      /* Convert to font units. */
      إذا (strike_ppem && scale)
      {
	عائم scale = font->face->get_upem () / (عائم) strike_ppem;
	extents->x_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->عرض = hb_clamp_مقصد<hb_position_t> (roundf (extents->عرض * scale));
	extents->ارتفاع = hb_clamp_مقصد<hb_position_t> (roundf (extents->ارتفاع * scale));
      }

      إذا (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      أرجع strike_ppem;
    }

    خاص:
    hb_blob_ptr_t<sbix> جدول;

    بیعلامت صحيح رقم_glyphs;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخه >= 1 &&
			  strikes.sanitize (c, هذا)));
  }

  منطقي
  جمع_کن_strike (hb_subset_context_t *c, بیعلامت i) ثابت
  {
    إذا (strikes[i].is_null () || c->منبع_blob->درازی < (بیعلامت) strikes[i])
      أرجع خطأ;

    أرجع (هذا+strikes[i]).subset (c, c->منبع_blob->درازی - (بیعلامت) strikes[i]);
  }

  منطقي serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودکار *بیرون = c->serializer->شروع_embed<Array32_از_Offset32_مقصد<SBIXStrike>> ();
    إذا (unlikely (!c->serializer->extend_min (بیرون))) return_trace (خطأ);

    hb_وکتور_t<Offset32_مقصد<SBIXStrike>*> new_strikes;
    hb_وکتور_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (صحيح i = strikes.len - 1; i >= 0; --i)
    {
      خودکار* o = بیرون->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      *o = 0;
      خودکار snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقي ret = جمع_کن_strike (c, i);
      إذا (!ret)
      {
	c->serializer->برداشت_discard ();
	بیرون->برداشت ();
	c->serializer->revert (snap);
      }
      وإلا
      {
	objidxs.push (c->serializer->برداشت_pack ());
	new_strikes.push (o);
      }
    }
    لكل (بیعلامت صحيح i = 0; i < new_strikes.درازی; ++i)
      c->serializer->جمع_کن_link (*new_strikes[i], objidxs[new_strikes.درازی - 1 - i]);

    return_trace (درست);
  }

  منطقي subset (hb_subset_context_t* c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->نسخه))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->flags))) return_trace (خطأ);

    return_trace (serialize_strike_offsets (c));
  }

  محمي:
  HBUINT16	نسخه;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32_از_Offset32_مقصد<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عام:
  DEFINE_اندازه_ARRAY (8, strikes);
};

بنية sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#پایان_اگر /* OT_COLOR_SBIX_SBIX_HH */

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

#که_نه_تعریف_شوی OT_COLOR_SBIX_SBIX_HH
#تعریف_کړه OT_COLOR_SBIX_SBIX_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#تعریف_کړه HB_OT_نښان_sbix HB_نښان('s','b','i','x')


نطاق OT {


بنية SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, بینښه صحيح معلومات_اوږدتیا) ثابت
  {
    TRACE_SERIALIZE (هذا);
    SBIXGlyph* new_glyph = c->پیل_embed<SBIXGlyph> ();
    إذا (unlikely (!c->extend_min (new_glyph))) return_trace (تش_اشاره);

    new_glyph->x_دننه_واټن = x_دننه_واټن;
    new_glyph->y_دننه_واټن = y_دننه_واټن;
    new_glyph->graphicType = graphicType;
    معلومات.copy (c, معلومات_اوږدتیا);
    return_trace (new_glyph);
  }

  HBINT16	x_دننه_واټن;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_دننه_واټن;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  نښان		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_د<HBUINT8>
		معلومات;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  عام:
  DEFINE_اندازه_ARRAY (8, معلومات);
};

بنية SBIXStrike
{
  ساكن size_t get_اندازه (بینښه عدد_glyphs)
  { أرجع min_اندازه + عدد_glyphs * HBUINT32::static_اندازه; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_عدد_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (بینښه صحيح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_نښان_t      file_type,
			     صحيح          *x_دننه_واټن_2,
			     صحيح          *y_دننه_واټن_2,
			     بینښه صحيح  عدد_glyphs,
			     بینښه صحيح *strike_ppem) ثابت
  {
    إذا (unlikely (!ppem)) أرجع hb_blob_get_تشه (); /* To get Null() object out of the way. */

    بینښه صحيح retry_شمېر = 8;
    بینښه صحيح sbix_len = sbix_blob->اوږدتیا;
    بینښه صحيح strike_دننه_واټن = (ثابت حرف *) هذا - (ثابت حرف *) sbix_blob->معلومات;
    أكد (strike_دننه_واټن < sbix_len);

  retry:
    إذا (unlikely (glyph_id >= عدد_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_اندازه ||
		  (بینښه صحيح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_دننه_واټن))
      أرجع hb_blob_get_تشه ();

    بینښه صحيح glyph_دننه_واټن = strike_دننه_واټن + (بینښه صحيح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_اندازه;
    بینښه صحيح glyph_اوږدتیا = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_اندازه;

    ثابت SBIXGlyph *glyph = &(هذا+imageOffsetsZ[glyph_id]);

    إذا (glyph->graphicType == HB_نښان ('d','u','p','e'))
    {
      إذا (glyph_اوږدتیا >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->معلومات);
	إذا (retry_شمېر--)
	  اذهب retry;
      }
      أرجع hb_blob_get_تشه ();
    }

    إذا (unlikely (file_type != glyph->graphicType))
      أرجع hb_blob_get_تشه ();

    إذا (strike_ppem) *strike_ppem = ppem;
    إذا (x_دننه_واټن_2) *x_دننه_واټن_2 = glyph->x_دننه_واټن;
    إذا (y_دننه_واټن_2) *y_دننه_واټن_2 = glyph->y_دننه_واټن;
    أرجع hb_blob_create_sub_blob (sbix_blob, glyph_دننه_واټن, glyph_اوږدتیا);
  }

  منطقي subset (hb_subset_context_t *c, بینښه صحيح شته_len) ثابت
  {
    TRACE_SUBSET (هذا);
    بینښه صحيح عدد_output_glyphs = c->plan->عدد_output_glyphs ();

    خپلکاری* بهر = c->serializer->پیل_embed<SBIXStrike> ();
    خپلکاری snap = c->serializer->snapshot ();
    إذا (unlikely (!c->serializer->extend (بهر, عدد_output_glyphs + 1))) return_trace (خطأ);
    بهر->ppem = ppem;
    بهر->resolution = resolution;
    HBUINT32 سر_2;
    سر_2 = get_اندازه (عدد_output_glyphs + 1);

    منطقي has_glyphs = خطأ;
    لكل (بینښه new_gid = 0; new_gid < عدد_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_اندازه) ||
		    (بینښه صحيح) imageOffsetsZ[old_gid + 1] > شته_len)
      {
	بهر->imageOffsetsZ[new_gid] = سر_2;
	استمر;
      }
      has_glyphs = ریښتیا;
      بینښه صحيح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      بینښه صحيح glyph_معلومات_اوږدتیا = delta - SBIXGlyph::min_اندازه;
      إذا (!(هذا+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_معلومات_اوږدتیا))
	return_trace (خطأ);
      بهر->imageOffsetsZ[new_gid] = سر_2;
      سر_2 += delta;
    }
    إذا (has_glyphs)
      بهر->imageOffsetsZ[عدد_output_glyphs] = سر_2;
    وإلا
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عام:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محمي:
  Unsized_Array_د<Offset32_منزل<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عام:
  DEFINE_اندازه_ARRAY (4, imageOffsetsZ);
};

بنية sbix
{
  ساكن constexpr hb_نښان_t جدول_نښان = HB_OT_نښان_sbix;

  منطقي has_معلومات () ثابت { أرجع نسخه; }

  ثابت SBIXStrike &get_strike (بینښه صحيح i) ثابت { أرجع هذا+strikes[i]; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<sbix> (face);
      عدد_glyphs = face->get_عدد_glyphs ();
    }
    ~accelerator_t () { جدول.destroy (); }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

    منطقي get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      منطقي                scale = ریښتیا) ثابت
    {
      /* We only support PNG right now, and following function checks type. */
      أرجع get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      صحيح            *x_دننه_واټن_2,
			      صحيح            *y_دننه_واټن_2,
			      بینښه صحيح   *شته_ppem) ثابت
    {
      أرجع ټاکنه_strike (font).get_glyph_blob (glyph_id, جدول.get_blob (),
						  HB_نښان ('p','n','g',' '),
						  x_دننه_واټن_2, y_دننه_واټن_2,
						  عدد_glyphs, شته_ppem);
    }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *معلومات) ثابت
    {
      إذا (!has_معلومات ())
        أرجع خطأ;

      صحيح x_دننه_واټن_2 = 0, y_دننه_واټن_2 = 0;
      بینښه صحيح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      إذا (!font->get_glyph_extents (glyph, &extents, خطأ))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph, &x_دننه_واټن_2, &y_دننه_واټن_2, &strike_ppem);
      إذا (hb_blob_is_immutable (blob))
        أرجع خطأ;

      منطقي ret = funcs->image (معلومات,
			       blob,
			       pixel_extents.پلنوالی, -pixel_extents.لوړوالی,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:

    ثابت SBIXStrike &ټاکنه_strike (hb_font_t *font) ثابت
    {
      بینښه شمېر = جدول->strikes.len;
      إذا (unlikely (!شمېر))
	أرجع Null (SBIXStrike);

      بینښه صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      إذا (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      بینښه صحيح غوره_i = 0;
      بینښه صحيح غوره_ppem = جدول->get_strike (0).ppem;

      لكل (بینښه صحيح i = 1; i < شمېر; i++)
      {
	بینښه صحيح ppem = (جدول->get_strike (i)).ppem;
	إذا ((requested_ppem <= ppem && ppem < غوره_ppem) ||
	    (requested_ppem > غوره_ppem && ppem > غوره_ppem))
	{
	  غوره_i = i;
	  غوره_ppem = ppem;
	}
      }

      أرجع جدول->get_strike (غوره_i);
    }

    بنية PNGHeader
    {
      HBUINT8	signature[8];
      بنية
      {
	بنية
	{
	  HBUINT32	اوږدتیا;
	  نښان		type;
	}		سرلیک;
	HBUINT32	پلنوالی;
	HBUINT32	لوړوالی;
	HBUINT8		بټ_ژوروالی;
	HBUINT8		colorType;
	HBUINT8		compression_لاره;
	HBUINT8		filter_لاره;
	HBUINT8		interlace_لاره;
      } IHDR;

      عام:
      DEFINE_اندازه_STATIC (29);
    };

    منطقي get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقي                scale = ریښتیا) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      إذا (!has_معلومات ())
	أرجع خطأ;

      صحيح x_دننه_واټن_2 = 0, y_دننه_واټن_2 = 0;
      بینښه صحيح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_دننه_واټن_2, &y_دننه_واټن_2, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      إذا (png.IHDR.لوړوالی >= 65536 || png.IHDR.پلنوالی >= 65536)
      {
	hb_blob_destroy (blob);
	أرجع خطأ;
      }

      extents->x_bearing = x_دننه_واټن_2;
      extents->y_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) (بینښه صحيح) png.IHDR.لوړوالی + y_دننه_واټن_2);
      extents->پلنوالی     = (hb_position_t) (بینښه صحيح) png.IHDR.پلنوالی;
      extents->لوړوالی    = -(hb_position_t) (بینښه صحيح) png.IHDR.لوړوالی;

      /* Convert to font units. */
      إذا (strike_ppem && scale)
      {
	عائم scale = font->face->get_upem () / (عائم) strike_ppem;
	extents->x_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->پلنوالی = hb_clamp_منزل<hb_position_t> (roundf (extents->پلنوالی * scale));
	extents->لوړوالی = hb_clamp_منزل<hb_position_t> (roundf (extents->لوړوالی * scale));
      }

      إذا (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      أرجع strike_ppem;
    }

    خاص:
    hb_blob_ptr_t<sbix> جدول;

    بینښه صحيح عدد_glyphs;
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
  جمع_کړه_strike (hb_subset_context_t *c, بینښه i) ثابت
  {
    إذا (strikes[i].is_null () || c->سرچینه_blob->اوږدتیا < (بینښه) strikes[i])
      أرجع خطأ;

    أرجع (هذا+strikes[i]).subset (c, c->سرچینه_blob->اوږدتیا - (بینښه) strikes[i]);
  }

  منطقي serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خپلکاری *بهر = c->serializer->پیل_embed<Array32_د_Offset32_منزل<SBIXStrike>> ();
    إذا (unlikely (!c->serializer->extend_min (بهر))) return_trace (خطأ);

    hb_ویکټور_t<Offset32_منزل<SBIXStrike>*> new_strikes;
    hb_ویکټور_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (صحيح i = strikes.len - 1; i >= 0; --i)
    {
      خپلکاری* o = بهر->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      *o = 0;
      خپلکاری snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقي ret = جمع_کړه_strike (c, i);
      إذا (!ret)
      {
	c->serializer->راوباسه_discard ();
	بهر->راوباسه ();
	c->serializer->revert (snap);
      }
      وإلا
      {
	objidxs.push (c->serializer->راوباسه_pack ());
	new_strikes.push (o);
      }
    }
    لكل (بینښه صحيح i = 0; i < new_strikes.اوږدتیا; ++i)
      c->serializer->جمع_کړه_link (*new_strikes[i], objidxs[new_strikes.اوږدتیا - 1 - i]);

    return_trace (ریښتیا);
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
  Array32_د_Offset32_منزل<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عام:
  DEFINE_اندازه_ARRAY (8, strikes);
};

بنية sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#د_که_پای /* OT_COLOR_SBIX_SBIX_HH */

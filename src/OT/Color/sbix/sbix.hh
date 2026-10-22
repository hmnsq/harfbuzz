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

#جے_بنا_تعریف OT_COLOR_SBIX_SBIX_HH
#تعریف_کرو OT_COLOR_SBIX_SBIX_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#تعریف_کرو HB_OT_ٹیگ_sbix HB_ٹیگ('s','b','i','x')


نطاق OT {


بنية SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, بنانشانی صحيح انکڑے_لمائی) ثابت
  {
    TRACE_SERIALIZE (هذا);
    SBIXGlyph* new_glyph = c->شروع_embed<SBIXGlyph> ();
    إذا (unlikely (!c->extend_min (new_glyph))) return_trace (خالی_سوچک);

    new_glyph->x_وتھ = x_وتھ;
    new_glyph->y_وتھ = y_وتھ;
    new_glyph->graphicType = graphicType;
    انکڑے.copy (c, انکڑے_لمائی);
    return_trace (new_glyph);
  }

  HBINT16	x_وتھ;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_وتھ;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  ٹیگ		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_دا<HBUINT8>
		انکڑے;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  عام:
  DEFINE_ناپ_ARRAY (8, انکڑے);
};

بنية SBIXStrike
{
  ساكن size_t get_ناپ (بنانشانی عدد_glyphs)
  { أرجع min_ناپ + عدد_glyphs * HBUINT32::static_ناپ; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_عدد_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (بنانشانی صحيح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_ٹیگ_t      file_type,
			     صحيح          *x_وتھ_2,
			     صحيح          *y_وتھ_2,
			     بنانشانی صحيح  عدد_glyphs,
			     بنانشانی صحيح *strike_ppem) ثابت
  {
    إذا (unlikely (!ppem)) أرجع hb_blob_get_خالی_تھاواں (); /* To get Null() object out of the way. */

    بنانشانی صحيح retry_گنتی = 8;
    بنانشانی صحيح sbix_len = sbix_blob->لمائی;
    بنانشانی صحيح strike_وتھ = (ثابت حرف *) هذا - (ثابت حرف *) sbix_blob->انکڑے;
    أكد (strike_وتھ < sbix_len);

  retry:
    إذا (unlikely (glyph_id >= عدد_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_ناپ ||
		  (بنانشانی صحيح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_وتھ))
      أرجع hb_blob_get_خالی_تھاواں ();

    بنانشانی صحيح glyph_وتھ = strike_وتھ + (بنانشانی صحيح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_ناپ;
    بنانشانی صحيح glyph_لمائی = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_ناپ;

    ثابت SBIXGlyph *glyph = &(هذا+imageOffsetsZ[glyph_id]);

    إذا (glyph->graphicType == HB_ٹیگ ('d','u','p','e'))
    {
      إذا (glyph_لمائی >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->انکڑے);
	إذا (retry_گنتی--)
	  اذهب retry;
      }
      أرجع hb_blob_get_خالی_تھاواں ();
    }

    إذا (unlikely (file_type != glyph->graphicType))
      أرجع hb_blob_get_خالی_تھاواں ();

    إذا (strike_ppem) *strike_ppem = ppem;
    إذا (x_وتھ_2) *x_وتھ_2 = glyph->x_وتھ;
    إذا (y_وتھ_2) *y_وتھ_2 = glyph->y_وتھ;
    أرجع hb_blob_create_sub_blob (sbix_blob, glyph_وتھ, glyph_لمائی);
  }

  منطقي subset (hb_subset_context_t *c, بنانشانی صحيح دستیاب_len) ثابت
  {
    TRACE_SUBSET (هذا);
    بنانشانی صحيح عدد_output_glyphs = c->plan->عدد_output_glyphs ();

    خودکار* باہرلا = c->serializer->شروع_embed<SBIXStrike> ();
    خودکار snap = c->serializer->snapshot ();
    إذا (unlikely (!c->serializer->extend (باہرلا, عدد_output_glyphs + 1))) return_trace (خطأ);
    باہرلا->ppem = ppem;
    باہرلا->resolution = resolution;
    HBUINT32 سر_2;
    سر_2 = get_ناپ (عدد_output_glyphs + 1);

    منطقي has_glyphs = خطأ;
    لكل (بنانشانی new_gid = 0; new_gid < عدد_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_ناپ) ||
		    (بنانشانی صحيح) imageOffsetsZ[old_gid + 1] > دستیاب_len)
      {
	باہرلا->imageOffsetsZ[new_gid] = سر_2;
	استمر;
      }
      has_glyphs = سچ;
      بنانشانی صحيح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      بنانشانی صحيح glyph_انکڑے_لمائی = delta - SBIXGlyph::min_ناپ;
      إذا (!(هذا+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_انکڑے_لمائی))
	return_trace (خطأ);
      باہرلا->imageOffsetsZ[new_gid] = سر_2;
      سر_2 += delta;
    }
    إذا (has_glyphs)
      باہرلا->imageOffsetsZ[عدد_output_glyphs] = سر_2;
    وإلا
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عام:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محمي:
  Unsized_Array_دا<Offset32_منزل<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عام:
  DEFINE_ناپ_ARRAY (4, imageOffsetsZ);
};

بنية sbix
{
  ساكن constexpr hb_ٹیگ_t جدول_ٹیگ = HB_OT_ٹیگ_sbix;

  منطقي has_انکڑے () ثابت { أرجع نسخہ; }

  ثابت SBIXStrike &get_strike (بنانشانی صحيح i) ثابت { أرجع هذا+strikes[i]; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<sbix> (face);
      عدد_glyphs = face->get_عدد_glyphs ();
    }
    ~accelerator_t () { جدول.destroy (); }

    منطقي has_انکڑے () ثابت { أرجع جدول->has_انکڑے (); }

    منطقي get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      منطقي                scale = سچ) ثابت
    {
      /* We only support PNG right now, and following function checks type. */
      أرجع get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      صحيح            *x_وتھ_2,
			      صحيح            *y_وتھ_2,
			      بنانشانی صحيح   *دستیاب_ppem) ثابت
    {
      أرجع چنو_strike (font).get_glyph_blob (glyph_id, جدول.get_blob (),
						  HB_ٹیگ ('p','n','g',' '),
						  x_وتھ_2, y_وتھ_2,
						  عدد_glyphs, دستیاب_ppem);
    }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *انکڑے) ثابت
    {
      إذا (!has_انکڑے ())
        أرجع خطأ;

      صحيح x_وتھ_2 = 0, y_وتھ_2 = 0;
      بنانشانی صحيح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      إذا (!font->get_glyph_extents (glyph, &extents, خطأ))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph, &x_وتھ_2, &y_وتھ_2, &strike_ppem);
      إذا (hb_blob_is_immutable (blob))
        أرجع خطأ;

      منطقي ret = funcs->image (انکڑے,
			       blob,
			       pixel_extents.چوڑائی, -pixel_extents.اچائی,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:

    ثابت SBIXStrike &چنو_strike (hb_font_t *font) ثابت
    {
      بنانشانی گنتی = جدول->strikes.len;
      إذا (unlikely (!گنتی))
	أرجع Null (SBIXStrike);

      بنانشانی صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      إذا (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      بنانشانی صحيح بہترین_i = 0;
      بنانشانی صحيح بہترین_ppem = جدول->get_strike (0).ppem;

      لكل (بنانشانی صحيح i = 1; i < گنتی; i++)
      {
	بنانشانی صحيح ppem = (جدول->get_strike (i)).ppem;
	إذا ((requested_ppem <= ppem && ppem < بہترین_ppem) ||
	    (requested_ppem > بہترین_ppem && ppem > بہترین_ppem))
	{
	  بہترین_i = i;
	  بہترین_ppem = ppem;
	}
      }

      أرجع جدول->get_strike (بہترین_i);
    }

    بنية PNGHeader
    {
      HBUINT8	signature[8];
      بنية
      {
	بنية
	{
	  HBUINT32	لمائی;
	  ٹیگ		type;
	}		سرخی;
	HBUINT32	چوڑائی;
	HBUINT32	اچائی;
	HBUINT8		ثنائی_ہندسہ_ڈونگھائی;
	HBUINT8		colorType;
	HBUINT8		compression_طریقہ;
	HBUINT8		filter_طریقہ;
	HBUINT8		interlace_طریقہ;
      } IHDR;

      عام:
      DEFINE_ناپ_STATIC (29);
    };

    منطقي get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقي                scale = سچ) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      إذا (!has_انکڑے ())
	أرجع خطأ;

      صحيح x_وتھ_2 = 0, y_وتھ_2 = 0;
      بنانشانی صحيح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_وتھ_2, &y_وتھ_2, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      إذا (png.IHDR.اچائی >= 65536 || png.IHDR.چوڑائی >= 65536)
      {
	hb_blob_destroy (blob);
	أرجع خطأ;
      }

      extents->x_bearing = x_وتھ_2;
      extents->y_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) (بنانشانی صحيح) png.IHDR.اچائی + y_وتھ_2);
      extents->چوڑائی     = (hb_position_t) (بنانشانی صحيح) png.IHDR.چوڑائی;
      extents->اچائی    = -(hb_position_t) (بنانشانی صحيح) png.IHDR.اچائی;

      /* Convert to font units. */
      إذا (strike_ppem && scale)
      {
	عائم scale = font->face->get_upem () / (عائم) strike_ppem;
	extents->x_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->چوڑائی = hb_clamp_منزل<hb_position_t> (roundf (extents->چوڑائی * scale));
	extents->اچائی = hb_clamp_منزل<hb_position_t> (roundf (extents->اچائی * scale));
      }

      إذا (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      أرجع strike_ppem;
    }

    خاص:
    hb_blob_ptr_t<sbix> جدول;

    بنانشانی صحيح عدد_glyphs;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخہ >= 1 &&
			  strikes.sanitize (c, هذا)));
  }

  منطقي
  جوڑو_strike (hb_subset_context_t *c, بنانشانی i) ثابت
  {
    إذا (strikes[i].is_null () || c->سروت_blob->لمائی < (بنانشانی) strikes[i])
      أرجع خطأ;

    أرجع (هذا+strikes[i]).subset (c, c->سروت_blob->لمائی - (بنانشانی) strikes[i]);
  }

  منطقي serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودکار *باہرلا = c->serializer->شروع_embed<Array32_دا_Offset32_منزل<SBIXStrike>> ();
    إذا (unlikely (!c->serializer->extend_min (باہرلا))) return_trace (خطأ);

    hb_سمتیہ_t<Offset32_منزل<SBIXStrike>*> new_strikes;
    hb_سمتیہ_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (صحيح i = strikes.len - 1; i >= 0; --i)
    {
      خودکار* o = باہرلا->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      *o = 0;
      خودکار snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقي ret = جوڑو_strike (c, i);
      إذا (!ret)
      {
	c->serializer->نکالو_discard ();
	باہرلا->نکالو ();
	c->serializer->revert (snap);
      }
      وإلا
      {
	objidxs.push (c->serializer->نکالو_pack ());
	new_strikes.push (o);
      }
    }
    لكل (بنانشانی صحيح i = 0; i < new_strikes.لمائی; ++i)
      c->serializer->جوڑو_link (*new_strikes[i], objidxs[new_strikes.لمائی - 1 - i]);

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t* c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->نسخہ))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->flags))) return_trace (خطأ);

    return_trace (serialize_strike_offsets (c));
  }

  محمي:
  HBUINT16	نسخہ;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32_دا_Offset32_منزل<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عام:
  DEFINE_ناپ_ARRAY (8, strikes);
};

بنية sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#جے_انت /* OT_COLOR_SBIX_SBIX_HH */

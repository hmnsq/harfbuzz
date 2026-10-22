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

#اگر_غیر_تعریف_شدہ OT_COLOR_SBIX_SBIX_HH
#تعریف_کرو OT_COLOR_SBIX_SBIX_HH

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#تعریف_کرو HB_OT_لیبل_sbix HB_لیبل('s','b','i','x')


نامخلا OT {


ساخت SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, غیرعلامتی صحیح ڈیٹا_لمبائی) ثابت
  {
    TRACE_SERIALIZE (یہ);
    SBIXGlyph* new_glyph = c->آغاز_embed<SBIXGlyph> ();
    اگر (unlikely (!c->extend_min (new_glyph))) return_trace (خالی_اشاریہ);

    new_glyph->x_انحراف = x_انحراف;
    new_glyph->y_انحراف = y_انحراف;
    new_glyph->graphicType = graphicType;
    ڈیٹا.copy (c, ڈیٹا_لمبائی);
    return_trace (new_glyph);
  }

  HBINT16	x_انحراف;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_انحراف;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  لیبل		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_کا<HBUINT8>
		ڈیٹا;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  عوامی:
  DEFINE_سائز_ARRAY (8, ڈیٹا);
};

ساخت SBIXStrike
{
  ساکن size_t get_سائز (غیرعلامتی عدد_glyphs)
  { واپس min_سائز + عدد_glyphs * HBUINT32::static_سائز; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (c->check_struct (یہ) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_عدد_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (غیرعلامتی صحیح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_لیبل_t      file_type,
			     صحیح          *x_انحراف_2,
			     صحیح          *y_انحراف_2,
			     غیرعلامتی صحیح  عدد_glyphs,
			     غیرعلامتی صحیح *strike_ppem) ثابت
  {
    اگر (unlikely (!ppem)) واپس hb_blob_get_خالی_خانے (); /* To get Null() object out of the way. */

    غیرعلامتی صحیح retry_گنتی = 8;
    غیرعلامتی صحیح sbix_len = sbix_blob->لمبائی;
    غیرعلامتی صحیح strike_انحراف = (ثابت حرف *) یہ - (ثابت حرف *) sbix_blob->ڈیٹا;
    أكد (strike_انحراف < sbix_len);

  retry:
    اگر (unlikely (glyph_id >= عدد_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_سائز ||
		  (غیرعلامتی صحیح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_انحراف))
      واپس hb_blob_get_خالی_خانے ();

    غیرعلامتی صحیح glyph_انحراف = strike_انحراف + (غیرعلامتی صحیح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_سائز;
    غیرعلامتی صحیح glyph_لمبائی = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_سائز;

    ثابت SBIXGlyph *glyph = &(یہ+imageOffsetsZ[glyph_id]);

    اگر (glyph->graphicType == HB_لیبل ('d','u','p','e'))
    {
      اگر (glyph_لمبائی >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->ڈیٹا);
	اگر (retry_گنتی--)
	  جاؤ retry;
      }
      واپس hb_blob_get_خالی_خانے ();
    }

    اگر (unlikely (file_type != glyph->graphicType))
      واپس hb_blob_get_خالی_خانے ();

    اگر (strike_ppem) *strike_ppem = ppem;
    اگر (x_انحراف_2) *x_انحراف_2 = glyph->x_انحراف;
    اگر (y_انحراف_2) *y_انحراف_2 = glyph->y_انحراف;
    واپس hb_blob_create_sub_blob (sbix_blob, glyph_انحراف, glyph_لمبائی);
  }

  منطقی subset (hb_subset_context_t *c, غیرعلامتی صحیح دستیاب_len) ثابت
  {
    TRACE_SUBSET (یہ);
    غیرعلامتی صحیح عدد_output_glyphs = c->plan->عدد_output_glyphs ();

    خودکار* برآمد = c->serializer->آغاز_embed<SBIXStrike> ();
    خودکار snap = c->serializer->snapshot ();
    اگر (unlikely (!c->serializer->extend (برآمد, عدد_output_glyphs + 1))) return_trace (جھوٹ);
    برآمد->ppem = ppem;
    برآمد->resolution = resolution;
    HBUINT32 سر_2;
    سر_2 = get_سائز (عدد_output_glyphs + 1);

    منطقی has_glyphs = جھوٹ;
    برائے (غیرعلامتی new_gid = 0; new_gid < عدد_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      اگر (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_سائز) ||
		    (غیرعلامتی صحیح) imageOffsetsZ[old_gid + 1] > دستیاب_len)
      {
	برآمد->imageOffsetsZ[new_gid] = سر_2;
	جاری;
      }
      has_glyphs = سچ;
      غیرعلامتی صحیح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      غیرعلامتی صحیح glyph_ڈیٹا_لمبائی = delta - SBIXGlyph::min_سائز;
      اگر (!(یہ+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_ڈیٹا_لمبائی))
	return_trace (جھوٹ);
      برآمد->imageOffsetsZ[new_gid] = سر_2;
      سر_2 += delta;
    }
    اگر (has_glyphs)
      برآمد->imageOffsetsZ[عدد_output_glyphs] = سر_2;
    ورنہ
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عوامی:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محفوظ:
  Unsized_Array_کا<Offset32_مقصد<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عوامی:
  DEFINE_سائز_ARRAY (4, imageOffsetsZ);
};

ساخت sbix
{
  ساکن constexpr hb_لیبل_t جدول_لیبل = HB_OT_لیبل_sbix;

  منطقی has_ڈیٹا () ثابت { واپس نسخہ; }

  ثابت SBIXStrike &get_strike (غیرعلامتی صحیح i) ثابت { واپس یہ+strikes[i]; }

  ساخت accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<sbix> (face);
      عدد_glyphs = face->get_عدد_glyphs ();
    }
    ~accelerator_t () { جدول.destroy (); }

    منطقی has_ڈیٹا () ثابت { واپس جدول->has_ڈیٹا (); }

    منطقی get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      منطقی                scale = سچ) ثابت
    {
      /* We only support PNG right now, and following function checks type. */
      واپس get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      صحیح            *x_انحراف_2,
			      صحیح            *y_انحراف_2,
			      غیرعلامتی صحیح   *دستیاب_ppem) ثابت
    {
      واپس انتخاب_strike (font).get_glyph_blob (glyph_id, جدول.get_blob (),
						  HB_لیبل ('p','n','g',' '),
						  x_انحراف_2, y_انحراف_2,
						  عدد_glyphs, دستیاب_ppem);
    }

    منطقی paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, خالی *ڈیٹا) ثابت
    {
      اگر (!has_ڈیٹا ())
        واپس جھوٹ;

      صحیح x_انحراف_2 = 0, y_انحراف_2 = 0;
      غیرعلامتی صحیح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      اگر (!font->get_glyph_extents (glyph, &extents, جھوٹ))
        واپس جھوٹ;

      اگر (unlikely (!get_extents (font, glyph, &pixel_extents, جھوٹ)))
        واپس جھوٹ;

      hb_blob_t *blob = reference_png (font, glyph, &x_انحراف_2, &y_انحراف_2, &strike_ppem);
      اگر (hb_blob_is_immutable (blob))
        واپس جھوٹ;

      منطقی ret = funcs->image (ڈیٹا,
			       blob,
			       pixel_extents.چوڑائی, -pixel_extents.اونچائی,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      واپس ret;
    }

    نجی:

    ثابت SBIXStrike &انتخاب_strike (hb_font_t *font) ثابت
    {
      غیرعلامتی گنتی = جدول->strikes.len;
      اگر (unlikely (!گنتی))
	واپس Null (SBIXStrike);

      غیرعلامتی صحیح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      اگر (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      غیرعلامتی صحیح بہترین_i = 0;
      غیرعلامتی صحیح بہترین_ppem = جدول->get_strike (0).ppem;

      برائے (غیرعلامتی صحیح i = 1; i < گنتی; i++)
      {
	غیرعلامتی صحیح ppem = (جدول->get_strike (i)).ppem;
	اگر ((requested_ppem <= ppem && ppem < بہترین_ppem) ||
	    (requested_ppem > بہترین_ppem && ppem > بہترین_ppem))
	{
	  بہترین_i = i;
	  بہترین_ppem = ppem;
	}
      }

      واپس جدول->get_strike (بہترین_i);
    }

    ساخت PNGHeader
    {
      HBUINT8	signature[8];
      ساخت
      {
	ساخت
	{
	  HBUINT32	لمبائی;
	  لیبل		type;
	}		سرخی;
	HBUINT32	چوڑائی;
	HBUINT32	اونچائی;
	HBUINT8		ثنائی_ہندسہ_گہرائی;
	HBUINT8		colorType;
	HBUINT8		compression_طریقہ;
	HBUINT8		filter_طریقہ;
	HBUINT8		interlace_طریقہ;
      } IHDR;

      عوامی:
      DEFINE_سائز_STATIC (29);
    };

    منطقی get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقی                scale = سچ) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      اگر (!has_ڈیٹا ())
	واپس جھوٹ;

      صحیح x_انحراف_2 = 0, y_انحراف_2 = 0;
      غیرعلامتی صحیح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_انحراف_2, &y_انحراف_2, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      اگر (png.IHDR.اونچائی >= 65536 || png.IHDR.چوڑائی >= 65536)
      {
	hb_blob_destroy (blob);
	واپس جھوٹ;
      }

      extents->x_bearing = x_انحراف_2;
      extents->y_bearing = hb_clamp_مقصد<hb_position_t> ((int64_t) (غیرعلامتی صحیح) png.IHDR.اونچائی + y_انحراف_2);
      extents->چوڑائی     = (hb_position_t) (غیرعلامتی صحیح) png.IHDR.چوڑائی;
      extents->اونچائی    = -(hb_position_t) (غیرعلامتی صحیح) png.IHDR.اونچائی;

      /* Convert to font units. */
      اگر (strike_ppem && scale)
      {
	اعشاری scale = font->face->get_upem () / (اعشاری) strike_ppem;
	extents->x_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_مقصد<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->چوڑائی = hb_clamp_مقصد<hb_position_t> (roundf (extents->چوڑائی * scale));
	extents->اونچائی = hb_clamp_مقصد<hb_position_t> (roundf (extents->اونچائی * scale));
      }

      اگر (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      واپس strike_ppem;
    }

    نجی:
    hb_blob_ptr_t<sbix> جدول;

    غیرعلامتی صحیح عدد_glyphs;
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (یہ);
    return_trace (likely (c->check_struct (یہ) &&
			  hb_barrier () &&
			  نسخہ >= 1 &&
			  strikes.sanitize (c, یہ)));
  }

  منطقی
  جوڑو_strike (hb_subset_context_t *c, غیرعلامتی i) ثابت
  {
    اگر (strikes[i].is_null () || c->ماخذ_blob->لمبائی < (غیرعلامتی) strikes[i])
      واپس جھوٹ;

    واپس (یہ+strikes[i]).subset (c, c->ماخذ_blob->لمبائی - (غیرعلامتی) strikes[i]);
  }

  منطقی serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (یہ);

    خودکار *برآمد = c->serializer->آغاز_embed<Array32_کا_Offset32_مقصد<SBIXStrike>> ();
    اگر (unlikely (!c->serializer->extend_min (برآمد))) return_trace (جھوٹ);

    hb_سمتیہ_t<Offset32_مقصد<SBIXStrike>*> new_strikes;
    hb_سمتیہ_t<hb_serialize_context_t::objidx_t> objidxs;
    برائے (صحیح i = strikes.len - 1; i >= 0; --i)
    {
      خودکار* o = برآمد->serialize_append (c->serializer);
      اگر (unlikely (!o)) return_trace (جھوٹ);
      *o = 0;
      خودکار snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقی ret = جوڑو_strike (c, i);
      اگر (!ret)
      {
	c->serializer->نکالو_discard ();
	برآمد->نکالو ();
	c->serializer->revert (snap);
      }
      ورنہ
      {
	objidxs.push (c->serializer->نکالو_pack ());
	new_strikes.push (o);
      }
    }
    برائے (غیرعلامتی صحیح i = 0; i < new_strikes.لمبائی; ++i)
      c->serializer->جوڑو_link (*new_strikes[i], objidxs[new_strikes.لمبائی - 1 - i]);

    return_trace (سچ);
  }

  منطقی subset (hb_subset_context_t* c) ثابت
  {
    TRACE_SUBSET (یہ);

    اگر (unlikely (!c->serializer->embed (یہ->نسخہ))) return_trace (جھوٹ);
    اگر (unlikely (!c->serializer->embed (یہ->flags))) return_trace (جھوٹ);

    return_trace (serialize_strike_offsets (c));
  }

  محفوظ:
  HBUINT16	نسخہ;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32_کا_Offset32_مقصد<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عوامی:
  DEFINE_سائز_ARRAY (8, strikes);
};

ساخت sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#اگر_ختم /* OT_COLOR_SBIX_SBIX_HH */

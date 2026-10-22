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

#جيڪڏهن_وصف_نه_ٿيل OT_COLOR_SBIX_SBIX_HH
#وصف_ڪريو OT_COLOR_SBIX_SBIX_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#وصف_ڪريو HB_OT_ٽيگ_sbix HB_ٽيگ('s','b','i','x')


نطاق OT {


بنية SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, بنانشان صحيح معلومات_ڊيگهائي) ثابت
  {
    TRACE_SERIALIZE (هذا);
    SBIXGlyph* new_glyph = c->شروعات_embed<SBIXGlyph> ();
    إذا (unlikely (!c->extend_min (new_glyph))) return_trace (خالي_اشارو);

    new_glyph->x_وٿي = x_وٿي;
    new_glyph->y_وٿي = y_وٿي;
    new_glyph->graphicType = graphicType;
    معلومات.copy (c, معلومات_ڊيگهائي);
    return_trace (new_glyph);
  }

  HBINT16	x_وٿي;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_وٿي;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  ٽيگ		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_جو<HBUINT8>
		معلومات;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  عام:
  DEFINE_سائيز_ARRAY (8, معلومات);
};

بنية SBIXStrike
{
  ساكن size_t get_سائيز (بنانشان انگ_glyphs)
  { أرجع min_سائيز + انگ_glyphs * HBUINT32::static_سائيز; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_انگ_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (بنانشان صحيح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_ٽيگ_t      file_type,
			     صحيح          *x_وٿي_2,
			     صحيح          *y_وٿي_2,
			     بنانشان صحيح  انگ_glyphs,
			     بنانشان صحيح *strike_ppem) ثابت
  {
    إذا (unlikely (!ppem)) أرجع hb_blob_get_خالي_جايون (); /* To get Null() object out of the way. */

    بنانشان صحيح retry_ڳڻپ = 8;
    بنانشان صحيح sbix_len = sbix_blob->ڊيگهائي;
    بنانشان صحيح strike_وٿي = (ثابت حرف *) هذا - (ثابت حرف *) sbix_blob->معلومات;
    أكد (strike_وٿي < sbix_len);

  retry:
    إذا (unlikely (glyph_id >= انگ_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_سائيز ||
		  (بنانشان صحيح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_وٿي))
      أرجع hb_blob_get_خالي_جايون ();

    بنانشان صحيح glyph_وٿي = strike_وٿي + (بنانشان صحيح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_سائيز;
    بنانشان صحيح glyph_ڊيگهائي = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_سائيز;

    ثابت SBIXGlyph *glyph = &(هذا+imageOffsetsZ[glyph_id]);

    إذا (glyph->graphicType == HB_ٽيگ ('d','u','p','e'))
    {
      إذا (glyph_ڊيگهائي >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->معلومات);
	إذا (retry_ڳڻپ--)
	  اذهب retry;
      }
      أرجع hb_blob_get_خالي_جايون ();
    }

    إذا (unlikely (file_type != glyph->graphicType))
      أرجع hb_blob_get_خالي_جايون ();

    إذا (strike_ppem) *strike_ppem = ppem;
    إذا (x_وٿي_2) *x_وٿي_2 = glyph->x_وٿي;
    إذا (y_وٿي_2) *y_وٿي_2 = glyph->y_وٿي;
    أرجع hb_blob_create_sub_blob (sbix_blob, glyph_وٿي, glyph_ڊيگهائي);
  }

  منطقي subset (hb_subset_context_t *c, بنانشان صحيح موجود_len) ثابت
  {
    TRACE_SUBSET (هذا);
    بنانشان صحيح انگ_output_glyphs = c->plan->انگ_output_glyphs ();

    خودڪار* ٻاهريون = c->serializer->شروعات_embed<SBIXStrike> ();
    خودڪار snap = c->serializer->snapshot ();
    إذا (unlikely (!c->serializer->extend (ٻاهريون, انگ_output_glyphs + 1))) return_trace (خطأ);
    ٻاهريون->ppem = ppem;
    ٻاهريون->resolution = resolution;
    HBUINT32 سر_2;
    سر_2 = get_سائيز (انگ_output_glyphs + 1);

    منطقي has_glyphs = خطأ;
    لكل (بنانشان new_gid = 0; new_gid < انگ_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_سائيز) ||
		    (بنانشان صحيح) imageOffsetsZ[old_gid + 1] > موجود_len)
      {
	ٻاهريون->imageOffsetsZ[new_gid] = سر_2;
	استمر;
      }
      has_glyphs = سچ;
      بنانشان صحيح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      بنانشان صحيح glyph_معلومات_ڊيگهائي = delta - SBIXGlyph::min_سائيز;
      إذا (!(هذا+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_معلومات_ڊيگهائي))
	return_trace (خطأ);
      ٻاهريون->imageOffsetsZ[new_gid] = سر_2;
      سر_2 += delta;
    }
    إذا (has_glyphs)
      ٻاهريون->imageOffsetsZ[انگ_output_glyphs] = سر_2;
    وإلا
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عام:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محمي:
  Unsized_Array_جو<Offset32_منزل<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عام:
  DEFINE_سائيز_ARRAY (4, imageOffsetsZ);
};

بنية sbix
{
  ساكن constexpr hb_ٽيگ_t جدول_ٽيگ = HB_OT_ٽيگ_sbix;

  منطقي has_معلومات () ثابت { أرجع نسخو; }

  ثابت SBIXStrike &get_strike (بنانشان صحيح i) ثابت { أرجع هذا+strikes[i]; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      جدول = hb_sanitize_context_t ().reference_جدول<sbix> (face);
      انگ_glyphs = face->get_انگ_glyphs ();
    }
    ~accelerator_t () { جدول.destroy (); }

    منطقي has_معلومات () ثابت { أرجع جدول->has_معلومات (); }

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
			      صحيح            *x_وٿي_2,
			      صحيح            *y_وٿي_2,
			      بنانشان صحيح   *موجود_ppem) ثابت
    {
      أرجع چونڊيو_strike (font).get_glyph_blob (glyph_id, جدول.get_blob (),
						  HB_ٽيگ ('p','n','g',' '),
						  x_وٿي_2, y_وٿي_2,
						  انگ_glyphs, موجود_ppem);
    }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *معلومات) ثابت
    {
      إذا (!has_معلومات ())
        أرجع خطأ;

      صحيح x_وٿي_2 = 0, y_وٿي_2 = 0;
      بنانشان صحيح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      إذا (!font->get_glyph_extents (glyph, &extents, خطأ))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph, &x_وٿي_2, &y_وٿي_2, &strike_ppem);
      إذا (hb_blob_is_immutable (blob))
        أرجع خطأ;

      منطقي ret = funcs->image (معلومات,
			       blob,
			       pixel_extents.ويڪر, -pixel_extents.اوچائي,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:

    ثابت SBIXStrike &چونڊيو_strike (hb_font_t *font) ثابت
    {
      بنانشان ڳڻپ = جدول->strikes.len;
      إذا (unlikely (!ڳڻپ))
	أرجع Null (SBIXStrike);

      بنانشان صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      إذا (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      بنانشان صحيح بهترين_i = 0;
      بنانشان صحيح بهترين_ppem = جدول->get_strike (0).ppem;

      لكل (بنانشان صحيح i = 1; i < ڳڻپ; i++)
      {
	بنانشان صحيح ppem = (جدول->get_strike (i)).ppem;
	إذا ((requested_ppem <= ppem && ppem < بهترين_ppem) ||
	    (requested_ppem > بهترين_ppem && ppem > بهترين_ppem))
	{
	  بهترين_i = i;
	  بهترين_ppem = ppem;
	}
      }

      أرجع جدول->get_strike (بهترين_i);
    }

    بنية PNGHeader
    {
      HBUINT8	signature[8];
      بنية
      {
	بنية
	{
	  HBUINT32	ڊيگهائي;
	  ٽيگ		type;
	}		سرو;
	HBUINT32	ويڪر;
	HBUINT32	اوچائي;
	HBUINT8		ٻٽو_انگ_اونهائي;
	HBUINT8		colorType;
	HBUINT8		compression_طريقو;
	HBUINT8		filter_طريقو;
	HBUINT8		interlace_طريقو;
      } IHDR;

      عام:
      DEFINE_سائيز_STATIC (29);
    };

    منطقي get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقي                scale = سچ) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      إذا (!has_معلومات ())
	أرجع خطأ;

      صحيح x_وٿي_2 = 0, y_وٿي_2 = 0;
      بنانشان صحيح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_وٿي_2, &y_وٿي_2, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      إذا (png.IHDR.اوچائي >= 65536 || png.IHDR.ويڪر >= 65536)
      {
	hb_blob_destroy (blob);
	أرجع خطأ;
      }

      extents->x_bearing = x_وٿي_2;
      extents->y_bearing = hb_clamp_منزل<hb_position_t> ((int64_t) (بنانشان صحيح) png.IHDR.اوچائي + y_وٿي_2);
      extents->ويڪر     = (hb_position_t) (بنانشان صحيح) png.IHDR.ويڪر;
      extents->اوچائي    = -(hb_position_t) (بنانشان صحيح) png.IHDR.اوچائي;

      /* Convert to font units. */
      إذا (strike_ppem && scale)
      {
	عائم scale = font->face->get_upem () / (عائم) strike_ppem;
	extents->x_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_منزل<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->ويڪر = hb_clamp_منزل<hb_position_t> (roundf (extents->ويڪر * scale));
	extents->اوچائي = hb_clamp_منزل<hb_position_t> (roundf (extents->اوچائي * scale));
      }

      إذا (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      أرجع strike_ppem;
    }

    خاص:
    hb_blob_ptr_t<sbix> جدول;

    بنانشان صحيح انگ_glyphs;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  نسخو >= 1 &&
			  strikes.sanitize (c, هذا)));
  }

  منطقي
  جوڙيو_strike (hb_subset_context_t *c, بنانشان i) ثابت
  {
    إذا (strikes[i].is_null () || c->ذريعو_blob->ڊيگهائي < (بنانشان) strikes[i])
      أرجع خطأ;

    أرجع (هذا+strikes[i]).subset (c, c->ذريعو_blob->ڊيگهائي - (بنانشان) strikes[i]);
  }

  منطقي serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);

    خودڪار *ٻاهريون = c->serializer->شروعات_embed<Array32_جو_Offset32_منزل<SBIXStrike>> ();
    إذا (unlikely (!c->serializer->extend_min (ٻاهريون))) return_trace (خطأ);

    hb_ويڪٽر_t<Offset32_منزل<SBIXStrike>*> new_strikes;
    hb_ويڪٽر_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (صحيح i = strikes.len - 1; i >= 0; --i)
    {
      خودڪار* o = ٻاهريون->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      *o = 0;
      خودڪار snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقي ret = جوڙيو_strike (c, i);
      إذا (!ret)
      {
	c->serializer->ڪڍو_discard ();
	ٻاهريون->ڪڍو ();
	c->serializer->revert (snap);
      }
      وإلا
      {
	objidxs.push (c->serializer->ڪڍو_pack ());
	new_strikes.push (o);
      }
    }
    لكل (بنانشان صحيح i = 0; i < new_strikes.ڊيگهائي; ++i)
      c->serializer->جوڙيو_link (*new_strikes[i], objidxs[new_strikes.ڊيگهائي - 1 - i]);

    return_trace (سچ);
  }

  منطقي subset (hb_subset_context_t* c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->نسخو))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->flags))) return_trace (خطأ);

    return_trace (serialize_strike_offsets (c));
  }

  محمي:
  HBUINT16	نسخو;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32_جو_Offset32_منزل<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عام:
  DEFINE_سائيز_ARRAY (8, strikes);
};

بنية sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#جيڪڏهن_پورو /* OT_COLOR_SBIX_SBIX_HH */

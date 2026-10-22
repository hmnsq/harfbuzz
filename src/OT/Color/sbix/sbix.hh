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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OT_COLOR_SBIX_SBIX_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OT_COLOR_SBIX_SBIX_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-open-type.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_OT_ਟੈਗ_sbix HB_ਟੈਗ('s','b','i','x')


ਨਾਮਾਕਾਸ਼ OT {


ਬਣਤਰ SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕੜੇ_ਲੰਮਾਈ) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);
    SBIXGlyph* new_glyph = c->ਸ਼ੁਰੂ_embed<SBIXGlyph> ();
    ਜੇ (unlikely (!c->extend_min (new_glyph))) return_trace (ਖਾਲੀ_ਸੂਚਕ);

    new_glyph->x_ਵਿੱਥ = x_ਵਿੱਥ;
    new_glyph->y_ਵਿੱਥ = y_ਵਿੱਥ;
    new_glyph->graphicType = graphicType;
    ਅੰਕੜੇ.copy (c, ਅੰਕੜੇ_ਲੰਮਾਈ);
    return_trace (new_glyph);
  }

  HBINT16	x_ਵਿੱਥ;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_ਵਿੱਥ;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  ਟੈਗ		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_ਦਾ<HBUINT8>
		ਅੰਕੜੇ;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, ਅੰਕੜੇ);
};

ਬਣਤਰ SBIXStrike
{
  ਸਥਿਰ size_t get_ਮਾਪ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_glyphs)
  { ਵਾਪਸ min_ਮਾਪ + ਅੰਕ_glyphs * HBUINT32::static_ਮਾਪ; }

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (c->check_struct (ਇਹ) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_ਅੰਕ_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_ਟੈਗ_t      file_type,
			     ਪੂਰਨ          *x_ਵਿੱਥ_2,
			     ਪੂਰਨ          *y_ਵਿੱਥ_2,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ  ਅੰਕ_glyphs,
			     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *strike_ppem) ਅਚਲ
  {
    ਜੇ (unlikely (!ppem)) ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ (); /* To get Null() object out of the way. */

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ retry_ਗਿਣਤੀ = 8;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ sbix_len = sbix_blob->ਲੰਮਾਈ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ strike_ਵਿੱਥ = (ਅਚਲ ਅੱਖਰ *) ਇਹ - (ਅਚਲ ਅੱਖਰ *) sbix_blob->ਅੰਕੜੇ;
    ਪ੍ਰਤਿਜ੍ਞਾ (strike_ਵਿੱਥ < sbix_len);

  retry:
    ਜੇ (unlikely (glyph_id >= ਅੰਕ_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_ਮਾਪ ||
		  (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_ਵਿੱਥ))
      ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਵਿੱਥ = strike_ਵਿੱਥ + (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) imageOffsetsZ[glyph_id] + SBIXGlyph::min_ਮਾਪ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਲੰਮਾਈ = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_ਮਾਪ;

    ਅਚਲ SBIXGlyph *glyph = &(ਇਹ+imageOffsetsZ[glyph_id]);

    ਜੇ (glyph->graphicType == HB_ਟੈਗ ('d','u','p','e'))
    {
      ਜੇ (glyph_ਲੰਮਾਈ >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->ਅੰਕੜੇ);
	ਜੇ (retry_ਗਿਣਤੀ--)
	  ਜਾਓ retry;
      }
      ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();
    }

    ਜੇ (unlikely (file_type != glyph->graphicType))
      ਵਾਪਸ hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ();

    ਜੇ (strike_ppem) *strike_ppem = ppem;
    ਜੇ (x_ਵਿੱਥ_2) *x_ਵਿੱਥ_2 = glyph->x_ਵਿੱਥ;
    ਜੇ (y_ਵਿੱਥ_2) *y_ਵਿੱਥ_2 = glyph->y_ਵਿੱਥ;
    ਵਾਪਸ hb_blob_create_sub_blob (sbix_blob, glyph_ਵਿੱਥ, glyph_ਲੰਮਾਈ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉਪਲਬਧ_len) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_output_glyphs = c->plan->ਅੰਕ_output_glyphs ();

    ਸਵੈਚਾਲਿਤ* ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<SBIXStrike> ();
    ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
    ਜੇ (unlikely (!c->serializer->extend (ਬਾਹਰਲਾ, ਅੰਕ_output_glyphs + 1))) return_trace (ਝੂਠ);
    ਬਾਹਰਲਾ->ppem = ppem;
    ਬਾਹਰਲਾ->resolution = resolution;
    HBUINT32 ਸਿਰ_2;
    ਸਿਰ_2 = get_ਮਾਪ (ਅੰਕ_output_glyphs + 1);

    ਤਾਰਕਿਕ has_glyphs = ਝੂਠ;
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ new_gid = 0; new_gid < ਅੰਕ_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ਜੇ (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_ਮਾਪ) ||
		    (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) imageOffsetsZ[old_gid + 1] > ਉਪਲਬਧ_len)
      {
	ਬਾਹਰਲਾ->imageOffsetsZ[new_gid] = ਸਿਰ_2;
	ਜਾਰੀ;
      }
      has_glyphs = ਸੱਚ;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ glyph_ਅੰਕੜੇ_ਲੰਮਾਈ = delta - SBIXGlyph::min_ਮਾਪ;
      ਜੇ (!(ਇਹ+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_ਅੰਕੜੇ_ਲੰਮਾਈ))
	return_trace (ਝੂਠ);
      ਬਾਹਰਲਾ->imageOffsetsZ[new_gid] = ਸਿਰ_2;
      ਸਿਰ_2 += delta;
    }
    ਜੇ (has_glyphs)
      ਬਾਹਰਲਾ->imageOffsetsZ[ਅੰਕ_output_glyphs] = ਸਿਰ_2;
    ਨਹੀਂਤਾਂ
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  ਜਨਤਕ:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  ਸੁਰੱਖਿਅਤ:
  Unsized_Array_ਦਾ<Offset32_ਮੰਜ਼ਿਲ<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (4, imageOffsetsZ);
};

ਬਣਤਰ sbix
{
  ਸਥਿਰ constexpr hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ = HB_OT_ਟੈਗ_sbix;

  ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸੰਸਕਰਣ; }

  ਅਚਲ SBIXStrike &get_strike (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i) ਅਚਲ { ਵਾਪਸ ਇਹ+strikes[i]; }

  ਬਣਤਰ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      ਸਾਰਣੀ = hb_sanitize_context_t ().reference_ਸਾਰਣੀ<sbix> (face);
      ਅੰਕ_glyphs = face->get_ਅੰਕ_glyphs ();
    }
    ~accelerator_t () { ਸਾਰਣੀ.destroy (); }

    ਤਾਰਕਿਕ has_ਅੰਕੜੇ () ਅਚਲ { ਵਾਪਸ ਸਾਰਣੀ->has_ਅੰਕੜੇ (); }

    ਤਾਰਕਿਕ get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      ਤਾਰਕਿਕ                scale = ਸੱਚ) ਅਚਲ
    {
      /* We only support PNG right now, and following function checks type. */
      ਵਾਪਸ get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      ਪੂਰਨ            *x_ਵਿੱਥ_2,
			      ਪੂਰਨ            *y_ਵਿੱਥ_2,
			      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ   *ਉਪਲਬਧ_ppem) ਅਚਲ
    {
      ਵਾਪਸ ਚੁਣੋ_strike (font).get_glyph_blob (glyph_id, ਸਾਰਣੀ.get_blob (),
						  HB_ਟੈਗ ('p','n','g',' '),
						  x_ਵਿੱਥ_2, y_ਵਿੱਥ_2,
						  ਅੰਕ_glyphs, ਉਪਲਬਧ_ppem);
    }

    ਤਾਰਕਿਕ paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ਖਾਲੀ *ਅੰਕੜੇ) ਅਚਲ
    {
      ਜੇ (!has_ਅੰਕੜੇ ())
        ਵਾਪਸ ਝੂਠ;

      ਪੂਰਨ x_ਵਿੱਥ_2 = 0, y_ਵਿੱਥ_2 = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      ਜੇ (!font->get_glyph_extents (glyph, &extents, ਝੂਠ))
        ਵਾਪਸ ਝੂਠ;

      ਜੇ (unlikely (!get_extents (font, glyph, &pixel_extents, ਝੂਠ)))
        ਵਾਪਸ ਝੂਠ;

      hb_blob_t *blob = reference_png (font, glyph, &x_ਵਿੱਥ_2, &y_ਵਿੱਥ_2, &strike_ppem);
      ਜੇ (hb_blob_is_immutable (blob))
        ਵਾਪਸ ਝੂਠ;

      ਤਾਰਕਿਕ ret = funcs->image (ਅੰਕੜੇ,
			       blob,
			       pixel_extents.ਚੌੜਾਈ, -pixel_extents.ਉਚਾਈ,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ਵਾਪਸ ret;
    }

    ਨਿਜੀ:

    ਅਚਲ SBIXStrike &ਚੁਣੋ_strike (hb_font_t *font) ਅਚਲ
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = ਸਾਰਣੀ->strikes.len;
      ਜੇ (unlikely (!ਗਿਣਤੀ_2))
	ਵਾਪਸ Null (SBIXStrike);

      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      ਜੇ (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉੱਤਮ_i = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉੱਤਮ_ppem = ਸਾਰਣੀ->get_strike (0).ppem;

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 1; i < ਗਿਣਤੀ_2; i++)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ppem = (ਸਾਰਣੀ->get_strike (i)).ppem;
	ਜੇ ((requested_ppem <= ppem && ppem < ਉੱਤਮ_ppem) ||
	    (requested_ppem > ਉੱਤਮ_ppem && ppem > ਉੱਤਮ_ppem))
	{
	  ਉੱਤਮ_i = i;
	  ਉੱਤਮ_ppem = ppem;
	}
      }

      ਵਾਪਸ ਸਾਰਣੀ->get_strike (ਉੱਤਮ_i);
    }

    ਬਣਤਰ PNGHeader
    {
      HBUINT8	signature[8];
      ਬਣਤਰ
      {
	ਬਣਤਰ
	{
	  HBUINT32	ਲੰਮਾਈ;
	  ਟੈਗ		type;
	}		ਸਿਰਲੇਖ;
	HBUINT32	ਚੌੜਾਈ;
	HBUINT32	ਉਚਾਈ;
	HBUINT8		ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ;
	HBUINT8		colorType;
	HBUINT8		compression_ਵਿਧੀ;
	HBUINT8		filter_ਵਿਧੀ;
	HBUINT8		interlace_ਵਿਧੀ;
      } IHDR;

      ਜਨਤਕ:
      DEFINE_ਮਾਪ_STATIC (29);
    };

    ਤਾਰਕਿਕ get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  ਤਾਰਕਿਕ                scale = ਸੱਚ) ਅਚਲ
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      ਜੇ (!has_ਅੰਕੜੇ ())
	ਵਾਪਸ ਝੂਠ;

      ਪੂਰਨ x_ਵਿੱਥ_2 = 0, y_ਵਿੱਥ_2 = 0;
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_ਵਿੱਥ_2, &y_ਵਿੱਥ_2, &strike_ppem);

      ਅਚਲ PNGHeader &png = *blob->as<PNGHeader>();

      ਜੇ (png.IHDR.ਉਚਾਈ >= 65536 || png.IHDR.ਚੌੜਾਈ >= 65536)
      {
	hb_blob_destroy (blob);
	ਵਾਪਸ ਝੂਠ;
      }

      extents->x_bearing = x_ਵਿੱਥ_2;
      extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> ((int64_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) png.IHDR.ਉਚਾਈ + y_ਵਿੱਥ_2);
      extents->ਚੌੜਾਈ     = (hb_position_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) png.IHDR.ਚੌੜਾਈ;
      extents->ਉਚਾਈ    = -(hb_position_t) (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) png.IHDR.ਉਚਾਈ;

      /* Convert to font units. */
      ਜੇ (strike_ppem && scale)
      {
	ਦਸ਼ਮਲਵ scale = font->face->get_upem () / (ਦਸ਼ਮਲਵ) strike_ppem;
	extents->x_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->ਚੌੜਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->ਚੌੜਾਈ * scale));
	extents->ਉਚਾਈ = hb_clamp_ਮੰਜ਼ਿਲ<hb_position_t> (roundf (extents->ਉਚਾਈ * scale));
      }

      ਜੇ (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      ਵਾਪਸ strike_ppem;
    }

    ਨਿਜੀ:
    hb_blob_ptr_t<sbix> ਸਾਰਣੀ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_glyphs;
  };

  ਤਾਰਕਿਕ sanitize (hb_sanitize_context_t *c) ਅਚਲ
  {
    TRACE_SANITIZE (ਇਹ);
    return_trace (likely (c->check_struct (ਇਹ) &&
			  hb_barrier () &&
			  ਸੰਸਕਰਣ >= 1 &&
			  strikes.sanitize (c, ਇਹ)));
  }

  ਤਾਰਕਿਕ
  ਜੋੜੋ_strike (hb_subset_context_t *c, ਬਿਨਾਨਿਸ਼ਾਨੀ i) ਅਚਲ
  {
    ਜੇ (strikes[i].is_null () || c->ਸਰੋਤ_blob->ਲੰਮਾਈ < (ਬਿਨਾਨਿਸ਼ਾਨੀ) strikes[i])
      ਵਾਪਸ ਝੂਠ;

    ਵਾਪਸ (ਇਹ+strikes[i]).subset (c, c->ਸਰੋਤ_blob->ਲੰਮਾਈ - (ਬਿਨਾਨਿਸ਼ਾਨੀ) strikes[i]);
  }

  ਤਾਰਕਿਕ serialize_strike_offsets (hb_subset_context_t *c) ਅਚਲ
  {
    TRACE_SERIALIZE (ਇਹ);

    ਸਵੈਚਾਲਿਤ *ਬਾਹਰਲਾ = c->serializer->ਸ਼ੁਰੂ_embed<Array32_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<SBIXStrike>> ();
    ਜੇ (unlikely (!c->serializer->extend_min (ਬਾਹਰਲਾ))) return_trace (ਝੂਠ);

    hb_ਸਦਿਸ਼_t<Offset32_ਮੰਜ਼ਿਲ<SBIXStrike>*> new_strikes;
    hb_ਸਦਿਸ਼_t<hb_serialize_context_t::objidx_t> objidxs;
    ਲਈ (ਪੂਰਨ i = strikes.len - 1; i >= 0; --i)
    {
      ਸਵੈਚਾਲਿਤ* o = ਬਾਹਰਲਾ->serialize_append (c->serializer);
      ਜੇ (unlikely (!o)) return_trace (ਝੂਠ);
      *o = 0;
      ਸਵੈਚਾਲਿਤ snap = c->serializer->snapshot ();
      c->serializer->push ();
      ਤਾਰਕਿਕ ret = ਜੋੜੋ_strike (c, i);
      ਜੇ (!ret)
      {
	c->serializer->ਕੱਢੋ_discard ();
	ਬਾਹਰਲਾ->ਕੱਢੋ ();
	c->serializer->revert (snap);
      }
      ਨਹੀਂਤਾਂ
      {
	objidxs.push (c->serializer->ਕੱਢੋ_pack ());
	new_strikes.push (o);
      }
    }
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < new_strikes.ਲੰਮਾਈ; ++i)
      c->serializer->ਜੋੜੋ_link (*new_strikes[i], objidxs[new_strikes.ਲੰਮਾਈ - 1 - i]);

    return_trace (ਸੱਚ);
  }

  ਤਾਰਕਿਕ subset (hb_subset_context_t* c) ਅਚਲ
  {
    TRACE_SUBSET (ਇਹ);

    ਜੇ (unlikely (!c->serializer->embed (ਇਹ->ਸੰਸਕਰਣ))) return_trace (ਝੂਠ);
    ਜੇ (unlikely (!c->serializer->embed (ਇਹ->flags))) return_trace (ਝੂਠ);

    return_trace (serialize_strike_offsets (c));
  }

  ਸੁਰੱਖਿਅਤ:
  HBUINT16	ਸੰਸਕਰਣ;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32_ਦਾ_Offset32_ਮੰਜ਼ਿਲ<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  ਜਨਤਕ:
  DEFINE_ਮਾਪ_ARRAY (8, strikes);
};

ਬਣਤਰ sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#ਜੇ_ਅੰਤ /* OT_COLOR_SBIX_SBIX_HH */

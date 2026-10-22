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

#ifndef OT_COLOR_SBIX_SBIX_HH
#define OT_COLOR_SBIX_SBIX_HH

#ضمن "../../../hb-open-type.hh"
#ضمن "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


نطاق OT {


بنية SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned صحيح data_length) ثابت
  {
    TRACE_SERIALIZE (هذا);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    إذا (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

    new_glyph->xOffset = xOffset;
    new_glyph->yOffset = yOffset;
    new_glyph->graphicType = graphicType;
    data.copy (c, data_length);
    return_trace (new_glyph);
  }

  HBINT16	xOffset;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	yOffset;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  Tag		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  UnsizedArrayOf<HBUINT8>
		data;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  عام:
  DEFINE_SIZE_ARRAY (8, data);
};

بنية SBIXStrike
{
  ساكن size_t get_size (unsigned ژمارە_glyphs)
  { أرجع min_size + ژمارە_glyphs * HBUINT32::static_size; }

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (c->check_struct (هذا) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_ژمارە_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned صحيح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     صحيح          *x_offset,
			     صحيح          *y_offset,
			     unsigned صحيح  ژمارە_glyphs,
			     unsigned صحيح *strike_ppem) ثابت
  {
    إذا (unlikely (!ppem)) أرجع hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned صحيح retry_ژماردن = 8;
    unsigned صحيح sbix_len = sbix_blob->length;
    unsigned صحيح strike_offset = (ثابت حرف *) هذا - (ثابت حرف *) sbix_blob->data;
    أكد (strike_offset < sbix_len);

  retry:
    إذا (unlikely (glyph_id >= ژمارە_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned صحيح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      أرجع hb_blob_get_empty ();

    unsigned صحيح glyph_offset = strike_offset + (unsigned صحيح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned صحيح glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    ثابت SBIXGlyph *glyph = &(هذا+imageOffsetsZ[glyph_id]);

    إذا (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      إذا (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	إذا (retry_ژماردن--)
	  اذهب retry;
      }
      أرجع hb_blob_get_empty ();
    }

    إذا (unlikely (file_type != glyph->graphicType))
      أرجع hb_blob_get_empty ();

    إذا (strike_ppem) *strike_ppem = ppem;
    إذا (x_offset) *x_offset = glyph->xOffset;
    إذا (y_offset) *y_offset = glyph->yOffset;
    أرجع hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  منطقي subset (hb_subset_context_t *c, unsigned صحيح available_len) ثابت
  {
    TRACE_SUBSET (هذا);
    unsigned صحيح ژمارە_output_glyphs = c->plan->ژمارە_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    إذا (unlikely (!c->serializer->extend (out, ژمارە_output_glyphs + 1))) return_trace (خطأ);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (ژمارە_output_glyphs + 1);

    منطقي has_glyphs = خطأ;
    لكل (unsigned new_gid = 0; new_gid < ژمارە_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      إذا (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned صحيح) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	استمر;
      }
      has_glyphs = true;
      unsigned صحيح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned صحيح glyph_data_length = delta - SBIXGlyph::min_size;
      إذا (!(هذا+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (خطأ);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    إذا (has_glyphs)
      out->imageOffsetsZ[ژمارە_output_glyphs] = head;
    وإلا
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عام:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محمي:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عام:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

بنية sbix
{
  ساكن constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  منطقي has_data () ثابت { أرجع version; }

  ثابت SBIXStrike &get_strike (unsigned صحيح i) ثابت { أرجع هذا+strikes[i]; }

  بنية accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      ژمارە_glyphs = face->get_ژمارە_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    منطقي has_data () ثابت { أرجع table->has_data (); }

    منطقي get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      منطقي                scale = true) ثابت
    {
      /* We only support PNG right now, and following function checks type. */
      أرجع get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      صحيح            *x_offset,
			      صحيح            *y_offset,
			      unsigned صحيح   *available_ppem) ثابت
    {
      أرجع choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  ژمارە_glyphs, available_ppem);
    }

    منطقي paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, فراغ *data) ثابت
    {
      إذا (!has_data ())
        أرجع خطأ;

      صحيح x_offset = 0, y_offset = 0;
      unsigned صحيح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      إذا (!font->get_glyph_extents (glyph, &extents, خطأ))
        أرجع خطأ;

      إذا (unlikely (!get_extents (font, glyph, &pixel_extents, خطأ)))
        أرجع خطأ;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      إذا (hb_blob_is_immutable (blob))
        أرجع خطأ;

      منطقي ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      أرجع ret;
    }

    خاص:

    ثابت SBIXStrike &choose_strike (hb_font_t *font) ثابت
    {
      unsigned ژماردن = table->strikes.len;
      إذا (unlikely (!ژماردن))
	أرجع Null (SBIXStrike);

      unsigned صحيح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      إذا (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned صحيح best_i = 0;
      unsigned صحيح best_ppem = table->get_strike (0).ppem;

      لكل (unsigned صحيح i = 1; i < ژماردن; i++)
      {
	unsigned صحيح ppem = (table->get_strike (i)).ppem;
	إذا ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      أرجع table->get_strike (best_i);
    }

    بنية PNGHeader
    {
      HBUINT8	signature[8];
      بنية
      {
	بنية
	{
	  HBUINT32	length;
	  Tag		type;
	}		header;
	HBUINT32	width;
	HBUINT32	height;
	HBUINT8		bitDepth;
	HBUINT8		colorType;
	HBUINT8		compressionMethod;
	HBUINT8		filterMethod;
	HBUINT8		interlaceMethod;
      } IHDR;

      عام:
      DEFINE_SIZE_STATIC (29);
    };

    منطقي get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقي                scale = true) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      إذا (!has_data ())
	أرجع خطأ;

      صحيح x_offset = 0, y_offset = 0;
      unsigned صحيح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      إذا (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	أرجع خطأ;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned صحيح) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned صحيح) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned صحيح) png.IHDR.height;

      /* Convert to font units. */
      إذا (strike_ppem && scale)
      {
	عائم scale = font->face->get_upem () / (عائم) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      إذا (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      أرجع strike_ppem;
    }

    خاص:
    hb_blob_ptr_t<sbix> table;

    unsigned صحيح ژمارە_glyphs;
  };

  منطقي sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (هذا);
    return_trace (likely (c->check_struct (هذا) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, هذا)));
  }

  منطقي
  add_strike (hb_subset_context_t *c, unsigned i) ثابت
  {
    إذا (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      أرجع خطأ;

    أرجع (هذا+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  منطقي serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (هذا);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    إذا (unlikely (!c->serializer->extend_min (out))) return_trace (خطأ);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    لكل (صحيح i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      إذا (unlikely (!o)) return_trace (خطأ);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقي ret = add_strike (c, i);
      إذا (!ret)
      {
	c->serializer->دەرهێنان_discard ();
	out->دەرهێنان ();
	c->serializer->revert (snap);
      }
      وإلا
      {
	objidxs.push (c->serializer->دەرهێنان_pack ());
	new_strikes.push (o);
      }
    }
    لكل (unsigned صحيح i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (true);
  }

  منطقي subset (hb_subset_context_t* c) ثابت
  {
    TRACE_SUBSET (هذا);

    إذا (unlikely (!c->serializer->embed (هذا->version))) return_trace (خطأ);
    إذا (unlikely (!c->serializer->embed (هذا->flags))) return_trace (خطأ);

    return_trace (serialize_strike_offsets (c));
  }

  محمي:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عام:
  DEFINE_SIZE_ARRAY (8, strikes);
};

بنية sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

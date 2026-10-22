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

#شامل "../../../hb-open-type.hh"
#شامل "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


فضاینام OT {


ساختار SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned صحیح data_length) ثابت
  {
    TRACE_SERIALIZE (این);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    اگر (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  عمومی:
  DEFINE_SIZE_ARRAY (8, data);
};

ساختار SBIXStrike
{
  ایستا size_t get_size (unsigned رقم_glyphs)
  { بازگشت min_size + رقم_glyphs * HBUINT32::static_size; }

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (c->check_struct (این) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_رقم_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned صحیح  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     صحیح          *x_offset,
			     صحیح          *y_offset,
			     unsigned صحیح  رقم_glyphs,
			     unsigned صحیح *strike_ppem) ثابت
  {
    اگر (unlikely (!ppem)) بازگشت hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned صحیح retry_شمار = 8;
    unsigned صحیح sbix_len = sbix_blob->length;
    unsigned صحیح strike_offset = (ثابت نویسه *) این - (ثابت نویسه *) sbix_blob->data;
    أكد (strike_offset < sbix_len);

  retry:
    اگر (unlikely (glyph_id >= رقم_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned صحیح) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      بازگشت hb_blob_get_empty ();

    unsigned صحیح glyph_offset = strike_offset + (unsigned صحیح) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned صحیح glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    ثابت SBIXGlyph *glyph = &(این+imageOffsetsZ[glyph_id]);

    اگر (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      اگر (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	اگر (retry_شمار--)
	  برو retry;
      }
      بازگشت hb_blob_get_empty ();
    }

    اگر (unlikely (file_type != glyph->graphicType))
      بازگشت hb_blob_get_empty ();

    اگر (strike_ppem) *strike_ppem = ppem;
    اگر (x_offset) *x_offset = glyph->xOffset;
    اگر (y_offset) *y_offset = glyph->yOffset;
    بازگشت hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  منطقی subset (hb_subset_context_t *c, unsigned صحیح available_len) ثابت
  {
    TRACE_SUBSET (این);
    unsigned صحیح رقم_output_glyphs = c->plan->رقم_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    اگر (unlikely (!c->serializer->extend (out, رقم_output_glyphs + 1))) return_trace (نادرست);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (رقم_output_glyphs + 1);

    منطقی has_glyphs = نادرست;
    برای (unsigned new_gid = 0; new_gid < رقم_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      اگر (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned صحیح) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	ادامه;
      }
      has_glyphs = درست;
      unsigned صحیح delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned صحیح glyph_data_length = delta - SBIXGlyph::min_size;
      اگر (!(این+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (نادرست);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    اگر (has_glyphs)
      out->imageOffsetsZ[رقم_output_glyphs] = head;
    وگرنه
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  عمومی:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  محافظتشده:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  عمومی:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

ساختار sbix
{
  ایستا constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  منطقی has_data () ثابت { بازگشت version; }

  ثابت SBIXStrike &get_strike (unsigned صحیح i) ثابت { بازگشت این+strikes[i]; }

  ساختار accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      رقم_glyphs = face->get_رقم_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    منطقی has_data () ثابت { بازگشت table->has_data (); }

    منطقی get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      منطقی                scale = درست) ثابت
    {
      /* We only support PNG right now, and following function checks type. */
      بازگشت get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      صحیح            *x_offset,
			      صحیح            *y_offset,
			      unsigned صحیح   *available_ppem) ثابت
    {
      بازگشت choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  رقم_glyphs, available_ppem);
    }

    منطقی paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, تهی *data) ثابت
    {
      اگر (!has_data ())
        بازگشت نادرست;

      صحیح x_offset = 0, y_offset = 0;
      unsigned صحیح strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      اگر (!font->get_glyph_extents (glyph, &extents, نادرست))
        بازگشت نادرست;

      اگر (unlikely (!get_extents (font, glyph, &pixel_extents, نادرست)))
        بازگشت نادرست;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      اگر (hb_blob_is_immutable (blob))
        بازگشت نادرست;

      منطقی ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      بازگشت ret;
    }

    خصوصی:

    ثابت SBIXStrike &choose_strike (hb_font_t *font) ثابت
    {
      unsigned شمار = table->strikes.len;
      اگر (unlikely (!شمار))
	بازگشت Null (SBIXStrike);

      unsigned صحیح requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      اگر (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned صحیح best_i = 0;
      unsigned صحیح best_ppem = table->get_strike (0).ppem;

      برای (unsigned صحیح i = 1; i < شمار; i++)
      {
	unsigned صحیح ppem = (table->get_strike (i)).ppem;
	اگر ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      بازگشت table->get_strike (best_i);
    }

    ساختار PNGHeader
    {
      HBUINT8	signature[8];
      ساختار
      {
	ساختار
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

      عمومی:
      DEFINE_SIZE_STATIC (29);
    };

    منطقی get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  منطقی                scale = درست) ثابت
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      اگر (!has_data ())
	بازگشت نادرست;

      صحیح x_offset = 0, y_offset = 0;
      unsigned صحیح strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ثابت PNGHeader &png = *blob->as<PNGHeader>();

      اگر (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	بازگشت نادرست;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned صحیح) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned صحیح) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned صحیح) png.IHDR.height;

      /* Convert to font units. */
      اگر (strike_ppem && scale)
      {
	اعشاری scale = font->face->get_upem () / (اعشاری) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      اگر (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      بازگشت strike_ppem;
    }

    خصوصی:
    hb_blob_ptr_t<sbix> table;

    unsigned صحیح رقم_glyphs;
  };

  منطقی sanitize (hb_sanitize_context_t *c) ثابت
  {
    TRACE_SANITIZE (این);
    return_trace (likely (c->check_struct (این) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, این)));
  }

  منطقی
  add_strike (hb_subset_context_t *c, unsigned i) ثابت
  {
    اگر (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      بازگشت نادرست;

    بازگشت (این+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  منطقی serialize_strike_offsets (hb_subset_context_t *c) ثابت
  {
    TRACE_SERIALIZE (این);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    اگر (unlikely (!c->serializer->extend_min (out))) return_trace (نادرست);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    برای (صحیح i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      اگر (unlikely (!o)) return_trace (نادرست);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      منطقی ret = add_strike (c, i);
      اگر (!ret)
      {
	c->serializer->برداشت_discard ();
	out->برداشت ();
	c->serializer->revert (snap);
      }
      وگرنه
      {
	objidxs.push (c->serializer->برداشت_pack ());
	new_strikes.push (o);
      }
    }
    برای (unsigned صحیح i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (درست);
  }

  منطقی subset (hb_subset_context_t* c) ثابت
  {
    TRACE_SUBSET (این);

    اگر (unlikely (!c->serializer->embed (این->version))) return_trace (نادرست);
    اگر (unlikely (!c->serializer->embed (این->flags))) return_trace (نادرست);

    return_trace (serialize_strike_offsets (c));
  }

  محافظتشده:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  عمومی:
  DEFINE_SIZE_ARRAY (8, strikes);
};

ساختار sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

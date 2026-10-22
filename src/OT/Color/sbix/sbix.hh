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

#অন্তর্ভাব "../../../hb-open-type.hh"
#অন্তর্ভাব "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


নামাকাশ OT {


সংরচনা SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned পূর্ণাঙ্ক data_length) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    যদি (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (8, data);
};

সংরচনা SBIXStrike
{
  স্থির size_t get_size (unsigned অঙ্ক_glyphs)
  { প্রত্যাগম min_size + অঙ্ক_glyphs * HBUINT32::static_size; }

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (c->check_struct (এতত্) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_অঙ্ক_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned পূর্ণাঙ্ক  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     পূর্ণাঙ্ক          *x_offset,
			     পূর্ণাঙ্ক          *y_offset,
			     unsigned পূর্ণাঙ্ক  অঙ্ক_glyphs,
			     unsigned পূর্ণাঙ্ক *strike_ppem) ধ্রুব
  {
    যদি (unlikely (!ppem)) প্রত্যাগম hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned পূর্ণাঙ্ক retry_গুনতি = 8;
    unsigned পূর্ণাঙ্ক sbix_len = sbix_blob->length;
    unsigned পূর্ণাঙ্ক strike_offset = (ধ্রুব বর্ণ *) এতত্ - (ধ্রুব বর্ণ *) sbix_blob->data;
    প্রতিজ্ঞা (strike_offset < sbix_len);

  retry:
    যদি (unlikely (glyph_id >= অঙ্ক_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned পূর্ণাঙ্ক) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      প্রত্যাগম hb_blob_get_empty ();

    unsigned পূর্ণাঙ্ক glyph_offset = strike_offset + (unsigned পূর্ণাঙ্ক) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned পূর্ণাঙ্ক glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    ধ্রুব SBIXGlyph *glyph = &(এতত্+imageOffsetsZ[glyph_id]);

    যদি (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      যদি (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	যদি (retry_গুনতি--)
	  গচ্ছ retry;
      }
      প্রত্যাগম hb_blob_get_empty ();
    }

    যদি (unlikely (file_type != glyph->graphicType))
      প্রত্যাগম hb_blob_get_empty ();

    যদি (strike_ppem) *strike_ppem = ppem;
    যদি (x_offset) *x_offset = glyph->xOffset;
    যদি (y_offset) *y_offset = glyph->yOffset;
    প্রত্যাগম hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  তার্কিক subset (hb_subset_context_t *c, unsigned পূর্ণাঙ্ক available_len) ধ্রুব
  {
    TRACE_SUBSET (এতত্);
    unsigned পূর্ণাঙ্ক অঙ্ক_output_glyphs = c->plan->অঙ্ক_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    যদি (unlikely (!c->serializer->extend (out, অঙ্ক_output_glyphs + 1))) return_trace (অসত্য);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (অঙ্ক_output_glyphs + 1);

    তার্কিক has_glyphs = অসত্য;
    প্রতি (unsigned new_gid = 0; new_gid < অঙ্ক_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      যদি (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned পূর্ণাঙ্ক) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	অনুবর্ত;
      }
      has_glyphs = সত্য;
      unsigned পূর্ণাঙ্ক delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned পূর্ণাঙ্ক glyph_data_length = delta - SBIXGlyph::min_size;
      যদি (!(এতত্+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (অসত্য);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    যদি (has_glyphs)
      out->imageOffsetsZ[অঙ্ক_output_glyphs] = head;
    অন্যথা
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  সার্বজনিক:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  রক্ষিত:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

সংরচনা sbix
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  তার্কিক has_data () ধ্রুব { প্রত্যাগম version; }

  ধ্রুব SBIXStrike &get_strike (unsigned পূর্ণাঙ্ক i) ধ্রুব { প্রত্যাগম এতত্+strikes[i]; }

  সংরচনা accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      অঙ্ক_glyphs = face->get_অঙ্ক_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    তার্কিক has_data () ধ্রুব { প্রত্যাগম table->has_data (); }

    তার্কিক get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      তার্কিক                scale = সত্য) ধ্রুব
    {
      /* We only support PNG right now, and following function checks type. */
      প্রত্যাগম get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      পূর্ণাঙ্ক            *x_offset,
			      পূর্ণাঙ্ক            *y_offset,
			      unsigned পূর্ণাঙ্ক   *available_ppem) ধ্রুব
    {
      প্রত্যাগম choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  অঙ্ক_glyphs, available_ppem);
    }

    তার্কিক paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, শূন্য *data) ধ্রুব
    {
      যদি (!has_data ())
        প্রত্যাগম অসত্য;

      পূর্ণাঙ্ক x_offset = 0, y_offset = 0;
      unsigned পূর্ণাঙ্ক strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      যদি (!font->get_glyph_extents (glyph, &extents, অসত্য))
        প্রত্যাগম অসত্য;

      যদি (unlikely (!get_extents (font, glyph, &pixel_extents, অসত্য)))
        প্রত্যাগম অসত্য;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      যদি (hb_blob_is_immutable (blob))
        প্রত্যাগম অসত্য;

      তার্কিক ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      প্রত্যাগম ret;
    }

    গুপ্ত:

    ধ্রুব SBIXStrike &choose_strike (hb_font_t *font) ধ্রুব
    {
      unsigned গুনতি = table->strikes.len;
      যদি (unlikely (!গুনতি))
	প্রত্যাগম Null (SBIXStrike);

      unsigned পূর্ণাঙ্ক requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      যদি (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned পূর্ণাঙ্ক best_i = 0;
      unsigned পূর্ণাঙ্ক best_ppem = table->get_strike (0).ppem;

      প্রতি (unsigned পূর্ণাঙ্ক i = 1; i < গুনতি; i++)
      {
	unsigned পূর্ণাঙ্ক ppem = (table->get_strike (i)).ppem;
	যদি ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      প্রত্যাগম table->get_strike (best_i);
    }

    সংরচনা PNGHeader
    {
      HBUINT8	signature[8];
      সংরচনা
      {
	সংরচনা
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

      সার্বজনিক:
      DEFINE_SIZE_STATIC (29);
    };

    তার্কিক get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  তার্কিক                scale = সত্য) ধ্রুব
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      যদি (!has_data ())
	প্রত্যাগম অসত্য;

      পূর্ণাঙ্ক x_offset = 0, y_offset = 0;
      unsigned পূর্ণাঙ্ক strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ধ্রুব PNGHeader &png = *blob->as<PNGHeader>();

      যদি (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	প্রত্যাগম অসত্য;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned পূর্ণাঙ্ক) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned পূর্ণাঙ্ক) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned পূর্ণাঙ্ক) png.IHDR.height;

      /* Convert to font units. */
      যদি (strike_ppem && scale)
      {
	ভিন্ন scale = font->face->get_upem () / (ভিন্ন) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      যদি (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      প্রত্যাগম strike_ppem;
    }

    গুপ্ত:
    hb_blob_ptr_t<sbix> table;

    unsigned পূর্ণাঙ্ক অঙ্ক_glyphs;
  };

  তার্কিক sanitize (hb_sanitize_context_t *c) ধ্রুব
  {
    TRACE_SANITIZE (এতত্);
    return_trace (likely (c->check_struct (এতত্) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, এতত্)));
  }

  তার্কিক
  add_strike (hb_subset_context_t *c, unsigned i) ধ্রুব
  {
    যদি (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      প্রত্যাগম অসত্য;

    প্রত্যাগম (এতত্+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  তার্কিক serialize_strike_offsets (hb_subset_context_t *c) ধ্রুব
  {
    TRACE_SERIALIZE (এতত্);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (অসত্য);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    প্রতি (পূর্ণাঙ্ক i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      যদি (unlikely (!o)) return_trace (অসত্য);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      তার্কিক ret = add_strike (c, i);
      যদি (!ret)
      {
	c->serializer->বের_করো_discard ();
	out->বের_করো ();
	c->serializer->revert (snap);
      }
      অন্যথা
      {
	objidxs.push (c->serializer->বের_করো_pack ());
	new_strikes.push (o);
      }
    }
    প্রতি (unsigned পূর্ণাঙ্ক i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (সত্য);
  }

  তার্কিক subset (hb_subset_context_t* c) ধ্রুব
  {
    TRACE_SUBSET (এতত্);

    যদি (unlikely (!c->serializer->embed (এতত্->version))) return_trace (অসত্য);
    যদি (unlikely (!c->serializer->embed (এতত্->flags))) return_trace (অসত্য);

    return_trace (serialize_strike_offsets (c));
  }

  রক্ষিত:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  সার্বজনিক:
  DEFINE_SIZE_ARRAY (8, strikes);
};

সংরচনা sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

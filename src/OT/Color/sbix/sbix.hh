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

#ಅನ್ತರ್ಭಾವ "../../../hb-open-type.hh"
#ಅನ್ತರ್ಭಾವ "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


ನಾಮಾಕಾಶ OT {


ಸಂರಚನಾ SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ data_length) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    ಯದಿ (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, data);
};

ಸಂರಚನಾ SBIXStrike
{
  ಸ್ಥಿರ size_t get_size (unsigned ಅಂಕೆ_glyphs)
  { ಪ್ರತ್ಯಾಗಮ min_size + ಅಂಕೆ_glyphs * HBUINT32::static_size; }

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (c->check_struct (ಏತತ್) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_ಅಂಕೆ_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned ಪೂರ್ಣಾಙ್ಕ  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     ಪೂರ್ಣಾಙ್ಕ          *x_offset,
			     ಪೂರ್ಣಾಙ್ಕ          *y_offset,
			     unsigned ಪೂರ್ಣಾಙ್ಕ  ಅಂಕೆ_glyphs,
			     unsigned ಪೂರ್ಣಾಙ್ಕ *strike_ppem) ಧ್ರುವ
  {
    ಯದಿ (unlikely (!ppem)) ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned ಪೂರ್ಣಾಙ್ಕ retry_ಎಣಿಕೆ = 8;
    unsigned ಪೂರ್ಣಾಙ್ಕ sbix_len = sbix_blob->length;
    unsigned ಪೂರ್ಣಾಙ್ಕ strike_offset = (ಧ್ರುವ ವರ್ಣ *) ಏತತ್ - (ಧ್ರುವ ವರ್ಣ *) sbix_blob->data;
    ಪ್ರತಿಜ್ಞಾ (strike_offset < sbix_len);

  retry:
    ಯದಿ (unlikely (glyph_id >= ಅಂಕೆ_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned ಪೂರ್ಣಾಙ್ಕ) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

    unsigned ಪೂರ್ಣಾಙ್ಕ glyph_offset = strike_offset + (unsigned ಪೂರ್ಣಾಙ್ಕ) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned ಪೂರ್ಣಾಙ್ಕ glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    ಧ್ರುವ SBIXGlyph *glyph = &(ಏತತ್+imageOffsetsZ[glyph_id]);

    ಯದಿ (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      ಯದಿ (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	ಯದಿ (retry_ಎಣಿಕೆ--)
	  ಗಚ್ಛ retry;
      }
      ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();
    }

    ಯದಿ (unlikely (file_type != glyph->graphicType))
      ಪ್ರತ್ಯಾಗಮ hb_blob_get_empty ();

    ಯದಿ (strike_ppem) *strike_ppem = ppem;
    ಯದಿ (x_offset) *x_offset = glyph->xOffset;
    ಯದಿ (y_offset) *y_offset = glyph->yOffset;
    ಪ್ರತ್ಯಾಗಮ hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t *c, unsigned ಪೂರ್ಣಾಙ್ಕ available_len) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);
    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_output_glyphs = c->plan->ಅಂಕೆ_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    ಯದಿ (unlikely (!c->serializer->extend (out, ಅಂಕೆ_output_glyphs + 1))) return_trace (ಅಸತ್ಯ);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (ಅಂಕೆ_output_glyphs + 1);

    ತಾರ್ಕಿಕ has_glyphs = ಅಸತ್ಯ;
    ಪ್ರತಿ (unsigned new_gid = 0; new_gid < ಅಂಕೆ_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ಯದಿ (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned ಪೂರ್ಣಾಙ್ಕ) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	ಅನುವರ್ತ;
      }
      has_glyphs = ಸತ್ಯ;
      unsigned ಪೂರ್ಣಾಙ್ಕ delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned ಪೂರ್ಣಾಙ್ಕ glyph_data_length = delta - SBIXGlyph::min_size;
      ಯದಿ (!(ಏತತ್+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (ಅಸತ್ಯ);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    ಯದಿ (has_glyphs)
      out->imageOffsetsZ[ಅಂಕೆ_output_glyphs] = head;
    ಅನ್ಯಥಾ
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  ಸಾರ್ವಜನಿಕ:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  ರಕ್ಷಿತ:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

ಸಂರಚನಾ sbix
{
  ಸ್ಥಿರ constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ version; }

  ಧ್ರುವ SBIXStrike &get_strike (unsigned ಪೂರ್ಣಾಙ್ಕ i) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಏತತ್+strikes[i]; }

  ಸಂರಚನಾ accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      ಅಂಕೆ_glyphs = face->get_ಅಂಕೆ_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    ತಾರ್ಕಿಕ has_data () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ table->has_data (); }

    ತಾರ್ಕಿಕ get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      ತಾರ್ಕಿಕ                scale = ಸತ್ಯ) ಧ್ರುವ
    {
      /* We only support PNG right now, and following function checks type. */
      ಪ್ರತ್ಯಾಗಮ get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      ಪೂರ್ಣಾಙ್ಕ            *x_offset,
			      ಪೂರ್ಣಾಙ್ಕ            *y_offset,
			      unsigned ಪೂರ್ಣಾಙ್ಕ   *available_ppem) ಧ್ರುವ
    {
      ಪ್ರತ್ಯಾಗಮ choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  ಅಂಕೆ_glyphs, available_ppem);
    }

    ತಾರ್ಕಿಕ paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ಶೂನ್ಯ *data) ಧ್ರುವ
    {
      ಯದಿ (!has_data ())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪೂರ್ಣಾಙ್ಕ x_offset = 0, y_offset = 0;
      unsigned ಪೂರ್ಣಾಙ್ಕ strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      ಯದಿ (!font->get_glyph_extents (glyph, &extents, ಅಸತ್ಯ))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಯದಿ (unlikely (!get_extents (font, glyph, &pixel_extents, ಅಸತ್ಯ)))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      ಯದಿ (hb_blob_is_immutable (blob))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ತಾರ್ಕಿಕ ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ಪ್ರತ್ಯಾಗಮ ret;
    }

    ಗುಪ್ತ:

    ಧ್ರುವ SBIXStrike &choose_strike (hb_font_t *font) ಧ್ರುವ
    {
      unsigned ಎಣಿಕೆ = table->strikes.len;
      ಯದಿ (unlikely (!ಎಣಿಕೆ))
	ಪ್ರತ್ಯಾಗಮ Null (SBIXStrike);

      unsigned ಪೂರ್ಣಾಙ್ಕ requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      ಯದಿ (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned ಪೂರ್ಣಾಙ್ಕ best_i = 0;
      unsigned ಪೂರ್ಣಾಙ್ಕ best_ppem = table->get_strike (0).ppem;

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 1; i < ಎಣಿಕೆ; i++)
      {
	unsigned ಪೂರ್ಣಾಙ್ಕ ppem = (table->get_strike (i)).ppem;
	ಯದಿ ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      ಪ್ರತ್ಯಾಗಮ table->get_strike (best_i);
    }

    ಸಂರಚನಾ PNGHeader
    {
      HBUINT8	signature[8];
      ಸಂರಚನಾ
      {
	ಸಂರಚನಾ
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

      ಸಾರ್ವಜನಿಕ:
      DEFINE_SIZE_STATIC (29);
    };

    ತಾರ್ಕಿಕ get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  ತಾರ್ಕಿಕ                scale = ಸತ್ಯ) ಧ್ರುವ
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      ಯದಿ (!has_data ())
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

      ಪೂರ್ಣಾಙ್ಕ x_offset = 0, y_offset = 0;
      unsigned ಪೂರ್ಣಾಙ್ಕ strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ಧ್ರುವ PNGHeader &png = *blob->as<PNGHeader>();

      ಯದಿ (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned ಪೂರ್ಣಾಙ್ಕ) png.IHDR.height;

      /* Convert to font units. */
      ಯದಿ (strike_ppem && scale)
      {
	ಭಿನ್ನ scale = font->face->get_upem () / (ಭಿನ್ನ) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      ಯದಿ (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      ಪ್ರತ್ಯಾಗಮ strike_ppem;
    }

    ಗುಪ್ತ:
    hb_blob_ptr_t<sbix> table;

    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_glyphs;
  };

  ತಾರ್ಕಿಕ sanitize (hb_sanitize_context_t *c) ಧ್ರುವ
  {
    TRACE_SANITIZE (ಏತತ್);
    return_trace (likely (c->check_struct (ಏತತ್) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, ಏತತ್)));
  }

  ತಾರ್ಕಿಕ
  add_strike (hb_subset_context_t *c, unsigned i) ಧ್ರುವ
  {
    ಯದಿ (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    ಪ್ರತ್ಯಾಗಮ (ಏತತ್+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  ತಾರ್ಕಿಕ serialize_strike_offsets (hb_subset_context_t *c) ಧ್ರುವ
  {
    TRACE_SERIALIZE (ಏತತ್);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    ಯದಿ (unlikely (!c->serializer->extend_min (out))) return_trace (ಅಸತ್ಯ);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      ಯದಿ (unlikely (!o)) return_trace (ಅಸತ್ಯ);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      ತಾರ್ಕಿಕ ret = add_strike (c, i);
      ಯದಿ (!ret)
      {
	c->serializer->ತೆಗೆ_discard ();
	out->ತೆಗೆ ();
	c->serializer->revert (snap);
      }
      ಅನ್ಯಥಾ
      {
	objidxs.push (c->serializer->ತೆಗೆ_pack ());
	new_strikes.push (o);
      }
    }
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (ಸತ್ಯ);
  }

  ತಾರ್ಕಿಕ subset (hb_subset_context_t* c) ಧ್ರುವ
  {
    TRACE_SUBSET (ಏತತ್);

    ಯದಿ (unlikely (!c->serializer->embed (ಏತತ್->version))) return_trace (ಅಸತ್ಯ);
    ಯದಿ (unlikely (!c->serializer->embed (ಏತತ್->flags))) return_trace (ಅಸತ್ಯ);

    return_trace (serialize_strike_offsets (c));
  }

  ರಕ್ಷಿತ:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  ಸಾರ್ವಜನಿಕ:
  DEFINE_SIZE_ARRAY (8, strikes);
};

ಸಂರಚನಾ sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

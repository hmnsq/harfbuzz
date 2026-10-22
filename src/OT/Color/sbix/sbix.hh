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

#अन्तर्भाव "../../../hb-open-type.hh"
#अन्तर्भाव "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


नामाकाश OT {


संरचना SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned पूर्णाङ्क data_length) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    यदि (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, data);
};

संरचना SBIXStrike
{
  स्थिर size_t get_size (unsigned अंक_glyphs)
  { प्रत्यागम min_size + अंक_glyphs * HBUINT32::static_size; }

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (c->check_struct (एतत्) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_अंक_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned पूर्णाङ्क  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     पूर्णाङ्क          *x_offset,
			     पूर्णाङ्क          *y_offset,
			     unsigned पूर्णाङ्क  अंक_glyphs,
			     unsigned पूर्णाङ्क *strike_ppem) ध्रुव
  {
    यदि (unlikely (!ppem)) प्रत्यागम hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned पूर्णाङ्क retry_मोजणी = 8;
    unsigned पूर्णाङ्क sbix_len = sbix_blob->length;
    unsigned पूर्णाङ्क strike_offset = (ध्रुव वर्ण *) एतत् - (ध्रुव वर्ण *) sbix_blob->data;
    प्रतिज्ञा (strike_offset < sbix_len);

  retry:
    यदि (unlikely (glyph_id >= अंक_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned पूर्णाङ्क) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      प्रत्यागम hb_blob_get_empty ();

    unsigned पूर्णाङ्क glyph_offset = strike_offset + (unsigned पूर्णाङ्क) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned पूर्णाङ्क glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    ध्रुव SBIXGlyph *glyph = &(एतत्+imageOffsetsZ[glyph_id]);

    यदि (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      यदि (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	यदि (retry_मोजणी--)
	  गच्छ retry;
      }
      प्रत्यागम hb_blob_get_empty ();
    }

    यदि (unlikely (file_type != glyph->graphicType))
      प्रत्यागम hb_blob_get_empty ();

    यदि (strike_ppem) *strike_ppem = ppem;
    यदि (x_offset) *x_offset = glyph->xOffset;
    यदि (y_offset) *y_offset = glyph->yOffset;
    प्रत्यागम hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  तार्किक subset (hb_subset_context_t *c, unsigned पूर्णाङ्क available_len) ध्रुव
  {
    TRACE_SUBSET (एतत्);
    unsigned पूर्णाङ्क अंक_output_glyphs = c->plan->अंक_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    यदि (unlikely (!c->serializer->extend (out, अंक_output_glyphs + 1))) return_trace (असत्य);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (अंक_output_glyphs + 1);

    तार्किक has_glyphs = असत्य;
    प्रति (unsigned new_gid = 0; new_gid < अंक_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      यदि (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned पूर्णाङ्क) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	अनुवर्त;
      }
      has_glyphs = सत्य;
      unsigned पूर्णाङ्क delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned पूर्णाङ्क glyph_data_length = delta - SBIXGlyph::min_size;
      यदि (!(एतत्+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (असत्य);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    यदि (has_glyphs)
      out->imageOffsetsZ[अंक_output_glyphs] = head;
    अन्यथा
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  सार्वजनिक:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  रक्षित:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

संरचना sbix
{
  स्थिर constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  तार्किक has_data () ध्रुव { प्रत्यागम version; }

  ध्रुव SBIXStrike &get_strike (unsigned पूर्णाङ्क i) ध्रुव { प्रत्यागम एतत्+strikes[i]; }

  संरचना accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      अंक_glyphs = face->get_अंक_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    तार्किक has_data () ध्रुव { प्रत्यागम table->has_data (); }

    तार्किक get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      तार्किक                scale = सत्य) ध्रुव
    {
      /* We only support PNG right now, and following function checks type. */
      प्रत्यागम get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      पूर्णाङ्क            *x_offset,
			      पूर्णाङ्क            *y_offset,
			      unsigned पूर्णाङ्क   *available_ppem) ध्रुव
    {
      प्रत्यागम choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  अंक_glyphs, available_ppem);
    }

    तार्किक paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, शून्य *data) ध्रुव
    {
      यदि (!has_data ())
        प्रत्यागम असत्य;

      पूर्णाङ्क x_offset = 0, y_offset = 0;
      unsigned पूर्णाङ्क strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      यदि (!font->get_glyph_extents (glyph, &extents, असत्य))
        प्रत्यागम असत्य;

      यदि (unlikely (!get_extents (font, glyph, &pixel_extents, असत्य)))
        प्रत्यागम असत्य;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      यदि (hb_blob_is_immutable (blob))
        प्रत्यागम असत्य;

      तार्किक ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      प्रत्यागम ret;
    }

    गुप्त:

    ध्रुव SBIXStrike &choose_strike (hb_font_t *font) ध्रुव
    {
      unsigned मोजणी = table->strikes.len;
      यदि (unlikely (!मोजणी))
	प्रत्यागम Null (SBIXStrike);

      unsigned पूर्णाङ्क requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      यदि (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned पूर्णाङ्क best_i = 0;
      unsigned पूर्णाङ्क best_ppem = table->get_strike (0).ppem;

      प्रति (unsigned पूर्णाङ्क i = 1; i < मोजणी; i++)
      {
	unsigned पूर्णाङ्क ppem = (table->get_strike (i)).ppem;
	यदि ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      प्रत्यागम table->get_strike (best_i);
    }

    संरचना PNGHeader
    {
      HBUINT8	signature[8];
      संरचना
      {
	संरचना
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

      सार्वजनिक:
      DEFINE_SIZE_STATIC (29);
    };

    तार्किक get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  तार्किक                scale = सत्य) ध्रुव
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      यदि (!has_data ())
	प्रत्यागम असत्य;

      पूर्णाङ्क x_offset = 0, y_offset = 0;
      unsigned पूर्णाङ्क strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ध्रुव PNGHeader &png = *blob->as<PNGHeader>();

      यदि (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	प्रत्यागम असत्य;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned पूर्णाङ्क) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned पूर्णाङ्क) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned पूर्णाङ्क) png.IHDR.height;

      /* Convert to font units. */
      यदि (strike_ppem && scale)
      {
	भिन्न scale = font->face->get_upem () / (भिन्न) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      यदि (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      प्रत्यागम strike_ppem;
    }

    गुप्त:
    hb_blob_ptr_t<sbix> table;

    unsigned पूर्णाङ्क अंक_glyphs;
  };

  तार्किक sanitize (hb_sanitize_context_t *c) ध्रुव
  {
    TRACE_SANITIZE (एतत्);
    return_trace (likely (c->check_struct (एतत्) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, एतत्)));
  }

  तार्किक
  add_strike (hb_subset_context_t *c, unsigned i) ध्रुव
  {
    यदि (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      प्रत्यागम असत्य;

    प्रत्यागम (एतत्+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  तार्किक serialize_strike_offsets (hb_subset_context_t *c) ध्रुव
  {
    TRACE_SERIALIZE (एतत्);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    यदि (unlikely (!c->serializer->extend_min (out))) return_trace (असत्य);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    प्रति (पूर्णाङ्क i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      यदि (unlikely (!o)) return_trace (असत्य);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      तार्किक ret = add_strike (c, i);
      यदि (!ret)
      {
	c->serializer->काढा_discard ();
	out->काढा ();
	c->serializer->revert (snap);
      }
      अन्यथा
      {
	objidxs.push (c->serializer->काढा_pack ());
	new_strikes.push (o);
      }
    }
    प्रति (unsigned पूर्णाङ्क i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (सत्य);
  }

  तार्किक subset (hb_subset_context_t* c) ध्रुव
  {
    TRACE_SUBSET (एतत्);

    यदि (unlikely (!c->serializer->embed (एतत्->version))) return_trace (असत्य);
    यदि (unlikely (!c->serializer->embed (एतत्->flags))) return_trace (असत्य);

    return_trace (serialize_strike_offsets (c));
  }

  रक्षित:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  सार्वजनिक:
  DEFINE_SIZE_ARRAY (8, strikes);
};

संरचना sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

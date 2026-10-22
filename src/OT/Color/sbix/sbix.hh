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

#अगर_अघोषित OT_COLOR_SBIX_SBIX_HH
#घोषणा OT_COLOR_SBIX_SBIX_HH

#समावेश "../../../hb-open-type.hh"
#समावेश "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#घोषणा HB_OT_TAG_sbix HB_TAG('s','b','i','x')


namespace OT {


काष्ठा SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, अचिन्हित पूर्णांक तथ्य_लंबाई) स्थिर
  {
    TRACE_SERIALIZE (यह);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    अगर (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

    new_glyph->xOffset = xOffset;
    new_glyph->yOffset = yOffset;
    new_glyph->graphicType = graphicType;
    तथ्य.copy (c, तथ्य_लंबाई);
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
		तथ्य;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  खुला:
  DEFINE_आकार_ARRAY (8, तथ्य);
};

काष्ठा SBIXStrike
{
  अटल माप_प्रकार get_आकार (अचिन्हित अंक_glyphs)
  { वापस min_आकार + अंक_glyphs * HBUINT32::static_आकार; }

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (c->check_struct (यह) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_अंक_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (अचिन्हित पूर्णांक  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     पूर्णांक          *x_offset,
			     पूर्णांक          *y_offset,
			     अचिन्हित पूर्णांक  अंक_glyphs,
			     अचिन्हित पूर्णांक *strike_ppem) स्थिर
  {
    अगर (unlikely (!ppem)) वापस hb_blob_get_empty (); /* To get Null() object out of the way. */

    अचिन्हित पूर्णांक retry_गिनती = 8;
    अचिन्हित पूर्णांक sbix_len = sbix_blob->लंबाई;
    अचिन्हित पूर्णांक strike_offset = (स्थिर अक्षर *) यह - (स्थिर अक्षर *) sbix_blob->तथ्य;
    निश्चित (strike_offset < sbix_len);

  retry:
    अगर (unlikely (glyph_id >= अंक_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_आकार ||
		  (अचिन्हित पूर्णांक) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      वापस hb_blob_get_empty ();

    अचिन्हित पूर्णांक glyph_offset = strike_offset + (अचिन्हित पूर्णांक) imageOffsetsZ[glyph_id] + SBIXGlyph::min_आकार;
    अचिन्हित पूर्णांक glyph_लंबाई = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_आकार;

    स्थिर SBIXGlyph *glyph = &(यह+imageOffsetsZ[glyph_id]);

    अगर (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      अगर (glyph_लंबाई >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->तथ्य);
	अगर (retry_गिनती--)
	  जाओ retry;
      }
      वापस hb_blob_get_empty ();
    }

    अगर (unlikely (file_type != glyph->graphicType))
      वापस hb_blob_get_empty ();

    अगर (strike_ppem) *strike_ppem = ppem;
    अगर (x_offset) *x_offset = glyph->xOffset;
    अगर (y_offset) *y_offset = glyph->yOffset;
    वापस hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_लंबाई);
  }

  bool subset (hb_subset_context_t *c, अचिन्हित पूर्णांक available_len) स्थिर
  {
    TRACE_SUBSET (यह);
    अचिन्हित पूर्णांक अंक_output_glyphs = c->plan->अंक_output_glyphs ();

    स्वतः* out = c->serializer->start_embed<SBIXStrike> ();
    स्वतः snap = c->serializer->snapshot ();
    अगर (unlikely (!c->serializer->extend (out, अंक_output_glyphs + 1))) return_trace (false);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 शीर्ष_2;
    शीर्ष_2 = get_आकार (अंक_output_glyphs + 1);

    bool has_glyphs = false;
    क्रम (अचिन्हित new_gid = 0; new_gid < अंक_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      अगर (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_आकार) ||
		    (अचिन्हित पूर्णांक) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = शीर्ष_2;
	जारी;
      }
      has_glyphs = true;
      अचिन्हित पूर्णांक delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      अचिन्हित पूर्णांक glyph_तथ्य_लंबाई = delta - SBIXGlyph::min_आकार;
      अगर (!(यह+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_तथ्य_लंबाई))
	return_trace (false);
      out->imageOffsetsZ[new_gid] = शीर्ष_2;
      शीर्ष_2 += delta;
    }
    अगर (has_glyphs)
      out->imageOffsetsZ[अंक_output_glyphs] = शीर्ष_2;
    अन्यथा
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  खुला:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  रक्षित:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  खुला:
  DEFINE_आकार_ARRAY (4, imageOffsetsZ);
};

काष्ठा sbix
{
  अटल constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  bool has_तथ्य () स्थिर { वापस version; }

  स्थिर SBIXStrike &get_strike (अचिन्हित पूर्णांक i) स्थिर { वापस यह+strikes[i]; }

  काष्ठा accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      अंक_glyphs = face->get_अंक_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    bool has_तथ्य () स्थिर { वापस table->has_तथ्य (); }

    bool get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      bool                scale = true) स्थिर
    {
      /* We only support PNG right now, and following function checks type. */
      वापस get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      पूर्णांक            *x_offset,
			      पूर्णांक            *y_offset,
			      अचिन्हित पूर्णांक   *available_ppem) स्थिर
    {
      वापस choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  अंक_glyphs, available_ppem);
    }

    bool paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, व्योम *तथ्य) स्थिर
    {
      अगर (!has_तथ्य ())
        वापस false;

      पूर्णांक x_offset = 0, y_offset = 0;
      अचिन्हित पूर्णांक strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      अगर (!font->get_glyph_extents (glyph, &extents, false))
        वापस false;

      अगर (unlikely (!get_extents (font, glyph, &pixel_extents, false)))
        वापस false;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      अगर (hb_blob_is_immutable (blob))
        वापस false;

      bool ret = funcs->image (तथ्य,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      वापस ret;
    }

    निजी:

    स्थिर SBIXStrike &choose_strike (hb_font_t *font) स्थिर
    {
      अचिन्हित गिनती = table->strikes.len;
      अगर (unlikely (!गिनती))
	वापस Null (SBIXStrike);

      अचिन्हित पूर्णांक requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      अगर (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      अचिन्हित पूर्णांक best_i = 0;
      अचिन्हित पूर्णांक best_ppem = table->get_strike (0).ppem;

      क्रम (अचिन्हित पूर्णांक i = 1; i < गिनती; i++)
      {
	अचिन्हित पूर्णांक ppem = (table->get_strike (i)).ppem;
	अगर ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      वापस table->get_strike (best_i);
    }

    काष्ठा PNGHeader
    {
      HBUINT8	signature[8];
      काष्ठा
      {
	काष्ठा
	{
	  HBUINT32	लंबाई;
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

      खुला:
      DEFINE_आकार_STATIC (29);
    };

    bool get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  bool                scale = true) स्थिर
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      अगर (!has_तथ्य ())
	वापस false;

      पूर्णांक x_offset = 0, y_offset = 0;
      अचिन्हित पूर्णांक strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      स्थिर PNGHeader &png = *blob->as<PNGHeader>();

      अगर (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	वापस false;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (अचिन्हित पूर्णांक) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (अचिन्हित पूर्णांक) png.IHDR.width;
      extents->height    = -(hb_position_t) (अचिन्हित पूर्णांक) png.IHDR.height;

      /* Convert to font units. */
      अगर (strike_ppem && scale)
      {
	भग्न scale = font->face->get_upem () / (भग्न) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      अगर (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      वापस strike_ppem;
    }

    निजी:
    hb_blob_ptr_t<sbix> table;

    अचिन्हित पूर्णांक अंक_glyphs;
  };

  bool sanitize (hb_sanitize_context_t *c) स्थिर
  {
    TRACE_SANITIZE (यह);
    return_trace (likely (c->check_struct (यह) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, यह)));
  }

  bool
  add_strike (hb_subset_context_t *c, अचिन्हित i) स्थिर
  {
    अगर (strikes[i].is_null () || c->source_blob->लंबाई < (अचिन्हित) strikes[i])
      वापस false;

    वापस (यह+strikes[i]).subset (c, c->source_blob->लंबाई - (अचिन्हित) strikes[i]);
  }

  bool serialize_strike_offsets (hb_subset_context_t *c) स्थिर
  {
    TRACE_SERIALIZE (यह);

    स्वतः *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    अगर (unlikely (!c->serializer->extend_min (out))) return_trace (false);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    क्रम (पूर्णांक i = strikes.len - 1; i >= 0; --i)
    {
      स्वतः* o = out->serialize_append (c->serializer);
      अगर (unlikely (!o)) return_trace (false);
      *o = 0;
      स्वतः snap = c->serializer->snapshot ();
      c->serializer->push ();
      bool ret = add_strike (c, i);
      अगर (!ret)
      {
	c->serializer->निकालो_discard ();
	out->निकालो ();
	c->serializer->revert (snap);
      }
      अन्यथा
      {
	objidxs.push (c->serializer->निकालो_pack ());
	new_strikes.push (o);
      }
    }
    क्रम (अचिन्हित पूर्णांक i = 0; i < new_strikes.लंबाई; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.लंबाई - 1 - i]);

    return_trace (true);
  }

  bool subset (hb_subset_context_t* c) स्थिर
  {
    TRACE_SUBSET (यह);

    अगर (unlikely (!c->serializer->embed (यह->version))) return_trace (false);
    अगर (unlikely (!c->serializer->embed (यह->flags))) return_trace (false);

    return_trace (serialize_strike_offsets (c));
  }

  रक्षित:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  खुला:
  DEFINE_आकार_ARRAY (8, strikes);
};

काष्ठा sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#पूर्ण_अगर /* OT_COLOR_SBIX_SBIX_HH */

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

#ଅନ୍ତର୍ଭାଵ "../../../hb-open-type.hh"
#ଅନ୍ତର୍ଭାଵ "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


ନାମାକାଶ OT {


ସଂରଚନା SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା data_length) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    ଯଦି (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, data);
};

ସଂରଚନା SBIXStrike
{
  ସ୍ଥିର size_t get_size (unsigned ଅଙ୍କ_glyphs)
  { ଫେରନ୍ତୁ min_size + ଅଙ୍କ_glyphs * HBUINT32::static_size; }

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (c->check_struct (ଏହି) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_ଅଙ୍କ_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା          *x_offset,
			     ପୂର୍ଣ୍ଣସଂଖ୍ୟା          *y_offset,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା  ଅଙ୍କ_glyphs,
			     unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା *strike_ppem) ଅଚଳ
  {
    ଯଦି (unlikely (!ppem)) ଫେରନ୍ତୁ hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା retry_ଗଣନା = 8;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା sbix_len = sbix_blob->length;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା strike_offset = (ଅଚଳ ଅକ୍ଷର *) ଏହି - (ଅଚଳ ଅକ୍ଷର *) sbix_blob->data;
    ପ୍ରତିଜ୍ଞା (strike_offset < sbix_len);

  retry:
    ଯଦି (unlikely (glyph_id >= ଅଙ୍କ_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      ଫେରନ୍ତୁ hb_blob_get_empty ();

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_offset = strike_offset + (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    ଅଚଳ SBIXGlyph *glyph = &(ଏହି+imageOffsetsZ[glyph_id]);

    ଯଦି (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      ଯଦି (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	ଯଦି (retry_ଗଣନା--)
	  ଯାଅ retry;
      }
      ଫେରନ୍ତୁ hb_blob_get_empty ();
    }

    ଯଦି (unlikely (file_type != glyph->graphicType))
      ଫେରନ୍ତୁ hb_blob_get_empty ();

    ଯଦି (strike_ppem) *strike_ppem = ppem;
    ଯଦି (x_offset) *x_offset = glyph->xOffset;
    ଯଦି (y_offset) *y_offset = glyph->yOffset;
    ଫେରନ୍ତୁ hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  ତାର୍କିକ subset (hb_subset_context_t *c, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା available_len) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_output_glyphs = c->plan->ଅଙ୍କ_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    ଯଦି (unlikely (!c->serializer->extend (out, ଅଙ୍କ_output_glyphs + 1))) return_trace (ମିଥ୍ୟା);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (ଅଙ୍କ_output_glyphs + 1);

    ତାର୍କିକ has_glyphs = ମିଥ୍ୟା;
    ପାଇଁ (unsigned new_gid = 0; new_gid < ଅଙ୍କ_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ଯଦି (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	ଜାରି;
      }
      has_glyphs = ସତ୍ୟ;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା glyph_data_length = delta - SBIXGlyph::min_size;
      ଯଦି (!(ଏହି+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (ମିଥ୍ୟା);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    ଯଦି (has_glyphs)
      out->imageOffsetsZ[ଅଙ୍କ_output_glyphs] = head;
    ଅନ୍ୟଥା
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  ସର୍ବସାଧାରଣ:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  ସୁରକ୍ଷିତ:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

ସଂରଚନା sbix
{
  ସ୍ଥିର constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ version; }

  ଅଚଳ SBIXStrike &get_strike (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i) ଅଚଳ { ଫେରନ୍ତୁ ଏହି+strikes[i]; }

  ସଂରଚନା accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      ଅଙ୍କ_glyphs = face->get_ଅଙ୍କ_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    ତାର୍କିକ has_data () ଅଚଳ { ଫେରନ୍ତୁ table->has_data (); }

    ତାର୍କିକ get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      ତାର୍କିକ                scale = ସତ୍ୟ) ଅଚଳ
    {
      /* We only support PNG right now, and following function checks type. */
      ଫେରନ୍ତୁ get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା            *x_offset,
			      ପୂର୍ଣ୍ଣସଂଖ୍ୟା            *y_offset,
			      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା   *available_ppem) ଅଚଳ
    {
      ଫେରନ୍ତୁ choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  ଅଙ୍କ_glyphs, available_ppem);
    }

    ତାର୍କିକ paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ଶୂନ୍ୟ *data) ଅଚଳ
    {
      ଯଦି (!has_data ())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_offset = 0, y_offset = 0;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      ଯଦି (!font->get_glyph_extents (glyph, &extents, ମିଥ୍ୟା))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ଯଦି (unlikely (!get_extents (font, glyph, &pixel_extents, ମିଥ୍ୟା)))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      ଯଦି (hb_blob_is_immutable (blob))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ତାର୍କିକ ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ଫେରନ୍ତୁ ret;
    }

    ବ୍ୟକ୍ତିଗତ:

    ଅଚଳ SBIXStrike &choose_strike (hb_font_t *font) ଅଚଳ
    {
      unsigned ଗଣନା_2 = table->strikes.len;
      ଯଦି (unlikely (!ଗଣନା_2))
	ଫେରନ୍ତୁ Null (SBIXStrike);

      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      ଯଦି (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_i = 0;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_ppem = table->get_strike (0).ppem;

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < ଗଣନା_2; i++)
      {
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ppem = (table->get_strike (i)).ppem;
	ଯଦି ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      ଫେରନ୍ତୁ table->get_strike (best_i);
    }

    ସଂରଚନା PNGHeader
    {
      HBUINT8	signature[8];
      ସଂରଚନା
      {
	ସଂରଚନା
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

      ସର୍ବସାଧାରଣ:
      DEFINE_SIZE_STATIC (29);
    };

    ତାର୍କିକ get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  ତାର୍କିକ                scale = ସତ୍ୟ) ଅଚଳ
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      ଯଦି (!has_data ())
	ଫେରନ୍ତୁ ମିଥ୍ୟା;

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା x_offset = 0, y_offset = 0;
      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ଅଚଳ PNGHeader &png = *blob->as<PNGHeader>();

      ଯଦି (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	ଫେରନ୍ତୁ ମିଥ୍ୟା;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା) png.IHDR.height;

      /* Convert to font units. */
      ଯଦି (strike_ppem && scale)
      {
	ଦଶମିକ scale = font->face->get_upem () / (ଦଶମିକ) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      ଯଦି (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      ଫେରନ୍ତୁ strike_ppem;
    }

    ବ୍ୟକ୍ତିଗତ:
    hb_blob_ptr_t<sbix> table;

    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_glyphs;
  };

  ତାର୍କିକ sanitize (hb_sanitize_context_t *c) ଅଚଳ
  {
    TRACE_SANITIZE (ଏହି);
    return_trace (likely (c->check_struct (ଏହି) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, ଏହି)));
  }

  ତାର୍କିକ
  add_strike (hb_subset_context_t *c, unsigned i) ଅଚଳ
  {
    ଯଦି (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      ଫେରନ୍ତୁ ମିଥ୍ୟା;

    ଫେରନ୍ତୁ (ଏହି+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  ତାର୍କିକ serialize_strike_offsets (hb_subset_context_t *c) ଅଚଳ
  {
    TRACE_SERIALIZE (ଏହି);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    ଯଦି (unlikely (!c->serializer->extend_min (out))) return_trace (ମିଥ୍ୟା);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      ଯଦି (unlikely (!o)) return_trace (ମିଥ୍ୟା);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      ତାର୍କିକ ret = add_strike (c, i);
      ଯଦି (!ret)
      {
	c->serializer->କାଢ଼_discard ();
	out->କାଢ଼ ();
	c->serializer->revert (snap);
      }
      ଅନ୍ୟଥା
      {
	objidxs.push (c->serializer->କାଢ଼_pack ());
	new_strikes.push (o);
      }
    }
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (ସତ୍ୟ);
  }

  ତାର୍କିକ subset (hb_subset_context_t* c) ଅଚଳ
  {
    TRACE_SUBSET (ଏହି);

    ଯଦି (unlikely (!c->serializer->embed (ଏହି->version))) return_trace (ମିଥ୍ୟା);
    ଯଦି (unlikely (!c->serializer->embed (ଏହି->flags))) return_trace (ମିଥ୍ୟା);

    return_trace (serialize_strike_offsets (c));
  }

  ସୁରକ୍ଷିତ:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  ସର୍ବସାଧାରଣ:
  DEFINE_SIZE_ARRAY (8, strikes);
};

ସଂରଚନା sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

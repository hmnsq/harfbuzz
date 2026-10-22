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

#அந்தர்பாவ "../../../hb-open-type.hh"
#அந்தர்பாவ "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


நாமாகாஶ OT {


கட்டமைப்பு SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned முழு data_length) மாறிலி
  {
    TRACE_SERIALIZE (இது);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    எனில் (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  பொது:
  DEFINE_SIZE_ARRAY (8, data);
};

கட்டமைப்பு SBIXStrike
{
  நிலையான size_t get_size (unsigned இலக்கம்_glyphs)
  { திரும்பு min_size + இலக்கம்_glyphs * HBUINT32::static_size; }

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (c->check_struct (இது) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_இலக்கம்_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned முழு  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     முழு          *x_offset,
			     முழு          *y_offset,
			     unsigned முழு  இலக்கம்_glyphs,
			     unsigned முழு *strike_ppem) மாறிலி
  {
    எனில் (unlikely (!ppem)) திரும்பு hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned முழு retry_மொத்தம் = 8;
    unsigned முழு sbix_len = sbix_blob->length;
    unsigned முழு strike_offset = (மாறிலி எழுத்து *) இது - (மாறிலி எழுத்து *) sbix_blob->data;
    ப்ரதிஜ்ஞா (strike_offset < sbix_len);

  retry:
    எனில் (unlikely (glyph_id >= இலக்கம்_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned முழு) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      திரும்பு hb_blob_get_empty ();

    unsigned முழு glyph_offset = strike_offset + (unsigned முழு) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned முழு glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    மாறிலி SBIXGlyph *glyph = &(இது+imageOffsetsZ[glyph_id]);

    எனில் (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      எனில் (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	எனில் (retry_மொத்தம்--)
	  செல் retry;
      }
      திரும்பு hb_blob_get_empty ();
    }

    எனில் (unlikely (file_type != glyph->graphicType))
      திரும்பு hb_blob_get_empty ();

    எனில் (strike_ppem) *strike_ppem = ppem;
    எனில் (x_offset) *x_offset = glyph->xOffset;
    எனில் (y_offset) *y_offset = glyph->yOffset;
    திரும்பு hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  தருக்க subset (hb_subset_context_t *c, unsigned முழு available_len) மாறிலி
  {
    TRACE_SUBSET (இது);
    unsigned முழு இலக்கம்_output_glyphs = c->plan->இலக்கம்_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    எனில் (unlikely (!c->serializer->extend (out, இலக்கம்_output_glyphs + 1))) return_trace (பொய்);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (இலக்கம்_output_glyphs + 1);

    தருக்க has_glyphs = பொய்;
    க்கு (unsigned new_gid = 0; new_gid < இலக்கம்_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      எனில் (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned முழு) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	தொடர்;
      }
      has_glyphs = உண்மை;
      unsigned முழு delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned முழு glyph_data_length = delta - SBIXGlyph::min_size;
      எனில் (!(இது+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (பொய்);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    எனில் (has_glyphs)
      out->imageOffsetsZ[இலக்கம்_output_glyphs] = head;
    இல்லையெனில்
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  பொது:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  பாதுகாக்கப்பட்ட:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  பொது:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

கட்டமைப்பு sbix
{
  நிலையான constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  தருக்க has_data () மாறிலி { திரும்பு version; }

  மாறிலி SBIXStrike &get_strike (unsigned முழு i) மாறிலி { திரும்பு இது+strikes[i]; }

  கட்டமைப்பு accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      இலக்கம்_glyphs = face->get_இலக்கம்_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    தருக்க has_data () மாறிலி { திரும்பு table->has_data (); }

    தருக்க get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      தருக்க                scale = உண்மை) மாறிலி
    {
      /* We only support PNG right now, and following function checks type. */
      திரும்பு get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      முழு            *x_offset,
			      முழு            *y_offset,
			      unsigned முழு   *available_ppem) மாறிலி
    {
      திரும்பு choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  இலக்கம்_glyphs, available_ppem);
    }

    தருக்க paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, வெறுமை *data) மாறிலி
    {
      எனில் (!has_data ())
        திரும்பு பொய்;

      முழு x_offset = 0, y_offset = 0;
      unsigned முழு strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      எனில் (!font->get_glyph_extents (glyph, &extents, பொய்))
        திரும்பு பொய்;

      எனில் (unlikely (!get_extents (font, glyph, &pixel_extents, பொய்)))
        திரும்பு பொய்;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      எனில் (hb_blob_is_immutable (blob))
        திரும்பு பொய்;

      தருக்க ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      திரும்பு ret;
    }

    தனிப்பட்ட:

    மாறிலி SBIXStrike &choose_strike (hb_font_t *font) மாறிலி
    {
      unsigned மொத்தம் = table->strikes.len;
      எனில் (unlikely (!மொத்தம்))
	திரும்பு Null (SBIXStrike);

      unsigned முழு requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      எனில் (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned முழு best_i = 0;
      unsigned முழு best_ppem = table->get_strike (0).ppem;

      க்கு (unsigned முழு i = 1; i < மொத்தம்; i++)
      {
	unsigned முழு ppem = (table->get_strike (i)).ppem;
	எனில் ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      திரும்பு table->get_strike (best_i);
    }

    கட்டமைப்பு PNGHeader
    {
      HBUINT8	signature[8];
      கட்டமைப்பு
      {
	கட்டமைப்பு
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

      பொது:
      DEFINE_SIZE_STATIC (29);
    };

    தருக்க get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  தருக்க                scale = உண்மை) மாறிலி
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      எனில் (!has_data ())
	திரும்பு பொய்;

      முழு x_offset = 0, y_offset = 0;
      unsigned முழு strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      மாறிலி PNGHeader &png = *blob->as<PNGHeader>();

      எனில் (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	திரும்பு பொய்;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned முழு) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned முழு) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned முழு) png.IHDR.height;

      /* Convert to font units. */
      எனில் (strike_ppem && scale)
      {
	தசம scale = font->face->get_upem () / (தசம) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      எனில் (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      திரும்பு strike_ppem;
    }

    தனிப்பட்ட:
    hb_blob_ptr_t<sbix> table;

    unsigned முழு இலக்கம்_glyphs;
  };

  தருக்க sanitize (hb_sanitize_context_t *c) மாறிலி
  {
    TRACE_SANITIZE (இது);
    return_trace (likely (c->check_struct (இது) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, இது)));
  }

  தருக்க
  add_strike (hb_subset_context_t *c, unsigned i) மாறிலி
  {
    எனில் (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      திரும்பு பொய்;

    திரும்பு (இது+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  தருக்க serialize_strike_offsets (hb_subset_context_t *c) மாறிலி
  {
    TRACE_SERIALIZE (இது);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    எனில் (unlikely (!c->serializer->extend_min (out))) return_trace (பொய்);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    க்கு (முழு i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      எனில் (unlikely (!o)) return_trace (பொய்);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      தருக்க ret = add_strike (c, i);
      எனில் (!ret)
      {
	c->serializer->எடு_discard ();
	out->எடு ();
	c->serializer->revert (snap);
      }
      இல்லையெனில்
      {
	objidxs.push (c->serializer->எடு_pack ());
	new_strikes.push (o);
      }
    }
    க்கு (unsigned முழு i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (உண்மை);
  }

  தருக்க subset (hb_subset_context_t* c) மாறிலி
  {
    TRACE_SUBSET (இது);

    எனில் (unlikely (!c->serializer->embed (இது->version))) return_trace (பொய்);
    எனில் (unlikely (!c->serializer->embed (இது->flags))) return_trace (பொய்);

    return_trace (serialize_strike_offsets (c));
  }

  பாதுகாக்கப்பட்ட:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  பொது:
  DEFINE_SIZE_ARRAY (8, strikes);
};

கட்டமைப்பு sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

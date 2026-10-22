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

#അന്തര്ഭാവ "../../../hb-open-type.hh"
#അന്തര്ഭാവ "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#define HB_OT_TAG_sbix HB_TAG('s','b','i','x')


നാമാകാശ OT {


ഘടന SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned പൂർണ്ണസംഖ്യ data_length) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    എങ്കിൽ (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

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
  പൊതു:
  DEFINE_SIZE_ARRAY (8, data);
};

ഘടന SBIXStrike
{
  സ്ഥിതം size_t get_size (unsigned അക്കം_glyphs)
  { തിരികെ min_size + അക്കം_glyphs * HBUINT32::static_size; }

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (c->check_struct (ഇത്) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_അക്കം_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned പൂർണ്ണസംഖ്യ  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     പൂർണ്ണസംഖ്യ          *x_offset,
			     പൂർണ്ണസംഖ്യ          *y_offset,
			     unsigned പൂർണ്ണസംഖ്യ  അക്കം_glyphs,
			     unsigned പൂർണ്ണസംഖ്യ *strike_ppem) സ്ഥിരം
  {
    എങ്കിൽ (unlikely (!ppem)) തിരികെ hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned പൂർണ്ണസംഖ്യ retry_എണ്ണം = 8;
    unsigned പൂർണ്ണസംഖ്യ sbix_len = sbix_blob->length;
    unsigned പൂർണ്ണസംഖ്യ strike_offset = (സ്ഥിരം അക്ഷരം *) ഇത് - (സ്ഥിരം അക്ഷരം *) sbix_blob->data;
    പ്രതിജ്ഞാ (strike_offset < sbix_len);

  retry:
    എങ്കിൽ (unlikely (glyph_id >= അക്കം_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_size ||
		  (unsigned പൂർണ്ണസംഖ്യ) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      തിരികെ hb_blob_get_empty ();

    unsigned പൂർണ്ണസംഖ്യ glyph_offset = strike_offset + (unsigned പൂർണ്ണസംഖ്യ) imageOffsetsZ[glyph_id] + SBIXGlyph::min_size;
    unsigned പൂർണ്ണസംഖ്യ glyph_length = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_size;

    സ്ഥിരം SBIXGlyph *glyph = &(ഇത്+imageOffsetsZ[glyph_id]);

    എങ്കിൽ (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      എങ്കിൽ (glyph_length >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->data);
	എങ്കിൽ (retry_എണ്ണം--)
	  പോകുക retry;
      }
      തിരികെ hb_blob_get_empty ();
    }

    എങ്കിൽ (unlikely (file_type != glyph->graphicType))
      തിരികെ hb_blob_get_empty ();

    എങ്കിൽ (strike_ppem) *strike_ppem = ppem;
    എങ്കിൽ (x_offset) *x_offset = glyph->xOffset;
    എങ്കിൽ (y_offset) *y_offset = glyph->yOffset;
    തിരികെ hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_length);
  }

  തർക്കം subset (hb_subset_context_t *c, unsigned പൂർണ്ണസംഖ്യ available_len) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);
    unsigned പൂർണ്ണസംഖ്യ അക്കം_output_glyphs = c->plan->അക്കം_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    എങ്കിൽ (unlikely (!c->serializer->extend (out, അക്കം_output_glyphs + 1))) return_trace (അസത്യം);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 head;
    head = get_size (അക്കം_output_glyphs + 1);

    തർക്കം has_glyphs = അസത്യം;
    വേണ്ടി (unsigned new_gid = 0; new_gid < അക്കം_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      എങ്കിൽ (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_size) ||
		    (unsigned പൂർണ്ണസംഖ്യ) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = head;
	തുടരുക;
      }
      has_glyphs = സത്യം;
      unsigned പൂർണ്ണസംഖ്യ delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned പൂർണ്ണസംഖ്യ glyph_data_length = delta - SBIXGlyph::min_size;
      എങ്കിൽ (!(ഇത്+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_data_length))
	return_trace (അസത്യം);
      out->imageOffsetsZ[new_gid] = head;
      head += delta;
    }
    എങ്കിൽ (has_glyphs)
      out->imageOffsetsZ[അക്കം_output_glyphs] = head;
    അല്ലെങ്കിൽ
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  പൊതു:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  സംരക്ഷിത:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  പൊതു:
  DEFINE_SIZE_ARRAY (4, imageOffsetsZ);
};

ഘടന sbix
{
  സ്ഥിതം constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  തർക്കം has_data () സ്ഥിരം { തിരികെ version; }

  സ്ഥിരം SBIXStrike &get_strike (unsigned പൂർണ്ണസംഖ്യ i) സ്ഥിരം { തിരികെ ഇത്+strikes[i]; }

  ഘടന accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      അക്കം_glyphs = face->get_അക്കം_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    തർക്കം has_data () സ്ഥിരം { തിരികെ table->has_data (); }

    തർക്കം get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      തർക്കം                scale = സത്യം) സ്ഥിരം
    {
      /* We only support PNG right now, and following function checks type. */
      തിരികെ get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      പൂർണ്ണസംഖ്യ            *x_offset,
			      പൂർണ്ണസംഖ്യ            *y_offset,
			      unsigned പൂർണ്ണസംഖ്യ   *available_ppem) സ്ഥിരം
    {
      തിരികെ choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  അക്കം_glyphs, available_ppem);
    }

    തർക്കം paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, ശൂന്യം *data) സ്ഥിരം
    {
      എങ്കിൽ (!has_data ())
        തിരികെ അസത്യം;

      പൂർണ്ണസംഖ്യ x_offset = 0, y_offset = 0;
      unsigned പൂർണ്ണസംഖ്യ strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      എങ്കിൽ (!font->get_glyph_extents (glyph, &extents, അസത്യം))
        തിരികെ അസത്യം;

      എങ്കിൽ (unlikely (!get_extents (font, glyph, &pixel_extents, അസത്യം)))
        തിരികെ അസത്യം;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      എങ്കിൽ (hb_blob_is_immutable (blob))
        തിരികെ അസത്യം;

      തർക്കം ret = funcs->image (data,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      തിരികെ ret;
    }

    സ്വകാര്യ:

    സ്ഥിരം SBIXStrike &choose_strike (hb_font_t *font) സ്ഥിരം
    {
      unsigned എണ്ണം = table->strikes.len;
      എങ്കിൽ (unlikely (!എണ്ണം))
	തിരികെ Null (SBIXStrike);

      unsigned പൂർണ്ണസംഖ്യ requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      എങ്കിൽ (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned പൂർണ്ണസംഖ്യ best_i = 0;
      unsigned പൂർണ്ണസംഖ്യ best_ppem = table->get_strike (0).ppem;

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 1; i < എണ്ണം; i++)
      {
	unsigned പൂർണ്ണസംഖ്യ ppem = (table->get_strike (i)).ppem;
	എങ്കിൽ ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      തിരികെ table->get_strike (best_i);
    }

    ഘടന PNGHeader
    {
      HBUINT8	signature[8];
      ഘടന
      {
	ഘടന
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

      പൊതു:
      DEFINE_SIZE_STATIC (29);
    };

    തർക്കം get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  തർക്കം                scale = സത്യം) സ്ഥിരം
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      എങ്കിൽ (!has_data ())
	തിരികെ അസത്യം;

      പൂർണ്ണസംഖ്യ x_offset = 0, y_offset = 0;
      unsigned പൂർണ്ണസംഖ്യ strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      സ്ഥിരം PNGHeader &png = *blob->as<PNGHeader>();

      എങ്കിൽ (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	തിരികെ അസത്യം;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned പൂർണ്ണസംഖ്യ) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned പൂർണ്ണസംഖ്യ) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned പൂർണ്ണസംഖ്യ) png.IHDR.height;

      /* Convert to font units. */
      എങ്കിൽ (strike_ppem && scale)
      {
	ദശാംശം scale = font->face->get_upem () / (ദശാംശം) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      എങ്കിൽ (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      തിരികെ strike_ppem;
    }

    സ്വകാര്യ:
    hb_blob_ptr_t<sbix> table;

    unsigned പൂർണ്ണസംഖ്യ അക്കം_glyphs;
  };

  തർക്കം sanitize (hb_sanitize_context_t *c) സ്ഥിരം
  {
    TRACE_SANITIZE (ഇത്);
    return_trace (likely (c->check_struct (ഇത്) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, ഇത്)));
  }

  തർക്കം
  add_strike (hb_subset_context_t *c, unsigned i) സ്ഥിരം
  {
    എങ്കിൽ (strikes[i].is_null () || c->source_blob->length < (unsigned) strikes[i])
      തിരികെ അസത്യം;

    തിരികെ (ഇത്+strikes[i]).subset (c, c->source_blob->length - (unsigned) strikes[i]);
  }

  തർക്കം serialize_strike_offsets (hb_subset_context_t *c) സ്ഥിരം
  {
    TRACE_SERIALIZE (ഇത്);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    എങ്കിൽ (unlikely (!c->serializer->extend_min (out))) return_trace (അസത്യം);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      എങ്കിൽ (unlikely (!o)) return_trace (അസത്യം);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      തർക്കം ret = add_strike (c, i);
      എങ്കിൽ (!ret)
      {
	c->serializer->എടുക്കുക_discard ();
	out->എടുക്കുക ();
	c->serializer->revert (snap);
      }
      അല്ലെങ്കിൽ
      {
	objidxs.push (c->serializer->എടുക്കുക_pack ());
	new_strikes.push (o);
      }
    }
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < new_strikes.length; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.length - 1 - i]);

    return_trace (സത്യം);
  }

  തർക്കം subset (hb_subset_context_t* c) സ്ഥിരം
  {
    TRACE_SUBSET (ഇത്);

    എങ്കിൽ (unlikely (!c->serializer->embed (ഇത്->version))) return_trace (അസത്യം);
    എങ്കിൽ (unlikely (!c->serializer->embed (ഇത്->flags))) return_trace (അസത്യം);

    return_trace (serialize_strike_offsets (c));
  }

  സംരക്ഷിത:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  പൊതു:
  DEFINE_SIZE_ARRAY (8, strikes);
};

ഘടന sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

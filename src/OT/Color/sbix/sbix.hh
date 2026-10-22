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


কাঠামো SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, unsigned পূর্ণসংখ্যা তথ্য_দৈর্ঘ্য) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);
    SBIXGlyph* new_glyph = c->start_embed<SBIXGlyph> ();
    যদি (unlikely (!c->extend_min (new_glyph))) return_trace (nullptr);

    new_glyph->xOffset = xOffset;
    new_glyph->yOffset = yOffset;
    new_glyph->graphicType = graphicType;
    তথ্য.copy (c, তথ্য_দৈর্ঘ্য);
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
		তথ্য;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, তথ্য);
};

কাঠামো SBIXStrike
{
  স্থির size_t get_আকার (unsigned অঙ্ক_glyphs)
  { ফেরত min_আকার + অঙ্ক_glyphs * HBUINT32::static_আকার; }

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (c->check_struct (এই) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_অঙ্ক_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (unsigned পূর্ণসংখ্যা  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_tag_t      file_type,
			     পূর্ণসংখ্যা          *x_offset,
			     পূর্ণসংখ্যা          *y_offset,
			     unsigned পূর্ণসংখ্যা  অঙ্ক_glyphs,
			     unsigned পূর্ণসংখ্যা *strike_ppem) ধ্রুবক
  {
    যদি (unlikely (!ppem)) ফেরত hb_blob_get_empty (); /* To get Null() object out of the way. */

    unsigned পূর্ণসংখ্যা retry_গুনতি = 8;
    unsigned পূর্ণসংখ্যা sbix_len = sbix_blob->দৈর্ঘ্য;
    unsigned পূর্ণসংখ্যা strike_offset = (ধ্রুবক অক্ষর *) এই - (ধ্রুবক অক্ষর *) sbix_blob->তথ্য;
    প্রতিজ্ঞা (strike_offset < sbix_len);

  retry:
    যদি (unlikely (glyph_id >= অঙ্ক_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_আকার ||
		  (unsigned পূর্ণসংখ্যা) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_offset))
      ফেরত hb_blob_get_empty ();

    unsigned পূর্ণসংখ্যা glyph_offset = strike_offset + (unsigned পূর্ণসংখ্যা) imageOffsetsZ[glyph_id] + SBIXGlyph::min_আকার;
    unsigned পূর্ণসংখ্যা glyph_দৈর্ঘ্য = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_আকার;

    ধ্রুবক SBIXGlyph *glyph = &(এই+imageOffsetsZ[glyph_id]);

    যদি (glyph->graphicType == HB_TAG ('d','u','p','e'))
    {
      যদি (glyph_দৈর্ঘ্য >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->তথ্য);
	যদি (retry_গুনতি--)
	  যাও retry;
      }
      ফেরত hb_blob_get_empty ();
    }

    যদি (unlikely (file_type != glyph->graphicType))
      ফেরত hb_blob_get_empty ();

    যদি (strike_ppem) *strike_ppem = ppem;
    যদি (x_offset) *x_offset = glyph->xOffset;
    যদি (y_offset) *y_offset = glyph->yOffset;
    ফেরত hb_blob_create_sub_blob (sbix_blob, glyph_offset, glyph_দৈর্ঘ্য);
  }

  যৌক্তিক subset (hb_subset_context_t *c, unsigned পূর্ণসংখ্যা available_len) ধ্রুবক
  {
    TRACE_SUBSET (এই);
    unsigned পূর্ণসংখ্যা অঙ্ক_output_glyphs = c->plan->অঙ্ক_output_glyphs ();

    auto* out = c->serializer->start_embed<SBIXStrike> ();
    auto snap = c->serializer->snapshot ();
    যদি (unlikely (!c->serializer->extend (out, অঙ্ক_output_glyphs + 1))) return_trace (মিথ্যা);
    out->ppem = ppem;
    out->resolution = resolution;
    HBUINT32 শীর্ষ_2;
    শীর্ষ_2 = get_আকার (অঙ্ক_output_glyphs + 1);

    যৌক্তিক has_glyphs = মিথ্যা;
    জন্য (unsigned new_gid = 0; new_gid < অঙ্ক_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      যদি (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_আকার) ||
		    (unsigned পূর্ণসংখ্যা) imageOffsetsZ[old_gid + 1] > available_len)
      {
	out->imageOffsetsZ[new_gid] = শীর্ষ_2;
	চালু;
      }
      has_glyphs = সত্য;
      unsigned পূর্ণসংখ্যা delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      unsigned পূর্ণসংখ্যা glyph_তথ্য_দৈর্ঘ্য = delta - SBIXGlyph::min_আকার;
      যদি (!(এই+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_তথ্য_দৈর্ঘ্য))
	return_trace (মিথ্যা);
      out->imageOffsetsZ[new_gid] = শীর্ষ_2;
      শীর্ষ_2 += delta;
    }
    যদি (has_glyphs)
      out->imageOffsetsZ[অঙ্ক_output_glyphs] = শীর্ষ_2;
    অন্যথা
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  সর্বজনীন:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  সুরক্ষিত:
  UnsizedArrayOf<Offset32To<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (4, imageOffsetsZ);
};

কাঠামো sbix
{
  স্থির constexpr hb_tag_t tableTag = HB_OT_TAG_sbix;

  যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত version; }

  ধ্রুবক SBIXStrike &get_strike (unsigned পূর্ণসংখ্যা i) ধ্রুবক { ফেরত এই+strikes[i]; }

  কাঠামো accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      table = hb_sanitize_context_t ().reference_table<sbix> (face);
      অঙ্ক_glyphs = face->get_অঙ্ক_glyphs ();
    }
    ~accelerator_t () { table.destroy (); }

    যৌক্তিক has_তথ্য () ধ্রুবক { ফেরত table->has_তথ্য (); }

    যৌক্তিক get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      যৌক্তিক                scale = সত্য) ধ্রুবক
    {
      /* We only support PNG right now, and following function checks type. */
      ফেরত get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      পূর্ণসংখ্যা            *x_offset,
			      পূর্ণসংখ্যা            *y_offset,
			      unsigned পূর্ণসংখ্যা   *available_ppem) ধ্রুবক
    {
      ফেরত choose_strike (font).get_glyph_blob (glyph_id, table.get_blob (),
						  HB_TAG ('p','n','g',' '),
						  x_offset, y_offset,
						  অঙ্ক_glyphs, available_ppem);
    }

    যৌক্তিক paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, শূন্য *তথ্য) ধ্রুবক
    {
      যদি (!has_তথ্য ())
        ফেরত মিথ্যা;

      পূর্ণসংখ্যা x_offset = 0, y_offset = 0;
      unsigned পূর্ণসংখ্যা strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      যদি (!font->get_glyph_extents (glyph, &extents, মিথ্যা))
        ফেরত মিথ্যা;

      যদি (unlikely (!get_extents (font, glyph, &pixel_extents, মিথ্যা)))
        ফেরত মিথ্যা;

      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);
      যদি (hb_blob_is_immutable (blob))
        ফেরত মিথ্যা;

      যৌক্তিক ret = funcs->image (তথ্য,
			       blob,
			       pixel_extents.width, -pixel_extents.height,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      ফেরত ret;
    }

    ব্যক্তিগত:

    ধ্রুবক SBIXStrike &choose_strike (hb_font_t *font) ধ্রুবক
    {
      unsigned গুনতি = table->strikes.len;
      যদি (unlikely (!গুনতি))
	ফেরত Null (SBIXStrike);

      unsigned পূর্ণসংখ্যা requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      যদি (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      unsigned পূর্ণসংখ্যা best_i = 0;
      unsigned পূর্ণসংখ্যা best_ppem = table->get_strike (0).ppem;

      জন্য (unsigned পূর্ণসংখ্যা i = 1; i < গুনতি; i++)
      {
	unsigned পূর্ণসংখ্যা ppem = (table->get_strike (i)).ppem;
	যদি ((requested_ppem <= ppem && ppem < best_ppem) ||
	    (requested_ppem > best_ppem && ppem > best_ppem))
	{
	  best_i = i;
	  best_ppem = ppem;
	}
      }

      ফেরত table->get_strike (best_i);
    }

    কাঠামো PNGHeader
    {
      HBUINT8	signature[8];
      কাঠামো
      {
	কাঠামো
	{
	  HBUINT32	দৈর্ঘ্য;
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

      সর্বজনীন:
      DEFINE_আকার_STATIC (29);
    };

    যৌক্তিক get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  যৌক্তিক                scale = সত্য) ধ্রুবক
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      যদি (!has_তথ্য ())
	ফেরত মিথ্যা;

      পূর্ণসংখ্যা x_offset = 0, y_offset = 0;
      unsigned পূর্ণসংখ্যা strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_offset, &y_offset, &strike_ppem);

      ধ্রুবক PNGHeader &png = *blob->as<PNGHeader>();

      যদি (png.IHDR.height >= 65536 || png.IHDR.width >= 65536)
      {
	hb_blob_destroy (blob);
	ফেরত মিথ্যা;
      }

      extents->x_bearing = x_offset;
      extents->y_bearing = hb_clamp_to<hb_position_t> ((int64_t) (unsigned পূর্ণসংখ্যা) png.IHDR.height + y_offset);
      extents->width     = (hb_position_t) (unsigned পূর্ণসংখ্যা) png.IHDR.width;
      extents->height    = -(hb_position_t) (unsigned পূর্ণসংখ্যা) png.IHDR.height;

      /* Convert to font units. */
      যদি (strike_ppem && scale)
      {
	দশমিক scale = font->face->get_upem () / (দশমিক) strike_ppem;
	extents->x_bearing = hb_clamp_to<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_to<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->width = hb_clamp_to<hb_position_t> (roundf (extents->width * scale));
	extents->height = hb_clamp_to<hb_position_t> (roundf (extents->height * scale));
      }

      যদি (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      ফেরত strike_ppem;
    }

    ব্যক্তিগত:
    hb_blob_ptr_t<sbix> table;

    unsigned পূর্ণসংখ্যা অঙ্ক_glyphs;
  };

  যৌক্তিক sanitize (hb_sanitize_context_t *c) ধ্রুবক
  {
    TRACE_SANITIZE (এই);
    return_trace (likely (c->check_struct (এই) &&
			  hb_barrier () &&
			  version >= 1 &&
			  strikes.sanitize (c, এই)));
  }

  যৌক্তিক
  add_strike (hb_subset_context_t *c, unsigned i) ধ্রুবক
  {
    যদি (strikes[i].is_null () || c->source_blob->দৈর্ঘ্য < (unsigned) strikes[i])
      ফেরত মিথ্যা;

    ফেরত (এই+strikes[i]).subset (c, c->source_blob->দৈর্ঘ্য - (unsigned) strikes[i]);
  }

  যৌক্তিক serialize_strike_offsets (hb_subset_context_t *c) ধ্রুবক
  {
    TRACE_SERIALIZE (এই);

    auto *out = c->serializer->start_embed<Array32OfOffset32To<SBIXStrike>> ();
    যদি (unlikely (!c->serializer->extend_min (out))) return_trace (মিথ্যা);

    hb_vector_t<Offset32To<SBIXStrike>*> new_strikes;
    hb_vector_t<hb_serialize_context_t::objidx_t> objidxs;
    জন্য (পূর্ণসংখ্যা i = strikes.len - 1; i >= 0; --i)
    {
      auto* o = out->serialize_append (c->serializer);
      যদি (unlikely (!o)) return_trace (মিথ্যা);
      *o = 0;
      auto snap = c->serializer->snapshot ();
      c->serializer->push ();
      যৌক্তিক ret = add_strike (c, i);
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
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < new_strikes.দৈর্ঘ্য; ++i)
      c->serializer->add_link (*new_strikes[i], objidxs[new_strikes.দৈর্ঘ্য - 1 - i]);

    return_trace (সত্য);
  }

  যৌক্তিক subset (hb_subset_context_t* c) ধ্রুবক
  {
    TRACE_SUBSET (এই);

    যদি (unlikely (!c->serializer->embed (এই->version))) return_trace (মিথ্যা);
    যদি (unlikely (!c->serializer->embed (এই->flags))) return_trace (মিথ্যা);

    return_trace (serialize_strike_offsets (c));
  }

  সুরক্ষিত:
  HBUINT16	version;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32OfOffset32To<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  সর্বজনীন:
  DEFINE_আকার_ARRAY (8, strikes);
};

কাঠামো sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#endif /* OT_COLOR_SBIX_SBIX_HH */

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

#నిర్వచితం_కాకపోతే OT_COLOR_SBIX_SBIX_HH
#నిర్వచించు OT_COLOR_SBIX_SBIX_HH

#అన్తర్భావ "../../../hb-open-type.hh"
#అన్తర్భావ "../../../hb-paint.hh"

/*
 * sbix -- Standard Bitmap Graphics
 * https://docs.microsoft.com/en-us/typography/opentype/spec/sbix
 * https://developer.apple.com/fonts/TrueType-Reference-Manual/RM06/Chap6sbix.html
 */
#నిర్వచించు HB_OT_ట్యాగ్_sbix HB_ట్యాగ్('s','b','i','x')


నామాకాశ OT {


నిర్మాణం SBIXGlyph
{
  SBIXGlyph* copy (hb_serialize_context_t *c, గుర్తులేని పూర్ణాంకం దత్తాంశం_నిడివి) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);
    SBIXGlyph* new_glyph = c->ప్రారంభం_embed<SBIXGlyph> ();
    ఒకవేళ (unlikely (!c->extend_min (new_glyph))) return_trace (శూన్యసూచిక);

    new_glyph->x_స్థానభ్రంశం = x_స్థానభ్రంశం;
    new_glyph->y_స్థానభ్రంశం = y_స్థానభ్రంశం;
    new_glyph->graphicType = graphicType;
    దత్తాంశం.copy (c, దత్తాంశం_నిడివి);
    return_trace (new_glyph);
  }

  HBINT16	x_స్థానభ్రంశం;	/* The horizontal (x-axis) offset from the left
				 * edge of the graphic to the glyph’s origin.
				 * That is, the x-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  HBINT16	y_స్థానభ్రంశం;	/* The vertical (y-axis) offset from the bottom
				 * edge of the graphic to the glyph’s origin.
				 * That is, the y-coordinate of the point on the
				 * baseline at the left edge of the glyph. */
  ట్యాగ్		graphicType;	/* Indicates the format of the embedded graphic
				 * data: one of 'jpg ', 'png ' or 'tiff', or the
				 * special format 'dupe'. */
  Unsized_Array_యొక్క<HBUINT8>
		దత్తాంశం;		/* The actual embedded graphic data. The total
				 * length is inferred from sequential entries in
				 * the glyphDataOffsets array and the fixed size
				 * (8 bytes) of the preceding fields. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, దత్తాంశం);
};

నిర్మాణం SBIXStrike
{
  స్థిర size_t get_కొలత (గుర్తులేని అంకె_glyphs)
  { తిరిగి min_కొలత + అంకె_glyphs * HBUINT32::static_కొలత; }

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (c->check_struct (ఇది) &&
		  imageOffsetsZ.sanitize_shallow (c, c->get_అంకె_glyphs () + 1));
  }

  hb_blob_t *get_glyph_blob (గుర్తులేని పూర్ణాంకం  glyph_id,
			     hb_blob_t    *sbix_blob,
			     hb_ట్యాగ్_t      file_type,
			     పూర్ణాంకం          *x_స్థానభ్రంశం_2,
			     పూర్ణాంకం          *y_స్థానభ్రంశం_2,
			     గుర్తులేని పూర్ణాంకం  అంకె_glyphs,
			     గుర్తులేని పూర్ణాంకం *strike_ppem) స్థిరరాశి
  {
    ఒకవేళ (unlikely (!ppem)) తిరిగి hb_blob_get_ఖాళీలు (); /* To get Null() object out of the way. */

    గుర్తులేని పూర్ణాంకం retry_లెక్క = 8;
    గుర్తులేని పూర్ణాంకం sbix_len = sbix_blob->నిడివి;
    గుర్తులేని పూర్ణాంకం strike_స్థానభ్రంశం = (స్థిరరాశి అక్షరం *) ఇది - (స్థిరరాశి అక్షరం *) sbix_blob->దత్తాంశం;
    ప్రతిజ్ఞా (strike_స్థానభ్రంశం < sbix_len);

  retry:
    ఒకవేళ (unlikely (glyph_id >= అంకె_glyphs ||
		  imageOffsetsZ[glyph_id + 1] <= imageOffsetsZ[glyph_id] ||
		  imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] <= SBIXGlyph::min_కొలత ||
		  (గుర్తులేని పూర్ణాంకం) imageOffsetsZ[glyph_id + 1] > sbix_len - strike_స్థానభ్రంశం))
      తిరిగి hb_blob_get_ఖాళీలు ();

    గుర్తులేని పూర్ణాంకం glyph_స్థానభ్రంశం = strike_స్థానభ్రంశం + (గుర్తులేని పూర్ణాంకం) imageOffsetsZ[glyph_id] + SBIXGlyph::min_కొలత;
    గుర్తులేని పూర్ణాంకం glyph_నిడివి = imageOffsetsZ[glyph_id + 1] - imageOffsetsZ[glyph_id] - SBIXGlyph::min_కొలత;

    స్థిరరాశి SBIXGlyph *glyph = &(ఇది+imageOffsetsZ[glyph_id]);

    ఒకవేళ (glyph->graphicType == HB_ట్యాగ్ ('d','u','p','e'))
    {
      ఒకవేళ (glyph_నిడివి >= 2)
      {
	glyph_id = *((HBUINT16 *) &glyph->దత్తాంశం);
	ఒకవేళ (retry_లెక్క--)
	  వెళ్ళు retry;
      }
      తిరిగి hb_blob_get_ఖాళీలు ();
    }

    ఒకవేళ (unlikely (file_type != glyph->graphicType))
      తిరిగి hb_blob_get_ఖాళీలు ();

    ఒకవేళ (strike_ppem) *strike_ppem = ppem;
    ఒకవేళ (x_స్థానభ్రంశం_2) *x_స్థానభ్రంశం_2 = glyph->x_స్థానభ్రంశం;
    ఒకవేళ (y_స్థానభ్రంశం_2) *y_స్థానభ్రంశం_2 = glyph->y_స్థానభ్రంశం;
    తిరిగి hb_blob_create_sub_blob (sbix_blob, glyph_స్థానభ్రంశం, glyph_నిడివి);
  }

  తార్కిక subset (hb_subset_context_t *c, గుర్తులేని పూర్ణాంకం అందుబాటు_len) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);
    గుర్తులేని పూర్ణాంకం అంకె_output_glyphs = c->plan->అంకె_output_glyphs ();

    స్వయంచాలక* బహిర్గతం = c->serializer->ప్రారంభం_embed<SBIXStrike> ();
    స్వయంచాలక snap = c->serializer->snapshot ();
    ఒకవేళ (unlikely (!c->serializer->extend (బహిర్గతం, అంకె_output_glyphs + 1))) return_trace (అబద్ధం);
    బహిర్గతం->ppem = ppem;
    బహిర్గతం->resolution = resolution;
    HBUINT32 తల_2;
    తల_2 = get_కొలత (అంకె_output_glyphs + 1);

    తార్కిక has_glyphs = అబద్ధం;
    కొరకు (గుర్తులేని new_gid = 0; new_gid < అంకె_output_glyphs; new_gid++)
    {
      hb_codepoint_t old_gid;
      ఒకవేళ (!c->plan->old_gid_for_new_gid (new_gid, &old_gid) ||
	  unlikely (imageOffsetsZ[old_gid].is_null () ||
		    imageOffsetsZ[old_gid + 1].is_null () ||
		    imageOffsetsZ[old_gid + 1] <= imageOffsetsZ[old_gid] ||
		    imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid] <= SBIXGlyph::min_కొలత) ||
		    (గుర్తులేని పూర్ణాంకం) imageOffsetsZ[old_gid + 1] > అందుబాటు_len)
      {
	బహిర్గతం->imageOffsetsZ[new_gid] = తల_2;
	కొనసాగు;
      }
      has_glyphs = నిజం;
      గుర్తులేని పూర్ణాంకం delta = imageOffsetsZ[old_gid + 1] - imageOffsetsZ[old_gid];
      గుర్తులేని పూర్ణాంకం glyph_దత్తాంశం_నిడివి = delta - SBIXGlyph::min_కొలత;
      ఒకవేళ (!(ఇది+imageOffsetsZ[old_gid]).copy (c->serializer, glyph_దత్తాంశం_నిడివి))
	return_trace (అబద్ధం);
      బహిర్గతం->imageOffsetsZ[new_gid] = తల_2;
      తల_2 += delta;
    }
    ఒకవేళ (has_glyphs)
      బహిర్గతం->imageOffsetsZ[అంకె_output_glyphs] = తల_2;
    లేకపోతే
      c->serializer->revert (snap);
    return_trace (has_glyphs);
  }

  బహిరంగ:
  HBUINT16	ppem;		/* The PPEM size for which this strike was designed. */
  HBUINT16	resolution;	/* The device pixel density (in PPI) for which this
				 * strike was designed. (E.g., 96 PPI, 192 PPI.) */
  రక్షిత:
  Unsized_Array_యొక్క<Offset32_గమ్యం<SBIXGlyph>>
		imageOffsetsZ;	/* Offset from the beginning of the strike data header
				 * to bitmap data for an individual glyph ID. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (4, imageOffsetsZ);
};

నిర్మాణం sbix
{
  స్థిర constexpr hb_ట్యాగ్_t పట్టిక_ట్యాగ్ = HB_OT_ట్యాగ్_sbix;

  తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి సంస్కరణ; }

  స్థిరరాశి SBIXStrike &get_strike (గుర్తులేని పూర్ణాంకం i) స్థిరరాశి { తిరిగి ఇది+strikes[i]; }

  నిర్మాణం accelerator_t
  {
    accelerator_t (hb_face_t *face)
    {
      పట్టిక = hb_sanitize_context_t ().reference_పట్టిక<sbix> (face);
      అంకె_glyphs = face->get_అంకె_glyphs ();
    }
    ~accelerator_t () { పట్టిక.destroy (); }

    తార్కిక has_దత్తాంశం () స్థిరరాశి { తిరిగి పట్టిక->has_దత్తాంశం (); }

    తార్కిక get_extents (hb_font_t          *font,
		      hb_codepoint_t      glyph,
		      hb_glyph_extents_t *extents,
		      తార్కిక                scale = నిజం) స్థిరరాశి
    {
      /* We only support PNG right now, and following function checks type. */
      తిరిగి get_png_extents (font, glyph, extents, scale);
    }

    hb_blob_t *reference_png (hb_font_t      *font,
			      hb_codepoint_t  glyph_id,
			      పూర్ణాంకం            *x_స్థానభ్రంశం_2,
			      పూర్ణాంకం            *y_స్థానభ్రంశం_2,
			      గుర్తులేని పూర్ణాంకం   *అందుబాటు_ppem) స్థిరరాశి
    {
      తిరిగి ఎంచుకో_strike (font).get_glyph_blob (glyph_id, పట్టిక.get_blob (),
						  HB_ట్యాగ్ ('p','n','g',' '),
						  x_స్థానభ్రంశం_2, y_స్థానభ్రంశం_2,
						  అంకె_glyphs, అందుబాటు_ppem);
    }

    తార్కిక paint_glyph (hb_font_t *font, hb_codepoint_t glyph, hb_paint_funcs_t *funcs, శూన్య *దత్తాంశం) స్థిరరాశి
    {
      ఒకవేళ (!has_దత్తాంశం ())
        తిరిగి అబద్ధం;

      పూర్ణాంకం x_స్థానభ్రంశం_2 = 0, y_స్థానభ్రంశం_2 = 0;
      గుర్తులేని పూర్ణాంకం strike_ppem = 0;
      hb_glyph_extents_t extents;
      hb_glyph_extents_t pixel_extents;

      ఒకవేళ (!font->get_glyph_extents (glyph, &extents, అబద్ధం))
        తిరిగి అబద్ధం;

      ఒకవేళ (unlikely (!get_extents (font, glyph, &pixel_extents, అబద్ధం)))
        తిరిగి అబద్ధం;

      hb_blob_t *blob = reference_png (font, glyph, &x_స్థానభ్రంశం_2, &y_స్థానభ్రంశం_2, &strike_ppem);
      ఒకవేళ (hb_blob_is_immutable (blob))
        తిరిగి అబద్ధం;

      తార్కిక ret = funcs->image (దత్తాంశం,
			       blob,
			       pixel_extents.వెడల్పు, -pixel_extents.ఎత్తు,
			       HB_PAINT_IMAGE_FORMAT_PNG,
			       0.f,
			       &extents);

      hb_blob_destroy (blob);

      తిరిగి ret;
    }

    అంతరంగ:

    స్థిరరాశి SBIXStrike &ఎంచుకో_strike (hb_font_t *font) స్థిరరాశి
    {
      గుర్తులేని లెక్క = పట్టిక->strikes.len;
      ఒకవేళ (unlikely (!లెక్క))
	తిరిగి Null (SBIXStrike);

      గుర్తులేని పూర్ణాంకం requested_ppem = hb_max (font->x_ppem, font->y_ppem);
      ఒకవేళ (!requested_ppem)
	requested_ppem = 1<<30; /* Choose largest strike. */
      /* TODO Add DPI sensitivity as well? */
      గుర్తులేని పూర్ణాంకం ఉత్తమ_i = 0;
      గుర్తులేని పూర్ణాంకం ఉత్తమ_ppem = పట్టిక->get_strike (0).ppem;

      కొరకు (గుర్తులేని పూర్ణాంకం i = 1; i < లెక్క; i++)
      {
	గుర్తులేని పూర్ణాంకం ppem = (పట్టిక->get_strike (i)).ppem;
	ఒకవేళ ((requested_ppem <= ppem && ppem < ఉత్తమ_ppem) ||
	    (requested_ppem > ఉత్తమ_ppem && ppem > ఉత్తమ_ppem))
	{
	  ఉత్తమ_i = i;
	  ఉత్తమ_ppem = ppem;
	}
      }

      తిరిగి పట్టిక->get_strike (ఉత్తమ_i);
    }

    నిర్మాణం PNGHeader
    {
      HBUINT8	signature[8];
      నిర్మాణం
      {
	నిర్మాణం
	{
	  HBUINT32	నిడివి;
	  ట్యాగ్		type;
	}		శీర్షిక;
	HBUINT32	వెడల్పు;
	HBUINT32	ఎత్తు;
	HBUINT8		బిట్టు_లోతు;
	HBUINT8		colorType;
	HBUINT8		compression_పద్ధతి;
	HBUINT8		filter_పద్ధతి;
	HBUINT8		interlace_పద్ధతి;
      } IHDR;

      బహిరంగ:
      DEFINE_కొలత_STATIC (29);
    };

    తార్కిక get_png_extents (hb_font_t          *font,
			  hb_codepoint_t      glyph,
			  hb_glyph_extents_t *extents,
			  తార్కిక                scale = నిజం) స్థిరరాశి
    {
      /* Following code is safe to call even without data.
       * But faster to short-circuit. */
      ఒకవేళ (!has_దత్తాంశం ())
	తిరిగి అబద్ధం;

      పూర్ణాంకం x_స్థానభ్రంశం_2 = 0, y_స్థానభ్రంశం_2 = 0;
      గుర్తులేని పూర్ణాంకం strike_ppem = 0;
      hb_blob_t *blob = reference_png (font, glyph, &x_స్థానభ్రంశం_2, &y_స్థానభ్రంశం_2, &strike_ppem);

      స్థిరరాశి PNGHeader &png = *blob->as<PNGHeader>();

      ఒకవేళ (png.IHDR.ఎత్తు >= 65536 || png.IHDR.వెడల్పు >= 65536)
      {
	hb_blob_destroy (blob);
	తిరిగి అబద్ధం;
      }

      extents->x_bearing = x_స్థానభ్రంశం_2;
      extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> ((int64_t) (గుర్తులేని పూర్ణాంకం) png.IHDR.ఎత్తు + y_స్థానభ్రంశం_2);
      extents->వెడల్పు     = (hb_position_t) (గుర్తులేని పూర్ణాంకం) png.IHDR.వెడల్పు;
      extents->ఎత్తు    = -(hb_position_t) (గుర్తులేని పూర్ణాంకం) png.IHDR.ఎత్తు;

      /* Convert to font units. */
      ఒకవేళ (strike_ppem && scale)
      {
	దశాంశం scale = font->face->get_upem () / (దశాంశం) strike_ppem;
	extents->x_bearing = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->x_bearing * scale));
	extents->y_bearing = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->y_bearing * scale));
	extents->వెడల్పు = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->వెడల్పు * scale));
	extents->ఎత్తు = hb_clamp_గమ్యం<hb_position_t> (roundf (extents->ఎత్తు * scale));
      }

      ఒకవేళ (scale)
	font->scale_glyph_extents (extents);

      hb_blob_destroy (blob);

      తిరిగి strike_ppem;
    }

    అంతరంగ:
    hb_blob_ptr_t<sbix> పట్టిక;

    గుర్తులేని పూర్ణాంకం అంకె_glyphs;
  };

  తార్కిక sanitize (hb_sanitize_context_t *c) స్థిరరాశి
  {
    TRACE_SANITIZE (ఇది);
    return_trace (likely (c->check_struct (ఇది) &&
			  hb_barrier () &&
			  సంస్కరణ >= 1 &&
			  strikes.sanitize (c, ఇది)));
  }

  తార్కిక
  కూడు_strike (hb_subset_context_t *c, గుర్తులేని i) స్థిరరాశి
  {
    ఒకవేళ (strikes[i].is_null () || c->ఆధారం_blob->నిడివి < (గుర్తులేని) strikes[i])
      తిరిగి అబద్ధం;

    తిరిగి (ఇది+strikes[i]).subset (c, c->ఆధారం_blob->నిడివి - (గుర్తులేని) strikes[i]);
  }

  తార్కిక serialize_strike_offsets (hb_subset_context_t *c) స్థిరరాశి
  {
    TRACE_SERIALIZE (ఇది);

    స్వయంచాలక *బహిర్గతం = c->serializer->ప్రారంభం_embed<Array32_యొక్క_Offset32_గమ్యం<SBIXStrike>> ();
    ఒకవేళ (unlikely (!c->serializer->extend_min (బహిర్గతం))) return_trace (అబద్ధం);

    hb_సదిశ_t<Offset32_గమ్యం<SBIXStrike>*> new_strikes;
    hb_సదిశ_t<hb_serialize_context_t::objidx_t> objidxs;
    కొరకు (పూర్ణాంకం i = strikes.len - 1; i >= 0; --i)
    {
      స్వయంచాలక* o = బహిర్గతం->serialize_append (c->serializer);
      ఒకవేళ (unlikely (!o)) return_trace (అబద్ధం);
      *o = 0;
      స్వయంచాలక snap = c->serializer->snapshot ();
      c->serializer->push ();
      తార్కిక ret = కూడు_strike (c, i);
      ఒకవేళ (!ret)
      {
	c->serializer->తీయి_discard ();
	బహిర్గతం->తీయి ();
	c->serializer->revert (snap);
      }
      లేకపోతే
      {
	objidxs.push (c->serializer->తీయి_pack ());
	new_strikes.push (o);
      }
    }
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < new_strikes.నిడివి; ++i)
      c->serializer->కూడు_link (*new_strikes[i], objidxs[new_strikes.నిడివి - 1 - i]);

    return_trace (నిజం);
  }

  తార్కిక subset (hb_subset_context_t* c) స్థిరరాశి
  {
    TRACE_SUBSET (ఇది);

    ఒకవేళ (unlikely (!c->serializer->embed (ఇది->సంస్కరణ))) return_trace (అబద్ధం);
    ఒకవేళ (unlikely (!c->serializer->embed (ఇది->flags))) return_trace (అబద్ధం);

    return_trace (serialize_strike_offsets (c));
  }

  రక్షిత:
  HBUINT16	సంస్కరణ;	/* Table version number — set to 1 */
  HBUINT16	flags;		/* Bit 0: Set to 1. Bit 1: Draw outlines.
				 * Bits 2 to 15: reserved (set to 0). */
  Array32_యొక్క_Offset32_గమ్యం<SBIXStrike>
		strikes;	/* Offsets from the beginning of the 'sbix'
				 * table to data for each individual bitmap strike. */
  బహిరంగ:
  DEFINE_కొలత_ARRAY (8, strikes);
};

నిర్మాణం sbix_accelerator_t : sbix::accelerator_t {
  sbix_accelerator_t (hb_face_t *face) : sbix::accelerator_t (face) {}
};


} /* namespace OT */

#అయితే_ముగింపు /* OT_COLOR_SBIX_SBIX_HH */

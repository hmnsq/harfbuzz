/*
 * Copyright © 2026  Behdad Esfahbod
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
 * Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-vector-paint.hh"
#అన్తర్భావ "hb-vector-draw.hh"
#అన్తర్భావ "hb-paint.hh"

#అన్తర్భావ <math.h>
#అన్తర్భావ <stdio.h>

#నిర్వచితమైతే HAVE_ZLIB
#అన్తర్భావ <zlib.h>
#అయితే_ముగింపు

/* PDF paint backend for COLRv0/v1 color font rendering.
 *
 * Supports:
 *   - Solid colors with alpha (via ExtGState)
 *   - Glyph and rectangle clipping
 *   - Affine transforms
 *   - Groups (save/restore)
 *   - Linear gradients (PDF Type 2 axial shading)
 *   - Radial gradients (PDF Type 3 radial shading)
 *   - Sweep gradients (approximated via solid fallback)
 */


/* ---- PDF object collector ---- */

నిర్మాణం hb_pdf_obj_t
{
  hb_సదిశ_buf_t దత్తాంశం;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
స్థిర తార్కిక
hb_pdf_build_సూచీకృత_smask (hb_సదిశ_buf_t *బహిర్గతం,
			    స్థిరరాశి అక్షరం *idat_దత్తాంశం, గుర్తులేని idat_len,
			    గుర్తులేని వెడల్పు, గుర్తులేని ఎత్తు,
			    స్థిరరాశి uint8_t *trns, గుర్తులేని trns_len);

నిర్మాణం hb_pdf_resources_t
{
  hb_సదిశ_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_సదిశ_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_సదిశ_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_సదిశ_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  గుర్తులేని extgstate_లెక్క = 0;
  గుర్తులేని shading_లెక్క = 0;
  గుర్తులేని xobject_లెక్క = 0;

  గుర్తులేని కూడు_object (hb_సదిశ_buf_t &&obj_దత్తాంశం)
  {
    గుర్తులేని id = 5 + objects.నిడివి; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.దత్తాంశం = std::move (obj_దత్తాంశం);
    objects.push (std::move (obj));
    తిరిగి id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  గుర్తులేని కూడు_extgstate_alpha (దశాంశం alpha)
  {
    గుర్తులేని idx = extgstate_లెక్క++;
    hb_సదిశ_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_అంకె (alpha, 4);
    obj.append_str (" >>");
    గుర్తులేని obj_id = కూడు_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    తిరిగి idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  గుర్తులేని కూడు_extgstate_blend (స్థిరరాశి అక్షరం *bm)
  {
    గుర్తులేని idx = extgstate_లెక్క++;
    hb_సదిశ_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    గుర్తులేని obj_id = కూడు_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    తిరిగి idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  గుర్తులేని కూడు_extgstate_smask (గుర్తులేని alpha_shading_id,
				దశాంశం bbox_x, దశాంశం bbox_y,
				దశాంశం bbox_w, దశాంశం bbox_h,
				గుర్తులేని precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_సదిశ_buf_t form_ప్రవాహం;
    form_ప్రవాహం.append_str ("/SHa sh\n");

    hb_సదిశ_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_అంకె (bbox_x, precision);
    form.append_c (' ');
    form.append_అంకె (bbox_y, precision);
    form.append_c (' ');
    form.append_అంకె (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_అంకె (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_ప్రవాహం.నిడివి);
    form.append_str (" >>\nstream\n");
    form.append_len (form_ప్రవాహం.arrayZ, form_ప్రవాహం.నిడివి);
    form.append_str ("endstream");
    గుర్తులేని form_id = కూడు_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    గుర్తులేని idx = extgstate_లెక్క++;
    hb_సదిశ_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    గుర్తులేని gs_id = కూడు_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    తిరిగి idx;
  }

  /* Add a shading, return resource name index. */
  గుర్తులేని కూడు_shading (hb_సదిశ_buf_t &&shading_దత్తాంశం)
  {
    గుర్తులేని obj_id = కూడు_object (std::move (shading_దత్తాంశం));
    తిరిగి కూడు_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  గుర్తులేని కూడు_shading_by_id (గుర్తులేని obj_id)
  {
    గుర్తులేని idx = shading_లెక్క++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    తిరిగి idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  గుర్తులేని కూడు_xobject_png_image (స్థిరరాశి అక్షరం *idat_దత్తాంశం, గుర్తులేని idat_len,
				  గుర్తులేని వెడల్పు, గుర్తులేని ఎత్తు,
				  గుర్తులేని colors, తార్కిక has_alpha,
				  స్థిరరాశి uint8_t *plte = శూన్యసూచిక,
				  గుర్తులేని plte_len = 0,
				  స్థిరరాశి uint8_t *trns = శూన్యసూచిక,
				  గుర్తులేని trns_len = 0)
  {
    గుర్తులేని idx = xobject_లెక్క++;
    hb_సదిశ_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (వెడల్పు);
    obj.append_str (" /Height ");
    obj.append_unsigned (ఎత్తు);
    obj.append_str ("\n/BitsPerComponent 8\n");

    ఒకవేళ (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      గుర్తులేని n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      కొరకు (గుర్తులేని i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    లేకపోతే
    {
      obj.append_str ("/ColorSpace ");
      గుర్తులేని color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    గుర్తులేని smask_id = 0;
    ఒకవేళ (plte && trns && trns_len)
    {
      hb_సదిశ_buf_t smask_ప్రవాహం;
      ఒకవేళ (hb_pdf_build_సూచీకృత_smask (&smask_ప్రవాహం, idat_దత్తాంశం, idat_len,
				      వెడల్పు, ఎత్తు, trns, trns_len))
      {
	hb_సదిశ_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (వెడల్పు);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (ఎత్తు);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_ప్రవాహం.నిడివి);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_ప్రవాహం.arrayZ, smask_ప్రవాహం.నిడివి);
	smask_obj.append_str ("\nendstream");
	smask_id = కూడు_object (std::move (smask_obj));
      }
    }

    ఒకవేళ (smask_id)
    {
      obj.append_str ("/SMask ");
      obj.append_unsigned (smask_id);
      obj.append_str (" 0 R\n");
    }

    obj.append_str ("/Filter /FlateDecode\n");
    obj.append_str ("/DecodeParms << /Predictor 15 /Colors ");
    obj.append_unsigned (colors);
    obj.append_str (" /BitsPerComponent 8 /Columns ");
    obj.append_unsigned (వెడల్పు);
    obj.append_str (" >>\n");
    obj.append_str ("/Length ");
    obj.append_unsigned (idat_len);
    obj.append_str (" >>\nstream\n");
    obj.append_len (idat_దత్తాంశం, idat_len);
    obj.append_str ("\nendstream");

    (శూన్య) has_alpha;

    గుర్తులేని obj_id = కూడు_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    తిరిగి idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
స్థిర hb_pdf_resources_t *
hb_pdf_get_resources (hb_సదిశ_paint_t *paint)
{
  ఒకవేళ (!paint->defs.నిడివి)
  {
    /* First call: allocate and store. */
    స్వయంచాలక *res = (hb_pdf_resources_t *) hb_calloc (1, పరిమాణం (hb_pdf_resources_t));
    ఒకవేళ (unlikely (!res)) తిరిగి శూన్యసూచిక;
    కొత్త (res) hb_pdf_resources_t ();
    ఒకవేళ (unlikely (!paint->defs.resize (పరిమాణం (శూన్య *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      తిరిగి శూన్యసూచిక;
    }
    జ్ఞాపకం_నకలు (paint->defs.arrayZ, &res, పరిమాణం (శూన్య *));
  }
  hb_pdf_resources_t *res;
  జ్ఞాపకం_నకలు (&res, paint->defs.arrayZ, పరిమాణం (శూన్య *));
  తిరిగి res;
}

శూన్య
hb_సదిశ_paint_pdf_free_resources (hb_సదిశ_paint_t *paint)
{
  ఒకవేళ (paint->defs.నిడివి >= పరిమాణం (శూన్య *))
  {
    hb_pdf_resources_t *res;
    జ్ఞాపకం_నకలు (&res, paint->defs.arrayZ, పరిమాణం (శూన్య *));
    ఒకవేళ (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
స్థిర శూన్య
hb_pdf_emit_glyph_మార్గం (hb_సదిశ_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_సదిశ_buf_t *buf)
{
  paint->మార్గం.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  ఒకవేళ (likely (paint->budget_మిగిలిన >= 0))
  {
    hb_సదిశ_మార్గం_sink_t sink = {&paint->మార్గం, paint->get_precision (),
				 paint->x_scale_గుణకం, paint->y_scale_గుణకం,
				 &paint->budget_మిగిలిన};
    hb_font_draw_glyph (font, glyph,
			hb_సదిశ_pdf_మార్గం_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->మార్గం.arrayZ, paint->మార్గం.నిడివి);
  paint->మార్గం.clear ();
}


/* ---- paint callbacks ---- */

స్థిర శూన్య
hb_pdf_paint_నెట్టు_transform (hb_paint_funcs_t *,
			     శూన్య *paint_దత్తాంశం,
			     దశాంశం xx, దశాంశం yx,
			     దశాంశం xy, దశాంశం yy,
			     దశాంశం dx, దశాంశం dy,
			     శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();
  గుర్తులేని sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_అంకె (xx, sprec);
  body.append_c (' ');
  body.append_అంకె (yx, sprec);
  body.append_c (' ');
  body.append_అంకె (xy, sprec);
  body.append_c (' ');
  body.append_అంకె (yy, sprec);
  body.append_c (' ');
  body.append_అంకె (paint->sx (dx));
  body.append_c (' ');
  body.append_అంకె (paint->sy (dy));
  body.append_str (" cm\n");
}

స్థిర శూన్య
hb_pdf_paint_తీయి_transform (hb_paint_funcs_t *,
			    శూన్య *paint_దత్తాంశం,
			    శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  paint->current_body ().append_str ("Q\n");
}

స్థిర శూన్య
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 శూన్య *paint_దత్తాంశం,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  దశాంశం r = hb_color_get_red (c) / 255.f;
  దశాంశం g = hb_color_get_green (c) / 255.f;
  దశాంశం b = hb_color_get_blue (c) / 255.f;
  దశాంశం a = hb_color_get_alpha (c) / 255.f;

  ఒకవేళ (a < 1.f / 255.f)
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();

  తార్కిక scoped = అబద్ధం;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  ఒకవేళ (a < 1.f - 1.f / 512.f)
  {
    స్వయంచాలక *res = hb_pdf_get_resources (paint);
    ఒకవేళ (res)
    {
      గుర్తులేని gs_idx = res->కూడు_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = నిజం;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_అంకె (r, 4);
  body.append_c (' ');
  body.append_అంకె (g, 4);
  body.append_c (' ');
  body.append_అంకె (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_మార్గం (paint, font, glyph, &body);
  body.append_str ("f\n");
  ఒకవేళ (scoped)
    body.append_str ("Q\n");
}

స్థిర శూన్య
hb_pdf_paint_నెట్టు_clip_glyph (hb_paint_funcs_t *,
			      శూన్య *paint_దత్తాంశం,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_మార్గం (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

స్థిర శూన్య
hb_pdf_paint_నెట్టు_clip_rectangle (hb_paint_funcs_t *,
				  శూన్య *paint_దత్తాంశం,
				  దశాంశం xmin, దశాంశం ymin,
				  దశాంశం xmax, దశాంశం ymax,
				  శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_అంకె (paint->sx (xmin));
  body.append_c (' ');
  body.append_అంకె (paint->sy (ymin));
  body.append_c (' ');
  body.append_అంకె (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_అంకె (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

స్థిర hb_draw_funcs_t *
hb_pdf_paint_నెట్టు_clip_మార్గం_ప్రారంభం (hb_paint_funcs_t *,
				   శూన్య *paint_దత్తాంశం,
				   శూన్య **draw_దత్తాంశం,
				   శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
  {
    *draw_దత్తాంశం = శూన్యసూచిక;
    తిరిగి శూన్యసూచిక;
  }

  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_మార్గం_sink = {&body, paint->get_precision (),
			   paint->x_scale_గుణకం,
			   paint->y_scale_గుణకం,
			   &paint->budget_మిగిలిన};
  *draw_దత్తాంశం = &paint->clip_మార్గం_sink;
  తిరిగి hb_సదిశ_pdf_మార్గం_draw_funcs_get ();
}

స్థిర శూన్య
hb_pdf_paint_నెట్టు_clip_మార్గం_చివర (hb_paint_funcs_t *,
				 శూన్య *paint_దత్తాంశం,
				 శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  paint->current_body ().append_str ("W n\n");
}

స్థిర శూన్య
hb_pdf_paint_తీయి_clip (hb_paint_funcs_t *,
		       శూన్య *paint_దత్తాంశం,
		       శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
స్థిర శూన్య
hb_pdf_paint_solid_color (hb_సదిశ_paint_t *paint, hb_color_t c)
{
  స్వయంచాలక &body = paint->current_body ();

  దశాంశం r = hb_color_get_red (c) / 255.f;
  దశాంశం g = hb_color_get_green (c) / 255.f;
  దశాంశం b = hb_color_get_blue (c) / 255.f;
  దశాంశం a = hb_color_get_alpha (c) / 255.f;

  ఒకవేళ (a < 1.f / 255.f)
    తిరిగి;

  /* Set alpha via ExtGState if needed. */
  ఒకవేళ (a < 1.f - 1.f / 512.f)
  {
    స్వయంచాలక *res = hb_pdf_get_resources (paint);
    ఒకవేళ (res)
    {
      గుర్తులేని gs_idx = res->కూడు_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_అంకె (r, 4);
  body.append_c (' ');
  body.append_అంకె (g, 4);
  body.append_c (' ');
  body.append_అంకె (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

స్థిర శూన్య
hb_pdf_paint_color (hb_paint_funcs_t *,
		    శూన్య *paint_దత్తాంశం,
		    hb_bool_t,
		    hb_color_t color,
		    శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
స్థిర తార్కిక
hb_pdf_build_సూచీకృత_smask (hb_సదిశ_buf_t *బహిర్గతం,
			    స్థిరరాశి అక్షరం *idat_దత్తాంశం, గుర్తులేని idat_len,
			    గుర్తులేని వెడల్పు, గుర్తులేని ఎత్తు,
			    స్థిరరాశి uint8_t *trns, గుర్తులేని trns_len)
{
#నిర్వచితం_కాకపోతే HAVE_ZLIB
  (శూన్య) బహిర్గతం; (శూన్య) idat_దత్తాంశం; (శూన్య) idat_len;
  (శూన్య) వెడల్పు; (శూన్య) ఎత్తు; (శూన్య) trns; (శూన్య) trns_len;
  తిరిగి అబద్ధం;
#లేకపోతే
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  గుర్తులేని raw_len, ముసుగు_len;
  ఒకవేళ (!వెడల్పు || !ఎత్తు ||
      వెడల్పు == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (వెడల్పు + 1, ఎత్తు, &raw_len) ||
      hb_unsigned_mul_overflows (వెడల్పు, ఎత్తు, &ముసుగు_len))
    తిరిగి అబద్ధం;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  ఒకవేళ (!raw) తిరిగి అబద్ధం;
  HB_SCOPE_GUARD (hb_free (raw));

  z_ప్రవాహం ప్రవాహం = {};
  ప్రవాహం.తదుపరి_లోపల = (Bytef *) idat_దత్తాంశం;
  ప్రవాహం.avail_లోపల = idat_len;
  ప్రవాహం.తదుపరి_బహిర్గతం = (Bytef *) raw;
  ప్రవాహం.avail_బహిర్గతం = raw_len;

  ఒకవేళ (inflateInit (&ప్రవాహం) != Z_OK)
    తిరిగి అబద్ధం;
  పూర్ణాంకం స్థితి_2 = inflate (&ప్రవాహం, Z_ముగింపు);
  గుర్తులేని దీర్ఘ మొత్తం_బహిర్గతం = ప్రవాహం.మొత్తం_బహిర్గతం;
  inflate_చివర (&ప్రవాహం);
  ఒకవేళ (స్థితి_2 != Z_ప్రవాహం_చివర || మొత్తం_బహిర్గతం != raw_len)
    తిరిగి అబద్ధం;

  /* Un-filter and map to alpha. */
  ఒకవేళ (!బహిర్గతం->resize (ముసుగు_len))
    తిరిగి అబద్ధం;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (వెడల్పు);
  ఒకవేళ (!unfiltered)
    తిరిగి అబద్ధం;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (వెడల్పు, 1);
  ఒకవేళ (!prev_unfiltered)
    తిరిగి అబద్ధం;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  కొరకు (గుర్తులేని y = 0; y < ఎత్తు; y++)
  {
    uint8_t *అడ్డువరుస = raw + y * (వెడల్పు + 1);
    uint8_t filter = అడ్డువరుస[0];
    uint8_t *pixels = అడ్డువరుస + 1;

    కొరకు (గుర్తులేని x = 0; x < వెడల్పు; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      ఎంపిక (filter)
      {
      స్థితి 0: unfiltered[x] = val; ఆపు; /* None */
      స్థితి 1: unfiltered[x] = val + a; ఆపు; /* Sub */
      స్థితి 2: unfiltered[x] = val + b; ఆపు; /*పైకి*/
      స్థితి 3: unfiltered[x] = val + (uint8_t) (((గుర్తులేని) a + b) / 2); ఆపు; /*సరాసరి*/
      స్థితి 4: /* Paeth */
      {
	పూర్ణాంకం p = (పూర్ణాంకం) a + (పూర్ణాంకం) b - (పూర్ణాంకం) c;
	పూర్ణాంకం pa = నిరపేక్ష (p - (పూర్ణాంకం) a);
	పూర్ణాంకం pb = నిరపేక్ష (p - (పూర్ణాంకం) b);
	పూర్ణాంకం pc = నిరపేక్ష (p - (పూర్ణాంకం) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	ఆపు;
      }
      అప్రమేయ: unfiltered[x] = val; ఆపు;
      }

      /* Map palette index to alpha. */
      uint8_t idx = unfiltered[x];
      బహిర్గతం->arrayZ[y * వెడల్పు + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = prev_unfiltered;
    prev_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  తిరిగి నిజం;
#అయితే_ముగింపు
}

/* Read a big-endian uint32 from a byte pointer. */
స్థిర అంతఃవరుస uint32_t
hb_pdf_png_u32 (స్థిరరాశి uint8_t *p)
{
  తిరిగి ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

స్థిర hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    శూన్య *paint_దత్తాంశం,
		    hb_blob_t *image,
		    గుర్తులేని వెడల్పు,
		    గుర్తులేని ఎత్తు,
		    hb_ట్యాగ్_t format,
		    దశాంశం slant HB_వాడని,
		    hb_glyph_extents_t *extents,
		    శూన్య *)
{
  ఒకవేళ (format != HB_ట్యాగ్ ('p','n','g',' '))
    తిరిగి అబద్ధం;
  ఒకవేళ (!extents || !వెడల్పు || !ఎత్తు)
    తిరిగి అబద్ధం;

  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి అబద్ధం;
  స్వయంచాలక *res = hb_pdf_get_resources (paint);
  ఒకవేళ (unlikely (!res))
    తిరిగి అబద్ధం;

  గుర్తులేని len = 0;
  స్థిరరాశి uint8_t *దత్తాంశం = (స్థిరరాశి uint8_t *) hb_blob_get_దత్తాంశం (image, &len);
  ఒకవేళ (!దత్తాంశం || len < 8)
    తిరిగి అబద్ధం;

  /* Verify PNG signature. */
  స్థిర స్థిరరాశి uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  ఒకవేళ (hb_memcmp (దత్తాంశం, png_sig, 8) != 0)
    తిరిగి అబద్ధం;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  గుర్తులేని png_w = 0, png_h = 0;
  గుర్తులేని colors = 3; /* default RGB */
  uint8_t color_type = 2;
  తార్కిక has_alpha = అబద్ధం;
  స్థిరరాశి uint8_t *plte_దత్తాంశం = శూన్యసూచిక;
  గుర్తులేని plte_len = 0;
  స్థిరరాశి uint8_t *trns_దత్తాంశం = శూన్యసూచిక;
  గుర్తులేని trns_len = 0;
  hb_సదిశ_buf_t idat;

  గుర్తులేని pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  అయితే (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (దత్తాంశం + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (దత్తాంశం + pos + 4);
    ఒకవేళ (chunk_len > len - pos - 12)
      ఆపు;
    స్థిరరాశి uint8_t *chunk_దత్తాంశం = దత్తాంశం + pos + 8;

    ఒకవేళ (chunk_type == 0x49484452u) /* IHDR */
    {
      ఒకవేళ (chunk_len < 13) తిరిగి అబద్ధం;
      png_w = hb_pdf_png_u32 (chunk_దత్తాంశం);
      png_h = hb_pdf_png_u32 (chunk_దత్తాంశం + 4);
      uint8_t బిట్టు_లోతు_2 = chunk_దత్తాంశం[8];
      color_type = chunk_దత్తాంశం[9];
      ఒకవేళ (బిట్టు_లోతు_2 != 8) తిరిగి అబద్ధం; /* only 8-bit supported */

      ఎంపిక (color_type)
      {
      స్థితి 0: colors = 1; has_alpha = అబద్ధం; ఆపు; /* Grayscale */
      స్థితి 2: colors = 3; has_alpha = అబద్ధం; ఆపు; /* RGB */
      స్థితి 3: colors = 1; has_alpha = అబద్ధం; ఆపు; /*సూచీకృత*/
      స్థితి 4: colors = 2; has_alpha = నిజం;  ఆపు; /* Gray+Alpha */
      స్థితి 6: colors = 4; has_alpha = నిజం;  ఆపు; /* RGBA */
      అప్రమేయ: తిరిగి అబద్ధం;
      }
    }
    లేకపోతే ఒకవేళ (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_దత్తాంశం = chunk_దత్తాంశం;
      plte_len = chunk_len;
    }
    లేకపోతే ఒకవేళ (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_దత్తాంశం = chunk_దత్తాంశం;
      trns_len = chunk_len;
    }
    లేకపోతే ఒకవేళ (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((స్థిరరాశి అక్షరం *) chunk_దత్తాంశం, chunk_len);
    }
    లేకపోతే ఒకవేళ (chunk_type == 0x49454E44u) /* IEND */
      ఆపు;

    pos += 12 + chunk_len;
  }

  ఒకవేళ (!png_w || !png_h || !idat.నిడివి)
    తిరిగి అబద్ధం;

  గుర్తులేని im_idx = res->కూడు_xobject_png_image (idat.arrayZ, idat.నిడివి,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_దత్తాంశం, plte_len,
						 trns_దత్తాంశం, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  దశాంశం ix = (దశాంశం) extents->x_bearing;
  దశాంశం iy = (దశాంశం) extents->y_bearing + (దశాంశం) extents->ఎత్తు;
  దశాంశం iw = (దశాంశం) extents->వెడల్పు;
  దశాంశం ih = -(దశాంశం) extents->ఎత్తు; /* negative because image Y goes up but height is negative in extents */

  body.append_అంకె (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_అంకె (paint->sy (ih));
  body.append_c (' ');
  body.append_అంకె (paint->sx (ix));
  body.append_c (' ');
  body.append_అంకె (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  తిరిగి నిజం;
}

/* ---- Gradient helpers ---- */

స్థిర తార్కిక
hb_pdf_gradient_needs_alpha (hb_array_t<స్థిరరాశి hb_color_stop_t> stops)
{
  కొరకు (స్థిరరాశి స్వయంచాలక &s : stops)
    ఒకవేళ (hb_color_get_alpha (s.color) != 255)
      తిరిగి నిజం;
  తిరిగి అబద్ధం;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
స్థిర శూన్య
hb_pdf_build_alpha_interpolation_function (hb_సదిశ_buf_t *obj,
					   దశాంశం a0, దశాంశం a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_అంకె (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_అంకె (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
స్థిర గుర్తులేని
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_సదిశ_paint_t *paint)
{
  గుర్తులేని లెక్క = paint->color_stops_scratch.నిడివి;

  ఒకవేళ (లెక్క < 2)
  {
    దశాంశం a = లెక్క ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_సదిశ_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    తిరిగి res->కూడు_object (std::move (obj));
  }

  ఒకవేళ (లెక్క == 2)
  {
    hb_సదిశ_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    తిరిగి res->కూడు_object (std::move (obj));
  }

  hb_సదిశ_t<గుర్తులేని> sub_func_ids;
  కొరకు (గుర్తులేని i = 0; i + 1 < లెక్క; i++)
  {
    hb_సదిశ_buf_t sub;
    hb_pdf_build_alpha_interpolation_function (&sub,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i + 1].color) / 255.f);
    sub_func_ids.push (res->కూడు_object (std::move (sub)));
  }

  hb_సదిశ_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");
  obj.append_str ("/Functions [");
  కొరకు (గుర్తులేని i = 0; i < sub_func_ids.నిడివి; i++)
  {
    ఒకవేళ (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  కొరకు (గుర్తులేని i = 1; i + 1 < లెక్క; i++)
  {
    ఒకవేళ (i > 1) obj.append_c (' ');
    obj.append_అంకె (paint->color_stops_scratch.arrayZ[i].స్థానభ్రంశం, 4);
  }
  obj.append_str ("]\n/Encode [");
  కొరకు (గుర్తులేని i = 0; i + 1 < లెక్క; i++)
  {
    ఒకవేళ (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  తిరిగి res->కూడు_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
స్థిర శూన్య
hb_pdf_build_interpolation_function (hb_సదిశ_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_అంకె (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_అంకె (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_అంకె (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_అంకె (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_అంకె (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_అంకె (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
స్థిర గుర్తులేని
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_సదిశ_paint_t *paint)
{
  గుర్తులేని లెక్క = paint->color_stops_scratch.నిడివి;

  ఒకవేళ (లెక్క < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = లెక్క ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_సదిశ_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    తిరిగి res->కూడు_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (స్థిరరాశి hb_color_stop_t &a, స్థిరరాశి hb_color_stop_t &b)
    { తిరిగి (a.స్థానభ్రంశం > b.స్థానభ్రంశం) - (a.స్థానభ్రంశం < b.స్థానభ్రంశం); });

  ఒకవేళ (లెక్క == 2)
  {
    /* Two stops: single interpolation function. */
    hb_సదిశ_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    తిరిగి res->కూడు_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_సదిశ_t<గుర్తులేని> sub_func_ids;
  కొరకు (గుర్తులేని i = 0; i + 1 < లెక్క; i++)
  {
    hb_సదిశ_buf_t sub;
    hb_pdf_build_interpolation_function (&sub,
					  paint->color_stops_scratch.arrayZ[i].color,
					  paint->color_stops_scratch.arrayZ[i + 1].color);
    sub_func_ids.push (res->కూడు_object (std::move (sub)));
  }

  /* Stitching function (Type 3). */
  hb_సదిశ_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");

  /* Functions array. */
  obj.append_str ("/Functions [");
  కొరకు (గుర్తులేని i = 0; i < sub_func_ids.నిడివి; i++)
  {
    ఒకవేళ (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  కొరకు (గుర్తులేని i = 1; i + 1 < లెక్క; i++)
  {
    ఒకవేళ (i > 1) obj.append_c (' ');
    obj.append_అంకె (paint->color_stops_scratch.arrayZ[i].స్థానభ్రంశం, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  కొరకు (గుర్తులేని i = 0; i + 1 < లెక్క; i++)
  {
    ఒకవేళ (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  తిరిగి res->కూడు_object (std::move (obj));
}

స్థిర శూన్య
hb_pdf_paint_రేఖీయ_gradient (hb_paint_funcs_t *,
			      శూన్య *paint_దత్తాంశం,
			      hb_color_పంక్తి_t *color_పంక్తి,
			      దశాంశం x0, దశాంశం y0,
			      దశాంశం x1, దశాంశం y1,
			      దశాంశం x2 HB_వాడని, దశాంశం y2 HB_వాడని,
			      శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  స్వయంచాలక *res = hb_pdf_get_resources (paint);
  ఒకవేళ (unlikely (!res))
    తిరిగి;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  ఒకవేళ (!paint->fetch_color_stops (color_పంక్తి))
    తిరిగి;
  hb_సదిశ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops.arrayZ, stops.నిడివి, &mn, &mx);
  దశాంశం gx0 = x0 + mn * (x1 - x0);
  దశాంశం gy0 = y0 + mn * (y1 - y0);
  దశాంశం gx1 = x0 + mx * (x1 - x0);
  దశాంశం gy1 = y0 + mx * (y1 - y0);

  గుర్తులేని func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);
  స్థిరరాశి అక్షరం *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_సదిశ_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_అంకె (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_అంకె (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_అంకె (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_అంకె (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  గుర్తులేని sh_idx = res->కూడు_shading (std::move (sh));

  స్వయంచాలక &body = paint->current_body ();

  తార్కిక needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  ఒకవేళ (needs_alpha)
  {
    గుర్తులేని alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_సదిశ_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_అంకె (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_అంకె (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_అంకె (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_అంకె (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    గుర్తులేని alpha_sh_id = res->కూడు_object (std::move (ash));

    గుర్తులేని gs_idx = res->కూడు_extgstate_smask (alpha_sh_id,
						paint->sx (gx0), paint->sy (gy0),
						paint->sx (gx1 - gx0), paint->sy (gy1 - gy0),
						paint->get_precision ());
    body.append_str ("/GS");
    body.append_unsigned (gs_idx);
    body.append_str (" gs\n");
  }

  body.append_str ("/SH");
  body.append_unsigned (sh_idx);
  body.append_str (" sh\n");
}

స్థిర శూన్య
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      శూన్య *paint_దత్తాంశం,
			      hb_color_పంక్తి_t *color_పంక్తి,
			      దశాంశం x0, దశాంశం y0, దశాంశం r0,
			      దశాంశం x1, దశాంశం y1, దశాంశం r1,
			      శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  స్వయంచాలక *res = hb_pdf_get_resources (paint);
  ఒకవేళ (unlikely (!res))
    తిరిగి;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  ఒకవేళ (!paint->fetch_color_stops (color_పంక్తి))
    తిరిగి;
  hb_సదిశ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  దశాంశం mn, mx;
  hb_paint_normalize_color_పంక్తి (stops.arrayZ, stops.నిడివి, &mn, &mx);
  దశాంశం gx0 = x0 + mn * (x1 - x0);
  దశాంశం gy0 = y0 + mn * (y1 - y0);
  దశాంశం gr0 = r0 + mn * (r1 - r0);
  దశాంశం gx1 = x0 + mx * (x1 - x0);
  దశాంశం gy1 = y0 + mx * (y1 - y0);
  దశాంశం gr1 = r0 + mx * (r1 - r0);

  గుర్తులేని func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);
  స్థిరరాశి అక్షరం *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_సదిశ_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_అంకె (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_అంకె (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_అంకె (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_అంకె (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_అంకె (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_అంకె (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  గుర్తులేని sh_idx = res->కూడు_shading (std::move (sh));

  స్వయంచాలక &body = paint->current_body ();

  తార్కిక needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  ఒకవేళ (needs_alpha)
  {
    గుర్తులేని alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_సదిశ_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_అంకె (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_అంకె (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_అంకె (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_అంకె (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_అంకె (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_అంకె (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    గుర్తులేని alpha_sh_id = res->కూడు_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    దశాంశం cx = (gr1 >= gr0) ? gx1 : gx0;
    దశాంశం cy = (gr1 >= gr0) ? gy1 : gy0;
    దశాంశం rr = hb_max (gr0, gr1);
    గుర్తులేని gs_idx = res->కూడు_extgstate_smask (alpha_sh_id,
						paint->sx (cx - rr), paint->sy (cy - rr),
						paint->sx (2 * rr), paint->sy (2 * rr),
						paint->get_precision ());
    body.append_str ("/GS");
    body.append_unsigned (gs_idx);
    body.append_str (" gs\n");
  }

  body.append_str ("/SH");
  body.append_unsigned (sh_idx);
  body.append_str (" sh\n");
}


/* Encode a 16-bit big-endian unsigned value into buf. */
స్థిర శూన్య
hb_pdf_encode_u16 (hb_సదిశ_buf_t *buf, uint16_t v)
{
  అక్షరం బైట్లు[2] = {(అక్షరం) (v >> 8), (అక్షరం) (v & 0xFF)};
  buf->append_len (బైట్లు, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
స్థిర శూన్య
hb_pdf_encode_coord (hb_సదిశ_buf_t *buf,
		     దశాంశం val, దశాంశం lo, దశాంశం hi)
{
  దశాంశం t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
స్థిర శూన్య
hb_pdf_encode_color_rgb (hb_సదిశ_buf_t *buf, hb_color_t c)
{
  అక్షరం rgb[3] = {(అక్షరం) hb_color_get_red (c),
		 (అక్షరం) hb_color_get_green (c),
		 (అక్షరం) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
స్థిర శూన్య
hb_pdf_encode_color_alpha (hb_సదిశ_buf_t *buf, hb_color_t c)
{
  అక్షరం a = (అక్షరం) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
స్థిర శూన్య
hb_pdf_encode_బిందువు (hb_సదిశ_buf_t *buf,
		     దశాంశం x, దశాంశం y,
		     దశాంశం xlo, దశాంశం xhi,
		     దశాంశం ylo, దశాంశం yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
స్థిర శూన్య
hb_pdf_కూడు_sweep_patch (hb_సదిశ_buf_t *mesh,
			hb_సదిశ_buf_t *alpha_mesh,
			దశాంశం cx, దశాంశం cy,
			దశాంశం xlo, దశాంశం xhi, దశాంశం ylo, దశాంశం yhi,
			దశాంశం a0, hb_color_t c0_లోపల,
			దశాంశం a1, hb_color_t c1_లోపల)
{
  స్థిరరాశి దశాంశం R = 32767.f;
  స్థిరరాశి దశాంశం eps = 0.5f;
  స్థిరరాశి దశాంశం MAX_SECTOR = (దశాంశం) M_PI / 2.f;

  పూర్ణాంకం అంకె_splits = (పూర్ణాంకం) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  ఒకవేళ (అంకె_splits < 1) అంకె_splits = 1;

  కొరకు (పూర్ణాంకం s = 0; s < అంకె_splits; s++)
  {
    దశాంశం k0 = (దశాంశం) s / అంకె_splits;
    దశాంశం k1 = (దశాంశం) (s + 1) / అంకె_splits;
    దశాంశం sa0 = a0 + k0 * (a1 - a0);
    దశాంశం sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_లోపల, c1_లోపల, k0);
    hb_color_t sc1 = hb_color_lerp (c0_లోపల, c1_లోపల, k1);

    దశాంశం da = sa1 - sa0;
    దశాంశం kappa = (4.f / 3.f) * tanf (da / 4.f);

    దశాంశం cos0 = cosf (sa0), sin0 = sinf (sa0);
    దశాంశం cos1 = cosf (sa1), sin1 = sinf (sa1);

    దశాంశం p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    దశాంశం p3x = cx + R * cos0,   p3y = cy + R * sin0;
    దశాంశం p6x = cx + R * cos1,   p6y = cy + R * sin1;
    దశాంశం p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    దశాంశం e1_1x = p0x + (p3x - p0x) / 3.f;
    దశాంశం e1_1y = p0y + (p3y - p0y) / 3.f;
    దశాంశం e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    దశాంశం e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    దశాంశం e2_1x = p3x + kappa * R * (-sin0);
    దశాంశం e2_1y = p3y + kappa * R * ( cos0);
    దశాంశం e2_2x = p6x - kappa * R * (-sin1);
    దశాంశం e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    దశాంశం e3_1x = p6x + (p9x - p6x) / 3.f;
    దశాంశం e3_1y = p6y + (p9y - p6y) / 3.f;
    దశాంశం e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    దశాంశం e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    దశాంశం e4_1x = p9x + kappa * eps * (-sin1);
    దశాంశం e4_1y = p9y + kappa * eps * ( cos1);
    దశాంశం e4_2x = p0x - kappa * eps * (-sin0);
    దశాంశం e4_2y = p0y - kappa * eps * ( cos0);

    mesh->append_c ('\0'); /* flag = 0, new patch */

    hb_pdf_encode_బిందువు (mesh, p0x, p0y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, p3x, p3y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, p6x, p6y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, p9x, p9y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_బిందువు (mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

    hb_pdf_encode_color_rgb (mesh, sc0); /* inner start */
    hb_pdf_encode_color_rgb (mesh, sc0); /* outer start */
    hb_pdf_encode_color_rgb (mesh, sc1); /* outer end */
    hb_pdf_encode_color_rgb (mesh, sc1); /* inner end */

    ఒకవేళ (alpha_mesh)
    {
      alpha_mesh->append_c ('\0');

      hb_pdf_encode_బిందువు (alpha_mesh, p0x, p0y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, p3x, p3y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, p6x, p6y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, p9x, p9y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_బిందువు (alpha_mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
    }
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
నిర్మాణం hb_pdf_sweep_ctx_t {
  hb_సదిశ_buf_t *mesh;
  hb_సదిశ_buf_t *alpha_mesh;
  దశాంశం cx, cy, xlo, xhi, ylo, yhi;
};

స్థిర శూన్య
hb_pdf_sweep_emit_patch (దశాంశం a0, hb_color_t c0,
			 దశాంశం a1, hb_color_t c1,
			 శూన్య *user_దత్తాంశం)
{
  స్వయంచాలక *ctx = (hb_pdf_sweep_ctx_t *) user_దత్తాంశం;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  ఒకవేళ (unlikely (ctx->mesh->లోపల_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->లోపల_error ())))
    తిరిగి;
  hb_pdf_కూడు_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

స్థిర శూన్య
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     శూన్య *paint_దత్తాంశం,
			     hb_color_పంక్తి_t *color_పంక్తి,
			     దశాంశం cx, దశాంశం cy,
			     దశాంశం ప్రారంభం_angle,
			     దశాంశం చివర_angle,
			     శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  స్వయంచాలక *res = hb_pdf_get_resources (paint);
  ఒకవేళ (unlikely (!res))
    తిరిగి;

  /* Get and sort color stops. */
  ఒకవేళ (!paint->fetch_color_stops (color_పంక్తి))
    తిరిగి;
  hb_సదిశ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (స్థిరరాశి hb_color_stop_t &a, స్థిరరాశి hb_color_stop_t &b)
    { తిరిగి (a.స్థానభ్రంశం > b.స్థానభ్రంశం) - (a.స్థానభ్రంశం < b.స్థానభ్రంశం); });

  hb_paint_extend_t extend = hb_color_పంక్తి_get_extend (color_పంక్తి);

  స్థిరరాశి దశాంశం R = 32767.f;
  దశాంశం scx = paint->sx (cx), scy = paint->sy (cy);
  దశాంశం xlo = scx - R - 1, xhi = scx + R + 1;
  దశాంశం ylo = scy - R - 1, yhi = scy + R + 1;

  తార్కిక needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_సదిశ_buf_t mesh;
  hb_సదిశ_buf_t alpha_mesh;
  mesh.alloc (256);
  ఒకవేళ (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : శూన్యసూచిక,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.నిడివి, extend,
				 ప్రారంభం_angle, చివర_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  ఒకవేళ (!mesh.నిడివి)
    తిరిగి;

  స్వయంచాలక hb_pdf_build_mesh_shading = [&] (hb_సదిశ_buf_t &m,
					 స్థిరరాశి అక్షరం *cs,
					 స్థిరరాశి అక్షరం *decode_suffix) -> గుర్తులేని
  {
    hb_సదిశ_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_అంకె (xlo, 2);
    sh.append_c (' ');
    sh.append_అంకె (xhi, 2);
    sh.append_c (' ');
    sh.append_అంకె (ylo, 2);
    sh.append_c (' ');
    sh.append_అంకె (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.నిడివి);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.నిడివి);
    sh.append_str ("\nendstream");
    తిరిగి res->కూడు_object (std::move (sh));
  };

  గుర్తులేని sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  గుర్తులేని sh_idx = res->కూడు_shading_by_id (sh_obj_id);

  స్వయంచాలక &body = paint->current_body ();

  ఒకవేళ (needs_alpha && alpha_mesh.నిడివి)
  {
    గుర్తులేని alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    గుర్తులేని gs_idx = res->కూడు_extgstate_smask (alpha_sh_id,
						xlo, ylo,
						xhi - xlo, yhi - ylo,
						paint->get_precision ());
    body.append_str ("/GS");
    body.append_unsigned (gs_idx);
    body.append_str (" gs\n");
  }

  body.append_str ("/SH");
  body.append_unsigned (sh_idx);
  body.append_str (" sh\n");
}

స్థిర స్థిరరాశి అక్షరం *
hb_pdf_blend_బహుళకం_name (hb_paint_సంయుక్త_బహుళకం_t బహుళకం)
{
  ఎంపిక (బహుళకం)
  {
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_MULTIPLY:       తిరిగి "Multiply";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SCREEN:         తిరిగి "Screen";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_OVERLAY:        తిరిగి "Overlay";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DARKEN:         తిరిగి "Darken";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_LIGHTEN:        తిరిగి "Lighten";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_DODGE:    తిరిగి "ColorDodge";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_COLOR_BURN:     తిరిగి "ColorBurn";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HARD_LIGHT:     తిరిగి "HardLight";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SOFT_LIGHT:     తిరిగి "SoftLight";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DIFFERENCE:     తిరిగి "Difference";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_EXCLUSION:      తిరిగి "Exclusion";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_HUE:        తిరిగి "Hue";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_SATURATION: తిరిగి "Saturation";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_COLOR:      తిరిగి "Color";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_HSL_LUMINOSITY: తిరిగి "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_PLUS:           తిరిగి "Screen";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_XOR:            తిరిగి "Difference";
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_CLEAR:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_OVER:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_OVER:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_లోపల:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_లోపల:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_బహిర్గతం:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_బహిర్గతం:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_SRC_ATOP:
  స్థితి HB_PAINT_సంయుక్త_బహుళకం_DEST_ATOP:
  అప్రమేయ:                                     తిరిగి శూన్యసూచిక; /*సాధారణ*/
  }
}

స్థిర శూన్య
hb_pdf_paint_నెట్టు_సమూహం (hb_paint_funcs_t *,
			 శూన్య *paint_దత్తాంశం,
			 శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  paint->current_body ().append_str ("q\n");
}

స్థిర శూన్య
hb_pdf_paint_నెట్టు_సమూహం_for (hb_paint_funcs_t *,
			     శూన్య *paint_దత్తాంశం,
			     hb_paint_సంయుక్త_బహుళకం_t బహుళకం,
			     శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;

  స్వయంచాలక &body = paint->current_body ();
  body.append_str ("q\n");

  స్థిరరాశి అక్షరం *bm = hb_pdf_blend_బహుళకం_name (బహుళకం);
  ఒకవేళ (bm)
  {
    స్వయంచాలక *res = hb_pdf_get_resources (paint);
    ఒకవేళ (likely (res))
    {
      గుర్తులేని gs_idx = res->కూడు_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

స్థిర శూన్య
hb_pdf_paint_తీయి_సమూహం (hb_paint_funcs_t *,
			శూన్య *paint_దత్తాంశం,
			hb_paint_సంయుక్త_బహుళకం_t బహుళకం HB_వాడని,
			శూన్య *)
{
  స్వయంచాలక *paint = (hb_సదిశ_paint_t *) paint_దత్తాంశం;
  ఒకవేళ (unlikely (!paint->ensure_initialized ()))
    తిరిగి;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

స్థిర అంతఃవరుస శూన్య free_static_pdf_paint_funcs ();

స్థిర నిర్మాణం hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  స్థిర hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_సమితి_నెట్టు_transform_func (funcs, (hb_paint_నెట్టు_transform_func_t) hb_pdf_paint_నెట్టు_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_transform_func (funcs, (hb_paint_తీయి_transform_func_t) hb_pdf_paint_తీయి_transform, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_glyph_func (funcs, (hb_paint_నెట్టు_clip_glyph_func_t) hb_pdf_paint_నెట్టు_clip_glyph, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_rectangle_func (funcs, (hb_paint_నెట్టు_clip_rectangle_func_t) hb_pdf_paint_నెట్టు_clip_rectangle, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_ప్రారంభం_func (funcs, (hb_paint_నెట్టు_clip_మార్గం_ప్రారంభం_func_t) hb_pdf_paint_నెట్టు_clip_మార్గం_ప్రారంభం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_clip_మార్గం_చివర_func (funcs, (hb_paint_నెట్టు_clip_మార్గం_చివర_func_t) hb_pdf_paint_నెట్టు_clip_మార్గం_చివర, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_clip_func (funcs, (hb_paint_తీయి_clip_func_t) hb_pdf_paint_తీయి_clip, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_రేఖీయ_gradient_func (funcs, (hb_paint_రేఖీయ_gradient_func_t) hb_pdf_paint_రేఖీయ_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_func (funcs, (hb_paint_నెట్టు_సమూహం_func_t) hb_pdf_paint_నెట్టు_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_నెట్టు_సమూహం_for_func (funcs, (hb_paint_నెట్టు_సమూహం_for_func_t) hb_pdf_paint_నెట్టు_సమూహం_for, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_తీయి_సమూహం_func (funcs, (hb_paint_తీయి_సమూహం_func_t) hb_pdf_paint_తీయి_సమూహం, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_సమితి_budget_func (funcs, hb_సదిశ_paint_సమితి_budget_callback, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_func (funcs, hb_సదిశ_paint_get_budget_callback, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_సమితి_get_budget_మిగిలిన_func (funcs, hb_సదిశ_paint_get_budget_మిగిలిన_callback, శూన్యసూచిక, శూన్యసూచిక);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    తిరిగి funcs;
  }
} static_pdf_paint_funcs;

స్థిర అంతఃవరుస శూన్య
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_సదిశ_paint_pdf_funcs_get ()
{
  తిరిగి static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_సదిశ_paint_render_pdf (hb_సదిశ_paint_t *paint)
{
  ఒకవేళ (!paint->has_extents)
    తిరిగి శూన్యసూచిక;
  ఒకవేళ (!paint->సమూహం_దొంతర.నిడివి ||
      !paint->సమూహం_దొంతర.arrayZ[0].నిడివి)
    తిరిగి శూన్యసూచిక;

  hb_సదిశ_buf_t &content = paint->సమూహం_దొంతర.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  దశాంశం ex = paint->extents.x;
  దశాంశం ey = paint->extents.y;
  దశాంశం ew = paint->extents.వెడల్పు;
  దశాంశం eh = paint->extents.ఎత్తు;

  గుర్తులేని అంకె_extra = res ? res->objects.నిడివి : 0;
  గుర్తులేని మొత్తం_objects = 4 + అంకె_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_సదిశ_buf_t బహిర్గతం;
  hb_buf_recover_recycled (paint->recycled_blob, &బహిర్గతం);
  బహిర్గతం.alloc (content.నిడివి + అంకె_extra * 128 + 1024);

  hb_సదిశ_t<గుర్తులేని> offsets;
  ఒకవేళ (unlikely (!offsets.resize (మొత్తం_objects)))
    తిరిగి శూన్యసూచిక;

  బహిర్గతం.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  బహిర్గతం.append_అంకె (ex);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (ey);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (ex + ew);
  బహిర్గతం.append_c (' ');
  బహిర్గతం.append_అంకె (ey + eh);
  బహిర్గతం.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  తార్కిక has_resources = res &&
    (res->extgstate_dict.నిడివి || res->shading_dict.నిడివి || res->xobject_dict.నిడివి);
  ఒకవేళ (has_resources)
  {
    బహిర్గతం.append_str ("\n/Resources <<");
    ఒకవేళ (res->extgstate_dict.నిడివి)
    {
      బహిర్గతం.append_str (" /ExtGState << ");
      బహిర్గతం.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.నిడివి);
      బహిర్గతం.append_str (">>");
    }
    ఒకవేళ (res->shading_dict.నిడివి)
    {
      బహిర్గతం.append_str (" /Shading << ");
      బహిర్గతం.append_len (res->shading_dict.arrayZ, res->shading_dict.నిడివి);
      బహిర్గతం.append_str (">>");
    }
    ఒకవేళ (res->xobject_dict.నిడివి)
    {
      బహిర్గతం.append_str (" /XObject << ");
      బహిర్గతం.append_len (res->xobject_dict.arrayZ, res->xobject_dict.నిడివి);
      బహిర్గతం.append_str (">>");
    }
    బహిర్గతం.append_str (" >>");
  }

  బహిర్గతం.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_సదిశ_buf_t bg_ఉపసర్గ;
  ఒకవేళ (hb_color_get_alpha (paint->background))
  {
    దశాంశం r = hb_color_get_red (paint->background) / 255.f;
    దశాంశం g = hb_color_get_green (paint->background) / 255.f;
    దశాంశం b = hb_color_get_blue (paint->background) / 255.f;
    దశాంశం a = hb_color_get_alpha (paint->background) / 255.f;
    ఒకవేళ (a < 1.f - 1.f / 512.f)
    {
      ఒకవేళ (res)
      {
	గుర్తులేని gs_idx = res->కూడు_extgstate_alpha (a);
	bg_ఉపసర్గ.append_str ("/GS");
	bg_ఉపసర్గ.append_unsigned (gs_idx);
	bg_ఉపసర్గ.append_str (" gs\n");
      }
    }
    bg_ఉపసర్గ.append_అంకె (r, 4);
    bg_ఉపసర్గ.append_c (' ');
    bg_ఉపసర్గ.append_అంకె (g, 4);
    bg_ఉపసర్గ.append_c (' ');
    bg_ఉపసర్గ.append_అంకె (b, 4);
    bg_ఉపసర్గ.append_str (" rg\n");
    bg_ఉపసర్గ.append_అంకె (ex);
    bg_ఉపసర్గ.append_c (' ');
    bg_ఉపసర్గ.append_అంకె (ey);
    bg_ఉపసర్గ.append_c (' ');
    bg_ఉపసర్గ.append_అంకె (ew);
    bg_ఉపసర్గ.append_c (' ');
    bg_ఉపసర్గ.append_అంకె (eh);
    bg_ఉపసర్గ.append_str (" re f\n");
  }
  గుర్తులేని ప్రవాహం_len = bg_ఉపసర్గ.నిడివి + content.నిడివి;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("4 0 obj\n<< /Length ");
  బహిర్గతం.append_unsigned (ప్రవాహం_len);
  బహిర్గతం.append_str (" >>\nstream\n");
  బహిర్గతం.append_len (bg_ఉపసర్గ.arrayZ, bg_ఉపసర్గ.నిడివి);
  బహిర్గతం.append_len (content.arrayZ, content.నిడివి);
  బహిర్గతం.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  కొరకు (గుర్తులేని i = 0; i < అంకె_extra; i++)
  {
    offsets.arrayZ[4 + i] = బహిర్గతం.నిడివి;
    బహిర్గతం.append_unsigned (5 + i);
    బహిర్గతం.append_str (" 0 obj\n");
    స్వయంచాలక &obj = res->objects.arrayZ[i];
    బహిర్గతం.append_len (obj.దత్తాంశం.arrayZ, obj.దత్తాంశం.నిడివి);
    బహిర్గతం.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  గుర్తులేని xref_స్థానభ్రంశం = బహిర్గతం.నిడివి;
  బహిర్గతం.append_str ("xref\n0 ");
  బహిర్గతం.append_unsigned (మొత్తం_objects + 1);
  బహిర్గతం.append_str ("\n0000000000 65535 f \n");
  కొరకు (గుర్తులేని i = 0; i < మొత్తం_objects; i++)
  {
    అక్షరం tmp[21];
    snprintf (tmp, పరిమాణం (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    బహిర్గతం.append_len (tmp, 20);
  }

  /* Trailer */
  బహిర్గతం.append_str ("trailer\n<< /Size ");
  బహిర్గతం.append_unsigned (మొత్తం_objects + 1);
  బహిర్గతం.append_str (" /Root 1 0 R >>\nstartxref\n");
  బహిర్గతం.append_unsigned (xref_స్థానభ్రంశం);
  బహిర్గతం.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &బహిర్గతం);

  తిరిగి blob;
}

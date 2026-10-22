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

#شامل "hb.hh"

#شامل "hb-vector-paint.hh"
#شامل "hb-vector-draw.hh"
#شامل "hb-paint.hh"

#شامل <math.h>
#شامل <stdio.h>

#اگر_تعریف_شدہ HAVE_ZLIB
#شامل <zlib.h>
#اگر_ختم

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

ساخت hb_pdf_obj_t
{
  hb_سمتیہ_buf_t ڈیٹا;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
ساکن منطقی
hb_pdf_build_اشاریہ_دار_smask (hb_سمتیہ_buf_t *برآمد,
			    ثابت حرف *idat_ڈیٹا, غیرعلامتی idat_len,
			    غیرعلامتی چوڑائی, غیرعلامتی اونچائی,
			    ثابت uint8_t *trns, غیرعلامتی trns_len);

ساخت hb_pdf_resources_t
{
  hb_سمتیہ_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_سمتیہ_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_سمتیہ_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_سمتیہ_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  غیرعلامتی extgstate_گنتی = 0;
  غیرعلامتی shading_گنتی = 0;
  غیرعلامتی xobject_گنتی = 0;

  غیرعلامتی جوڑو_object (hb_سمتیہ_buf_t &&obj_ڈیٹا)
  {
    غیرعلامتی id = 5 + objects.لمبائی; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.ڈیٹا = std::move (obj_ڈیٹا);
    objects.push (std::move (obj));
    واپس id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  غیرعلامتی جوڑو_extgstate_alpha (اعشاری alpha)
  {
    غیرعلامتی idx = extgstate_گنتی++;
    hb_سمتیہ_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_عدد (alpha, 4);
    obj.append_str (" >>");
    غیرعلامتی obj_id = جوڑو_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    واپس idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  غیرعلامتی جوڑو_extgstate_blend (ثابت حرف *bm)
  {
    غیرعلامتی idx = extgstate_گنتی++;
    hb_سمتیہ_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    غیرعلامتی obj_id = جوڑو_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    واپس idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  غیرعلامتی جوڑو_extgstate_smask (غیرعلامتی alpha_shading_id,
				اعشاری bbox_x, اعشاری bbox_y,
				اعشاری bbox_w, اعشاری bbox_h,
				غیرعلامتی precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_سمتیہ_buf_t form_دھارا;
    form_دھارا.append_str ("/SHa sh\n");

    hb_سمتیہ_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_عدد (bbox_x, precision);
    form.append_c (' ');
    form.append_عدد (bbox_y, precision);
    form.append_c (' ');
    form.append_عدد (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_عدد (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_دھارا.لمبائی);
    form.append_str (" >>\nstream\n");
    form.append_len (form_دھارا.arrayZ, form_دھارا.لمبائی);
    form.append_str ("endstream");
    غیرعلامتی form_id = جوڑو_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    غیرعلامتی idx = extgstate_گنتی++;
    hb_سمتیہ_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    غیرعلامتی gs_id = جوڑو_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    واپس idx;
  }

  /* Add a shading, return resource name index. */
  غیرعلامتی جوڑو_shading (hb_سمتیہ_buf_t &&shading_ڈیٹا)
  {
    غیرعلامتی obj_id = جوڑو_object (std::move (shading_ڈیٹا));
    واپس جوڑو_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  غیرعلامتی جوڑو_shading_by_id (غیرعلامتی obj_id)
  {
    غیرعلامتی idx = shading_گنتی++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    واپس idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  غیرعلامتی جوڑو_xobject_png_image (ثابت حرف *idat_ڈیٹا, غیرعلامتی idat_len,
				  غیرعلامتی چوڑائی, غیرعلامتی اونچائی,
				  غیرعلامتی colors, منطقی has_alpha,
				  ثابت uint8_t *plte = خالی_اشاریہ,
				  غیرعلامتی plte_len = 0,
				  ثابت uint8_t *trns = خالی_اشاریہ,
				  غیرعلامتی trns_len = 0)
  {
    غیرعلامتی idx = xobject_گنتی++;
    hb_سمتیہ_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (چوڑائی);
    obj.append_str (" /Height ");
    obj.append_unsigned (اونچائی);
    obj.append_str ("\n/BitsPerComponent 8\n");

    اگر (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      غیرعلامتی n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      برائے (غیرعلامتی i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    ورنہ
    {
      obj.append_str ("/ColorSpace ");
      غیرعلامتی color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    غیرعلامتی smask_id = 0;
    اگر (plte && trns && trns_len)
    {
      hb_سمتیہ_buf_t smask_دھارا;
      اگر (hb_pdf_build_اشاریہ_دار_smask (&smask_دھارا, idat_ڈیٹا, idat_len,
				      چوڑائی, اونچائی, trns, trns_len))
      {
	hb_سمتیہ_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (چوڑائی);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (اونچائی);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_دھارا.لمبائی);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_دھارا.arrayZ, smask_دھارا.لمبائی);
	smask_obj.append_str ("\nendstream");
	smask_id = جوڑو_object (std::move (smask_obj));
      }
    }

    اگر (smask_id)
    {
      obj.append_str ("/SMask ");
      obj.append_unsigned (smask_id);
      obj.append_str (" 0 R\n");
    }

    obj.append_str ("/Filter /FlateDecode\n");
    obj.append_str ("/DecodeParms << /Predictor 15 /Colors ");
    obj.append_unsigned (colors);
    obj.append_str (" /BitsPerComponent 8 /Columns ");
    obj.append_unsigned (چوڑائی);
    obj.append_str (" >>\n");
    obj.append_str ("/Length ");
    obj.append_unsigned (idat_len);
    obj.append_str (" >>\nstream\n");
    obj.append_len (idat_ڈیٹا, idat_len);
    obj.append_str ("\nendstream");

    (خالی) has_alpha;

    غیرعلامتی obj_id = جوڑو_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    واپس idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
ساکن hb_pdf_resources_t *
hb_pdf_get_resources (hb_سمتیہ_paint_t *paint)
{
  اگر (!paint->defs.لمبائی)
  {
    /* First call: allocate and store. */
    خودکار *res = (hb_pdf_resources_t *) hb_calloc (1, حجم (hb_pdf_resources_t));
    اگر (unlikely (!res)) واپس خالی_اشاریہ;
    نیا (res) hb_pdf_resources_t ();
    اگر (unlikely (!paint->defs.resize (حجم (خالی *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      واپس خالی_اشاریہ;
    }
    حافظہ_نقل (paint->defs.arrayZ, &res, حجم (خالی *));
  }
  hb_pdf_resources_t *res;
  حافظہ_نقل (&res, paint->defs.arrayZ, حجم (خالی *));
  واپس res;
}

خالی
hb_سمتیہ_paint_pdf_free_resources (hb_سمتیہ_paint_t *paint)
{
  اگر (paint->defs.لمبائی >= حجم (خالی *))
  {
    hb_pdf_resources_t *res;
    حافظہ_نقل (&res, paint->defs.arrayZ, حجم (خالی *));
    اگر (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
ساکن خالی
hb_pdf_emit_glyph_راستہ (hb_سمتیہ_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_سمتیہ_buf_t *buf)
{
  paint->راستہ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  اگر (likely (paint->budget_بقایا >= 0))
  {
    hb_سمتیہ_راستہ_sink_t sink = {&paint->راستہ, paint->get_precision (),
				 paint->x_scale_ضربی_عدد, paint->y_scale_ضربی_عدد,
				 &paint->budget_بقایا};
    hb_font_draw_glyph (font, glyph,
			hb_سمتیہ_pdf_راستہ_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->راستہ.arrayZ, paint->راستہ.لمبائی);
  paint->راستہ.clear ();
}


/* ---- paint callbacks ---- */

ساکن خالی
hb_pdf_paint_ڈالو_transform (hb_paint_funcs_t *,
			     خالی *paint_ڈیٹا,
			     اعشاری xx, اعشاری yx,
			     اعشاری xy, اعشاری yy,
			     اعشاری dx, اعشاری dy,
			     خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  خودکار &body = paint->current_body ();
  غیرعلامتی sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_عدد (xx, sprec);
  body.append_c (' ');
  body.append_عدد (yx, sprec);
  body.append_c (' ');
  body.append_عدد (xy, sprec);
  body.append_c (' ');
  body.append_عدد (yy, sprec);
  body.append_c (' ');
  body.append_عدد (paint->sx (dx));
  body.append_c (' ');
  body.append_عدد (paint->sy (dy));
  body.append_str (" cm\n");
}

ساکن خالی
hb_pdf_paint_نکالو_transform (hb_paint_funcs_t *,
			    خالی *paint_ڈیٹا,
			    خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  paint->current_body ().append_str ("Q\n");
}

ساکن خالی
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 خالی *paint_ڈیٹا,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  اعشاری r = hb_color_get_red (c) / 255.f;
  اعشاری g = hb_color_get_green (c) / 255.f;
  اعشاری b = hb_color_get_blue (c) / 255.f;
  اعشاری a = hb_color_get_alpha (c) / 255.f;

  اگر (a < 1.f / 255.f)
    واپس;

  خودکار &body = paint->current_body ();

  منطقی scoped = جھوٹ;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  اگر (a < 1.f - 1.f / 512.f)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    اگر (res)
    {
      غیرعلامتی gs_idx = res->جوڑو_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = سچ;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_عدد (r, 4);
  body.append_c (' ');
  body.append_عدد (g, 4);
  body.append_c (' ');
  body.append_عدد (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_راستہ (paint, font, glyph, &body);
  body.append_str ("f\n");
  اگر (scoped)
    body.append_str ("Q\n");
}

ساکن خالی
hb_pdf_paint_ڈالو_clip_glyph (hb_paint_funcs_t *,
			      خالی *paint_ڈیٹا,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_راستہ (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

ساکن خالی
hb_pdf_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *,
				  خالی *paint_ڈیٹا,
				  اعشاری xmin, اعشاری ymin,
				  اعشاری xmax, اعشاری ymax,
				  خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_عدد (paint->sx (xmin));
  body.append_c (' ');
  body.append_عدد (paint->sy (ymin));
  body.append_c (' ');
  body.append_عدد (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_عدد (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

ساکن hb_draw_funcs_t *
hb_pdf_paint_ڈالو_clip_راستہ_آغاز (hb_paint_funcs_t *,
				   خالی *paint_ڈیٹا,
				   خالی **draw_ڈیٹا,
				   خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
  {
    *draw_ڈیٹا = خالی_اشاریہ;
    واپس خالی_اشاریہ;
  }

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_راستہ_sink = {&body, paint->get_precision (),
			   paint->x_scale_ضربی_عدد,
			   paint->y_scale_ضربی_عدد,
			   &paint->budget_بقایا};
  *draw_ڈیٹا = &paint->clip_راستہ_sink;
  واپس hb_سمتیہ_pdf_راستہ_draw_funcs_get ();
}

ساکن خالی
hb_pdf_paint_ڈالو_clip_راستہ_انتہا (hb_paint_funcs_t *,
				 خالی *paint_ڈیٹا,
				 خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  paint->current_body ().append_str ("W n\n");
}

ساکن خالی
hb_pdf_paint_نکالو_clip (hb_paint_funcs_t *,
		       خالی *paint_ڈیٹا,
		       خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
ساکن خالی
hb_pdf_paint_solid_color (hb_سمتیہ_paint_t *paint, hb_color_t c)
{
  خودکار &body = paint->current_body ();

  اعشاری r = hb_color_get_red (c) / 255.f;
  اعشاری g = hb_color_get_green (c) / 255.f;
  اعشاری b = hb_color_get_blue (c) / 255.f;
  اعشاری a = hb_color_get_alpha (c) / 255.f;

  اگر (a < 1.f / 255.f)
    واپس;

  /* Set alpha via ExtGState if needed. */
  اگر (a < 1.f - 1.f / 512.f)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    اگر (res)
    {
      غیرعلامتی gs_idx = res->جوڑو_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_عدد (r, 4);
  body.append_c (' ');
  body.append_عدد (g, 4);
  body.append_c (' ');
  body.append_عدد (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

ساکن خالی
hb_pdf_paint_color (hb_paint_funcs_t *,
		    خالی *paint_ڈیٹا,
		    hb_bool_t,
		    hb_color_t color,
		    خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
ساکن منطقی
hb_pdf_build_اشاریہ_دار_smask (hb_سمتیہ_buf_t *برآمد,
			    ثابت حرف *idat_ڈیٹا, غیرعلامتی idat_len,
			    غیرعلامتی چوڑائی, غیرعلامتی اونچائی,
			    ثابت uint8_t *trns, غیرعلامتی trns_len)
{
#اگر_غیر_تعریف_شدہ HAVE_ZLIB
  (خالی) برآمد; (خالی) idat_ڈیٹا; (خالی) idat_len;
  (خالی) چوڑائی; (خالی) اونچائی; (خالی) trns; (خالی) trns_len;
  واپس جھوٹ;
#ورنہ
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  غیرعلامتی raw_len, نقاب_len;
  اگر (!چوڑائی || !اونچائی ||
      چوڑائی == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (چوڑائی + 1, اونچائی, &raw_len) ||
      hb_unsigned_mul_overflows (چوڑائی, اونچائی, &نقاب_len))
    واپس جھوٹ;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  اگر (!raw) واپس جھوٹ;
  HB_SCOPE_GUARD (hb_free (raw));

  z_دھارا دھارا = {};
  دھارا.اگلا_اندر = (Bytef *) idat_ڈیٹا;
  دھارا.avail_اندر = idat_len;
  دھارا.اگلا_برآمد = (Bytef *) raw;
  دھارا.avail_برآمد = raw_len;

  اگر (inflateInit (&دھارا) != Z_OK)
    واپس جھوٹ;
  صحیح حالت_2 = inflate (&دھارا, Z_اختتام);
  غیرعلامتی طویل کل_برآمد = دھارا.کل_برآمد;
  inflate_انتہا (&دھارا);
  اگر (حالت_2 != Z_دھارا_انتہا || کل_برآمد != raw_len)
    واپس جھوٹ;

  /* Un-filter and map to alpha. */
  اگر (!برآمد->resize (نقاب_len))
    واپس جھوٹ;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (چوڑائی);
  اگر (!unfiltered)
    واپس جھوٹ;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *پچھلا_unfiltered = (uint8_t *) hb_calloc (چوڑائی, 1);
  اگر (!پچھلا_unfiltered)
    واپس جھوٹ;
  HB_SCOPE_GUARD (hb_free (پچھلا_unfiltered));

  برائے (غیرعلامتی y = 0; y < اونچائی; y++)
  {
    uint8_t *صف = raw + y * (چوڑائی + 1);
    uint8_t filter = صف[0];
    uint8_t *pixels = صف + 1;

    برائے (غیرعلامتی x = 0; x < چوڑائی; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = پچھلا_unfiltered[x];
      uint8_t c = (x > 0) ? پچھلا_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      انتخاب (filter)
      {
      حالت 0: unfiltered[x] = val; توڑو; /* None */
      حالت 1: unfiltered[x] = val + a; توڑو; /* Sub */
      حالت 2: unfiltered[x] = val + b; توڑو; /*اوپر*/
      حالت 3: unfiltered[x] = val + (uint8_t) (((غیرعلامتی) a + b) / 2); توڑو; /*اوسط_قدر*/
      حالت 4: /* Paeth */
      {
	صحیح p = (صحیح) a + (صحیح) b - (صحیح) c;
	صحیح pa = مطلق_قدر (p - (صحیح) a);
	صحیح pb = مطلق_قدر (p - (صحیح) b);
	صحیح pc = مطلق_قدر (p - (صحیح) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	توڑو;
      }
      طےشدہ: unfiltered[x] = val; توڑو;
      }

      /* Map palette index to alpha. */
      uint8_t idx = unfiltered[x];
      برآمد->arrayZ[y * چوڑائی + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = پچھلا_unfiltered;
    پچھلا_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  واپس سچ;
#اگر_ختم
}

/* Read a big-endian uint32 from a byte pointer. */
ساکن اندرونی uint32_t
hb_pdf_png_u32 (ثابت uint8_t *p)
{
  واپس ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

ساکن hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    خالی *paint_ڈیٹا,
		    hb_blob_t *image,
		    غیرعلامتی چوڑائی,
		    غیرعلامتی اونچائی,
		    hb_لیبل_t format,
		    اعشاری slant HB_غیر_مستعمل,
		    hb_glyph_extents_t *extents,
		    خالی *)
{
  اگر (format != HB_لیبل ('p','n','g',' '))
    واپس جھوٹ;
  اگر (!extents || !چوڑائی || !اونچائی)
    واپس جھوٹ;

  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس جھوٹ;
  خودکار *res = hb_pdf_get_resources (paint);
  اگر (unlikely (!res))
    واپس جھوٹ;

  غیرعلامتی len = 0;
  ثابت uint8_t *ڈیٹا = (ثابت uint8_t *) hb_blob_get_ڈیٹا (image, &len);
  اگر (!ڈیٹا || len < 8)
    واپس جھوٹ;

  /* Verify PNG signature. */
  ساکن ثابت uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  اگر (hb_memcmp (ڈیٹا, png_sig, 8) != 0)
    واپس جھوٹ;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  غیرعلامتی png_w = 0, png_h = 0;
  غیرعلامتی colors = 3; /* default RGB */
  uint8_t color_type = 2;
  منطقی has_alpha = جھوٹ;
  ثابت uint8_t *plte_ڈیٹا = خالی_اشاریہ;
  غیرعلامتی plte_len = 0;
  ثابت uint8_t *trns_ڈیٹا = خالی_اشاریہ;
  غیرعلامتی trns_len = 0;
  hb_سمتیہ_buf_t idat;

  غیرعلامتی pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  جب (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (ڈیٹا + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (ڈیٹا + pos + 4);
    اگر (chunk_len > len - pos - 12)
      توڑو;
    ثابت uint8_t *chunk_ڈیٹا = ڈیٹا + pos + 8;

    اگر (chunk_type == 0x49484452u) /* IHDR */
    {
      اگر (chunk_len < 13) واپس جھوٹ;
      png_w = hb_pdf_png_u32 (chunk_ڈیٹا);
      png_h = hb_pdf_png_u32 (chunk_ڈیٹا + 4);
      uint8_t ثنائی_ہندسہ_گہرائی_2 = chunk_ڈیٹا[8];
      color_type = chunk_ڈیٹا[9];
      اگر (ثنائی_ہندسہ_گہرائی_2 != 8) واپس جھوٹ; /* only 8-bit supported */

      انتخاب (color_type)
      {
      حالت 0: colors = 1; has_alpha = جھوٹ; توڑو; /* Grayscale */
      حالت 2: colors = 3; has_alpha = جھوٹ; توڑو; /* RGB */
      حالت 3: colors = 1; has_alpha = جھوٹ; توڑو; /*اشاریہ_دار*/
      حالت 4: colors = 2; has_alpha = سچ;  توڑو; /* Gray+Alpha */
      حالت 6: colors = 4; has_alpha = سچ;  توڑو; /* RGBA */
      طےشدہ: واپس جھوٹ;
      }
    }
    ورنہ اگر (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_ڈیٹا = chunk_ڈیٹا;
      plte_len = chunk_len;
    }
    ورنہ اگر (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_ڈیٹا = chunk_ڈیٹا;
      trns_len = chunk_len;
    }
    ورنہ اگر (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ثابت حرف *) chunk_ڈیٹا, chunk_len);
    }
    ورنہ اگر (chunk_type == 0x49454E44u) /* IEND */
      توڑو;

    pos += 12 + chunk_len;
  }

  اگر (!png_w || !png_h || !idat.لمبائی)
    واپس جھوٹ;

  غیرعلامتی im_idx = res->جوڑو_xobject_png_image (idat.arrayZ, idat.لمبائی,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_ڈیٹا, plte_len,
						 trns_ڈیٹا, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  خودکار &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  اعشاری ix = (اعشاری) extents->x_bearing;
  اعشاری iy = (اعشاری) extents->y_bearing + (اعشاری) extents->اونچائی;
  اعشاری iw = (اعشاری) extents->چوڑائی;
  اعشاری ih = -(اعشاری) extents->اونچائی; /* negative because image Y goes up but height is negative in extents */

  body.append_عدد (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_عدد (paint->sy (ih));
  body.append_c (' ');
  body.append_عدد (paint->sx (ix));
  body.append_c (' ');
  body.append_عدد (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  واپس سچ;
}

/* ---- Gradient helpers ---- */

ساکن منطقی
hb_pdf_gradient_needs_alpha (hb_array_t<ثابت hb_color_stop_t> stops)
{
  برائے (ثابت خودکار &s : stops)
    اگر (hb_color_get_alpha (s.color) != 255)
      واپس سچ;
  واپس جھوٹ;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
ساکن خالی
hb_pdf_build_alpha_interpolation_function (hb_سمتیہ_buf_t *obj,
					   اعشاری a0, اعشاری a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_عدد (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_عدد (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
ساکن غیرعلامتی
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_سمتیہ_paint_t *paint)
{
  غیرعلامتی گنتی = paint->color_stops_scratch.لمبائی;

  اگر (گنتی < 2)
  {
    اعشاری a = گنتی ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    واپس res->جوڑو_object (std::move (obj));
  }

  اگر (گنتی == 2)
  {
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    واپس res->جوڑو_object (std::move (obj));
  }

  hb_سمتیہ_t<غیرعلامتی> sub_func_ids;
  برائے (غیرعلامتی i = 0; i + 1 < گنتی; i++)
  {
    hb_سمتیہ_buf_t sub;
    hb_pdf_build_alpha_interpolation_function (&sub,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i + 1].color) / 255.f);
    sub_func_ids.push (res->جوڑو_object (std::move (sub)));
  }

  hb_سمتیہ_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");
  obj.append_str ("/Functions [");
  برائے (غیرعلامتی i = 0; i < sub_func_ids.لمبائی; i++)
  {
    اگر (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  برائے (غیرعلامتی i = 1; i + 1 < گنتی; i++)
  {
    اگر (i > 1) obj.append_c (' ');
    obj.append_عدد (paint->color_stops_scratch.arrayZ[i].انحراف, 4);
  }
  obj.append_str ("]\n/Encode [");
  برائے (غیرعلامتی i = 0; i + 1 < گنتی; i++)
  {
    اگر (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  واپس res->جوڑو_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
ساکن خالی
hb_pdf_build_interpolation_function (hb_سمتیہ_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_عدد (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_عدد (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_عدد (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_عدد (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_عدد (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_عدد (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
ساکن غیرعلامتی
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_سمتیہ_paint_t *paint)
{
  غیرعلامتی گنتی = paint->color_stops_scratch.لمبائی;

  اگر (گنتی < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = گنتی ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    واپس res->جوڑو_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { واپس (a.انحراف > b.انحراف) - (a.انحراف < b.انحراف); });

  اگر (گنتی == 2)
  {
    /* Two stops: single interpolation function. */
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    واپس res->جوڑو_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_سمتیہ_t<غیرعلامتی> sub_func_ids;
  برائے (غیرعلامتی i = 0; i + 1 < گنتی; i++)
  {
    hb_سمتیہ_buf_t sub;
    hb_pdf_build_interpolation_function (&sub,
					  paint->color_stops_scratch.arrayZ[i].color,
					  paint->color_stops_scratch.arrayZ[i + 1].color);
    sub_func_ids.push (res->جوڑو_object (std::move (sub)));
  }

  /* Stitching function (Type 3). */
  hb_سمتیہ_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");

  /* Functions array. */
  obj.append_str ("/Functions [");
  برائے (غیرعلامتی i = 0; i < sub_func_ids.لمبائی; i++)
  {
    اگر (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  برائے (غیرعلامتی i = 1; i + 1 < گنتی; i++)
  {
    اگر (i > 1) obj.append_c (' ');
    obj.append_عدد (paint->color_stops_scratch.arrayZ[i].انحراف, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  برائے (غیرعلامتی i = 0; i + 1 < گنتی; i++)
  {
    اگر (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  واپس res->جوڑو_object (std::move (obj));
}

ساکن خالی
hb_pdf_paint_خطی_gradient (hb_paint_funcs_t *,
			      خالی *paint_ڈیٹا,
			      hb_color_سطر_t *color_سطر,
			      اعشاری x0, اعشاری y0,
			      اعشاری x1, اعشاری y1,
			      اعشاری x2 HB_غیر_مستعمل, اعشاری y2 HB_غیر_مستعمل,
			      خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  خودکار *res = hb_pdf_get_resources (paint);
  اگر (unlikely (!res))
    واپس;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  اگر (!paint->fetch_color_stops (color_سطر))
    واپس;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمبائی, &mn, &mx);
  اعشاری gx0 = x0 + mn * (x1 - x0);
  اعشاری gy0 = y0 + mn * (y1 - y0);
  اعشاری gx1 = x0 + mx * (x1 - x0);
  اعشاری gy1 = y0 + mx * (y1 - y0);

  غیرعلامتی func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت حرف *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_سمتیہ_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_عدد (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_عدد (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_عدد (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_عدد (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  غیرعلامتی sh_idx = res->جوڑو_shading (std::move (sh));

  خودکار &body = paint->current_body ();

  منطقی needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  اگر (needs_alpha)
  {
    غیرعلامتی alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_سمتیہ_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_عدد (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_عدد (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_عدد (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_عدد (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    غیرعلامتی alpha_sh_id = res->جوڑو_object (std::move (ash));

    غیرعلامتی gs_idx = res->جوڑو_extgstate_smask (alpha_sh_id,
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

ساکن خالی
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      خالی *paint_ڈیٹا,
			      hb_color_سطر_t *color_سطر,
			      اعشاری x0, اعشاری y0, اعشاری r0,
			      اعشاری x1, اعشاری y1, اعشاری r1,
			      خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  خودکار *res = hb_pdf_get_resources (paint);
  اگر (unlikely (!res))
    واپس;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  اگر (!paint->fetch_color_stops (color_سطر))
    واپس;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  اعشاری mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمبائی, &mn, &mx);
  اعشاری gx0 = x0 + mn * (x1 - x0);
  اعشاری gy0 = y0 + mn * (y1 - y0);
  اعشاری gr0 = r0 + mn * (r1 - r0);
  اعشاری gx1 = x0 + mx * (x1 - x0);
  اعشاری gy1 = y0 + mx * (y1 - y0);
  اعشاری gr1 = r0 + mx * (r1 - r0);

  غیرعلامتی func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت حرف *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_سمتیہ_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_عدد (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_عدد (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_عدد (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_عدد (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_عدد (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_عدد (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  غیرعلامتی sh_idx = res->جوڑو_shading (std::move (sh));

  خودکار &body = paint->current_body ();

  منطقی needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  اگر (needs_alpha)
  {
    غیرعلامتی alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_سمتیہ_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_عدد (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_عدد (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_عدد (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_عدد (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_عدد (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_عدد (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    غیرعلامتی alpha_sh_id = res->جوڑو_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    اعشاری cx = (gr1 >= gr0) ? gx1 : gx0;
    اعشاری cy = (gr1 >= gr0) ? gy1 : gy0;
    اعشاری rr = hb_max (gr0, gr1);
    غیرعلامتی gs_idx = res->جوڑو_extgstate_smask (alpha_sh_id,
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
ساکن خالی
hb_pdf_encode_u16 (hb_سمتیہ_buf_t *buf, uint16_t v)
{
  حرف بائٹیں[2] = {(حرف) (v >> 8), (حرف) (v & 0xFF)};
  buf->append_len (بائٹیں, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
ساکن خالی
hb_pdf_encode_coord (hb_سمتیہ_buf_t *buf,
		     اعشاری val, اعشاری lo, اعشاری hi)
{
  اعشاری t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
ساکن خالی
hb_pdf_encode_color_rgb (hb_سمتیہ_buf_t *buf, hb_color_t c)
{
  حرف rgb[3] = {(حرف) hb_color_get_red (c),
		 (حرف) hb_color_get_green (c),
		 (حرف) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
ساکن خالی
hb_pdf_encode_color_alpha (hb_سمتیہ_buf_t *buf, hb_color_t c)
{
  حرف a = (حرف) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
ساکن خالی
hb_pdf_encode_نقطہ (hb_سمتیہ_buf_t *buf,
		     اعشاری x, اعشاری y,
		     اعشاری xlo, اعشاری xhi,
		     اعشاری ylo, اعشاری yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
ساکن خالی
hb_pdf_جوڑو_sweep_patch (hb_سمتیہ_buf_t *mesh,
			hb_سمتیہ_buf_t *alpha_mesh,
			اعشاری cx, اعشاری cy,
			اعشاری xlo, اعشاری xhi, اعشاری ylo, اعشاری yhi,
			اعشاری a0, hb_color_t c0_اندر,
			اعشاری a1, hb_color_t c1_اندر)
{
  ثابت اعشاری R = 32767.f;
  ثابت اعشاری eps = 0.5f;
  ثابت اعشاری MAX_SECTOR = (اعشاری) M_PI / 2.f;

  صحیح عدد_splits = (صحیح) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  اگر (عدد_splits < 1) عدد_splits = 1;

  برائے (صحیح s = 0; s < عدد_splits; s++)
  {
    اعشاری k0 = (اعشاری) s / عدد_splits;
    اعشاری k1 = (اعشاری) (s + 1) / عدد_splits;
    اعشاری sa0 = a0 + k0 * (a1 - a0);
    اعشاری sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_اندر, c1_اندر, k0);
    hb_color_t sc1 = hb_color_lerp (c0_اندر, c1_اندر, k1);

    اعشاری da = sa1 - sa0;
    اعشاری kappa = (4.f / 3.f) * tanf (da / 4.f);

    اعشاری cos0 = cosf (sa0), sin0 = sinf (sa0);
    اعشاری cos1 = cosf (sa1), sin1 = sinf (sa1);

    اعشاری p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    اعشاری p3x = cx + R * cos0,   p3y = cy + R * sin0;
    اعشاری p6x = cx + R * cos1,   p6y = cy + R * sin1;
    اعشاری p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    اعشاری e1_1x = p0x + (p3x - p0x) / 3.f;
    اعشاری e1_1y = p0y + (p3y - p0y) / 3.f;
    اعشاری e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    اعشاری e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    اعشاری e2_1x = p3x + kappa * R * (-sin0);
    اعشاری e2_1y = p3y + kappa * R * ( cos0);
    اعشاری e2_2x = p6x - kappa * R * (-sin1);
    اعشاری e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    اعشاری e3_1x = p6x + (p9x - p6x) / 3.f;
    اعشاری e3_1y = p6y + (p9y - p6y) / 3.f;
    اعشاری e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    اعشاری e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    اعشاری e4_1x = p9x + kappa * eps * (-sin1);
    اعشاری e4_1y = p9y + kappa * eps * ( cos1);
    اعشاری e4_2x = p0x - kappa * eps * (-sin0);
    اعشاری e4_2y = p0y - kappa * eps * ( cos0);

    mesh->append_c ('\0'); /* flag = 0, new patch */

    hb_pdf_encode_نقطہ (mesh, p0x, p0y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, p3x, p3y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, p6x, p6y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, p9x, p9y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطہ (mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

    hb_pdf_encode_color_rgb (mesh, sc0); /* inner start */
    hb_pdf_encode_color_rgb (mesh, sc0); /* outer start */
    hb_pdf_encode_color_rgb (mesh, sc1); /* outer end */
    hb_pdf_encode_color_rgb (mesh, sc1); /* inner end */

    اگر (alpha_mesh)
    {
      alpha_mesh->append_c ('\0');

      hb_pdf_encode_نقطہ (alpha_mesh, p0x, p0y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, p3x, p3y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, p6x, p6y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, p9x, p9y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطہ (alpha_mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
    }
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ساخت hb_pdf_sweep_ctx_t {
  hb_سمتیہ_buf_t *mesh;
  hb_سمتیہ_buf_t *alpha_mesh;
  اعشاری cx, cy, xlo, xhi, ylo, yhi;
};

ساکن خالی
hb_pdf_sweep_emit_patch (اعشاری a0, hb_color_t c0,
			 اعشاری a1, hb_color_t c1,
			 خالی *user_ڈیٹا)
{
  خودکار *ctx = (hb_pdf_sweep_ctx_t *) user_ڈیٹا;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  اگر (unlikely (ctx->mesh->اندر_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->اندر_error ())))
    واپس;
  hb_pdf_جوڑو_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

ساکن خالی
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     خالی *paint_ڈیٹا,
			     hb_color_سطر_t *color_سطر,
			     اعشاری cx, اعشاری cy,
			     اعشاری آغاز_angle,
			     اعشاری انتہا_angle,
			     خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  خودکار *res = hb_pdf_get_resources (paint);
  اگر (unlikely (!res))
    واپس;

  /* Get and sort color stops. */
  اگر (!paint->fetch_color_stops (color_سطر))
    واپس;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { واپس (a.انحراف > b.انحراف) - (a.انحراف < b.انحراف); });

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  ثابت اعشاری R = 32767.f;
  اعشاری scx = paint->sx (cx), scy = paint->sy (cy);
  اعشاری xlo = scx - R - 1, xhi = scx + R + 1;
  اعشاری ylo = scy - R - 1, yhi = scy + R + 1;

  منطقی needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_سمتیہ_buf_t mesh;
  hb_سمتیہ_buf_t alpha_mesh;
  mesh.alloc (256);
  اگر (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : خالی_اشاریہ,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.لمبائی, extend,
				 آغاز_angle, انتہا_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  اگر (!mesh.لمبائی)
    واپس;

  خودکار hb_pdf_build_mesh_shading = [&] (hb_سمتیہ_buf_t &m,
					 ثابت حرف *cs,
					 ثابت حرف *decode_suffix) -> غیرعلامتی
  {
    hb_سمتیہ_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_عدد (xlo, 2);
    sh.append_c (' ');
    sh.append_عدد (xhi, 2);
    sh.append_c (' ');
    sh.append_عدد (ylo, 2);
    sh.append_c (' ');
    sh.append_عدد (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.لمبائی);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.لمبائی);
    sh.append_str ("\nendstream");
    واپس res->جوڑو_object (std::move (sh));
  };

  غیرعلامتی sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  غیرعلامتی sh_idx = res->جوڑو_shading_by_id (sh_obj_id);

  خودکار &body = paint->current_body ();

  اگر (needs_alpha && alpha_mesh.لمبائی)
  {
    غیرعلامتی alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    غیرعلامتی gs_idx = res->جوڑو_extgstate_smask (alpha_sh_id,
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

ساکن ثابت حرف *
hb_pdf_blend_منوال_name (hb_paint_مرکب_منوال_t منوال)
{
  انتخاب (منوال)
  {
  حالت HB_PAINT_مرکب_منوال_MULTIPLY:       واپس "Multiply";
  حالت HB_PAINT_مرکب_منوال_SCREEN:         واپس "Screen";
  حالت HB_PAINT_مرکب_منوال_OVERLAY:        واپس "Overlay";
  حالت HB_PAINT_مرکب_منوال_DARKEN:         واپس "Darken";
  حالت HB_PAINT_مرکب_منوال_LIGHTEN:        واپس "Lighten";
  حالت HB_PAINT_مرکب_منوال_COLOR_DODGE:    واپس "ColorDodge";
  حالت HB_PAINT_مرکب_منوال_COLOR_BURN:     واپس "ColorBurn";
  حالت HB_PAINT_مرکب_منوال_HARD_LIGHT:     واپس "HardLight";
  حالت HB_PAINT_مرکب_منوال_SOFT_LIGHT:     واپس "SoftLight";
  حالت HB_PAINT_مرکب_منوال_DIFFERENCE:     واپس "Difference";
  حالت HB_PAINT_مرکب_منوال_EXCLUSION:      واپس "Exclusion";
  حالت HB_PAINT_مرکب_منوال_HSL_HUE:        واپس "Hue";
  حالت HB_PAINT_مرکب_منوال_HSL_SATURATION: واپس "Saturation";
  حالت HB_PAINT_مرکب_منوال_HSL_COLOR:      واپس "Color";
  حالت HB_PAINT_مرکب_منوال_HSL_LUMINOSITY: واپس "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  حالت HB_PAINT_مرکب_منوال_PLUS:           واپس "Screen";
  حالت HB_PAINT_مرکب_منوال_XOR:            واپس "Difference";
  حالت HB_PAINT_مرکب_منوال_CLEAR:
  حالت HB_PAINT_مرکب_منوال_SRC:
  حالت HB_PAINT_مرکب_منوال_DEST:
  حالت HB_PAINT_مرکب_منوال_SRC_OVER:
  حالت HB_PAINT_مرکب_منوال_DEST_OVER:
  حالت HB_PAINT_مرکب_منوال_SRC_اندر:
  حالت HB_PAINT_مرکب_منوال_DEST_اندر:
  حالت HB_PAINT_مرکب_منوال_SRC_برآمد:
  حالت HB_PAINT_مرکب_منوال_DEST_برآمد:
  حالت HB_PAINT_مرکب_منوال_SRC_ATOP:
  حالت HB_PAINT_مرکب_منوال_DEST_ATOP:
  طےشدہ:                                     واپس خالی_اشاریہ; /*معمول*/
  }
}

ساکن خالی
hb_pdf_paint_ڈالو_گروہ (hb_paint_funcs_t *,
			 خالی *paint_ڈیٹا,
			 خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  paint->current_body ().append_str ("q\n");
}

ساکن خالی
hb_pdf_paint_ڈالو_گروہ_for (hb_paint_funcs_t *,
			     خالی *paint_ڈیٹا,
			     hb_paint_مرکب_منوال_t منوال,
			     خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");

  ثابت حرف *bm = hb_pdf_blend_منوال_name (منوال);
  اگر (bm)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    اگر (likely (res))
    {
      غیرعلامتی gs_idx = res->جوڑو_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

ساکن خالی
hb_pdf_paint_نکالو_گروہ (hb_paint_funcs_t *,
			خالی *paint_ڈیٹا,
			hb_paint_مرکب_منوال_t منوال HB_غیر_مستعمل,
			خالی *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_ڈیٹا;
  اگر (unlikely (!paint->ensure_initialized ()))
    واپس;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

ساکن اندرونی خالی free_static_pdf_paint_funcs ();

ساکن ساخت hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  ساکن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, (hb_paint_ڈالو_transform_func_t) hb_pdf_paint_ڈالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, (hb_paint_نکالو_transform_func_t) hb_pdf_paint_نکالو_transform, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, (hb_paint_ڈالو_clip_glyph_func_t) hb_pdf_paint_ڈالو_clip_glyph, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, (hb_paint_ڈالو_clip_rectangle_func_t) hb_pdf_paint_ڈالو_clip_rectangle, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_آغاز_func (funcs, (hb_paint_ڈالو_clip_راستہ_آغاز_func_t) hb_pdf_paint_ڈالو_clip_راستہ_آغاز, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_clip_راستہ_انتہا_func (funcs, (hb_paint_ڈالو_clip_راستہ_انتہا_func_t) hb_pdf_paint_ڈالو_clip_راستہ_انتہا, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, (hb_paint_نکالو_clip_func_t) hb_pdf_paint_نکالو_clip, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, (hb_paint_خطی_gradient_func_t) hb_pdf_paint_خطی_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, (hb_paint_ڈالو_گروہ_func_t) hb_pdf_paint_ڈالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_for_func (funcs, (hb_paint_ڈالو_گروہ_for_func_t) hb_pdf_paint_ڈالو_گروہ_for, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, (hb_paint_نکالو_گروہ_func_t) hb_pdf_paint_نکالو_گروہ, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_سمتیہ_paint_سیٹ_budget_callback, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_سمتیہ_paint_get_budget_callback, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_سیٹ_get_budget_بقایا_func (funcs, hb_سمتیہ_paint_get_budget_بقایا_callback, خالی_اشاریہ, خالی_اشاریہ);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    واپس funcs;
  }
} static_pdf_paint_funcs;

ساکن اندرونی خالی
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_سمتیہ_paint_pdf_funcs_get ()
{
  واپس static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_سمتیہ_paint_render_pdf (hb_سمتیہ_paint_t *paint)
{
  اگر (!paint->has_extents)
    واپس خالی_اشاریہ;
  اگر (!paint->گروہ_ڈھیر.لمبائی ||
      !paint->گروہ_ڈھیر.arrayZ[0].لمبائی)
    واپس خالی_اشاریہ;

  hb_سمتیہ_buf_t &content = paint->گروہ_ڈھیر.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  اعشاری ex = paint->extents.x;
  اعشاری ey = paint->extents.y;
  اعشاری ew = paint->extents.چوڑائی;
  اعشاری eh = paint->extents.اونچائی;

  غیرعلامتی عدد_extra = res ? res->objects.لمبائی : 0;
  غیرعلامتی کل_objects = 4 + عدد_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_سمتیہ_buf_t برآمد;
  hb_buf_recover_recycled (paint->recycled_blob, &برآمد);
  برآمد.alloc (content.لمبائی + عدد_extra * 128 + 1024);

  hb_سمتیہ_t<غیرعلامتی> offsets;
  اگر (unlikely (!offsets.resize (کل_objects)))
    واپس خالی_اشاریہ;

  برآمد.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = برآمد.لمبائی;
  برآمد.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = برآمد.لمبائی;
  برآمد.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = برآمد.لمبائی;
  برآمد.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  برآمد.append_عدد (ex);
  برآمد.append_c (' ');
  برآمد.append_عدد (ey);
  برآمد.append_c (' ');
  برآمد.append_عدد (ex + ew);
  برآمد.append_c (' ');
  برآمد.append_عدد (ey + eh);
  برآمد.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  منطقی has_resources = res &&
    (res->extgstate_dict.لمبائی || res->shading_dict.لمبائی || res->xobject_dict.لمبائی);
  اگر (has_resources)
  {
    برآمد.append_str ("\n/Resources <<");
    اگر (res->extgstate_dict.لمبائی)
    {
      برآمد.append_str (" /ExtGState << ");
      برآمد.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.لمبائی);
      برآمد.append_str (">>");
    }
    اگر (res->shading_dict.لمبائی)
    {
      برآمد.append_str (" /Shading << ");
      برآمد.append_len (res->shading_dict.arrayZ, res->shading_dict.لمبائی);
      برآمد.append_str (">>");
    }
    اگر (res->xobject_dict.لمبائی)
    {
      برآمد.append_str (" /XObject << ");
      برآمد.append_len (res->xobject_dict.arrayZ, res->xobject_dict.لمبائی);
      برآمد.append_str (">>");
    }
    برآمد.append_str (" >>");
  }

  برآمد.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_سمتیہ_buf_t bg_سابقہ;
  اگر (hb_color_get_alpha (paint->background))
  {
    اعشاری r = hb_color_get_red (paint->background) / 255.f;
    اعشاری g = hb_color_get_green (paint->background) / 255.f;
    اعشاری b = hb_color_get_blue (paint->background) / 255.f;
    اعشاری a = hb_color_get_alpha (paint->background) / 255.f;
    اگر (a < 1.f - 1.f / 512.f)
    {
      اگر (res)
      {
	غیرعلامتی gs_idx = res->جوڑو_extgstate_alpha (a);
	bg_سابقہ.append_str ("/GS");
	bg_سابقہ.append_unsigned (gs_idx);
	bg_سابقہ.append_str (" gs\n");
      }
    }
    bg_سابقہ.append_عدد (r, 4);
    bg_سابقہ.append_c (' ');
    bg_سابقہ.append_عدد (g, 4);
    bg_سابقہ.append_c (' ');
    bg_سابقہ.append_عدد (b, 4);
    bg_سابقہ.append_str (" rg\n");
    bg_سابقہ.append_عدد (ex);
    bg_سابقہ.append_c (' ');
    bg_سابقہ.append_عدد (ey);
    bg_سابقہ.append_c (' ');
    bg_سابقہ.append_عدد (ew);
    bg_سابقہ.append_c (' ');
    bg_سابقہ.append_عدد (eh);
    bg_سابقہ.append_str (" re f\n");
  }
  غیرعلامتی دھارا_len = bg_سابقہ.لمبائی + content.لمبائی;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = برآمد.لمبائی;
  برآمد.append_str ("4 0 obj\n<< /Length ");
  برآمد.append_unsigned (دھارا_len);
  برآمد.append_str (" >>\nstream\n");
  برآمد.append_len (bg_سابقہ.arrayZ, bg_سابقہ.لمبائی);
  برآمد.append_len (content.arrayZ, content.لمبائی);
  برآمد.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  برائے (غیرعلامتی i = 0; i < عدد_extra; i++)
  {
    offsets.arrayZ[4 + i] = برآمد.لمبائی;
    برآمد.append_unsigned (5 + i);
    برآمد.append_str (" 0 obj\n");
    خودکار &obj = res->objects.arrayZ[i];
    برآمد.append_len (obj.ڈیٹا.arrayZ, obj.ڈیٹا.لمبائی);
    برآمد.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  غیرعلامتی xref_انحراف = برآمد.لمبائی;
  برآمد.append_str ("xref\n0 ");
  برآمد.append_unsigned (کل_objects + 1);
  برآمد.append_str ("\n0000000000 65535 f \n");
  برائے (غیرعلامتی i = 0; i < کل_objects; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    برآمد.append_len (tmp, 20);
  }

  /* Trailer */
  برآمد.append_str ("trailer\n<< /Size ");
  برآمد.append_unsigned (کل_objects + 1);
  برآمد.append_str (" /Root 1 0 R >>\nstartxref\n");
  برآمد.append_unsigned (xref_انحراف);
  برآمد.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &برآمد);

  واپس blob;
}

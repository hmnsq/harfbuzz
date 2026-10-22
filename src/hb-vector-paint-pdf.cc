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

#ضمن "hb.hh"

#ضمن "hb-vector-paint.hh"
#ضمن "hb-vector-draw.hh"
#ضمن "hb-paint.hh"

#ضمن <math.h>
#ضمن <stdio.h>

#جے_تعریف_شدہ HAVE_ZLIB
#ضمن <zlib.h>
#جے_انت

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

بنية hb_pdf_obj_t
{
  hb_سمتیہ_buf_t انکڑے;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
ساكن منطقي
hb_pdf_build_اشاریہ_دار_smask (hb_سمتیہ_buf_t *باہرلا,
			    ثابت حرف *idat_انکڑے, بنانشانی idat_len,
			    بنانشانی چوڑائی, بنانشانی اچائی,
			    ثابت uint8_t *trns, بنانشانی trns_len);

بنية hb_pdf_resources_t
{
  hb_سمتیہ_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_سمتیہ_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_سمتیہ_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_سمتیہ_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  بنانشانی extgstate_گنتی = 0;
  بنانشانی shading_گنتی = 0;
  بنانشانی xobject_گنتی = 0;

  بنانشانی جوڑو_object (hb_سمتیہ_buf_t &&obj_انکڑے)
  {
    بنانشانی id = 5 + objects.لمائی; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.انکڑے = std::move (obj_انکڑے);
    objects.push (std::move (obj));
    أرجع id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  بنانشانی جوڑو_extgstate_alpha (عائم alpha)
  {
    بنانشانی idx = extgstate_گنتی++;
    hb_سمتیہ_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_عدد (alpha, 4);
    obj.append_str (" >>");
    بنانشانی obj_id = جوڑو_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  بنانشانی جوڑو_extgstate_blend (ثابت حرف *bm)
  {
    بنانشانی idx = extgstate_گنتی++;
    hb_سمتیہ_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    بنانشانی obj_id = جوڑو_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  بنانشانی جوڑو_extgstate_smask (بنانشانی alpha_shading_id,
				عائم bbox_x, عائم bbox_y,
				عائم bbox_w, عائم bbox_h,
				بنانشانی precision)
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
    form.append_unsigned (form_دھارا.لمائی);
    form.append_str (" >>\nstream\n");
    form.append_len (form_دھارا.arrayZ, form_دھارا.لمائی);
    form.append_str ("endstream");
    بنانشانی form_id = جوڑو_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    بنانشانی idx = extgstate_گنتی++;
    hb_سمتیہ_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    بنانشانی gs_id = جوڑو_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add a shading, return resource name index. */
  بنانشانی جوڑو_shading (hb_سمتیہ_buf_t &&shading_انکڑے)
  {
    بنانشانی obj_id = جوڑو_object (std::move (shading_انکڑے));
    أرجع جوڑو_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  بنانشانی جوڑو_shading_by_id (بنانشانی obj_id)
  {
    بنانشانی idx = shading_گنتی++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  بنانشانی جوڑو_xobject_png_image (ثابت حرف *idat_انکڑے, بنانشانی idat_len,
				  بنانشانی چوڑائی, بنانشانی اچائی,
				  بنانشانی colors, منطقي has_alpha,
				  ثابت uint8_t *plte = خالی_سوچک,
				  بنانشانی plte_len = 0,
				  ثابت uint8_t *trns = خالی_سوچک,
				  بنانشانی trns_len = 0)
  {
    بنانشانی idx = xobject_گنتی++;
    hb_سمتیہ_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (چوڑائی);
    obj.append_str (" /Height ");
    obj.append_unsigned (اچائی);
    obj.append_str ("\n/BitsPerComponent 8\n");

    إذا (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      بنانشانی n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      لكل (بنانشانی i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    وإلا
    {
      obj.append_str ("/ColorSpace ");
      بنانشانی color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    بنانشانی smask_id = 0;
    إذا (plte && trns && trns_len)
    {
      hb_سمتیہ_buf_t smask_دھارا;
      إذا (hb_pdf_build_اشاریہ_دار_smask (&smask_دھارا, idat_انکڑے, idat_len,
				      چوڑائی, اچائی, trns, trns_len))
      {
	hb_سمتیہ_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (چوڑائی);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (اچائی);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_دھارا.لمائی);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_دھارا.arrayZ, smask_دھارا.لمائی);
	smask_obj.append_str ("\nendstream");
	smask_id = جوڑو_object (std::move (smask_obj));
      }
    }

    إذا (smask_id)
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
    obj.append_len (idat_انکڑے, idat_len);
    obj.append_str ("\nendstream");

    (فراغ) has_alpha;

    بنانشانی obj_id = جوڑو_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    أرجع idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
ساكن hb_pdf_resources_t *
hb_pdf_get_resources (hb_سمتیہ_paint_t *paint)
{
  إذا (!paint->defs.لمائی)
  {
    /* First call: allocate and store. */
    خودکار *res = (hb_pdf_resources_t *) hb_calloc (1, حجم (hb_pdf_resources_t));
    إذا (unlikely (!res)) أرجع خالی_سوچک;
    جديد (res) hb_pdf_resources_t ();
    إذا (unlikely (!paint->defs.resize (حجم (فراغ *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      أرجع خالی_سوچک;
    }
    حافظہ_نقل (paint->defs.arrayZ, &res, حجم (فراغ *));
  }
  hb_pdf_resources_t *res;
  حافظہ_نقل (&res, paint->defs.arrayZ, حجم (فراغ *));
  أرجع res;
}

فراغ
hb_سمتیہ_paint_pdf_free_resources (hb_سمتیہ_paint_t *paint)
{
  إذا (paint->defs.لمائی >= حجم (فراغ *))
  {
    hb_pdf_resources_t *res;
    حافظہ_نقل (&res, paint->defs.arrayZ, حجم (فراغ *));
    إذا (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
ساكن فراغ
hb_pdf_emit_glyph_رستہ (hb_سمتیہ_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_سمتیہ_buf_t *buf)
{
  paint->رستہ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_بچیا >= 0))
  {
    hb_سمتیہ_رستہ_sink_t sink = {&paint->رستہ, paint->get_precision (),
				 paint->x_scale_ضارب, paint->y_scale_ضارب,
				 &paint->budget_بچیا};
    hb_font_draw_glyph (font, glyph,
			hb_سمتیہ_pdf_رستہ_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->رستہ.arrayZ, paint->رستہ.لمائی);
  paint->رستہ.clear ();
}


/* ---- paint callbacks ---- */

ساكن فراغ
hb_pdf_paint_ڈالو_transform (hb_paint_funcs_t *,
			     فراغ *paint_انکڑے,
			     عائم xx, عائم yx,
			     عائم xy, عائم yy,
			     عائم dx, عائم dy,
			     فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  بنانشانی sprec = body.scale_precision ();
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

ساكن فراغ
hb_pdf_paint_نکالو_transform (hb_paint_funcs_t *,
			    فراغ *paint_انکڑے,
			    فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}

ساكن فراغ
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 فراغ *paint_انکڑے,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  عائم r = hb_color_get_red (c) / 255.f;
  عائم g = hb_color_get_green (c) / 255.f;
  عائم b = hb_color_get_blue (c) / 255.f;
  عائم a = hb_color_get_alpha (c) / 255.f;

  إذا (a < 1.f / 255.f)
    أرجع;

  خودکار &body = paint->current_body ();

  منطقي scoped = خطأ;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  إذا (a < 1.f - 1.f / 512.f)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    إذا (res)
    {
      بنانشانی gs_idx = res->جوڑو_extgstate_alpha (a);
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
  hb_pdf_emit_glyph_رستہ (paint, font, glyph, &body);
  body.append_str ("f\n");
  إذا (scoped)
    body.append_str ("Q\n");
}

ساكن فراغ
hb_pdf_paint_ڈالو_clip_glyph (hb_paint_funcs_t *,
			      فراغ *paint_انکڑے,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_رستہ (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

ساكن فراغ
hb_pdf_paint_ڈالو_clip_rectangle (hb_paint_funcs_t *,
				  فراغ *paint_انکڑے,
				  عائم xmin, عائم ymin,
				  عائم xmax, عائم ymax,
				  فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

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

ساكن hb_draw_funcs_t *
hb_pdf_paint_ڈالو_clip_رستہ_شروع (hb_paint_funcs_t *,
				   فراغ *paint_انکڑے,
				   فراغ **draw_انکڑے,
				   فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_انکڑے = خالی_سوچک;
    أرجع خالی_سوچک;
  }

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_رستہ_sink = {&body, paint->get_precision (),
			   paint->x_scale_ضارب,
			   paint->y_scale_ضارب,
			   &paint->budget_بچیا};
  *draw_انکڑے = &paint->clip_رستہ_sink;
  أرجع hb_سمتیہ_pdf_رستہ_draw_funcs_get ();
}

ساكن فراغ
hb_pdf_paint_ڈالو_clip_رستہ_سرا (hb_paint_funcs_t *,
				 فراغ *paint_انکڑے,
				 فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("W n\n");
}

ساكن فراغ
hb_pdf_paint_نکالو_clip (hb_paint_funcs_t *,
		       فراغ *paint_انکڑے,
		       فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
ساكن فراغ
hb_pdf_paint_solid_color (hb_سمتیہ_paint_t *paint, hb_color_t c)
{
  خودکار &body = paint->current_body ();

  عائم r = hb_color_get_red (c) / 255.f;
  عائم g = hb_color_get_green (c) / 255.f;
  عائم b = hb_color_get_blue (c) / 255.f;
  عائم a = hb_color_get_alpha (c) / 255.f;

  إذا (a < 1.f / 255.f)
    أرجع;

  /* Set alpha via ExtGState if needed. */
  إذا (a < 1.f - 1.f / 512.f)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    إذا (res)
    {
      بنانشانی gs_idx = res->جوڑو_extgstate_alpha (a);
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

ساكن فراغ
hb_pdf_paint_color (hb_paint_funcs_t *,
		    فراغ *paint_انکڑے,
		    hb_bool_t,
		    hb_color_t color,
		    فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
ساكن منطقي
hb_pdf_build_اشاریہ_دار_smask (hb_سمتیہ_buf_t *باہرلا,
			    ثابت حرف *idat_انکڑے, بنانشانی idat_len,
			    بنانشانی چوڑائی, بنانشانی اچائی,
			    ثابت uint8_t *trns, بنانشانی trns_len)
{
#جے_بنا_تعریف HAVE_ZLIB
  (فراغ) باہرلا; (فراغ) idat_انکڑے; (فراغ) idat_len;
  (فراغ) چوڑائی; (فراغ) اچائی; (فراغ) trns; (فراغ) trns_len;
  أرجع خطأ;
#وإلا
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  بنانشانی raw_len, نقاب_len;
  إذا (!چوڑائی || !اچائی ||
      چوڑائی == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (چوڑائی + 1, اچائی, &raw_len) ||
      hb_unsigned_mul_overflows (چوڑائی, اچائی, &نقاب_len))
    أرجع خطأ;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  إذا (!raw) أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (raw));

  z_دھارا دھارا = {};
  دھارا.اگلا_وچ = (Bytef *) idat_انکڑے;
  دھارا.avail_وچ = idat_len;
  دھارا.اگلا_باہرلا = (Bytef *) raw;
  دھارا.avail_باہرلا = raw_len;

  إذا (inflateInit (&دھارا) != Z_OK)
    أرجع خطأ;
  صحيح حالت = inflate (&دھارا, Z_انت);
  بنانشانی لما کل_باہرلا = دھارا.کل_باہرلا;
  inflate_سرا (&دھارا);
  إذا (حالت != Z_دھارا_سرا || کل_باہرلا != raw_len)
    أرجع خطأ;

  /* Un-filter and map to alpha. */
  إذا (!باہرلا->resize (نقاب_len))
    أرجع خطأ;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (چوڑائی);
  إذا (!unfiltered)
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (چوڑائی, 1);
  إذا (!prev_unfiltered)
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  لكل (بنانشانی y = 0; y < اچائی; y++)
  {
    uint8_t *صف = raw + y * (چوڑائی + 1);
    uint8_t filter = صف[0];
    uint8_t *pixels = صف + 1;

    لكل (بنانشانی x = 0; x < چوڑائی; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      بدل (filter)
      {
      حالة 0: unfiltered[x] = val; اكسر; /* None */
      حالة 1: unfiltered[x] = val + a; اكسر; /* Sub */
      حالة 2: unfiltered[x] = val + b; اكسر; /*اتے*/
      حالة 3: unfiltered[x] = val + (uint8_t) (((بنانشانی) a + b) / 2); اكسر; /*اوسطن*/
      حالة 4: /* Paeth */
      {
	صحيح p = (صحيح) a + (صحيح) b - (صحيح) c;
	صحيح pa = مطلق_قدر (p - (صحيح) a);
	صحيح pb = مطلق_قدر (p - (صحيح) b);
	صحيح pc = مطلق_قدر (p - (صحيح) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	اكسر;
      }
      افتراضي: unfiltered[x] = val; اكسر;
      }

      /* Map palette index to alpha. */
      uint8_t idx = unfiltered[x];
      باہرلا->arrayZ[y * چوڑائی + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = prev_unfiltered;
    prev_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  أرجع سچ;
#جے_انت
}

/* Read a big-endian uint32 from a byte pointer. */
ساكن مضمن uint32_t
hb_pdf_png_u32 (ثابت uint8_t *p)
{
  أرجع ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

ساكن hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    فراغ *paint_انکڑے,
		    hb_blob_t *image,
		    بنانشانی چوڑائی,
		    بنانشانی اچائی,
		    hb_ٹیگ_t format,
		    عائم slant HB_بن_ورتیا,
		    hb_glyph_extents_t *extents,
		    فراغ *)
{
  إذا (format != HB_ٹیگ ('p','n','g',' '))
    أرجع خطأ;
  إذا (!extents || !چوڑائی || !اچائی)
    أرجع خطأ;

  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع خطأ;

  بنانشانی len = 0;
  ثابت uint8_t *انکڑے = (ثابت uint8_t *) hb_blob_get_انکڑے (image, &len);
  إذا (!انکڑے || len < 8)
    أرجع خطأ;

  /* Verify PNG signature. */
  ساكن ثابت uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  إذا (hb_memcmp (انکڑے, png_sig, 8) != 0)
    أرجع خطأ;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  بنانشانی png_w = 0, png_h = 0;
  بنانشانی colors = 3; /* default RGB */
  uint8_t color_type = 2;
  منطقي has_alpha = خطأ;
  ثابت uint8_t *plte_انکڑے = خالی_سوچک;
  بنانشانی plte_len = 0;
  ثابت uint8_t *trns_انکڑے = خالی_سوچک;
  بنانشانی trns_len = 0;
  hb_سمتیہ_buf_t idat;

  بنانشانی pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  طالما (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (انکڑے + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (انکڑے + pos + 4);
    إذا (chunk_len > len - pos - 12)
      اكسر;
    ثابت uint8_t *chunk_انکڑے = انکڑے + pos + 8;

    إذا (chunk_type == 0x49484452u) /* IHDR */
    {
      إذا (chunk_len < 13) أرجع خطأ;
      png_w = hb_pdf_png_u32 (chunk_انکڑے);
      png_h = hb_pdf_png_u32 (chunk_انکڑے + 4);
      uint8_t ثنائی_ہندسہ_ڈونگھائی_2 = chunk_انکڑے[8];
      color_type = chunk_انکڑے[9];
      إذا (ثنائی_ہندسہ_ڈونگھائی_2 != 8) أرجع خطأ; /* only 8-bit supported */

      بدل (color_type)
      {
      حالة 0: colors = 1; has_alpha = خطأ; اكسر; /* Grayscale */
      حالة 2: colors = 3; has_alpha = خطأ; اكسر; /* RGB */
      حالة 3: colors = 1; has_alpha = خطأ; اكسر; /*اشاریہ_دار*/
      حالة 4: colors = 2; has_alpha = سچ;  اكسر; /* Gray+Alpha */
      حالة 6: colors = 4; has_alpha = سچ;  اكسر; /* RGBA */
      افتراضي: أرجع خطأ;
      }
    }
    وإلا إذا (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_انکڑے = chunk_انکڑے;
      plte_len = chunk_len;
    }
    وإلا إذا (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_انکڑے = chunk_انکڑے;
      trns_len = chunk_len;
    }
    وإلا إذا (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ثابت حرف *) chunk_انکڑے, chunk_len);
    }
    وإلا إذا (chunk_type == 0x49454E44u) /* IEND */
      اكسر;

    pos += 12 + chunk_len;
  }

  إذا (!png_w || !png_h || !idat.لمائی)
    أرجع خطأ;

  بنانشانی im_idx = res->جوڑو_xobject_png_image (idat.arrayZ, idat.لمائی,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_انکڑے, plte_len,
						 trns_انکڑے, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  خودکار &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  عائم ix = (عائم) extents->x_bearing;
  عائم iy = (عائم) extents->y_bearing + (عائم) extents->اچائی;
  عائم iw = (عائم) extents->چوڑائی;
  عائم ih = -(عائم) extents->اچائی; /* negative because image Y goes up but height is negative in extents */

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

  أرجع سچ;
}

/* ---- Gradient helpers ---- */

ساكن منطقي
hb_pdf_gradient_needs_alpha (hb_array_t<ثابت hb_color_stop_t> stops)
{
  لكل (ثابت خودکار &s : stops)
    إذا (hb_color_get_alpha (s.color) != 255)
      أرجع سچ;
  أرجع خطأ;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
ساكن فراغ
hb_pdf_build_alpha_interpolation_function (hb_سمتیہ_buf_t *obj,
					   عائم a0, عائم a1)
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
ساكن بنانشانی
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_سمتیہ_paint_t *paint)
{
  بنانشانی گنتی = paint->color_stops_scratch.لمائی;

  إذا (گنتی < 2)
  {
    عائم a = گنتی ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    أرجع res->جوڑو_object (std::move (obj));
  }

  إذا (گنتی == 2)
  {
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    أرجع res->جوڑو_object (std::move (obj));
  }

  hb_سمتیہ_t<بنانشانی> sub_func_ids;
  لكل (بنانشانی i = 0; i + 1 < گنتی; i++)
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
  لكل (بنانشانی i = 0; i < sub_func_ids.لمائی; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  لكل (بنانشانی i = 1; i + 1 < گنتی; i++)
  {
    إذا (i > 1) obj.append_c (' ');
    obj.append_عدد (paint->color_stops_scratch.arrayZ[i].وتھ, 4);
  }
  obj.append_str ("]\n/Encode [");
  لكل (بنانشانی i = 0; i + 1 < گنتی; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  أرجع res->جوڑو_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
ساكن فراغ
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
ساكن بنانشانی
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_سمتیہ_paint_t *paint)
{
  بنانشانی گنتی = paint->color_stops_scratch.لمائی;

  إذا (گنتی < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = گنتی ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    أرجع res->جوڑو_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { أرجع (a.وتھ > b.وتھ) - (a.وتھ < b.وتھ); });

  إذا (گنتی == 2)
  {
    /* Two stops: single interpolation function. */
    hb_سمتیہ_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    أرجع res->جوڑو_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_سمتیہ_t<بنانشانی> sub_func_ids;
  لكل (بنانشانی i = 0; i + 1 < گنتی; i++)
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
  لكل (بنانشانی i = 0; i < sub_func_ids.لمائی; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  لكل (بنانشانی i = 1; i + 1 < گنتی; i++)
  {
    إذا (i > 1) obj.append_c (' ');
    obj.append_عدد (paint->color_stops_scratch.arrayZ[i].وتھ, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  لكل (بنانشانی i = 0; i + 1 < گنتی; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  أرجع res->جوڑو_object (std::move (obj));
}

ساكن فراغ
hb_pdf_paint_خطی_gradient (hb_paint_funcs_t *,
			      فراغ *paint_انکڑے,
			      hb_color_سطر_t *color_سطر,
			      عائم x0, عائم y0,
			      عائم x1, عائم y1,
			      عائم x2 HB_بن_ورتیا, عائم y2 HB_بن_ورتیا,
			      فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمائی, &mn, &mx);
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);

  بنانشانی func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

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

  بنانشانی sh_idx = res->جوڑو_shading (std::move (sh));

  خودکار &body = paint->current_body ();

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  إذا (needs_alpha)
  {
    بنانشانی alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

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
    بنانشانی alpha_sh_id = res->جوڑو_object (std::move (ash));

    بنانشانی gs_idx = res->جوڑو_extgstate_smask (alpha_sh_id,
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

ساكن فراغ
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      فراغ *paint_انکڑے,
			      hb_color_سطر_t *color_سطر,
			      عائم x0, عائم y0, عائم r0,
			      عائم x1, عائم y1, عائم r1,
			      فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.لمائی, &mn, &mx);
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  بنانشانی func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

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

  بنانشانی sh_idx = res->جوڑو_shading (std::move (sh));

  خودکار &body = paint->current_body ();

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  إذا (needs_alpha)
  {
    بنانشانی alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

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
    بنانشانی alpha_sh_id = res->جوڑو_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    عائم cx = (gr1 >= gr0) ? gx1 : gx0;
    عائم cy = (gr1 >= gr0) ? gy1 : gy0;
    عائم rr = hb_max (gr0, gr1);
    بنانشانی gs_idx = res->جوڑو_extgstate_smask (alpha_sh_id,
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
ساكن فراغ
hb_pdf_encode_u16 (hb_سمتیہ_buf_t *buf, uint16_t v)
{
  حرف بائٹاں[2] = {(حرف) (v >> 8), (حرف) (v & 0xFF)};
  buf->append_len (بائٹاں, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
ساكن فراغ
hb_pdf_encode_coord (hb_سمتیہ_buf_t *buf,
		     عائم val, عائم lo, عائم hi)
{
  عائم t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
ساكن فراغ
hb_pdf_encode_color_rgb (hb_سمتیہ_buf_t *buf, hb_color_t c)
{
  حرف rgb[3] = {(حرف) hb_color_get_red (c),
		 (حرف) hb_color_get_green (c),
		 (حرف) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
ساكن فراغ
hb_pdf_encode_color_alpha (hb_سمتیہ_buf_t *buf, hb_color_t c)
{
  حرف a = (حرف) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
ساكن فراغ
hb_pdf_encode_نقطہ (hb_سمتیہ_buf_t *buf,
		     عائم x, عائم y,
		     عائم xlo, عائم xhi,
		     عائم ylo, عائم yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
ساكن فراغ
hb_pdf_جوڑو_sweep_patch (hb_سمتیہ_buf_t *mesh,
			hb_سمتیہ_buf_t *alpha_mesh,
			عائم cx, عائم cy,
			عائم xlo, عائم xhi, عائم ylo, عائم yhi,
			عائم a0, hb_color_t c0_وچ,
			عائم a1, hb_color_t c1_وچ)
{
  ثابت عائم R = 32767.f;
  ثابت عائم eps = 0.5f;
  ثابت عائم MAX_SECTOR = (عائم) M_PI / 2.f;

  صحيح عدد_splits = (صحيح) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  إذا (عدد_splits < 1) عدد_splits = 1;

  لكل (صحيح s = 0; s < عدد_splits; s++)
  {
    عائم k0 = (عائم) s / عدد_splits;
    عائم k1 = (عائم) (s + 1) / عدد_splits;
    عائم sa0 = a0 + k0 * (a1 - a0);
    عائم sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_وچ, c1_وچ, k0);
    hb_color_t sc1 = hb_color_lerp (c0_وچ, c1_وچ, k1);

    عائم da = sa1 - sa0;
    عائم kappa = (4.f / 3.f) * tanf (da / 4.f);

    عائم cos0 = cosf (sa0), sin0 = sinf (sa0);
    عائم cos1 = cosf (sa1), sin1 = sinf (sa1);

    عائم p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    عائم p3x = cx + R * cos0,   p3y = cy + R * sin0;
    عائم p6x = cx + R * cos1,   p6y = cy + R * sin1;
    عائم p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    عائم e1_1x = p0x + (p3x - p0x) / 3.f;
    عائم e1_1y = p0y + (p3y - p0y) / 3.f;
    عائم e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    عائم e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    عائم e2_1x = p3x + kappa * R * (-sin0);
    عائم e2_1y = p3y + kappa * R * ( cos0);
    عائم e2_2x = p6x - kappa * R * (-sin1);
    عائم e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    عائم e3_1x = p6x + (p9x - p6x) / 3.f;
    عائم e3_1y = p6y + (p9y - p6y) / 3.f;
    عائم e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    عائم e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    عائم e4_1x = p9x + kappa * eps * (-sin1);
    عائم e4_1y = p9y + kappa * eps * ( cos1);
    عائم e4_2x = p0x - kappa * eps * (-sin0);
    عائم e4_2y = p0y - kappa * eps * ( cos0);

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

    إذا (alpha_mesh)
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
بنية hb_pdf_sweep_ctx_t {
  hb_سمتیہ_buf_t *mesh;
  hb_سمتیہ_buf_t *alpha_mesh;
  عائم cx, cy, xlo, xhi, ylo, yhi;
};

ساكن فراغ
hb_pdf_sweep_emit_patch (عائم a0, hb_color_t c0,
			 عائم a1, hb_color_t c1,
			 فراغ *user_انکڑے)
{
  خودکار *ctx = (hb_pdf_sweep_ctx_t *) user_انکڑے;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->mesh->وچ_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->وچ_error ())))
    أرجع;
  hb_pdf_جوڑو_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

ساكن فراغ
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     فراغ *paint_انکڑے,
			     hb_color_سطر_t *color_سطر,
			     عائم cx, عائم cy,
			     عائم شروع_angle,
			     عائم سرا_angle,
			     فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Get and sort color stops. */
  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_سمتیہ_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { أرجع (a.وتھ > b.وتھ) - (a.وتھ < b.وتھ); });

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  ثابت عائم R = 32767.f;
  عائم scx = paint->sx (cx), scy = paint->sy (cy);
  عائم xlo = scx - R - 1, xhi = scx + R + 1;
  عائم ylo = scy - R - 1, yhi = scy + R + 1;

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_سمتیہ_buf_t mesh;
  hb_سمتیہ_buf_t alpha_mesh;
  mesh.alloc (256);
  إذا (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : خالی_سوچک,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.لمائی, extend,
				 شروع_angle, سرا_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  إذا (!mesh.لمائی)
    أرجع;

  خودکار hb_pdf_build_mesh_shading = [&] (hb_سمتیہ_buf_t &m,
					 ثابت حرف *cs,
					 ثابت حرف *decode_suffix) -> بنانشانی
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
    sh.append_unsigned (m.لمائی);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.لمائی);
    sh.append_str ("\nendstream");
    أرجع res->جوڑو_object (std::move (sh));
  };

  بنانشانی sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  بنانشانی sh_idx = res->جوڑو_shading_by_id (sh_obj_id);

  خودکار &body = paint->current_body ();

  إذا (needs_alpha && alpha_mesh.لمائی)
  {
    بنانشانی alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    بنانشانی gs_idx = res->جوڑو_extgstate_smask (alpha_sh_id,
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

ساكن ثابت حرف *
hb_pdf_blend_منوال_name (hb_paint_مرکب_منوال_t منوال)
{
  بدل (منوال)
  {
  حالة HB_PAINT_مرکب_منوال_MULTIPLY:       أرجع "Multiply";
  حالة HB_PAINT_مرکب_منوال_SCREEN:         أرجع "Screen";
  حالة HB_PAINT_مرکب_منوال_OVERLAY:        أرجع "Overlay";
  حالة HB_PAINT_مرکب_منوال_DARKEN:         أرجع "Darken";
  حالة HB_PAINT_مرکب_منوال_LIGHTEN:        أرجع "Lighten";
  حالة HB_PAINT_مرکب_منوال_COLOR_DODGE:    أرجع "ColorDodge";
  حالة HB_PAINT_مرکب_منوال_COLOR_BURN:     أرجع "ColorBurn";
  حالة HB_PAINT_مرکب_منوال_HARD_LIGHT:     أرجع "HardLight";
  حالة HB_PAINT_مرکب_منوال_SOFT_LIGHT:     أرجع "SoftLight";
  حالة HB_PAINT_مرکب_منوال_DIFFERENCE:     أرجع "Difference";
  حالة HB_PAINT_مرکب_منوال_EXCLUSION:      أرجع "Exclusion";
  حالة HB_PAINT_مرکب_منوال_HSL_HUE:        أرجع "Hue";
  حالة HB_PAINT_مرکب_منوال_HSL_SATURATION: أرجع "Saturation";
  حالة HB_PAINT_مرکب_منوال_HSL_COLOR:      أرجع "Color";
  حالة HB_PAINT_مرکب_منوال_HSL_LUMINOSITY: أرجع "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  حالة HB_PAINT_مرکب_منوال_PLUS:           أرجع "Screen";
  حالة HB_PAINT_مرکب_منوال_XOR:            أرجع "Difference";
  حالة HB_PAINT_مرکب_منوال_CLEAR:
  حالة HB_PAINT_مرکب_منوال_SRC:
  حالة HB_PAINT_مرکب_منوال_DEST:
  حالة HB_PAINT_مرکب_منوال_SRC_OVER:
  حالة HB_PAINT_مرکب_منوال_DEST_OVER:
  حالة HB_PAINT_مرکب_منوال_SRC_وچ:
  حالة HB_PAINT_مرکب_منوال_DEST_وچ:
  حالة HB_PAINT_مرکب_منوال_SRC_باہرلا:
  حالة HB_PAINT_مرکب_منوال_DEST_باہرلا:
  حالة HB_PAINT_مرکب_منوال_SRC_ATOP:
  حالة HB_PAINT_مرکب_منوال_DEST_ATOP:
  افتراضي:                                     أرجع خالی_سوچک; /*عام*/
  }
}

ساكن فراغ
hb_pdf_paint_ڈالو_گروہ (hb_paint_funcs_t *,
			 فراغ *paint_انکڑے,
			 فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("q\n");
}

ساكن فراغ
hb_pdf_paint_ڈالو_گروہ_for (hb_paint_funcs_t *,
			     فراغ *paint_انکڑے,
			     hb_paint_مرکب_منوال_t منوال,
			     فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");

  ثابت حرف *bm = hb_pdf_blend_منوال_name (منوال);
  إذا (bm)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    إذا (likely (res))
    {
      بنانشانی gs_idx = res->جوڑو_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

ساكن فراغ
hb_pdf_paint_نکالو_گروہ (hb_paint_funcs_t *,
			فراغ *paint_انکڑے,
			hb_paint_مرکب_منوال_t منوال HB_بن_ورتیا,
			فراغ *)
{
  خودکار *paint = (hb_سمتیہ_paint_t *) paint_انکڑے;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

ساكن مضمن فراغ free_static_pdf_paint_funcs ();

ساكن بنية hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  ساكن hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_سیٹ_ڈالو_transform_func (funcs, (hb_paint_ڈالو_transform_func_t) hb_pdf_paint_ڈالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_transform_func (funcs, (hb_paint_نکالو_transform_func_t) hb_pdf_paint_نکالو_transform, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_glyph_func (funcs, (hb_paint_ڈالو_clip_glyph_func_t) hb_pdf_paint_ڈالو_clip_glyph, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_rectangle_func (funcs, (hb_paint_ڈالو_clip_rectangle_func_t) hb_pdf_paint_ڈالو_clip_rectangle, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_شروع_func (funcs, (hb_paint_ڈالو_clip_رستہ_شروع_func_t) hb_pdf_paint_ڈالو_clip_رستہ_شروع, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_clip_رستہ_سرا_func (funcs, (hb_paint_ڈالو_clip_رستہ_سرا_func_t) hb_pdf_paint_ڈالو_clip_رستہ_سرا, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_clip_func (funcs, (hb_paint_نکالو_clip_func_t) hb_pdf_paint_نکالو_clip, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_خطی_gradient_func (funcs, (hb_paint_خطی_gradient_func_t) hb_pdf_paint_خطی_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_func (funcs, (hb_paint_ڈالو_گروہ_func_t) hb_pdf_paint_ڈالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_ڈالو_گروہ_for_func (funcs, (hb_paint_ڈالو_گروہ_for_func_t) hb_pdf_paint_ڈالو_گروہ_for, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_نکالو_گروہ_func (funcs, (hb_paint_نکالو_گروہ_func_t) hb_pdf_paint_نکالو_گروہ, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_سیٹ_budget_func (funcs, hb_سمتیہ_paint_سیٹ_budget_callback, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_func (funcs, hb_سمتیہ_paint_get_budget_callback, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_سیٹ_get_budget_بچیا_func (funcs, hb_سمتیہ_paint_get_budget_بچیا_callback, خالی_سوچک, خالی_سوچک);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    أرجع funcs;
  }
} static_pdf_paint_funcs;

ساكن مضمن فراغ
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_سمتیہ_paint_pdf_funcs_get ()
{
  أرجع static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_سمتیہ_paint_render_pdf (hb_سمتیہ_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع خالی_سوچک;
  إذا (!paint->گروہ_ڈھیر.لمائی ||
      !paint->گروہ_ڈھیر.arrayZ[0].لمائی)
    أرجع خالی_سوچک;

  hb_سمتیہ_buf_t &content = paint->گروہ_ڈھیر.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  عائم ex = paint->extents.x;
  عائم ey = paint->extents.y;
  عائم ew = paint->extents.چوڑائی;
  عائم eh = paint->extents.اچائی;

  بنانشانی عدد_extra = res ? res->objects.لمائی : 0;
  بنانشانی کل_objects = 4 + عدد_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_سمتیہ_buf_t باہرلا;
  hb_buf_recover_recycled (paint->recycled_blob, &باہرلا);
  باہرلا.alloc (content.لمائی + عدد_extra * 128 + 1024);

  hb_سمتیہ_t<بنانشانی> offsets;
  إذا (unlikely (!offsets.resize (کل_objects)))
    أرجع خالی_سوچک;

  باہرلا.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = باہرلا.لمائی;
  باہرلا.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = باہرلا.لمائی;
  باہرلا.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = باہرلا.لمائی;
  باہرلا.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  باہرلا.append_عدد (ex);
  باہرلا.append_c (' ');
  باہرلا.append_عدد (ey);
  باہرلا.append_c (' ');
  باہرلا.append_عدد (ex + ew);
  باہرلا.append_c (' ');
  باہرلا.append_عدد (ey + eh);
  باہرلا.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  منطقي has_resources = res &&
    (res->extgstate_dict.لمائی || res->shading_dict.لمائی || res->xobject_dict.لمائی);
  إذا (has_resources)
  {
    باہرلا.append_str ("\n/Resources <<");
    إذا (res->extgstate_dict.لمائی)
    {
      باہرلا.append_str (" /ExtGState << ");
      باہرلا.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.لمائی);
      باہرلا.append_str (">>");
    }
    إذا (res->shading_dict.لمائی)
    {
      باہرلا.append_str (" /Shading << ");
      باہرلا.append_len (res->shading_dict.arrayZ, res->shading_dict.لمائی);
      باہرلا.append_str (">>");
    }
    إذا (res->xobject_dict.لمائی)
    {
      باہرلا.append_str (" /XObject << ");
      باہرلا.append_len (res->xobject_dict.arrayZ, res->xobject_dict.لمائی);
      باہرلا.append_str (">>");
    }
    باہرلا.append_str (" >>");
  }

  باہرلا.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_سمتیہ_buf_t bg_اگیتر;
  إذا (hb_color_get_alpha (paint->background))
  {
    عائم r = hb_color_get_red (paint->background) / 255.f;
    عائم g = hb_color_get_green (paint->background) / 255.f;
    عائم b = hb_color_get_blue (paint->background) / 255.f;
    عائم a = hb_color_get_alpha (paint->background) / 255.f;
    إذا (a < 1.f - 1.f / 512.f)
    {
      إذا (res)
      {
	بنانشانی gs_idx = res->جوڑو_extgstate_alpha (a);
	bg_اگیتر.append_str ("/GS");
	bg_اگیتر.append_unsigned (gs_idx);
	bg_اگیتر.append_str (" gs\n");
      }
    }
    bg_اگیتر.append_عدد (r, 4);
    bg_اگیتر.append_c (' ');
    bg_اگیتر.append_عدد (g, 4);
    bg_اگیتر.append_c (' ');
    bg_اگیتر.append_عدد (b, 4);
    bg_اگیتر.append_str (" rg\n");
    bg_اگیتر.append_عدد (ex);
    bg_اگیتر.append_c (' ');
    bg_اگیتر.append_عدد (ey);
    bg_اگیتر.append_c (' ');
    bg_اگیتر.append_عدد (ew);
    bg_اگیتر.append_c (' ');
    bg_اگیتر.append_عدد (eh);
    bg_اگیتر.append_str (" re f\n");
  }
  بنانشانی دھارا_len = bg_اگیتر.لمائی + content.لمائی;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = باہرلا.لمائی;
  باہرلا.append_str ("4 0 obj\n<< /Length ");
  باہرلا.append_unsigned (دھارا_len);
  باہرلا.append_str (" >>\nstream\n");
  باہرلا.append_len (bg_اگیتر.arrayZ, bg_اگیتر.لمائی);
  باہرلا.append_len (content.arrayZ, content.لمائی);
  باہرلا.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  لكل (بنانشانی i = 0; i < عدد_extra; i++)
  {
    offsets.arrayZ[4 + i] = باہرلا.لمائی;
    باہرلا.append_unsigned (5 + i);
    باہرلا.append_str (" 0 obj\n");
    خودکار &obj = res->objects.arrayZ[i];
    باہرلا.append_len (obj.انکڑے.arrayZ, obj.انکڑے.لمائی);
    باہرلا.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  بنانشانی xref_وتھ = باہرلا.لمائی;
  باہرلا.append_str ("xref\n0 ");
  باہرلا.append_unsigned (کل_objects + 1);
  باہرلا.append_str ("\n0000000000 65535 f \n");
  لكل (بنانشانی i = 0; i < کل_objects; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    باہرلا.append_len (tmp, 20);
  }

  /* Trailer */
  باہرلا.append_str ("trailer\n<< /Size ");
  باہرلا.append_unsigned (کل_objects + 1);
  باہرلا.append_str (" /Root 1 0 R >>\nstartxref\n");
  باہرلا.append_unsigned (xref_وتھ);
  باہرلا.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &باہرلا);

  أرجع blob;
}

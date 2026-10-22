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

#اگر_تعریف_شده HAVE_ZLIB
#ضمن <zlib.h>
#پایان_اگر

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
  hb_وکتور_buf_t داده;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
ساكن منطقي
hb_pdf_build_نمایهای_smask (hb_وکتور_buf_t *بیرون,
			    ثابت حرف *idat_داده, بیعلامت idat_len,
			    بیعلامت عرض, بیعلامت ارتفاع,
			    ثابت uint8_t *trns, بیعلامت trns_len);

بنية hb_pdf_resources_t
{
  hb_وکتور_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_وکتور_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_وکتور_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_وکتور_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  بیعلامت extgstate_شمار = 0;
  بیعلامت shading_شمار = 0;
  بیعلامت xobject_شمار = 0;

  بیعلامت جمع_کن_object (hb_وکتور_buf_t &&obj_داده)
  {
    بیعلامت id = 5 + objects.درازی; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.داده = std::move (obj_داده);
    objects.push (std::move (obj));
    أرجع id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  بیعلامت جمع_کن_extgstate_alpha (عائم alpha)
  {
    بیعلامت idx = extgstate_شمار++;
    hb_وکتور_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_رقم (alpha, 4);
    obj.append_str (" >>");
    بیعلامت obj_id = جمع_کن_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  بیعلامت جمع_کن_extgstate_blend (ثابت حرف *bm)
  {
    بیعلامت idx = extgstate_شمار++;
    hb_وکتور_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    بیعلامت obj_id = جمع_کن_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  بیعلامت جمع_کن_extgstate_smask (بیعلامت alpha_shading_id,
				عائم bbox_x, عائم bbox_y,
				عائم bbox_w, عائم bbox_h,
				بیعلامت precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_وکتور_buf_t form_جریان;
    form_جریان.append_str ("/SHa sh\n");

    hb_وکتور_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_رقم (bbox_x, precision);
    form.append_c (' ');
    form.append_رقم (bbox_y, precision);
    form.append_c (' ');
    form.append_رقم (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_رقم (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_جریان.درازی);
    form.append_str (" >>\nstream\n");
    form.append_len (form_جریان.arrayZ, form_جریان.درازی);
    form.append_str ("endstream");
    بیعلامت form_id = جمع_کن_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    بیعلامت idx = extgstate_شمار++;
    hb_وکتور_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    بیعلامت gs_id = جمع_کن_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add a shading, return resource name index. */
  بیعلامت جمع_کن_shading (hb_وکتور_buf_t &&shading_داده)
  {
    بیعلامت obj_id = جمع_کن_object (std::move (shading_داده));
    أرجع جمع_کن_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  بیعلامت جمع_کن_shading_by_id (بیعلامت obj_id)
  {
    بیعلامت idx = shading_شمار++;
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
  بیعلامت جمع_کن_xobject_png_image (ثابت حرف *idat_داده, بیعلامت idat_len,
				  بیعلامت عرض, بیعلامت ارتفاع,
				  بیعلامت colors, منطقي has_alpha,
				  ثابت uint8_t *plte = اشاره_تهی,
				  بیعلامت plte_len = 0,
				  ثابت uint8_t *trns = اشاره_تهی,
				  بیعلامت trns_len = 0)
  {
    بیعلامت idx = xobject_شمار++;
    hb_وکتور_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (عرض);
    obj.append_str (" /Height ");
    obj.append_unsigned (ارتفاع);
    obj.append_str ("\n/BitsPerComponent 8\n");

    إذا (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      بیعلامت n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      لكل (بیعلامت i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    وإلا
    {
      obj.append_str ("/ColorSpace ");
      بیعلامت color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    بیعلامت smask_id = 0;
    إذا (plte && trns && trns_len)
    {
      hb_وکتور_buf_t smask_جریان;
      إذا (hb_pdf_build_نمایهای_smask (&smask_جریان, idat_داده, idat_len,
				      عرض, ارتفاع, trns, trns_len))
      {
	hb_وکتور_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (عرض);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (ارتفاع);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_جریان.درازی);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_جریان.arrayZ, smask_جریان.درازی);
	smask_obj.append_str ("\nendstream");
	smask_id = جمع_کن_object (std::move (smask_obj));
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
    obj.append_unsigned (عرض);
    obj.append_str (" >>\n");
    obj.append_str ("/Length ");
    obj.append_unsigned (idat_len);
    obj.append_str (" >>\nstream\n");
    obj.append_len (idat_داده, idat_len);
    obj.append_str ("\nendstream");

    (فراغ) has_alpha;

    بیعلامت obj_id = جمع_کن_object (std::move (obj));

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
hb_pdf_get_resources (hb_وکتور_paint_t *paint)
{
  إذا (!paint->defs.درازی)
  {
    /* First call: allocate and store. */
    خودکار *res = (hb_pdf_resources_t *) hb_calloc (1, حجم (hb_pdf_resources_t));
    إذا (unlikely (!res)) أرجع اشاره_تهی;
    جديد (res) hb_pdf_resources_t ();
    إذا (unlikely (!paint->defs.resize (حجم (فراغ *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      أرجع اشاره_تهی;
    }
    کپی_حافظه (paint->defs.arrayZ, &res, حجم (فراغ *));
  }
  hb_pdf_resources_t *res;
  کپی_حافظه (&res, paint->defs.arrayZ, حجم (فراغ *));
  أرجع res;
}

فراغ
hb_وکتور_paint_pdf_free_resources (hb_وکتور_paint_t *paint)
{
  إذا (paint->defs.درازی >= حجم (فراغ *))
  {
    hb_pdf_resources_t *res;
    کپی_حافظه (&res, paint->defs.arrayZ, حجم (فراغ *));
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
hb_pdf_emit_glyph_مسیر (hb_وکتور_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_وکتور_buf_t *buf)
{
  paint->مسیر.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_مانده >= 0))
  {
    hb_وکتور_مسیر_sink_t sink = {&paint->مسیر, paint->get_precision (),
				 paint->x_scale_ضریب, paint->y_scale_ضریب,
				 &paint->budget_مانده};
    hb_font_draw_glyph (font, glyph,
			hb_وکتور_pdf_مسیر_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->مسیر.arrayZ, paint->مسیر.درازی);
  paint->مسیر.clear ();
}


/* ---- paint callbacks ---- */

ساكن فراغ
hb_pdf_paint_افزودن_transform (hb_paint_funcs_t *,
			     فراغ *paint_داده,
			     عائم xx, عائم yx,
			     عائم xy, عائم yy,
			     عائم dx, عائم dy,
			     فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  بیعلامت sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_رقم (xx, sprec);
  body.append_c (' ');
  body.append_رقم (yx, sprec);
  body.append_c (' ');
  body.append_رقم (xy, sprec);
  body.append_c (' ');
  body.append_رقم (yy, sprec);
  body.append_c (' ');
  body.append_رقم (paint->sx (dx));
  body.append_c (' ');
  body.append_رقم (paint->sy (dy));
  body.append_str (" cm\n");
}

ساكن فراغ
hb_pdf_paint_برداشت_transform (hb_paint_funcs_t *,
			    فراغ *paint_داده,
			    فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}

ساكن فراغ
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 فراغ *paint_داده,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
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
      بیعلامت gs_idx = res->جمع_کن_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = درست;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_رقم (r, 4);
  body.append_c (' ');
  body.append_رقم (g, 4);
  body.append_c (' ');
  body.append_رقم (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_مسیر (paint, font, glyph, &body);
  body.append_str ("f\n");
  إذا (scoped)
    body.append_str ("Q\n");
}

ساكن فراغ
hb_pdf_paint_افزودن_clip_glyph (hb_paint_funcs_t *,
			      فراغ *paint_داده,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_مسیر (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

ساكن فراغ
hb_pdf_paint_افزودن_clip_rectangle (hb_paint_funcs_t *,
				  فراغ *paint_داده,
				  عائم xmin, عائم ymin,
				  عائم xmax, عائم ymax,
				  فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_رقم (paint->sx (xmin));
  body.append_c (' ');
  body.append_رقم (paint->sy (ymin));
  body.append_c (' ');
  body.append_رقم (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_رقم (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

ساكن hb_draw_funcs_t *
hb_pdf_paint_افزودن_clip_مسیر_شروع (hb_paint_funcs_t *,
				   فراغ *paint_داده,
				   فراغ **draw_داده,
				   فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_داده = اشاره_تهی;
    أرجع اشاره_تهی;
  }

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_مسیر_sink = {&body, paint->get_precision (),
			   paint->x_scale_ضریب,
			   paint->y_scale_ضریب,
			   &paint->budget_مانده};
  *draw_داده = &paint->clip_مسیر_sink;
  أرجع hb_وکتور_pdf_مسیر_draw_funcs_get ();
}

ساكن فراغ
hb_pdf_paint_افزودن_clip_مسیر_انتها (hb_paint_funcs_t *,
				 فراغ *paint_داده,
				 فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("W n\n");
}

ساكن فراغ
hb_pdf_paint_برداشت_clip (hb_paint_funcs_t *,
		       فراغ *paint_داده,
		       فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
ساكن فراغ
hb_pdf_paint_solid_color (hb_وکتور_paint_t *paint, hb_color_t c)
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
      بیعلامت gs_idx = res->جمع_کن_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_رقم (r, 4);
  body.append_c (' ');
  body.append_رقم (g, 4);
  body.append_c (' ');
  body.append_رقم (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

ساكن فراغ
hb_pdf_paint_color (hb_paint_funcs_t *,
		    فراغ *paint_داده,
		    hb_bool_t,
		    hb_color_t color,
		    فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
ساكن منطقي
hb_pdf_build_نمایهای_smask (hb_وکتور_buf_t *بیرون,
			    ثابت حرف *idat_داده, بیعلامت idat_len,
			    بیعلامت عرض, بیعلامت ارتفاع,
			    ثابت uint8_t *trns, بیعلامت trns_len)
{
#اگر_تعریف_نشده HAVE_ZLIB
  (فراغ) بیرون; (فراغ) idat_داده; (فراغ) idat_len;
  (فراغ) عرض; (فراغ) ارتفاع; (فراغ) trns; (فراغ) trns_len;
  أرجع خطأ;
#وإلا
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  بیعلامت raw_len, ماسک_len;
  إذا (!عرض || !ارتفاع ||
      عرض == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (عرض + 1, ارتفاع, &raw_len) ||
      hb_unsigned_mul_overflows (عرض, ارتفاع, &ماسک_len))
    أرجع خطأ;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  إذا (!raw) أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (raw));

  z_جریان جریان = {};
  جریان.بعدی_درون = (Bytef *) idat_داده;
  جریان.avail_درون = idat_len;
  جریان.بعدی_بیرون = (Bytef *) raw;
  جریان.avail_بیرون = raw_len;

  إذا (inflateInit (&جریان) != Z_OK)
    أرجع خطأ;
  صحيح وضعیت = inflate (&جریان, Z_پایان);
  بیعلامت دراز مجموع_بیرون = جریان.مجموع_بیرون;
  inflate_انتها (&جریان);
  إذا (وضعیت != Z_جریان_انتها || مجموع_بیرون != raw_len)
    أرجع خطأ;

  /* Un-filter and map to alpha. */
  إذا (!بیرون->resize (ماسک_len))
    أرجع خطأ;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (عرض);
  إذا (!unfiltered)
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (عرض, 1);
  إذا (!prev_unfiltered)
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  لكل (بیعلامت y = 0; y < ارتفاع; y++)
  {
    uint8_t *ردیف = raw + y * (عرض + 1);
    uint8_t filter = ردیف[0];
    uint8_t *pixels = ردیف + 1;

    لكل (بیعلامت x = 0; x < عرض; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      بدل (filter)
      {
      حالة 0: unfiltered[x] = val; اكسر; /* None */
      حالة 1: unfiltered[x] = val + a; اكسر; /* Sub */
      حالة 2: unfiltered[x] = val + b; اكسر; /*به_بالا*/
      حالة 3: unfiltered[x] = val + (uint8_t) (((بیعلامت) a + b) / 2); اكسر; /*معدل*/
      حالة 4: /* Paeth */
      {
	صحيح p = (صحيح) a + (صحيح) b - (صحيح) c;
	صحيح pa = قدر_مطلق (p - (صحيح) a);
	صحيح pb = قدر_مطلق (p - (صحيح) b);
	صحيح pc = قدر_مطلق (p - (صحيح) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	اكسر;
      }
      افتراضي: unfiltered[x] = val; اكسر;
      }

      /* Map palette index to alpha. */
      uint8_t idx = unfiltered[x];
      بیرون->arrayZ[y * عرض + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = prev_unfiltered;
    prev_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  أرجع درست;
#پایان_اگر
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
		    فراغ *paint_داده,
		    hb_blob_t *image,
		    بیعلامت عرض,
		    بیعلامت ارتفاع,
		    hb_برچسب_t format,
		    عائم slant HB_استفادهنشده,
		    hb_glyph_extents_t *extents,
		    فراغ *)
{
  إذا (format != HB_برچسب ('p','n','g',' '))
    أرجع خطأ;
  إذا (!extents || !عرض || !ارتفاع)
    أرجع خطأ;

  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع خطأ;

  بیعلامت len = 0;
  ثابت uint8_t *داده = (ثابت uint8_t *) hb_blob_get_داده (image, &len);
  إذا (!داده || len < 8)
    أرجع خطأ;

  /* Verify PNG signature. */
  ساكن ثابت uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  إذا (hb_memcmp (داده, png_sig, 8) != 0)
    أرجع خطأ;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  بیعلامت png_w = 0, png_h = 0;
  بیعلامت colors = 3; /* default RGB */
  uint8_t color_type = 2;
  منطقي has_alpha = خطأ;
  ثابت uint8_t *plte_داده = اشاره_تهی;
  بیعلامت plte_len = 0;
  ثابت uint8_t *trns_داده = اشاره_تهی;
  بیعلامت trns_len = 0;
  hb_وکتور_buf_t idat;

  بیعلامت pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  طالما (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (داده + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (داده + pos + 4);
    إذا (chunk_len > len - pos - 12)
      اكسر;
    ثابت uint8_t *chunk_داده = داده + pos + 8;

    إذا (chunk_type == 0x49484452u) /* IHDR */
    {
      إذا (chunk_len < 13) أرجع خطأ;
      png_w = hb_pdf_png_u32 (chunk_داده);
      png_h = hb_pdf_png_u32 (chunk_داده + 4);
      uint8_t بیت_عمق_2 = chunk_داده[8];
      color_type = chunk_داده[9];
      إذا (بیت_عمق_2 != 8) أرجع خطأ; /* only 8-bit supported */

      بدل (color_type)
      {
      حالة 0: colors = 1; has_alpha = خطأ; اكسر; /* Grayscale */
      حالة 2: colors = 3; has_alpha = خطأ; اكسر; /* RGB */
      حالة 3: colors = 1; has_alpha = خطأ; اكسر; /*نمایهای*/
      حالة 4: colors = 2; has_alpha = درست;  اكسر; /* Gray+Alpha */
      حالة 6: colors = 4; has_alpha = درست;  اكسر; /* RGBA */
      افتراضي: أرجع خطأ;
      }
    }
    وإلا إذا (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_داده = chunk_داده;
      plte_len = chunk_len;
    }
    وإلا إذا (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_داده = chunk_داده;
      trns_len = chunk_len;
    }
    وإلا إذا (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ثابت حرف *) chunk_داده, chunk_len);
    }
    وإلا إذا (chunk_type == 0x49454E44u) /* IEND */
      اكسر;

    pos += 12 + chunk_len;
  }

  إذا (!png_w || !png_h || !idat.درازی)
    أرجع خطأ;

  بیعلامت im_idx = res->جمع_کن_xobject_png_image (idat.arrayZ, idat.درازی,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_داده, plte_len,
						 trns_داده, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  خودکار &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  عائم ix = (عائم) extents->x_bearing;
  عائم iy = (عائم) extents->y_bearing + (عائم) extents->ارتفاع;
  عائم iw = (عائم) extents->عرض;
  عائم ih = -(عائم) extents->ارتفاع; /* negative because image Y goes up but height is negative in extents */

  body.append_رقم (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_رقم (paint->sy (ih));
  body.append_c (' ');
  body.append_رقم (paint->sx (ix));
  body.append_c (' ');
  body.append_رقم (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  أرجع درست;
}

/* ---- Gradient helpers ---- */

ساكن منطقي
hb_pdf_gradient_needs_alpha (hb_array_t<ثابت hb_color_stop_t> stops)
{
  لكل (ثابت خودکار &s : stops)
    إذا (hb_color_get_alpha (s.color) != 255)
      أرجع درست;
  أرجع خطأ;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
ساكن فراغ
hb_pdf_build_alpha_interpolation_function (hb_وکتور_buf_t *obj,
					   عائم a0, عائم a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_رقم (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_رقم (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
ساكن بیعلامت
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_وکتور_paint_t *paint)
{
  بیعلامت شمار = paint->color_stops_scratch.درازی;

  إذا (شمار < 2)
  {
    عائم a = شمار ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_وکتور_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    أرجع res->جمع_کن_object (std::move (obj));
  }

  إذا (شمار == 2)
  {
    hb_وکتور_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    أرجع res->جمع_کن_object (std::move (obj));
  }

  hb_وکتور_t<بیعلامت> sub_func_ids;
  لكل (بیعلامت i = 0; i + 1 < شمار; i++)
  {
    hb_وکتور_buf_t sub;
    hb_pdf_build_alpha_interpolation_function (&sub,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i + 1].color) / 255.f);
    sub_func_ids.push (res->جمع_کن_object (std::move (sub)));
  }

  hb_وکتور_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");
  obj.append_str ("/Functions [");
  لكل (بیعلامت i = 0; i < sub_func_ids.درازی; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  لكل (بیعلامت i = 1; i + 1 < شمار; i++)
  {
    إذا (i > 1) obj.append_c (' ');
    obj.append_رقم (paint->color_stops_scratch.arrayZ[i].جابه_جایی, 4);
  }
  obj.append_str ("]\n/Encode [");
  لكل (بیعلامت i = 0; i + 1 < شمار; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  أرجع res->جمع_کن_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
ساكن فراغ
hb_pdf_build_interpolation_function (hb_وکتور_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_رقم (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_رقم (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_رقم (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_رقم (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_رقم (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_رقم (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
ساكن بیعلامت
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_وکتور_paint_t *paint)
{
  بیعلامت شمار = paint->color_stops_scratch.درازی;

  إذا (شمار < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = شمار ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_وکتور_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    أرجع res->جمع_کن_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { أرجع (a.جابه_جایی > b.جابه_جایی) - (a.جابه_جایی < b.جابه_جایی); });

  إذا (شمار == 2)
  {
    /* Two stops: single interpolation function. */
    hb_وکتور_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    أرجع res->جمع_کن_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_وکتور_t<بیعلامت> sub_func_ids;
  لكل (بیعلامت i = 0; i + 1 < شمار; i++)
  {
    hb_وکتور_buf_t sub;
    hb_pdf_build_interpolation_function (&sub,
					  paint->color_stops_scratch.arrayZ[i].color,
					  paint->color_stops_scratch.arrayZ[i + 1].color);
    sub_func_ids.push (res->جمع_کن_object (std::move (sub)));
  }

  /* Stitching function (Type 3). */
  hb_وکتور_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");

  /* Functions array. */
  obj.append_str ("/Functions [");
  لكل (بیعلامت i = 0; i < sub_func_ids.درازی; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  لكل (بیعلامت i = 1; i + 1 < شمار; i++)
  {
    إذا (i > 1) obj.append_c (' ');
    obj.append_رقم (paint->color_stops_scratch.arrayZ[i].جابه_جایی, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  لكل (بیعلامت i = 0; i + 1 < شمار; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  أرجع res->جمع_کن_object (std::move (obj));
}

ساكن فراغ
hb_pdf_paint_خطی_gradient (hb_paint_funcs_t *,
			      فراغ *paint_داده,
			      hb_color_سطر_t *color_سطر,
			      عائم x0, عائم y0,
			      عائم x1, عائم y1,
			      عائم x2 HB_استفادهنشده, عائم y2 HB_استفادهنشده,
			      فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_وکتور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.درازی, &mn, &mx);
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);

  بیعلامت func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت حرف *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_وکتور_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_رقم (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_رقم (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_رقم (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_رقم (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  بیعلامت sh_idx = res->جمع_کن_shading (std::move (sh));

  خودکار &body = paint->current_body ();

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  إذا (needs_alpha)
  {
    بیعلامت alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_وکتور_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_رقم (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_رقم (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_رقم (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_رقم (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    بیعلامت alpha_sh_id = res->جمع_کن_object (std::move (ash));

    بیعلامت gs_idx = res->جمع_کن_extgstate_smask (alpha_sh_id,
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
			      فراغ *paint_داده,
			      hb_color_سطر_t *color_سطر,
			      عائم x0, عائم y0, عائم r0,
			      عائم x1, عائم y1, عائم r1,
			      فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_وکتور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سطر (stops.arrayZ, stops.درازی, &mn, &mx);
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  بیعلامت func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);
  ثابت حرف *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_وکتور_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_رقم (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_رقم (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_رقم (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_رقم (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_رقم (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_رقم (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  بیعلامت sh_idx = res->جمع_کن_shading (std::move (sh));

  خودکار &body = paint->current_body ();

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  إذا (needs_alpha)
  {
    بیعلامت alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_وکتور_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_رقم (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_رقم (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_رقم (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_رقم (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_رقم (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_رقم (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    بیعلامت alpha_sh_id = res->جمع_کن_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    عائم cx = (gr1 >= gr0) ? gx1 : gx0;
    عائم cy = (gr1 >= gr0) ? gy1 : gy0;
    عائم rr = hb_max (gr0, gr1);
    بیعلامت gs_idx = res->جمع_کن_extgstate_smask (alpha_sh_id,
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
hb_pdf_encode_u16 (hb_وکتور_buf_t *buf, uint16_t v)
{
  حرف بایتها[2] = {(حرف) (v >> 8), (حرف) (v & 0xFF)};
  buf->append_len (بایتها, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
ساكن فراغ
hb_pdf_encode_coord (hb_وکتور_buf_t *buf,
		     عائم val, عائم lo, عائم hi)
{
  عائم t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
ساكن فراغ
hb_pdf_encode_color_rgb (hb_وکتور_buf_t *buf, hb_color_t c)
{
  حرف rgb[3] = {(حرف) hb_color_get_red (c),
		 (حرف) hb_color_get_green (c),
		 (حرف) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
ساكن فراغ
hb_pdf_encode_color_alpha (hb_وکتور_buf_t *buf, hb_color_t c)
{
  حرف a = (حرف) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
ساكن فراغ
hb_pdf_encode_نقطه (hb_وکتور_buf_t *buf,
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
hb_pdf_جمع_کن_sweep_patch (hb_وکتور_buf_t *mesh,
			hb_وکتور_buf_t *alpha_mesh,
			عائم cx, عائم cy,
			عائم xlo, عائم xhi, عائم ylo, عائم yhi,
			عائم a0, hb_color_t c0_درون,
			عائم a1, hb_color_t c1_درون)
{
  ثابت عائم R = 32767.f;
  ثابت عائم eps = 0.5f;
  ثابت عائم MAX_SECTOR = (عائم) M_PI / 2.f;

  صحيح رقم_splits = (صحيح) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  إذا (رقم_splits < 1) رقم_splits = 1;

  لكل (صحيح s = 0; s < رقم_splits; s++)
  {
    عائم k0 = (عائم) s / رقم_splits;
    عائم k1 = (عائم) (s + 1) / رقم_splits;
    عائم sa0 = a0 + k0 * (a1 - a0);
    عائم sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_درون, c1_درون, k0);
    hb_color_t sc1 = hb_color_lerp (c0_درون, c1_درون, k1);

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

    hb_pdf_encode_نقطه (mesh, p0x, p0y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, p3x, p3y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, p6x, p6y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, p9x, p9y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطه (mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

    hb_pdf_encode_color_rgb (mesh, sc0); /* inner start */
    hb_pdf_encode_color_rgb (mesh, sc0); /* outer start */
    hb_pdf_encode_color_rgb (mesh, sc1); /* outer end */
    hb_pdf_encode_color_rgb (mesh, sc1); /* inner end */

    إذا (alpha_mesh)
    {
      alpha_mesh->append_c ('\0');

      hb_pdf_encode_نقطه (alpha_mesh, p0x, p0y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, p3x, p3y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, p6x, p6y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, p9x, p9y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطه (alpha_mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
    }
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
بنية hb_pdf_sweep_ctx_t {
  hb_وکتور_buf_t *mesh;
  hb_وکتور_buf_t *alpha_mesh;
  عائم cx, cy, xlo, xhi, ylo, yhi;
};

ساكن فراغ
hb_pdf_sweep_emit_patch (عائم a0, hb_color_t c0,
			 عائم a1, hb_color_t c1,
			 فراغ *user_داده)
{
  خودکار *ctx = (hb_pdf_sweep_ctx_t *) user_داده;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->mesh->درون_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->درون_error ())))
    أرجع;
  hb_pdf_جمع_کن_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

ساكن فراغ
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     فراغ *paint_داده,
			     hb_color_سطر_t *color_سطر,
			     عائم cx, عائم cy,
			     عائم شروع_angle,
			     عائم انتها_angle,
			     فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودکار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Get and sort color stops. */
  إذا (!paint->fetch_color_stops (color_سطر))
    أرجع;
  hb_وکتور_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { أرجع (a.جابه_جایی > b.جابه_جایی) - (a.جابه_جایی < b.جابه_جایی); });

  hb_paint_extend_t extend = hb_color_سطر_get_extend (color_سطر);

  ثابت عائم R = 32767.f;
  عائم scx = paint->sx (cx), scy = paint->sy (cy);
  عائم xlo = scx - R - 1, xhi = scx + R + 1;
  عائم ylo = scy - R - 1, yhi = scy + R + 1;

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_وکتور_buf_t mesh;
  hb_وکتور_buf_t alpha_mesh;
  mesh.alloc (256);
  إذا (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : اشاره_تهی,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.درازی, extend,
				 شروع_angle, انتها_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  إذا (!mesh.درازی)
    أرجع;

  خودکار hb_pdf_build_mesh_shading = [&] (hb_وکتور_buf_t &m,
					 ثابت حرف *cs,
					 ثابت حرف *decode_suffix) -> بیعلامت
  {
    hb_وکتور_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_رقم (xlo, 2);
    sh.append_c (' ');
    sh.append_رقم (xhi, 2);
    sh.append_c (' ');
    sh.append_رقم (ylo, 2);
    sh.append_c (' ');
    sh.append_رقم (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.درازی);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.درازی);
    sh.append_str ("\nendstream");
    أرجع res->جمع_کن_object (std::move (sh));
  };

  بیعلامت sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  بیعلامت sh_idx = res->جمع_کن_shading_by_id (sh_obj_id);

  خودکار &body = paint->current_body ();

  إذا (needs_alpha && alpha_mesh.درازی)
  {
    بیعلامت alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    بیعلامت gs_idx = res->جمع_کن_extgstate_smask (alpha_sh_id,
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
hb_pdf_blend_نما_name (hb_paint_مرکب_نما_t نما)
{
  بدل (نما)
  {
  حالة HB_PAINT_مرکب_نما_MULTIPLY:       أرجع "Multiply";
  حالة HB_PAINT_مرکب_نما_SCREEN:         أرجع "Screen";
  حالة HB_PAINT_مرکب_نما_OVERLAY:        أرجع "Overlay";
  حالة HB_PAINT_مرکب_نما_DARKEN:         أرجع "Darken";
  حالة HB_PAINT_مرکب_نما_LIGHTEN:        أرجع "Lighten";
  حالة HB_PAINT_مرکب_نما_COLOR_DODGE:    أرجع "ColorDodge";
  حالة HB_PAINT_مرکب_نما_COLOR_BURN:     أرجع "ColorBurn";
  حالة HB_PAINT_مرکب_نما_HARD_LIGHT:     أرجع "HardLight";
  حالة HB_PAINT_مرکب_نما_SOFT_LIGHT:     أرجع "SoftLight";
  حالة HB_PAINT_مرکب_نما_DIFFERENCE:     أرجع "Difference";
  حالة HB_PAINT_مرکب_نما_EXCLUSION:      أرجع "Exclusion";
  حالة HB_PAINT_مرکب_نما_HSL_HUE:        أرجع "Hue";
  حالة HB_PAINT_مرکب_نما_HSL_SATURATION: أرجع "Saturation";
  حالة HB_PAINT_مرکب_نما_HSL_COLOR:      أرجع "Color";
  حالة HB_PAINT_مرکب_نما_HSL_LUMINOSITY: أرجع "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  حالة HB_PAINT_مرکب_نما_PLUS:           أرجع "Screen";
  حالة HB_PAINT_مرکب_نما_XOR:            أرجع "Difference";
  حالة HB_PAINT_مرکب_نما_CLEAR:
  حالة HB_PAINT_مرکب_نما_SRC:
  حالة HB_PAINT_مرکب_نما_DEST:
  حالة HB_PAINT_مرکب_نما_SRC_OVER:
  حالة HB_PAINT_مرکب_نما_DEST_OVER:
  حالة HB_PAINT_مرکب_نما_SRC_درون:
  حالة HB_PAINT_مرکب_نما_DEST_درون:
  حالة HB_PAINT_مرکب_نما_SRC_بیرون:
  حالة HB_PAINT_مرکب_نما_DEST_بیرون:
  حالة HB_PAINT_مرکب_نما_SRC_ATOP:
  حالة HB_PAINT_مرکب_نما_DEST_ATOP:
  افتراضي:                                     أرجع اشاره_تهی; /*نرمال*/
  }
}

ساكن فراغ
hb_pdf_paint_افزودن_گروه (hb_paint_funcs_t *,
			 فراغ *paint_داده,
			 فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("q\n");
}

ساكن فراغ
hb_pdf_paint_افزودن_گروه_for (hb_paint_funcs_t *,
			     فراغ *paint_داده,
			     hb_paint_مرکب_نما_t نما,
			     فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودکار &body = paint->current_body ();
  body.append_str ("q\n");

  ثابت حرف *bm = hb_pdf_blend_نما_name (نما);
  إذا (bm)
  {
    خودکار *res = hb_pdf_get_resources (paint);
    إذا (likely (res))
    {
      بیعلامت gs_idx = res->جمع_کن_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

ساكن فراغ
hb_pdf_paint_برداشت_گروه (hb_paint_funcs_t *,
			فراغ *paint_داده,
			hb_paint_مرکب_نما_t نما HB_استفادهنشده,
			فراغ *)
{
  خودکار *paint = (hb_وکتور_paint_t *) paint_داده;
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
    hb_paint_funcs_مجموعه_افزودن_transform_func (funcs, (hb_paint_افزودن_transform_func_t) hb_pdf_paint_افزودن_transform, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_transform_func (funcs, (hb_paint_برداشت_transform_func_t) hb_pdf_paint_برداشت_transform, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_glyph_func (funcs, (hb_paint_افزودن_clip_glyph_func_t) hb_pdf_paint_افزودن_clip_glyph, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_rectangle_func (funcs, (hb_paint_افزودن_clip_rectangle_func_t) hb_pdf_paint_افزودن_clip_rectangle, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_مسیر_شروع_func (funcs, (hb_paint_افزودن_clip_مسیر_شروع_func_t) hb_pdf_paint_افزودن_clip_مسیر_شروع, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_clip_مسیر_انتها_func (funcs, (hb_paint_افزودن_clip_مسیر_انتها_func_t) hb_pdf_paint_افزودن_clip_مسیر_انتها, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_clip_func (funcs, (hb_paint_برداشت_clip_func_t) hb_pdf_paint_برداشت_clip, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_خطی_gradient_func (funcs, (hb_paint_خطی_gradient_func_t) hb_pdf_paint_خطی_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_گروه_func (funcs, (hb_paint_افزودن_گروه_func_t) hb_pdf_paint_افزودن_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_افزودن_گروه_for_func (funcs, (hb_paint_افزودن_گروه_for_func_t) hb_pdf_paint_افزودن_گروه_for, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_برداشت_گروه_func (funcs, (hb_paint_برداشت_گروه_func_t) hb_pdf_paint_برداشت_گروه, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_مجموعه_budget_func (funcs, hb_وکتور_paint_مجموعه_budget_callback, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_get_budget_func (funcs, hb_وکتور_paint_get_budget_callback, اشاره_تهی, اشاره_تهی);
    hb_paint_funcs_مجموعه_get_budget_مانده_func (funcs, hb_وکتور_paint_get_budget_مانده_callback, اشاره_تهی, اشاره_تهی);
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
hb_وکتور_paint_pdf_funcs_get ()
{
  أرجع static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_وکتور_paint_render_pdf (hb_وکتور_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع اشاره_تهی;
  إذا (!paint->گروه_پشته.درازی ||
      !paint->گروه_پشته.arrayZ[0].درازی)
    أرجع اشاره_تهی;

  hb_وکتور_buf_t &content = paint->گروه_پشته.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  عائم ex = paint->extents.x;
  عائم ey = paint->extents.y;
  عائم ew = paint->extents.عرض;
  عائم eh = paint->extents.ارتفاع;

  بیعلامت رقم_extra = res ? res->objects.درازی : 0;
  بیعلامت مجموع_objects = 4 + رقم_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_وکتور_buf_t بیرون;
  hb_buf_recover_recycled (paint->recycled_blob, &بیرون);
  بیرون.alloc (content.درازی + رقم_extra * 128 + 1024);

  hb_وکتور_t<بیعلامت> offsets;
  إذا (unlikely (!offsets.resize (مجموع_objects)))
    أرجع اشاره_تهی;

  بیرون.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = بیرون.درازی;
  بیرون.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = بیرون.درازی;
  بیرون.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = بیرون.درازی;
  بیرون.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  بیرون.append_رقم (ex);
  بیرون.append_c (' ');
  بیرون.append_رقم (ey);
  بیرون.append_c (' ');
  بیرون.append_رقم (ex + ew);
  بیرون.append_c (' ');
  بیرون.append_رقم (ey + eh);
  بیرون.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  منطقي has_resources = res &&
    (res->extgstate_dict.درازی || res->shading_dict.درازی || res->xobject_dict.درازی);
  إذا (has_resources)
  {
    بیرون.append_str ("\n/Resources <<");
    إذا (res->extgstate_dict.درازی)
    {
      بیرون.append_str (" /ExtGState << ");
      بیرون.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.درازی);
      بیرون.append_str (">>");
    }
    إذا (res->shading_dict.درازی)
    {
      بیرون.append_str (" /Shading << ");
      بیرون.append_len (res->shading_dict.arrayZ, res->shading_dict.درازی);
      بیرون.append_str (">>");
    }
    إذا (res->xobject_dict.درازی)
    {
      بیرون.append_str (" /XObject << ");
      بیرون.append_len (res->xobject_dict.arrayZ, res->xobject_dict.درازی);
      بیرون.append_str (">>");
    }
    بیرون.append_str (" >>");
  }

  بیرون.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_وکتور_buf_t bg_پیشوند;
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
	بیعلامت gs_idx = res->جمع_کن_extgstate_alpha (a);
	bg_پیشوند.append_str ("/GS");
	bg_پیشوند.append_unsigned (gs_idx);
	bg_پیشوند.append_str (" gs\n");
      }
    }
    bg_پیشوند.append_رقم (r, 4);
    bg_پیشوند.append_c (' ');
    bg_پیشوند.append_رقم (g, 4);
    bg_پیشوند.append_c (' ');
    bg_پیشوند.append_رقم (b, 4);
    bg_پیشوند.append_str (" rg\n");
    bg_پیشوند.append_رقم (ex);
    bg_پیشوند.append_c (' ');
    bg_پیشوند.append_رقم (ey);
    bg_پیشوند.append_c (' ');
    bg_پیشوند.append_رقم (ew);
    bg_پیشوند.append_c (' ');
    bg_پیشوند.append_رقم (eh);
    bg_پیشوند.append_str (" re f\n");
  }
  بیعلامت جریان_len = bg_پیشوند.درازی + content.درازی;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = بیرون.درازی;
  بیرون.append_str ("4 0 obj\n<< /Length ");
  بیرون.append_unsigned (جریان_len);
  بیرون.append_str (" >>\nstream\n");
  بیرون.append_len (bg_پیشوند.arrayZ, bg_پیشوند.درازی);
  بیرون.append_len (content.arrayZ, content.درازی);
  بیرون.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  لكل (بیعلامت i = 0; i < رقم_extra; i++)
  {
    offsets.arrayZ[4 + i] = بیرون.درازی;
    بیرون.append_unsigned (5 + i);
    بیرون.append_str (" 0 obj\n");
    خودکار &obj = res->objects.arrayZ[i];
    بیرون.append_len (obj.داده.arrayZ, obj.داده.درازی);
    بیرون.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  بیعلامت xref_جابه_جایی = بیرون.درازی;
  بیرون.append_str ("xref\n0 ");
  بیرون.append_unsigned (مجموع_objects + 1);
  بیرون.append_str ("\n0000000000 65535 f \n");
  لكل (بیعلامت i = 0; i < مجموع_objects; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    بیرون.append_len (tmp, 20);
  }

  /* Trailer */
  بیرون.append_str ("trailer\n<< /Size ");
  بیرون.append_unsigned (مجموع_objects + 1);
  بیرون.append_str (" /Root 1 0 R >>\nstartxref\n");
  بیرون.append_unsigned (xref_جابه_جایی);
  بیرون.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &بیرون);

  أرجع blob;
}

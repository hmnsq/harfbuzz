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

#جيڪڏهن_وصف_ٿيل HAVE_ZLIB
#ضمن <zlib.h>
#جيڪڏهن_پورو

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
  hb_ويڪٽر_buf_t معلومات;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
ساكن منطقي
hb_pdf_build_اشاريي_وارو_smask (hb_ويڪٽر_buf_t *ٻاهريون,
			    ثابت حرف *idat_معلومات, بنانشان idat_len,
			    بنانشان ويڪر, بنانشان اوچائي,
			    ثابت uint8_t *trns, بنانشان trns_len);

بنية hb_pdf_resources_t
{
  hb_ويڪٽر_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_ويڪٽر_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_ويڪٽر_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_ويڪٽر_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  بنانشان extgstate_ڳڻپ = 0;
  بنانشان shading_ڳڻپ = 0;
  بنانشان xobject_ڳڻپ = 0;

  بنانشان جوڙيو_object (hb_ويڪٽر_buf_t &&obj_معلومات)
  {
    بنانشان id = 5 + objects.ڊيگهائي; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.معلومات = std::move (obj_معلومات);
    objects.push (std::move (obj));
    أرجع id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  بنانشان جوڙيو_extgstate_alpha (عائم alpha)
  {
    بنانشان idx = extgstate_ڳڻپ++;
    hb_ويڪٽر_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_انگ (alpha, 4);
    obj.append_str (" >>");
    بنانشان obj_id = جوڙيو_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  بنانشان جوڙيو_extgstate_blend (ثابت حرف *bm)
  {
    بنانشان idx = extgstate_ڳڻپ++;
    hb_ويڪٽر_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    بنانشان obj_id = جوڙيو_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  بنانشان جوڙيو_extgstate_smask (بنانشان alpha_shading_id,
				عائم bbox_x, عائم bbox_y,
				عائم bbox_w, عائم bbox_h,
				بنانشان precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_ويڪٽر_buf_t form_وهڪرو;
    form_وهڪرو.append_str ("/SHa sh\n");

    hb_ويڪٽر_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_انگ (bbox_x, precision);
    form.append_c (' ');
    form.append_انگ (bbox_y, precision);
    form.append_c (' ');
    form.append_انگ (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_انگ (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_وهڪرو.ڊيگهائي);
    form.append_str (" >>\nstream\n");
    form.append_len (form_وهڪرو.arrayZ, form_وهڪرو.ڊيگهائي);
    form.append_str ("endstream");
    بنانشان form_id = جوڙيو_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    بنانشان idx = extgstate_ڳڻپ++;
    hb_ويڪٽر_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    بنانشان gs_id = جوڙيو_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    أرجع idx;
  }

  /* Add a shading, return resource name index. */
  بنانشان جوڙيو_shading (hb_ويڪٽر_buf_t &&shading_معلومات)
  {
    بنانشان obj_id = جوڙيو_object (std::move (shading_معلومات));
    أرجع جوڙيو_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  بنانشان جوڙيو_shading_by_id (بنانشان obj_id)
  {
    بنانشان idx = shading_ڳڻپ++;
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
  بنانشان جوڙيو_xobject_png_image (ثابت حرف *idat_معلومات, بنانشان idat_len,
				  بنانشان ويڪر, بنانشان اوچائي,
				  بنانشان colors, منطقي has_alpha,
				  ثابت uint8_t *plte = خالي_اشارو,
				  بنانشان plte_len = 0,
				  ثابت uint8_t *trns = خالي_اشارو,
				  بنانشان trns_len = 0)
  {
    بنانشان idx = xobject_ڳڻپ++;
    hb_ويڪٽر_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (ويڪر);
    obj.append_str (" /Height ");
    obj.append_unsigned (اوچائي);
    obj.append_str ("\n/BitsPerComponent 8\n");

    إذا (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      بنانشان n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      لكل (بنانشان i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    وإلا
    {
      obj.append_str ("/ColorSpace ");
      بنانشان color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    بنانشان smask_id = 0;
    إذا (plte && trns && trns_len)
    {
      hb_ويڪٽر_buf_t smask_وهڪرو;
      إذا (hb_pdf_build_اشاريي_وارو_smask (&smask_وهڪرو, idat_معلومات, idat_len,
				      ويڪر, اوچائي, trns, trns_len))
      {
	hb_ويڪٽر_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (ويڪر);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (اوچائي);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_وهڪرو.ڊيگهائي);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_وهڪرو.arrayZ, smask_وهڪرو.ڊيگهائي);
	smask_obj.append_str ("\nendstream");
	smask_id = جوڙيو_object (std::move (smask_obj));
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
    obj.append_unsigned (ويڪر);
    obj.append_str (" >>\n");
    obj.append_str ("/Length ");
    obj.append_unsigned (idat_len);
    obj.append_str (" >>\nstream\n");
    obj.append_len (idat_معلومات, idat_len);
    obj.append_str ("\nendstream");

    (فراغ) has_alpha;

    بنانشان obj_id = جوڙيو_object (std::move (obj));

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
hb_pdf_get_resources (hb_ويڪٽر_paint_t *paint)
{
  إذا (!paint->defs.ڊيگهائي)
  {
    /* First call: allocate and store. */
    خودڪار *res = (hb_pdf_resources_t *) hb_calloc (1, حجم (hb_pdf_resources_t));
    إذا (unlikely (!res)) أرجع خالي_اشارو;
    جديد (res) hb_pdf_resources_t ();
    إذا (unlikely (!paint->defs.resize (حجم (فراغ *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      أرجع خالي_اشارو;
    }
    يادگيري_نقل (paint->defs.arrayZ, &res, حجم (فراغ *));
  }
  hb_pdf_resources_t *res;
  يادگيري_نقل (&res, paint->defs.arrayZ, حجم (فراغ *));
  أرجع res;
}

فراغ
hb_ويڪٽر_paint_pdf_free_resources (hb_ويڪٽر_paint_t *paint)
{
  إذا (paint->defs.ڊيگهائي >= حجم (فراغ *))
  {
    hb_pdf_resources_t *res;
    يادگيري_نقل (&res, paint->defs.arrayZ, حجم (فراغ *));
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
hb_pdf_emit_glyph_رستو (hb_ويڪٽر_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_ويڪٽر_buf_t *buf)
{
  paint->رستو.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  إذا (likely (paint->budget_بچيل >= 0))
  {
    hb_ويڪٽر_رستو_sink_t sink = {&paint->رستو, paint->get_precision (),
				 paint->x_scale_ضرب_ڪندڙ, paint->y_scale_ضرب_ڪندڙ,
				 &paint->budget_بچيل};
    hb_font_draw_glyph (font, glyph,
			hb_ويڪٽر_pdf_رستو_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->رستو.arrayZ, paint->رستو.ڊيگهائي);
  paint->رستو.clear ();
}


/* ---- paint callbacks ---- */

ساكن فراغ
hb_pdf_paint_ڌڪو_transform (hb_paint_funcs_t *,
			     فراغ *paint_معلومات,
			     عائم xx, عائم yx,
			     عائم xy, عائم yy,
			     عائم dx, عائم dy,
			     فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودڪار &body = paint->current_body ();
  بنانشان sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_انگ (xx, sprec);
  body.append_c (' ');
  body.append_انگ (yx, sprec);
  body.append_c (' ');
  body.append_انگ (xy, sprec);
  body.append_c (' ');
  body.append_انگ (yy, sprec);
  body.append_c (' ');
  body.append_انگ (paint->sx (dx));
  body.append_c (' ');
  body.append_انگ (paint->sy (dy));
  body.append_str (" cm\n");
}

ساكن فراغ
hb_pdf_paint_ڪڍو_transform (hb_paint_funcs_t *,
			    فراغ *paint_معلومات,
			    فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}

ساكن فراغ
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 فراغ *paint_معلومات,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  عائم r = hb_color_get_red (c) / 255.f;
  عائم g = hb_color_get_green (c) / 255.f;
  عائم b = hb_color_get_blue (c) / 255.f;
  عائم a = hb_color_get_alpha (c) / 255.f;

  إذا (a < 1.f / 255.f)
    أرجع;

  خودڪار &body = paint->current_body ();

  منطقي scoped = خطأ;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  إذا (a < 1.f - 1.f / 512.f)
  {
    خودڪار *res = hb_pdf_get_resources (paint);
    إذا (res)
    {
      بنانشان gs_idx = res->جوڙيو_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = سچ;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_انگ (r, 4);
  body.append_c (' ');
  body.append_انگ (g, 4);
  body.append_c (' ');
  body.append_انگ (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_رستو (paint, font, glyph, &body);
  body.append_str ("f\n");
  إذا (scoped)
    body.append_str ("Q\n");
}

ساكن فراغ
hb_pdf_paint_ڌڪو_clip_glyph (hb_paint_funcs_t *,
			      فراغ *paint_معلومات,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودڪار &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_رستو (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

ساكن فراغ
hb_pdf_paint_ڌڪو_clip_rectangle (hb_paint_funcs_t *,
				  فراغ *paint_معلومات,
				  عائم xmin, عائم ymin,
				  عائم xmax, عائم ymax,
				  فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودڪار &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_انگ (paint->sx (xmin));
  body.append_c (' ');
  body.append_انگ (paint->sy (ymin));
  body.append_c (' ');
  body.append_انگ (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_انگ (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

ساكن hb_draw_funcs_t *
hb_pdf_paint_ڌڪو_clip_رستو_شروعات (hb_paint_funcs_t *,
				   فراغ *paint_معلومات,
				   فراغ **draw_معلومات,
				   فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
  {
    *draw_معلومات = خالي_اشارو;
    أرجع خالي_اشارو;
  }

  خودڪار &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_رستو_sink = {&body, paint->get_precision (),
			   paint->x_scale_ضرب_ڪندڙ,
			   paint->y_scale_ضرب_ڪندڙ,
			   &paint->budget_بچيل};
  *draw_معلومات = &paint->clip_رستو_sink;
  أرجع hb_ويڪٽر_pdf_رستو_draw_funcs_get ();
}

ساكن فراغ
hb_pdf_paint_ڌڪو_clip_رستو_پڇاڙي (hb_paint_funcs_t *,
				 فراغ *paint_معلومات,
				 فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("W n\n");
}

ساكن فراغ
hb_pdf_paint_ڪڍو_clip (hb_paint_funcs_t *,
		       فراغ *paint_معلومات,
		       فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
ساكن فراغ
hb_pdf_paint_solid_color (hb_ويڪٽر_paint_t *paint, hb_color_t c)
{
  خودڪار &body = paint->current_body ();

  عائم r = hb_color_get_red (c) / 255.f;
  عائم g = hb_color_get_green (c) / 255.f;
  عائم b = hb_color_get_blue (c) / 255.f;
  عائم a = hb_color_get_alpha (c) / 255.f;

  إذا (a < 1.f / 255.f)
    أرجع;

  /* Set alpha via ExtGState if needed. */
  إذا (a < 1.f - 1.f / 512.f)
  {
    خودڪار *res = hb_pdf_get_resources (paint);
    إذا (res)
    {
      بنانشان gs_idx = res->جوڙيو_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_انگ (r, 4);
  body.append_c (' ');
  body.append_انگ (g, 4);
  body.append_c (' ');
  body.append_انگ (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

ساكن فراغ
hb_pdf_paint_color (hb_paint_funcs_t *,
		    فراغ *paint_معلومات,
		    hb_bool_t,
		    hb_color_t color,
		    فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
ساكن منطقي
hb_pdf_build_اشاريي_وارو_smask (hb_ويڪٽر_buf_t *ٻاهريون,
			    ثابت حرف *idat_معلومات, بنانشان idat_len,
			    بنانشان ويڪر, بنانشان اوچائي,
			    ثابت uint8_t *trns, بنانشان trns_len)
{
#جيڪڏهن_وصف_نه_ٿيل HAVE_ZLIB
  (فراغ) ٻاهريون; (فراغ) idat_معلومات; (فراغ) idat_len;
  (فراغ) ويڪر; (فراغ) اوچائي; (فراغ) trns; (فراغ) trns_len;
  أرجع خطأ;
#وإلا
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  بنانشان raw_len, نقاب_len;
  إذا (!ويڪر || !اوچائي ||
      ويڪر == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (ويڪر + 1, اوچائي, &raw_len) ||
      hb_unsigned_mul_overflows (ويڪر, اوچائي, &نقاب_len))
    أرجع خطأ;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  إذا (!raw) أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (raw));

  z_وهڪرو وهڪرو = {};
  وهڪرو.ايندڙ_اندريون = (Bytef *) idat_معلومات;
  وهڪرو.avail_اندريون = idat_len;
  وهڪرو.ايندڙ_ٻاهريون = (Bytef *) raw;
  وهڪرو.avail_ٻاهريون = raw_len;

  إذا (inflateInit (&وهڪرو) != Z_OK)
    أرجع خطأ;
  صحيح حالت = inflate (&وهڪرو, Z_پڄاڻي);
  بنانشان ڊگهو ڪل_ٻاهريون = وهڪرو.ڪل_ٻاهريون;
  inflate_پڇاڙي (&وهڪرو);
  إذا (حالت != Z_وهڪرو_پڇاڙي || ڪل_ٻاهريون != raw_len)
    أرجع خطأ;

  /* Un-filter and map to alpha. */
  إذا (!ٻاهريون->resize (نقاب_len))
    أرجع خطأ;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (ويڪر);
  إذا (!unfiltered)
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (ويڪر, 1);
  إذا (!prev_unfiltered)
    أرجع خطأ;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  لكل (بنانشان y = 0; y < اوچائي; y++)
  {
    uint8_t *صف = raw + y * (ويڪر + 1);
    uint8_t filter = صف[0];
    uint8_t *pixels = صف + 1;

    لكل (بنانشان x = 0; x < ويڪر; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      بدل (filter)
      {
      حالة 0: unfiltered[x] = val; اكسر; /* None */
      حالة 1: unfiltered[x] = val + a; اكسر; /* Sub */
      حالة 2: unfiltered[x] = val + b; اكسر; /*مٿي*/
      حالة 3: unfiltered[x] = val + (uint8_t) (((بنانشان) a + b) / 2); اكسر; /*اوسط*/
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
      ٻاهريون->arrayZ[y * ويڪر + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = prev_unfiltered;
    prev_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  أرجع سچ;
#جيڪڏهن_پورو
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
		    فراغ *paint_معلومات,
		    hb_blob_t *image,
		    بنانشان ويڪر,
		    بنانشان اوچائي,
		    hb_ٽيگ_t format,
		    عائم slant HB_اڻ_استعمال_ٿيل,
		    hb_glyph_extents_t *extents,
		    فراغ *)
{
  إذا (format != HB_ٽيگ ('p','n','g',' '))
    أرجع خطأ;
  إذا (!extents || !ويڪر || !اوچائي)
    أرجع خطأ;

  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع خطأ;
  خودڪار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع خطأ;

  بنانشان len = 0;
  ثابت uint8_t *معلومات = (ثابت uint8_t *) hb_blob_get_معلومات (image, &len);
  إذا (!معلومات || len < 8)
    أرجع خطأ;

  /* Verify PNG signature. */
  ساكن ثابت uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  إذا (hb_memcmp (معلومات, png_sig, 8) != 0)
    أرجع خطأ;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  بنانشان png_w = 0, png_h = 0;
  بنانشان colors = 3; /* default RGB */
  uint8_t color_type = 2;
  منطقي has_alpha = خطأ;
  ثابت uint8_t *plte_معلومات = خالي_اشارو;
  بنانشان plte_len = 0;
  ثابت uint8_t *trns_معلومات = خالي_اشارو;
  بنانشان trns_len = 0;
  hb_ويڪٽر_buf_t idat;

  بنانشان pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  طالما (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (معلومات + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (معلومات + pos + 4);
    إذا (chunk_len > len - pos - 12)
      اكسر;
    ثابت uint8_t *chunk_معلومات = معلومات + pos + 8;

    إذا (chunk_type == 0x49484452u) /* IHDR */
    {
      إذا (chunk_len < 13) أرجع خطأ;
      png_w = hb_pdf_png_u32 (chunk_معلومات);
      png_h = hb_pdf_png_u32 (chunk_معلومات + 4);
      uint8_t ٻٽو_انگ_اونهائي_2 = chunk_معلومات[8];
      color_type = chunk_معلومات[9];
      إذا (ٻٽو_انگ_اونهائي_2 != 8) أرجع خطأ; /* only 8-bit supported */

      بدل (color_type)
      {
      حالة 0: colors = 1; has_alpha = خطأ; اكسر; /* Grayscale */
      حالة 2: colors = 3; has_alpha = خطأ; اكسر; /* RGB */
      حالة 3: colors = 1; has_alpha = خطأ; اكسر; /*اشاريي_وارو*/
      حالة 4: colors = 2; has_alpha = سچ;  اكسر; /* Gray+Alpha */
      حالة 6: colors = 4; has_alpha = سچ;  اكسر; /* RGBA */
      افتراضي: أرجع خطأ;
      }
    }
    وإلا إذا (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_معلومات = chunk_معلومات;
      plte_len = chunk_len;
    }
    وإلا إذا (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_معلومات = chunk_معلومات;
      trns_len = chunk_len;
    }
    وإلا إذا (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ثابت حرف *) chunk_معلومات, chunk_len);
    }
    وإلا إذا (chunk_type == 0x49454E44u) /* IEND */
      اكسر;

    pos += 12 + chunk_len;
  }

  إذا (!png_w || !png_h || !idat.ڊيگهائي)
    أرجع خطأ;

  بنانشان im_idx = res->جوڙيو_xobject_png_image (idat.arrayZ, idat.ڊيگهائي,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_معلومات, plte_len,
						 trns_معلومات, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  خودڪار &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  عائم ix = (عائم) extents->x_bearing;
  عائم iy = (عائم) extents->y_bearing + (عائم) extents->اوچائي;
  عائم iw = (عائم) extents->ويڪر;
  عائم ih = -(عائم) extents->اوچائي; /* negative because image Y goes up but height is negative in extents */

  body.append_انگ (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_انگ (paint->sy (ih));
  body.append_c (' ');
  body.append_انگ (paint->sx (ix));
  body.append_c (' ');
  body.append_انگ (paint->sy (iy));
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
  لكل (ثابت خودڪار &s : stops)
    إذا (hb_color_get_alpha (s.color) != 255)
      أرجع سچ;
  أرجع خطأ;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
ساكن فراغ
hb_pdf_build_alpha_interpolation_function (hb_ويڪٽر_buf_t *obj,
					   عائم a0, عائم a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_انگ (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_انگ (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
ساكن بنانشان
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_ويڪٽر_paint_t *paint)
{
  بنانشان ڳڻپ = paint->color_stops_scratch.ڊيگهائي;

  إذا (ڳڻپ < 2)
  {
    عائم a = ڳڻپ ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_ويڪٽر_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    أرجع res->جوڙيو_object (std::move (obj));
  }

  إذا (ڳڻپ == 2)
  {
    hb_ويڪٽر_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    أرجع res->جوڙيو_object (std::move (obj));
  }

  hb_ويڪٽر_t<بنانشان> sub_func_ids;
  لكل (بنانشان i = 0; i + 1 < ڳڻپ; i++)
  {
    hb_ويڪٽر_buf_t sub;
    hb_pdf_build_alpha_interpolation_function (&sub,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i + 1].color) / 255.f);
    sub_func_ids.push (res->جوڙيو_object (std::move (sub)));
  }

  hb_ويڪٽر_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");
  obj.append_str ("/Functions [");
  لكل (بنانشان i = 0; i < sub_func_ids.ڊيگهائي; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  لكل (بنانشان i = 1; i + 1 < ڳڻپ; i++)
  {
    إذا (i > 1) obj.append_c (' ');
    obj.append_انگ (paint->color_stops_scratch.arrayZ[i].وٿي, 4);
  }
  obj.append_str ("]\n/Encode [");
  لكل (بنانشان i = 0; i + 1 < ڳڻپ; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  أرجع res->جوڙيو_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
ساكن فراغ
hb_pdf_build_interpolation_function (hb_ويڪٽر_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_انگ (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_انگ (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_انگ (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_انگ (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_انگ (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_انگ (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
ساكن بنانشان
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_ويڪٽر_paint_t *paint)
{
  بنانشان ڳڻپ = paint->color_stops_scratch.ڊيگهائي;

  إذا (ڳڻپ < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = ڳڻپ ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_ويڪٽر_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    أرجع res->جوڙيو_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { أرجع (a.وٿي > b.وٿي) - (a.وٿي < b.وٿي); });

  إذا (ڳڻپ == 2)
  {
    /* Two stops: single interpolation function. */
    hb_ويڪٽر_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    أرجع res->جوڙيو_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_ويڪٽر_t<بنانشان> sub_func_ids;
  لكل (بنانشان i = 0; i + 1 < ڳڻپ; i++)
  {
    hb_ويڪٽر_buf_t sub;
    hb_pdf_build_interpolation_function (&sub,
					  paint->color_stops_scratch.arrayZ[i].color,
					  paint->color_stops_scratch.arrayZ[i + 1].color);
    sub_func_ids.push (res->جوڙيو_object (std::move (sub)));
  }

  /* Stitching function (Type 3). */
  hb_ويڪٽر_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");

  /* Functions array. */
  obj.append_str ("/Functions [");
  لكل (بنانشان i = 0; i < sub_func_ids.ڊيگهائي; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  لكل (بنانشان i = 1; i + 1 < ڳڻپ; i++)
  {
    إذا (i > 1) obj.append_c (' ');
    obj.append_انگ (paint->color_stops_scratch.arrayZ[i].وٿي, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  لكل (بنانشان i = 0; i + 1 < ڳڻپ; i++)
  {
    إذا (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  أرجع res->جوڙيو_object (std::move (obj));
}

ساكن فراغ
hb_pdf_paint_لڪيري_gradient (hb_paint_funcs_t *,
			      فراغ *paint_معلومات,
			      hb_color_سٽ_t *color_سٽ,
			      عائم x0, عائم y0,
			      عائم x1, عائم y1,
			      عائم x2 HB_اڻ_استعمال_ٿيل, عائم y2 HB_اڻ_استعمال_ٿيل,
			      فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودڪار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  إذا (!paint->fetch_color_stops (color_سٽ))
    أرجع;
  hb_ويڪٽر_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سٽ (stops.arrayZ, stops.ڊيگهائي, &mn, &mx);
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);

  بنانشان func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_سٽ_get_extend (color_سٽ);
  ثابت حرف *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_ويڪٽر_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_انگ (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_انگ (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_انگ (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_انگ (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  بنانشان sh_idx = res->جوڙيو_shading (std::move (sh));

  خودڪار &body = paint->current_body ();

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  إذا (needs_alpha)
  {
    بنانشان alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_ويڪٽر_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_انگ (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_انگ (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_انگ (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_انگ (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    بنانشان alpha_sh_id = res->جوڙيو_object (std::move (ash));

    بنانشان gs_idx = res->جوڙيو_extgstate_smask (alpha_sh_id,
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
			      فراغ *paint_معلومات,
			      hb_color_سٽ_t *color_سٽ,
			      عائم x0, عائم y0, عائم r0,
			      عائم x1, عائم y1, عائم r1,
			      فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودڪار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  إذا (!paint->fetch_color_stops (color_سٽ))
    أرجع;
  hb_ويڪٽر_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  عائم mn, mx;
  hb_paint_normalize_color_سٽ (stops.arrayZ, stops.ڊيگهائي, &mn, &mx);
  عائم gx0 = x0 + mn * (x1 - x0);
  عائم gy0 = y0 + mn * (y1 - y0);
  عائم gr0 = r0 + mn * (r1 - r0);
  عائم gx1 = x0 + mx * (x1 - x0);
  عائم gy1 = y0 + mx * (y1 - y0);
  عائم gr1 = r0 + mx * (r1 - r0);

  بنانشان func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_سٽ_get_extend (color_سٽ);
  ثابت حرف *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_ويڪٽر_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_انگ (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_انگ (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_انگ (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_انگ (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_انگ (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_انگ (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  بنانشان sh_idx = res->جوڙيو_shading (std::move (sh));

  خودڪار &body = paint->current_body ();

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  إذا (needs_alpha)
  {
    بنانشان alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_ويڪٽر_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_انگ (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_انگ (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_انگ (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_انگ (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_انگ (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_انگ (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    بنانشان alpha_sh_id = res->جوڙيو_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    عائم cx = (gr1 >= gr0) ? gx1 : gx0;
    عائم cy = (gr1 >= gr0) ? gy1 : gy0;
    عائم rr = hb_max (gr0, gr1);
    بنانشان gs_idx = res->جوڙيو_extgstate_smask (alpha_sh_id,
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
hb_pdf_encode_u16 (hb_ويڪٽر_buf_t *buf, uint16_t v)
{
  حرف بائيٽون[2] = {(حرف) (v >> 8), (حرف) (v & 0xFF)};
  buf->append_len (بائيٽون, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
ساكن فراغ
hb_pdf_encode_coord (hb_ويڪٽر_buf_t *buf,
		     عائم val, عائم lo, عائم hi)
{
  عائم t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
ساكن فراغ
hb_pdf_encode_color_rgb (hb_ويڪٽر_buf_t *buf, hb_color_t c)
{
  حرف rgb[3] = {(حرف) hb_color_get_red (c),
		 (حرف) hb_color_get_green (c),
		 (حرف) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
ساكن فراغ
hb_pdf_encode_color_alpha (hb_ويڪٽر_buf_t *buf, hb_color_t c)
{
  حرف a = (حرف) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
ساكن فراغ
hb_pdf_encode_نقطو (hb_ويڪٽر_buf_t *buf,
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
hb_pdf_جوڙيو_sweep_patch (hb_ويڪٽر_buf_t *mesh,
			hb_ويڪٽر_buf_t *alpha_mesh,
			عائم cx, عائم cy,
			عائم xlo, عائم xhi, عائم ylo, عائم yhi,
			عائم a0, hb_color_t c0_اندريون,
			عائم a1, hb_color_t c1_اندريون)
{
  ثابت عائم R = 32767.f;
  ثابت عائم eps = 0.5f;
  ثابت عائم MAX_SECTOR = (عائم) M_PI / 2.f;

  صحيح انگ_splits = (صحيح) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  إذا (انگ_splits < 1) انگ_splits = 1;

  لكل (صحيح s = 0; s < انگ_splits; s++)
  {
    عائم k0 = (عائم) s / انگ_splits;
    عائم k1 = (عائم) (s + 1) / انگ_splits;
    عائم sa0 = a0 + k0 * (a1 - a0);
    عائم sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_اندريون, c1_اندريون, k0);
    hb_color_t sc1 = hb_color_lerp (c0_اندريون, c1_اندريون, k1);

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

    hb_pdf_encode_نقطو (mesh, p0x, p0y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, p3x, p3y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, p6x, p6y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, p9x, p9y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_نقطو (mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

    hb_pdf_encode_color_rgb (mesh, sc0); /* inner start */
    hb_pdf_encode_color_rgb (mesh, sc0); /* outer start */
    hb_pdf_encode_color_rgb (mesh, sc1); /* outer end */
    hb_pdf_encode_color_rgb (mesh, sc1); /* inner end */

    إذا (alpha_mesh)
    {
      alpha_mesh->append_c ('\0');

      hb_pdf_encode_نقطو (alpha_mesh, p0x, p0y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, p3x, p3y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, p6x, p6y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, p9x, p9y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_نقطو (alpha_mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
    }
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
بنية hb_pdf_sweep_ctx_t {
  hb_ويڪٽر_buf_t *mesh;
  hb_ويڪٽر_buf_t *alpha_mesh;
  عائم cx, cy, xlo, xhi, ylo, yhi;
};

ساكن فراغ
hb_pdf_sweep_emit_patch (عائم a0, hb_color_t c0,
			 عائم a1, hb_color_t c1,
			 فراغ *user_معلومات)
{
  خودڪار *ctx = (hb_pdf_sweep_ctx_t *) user_معلومات;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  إذا (unlikely (ctx->mesh->اندريون_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->اندريون_error ())))
    أرجع;
  hb_pdf_جوڙيو_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

ساكن فراغ
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     فراغ *paint_معلومات,
			     hb_color_سٽ_t *color_سٽ,
			     عائم cx, عائم cy,
			     عائم شروعات_angle,
			     عائم پڇاڙي_angle,
			     فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  خودڪار *res = hb_pdf_get_resources (paint);
  إذا (unlikely (!res))
    أرجع;

  /* Get and sort color stops. */
  إذا (!paint->fetch_color_stops (color_سٽ))
    أرجع;
  hb_ويڪٽر_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (ثابت hb_color_stop_t &a, ثابت hb_color_stop_t &b)
    { أرجع (a.وٿي > b.وٿي) - (a.وٿي < b.وٿي); });

  hb_paint_extend_t extend = hb_color_سٽ_get_extend (color_سٽ);

  ثابت عائم R = 32767.f;
  عائم scx = paint->sx (cx), scy = paint->sy (cy);
  عائم xlo = scx - R - 1, xhi = scx + R + 1;
  عائم ylo = scy - R - 1, yhi = scy + R + 1;

  منطقي needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_ويڪٽر_buf_t mesh;
  hb_ويڪٽر_buf_t alpha_mesh;
  mesh.alloc (256);
  إذا (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : خالي_اشارو,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.ڊيگهائي, extend,
				 شروعات_angle, پڇاڙي_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  إذا (!mesh.ڊيگهائي)
    أرجع;

  خودڪار hb_pdf_build_mesh_shading = [&] (hb_ويڪٽر_buf_t &m,
					 ثابت حرف *cs,
					 ثابت حرف *decode_suffix) -> بنانشان
  {
    hb_ويڪٽر_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_انگ (xlo, 2);
    sh.append_c (' ');
    sh.append_انگ (xhi, 2);
    sh.append_c (' ');
    sh.append_انگ (ylo, 2);
    sh.append_c (' ');
    sh.append_انگ (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.ڊيگهائي);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.ڊيگهائي);
    sh.append_str ("\nendstream");
    أرجع res->جوڙيو_object (std::move (sh));
  };

  بنانشان sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  بنانشان sh_idx = res->جوڙيو_shading_by_id (sh_obj_id);

  خودڪار &body = paint->current_body ();

  إذا (needs_alpha && alpha_mesh.ڊيگهائي)
  {
    بنانشان alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    بنانشان gs_idx = res->جوڙيو_extgstate_smask (alpha_sh_id,
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
hb_pdf_blend_گهڻائي_name (hb_paint_مرکب_گهڻائي_t گهڻائي)
{
  بدل (گهڻائي)
  {
  حالة HB_PAINT_مرکب_گهڻائي_MULTIPLY:       أرجع "Multiply";
  حالة HB_PAINT_مرکب_گهڻائي_SCREEN:         أرجع "Screen";
  حالة HB_PAINT_مرکب_گهڻائي_OVERLAY:        أرجع "Overlay";
  حالة HB_PAINT_مرکب_گهڻائي_DARKEN:         أرجع "Darken";
  حالة HB_PAINT_مرکب_گهڻائي_LIGHTEN:        أرجع "Lighten";
  حالة HB_PAINT_مرکب_گهڻائي_COLOR_DODGE:    أرجع "ColorDodge";
  حالة HB_PAINT_مرکب_گهڻائي_COLOR_BURN:     أرجع "ColorBurn";
  حالة HB_PAINT_مرکب_گهڻائي_HARD_LIGHT:     أرجع "HardLight";
  حالة HB_PAINT_مرکب_گهڻائي_SOFT_LIGHT:     أرجع "SoftLight";
  حالة HB_PAINT_مرکب_گهڻائي_DIFFERENCE:     أرجع "Difference";
  حالة HB_PAINT_مرکب_گهڻائي_EXCLUSION:      أرجع "Exclusion";
  حالة HB_PAINT_مرکب_گهڻائي_HSL_HUE:        أرجع "Hue";
  حالة HB_PAINT_مرکب_گهڻائي_HSL_SATURATION: أرجع "Saturation";
  حالة HB_PAINT_مرکب_گهڻائي_HSL_COLOR:      أرجع "Color";
  حالة HB_PAINT_مرکب_گهڻائي_HSL_LUMINOSITY: أرجع "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  حالة HB_PAINT_مرکب_گهڻائي_PLUS:           أرجع "Screen";
  حالة HB_PAINT_مرکب_گهڻائي_XOR:            أرجع "Difference";
  حالة HB_PAINT_مرکب_گهڻائي_CLEAR:
  حالة HB_PAINT_مرکب_گهڻائي_SRC:
  حالة HB_PAINT_مرکب_گهڻائي_DEST:
  حالة HB_PAINT_مرکب_گهڻائي_SRC_OVER:
  حالة HB_PAINT_مرکب_گهڻائي_DEST_OVER:
  حالة HB_PAINT_مرکب_گهڻائي_SRC_اندريون:
  حالة HB_PAINT_مرکب_گهڻائي_DEST_اندريون:
  حالة HB_PAINT_مرکب_گهڻائي_SRC_ٻاهريون:
  حالة HB_PAINT_مرکب_گهڻائي_DEST_ٻاهريون:
  حالة HB_PAINT_مرکب_گهڻائي_SRC_ATOP:
  حالة HB_PAINT_مرکب_گهڻائي_DEST_ATOP:
  افتراضي:                                     أرجع خالي_اشارو; /*عام*/
  }
}

ساكن فراغ
hb_pdf_paint_ڌڪو_گروهه (hb_paint_funcs_t *,
			 فراغ *paint_معلومات,
			 فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;
  paint->current_body ().append_str ("q\n");
}

ساكن فراغ
hb_pdf_paint_ڌڪو_گروهه_for (hb_paint_funcs_t *,
			     فراغ *paint_معلومات,
			     hb_paint_مرکب_گهڻائي_t گهڻائي,
			     فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
  إذا (unlikely (!paint->ensure_initialized ()))
    أرجع;

  خودڪار &body = paint->current_body ();
  body.append_str ("q\n");

  ثابت حرف *bm = hb_pdf_blend_گهڻائي_name (گهڻائي);
  إذا (bm)
  {
    خودڪار *res = hb_pdf_get_resources (paint);
    إذا (likely (res))
    {
      بنانشان gs_idx = res->جوڙيو_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

ساكن فراغ
hb_pdf_paint_ڪڍو_گروهه (hb_paint_funcs_t *,
			فراغ *paint_معلومات,
			hb_paint_مرکب_گهڻائي_t گهڻائي HB_اڻ_استعمال_ٿيل,
			فراغ *)
{
  خودڪار *paint = (hb_ويڪٽر_paint_t *) paint_معلومات;
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
    hb_paint_funcs_سيٽ_ڌڪو_transform_func (funcs, (hb_paint_ڌڪو_transform_func_t) hb_pdf_paint_ڌڪو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_transform_func (funcs, (hb_paint_ڪڍو_transform_func_t) hb_pdf_paint_ڪڍو_transform, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_glyph_func (funcs, (hb_paint_ڌڪو_clip_glyph_func_t) hb_pdf_paint_ڌڪو_clip_glyph, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_rectangle_func (funcs, (hb_paint_ڌڪو_clip_rectangle_func_t) hb_pdf_paint_ڌڪو_clip_rectangle, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_رستو_شروعات_func (funcs, (hb_paint_ڌڪو_clip_رستو_شروعات_func_t) hb_pdf_paint_ڌڪو_clip_رستو_شروعات, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_clip_رستو_پڇاڙي_func (funcs, (hb_paint_ڌڪو_clip_رستو_پڇاڙي_func_t) hb_pdf_paint_ڌڪو_clip_رستو_پڇاڙي, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_clip_func (funcs, (hb_paint_ڪڍو_clip_func_t) hb_pdf_paint_ڪڍو_clip, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_لڪيري_gradient_func (funcs, (hb_paint_لڪيري_gradient_func_t) hb_pdf_paint_لڪيري_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_گروهه_func (funcs, (hb_paint_ڌڪو_گروهه_func_t) hb_pdf_paint_ڌڪو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڌڪو_گروهه_for_func (funcs, (hb_paint_ڌڪو_گروهه_for_func_t) hb_pdf_paint_ڌڪو_گروهه_for, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_ڪڍو_گروهه_func (funcs, (hb_paint_ڪڍو_گروهه_func_t) hb_pdf_paint_ڪڍو_گروهه, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_سيٽ_budget_func (funcs, hb_ويڪٽر_paint_سيٽ_budget_callback, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_func (funcs, hb_ويڪٽر_paint_get_budget_callback, خالي_اشارو, خالي_اشارو);
    hb_paint_funcs_سيٽ_get_budget_بچيل_func (funcs, hb_ويڪٽر_paint_get_budget_بچيل_callback, خالي_اشارو, خالي_اشارو);
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
hb_ويڪٽر_paint_pdf_funcs_get ()
{
  أرجع static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_ويڪٽر_paint_render_pdf (hb_ويڪٽر_paint_t *paint)
{
  إذا (!paint->has_extents)
    أرجع خالي_اشارو;
  إذا (!paint->گروهه_ڍير.ڊيگهائي ||
      !paint->گروهه_ڍير.arrayZ[0].ڊيگهائي)
    أرجع خالي_اشارو;

  hb_ويڪٽر_buf_t &content = paint->گروهه_ڍير.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  عائم ex = paint->extents.x;
  عائم ey = paint->extents.y;
  عائم ew = paint->extents.ويڪر;
  عائم eh = paint->extents.اوچائي;

  بنانشان انگ_extra = res ? res->objects.ڊيگهائي : 0;
  بنانشان ڪل_objects = 4 + انگ_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_ويڪٽر_buf_t ٻاهريون;
  hb_buf_recover_recycled (paint->recycled_blob, &ٻاهريون);
  ٻاهريون.alloc (content.ڊيگهائي + انگ_extra * 128 + 1024);

  hb_ويڪٽر_t<بنانشان> offsets;
  إذا (unlikely (!offsets.resize (ڪل_objects)))
    أرجع خالي_اشارو;

  ٻاهريون.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  ٻاهريون.append_انگ (ex);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (ey);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (ex + ew);
  ٻاهريون.append_c (' ');
  ٻاهريون.append_انگ (ey + eh);
  ٻاهريون.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  منطقي has_resources = res &&
    (res->extgstate_dict.ڊيگهائي || res->shading_dict.ڊيگهائي || res->xobject_dict.ڊيگهائي);
  إذا (has_resources)
  {
    ٻاهريون.append_str ("\n/Resources <<");
    إذا (res->extgstate_dict.ڊيگهائي)
    {
      ٻاهريون.append_str (" /ExtGState << ");
      ٻاهريون.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.ڊيگهائي);
      ٻاهريون.append_str (">>");
    }
    إذا (res->shading_dict.ڊيگهائي)
    {
      ٻاهريون.append_str (" /Shading << ");
      ٻاهريون.append_len (res->shading_dict.arrayZ, res->shading_dict.ڊيگهائي);
      ٻاهريون.append_str (">>");
    }
    إذا (res->xobject_dict.ڊيگهائي)
    {
      ٻاهريون.append_str (" /XObject << ");
      ٻاهريون.append_len (res->xobject_dict.arrayZ, res->xobject_dict.ڊيگهائي);
      ٻاهريون.append_str (">>");
    }
    ٻاهريون.append_str (" >>");
  }

  ٻاهريون.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_ويڪٽر_buf_t bg_اڳياڙي;
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
	بنانشان gs_idx = res->جوڙيو_extgstate_alpha (a);
	bg_اڳياڙي.append_str ("/GS");
	bg_اڳياڙي.append_unsigned (gs_idx);
	bg_اڳياڙي.append_str (" gs\n");
      }
    }
    bg_اڳياڙي.append_انگ (r, 4);
    bg_اڳياڙي.append_c (' ');
    bg_اڳياڙي.append_انگ (g, 4);
    bg_اڳياڙي.append_c (' ');
    bg_اڳياڙي.append_انگ (b, 4);
    bg_اڳياڙي.append_str (" rg\n");
    bg_اڳياڙي.append_انگ (ex);
    bg_اڳياڙي.append_c (' ');
    bg_اڳياڙي.append_انگ (ey);
    bg_اڳياڙي.append_c (' ');
    bg_اڳياڙي.append_انگ (ew);
    bg_اڳياڙي.append_c (' ');
    bg_اڳياڙي.append_انگ (eh);
    bg_اڳياڙي.append_str (" re f\n");
  }
  بنانشان وهڪرو_len = bg_اڳياڙي.ڊيگهائي + content.ڊيگهائي;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("4 0 obj\n<< /Length ");
  ٻاهريون.append_unsigned (وهڪرو_len);
  ٻاهريون.append_str (" >>\nstream\n");
  ٻاهريون.append_len (bg_اڳياڙي.arrayZ, bg_اڳياڙي.ڊيگهائي);
  ٻاهريون.append_len (content.arrayZ, content.ڊيگهائي);
  ٻاهريون.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  لكل (بنانشان i = 0; i < انگ_extra; i++)
  {
    offsets.arrayZ[4 + i] = ٻاهريون.ڊيگهائي;
    ٻاهريون.append_unsigned (5 + i);
    ٻاهريون.append_str (" 0 obj\n");
    خودڪار &obj = res->objects.arrayZ[i];
    ٻاهريون.append_len (obj.معلومات.arrayZ, obj.معلومات.ڊيگهائي);
    ٻاهريون.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  بنانشان xref_وٿي = ٻاهريون.ڊيگهائي;
  ٻاهريون.append_str ("xref\n0 ");
  ٻاهريون.append_unsigned (ڪل_objects + 1);
  ٻاهريون.append_str ("\n0000000000 65535 f \n");
  لكل (بنانشان i = 0; i < ڪل_objects; i++)
  {
    حرف tmp[21];
    snprintf (tmp, حجم (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    ٻاهريون.append_len (tmp, 20);
  }

  /* Trailer */
  ٻاهريون.append_str ("trailer\n<< /Size ");
  ٻاهريون.append_unsigned (ڪل_objects + 1);
  ٻاهريون.append_str (" /Root 1 0 R >>\nstartxref\n");
  ٻاهريون.append_unsigned (xref_وٿي);
  ٻاهريون.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &ٻاهريون);

  أرجع blob;
}

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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-paint.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-paint.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_ZLIB
#ਅਨ੍ਤਰ੍ਭਾਵ <zlib.h>
#ਜੇ_ਅੰਤ

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

ਬਣਤਰ hb_pdf_obj_t
{
  hb_ਸਦਿਸ਼_buf_t ਅੰਕੜੇ;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
ਸਥਿਰ ਤਾਰਕਿਕ
hb_pdf_build_ਸੂਚਕਾਂਕਿਤ_smask (hb_ਸਦਿਸ਼_buf_t *ਬਾਹਰਲਾ,
			    ਅਚਲ ਅੱਖਰ *idat_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ idat_len,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
			    ਅਚਲ uint8_t *trns, ਬਿਨਾਨਿਸ਼ਾਨੀ trns_len);

ਬਣਤਰ hb_pdf_resources_t
{
  hb_ਸਦਿਸ਼_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_ਸਦਿਸ਼_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_ਸਦਿਸ਼_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_ਸਦਿਸ਼_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  ਬਿਨਾਨਿਸ਼ਾਨੀ extgstate_ਗਿਣਤੀ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ shading_ਗਿਣਤੀ = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ xobject_ਗਿਣਤੀ = 0;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_object (hb_ਸਦਿਸ਼_buf_t &&obj_ਅੰਕੜੇ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ id = 5 + objects.ਲੰਮਾਈ; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.ਅੰਕੜੇ = std::move (obj_ਅੰਕੜੇ);
    objects.push (std::move (obj));
    ਵਾਪਸ id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_extgstate_alpha (ਦਸ਼ਮਲਵ alpha)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = extgstate_ਗਿਣਤੀ++;
    hb_ਸਦਿਸ਼_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_ਅੰਕ (alpha, 4);
    obj.append_str (" >>");
    ਬਿਨਾਨਿਸ਼ਾਨੀ obj_id = ਜੋੜੋ_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    ਵਾਪਸ idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_extgstate_blend (ਅਚਲ ਅੱਖਰ *bm)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = extgstate_ਗਿਣਤੀ++;
    hb_ਸਦਿਸ਼_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    ਬਿਨਾਨਿਸ਼ਾਨੀ obj_id = ਜੋੜੋ_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    ਵਾਪਸ idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_extgstate_smask (ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_shading_id,
				ਦਸ਼ਮਲਵ bbox_x, ਦਸ਼ਮਲਵ bbox_y,
				ਦਸ਼ਮਲਵ bbox_w, ਦਸ਼ਮਲਵ bbox_h,
				ਬਿਨਾਨਿਸ਼ਾਨੀ precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_ਸਦਿਸ਼_buf_t form_ਧਾਰਾ;
    form_ਧਾਰਾ.append_str ("/SHa sh\n");

    hb_ਸਦਿਸ਼_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_ਅੰਕ (bbox_x, precision);
    form.append_c (' ');
    form.append_ਅੰਕ (bbox_y, precision);
    form.append_c (' ');
    form.append_ਅੰਕ (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_ਅੰਕ (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_ਧਾਰਾ.ਲੰਮਾਈ);
    form.append_str (" >>\nstream\n");
    form.append_len (form_ਧਾਰਾ.arrayZ, form_ਧਾਰਾ.ਲੰਮਾਈ);
    form.append_str ("endstream");
    ਬਿਨਾਨਿਸ਼ਾਨੀ form_id = ਜੋੜੋ_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = extgstate_ਗਿਣਤੀ++;
    hb_ਸਦਿਸ਼_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    ਬਿਨਾਨਿਸ਼ਾਨੀ gs_id = ਜੋੜੋ_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    ਵਾਪਸ idx;
  }

  /* Add a shading, return resource name index. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_shading (hb_ਸਦਿਸ਼_buf_t &&shading_ਅੰਕੜੇ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ obj_id = ਜੋੜੋ_object (std::move (shading_ਅੰਕੜੇ));
    ਵਾਪਸ ਜੋੜੋ_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_shading_by_id (ਬਿਨਾਨਿਸ਼ਾਨੀ obj_id)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = shading_ਗਿਣਤੀ++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    ਵਾਪਸ idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਜੋੜੋ_xobject_png_image (ਅਚਲ ਅੱਖਰ *idat_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ idat_len,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ colors, ਤਾਰਕਿਕ has_alpha,
				  ਅਚਲ uint8_t *plte = ਖਾਲੀ_ਸੂਚਕ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ plte_len = 0,
				  ਅਚਲ uint8_t *trns = ਖਾਲੀ_ਸੂਚਕ,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ trns_len = 0)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ idx = xobject_ਗਿਣਤੀ++;
    hb_ਸਦਿਸ਼_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (ਚੌੜਾਈ);
    obj.append_str (" /Height ");
    obj.append_unsigned (ਉਚਾਈ);
    obj.append_str ("\n/BitsPerComponent 8\n");

    ਜੇ (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      ਬਿਨਾਨਿਸ਼ਾਨੀ n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    ਨਹੀਂਤਾਂ
    {
      obj.append_str ("/ColorSpace ");
      ਬਿਨਾਨਿਸ਼ਾਨੀ color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ smask_id = 0;
    ਜੇ (plte && trns && trns_len)
    {
      hb_ਸਦਿਸ਼_buf_t smask_ਧਾਰਾ;
      ਜੇ (hb_pdf_build_ਸੂਚਕਾਂਕਿਤ_smask (&smask_ਧਾਰਾ, idat_ਅੰਕੜੇ, idat_len,
				      ਚੌੜਾਈ, ਉਚਾਈ, trns, trns_len))
      {
	hb_ਸਦਿਸ਼_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (ਚੌੜਾਈ);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (ਉਚਾਈ);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_ਧਾਰਾ.ਲੰਮਾਈ);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_ਧਾਰਾ.arrayZ, smask_ਧਾਰਾ.ਲੰਮਾਈ);
	smask_obj.append_str ("\nendstream");
	smask_id = ਜੋੜੋ_object (std::move (smask_obj));
      }
    }

    ਜੇ (smask_id)
    {
      obj.append_str ("/SMask ");
      obj.append_unsigned (smask_id);
      obj.append_str (" 0 R\n");
    }

    obj.append_str ("/Filter /FlateDecode\n");
    obj.append_str ("/DecodeParms << /Predictor 15 /Colors ");
    obj.append_unsigned (colors);
    obj.append_str (" /BitsPerComponent 8 /Columns ");
    obj.append_unsigned (ਚੌੜਾਈ);
    obj.append_str (" >>\n");
    obj.append_str ("/Length ");
    obj.append_unsigned (idat_len);
    obj.append_str (" >>\nstream\n");
    obj.append_len (idat_ਅੰਕੜੇ, idat_len);
    obj.append_str ("\nendstream");

    (ਖਾਲੀ) has_alpha;

    ਬਿਨਾਨਿਸ਼ਾਨੀ obj_id = ਜੋੜੋ_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    ਵਾਪਸ idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
ਸਥਿਰ hb_pdf_resources_t *
hb_pdf_get_resources (hb_ਸਦਿਸ਼_paint_t *paint)
{
  ਜੇ (!paint->defs.ਲੰਮਾਈ)
  {
    /* First call: allocate and store. */
    ਸਵੈਚਾਲਿਤ *res = (hb_pdf_resources_t *) hb_calloc (1, ਆਕਾਰ (hb_pdf_resources_t));
    ਜੇ (unlikely (!res)) ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ਨਵਾਂ (res) hb_pdf_resources_t ();
    ਜੇ (unlikely (!paint->defs.resize (ਆਕਾਰ (ਖਾਲੀ *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (paint->defs.arrayZ, &res, ਆਕਾਰ (ਖਾਲੀ *));
  }
  hb_pdf_resources_t *res;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (&res, paint->defs.arrayZ, ਆਕਾਰ (ਖਾਲੀ *));
  ਵਾਪਸ res;
}

ਖਾਲੀ
hb_ਸਦਿਸ਼_paint_pdf_free_resources (hb_ਸਦਿਸ਼_paint_t *paint)
{
  ਜੇ (paint->defs.ਲੰਮਾਈ >= ਆਕਾਰ (ਖਾਲੀ *))
  {
    hb_pdf_resources_t *res;
    ਸਮ੍ਰਿਤੀ_ਨਕਲ (&res, paint->defs.arrayZ, ਆਕਾਰ (ਖਾਲੀ *));
    ਜੇ (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_emit_glyph_ਰਸਤਾ (hb_ਸਦਿਸ਼_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_ਸਦਿਸ਼_buf_t *buf)
{
  paint->ਰਸਤਾ.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  ਜੇ (likely (paint->budget_ਬਚਿਆ >= 0))
  {
    hb_ਸਦਿਸ਼_ਰਸਤਾ_sink_t sink = {&paint->ਰਸਤਾ, paint->get_precision (),
				 paint->x_scale_ਗੁਣਕ, paint->y_scale_ਗੁਣਕ,
				 &paint->budget_ਬਚਿਆ};
    hb_font_draw_glyph (font, glyph,
			hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->ਰਸਤਾ.arrayZ, paint->ਰਸਤਾ.ਲੰਮਾਈ);
  paint->ਰਸਤਾ.clear ();
}


/* ---- paint callbacks ---- */

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਧੱਕੋ_transform (hb_paint_funcs_t *,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
			     ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
			     ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy,
			     ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  ਬਿਨਾਨਿਸ਼ਾਨੀ sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_ਅੰਕ (xx, sprec);
  body.append_c (' ');
  body.append_ਅੰਕ (yx, sprec);
  body.append_c (' ');
  body.append_ਅੰਕ (xy, sprec);
  body.append_c (' ');
  body.append_ਅੰਕ (yy, sprec);
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sx (dx));
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sy (dy));
  body.append_str (" cm\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਕੱਢੋ_transform (hb_paint_funcs_t *,
			    ਖਾਲੀ *paint_ਅੰਕੜੇ,
			    ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  paint->current_body ().append_str ("Q\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 ਖਾਲੀ *paint_ਅੰਕੜੇ,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਦਸ਼ਮਲਵ r = hb_color_get_red (c) / 255.f;
  ਦਸ਼ਮਲਵ g = hb_color_get_green (c) / 255.f;
  ਦਸ਼ਮਲਵ b = hb_color_get_blue (c) / 255.f;
  ਦਸ਼ਮਲਵ a = hb_color_get_alpha (c) / 255.f;

  ਜੇ (a < 1.f / 255.f)
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();

  ਤਾਰਕਿਕ scoped = ਝੂਠ;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  ਜੇ (a < 1.f - 1.f / 512.f)
  {
    ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
    ਜੇ (res)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = ਸੱਚ;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_ਅੰਕ (r, 4);
  body.append_c (' ');
  body.append_ਅੰਕ (g, 4);
  body.append_c (' ');
  body.append_ਅੰਕ (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_ਰਸਤਾ (paint, font, glyph, &body);
  body.append_str ("f\n");
  ਜੇ (scoped)
    body.append_str ("Q\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਧੱਕੋ_clip_glyph (hb_paint_funcs_t *,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_ਰਸਤਾ (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਧੱਕੋ_clip_rectangle (hb_paint_funcs_t *,
				  ਖਾਲੀ *paint_ਅੰਕੜੇ,
				  ਦਸ਼ਮਲਵ xmin, ਦਸ਼ਮਲਵ ymin,
				  ਦਸ਼ਮਲਵ xmax, ਦਸ਼ਮਲਵ ymax,
				  ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_ਅੰਕ (paint->sx (xmin));
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sy (ymin));
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

ਸਥਿਰ hb_draw_funcs_t *
hb_pdf_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ (hb_paint_funcs_t *,
				   ਖਾਲੀ *paint_ਅੰਕੜੇ,
				   ਖਾਲੀ **draw_ਅੰਕੜੇ,
				   ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
  {
    *draw_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_ਰਸਤਾ_sink = {&body, paint->get_precision (),
			   paint->x_scale_ਗੁਣਕ,
			   paint->y_scale_ਗੁਣਕ,
			   &paint->budget_ਬਚਿਆ};
  *draw_ਅੰਕੜੇ = &paint->clip_ਰਸਤਾ_sink;
  ਵਾਪਸ hb_ਸਦਿਸ਼_pdf_ਰਸਤਾ_draw_funcs_get ();
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ (hb_paint_funcs_t *,
				 ਖਾਲੀ *paint_ਅੰਕੜੇ,
				 ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  paint->current_body ().append_str ("W n\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਕੱਢੋ_clip (hb_paint_funcs_t *,
		       ਖਾਲੀ *paint_ਅੰਕੜੇ,
		       ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_solid_color (hb_ਸਦਿਸ਼_paint_t *paint, hb_color_t c)
{
  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();

  ਦਸ਼ਮਲਵ r = hb_color_get_red (c) / 255.f;
  ਦਸ਼ਮਲਵ g = hb_color_get_green (c) / 255.f;
  ਦਸ਼ਮਲਵ b = hb_color_get_blue (c) / 255.f;
  ਦਸ਼ਮਲਵ a = hb_color_get_alpha (c) / 255.f;

  ਜੇ (a < 1.f / 255.f)
    ਵਾਪਸ;

  /* Set alpha via ExtGState if needed. */
  ਜੇ (a < 1.f - 1.f / 512.f)
  {
    ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
    ਜੇ (res)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_ਅੰਕ (r, 4);
  body.append_c (' ');
  body.append_ਅੰਕ (g, 4);
  body.append_c (' ');
  body.append_ਅੰਕ (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_color (hb_paint_funcs_t *,
		    ਖਾਲੀ *paint_ਅੰਕੜੇ,
		    hb_bool_t,
		    hb_color_t color,
		    ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
ਸਥਿਰ ਤਾਰਕਿਕ
hb_pdf_build_ਸੂਚਕਾਂਕਿਤ_smask (hb_ਸਦਿਸ਼_buf_t *ਬਾਹਰਲਾ,
			    ਅਚਲ ਅੱਖਰ *idat_ਅੰਕੜੇ, ਬਿਨਾਨਿਸ਼ਾਨੀ idat_len,
			    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
			    ਅਚਲ uint8_t *trns, ਬਿਨਾਨਿਸ਼ਾਨੀ trns_len)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_ZLIB
  (ਖਾਲੀ) ਬਾਹਰਲਾ; (ਖਾਲੀ) idat_ਅੰਕੜੇ; (ਖਾਲੀ) idat_len;
  (ਖਾਲੀ) ਚੌੜਾਈ; (ਖਾਲੀ) ਉਚਾਈ; (ਖਾਲੀ) trns; (ਖਾਲੀ) trns_len;
  ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  ਬਿਨਾਨਿਸ਼ਾਨੀ raw_len, ਨਕਾਬ_len;
  ਜੇ (!ਚੌੜਾਈ || !ਉਚਾਈ ||
      ਚੌੜਾਈ == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (ਚੌੜਾਈ + 1, ਉਚਾਈ, &raw_len) ||
      hb_unsigned_mul_overflows (ਚੌੜਾਈ, ਉਚਾਈ, &ਨਕਾਬ_len))
    ਵਾਪਸ ਝੂਠ;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  ਜੇ (!raw) ਵਾਪਸ ਝੂਠ;
  HB_SCOPE_GUARD (hb_free (raw));

  z_ਧਾਰਾ ਧਾਰਾ = {};
  ਧਾਰਾ.ਅਗਲਾ_ਵਿੱਚ = (Bytef *) idat_ਅੰਕੜੇ;
  ਧਾਰਾ.avail_ਵਿੱਚ = idat_len;
  ਧਾਰਾ.ਅਗਲਾ_ਬਾਹਰਲਾ = (Bytef *) raw;
  ਧਾਰਾ.avail_ਬਾਹਰਲਾ = raw_len;

  ਜੇ (inflateInit (&ਧਾਰਾ) != Z_OK)
    ਵਾਪਸ ਝੂਠ;
  ਪੂਰਨ ਸਥਿਤੀ = inflate (&ਧਾਰਾ, Z_ਅੰਤ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਕੁੱਲ_ਬਾਹਰਲਾ = ਧਾਰਾ.ਕੁੱਲ_ਬਾਹਰਲਾ;
  inflate_ਸਿਰਾ (&ਧਾਰਾ);
  ਜੇ (ਸਥਿਤੀ != Z_ਧਾਰਾ_ਸਿਰਾ || ਕੁੱਲ_ਬਾਹਰਲਾ != raw_len)
    ਵਾਪਸ ਝੂਠ;

  /* Un-filter and map to alpha. */
  ਜੇ (!ਬਾਹਰਲਾ->resize (ਨਕਾਬ_len))
    ਵਾਪਸ ਝੂਠ;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (ਚੌੜਾਈ);
  ਜੇ (!unfiltered)
    ਵਾਪਸ ਝੂਠ;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (ਚੌੜਾਈ, 1);
  ਜੇ (!prev_unfiltered)
    ਵਾਪਸ ਝੂਠ;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ y = 0; y < ਉਚਾਈ; y++)
  {
    uint8_t *ਪੰਕਤੀ = raw + y * (ਚੌੜਾਈ + 1);
    uint8_t filter = ਪੰਕਤੀ[0];
    uint8_t *pixels = ਪੰਕਤੀ + 1;

    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ x = 0; x < ਚੌੜਾਈ; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      ਚੋਣ (filter)
      {
      ਹਾਲਤ 0: unfiltered[x] = val; ਰੋਕੋ; /* None */
      ਹਾਲਤ 1: unfiltered[x] = val + a; ਰੋਕੋ; /* Sub */
      ਹਾਲਤ 2: unfiltered[x] = val + b; ਰੋਕੋ; /*ਉੱਪਰ*/
      ਹਾਲਤ 3: unfiltered[x] = val + (uint8_t) (((ਬਿਨਾਨਿਸ਼ਾਨੀ) a + b) / 2); ਰੋਕੋ; /*ਔਸਤਨ*/
      ਹਾਲਤ 4: /* Paeth */
      {
	ਪੂਰਨ p = (ਪੂਰਨ) a + (ਪੂਰਨ) b - (ਪੂਰਨ) c;
	ਪੂਰਨ pa = ਨਿਰਪੇਖ (p - (ਪੂਰਨ) a);
	ਪੂਰਨ pb = ਨਿਰਪੇਖ (p - (ਪੂਰਨ) b);
	ਪੂਰਨ pc = ਨਿਰਪੇਖ (p - (ਪੂਰਨ) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	ਰੋਕੋ;
      }
      ਮੂਲ: unfiltered[x] = val; ਰੋਕੋ;
      }

      /* Map palette index to alpha. */
      uint8_t idx = unfiltered[x];
      ਬਾਹਰਲਾ->arrayZ[y * ਚੌੜਾਈ + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = prev_unfiltered;
    prev_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  ਵਾਪਸ ਸੱਚ;
#ਜੇ_ਅੰਤ
}

/* Read a big-endian uint32 from a byte pointer. */
ਸਥਿਰ ਸਤਰਅੰਦਰ uint32_t
hb_pdf_png_u32 (ਅਚਲ uint8_t *p)
{
  ਵਾਪਸ ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

ਸਥਿਰ hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    ਖਾਲੀ *paint_ਅੰਕੜੇ,
		    hb_blob_t *image,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਚੌੜਾਈ,
		    ਬਿਨਾਨਿਸ਼ਾਨੀ ਉਚਾਈ,
		    hb_ਟੈਗ_t format,
		    ਦਸ਼ਮਲਵ slant HB_ਅਣਵਰਤਿਆ,
		    hb_glyph_extents_t *extents,
		    ਖਾਲੀ *)
{
  ਜੇ (format != HB_ਟੈਗ ('p','n','g',' '))
    ਵਾਪਸ ਝੂਠ;
  ਜੇ (!extents || !ਚੌੜਾਈ || !ਉਚਾਈ)
    ਵਾਪਸ ਝੂਠ;

  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ ਝੂਠ;
  ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
  ਜੇ (unlikely (!res))
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ len = 0;
  ਅਚਲ uint8_t *ਅੰਕੜੇ = (ਅਚਲ uint8_t *) hb_blob_get_ਅੰਕੜੇ (image, &len);
  ਜੇ (!ਅੰਕੜੇ || len < 8)
    ਵਾਪਸ ਝੂਠ;

  /* Verify PNG signature. */
  ਸਥਿਰ ਅਚਲ uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  ਜੇ (hb_memcmp (ਅੰਕੜੇ, png_sig, 8) != 0)
    ਵਾਪਸ ਝੂਠ;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  ਬਿਨਾਨਿਸ਼ਾਨੀ png_w = 0, png_h = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ colors = 3; /* default RGB */
  uint8_t color_type = 2;
  ਤਾਰਕਿਕ has_alpha = ਝੂਠ;
  ਅਚਲ uint8_t *plte_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ plte_len = 0;
  ਅਚਲ uint8_t *trns_ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ trns_len = 0;
  hb_ਸਦਿਸ਼_buf_t idat;

  ਬਿਨਾਨਿਸ਼ਾਨੀ pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  ਜਦੋਂ (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (ਅੰਕੜੇ + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (ਅੰਕੜੇ + pos + 4);
    ਜੇ (chunk_len > len - pos - 12)
      ਰੋਕੋ;
    ਅਚਲ uint8_t *chunk_ਅੰਕੜੇ = ਅੰਕੜੇ + pos + 8;

    ਜੇ (chunk_type == 0x49484452u) /* IHDR */
    {
      ਜੇ (chunk_len < 13) ਵਾਪਸ ਝੂਠ;
      png_w = hb_pdf_png_u32 (chunk_ਅੰਕੜੇ);
      png_h = hb_pdf_png_u32 (chunk_ਅੰਕੜੇ + 4);
      uint8_t ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ_2 = chunk_ਅੰਕੜੇ[8];
      color_type = chunk_ਅੰਕੜੇ[9];
      ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ_ਡੂੰਘਾਈ_2 != 8) ਵਾਪਸ ਝੂਠ; /* only 8-bit supported */

      ਚੋਣ (color_type)
      {
      ਹਾਲਤ 0: colors = 1; has_alpha = ਝੂਠ; ਰੋਕੋ; /* Grayscale */
      ਹਾਲਤ 2: colors = 3; has_alpha = ਝੂਠ; ਰੋਕੋ; /* RGB */
      ਹਾਲਤ 3: colors = 1; has_alpha = ਝੂਠ; ਰੋਕੋ; /*ਸੂਚਕਾਂਕਿਤ*/
      ਹਾਲਤ 4: colors = 2; has_alpha = ਸੱਚ;  ਰੋਕੋ; /* Gray+Alpha */
      ਹਾਲਤ 6: colors = 4; has_alpha = ਸੱਚ;  ਰੋਕੋ; /* RGBA */
      ਮੂਲ: ਵਾਪਸ ਝੂਠ;
      }
    }
    ਨਹੀਂਤਾਂ ਜੇ (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_ਅੰਕੜੇ = chunk_ਅੰਕੜੇ;
      plte_len = chunk_len;
    }
    ਨਹੀਂਤਾਂ ਜੇ (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_ਅੰਕੜੇ = chunk_ਅੰਕੜੇ;
      trns_len = chunk_len;
    }
    ਨਹੀਂਤਾਂ ਜੇ (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ਅਚਲ ਅੱਖਰ *) chunk_ਅੰਕੜੇ, chunk_len);
    }
    ਨਹੀਂਤਾਂ ਜੇ (chunk_type == 0x49454E44u) /* IEND */
      ਰੋਕੋ;

    pos += 12 + chunk_len;
  }

  ਜੇ (!png_w || !png_h || !idat.ਲੰਮਾਈ)
    ਵਾਪਸ ਝੂਠ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ im_idx = res->ਜੋੜੋ_xobject_png_image (idat.arrayZ, idat.ਲੰਮਾਈ,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_ਅੰਕੜੇ, plte_len,
						 trns_ਅੰਕੜੇ, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  ਦਸ਼ਮਲਵ ix = (ਦਸ਼ਮਲਵ) extents->x_bearing;
  ਦਸ਼ਮਲਵ iy = (ਦਸ਼ਮਲਵ) extents->y_bearing + (ਦਸ਼ਮਲਵ) extents->ਉਚਾਈ;
  ਦਸ਼ਮਲਵ iw = (ਦਸ਼ਮਲਵ) extents->ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ ih = -(ਦਸ਼ਮਲਵ) extents->ਉਚਾਈ; /* negative because image Y goes up but height is negative in extents */

  body.append_ਅੰਕ (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_ਅੰਕ (paint->sy (ih));
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sx (ix));
  body.append_c (' ');
  body.append_ਅੰਕ (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  ਵਾਪਸ ਸੱਚ;
}

/* ---- Gradient helpers ---- */

ਸਥਿਰ ਤਾਰਕਿਕ
hb_pdf_gradient_needs_alpha (hb_array_t<ਅਚਲ hb_color_stop_t> stops)
{
  ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &s : stops)
    ਜੇ (hb_color_get_alpha (s.color) != 255)
      ਵਾਪਸ ਸੱਚ;
  ਵਾਪਸ ਝੂਠ;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_build_alpha_interpolation_function (hb_ਸਦਿਸ਼_buf_t *obj,
					   ਦਸ਼ਮਲਵ a0, ਦਸ਼ਮਲਵ a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_ਅੰਕ (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_ਅੰਕ (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_ਸਦਿਸ਼_paint_t *paint)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = paint->color_stops_scratch.ਲੰਮਾਈ;

  ਜੇ (ਗਿਣਤੀ_2 < 2)
  {
    ਦਸ਼ਮਲਵ a = ਗਿਣਤੀ_2 ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_ਸਦਿਸ਼_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    ਵਾਪਸ res->ਜੋੜੋ_object (std::move (obj));
  }

  ਜੇ (ਗਿਣਤੀ_2 == 2)
  {
    hb_ਸਦਿਸ਼_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    ਵਾਪਸ res->ਜੋੜੋ_object (std::move (obj));
  }

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> sub_func_ids;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਗਿਣਤੀ_2; i++)
  {
    hb_ਸਦਿਸ਼_buf_t sub;
    hb_pdf_build_alpha_interpolation_function (&sub,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i + 1].color) / 255.f);
    sub_func_ids.push (res->ਜੋੜੋ_object (std::move (sub)));
  }

  hb_ਸਦਿਸ਼_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");
  obj.append_str ("/Functions [");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sub_func_ids.ਲੰਮਾਈ; i++)
  {
    ਜੇ (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i + 1 < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (i > 1) obj.append_c (' ');
    obj.append_ਅੰਕ (paint->color_stops_scratch.arrayZ[i].ਵਿੱਥ, 4);
  }
  obj.append_str ("]\n/Encode [");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  ਵਾਪਸ res->ਜੋੜੋ_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_build_interpolation_function (hb_ਸਦਿਸ਼_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_ਅੰਕ (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_ਅੰਕ (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_ਅੰਕ (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_ਅੰਕ (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_ਅੰਕ (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_ਅੰਕ (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_ਸਦਿਸ਼_paint_t *paint)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਗਿਣਤੀ_2 = paint->color_stops_scratch.ਲੰਮਾਈ;

  ਜੇ (ਗਿਣਤੀ_2 < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = ਗਿਣਤੀ_2 ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_ਸਦਿਸ਼_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    ਵਾਪਸ res->ਜੋੜੋ_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (ਅਚਲ hb_color_stop_t &a, ਅਚਲ hb_color_stop_t &b)
    { ਵਾਪਸ (a.ਵਿੱਥ > b.ਵਿੱਥ) - (a.ਵਿੱਥ < b.ਵਿੱਥ); });

  ਜੇ (ਗਿਣਤੀ_2 == 2)
  {
    /* Two stops: single interpolation function. */
    hb_ਸਦਿਸ਼_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    ਵਾਪਸ res->ਜੋੜੋ_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> sub_func_ids;
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਗਿਣਤੀ_2; i++)
  {
    hb_ਸਦਿਸ਼_buf_t sub;
    hb_pdf_build_interpolation_function (&sub,
					  paint->color_stops_scratch.arrayZ[i].color,
					  paint->color_stops_scratch.arrayZ[i + 1].color);
    sub_func_ids.push (res->ਜੋੜੋ_object (std::move (sub)));
  }

  /* Stitching function (Type 3). */
  hb_ਸਦਿਸ਼_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");

  /* Functions array. */
  obj.append_str ("/Functions [");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < sub_func_ids.ਲੰਮਾਈ; i++)
  {
    ਜੇ (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 1; i + 1 < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (i > 1) obj.append_c (' ');
    obj.append_ਅੰਕ (paint->color_stops_scratch.arrayZ[i].ਵਿੱਥ, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + 1 < ਗਿਣਤੀ_2; i++)
  {
    ਜੇ (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  ਵਾਪਸ res->ਜੋੜੋ_object (std::move (obj));
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਰੇਖਿਕ_gradient (hb_paint_funcs_t *,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_color_ਸਤਰ_t *color_ਸਤਰ,
			      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0,
			      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1,
			      ਦਸ਼ਮਲਵ x2 HB_ਅਣਵਰਤਿਆ, ਦਸ਼ਮਲਵ y2 HB_ਅਣਵਰਤਿਆ,
			      ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
  ਜੇ (unlikely (!res))
    ਵਾਪਸ;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  ਜੇ (!paint->fetch_color_stops (color_ਸਤਰ))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops.arrayZ, stops.ਲੰਮਾਈ, &mn, &mx);
  ਦਸ਼ਮਲਵ gx0 = x0 + mn * (x1 - x0);
  ਦਸ਼ਮਲਵ gy0 = y0 + mn * (y1 - y0);
  ਦਸ਼ਮਲਵ gx1 = x0 + mx * (x1 - x0);
  ਦਸ਼ਮਲਵ gy1 = y0 + mx * (y1 - y0);

  ਬਿਨਾਨਿਸ਼ਾਨੀ func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);
  ਅਚਲ ਅੱਖਰ *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_ਸਦਿਸ਼_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_ਅੰਕ (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  ਬਿਨਾਨਿਸ਼ਾਨੀ sh_idx = res->ਜੋੜੋ_shading (std::move (sh));

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();

  ਤਾਰਕਿਕ needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  ਜੇ (needs_alpha)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_ਸਦਿਸ਼_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_ਅੰਕ (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_sh_id = res->ਜੋੜੋ_object (std::move (ash));

    ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_smask (alpha_sh_id,
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

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      ਖਾਲੀ *paint_ਅੰਕੜੇ,
			      hb_color_ਸਤਰ_t *color_ਸਤਰ,
			      ਦਸ਼ਮਲਵ x0, ਦਸ਼ਮਲਵ y0, ਦਸ਼ਮਲਵ r0,
			      ਦਸ਼ਮਲਵ x1, ਦਸ਼ਮਲਵ y1, ਦਸ਼ਮਲਵ r1,
			      ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
  ਜੇ (unlikely (!res))
    ਵਾਪਸ;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  ਜੇ (!paint->fetch_color_stops (color_ਸਤਰ))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ਦਸ਼ਮਲਵ mn, mx;
  hb_paint_normalize_color_ਸਤਰ (stops.arrayZ, stops.ਲੰਮਾਈ, &mn, &mx);
  ਦਸ਼ਮਲਵ gx0 = x0 + mn * (x1 - x0);
  ਦਸ਼ਮਲਵ gy0 = y0 + mn * (y1 - y0);
  ਦਸ਼ਮਲਵ gr0 = r0 + mn * (r1 - r0);
  ਦਸ਼ਮਲਵ gx1 = x0 + mx * (x1 - x0);
  ਦਸ਼ਮਲਵ gy1 = y0 + mx * (y1 - y0);
  ਦਸ਼ਮਲਵ gr1 = r0 + mx * (r1 - r0);

  ਬਿਨਾਨਿਸ਼ਾਨੀ func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);
  ਅਚਲ ਅੱਖਰ *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_ਸਦਿਸ਼_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_ਅੰਕ (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_ਅੰਕ (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  ਬਿਨਾਨਿਸ਼ਾਨੀ sh_idx = res->ਜੋੜੋ_shading (std::move (sh));

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();

  ਤਾਰਕਿਕ needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  ਜੇ (needs_alpha)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_ਸਦਿਸ਼_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_ਅੰਕ (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_ਅੰਕ (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_sh_id = res->ਜੋੜੋ_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    ਦਸ਼ਮਲਵ cx = (gr1 >= gr0) ? gx1 : gx0;
    ਦਸ਼ਮਲਵ cy = (gr1 >= gr0) ? gy1 : gy0;
    ਦਸ਼ਮਲਵ rr = hb_max (gr0, gr1);
    ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_smask (alpha_sh_id,
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
ਸਥਿਰ ਖਾਲੀ
hb_pdf_encode_u16 (hb_ਸਦਿਸ਼_buf_t *buf, uint16_t v)
{
  ਅੱਖਰ ਬਾਈਟਾਂ[2] = {(ਅੱਖਰ) (v >> 8), (ਅੱਖਰ) (v & 0xFF)};
  buf->append_len (ਬਾਈਟਾਂ, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_encode_coord (hb_ਸਦਿਸ਼_buf_t *buf,
		     ਦਸ਼ਮਲਵ val, ਦਸ਼ਮਲਵ lo, ਦਸ਼ਮਲਵ hi)
{
  ਦਸ਼ਮਲਵ t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_encode_color_rgb (hb_ਸਦਿਸ਼_buf_t *buf, hb_color_t c)
{
  ਅੱਖਰ rgb[3] = {(ਅੱਖਰ) hb_color_get_red (c),
		 (ਅੱਖਰ) hb_color_get_green (c),
		 (ਅੱਖਰ) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_encode_color_alpha (hb_ਸਦਿਸ਼_buf_t *buf, hb_color_t c)
{
  ਅੱਖਰ a = (ਅੱਖਰ) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_encode_ਬਿੰਦੂ (hb_ਸਦਿਸ਼_buf_t *buf,
		     ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y,
		     ਦਸ਼ਮਲਵ xlo, ਦਸ਼ਮਲਵ xhi,
		     ਦਸ਼ਮਲਵ ylo, ਦਸ਼ਮਲਵ yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
ਸਥਿਰ ਖਾਲੀ
hb_pdf_ਜੋੜੋ_sweep_patch (hb_ਸਦਿਸ਼_buf_t *mesh,
			hb_ਸਦਿਸ਼_buf_t *alpha_mesh,
			ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
			ਦਸ਼ਮਲਵ xlo, ਦਸ਼ਮਲਵ xhi, ਦਸ਼ਮਲਵ ylo, ਦਸ਼ਮਲਵ yhi,
			ਦਸ਼ਮਲਵ a0, hb_color_t c0_ਵਿੱਚ,
			ਦਸ਼ਮਲਵ a1, hb_color_t c1_ਵਿੱਚ)
{
  ਅਚਲ ਦਸ਼ਮਲਵ R = 32767.f;
  ਅਚਲ ਦਸ਼ਮਲਵ eps = 0.5f;
  ਅਚਲ ਦਸ਼ਮਲਵ MAX_SECTOR = (ਦਸ਼ਮਲਵ) M_PI / 2.f;

  ਪੂਰਨ ਅੰਕ_splits = (ਪੂਰਨ) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  ਜੇ (ਅੰਕ_splits < 1) ਅੰਕ_splits = 1;

  ਲਈ (ਪੂਰਨ s = 0; s < ਅੰਕ_splits; s++)
  {
    ਦਸ਼ਮਲਵ k0 = (ਦਸ਼ਮਲਵ) s / ਅੰਕ_splits;
    ਦਸ਼ਮਲਵ k1 = (ਦਸ਼ਮਲਵ) (s + 1) / ਅੰਕ_splits;
    ਦਸ਼ਮਲਵ sa0 = a0 + k0 * (a1 - a0);
    ਦਸ਼ਮਲਵ sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_ਵਿੱਚ, c1_ਵਿੱਚ, k0);
    hb_color_t sc1 = hb_color_lerp (c0_ਵਿੱਚ, c1_ਵਿੱਚ, k1);

    ਦਸ਼ਮਲਵ da = sa1 - sa0;
    ਦਸ਼ਮਲਵ kappa = (4.f / 3.f) * tanf (da / 4.f);

    ਦਸ਼ਮਲਵ cos0 = cosf (sa0), sin0 = sinf (sa0);
    ਦਸ਼ਮਲਵ cos1 = cosf (sa1), sin1 = sinf (sa1);

    ਦਸ਼ਮਲਵ p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    ਦਸ਼ਮਲਵ p3x = cx + R * cos0,   p3y = cy + R * sin0;
    ਦਸ਼ਮਲਵ p6x = cx + R * cos1,   p6y = cy + R * sin1;
    ਦਸ਼ਮਲਵ p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    ਦਸ਼ਮਲਵ e1_1x = p0x + (p3x - p0x) / 3.f;
    ਦਸ਼ਮਲਵ e1_1y = p0y + (p3y - p0y) / 3.f;
    ਦਸ਼ਮਲਵ e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    ਦਸ਼ਮਲਵ e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    ਦਸ਼ਮਲਵ e2_1x = p3x + kappa * R * (-sin0);
    ਦਸ਼ਮਲਵ e2_1y = p3y + kappa * R * ( cos0);
    ਦਸ਼ਮਲਵ e2_2x = p6x - kappa * R * (-sin1);
    ਦਸ਼ਮਲਵ e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    ਦਸ਼ਮਲਵ e3_1x = p6x + (p9x - p6x) / 3.f;
    ਦਸ਼ਮਲਵ e3_1y = p6y + (p9y - p6y) / 3.f;
    ਦਸ਼ਮਲਵ e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    ਦਸ਼ਮਲਵ e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    ਦਸ਼ਮਲਵ e4_1x = p9x + kappa * eps * (-sin1);
    ਦਸ਼ਮਲਵ e4_1y = p9y + kappa * eps * ( cos1);
    ਦਸ਼ਮਲਵ e4_2x = p0x - kappa * eps * (-sin0);
    ਦਸ਼ਮਲਵ e4_2y = p0y - kappa * eps * ( cos0);

    mesh->append_c ('\0'); /* flag = 0, new patch */

    hb_pdf_encode_ਬਿੰਦੂ (mesh, p0x, p0y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, p3x, p3y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, p6x, p6y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, p9x, p9y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_ਬਿੰਦੂ (mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

    hb_pdf_encode_color_rgb (mesh, sc0); /* inner start */
    hb_pdf_encode_color_rgb (mesh, sc0); /* outer start */
    hb_pdf_encode_color_rgb (mesh, sc1); /* outer end */
    hb_pdf_encode_color_rgb (mesh, sc1); /* inner end */

    ਜੇ (alpha_mesh)
    {
      alpha_mesh->append_c ('\0');

      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, p0x, p0y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, p3x, p3y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, p6x, p6y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, p9x, p9y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_ਬਿੰਦੂ (alpha_mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
    }
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ਬਣਤਰ hb_pdf_sweep_ctx_t {
  hb_ਸਦਿਸ਼_buf_t *mesh;
  hb_ਸਦਿਸ਼_buf_t *alpha_mesh;
  ਦਸ਼ਮਲਵ cx, cy, xlo, xhi, ylo, yhi;
};

ਸਥਿਰ ਖਾਲੀ
hb_pdf_sweep_emit_patch (ਦਸ਼ਮਲਵ a0, hb_color_t c0,
			 ਦਸ਼ਮਲਵ a1, hb_color_t c1,
			 ਖਾਲੀ *user_ਅੰਕੜੇ)
{
  ਸਵੈਚਾਲਿਤ *ctx = (hb_pdf_sweep_ctx_t *) user_ਅੰਕੜੇ;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  ਜੇ (unlikely (ctx->mesh->ਵਿੱਚ_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->ਵਿੱਚ_error ())))
    ਵਾਪਸ;
  hb_pdf_ਜੋੜੋ_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     hb_color_ਸਤਰ_t *color_ਸਤਰ,
			     ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy,
			     ਦਸ਼ਮਲਵ ਸ਼ੁਰੂ_angle,
			     ਦਸ਼ਮਲਵ ਸਿਰਾ_angle,
			     ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
  ਜੇ (unlikely (!res))
    ਵਾਪਸ;

  /* Get and sort color stops. */
  ਜੇ (!paint->fetch_color_stops (color_ਸਤਰ))
    ਵਾਪਸ;
  hb_ਸਦਿਸ਼_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (ਅਚਲ hb_color_stop_t &a, ਅਚਲ hb_color_stop_t &b)
    { ਵਾਪਸ (a.ਵਿੱਥ > b.ਵਿੱਥ) - (a.ਵਿੱਥ < b.ਵਿੱਥ); });

  hb_paint_extend_t extend = hb_color_ਸਤਰ_get_extend (color_ਸਤਰ);

  ਅਚਲ ਦਸ਼ਮਲਵ R = 32767.f;
  ਦਸ਼ਮਲਵ scx = paint->sx (cx), scy = paint->sy (cy);
  ਦਸ਼ਮਲਵ xlo = scx - R - 1, xhi = scx + R + 1;
  ਦਸ਼ਮਲਵ ylo = scy - R - 1, yhi = scy + R + 1;

  ਤਾਰਕਿਕ needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_ਸਦਿਸ਼_buf_t mesh;
  hb_ਸਦਿਸ਼_buf_t alpha_mesh;
  mesh.alloc (256);
  ਜੇ (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : ਖਾਲੀ_ਸੂਚਕ,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.ਲੰਮਾਈ, extend,
				 ਸ਼ੁਰੂ_angle, ਸਿਰਾ_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  ਜੇ (!mesh.ਲੰਮਾਈ)
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ hb_pdf_build_mesh_shading = [&] (hb_ਸਦਿਸ਼_buf_t &m,
					 ਅਚਲ ਅੱਖਰ *cs,
					 ਅਚਲ ਅੱਖਰ *decode_suffix) -> ਬਿਨਾਨਿਸ਼ਾਨੀ
  {
    hb_ਸਦਿਸ਼_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_ਅੰਕ (xlo, 2);
    sh.append_c (' ');
    sh.append_ਅੰਕ (xhi, 2);
    sh.append_c (' ');
    sh.append_ਅੰਕ (ylo, 2);
    sh.append_c (' ');
    sh.append_ਅੰਕ (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.ਲੰਮਾਈ);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.ਲੰਮਾਈ);
    sh.append_str ("\nendstream");
    ਵਾਪਸ res->ਜੋੜੋ_object (std::move (sh));
  };

  ਬਿਨਾਨਿਸ਼ਾਨੀ sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  ਬਿਨਾਨਿਸ਼ਾਨੀ sh_idx = res->ਜੋੜੋ_shading_by_id (sh_obj_id);

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();

  ਜੇ (needs_alpha && alpha_mesh.ਲੰਮਾਈ)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_smask (alpha_sh_id,
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

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
hb_pdf_blend_ਬਹੁਲਕ_name (hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ)
{
  ਚੋਣ (ਬਹੁਲਕ)
  {
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_MULTIPLY:       ਵਾਪਸ "Multiply";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SCREEN:         ਵਾਪਸ "Screen";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_OVERLAY:        ਵਾਪਸ "Overlay";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DARKEN:         ਵਾਪਸ "Darken";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_LIGHTEN:        ਵਾਪਸ "Lighten";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_DODGE:    ਵਾਪਸ "ColorDodge";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_COLOR_BURN:     ਵਾਪਸ "ColorBurn";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HARD_LIGHT:     ਵਾਪਸ "HardLight";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SOFT_LIGHT:     ਵਾਪਸ "SoftLight";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DIFFERENCE:     ਵਾਪਸ "Difference";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_EXCLUSION:      ਵਾਪਸ "Exclusion";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_HUE:        ਵਾਪਸ "Hue";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_SATURATION: ਵਾਪਸ "Saturation";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_COLOR:      ਵਾਪਸ "Color";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_HSL_LUMINOSITY: ਵਾਪਸ "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_PLUS:           ਵਾਪਸ "Screen";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_XOR:            ਵਾਪਸ "Difference";
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_CLEAR:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_OVER:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_OVER:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਵਿੱਚ:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਵਿੱਚ:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ਬਾਹਰਲਾ:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ਬਾਹਰਲਾ:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_SRC_ATOP:
  ਹਾਲਤ HB_PAINT_ਸੰਯੁਕਤ_ਬਹੁਲਕ_DEST_ATOP:
  ਮੂਲ:                                     ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ; /*ਸਾਧਾਰਨ*/
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਧੱਕੋ_ਸਮੂਹ (hb_paint_funcs_t *,
			 ਖਾਲੀ *paint_ਅੰਕੜੇ,
			 ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  paint->current_body ().append_str ("q\n");
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਧੱਕੋ_ਸਮੂਹ_for (hb_paint_funcs_t *,
			     ਖਾਲੀ *paint_ਅੰਕੜੇ,
			     hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ,
			     ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;

  ਸਵੈਚਾਲਿਤ &body = paint->current_body ();
  body.append_str ("q\n");

  ਅਚਲ ਅੱਖਰ *bm = hb_pdf_blend_ਬਹੁਲਕ_name (ਬਹੁਲਕ);
  ਜੇ (bm)
  {
    ਸਵੈਚਾਲਿਤ *res = hb_pdf_get_resources (paint);
    ਜੇ (likely (res))
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

ਸਥਿਰ ਖਾਲੀ
hb_pdf_paint_ਕੱਢੋ_ਸਮੂਹ (hb_paint_funcs_t *,
			ਖਾਲੀ *paint_ਅੰਕੜੇ,
			hb_paint_ਸੰਯੁਕਤ_ਬਹੁਲਕ_t ਬਹੁਲਕ HB_ਅਣਵਰਤਿਆ,
			ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *paint = (hb_ਸਦਿਸ਼_paint_t *) paint_ਅੰਕੜੇ;
  ਜੇ (unlikely (!paint->ensure_initialized ()))
    ਵਾਪਸ;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_pdf_paint_funcs ();

ਸਥਿਰ ਬਣਤਰ hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_transform_func (funcs, (hb_paint_ਧੱਕੋ_transform_func_t) hb_pdf_paint_ਧੱਕੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_transform_func (funcs, (hb_paint_ਕੱਢੋ_transform_func_t) hb_pdf_paint_ਕੱਢੋ_transform, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_glyph_func (funcs, (hb_paint_ਧੱਕੋ_clip_glyph_func_t) hb_pdf_paint_ਧੱਕੋ_clip_glyph, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_rectangle_func (funcs, (hb_paint_ਧੱਕੋ_clip_rectangle_func_t) hb_pdf_paint_ਧੱਕੋ_clip_rectangle, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func (funcs, (hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ_func_t) hb_pdf_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸ਼ੁਰੂ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func (funcs, (hb_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ_func_t) hb_pdf_paint_ਧੱਕੋ_clip_ਰਸਤਾ_ਸਿਰਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_clip_func (funcs, (hb_paint_ਕੱਢੋ_clip_func_t) hb_pdf_paint_ਕੱਢੋ_clip, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਰੇਖਿਕ_gradient_func (funcs, (hb_paint_ਰੇਖਿਕ_gradient_func_t) hb_pdf_paint_ਰੇਖਿਕ_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_func (funcs, (hb_paint_ਧੱਕੋ_ਸਮੂਹ_func_t) hb_pdf_paint_ਧੱਕੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਧੱਕੋ_ਸਮੂਹ_for_func (funcs, (hb_paint_ਧੱਕੋ_ਸਮੂਹ_for_func_t) hb_pdf_paint_ਧੱਕੋ_ਸਮੂਹ_for, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਕੱਢੋ_ਸਮੂਹ_func (funcs, (hb_paint_ਕੱਢੋ_ਸਮੂਹ_func_t) hb_pdf_paint_ਕੱਢੋ_ਸਮੂਹ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_ਸਦਿਸ਼_paint_ਸੈੱਟ_budget_callback, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_ਸਦਿਸ਼_paint_get_budget_callback, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_ਸਦਿਸ਼_paint_get_budget_ਬਚਿਆ_callback, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    ਵਾਪਸ funcs;
  }
} static_pdf_paint_funcs;

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_ਸਦਿਸ਼_paint_pdf_funcs_get ()
{
  ਵਾਪਸ static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_ਸਦਿਸ਼_paint_render_pdf (hb_ਸਦਿਸ਼_paint_t *paint)
{
  ਜੇ (!paint->has_extents)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!paint->ਸਮੂਹ_ਢੇਰ.ਲੰਮਾਈ ||
      !paint->ਸਮੂਹ_ਢੇਰ.arrayZ[0].ਲੰਮਾਈ)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_ਸਦਿਸ਼_buf_t &content = paint->ਸਮੂਹ_ਢੇਰ.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  ਦਸ਼ਮਲਵ ex = paint->extents.x;
  ਦਸ਼ਮਲਵ ey = paint->extents.y;
  ਦਸ਼ਮਲਵ ew = paint->extents.ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ eh = paint->extents.ਉਚਾਈ;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_extra = res ? res->objects.ਲੰਮਾਈ : 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਕੁੱਲ_objects = 4 + ਅੰਕ_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_ਸਦਿਸ਼_buf_t ਬਾਹਰਲਾ;
  hb_buf_recover_recycled (paint->recycled_blob, &ਬਾਹਰਲਾ);
  ਬਾਹਰਲਾ.alloc (content.ਲੰਮਾਈ + ਅੰਕ_extra * 128 + 1024);

  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> offsets;
  ਜੇ (unlikely (!offsets.resize (ਕੁੱਲ_objects)))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  ਬਾਹਰਲਾ.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  ਬਾਹਰਲਾ.append_ਅੰਕ (ex);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (ey);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (ex + ew);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (ey + eh);
  ਬਾਹਰਲਾ.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  ਤਾਰਕਿਕ has_resources = res &&
    (res->extgstate_dict.ਲੰਮਾਈ || res->shading_dict.ਲੰਮਾਈ || res->xobject_dict.ਲੰਮਾਈ);
  ਜੇ (has_resources)
  {
    ਬਾਹਰਲਾ.append_str ("\n/Resources <<");
    ਜੇ (res->extgstate_dict.ਲੰਮਾਈ)
    {
      ਬਾਹਰਲਾ.append_str (" /ExtGState << ");
      ਬਾਹਰਲਾ.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.ਲੰਮਾਈ);
      ਬਾਹਰਲਾ.append_str (">>");
    }
    ਜੇ (res->shading_dict.ਲੰਮਾਈ)
    {
      ਬਾਹਰਲਾ.append_str (" /Shading << ");
      ਬਾਹਰਲਾ.append_len (res->shading_dict.arrayZ, res->shading_dict.ਲੰਮਾਈ);
      ਬਾਹਰਲਾ.append_str (">>");
    }
    ਜੇ (res->xobject_dict.ਲੰਮਾਈ)
    {
      ਬਾਹਰਲਾ.append_str (" /XObject << ");
      ਬਾਹਰਲਾ.append_len (res->xobject_dict.arrayZ, res->xobject_dict.ਲੰਮਾਈ);
      ਬਾਹਰਲਾ.append_str (">>");
    }
    ਬਾਹਰਲਾ.append_str (" >>");
  }

  ਬਾਹਰਲਾ.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_ਸਦਿਸ਼_buf_t bg_ਅਗੇਤਰ;
  ਜੇ (hb_color_get_alpha (paint->background))
  {
    ਦਸ਼ਮਲਵ r = hb_color_get_red (paint->background) / 255.f;
    ਦਸ਼ਮਲਵ g = hb_color_get_green (paint->background) / 255.f;
    ਦਸ਼ਮਲਵ b = hb_color_get_blue (paint->background) / 255.f;
    ਦਸ਼ਮਲਵ a = hb_color_get_alpha (paint->background) / 255.f;
    ਜੇ (a < 1.f - 1.f / 512.f)
    {
      ਜੇ (res)
      {
	ਬਿਨਾਨਿਸ਼ਾਨੀ gs_idx = res->ਜੋੜੋ_extgstate_alpha (a);
	bg_ਅਗੇਤਰ.append_str ("/GS");
	bg_ਅਗੇਤਰ.append_unsigned (gs_idx);
	bg_ਅਗੇਤਰ.append_str (" gs\n");
      }
    }
    bg_ਅਗੇਤਰ.append_ਅੰਕ (r, 4);
    bg_ਅਗੇਤਰ.append_c (' ');
    bg_ਅਗੇਤਰ.append_ਅੰਕ (g, 4);
    bg_ਅਗੇਤਰ.append_c (' ');
    bg_ਅਗੇਤਰ.append_ਅੰਕ (b, 4);
    bg_ਅਗੇਤਰ.append_str (" rg\n");
    bg_ਅਗੇਤਰ.append_ਅੰਕ (ex);
    bg_ਅਗੇਤਰ.append_c (' ');
    bg_ਅਗੇਤਰ.append_ਅੰਕ (ey);
    bg_ਅਗੇਤਰ.append_c (' ');
    bg_ਅਗੇਤਰ.append_ਅੰਕ (ew);
    bg_ਅਗੇਤਰ.append_c (' ');
    bg_ਅਗੇਤਰ.append_ਅੰਕ (eh);
    bg_ਅਗੇਤਰ.append_str (" re f\n");
  }
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਧਾਰਾ_len = bg_ਅਗੇਤਰ.ਲੰਮਾਈ + content.ਲੰਮਾਈ;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("4 0 obj\n<< /Length ");
  ਬਾਹਰਲਾ.append_unsigned (ਧਾਰਾ_len);
  ਬਾਹਰਲਾ.append_str (" >>\nstream\n");
  ਬਾਹਰਲਾ.append_len (bg_ਅਗੇਤਰ.arrayZ, bg_ਅਗੇਤਰ.ਲੰਮਾਈ);
  ਬਾਹਰਲਾ.append_len (content.arrayZ, content.ਲੰਮਾਈ);
  ਬਾਹਰਲਾ.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_extra; i++)
  {
    offsets.arrayZ[4 + i] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
    ਬਾਹਰਲਾ.append_unsigned (5 + i);
    ਬਾਹਰਲਾ.append_str (" 0 obj\n");
    ਸਵੈਚਾਲਿਤ &obj = res->objects.arrayZ[i];
    ਬਾਹਰਲਾ.append_len (obj.ਅੰਕੜੇ.arrayZ, obj.ਅੰਕੜੇ.ਲੰਮਾਈ);
    ਬਾਹਰਲਾ.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  ਬਿਨਾਨਿਸ਼ਾਨੀ xref_ਵਿੱਥ = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("xref\n0 ");
  ਬਾਹਰਲਾ.append_unsigned (ਕੁੱਲ_objects + 1);
  ਬਾਹਰਲਾ.append_str ("\n0000000000 65535 f \n");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਕੁੱਲ_objects; i++)
  {
    ਅੱਖਰ tmp[21];
    snprintf (tmp, ਆਕਾਰ (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    ਬਾਹਰਲਾ.append_len (tmp, 20);
  }

  /* Trailer */
  ਬਾਹਰਲਾ.append_str ("trailer\n<< /Size ");
  ਬਾਹਰਲਾ.append_unsigned (ਕੁੱਲ_objects + 1);
  ਬਾਹਰਲਾ.append_str (" /Root 1 0 R >>\nstartxref\n");
  ਬਾਹਰਲਾ.append_unsigned (xref_ਵਿੱਥ);
  ਬਾਹਰਲਾ.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &ਬਾਹਰਲਾ);

  ਵਾਪਸ blob;
}

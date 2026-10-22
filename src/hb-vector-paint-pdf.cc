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

#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-vector-paint.hh"
#അന്തര്ഭാവ "hb-vector-draw.hh"
#അന്തര്ഭാവ "hb-paint.hh"

#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdio.h>

#ifdef HAVE_ZLIB
#അന്തര്ഭാവ <zlib.h>
#endif

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

ഘടന hb_pdf_obj_t
{
  hb_vector_buf_t data;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
സ്ഥിതം തർക്കം
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    സ്ഥിരം അക്ഷരം *idat_data, unsigned idat_len,
			    unsigned width, unsigned height,
			    സ്ഥിരം uint8_t *trns, unsigned trns_len);

ഘടന hb_pdf_resources_t
{
  hb_vector_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_vector_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_vector_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_vector_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  unsigned extgstate_എണ്ണം = 0;
  unsigned shading_എണ്ണം = 0;
  unsigned xobject_എണ്ണം = 0;

  unsigned add_object (hb_vector_buf_t &&obj_data)
  {
    unsigned id = 5 + objects.length; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.data = std::move (obj_data);
    objects.push (std::move (obj));
    തിരികെ id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  unsigned add_extgstate_alpha (ദശാംശം alpha)
  {
    unsigned idx = extgstate_എണ്ണം++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_അക്കം (alpha, 4);
    obj.append_str (" >>");
    unsigned obj_id = add_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    തിരികെ idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  unsigned add_extgstate_blend (സ്ഥിരം അക്ഷരം *bm)
  {
    unsigned idx = extgstate_എണ്ണം++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    unsigned obj_id = add_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    തിരികെ idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  unsigned add_extgstate_smask (unsigned alpha_shading_id,
				ദശാംശം bbox_x, ദശാംശം bbox_y,
				ദശാംശം bbox_w, ദശാംശം bbox_h,
				unsigned precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_vector_buf_t form_stream;
    form_stream.append_str ("/SHa sh\n");

    hb_vector_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_അക്കം (bbox_x, precision);
    form.append_c (' ');
    form.append_അക്കം (bbox_y, precision);
    form.append_c (' ');
    form.append_അക്കം (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_അക്കം (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_stream.length);
    form.append_str (" >>\nstream\n");
    form.append_len (form_stream.arrayZ, form_stream.length);
    form.append_str ("endstream");
    unsigned form_id = add_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    unsigned idx = extgstate_എണ്ണം++;
    hb_vector_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    unsigned gs_id = add_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    തിരികെ idx;
  }

  /* Add a shading, return resource name index. */
  unsigned add_shading (hb_vector_buf_t &&shading_data)
  {
    unsigned obj_id = add_object (std::move (shading_data));
    തിരികെ add_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  unsigned add_shading_by_id (unsigned obj_id)
  {
    unsigned idx = shading_എണ്ണം++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    തിരികെ idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  unsigned add_xobject_png_image (സ്ഥിരം അക്ഷരം *idat_data, unsigned idat_len,
				  unsigned width, unsigned height,
				  unsigned colors, തർക്കം has_alpha,
				  സ്ഥിരം uint8_t *plte = nullptr,
				  unsigned plte_len = 0,
				  സ്ഥിരം uint8_t *trns = nullptr,
				  unsigned trns_len = 0)
  {
    unsigned idx = xobject_എണ്ണം++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (width);
    obj.append_str (" /Height ");
    obj.append_unsigned (height);
    obj.append_str ("\n/BitsPerComponent 8\n");

    എങ്കിൽ (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      unsigned n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      വേണ്ടി (unsigned i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    അല്ലെങ്കിൽ
    {
      obj.append_str ("/ColorSpace ");
      unsigned color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    unsigned smask_id = 0;
    എങ്കിൽ (plte && trns && trns_len)
    {
      hb_vector_buf_t smask_stream;
      എങ്കിൽ (hb_pdf_build_indexed_smask (&smask_stream, idat_data, idat_len,
				      width, height, trns, trns_len))
      {
	hb_vector_buf_t smask_obj;
	smask_obj.append_str ("<< /Type /XObject /Subtype /Image\n");
	smask_obj.append_str ("/Width ");
	smask_obj.append_unsigned (width);
	smask_obj.append_str (" /Height ");
	smask_obj.append_unsigned (height);
	smask_obj.append_str ("\n/ColorSpace /DeviceGray /BitsPerComponent 8\n");
	smask_obj.append_str ("/Length ");
	smask_obj.append_unsigned (smask_stream.length);
	smask_obj.append_str (" >>\nstream\n");
	smask_obj.append_len (smask_stream.arrayZ, smask_stream.length);
	smask_obj.append_str ("\nendstream");
	smask_id = add_object (std::move (smask_obj));
      }
    }

    എങ്കിൽ (smask_id)
    {
      obj.append_str ("/SMask ");
      obj.append_unsigned (smask_id);
      obj.append_str (" 0 R\n");
    }

    obj.append_str ("/Filter /FlateDecode\n");
    obj.append_str ("/DecodeParms << /Predictor 15 /Colors ");
    obj.append_unsigned (colors);
    obj.append_str (" /BitsPerComponent 8 /Columns ");
    obj.append_unsigned (width);
    obj.append_str (" >>\n");
    obj.append_str ("/Length ");
    obj.append_unsigned (idat_len);
    obj.append_str (" >>\nstream\n");
    obj.append_len (idat_data, idat_len);
    obj.append_str ("\nendstream");

    (ശൂന്യം) has_alpha;

    unsigned obj_id = add_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    തിരികെ idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
സ്ഥിതം hb_pdf_resources_t *
hb_pdf_get_resources (hb_vector_paint_t *paint)
{
  എങ്കിൽ (!paint->defs.length)
  {
    /* First call: allocate and store. */
    auto *res = (hb_pdf_resources_t *) hb_calloc (1, വലുപ്പം (hb_pdf_resources_t));
    എങ്കിൽ (unlikely (!res)) തിരികെ nullptr;
    പുതിയ (res) hb_pdf_resources_t ();
    എങ്കിൽ (unlikely (!paint->defs.resize (വലുപ്പം (ശൂന്യം *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      തിരികെ nullptr;
    }
    memcpy (paint->defs.arrayZ, &res, വലുപ്പം (ശൂന്യം *));
  }
  hb_pdf_resources_t *res;
  memcpy (&res, paint->defs.arrayZ, വലുപ്പം (ശൂന്യം *));
  തിരികെ res;
}

ശൂന്യം
hb_vector_paint_pdf_free_resources (hb_vector_paint_t *paint)
{
  എങ്കിൽ (paint->defs.length >= വലുപ്പം (ശൂന്യം *))
  {
    hb_pdf_resources_t *res;
    memcpy (&res, paint->defs.arrayZ, വലുപ്പം (ശൂന്യം *));
    എങ്കിൽ (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
സ്ഥിതം ശൂന്യം
hb_pdf_emit_glyph_path (hb_vector_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_vector_buf_t *buf)
{
  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  എങ്കിൽ (likely (paint->budget_remaining >= 0))
  {
    hb_vector_path_sink_t sink = {&paint->path, paint->get_precision (),
				 paint->x_scale_factor, paint->y_scale_factor,
				 &paint->budget_remaining};
    hb_font_draw_glyph (font, glyph,
			hb_vector_pdf_path_draw_funcs_get (),
			&sink);
  }
  buf->append_len (paint->path.arrayZ, paint->path.length);
  paint->path.clear ();
}


/* ---- paint callbacks ---- */

സ്ഥിതം ശൂന്യം
hb_pdf_paint_തള്ളുക_transform (hb_paint_funcs_t *,
			     ശൂന്യം *paint_data,
			     ദശാംശം xx, ദശാംശം yx,
			     ദശാംശം xy, ദശാംശം yy,
			     ദശാംശം dx, ദശാംശം dy,
			     ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  auto &body = paint->current_body ();
  unsigned sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_അക്കം (xx, sprec);
  body.append_c (' ');
  body.append_അക്കം (yx, sprec);
  body.append_c (' ');
  body.append_അക്കം (xy, sprec);
  body.append_c (' ');
  body.append_അക്കം (yy, sprec);
  body.append_c (' ');
  body.append_അക്കം (paint->sx (dx));
  body.append_c (' ');
  body.append_അക്കം (paint->sy (dy));
  body.append_str (" cm\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_എടുക്കുക_transform (hb_paint_funcs_t *,
			    ശൂന്യം *paint_data,
			    ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  paint->current_body ().append_str ("Q\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 ശൂന്യം *paint_data,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  ദശാംശം r = hb_color_get_red (c) / 255.f;
  ദശാംശം g = hb_color_get_green (c) / 255.f;
  ദശാംശം b = hb_color_get_blue (c) / 255.f;
  ദശാംശം a = hb_color_get_alpha (c) / 255.f;

  എങ്കിൽ (a < 1.f / 255.f)
    തിരികെ;

  auto &body = paint->current_body ();

  തർക്കം scoped = അസത്യം;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  എങ്കിൽ (a < 1.f - 1.f / 512.f)
  {
    auto *res = hb_pdf_get_resources (paint);
    എങ്കിൽ (res)
    {
      unsigned gs_idx = res->add_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = സത്യം;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_അക്കം (r, 4);
  body.append_c (' ');
  body.append_അക്കം (g, 4);
  body.append_c (' ');
  body.append_അക്കം (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("f\n");
  എങ്കിൽ (scoped)
    body.append_str ("Q\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_തള്ളുക_clip_glyph (hb_paint_funcs_t *,
			      ശൂന്യം *paint_data,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  auto &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_തള്ളുക_clip_rectangle (hb_paint_funcs_t *,
				  ശൂന്യം *paint_data,
				  ദശാംശം xmin, ദശാംശം ymin,
				  ദശാംശം xmax, ദശാംശം ymax,
				  ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  auto &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_അക്കം (paint->sx (xmin));
  body.append_c (' ');
  body.append_അക്കം (paint->sy (ymin));
  body.append_c (' ');
  body.append_അക്കം (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_അക്കം (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

സ്ഥിതം hb_draw_funcs_t *
hb_pdf_paint_തള്ളുക_clip_path_start (hb_paint_funcs_t *,
				   ശൂന്യം *paint_data,
				   ശൂന്യം **draw_data,
				   ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    തിരികെ nullptr;
  }

  auto &body = paint->current_body ();
  body.append_str ("q\n");
  /* Stream path operators straight into the body; end() seals
   * the path with "W n" to turn it into the clip region.
   * Coordinates arrive in font-scale; the sink divides by
   * scale_factor so they land in output space. */
  paint->clip_path_sink = {&body, paint->get_precision (),
			   paint->x_scale_factor,
			   paint->y_scale_factor,
			   &paint->budget_remaining};
  *draw_data = &paint->clip_path_sink;
  തിരികെ hb_vector_pdf_path_draw_funcs_get ();
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_തള്ളുക_clip_path_end (hb_paint_funcs_t *,
				 ശൂന്യം *paint_data,
				 ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  paint->current_body ().append_str ("W n\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_എടുക്കുക_clip (hb_paint_funcs_t *,
		       ശൂന്യം *paint_data,
		       ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
സ്ഥിതം ശൂന്യം
hb_pdf_paint_solid_color (hb_vector_paint_t *paint, hb_color_t c)
{
  auto &body = paint->current_body ();

  ദശാംശം r = hb_color_get_red (c) / 255.f;
  ദശാംശം g = hb_color_get_green (c) / 255.f;
  ദശാംശം b = hb_color_get_blue (c) / 255.f;
  ദശാംശം a = hb_color_get_alpha (c) / 255.f;

  എങ്കിൽ (a < 1.f / 255.f)
    തിരികെ;

  /* Set alpha via ExtGState if needed. */
  എങ്കിൽ (a < 1.f - 1.f / 512.f)
  {
    auto *res = hb_pdf_get_resources (paint);
    എങ്കിൽ (res)
    {
      unsigned gs_idx = res->add_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_അക്കം (r, 4);
  body.append_c (' ');
  body.append_അക്കം (g, 4);
  body.append_c (' ');
  body.append_അക്കം (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_color (hb_paint_funcs_t *,
		    ശൂന്യം *paint_data,
		    hb_bool_t,
		    hb_color_t color,
		    ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
സ്ഥിതം തർക്കം
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    സ്ഥിരം അക്ഷരം *idat_data, unsigned idat_len,
			    unsigned width, unsigned height,
			    സ്ഥിരം uint8_t *trns, unsigned trns_len)
{
#ifndef HAVE_ZLIB
  (ശൂന്യം) out; (ശൂന്യം) idat_data; (ശൂന്യം) idat_len;
  (ശൂന്യം) width; (ശൂന്യം) height; (ശൂന്യം) trns; (ശൂന്യം) trns_len;
  തിരികെ അസത്യം;
#അല്ലെങ്കിൽ
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  unsigned raw_len, mask_len;
  എങ്കിൽ (!width || !height ||
      width == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (width + 1, height, &raw_len) ||
      hb_unsigned_mul_overflows (width, height, &mask_len))
    തിരികെ അസത്യം;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  എങ്കിൽ (!raw) തിരികെ അസത്യം;
  HB_SCOPE_GUARD (hb_free (raw));

  z_stream stream = {};
  stream.next_in = (Bytef *) idat_data;
  stream.avail_in = idat_len;
  stream.next_out = (Bytef *) raw;
  stream.avail_out = raw_len;

  എങ്കിൽ (inflateInit (&stream) != Z_OK)
    തിരികെ അസത്യം;
  പൂർണ്ണസംഖ്യ status = inflate (&stream, Z_FINISH);
  unsigned long total_out = stream.total_out;
  inflateEnd (&stream);
  എങ്കിൽ (status != Z_STREAM_END || total_out != raw_len)
    തിരികെ അസത്യം;

  /* Un-filter and map to alpha. */
  എങ്കിൽ (!out->resize (mask_len))
    തിരികെ അസത്യം;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (width);
  എങ്കിൽ (!unfiltered)
    തിരികെ അസത്യം;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (width, 1);
  എങ്കിൽ (!prev_unfiltered)
    തിരികെ അസത്യം;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  വേണ്ടി (unsigned y = 0; y < height; y++)
  {
    uint8_t *row = raw + y * (width + 1);
    uint8_t filter = row[0];
    uint8_t *pixels = row + 1;

    വേണ്ടി (unsigned x = 0; x < width; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      തിരഞ്ഞെടുപ്പ് (filter)
      {
      അവസ്ഥ 0: unfiltered[x] = val; നിർത്തുക; /* None */
      അവസ്ഥ 1: unfiltered[x] = val + a; നിർത്തുക; /* Sub */
      അവസ്ഥ 2: unfiltered[x] = val + b; നിർത്തുക; /* Up */
      അവസ്ഥ 3: unfiltered[x] = val + (uint8_t) (((unsigned) a + b) / 2); നിർത്തുക; /* Average */
      അവസ്ഥ 4: /* Paeth */
      {
	പൂർണ്ണസംഖ്യ p = (പൂർണ്ണസംഖ്യ) a + (പൂർണ്ണസംഖ്യ) b - (പൂർണ്ണസംഖ്യ) c;
	പൂർണ്ണസംഖ്യ pa = abs (p - (പൂർണ്ണസംഖ്യ) a);
	പൂർണ്ണസംഖ്യ pb = abs (p - (പൂർണ്ണസംഖ്യ) b);
	പൂർണ്ണസംഖ്യ pc = abs (p - (പൂർണ്ണസംഖ്യ) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	നിർത്തുക;
      }
      സ്ഥിരസ്ഥിതി: unfiltered[x] = val; നിർത്തുക;
      }

      /* Map palette index to alpha. */
      uint8_t idx = unfiltered[x];
      out->arrayZ[y * width + x] = (idx < trns_len) ? trns[idx] : 0xFF;
    }

    /* Swap buffers. */
    uint8_t *tmp = prev_unfiltered;
    prev_unfiltered = unfiltered;
    unfiltered = tmp;
  }

  തിരികെ സത്യം;
#endif
}

/* Read a big-endian uint32 from a byte pointer. */
സ്ഥിതം വരിയിൽ uint32_t
hb_pdf_png_u32 (സ്ഥിരം uint8_t *p)
{
  തിരികെ ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

സ്ഥിതം hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    ശൂന്യം *paint_data,
		    hb_blob_t *image,
		    unsigned width,
		    unsigned height,
		    hb_tag_t format,
		    ദശാംശം slant HB_UNUSED,
		    hb_glyph_extents_t *extents,
		    ശൂന്യം *)
{
  എങ്കിൽ (format != HB_TAG ('p','n','g',' '))
    തിരികെ അസത്യം;
  എങ്കിൽ (!extents || !width || !height)
    തിരികെ അസത്യം;

  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ അസത്യം;
  auto *res = hb_pdf_get_resources (paint);
  എങ്കിൽ (unlikely (!res))
    തിരികെ അസത്യം;

  unsigned len = 0;
  സ്ഥിരം uint8_t *data = (സ്ഥിരം uint8_t *) hb_blob_get_data (image, &len);
  എങ്കിൽ (!data || len < 8)
    തിരികെ അസത്യം;

  /* Verify PNG signature. */
  സ്ഥിതം സ്ഥിരം uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  എങ്കിൽ (hb_memcmp (data, png_sig, 8) != 0)
    തിരികെ അസത്യം;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  unsigned png_w = 0, png_h = 0;
  unsigned colors = 3; /* default RGB */
  uint8_t color_type = 2;
  തർക്കം has_alpha = അസത്യം;
  സ്ഥിരം uint8_t *plte_data = nullptr;
  unsigned plte_len = 0;
  സ്ഥിരം uint8_t *trns_data = nullptr;
  unsigned trns_len = 0;
  hb_vector_buf_t idat;

  unsigned pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  ആകുമ്പോൾ (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (data + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (data + pos + 4);
    എങ്കിൽ (chunk_len > len - pos - 12)
      നിർത്തുക;
    സ്ഥിരം uint8_t *chunk_data = data + pos + 8;

    എങ്കിൽ (chunk_type == 0x49484452u) /* IHDR */
    {
      എങ്കിൽ (chunk_len < 13) തിരികെ അസത്യം;
      png_w = hb_pdf_png_u32 (chunk_data);
      png_h = hb_pdf_png_u32 (chunk_data + 4);
      uint8_t bit_depth = chunk_data[8];
      color_type = chunk_data[9];
      എങ്കിൽ (bit_depth != 8) തിരികെ അസത്യം; /* only 8-bit supported */

      തിരഞ്ഞെടുപ്പ് (color_type)
      {
      അവസ്ഥ 0: colors = 1; has_alpha = അസത്യം; നിർത്തുക; /* Grayscale */
      അവസ്ഥ 2: colors = 3; has_alpha = അസത്യം; നിർത്തുക; /* RGB */
      അവസ്ഥ 3: colors = 1; has_alpha = അസത്യം; നിർത്തുക; /* Indexed */
      അവസ്ഥ 4: colors = 2; has_alpha = സത്യം;  നിർത്തുക; /* Gray+Alpha */
      അവസ്ഥ 6: colors = 4; has_alpha = സത്യം;  നിർത്തുക; /* RGBA */
      സ്ഥിരസ്ഥിതി: തിരികെ അസത്യം;
      }
    }
    അല്ലെങ്കിൽ എങ്കിൽ (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_data = chunk_data;
      plte_len = chunk_len;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_data = chunk_data;
      trns_len = chunk_len;
    }
    അല്ലെങ്കിൽ എങ്കിൽ (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((സ്ഥിരം അക്ഷരം *) chunk_data, chunk_len);
    }
    അല്ലെങ്കിൽ എങ്കിൽ (chunk_type == 0x49454E44u) /* IEND */
      നിർത്തുക;

    pos += 12 + chunk_len;
  }

  എങ്കിൽ (!png_w || !png_h || !idat.length)
    തിരികെ അസത്യം;

  unsigned im_idx = res->add_xobject_png_image (idat.arrayZ, idat.length,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_data, plte_len,
						 trns_data, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  auto &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  ദശാംശം ix = (ദശാംശം) extents->x_bearing;
  ദശാംശം iy = (ദശാംശം) extents->y_bearing + (ദശാംശം) extents->height;
  ദശാംശം iw = (ദശാംശം) extents->width;
  ദശാംശം ih = -(ദശാംശം) extents->height; /* negative because image Y goes up but height is negative in extents */

  body.append_അക്കം (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_അക്കം (paint->sy (ih));
  body.append_c (' ');
  body.append_അക്കം (paint->sx (ix));
  body.append_c (' ');
  body.append_അക്കം (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  തിരികെ സത്യം;
}

/* ---- Gradient helpers ---- */

സ്ഥിതം തർക്കം
hb_pdf_gradient_needs_alpha (hb_array_t<സ്ഥിരം hb_color_stop_t> stops)
{
  വേണ്ടി (സ്ഥിരം auto &s : stops)
    എങ്കിൽ (hb_color_get_alpha (s.color) != 255)
      തിരികെ സത്യം;
  തിരികെ അസത്യം;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
സ്ഥിതം ശൂന്യം
hb_pdf_build_alpha_interpolation_function (hb_vector_buf_t *obj,
					   ദശാംശം a0, ദശാംശം a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_അക്കം (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_അക്കം (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
സ്ഥിതം unsigned
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_vector_paint_t *paint)
{
  unsigned എണ്ണം = paint->color_stops_scratch.length;

  എങ്കിൽ (എണ്ണം < 2)
  {
    ദശാംശം a = എണ്ണം ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    തിരികെ res->add_object (std::move (obj));
  }

  എങ്കിൽ (എണ്ണം == 2)
  {
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    തിരികെ res->add_object (std::move (obj));
  }

  hb_vector_t<unsigned> sub_func_ids;
  വേണ്ടി (unsigned i = 0; i + 1 < എണ്ണം; i++)
  {
    hb_vector_buf_t sub;
    hb_pdf_build_alpha_interpolation_function (&sub,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[i + 1].color) / 255.f);
    sub_func_ids.push (res->add_object (std::move (sub)));
  }

  hb_vector_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");
  obj.append_str ("/Functions [");
  വേണ്ടി (unsigned i = 0; i < sub_func_ids.length; i++)
  {
    എങ്കിൽ (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  വേണ്ടി (unsigned i = 1; i + 1 < എണ്ണം; i++)
  {
    എങ്കിൽ (i > 1) obj.append_c (' ');
    obj.append_അക്കം (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n/Encode [");
  വേണ്ടി (unsigned i = 0; i + 1 < എണ്ണം; i++)
  {
    എങ്കിൽ (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  തിരികെ res->add_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
സ്ഥിതം ശൂന്യം
hb_pdf_build_interpolation_function (hb_vector_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_അക്കം (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_അക്കം (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_അക്കം (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_അക്കം (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_അക്കം (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_അക്കം (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
സ്ഥിതം unsigned
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_vector_paint_t *paint)
{
  unsigned എണ്ണം = paint->color_stops_scratch.length;

  എങ്കിൽ (എണ്ണം < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = എണ്ണം ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    തിരികെ res->add_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (സ്ഥിരം hb_color_stop_t &a, സ്ഥിരം hb_color_stop_t &b)
    { തിരികെ (a.offset > b.offset) - (a.offset < b.offset); });

  എങ്കിൽ (എണ്ണം == 2)
  {
    /* Two stops: single interpolation function. */
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    തിരികെ res->add_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_vector_t<unsigned> sub_func_ids;
  വേണ്ടി (unsigned i = 0; i + 1 < എണ്ണം; i++)
  {
    hb_vector_buf_t sub;
    hb_pdf_build_interpolation_function (&sub,
					  paint->color_stops_scratch.arrayZ[i].color,
					  paint->color_stops_scratch.arrayZ[i + 1].color);
    sub_func_ids.push (res->add_object (std::move (sub)));
  }

  /* Stitching function (Type 3). */
  hb_vector_buf_t obj;
  obj.append_str ("<< /FunctionType 3 /Domain [0 1]\n");

  /* Functions array. */
  obj.append_str ("/Functions [");
  വേണ്ടി (unsigned i = 0; i < sub_func_ids.length; i++)
  {
    എങ്കിൽ (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  വേണ്ടി (unsigned i = 1; i + 1 < എണ്ണം; i++)
  {
    എങ്കിൽ (i > 1) obj.append_c (' ');
    obj.append_അക്കം (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  വേണ്ടി (unsigned i = 0; i + 1 < എണ്ണം; i++)
  {
    എങ്കിൽ (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  തിരികെ res->add_object (std::move (obj));
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_linear_gradient (hb_paint_funcs_t *,
			      ശൂന്യം *paint_data,
			      hb_color_വരി_t *color_വരി,
			      ദശാംശം x0, ദശാംശം y0,
			      ദശാംശം x1, ദശാംശം y1,
			      ദശാംശം x2 HB_UNUSED, ദശാംശം y2 HB_UNUSED,
			      ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  auto *res = hb_pdf_get_resources (paint);
  എങ്കിൽ (unlikely (!res))
    തിരികെ;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  എങ്കിൽ (!paint->fetch_color_stops (color_വരി))
    തിരികെ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ദശാംശം mn, mx;
  hb_paint_normalize_color_വരി (stops.arrayZ, stops.length, &mn, &mx);
  ദശാംശം gx0 = x0 + mn * (x1 - x0);
  ദശാംശം gy0 = y0 + mn * (y1 - y0);
  ദശാംശം gx1 = x0 + mx * (x1 - x0);
  ദശാംശം gy1 = y0 + mx * (y1 - y0);

  unsigned func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_വരി_get_extend (color_വരി);
  സ്ഥിരം അക്ഷരം *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_vector_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_അക്കം (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  unsigned sh_idx = res->add_shading (std::move (sh));

  auto &body = paint->current_body ();

  തർക്കം needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  എങ്കിൽ (needs_alpha)
  {
    unsigned alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_vector_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_അക്കം (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    unsigned alpha_sh_id = res->add_object (std::move (ash));

    unsigned gs_idx = res->add_extgstate_smask (alpha_sh_id,
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

സ്ഥിതം ശൂന്യം
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      ശൂന്യം *paint_data,
			      hb_color_വരി_t *color_വരി,
			      ദശാംശം x0, ദശാംശം y0, ദശാംശം r0,
			      ദശാംശം x1, ദശാംശം y1, ദശാംശം r1,
			      ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  auto *res = hb_pdf_get_resources (paint);
  എങ്കിൽ (unlikely (!res))
    തിരികെ;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  എങ്കിൽ (!paint->fetch_color_stops (color_വരി))
    തിരികെ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ദശാംശം mn, mx;
  hb_paint_normalize_color_വരി (stops.arrayZ, stops.length, &mn, &mx);
  ദശാംശം gx0 = x0 + mn * (x1 - x0);
  ദശാംശം gy0 = y0 + mn * (y1 - y0);
  ദശാംശം gr0 = r0 + mn * (r1 - r0);
  ദശാംശം gx1 = x0 + mx * (x1 - x0);
  ദശാംശം gy1 = y0 + mx * (y1 - y0);
  ദശാംശം gr1 = r0 + mx * (r1 - r0);

  unsigned func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_വരി_get_extend (color_വരി);
  സ്ഥിരം അക്ഷരം *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_vector_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_അക്കം (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_അക്കം (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  unsigned sh_idx = res->add_shading (std::move (sh));

  auto &body = paint->current_body ();

  തർക്കം needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  എങ്കിൽ (needs_alpha)
  {
    unsigned alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_vector_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_അക്കം (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_അക്കം (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    unsigned alpha_sh_id = res->add_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    ദശാംശം cx = (gr1 >= gr0) ? gx1 : gx0;
    ദശാംശം cy = (gr1 >= gr0) ? gy1 : gy0;
    ദശാംശം rr = hb_max (gr0, gr1);
    unsigned gs_idx = res->add_extgstate_smask (alpha_sh_id,
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
സ്ഥിതം ശൂന്യം
hb_pdf_encode_u16 (hb_vector_buf_t *buf, uint16_t v)
{
  അക്ഷരം bytes[2] = {(അക്ഷരം) (v >> 8), (അക്ഷരം) (v & 0xFF)};
  buf->append_len (bytes, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
സ്ഥിതം ശൂന്യം
hb_pdf_encode_coord (hb_vector_buf_t *buf,
		     ദശാംശം val, ദശാംശം lo, ദശാംശം hi)
{
  ദശാംശം t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
സ്ഥിതം ശൂന്യം
hb_pdf_encode_color_rgb (hb_vector_buf_t *buf, hb_color_t c)
{
  അക്ഷരം rgb[3] = {(അക്ഷരം) hb_color_get_red (c),
		 (അക്ഷരം) hb_color_get_green (c),
		 (അക്ഷരം) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
സ്ഥിതം ശൂന്യം
hb_pdf_encode_color_alpha (hb_vector_buf_t *buf, hb_color_t c)
{
  അക്ഷരം a = (അക്ഷരം) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
സ്ഥിതം ശൂന്യം
hb_pdf_encode_point (hb_vector_buf_t *buf,
		     ദശാംശം x, ദശാംശം y,
		     ദശാംശം xlo, ദശാംശം xhi,
		     ദശാംശം ylo, ദശാംശം yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
സ്ഥിതം ശൂന്യം
hb_pdf_add_sweep_patch (hb_vector_buf_t *mesh,
			hb_vector_buf_t *alpha_mesh,
			ദശാംശം cx, ദശാംശം cy,
			ദശാംശം xlo, ദശാംശം xhi, ദശാംശം ylo, ദശാംശം yhi,
			ദശാംശം a0, hb_color_t c0_in,
			ദശാംശം a1, hb_color_t c1_in)
{
  സ്ഥിരം ദശാംശം R = 32767.f;
  സ്ഥിരം ദശാംശം eps = 0.5f;
  സ്ഥിരം ദശാംശം MAX_SECTOR = (ദശാംശം) M_PI / 2.f;

  പൂർണ്ണസംഖ്യ അക്കം_splits = (പൂർണ്ണസംഖ്യ) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  എങ്കിൽ (അക്കം_splits < 1) അക്കം_splits = 1;

  വേണ്ടി (പൂർണ്ണസംഖ്യ s = 0; s < അക്കം_splits; s++)
  {
    ദശാംശം k0 = (ദശാംശം) s / അക്കം_splits;
    ദശാംശം k1 = (ദശാംശം) (s + 1) / അക്കം_splits;
    ദശാംശം sa0 = a0 + k0 * (a1 - a0);
    ദശാംശം sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_in, c1_in, k0);
    hb_color_t sc1 = hb_color_lerp (c0_in, c1_in, k1);

    ദശാംശം da = sa1 - sa0;
    ദശാംശം kappa = (4.f / 3.f) * tanf (da / 4.f);

    ദശാംശം cos0 = cosf (sa0), sin0 = sinf (sa0);
    ദശാംശം cos1 = cosf (sa1), sin1 = sinf (sa1);

    ദശാംശം p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    ദശാംശം p3x = cx + R * cos0,   p3y = cy + R * sin0;
    ദശാംശം p6x = cx + R * cos1,   p6y = cy + R * sin1;
    ദശാംശം p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    ദശാംശം e1_1x = p0x + (p3x - p0x) / 3.f;
    ദശാംശം e1_1y = p0y + (p3y - p0y) / 3.f;
    ദശാംശം e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    ദശാംശം e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    ദശാംശം e2_1x = p3x + kappa * R * (-sin0);
    ദശാംശം e2_1y = p3y + kappa * R * ( cos0);
    ദശാംശം e2_2x = p6x - kappa * R * (-sin1);
    ദശാംശം e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    ദശാംശം e3_1x = p6x + (p9x - p6x) / 3.f;
    ദശാംശം e3_1y = p6y + (p9y - p6y) / 3.f;
    ദശാംശം e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    ദശാംശം e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    ദശാംശം e4_1x = p9x + kappa * eps * (-sin1);
    ദശാംശം e4_1y = p9y + kappa * eps * ( cos1);
    ദശാംശം e4_2x = p0x - kappa * eps * (-sin0);
    ദശാംശം e4_2y = p0y - kappa * eps * ( cos0);

    mesh->append_c ('\0'); /* flag = 0, new patch */

    hb_pdf_encode_point (mesh, p0x, p0y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, p3x, p3y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, p6x, p6y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, p9x, p9y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
    hb_pdf_encode_point (mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

    hb_pdf_encode_color_rgb (mesh, sc0); /* inner start */
    hb_pdf_encode_color_rgb (mesh, sc0); /* outer start */
    hb_pdf_encode_color_rgb (mesh, sc1); /* outer end */
    hb_pdf_encode_color_rgb (mesh, sc1); /* inner end */

    എങ്കിൽ (alpha_mesh)
    {
      alpha_mesh->append_c ('\0');

      hb_pdf_encode_point (alpha_mesh, p0x, p0y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e1_1x, e1_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e1_2x, e1_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, p3x, p3y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e2_1x, e2_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e2_2x, e2_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, p6x, p6y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e3_1x, e3_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e3_2x, e3_2y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, p9x, p9y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e4_1x, e4_1y, xlo, xhi, ylo, yhi);
      hb_pdf_encode_point (alpha_mesh, e4_2x, e4_2y, xlo, xhi, ylo, yhi);

      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc0);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
      hb_pdf_encode_color_alpha (alpha_mesh, sc1);
    }
  }
}

/* Callback context + trampoline for hb_paint_sweep_gradient_tiles. */
ഘടന hb_pdf_sweep_ctx_t {
  hb_vector_buf_t *mesh;
  hb_vector_buf_t *alpha_mesh;
  ദശാംശം cx, cy, xlo, xhi, ylo, yhi;
};

സ്ഥിതം ശൂന്യം
hb_pdf_sweep_emit_patch (ദശാംശം a0, hb_color_t c0,
			 ദശാംശം a1, hb_color_t c1,
			 ശൂന്യം *user_data)
{
  auto *ctx = (hb_pdf_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  എങ്കിൽ (unlikely (ctx->mesh->in_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->in_error ())))
    തിരികെ;
  hb_pdf_add_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     ശൂന്യം *paint_data,
			     hb_color_വരി_t *color_വരി,
			     ദശാംശം cx, ദശാംശം cy,
			     ദശാംശം start_angle,
			     ദശാംശം end_angle,
			     ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  auto *res = hb_pdf_get_resources (paint);
  എങ്കിൽ (unlikely (!res))
    തിരികെ;

  /* Get and sort color stops. */
  എങ്കിൽ (!paint->fetch_color_stops (color_വരി))
    തിരികെ;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (സ്ഥിരം hb_color_stop_t &a, സ്ഥിരം hb_color_stop_t &b)
    { തിരികെ (a.offset > b.offset) - (a.offset < b.offset); });

  hb_paint_extend_t extend = hb_color_വരി_get_extend (color_വരി);

  സ്ഥിരം ദശാംശം R = 32767.f;
  ദശാംശം scx = paint->sx (cx), scy = paint->sy (cy);
  ദശാംശം xlo = scx - R - 1, xhi = scx + R + 1;
  ദശാംശം ylo = scy - R - 1, yhi = scy + R + 1;

  തർക്കം needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_vector_buf_t mesh;
  hb_vector_buf_t alpha_mesh;
  mesh.alloc (256);
  എങ്കിൽ (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : nullptr,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length, extend,
				 start_angle, end_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  എങ്കിൽ (!mesh.length)
    തിരികെ;

  auto hb_pdf_build_mesh_shading = [&] (hb_vector_buf_t &m,
					 സ്ഥിരം അക്ഷരം *cs,
					 സ്ഥിരം അക്ഷരം *decode_suffix) -> unsigned
  {
    hb_vector_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_അക്കം (xlo, 2);
    sh.append_c (' ');
    sh.append_അക്കം (xhi, 2);
    sh.append_c (' ');
    sh.append_അക്കം (ylo, 2);
    sh.append_c (' ');
    sh.append_അക്കം (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.length);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.length);
    sh.append_str ("\nendstream");
    തിരികെ res->add_object (std::move (sh));
  };

  unsigned sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  unsigned sh_idx = res->add_shading_by_id (sh_obj_id);

  auto &body = paint->current_body ();

  എങ്കിൽ (needs_alpha && alpha_mesh.length)
  {
    unsigned alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    unsigned gs_idx = res->add_extgstate_smask (alpha_sh_id,
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

സ്ഥിതം സ്ഥിരം അക്ഷരം *
hb_pdf_blend_mode_name (hb_paint_composite_mode_t mode)
{
  തിരഞ്ഞെടുപ്പ് (mode)
  {
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_MULTIPLY:       തിരികെ "Multiply";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SCREEN:         തിരികെ "Screen";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_OVERLAY:        തിരികെ "Overlay";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DARKEN:         തിരികെ "Darken";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_LIGHTEN:        തിരികെ "Lighten";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:    തിരികെ "ColorDodge";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_COLOR_BURN:     തിരികെ "ColorBurn";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:     തിരികെ "HardLight";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:     തിരികെ "SoftLight";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DIFFERENCE:     തിരികെ "Difference";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_EXCLUSION:      തിരികെ "Exclusion";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_HUE:        തിരികെ "Hue";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: തിരികെ "Saturation";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_COLOR:      തിരികെ "Color";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: തിരികെ "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_PLUS:           തിരികെ "Screen";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_XOR:            തിരികെ "Difference";
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_CLEAR:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OVER:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_IN:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_IN:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  അവസ്ഥ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  സ്ഥിരസ്ഥിതി:                                     തിരികെ nullptr; /* Normal */
  }
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_തള്ളുക_group (hb_paint_funcs_t *,
			 ശൂന്യം *paint_data,
			 ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  paint->current_body ().append_str ("q\n");
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_തള്ളുക_group_for (hb_paint_funcs_t *,
			     ശൂന്യം *paint_data,
			     hb_paint_composite_mode_t mode,
			     ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;

  auto &body = paint->current_body ();
  body.append_str ("q\n");

  സ്ഥിരം അക്ഷരം *bm = hb_pdf_blend_mode_name (mode);
  എങ്കിൽ (bm)
  {
    auto *res = hb_pdf_get_resources (paint);
    എങ്കിൽ (likely (res))
    {
      unsigned gs_idx = res->add_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

സ്ഥിതം ശൂന്യം
hb_pdf_paint_എടുക്കുക_group (hb_paint_funcs_t *,
			ശൂന്യം *paint_data,
			hb_paint_composite_mode_t mode HB_UNUSED,
			ശൂന്യം *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  എങ്കിൽ (unlikely (!paint->ensure_initialized ()))
    തിരികെ;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_pdf_paint_funcs ();

സ്ഥിതം ഘടന hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  സ്ഥിതം hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_തള്ളുക_transform_func (funcs, (hb_paint_തള്ളുക_transform_func_t) hb_pdf_paint_തള്ളുക_transform, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_transform_func (funcs, (hb_paint_എടുക്കുക_transform_func_t) hb_pdf_paint_എടുക്കുക_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_glyph_func (funcs, (hb_paint_തള്ളുക_clip_glyph_func_t) hb_pdf_paint_തള്ളുക_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_rectangle_func (funcs, (hb_paint_തള്ളുക_clip_rectangle_func_t) hb_pdf_paint_തള്ളുക_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_path_start_func (funcs, (hb_paint_തള്ളുക_clip_path_start_func_t) hb_pdf_paint_തള്ളുക_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_clip_path_end_func (funcs, (hb_paint_തള്ളുക_clip_path_end_func_t) hb_pdf_paint_തള്ളുക_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_clip_func (funcs, (hb_paint_എടുക്കുക_clip_func_t) hb_pdf_paint_എടുക്കുക_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_pdf_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_group_func (funcs, (hb_paint_തള്ളുക_group_func_t) hb_pdf_paint_തള്ളുക_group, nullptr, nullptr);
    hb_paint_funcs_set_തള്ളുക_group_for_func (funcs, (hb_paint_തള്ളുക_group_for_func_t) hb_pdf_paint_തള്ളുക_group_for, nullptr, nullptr);
    hb_paint_funcs_set_എടുക്കുക_group_func (funcs, (hb_paint_എടുക്കുക_group_func_t) hb_pdf_paint_എടുക്കുക_group, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    തിരികെ funcs;
  }
} static_pdf_paint_funcs;

സ്ഥിതം വരിയിൽ ശൂന്യം
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_pdf_funcs_get ()
{
  തിരികെ static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_vector_paint_render_pdf (hb_vector_paint_t *paint)
{
  എങ്കിൽ (!paint->has_extents)
    തിരികെ nullptr;
  എങ്കിൽ (!paint->group_അട്ടി.length ||
      !paint->group_അട്ടി.arrayZ[0].length)
    തിരികെ nullptr;

  hb_vector_buf_t &content = paint->group_അട്ടി.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  ദശാംശം ex = paint->extents.x;
  ദശാംശം ey = paint->extents.y;
  ദശാംശം ew = paint->extents.width;
  ദശാംശം eh = paint->extents.height;

  unsigned അക്കം_extra = res ? res->objects.length : 0;
  unsigned total_objects = 4 + അക്കം_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  out.alloc (content.length + അക്കം_extra * 128 + 1024);

  hb_vector_t<unsigned> offsets;
  എങ്കിൽ (unlikely (!offsets.resize (total_objects)))
    തിരികെ nullptr;

  out.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets.arrayZ[0] = out.length;
  out.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets.arrayZ[1] = out.length;
  out.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page */
  offsets.arrayZ[2] = out.length;
  out.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  out.append_അക്കം (ex);
  out.append_c (' ');
  out.append_അക്കം (ey);
  out.append_c (' ');
  out.append_അക്കം (ex + ew);
  out.append_c (' ');
  out.append_അക്കം (ey + eh);
  out.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  തർക്കം has_resources = res &&
    (res->extgstate_dict.length || res->shading_dict.length || res->xobject_dict.length);
  എങ്കിൽ (has_resources)
  {
    out.append_str ("\n/Resources <<");
    എങ്കിൽ (res->extgstate_dict.length)
    {
      out.append_str (" /ExtGState << ");
      out.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.length);
      out.append_str (">>");
    }
    എങ്കിൽ (res->shading_dict.length)
    {
      out.append_str (" /Shading << ");
      out.append_len (res->shading_dict.arrayZ, res->shading_dict.length);
      out.append_str (">>");
    }
    എങ്കിൽ (res->xobject_dict.length)
    {
      out.append_str (" /XObject << ");
      out.append_len (res->xobject_dict.arrayZ, res->xobject_dict.length);
      out.append_str (">>");
    }
    out.append_str (" >>");
  }

  out.append_str (" >>\nendobj\n");

  /* Build content stream: optional background rect + glyph content. */
  hb_vector_buf_t bg_prefix;
  എങ്കിൽ (hb_color_get_alpha (paint->background))
  {
    ദശാംശം r = hb_color_get_red (paint->background) / 255.f;
    ദശാംശം g = hb_color_get_green (paint->background) / 255.f;
    ദശാംശം b = hb_color_get_blue (paint->background) / 255.f;
    ദശാംശം a = hb_color_get_alpha (paint->background) / 255.f;
    എങ്കിൽ (a < 1.f - 1.f / 512.f)
    {
      എങ്കിൽ (res)
      {
	unsigned gs_idx = res->add_extgstate_alpha (a);
	bg_prefix.append_str ("/GS");
	bg_prefix.append_unsigned (gs_idx);
	bg_prefix.append_str (" gs\n");
      }
    }
    bg_prefix.append_അക്കം (r, 4);
    bg_prefix.append_c (' ');
    bg_prefix.append_അക്കം (g, 4);
    bg_prefix.append_c (' ');
    bg_prefix.append_അക്കം (b, 4);
    bg_prefix.append_str (" rg\n");
    bg_prefix.append_അക്കം (ex);
    bg_prefix.append_c (' ');
    bg_prefix.append_അക്കം (ey);
    bg_prefix.append_c (' ');
    bg_prefix.append_അക്കം (ew);
    bg_prefix.append_c (' ');
    bg_prefix.append_അക്കം (eh);
    bg_prefix.append_str (" re f\n");
  }
  unsigned stream_len = bg_prefix.length + content.length;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = out.length;
  out.append_str ("4 0 obj\n<< /Length ");
  out.append_unsigned (stream_len);
  out.append_str (" >>\nstream\n");
  out.append_len (bg_prefix.arrayZ, bg_prefix.length);
  out.append_len (content.arrayZ, content.length);
  out.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  വേണ്ടി (unsigned i = 0; i < അക്കം_extra; i++)
  {
    offsets.arrayZ[4 + i] = out.length;
    out.append_unsigned (5 + i);
    out.append_str (" 0 obj\n");
    auto &obj = res->objects.arrayZ[i];
    out.append_len (obj.data.arrayZ, obj.data.length);
    out.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  unsigned xref_offset = out.length;
  out.append_str ("xref\n0 ");
  out.append_unsigned (total_objects + 1);
  out.append_str ("\n0000000000 65535 f \n");
  വേണ്ടി (unsigned i = 0; i < total_objects; i++)
  {
    അക്ഷരം tmp[21];
    snprintf (tmp, വലുപ്പം (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    out.append_len (tmp, 20);
  }

  /* Trailer */
  out.append_str ("trailer\n<< /Size ");
  out.append_unsigned (total_objects + 1);
  out.append_str (" /Root 1 0 R >>\nstartxref\n");
  out.append_unsigned (xref_offset);
  out.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  തിരികെ blob;
}

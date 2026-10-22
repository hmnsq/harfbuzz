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

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-vector-paint.hh"
#अन्तर्भाव "hb-vector-draw.hh"
#अन्तर्भाव "hb-paint.hh"

#अन्तर्भाव <math.h>
#अन्तर्भाव <stdio.h>

#ifdef HAVE_ZLIB
#अन्तर्भाव <zlib.h>
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

संरचना hb_pdf_obj_t
{
  hb_vector_buf_t data;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
स्थिर तार्किक
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    ध्रुव वर्ण *idat_data, अचिन्हित idat_len,
			    अचिन्हित width, अचिन्हित height,
			    ध्रुव uint8_t *trns, अचिन्हित trns_len);

संरचना hb_pdf_resources_t
{
  hb_vector_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_vector_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_vector_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_vector_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  अचिन्हित extgstate_गिनती = 0;
  अचिन्हित shading_गिनती = 0;
  अचिन्हित xobject_गिनती = 0;

  अचिन्हित add_object (hb_vector_buf_t &&obj_data)
  {
    अचिन्हित id = 5 + objects.length; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.data = std::move (obj_data);
    objects.push (std::move (obj));
    प्रत्यागम id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  अचिन्हित add_extgstate_alpha (भिन्न alpha)
  {
    अचिन्हित idx = extgstate_गिनती++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_अंक (alpha, 4);
    obj.append_str (" >>");
    अचिन्हित obj_id = add_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    प्रत्यागम idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  अचिन्हित add_extgstate_blend (ध्रुव वर्ण *bm)
  {
    अचिन्हित idx = extgstate_गिनती++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /BM /");
    obj.append_str (bm);
    obj.append_str (" >>");
    अचिन्हित obj_id = add_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    प्रत्यागम idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  अचिन्हित add_extgstate_smask (अचिन्हित alpha_shading_id,
				भिन्न bbox_x, भिन्न bbox_y,
				भिन्न bbox_w, भिन्न bbox_h,
				अचिन्हित precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_vector_buf_t form_stream;
    form_stream.append_str ("/SHa sh\n");

    hb_vector_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_अंक (bbox_x, precision);
    form.append_c (' ');
    form.append_अंक (bbox_y, precision);
    form.append_c (' ');
    form.append_अंक (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_अंक (bbox_y + bbox_h, precision);
    form.append_str ("]\n/Group << /S /Transparency /CS /DeviceGray >>\n");
    form.append_str ("/Resources << /Shading << /SHa ");
    form.append_unsigned (alpha_shading_id);
    form.append_str (" 0 R >> >>\n");
    form.append_str ("/Length ");
    form.append_unsigned (form_stream.length);
    form.append_str (" >>\nstream\n");
    form.append_len (form_stream.arrayZ, form_stream.length);
    form.append_str ("endstream");
    अचिन्हित form_id = add_object (std::move (form));

    /* ExtGState with luminosity soft mask. */
    अचिन्हित idx = extgstate_गिनती++;
    hb_vector_buf_t gs;
    gs.append_str ("<< /Type /ExtGState\n");
    gs.append_str ("/SMask << /Type /Mask /S /Luminosity /G ");
    gs.append_unsigned (form_id);
    gs.append_str (" 0 R >> >>");
    अचिन्हित gs_id = add_object (std::move (gs));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (gs_id);
    extgstate_dict.append_str (" 0 R ");
    प्रत्यागम idx;
  }

  /* Add a shading, return resource name index. */
  अचिन्हित add_shading (hb_vector_buf_t &&shading_data)
  {
    अचिन्हित obj_id = add_object (std::move (shading_data));
    प्रत्यागम add_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  अचिन्हित add_shading_by_id (अचिन्हित obj_id)
  {
    अचिन्हित idx = shading_गिनती++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    प्रत्यागम idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  अचिन्हित add_xobject_png_image (ध्रुव वर्ण *idat_data, अचिन्हित idat_len,
				  अचिन्हित width, अचिन्हित height,
				  अचिन्हित colors, तार्किक has_alpha,
				  ध्रुव uint8_t *plte = nullptr,
				  अचिन्हित plte_len = 0,
				  ध्रुव uint8_t *trns = nullptr,
				  अचिन्हित trns_len = 0)
  {
    अचिन्हित idx = xobject_गिनती++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (width);
    obj.append_str (" /Height ");
    obj.append_unsigned (height);
    obj.append_str ("\n/BitsPerComponent 8\n");

    यदि (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      अचिन्हित n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      प्रति (अचिन्हित i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    अन्यथा
    {
      obj.append_str ("/ColorSpace ");
      अचिन्हित color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    अचिन्हित smask_id = 0;
    यदि (plte && trns && trns_len)
    {
      hb_vector_buf_t smask_stream;
      यदि (hb_pdf_build_indexed_smask (&smask_stream, idat_data, idat_len,
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

    यदि (smask_id)
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

    (शून्य) has_alpha;

    अचिन्हित obj_id = add_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    प्रत्यागम idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
स्थिर hb_pdf_resources_t *
hb_pdf_get_resources (hb_vector_paint_t *paint)
{
  यदि (!paint->defs.length)
  {
    /* First call: allocate and store. */
    स्वतः *res = (hb_pdf_resources_t *) hb_calloc (1, परिमाण (hb_pdf_resources_t));
    यदि (unlikely (!res)) प्रत्यागम nullptr;
    नूतन (res) hb_pdf_resources_t ();
    यदि (unlikely (!paint->defs.resize (परिमाण (शून्य *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      प्रत्यागम nullptr;
    }
    स_नकल (paint->defs.arrayZ, &res, परिमाण (शून्य *));
  }
  hb_pdf_resources_t *res;
  स_नकल (&res, paint->defs.arrayZ, परिमाण (शून्य *));
  प्रत्यागम res;
}

शून्य
hb_vector_paint_pdf_free_resources (hb_vector_paint_t *paint)
{
  यदि (paint->defs.length >= परिमाण (शून्य *))
  {
    hb_pdf_resources_t *res;
    स_नकल (&res, paint->defs.arrayZ, परिमाण (शून्य *));
    यदि (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
स्थिर शून्य
hb_pdf_emit_glyph_path (hb_vector_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_vector_buf_t *buf)
{
  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  यदि (likely (paint->budget_remaining >= 0))
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

स्थिर शून्य
hb_pdf_paint_धकेलो_transform (hb_paint_funcs_t *,
			     शून्य *paint_data,
			     भिन्न xx, भिन्न yx,
			     भिन्न xy, भिन्न yy,
			     भिन्न dx, भिन्न dy,
			     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  स्वतः &body = paint->current_body ();
  अचिन्हित sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_अंक (xx, sprec);
  body.append_c (' ');
  body.append_अंक (yx, sprec);
  body.append_c (' ');
  body.append_अंक (xy, sprec);
  body.append_c (' ');
  body.append_अंक (yy, sprec);
  body.append_c (' ');
  body.append_अंक (paint->sx (dx));
  body.append_c (' ');
  body.append_अंक (paint->sy (dy));
  body.append_str (" cm\n");
}

स्थिर शून्य
hb_pdf_paint_निकालो_transform (hb_paint_funcs_t *,
			    शून्य *paint_data,
			    शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  paint->current_body ().append_str ("Q\n");
}

स्थिर शून्य
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 शून्य *paint_data,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  भिन्न r = hb_color_get_red (c) / 255.f;
  भिन्न g = hb_color_get_green (c) / 255.f;
  भिन्न b = hb_color_get_blue (c) / 255.f;
  भिन्न a = hb_color_get_alpha (c) / 255.f;

  यदि (a < 1.f / 255.f)
    प्रत्यागम;

  स्वतः &body = paint->current_body ();

  तार्किक scoped = असत्य;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  यदि (a < 1.f - 1.f / 512.f)
  {
    स्वतः *res = hb_pdf_get_resources (paint);
    यदि (res)
    {
      अचिन्हित gs_idx = res->add_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = सत्य;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_अंक (r, 4);
  body.append_c (' ');
  body.append_अंक (g, 4);
  body.append_c (' ');
  body.append_अंक (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("f\n");
  यदि (scoped)
    body.append_str ("Q\n");
}

स्थिर शून्य
hb_pdf_paint_धकेलो_clip_glyph (hb_paint_funcs_t *,
			      शून्य *paint_data,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  स्वतः &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

स्थिर शून्य
hb_pdf_paint_धकेलो_clip_rectangle (hb_paint_funcs_t *,
				  शून्य *paint_data,
				  भिन्न xmin, भिन्न ymin,
				  भिन्न xmax, भिन्न ymax,
				  शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  स्वतः &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_अंक (paint->sx (xmin));
  body.append_c (' ');
  body.append_अंक (paint->sy (ymin));
  body.append_c (' ');
  body.append_अंक (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_अंक (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

स्थिर hb_draw_funcs_t *
hb_pdf_paint_धकेलो_clip_path_start (hb_paint_funcs_t *,
				   शून्य *paint_data,
				   शून्य **draw_data,
				   शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    प्रत्यागम nullptr;
  }

  स्वतः &body = paint->current_body ();
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
  प्रत्यागम hb_vector_pdf_path_draw_funcs_get ();
}

स्थिर शून्य
hb_pdf_paint_धकेलो_clip_path_end (hb_paint_funcs_t *,
				 शून्य *paint_data,
				 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  paint->current_body ().append_str ("W n\n");
}

स्थिर शून्य
hb_pdf_paint_निकालो_clip (hb_paint_funcs_t *,
		       शून्य *paint_data,
		       शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
स्थिर शून्य
hb_pdf_paint_solid_color (hb_vector_paint_t *paint, hb_color_t c)
{
  स्वतः &body = paint->current_body ();

  भिन्न r = hb_color_get_red (c) / 255.f;
  भिन्न g = hb_color_get_green (c) / 255.f;
  भिन्न b = hb_color_get_blue (c) / 255.f;
  भिन्न a = hb_color_get_alpha (c) / 255.f;

  यदि (a < 1.f / 255.f)
    प्रत्यागम;

  /* Set alpha via ExtGState if needed. */
  यदि (a < 1.f - 1.f / 512.f)
  {
    स्वतः *res = hb_pdf_get_resources (paint);
    यदि (res)
    {
      अचिन्हित gs_idx = res->add_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_अंक (r, 4);
  body.append_c (' ');
  body.append_अंक (g, 4);
  body.append_c (' ');
  body.append_अंक (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

स्थिर शून्य
hb_pdf_paint_color (hb_paint_funcs_t *,
		    शून्य *paint_data,
		    hb_bool_t,
		    hb_color_t color,
		    शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
स्थिर तार्किक
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    ध्रुव वर्ण *idat_data, अचिन्हित idat_len,
			    अचिन्हित width, अचिन्हित height,
			    ध्रुव uint8_t *trns, अचिन्हित trns_len)
{
#ifndef HAVE_ZLIB
  (शून्य) out; (शून्य) idat_data; (शून्य) idat_len;
  (शून्य) width; (शून्य) height; (शून्य) trns; (शून्य) trns_len;
  प्रत्यागम असत्य;
#अन्यथा
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  अचिन्हित raw_len, mask_len;
  यदि (!width || !height ||
      width == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (width + 1, height, &raw_len) ||
      hb_unsigned_mul_overflows (width, height, &mask_len))
    प्रत्यागम असत्य;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  यदि (!raw) प्रत्यागम असत्य;
  HB_SCOPE_GUARD (hb_free (raw));

  z_stream stream = {};
  stream.next_in = (Bytef *) idat_data;
  stream.avail_in = idat_len;
  stream.next_out = (Bytef *) raw;
  stream.avail_out = raw_len;

  यदि (inflateInit (&stream) != Z_OK)
    प्रत्यागम असत्य;
  पूर्णाङ्क status = inflate (&stream, Z_FINISH);
  अचिन्हित दीर्घ total_out = stream.total_out;
  inflateEnd (&stream);
  यदि (status != Z_STREAM_END || total_out != raw_len)
    प्रत्यागम असत्य;

  /* Un-filter and map to alpha. */
  यदि (!out->resize (mask_len))
    प्रत्यागम असत्य;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (width);
  यदि (!unfiltered)
    प्रत्यागम असत्य;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (width, 1);
  यदि (!prev_unfiltered)
    प्रत्यागम असत्य;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  प्रति (अचिन्हित y = 0; y < height; y++)
  {
    uint8_t *row = raw + y * (width + 1);
    uint8_t filter = row[0];
    uint8_t *pixels = row + 1;

    प्रति (अचिन्हित x = 0; x < width; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      विकल्प (filter)
      {
      पक्ष 0: unfiltered[x] = val; भङ्ग; /* None */
      पक्ष 1: unfiltered[x] = val + a; भङ्ग; /* Sub */
      पक्ष 2: unfiltered[x] = val + b; भङ्ग; /* Up */
      पक्ष 3: unfiltered[x] = val + (uint8_t) (((अचिन्हित) a + b) / 2); भङ्ग; /* Average */
      पक्ष 4: /* Paeth */
      {
	पूर्णाङ्क p = (पूर्णाङ्क) a + (पूर्णाङ्क) b - (पूर्णाङ्क) c;
	पूर्णाङ्क pa = असल (p - (पूर्णाङ्क) a);
	पूर्णाङ्क pb = असल (p - (पूर्णाङ्क) b);
	पूर्णाङ्क pc = असल (p - (पूर्णाङ्क) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	भङ्ग;
      }
      स्वभाव: unfiltered[x] = val; भङ्ग;
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

  प्रत्यागम सत्य;
#endif
}

/* Read a big-endian uint32 from a byte pointer. */
स्थिर अन्तःस्थ uint32_t
hb_pdf_png_u32 (ध्रुव uint8_t *p)
{
  प्रत्यागम ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

स्थिर hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    शून्य *paint_data,
		    hb_blob_t *image,
		    अचिन्हित width,
		    अचिन्हित height,
		    hb_tag_t format,
		    भिन्न slant HB_UNUSED,
		    hb_glyph_extents_t *extents,
		    शून्य *)
{
  यदि (format != HB_TAG ('p','n','g',' '))
    प्रत्यागम असत्य;
  यदि (!extents || !width || !height)
    प्रत्यागम असत्य;

  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम असत्य;
  स्वतः *res = hb_pdf_get_resources (paint);
  यदि (unlikely (!res))
    प्रत्यागम असत्य;

  अचिन्हित len = 0;
  ध्रुव uint8_t *data = (ध्रुव uint8_t *) hb_blob_get_data (image, &len);
  यदि (!data || len < 8)
    प्रत्यागम असत्य;

  /* Verify PNG signature. */
  स्थिर ध्रुव uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  यदि (hb_memcmp (data, png_sig, 8) != 0)
    प्रत्यागम असत्य;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  अचिन्हित png_w = 0, png_h = 0;
  अचिन्हित colors = 3; /* default RGB */
  uint8_t color_type = 2;
  तार्किक has_alpha = असत्य;
  ध्रुव uint8_t *plte_data = nullptr;
  अचिन्हित plte_len = 0;
  ध्रुव uint8_t *trns_data = nullptr;
  अचिन्हित trns_len = 0;
  hb_vector_buf_t idat;

  अचिन्हित pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  यावत् (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (data + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (data + pos + 4);
    यदि (chunk_len > len - pos - 12)
      भङ्ग;
    ध्रुव uint8_t *chunk_data = data + pos + 8;

    यदि (chunk_type == 0x49484452u) /* IHDR */
    {
      यदि (chunk_len < 13) प्रत्यागम असत्य;
      png_w = hb_pdf_png_u32 (chunk_data);
      png_h = hb_pdf_png_u32 (chunk_data + 4);
      uint8_t bit_depth = chunk_data[8];
      color_type = chunk_data[9];
      यदि (bit_depth != 8) प्रत्यागम असत्य; /* only 8-bit supported */

      विकल्प (color_type)
      {
      पक्ष 0: colors = 1; has_alpha = असत्य; भङ्ग; /* Grayscale */
      पक्ष 2: colors = 3; has_alpha = असत्य; भङ्ग; /* RGB */
      पक्ष 3: colors = 1; has_alpha = असत्य; भङ्ग; /* Indexed */
      पक्ष 4: colors = 2; has_alpha = सत्य;  भङ्ग; /* Gray+Alpha */
      पक्ष 6: colors = 4; has_alpha = सत्य;  भङ्ग; /* RGBA */
      स्वभाव: प्रत्यागम असत्य;
      }
    }
    अन्यथा यदि (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_data = chunk_data;
      plte_len = chunk_len;
    }
    अन्यथा यदि (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_data = chunk_data;
      trns_len = chunk_len;
    }
    अन्यथा यदि (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ध्रुव वर्ण *) chunk_data, chunk_len);
    }
    अन्यथा यदि (chunk_type == 0x49454E44u) /* IEND */
      भङ्ग;

    pos += 12 + chunk_len;
  }

  यदि (!png_w || !png_h || !idat.length)
    प्रत्यागम असत्य;

  अचिन्हित im_idx = res->add_xobject_png_image (idat.arrayZ, idat.length,
						 png_w, png_h,
						 colors, has_alpha,
						 plte_data, plte_len,
						 trns_data, trns_len);

  /* Emit: save state, set CTM to map image (0,0)-(1,1) to extents, paint. */
  स्वतः &body = paint->current_body ();
  body.append_str ("q\n");

  /* Image space: (0,0) at bottom-left, (1,1) at top-right.
   * We need to map to extents (x_bearing, y_bearing+height) .. (x_bearing+width, y_bearing).
   * Since font coords are Y-up like PDF, y_bearing is the top. */
  भिन्न ix = (भिन्न) extents->x_bearing;
  भिन्न iy = (भिन्न) extents->y_bearing + (भिन्न) extents->height;
  भिन्न iw = (भिन्न) extents->width;
  भिन्न ih = -(भिन्न) extents->height; /* negative because image Y goes up but height is negative in extents */

  body.append_अंक (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_अंक (paint->sy (ih));
  body.append_c (' ');
  body.append_अंक (paint->sx (ix));
  body.append_c (' ');
  body.append_अंक (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  प्रत्यागम सत्य;
}

/* ---- Gradient helpers ---- */

स्थिर तार्किक
hb_pdf_gradient_needs_alpha (hb_array_t<ध्रुव hb_color_stop_t> stops)
{
  प्रति (ध्रुव स्वतः &s : stops)
    यदि (hb_color_get_alpha (s.color) != 255)
      प्रत्यागम सत्य;
  प्रत्यागम असत्य;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
स्थिर शून्य
hb_pdf_build_alpha_interpolation_function (hb_vector_buf_t *obj,
					   भिन्न a0, भिन्न a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_अंक (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_अंक (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
स्थिर अचिन्हित
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_vector_paint_t *paint)
{
  अचिन्हित गिनती = paint->color_stops_scratch.length;

  यदि (गिनती < 2)
  {
    भिन्न a = गिनती ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    प्रत्यागम res->add_object (std::move (obj));
  }

  यदि (गिनती == 2)
  {
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    प्रत्यागम res->add_object (std::move (obj));
  }

  hb_vector_t<अचिन्हित> sub_func_ids;
  प्रति (अचिन्हित i = 0; i + 1 < गिनती; i++)
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
  प्रति (अचिन्हित i = 0; i < sub_func_ids.length; i++)
  {
    यदि (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  प्रति (अचिन्हित i = 1; i + 1 < गिनती; i++)
  {
    यदि (i > 1) obj.append_c (' ');
    obj.append_अंक (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n/Encode [");
  प्रति (अचिन्हित i = 0; i + 1 < गिनती; i++)
  {
    यदि (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  प्रत्यागम res->add_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
स्थिर शून्य
hb_pdf_build_interpolation_function (hb_vector_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_अंक (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_अंक (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_अंक (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_अंक (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_अंक (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_अंक (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
स्थिर अचिन्हित
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_vector_paint_t *paint)
{
  अचिन्हित गिनती = paint->color_stops_scratch.length;

  यदि (गिनती < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = गिनती ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    प्रत्यागम res->add_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().क्विक (
    [] (ध्रुव hb_color_stop_t &a, ध्रुव hb_color_stop_t &b)
    { प्रत्यागम (a.offset > b.offset) - (a.offset < b.offset); });

  यदि (गिनती == 2)
  {
    /* Two stops: single interpolation function. */
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    प्रत्यागम res->add_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_vector_t<अचिन्हित> sub_func_ids;
  प्रति (अचिन्हित i = 0; i + 1 < गिनती; i++)
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
  प्रति (अचिन्हित i = 0; i < sub_func_ids.length; i++)
  {
    यदि (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  प्रति (अचिन्हित i = 1; i + 1 < गिनती; i++)
  {
    यदि (i > 1) obj.append_c (' ');
    obj.append_अंक (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  प्रति (अचिन्हित i = 0; i + 1 < गिनती; i++)
  {
    यदि (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  प्रत्यागम res->add_object (std::move (obj));
}

स्थिर शून्य
hb_pdf_paint_linear_gradient (hb_paint_funcs_t *,
			      शून्य *paint_data,
			      hb_color_पंक्ति_t *color_पंक्ति,
			      भिन्न x0, भिन्न y0,
			      भिन्न x1, भिन्न y1,
			      भिन्न x2 HB_UNUSED, भिन्न y2 HB_UNUSED,
			      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  स्वतः *res = hb_pdf_get_resources (paint);
  यदि (unlikely (!res))
    प्रत्यागम;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  यदि (!paint->fetch_color_stops (color_पंक्ति))
    प्रत्यागम;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  भिन्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.length, &mn, &mx);
  भिन्न gx0 = x0 + mn * (x1 - x0);
  भिन्न gy0 = y0 + mn * (y1 - y0);
  भिन्न gx1 = x0 + mx * (x1 - x0);
  भिन्न gy1 = y0 + mx * (y1 - y0);

  अचिन्हित func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_पंक्ति_get_extend (color_पंक्ति);
  ध्रुव वर्ण *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_vector_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_अंक (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_अंक (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_अंक (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_अंक (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  अचिन्हित sh_idx = res->add_shading (std::move (sh));

  स्वतः &body = paint->current_body ();

  तार्किक needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  यदि (needs_alpha)
  {
    अचिन्हित alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_vector_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_अंक (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_अंक (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_अंक (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_अंक (paint->sy (gy1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    अचिन्हित alpha_sh_id = res->add_object (std::move (ash));

    अचिन्हित gs_idx = res->add_extgstate_smask (alpha_sh_id,
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

स्थिर शून्य
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      शून्य *paint_data,
			      hb_color_पंक्ति_t *color_पंक्ति,
			      भिन्न x0, भिन्न y0, भिन्न r0,
			      भिन्न x1, भिन्न y1, भिन्न r1,
			      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  स्वतः *res = hb_pdf_get_resources (paint);
  यदि (unlikely (!res))
    प्रत्यागम;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  यदि (!paint->fetch_color_stops (color_पंक्ति))
    प्रत्यागम;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  भिन्न mn, mx;
  hb_paint_normalize_color_पंक्ति (stops.arrayZ, stops.length, &mn, &mx);
  भिन्न gx0 = x0 + mn * (x1 - x0);
  भिन्न gy0 = y0 + mn * (y1 - y0);
  भिन्न gr0 = r0 + mn * (r1 - r0);
  भिन्न gx1 = x0 + mx * (x1 - x0);
  भिन्न gy1 = y0 + mx * (y1 - y0);
  भिन्न gr1 = r0 + mx * (r1 - r0);

  अचिन्हित func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_पंक्ति_get_extend (color_पंक्ति);
  ध्रुव वर्ण *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_vector_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_अंक (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_अंक (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_अंक (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_अंक (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_अंक (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_अंक (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  अचिन्हित sh_idx = res->add_shading (std::move (sh));

  स्वतः &body = paint->current_body ();

  तार्किक needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  यदि (needs_alpha)
  {
    अचिन्हित alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_vector_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_अंक (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_अंक (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_अंक (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_अंक (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_अंक (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_अंक (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    अचिन्हित alpha_sh_id = res->add_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    भिन्न cx = (gr1 >= gr0) ? gx1 : gx0;
    भिन्न cy = (gr1 >= gr0) ? gy1 : gy0;
    भिन्न rr = hb_max (gr0, gr1);
    अचिन्हित gs_idx = res->add_extgstate_smask (alpha_sh_id,
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
स्थिर शून्य
hb_pdf_encode_u16 (hb_vector_buf_t *buf, uint16_t v)
{
  वर्ण bytes[2] = {(वर्ण) (v >> 8), (वर्ण) (v & 0xFF)};
  buf->append_len (bytes, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
स्थिर शून्य
hb_pdf_encode_coord (hb_vector_buf_t *buf,
		     भिन्न val, भिन्न lo, भिन्न hi)
{
  भिन्न t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
स्थिर शून्य
hb_pdf_encode_color_rgb (hb_vector_buf_t *buf, hb_color_t c)
{
  वर्ण rgb[3] = {(वर्ण) hb_color_get_red (c),
		 (वर्ण) hb_color_get_green (c),
		 (वर्ण) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
स्थिर शून्य
hb_pdf_encode_color_alpha (hb_vector_buf_t *buf, hb_color_t c)
{
  वर्ण a = (वर्ण) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
स्थिर शून्य
hb_pdf_encode_point (hb_vector_buf_t *buf,
		     भिन्न x, भिन्न y,
		     भिन्न xlo, भिन्न xhi,
		     भिन्न ylo, भिन्न yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
स्थिर शून्य
hb_pdf_add_sweep_patch (hb_vector_buf_t *mesh,
			hb_vector_buf_t *alpha_mesh,
			भिन्न cx, भिन्न cy,
			भिन्न xlo, भिन्न xhi, भिन्न ylo, भिन्न yhi,
			भिन्न a0, hb_color_t c0_in,
			भिन्न a1, hb_color_t c1_in)
{
  ध्रुव भिन्न R = 32767.f;
  ध्रुव भिन्न eps = 0.5f;
  ध्रुव भिन्न MAX_SECTOR = (भिन्न) M_PI / 2.f;

  पूर्णाङ्क अंक_splits = (पूर्णाङ्क) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  यदि (अंक_splits < 1) अंक_splits = 1;

  प्रति (पूर्णाङ्क s = 0; s < अंक_splits; s++)
  {
    भिन्न k0 = (भिन्न) s / अंक_splits;
    भिन्न k1 = (भिन्न) (s + 1) / अंक_splits;
    भिन्न sa0 = a0 + k0 * (a1 - a0);
    भिन्न sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_in, c1_in, k0);
    hb_color_t sc1 = hb_color_lerp (c0_in, c1_in, k1);

    भिन्न da = sa1 - sa0;
    भिन्न kappa = (4.f / 3.f) * tanf (da / 4.f);

    भिन्न cos0 = cosf (sa0), sin0 = sinf (sa0);
    भिन्न cos1 = cosf (sa1), sin1 = sinf (sa1);

    भिन्न p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    भिन्न p3x = cx + R * cos0,   p3y = cy + R * sin0;
    भिन्न p6x = cx + R * cos1,   p6y = cy + R * sin1;
    भिन्न p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    भिन्न e1_1x = p0x + (p3x - p0x) / 3.f;
    भिन्न e1_1y = p0y + (p3y - p0y) / 3.f;
    भिन्न e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    भिन्न e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    भिन्न e2_1x = p3x + kappa * R * (-sin0);
    भिन्न e2_1y = p3y + kappa * R * ( cos0);
    भिन्न e2_2x = p6x - kappa * R * (-sin1);
    भिन्न e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    भिन्न e3_1x = p6x + (p9x - p6x) / 3.f;
    भिन्न e3_1y = p6y + (p9y - p6y) / 3.f;
    भिन्न e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    भिन्न e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    भिन्न e4_1x = p9x + kappa * eps * (-sin1);
    भिन्न e4_1y = p9y + kappa * eps * ( cos1);
    भिन्न e4_2x = p0x - kappa * eps * (-sin0);
    भिन्न e4_2y = p0y - kappa * eps * ( cos0);

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

    यदि (alpha_mesh)
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
संरचना hb_pdf_sweep_ctx_t {
  hb_vector_buf_t *mesh;
  hb_vector_buf_t *alpha_mesh;
  भिन्न cx, cy, xlo, xhi, ylo, yhi;
};

स्थिर शून्य
hb_pdf_sweep_emit_patch (भिन्न a0, hb_color_t c0,
			 भिन्न a1, hb_color_t c1,
			 शून्य *user_data)
{
  स्वतः *ctx = (hb_pdf_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  यदि (unlikely (ctx->mesh->in_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->in_error ())))
    प्रत्यागम;
  hb_pdf_add_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

स्थिर शून्य
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     शून्य *paint_data,
			     hb_color_पंक्ति_t *color_पंक्ति,
			     भिन्न cx, भिन्न cy,
			     भिन्न start_angle,
			     भिन्न end_angle,
			     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  स्वतः *res = hb_pdf_get_resources (paint);
  यदि (unlikely (!res))
    प्रत्यागम;

  /* Get and sort color stops. */
  यदि (!paint->fetch_color_stops (color_पंक्ति))
    प्रत्यागम;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().क्विक (
    [] (ध्रुव hb_color_stop_t &a, ध्रुव hb_color_stop_t &b)
    { प्रत्यागम (a.offset > b.offset) - (a.offset < b.offset); });

  hb_paint_extend_t extend = hb_color_पंक्ति_get_extend (color_पंक्ति);

  ध्रुव भिन्न R = 32767.f;
  भिन्न scx = paint->sx (cx), scy = paint->sy (cy);
  भिन्न xlo = scx - R - 1, xhi = scx + R + 1;
  भिन्न ylo = scy - R - 1, yhi = scy + R + 1;

  तार्किक needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_vector_buf_t mesh;
  hb_vector_buf_t alpha_mesh;
  mesh.alloc (256);
  यदि (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : nullptr,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length, extend,
				 start_angle, end_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  यदि (!mesh.length)
    प्रत्यागम;

  स्वतः hb_pdf_build_mesh_shading = [&] (hb_vector_buf_t &m,
					 ध्रुव वर्ण *cs,
					 ध्रुव वर्ण *decode_suffix) -> अचिन्हित
  {
    hb_vector_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_अंक (xlo, 2);
    sh.append_c (' ');
    sh.append_अंक (xhi, 2);
    sh.append_c (' ');
    sh.append_अंक (ylo, 2);
    sh.append_c (' ');
    sh.append_अंक (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.length);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.length);
    sh.append_str ("\nendstream");
    प्रत्यागम res->add_object (std::move (sh));
  };

  अचिन्हित sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  अचिन्हित sh_idx = res->add_shading_by_id (sh_obj_id);

  स्वतः &body = paint->current_body ();

  यदि (needs_alpha && alpha_mesh.length)
  {
    अचिन्हित alpha_sh_id = hb_pdf_build_mesh_shading (alpha_mesh, "DeviceGray",
						       " 0 1");
    अचिन्हित gs_idx = res->add_extgstate_smask (alpha_sh_id,
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

स्थिर ध्रुव वर्ण *
hb_pdf_blend_mode_name (hb_paint_composite_mode_t mode)
{
  विकल्प (mode)
  {
  पक्ष HB_PAINT_COMPOSITE_MODE_MULTIPLY:       प्रत्यागम "Multiply";
  पक्ष HB_PAINT_COMPOSITE_MODE_SCREEN:         प्रत्यागम "Screen";
  पक्ष HB_PAINT_COMPOSITE_MODE_OVERLAY:        प्रत्यागम "Overlay";
  पक्ष HB_PAINT_COMPOSITE_MODE_DARKEN:         प्रत्यागम "Darken";
  पक्ष HB_PAINT_COMPOSITE_MODE_LIGHTEN:        प्रत्यागम "Lighten";
  पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:    प्रत्यागम "ColorDodge";
  पक्ष HB_PAINT_COMPOSITE_MODE_COLOR_BURN:     प्रत्यागम "ColorBurn";
  पक्ष HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:     प्रत्यागम "HardLight";
  पक्ष HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:     प्रत्यागम "SoftLight";
  पक्ष HB_PAINT_COMPOSITE_MODE_DIFFERENCE:     प्रत्यागम "Difference";
  पक्ष HB_PAINT_COMPOSITE_MODE_EXCLUSION:      प्रत्यागम "Exclusion";
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_HUE:        प्रत्यागम "Hue";
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: प्रत्यागम "Saturation";
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_COLOR:      प्रत्यागम "Color";
  पक्ष HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: प्रत्यागम "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  पक्ष HB_PAINT_COMPOSITE_MODE_PLUS:           प्रत्यागम "Screen";
  पक्ष HB_PAINT_COMPOSITE_MODE_XOR:            प्रत्यागम "Difference";
  पक्ष HB_PAINT_COMPOSITE_MODE_CLEAR:
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC:
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST:
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OVER:
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OVER:
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_IN:
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_IN:
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_OUT:
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_OUT:
  पक्ष HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  पक्ष HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  स्वभाव:                                     प्रत्यागम nullptr; /* Normal */
  }
}

स्थिर शून्य
hb_pdf_paint_धकेलो_group (hb_paint_funcs_t *,
			 शून्य *paint_data,
			 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  paint->current_body ().append_str ("q\n");
}

स्थिर शून्य
hb_pdf_paint_धकेलो_group_for (hb_paint_funcs_t *,
			     शून्य *paint_data,
			     hb_paint_composite_mode_t mode,
			     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;

  स्वतः &body = paint->current_body ();
  body.append_str ("q\n");

  ध्रुव वर्ण *bm = hb_pdf_blend_mode_name (mode);
  यदि (bm)
  {
    स्वतः *res = hb_pdf_get_resources (paint);
    यदि (likely (res))
    {
      अचिन्हित gs_idx = res->add_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

स्थिर शून्य
hb_pdf_paint_निकालो_group (hb_paint_funcs_t *,
			शून्य *paint_data,
			hb_paint_composite_mode_t mode HB_UNUSED,
			शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  यदि (unlikely (!paint->ensure_initialized ()))
    प्रत्यागम;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

स्थिर अन्तःस्थ शून्य free_static_pdf_paint_funcs ();

स्थिर संरचना hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  स्थिर hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_धकेलो_transform_func (funcs, (hb_paint_धकेलो_transform_func_t) hb_pdf_paint_धकेलो_transform, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_transform_func (funcs, (hb_paint_निकालो_transform_func_t) hb_pdf_paint_निकालो_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_glyph_func (funcs, (hb_paint_धकेलो_clip_glyph_func_t) hb_pdf_paint_धकेलो_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_rectangle_func (funcs, (hb_paint_धकेलो_clip_rectangle_func_t) hb_pdf_paint_धकेलो_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_path_start_func (funcs, (hb_paint_धकेलो_clip_path_start_func_t) hb_pdf_paint_धकेलो_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_clip_path_end_func (funcs, (hb_paint_धकेलो_clip_path_end_func_t) hb_pdf_paint_धकेलो_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_clip_func (funcs, (hb_paint_निकालो_clip_func_t) hb_pdf_paint_निकालो_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_pdf_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_group_func (funcs, (hb_paint_धकेलो_group_func_t) hb_pdf_paint_धकेलो_group, nullptr, nullptr);
    hb_paint_funcs_set_धकेलो_group_for_func (funcs, (hb_paint_धकेलो_group_for_func_t) hb_pdf_paint_धकेलो_group_for, nullptr, nullptr);
    hb_paint_funcs_set_निकालो_group_func (funcs, (hb_paint_निकालो_group_func_t) hb_pdf_paint_निकालो_group, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    प्रत्यागम funcs;
  }
} static_pdf_paint_funcs;

स्थिर अन्तःस्थ शून्य
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_pdf_funcs_get ()
{
  प्रत्यागम static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_vector_paint_render_pdf (hb_vector_paint_t *paint)
{
  यदि (!paint->has_extents)
    प्रत्यागम nullptr;
  यदि (!paint->group_ढेर.length ||
      !paint->group_ढेर.arrayZ[0].length)
    प्रत्यागम nullptr;

  hb_vector_buf_t &content = paint->group_ढेर.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  भिन्न ex = paint->extents.x;
  भिन्न ey = paint->extents.y;
  भिन्न ew = paint->extents.width;
  भिन्न eh = paint->extents.height;

  अचिन्हित अंक_extra = res ? res->objects.length : 0;
  अचिन्हित total_objects = 4 + अंक_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  out.alloc (content.length + अंक_extra * 128 + 1024);

  hb_vector_t<अचिन्हित> offsets;
  यदि (unlikely (!offsets.resize (total_objects)))
    प्रत्यागम nullptr;

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
  out.append_अंक (ex);
  out.append_c (' ');
  out.append_अंक (ey);
  out.append_c (' ');
  out.append_अंक (ex + ew);
  out.append_c (' ');
  out.append_अंक (ey + eh);
  out.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  तार्किक has_resources = res &&
    (res->extgstate_dict.length || res->shading_dict.length || res->xobject_dict.length);
  यदि (has_resources)
  {
    out.append_str ("\n/Resources <<");
    यदि (res->extgstate_dict.length)
    {
      out.append_str (" /ExtGState << ");
      out.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.length);
      out.append_str (">>");
    }
    यदि (res->shading_dict.length)
    {
      out.append_str (" /Shading << ");
      out.append_len (res->shading_dict.arrayZ, res->shading_dict.length);
      out.append_str (">>");
    }
    यदि (res->xobject_dict.length)
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
  यदि (hb_color_get_alpha (paint->background))
  {
    भिन्न r = hb_color_get_red (paint->background) / 255.f;
    भिन्न g = hb_color_get_green (paint->background) / 255.f;
    भिन्न b = hb_color_get_blue (paint->background) / 255.f;
    भिन्न a = hb_color_get_alpha (paint->background) / 255.f;
    यदि (a < 1.f - 1.f / 512.f)
    {
      यदि (res)
      {
	अचिन्हित gs_idx = res->add_extgstate_alpha (a);
	bg_prefix.append_str ("/GS");
	bg_prefix.append_unsigned (gs_idx);
	bg_prefix.append_str (" gs\n");
      }
    }
    bg_prefix.append_अंक (r, 4);
    bg_prefix.append_c (' ');
    bg_prefix.append_अंक (g, 4);
    bg_prefix.append_c (' ');
    bg_prefix.append_अंक (b, 4);
    bg_prefix.append_str (" rg\n");
    bg_prefix.append_अंक (ex);
    bg_prefix.append_c (' ');
    bg_prefix.append_अंक (ey);
    bg_prefix.append_c (' ');
    bg_prefix.append_अंक (ew);
    bg_prefix.append_c (' ');
    bg_prefix.append_अंक (eh);
    bg_prefix.append_str (" re f\n");
  }
  अचिन्हित stream_len = bg_prefix.length + content.length;

  /* Object 4: Content stream */
  offsets.arrayZ[3] = out.length;
  out.append_str ("4 0 obj\n<< /Length ");
  out.append_unsigned (stream_len);
  out.append_str (" >>\nstream\n");
  out.append_len (bg_prefix.arrayZ, bg_prefix.length);
  out.append_len (content.arrayZ, content.length);
  out.append_str ("endstream\nendobj\n");

  /* Extra objects (functions, shadings, ExtGState). */
  प्रति (अचिन्हित i = 0; i < अंक_extra; i++)
  {
    offsets.arrayZ[4 + i] = out.length;
    out.append_unsigned (5 + i);
    out.append_str (" 0 obj\n");
    स्वतः &obj = res->objects.arrayZ[i];
    out.append_len (obj.data.arrayZ, obj.data.length);
    out.append_str ("\nendobj\n");
  }

  /* Cross-reference table */
  अचिन्हित xref_offset = out.length;
  out.append_str ("xref\n0 ");
  out.append_unsigned (total_objects + 1);
  out.append_str ("\n0000000000 65535 f \n");
  प्रति (अचिन्हित i = 0; i < total_objects; i++)
  {
    वर्ण tmp[21];
    snprintf (tmp, परिमाण (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    out.append_len (tmp, 20);
  }

  /* Trailer */
  out.append_str ("trailer\n<< /Size ");
  out.append_unsigned (total_objects + 1);
  out.append_str (" /Root 1 0 R >>\nstartxref\n");
  out.append_unsigned (xref_offset);
  out.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  प्रत्यागम blob;
}

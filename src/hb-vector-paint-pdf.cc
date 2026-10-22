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
			    स्थिरांक अक्षर *idat_data, अचिन्हित idat_len,
			    अचिन्हित width, अचिन्हित height,
			    स्थिरांक uint8_t *trns, अचिन्हित trns_len);

संरचना hb_pdf_resources_t
{
  hb_vector_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_vector_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_vector_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_vector_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  अचिन्हित extgstate_मोजणी = 0;
  अचिन्हित shading_मोजणी = 0;
  अचिन्हित xobject_मोजणी = 0;

  अचिन्हित add_object (hb_vector_buf_t &&obj_data)
  {
    अचिन्हित id = 5 + objects.length; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.data = std::move (obj_data);
    objects.push (std::move (obj));
    परत id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  अचिन्हित add_extgstate_alpha (दशांश alpha)
  {
    अचिन्हित idx = extgstate_मोजणी++;
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
    परत idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  अचिन्हित add_extgstate_blend (स्थिरांक अक्षर *bm)
  {
    अचिन्हित idx = extgstate_मोजणी++;
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
    परत idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  अचिन्हित add_extgstate_smask (अचिन्हित alpha_shading_id,
				दशांश bbox_x, दशांश bbox_y,
				दशांश bbox_w, दशांश bbox_h,
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
    अचिन्हित idx = extgstate_मोजणी++;
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
    परत idx;
  }

  /* Add a shading, return resource name index. */
  अचिन्हित add_shading (hb_vector_buf_t &&shading_data)
  {
    अचिन्हित obj_id = add_object (std::move (shading_data));
    परत add_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  अचिन्हित add_shading_by_id (अचिन्हित obj_id)
  {
    अचिन्हित idx = shading_मोजणी++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    परत idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  अचिन्हित add_xobject_png_image (स्थिरांक अक्षर *idat_data, अचिन्हित idat_len,
				  अचिन्हित width, अचिन्हित height,
				  अचिन्हित colors, तार्किक has_alpha,
				  स्थिरांक uint8_t *plte = nullptr,
				  अचिन्हित plte_len = 0,
				  स्थिरांक uint8_t *trns = nullptr,
				  अचिन्हित trns_len = 0)
  {
    अचिन्हित idx = xobject_मोजणी++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (width);
    obj.append_str (" /Height ");
    obj.append_unsigned (height);
    obj.append_str ("\n/BitsPerComponent 8\n");

    जर (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      अचिन्हित n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      साठी (अचिन्हित i = 0; i < n_entries * 3; i++)
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
    जर (plte && trns && trns_len)
    {
      hb_vector_buf_t smask_stream;
      जर (hb_pdf_build_indexed_smask (&smask_stream, idat_data, idat_len,
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

    जर (smask_id)
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
    परत idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
स्थिर hb_pdf_resources_t *
hb_pdf_get_resources (hb_vector_paint_t *paint)
{
  जर (!paint->defs.length)
  {
    /* First call: allocate and store. */
    स्वतः *res = (hb_pdf_resources_t *) hb_calloc (1, आकार (hb_pdf_resources_t));
    जर (unlikely (!res)) परत nullptr;
    नवीन (res) hb_pdf_resources_t ();
    जर (unlikely (!paint->defs.resize (आकार (शून्य *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      परत nullptr;
    }
    स_नकल (paint->defs.arrayZ, &res, आकार (शून्य *));
  }
  hb_pdf_resources_t *res;
  स_नकल (&res, paint->defs.arrayZ, आकार (शून्य *));
  परत res;
}

शून्य
hb_vector_paint_pdf_free_resources (hb_vector_paint_t *paint)
{
  जर (paint->defs.length >= आकार (शून्य *))
  {
    hb_pdf_resources_t *res;
    स_नकल (&res, paint->defs.arrayZ, आकार (शून्य *));
    जर (res)
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
  जर (likely (paint->budget_remaining >= 0))
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
hb_pdf_paint_ढकला_transform (hb_paint_funcs_t *,
			     शून्य *paint_data,
			     दशांश xx, दशांश yx,
			     दशांश xy, दशांश yy,
			     दशांश dx, दशांश dy,
			     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;

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
hb_pdf_paint_काढा_transform (hb_paint_funcs_t *,
			    शून्य *paint_data,
			    शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
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
  जर (unlikely (!paint->ensure_initialized ()))
    परत;

  दशांश r = hb_color_get_red (c) / 255.f;
  दशांश g = hb_color_get_green (c) / 255.f;
  दशांश b = hb_color_get_blue (c) / 255.f;
  दशांश a = hb_color_get_alpha (c) / 255.f;

  जर (a < 1.f / 255.f)
    परत;

  स्वतः &body = paint->current_body ();

  तार्किक scoped = असत्य;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  जर (a < 1.f - 1.f / 512.f)
  {
    स्वतः *res = hb_pdf_get_resources (paint);
    जर (res)
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
  जर (scoped)
    body.append_str ("Q\n");
}

स्थिर शून्य
hb_pdf_paint_ढकला_clip_glyph (hb_paint_funcs_t *,
			      शून्य *paint_data,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;

  स्वतः &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

स्थिर शून्य
hb_pdf_paint_ढकला_clip_rectangle (hb_paint_funcs_t *,
				  शून्य *paint_data,
				  दशांश xmin, दशांश ymin,
				  दशांश xmax, दशांश ymax,
				  शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;

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
hb_pdf_paint_ढकला_clip_path_start (hb_paint_funcs_t *,
				   शून्य *paint_data,
				   शून्य **draw_data,
				   शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    परत nullptr;
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
  परत hb_vector_pdf_path_draw_funcs_get ();
}

स्थिर शून्य
hb_pdf_paint_ढकला_clip_path_end (hb_paint_funcs_t *,
				 शून्य *paint_data,
				 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  paint->current_body ().append_str ("W n\n");
}

स्थिर शून्य
hb_pdf_paint_काढा_clip (hb_paint_funcs_t *,
		       शून्य *paint_data,
		       शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
स्थिर शून्य
hb_pdf_paint_solid_color (hb_vector_paint_t *paint, hb_color_t c)
{
  स्वतः &body = paint->current_body ();

  दशांश r = hb_color_get_red (c) / 255.f;
  दशांश g = hb_color_get_green (c) / 255.f;
  दशांश b = hb_color_get_blue (c) / 255.f;
  दशांश a = hb_color_get_alpha (c) / 255.f;

  जर (a < 1.f / 255.f)
    परत;

  /* Set alpha via ExtGState if needed. */
  जर (a < 1.f - 1.f / 512.f)
  {
    स्वतः *res = hb_pdf_get_resources (paint);
    जर (res)
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
  जर (unlikely (!paint->ensure_initialized ()))
    परत;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
स्थिर तार्किक
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    स्थिरांक अक्षर *idat_data, अचिन्हित idat_len,
			    अचिन्हित width, अचिन्हित height,
			    स्थिरांक uint8_t *trns, अचिन्हित trns_len)
{
#ifndef HAVE_ZLIB
  (शून्य) out; (शून्य) idat_data; (शून्य) idat_len;
  (शून्य) width; (शून्य) height; (शून्य) trns; (शून्य) trns_len;
  परत असत्य;
#अन्यथा
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  अचिन्हित raw_len, mask_len;
  जर (!width || !height ||
      width == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (width + 1, height, &raw_len) ||
      hb_unsigned_mul_overflows (width, height, &mask_len))
    परत असत्य;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  जर (!raw) परत असत्य;
  HB_SCOPE_GUARD (hb_free (raw));

  z_stream stream = {};
  stream.next_in = (Bytef *) idat_data;
  stream.avail_in = idat_len;
  stream.next_out = (Bytef *) raw;
  stream.avail_out = raw_len;

  जर (inflateInit (&stream) != Z_OK)
    परत असत्य;
  पूर्णांक status = inflate (&stream, Z_FINISH);
  अचिन्हित दीर्घ total_out = stream.total_out;
  inflateEnd (&stream);
  जर (status != Z_STREAM_END || total_out != raw_len)
    परत असत्य;

  /* Un-filter and map to alpha. */
  जर (!out->resize (mask_len))
    परत असत्य;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (width);
  जर (!unfiltered)
    परत असत्य;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (width, 1);
  जर (!prev_unfiltered)
    परत असत्य;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  साठी (अचिन्हित y = 0; y < height; y++)
  {
    uint8_t *row = raw + y * (width + 1);
    uint8_t filter = row[0];
    uint8_t *pixels = row + 1;

    साठी (अचिन्हित x = 0; x < width; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      निवड (filter)
      {
      स्थिती 0: unfiltered[x] = val; थांबा; /* None */
      स्थिती 1: unfiltered[x] = val + a; थांबा; /* Sub */
      स्थिती 2: unfiltered[x] = val + b; थांबा; /* Up */
      स्थिती 3: unfiltered[x] = val + (uint8_t) (((अचिन्हित) a + b) / 2); थांबा; /* Average */
      स्थिती 4: /* Paeth */
      {
	पूर्णांक p = (पूर्णांक) a + (पूर्णांक) b - (पूर्णांक) c;
	पूर्णांक pa = असल (p - (पूर्णांक) a);
	पूर्णांक pb = असल (p - (पूर्णांक) b);
	पूर्णांक pc = असल (p - (पूर्णांक) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	थांबा;
      }
      पूर्वनिर्धारित: unfiltered[x] = val; थांबा;
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

  परत सत्य;
#endif
}

/* Read a big-endian uint32 from a byte pointer. */
स्थिर ओळीत uint32_t
hb_pdf_png_u32 (स्थिरांक uint8_t *p)
{
  परत ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

स्थिर hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    शून्य *paint_data,
		    hb_blob_t *image,
		    अचिन्हित width,
		    अचिन्हित height,
		    hb_tag_t format,
		    दशांश slant HB_UNUSED,
		    hb_glyph_extents_t *extents,
		    शून्य *)
{
  जर (format != HB_TAG ('p','n','g',' '))
    परत असत्य;
  जर (!extents || !width || !height)
    परत असत्य;

  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत असत्य;
  स्वतः *res = hb_pdf_get_resources (paint);
  जर (unlikely (!res))
    परत असत्य;

  अचिन्हित len = 0;
  स्थिरांक uint8_t *data = (स्थिरांक uint8_t *) hb_blob_get_data (image, &len);
  जर (!data || len < 8)
    परत असत्य;

  /* Verify PNG signature. */
  स्थिर स्थिरांक uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  जर (hb_memcmp (data, png_sig, 8) != 0)
    परत असत्य;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  अचिन्हित png_w = 0, png_h = 0;
  अचिन्हित colors = 3; /* default RGB */
  uint8_t color_type = 2;
  तार्किक has_alpha = असत्य;
  स्थिरांक uint8_t *plte_data = nullptr;
  अचिन्हित plte_len = 0;
  स्थिरांक uint8_t *trns_data = nullptr;
  अचिन्हित trns_len = 0;
  hb_vector_buf_t idat;

  अचिन्हित pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  पर्यंत (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (data + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (data + pos + 4);
    जर (chunk_len > len - pos - 12)
      थांबा;
    स्थिरांक uint8_t *chunk_data = data + pos + 8;

    जर (chunk_type == 0x49484452u) /* IHDR */
    {
      जर (chunk_len < 13) परत असत्य;
      png_w = hb_pdf_png_u32 (chunk_data);
      png_h = hb_pdf_png_u32 (chunk_data + 4);
      uint8_t bit_depth = chunk_data[8];
      color_type = chunk_data[9];
      जर (bit_depth != 8) परत असत्य; /* only 8-bit supported */

      निवड (color_type)
      {
      स्थिती 0: colors = 1; has_alpha = असत्य; थांबा; /* Grayscale */
      स्थिती 2: colors = 3; has_alpha = असत्य; थांबा; /* RGB */
      स्थिती 3: colors = 1; has_alpha = असत्य; थांबा; /* Indexed */
      स्थिती 4: colors = 2; has_alpha = सत्य;  थांबा; /* Gray+Alpha */
      स्थिती 6: colors = 4; has_alpha = सत्य;  थांबा; /* RGBA */
      पूर्वनिर्धारित: परत असत्य;
      }
    }
    अन्यथा जर (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_data = chunk_data;
      plte_len = chunk_len;
    }
    अन्यथा जर (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_data = chunk_data;
      trns_len = chunk_len;
    }
    अन्यथा जर (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((स्थिरांक अक्षर *) chunk_data, chunk_len);
    }
    अन्यथा जर (chunk_type == 0x49454E44u) /* IEND */
      थांबा;

    pos += 12 + chunk_len;
  }

  जर (!png_w || !png_h || !idat.length)
    परत असत्य;

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
  दशांश ix = (दशांश) extents->x_bearing;
  दशांश iy = (दशांश) extents->y_bearing + (दशांश) extents->height;
  दशांश iw = (दशांश) extents->width;
  दशांश ih = -(दशांश) extents->height; /* negative because image Y goes up but height is negative in extents */

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

  परत सत्य;
}

/* ---- Gradient helpers ---- */

स्थिर तार्किक
hb_pdf_gradient_needs_alpha (hb_array_t<स्थिरांक hb_color_stop_t> stops)
{
  साठी (स्थिरांक स्वतः &s : stops)
    जर (hb_color_get_alpha (s.color) != 255)
      परत सत्य;
  परत असत्य;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
स्थिर शून्य
hb_pdf_build_alpha_interpolation_function (hb_vector_buf_t *obj,
					   दशांश a0, दशांश a1)
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
  अचिन्हित मोजणी = paint->color_stops_scratch.length;

  जर (मोजणी < 2)
  {
    दशांश a = मोजणी ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    परत res->add_object (std::move (obj));
  }

  जर (मोजणी == 2)
  {
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    परत res->add_object (std::move (obj));
  }

  hb_vector_t<अचिन्हित> sub_func_ids;
  साठी (अचिन्हित i = 0; i + 1 < मोजणी; i++)
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
  साठी (अचिन्हित i = 0; i < sub_func_ids.length; i++)
  {
    जर (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  साठी (अचिन्हित i = 1; i + 1 < मोजणी; i++)
  {
    जर (i > 1) obj.append_c (' ');
    obj.append_अंक (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n/Encode [");
  साठी (अचिन्हित i = 0; i + 1 < मोजणी; i++)
  {
    जर (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  परत res->add_object (std::move (obj));
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
  अचिन्हित मोजणी = paint->color_stops_scratch.length;

  जर (मोजणी < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = मोजणी ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    परत res->add_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().क्विक (
    [] (स्थिरांक hb_color_stop_t &a, स्थिरांक hb_color_stop_t &b)
    { परत (a.offset > b.offset) - (a.offset < b.offset); });

  जर (मोजणी == 2)
  {
    /* Two stops: single interpolation function. */
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    परत res->add_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_vector_t<अचिन्हित> sub_func_ids;
  साठी (अचिन्हित i = 0; i + 1 < मोजणी; i++)
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
  साठी (अचिन्हित i = 0; i < sub_func_ids.length; i++)
  {
    जर (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  साठी (अचिन्हित i = 1; i + 1 < मोजणी; i++)
  {
    जर (i > 1) obj.append_c (' ');
    obj.append_अंक (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  साठी (अचिन्हित i = 0; i + 1 < मोजणी; i++)
  {
    जर (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  परत res->add_object (std::move (obj));
}

स्थिर शून्य
hb_pdf_paint_linear_gradient (hb_paint_funcs_t *,
			      शून्य *paint_data,
			      hb_color_ओळ_t *color_ओळ,
			      दशांश x0, दशांश y0,
			      दशांश x1, दशांश y1,
			      दशांश x2 HB_UNUSED, दशांश y2 HB_UNUSED,
			      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  स्वतः *res = hb_pdf_get_resources (paint);
  जर (unlikely (!res))
    परत;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  जर (!paint->fetch_color_stops (color_ओळ))
    परत;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  दशांश mn, mx;
  hb_paint_normalize_color_ओळ (stops.arrayZ, stops.length, &mn, &mx);
  दशांश gx0 = x0 + mn * (x1 - x0);
  दशांश gy0 = y0 + mn * (y1 - y0);
  दशांश gx1 = x0 + mx * (x1 - x0);
  दशांश gy1 = y0 + mx * (y1 - y0);

  अचिन्हित func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_ओळ_get_extend (color_ओळ);
  स्थिरांक अक्षर *extend_str = (extend == HB_PAINT_EXTEND_PAD)
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
  जर (needs_alpha)
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
			      hb_color_ओळ_t *color_ओळ,
			      दशांश x0, दशांश y0, दशांश r0,
			      दशांश x1, दशांश y1, दशांश r1,
			      शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  स्वतः *res = hb_pdf_get_resources (paint);
  जर (unlikely (!res))
    परत;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  जर (!paint->fetch_color_stops (color_ओळ))
    परत;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  दशांश mn, mx;
  hb_paint_normalize_color_ओळ (stops.arrayZ, stops.length, &mn, &mx);
  दशांश gx0 = x0 + mn * (x1 - x0);
  दशांश gy0 = y0 + mn * (y1 - y0);
  दशांश gr0 = r0 + mn * (r1 - r0);
  दशांश gx1 = x0 + mx * (x1 - x0);
  दशांश gy1 = y0 + mx * (y1 - y0);
  दशांश gr1 = r0 + mx * (r1 - r0);

  अचिन्हित func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_ओळ_get_extend (color_ओळ);
  स्थिरांक अक्षर *extend_str = (extend == HB_PAINT_EXTEND_PAD)
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
  जर (needs_alpha)
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
    दशांश cx = (gr1 >= gr0) ? gx1 : gx0;
    दशांश cy = (gr1 >= gr0) ? gy1 : gy0;
    दशांश rr = hb_max (gr0, gr1);
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
  अक्षर bytes[2] = {(अक्षर) (v >> 8), (अक्षर) (v & 0xFF)};
  buf->append_len (bytes, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
स्थिर शून्य
hb_pdf_encode_coord (hb_vector_buf_t *buf,
		     दशांश val, दशांश lo, दशांश hi)
{
  दशांश t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
स्थिर शून्य
hb_pdf_encode_color_rgb (hb_vector_buf_t *buf, hb_color_t c)
{
  अक्षर rgb[3] = {(अक्षर) hb_color_get_red (c),
		 (अक्षर) hb_color_get_green (c),
		 (अक्षर) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
स्थिर शून्य
hb_pdf_encode_color_alpha (hb_vector_buf_t *buf, hb_color_t c)
{
  अक्षर a = (अक्षर) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
स्थिर शून्य
hb_pdf_encode_point (hb_vector_buf_t *buf,
		     दशांश x, दशांश y,
		     दशांश xlo, दशांश xhi,
		     दशांश ylo, दशांश yhi)
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
			दशांश cx, दशांश cy,
			दशांश xlo, दशांश xhi, दशांश ylo, दशांश yhi,
			दशांश a0, hb_color_t c0_in,
			दशांश a1, hb_color_t c1_in)
{
  स्थिरांक दशांश R = 32767.f;
  स्थिरांक दशांश eps = 0.5f;
  स्थिरांक दशांश MAX_SECTOR = (दशांश) M_PI / 2.f;

  पूर्णांक अंक_splits = (पूर्णांक) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  जर (अंक_splits < 1) अंक_splits = 1;

  साठी (पूर्णांक s = 0; s < अंक_splits; s++)
  {
    दशांश k0 = (दशांश) s / अंक_splits;
    दशांश k1 = (दशांश) (s + 1) / अंक_splits;
    दशांश sa0 = a0 + k0 * (a1 - a0);
    दशांश sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_in, c1_in, k0);
    hb_color_t sc1 = hb_color_lerp (c0_in, c1_in, k1);

    दशांश da = sa1 - sa0;
    दशांश kappa = (4.f / 3.f) * tanf (da / 4.f);

    दशांश cos0 = cosf (sa0), sin0 = sinf (sa0);
    दशांश cos1 = cosf (sa1), sin1 = sinf (sa1);

    दशांश p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    दशांश p3x = cx + R * cos0,   p3y = cy + R * sin0;
    दशांश p6x = cx + R * cos1,   p6y = cy + R * sin1;
    दशांश p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    दशांश e1_1x = p0x + (p3x - p0x) / 3.f;
    दशांश e1_1y = p0y + (p3y - p0y) / 3.f;
    दशांश e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    दशांश e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    दशांश e2_1x = p3x + kappa * R * (-sin0);
    दशांश e2_1y = p3y + kappa * R * ( cos0);
    दशांश e2_2x = p6x - kappa * R * (-sin1);
    दशांश e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    दशांश e3_1x = p6x + (p9x - p6x) / 3.f;
    दशांश e3_1y = p6y + (p9y - p6y) / 3.f;
    दशांश e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    दशांश e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    दशांश e4_1x = p9x + kappa * eps * (-sin1);
    दशांश e4_1y = p9y + kappa * eps * ( cos1);
    दशांश e4_2x = p0x - kappa * eps * (-sin0);
    दशांश e4_2y = p0y - kappa * eps * ( cos0);

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

    जर (alpha_mesh)
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
  दशांश cx, cy, xlo, xhi, ylo, yhi;
};

स्थिर शून्य
hb_pdf_sweep_emit_patch (दशांश a0, hb_color_t c0,
			 दशांश a1, hb_color_t c1,
			 शून्य *user_data)
{
  स्वतः *ctx = (hb_pdf_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  जर (unlikely (ctx->mesh->in_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->in_error ())))
    परत;
  hb_pdf_add_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

स्थिर शून्य
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     शून्य *paint_data,
			     hb_color_ओळ_t *color_ओळ,
			     दशांश cx, दशांश cy,
			     दशांश start_angle,
			     दशांश end_angle,
			     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  स्वतः *res = hb_pdf_get_resources (paint);
  जर (unlikely (!res))
    परत;

  /* Get and sort color stops. */
  जर (!paint->fetch_color_stops (color_ओळ))
    परत;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().क्विक (
    [] (स्थिरांक hb_color_stop_t &a, स्थिरांक hb_color_stop_t &b)
    { परत (a.offset > b.offset) - (a.offset < b.offset); });

  hb_paint_extend_t extend = hb_color_ओळ_get_extend (color_ओळ);

  स्थिरांक दशांश R = 32767.f;
  दशांश scx = paint->sx (cx), scy = paint->sy (cy);
  दशांश xlo = scx - R - 1, xhi = scx + R + 1;
  दशांश ylo = scy - R - 1, yhi = scy + R + 1;

  तार्किक needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_vector_buf_t mesh;
  hb_vector_buf_t alpha_mesh;
  mesh.alloc (256);
  जर (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : nullptr,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length, extend,
				 start_angle, end_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  जर (!mesh.length)
    परत;

  स्वतः hb_pdf_build_mesh_shading = [&] (hb_vector_buf_t &m,
					 स्थिरांक अक्षर *cs,
					 स्थिरांक अक्षर *decode_suffix) -> अचिन्हित
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
    परत res->add_object (std::move (sh));
  };

  अचिन्हित sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  अचिन्हित sh_idx = res->add_shading_by_id (sh_obj_id);

  स्वतः &body = paint->current_body ();

  जर (needs_alpha && alpha_mesh.length)
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

स्थिर स्थिरांक अक्षर *
hb_pdf_blend_mode_name (hb_paint_composite_mode_t mode)
{
  निवड (mode)
  {
  स्थिती HB_PAINT_COMPOSITE_MODE_MULTIPLY:       परत "Multiply";
  स्थिती HB_PAINT_COMPOSITE_MODE_SCREEN:         परत "Screen";
  स्थिती HB_PAINT_COMPOSITE_MODE_OVERLAY:        परत "Overlay";
  स्थिती HB_PAINT_COMPOSITE_MODE_DARKEN:         परत "Darken";
  स्थिती HB_PAINT_COMPOSITE_MODE_LIGHTEN:        परत "Lighten";
  स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:    परत "ColorDodge";
  स्थिती HB_PAINT_COMPOSITE_MODE_COLOR_BURN:     परत "ColorBurn";
  स्थिती HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:     परत "HardLight";
  स्थिती HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:     परत "SoftLight";
  स्थिती HB_PAINT_COMPOSITE_MODE_DIFFERENCE:     परत "Difference";
  स्थिती HB_PAINT_COMPOSITE_MODE_EXCLUSION:      परत "Exclusion";
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_HUE:        परत "Hue";
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: परत "Saturation";
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_COLOR:      परत "Color";
  स्थिती HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: परत "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  स्थिती HB_PAINT_COMPOSITE_MODE_PLUS:           परत "Screen";
  स्थिती HB_PAINT_COMPOSITE_MODE_XOR:            परत "Difference";
  स्थिती HB_PAINT_COMPOSITE_MODE_CLEAR:
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC:
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST:
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OVER:
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OVER:
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_IN:
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_IN:
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_OUT:
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_OUT:
  स्थिती HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  स्थिती HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  पूर्वनिर्धारित:                                     परत nullptr; /* Normal */
  }
}

स्थिर शून्य
hb_pdf_paint_ढकला_group (hb_paint_funcs_t *,
			 शून्य *paint_data,
			 शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  paint->current_body ().append_str ("q\n");
}

स्थिर शून्य
hb_pdf_paint_ढकला_group_for (hb_paint_funcs_t *,
			     शून्य *paint_data,
			     hb_paint_composite_mode_t mode,
			     शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;

  स्वतः &body = paint->current_body ();
  body.append_str ("q\n");

  स्थिरांक अक्षर *bm = hb_pdf_blend_mode_name (mode);
  जर (bm)
  {
    स्वतः *res = hb_pdf_get_resources (paint);
    जर (likely (res))
    {
      अचिन्हित gs_idx = res->add_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

स्थिर शून्य
hb_pdf_paint_काढा_group (hb_paint_funcs_t *,
			शून्य *paint_data,
			hb_paint_composite_mode_t mode HB_UNUSED,
			शून्य *)
{
  स्वतः *paint = (hb_vector_paint_t *) paint_data;
  जर (unlikely (!paint->ensure_initialized ()))
    परत;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

स्थिर ओळीत शून्य free_static_pdf_paint_funcs ();

स्थिर संरचना hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  स्थिर hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_ढकला_transform_func (funcs, (hb_paint_ढकला_transform_func_t) hb_pdf_paint_ढकला_transform, nullptr, nullptr);
    hb_paint_funcs_set_काढा_transform_func (funcs, (hb_paint_काढा_transform_func_t) hb_pdf_paint_काढा_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ढकला_clip_glyph_func (funcs, (hb_paint_ढकला_clip_glyph_func_t) hb_pdf_paint_ढकला_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ढकला_clip_rectangle_func (funcs, (hb_paint_ढकला_clip_rectangle_func_t) hb_pdf_paint_ढकला_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ढकला_clip_path_start_func (funcs, (hb_paint_ढकला_clip_path_start_func_t) hb_pdf_paint_ढकला_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ढकला_clip_path_end_func (funcs, (hb_paint_ढकला_clip_path_end_func_t) hb_pdf_paint_ढकला_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_काढा_clip_func (funcs, (hb_paint_काढा_clip_func_t) hb_pdf_paint_काढा_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_pdf_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_ढकला_group_func (funcs, (hb_paint_ढकला_group_func_t) hb_pdf_paint_ढकला_group, nullptr, nullptr);
    hb_paint_funcs_set_ढकला_group_for_func (funcs, (hb_paint_ढकला_group_for_func_t) hb_pdf_paint_ढकला_group_for, nullptr, nullptr);
    hb_paint_funcs_set_काढा_group_func (funcs, (hb_paint_काढा_group_func_t) hb_pdf_paint_काढा_group, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    परत funcs;
  }
} static_pdf_paint_funcs;

स्थिर ओळीत शून्य
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_pdf_funcs_get ()
{
  परत static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_vector_paint_render_pdf (hb_vector_paint_t *paint)
{
  जर (!paint->has_extents)
    परत nullptr;
  जर (!paint->group_चळत.length ||
      !paint->group_चळत.arrayZ[0].length)
    परत nullptr;

  hb_vector_buf_t &content = paint->group_चळत.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  दशांश ex = paint->extents.x;
  दशांश ey = paint->extents.y;
  दशांश ew = paint->extents.width;
  दशांश eh = paint->extents.height;

  अचिन्हित अंक_extra = res ? res->objects.length : 0;
  अचिन्हित total_objects = 4 + अंक_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  out.alloc (content.length + अंक_extra * 128 + 1024);

  hb_vector_t<अचिन्हित> offsets;
  जर (unlikely (!offsets.resize (total_objects)))
    परत nullptr;

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
  जर (has_resources)
  {
    out.append_str ("\n/Resources <<");
    जर (res->extgstate_dict.length)
    {
      out.append_str (" /ExtGState << ");
      out.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.length);
      out.append_str (">>");
    }
    जर (res->shading_dict.length)
    {
      out.append_str (" /Shading << ");
      out.append_len (res->shading_dict.arrayZ, res->shading_dict.length);
      out.append_str (">>");
    }
    जर (res->xobject_dict.length)
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
  जर (hb_color_get_alpha (paint->background))
  {
    दशांश r = hb_color_get_red (paint->background) / 255.f;
    दशांश g = hb_color_get_green (paint->background) / 255.f;
    दशांश b = hb_color_get_blue (paint->background) / 255.f;
    दशांश a = hb_color_get_alpha (paint->background) / 255.f;
    जर (a < 1.f - 1.f / 512.f)
    {
      जर (res)
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
  साठी (अचिन्हित i = 0; i < अंक_extra; i++)
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
  साठी (अचिन्हित i = 0; i < total_objects; i++)
  {
    अक्षर tmp[21];
    snprintf (tmp, आकार (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    out.append_len (tmp, 20);
  }

  /* Trailer */
  out.append_str ("trailer\n<< /Size ");
  out.append_unsigned (total_objects + 1);
  out.append_str (" /Root 1 0 R >>\nstartxref\n");
  out.append_unsigned (xref_offset);
  out.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  परत blob;
}

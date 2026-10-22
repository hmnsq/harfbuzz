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

#অন্তর্ভাব "hb.hh"

#অন্তর্ভাব "hb-vector-paint.hh"
#অন্তর্ভাব "hb-vector-draw.hh"
#অন্তর্ভাব "hb-paint.hh"

#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdio.h>

#ifdef HAVE_ZLIB
#অন্তর্ভাব <zlib.h>
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

সংরচনা hb_pdf_obj_t
{
  hb_vector_buf_t data;
};

/* Collects extra PDF objects (shadings, functions, ExtGState)
 * during painting.  Referenced from the content stream by name
 * (e.g. /SH0, /GS0) and emitted at render time. */
স্থির তার্কিক
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    ধ্রুব বর্ণ *idat_data, unsigned idat_len,
			    unsigned width, unsigned height,
			    ধ্রুব uint8_t *trns, unsigned trns_len);

সংরচনা hb_pdf_resources_t
{
  hb_vector_t<hb_pdf_obj_t> objects;   /* extra objects, starting at id 5 */
  hb_vector_buf_t extgstate_dict;    /* /GS0 5 0 R /GS1 6 0 R ... */
  hb_vector_buf_t shading_dict;      /* /SH0 7 0 R ... */
  hb_vector_buf_t xobject_dict;      /* /Im0 8 0 R ... */
  unsigned extgstate_গুনতি = 0;
  unsigned shading_গুনতি = 0;
  unsigned xobject_গুনতি = 0;

  unsigned add_object (hb_vector_buf_t &&obj_data)
  {
    unsigned id = 5 + objects.length; /* objects 1-4 are fixed */
    hb_pdf_obj_t obj;
    obj.data = std::move (obj_data);
    objects.push (std::move (obj));
    প্রত্যাগম id;
  }

  /* Add ExtGState for fill opacity, return resource name index. */
  unsigned add_extgstate_alpha (ভিন্ন alpha)
  {
    unsigned idx = extgstate_গুনতি++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /ExtGState /ca ");
    obj.append_অঙ্ক (alpha, 4);
    obj.append_str (" >>");
    unsigned obj_id = add_object (std::move (obj));

    extgstate_dict.append_str ("/GS");
    extgstate_dict.append_unsigned (idx);
    extgstate_dict.append_c (' ');
    extgstate_dict.append_unsigned (obj_id);
    extgstate_dict.append_str (" 0 R ");
    প্রত্যাগম idx;
  }

  /* Add ExtGState for blend mode, return resource name index. */
  unsigned add_extgstate_blend (ধ্রুব বর্ণ *bm)
  {
    unsigned idx = extgstate_গুনতি++;
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
    প্রত্যাগম idx;
  }

  /* Add ExtGState with an SMask (soft mask) referencing a Form XObject
   * that paints a DeviceGray shading.  Returns the ExtGState resource index. */
  unsigned add_extgstate_smask (unsigned alpha_shading_id,
				ভিন্ন bbox_x, ভিন্ন bbox_y,
				ভিন্ন bbox_w, ভিন্ন bbox_h,
				unsigned precision)
  {
    /* Form XObject: transparency group painting the alpha shading. */
    hb_vector_buf_t form_stream;
    form_stream.append_str ("/SHa sh\n");

    hb_vector_buf_t form;
    form.append_str ("<< /Type /XObject /Subtype /Form\n");
    form.append_str ("/BBox [");
    form.append_অঙ্ক (bbox_x, precision);
    form.append_c (' ');
    form.append_অঙ্ক (bbox_y, precision);
    form.append_c (' ');
    form.append_অঙ্ক (bbox_x + bbox_w, precision);
    form.append_c (' ');
    form.append_অঙ্ক (bbox_y + bbox_h, precision);
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
    unsigned idx = extgstate_গুনতি++;
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
    প্রত্যাগম idx;
  }

  /* Add a shading, return resource name index. */
  unsigned add_shading (hb_vector_buf_t &&shading_data)
  {
    unsigned obj_id = add_object (std::move (shading_data));
    প্রত্যাগম add_shading_by_id (obj_id);
  }

  /* Register an already-allocated object as a shading resource. */
  unsigned add_shading_by_id (unsigned obj_id)
  {
    unsigned idx = shading_গুনতি++;
    shading_dict.append_str ("/SH");
    shading_dict.append_unsigned (idx);
    shading_dict.append_c (' ');
    shading_dict.append_unsigned (obj_id);
    shading_dict.append_str (" 0 R ");
    প্রত্যাগম idx;
  }

  /* Add an XObject image, return resource name index.
   * idat_data/idat_len is the concatenated PNG IDAT payload (zlib data).
   * colors is 1 (gray), 3 (RGB), or 4 (RGBA); for indexed, pass colors=1.
   * plte/plte_len is the PLTE chunk data for indexed images (may be null). */
  unsigned add_xobject_png_image (ধ্রুব বর্ণ *idat_data, unsigned idat_len,
				  unsigned width, unsigned height,
				  unsigned colors, তার্কিক has_alpha,
				  ধ্রুব uint8_t *plte = nullptr,
				  unsigned plte_len = 0,
				  ধ্রুব uint8_t *trns = nullptr,
				  unsigned trns_len = 0)
  {
    unsigned idx = xobject_গুনতি++;
    hb_vector_buf_t obj;
    obj.append_str ("<< /Type /XObject /Subtype /Image\n");
    obj.append_str ("/Width ");
    obj.append_unsigned (width);
    obj.append_str (" /Height ");
    obj.append_unsigned (height);
    obj.append_str ("\n/BitsPerComponent 8\n");

    যদি (plte && plte_len >= 3)
    {
      /* Indexed color: /ColorSpace [/Indexed /DeviceRGB N <hex palette>] */
      unsigned n_entries = plte_len / 3;
      obj.append_str ("/ColorSpace [/Indexed /DeviceRGB ");
      obj.append_unsigned (n_entries - 1);
      obj.append_str (" <");
      প্রতি (unsigned i = 0; i < n_entries * 3; i++)
      {
	obj.append_c ("0123456789ABCDEF"[plte[i] >> 4]);
	obj.append_c ("0123456789ABCDEF"[plte[i] & 0xF]);
      }
      obj.append_str (">]\n");
    }
    অন্যথা
    {
      obj.append_str ("/ColorSpace ");
      unsigned color_channels = has_alpha ? colors - 1 : colors;
      obj.append_str (color_channels == 1 ? "/DeviceGray" : "/DeviceRGB");
      obj.append_c ('\n');
    }

    /* Build SMask for indexed images with tRNS transparency. */
    unsigned smask_id = 0;
    যদি (plte && trns && trns_len)
    {
      hb_vector_buf_t smask_stream;
      যদি (hb_pdf_build_indexed_smask (&smask_stream, idat_data, idat_len,
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

    যদি (smask_id)
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

    (শূন্য) has_alpha;

    unsigned obj_id = add_object (std::move (obj));

    xobject_dict.append_str ("/Im");
    xobject_dict.append_unsigned (idx);
    xobject_dict.append_c (' ');
    xobject_dict.append_unsigned (obj_id);
    xobject_dict.append_str (" 0 R ");
    প্রত্যাগম idx;
  }
};

/* Store resources pointer in the paint struct's defs buffer
 * (repurposed — defs is unused for PDF). */
স্থির hb_pdf_resources_t *
hb_pdf_get_resources (hb_vector_paint_t *paint)
{
  যদি (!paint->defs.length)
  {
    /* First call: allocate and store. */
    auto *res = (hb_pdf_resources_t *) hb_calloc (1, পরিমাণ (hb_pdf_resources_t));
    যদি (unlikely (!res)) প্রত্যাগম nullptr;
    নূতন (res) hb_pdf_resources_t ();
    যদি (unlikely (!paint->defs.resize (পরিমাণ (শূন্য *))))
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
      প্রত্যাগম nullptr;
    }
    memcpy (paint->defs.arrayZ, &res, পরিমাণ (শূন্য *));
  }
  hb_pdf_resources_t *res;
  memcpy (&res, paint->defs.arrayZ, পরিমাণ (শূন্য *));
  প্রত্যাগম res;
}

শূন্য
hb_vector_paint_pdf_free_resources (hb_vector_paint_t *paint)
{
  যদি (paint->defs.length >= পরিমাণ (শূন্য *))
  {
    hb_pdf_resources_t *res;
    memcpy (&res, paint->defs.arrayZ, পরিমাণ (শূন্য *));
    যদি (res)
    {
      res->~hb_pdf_resources_t ();
      hb_free (res);
    }
  }
  paint->defs.clear ();
}


/* ---- helpers ---- */


/* Emit a glyph outline as PDF path operators into buf. */
স্থির শূন্য
hb_pdf_emit_glyph_path (hb_vector_paint_t *paint,
			 hb_font_t *font,
			 hb_codepoint_t glyph,
			 hb_vector_buf_t *buf)
{
  paint->path.clear ();
  /* Skip the outline extraction when the session work budget is
   * spent; an empty path keeps the document structure intact. */
  যদি (likely (paint->budget_remaining >= 0))
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

স্থির শূন্য
hb_pdf_paint_ঠেলো_transform (hb_paint_funcs_t *,
			     শূন্য *paint_data,
			     ভিন্ন xx, ভিন্ন yx,
			     ভিন্ন xy, ভিন্ন yy,
			     ভিন্ন dx, ভিন্ন dy,
			     শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  auto &body = paint->current_body ();
  unsigned sprec = body.scale_precision ();
  body.append_str ("q\n");
  body.append_অঙ্ক (xx, sprec);
  body.append_c (' ');
  body.append_অঙ্ক (yx, sprec);
  body.append_c (' ');
  body.append_অঙ্ক (xy, sprec);
  body.append_c (' ');
  body.append_অঙ্ক (yy, sprec);
  body.append_c (' ');
  body.append_অঙ্ক (paint->sx (dx));
  body.append_c (' ');
  body.append_অঙ্ক (paint->sy (dy));
  body.append_str (" cm\n");
}

স্থির শূন্য
hb_pdf_paint_বের_করো_transform (hb_paint_funcs_t *,
			    শূন্য *paint_data,
			    শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  paint->current_body ().append_str ("Q\n");
}

স্থির শূন্য
hb_pdf_paint_fill_glyph (hb_paint_funcs_t *,
			 শূন্য *paint_data,
			 hb_codepoint_t glyph,
			 hb_font_t *font,
			 hb_bool_t,
			 hb_color_t c,
			 শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  ভিন্ন r = hb_color_get_red (c) / 255.f;
  ভিন্ন g = hb_color_get_green (c) / 255.f;
  ভিন্ন b = hb_color_get_blue (c) / 255.f;
  ভিন্ন a = hb_color_get_alpha (c) / 255.f;

  যদি (a < 1.f / 255.f)
    প্রত্যাগম;

  auto &body = paint->current_body ();

  তার্কিক scoped = অসত্য;
  /* Set alpha via ExtGState if needed, scoped so it does not leak
   * into later paints. */
  যদি (a < 1.f - 1.f / 512.f)
  {
    auto *res = hb_pdf_get_resources (paint);
    যদি (res)
    {
      unsigned gs_idx = res->add_extgstate_alpha (a);
      body.append_str ("q\n");
      scoped = সত্য;
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  body.append_অঙ্ক (r, 4);
  body.append_c (' ');
  body.append_অঙ্ক (g, 4);
  body.append_c (' ');
  body.append_অঙ্ক (b, 4);
  body.append_str (" rg\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("f\n");
  যদি (scoped)
    body.append_str ("Q\n");
}

স্থির শূন্য
hb_pdf_paint_ঠেলো_clip_glyph (hb_paint_funcs_t *,
			      শূন্য *paint_data,
			      hb_codepoint_t glyph,
			      hb_font_t *font,
			      শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  auto &body = paint->current_body ();
  body.append_str ("q\n");
  hb_pdf_emit_glyph_path (paint, font, glyph, &body);
  body.append_str ("W n\n");
}

স্থির শূন্য
hb_pdf_paint_ঠেলো_clip_rectangle (hb_paint_funcs_t *,
				  শূন্য *paint_data,
				  ভিন্ন xmin, ভিন্ন ymin,
				  ভিন্ন xmax, ভিন্ন ymax,
				  শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  auto &body = paint->current_body ();
  body.append_str ("q\n");
  body.append_অঙ্ক (paint->sx (xmin));
  body.append_c (' ');
  body.append_অঙ্ক (paint->sy (ymin));
  body.append_c (' ');
  body.append_অঙ্ক (paint->sx (xmax - xmin));
  body.append_c (' ');
  body.append_অঙ্ক (paint->sy (ymax - ymin));
  body.append_str (" re W n\n");
}

স্থির hb_draw_funcs_t *
hb_pdf_paint_ঠেলো_clip_path_start (hb_paint_funcs_t *,
				   শূন্য *paint_data,
				   শূন্য **draw_data,
				   শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
  {
    *draw_data = nullptr;
    প্রত্যাগম nullptr;
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
  প্রত্যাগম hb_vector_pdf_path_draw_funcs_get ();
}

স্থির শূন্য
hb_pdf_paint_ঠেলো_clip_path_end (hb_paint_funcs_t *,
				 শূন্য *paint_data,
				 শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  paint->current_body ().append_str ("W n\n");
}

স্থির শূন্য
hb_pdf_paint_বের_করো_clip (hb_paint_funcs_t *,
		       শূন্য *paint_data,
		       শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  paint->current_body ().append_str ("Q\n");
}

/* Paint a solid color, including alpha via ExtGState. */
স্থির শূন্য
hb_pdf_paint_solid_color (hb_vector_paint_t *paint, hb_color_t c)
{
  auto &body = paint->current_body ();

  ভিন্ন r = hb_color_get_red (c) / 255.f;
  ভিন্ন g = hb_color_get_green (c) / 255.f;
  ভিন্ন b = hb_color_get_blue (c) / 255.f;
  ভিন্ন a = hb_color_get_alpha (c) / 255.f;

  যদি (a < 1.f / 255.f)
    প্রত্যাগম;

  /* Set alpha via ExtGState if needed. */
  যদি (a < 1.f - 1.f / 512.f)
  {
    auto *res = hb_pdf_get_resources (paint);
    যদি (res)
    {
      unsigned gs_idx = res->add_extgstate_alpha (a);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }

  /* Set fill color. */
  body.append_অঙ্ক (r, 4);
  body.append_c (' ');
  body.append_অঙ্ক (g, 4);
  body.append_c (' ');
  body.append_অঙ্ক (b, 4);
  body.append_str (" rg\n");

  /* Paint a huge rect (will be clipped). */
  body.append_str ("-32767 -32767 65534 65534 re f\n");
}

স্থির শূন্য
hb_pdf_paint_color (hb_paint_funcs_t *,
		    শূন্য *paint_data,
		    hb_bool_t,
		    hb_color_t color,
		    শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  hb_pdf_paint_solid_color (paint, color);
}

/* Build an uncompressed alpha mask from indexed PNG IDAT data + tRNS.
 * Decompresses IDAT, un-filters PNG rows, maps palette indices to alpha. */
স্থির তার্কিক
hb_pdf_build_indexed_smask (hb_vector_buf_t *out,
			    ধ্রুব বর্ণ *idat_data, unsigned idat_len,
			    unsigned width, unsigned height,
			    ধ্রুব uint8_t *trns, unsigned trns_len)
{
#ifndef HAVE_ZLIB
  (শূন্য) out; (শূন্য) idat_data; (শূন্য) idat_len;
  (শূন্য) width; (শূন্য) height; (শূন্য) trns; (শূন্য) trns_len;
  প্রত্যাগম অসত্য;
#অন্যথা
  /* Guard the size arithmetic below against 32-bit overflow.  width/height come
   * from the attacker-controlled PNG IHDR; without this, (width + 1) * height or
   * width * height can wrap and under-size buffers that are then indexed per
   * pixel (raw + y * (width + 1), out->arrayZ[y * width + x]). */
  unsigned raw_len, mask_len;
  যদি (!width || !height ||
      width == 0xFFFFFFFFu ||
      hb_unsigned_mul_overflows (width + 1, height, &raw_len) ||
      hb_unsigned_mul_overflows (width, height, &mask_len))
    প্রত্যাগম অসত্য;

  /* Decompress IDAT (zlib). */
  /* raw_len = (width + 1) * height: 1 filter byte per row + width bytes. */
  uint8_t *raw = (uint8_t *) hb_malloc (raw_len);
  যদি (!raw) প্রত্যাগম অসত্য;
  HB_SCOPE_GUARD (hb_free (raw));

  z_stream stream = {};
  stream.next_in = (Bytef *) idat_data;
  stream.avail_in = idat_len;
  stream.next_out = (Bytef *) raw;
  stream.avail_out = raw_len;

  যদি (inflateInit (&stream) != Z_OK)
    প্রত্যাগম অসত্য;
  পূর্ণাঙ্ক status = inflate (&stream, Z_FINISH);
  unsigned long total_out = stream.total_out;
  inflateEnd (&stream);
  যদি (status != Z_STREAM_END || total_out != raw_len)
    প্রত্যাগম অসত্য;

  /* Un-filter and map to alpha. */
  যদি (!out->resize (mask_len))
    প্রত্যাগম অসত্য;

  uint8_t *unfiltered = (uint8_t *) hb_malloc (width);
  যদি (!unfiltered)
    প্রত্যাগম অসত্য;
  HB_SCOPE_GUARD (hb_free (unfiltered));

  uint8_t *prev_unfiltered = (uint8_t *) hb_calloc (width, 1);
  যদি (!prev_unfiltered)
    প্রত্যাগম অসত্য;
  HB_SCOPE_GUARD (hb_free (prev_unfiltered));

  প্রতি (unsigned y = 0; y < height; y++)
  {
    uint8_t *row = raw + y * (width + 1);
    uint8_t filter = row[0];
    uint8_t *pixels = row + 1;

    প্রতি (unsigned x = 0; x < width; x++)
    {
      uint8_t a = (x > 0) ? unfiltered[x - 1] : 0;
      uint8_t b = prev_unfiltered[x];
      uint8_t c = (x > 0) ? prev_unfiltered[x - 1] : 0;
      uint8_t val = pixels[x];

      বিকল্প (filter)
      {
      পক্ষ 0: unfiltered[x] = val; ভঙ্গ; /* None */
      পক্ষ 1: unfiltered[x] = val + a; ভঙ্গ; /* Sub */
      পক্ষ 2: unfiltered[x] = val + b; ভঙ্গ; /* Up */
      পক্ষ 3: unfiltered[x] = val + (uint8_t) (((unsigned) a + b) / 2); ভঙ্গ; /* Average */
      পক্ষ 4: /* Paeth */
      {
	পূর্ণাঙ্ক p = (পূর্ণাঙ্ক) a + (পূর্ণাঙ্ক) b - (পূর্ণাঙ্ক) c;
	পূর্ণাঙ্ক pa = abs (p - (পূর্ণাঙ্ক) a);
	পূর্ণাঙ্ক pb = abs (p - (পূর্ণাঙ্ক) b);
	পূর্ণাঙ্ক pc = abs (p - (পূর্ণাঙ্ক) c);
	uint8_t pr = (pa <= pb && pa <= pc) ? a : (pb <= pc) ? b : c;
	unfiltered[x] = val + pr;
	ভঙ্গ;
      }
      স্বভাব: unfiltered[x] = val; ভঙ্গ;
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

  প্রত্যাগম সত্য;
#endif
}

/* Read a big-endian uint32 from a byte pointer. */
স্থির অন্তঃস্থ uint32_t
hb_pdf_png_u32 (ধ্রুব uint8_t *p)
{
  প্রত্যাগম ((uint32_t) p[0] << 24) | ((uint32_t) p[1] << 16) |
	 ((uint32_t) p[2] << 8)  | (uint32_t) p[3];
}

স্থির hb_bool_t
hb_pdf_paint_image (hb_paint_funcs_t *,
		    শূন্য *paint_data,
		    hb_blob_t *image,
		    unsigned width,
		    unsigned height,
		    hb_tag_t format,
		    ভিন্ন slant HB_UNUSED,
		    hb_glyph_extents_t *extents,
		    শূন্য *)
{
  যদি (format != HB_TAG ('p','n','g',' '))
    প্রত্যাগম অসত্য;
  যদি (!extents || !width || !height)
    প্রত্যাগম অসত্য;

  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম অসত্য;
  auto *res = hb_pdf_get_resources (paint);
  যদি (unlikely (!res))
    প্রত্যাগম অসত্য;

  unsigned len = 0;
  ধ্রুব uint8_t *data = (ধ্রুব uint8_t *) hb_blob_get_data (image, &len);
  যদি (!data || len < 8)
    প্রত্যাগম অসত্য;

  /* Verify PNG signature. */
  স্থির ধ্রুব uint8_t png_sig[8] = {137, 80, 78, 71, 13, 10, 26, 10};
  যদি (hb_memcmp (data, png_sig, 8) != 0)
    প্রত্যাগম অসত্য;

  /* Parse PNG chunks: extract IHDR and concatenate IDAT payloads. */
  unsigned png_w = 0, png_h = 0;
  unsigned colors = 3; /* default RGB */
  uint8_t color_type = 2;
  তার্কিক has_alpha = অসত্য;
  ধ্রুব uint8_t *plte_data = nullptr;
  unsigned plte_len = 0;
  ধ্রুব uint8_t *trns_data = nullptr;
  unsigned trns_len = 0;
  hb_vector_buf_t idat;

  unsigned pos = 8;
  /* Invariant: pos <= len (len >= 8 checked above).  All bounds checks below
   * are written to avoid 32-bit wrapping: a malicious chunk_len must not be
   * able to wrap pos + 12 + chunk_len past the end of the buffer. */
  যাবত্ (len - pos >= 12)
  {
    uint32_t chunk_len = hb_pdf_png_u32 (data + pos);
    uint32_t chunk_type = hb_pdf_png_u32 (data + pos + 4);
    যদি (chunk_len > len - pos - 12)
      ভঙ্গ;
    ধ্রুব uint8_t *chunk_data = data + pos + 8;

    যদি (chunk_type == 0x49484452u) /* IHDR */
    {
      যদি (chunk_len < 13) প্রত্যাগম অসত্য;
      png_w = hb_pdf_png_u32 (chunk_data);
      png_h = hb_pdf_png_u32 (chunk_data + 4);
      uint8_t bit_depth = chunk_data[8];
      color_type = chunk_data[9];
      যদি (bit_depth != 8) প্রত্যাগম অসত্য; /* only 8-bit supported */

      বিকল্প (color_type)
      {
      পক্ষ 0: colors = 1; has_alpha = অসত্য; ভঙ্গ; /* Grayscale */
      পক্ষ 2: colors = 3; has_alpha = অসত্য; ভঙ্গ; /* RGB */
      পক্ষ 3: colors = 1; has_alpha = অসত্য; ভঙ্গ; /* Indexed */
      পক্ষ 4: colors = 2; has_alpha = সত্য;  ভঙ্গ; /* Gray+Alpha */
      পক্ষ 6: colors = 4; has_alpha = সত্য;  ভঙ্গ; /* RGBA */
      স্বভাব: প্রত্যাগম অসত্য;
      }
    }
    অন্যথা যদি (chunk_type == 0x504C5445u) /* PLTE */
    {
      plte_data = chunk_data;
      plte_len = chunk_len;
    }
    অন্যথা যদি (chunk_type == 0x74524E53u) /* tRNS */
    {
      trns_data = chunk_data;
      trns_len = chunk_len;
    }
    অন্যথা যদি (chunk_type == 0x49444154u) /* IDAT */
    {
      idat.append_len ((ধ্রুব বর্ণ *) chunk_data, chunk_len);
    }
    অন্যথা যদি (chunk_type == 0x49454E44u) /* IEND */
      ভঙ্গ;

    pos += 12 + chunk_len;
  }

  যদি (!png_w || !png_h || !idat.length)
    প্রত্যাগম অসত্য;

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
  ভিন্ন ix = (ভিন্ন) extents->x_bearing;
  ভিন্ন iy = (ভিন্ন) extents->y_bearing + (ভিন্ন) extents->height;
  ভিন্ন iw = (ভিন্ন) extents->width;
  ভিন্ন ih = -(ভিন্ন) extents->height; /* negative because image Y goes up but height is negative in extents */

  body.append_অঙ্ক (paint->sx (iw));
  body.append_str (" 0 0 ");
  body.append_অঙ্ক (paint->sy (ih));
  body.append_c (' ');
  body.append_অঙ্ক (paint->sx (ix));
  body.append_c (' ');
  body.append_অঙ্ক (paint->sy (iy));
  body.append_str (" cm\n");

  body.append_str ("/Im");
  body.append_unsigned (im_idx);
  body.append_str (" Do\nQ\n");

  প্রত্যাগম সত্য;
}

/* ---- Gradient helpers ---- */

স্থির তার্কিক
hb_pdf_gradient_needs_alpha (hb_array_t<ধ্রুব hb_color_stop_t> stops)
{
  প্রতি (ধ্রুব auto &s : stops)
    যদি (hb_color_get_alpha (s.color) != 255)
      প্রত্যাগম সত্য;
  প্রত্যাগম অসত্য;
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single alpha stop pair (DeviceGray, scalar output). */
স্থির শূন্য
hb_pdf_build_alpha_interpolation_function (hb_vector_buf_t *obj,
					   ভিন্ন a0, ভিন্ন a1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_অঙ্ক (a0, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_অঙ্ক (a1, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) for the alpha channel of
 * pre-populated paint->color_stops_scratch (already sorted+normalized). */
স্থির unsigned
hb_pdf_build_alpha_gradient_function_from_stops (hb_pdf_resources_t *res,
						 hb_vector_paint_t *paint)
{
  unsigned গুনতি = paint->color_stops_scratch.length;

  যদি (গুনতি < 2)
  {
    ভিন্ন a = গুনতি ? hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f : 1.f;
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj, a, a);
    প্রত্যাগম res->add_object (std::move (obj));
  }

  যদি (গুনতি == 2)
  {
    hb_vector_buf_t obj;
    hb_pdf_build_alpha_interpolation_function (&obj,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[0].color) / 255.f,
      hb_color_get_alpha (paint->color_stops_scratch.arrayZ[1].color) / 255.f);
    প্রত্যাগম res->add_object (std::move (obj));
  }

  hb_vector_t<unsigned> sub_func_ids;
  প্রতি (unsigned i = 0; i + 1 < গুনতি; i++)
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
  প্রতি (unsigned i = 0; i < sub_func_ids.length; i++)
  {
    যদি (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n/Bounds [");
  প্রতি (unsigned i = 1; i + 1 < গুনতি; i++)
  {
    যদি (i > 1) obj.append_c (' ');
    obj.append_অঙ্ক (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n/Encode [");
  প্রতি (unsigned i = 0; i + 1 < গুনতি; i++)
  {
    যদি (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");
  প্রত্যাগম res->add_object (std::move (obj));
}

/* Build a PDF Type 2 (exponential interpolation) function for
 * a single color stop pair. */
স্থির শূন্য
hb_pdf_build_interpolation_function (hb_vector_buf_t *obj,
				     hb_color_t c0, hb_color_t c1)
{
  obj->append_str ("<< /FunctionType 2 /Domain [0 1] /N 1\n");
  obj->append_str ("/C0 [");
  obj->append_অঙ্ক (hb_color_get_red (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_অঙ্ক (hb_color_get_green (c0) / 255.f, 4);
  obj->append_c (' ');
  obj->append_অঙ্ক (hb_color_get_blue (c0) / 255.f, 4);
  obj->append_str ("]\n/C1 [");
  obj->append_অঙ্ক (hb_color_get_red (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_অঙ্ক (hb_color_get_green (c1) / 255.f, 4);
  obj->append_c (' ');
  obj->append_অঙ্ক (hb_color_get_blue (c1) / 255.f, 4);
  obj->append_str ("] >>");
}

/* Build a stitching function (Type 3) from pre-populated
 * paint->color_stops_scratch (already sorted+normalized). */
স্থির unsigned
hb_pdf_build_gradient_function_from_stops (hb_pdf_resources_t *res,
					   hb_vector_paint_t *paint)
{
  unsigned গুনতি = paint->color_stops_scratch.length;

  যদি (গুনতি < 2)
  {
    /* Single stop: constant function. */
    hb_color_t c = গুনতি ? paint->color_stops_scratch.arrayZ[0].color
			 : HB_COLOR (0, 0, 0, 255);
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj, c, c);
    প্রত্যাগম res->add_object (std::move (obj));
  }

  /* Sort by offset. */
  paint->color_stops_scratch.as_array ().qsort (
    [] (ধ্রুব hb_color_stop_t &a, ধ্রুব hb_color_stop_t &b)
    { প্রত্যাগম (a.offset > b.offset) - (a.offset < b.offset); });

  যদি (গুনতি == 2)
  {
    /* Two stops: single interpolation function. */
    hb_vector_buf_t obj;
    hb_pdf_build_interpolation_function (&obj,
					  paint->color_stops_scratch.arrayZ[0].color,
					  paint->color_stops_scratch.arrayZ[1].color);
    প্রত্যাগম res->add_object (std::move (obj));
  }

  /* Multiple stops: create sub-functions and stitch. */
  hb_vector_t<unsigned> sub_func_ids;
  প্রতি (unsigned i = 0; i + 1 < গুনতি; i++)
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
  প্রতি (unsigned i = 0; i < sub_func_ids.length; i++)
  {
    যদি (i) obj.append_c (' ');
    obj.append_unsigned (sub_func_ids.arrayZ[i]);
    obj.append_str (" 0 R");
  }
  obj.append_str ("]\n");

  /* Bounds. */
  obj.append_str ("/Bounds [");
  প্রতি (unsigned i = 1; i + 1 < গুনতি; i++)
  {
    যদি (i > 1) obj.append_c (' ');
    obj.append_অঙ্ক (paint->color_stops_scratch.arrayZ[i].offset, 4);
  }
  obj.append_str ("]\n");

  /* Encode array. */
  obj.append_str ("/Encode [");
  প্রতি (unsigned i = 0; i + 1 < গুনতি; i++)
  {
    যদি (i) obj.append_c (' ');
    obj.append_str ("0 1");
  }
  obj.append_str ("] >>");

  প্রত্যাগম res->add_object (std::move (obj));
}

স্থির শূন্য
hb_pdf_paint_linear_gradient (hb_paint_funcs_t *,
			      শূন্য *paint_data,
			      hb_color_সারি_t *color_সারি,
			      ভিন্ন x0, ভিন্ন y0,
			      ভিন্ন x1, ভিন্ন y1,
			      ভিন্ন x2 HB_UNUSED, ভিন্ন y2 HB_UNUSED,
			      শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  auto *res = hb_pdf_get_resources (paint);
  যদি (unlikely (!res))
    প্রত্যাগম;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  যদি (!paint->fetch_color_stops (color_সারি))
    প্রত্যাগম;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ভিন্ন mn, mx;
  hb_paint_normalize_color_সারি (stops.arrayZ, stops.length, &mn, &mx);
  ভিন্ন gx0 = x0 + mn * (x1 - x0);
  ভিন্ন gy0 = y0 + mn * (y1 - y0);
  ভিন্ন gx1 = x0 + mx * (x1 - x0);
  ভিন্ন gy1 = y0 + mx * (y1 - y0);

  unsigned func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_সারি_get_extend (color_সারি);
  ধ্রুব বর্ণ *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 2 (axial) shading — color only. */
  hb_vector_buf_t sh;
  sh.append_str ("<< /ShadingType 2 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_অঙ্ক (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sy (gy1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  unsigned sh_idx = res->add_shading (std::move (sh));

  auto &body = paint->current_body ();

  তার্কিক needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  যদি (needs_alpha)
  {
    unsigned alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_vector_buf_t ash;
    ash.append_str ("<< /ShadingType 2 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_অঙ্ক (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sy (gy1));
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

স্থির শূন্য
hb_pdf_paint_radial_gradient (hb_paint_funcs_t *,
			      শূন্য *paint_data,
			      hb_color_সারি_t *color_সারি,
			      ভিন্ন x0, ভিন্ন y0, ভিন্ন r0,
			      ভিন্ন x1, ভিন্ন y1, ভিন্ন r1,
			      শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  auto *res = hb_pdf_get_resources (paint);
  যদি (unlikely (!res))
    প্রত্যাগম;

  /* Fetch, normalize stops to [0,1], and adjust coordinates. */
  যদি (!paint->fetch_color_stops (color_সারি))
    প্রত্যাগম;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;

  ভিন্ন mn, mx;
  hb_paint_normalize_color_সারি (stops.arrayZ, stops.length, &mn, &mx);
  ভিন্ন gx0 = x0 + mn * (x1 - x0);
  ভিন্ন gy0 = y0 + mn * (y1 - y0);
  ভিন্ন gr0 = r0 + mn * (r1 - r0);
  ভিন্ন gx1 = x0 + mx * (x1 - x0);
  ভিন্ন gy1 = y0 + mx * (y1 - y0);
  ভিন্ন gr1 = r0 + mx * (r1 - r0);

  unsigned func_id = hb_pdf_build_gradient_function_from_stops (res, paint);

  hb_paint_extend_t extend = hb_color_সারি_get_extend (color_সারি);
  ধ্রুব বর্ণ *extend_str = (extend == HB_PAINT_EXTEND_PAD)
			    ? "/Extend [true true]\n" : "";

  /* Build Type 3 (radial) shading — color only. */
  hb_vector_buf_t sh;
  sh.append_str ("<< /ShadingType 3 /ColorSpace /DeviceRGB\n");
  sh.append_str ("/Coords [");
  sh.append_অঙ্ক (paint->sx (gx0));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sy (gy0));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sx (gr0));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sx (gx1));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sy (gy1));
  sh.append_c (' ');
  sh.append_অঙ্ক (paint->sx (gr1));
  sh.append_str ("]\n/Function ");
  sh.append_unsigned (func_id);
  sh.append_str (" 0 R\n");
  sh.append_str (extend_str);
  sh.append_str (">>");

  unsigned sh_idx = res->add_shading (std::move (sh));

  auto &body = paint->current_body ();

  তার্কিক needs_alpha = hb_pdf_gradient_needs_alpha (stops);
  যদি (needs_alpha)
  {
    unsigned alpha_func_id = hb_pdf_build_alpha_gradient_function_from_stops (res, paint);

    hb_vector_buf_t ash;
    ash.append_str ("<< /ShadingType 3 /ColorSpace /DeviceGray\n");
    ash.append_str ("/Coords [");
    ash.append_অঙ্ক (paint->sx (gx0));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sy (gy0));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sx (gr0));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sx (gx1));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sy (gy1));
    ash.append_c (' ');
    ash.append_অঙ্ক (paint->sx (gr1));
    ash.append_str ("]\n/Function ");
    ash.append_unsigned (alpha_func_id);
    ash.append_str (" 0 R\n");
    ash.append_str (extend_str);
    ash.append_str (">>");
    unsigned alpha_sh_id = res->add_object (std::move (ash));

    /* BBox: enclosing square of the outer circle. */
    ভিন্ন cx = (gr1 >= gr0) ? gx1 : gx0;
    ভিন্ন cy = (gr1 >= gr0) ? gy1 : gy0;
    ভিন্ন rr = hb_max (gr0, gr1);
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
স্থির শূন্য
hb_pdf_encode_u16 (hb_vector_buf_t *buf, uint16_t v)
{
  বর্ণ bytes[2] = {(বর্ণ) (v >> 8), (বর্ণ) (v & 0xFF)};
  buf->append_len (bytes, 2);
}

/* Encode a coordinate as 16-bit value relative to Decode range. */
স্থির শূন্য
hb_pdf_encode_coord (hb_vector_buf_t *buf,
		     ভিন্ন val, ভিন্ন lo, ভিন্ন hi)
{
  ভিন্ন t = (val - lo) / (hi - lo);
  t = hb_clamp (t, 0.f, 1.f);
  hb_pdf_encode_u16 (buf, (uint16_t) (t * 65535.f + 0.5f));
}

/* Encode RGB from hb_color_t as 3 bytes. */
স্থির শূন্য
hb_pdf_encode_color_rgb (hb_vector_buf_t *buf, hb_color_t c)
{
  বর্ণ rgb[3] = {(বর্ণ) hb_color_get_red (c),
		 (বর্ণ) hb_color_get_green (c),
		 (বর্ণ) hb_color_get_blue (c)};
  buf->append_len (rgb, 3);
}

/* Encode alpha from hb_color_t as 1 byte (gray). */
স্থির শূন্য
hb_pdf_encode_color_alpha (hb_vector_buf_t *buf, hb_color_t c)
{
  বর্ণ a = (বর্ণ) hb_color_get_alpha (c);
  buf->append_len (&a, 1);
}

/* Encode one Coons patch control point. */
স্থির শূন্য
hb_pdf_encode_point (hb_vector_buf_t *buf,
		     ভিন্ন x, ভিন্ন y,
		     ভিন্ন xlo, ভিন্ন xhi,
		     ভিন্ন ylo, ভিন্ন yhi)
{
  hb_pdf_encode_coord (buf, x, xlo, xhi);
  hb_pdf_encode_coord (buf, y, ylo, yhi);
}

/* Emit one Coons patch sector into the mesh stream(s).
 * Splits large arcs into sub-patches of max 90°.
 * If alpha_mesh is non-null, emits a parallel DeviceGray
 * patch with the alpha channel. */
স্থির শূন্য
hb_pdf_add_sweep_patch (hb_vector_buf_t *mesh,
			hb_vector_buf_t *alpha_mesh,
			ভিন্ন cx, ভিন্ন cy,
			ভিন্ন xlo, ভিন্ন xhi, ভিন্ন ylo, ভিন্ন yhi,
			ভিন্ন a0, hb_color_t c0_in,
			ভিন্ন a1, hb_color_t c1_in)
{
  ধ্রুব ভিন্ন R = 32767.f;
  ধ্রুব ভিন্ন eps = 0.5f;
  ধ্রুব ভিন্ন MAX_SECTOR = (ভিন্ন) M_PI / 2.f;

  পূর্ণাঙ্ক অঙ্ক_splits = (পূর্ণাঙ্ক) ceilf (fabsf (a1 - a0) / MAX_SECTOR);
  যদি (অঙ্ক_splits < 1) অঙ্ক_splits = 1;

  প্রতি (পূর্ণাঙ্ক s = 0; s < অঙ্ক_splits; s++)
  {
    ভিন্ন k0 = (ভিন্ন) s / অঙ্ক_splits;
    ভিন্ন k1 = (ভিন্ন) (s + 1) / অঙ্ক_splits;
    ভিন্ন sa0 = a0 + k0 * (a1 - a0);
    ভিন্ন sa1 = a0 + k1 * (a1 - a0);
    hb_color_t sc0 = hb_color_lerp (c0_in, c1_in, k0);
    hb_color_t sc1 = hb_color_lerp (c0_in, c1_in, k1);

    ভিন্ন da = sa1 - sa0;
    ভিন্ন kappa = (4.f / 3.f) * tanf (da / 4.f);

    ভিন্ন cos0 = cosf (sa0), sin0 = sinf (sa0);
    ভিন্ন cos1 = cosf (sa1), sin1 = sinf (sa1);

    ভিন্ন p0x = cx + eps * cos0, p0y = cy + eps * sin0;
    ভিন্ন p3x = cx + R * cos0,   p3y = cy + R * sin0;
    ভিন্ন p6x = cx + R * cos1,   p6y = cy + R * sin1;
    ভিন্ন p9x = cx + eps * cos1, p9y = cy + eps * sin1;

    /* Edge 1: p0→p3, radial straight line. */
    ভিন্ন e1_1x = p0x + (p3x - p0x) / 3.f;
    ভিন্ন e1_1y = p0y + (p3y - p0y) / 3.f;
    ভিন্ন e1_2x = p0x + 2.f * (p3x - p0x) / 3.f;
    ভিন্ন e1_2y = p0y + 2.f * (p3y - p0y) / 3.f;

    /* Edge 2: p3→p6, outer arc. */
    ভিন্ন e2_1x = p3x + kappa * R * (-sin0);
    ভিন্ন e2_1y = p3y + kappa * R * ( cos0);
    ভিন্ন e2_2x = p6x - kappa * R * (-sin1);
    ভিন্ন e2_2y = p6y - kappa * R * ( cos1);

    /* Edge 3: p6→p9, radial straight line. */
    ভিন্ন e3_1x = p6x + (p9x - p6x) / 3.f;
    ভিন্ন e3_1y = p6y + (p9y - p6y) / 3.f;
    ভিন্ন e3_2x = p6x + 2.f * (p9x - p6x) / 3.f;
    ভিন্ন e3_2y = p6y + 2.f * (p9y - p6y) / 3.f;

    /* Edge 4: p9→p0, inner arc. */
    ভিন্ন e4_1x = p9x + kappa * eps * (-sin1);
    ভিন্ন e4_1y = p9y + kappa * eps * ( cos1);
    ভিন্ন e4_2x = p0x - kappa * eps * (-sin0);
    ভিন্ন e4_2y = p0y - kappa * eps * ( cos0);

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

    যদি (alpha_mesh)
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
সংরচনা hb_pdf_sweep_ctx_t {
  hb_vector_buf_t *mesh;
  hb_vector_buf_t *alpha_mesh;
  ভিন্ন cx, cy, xlo, xhi, ylo, yhi;
};

স্থির শূন্য
hb_pdf_sweep_emit_patch (ভিন্ন a0, hb_color_t c0,
			 ভিন্ন a1, hb_color_t c1,
			 শূন্য *user_data)
{
  auto *ctx = (hb_pdf_sweep_ctx_t *) user_data;
  /* Sweep-gradient meshes are not outline-derived, so they are bounded by
   * the output document-size cap, not the outline work budget: stop once a
   * mesh buffer trips its cap so per-gradient patch counts cannot multiply
   * with the paint-graph traversal limits of the font tables driving us. */
  যদি (unlikely (ctx->mesh->in_error () ||
		(ctx->alpha_mesh && ctx->alpha_mesh->in_error ())))
    প্রত্যাগম;
  hb_pdf_add_sweep_patch (ctx->mesh, ctx->alpha_mesh,
			  ctx->cx, ctx->cy,
			  ctx->xlo, ctx->xhi, ctx->ylo, ctx->yhi,
			  a0, c0, a1, c1);
}

স্থির শূন্য
hb_pdf_paint_sweep_gradient (hb_paint_funcs_t *,
			     শূন্য *paint_data,
			     hb_color_সারি_t *color_সারি,
			     ভিন্ন cx, ভিন্ন cy,
			     ভিন্ন start_angle,
			     ভিন্ন end_angle,
			     শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  auto *res = hb_pdf_get_resources (paint);
  যদি (unlikely (!res))
    প্রত্যাগম;

  /* Get and sort color stops. */
  যদি (!paint->fetch_color_stops (color_সারি))
    প্রত্যাগম;
  hb_vector_t<hb_color_stop_t> &stops = paint->color_stops_scratch;
  stops.as_array ().qsort (
    [] (ধ্রুব hb_color_stop_t &a, ধ্রুব hb_color_stop_t &b)
    { প্রত্যাগম (a.offset > b.offset) - (a.offset < b.offset); });

  hb_paint_extend_t extend = hb_color_সারি_get_extend (color_সারি);

  ধ্রুব ভিন্ন R = 32767.f;
  ভিন্ন scx = paint->sx (cx), scy = paint->sy (cy);
  ভিন্ন xlo = scx - R - 1, xhi = scx + R + 1;
  ভিন্ন ylo = scy - R - 1, yhi = scy + R + 1;

  তার্কিক needs_alpha = hb_pdf_gradient_needs_alpha (stops);

  hb_vector_buf_t mesh;
  hb_vector_buf_t alpha_mesh;
  mesh.alloc (256);
  যদি (needs_alpha)
    alpha_mesh.alloc (256);

  hb_pdf_sweep_ctx_t ctx { &mesh, needs_alpha ? &alpha_mesh : nullptr,
			    scx, scy, xlo, xhi, ylo, yhi };
  hb_paint_sweep_gradient_tiles (stops.arrayZ, stops.length, extend,
				 start_angle, end_angle,
				 hb_pdf_sweep_emit_patch, &ctx);

  যদি (!mesh.length)
    প্রত্যাগম;

  auto hb_pdf_build_mesh_shading = [&] (hb_vector_buf_t &m,
					 ধ্রুব বর্ণ *cs,
					 ধ্রুব বর্ণ *decode_suffix) -> unsigned
  {
    hb_vector_buf_t sh;
    sh.append_str ("<< /ShadingType 6 /ColorSpace /");
    sh.append_str (cs);
    sh.append_str ("\n/BitsPerCoordinate 16 /BitsPerComponent 8 /BitsPerFlag 8\n");
    sh.append_str ("/Decode [");
    sh.append_অঙ্ক (xlo, 2);
    sh.append_c (' ');
    sh.append_অঙ্ক (xhi, 2);
    sh.append_c (' ');
    sh.append_অঙ্ক (ylo, 2);
    sh.append_c (' ');
    sh.append_অঙ্ক (yhi, 2);
    sh.append_str (decode_suffix);
    sh.append_str ("]\n/Length ");
    sh.append_unsigned (m.length);
    sh.append_str (" >>\nstream\n");
    sh.append_len (m.arrayZ, m.length);
    sh.append_str ("\nendstream");
    প্রত্যাগম res->add_object (std::move (sh));
  };

  unsigned sh_obj_id = hb_pdf_build_mesh_shading (mesh, "DeviceRGB",
						   " 0 1 0 1 0 1");
  unsigned sh_idx = res->add_shading_by_id (sh_obj_id);

  auto &body = paint->current_body ();

  যদি (needs_alpha && alpha_mesh.length)
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

স্থির ধ্রুব বর্ণ *
hb_pdf_blend_mode_name (hb_paint_composite_mode_t mode)
{
  বিকল্প (mode)
  {
  পক্ষ HB_PAINT_COMPOSITE_MODE_MULTIPLY:       প্রত্যাগম "Multiply";
  পক্ষ HB_PAINT_COMPOSITE_MODE_SCREEN:         প্রত্যাগম "Screen";
  পক্ষ HB_PAINT_COMPOSITE_MODE_OVERLAY:        প্রত্যাগম "Overlay";
  পক্ষ HB_PAINT_COMPOSITE_MODE_DARKEN:         প্রত্যাগম "Darken";
  পক্ষ HB_PAINT_COMPOSITE_MODE_LIGHTEN:        প্রত্যাগম "Lighten";
  পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_DODGE:    প্রত্যাগম "ColorDodge";
  পক্ষ HB_PAINT_COMPOSITE_MODE_COLOR_BURN:     প্রত্যাগম "ColorBurn";
  পক্ষ HB_PAINT_COMPOSITE_MODE_HARD_LIGHT:     প্রত্যাগম "HardLight";
  পক্ষ HB_PAINT_COMPOSITE_MODE_SOFT_LIGHT:     প্রত্যাগম "SoftLight";
  পক্ষ HB_PAINT_COMPOSITE_MODE_DIFFERENCE:     প্রত্যাগম "Difference";
  পক্ষ HB_PAINT_COMPOSITE_MODE_EXCLUSION:      প্রত্যাগম "Exclusion";
  পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_HUE:        প্রত্যাগম "Hue";
  পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_SATURATION: প্রত্যাগম "Saturation";
  পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_COLOR:      প্রত্যাগম "Color";
  পক্ষ HB_PAINT_COMPOSITE_MODE_HSL_LUMINOSITY: প্রত্যাগম "Luminosity";
  /* Porter-Duff modes have no PDF blend-mode equivalent; approximate
   * the two that have a plausible color-blend analog, and let the
   * rest fall through to Normal (SRC_OVER). */
  পক্ষ HB_PAINT_COMPOSITE_MODE_PLUS:           প্রত্যাগম "Screen";
  পক্ষ HB_PAINT_COMPOSITE_MODE_XOR:            প্রত্যাগম "Difference";
  পক্ষ HB_PAINT_COMPOSITE_MODE_CLEAR:
  পক্ষ HB_PAINT_COMPOSITE_MODE_SRC:
  পক্ষ HB_PAINT_COMPOSITE_MODE_DEST:
  পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OVER:
  পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OVER:
  পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_IN:
  পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_IN:
  পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_OUT:
  পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_OUT:
  পক্ষ HB_PAINT_COMPOSITE_MODE_SRC_ATOP:
  পক্ষ HB_PAINT_COMPOSITE_MODE_DEST_ATOP:
  স্বভাব:                                     প্রত্যাগম nullptr; /* Normal */
  }
}

স্থির শূন্য
hb_pdf_paint_ঠেলো_group (hb_paint_funcs_t *,
			 শূন্য *paint_data,
			 শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  paint->current_body ().append_str ("q\n");
}

স্থির শূন্য
hb_pdf_paint_ঠেলো_group_for (hb_paint_funcs_t *,
			     শূন্য *paint_data,
			     hb_paint_composite_mode_t mode,
			     শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;

  auto &body = paint->current_body ();
  body.append_str ("q\n");

  ধ্রুব বর্ণ *bm = hb_pdf_blend_mode_name (mode);
  যদি (bm)
  {
    auto *res = hb_pdf_get_resources (paint);
    যদি (likely (res))
    {
      unsigned gs_idx = res->add_extgstate_blend (bm);
      body.append_str ("/GS");
      body.append_unsigned (gs_idx);
      body.append_str (" gs\n");
    }
  }
}

স্থির শূন্য
hb_pdf_paint_বের_করো_group (hb_paint_funcs_t *,
			শূন্য *paint_data,
			hb_paint_composite_mode_t mode HB_UNUSED,
			শূন্য *)
{
  auto *paint = (hb_vector_paint_t *) paint_data;
  যদি (unlikely (!paint->ensure_initialized ()))
    প্রত্যাগম;
  paint->current_body ().append_str ("Q\n");
}


/* ---- lazy loader for paint funcs ---- */

স্থির অন্তঃস্থ শূন্য free_static_pdf_paint_funcs ();

স্থির সংরচনা hb_pdf_paint_funcs_lazy_loader_t
  : hb_paint_funcs_lazy_loader_t<hb_pdf_paint_funcs_lazy_loader_t>
{
  স্থির hb_paint_funcs_t *create ()
  {
    hb_paint_funcs_t *funcs = hb_paint_funcs_create ();
    hb_paint_funcs_set_ঠেলো_transform_func (funcs, (hb_paint_ঠেলো_transform_func_t) hb_pdf_paint_ঠেলো_transform, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_transform_func (funcs, (hb_paint_বের_করো_transform_func_t) hb_pdf_paint_বের_করো_transform, nullptr, nullptr);
    hb_paint_funcs_set_fill_glyph_func (funcs, (hb_paint_fill_glyph_func_t) hb_pdf_paint_fill_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_glyph_func (funcs, (hb_paint_ঠেলো_clip_glyph_func_t) hb_pdf_paint_ঠেলো_clip_glyph, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_rectangle_func (funcs, (hb_paint_ঠেলো_clip_rectangle_func_t) hb_pdf_paint_ঠেলো_clip_rectangle, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_path_start_func (funcs, (hb_paint_ঠেলো_clip_path_start_func_t) hb_pdf_paint_ঠেলো_clip_path_start, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_clip_path_end_func (funcs, (hb_paint_ঠেলো_clip_path_end_func_t) hb_pdf_paint_ঠেলো_clip_path_end, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_clip_func (funcs, (hb_paint_বের_করো_clip_func_t) hb_pdf_paint_বের_করো_clip, nullptr, nullptr);
    hb_paint_funcs_set_color_func (funcs, (hb_paint_color_func_t) hb_pdf_paint_color, nullptr, nullptr);
    hb_paint_funcs_set_image_func (funcs, (hb_paint_image_func_t) hb_pdf_paint_image, nullptr, nullptr);
    hb_paint_funcs_set_linear_gradient_func (funcs, (hb_paint_linear_gradient_func_t) hb_pdf_paint_linear_gradient, nullptr, nullptr);
    hb_paint_funcs_set_radial_gradient_func (funcs, (hb_paint_radial_gradient_func_t) hb_pdf_paint_radial_gradient, nullptr, nullptr);
    hb_paint_funcs_set_sweep_gradient_func (funcs, (hb_paint_sweep_gradient_func_t) hb_pdf_paint_sweep_gradient, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_group_func (funcs, (hb_paint_ঠেলো_group_func_t) hb_pdf_paint_ঠেলো_group, nullptr, nullptr);
    hb_paint_funcs_set_ঠেলো_group_for_func (funcs, (hb_paint_ঠেলো_group_for_func_t) hb_pdf_paint_ঠেলো_group_for, nullptr, nullptr);
    hb_paint_funcs_set_বের_করো_group_func (funcs, (hb_paint_বের_করো_group_func_t) hb_pdf_paint_বের_করো_group, nullptr, nullptr);
    hb_paint_funcs_set_set_budget_func (funcs, hb_vector_paint_set_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_func (funcs, hb_vector_paint_get_budget_callback, nullptr, nullptr);
    hb_paint_funcs_set_get_budget_remaining_func (funcs, hb_vector_paint_get_budget_remaining_callback, nullptr, nullptr);
    hb_paint_funcs_make_immutable (funcs);
    hb_atexit (free_static_pdf_paint_funcs);
    প্রত্যাগম funcs;
  }
} static_pdf_paint_funcs;

স্থির অন্তঃস্থ শূন্য
free_static_pdf_paint_funcs ()
{
  static_pdf_paint_funcs.free_instance ();
}

hb_paint_funcs_t *
hb_vector_paint_pdf_funcs_get ()
{
  প্রত্যাগম static_pdf_paint_funcs.get_unconst ();
}


/* ---- render ---- */

hb_blob_t *
hb_vector_paint_render_pdf (hb_vector_paint_t *paint)
{
  যদি (!paint->has_extents)
    প্রত্যাগম nullptr;
  যদি (!paint->group_স্তূপ.length ||
      !paint->group_স্তূপ.arrayZ[0].length)
    প্রত্যাগম nullptr;

  hb_vector_buf_t &content = paint->group_স্তূপ.arrayZ[0];
  hb_pdf_resources_t *res = hb_pdf_get_resources (paint);

  ভিন্ন ex = paint->extents.x;
  ভিন্ন ey = paint->extents.y;
  ভিন্ন ew = paint->extents.width;
  ভিন্ন eh = paint->extents.height;

  unsigned অঙ্ক_extra = res ? res->objects.length : 0;
  unsigned total_objects = 4 + অঙ্ক_extra; /* 1-based: 1..total_objects */

  /* Build PDF. */
  hb_vector_buf_t out;
  hb_buf_recover_recycled (paint->recycled_blob, &out);
  out.alloc (content.length + অঙ্ক_extra * 128 + 1024);

  hb_vector_t<unsigned> offsets;
  যদি (unlikely (!offsets.resize (total_objects)))
    প্রত্যাগম nullptr;

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
  out.append_অঙ্ক (ex);
  out.append_c (' ');
  out.append_অঙ্ক (ey);
  out.append_c (' ');
  out.append_অঙ্ক (ex + ew);
  out.append_c (' ');
  out.append_অঙ্ক (ey + eh);
  out.append_str ("]\n/Contents 4 0 R");

  /* Resources. */
  তার্কিক has_resources = res &&
    (res->extgstate_dict.length || res->shading_dict.length || res->xobject_dict.length);
  যদি (has_resources)
  {
    out.append_str ("\n/Resources <<");
    যদি (res->extgstate_dict.length)
    {
      out.append_str (" /ExtGState << ");
      out.append_len (res->extgstate_dict.arrayZ, res->extgstate_dict.length);
      out.append_str (">>");
    }
    যদি (res->shading_dict.length)
    {
      out.append_str (" /Shading << ");
      out.append_len (res->shading_dict.arrayZ, res->shading_dict.length);
      out.append_str (">>");
    }
    যদি (res->xobject_dict.length)
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
  যদি (hb_color_get_alpha (paint->background))
  {
    ভিন্ন r = hb_color_get_red (paint->background) / 255.f;
    ভিন্ন g = hb_color_get_green (paint->background) / 255.f;
    ভিন্ন b = hb_color_get_blue (paint->background) / 255.f;
    ভিন্ন a = hb_color_get_alpha (paint->background) / 255.f;
    যদি (a < 1.f - 1.f / 512.f)
    {
      যদি (res)
      {
	unsigned gs_idx = res->add_extgstate_alpha (a);
	bg_prefix.append_str ("/GS");
	bg_prefix.append_unsigned (gs_idx);
	bg_prefix.append_str (" gs\n");
      }
    }
    bg_prefix.append_অঙ্ক (r, 4);
    bg_prefix.append_c (' ');
    bg_prefix.append_অঙ্ক (g, 4);
    bg_prefix.append_c (' ');
    bg_prefix.append_অঙ্ক (b, 4);
    bg_prefix.append_str (" rg\n");
    bg_prefix.append_অঙ্ক (ex);
    bg_prefix.append_c (' ');
    bg_prefix.append_অঙ্ক (ey);
    bg_prefix.append_c (' ');
    bg_prefix.append_অঙ্ক (ew);
    bg_prefix.append_c (' ');
    bg_prefix.append_অঙ্ক (eh);
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
  প্রতি (unsigned i = 0; i < অঙ্ক_extra; i++)
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
  প্রতি (unsigned i = 0; i < total_objects; i++)
  {
    বর্ণ tmp[21];
    snprintf (tmp, পরিমাণ (tmp), "%010u 00000 n \n", offsets.arrayZ[i]);
    out.append_len (tmp, 20);
  }

  /* Trailer */
  out.append_str ("trailer\n<< /Size ");
  out.append_unsigned (total_objects + 1);
  out.append_str (" /Root 1 0 R >>\nstartxref\n");
  out.append_unsigned (xref_offset);
  out.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&paint->recycled_blob, &out);

  প্রত্যাগম blob;
}

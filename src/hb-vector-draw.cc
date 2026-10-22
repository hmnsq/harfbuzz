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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-draw.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-vector-path.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

HB_ਅਣਵਰਤਿਆ ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
hb_ਸਦਿਸ਼_svg_ਬਫਰ_ਸ਼ਾਮਲ_ਹੈ (ਅਚਲ hb_ਸਦਿਸ਼_buf_t &buf, ਅਚਲ ਅੱਖਰ *needle)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ nlen = (ਬਿਨਾਨਿਸ਼ਾਨੀ) ਲੜੀ_ਲੰਬਾਈ (needle);
  ਜੇ (!nlen || buf.ਲੰਮਾਈ < nlen)
    ਵਾਪਸ ਝੂਠ;

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i + nlen <= buf.ਲੰਮਾਈ; i++)
    ਜੇ (buf.arrayZ[i] == needle[0] &&
        !memcmp (buf.arrayZ + i, needle, nlen))
      ਵਾਪਸ ਸੱਚ;
  ਵਾਪਸ ਝੂਠ;
}

/* ---- SVG draw callbacks ---- */

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_svg_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->ਰਸਤਾ.append_c ('M'); d->append_xy_svg (x, y);
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_svg_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->ਰਸਤਾ.append_c ('L'); d->append_xy_svg (x, y);
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_svg_quadratic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ cx, ਦਸ਼ਮਲਵ cy, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->ਰਸਤਾ.append_c ('Q'); d->append_xy_svg (cx, cy); d->ਰਸਤਾ.append_c (' '); d->append_xy_svg (x, y);
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_svg_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ c1x, ਦਸ਼ਮਲਵ c1y, ਦਸ਼ਮਲਵ c2x, ਦਸ਼ਮਲਵ c2y, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->ਰਸਤਾ.append_c ('C'); d->append_xy_svg (c1x, c1y); d->ਰਸਤਾ.append_c (' '); d->append_xy_svg (c2x, c2y); d->ਰਸਤਾ.append_c (' '); d->append_xy_svg (x, y);
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_svg_close_ਰਸਤਾ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->ਰਸਤਾ.append_c ('Z');
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}


/* ---- PDF draw callbacks ---- */

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_pdf_move_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->append_xy_pdf (x, y); d->ਰਸਤਾ.append_str (" m\n");
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_pdf_ਸਤਰ_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->append_xy_pdf (x, y); d->ਰਸਤਾ.append_str (" l\n");
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

/* No quadratic_to — null fallback auto-promotes to cubic. */

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_pdf_cubic_ਮੰਜ਼ਿਲ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਦਸ਼ਮਲਵ c1x, ਦਸ਼ਮਲਵ c1y, ਦਸ਼ਮਲਵ c2x, ਦਸ਼ਮਲਵ c2y, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->append_xy_pdf (c1x, c1y); d->ਰਸਤਾ.append_c (' '); d->append_xy_pdf (c2x, c2y); d->ਰਸਤਾ.append_c (' '); d->append_xy_pdf (x, y); d->ਰਸਤਾ.append_str (" c\n");
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ ਖਾਲੀ hb_ਸਦਿਸ਼_draw_pdf_close_ਰਸਤਾ (hb_draw_funcs_t *, ਖਾਲੀ *dd, hb_draw_state_t *, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *d = (hb_ਸਦਿਸ਼_draw_t *) dd;
  ਜੇ (unlikely (!d->begin_ਰਸਤਾ_command ())) ਵਾਪਸ;
  d->ਰਸਤਾ.append_str ("h\n");
  d->ਸਿਰਾ_ਰਸਤਾ_command ();
}

ਸਥਿਰ hb_bool_t
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_budget (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ,
			   int64_t budget, ਖਾਲੀ *)
{
  ਸਵੈਚਾਲਿਤ *draw = (hb_ਸਦਿਸ਼_draw_t *) draw_ਅੰਕੜੇ;
  draw->budget = budget;
  draw->recharge_budget ();
  ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ int64_t
hb_ਸਦਿਸ਼_draw_get_budget (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ ((hb_ਸਦਿਸ਼_draw_t *) draw_ਅੰਕੜੇ)->budget;
}

ਸਥਿਰ int64_t *
hb_ਸਦਿਸ਼_draw_get_budget_ਬਚਿਆ (hb_draw_funcs_t *, ਖਾਲੀ *draw_ਅੰਕੜੇ, ਖਾਲੀ *)
{
  ਵਾਪਸ &((hb_ਸਦਿਸ਼_draw_t *) draw_ਅੰਕੜੇ)->budget_ਬਚਿਆ;
}


/* ---- Lazy loaders ---- */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_draw_svg_funcs ();
ਸਥਿਰ ਬਣਤਰ hb_ਸਦਿਸ਼_draw_svg_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ਸਦਿਸ਼_draw_svg_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_move_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_svg_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_svg_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_quadratic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_quadratic_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_svg_quadratic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_cubic_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_svg_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, (hb_draw_close_ਰਸਤਾ_func_t) hb_ਸਦਿਸ਼_draw_svg_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_ਸਦਿਸ਼_draw_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_ਸਦਿਸ਼_draw_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ਸਦਿਸ਼_draw_svg_funcs);
    ਵਾਪਸ funcs;
  }
} static_ਸਦਿਸ਼_draw_svg_funcs;
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_draw_svg_funcs () { static_ਸਦਿਸ਼_draw_svg_funcs.free_instance (); }

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_draw_pdf_funcs ();
ਸਥਿਰ ਬਣਤਰ hb_ਸਦਿਸ਼_draw_pdf_funcs_lazy_loader_t
  : hb_draw_funcs_lazy_loader_t<hb_ਸਦਿਸ਼_draw_pdf_funcs_lazy_loader_t>
{
  ਸਥਿਰ hb_draw_funcs_t *create ()
  {
    hb_draw_funcs_t *funcs = hb_draw_funcs_create ();
    hb_draw_funcs_ਸੈੱਟ_move_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_move_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_pdf_move_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸਤਰ_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_ਸਤਰ_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_pdf_ਸਤਰ_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    /* No quadratic_to: null fallback auto-promotes to cubic. */
    hb_draw_funcs_ਸੈੱਟ_cubic_ਮੰਜ਼ਿਲ_func (funcs, (hb_draw_cubic_ਮੰਜ਼ਿਲ_func_t) hb_ਸਦਿਸ਼_draw_pdf_cubic_ਮੰਜ਼ਿਲ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_close_ਰਸਤਾ_func (funcs, (hb_draw_close_ਰਸਤਾ_func_t) hb_ਸਦਿਸ਼_draw_pdf_close_ਰਸਤਾ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_ਸੈੱਟ_budget_func (funcs, hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_func (funcs, hb_ਸਦਿਸ਼_draw_get_budget, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_ਸੈੱਟ_get_budget_ਬਚਿਆ_func (funcs, hb_ਸਦਿਸ਼_draw_get_budget_ਬਚਿਆ, ਖਾਲੀ_ਸੂਚਕ, ਖਾਲੀ_ਸੂਚਕ);
    hb_draw_funcs_make_immutable (funcs);
    hb_atexit (free_static_ਸਦਿਸ਼_draw_pdf_funcs);
    ਵਾਪਸ funcs;
  }
} static_ਸਦਿਸ਼_draw_pdf_funcs;
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_ਸਦਿਸ਼_draw_pdf_funcs () { static_ਸਦਿਸ਼_draw_pdf_funcs.free_instance (); }



/**
 * hb_vector_draw_create_or_fail:
 * @format: output format.
 *
 * Creates a new draw context for vector output.
 *
 * Return value: (nullable): a newly allocated #hb_vector_draw_t, or `NULL` on failure.
 *
 * Since: 13.0.0
 */
hb_ਸਦਿਸ਼_draw_t *
hb_ਸਦਿਸ਼_draw_create_or_fail (hb_ਸਦਿਸ਼_format_t format)
{
  ਚੋਣ (format)
  {
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_SVG:
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_PDF:
      ਰੋਕੋ;
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_INVALID: ਮੂਲ:
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }

  hb_ਸਦਿਸ਼_draw_t *draw = hb_object_create<hb_ਸਦਿਸ਼_draw_t> ();
  ਜੇ (unlikely (!draw))
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  draw->format = format;
  draw->defs.alloc (2048);
  draw->body.alloc (8192);
  draw->ਰਸਤਾ.alloc (2048);
  ਵਾਪਸ draw;
}

/**
 * hb_vector_draw_reference:
 * @draw: a draw context.
 *
 * Increases the reference count of @draw.
 *
 * Return value: (transfer full): referenced @draw.
 *
 * Since: 13.0.0
 */
hb_ਸਦਿਸ਼_draw_t *
hb_ਸਦਿਸ਼_draw_reference (hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਵਾਪਸ hb_object_reference (draw);
}

/**
 * hb_vector_draw_destroy:
 * @draw: a draw context.
 *
 * Decreases the reference count of @draw and destroys it when it reaches zero.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_destroy (hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਜੇ (!hb_object_should_destroy (draw))
    ਵਾਪਸ;

  hb_blob_destroy (draw->recycled_blob);
  hb_object_actually_destroy (draw);
  hb_free (draw);
}

/**
 * hb_vector_draw_set_user_data:
 * @draw: a draw context.
 * @key: user-data key.
 * @data: user-data value.
 * @destroy: (nullable): destroy callback for @data.
 * @replace: whether to replace an existing value for @key.
 *
 * Attaches user data to @draw.
 *
 * Return value: `true` on success, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_user_ਅੰਕੜੇ (hb_ਸਦਿਸ਼_draw_t   *draw,
                              hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key,
                              ਖਾਲੀ               *ਅੰਕੜੇ,
                              hb_destroy_func_t   destroy,
                              hb_bool_t           replace)
{
  ਵਾਪਸ hb_object_ਸੈੱਟ_user_ਅੰਕੜੇ (draw, key, ਅੰਕੜੇ, destroy, replace);
}

/**
 * hb_vector_draw_get_user_data:
 * @draw: a draw context.
 * @key: user-data key.
 *
 * Gets previously attached user data from @draw.
 *
 * Return value: (nullable): user-data value associated with @key.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ *
hb_ਸਦਿਸ਼_draw_get_user_ਅੰਕੜੇ (ਅਚਲ hb_ਸਦਿਸ਼_draw_t   *draw,
                              hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t *key)
{
  ਵਾਪਸ hb_object_get_user_ਅੰਕੜੇ (draw, key);
}

/**
 * hb_vector_draw_set_transform:
 * @draw: a draw context.
 * @xx: transform xx component.
 * @yx: transform yx component.
 * @xy: transform xy component.
 * @yy: transform yy component.
 * @dx: transform x translation.
 * @dy: transform y translation.
 *
 * Sets the affine transform used when drawing glyphs.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_transform (hb_ਸਦਿਸ਼_draw_t *draw,
                              ਦਸ਼ਮਲਵ xx, ਦਸ਼ਮਲਵ yx,
                              ਦਸ਼ਮਲਵ xy, ਦਸ਼ਮਲਵ yy,
                              ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy)
{
  draw->transform = {xx, yx, xy, yy, dx, dy};
}

/**
 * hb_vector_draw_get_transform:
 * @draw: a draw context.
 * @xx: (out) (nullable): transform xx component.
 * @yx: (out) (nullable): transform yx component.
 * @xy: (out) (nullable): transform xy component.
 * @yy: (out) (nullable): transform yy component.
 * @dx: (out) (nullable): transform x translation.
 * @dy: (out) (nullable): transform y translation.
 *
 * Gets the affine transform used when drawing glyphs.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_get_transform (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw,
                              ਦਸ਼ਮਲਵ *xx, ਦਸ਼ਮਲਵ *yx,
                              ਦਸ਼ਮਲਵ *xy, ਦਸ਼ਮਲਵ *yy,
                              ਦਸ਼ਮਲਵ *dx, ਦਸ਼ਮਲਵ *dy)
{
  ਜੇ (xx) *xx = draw->transform.xx;
  ਜੇ (yx) *yx = draw->transform.yx;
  ਜੇ (xy) *xy = draw->transform.xy;
  ਜੇ (yy) *yy = draw->transform.yy;
  ਜੇ (dx) *dx = draw->transform.x0;
  ਜੇ (dy) *dy = draw->transform.y0;
}

/**
 * hb_vector_draw_set_scale_factor:
 * @draw: a draw context.
 * @x_scale_factor: x scale factor.
 * @y_scale_factor: y scale factor.
 *
 * Sets additional output scaling factors.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_scale_ਗੁਣਕ (hb_ਸਦਿਸ਼_draw_t *draw,
                                 ਦਸ਼ਮਲਵ x_scale_ਗੁਣਕ,
                                 ਦਸ਼ਮਲਵ y_scale_ਗੁਣਕ)
{
  draw->x_scale_ਗੁਣਕ = x_scale_ਗੁਣਕ > 0.f ? x_scale_ਗੁਣਕ : 1.f;
  draw->y_scale_ਗੁਣਕ = y_scale_ਗੁਣਕ > 0.f ? y_scale_ਗੁਣਕ : 1.f;
}

/**
 * hb_vector_draw_get_scale_factor:
 * @draw: a draw context.
 * @x_scale_factor: (out) (nullable): x scale factor.
 * @y_scale_factor: (out) (nullable): y scale factor.
 *
 * Gets additional output scaling factors.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_get_scale_ਗੁਣਕ (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw,
                                 ਦਸ਼ਮਲਵ *x_scale_ਗੁਣਕ,
                                 ਦਸ਼ਮਲਵ *y_scale_ਗੁਣਕ)
{
  ਜੇ (x_scale_ਗੁਣਕ) *x_scale_ਗੁਣਕ = draw->x_scale_ਗੁਣਕ;
  ਜੇ (y_scale_ਗੁਣਕ) *y_scale_ਗੁਣਕ = draw->y_scale_ਗੁਣਕ;
}

/**
 * hb_vector_draw_set_extents:
 * @draw: a draw context.
 * @extents: (nullable): output extents to set or expand.
 *
 * Sets or expands output extents on @draw. Passing `NULL` clears extents.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_extents (hb_ਸਦਿਸ਼_draw_t *draw,
                            ਅਚਲ hb_ਸਦਿਸ਼_extents_t *extents)
{
  ਜੇ (!extents)
  {
    draw->extents = {0, 0, 0, 0};
    draw->has_extents = ਝੂਠ;
    ਵਾਪਸ;
  }

  ਜੇ (extents->ਚੌੜਾਈ == 0.f || extents->ਉਚਾਈ == 0.f)
    ਵਾਪਸ;

  /* Caller-supplied extents are in input-space; divide by
   * scale_factor so they end up in output-space, matching
   * the per-glyph extents accumulated via
   * hb_vector_set_glyph_extents_common (which applies the
   * same divide).  Normalize so origin is the min corner and
   * width/height are positive — callers may pass a
   * glyph-extents-style box with negative height. */
  ਦਸ਼ਮਲਵ x0 = extents->x / draw->x_scale_ਗੁਣਕ;
  ਦਸ਼ਮਲਵ y0 = extents->y / draw->y_scale_ਗੁਣਕ;
  ਦਸ਼ਮਲਵ x1 = x0 + extents->ਚੌੜਾਈ  / draw->x_scale_ਗੁਣਕ;
  ਦਸ਼ਮਲਵ y1 = y0 + extents->ਉਚਾਈ / draw->y_scale_ਗੁਣਕ;
  hb_ਸਦਿਸ਼_extents_t e = {
    hb_min (x0, x1), hb_min (y0, y1),
    fabsf (x1 - x0), fabsf (y1 - y0),
  };

  ਜੇ (draw->has_extents)
  {
    ਦਸ਼ਮਲਵ x0 = hb_min (draw->extents.x, e.x);
    ਦਸ਼ਮਲਵ y0 = hb_min (draw->extents.y, e.y);
    ਦਸ਼ਮਲਵ x1 = hb_max (draw->extents.x + draw->extents.ਚੌੜਾਈ,
                       e.x + e.ਚੌੜਾਈ);
    ਦਸ਼ਮਲਵ y1 = hb_max (draw->extents.y + draw->extents.ਉਚਾਈ,
                       e.y + e.ਉਚਾਈ);
    draw->extents = {x0, y0, x1 - x0, y1 - y0};
  }
  ਨਹੀਂਤਾਂ
  {
    draw->extents = e;
    draw->has_extents = ਸੱਚ;
  }
}

/**
 * hb_vector_draw_get_extents:
 * @draw: a draw context.
 * @extents: (out) (nullable): where to store current output extents.
 *
 * Gets current output extents from @draw.
 *
 * Return value: `true` if extents are set, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ਸਦਿਸ਼_draw_get_extents (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw,
                            hb_ਸਦਿਸ਼_extents_t *extents)
{
  ਜੇ (!draw->has_extents)
    ਵਾਪਸ ਝੂਠ;

  ਜੇ (extents)
    *extents = draw->extents;
  ਵਾਪਸ ਸੱਚ;
}

/**
 * hb_vector_draw_set_glyph_extents:
 * @draw: a draw context.
 * @glyph_extents: glyph extents in font units.
 *
 * Expands @draw extents using @glyph_extents under the current transform.
 *
 * Return value: `true` on success, `false` otherwise.
 *
 * Since: 13.0.0
 */
hb_bool_t
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_glyph_extents (hb_ਸਦਿਸ਼_draw_t *draw,
                                  ਅਚਲ hb_glyph_extents_t *glyph_extents)
{
  hb_bool_t has_extents = draw->has_extents;
  hb_bool_t ret = hb_ਸਦਿਸ਼_ਸੈੱਟ_glyph_extents_common (draw->transform,
						   draw->x_scale_ਗੁਣਕ,
						   draw->y_scale_ਗੁਣਕ,
						   glyph_extents,
						   &draw->extents,
						   &has_extents);
  draw->has_extents = has_extents;
  ਵਾਪਸ ret;
}

/**
 * hb_vector_draw_get_format:
 * @draw: a vector draw context.
 *
 * Gets the output format @draw was created with.
 *
 * Return value: the output format.
 *
 * Since: 14.2.0
 */
hb_ਸਦਿਸ਼_format_t
hb_ਸਦਿਸ਼_draw_get_format (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਵਾਪਸ draw->format;
}

/**
 * hb_vector_draw_get_funcs:
 * @draw: a vector draw context.
 *
 * Gets draw callbacks for feeding outline data into @draw.
 * Pass @draw as the @draw_data argument when calling them.
 *
 * Return value: (transfer none): immutable #hb_draw_funcs_t.
 *
 * Since: 14.2.0
 */
hb_draw_funcs_t *
hb_ਸਦਿਸ਼_draw_get_funcs (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਚੋਣ (draw ? draw->format : HB_ਸਦਿਸ਼_FORMAT_INVALID)
  {
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_SVG: ਵਾਪਸ static_ਸਦਿਸ਼_draw_svg_funcs.get_unconst ();
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_PDF: ਵਾਪਸ static_ਸਦਿਸ਼_draw_pdf_funcs.get_unconst ();
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_INVALID: ਮੂਲ: ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
}

/**
 * hb_vector_draw_new_path:
 * @draw: a draw context.
 *
 * Flushes any pending path and starts a new one.  Call this
 * between glyphs to separate their outlines so fill rules
 * don't interact across glyphs.
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_new_ਰਸਤਾ (hb_ਸਦਿਸ਼_draw_t *draw)
{
  draw->new_ਰਸਤਾ ();
}

/**
 * hb_vector_draw_glyph_or_fail:
 * @draw: a draw context.
 * @font: font object.
 * @glyph: glyph ID.
 * @extents_mode: extents update mode.
 *
 * Convenience to draw one glyph into @draw.  Equivalent to:
 *
 * |[<!-- language="plain" -->
 * // extend extents if requested
 * hb_vector_draw_new_path (draw);
 * hb_font_draw_glyph_or_fail (font, glyph,
 *   hb_vector_draw_get_funcs (draw), draw);
 * ]|
 *
 * Return value: `true` if glyph data was emitted, `false` otherwise.
 *
 * Since: 14.2.0
 */
hb_bool_t
hb_ਸਦਿਸ਼_draw_glyph_or_fail (hb_ਸਦਿਸ਼_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t extents_ਬਹੁਲਕ)
{

  ਜੇ (extents_ਬਹੁਲਕ == HB_ਸਦਿਸ਼_EXTENTS_ਬਹੁਲਕ_EXPAND)
  {
    hb_glyph_extents_t ge;
    ਜੇ (hb_font_get_glyph_extents (font, glyph, &ge))
    {
      hb_bool_t has_extents = draw->has_extents;
      hb_ਸਦਿਸ਼_ਸੈੱਟ_glyph_extents_common (draw->transform,
					  draw->x_scale_ਗੁਣਕ,
					  draw->y_scale_ਗੁਣਕ,
					  &ge,
					  &draw->extents,
					  &has_extents);
      draw->has_extents = has_extents;
    }
  }

  draw->new_ਰਸਤਾ ();
  ਵਾਪਸ hb_font_draw_glyph_or_fail (font, glyph, hb_ਸਦਿਸ਼_draw_get_funcs (draw), draw);
}

/**
 * hb_vector_draw_glyph:
 * @draw: a draw context.
 * @font: font object.
 * @glyph: glyph ID.
 * @extents_mode: extents update mode.
 *
 * Draws one glyph into @draw.  Equivalent to
 * hb_vector_draw_glyph_or_fail() with the return value ignored.
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_glyph (hb_ਸਦਿਸ਼_draw_t *draw,
                      hb_font_t *font,
                      hb_codepoint_t glyph,
                      hb_ਸਦਿਸ਼_extents_ਬਹੁਲਕ_t extents_ਬਹੁਲਕ)
{
  hb_ਸਦਿਸ਼_draw_glyph_or_fail (draw, font, glyph, extents_ਬਹੁਲਕ);
}


/**
 * hb_vector_draw_set_precision:
 * @draw: a draw context.
 * @precision: decimal precision.
 *
 * Sets numeric output precision for draw output.
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_precision (hb_ਸਦਿਸ਼_draw_t *draw,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ precision)
{
  draw->ਸੈੱਟ_precision (precision);
}

/**
 * hb_vector_draw_get_precision:
 * @draw: a draw context.
 *
 * Returns the numeric output precision previously set on @draw,
 * or the default if none was set.
 *
 * Return value: the precision.
 *
 * Since: 14.2.0
 */
ਬਿਨਾਨਿਸ਼ਾਨੀ
hb_ਸਦਿਸ਼_draw_get_precision (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਵਾਪਸ draw->get_precision ();
}

/**
 * hb_vector_draw_set_foreground:
 * @draw: a draw context.
 * @foreground: foreground fill color.
 *
 * Sets the fill color for drawn glyph outlines.
 * Default is opaque black.
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_foreground (hb_ਸਦਿਸ਼_draw_t *draw,
                               hb_color_t foreground)
{
  draw->flush_ਰਸਤਾ ();
  draw->foreground = foreground;
}

/**
 * hb_vector_draw_get_foreground:
 * @draw: a draw context.
 *
 * Returns the foreground fill color.
 *
 * Return value: the foreground color.
 *
 * Since: 14.2.0
 */
hb_color_t
hb_ਸਦਿਸ਼_draw_get_foreground (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਵਾਪਸ draw->foreground;
}

/**
 * hb_vector_draw_set_background:
 * @draw: a draw context.
 * @background: background color.
 *
 * Sets the background color.  If non-transparent, a filled
 * rectangle covering the extents is emitted behind all content.
 * Default is transparent (no background).
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_ਸੈੱਟ_background (hb_ਸਦਿਸ਼_draw_t *draw,
                               hb_color_t background)
{
  draw->background = background;
}

/**
 * hb_vector_draw_get_background:
 * @draw: a draw context.
 *
 * Returns the background color.
 *
 * Return value: the background color.
 *
 * Since: 14.2.0
 */
hb_color_t
hb_ਸਦਿਸ਼_draw_get_background (ਅਚਲ hb_ਸਦਿਸ਼_draw_t *draw)
{
  ਵਾਪਸ draw->background;
}

ਸਥਿਰ hb_blob_t *
hb_ਸਦਿਸ਼_draw_render_pdf (hb_ਸਦਿਸ਼_draw_t *draw)
{
  draw->flush_ਰਸਤਾ ();
  ਜੇ (!draw->has_extents)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  /* Collect the content stream.  The path coordinates are in
   * SVG space (Y-down).  Prepend a CTM that flips Y so the
   * PDF page (Y-up) renders correctly. */
  ਦਸ਼ਮਲਵ ex = draw->extents.x;
  ਦਸ਼ਮਲਵ ey = draw->extents.y;
  ਦਸ਼ਮਲਵ ew = draw->extents.ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ eh = draw->extents.ਉਚਾਈ;

  hb_ਸਦਿਸ਼_buf_t ਧਾਰਾ;
  ਧਾਰਾ.alloc (draw->body.ਲੰਮਾਈ + draw->ਰਸਤਾ.ਲੰਮਾਈ + 256);

  /* Background rect. */
  ਜੇ (hb_color_get_alpha (draw->background))
  {
    ਦਸ਼ਮਲਵ a = hb_color_get_alpha (draw->background) / 255.f;
    ਜੇ (a < 1.f - 1.f / 512.f)
    {
      ਧਾਰਾ.append_ਅੰਕ (a, 4);
      ਧਾਰਾ.append_str (" ca gs\n");
    }
    ਧਾਰਾ.append_ਅੰਕ (hb_color_get_red (draw->background) / 255.f, 4);
    ਧਾਰਾ.append_c (' ');
    ਧਾਰਾ.append_ਅੰਕ (hb_color_get_green (draw->background) / 255.f, 4);
    ਧਾਰਾ.append_c (' ');
    ਧਾਰਾ.append_ਅੰਕ (hb_color_get_blue (draw->background) / 255.f, 4);
    ਧਾਰਾ.append_str (" rg\n");
    ਧਾਰਾ.append_ਅੰਕ (ex);
    ਧਾਰਾ.append_c (' ');
    ਧਾਰਾ.append_ਅੰਕ (-(ey + eh));
    ਧਾਰਾ.append_c (' ');
    ਧਾਰਾ.append_ਅੰਕ (ew);
    ਧਾਰਾ.append_c (' ');
    ਧਾਰਾ.append_ਅੰਕ (eh);
    ਧਾਰਾ.append_str (" re f\n");
  }

  draw->flush_ਰਸਤਾ ();
  ਜੇ (draw->body.ਲੰਮਾਈ)
    ਧਾਰਾ.append_len (draw->body.arrayZ, draw->body.ਲੰਮਾਈ);

  /* Build PDF objects, tracking byte offsets for xref. */
  hb_ਸਦਿਸ਼_buf_t ਬਾਹਰਲਾ;
  hb_buf_recover_recycled (draw->recycled_blob, &ਬਾਹਰਲਾ);
  ਬਾਹਰਲਾ.alloc (ਧਾਰਾ.ਲੰਮਾਈ + 512);

  ਬਿਨਾਨਿਸ਼ਾਨੀ offsets[5]; /* objects 1-4, plus end */

  ਬਾਹਰਲਾ.append_str ("%PDF-1.4\n%\xC0\xC1\xC2\xC3\n");

  /* Object 1: Catalog */
  offsets[0] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("1 0 obj\n<< /Type /Catalog /Pages 2 0 R >>\nendobj\n");

  /* Object 2: Pages */
  offsets[1] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("2 0 obj\n<< /Type /Pages /Kids [3 0 R] /Count 1 >>\nendobj\n");

  /* Object 3: Page.  Extents are in SVG space (y = -font_y).
   * Convert back: font Y range = [-(ey+eh) .. -ey]. */
  offsets[2] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("3 0 obj\n<< /Type /Page /Parent 2 0 R /MediaBox [");
  ਬਾਹਰਲਾ.append_ਅੰਕ (ex);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (-(ey + eh));
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (ex + ew);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (-ey);
  ਬਾਹਰਲਾ.append_str ("] /Contents 4 0 R");
  ਜੇ (draw->pdf_extgstate_dict.ਲੰਮਾਈ)
  {
    ਬਾਹਰਲਾ.append_str (" /Resources << /ExtGState << ");
    ਬਾਹਰਲਾ.append_len (draw->pdf_extgstate_dict.arrayZ, draw->pdf_extgstate_dict.ਲੰਮਾਈ);
    ਬਾਹਰਲਾ.append_str (">> >>");
  }
  ਬਾਹਰਲਾ.append_str (" >>\nendobj\n");

  /* Object 4: Content stream */
  offsets[3] = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("4 0 obj\n<< /Length ");
  ਬਾਹਰਲਾ.append_unsigned (ਧਾਰਾ.ਲੰਮਾਈ);
  ਬਾਹਰਲਾ.append_str (" >>\nstream\n");
  ਬਾਹਰਲਾ.append_len (ਧਾਰਾ.arrayZ, ਧਾਰਾ.ਲੰਮਾਈ);
  ਬਾਹਰਲਾ.append_str ("endstream\nendobj\n");

  /* Cross-reference table */
  ਬਿਨਾਨਿਸ਼ਾਨੀ xref_ਵਿੱਥ = ਬਾਹਰਲਾ.ਲੰਮਾਈ;
  ਬਾਹਰਲਾ.append_str ("xref\n0 5\n");
  ਬਾਹਰਲਾ.append_str ("0000000000 65535 f \n");
  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < 4; i++)
  {
    ਅੱਖਰ tmp[21];
    snprintf (tmp, ਆਕਾਰ (tmp), "%010u 00000 n \n", offsets[i]);
    ਬਾਹਰਲਾ.append_len (tmp, 20);
  }

  /* Trailer */
  ਬਾਹਰਲਾ.append_str ("trailer\n<< /Size 5 /Root 1 0 R >>\nstartxref\n");
  ਬਾਹਰਲਾ.append_unsigned (xref_ਵਿੱਥ);
  ਬਾਹਰਲਾ.append_str ("\n%%EOF\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &ਬਾਹਰਲਾ);

  ਵਾਪਸ blob;
}

ਸਥਿਰ hb_blob_t *
hb_ਸਦਿਸ਼_draw_render_svg (hb_ਸਦਿਸ਼_draw_t *draw)
{
  draw->flush_ਰਸਤਾ ();
  ਜੇ (!draw->has_extents)
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

  hb_ਸਦਿਸ਼_buf_t ਬਾਹਰਲਾ;
  hb_buf_recover_recycled (draw->recycled_blob, &ਬਾਹਰਲਾ);
  ਬਿਨਾਨਿਸ਼ਾਨੀ estimated = draw->defs.ਲੰਮਾਈ +
		       (draw->body.ਲੰਮਾਈ ? draw->body.ਲੰਮਾਈ : draw->ਰਸਤਾ.ਲੰਮਾਈ) +
		       256;
  ਬਾਹਰਲਾ.alloc (estimated);
  /* Extents are in Y-up (font) space.  SVG is Y-down.
   * The global scale(1,-1) wrapper flips content; the
   * viewBox maps the flipped range. */
  ਦਸ਼ਮਲਵ vb_x = draw->extents.x;
  ਦਸ਼ਮਲਵ vb_y = -(draw->extents.y + draw->extents.ਉਚਾਈ);
  ਦਸ਼ਮਲਵ vb_w = draw->extents.ਚੌੜਾਈ;
  ਦਸ਼ਮਲਵ vb_h = draw->extents.ਉਚਾਈ;
  ਬਾਹਰਲਾ.append_str ("<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"");
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_x);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_y);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_w);
  ਬਾਹਰਲਾ.append_c (' ');
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_h);
  ਬਾਹਰਲਾ.append_str ("\" width=\"");
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_w);
  ਬਾਹਰਲਾ.append_str ("\" height=\"");
  ਬਾਹਰਲਾ.append_ਅੰਕ (vb_h);
  ਬਾਹਰਲਾ.append_str ("\">\n");

  ਜੇ (draw->defs.ਲੰਮਾਈ)
  {
    ਬਾਹਰਲਾ.append_str ("<defs>\n");
    ਬਾਹਰਲਾ.append_len (draw->defs.arrayZ, draw->defs.ਲੰਮਾਈ);
    ਬਾਹਰਲਾ.append_str ("</defs>\n");
  }

  /* Background rect. */
  ਜੇ (hb_color_get_alpha (draw->background))
  {
    ਬਾਹਰਲਾ.append_str ("<rect x=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_x);
    ਬਾਹਰਲਾ.append_str ("\" y=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_y);
    ਬਾਹਰਲਾ.append_str ("\" width=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_w);
    ਬਾਹਰਲਾ.append_str ("\" height=\"");
    ਬਾਹਰਲਾ.append_ਅੰਕ (vb_h);
    ਬਾਹਰਲਾ.append_str ("\" fill=\"rgb(");
    ਬਾਹਰਲਾ.append_unsigned (hb_color_get_red (draw->background));
    ਬਾਹਰਲਾ.append_c (',');
    ਬਾਹਰਲਾ.append_unsigned (hb_color_get_green (draw->background));
    ਬਾਹਰਲਾ.append_c (',');
    ਬਾਹਰਲਾ.append_unsigned (hb_color_get_blue (draw->background));
    ਬਾਹਰਲਾ.append_str (")\"");
    ਜੇ (hb_color_get_alpha (draw->background) < 255)
    {
      ਬਾਹਰਲਾ.append_str (" fill-opacity=\"");
      ਬਾਹਰਲਾ.append_ਅੰਕ (hb_color_get_alpha (draw->background) / 255.f, 4);
      ਬਾਹਰਲਾ.append_c ('"');
    }
    ਬਾਹਰਲਾ.append_str ("/>\n");
  }

  draw->flush_ਰਸਤਾ ();
  ਜੇ (draw->body.ਲੰਮਾਈ)
  {
    ਬਾਹਰਲਾ.append_str ("<g transform=\"scale(1,-1)\">\n");
    ਬਾਹਰਲਾ.append_len (draw->body.arrayZ, draw->body.ਲੰਮਾਈ);
    ਬਾਹਰਲਾ.append_str ("</g>\n");
  }

  ਬਾਹਰਲਾ.append_str ("</svg>\n");

  hb_blob_t *blob = hb_buf_blob_from (&draw->recycled_blob, &ਬਾਹਰਲਾ);

  ਵਾਪਸ blob;
}

/**
 * hb_vector_draw_render:
 * @draw: a draw context.
 *
 * Renders accumulated draw content to an output blob.
 * Accumulated content and extents are cleared on return, including
 * when rendering fails. User configuration is preserved.
 *
 * Return value: (transfer full) (nullable): output blob, or `NULL` if rendering cannot proceed.
 *
 * Since: 13.0.0
 */
hb_blob_t *
hb_ਸਦਿਸ਼_draw_render (hb_ਸਦਿਸ਼_draw_t *draw)
{
  HB_SCOPE_GUARD (hb_ਸਦਿਸ਼_draw_clear (draw));
  ਚੋਣ (draw->format)
  {
    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_SVG:
      ਵਾਪਸ hb_ਸਦਿਸ਼_draw_render_svg (draw);

    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_PDF:
      ਵਾਪਸ hb_ਸਦਿਸ਼_draw_render_pdf (draw);

    ਹਾਲਤ HB_ਸਦਿਸ਼_FORMAT_INVALID: ਮੂਲ:
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
  }
}

/**
 * hb_vector_draw_clear:
 * @draw: a draw context.
 *
 * Discards accumulated draw output so @draw can be reused for
 * another render.  User configuration (transform, scale factors,
 * precision, foreground, background, and budget) is preserved.  Call
 * hb_vector_draw_reset() to also reset user configuration to defaults.
 *
 * Since: 14.2.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_clear (hb_ਸਦਿਸ਼_draw_t *draw)
{
  draw->extents = {0, 0, 0, 0};
  draw->has_extents = ਝੂਠ;
  draw->defs.clear ();
  draw->body.clear ();
  draw->ਰਸਤਾ.clear ();
  draw->pdf_extgstate_dict.clear ();
  draw->pdf_extgstate_ਗਿਣਤੀ = 0;
  draw->recharge_budget ();
}

/**
 * hb_vector_draw_reset:
 * @draw: a draw context.
 *
 * Restores user configuration to defaults and clears accumulated content.
 * The output format and attached user data are preserved.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_reset (hb_ਸਦਿਸ਼_draw_t *draw)
{
  draw->transform = {1, 0, 0, 1, 0, 0};
  draw->x_scale_ਗੁਣਕ = 1.f;
  draw->y_scale_ਗੁਣਕ = 1.f;
  draw->foreground = HB_COLOR (0, 0, 0, 255);
  draw->background = HB_COLOR (0, 0, 0, 0);
  draw->ਸੈੱਟ_precision (2);
  draw->budget = HB_BUDGET_DEFAULT;
  hb_ਸਦਿਸ਼_draw_clear (draw);
}

/**
 * hb_vector_draw_recycle_blob:
 * @draw: a draw context.
 * @blob: (transfer full) (nullable): previously rendered blob to recycle.
 *
 * Provides a blob for internal buffer reuse by later render calls.
 * The caller transfers ownership of one reference to @draw and must
 * not destroy that reference after this call. Finish accessing @blob
 * and its data before recycling: later renders may reuse the blob
 * and overwrite its data.
 *
 * Since: 13.0.0
 */
ਖਾਲੀ
hb_ਸਦਿਸ਼_draw_recycle_blob (hb_ਸਦਿਸ਼_draw_t *draw,
                             hb_blob_t *blob)
{
  hb_blob_destroy (draw->recycled_blob);
  draw->recycled_blob = ਖਾਲੀ_ਸੂਚਕ;
  ਜੇ (!blob || blob == hb_blob_get_ਖਾਲੀ_ਥਾਵਾਂ ())
    ਵਾਪਸ;
  draw->recycled_blob = blob;
}

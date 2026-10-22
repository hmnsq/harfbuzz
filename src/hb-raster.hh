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

#ifndef HB_RASTER_HH
#define HB_RASTER_HH

#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-raster.h"

/* Visibility clip box for curve flattening, in device pixels.  Curves
 * whose control-point bounding box falls entirely outside the box are
 * replaced by their chord, which is exact for coverage inside the box.
 * Cleared by hb_raster_draw_clear() (and hence after every render). */
HB_INTERNAL शून्य
hb_raster_draw_set_clip_box (hb_raster_draw_t *draw,
			     भिन्न x0, भिन्न y0,
			     भिन्न x1, भिन्न y1);

/* Estimated sweep work for the accumulated edges: each edge is charged
 * its scanline span plus its column span, both clipped to the surface.
 * Raster paint uses this to precharge its separate pixel budget before
 * rendering a mask. */
HB_INTERNAL int64_t
hb_raster_draw_get_pixel_work (अचर hb_raster_draw_t *draw,
			       अचिन्हित पूर्णाङ्क max_rows,
			       अचिन्हित पूर्णाङ्क max_cols);

/* Shared pixel helpers (used by paint and image compositing). */

स्थिर HB_ALWAYS_INLINE uint8_t
hb_raster_div255 (अचिन्हित a)
{
  यदि (सत्य)
  {
    // An approximation. Slightly faster.
    // https://github.com/linebender/vello/blob/ab58009c8289e83689cd0effc4e34d1c6e8b51f5/sparse_strips/vello_cpu/src/util.rs#L10-L63
    फर्काऊ (a + 255) >> 8;
  }

  फर्काऊ (uint8_t) ((a + 128 + ((a + 128) >> 8)) >> 8);
}

स्थिर HB_ALWAYS_INLINE uint32_t
hb_raster_pack_pixel (uint8_t b, uint8_t g, uint8_t r, uint8_t a)
{
  फर्काऊ (uint32_t) b | ((uint32_t) g << 8) | ((uint32_t) r << 16) | ((uint32_t) a << 24);
}

/* SRC_OVER: premultiplied src over premultiplied dst. */
स्थिर HB_ALWAYS_INLINE uint32_t
hb_raster_src_over (uint32_t src, uint32_t dst)
{
  uint8_t sa = (uint8_t) (src >> 24);
  यदि (sa == 255) फर्काऊ src;
  यदि (sa == 0) फर्काऊ dst;
  अचिन्हित inv_sa = 255 - sa;
  uint8_t rb = hb_raster_div255 ((dst & 0xFF) * inv_sa) + (uint8_t) (src & 0xFF);
  uint8_t rg = hb_raster_div255 (((dst >> 8) & 0xFF) * inv_sa) + (uint8_t) ((src >> 8) & 0xFF);
  uint8_t rr = hb_raster_div255 (((dst >> 16) & 0xFF) * inv_sa) + (uint8_t) ((src >> 16) & 0xFF);
  uint8_t ra = hb_raster_div255 (((dst >> 24) & 0xFF) * inv_sa) + sa;
  फर्काऊ (uint32_t) rb | ((uint32_t) rg << 8) | ((uint32_t) rr << 16) | ((uint32_t) ra << 24);
}

/* Scale a premultiplied pixel by an alpha [0,255]. */
स्थिर HB_ALWAYS_INLINE uint32_t
hb_raster_alpha_mul (uint32_t px, अचिन्हित a)
{
  यदि (a == 255) फर्काऊ px;
  यदि (a == 0) फर्काऊ 0;
  uint8_t rb = hb_raster_div255 ((px & 0xFF) * a);
  uint8_t rg = hb_raster_div255 (((px >> 8) & 0xFF) * a);
  uint8_t rr = hb_raster_div255 (((px >> 16) & 0xFF) * a);
  uint8_t ra = hb_raster_div255 (((px >> 24) & 0xFF) * a);
  फर्काऊ (uint32_t) rb | ((uint32_t) rg << 8) | ((uint32_t) rr << 16) | ((uint32_t) ra << 24);
}

#endif /* HB_RASTER_HH */

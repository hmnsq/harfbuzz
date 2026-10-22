/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2012  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#എങ്കിൽ !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#error "Include <hb.h> instead."
#endif

#ifndef HB_SHAPE_H
#define HB_SHAPE_H

#അന്തര്ഭാവ "hb-common.h"
#അന്തര്ഭാവ "hb-buffer.h"
#അന്തര്ഭാവ "hb-font.h"

HB_BEGIN_DECLS


HB_EXTERN ശൂന്യം
hb_shape (hb_font_t           *font,
	  hb_buffer_t         *buffer,
	  സ്ഥിരം hb_feature_t  *features,
	  unsigned പൂർണ്ണസംഖ്യ         അക്കം_features);

HB_EXTERN hb_bool_t
hb_shape_full (hb_font_t          *font,
	       hb_buffer_t        *buffer,
	       സ്ഥിരം hb_feature_t *features,
	       unsigned പൂർണ്ണസംഖ്യ        അക്കം_features,
	       സ്ഥിരം അക്ഷരം * സ്ഥിരം *shaper_list);

#ifdef HB_EXPERIMENTAL_API
HB_EXTERN hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_buffer_t        *buffer,
		  സ്ഥിരം hb_feature_t *features,
		  unsigned പൂർണ്ണസംഖ്യ        അക്കം_features,
		  സ്ഥിരം അക്ഷരം * സ്ഥിരം *shaper_list,
		  ദശാംശം               min_target_advance,
		  ദശാംശം               max_target_advance,
		  ദശാംശം              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  ദശാംശം              *var_മൂല്യം /* OUT */);
#endif

HB_EXTERN സ്ഥിരം അക്ഷരം **
hb_shape_list_shapers (ശൂന്യം);


HB_END_DECLS

#endif /* HB_SHAPE_H */

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

#अगर !defined(HB_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#त्रुटि "Include <hb.h> instead."
#पूर्ण_अगर

#अगर_अघोषित HB_SHAPE_H
#घोषणा HB_SHAPE_H

#समावेश "hb-common.h"
#समावेश "hb-buffer.h"
#समावेश "hb-font.h"

HB_BEGIN_DECLS


HB_EXTERN व्योम
hb_shape (hb_font_t           *font,
	  hb_बफर_t         *बफर,
	  स्थिर hb_feature_t  *features,
	  अचिन्हित पूर्णांक         अंक_features);

HB_EXTERN hb_bool_t
hb_shape_full (hb_font_t          *font,
	       hb_बफर_t        *बफर,
	       स्थिर hb_feature_t *features,
	       अचिन्हित पूर्णांक        अंक_features,
	       स्थिर अक्षर * स्थिर *shaper_list);

#अगर_घोषित HB_EXPERIMENTAL_API
HB_EXTERN hb_bool_t
hb_shape_justify (hb_font_t          *font,
		  hb_बफर_t        *बफर,
		  स्थिर hb_feature_t *features,
		  अचिन्हित पूर्णांक        अंक_features,
		  स्थिर अक्षर * स्थिर *shaper_list,
		  भग्न               min_target_advance,
		  भग्न               max_target_advance,
		  भग्न              *advance, /* IN/OUT */
		  hb_tag_t           *var_tag, /* OUT */
		  भग्न              *var_मूल्य /* OUT */);
#पूर्ण_अगर

HB_EXTERN स्थिर अक्षर **
hb_shape_list_shapers (व्योम);


HB_END_DECLS

#पूर्ण_अगर /* HB_SHAPE_H */

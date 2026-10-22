/*
 * Copyright © 2013  Red Hat, Inc.
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
 */

#अगर !defined(HB_OT_H_IN) && !defined(HB_NO_SINGLE_HEADER_ERROR)
#त्रुटि "Include <hb-ot.h> instead."
#पूर्ण_अगर

#अगर_अघोषित HB_OT_SHAPE_H
#घोषणा HB_OT_SHAPE_H

#समावेश "hb.h"

HB_BEGIN_DECLS

/* TODO port to shape-plan / set. */
HB_EXTERN व्योम
hb_ot_shape_glyphs_closure (hb_font_t          *font,
			    hb_बफर_t        *बफर,
			    स्थिर hb_feature_t *features,
			    अचिन्हित पूर्णांक        अंक_features,
			    hb_set_t           *glyphs);

HB_EXTERN व्योम
hb_ot_shape_plan_collect_lookups (hb_shape_plan_t *shape_plan,
				  hb_tag_t         table_tag,
				  hb_set_t        *lookup_indexes /* OUT */);

HB_EXTERN अचिन्हित पूर्णांक
hb_ot_shape_plan_get_feature_tags (hb_shape_plan_t *shape_plan,
				   अचिन्हित पूर्णांक     start_offset,
				   अचिन्हित पूर्णांक    *tag_गिनती, /* IN/OUT */
				   hb_tag_t        *tags /* OUT */);

/**
 * HB_OT_SHAPE_BUFFER_FORMAT_SERIAL:
 *
 * The serial number of the current internal buffer format.
 *
 * The serial number will increase when internal #hb_glyph_info_t and
 * #hb_glyph_position_t members change their format.
 *
 * Since: 13.2.0
 */
#घोषणा HB_OT_SHAPE_बफर_FORMAT_SERIAL 1

HB_EXTERN अचिन्हित पूर्णांक
hb_ot_shape_get_बफर_format_serial (व्योम);

HB_END_DECLS

#पूर्ण_अगर /* HB_OT_SHAPE_H */

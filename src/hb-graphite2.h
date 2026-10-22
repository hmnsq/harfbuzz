/*
 * Copyright © 2011  Martin Hosken
 * Copyright © 2011  SIL International
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
 */

#جيڪڏهن_وصف_نه_ٿيل HB_GRAPHITE2_H
#وصف_ڪريو HB_GRAPHITE2_H

#ضمن "hb.h"

#ضمن <graphite2/Font.h>

HB_BEGIN_DECLS

/**
 * HB_GRAPHITE2_TAG_SILF:
 *
 * The #hb_tag_t tag for the `Silf` table, which holds Graphite
 * features. 
 *
 * For more information, see http://graphite.sil.org/
 *
 **/
#وصف_ڪريو HB_GRAPHITE2_ٽيگ_SILF HB_ٽيگ('S','i','l','f')


HB_EXTERN gr_face *
hb_graphite2_face_get_gr_face (hb_face_t *face);

#جيڪڏهن_وصف_نه_ٿيل HB_DISABLE_DEPRECATED

HB_DEPRECATED_FOR (hb_graphite2_face_get_gr_face)
HB_EXTERN gr_font *
hb_graphite2_font_get_gr_font (hb_font_t *font);

#جيڪڏهن_پورو


HB_پڇاڙي_DECLS

#جيڪڏهن_پورو /* HB_GRAPHITE2_H */

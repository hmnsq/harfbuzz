/*
 * Copyright © 2009  Red Hat, Inc.
 * Copyright © 2011  Google, Inc.
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

#నిర్వచితం_కాకపోతే HB_GLIB_H
#నిర్వచించు HB_GLIB_H

#అన్తర్భావ "hb.h"

#అన్తర్భావ <glib.h>

HB_BEGIN_DECLS


HB_EXTERN hb_script_t
hb_glib_script_గమ్యం_script (GUnicodeScript script);

HB_EXTERN GUnicodeScript
hb_glib_script_from_script (hb_script_t script);


HB_EXTERN hb_unicode_funcs_t *
hb_glib_get_unicode_funcs (శూన్య);

#ఒకవేళ GLIB_CHECK_సంస్కరణ(2,31,10)
HB_EXTERN hb_blob_t *
hb_glib_blob_create (GBytes *gbytes);
#అయితే_ముగింపు

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_GLIB_H */

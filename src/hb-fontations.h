/*
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

#నిర్వచితం_కాకపోతే HB_FONTATIONS_H
#నిర్వచించు HB_FONTATIONS_H

#అన్తర్భావ "hb.h"

/**
 * SECTION: hb-fontations
 * @title: hb-fontations
 * @short_description: Fontations integration
 * @include: hb-fontations.h
 *
 * Functions for using HarfBuzz with
 * [Fontations](https://github.com/googlefonts/fontations/) fonts.
 **/

HB_BEGIN_DECLS

/**
 * hb_fontations_font_set_funcs:
 * @font: #hb_font_t to work upon
 *
 * Configures the font-functions structure of the specified #hb_font_t font
 * object to use Fontations font functions.
 *
 * Since: 11.0.0
 **/
HB_EXTERN శూన్య
hb_fontations_font_సమితి_funcs (hb_font_t *font);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_FONTATIONS_H */

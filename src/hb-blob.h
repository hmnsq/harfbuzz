/*
 * Copyright © 2009  Red Hat, Inc.
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

#إذا !defined(HB_H_دننه) && !defined(HB_NO_SINGLE_سرلیک_ERROR)
#تېروتنه "Include <hb.h> instead."
#د_که_پای

#که_نه_تعریف_شوی HB_BLOB_H
#تعریف_کړه HB_BLOB_H

#ضمن "hb-common.h"

HB_BEGIN_DECLS


/**
 * hb_memory_mode_t:
 * @HB_MEMORY_MODE_DUPLICATE: HarfBuzz immediately makes a copy of the data.
 * @HB_MEMORY_MODE_READONLY: HarfBuzz client will never modify the data,
 *     and HarfBuzz will never modify the data.
 * @HB_MEMORY_MODE_WRITABLE: HarfBuzz client made a copy of the data solely
 *     for HarfBuzz, so HarfBuzz may modify the data.
 * @HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE: See above
 *
 * Data type holding the memory modes available to
 * client programs.
 *
 * Regarding these various memory-modes:
 *
 * - In no case shall the HarfBuzz client modify memory
 *   that is passed to HarfBuzz in a blob.  If there is
 *   any such possibility, @HB_MEMORY_MODE_DUPLICATE should be used
 *   such that HarfBuzz makes a copy immediately,
 *
 * - Use @HB_MEMORY_MODE_READONLY otherwise, unless you really really
 *   really know what you are doing,
 *
 * - @HB_MEMORY_MODE_WRITABLE is appropriate if you really made a
 *   copy of data solely for the purpose of passing to
 *   HarfBuzz and doing that just once (no reuse!),
 *
 * - If the font is mmap()ed, it's okay to use
 *   @HB_MEMORY_MODE_READONLY_MAY_MAKE_WRITABLE, however, using that mode
 *   correctly is very tricky.  Use @HB_MEMORY_MODE_READONLY instead.
 **/
تعريفنوع تعداد {
  HB_حافظه_ډېرپېښ_DUPLICATE,
  HB_حافظه_ډېرپېښ_READONLY,
  HB_حافظه_ډېرپېښ_WRITABLE,
  HB_حافظه_ډېرپېښ_READONLY_MAY_MAKE_WRITABLE
} hb_حافظه_ډېرپېښ_t;

/**
 * hb_blob_t:
 *
 * Data type for blobs. A blob wraps a chunk of binary
 * data and facilitates its lifecycle management between
 * a client program and HarfBuzz.
 *
 **/
تعريفنوع بنية hb_blob_t hb_blob_t;

HB_EXTERN hb_blob_t *
hb_blob_create (ثابت حرف        *معلومات,
		بینښه صحيح       اوږدتیا,
		hb_حافظه_ډېرپېښ_t   ډېرپېښ,
		فراغ              *user_معلومات,
		hb_destroy_func_t  destroy);

HB_EXTERN hb_blob_t *
hb_blob_create_or_fail (ثابت حرف        *معلومات,
			بینښه صحيح       اوږدتیا,
			hb_حافظه_ډېرپېښ_t   ډېرپېښ,
			فراغ              *user_معلومات,
			hb_destroy_func_t  destroy);

HB_EXTERN hb_blob_t *
hb_blob_create_from_file (ثابت حرف *file_name);

HB_EXTERN hb_blob_t *
hb_blob_create_from_file_or_fail (ثابت حرف *file_name);

/* Always creates with MEMORY_MODE_READONLY.
 * Even if the parent blob is writable, we don't
 * want the user of the sub-blob to be able to
 * modify the parent data as that data may be
 * shared among multiple sub-blobs.
 */
HB_EXTERN hb_blob_t *
hb_blob_create_sub_blob (hb_blob_t    *پلار,
			 بینښه صحيح  دننه_واټن,
			 بینښه صحيح  اوږدتیا);

HB_EXTERN hb_blob_t *
hb_blob_copy_writable_or_fail (hb_blob_t *blob);

HB_EXTERN hb_blob_t *
hb_blob_get_تشه (فراغ);

HB_EXTERN hb_blob_t *
hb_blob_reference (hb_blob_t *blob);

HB_EXTERN فراغ
hb_blob_destroy (hb_blob_t *blob);

HB_EXTERN hb_bool_t
hb_blob_سېټ_user_معلومات (hb_blob_t          *blob,
		       hb_user_معلومات_کلی_t *کلی_2,
		       فراغ *              معلومات,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);


HB_EXTERN فراغ *
hb_blob_get_user_معلومات (ثابت hb_blob_t    *blob,
		       hb_user_معلومات_کلی_t *کلی_2);


HB_EXTERN فراغ
hb_blob_make_immutable (hb_blob_t *blob);

HB_EXTERN hb_bool_t
hb_blob_is_immutable (hb_blob_t *blob);


HB_EXTERN بینښه صحيح
hb_blob_get_اوږدتیا (hb_blob_t *blob);

HB_EXTERN ثابت حرف *
hb_blob_get_معلومات (hb_blob_t *blob, بینښه صحيح *اوږدتیا);

HB_EXTERN حرف *
hb_blob_get_معلومات_writable (hb_blob_t *blob, بینښه صحيح *اوږدتیا);

HB_پای_ټکی_DECLS

#د_که_پای /* HB_BLOB_H */

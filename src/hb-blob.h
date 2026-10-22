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

#ఒకవేళ !defined(HB_H_లోపల) && !defined(HB_NO_SINGLE_శీర్షిక_ERROR)
#దోషం "Include <hb.h> instead."
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_BLOB_H
#నిర్వచించు HB_BLOB_H

#అన్తర్భావ "hb-common.h"

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
రకనిర్వచనం గణన {
  HB_జ్ఞాపకం_బహుళకం_DUPLICATE,
  HB_జ్ఞాపకం_బహుళకం_READONLY,
  HB_జ్ఞాపకం_బహుళకం_WRITABLE,
  HB_జ్ఞాపకం_బహుళకం_READONLY_MAY_MAKE_WRITABLE
} hb_జ్ఞాపకం_బహుళకం_t;

/**
 * hb_blob_t:
 *
 * Data type for blobs. A blob wraps a chunk of binary
 * data and facilitates its lifecycle management between
 * a client program and HarfBuzz.
 *
 **/
రకనిర్వచనం నిర్మాణం hb_blob_t hb_blob_t;

HB_EXTERN hb_blob_t *
hb_blob_create (స్థిరరాశి అక్షరం        *దత్తాంశం,
		గుర్తులేని పూర్ణాంకం       నిడివి,
		hb_జ్ఞాపకం_బహుళకం_t   బహుళకం,
		శూన్య              *user_దత్తాంశం,
		hb_destroy_func_t  destroy);

HB_EXTERN hb_blob_t *
hb_blob_create_or_fail (స్థిరరాశి అక్షరం        *దత్తాంశం,
			గుర్తులేని పూర్ణాంకం       నిడివి,
			hb_జ్ఞాపకం_బహుళకం_t   బహుళకం,
			శూన్య              *user_దత్తాంశం,
			hb_destroy_func_t  destroy);

HB_EXTERN hb_blob_t *
hb_blob_create_from_file (స్థిరరాశి అక్షరం *file_name);

HB_EXTERN hb_blob_t *
hb_blob_create_from_file_or_fail (స్థిరరాశి అక్షరం *file_name);

/* Always creates with MEMORY_MODE_READONLY.
 * Even if the parent blob is writable, we don't
 * want the user of the sub-blob to be able to
 * modify the parent data as that data may be
 * shared among multiple sub-blobs.
 */
HB_EXTERN hb_blob_t *
hb_blob_create_sub_blob (hb_blob_t    *మాతృ,
			 గుర్తులేని పూర్ణాంకం  స్థానభ్రంశం,
			 గుర్తులేని పూర్ణాంకం  నిడివి);

HB_EXTERN hb_blob_t *
hb_blob_copy_writable_or_fail (hb_blob_t *blob);

HB_EXTERN hb_blob_t *
hb_blob_get_ఖాళీలు (శూన్య);

HB_EXTERN hb_blob_t *
hb_blob_reference (hb_blob_t *blob);

HB_EXTERN శూన్య
hb_blob_destroy (hb_blob_t *blob);

HB_EXTERN hb_bool_t
hb_blob_సమితి_user_దత్తాంశం (hb_blob_t          *blob,
		       hb_user_దత్తాంశం_కీలకం_t *కీలకం_2,
		       శూన్య *              దత్తాంశం,
		       hb_destroy_func_t   destroy,
		       hb_bool_t           replace);


HB_EXTERN శూన్య *
hb_blob_get_user_దత్తాంశం (స్థిరరాశి hb_blob_t    *blob,
		       hb_user_దత్తాంశం_కీలకం_t *కీలకం_2);


HB_EXTERN శూన్య
hb_blob_make_immutable (hb_blob_t *blob);

HB_EXTERN hb_bool_t
hb_blob_is_immutable (hb_blob_t *blob);


HB_EXTERN గుర్తులేని పూర్ణాంకం
hb_blob_get_నిడివి (hb_blob_t *blob);

HB_EXTERN స్థిరరాశి అక్షరం *
hb_blob_get_దత్తాంశం (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం *నిడివి);

HB_EXTERN అక్షరం *
hb_blob_get_దత్తాంశం_writable (hb_blob_t *blob, గుర్తులేని పూర్ణాంకం *నిడివి);

HB_చివర_DECLS

#అయితే_ముగింపు /* HB_BLOB_H */

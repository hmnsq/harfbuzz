/*
 * Copyright © 2022  Google, Inc.
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
 */

#ضمن "hb.hh"

#ضمن "hb-subset-serialize.h"
#ضمن "hb-repacker.hh"

/**
 * hb_subset_serialize_or_fail:
 * @table_tag: tag of the table being packed, needed to allow table specific optimizations.
 * @hb_objects: raw array of struct hb_subset_serialize_object_t, which provides
 * object graph info
 * @num_hb_objs: number of hb_subset_serialize_object_t in the hb_objects array.
 *
 * Given the input object graph info, repack a table to eliminate offset overflows and
 * serialize it into a continuous array of bytes. A nullptr is returned if the serializing attempt fails.
 * Table specific optimizations (eg. extension promotion in GSUB/GPOS) may be performed.
 * Passing HB_TAG_NONE will disable table specific optimizations.
 *
 * Since: 10.2.0
 **/
HB_EXTERN hb_blob_t *
hb_subset_serialize_or_fail (hb_برچسب_t                      جدول_برچسب_2,
                             hb_subset_serialize_object_t *hb_objects,
                             بیعلامت                      رقم_hb_objs)
{
  hb_وکتور_t<ثابت hb_subset_serialize_object_t *> packed;
  packed.alloc (رقم_hb_objs + 1);
  packed.push (اشاره_تهی);
  لكل (بیعلامت i = 0 ; i < رقم_hb_objs ; i++)
    packed.push (&(hb_objects[i]));

  أرجع hb_resolve_overflows (packed, جدول_برچسب_2, 20, درست);
}

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_SERIALIZE_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_SERIALIZE_H

#ਜੇ !defined(HB_SUBSET_H_ਵਿੱਚ) && !defined(HB_NO_SINGLE_ਸਿਰਲੇਖ_ERROR)
// Disabled to not break pre-existing users.
//#error "Include <hb-subset.h> instead."
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.h"

HB_BEGIN_DECLS

/**
 * hb_subset_serialize_link_t:
 * @width: offsetSize in bytes
 * @position: position of the offset field in bytes from
 *            beginning of subtable
 * @objidx: index of subtable
 *
 * Represents a link between two objects in the object graph
 * to be serialized.
 *
 * Since: 10.2.0
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_subset_serialize_link_t {
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ position;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ objidx;
} hb_subset_serialize_link_t;

/**
 * hb_subset_serialize_object_t:
 * @head: start of object data
 * @tail: end of object data
 * @num_real_links: number of offset field in the object
 * @real_links: array of offset info
 * @num_virtual_links: number of objects that must be packed
 *                     after current object in the final
 *                     serialized order
 * @virtual_links: array of virtual link info
 *
 * Represents an object in the object graph to be serialized.
 *
 * Since: 10.2.0
 */
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_subset_serialize_object_t {
  ਅੱਖਰ *ਸਿਰ_2;
  ਅੱਖਰ *ਪੂਛ;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_real_links;
  hb_subset_serialize_link_t *real_links;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ_virtual_links;
  hb_subset_serialize_link_t *virtual_links;
} hb_subset_serialize_object_t;

HB_EXTERN hb_blob_t *
hb_subset_serialize_or_fail (hb_ਟੈਗ_t                      ਸਾਰਣੀ_ਟੈਗ_2,
                             hb_subset_serialize_object_t *hb_objects,
                             ਬਿਨਾਨਿਸ਼ਾਨੀ                      ਅੰਕ_hb_objs);


HB_ਸਿਰਾ_DECLS

#ਜੇ_ਅੰਤ /* HB_SUBSET_SERIALIZE_H */

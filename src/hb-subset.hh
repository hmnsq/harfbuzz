/*
 * Copyright © 2018  Google, Inc.
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
 * Google Author(s): Garret Rieger, Roderick Sheeter
 */

#جيڪڏهن_وصف_نه_ٿيل HB_SUBSET_HH
#وصف_ڪريو HB_SUBSET_HH


#ضمن "hb.hh"

#ضمن "hb-subset.h"

#ضمن "hb-machinery.hh"
#ضمن "hb-serialize.hh"
#ضمن "hb-subset-input.hh"
#ضمن "hb-subset-plan.hh"

بنية hb_subset_context_t :
       hb_dispatch_context_t<hb_subset_context_t, منطقي, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { أرجع "SUBSET"; }
  ساكن return_t default_return_قدر () { أرجع سچ; }

  خاص:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.subset (هذا, std::forward<Ts> (ds)...) )
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  _dispatch (ثابت T &obj, hb_ترجيح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  سانچو <قسم_نالو T, قسم_نالو ...Ts> خودڪار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_blob_t *ذريعو_blob;
  hb_subset_plan_t *plan;
  hb_serialize_context_t *serializer;
  hb_ٽيگ_t جدول_ٽيگ_2;

  hb_subset_context_t (hb_blob_t *ذريعو_blob_2,
		       hb_subset_plan_t *plan_,
		       hb_serialize_context_t *serializer_,
		       hb_ٽيگ_t جدول_ٽيگ_3) :
		        ذريعو_blob (ذريعو_blob_2),
			plan (plan_),
			serializer (serializer_),
			جدول_ٽيگ_2 (جدول_ٽيگ_3) {}
};

#جيڪڏهن_پورو /* HB_SUBSET_HH */

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

#که_نه_تعریف_شوی HB_SUBSET_HH
#تعریف_کړه HB_SUBSET_HH


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
  ساكن return_t default_return_ارزښت () { أرجع ریښتیا; }

  خاص:
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  _dispatch (ثابت T &obj, hb_لومړیتوب<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.subset (هذا, std::forward<Ts> (ds)...) )
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  _dispatch (ثابت T &obj, hb_لومړیتوب<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (هذا, std::forward<Ts> (ds)...) )
  عام:
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts> خپلکاری
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_blob_t *سرچینه_blob;
  hb_subset_plan_t *plan;
  hb_serialize_context_t *serializer;
  hb_نښان_t جدول_نښان_2;

  hb_subset_context_t (hb_blob_t *سرچینه_blob_2,
		       hb_subset_plan_t *plan_,
		       hb_serialize_context_t *serializer_,
		       hb_نښان_t جدول_نښان_3) :
		        سرچینه_blob (سرچینه_blob_2),
			plan (plan_),
			serializer (serializer_),
			جدول_نښان_2 (جدول_نښان_3) {}
};

#د_که_پای /* HB_SUBSET_HH */

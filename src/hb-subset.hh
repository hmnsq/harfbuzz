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

#اگر_غیر_تعریف_شدہ HB_SUBSET_HH
#تعریف_کرو HB_SUBSET_HH


#شامل "hb.hh"

#شامل "hb-subset.h"

#شامل "hb-machinery.hh"
#شامل "hb-serialize.hh"
#شامل "hb-subset-input.hh"
#شامل "hb-subset-plan.hh"

ساخت hb_subset_context_t :
       hb_dispatch_context_t<hb_subset_context_t, منطقی, HB_DEBUG_SUBSET>
{
  ثابت حرف *get_name () { واپس "SUBSET"; }
  ساکن return_t default_return_قدر () { واپس سچ; }

  نجی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.subset (یہ, std::forward<Ts> (ds)...) )
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  _dispatch (ثابت T &obj, hb_ترجیح<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (یہ, std::forward<Ts> (ds)...) )
  عوامی:
  سانچہ <قسم_نام T, قسم_نام ...Ts> خودکار
  dispatch (ثابت T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_blob_t *ماخذ_blob;
  hb_subset_plan_t *plan;
  hb_serialize_context_t *serializer;
  hb_لیبل_t جدول_لیبل_2;

  hb_subset_context_t (hb_blob_t *ماخذ_blob_2,
		       hb_subset_plan_t *plan_,
		       hb_serialize_context_t *serializer_,
		       hb_لیبل_t جدول_لیبل_3) :
		        ماخذ_blob (ماخذ_blob_2),
			plan (plan_),
			serializer (serializer_),
			جدول_لیبل_2 (جدول_لیبل_3) {}
};

#اگر_ختم /* HB_SUBSET_HH */

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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-serialize.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-input.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-plan.hh"

ਬਣਤਰ hb_subset_context_t :
       hb_dispatch_context_t<hb_subset_context_t, ਤਾਰਕਿਕ, HB_DEBUG_SUBSET>
{
  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "SUBSET"; }
  ਸਥਿਰ return_t default_return_ਮੁੱਲ () { ਵਾਪਸ ਸੱਚ; }

  ਨਿਜੀ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.subset (ਇਹ, std::forward<Ts> (ds)...) )
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  _dispatch (ਅਚਲ T &obj, hb_ਤਰਜੀਹ<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ਇਹ, std::forward<Ts> (ds)...) )
  ਜਨਤਕ:
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts> ਸਵੈਚਾਲਿਤ
  dispatch (ਅਚਲ T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_blob_t *ਸਰੋਤ_blob;
  hb_subset_plan_t *plan;
  hb_serialize_context_t *serializer;
  hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_2;

  hb_subset_context_t (hb_blob_t *ਸਰੋਤ_blob_2,
		       hb_subset_plan_t *plan_,
		       hb_serialize_context_t *serializer_,
		       hb_ਟੈਗ_t ਸਾਰਣੀ_ਟੈਗ_3) :
		        ਸਰੋਤ_blob (ਸਰੋਤ_blob_2),
			plan (plan_),
			serializer (serializer_),
			ਸਾਰਣੀ_ਟੈਗ_2 (ਸਾਰਣੀ_ਟੈਗ_3) {}
};

#ਜੇ_ਅੰਤ /* HB_SUBSET_HH */

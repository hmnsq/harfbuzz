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

#ifndef HB_SUBSET_HH
#define HB_SUBSET_HH


#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-subset.h"

#അന്തര്ഭാവ "hb-machinery.hh"
#അന്തര്ഭാവ "hb-serialize.hh"
#അന്തര്ഭാവ "hb-subset-input.hh"
#അന്തര്ഭാവ "hb-subset-plan.hh"

ഘടന hb_subset_context_t :
       hb_dispatch_context_t<hb_subset_context_t, തർക്കം, HB_DEBUG_SUBSET>
{
  സ്ഥിരം അക്ഷരം *get_name () { തിരികെ "SUBSET"; }
  സ്ഥിതം return_t default_return_മൂല്യം () { തിരികെ സത്യം; }

  സ്വകാര്യ:
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<1>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.subset (ഇത്, std::forward<Ts> (ds)...) )
  template <typename T, typename ...Ts> auto
  _dispatch (സ്ഥിരം T &obj, hb_priority<0>, Ts&&... ds) HB_AUTO_RETURN
  ( obj.dispatch (ഇത്, std::forward<Ts> (ds)...) )
  പൊതു:
  template <typename T, typename ...Ts> auto
  dispatch (സ്ഥിരം T &obj, Ts&&... ds) HB_AUTO_RETURN
  ( _dispatch (obj, hb_prioritize, std::forward<Ts> (ds)...) )

  hb_blob_t *source_blob;
  hb_subset_plan_t *plan;
  hb_serialize_context_t *serializer;
  hb_tag_t table_tag;

  hb_subset_context_t (hb_blob_t *source_blob_,
		       hb_subset_plan_t *plan_,
		       hb_serialize_context_t *serializer_,
		       hb_tag_t table_tag_) :
		        source_blob (source_blob_),
			plan (plan_),
			serializer (serializer_),
			table_tag (table_tag_) {}
};

#endif /* HB_SUBSET_HH */

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

#ifndef HB_SUBSET_ନିବେଶ_HH
#define HB_SUBSET_ନିବେଶ_HH


#ଅନ୍ତର୍ଭାଵ "hb.hh"

#ଅନ୍ତର୍ଭାଵ "hb-subset.h"
#ଅନ୍ତର୍ଭାଵ "hb-map.hh"
#ଅନ୍ତର୍ଭାଵ "hb-set.hh"
#ଅନ୍ତର୍ଭାଵ "hb-cplusplus.hh"
#ଅନ୍ତର୍ଭାଵ "hb-font.hh"
#ଅନ୍ତର୍ଭାଵ "hb-subset-instancer-solver.hh"

ସଂରଚନା hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = ପୂର୍ବନିର୍ଦ୍ଧାରିତ;
  hb_ot_name_record_ids_t (unsigned platform_id_,
                           unsigned encoding_id_,
                           unsigned language_id_,
                           unsigned name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  ତାର୍କିକ operator != (ଅଚଳ hb_ot_name_record_ids_t o) ଅଚଳ
  { ଫେରନ୍ତୁ !(*ଏହି == o); }

  ଧାଡ଼ିଭିତରେ ତାର୍କିକ operator == (ଅଚଳ hb_ot_name_record_ids_t& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  ଧାଡ଼ିଭିତରେ uint32_t hash () ଅଚଳ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    ଫେରନ୍ତୁ current;
  }

  unsigned platform_id;
  unsigned encoding_id;
  unsigned language_id;
  unsigned name_id;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

ସଂରଚନା hb_subset_ନିବେଶ_t
{
  HB_INTERNAL hb_subset_ନିବେଶ_t ();

  ~hb_subset_ନିବେଶ_t ()
  {
    sets.~sets_t ();

#ifdef HB_EXPERIMENTAL_API
    ପାଇଁ (auto _ : name_table_overrides.values ())
      _.fini ();
#endif
  }

  hb_object_header_t header;

  ସଂରଚନା sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  ସଂଘ {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[ଆକାର (sets_t) / ଆକାର (hb_set_t*)];
  };

  unsigned flags;
  ତାର୍କିକ attach_accelerator_data = ମିଥ୍ୟା;

  // If set loca format will always be the long version.
  ତାର୍କିକ force_long_loca = ମିଥ୍ୟା;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#ifdef HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#endif

  ଧାଡ଼ିଭିତରେ unsigned ଅଙ୍କ_sets () ଅଚଳ
  {
    ଫେରନ୍ତୁ ଆକାର (set_ptrs) / ଆକାର (hb_set_t*);
  }

  ଧାଡ଼ିଭିତରେ hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    ଫେରନ୍ତୁ hb_array (set_ptrs);
  }

  ତାର୍କିକ in_error () ଅଚଳ
  {
    ପାଇଁ (unsigned i = 0; i < ଅଙ୍କ_sets (); i++)
    {
      ଯଦି (unlikely (set_ptrs[i]->in_error ()))
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }

    ଫେରନ୍ତୁ axes_location.in_error ()
#ifdef HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#endif
	;
  }
};


#endif /* HB_SUBSET_INPUT_HH */

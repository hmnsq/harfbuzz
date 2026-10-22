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

#ifndef HB_SUBSET_প্ৰবেশ_HH
#define HB_SUBSET_প্ৰবেশ_HH


#অন্তর্ভাৱ "hb.hh"

#অন্তর্ভাৱ "hb-subset.h"
#অন্তর্ভাৱ "hb-map.hh"
#অন্তর্ভাৱ "hb-set.hh"
#অন্তর্ভাৱ "hb-cplusplus.hh"
#অন্তর্ভাৱ "hb-font.hh"
#অন্তর্ভাৱ "hb-subset-instancer-solver.hh"

সংরচনা hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = স্ৱভাৱ;
  hb_ot_name_record_ids_t (unsigned platform_id_,
                           unsigned encoding_id_,
                           unsigned language_id_,
                           unsigned name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  তার্কিক operator != (ধ্রুৱ hb_ot_name_record_ids_t o) ধ্রুৱ
  { ঘূৰাই !(*এতত্ == o); }

  অন্তঃস্থ তার্কিক operator == (ধ্রুৱ hb_ot_name_record_ids_t& o) ধ্রুৱ
  {
    ঘূৰাই platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  অন্তঃস্থ uint32_t hash () ধ্রুৱ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    ঘূৰাই current;
  }

  unsigned platform_id;
  unsigned encoding_id;
  unsigned language_id;
  unsigned name_id;
};

প্রকারনির্দেশ সংরচনা hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

সংরচনা hb_subset_প্ৰবেশ_t
{
  HB_INTERNAL hb_subset_প্ৰবেশ_t ();

  ~hb_subset_প্ৰবেশ_t ()
  {
    sets.~sets_t ();

#ifdef HB_EXPERIMENTAL_API
    বাবে (auto _ : name_table_overrides.values ())
      _.fini ();
#endif
  }

  hb_object_header_t header;

  সংরচনা sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  সঙ্ঘ {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[পরিমাণ (sets_t) / পরিমাণ (hb_set_t*)];
  };

  unsigned flags;
  তার্কিক attach_accelerator_data = মিছা;

  // If set loca format will always be the long version.
  তার্কিক force_long_loca = মিছা;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#ifdef HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#endif

  অন্তঃস্থ unsigned অঙ্ক_sets () ধ্রুৱ
  {
    ঘূৰাই পরিমাণ (set_ptrs) / পরিমাণ (hb_set_t*);
  }

  অন্তঃস্থ hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    ঘূৰাই hb_array (set_ptrs);
  }

  তার্কিক in_error () ধ্রুৱ
  {
    বাবে (unsigned i = 0; i < অঙ্ক_sets (); i++)
    {
      যদি (unlikely (set_ptrs[i]->in_error ()))
        ঘূৰাই সঁচা;
    }

    ঘূৰাই axes_location.in_error ()
#ifdef HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#endif
	;
  }
};


#endif /* HB_SUBSET_INPUT_HH */

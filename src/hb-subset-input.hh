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

#ifndef HB_SUBSET_ಒಳಹರಿವು_HH
#define HB_SUBSET_ಒಳಹರಿವು_HH


#ಅನ್ತರ್ಭಾವ "hb.hh"

#ಅನ್ತರ್ಭಾವ "hb-subset.h"
#ಅನ್ತರ್ಭಾವ "hb-map.hh"
#ಅನ್ತರ್ಭಾವ "hb-set.hh"
#ಅನ್ತರ್ಭಾವ "hb-cplusplus.hh"
#ಅನ್ತರ್ಭಾವ "hb-font.hh"
#ಅನ್ತರ್ಭಾವ "hb-subset-instancer-solver.hh"

ಸಂರಚನಾ hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = ಸ್ವಭಾವ;
  hb_ot_name_record_ids_t (unsigned platform_id_,
                           unsigned encoding_id_,
                           unsigned language_id_,
                           unsigned name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  ತಾರ್ಕಿಕ operator != (ಧ್ರುವ hb_ot_name_record_ids_t o) ಧ್ರುವ
  { ಪ್ರತ್ಯಾಗಮ !(*ಏತತ್ == o); }

  ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ operator == (ಧ್ರುವ hb_ot_name_record_ids_t& o) ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  ಅನ್ತಃಸ್ಥ uint32_t hash () ಧ್ರುವ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    ಪ್ರತ್ಯಾಗಮ current;
  }

  unsigned platform_id;
  unsigned encoding_id;
  unsigned language_id;
  unsigned name_id;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

ಸಂರಚನಾ hb_subset_ಒಳಹರಿವು_t
{
  HB_INTERNAL hb_subset_ಒಳಹರಿವು_t ();

  ~hb_subset_ಒಳಹರಿವು_t ()
  {
    sets.~sets_t ();

#ifdef HB_EXPERIMENTAL_API
    ಪ್ರತಿ (auto _ : name_table_overrides.values ())
      _.fini ();
#endif
  }

  hb_object_header_t header;

  ಸಂರಚನಾ sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  ಸಙ್ಘ {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[ಪರಿಮಾಣ (sets_t) / ಪರಿಮಾಣ (hb_set_t*)];
  };

  unsigned flags;
  ತಾರ್ಕಿಕ attach_accelerator_data = ಅಸತ್ಯ;

  // If set loca format will always be the long version.
  ತಾರ್ಕಿಕ force_long_loca = ಅಸತ್ಯ;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#ifdef HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#endif

  ಅನ್ತಃಸ್ಥ unsigned ಅಂಕೆ_sets () ಧ್ರುವ
  {
    ಪ್ರತ್ಯಾಗಮ ಪರಿಮಾಣ (set_ptrs) / ಪರಿಮಾಣ (hb_set_t*);
  }

  ಅನ್ತಃಸ್ಥ hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    ಪ್ರತ್ಯಾಗಮ hb_array (set_ptrs);
  }

  ತಾರ್ಕಿಕ in_error () ಧ್ರುವ
  {
    ಪ್ರತಿ (unsigned i = 0; i < ಅಂಕೆ_sets (); i++)
    {
      ಯದಿ (unlikely (set_ptrs[i]->in_error ()))
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }

    ಪ್ರತ್ಯಾಗಮ axes_location.in_error ()
#ifdef HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#endif
	;
  }
};


#endif /* HB_SUBSET_INPUT_HH */

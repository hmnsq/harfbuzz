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

#ifndef HB_SUBSET_आगत_HH
#define HB_SUBSET_आगत_HH


#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-subset.h"
#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-set.hh"
#अन्तर्भाव "hb-cplusplus.hh"
#अन्तर्भाव "hb-font.hh"
#अन्तर्भाव "hb-subset-instancer-solver.hh"

संरचना hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = स्वभाव;
  hb_ot_name_record_ids_t (अचिन्हित platform_id_,
                           अचिन्हित encoding_id_,
                           अचिन्हित language_id_,
                           अचिन्हित name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  तार्किक चालक != (अचर hb_ot_name_record_ids_t o) अचर
  { फर्काऊ !(*एतत् == o); }

  अन्तःस्थ तार्किक चालक == (अचर hb_ot_name_record_ids_t& o) अचर
  {
    फर्काऊ platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  अन्तःस्थ uint32_t hash () अचर
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    फर्काऊ current;
  }

  अचिन्हित platform_id;
  अचिन्हित encoding_id;
  अचिन्हित language_id;
  अचिन्हित name_id;
};

प्रकारनिर्देश संरचना hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

संरचना hb_subset_आगत_t
{
  HB_INTERNAL hb_subset_आगत_t ();

  ~hb_subset_आगत_t ()
  {
    sets.~sets_t ();

#ifdef HB_EXPERIMENTAL_API
    लागि (स्वतः _ : name_table_overrides.values ())
      _.fini ();
#endif
  }

  hb_object_header_t header;

  संरचना sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  सङ्घ {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[परिमाण (sets_t) / परिमाण (hb_set_t*)];
  };

  अचिन्हित flags;
  तार्किक attach_accelerator_data = असत्य;

  // If set loca format will always be the long version.
  तार्किक force_long_loca = असत्य;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#ifdef HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#endif

  अन्तःस्थ अचिन्हित अङ्क_sets () अचर
  {
    फर्काऊ परिमाण (set_ptrs) / परिमाण (hb_set_t*);
  }

  अन्तःस्थ hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    फर्काऊ hb_array (set_ptrs);
  }

  तार्किक in_error () अचर
  {
    लागि (अचिन्हित i = 0; i < अङ्क_sets (); i++)
    {
      यदि (unlikely (set_ptrs[i]->in_error ()))
        फर्काऊ सत्य;
    }

    फर्काऊ axes_location.in_error ()
#ifdef HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#endif
	;
  }
};


#endif /* HB_SUBSET_INPUT_HH */

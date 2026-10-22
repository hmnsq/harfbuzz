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

#ifndef HB_SUBSET_നിവേശം_HH
#define HB_SUBSET_നിവേശം_HH


#അന്തര്ഭാവ "hb.hh"

#അന്തര്ഭാവ "hb-subset.h"
#അന്തര്ഭാവ "hb-map.hh"
#അന്തര്ഭാവ "hb-set.hh"
#അന്തര്ഭാവ "hb-cplusplus.hh"
#അന്തര്ഭാവ "hb-font.hh"
#അന്തര്ഭാവ "hb-subset-instancer-solver.hh"

ഘടന hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = സ്ഥിരസ്ഥിതി;
  hb_ot_name_record_ids_t (unsigned platform_id_,
                           unsigned encoding_id_,
                           unsigned language_id_,
                           unsigned name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  തർക്കം operator != (സ്ഥിരം hb_ot_name_record_ids_t o) സ്ഥിരം
  { തിരികെ !(*ഇത് == o); }

  വരിയിൽ തർക്കം operator == (സ്ഥിരം hb_ot_name_record_ids_t& o) സ്ഥിരം
  {
    തിരികെ platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  വരിയിൽ uint32_t hash () സ്ഥിരം
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    തിരികെ current;
  }

  unsigned platform_id;
  unsigned encoding_id;
  unsigned language_id;
  unsigned name_id;
};

തരനിർവചനം ഘടന hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

ഘടന hb_subset_നിവേശം_t
{
  HB_INTERNAL hb_subset_നിവേശം_t ();

  ~hb_subset_നിവേശം_t ()
  {
    sets.~sets_t ();

#ifdef HB_EXPERIMENTAL_API
    വേണ്ടി (auto _ : name_table_overrides.values ())
      _.fini ();
#endif
  }

  hb_object_header_t header;

  ഘടന sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  സംഘം {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[വലുപ്പം (sets_t) / വലുപ്പം (hb_set_t*)];
  };

  unsigned flags;
  തർക്കം attach_accelerator_data = അസത്യം;

  // If set loca format will always be the long version.
  തർക്കം force_long_loca = അസത്യം;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#ifdef HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#endif

  വരിയിൽ unsigned അക്കം_sets () സ്ഥിരം
  {
    തിരികെ വലുപ്പം (set_ptrs) / വലുപ്പം (hb_set_t*);
  }

  വരിയിൽ hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    തിരികെ hb_array (set_ptrs);
  }

  തർക്കം in_error () സ്ഥിരം
  {
    വേണ്ടി (unsigned i = 0; i < അക്കം_sets (); i++)
    {
      എങ്കിൽ (unlikely (set_ptrs[i]->in_error ()))
        തിരികെ സത്യം;
    }

    തിരികെ axes_location.in_error ()
#ifdef HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#endif
	;
  }
};


#endif /* HB_SUBSET_INPUT_HH */

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

#ifndef HB_SUBSET_આગત_HH
#define HB_SUBSET_આગત_HH


#અન્તર્ભાવ "hb.hh"

#અન્તર્ભાવ "hb-subset.h"
#અન્તર્ભાવ "hb-map.hh"
#અન્તર્ભાવ "hb-set.hh"
#અન્તર્ભાવ "hb-cplusplus.hh"
#અન્તર્ભાવ "hb-font.hh"
#અન્તર્ભાવ "hb-subset-instancer-solver.hh"

સંરચના hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = મૂળભૂત;
  hb_ot_name_record_ids_t (unsigned platform_id_,
                           unsigned encoding_id_,
                           unsigned language_id_,
                           unsigned name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  તાર્કિક operator != (અચલ hb_ot_name_record_ids_t o) અચલ
  { પાછા !(*આ == o); }

  અંતઃરેખ તાર્કિક operator == (અચલ hb_ot_name_record_ids_t& o) અચલ
  {
    પાછા platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  અંતઃરેખ uint32_t hash () અચલ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    પાછા current;
  }

  unsigned platform_id;
  unsigned encoding_id;
  unsigned language_id;
  unsigned name_id;
};

પ્રકારવ્યાખ્યા સંરચના hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

સંરચના hb_subset_આગત_t
{
  HB_INTERNAL hb_subset_આગત_t ();

  ~hb_subset_આગત_t ()
  {
    sets.~sets_t ();

#ifdef HB_EXPERIMENTAL_API
    માટે (auto _ : name_table_overrides.values ())
      _.fini ();
#endif
  }

  hb_object_header_t header;

  સંરચના sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  સંઘ {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[કદ (sets_t) / કદ (hb_set_t*)];
  };

  unsigned flags;
  તાર્કિક attach_accelerator_data = ખોટું;

  // If set loca format will always be the long version.
  તાર્કિક force_long_loca = ખોટું;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#ifdef HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#endif

  અંતઃરેખ unsigned અંક_sets () અચલ
  {
    પાછા કદ (set_ptrs) / કદ (hb_set_t*);
  }

  અંતઃરેખ hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    પાછા hb_array (set_ptrs);
  }

  તાર્કિક in_error () અચલ
  {
    માટે (unsigned i = 0; i < અંક_sets (); i++)
    {
      જો (unlikely (set_ptrs[i]->in_error ()))
        પાછા સાચું;
    }

    પાછા axes_location.in_error ()
#ifdef HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#endif
	;
  }
};


#endif /* HB_SUBSET_INPUT_HH */

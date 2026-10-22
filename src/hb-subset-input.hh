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

#अगर_अघोषित HB_SUBSET_निवेश_HH
#घोषणा HB_SUBSET_निवेश_HH


#समावेश "hb.hh"

#समावेश "hb-subset.h"
#समावेश "hb-map.hh"
#समावेश "hb-set.hh"
#समावेश "hb-cplusplus.hh"
#समावेश "hb-font.hh"
#समावेश "hb-subset-instancer-solver.hh"

काष्ठा hb_ot_name_record_ids_t
{
  hb_ot_name_record_ids_t () = शेष;
  hb_ot_name_record_ids_t (अचिन्हित platform_id_,
                           अचिन्हित encoding_id_,
                           अचिन्हित language_id_,
                           अचिन्हित name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  bool चालक != (स्थिर hb_ot_name_record_ids_t o) स्थिर
  { वापस !(*यह == o); }

  अंतरभूत bool चालक == (स्थिर hb_ot_name_record_ids_t& o) स्थिर
  {
    वापस platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  अंतरभूत uint32_t hash () स्थिर
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    वापस current;
  }

  अचिन्हित platform_id;
  अचिन्हित encoding_id;
  अचिन्हित language_id;
  अचिन्हित name_id;
};

प्रकार काष्ठा hb_ot_name_record_ids_t hb_ot_name_record_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

काष्ठा hb_subset_निवेश_t
{
  HB_INTERNAL hb_subset_निवेश_t ();

  ~hb_subset_निवेश_t ()
  {
    sets.~sets_t ();

#अगर_घोषित HB_EXPERIMENTAL_API
    क्रम (स्वतः _ : name_table_overrides.values ())
      _.fini ();
#पूर्ण_अगर
  }

  hb_object_header_t header;

  काष्ठा sets_t {
    hb::shared_ptr<hb_set_t> glyphs;
    hb::shared_ptr<hb_set_t> unicodes;
    hb::shared_ptr<hb_set_t> no_subset_tables;
    hb::shared_ptr<hb_set_t> drop_tables;
    hb::shared_ptr<hb_set_t> name_ids;
    hb::shared_ptr<hb_set_t> name_languages;
    hb::shared_ptr<hb_set_t> layout_features;
    hb::shared_ptr<hb_set_t> layout_scripts;
  };

  जोड़ {
    sets_t sets;
    hb::shared_ptr<hb_set_t> set_ptrs[माप (sets_t) / माप (hb_set_t*)];
  };

  अचिन्हित flags;
  bool attach_accelerator_तथ्य = false;

  // If set loca format will always be the long version.
  bool force_long_loca = false;

  hb_hashmap_t<hb_tag_t, Triple> axes_location;
  hb_map_t glyph_map;
#अगर_घोषित HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_record_ids_t, hb_bytes_t> name_table_overrides;
#पूर्ण_अगर

  अंतरभूत अचिन्हित अंक_sets () स्थिर
  {
    वापस माप (set_ptrs) / माप (hb_set_t*);
  }

  अंतरभूत hb_array_t<hb::shared_ptr<hb_set_t>> sets_iter ()
  {
    वापस hb_array (set_ptrs);
  }

  bool in_error () स्थिर
  {
    क्रम (अचिन्हित i = 0; i < अंक_sets (); i++)
    {
      अगर (unlikely (set_ptrs[i]->in_error ()))
        वापस true;
    }

    वापस axes_location.in_error ()
#अगर_घोषित HB_EXPERIMENTAL_API
	|| name_table_overrides.in_error ()
#पूर्ण_अगर
	;
  }
};


#पूर्ण_अगर /* HB_SUBSET_INPUT_HH */

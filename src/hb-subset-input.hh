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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_ਨਿਵੇਸ਼_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_ਨਿਵੇਸ਼_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-cplusplus.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-font.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-subset-instancer-solver.hh"

ਬਣਤਰ hb_ot_name_ਰਿਕਾਰਡ_ids_t
{
  hb_ot_name_ਰਿਕਾਰਡ_ids_t () = ਮੂਲ;
  hb_ot_name_ਰਿਕਾਰਡ_ids_t (ਬਿਨਾਨਿਸ਼ਾਨੀ platform_id_,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ encoding_id_,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ language_id_,
                           ਬਿਨਾਨਿਸ਼ਾਨੀ name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  ਤਾਰਕਿਕ ਸੰਚਾਲਕ != (ਅਚਲ hb_ot_name_ਰਿਕਾਰਡ_ids_t o) ਅਚਲ
  { ਵਾਪਸ !(*ਇਹ == o); }

  ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ ਸੰਚਾਲਕ == (ਅਚਲ hb_ot_name_ਰਿਕਾਰਡ_ids_t& o) ਅਚਲ
  {
    ਵਾਪਸ platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  ਸਤਰਅੰਦਰ uint32_t hash () ਅਚਲ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    ਵਾਪਸ current;
  }

  ਬਿਨਾਨਿਸ਼ਾਨੀ platform_id;
  ਬਿਨਾਨਿਸ਼ਾਨੀ encoding_id;
  ਬਿਨਾਨਿਸ਼ਾਨੀ language_id;
  ਬਿਨਾਨਿਸ਼ਾਨੀ name_id;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ hb_ot_name_ਰਿਕਾਰਡ_ids_t hb_ot_name_ਰਿਕਾਰਡ_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

ਬਣਤਰ hb_subset_ਨਿਵੇਸ਼_t
{
  HB_ਅੰਦਰੂਨੀ hb_subset_ਨਿਵੇਸ਼_t ();

  ~hb_subset_ਨਿਵੇਸ਼_t ()
  {
    sets.~sets_t ();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
    ਲਈ (ਸਵੈਚਾਲਿਤ _ : name_ਸਾਰਣੀ_overrides.values ())
      _.fini ();
#ਜੇ_ਅੰਤ
  }

  hb_object_ਸਿਰਲੇਖ_t ਸਿਰਲੇਖ;

  ਬਣਤਰ sets_t {
    hb::shared_ptr<hb_ਸੈੱਟ_t> glyphs;
    hb::shared_ptr<hb_ਸੈੱਟ_t> unicodes;
    hb::shared_ptr<hb_ਸੈੱਟ_t> no_subset_tables;
    hb::shared_ptr<hb_ਸੈੱਟ_t> drop_tables;
    hb::shared_ptr<hb_ਸੈੱਟ_t> name_ids;
    hb::shared_ptr<hb_ਸੈੱਟ_t> name_languages;
    hb::shared_ptr<hb_ਸੈੱਟ_t> layout_features;
    hb::shared_ptr<hb_ਸੈੱਟ_t> layout_scripts;
  };

  ਸੰਘ {
    sets_t sets;
    hb::shared_ptr<hb_ਸੈੱਟ_t> ਸੈੱਟ_ptrs[ਆਕਾਰ (sets_t) / ਆਕਾਰ (hb_ਸੈੱਟ_t*)];
  };

  ਬਿਨਾਨਿਸ਼ਾਨੀ flags;
  ਤਾਰਕਿਕ attach_accelerator_ਅੰਕੜੇ = ਝੂਠ;

  // If set loca format will always be the long version.
  ਤਾਰਕਿਕ force_long_loca = ਝੂਠ;

  hb_hashmap_t<hb_ਟੈਗ_t, Triple> axes_location;
  hb_map_t glyph_map;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_ਰਿਕਾਰਡ_ids_t, hb_ਬਾਈਟਾਂ_t> name_ਸਾਰਣੀ_overrides;
#ਜੇ_ਅੰਤ

  ਸਤਰਅੰਦਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੰਕ_sets () ਅਚਲ
  {
    ਵਾਪਸ ਆਕਾਰ (ਸੈੱਟ_ptrs) / ਆਕਾਰ (hb_ਸੈੱਟ_t*);
  }

  ਸਤਰਅੰਦਰ hb_array_t<hb::shared_ptr<hb_ਸੈੱਟ_t>> sets_iter ()
  {
    ਵਾਪਸ hb_array (ਸੈੱਟ_ptrs);
  }

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  {
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਅੰਕ_sets (); i++)
    {
      ਜੇ (unlikely (ਸੈੱਟ_ptrs[i]->ਵਿੱਚ_error ()))
        ਵਾਪਸ ਸੱਚ;
    }

    ਵਾਪਸ axes_location.ਵਿੱਚ_error ()
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HB_EXPERIMENTAL_API
	|| name_ਸਾਰਣੀ_overrides.ਵਿੱਚ_error ()
#ਜੇ_ਅੰਤ
	;
  }
};


#ਜੇ_ਅੰਤ /* HB_SUBSET_INPUT_HH */

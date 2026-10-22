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

#اگر_غیر_تعریف_شدہ HB_SUBSET_داخلہ_HH
#تعریف_کرو HB_SUBSET_داخلہ_HH


#شامل "hb.hh"

#شامل "hb-subset.h"
#شامل "hb-map.hh"
#شامل "hb-set.hh"
#شامل "hb-cplusplus.hh"
#شامل "hb-font.hh"
#شامل "hb-subset-instancer-solver.hh"

ساخت hb_ot_name_ریکارڈ_ids_t
{
  hb_ot_name_ریکارڈ_ids_t () = طےشدہ;
  hb_ot_name_ریکارڈ_ids_t (غیرعلامتی platform_id_,
                           غیرعلامتی encoding_id_,
                           غیرعلامتی language_id_,
                           غیرعلامتی name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  منطقی عامل != (ثابت hb_ot_name_ریکارڈ_ids_t o) ثابت
  { واپس !(*یہ == o); }

  اندرونی منطقی عامل == (ثابت hb_ot_name_ریکارڈ_ids_t& o) ثابت
  {
    واپس platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  اندرونی uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    واپس current;
  }

  غیرعلامتی platform_id;
  غیرعلامتی encoding_id;
  غیرعلامتی language_id;
  غیرعلامتی name_id;
};

قسمتعریف ساخت hb_ot_name_ریکارڈ_ids_t hb_ot_name_ریکارڈ_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

ساخت hb_subset_داخلہ_t
{
  HB_اندرونی hb_subset_داخلہ_t ();

  ~hb_subset_داخلہ_t ()
  {
    sets.~sets_t ();

#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
    برائے (خودکار _ : name_جدول_overrides.values ())
      _.fini ();
#اگر_ختم
  }

  hb_object_سرخی_t سرخی;

  ساخت sets_t {
    hb::shared_ptr<hb_سیٹ_t> glyphs;
    hb::shared_ptr<hb_سیٹ_t> unicodes;
    hb::shared_ptr<hb_سیٹ_t> no_subset_tables;
    hb::shared_ptr<hb_سیٹ_t> drop_tables;
    hb::shared_ptr<hb_سیٹ_t> name_ids;
    hb::shared_ptr<hb_سیٹ_t> name_languages;
    hb::shared_ptr<hb_سیٹ_t> layout_features;
    hb::shared_ptr<hb_سیٹ_t> layout_scripts;
  };

  اتحاد {
    sets_t sets;
    hb::shared_ptr<hb_سیٹ_t> سیٹ_ptrs[حجم (sets_t) / حجم (hb_سیٹ_t*)];
  };

  غیرعلامتی flags;
  منطقی attach_accelerator_ڈیٹا = جھوٹ;

  // If set loca format will always be the long version.
  منطقی force_long_loca = جھوٹ;

  hb_hashmap_t<hb_لیبل_t, Triple> axes_location;
  hb_map_t glyph_map;
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_ریکارڈ_ids_t, hb_بائٹیں_t> name_جدول_overrides;
#اگر_ختم

  اندرونی غیرعلامتی عدد_sets () ثابت
  {
    واپس حجم (سیٹ_ptrs) / حجم (hb_سیٹ_t*);
  }

  اندرونی hb_array_t<hb::shared_ptr<hb_سیٹ_t>> sets_iter ()
  {
    واپس hb_array (سیٹ_ptrs);
  }

  منطقی اندر_error () ثابت
  {
    برائے (غیرعلامتی i = 0; i < عدد_sets (); i++)
    {
      اگر (unlikely (سیٹ_ptrs[i]->اندر_error ()))
        واپس سچ;
    }

    واپس axes_location.اندر_error ()
#اگر_تعریف_شدہ HB_EXPERIMENTAL_API
	|| name_جدول_overrides.اندر_error ()
#اگر_ختم
	;
  }
};


#اگر_ختم /* HB_SUBSET_INPUT_HH */

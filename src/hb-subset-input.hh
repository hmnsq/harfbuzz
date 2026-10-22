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

#که_نه_تعریف_شوی HB_SUBSET_ننوت_HH
#تعریف_کړه HB_SUBSET_ننوت_HH


#ضمن "hb.hh"

#ضمن "hb-subset.h"
#ضمن "hb-map.hh"
#ضمن "hb-set.hh"
#ضمن "hb-cplusplus.hh"
#ضمن "hb-font.hh"
#ضمن "hb-subset-instancer-solver.hh"

بنية hb_ot_name_ریکارډ_ids_t
{
  hb_ot_name_ریکارډ_ids_t () = افتراضي;
  hb_ot_name_ریکارډ_ids_t (بینښه platform_id_,
                           بینښه encoding_id_,
                           بینښه language_id_,
                           بینښه name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  منطقي چلوونکی != (ثابت hb_ot_name_ریکارډ_ids_t o) ثابت
  { أرجع !(*هذا == o); }

  مضمن منطقي چلوونکی == (ثابت hb_ot_name_ریکارډ_ids_t& o) ثابت
  {
    أرجع platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  مضمن uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    أرجع current;
  }

  بینښه platform_id;
  بینښه encoding_id;
  بینښه language_id;
  بینښه name_id;
};

تعريفنوع بنية hb_ot_name_ریکارډ_ids_t hb_ot_name_ریکارډ_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

بنية hb_subset_ننوت_t
{
  HB_دننی hb_subset_ننوت_t ();

  ~hb_subset_ننوت_t ()
  {
    sets.~sets_t ();

#که_تعریف_شوی HB_EXPERIMENTAL_API
    لكل (خپلکاری _ : name_جدول_overrides.values ())
      _.fini ();
#د_که_پای
  }

  hb_object_سرلیک_t سرلیک;

  بنية sets_t {
    hb::shared_ptr<hb_سېټ_t> glyphs;
    hb::shared_ptr<hb_سېټ_t> unicodes;
    hb::shared_ptr<hb_سېټ_t> no_subset_tables;
    hb::shared_ptr<hb_سېټ_t> drop_tables;
    hb::shared_ptr<hb_سېټ_t> name_ids;
    hb::shared_ptr<hb_سېټ_t> name_languages;
    hb::shared_ptr<hb_سېټ_t> layout_features;
    hb::shared_ptr<hb_سېټ_t> layout_scripts;
  };

  اتحاد {
    sets_t sets;
    hb::shared_ptr<hb_سېټ_t> سېټ_ptrs[حجم (sets_t) / حجم (hb_سېټ_t*)];
  };

  بینښه flags;
  منطقي attach_accelerator_معلومات = خطأ;

  // If set loca format will always be the long version.
  منطقي force_long_loca = خطأ;

  hb_hashmap_t<hb_نښان_t, Triple> axes_location;
  hb_map_t glyph_map;
#که_تعریف_شوی HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_ریکارډ_ids_t, hb_بایټونه_t> name_جدول_overrides;
#د_که_پای

  مضمن بینښه عدد_sets () ثابت
  {
    أرجع حجم (سېټ_ptrs) / حجم (hb_سېټ_t*);
  }

  مضمن hb_array_t<hb::shared_ptr<hb_سېټ_t>> sets_iter ()
  {
    أرجع hb_array (سېټ_ptrs);
  }

  منطقي دننه_error () ثابت
  {
    لكل (بینښه i = 0; i < عدد_sets (); i++)
    {
      إذا (unlikely (سېټ_ptrs[i]->دننه_error ()))
        أرجع ریښتیا;
    }

    أرجع axes_location.دننه_error ()
#که_تعریف_شوی HB_EXPERIMENTAL_API
	|| name_جدول_overrides.دننه_error ()
#د_که_پای
	;
  }
};


#د_که_پای /* HB_SUBSET_INPUT_HH */

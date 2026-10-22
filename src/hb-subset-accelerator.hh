/*
 * Copyright © 2022  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#اگر_تعریف_نشده HB_SUBSET_ACCELERATOR_HH
#تعریف_کن HB_SUBSET_ACCELERATOR_HH


#ضمن "hb.hh"

#ضمن "hb-map.hh"
#ضمن "hb-multimap.hh"
#ضمن "hb-set.hh"

خارجي HB_داخلی hb_user_داده_کلید_t hb_subset_accelerator_user_داده_کلید;

نطاق CFF {
بنية cff_subset_accelerator_t;
}

نطاق OT {
بنية SubtableUnicodesCache;
بنية cff1_subset_accelerator_t;
بنية cff2_subset_accelerator_t;
}

بنية hb_subset_accelerator_t
{
  ساكن hb_user_داده_کلید_t* user_داده_کلید()
  {
    أرجع &hb_subset_accelerator_user_داده_کلید;
  }

  ساكن hb_subset_accelerator_t* create(hb_face_t *منبع,
					 ثابت hb_map_t& unicode_مقصد_gid,
					 ثابت hb_مجموعه_t& unicodes_,
					 منطقي has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, حجم(hb_subset_accelerator_t));

    إذا (unlikely (!accel)) أرجع accel;

    جديد (accel) hb_subset_accelerator_t (منبع,
					 unicode_مقصد_gid,
					 unicodes_,
					 has_seac_);

    أرجع accel;
  }

  ساكن فراغ destroy (فراغ* p)
  {
    إذا (!p) أرجع;

    hb_subset_accelerator_t *accel = (hb_subset_accelerator_t *) p;

    accel->~hb_subset_accelerator_t ();

    hb_free (accel);
  }

  hb_subset_accelerator_t (hb_face_t *منبع,
			   ثابت hb_map_t& unicode_مقصد_gid,
			   ثابت hb_مجموعه_t& unicodes_,
			   منطقي has_seac_) :
    unicode_مقصد_gid_2(unicode_مقصد_gid),
    unicodes(unicodes_),
    cmap_cache(اشاره_تهی),
    destroy_cmap_cache(اشاره_تهی),
    has_seac(has_seac_),
    منبع(hb_face_reference (منبع))
  {
    gid_مقصد_unicodes.alloc (unicode_مقصد_gid_2.get_population ());
    لكل (ثابت خودکار &_ : unicode_مقصد_gid_2)
    {
      خودکار unicode = _.اولی;
      خودکار gid = _.second;
      gid_مقصد_unicodes.جمع_کن (gid, unicode);
    }
  }

  HB_داخلی ~hb_subset_accelerator_t ();

  // Generic

  تغییرپذیر hb_mutex_t sanitized_جدول_cache_قفل;
  تغییرپذیر hb_hashmap_t<hb_برچسب_t, hb::unique_ptr<hb_blob_t>> sanitized_جدول_cache;

  hb_map_t unicode_مقصد_gid_2;
  hb_multimap_t gid_مقصد_unicodes;
  hb_مجموعه_t unicodes;

  // cmap
  ثابت OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  منطقي has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  منطقي درون_error () ثابت
  {
    أرجع unicode_مقصد_gid_2.درون_error () ||
	   gid_مقصد_unicodes.درون_error () ||
	   unicodes.درون_error () ||
	   sanitized_جدول_cache.درون_error ();
  }

  hb_face_t *منبع;
#اگر_تعریف_نشده HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  تغییرپذیر hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  تغییرپذیر hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#پایان_اگر
};


#پایان_اگر /* HB_SUBSET_ACCELERATOR_HH */

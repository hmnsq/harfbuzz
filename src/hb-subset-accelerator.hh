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

#اگر_غیر_تعریف_شدہ HB_SUBSET_ACCELERATOR_HH
#تعریف_کرو HB_SUBSET_ACCELERATOR_HH


#شامل "hb.hh"

#شامل "hb-map.hh"
#شامل "hb-multimap.hh"
#شامل "hb-set.hh"

خارجی HB_اندرونی hb_user_ڈیٹا_کلید_t hb_subset_accelerator_user_ڈیٹا_کلید;

نامخلا CFF {
ساخت cff_subset_accelerator_t;
}

نامخلا OT {
ساخت SubtableUnicodesCache;
ساخت cff1_subset_accelerator_t;
ساخت cff2_subset_accelerator_t;
}

ساخت hb_subset_accelerator_t
{
  ساکن hb_user_ڈیٹا_کلید_t* user_ڈیٹا_کلید()
  {
    واپس &hb_subset_accelerator_user_ڈیٹا_کلید;
  }

  ساکن hb_subset_accelerator_t* create(hb_face_t *ماخذ,
					 ثابت hb_map_t& unicode_مقصد_gid,
					 ثابت hb_سیٹ_t& unicodes_,
					 منطقی has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, حجم(hb_subset_accelerator_t));

    اگر (unlikely (!accel)) واپس accel;

    نیا (accel) hb_subset_accelerator_t (ماخذ,
					 unicode_مقصد_gid,
					 unicodes_,
					 has_seac_);

    واپس accel;
  }

  ساکن خالی destroy (خالی* p)
  {
    اگر (!p) واپس;

    hb_subset_accelerator_t *accel = (hb_subset_accelerator_t *) p;

    accel->~hb_subset_accelerator_t ();

    hb_free (accel);
  }

  hb_subset_accelerator_t (hb_face_t *ماخذ,
			   ثابت hb_map_t& unicode_مقصد_gid,
			   ثابت hb_سیٹ_t& unicodes_,
			   منطقی has_seac_) :
    unicode_مقصد_gid_2(unicode_مقصد_gid),
    unicodes(unicodes_),
    cmap_cache(خالی_اشاریہ),
    destroy_cmap_cache(خالی_اشاریہ),
    has_seac(has_seac_),
    ماخذ(hb_face_reference (ماخذ))
  {
    gid_مقصد_unicodes.alloc (unicode_مقصد_gid_2.get_population ());
    برائے (ثابت خودکار &_ : unicode_مقصد_gid_2)
    {
      خودکار unicode = _.پہلا;
      خودکار gid = _.second;
      gid_مقصد_unicodes.جوڑو (gid, unicode);
    }
  }

  HB_اندرونی ~hb_subset_accelerator_t ();

  // Generic

  تغیرپذیر hb_mutex_t sanitized_جدول_cache_تالا;
  تغیرپذیر hb_hashmap_t<hb_لیبل_t, hb::unique_ptr<hb_blob_t>> sanitized_جدول_cache;

  hb_map_t unicode_مقصد_gid_2;
  hb_multimap_t gid_مقصد_unicodes;
  hb_سیٹ_t unicodes;

  // cmap
  ثابت OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  منطقی has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  منطقی اندر_error () ثابت
  {
    واپس unicode_مقصد_gid_2.اندر_error () ||
	   gid_مقصد_unicodes.اندر_error () ||
	   unicodes.اندر_error () ||
	   sanitized_جدول_cache.اندر_error ();
  }

  hb_face_t *ماخذ;
#اگر_غیر_تعریف_شدہ HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  تغیرپذیر hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  تغیرپذیر hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#اگر_ختم
};


#اگر_ختم /* HB_SUBSET_ACCELERATOR_HH */

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

#جيڪڏهن_وصف_نه_ٿيل HB_SUBSET_ACCELERATOR_HH
#وصف_ڪريو HB_SUBSET_ACCELERATOR_HH


#ضمن "hb.hh"

#ضمن "hb-map.hh"
#ضمن "hb-multimap.hh"
#ضمن "hb-set.hh"

خارجي HB_اندروني hb_user_معلومات_ڪنجي_t hb_subset_accelerator_user_معلومات_ڪنجي;

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
  ساكن hb_user_معلومات_ڪنجي_t* user_معلومات_ڪنجي()
  {
    أرجع &hb_subset_accelerator_user_معلومات_ڪنجي;
  }

  ساكن hb_subset_accelerator_t* create(hb_face_t *ذريعو,
					 ثابت hb_map_t& unicode_منزل_gid,
					 ثابت hb_سيٽ_t& unicodes_,
					 منطقي has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, حجم(hb_subset_accelerator_t));

    إذا (unlikely (!accel)) أرجع accel;

    جديد (accel) hb_subset_accelerator_t (ذريعو,
					 unicode_منزل_gid,
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

  hb_subset_accelerator_t (hb_face_t *ذريعو,
			   ثابت hb_map_t& unicode_منزل_gid,
			   ثابت hb_سيٽ_t& unicodes_,
			   منطقي has_seac_) :
    unicode_منزل_gid_2(unicode_منزل_gid),
    unicodes(unicodes_),
    cmap_cache(خالي_اشارو),
    destroy_cmap_cache(خالي_اشارو),
    has_seac(has_seac_),
    ذريعو(hb_face_reference (ذريعو))
  {
    gid_منزل_unicodes.alloc (unicode_منزل_gid_2.get_population ());
    لكل (ثابت خودڪار &_ : unicode_منزل_gid_2)
    {
      خودڪار unicode = _.پهريون;
      خودڪار gid = _.second;
      gid_منزل_unicodes.جوڙيو (gid, unicode);
    }
  }

  HB_اندروني ~hb_subset_accelerator_t ();

  // Generic

  بدلجندڙ hb_mutex_t sanitized_جدول_cache_تالو;
  بدلجندڙ hb_hashmap_t<hb_ٽيگ_t, hb::unique_ptr<hb_blob_t>> sanitized_جدول_cache;

  hb_map_t unicode_منزل_gid_2;
  hb_multimap_t gid_منزل_unicodes;
  hb_سيٽ_t unicodes;

  // cmap
  ثابت OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  منطقي has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  منطقي اندريون_error () ثابت
  {
    أرجع unicode_منزل_gid_2.اندريون_error () ||
	   gid_منزل_unicodes.اندريون_error () ||
	   unicodes.اندريون_error () ||
	   sanitized_جدول_cache.اندريون_error ();
  }

  hb_face_t *ذريعو;
#جيڪڏهن_وصف_نه_ٿيل HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  بدلجندڙ hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  بدلجندڙ hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#جيڪڏهن_پورو
};


#جيڪڏهن_پورو /* HB_SUBSET_ACCELERATOR_HH */

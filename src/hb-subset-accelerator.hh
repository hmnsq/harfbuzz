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

#ifndef HB_SUBSET_ACCELERATOR_HH
#define HB_SUBSET_ACCELERATOR_HH


#अन्तर्भाव "hb.hh"

#अन्तर्भाव "hb-map.hh"
#अन्तर्भाव "hb-multimap.hh"
#अन्तर्भाव "hb-set.hh"

बाह्य HB_INTERNAL hb_user_data_किल्ली_t hb_subset_accelerator_user_data_किल्ली;

नामाकाश CFF {
संरचना cff_subset_accelerator_t;
}

नामाकाश OT {
संरचना SubtableUnicodesCache;
संरचना cff1_subset_accelerator_t;
संरचना cff2_subset_accelerator_t;
}

संरचना hb_subset_accelerator_t
{
  स्थिर hb_user_data_किल्ली_t* user_data_किल्ली()
  {
    प्रत्यागम &hb_subset_accelerator_user_data_किल्ली;
  }

  स्थिर hb_subset_accelerator_t* create(hb_face_t *source,
					 ध्रुव hb_map_t& unicode_to_gid_,
					 ध्रुव hb_set_t& unicodes_,
					 तार्किक has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, परिमाण(hb_subset_accelerator_t));

    यदि (unlikely (!accel)) प्रत्यागम accel;

    नूतन (accel) hb_subset_accelerator_t (source,
					 unicode_to_gid_,
					 unicodes_,
					 has_seac_);

    प्रत्यागम accel;
  }

  स्थिर शून्य destroy (शून्य* p)
  {
    यदि (!p) प्रत्यागम;

    hb_subset_accelerator_t *accel = (hb_subset_accelerator_t *) p;

    accel->~hb_subset_accelerator_t ();

    hb_free (accel);
  }

  hb_subset_accelerator_t (hb_face_t *source,
			   ध्रुव hb_map_t& unicode_to_gid_,
			   ध्रुव hb_set_t& unicodes_,
			   तार्किक has_seac_) :
    unicode_to_gid(unicode_to_gid_),
    unicodes(unicodes_),
    cmap_cache(nullptr),
    destroy_cmap_cache(nullptr),
    has_seac(has_seac_),
    source(hb_face_reference (source))
  {
    gid_to_unicodes.alloc (unicode_to_gid.get_population ());
    प्रति (ध्रुव auto &_ : unicode_to_gid)
    {
      auto unicode = _.first;
      auto gid = _.second;
      gid_to_unicodes.add (gid, unicode);
    }
  }

  HB_INTERNAL ~hb_subset_accelerator_t ();

  // Generic

  mutable hb_mutex_t sanitized_table_cache_lock;
  mutable hb_hashmap_t<hb_tag_t, hb::unique_ptr<hb_blob_t>> sanitized_table_cache;

  hb_map_t unicode_to_gid;
  hb_multimap_t gid_to_unicodes;
  hb_set_t unicodes;

  // cmap
  ध्रुव OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  तार्किक has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  तार्किक in_error () ध्रुव
  {
    प्रत्यागम unicode_to_gid.in_error () ||
	   gid_to_unicodes.in_error () ||
	   unicodes.in_error () ||
	   sanitized_table_cache.in_error ();
  }

  hb_face_t *source;
#ifndef HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  mutable hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  mutable hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#endif
};


#endif /* HB_SUBSET_ACCELERATOR_HH */

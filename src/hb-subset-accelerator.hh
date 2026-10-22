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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_SUBSET_ACCELERATOR_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SUBSET_ACCELERATOR_HH


#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

#ਅਨ੍ਤਰ੍ਭਾਵ "hb-map.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-multimap.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-set.hh"

ਬਾਹਰੀ HB_ਅੰਦਰੂਨੀ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t hb_subset_accelerator_user_ਅੰਕੜੇ_ਕੁੰਜੀ;

ਨਾਮਾਕਾਸ਼ CFF {
ਬਣਤਰ cff_subset_accelerator_t;
}

ਨਾਮਾਕਾਸ਼ OT {
ਬਣਤਰ SubtableUnicodesCache;
ਬਣਤਰ cff1_subset_accelerator_t;
ਬਣਤਰ cff2_subset_accelerator_t;
}

ਬਣਤਰ hb_subset_accelerator_t
{
  ਸਥਿਰ hb_user_ਅੰਕੜੇ_ਕੁੰਜੀ_t* user_ਅੰਕੜੇ_ਕੁੰਜੀ()
  {
    ਵਾਪਸ &hb_subset_accelerator_user_ਅੰਕੜੇ_ਕੁੰਜੀ;
  }

  ਸਥਿਰ hb_subset_accelerator_t* create(hb_face_t *ਸਰੋਤ,
					 ਅਚਲ hb_map_t& unicode_ਮੰਜ਼ਿਲ_gid,
					 ਅਚਲ hb_ਸੈੱਟ_t& unicodes_,
					 ਤਾਰਕਿਕ has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, ਆਕਾਰ(hb_subset_accelerator_t));

    ਜੇ (unlikely (!accel)) ਵਾਪਸ accel;

    ਨਵਾਂ (accel) hb_subset_accelerator_t (ਸਰੋਤ,
					 unicode_ਮੰਜ਼ਿਲ_gid,
					 unicodes_,
					 has_seac_);

    ਵਾਪਸ accel;
  }

  ਸਥਿਰ ਖਾਲੀ destroy (ਖਾਲੀ* p)
  {
    ਜੇ (!p) ਵਾਪਸ;

    hb_subset_accelerator_t *accel = (hb_subset_accelerator_t *) p;

    accel->~hb_subset_accelerator_t ();

    hb_free (accel);
  }

  hb_subset_accelerator_t (hb_face_t *ਸਰੋਤ,
			   ਅਚਲ hb_map_t& unicode_ਮੰਜ਼ਿਲ_gid,
			   ਅਚਲ hb_ਸੈੱਟ_t& unicodes_,
			   ਤਾਰਕਿਕ has_seac_) :
    unicode_ਮੰਜ਼ਿਲ_gid_2(unicode_ਮੰਜ਼ਿਲ_gid),
    unicodes(unicodes_),
    cmap_cache(ਖਾਲੀ_ਸੂਚਕ),
    destroy_cmap_cache(ਖਾਲੀ_ਸੂਚਕ),
    has_seac(has_seac_),
    ਸਰੋਤ(hb_face_reference (ਸਰੋਤ))
  {
    gid_ਮੰਜ਼ਿਲ_unicodes.alloc (unicode_ਮੰਜ਼ਿਲ_gid_2.get_population ());
    ਲਈ (ਅਚਲ ਸਵੈਚਾਲਿਤ &_ : unicode_ਮੰਜ਼ਿਲ_gid_2)
    {
      ਸਵੈਚਾਲਿਤ unicode = _.ਪਹਿਲਾ;
      ਸਵੈਚਾਲਿਤ gid = _.second;
      gid_ਮੰਜ਼ਿਲ_unicodes.ਜੋੜੋ (gid, unicode);
    }
  }

  HB_ਅੰਦਰੂਨੀ ~hb_subset_accelerator_t ();

  // Generic

  ਬਦਲਣਯੋਗ hb_mutex_t sanitized_ਸਾਰਣੀ_cache_ਤਾਲਾ;
  ਬਦਲਣਯੋਗ hb_hashmap_t<hb_ਟੈਗ_t, hb::unique_ptr<hb_blob_t>> sanitized_ਸਾਰਣੀ_cache;

  hb_map_t unicode_ਮੰਜ਼ਿਲ_gid_2;
  hb_multimap_t gid_ਮੰਜ਼ਿਲ_unicodes;
  hb_ਸੈੱਟ_t unicodes;

  // cmap
  ਅਚਲ OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  ਤਾਰਕਿਕ has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  ਤਾਰਕਿਕ ਵਿੱਚ_error () ਅਚਲ
  {
    ਵਾਪਸ unicode_ਮੰਜ਼ਿਲ_gid_2.ਵਿੱਚ_error () ||
	   gid_ਮੰਜ਼ਿਲ_unicodes.ਵਿੱਚ_error () ||
	   unicodes.ਵਿੱਚ_error () ||
	   sanitized_ਸਾਰਣੀ_cache.ਵਿੱਚ_error ();
  }

  hb_face_t *ਸਰੋਤ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  ਬਦਲਣਯੋਗ hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  ਬਦਲਣਯੋਗ hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#ਜੇ_ਅੰਤ
};


#ਜੇ_ਅੰਤ /* HB_SUBSET_ACCELERATOR_HH */

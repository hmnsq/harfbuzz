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

#अगर_अघोषित HB_SUBSET_ACCELERATOR_HH
#घोषणा HB_SUBSET_ACCELERATOR_HH


#समावेश "hb.hh"

#समावेश "hb-map.hh"
#समावेश "hb-multimap.hh"
#समावेश "hb-set.hh"

बाह्य HB_INTERNAL hb_user_तथ्य_कुंजी_t hb_subset_accelerator_user_तथ्य_कुंजी;

namespace CFF {
काष्ठा cff_subset_accelerator_t;
}

namespace OT {
काष्ठा SubtableUnicodesCache;
काष्ठा cff1_subset_accelerator_t;
काष्ठा cff2_subset_accelerator_t;
}

काष्ठा hb_subset_accelerator_t
{
  अटल hb_user_तथ्य_कुंजी_t* user_तथ्य_कुंजी()
  {
    वापस &hb_subset_accelerator_user_तथ्य_कुंजी;
  }

  अटल hb_subset_accelerator_t* create(hb_face_t *source,
					 स्थिर hb_map_t& unicode_to_gid_,
					 स्थिर hb_set_t& unicodes_,
					 bool has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, माप(hb_subset_accelerator_t));

    अगर (unlikely (!accel)) वापस accel;

    नया (accel) hb_subset_accelerator_t (source,
					 unicode_to_gid_,
					 unicodes_,
					 has_seac_);

    वापस accel;
  }

  अटल व्योम destroy (व्योम* p)
  {
    अगर (!p) वापस;

    hb_subset_accelerator_t *accel = (hb_subset_accelerator_t *) p;

    accel->~hb_subset_accelerator_t ();

    hb_free (accel);
  }

  hb_subset_accelerator_t (hb_face_t *source,
			   स्थिर hb_map_t& unicode_to_gid_,
			   स्थिर hb_set_t& unicodes_,
			   bool has_seac_) :
    unicode_to_gid(unicode_to_gid_),
    unicodes(unicodes_),
    cmap_cache(nullptr),
    destroy_cmap_cache(nullptr),
    has_seac(has_seac_),
    source(hb_face_reference (source))
  {
    gid_to_unicodes.alloc (unicode_to_gid.get_population ());
    क्रम (स्थिर स्वतः &_ : unicode_to_gid)
    {
      स्वतः unicode = _.first;
      स्वतः gid = _.second;
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
  स्थिर OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  bool has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  bool in_error () स्थिर
  {
    वापस unicode_to_gid.in_error () ||
	   gid_to_unicodes.in_error () ||
	   unicodes.in_error () ||
	   sanitized_table_cache.in_error ();
  }

  hb_face_t *source;
#अगर_अघोषित HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  mutable hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  mutable hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#पूर्ण_अगर
};


#पूर्ण_अगर /* HB_SUBSET_ACCELERATOR_HH */

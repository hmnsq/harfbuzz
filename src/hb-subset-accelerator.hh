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

#నిర్వచితం_కాకపోతే HB_SUBSET_ACCELERATOR_HH
#నిర్వచించు HB_SUBSET_ACCELERATOR_HH


#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-multimap.hh"
#అన్తర్భావ "hb-set.hh"

బాహ్య HB_అంతర్గత hb_user_దత్తాంశం_కీలకం_t hb_subset_accelerator_user_దత్తాంశం_కీలకం;

నామాకాశ CFF {
నిర్మాణం cff_subset_accelerator_t;
}

నామాకాశ OT {
నిర్మాణం SubtableUnicodesCache;
నిర్మాణం cff1_subset_accelerator_t;
నిర్మాణం cff2_subset_accelerator_t;
}

నిర్మాణం hb_subset_accelerator_t
{
  స్థిర hb_user_దత్తాంశం_కీలకం_t* user_దత్తాంశం_కీలకం()
  {
    తిరిగి &hb_subset_accelerator_user_దత్తాంశం_కీలకం;
  }

  స్థిర hb_subset_accelerator_t* create(hb_face_t *ఆధారం,
					 స్థిరరాశి hb_map_t& unicode_గమ్యం_gid,
					 స్థిరరాశి hb_సమితి_t& unicodes_,
					 తార్కిక has_seac_) {
    hb_subset_accelerator_t* accel =
        (hb_subset_accelerator_t*) hb_calloc (1, పరిమాణం(hb_subset_accelerator_t));

    ఒకవేళ (unlikely (!accel)) తిరిగి accel;

    కొత్త (accel) hb_subset_accelerator_t (ఆధారం,
					 unicode_గమ్యం_gid,
					 unicodes_,
					 has_seac_);

    తిరిగి accel;
  }

  స్థిర శూన్య destroy (శూన్య* p)
  {
    ఒకవేళ (!p) తిరిగి;

    hb_subset_accelerator_t *accel = (hb_subset_accelerator_t *) p;

    accel->~hb_subset_accelerator_t ();

    hb_free (accel);
  }

  hb_subset_accelerator_t (hb_face_t *ఆధారం,
			   స్థిరరాశి hb_map_t& unicode_గమ్యం_gid,
			   స్థిరరాశి hb_సమితి_t& unicodes_,
			   తార్కిక has_seac_) :
    unicode_గమ్యం_gid_2(unicode_గమ్యం_gid),
    unicodes(unicodes_),
    cmap_cache(శూన్యసూచిక),
    destroy_cmap_cache(శూన్యసూచిక),
    has_seac(has_seac_),
    ఆధారం(hb_face_reference (ఆధారం))
  {
    gid_గమ్యం_unicodes.alloc (unicode_గమ్యం_gid_2.get_population ());
    కొరకు (స్థిరరాశి స్వయంచాలక &_ : unicode_గమ్యం_gid_2)
    {
      స్వయంచాలక unicode = _.మొదటి;
      స్వయంచాలక gid = _.second;
      gid_గమ్యం_unicodes.కూడు (gid, unicode);
    }
  }

  HB_అంతర్గత ~hb_subset_accelerator_t ();

  // Generic

  మార్చదగిన hb_mutex_t sanitized_పట్టిక_cache_తాళం;
  మార్చదగిన hb_hashmap_t<hb_ట్యాగ్_t, hb::unique_ptr<hb_blob_t>> sanitized_పట్టిక_cache;

  hb_map_t unicode_గమ్యం_gid_2;
  hb_multimap_t gid_గమ్యం_unicodes;
  hb_సమితి_t unicodes;

  // cmap
  స్థిరరాశి OT::SubtableUnicodesCache* cmap_cache;
  hb_destroy_func_t destroy_cmap_cache;

  // CFF
  తార్కిక has_seac;

  // TODO(garretrieger): cumulative glyf checksum map

  తార్కిక లోపల_error () స్థిరరాశి
  {
    తిరిగి unicode_గమ్యం_gid_2.లోపల_error () ||
	   gid_గమ్యం_unicodes.లోపల_error () ||
	   unicodes.లోపల_error () ||
	   sanitized_పట్టిక_cache.లోపల_error ();
  }

  hb_face_t *ఆధారం;
#నిర్వచితం_కాకపోతే HB_NO_SUBSET_CFF
  // These have to be immediately after source:
  మార్చదగిన hb_face_lazy_loader_t<OT::cff1_subset_accelerator_t, 1> cff1_accel;
  మార్చదగిన hb_face_lazy_loader_t<OT::cff2_subset_accelerator_t, 2> cff2_accel;
#అయితే_ముగింపు
};


#అయితే_ముగింపు /* HB_SUBSET_ACCELERATOR_HH */

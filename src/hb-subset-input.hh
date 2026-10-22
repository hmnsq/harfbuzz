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

#నిర్వచితం_కాకపోతే HB_SUBSET_ఆదానం_HH
#నిర్వచించు HB_SUBSET_ఆదానం_HH


#అన్తర్భావ "hb.hh"

#అన్తర్భావ "hb-subset.h"
#అన్తర్భావ "hb-map.hh"
#అన్తర్భావ "hb-set.hh"
#అన్తర్భావ "hb-cplusplus.hh"
#అన్తర్భావ "hb-font.hh"
#అన్తర్భావ "hb-subset-instancer-solver.hh"

నిర్మాణం hb_ot_name_రికార్డు_ids_t
{
  hb_ot_name_రికార్డు_ids_t () = అప్రమేయ;
  hb_ot_name_రికార్డు_ids_t (గుర్తులేని platform_id_,
                           గుర్తులేని encoding_id_,
                           గుర్తులేని language_id_,
                           గుర్తులేని name_id_)
      :platform_id (platform_id_),
      encoding_id (encoding_id_),
      language_id (language_id_),
      name_id (name_id_) {}

  తార్కిక క్రియాకారకం != (స్థిరరాశి hb_ot_name_రికార్డు_ids_t o) స్థిరరాశి
  { తిరిగి !(*ఇది == o); }

  అంతఃవరుస తార్కిక క్రియాకారకం == (స్థిరరాశి hb_ot_name_రికార్డు_ids_t& o) స్థిరరాశి
  {
    తిరిగి platform_id == o.platform_id &&
           encoding_id == o.encoding_id &&
           language_id == o.language_id &&
           name_id == o.name_id;
  }

  అంతఃవరుస uint32_t hash () స్థిరరాశి
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (platform_id);
    current = current * 31 + hb_hash (encoding_id);
    current = current * 31 + hb_hash (language_id);
    current = current * 31 + hb_hash (name_id);
    తిరిగి current;
  }

  గుర్తులేని platform_id;
  గుర్తులేని encoding_id;
  గుర్తులేని language_id;
  గుర్తులేని name_id;
};

రకనిర్వచనం నిర్మాణం hb_ot_name_రికార్డు_ids_t hb_ot_name_రికార్డు_ids_t;


HB_MARK_AS_FLAG_T (hb_subset_flags_t);

నిర్మాణం hb_subset_ఆదానం_t
{
  HB_అంతర్గత hb_subset_ఆదానం_t ();

  ~hb_subset_ఆదానం_t ()
  {
    sets.~sets_t ();

#నిర్వచితమైతే HB_EXPERIMENTAL_API
    కొరకు (స్వయంచాలక _ : name_పట్టిక_overrides.values ())
      _.fini ();
#అయితే_ముగింపు
  }

  hb_object_శీర్షిక_t శీర్షిక;

  నిర్మాణం sets_t {
    hb::shared_ptr<hb_సమితి_t> glyphs;
    hb::shared_ptr<hb_సమితి_t> unicodes;
    hb::shared_ptr<hb_సమితి_t> no_subset_tables;
    hb::shared_ptr<hb_సమితి_t> drop_tables;
    hb::shared_ptr<hb_సమితి_t> name_ids;
    hb::shared_ptr<hb_సమితి_t> name_languages;
    hb::shared_ptr<hb_సమితి_t> layout_features;
    hb::shared_ptr<hb_సమితి_t> layout_scripts;
  };

  సంఘం {
    sets_t sets;
    hb::shared_ptr<hb_సమితి_t> సమితి_ptrs[పరిమాణం (sets_t) / పరిమాణం (hb_సమితి_t*)];
  };

  గుర్తులేని flags;
  తార్కిక attach_accelerator_దత్తాంశం = అబద్ధం;

  // If set loca format will always be the long version.
  తార్కిక force_long_loca = అబద్ధం;

  hb_hashmap_t<hb_ట్యాగ్_t, Triple> axes_location;
  hb_map_t glyph_map;
#నిర్వచితమైతే HB_EXPERIMENTAL_API
  hb_hashmap_t<hb_ot_name_రికార్డు_ids_t, hb_బైట్లు_t> name_పట్టిక_overrides;
#అయితే_ముగింపు

  అంతఃవరుస గుర్తులేని అంకె_sets () స్థిరరాశి
  {
    తిరిగి పరిమాణం (సమితి_ptrs) / పరిమాణం (hb_సమితి_t*);
  }

  అంతఃవరుస hb_array_t<hb::shared_ptr<hb_సమితి_t>> sets_iter ()
  {
    తిరిగి hb_array (సమితి_ptrs);
  }

  తార్కిక లోపల_error () స్థిరరాశి
  {
    కొరకు (గుర్తులేని i = 0; i < అంకె_sets (); i++)
    {
      ఒకవేళ (unlikely (సమితి_ptrs[i]->లోపల_error ()))
        తిరిగి నిజం;
    }

    తిరిగి axes_location.లోపల_error ()
#నిర్వచితమైతే HB_EXPERIMENTAL_API
	|| name_పట్టిక_overrides.లోపల_error ()
#అయితే_ముగింపు
	;
  }
};


#అయితే_ముగింపు /* HB_SUBSET_INPUT_HH */

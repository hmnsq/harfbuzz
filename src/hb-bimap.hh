/*
 * Copyright © 2019 Adobe Inc.
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
 * Adobe Author(s): Michiharu Ariza
 */

#నిర్వచితం_కాకపోతే HB_BIMAP_HH
#నిర్వచించు HB_BIMAP_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-map.hh"

/* Bi-directional map */
నిర్మాణం hb_bimap_t
{
  శూన్య reset ()
  {
    forw_map.reset ();
    వెనుక_map.reset ();
  }

  శూన్య alloc (గుర్తులేని తీయి)
  {
    forw_map.alloc (తీయి);
    వెనుక_map.alloc (తీయి);
  }

  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి forw_map.లోపల_error () || వెనుక_map.లోపల_error (); }

  శూన్య సమితి (hb_codepoint_t lhs, hb_codepoint_t rhs)
  {
    ఒకవేళ (లోపల_error ()) తిరిగి;
    ఒకవేళ (unlikely (lhs == HB_MAP_విలువ_INVALID)) తిరిగి;
    ఒకవేళ (unlikely (rhs == HB_MAP_విలువ_INVALID)) { del (lhs); తిరిగి; }

    forw_map.సమితి (lhs, rhs);
    ఒకవేళ (unlikely (లోపల_error ())) తిరిగి;

    వెనుక_map.సమితి (rhs, lhs);
    ఒకవేళ (unlikely (లోపల_error ())) forw_map.del (lhs);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) స్థిరరాశి { తిరిగి forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) స్థిరరాశి { తిరిగి వెనుక_map.get (rhs); }

  hb_codepoint_t క్రియాకారకం [] (hb_codepoint_t lhs) స్థిరరాశి { తిరిగి get (lhs); }
  తార్కిక has (hb_codepoint_t lhs) స్థిరరాశి { తిరిగి forw_map.has (lhs); }


  శూన్య del (hb_codepoint_t lhs)
  {
    వెనుక_map.del (get (lhs));
    forw_map.del (lhs);
  }

  శూన్య clear ()
  {
    forw_map.clear ();
    వెనుక_map.clear ();
  }

  తార్కిక is_empty () స్థిరరాశి { తిరిగి forw_map.is_empty (); }

  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి { తిరిగి forw_map.get_population (); }

  రక్షిత:
  hb_map_t  forw_map;
  hb_map_t  వెనుక_map;

  బహిరంగ:
  స్వయంచాలక కీలకాలు () స్థిరరాశి HB_AUTO_RETURN (+ forw_map.కీలకాలు())
  స్వయంచాలక values () స్థిరరాశి HB_AUTO_RETURN (+ forw_map.values())
  స్వయంచాలక iter () స్థిరరాశి HB_AUTO_RETURN (+ forw_map.iter())
};

/* Incremental bimap: only lhs is given, rhs is incrementally assigned */
నిర్మాణం hb_inc_bimap_t
{
  తార్కిక లోపల_error () స్థిరరాశి { తిరిగి forw_map.లోపల_error () || వెనుక_map.లోపల_error (); }

  గుర్తులేని పూర్ణాంకం get_population () స్థిరరాశి { తిరిగి forw_map.get_population (); }

  శూన్య reset ()
  {
    forw_map.reset ();
    వెనుక_map.reset ();
  }

  శూన్య alloc (గుర్తులేని తీయి)
  {
    forw_map.alloc (తీయి);
    వెనుక_map.alloc (తీయి);
  }

  శూన్య clear ()
  {
    forw_map.clear ();
    వెనుక_map.clear ();
  }

  /* Add a mapping from lhs to rhs with a unique value if lhs is unknown.
   * Return the rhs value as the result.
   */
  hb_codepoint_t కూడు (hb_codepoint_t lhs)
  {
    hb_codepoint_t  rhs = forw_map[lhs];
    ఒకవేళ (rhs == HB_MAP_విలువ_INVALID)
    {
      rhs = వెనుక_map.నిడివి;
      forw_map.సమితి (lhs, rhs);
      వెనుక_map.push (lhs);
    }
    తిరిగి rhs;
  }

  hb_codepoint_t skip ()
  {
    hb_codepoint_t ప్రారంభం = వెనుక_map.నిడివి;
    వెనుక_map.push (HB_MAP_విలువ_INVALID);
    తిరిగి ప్రారంభం;
  }

  hb_codepoint_t skip (గుర్తులేని లెక్క)
  {
    hb_codepoint_t ప్రారంభం = వెనుక_map.నిడివి;
    వెనుక_map.alloc (వెనుక_map.నిడివి + లెక్క);
    కొరకు (గుర్తులేని i = 0; i < లెక్క; i++)
      వెనుక_map.push (HB_MAP_విలువ_INVALID);
    తిరిగి ప్రారంభం;
  }

  hb_codepoint_t get_తదుపరి_విలువ () స్థిరరాశి
  { తిరిగి వెనుక_map.నిడివి; }

  శూన్య కూడు_సమితి (స్థిరరాశి hb_సమితి_t *సమితి)
  {
    కొరకు (స్వయంచాలక i : *సమితి) కూడు (i);
  }

  /* Create an identity map. */
  తార్కిక తటస్థం (గుర్తులేని పూర్ణాంకం కొలత)
  {
    clear ();
    కొరకు (hb_codepoint_t i = 0; i < కొలత; i++) కూడు (i);
    తిరిగి !లోపల_error ();
  }

  రక్షిత:
  స్థిర పూర్ణాంకం cmp_id (స్థిరరాశి శూన్య* a, స్థిరరాశి శూన్య* b)
  { తిరిగి (పూర్ణాంకం)*(స్థిరరాశి hb_codepoint_t *)a - (పూర్ణాంకం)*(స్థిరరాశి hb_codepoint_t *)b; }

  బహిరంగ:
  /* Optional: after finished adding all mappings in a random order,
   * reassign rhs to lhs so that they are in the same order. */
  శూన్య sort ()
  {
    hb_codepoint_t  లెక్క = get_population ();
    hb_సదిశ_t <hb_codepoint_t> work;
    ఒకవేళ (unlikely (!work.resize_dirty  (లెక్క))) తిరిగి;

    కొరకు (hb_codepoint_t rhs = 0; rhs < లెక్క; rhs++)
      work.arrayZ[rhs] = వెనుక_map[rhs];

    work.qsort (cmp_id);

    clear ();
    కొరకు (hb_codepoint_t rhs = 0; rhs < లెక్క; rhs++)
      కూడు (work.arrayZ[rhs]);
  }

  hb_codepoint_t get (hb_codepoint_t lhs) స్థిరరాశి { తిరిగి forw_map.get (lhs); }
  hb_codepoint_t backward (hb_codepoint_t rhs) స్థిరరాశి { తిరిగి వెనుక_map[rhs]; }

  hb_codepoint_t క్రియాకారకం [] (hb_codepoint_t lhs) స్థిరరాశి { తిరిగి get (lhs); }
  తార్కిక has (hb_codepoint_t lhs) స్థిరరాశి { తిరిగి forw_map.has (lhs); }

  రక్షిత:
  hb_map_t forw_map;
  hb_సదిశ_t<hb_codepoint_t> వెనుక_map;

  బహిరంగ:
  స్వయంచాలక కీలకాలు () స్థిరరాశి HB_AUTO_RETURN (+ వెనుక_map.iter())
};

#అయితే_ముగింపు /* HB_BIMAP_HH */

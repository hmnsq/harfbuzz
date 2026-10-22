/*
 * Copyright © 2012  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-shaper.hh"
#అన్తర్భావ "hb-machinery.hh"


స్థిర స్థిరరాశి hb_shaper_నమోదు_t _hb_all_shapers[] = {
#నిర్వచించు HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#అన్తర్భావ "hb-shaper-list.hh"
#నిర్వచనం_రద్దు HB_SHAPER_IMPLEMENT
};
#నిర్వచితం_కాకపోతే HB_NO_SHAPER
static_assert (0 != ARRAY_నిడివి_CONST (_hb_all_shapers), "No shaper enabled.");
#అయితే_ముగింపు

స్థిర అంతఃవరుస శూన్య free_static_shapers ();

స్థిర నిర్మాణం hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_నమోదు_t,
							  hb_shapers_lazy_loader_t>
{
  స్థిర hb_shaper_నమోదు_t *create ()
  {
    అక్షరం *env = getenv ("HB_SHAPER_LIST");
    ఒకవేళ (!env || !*env)
      తిరిగి శూన్యసూచిక;

    hb_shaper_నమోదు_t *shapers = (hb_shaper_నమోదు_t *) hb_calloc (1, పరిమాణం (_hb_all_shapers));
    ఒకవేళ (unlikely (!shapers))
      తిరిగి శూన్యసూచిక;

    hb_memcpy (shapers, _hb_all_shapers, పరిమాణం (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    గుర్తులేని పూర్ణాంకం i = 0;
    అక్షరం *చివర, *p = env;
    కొరకు (;;)
    {
      చివర = strchr (p, ',');
      ఒకవేళ (!చివర)
	చివర = p + అక్షరశ్రేణి_పొడవు (p);

      కొరకు (గుర్తులేని పూర్ణాంకం j = i; j < ARRAY_నిడివి_CONST (_hb_all_shapers); j++)
	ఒకవేళ (చివర - p == (పూర్ణాంకం) అక్షరశ్రేణి_పొడవు (shapers[j].name) &&
	    0 == అక్షరశ్రేణి_పోలిక (shapers[j].name, p, చివర - p))
	{
	  /* Reorder this shaper to position i */
	 నిర్మాణం hb_shaper_నమోదు_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], పరిమాణం (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      ఒకవేళ (!*చివర)
	ఆపు;
      లేకపోతే
	p = చివర + 1;
    }

    hb_atexit (free_static_shapers);

    తిరిగి shapers;
  }
  స్థిర శూన్య destroy (hb_shaper_నమోదు_t *p) { hb_free (p); }
  స్థిర స్థిరరాశి hb_shaper_నమోదు_t *get_null ()      { తిరిగి _hb_all_shapers; }
} static_shapers;

స్థిర అంతఃవరుస
శూన్య free_static_shapers ()
{
  static_shapers.free_instance ();
}

స్థిరరాశి hb_shaper_నమోదు_t *
_hb_shapers_get ()
{
  తిరిగి static_shapers.get_unconst ();
}

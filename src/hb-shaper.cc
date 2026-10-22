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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-shaper.hh"
#અન્તર્ભાવ "hb-machinery.hh"


સ્થિર અચલ hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#અન્તર્ભાવ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

સ્થિર અંતઃરેખ શૂન્ય free_static_shapers ();

સ્થિર સંરચના hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  સ્થિર hb_shaper_entry_t *create ()
  {
    અક્ષર *env = getenv ("HB_SHAPER_LIST");
    જો (!env || !*env)
      પાછા nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, કદ (_hb_all_shapers));
    જો (unlikely (!shapers))
      પાછા nullptr;

    hb_memcpy (shapers, _hb_all_shapers, કદ (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned પૂર્ણાંક i = 0;
    અક્ષર *end, *p = env;
    માટે (;;)
    {
      end = strchr (p, ',');
      જો (!end)
	end = p + strlen (p);

      માટે (unsigned પૂર્ણાંક j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	જો (end - p == (પૂર્ણાંક) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 સંરચના hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], કદ (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      જો (!*end)
	તોડો;
      નહીંતર
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    પાછા shapers;
  }
  સ્થિર શૂન્ય destroy (hb_shaper_entry_t *p) { hb_free (p); }
  સ્થિર અચલ hb_shaper_entry_t *get_null ()      { પાછા _hb_all_shapers; }
} static_shapers;

સ્થિર અંતઃરેખ
શૂન્ય free_static_shapers ()
{
  static_shapers.free_instance ();
}

અચલ hb_shaper_entry_t *
_hb_shapers_get ()
{
  પાછા static_shapers.get_unconst ();
}

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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-shaper.hh"
#ಅನ್ತರ್ಭಾವ "hb-machinery.hh"


ಸ್ಥಿರ ಧ್ರುವ hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#ಅನ್ತರ್ಭಾವ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ free_static_shapers ();

ಸ್ಥಿರ ಸಂರಚನಾ hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  ಸ್ಥಿರ hb_shaper_entry_t *create ()
  {
    ವರ್ಣ *env = getenv ("HB_SHAPER_LIST");
    ಯದಿ (!env || !*env)
      ಪ್ರತ್ಯಾಗಮ nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, ಪರಿಮಾಣ (_hb_all_shapers));
    ಯದಿ (unlikely (!shapers))
      ಪ್ರತ್ಯಾಗಮ nullptr;

    hb_memcpy (shapers, _hb_all_shapers, ಪರಿಮಾಣ (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned ಪೂರ್ಣಾಙ್ಕ i = 0;
    ವರ್ಣ *end, *p = env;
    ಪ್ರತಿ (;;)
    {
      end = strchr (p, ',');
      ಯದಿ (!end)
	end = p + strlen (p);

      ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	ಯದಿ (end - p == (ಪೂರ್ಣಾಙ್ಕ) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 ಸಂರಚನಾ hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], ಪರಿಮಾಣ (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      ಯದಿ (!*end)
	ಭಙ್ಗ;
      ಅನ್ಯಥಾ
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    ಪ್ರತ್ಯಾಗಮ shapers;
  }
  ಸ್ಥಿರ ಶೂನ್ಯ destroy (hb_shaper_entry_t *p) { hb_free (p); }
  ಸ್ಥಿರ ಧ್ರುವ hb_shaper_entry_t *get_null ()      { ಪ್ರತ್ಯಾಗಮ _hb_all_shapers; }
} static_shapers;

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ free_static_shapers ()
{
  static_shapers.free_instance ();
}

ಧ್ರುವ hb_shaper_entry_t *
_hb_shapers_get ()
{
  ಪ್ರತ್ಯಾಗಮ static_shapers.get_unconst ();
}

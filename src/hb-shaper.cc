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

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-shaper.hh"
#അന്തര്ഭാവ "hb-machinery.hh"


സ്ഥിതം സ്ഥിരം hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#അന്തര്ഭാവ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

സ്ഥിതം വരിയിൽ ശൂന്യം free_static_shapers ();

സ്ഥിതം ഘടന hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  സ്ഥിതം hb_shaper_entry_t *create ()
  {
    അക്ഷരം *env = getenv ("HB_SHAPER_LIST");
    എങ്കിൽ (!env || !*env)
      തിരികെ nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, വലുപ്പം (_hb_all_shapers));
    എങ്കിൽ (unlikely (!shapers))
      തിരികെ nullptr;

    hb_memcpy (shapers, _hb_all_shapers, വലുപ്പം (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned പൂർണ്ണസംഖ്യ i = 0;
    അക്ഷരം *end, *p = env;
    വേണ്ടി (;;)
    {
      end = strchr (p, ',');
      എങ്കിൽ (!end)
	end = p + strlen (p);

      വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	എങ്കിൽ (end - p == (പൂർണ്ണസംഖ്യ) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 ഘടന hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], വലുപ്പം (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      എങ്കിൽ (!*end)
	നിർത്തുക;
      അല്ലെങ്കിൽ
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    തിരികെ shapers;
  }
  സ്ഥിതം ശൂന്യം destroy (hb_shaper_entry_t *p) { hb_free (p); }
  സ്ഥിതം സ്ഥിരം hb_shaper_entry_t *get_null ()      { തിരികെ _hb_all_shapers; }
} static_shapers;

സ്ഥിതം വരിയിൽ
ശൂന്യം free_static_shapers ()
{
  static_shapers.free_instance ();
}

സ്ഥിരം hb_shaper_entry_t *
_hb_shapers_get ()
{
  തിരികെ static_shapers.get_unconst ();
}

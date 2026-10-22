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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-shaper.hh"
#அந்தர்பாவ "hb-machinery.hh"


நிலையான மாறிலி hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#அந்தர்பாவ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

நிலையான உள்வரி வெறுமை free_static_shapers ();

நிலையான கட்டமைப்பு hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  நிலையான hb_shaper_entry_t *create ()
  {
    எழுத்து *env = getenv ("HB_SHAPER_LIST");
    எனில் (!env || !*env)
      திரும்பு nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, அளவு (_hb_all_shapers));
    எனில் (unlikely (!shapers))
      திரும்பு nullptr;

    hb_memcpy (shapers, _hb_all_shapers, அளவு (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned முழு i = 0;
    எழுத்து *end, *p = env;
    க்கு (;;)
    {
      end = strchr (p, ',');
      எனில் (!end)
	end = p + strlen (p);

      க்கு (unsigned முழு j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	எனில் (end - p == (முழு) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 கட்டமைப்பு hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], அளவு (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      எனில் (!*end)
	நிறுத்து;
      இல்லையெனில்
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    திரும்பு shapers;
  }
  நிலையான வெறுமை destroy (hb_shaper_entry_t *p) { hb_free (p); }
  நிலையான மாறிலி hb_shaper_entry_t *get_null ()      { திரும்பு _hb_all_shapers; }
} static_shapers;

நிலையான உள்வரி
வெறுமை free_static_shapers ()
{
  static_shapers.free_instance ();
}

மாறிலி hb_shaper_entry_t *
_hb_shapers_get ()
{
  திரும்பு static_shapers.get_unconst ();
}

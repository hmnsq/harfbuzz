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

#ଅନ୍ତର୍ଭାଵ "hb.hh"
#ଅନ୍ତର୍ଭାଵ "hb-shaper.hh"
#ଅନ୍ତର୍ଭାଵ "hb-machinery.hh"


ସ୍ଥିର ଅଚଳ hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#ଅନ୍ତର୍ଭାଵ "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ free_static_shapers ();

ସ୍ଥିର ସଂରଚନା hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  ସ୍ଥିର hb_shaper_entry_t *create ()
  {
    ଅକ୍ଷର *env = getenv ("HB_SHAPER_LIST");
    ଯଦି (!env || !*env)
      ଫେରନ୍ତୁ nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, ଆକାର (_hb_all_shapers));
    ଯଦି (unlikely (!shapers))
      ଫେରନ୍ତୁ nullptr;

    hb_memcpy (shapers, _hb_all_shapers, ଆକାର (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ଅକ୍ଷର *end, *p = env;
    ପାଇଁ (;;)
    {
      end = strchr (p, ',');
      ଯଦି (!end)
	end = p + strlen (p);

      ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	ଯଦି (end - p == (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 ସଂରଚନା hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], ଆକାର (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      ଯଦି (!*end)
	ଭାଙ୍ଗ;
      ଅନ୍ୟଥା
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    ଫେରନ୍ତୁ shapers;
  }
  ସ୍ଥିର ଶୂନ୍ୟ destroy (hb_shaper_entry_t *p) { hb_free (p); }
  ସ୍ଥିର ଅଚଳ hb_shaper_entry_t *get_null ()      { ଫେରନ୍ତୁ _hb_all_shapers; }
} static_shapers;

ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ free_static_shapers ()
{
  static_shapers.free_instance ();
}

ଅଚଳ hb_shaper_entry_t *
_hb_shapers_get ()
{
  ଫେରନ୍ତୁ static_shapers.get_unconst ();
}

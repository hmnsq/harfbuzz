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

#অন্তর্ভাব "hb.hh"
#অন্তর্ভাব "hb-shaper.hh"
#অন্তর্ভাব "hb-machinery.hh"


স্থির ধ্রুব hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

স্থির অন্তঃস্থ শূন্য free_static_shapers ();

স্থির সংরচনা hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  স্থির hb_shaper_entry_t *create ()
  {
    বর্ণ *env = getenv ("HB_SHAPER_LIST");
    যদি (!env || !*env)
      প্রত্যাগম nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, পরিমাণ (_hb_all_shapers));
    যদি (unlikely (!shapers))
      প্রত্যাগম nullptr;

    hb_memcpy (shapers, _hb_all_shapers, পরিমাণ (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned পূর্ণাঙ্ক i = 0;
    বর্ণ *end, *p = env;
    প্রতি (;;)
    {
      end = strchr (p, ',');
      যদি (!end)
	end = p + strlen (p);

      প্রতি (unsigned পূর্ণাঙ্ক j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	যদি (end - p == (পূর্ণাঙ্ক) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 সংরচনা hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], পরিমাণ (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      যদি (!*end)
	ভঙ্গ;
      অন্যথা
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    প্রত্যাগম shapers;
  }
  স্থির শূন্য destroy (hb_shaper_entry_t *p) { hb_free (p); }
  স্থির ধ্রুব hb_shaper_entry_t *get_null ()      { প্রত্যাগম _hb_all_shapers; }
} static_shapers;

স্থির অন্তঃস্থ
শূন্য free_static_shapers ()
{
  static_shapers.free_instance ();
}

ধ্রুব hb_shaper_entry_t *
_hb_shapers_get ()
{
  প্রত্যাগম static_shapers.get_unconst ();
}

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


স্থির ধ্রুবক hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#অন্তর্ভাব "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_দৈর্ঘ্য_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

স্থির অন্তঃরেখ শূন্য free_static_shapers ();

স্থির কাঠামো hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  স্থির hb_shaper_entry_t *create ()
  {
    অক্ষর *env = getenv ("HB_SHAPER_LIST");
    যদি (!env || !*env)
      ফেরত nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, আকার (_hb_all_shapers));
    যদি (unlikely (!shapers))
      ফেরত nullptr;

    hb_memcpy (shapers, _hb_all_shapers, আকার (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned পূর্ণসংখ্যা i = 0;
    অক্ষর *end, *p = env;
    জন্য (;;)
    {
      end = strchr (p, ',');
      যদি (!end)
	end = p + strlen (p);

      জন্য (unsigned পূর্ণসংখ্যা j = i; j < ARRAY_দৈর্ঘ্য_CONST (_hb_all_shapers); j++)
	যদি (end - p == (পূর্ণসংখ্যা) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 কাঠামো hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], আকার (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      যদি (!*end)
	বিরতি;
      অন্যথা
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    ফেরত shapers;
  }
  স্থির শূন্য destroy (hb_shaper_entry_t *p) { hb_free (p); }
  স্থির ধ্রুবক hb_shaper_entry_t *get_null ()      { ফেরত _hb_all_shapers; }
} static_shapers;

স্থির অন্তঃরেখ
শূন্য free_static_shapers ()
{
  static_shapers.free_instance ();
}

ধ্রুবক hb_shaper_entry_t *
_hb_shapers_get ()
{
  ফেরত static_shapers.get_unconst ();
}

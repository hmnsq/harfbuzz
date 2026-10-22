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

#شامل "hb.hh"
#شامل "hb-shaper.hh"
#شامل "hb-machinery.hh"


ایستا ثابت hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#شامل "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

ایستا درونخطی تهی free_static_shapers ();

ایستا ساختار hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  ایستا hb_shaper_entry_t *create ()
  {
    نویسه *env = getenv ("HB_SHAPER_LIST");
    اگر (!env || !*env)
      بازگشت nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, اندازه (_hb_all_shapers));
    اگر (unlikely (!shapers))
      بازگشت nullptr;

    hb_memcpy (shapers, _hb_all_shapers, اندازه (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned صحیح i = 0;
    نویسه *end, *p = env;
    برای (;;)
    {
      end = strchr (p, ',');
      اگر (!end)
	end = p + strlen (p);

      برای (unsigned صحیح j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	اگر (end - p == (صحیح) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 ساختار hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], اندازه (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      اگر (!*end)
	بشکن;
      وگرنه
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    بازگشت shapers;
  }
  ایستا تهی destroy (hb_shaper_entry_t *p) { hb_free (p); }
  ایستا ثابت hb_shaper_entry_t *get_null ()      { بازگشت _hb_all_shapers; }
} static_shapers;

ایستا درونخطی
تهی free_static_shapers ()
{
  static_shapers.free_instance ();
}

ثابت hb_shaper_entry_t *
_hb_shapers_get ()
{
  بازگشت static_shapers.get_unconst ();
}

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

#include "hb.hh"
#include "hb-shaper.hh"
#include "hb-machinery.hh"


static const hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#include "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

static anta_hstha void free_static_shapers ();

static sa_nracanaa hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  static hb_shaper_entry_t *create ()
  {
    var_na *env = getenv ("HB_SHAPER_LIST");
    ប្រសិនបើ (!env || !*env)
      ត្រឡប់ nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, parimaa_na (_hb_all_shapers));
    ប្រសិនបើ (unlikely (!shapers))
      ត្រឡប់ nullptr;

    hb_memcpy (shapers, _hb_all_shapers, parimaa_na (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned ចំនួនគត់ i = 0;
    var_na *end, *p = env;
    សម្រាប់ (;;)
    {
      end = strchr (p, ',');
      ប្រសិនបើ (!end)
	end = p + strlen (p);

      សម្រាប់ (unsigned ចំនួនគត់ j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	ប្រសិនបើ (end - p == (ចំនួនគត់) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 sa_nracanaa hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], parimaa_na (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      ប្រសិនបើ (!*end)
	bha_nga;
      បើមិនដូច្នេះ
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    ត្រឡប់ shapers;
  }
  static void destroy (hb_shaper_entry_t *p) { hb_free (p); }
  static const hb_shaper_entry_t *get_null ()      { ត្រឡប់ _hb_all_shapers; }
} static_shapers;

static anta_hstha
void free_static_shapers ()
{
  static_shapers.free_instance ();
}

const hb_shaper_entry_t *
_hb_shapers_get ()
{
  ត្រឡប់ static_shapers.get_unconst ();
}

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
    ຖ້າ (!env || !*env)
      ສົ່ງຄືນ nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, parimaa_na (_hb_all_shapers));
    ຖ້າ (unlikely (!shapers))
      ສົ່ງຄືນ nullptr;

    hb_memcpy (shapers, _hb_all_shapers, parimaa_na (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned ຈຳນວນເຕັມ i = 0;
    var_na *end, *p = env;
    ສຳລັບ (;;)
    {
      end = strchr (p, ',');
      ຖ້າ (!end)
	end = p + strlen (p);

      ສຳລັບ (unsigned ຈຳນວນເຕັມ j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	ຖ້າ (end - p == (ຈຳນວນເຕັມ) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 sa_nracanaa hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], parimaa_na (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      ຖ້າ (!*end)
	bha_nga;
      ບໍ່ດັ່ງນັ້ນ
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    ສົ່ງຄືນ shapers;
  }
  static void destroy (hb_shaper_entry_t *p) { hb_free (p); }
  static const hb_shaper_entry_t *get_null ()      { ສົ່ງຄືນ _hb_all_shapers; }
} static_shapers;

static anta_hstha
void free_static_shapers ()
{
  static_shapers.free_instance ();
}

const hb_shaper_entry_t *
_hb_shapers_get ()
{
  ສົ່ງຄືນ static_shapers.get_unconst ();
}

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

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-shaper.hh"
#अन्तर्भाव "hb-machinery.hh"


स्थिर ध्रुव hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

स्थिर अन्तःस्थ शून्य free_static_shapers ();

स्थिर संरचना hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  स्थिर hb_shaper_entry_t *create ()
  {
    वर्ण *env = getenv ("HB_SHAPER_LIST");
    यदि (!env || !*env)
      प्रत्यागम nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, परिमाण (_hb_all_shapers));
    यदि (unlikely (!shapers))
      प्रत्यागम nullptr;

    hb_memcpy (shapers, _hb_all_shapers, परिमाण (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    unsigned पूर्णाङ्क i = 0;
    वर्ण *end, *p = env;
    प्रति (;;)
    {
      end = strchr (p, ',');
      यदि (!end)
	end = p + strlen (p);

      प्रति (unsigned पूर्णाङ्क j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	यदि (end - p == (पूर्णाङ्क) strlen (shapers[j].name) &&
	    0 == strncmp (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 संरचना hb_shaper_entry_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], परिमाण (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      यदि (!*end)
	भङ्ग;
      अन्यथा
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    प्रत्यागम shapers;
  }
  स्थिर शून्य destroy (hb_shaper_entry_t *p) { hb_free (p); }
  स्थिर ध्रुव hb_shaper_entry_t *get_null ()      { प्रत्यागम _hb_all_shapers; }
} static_shapers;

स्थिर अन्तःस्थ
शून्य free_static_shapers ()
{
  static_shapers.free_instance ();
}

ध्रुव hb_shaper_entry_t *
_hb_shapers_get ()
{
  प्रत्यागम static_shapers.get_unconst ();
}

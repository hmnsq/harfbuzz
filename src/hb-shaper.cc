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


स्थिर स्थिरांक hb_shaper_entry_t _hb_all_shapers[] = {
#define HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#अन्तर्भाव "hb-shaper-list.hh"
#undef HB_SHAPER_IMPLEMENT
};
#ifndef HB_NO_SHAPER
static_assert (0 != ARRAY_LENGTH_CONST (_hb_all_shapers), "No shaper enabled.");
#endif

स्थिर ओळीत शून्य free_static_shapers ();

स्थिर संरचना hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  स्थिर hb_shaper_entry_t *create ()
  {
    अक्षर *env = दो_पर्या ("HB_SHAPER_LIST");
    जर (!env || !*env)
      परत nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, आकार (_hb_all_shapers));
    जर (unlikely (!shapers))
      परत nullptr;

    hb_memcpy (shapers, _hb_all_shapers, आकार (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    अचिन्हित पूर्णांक i = 0;
    अक्षर *end, *p = env;
    साठी (;;)
    {
      end = म_अक्षर (p, ',');
      जर (!end)
	end = p + म_माप (p);

      साठी (अचिन्हित पूर्णांक j = i; j < ARRAY_LENGTH_CONST (_hb_all_shapers); j++)
	जर (end - p == (पूर्णांक) म_माप (shapers[j].name) &&
	    0 == म_भेदन (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 संरचना hb_shaper_entry_t t = shapers[j];
	 स_हटाओ (&shapers[i + 1], &shapers[i], आकार (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      जर (!*end)
	थांबा;
      अन्यथा
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    परत shapers;
  }
  स्थिर शून्य destroy (hb_shaper_entry_t *p) { hb_free (p); }
  स्थिर स्थिरांक hb_shaper_entry_t *get_null ()      { परत _hb_all_shapers; }
} static_shapers;

स्थिर ओळीत
शून्य free_static_shapers ()
{
  static_shapers.free_instance ();
}

स्थिरांक hb_shaper_entry_t *
_hb_shapers_get ()
{
  परत static_shapers.get_unconst ();
}

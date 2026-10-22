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

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-machinery.hh"


ਸਥਿਰ ਅਚਲ hb_shaper_ਇੰਦਰਾਜ_t _hb_all_shapers[] = {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-shaper-list.hh"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ HB_SHAPER_IMPLEMENT
};
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_NO_SHAPER
static_assert (0 != ARRAY_ਲੰਮਾਈ_CONST (_hb_all_shapers), "No shaper enabled.");
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ free_static_shapers ();

ਸਥਿਰ ਬਣਤਰ hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_ਇੰਦਰਾਜ_t,
							  hb_shapers_lazy_loader_t>
{
  ਸਥਿਰ hb_shaper_ਇੰਦਰਾਜ_t *create ()
  {
    ਅੱਖਰ *env = getenv ("HB_SHAPER_LIST");
    ਜੇ (!env || !*env)
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    hb_shaper_ਇੰਦਰਾਜ_t *shapers = (hb_shaper_ਇੰਦਰਾਜ_t *) hb_calloc (1, ਆਕਾਰ (_hb_all_shapers));
    ਜੇ (unlikely (!shapers))
      ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;

    hb_memcpy (shapers, _hb_all_shapers, ਆਕਾਰ (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0;
    ਅੱਖਰ *ਸਿਰਾ, *p = env;
    ਲਈ (;;)
    {
      ਸਿਰਾ = strchr (p, ',');
      ਜੇ (!ਸਿਰਾ)
	ਸਿਰਾ = p + ਲੜੀ_ਲੰਬਾਈ (p);

      ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ j = i; j < ARRAY_ਲੰਮਾਈ_CONST (_hb_all_shapers); j++)
	ਜੇ (ਸਿਰਾ - p == (ਪੂਰਨ) ਲੜੀ_ਲੰਬਾਈ (shapers[j].name) &&
	    0 == ਲੜੀ_ਤੁਲਨਾ (shapers[j].name, p, ਸਿਰਾ - p))
	{
	  /* Reorder this shaper to position i */
	 ਬਣਤਰ hb_shaper_ਇੰਦਰਾਜ_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], ਆਕਾਰ (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      ਜੇ (!*ਸਿਰਾ)
	ਰੋਕੋ;
      ਨਹੀਂਤਾਂ
	p = ਸਿਰਾ + 1;
    }

    hb_atexit (free_static_shapers);

    ਵਾਪਸ shapers;
  }
  ਸਥਿਰ ਖਾਲੀ destroy (hb_shaper_ਇੰਦਰਾਜ_t *p) { hb_free (p); }
  ਸਥਿਰ ਅਚਲ hb_shaper_ਇੰਦਰਾਜ_t *get_null ()      { ਵਾਪਸ _hb_all_shapers; }
} static_shapers;

ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ free_static_shapers ()
{
  static_shapers.free_instance ();
}

ਅਚਲ hb_shaper_ਇੰਦਰਾਜ_t *
_hb_shapers_get ()
{
  ਵਾਪਸ static_shapers.get_unconst ();
}

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

#ضمن "hb.hh"
#ضمن "hb-shaper.hh"
#ضمن "hb-machinery.hh"


ساكن ثابت hb_shaper_اندراج_t _hb_all_shapers[] = {
#تعریف_کرو HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#ضمن "hb-shaper-list.hh"
#تعریف_رد HB_SHAPER_IMPLEMENT
};
#جے_بنا_تعریف HB_NO_SHAPER
static_assert (0 != ARRAY_لمائی_CONST (_hb_all_shapers), "No shaper enabled.");
#جے_انت

ساكن مضمن فراغ free_static_shapers ();

ساكن بنية hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_اندراج_t,
							  hb_shapers_lazy_loader_t>
{
  ساكن hb_shaper_اندراج_t *create ()
  {
    حرف *env = getenv ("HB_SHAPER_LIST");
    إذا (!env || !*env)
      أرجع خالی_سوچک;

    hb_shaper_اندراج_t *shapers = (hb_shaper_اندراج_t *) hb_calloc (1, حجم (_hb_all_shapers));
    إذا (unlikely (!shapers))
      أرجع خالی_سوچک;

    hb_memcpy (shapers, _hb_all_shapers, حجم (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    بنانشانی صحيح i = 0;
    حرف *سرا, *p = env;
    لكل (;;)
    {
      سرا = strchr (p, ',');
      إذا (!سرا)
	سرا = p + لڑی_لمبائی (p);

      لكل (بنانشانی صحيح j = i; j < ARRAY_لمائی_CONST (_hb_all_shapers); j++)
	إذا (سرا - p == (صحيح) لڑی_لمبائی (shapers[j].name) &&
	    0 == لڑی_مقابلہ (shapers[j].name, p, سرا - p))
	{
	  /* Reorder this shaper to position i */
	 بنية hb_shaper_اندراج_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], حجم (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      إذا (!*سرا)
	اكسر;
      وإلا
	p = سرا + 1;
    }

    hb_atexit (free_static_shapers);

    أرجع shapers;
  }
  ساكن فراغ destroy (hb_shaper_اندراج_t *p) { hb_free (p); }
  ساكن ثابت hb_shaper_اندراج_t *get_null ()      { أرجع _hb_all_shapers; }
} static_shapers;

ساكن مضمن
فراغ free_static_shapers ()
{
  static_shapers.free_instance ();
}

ثابت hb_shaper_اندراج_t *
_hb_shapers_get ()
{
  أرجع static_shapers.get_unconst ();
}

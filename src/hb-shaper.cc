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


ساکن ثابت hb_shaper_اندراج_t _hb_all_shapers[] = {
#تعریف_کرو HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#شامل "hb-shaper-list.hh"
#تعریف_منسوخ HB_SHAPER_IMPLEMENT
};
#اگر_غیر_تعریف_شدہ HB_NO_SHAPER
static_assert (0 != ARRAY_لمبائی_CONST (_hb_all_shapers), "No shaper enabled.");
#اگر_ختم

ساکن اندرونی خالی free_static_shapers ();

ساکن ساخت hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_اندراج_t,
							  hb_shapers_lazy_loader_t>
{
  ساکن hb_shaper_اندراج_t *create ()
  {
    حرف *env = getenv ("HB_SHAPER_LIST");
    اگر (!env || !*env)
      واپس خالی_اشاریہ;

    hb_shaper_اندراج_t *shapers = (hb_shaper_اندراج_t *) hb_calloc (1, حجم (_hb_all_shapers));
    اگر (unlikely (!shapers))
      واپس خالی_اشاریہ;

    hb_memcpy (shapers, _hb_all_shapers, حجم (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    غیرعلامتی صحیح i = 0;
    حرف *انتہا, *p = env;
    برائے (;;)
    {
      انتہا = strchr (p, ',');
      اگر (!انتہا)
	انتہا = p + لڑی_لمبائی (p);

      برائے (غیرعلامتی صحیح j = i; j < ARRAY_لمبائی_CONST (_hb_all_shapers); j++)
	اگر (انتہا - p == (صحیح) لڑی_لمبائی (shapers[j].name) &&
	    0 == لڑی_موازنہ (shapers[j].name, p, انتہا - p))
	{
	  /* Reorder this shaper to position i */
	 ساخت hb_shaper_اندراج_t t = shapers[j];
	 memmove (&shapers[i + 1], &shapers[i], حجم (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      اگر (!*انتہا)
	توڑو;
      ورنہ
	p = انتہا + 1;
    }

    hb_atexit (free_static_shapers);

    واپس shapers;
  }
  ساکن خالی destroy (hb_shaper_اندراج_t *p) { hb_free (p); }
  ساکن ثابت hb_shaper_اندراج_t *get_null ()      { واپس _hb_all_shapers; }
} static_shapers;

ساکن اندرونی
خالی free_static_shapers ()
{
  static_shapers.free_instance ();
}

ثابت hb_shaper_اندراج_t *
_hb_shapers_get ()
{
  واپس static_shapers.get_unconst ();
}

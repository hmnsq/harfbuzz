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

#समावेश "hb.hh"
#समावेश "hb-shaper.hh"
#समावेश "hb-machinery.hh"


अटल स्थिर hb_shaper_entry_t _hb_all_shapers[] = {
#घोषणा HB_SHAPER_IMPLEMENT(name) {#name, _hb_##name##_shape},
#समावेश "hb-shaper-list.hh"
#अघोषित HB_SHAPER_IMPLEMENT
};
#अगर_अघोषित HB_NO_SHAPER
static_assert (0 != ARRAY_लंबाई_CONST (_hb_all_shapers), "No shaper enabled.");
#पूर्ण_अगर

अटल अंतरभूत व्योम free_static_shapers ();

अटल काष्ठा hb_shapers_lazy_loader_t : hb_lazy_loader_t<hb_shaper_entry_t,
							  hb_shapers_lazy_loader_t>
{
  अटल hb_shaper_entry_t *create ()
  {
    अक्षर *env = दो_पर्या ("HB_SHAPER_LIST");
    अगर (!env || !*env)
      वापस nullptr;

    hb_shaper_entry_t *shapers = (hb_shaper_entry_t *) hb_calloc (1, माप (_hb_all_shapers));
    अगर (unlikely (!shapers))
      वापस nullptr;

    hb_memcpy (shapers, _hb_all_shapers, माप (_hb_all_shapers));

     /* Reorder shaper list to prefer requested shapers. */
    अचिन्हित पूर्णांक i = 0;
    अक्षर *end, *p = env;
    क्रम (;;)
    {
      end = म_अक्षर (p, ',');
      अगर (!end)
	end = p + म_माप (p);

      क्रम (अचिन्हित पूर्णांक j = i; j < ARRAY_लंबाई_CONST (_hb_all_shapers); j++)
	अगर (end - p == (पूर्णांक) म_माप (shapers[j].name) &&
	    0 == म_भेदन (shapers[j].name, p, end - p))
	{
	  /* Reorder this shaper to position i */
	 काष्ठा hb_shaper_entry_t t = shapers[j];
	 स_हटाओ (&shapers[i + 1], &shapers[i], माप (shapers[i]) * (j - i));
	 shapers[i] = t;
	 i++;
	}

      अगर (!*end)
	अवरोध;
      अन्यथा
	p = end + 1;
    }

    hb_atexit (free_static_shapers);

    वापस shapers;
  }
  अटल व्योम destroy (hb_shaper_entry_t *p) { hb_free (p); }
  अटल स्थिर hb_shaper_entry_t *get_null ()      { वापस _hb_all_shapers; }
} static_shapers;

अटल अंतरभूत
व्योम free_static_shapers ()
{
  static_shapers.free_instance ();
}

स्थिर hb_shaper_entry_t *
_hb_shapers_get ()
{
  वापस static_shapers.get_unconst ();
}

/*
 * Copyright © 2007,2008,2009,2010  Red Hat, Inc.
 * Copyright © 2012,2018  Google, Inc.
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
 * Red Hat Author(s): Behdad Esfahbod
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_DISPATCH_HH
#define HB_DISPATCH_HH

#अन्तर्भाव "hb.hh"

/*
 * Dispatch
 */

ढाँचा <typename Context, typename Return=hb_empty_t, अचिन्हित पूर्णांक MaxDebugDepth=0>
संरचना hb_dispatch_context_t
{
  खाजगी:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  स्थिरांक Context* thiz () स्थिरांक { परत static_cast<स्थिरांक Context *> (हे); }
	Context* thiz ()       { परत static_cast<      Context *> (हे); }
  सार्वजनिक:
  स्थिरांक अक्षर *get_name () { परत "UNKNOWN"; }
  स्थिर constexpr अचिन्हित max_debug_depth = MaxDebugDepth;
  प्रकारव्याख्या Return return_t;
  ढाँचा <typename T, typename F>
  तार्किक may_dispatch (स्थिरांक T *obj HB_UNUSED, स्थिरांक F *format HB_UNUSED) { परत सत्य; }
  ढाँचा <typename T, typename ...Ts>
  return_t dispatch (स्थिरांक T &obj, Ts&&... ds)
  { परत obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  स्थिर return_t no_dispatch_return_मूल्य () { परत Context::default_return_मूल्य (); }
  स्थिर तार्किक stop_sublookup_iteration (स्थिरांक return_t r HB_UNUSED) { परत असत्य; }
  अचिन्हित debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

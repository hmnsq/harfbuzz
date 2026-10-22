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

template <typename Context, typename Return=hb_empty_t, unsigned पूर्णाङ्क MaxDebugDepth=0>
संरचना hb_dispatch_context_t
{
  गुप्त:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ध्रुव Context* thiz () ध्रुव { पटिनिवत्त static_cast<ध्रुव Context *> (एतत्); }
	Context* thiz ()       { पटिनिवत्त static_cast<      Context *> (एतत्); }
  सार्वजनिक:
  ध्रुव वर्ण *get_name () { पटिनिवत्त "UNKNOWN"; }
  स्थिर constexpr unsigned max_debug_depth = MaxDebugDepth;
  प्रकारनिर्देश Return return_t;
  template <typename T, typename F>
  तार्किक may_dispatch (ध्रुव T *obj HB_UNUSED, ध्रुव F *format HB_UNUSED) { पटिनिवत्त सच्च; }
  template <typename T, typename ...Ts>
  return_t dispatch (ध्रुव T &obj, Ts&&... ds)
  { पटिनिवत्त obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  स्थिर return_t no_dispatch_return_मानम् () { पटिनिवत्त Context::default_return_मानम् (); }
  स्थिर तार्किक stop_sublookup_iteration (ध्रुव return_t r HB_UNUSED) { पटिनिवत्त मिच्छा; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

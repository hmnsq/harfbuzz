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

#ଅନ୍ତର୍ଭାଵ "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା MaxDebugDepth=0>
ସଂରଚନା hb_dispatch_context_t
{
  ବ୍ୟକ୍ତିଗତ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ଅଚଳ Context* thiz () ଅଚଳ { ଫେରନ୍ତୁ static_cast<ଅଚଳ Context *> (ଏହି); }
	Context* thiz ()       { ଫେରନ୍ତୁ static_cast<      Context *> (ଏହି); }
  ସର୍ବସାଧାରଣ:
  ଅଚଳ ଅକ୍ଷର *get_name () { ଫେରନ୍ତୁ "UNKNOWN"; }
  ସ୍ଥିର constexpr unsigned max_debug_depth = MaxDebugDepth;
  ପ୍ରକାରବ୍ୟାଖ୍ୟା Return return_t;
  template <typename T, typename F>
  ତାର୍କିକ may_dispatch (ଅଚଳ T *obj HB_UNUSED, ଅଚଳ F *format HB_UNUSED) { ଫେରନ୍ତୁ ସତ୍ୟ; }
  template <typename T, typename ...Ts>
  return_t dispatch (ଅଚଳ T &obj, Ts&&... ds)
  { ଫେରନ୍ତୁ obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ସ୍ଥିର return_t no_dispatch_return_ମୂଲ୍ୟ () { ଫେରନ୍ତୁ Context::default_return_ମୂଲ୍ୟ (); }
  ସ୍ଥିର ତାର୍କିକ stop_sublookup_iteration (ଅଚଳ return_t r HB_UNUSED) { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

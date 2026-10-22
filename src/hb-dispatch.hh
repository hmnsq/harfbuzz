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

#অন্তর্ভাব "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned পূর্ণাঙ্ক MaxDebugDepth=0>
সংরচনা hb_dispatch_context_t
{
  গুপ্ত:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুব Context* thiz () ধ্রুব { প্রত্যাগম static_cast<ধ্রুব Context *> (এতত্); }
	Context* thiz ()       { প্রত্যাগম static_cast<      Context *> (এতত্); }
  সার্বজনিক:
  ধ্রুব বর্ণ *get_name () { প্রত্যাগম "UNKNOWN"; }
  স্থির constexpr unsigned max_debug_depth = MaxDebugDepth;
  প্রকারনির্দেশ Return return_t;
  template <typename T, typename F>
  তার্কিক may_dispatch (ধ্রুব T *obj HB_UNUSED, ধ্রুব F *format HB_UNUSED) { প্রত্যাগম সত্য; }
  template <typename T, typename ...Ts>
  return_t dispatch (ধ্রুব T &obj, Ts&&... ds)
  { প্রত্যাগম obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  স্থির return_t no_dispatch_return_মান () { প্রত্যাগম Context::default_return_মান (); }
  স্থির তার্কিক stop_sublookup_iteration (ধ্রুব return_t r HB_UNUSED) { প্রত্যাগম অসত্য; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

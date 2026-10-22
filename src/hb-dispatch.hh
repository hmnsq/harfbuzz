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

template <typename Context, typename Return=hb_empty_t, unsigned পূর্ণসংখ্যা MaxDebugDepth=0>
কাঠামো hb_dispatch_context_t
{
  ব্যক্তিগত:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ধ্রুবক Context* thiz () ধ্রুবক { ফেরত static_cast<ধ্রুবক Context *> (এই); }
	Context* thiz ()       { ফেরত static_cast<      Context *> (এই); }
  সর্বজনীন:
  ধ্রুবক অক্ষর *get_name () { ফেরত "UNKNOWN"; }
  স্থির constexpr unsigned max_debug_depth = MaxDebugDepth;
  প্রকারলেখ Return return_t;
  template <typename T, typename F>
  যৌক্তিক may_dispatch (ধ্রুবক T *obj HB_UNUSED, ধ্রুবক F *format HB_UNUSED) { ফেরত সত্য; }
  template <typename T, typename ...Ts>
  return_t dispatch (ধ্রুবক T &obj, Ts&&... ds)
  { ফেরত obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  স্থির return_t no_dispatch_return_মান () { ফেরত Context::default_return_মান (); }
  স্থির যৌক্তিক stop_sublookup_iteration (ধ্রুবক return_t r HB_UNUSED) { ফেরত মিথ্যা; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

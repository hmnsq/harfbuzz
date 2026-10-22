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

#ضمن "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned صحيح MaxDebugDepth=0>
بنية hb_dispatch_context_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت Context* thiz () ثابت { أرجع static_cast<ثابت Context *> (هذا); }
	Context* thiz ()       { أرجع static_cast<      Context *> (هذا); }
  عام:
  ثابت حرف *get_name () { أرجع "UNKNOWN"; }
  ساكن constexpr unsigned max_debug_depth = MaxDebugDepth;
  تعريفنوع Return return_t;
  template <typename T, typename F>
  منطقي may_dispatch (ثابت T *obj HB_UNUSED, ثابت F *format HB_UNUSED) { أرجع 𐡒𐡔𐡈; }
  template <typename T, typename ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ساكن return_t no_dispatch_return_value () { أرجع Context::default_return_value (); }
  ساكن منطقي stop_sublookup_iteration (ثابت return_t r HB_UNUSED) { أرجع خطأ; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

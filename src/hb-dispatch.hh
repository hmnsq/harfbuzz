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

#که_نه_تعریف_شوی HB_DISPATCH_HH
#تعریف_کړه HB_DISPATCH_HH

#ضمن "hb.hh"

/*
 * Dispatch
 */

کينډۍ <ډول_نوم Context, ډول_نوم Return=hb_تشه_t, بینښه صحيح Max_Debug_ژوروالی=0>
بنية hb_dispatch_context_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت Context* thiz () ثابت { أرجع static_cast<ثابت Context *> (هذا); }
	Context* thiz ()       { أرجع static_cast<      Context *> (هذا); }
  عام:
  ثابت حرف *get_name () { أرجع "UNKNOWN"; }
  ساكن constexpr بینښه max_debug_ژوروالی = Max_Debug_ژوروالی;
  تعريفنوع Return return_t;
  کينډۍ <ډول_نوم T, ډول_نوم F>
  منطقي may_dispatch (ثابت T *obj HB_ناکارول_شوی, ثابت F *format HB_ناکارول_شوی) { أرجع ریښتیا; }
  کينډۍ <ډول_نوم T, ډول_نوم ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ساكن return_t no_dispatch_return_ارزښت () { أرجع Context::default_return_ارزښت (); }
  ساكن منطقي stop_sublookup_iteration (ثابت return_t r HB_ناکارول_شوی) { أرجع خطأ; }
  بینښه debug_ژوروالی = 0;
};


#د_که_پای /* HB_DISPATCH_HH */

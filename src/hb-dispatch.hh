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

#جے_بنا_تعریف HB_DISPATCH_HH
#تعریف_کرو HB_DISPATCH_HH

#ضمن "hb.hh"

/*
 * Dispatch
 */

سانچا <قسم_ناں Context, قسم_ناں Return=hb_خالی_تھاواں_t, بنانشانی صحيح Max_Debug_ڈونگھائی=0>
بنية hb_dispatch_context_t
{
  خاص:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ثابت Context* thiz () ثابت { أرجع static_cast<ثابت Context *> (هذا); }
	Context* thiz ()       { أرجع static_cast<      Context *> (هذا); }
  عام:
  ثابت حرف *get_name () { أرجع "UNKNOWN"; }
  ساكن constexpr بنانشانی max_debug_ڈونگھائی = Max_Debug_ڈونگھائی;
  تعريفنوع Return return_t;
  سانچا <قسم_ناں T, قسم_ناں F>
  منطقي may_dispatch (ثابت T *obj HB_بن_ورتیا, ثابت F *format HB_بن_ورتیا) { أرجع سچ; }
  سانچا <قسم_ناں T, قسم_ناں ...Ts>
  return_t dispatch (ثابت T &obj, Ts&&... ds)
  { أرجع obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ساكن return_t no_dispatch_return_قدر () { أرجع Context::default_return_قدر (); }
  ساكن منطقي stop_sublookup_iteration (ثابت return_t r HB_بن_ورتیا) { أرجع خطأ; }
  بنانشانی debug_ڈونگھائی = 0;
};


#جے_انت /* HB_DISPATCH_HH */

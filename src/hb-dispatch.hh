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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DISPATCH_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DISPATCH_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"

/*
 * Dispatch
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Context, ਕਿਸਮ_ਨਾਂ Return=hb_ਖਾਲੀ_ਥਾਵਾਂ_t, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ Max_Debug_ਡੂੰਘਾਈ=0>
ਬਣਤਰ hb_dispatch_context_t
{
  ਨਿਜੀ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ਅਚਲ Context* thiz () ਅਚਲ { ਵਾਪਸ static_cast<ਅਚਲ Context *> (ਇਹ); }
	Context* thiz ()       { ਵਾਪਸ static_cast<      Context *> (ਇਹ); }
  ਜਨਤਕ:
  ਅਚਲ ਅੱਖਰ *get_name () { ਵਾਪਸ "UNKNOWN"; }
  ਸਥਿਰ constexpr ਬਿਨਾਨਿਸ਼ਾਨੀ max_debug_ਡੂੰਘਾਈ = Max_Debug_ਡੂੰਘਾਈ;
  ਕਿਸਮਪਰਿਭਾਸ਼ਾ Return return_t;
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ F>
  ਤਾਰਕਿਕ may_dispatch (ਅਚਲ T *obj HB_ਅਣਵਰਤਿਆ, ਅਚਲ F *format HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ ਸੱਚ; }
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T, ਕਿਸਮ_ਨਾਂ ...Ts>
  return_t dispatch (ਅਚਲ T &obj, Ts&&... ds)
  { ਵਾਪਸ obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ਸਥਿਰ return_t no_dispatch_return_ਮੁੱਲ () { ਵਾਪਸ Context::default_return_ਮੁੱਲ (); }
  ਸਥਿਰ ਤਾਰਕਿਕ stop_sublookup_iteration (ਅਚਲ return_t r HB_ਅਣਵਰਤਿਆ) { ਵਾਪਸ ਝੂਠ; }
  ਬਿਨਾਨਿਸ਼ਾਨੀ debug_ਡੂੰਘਾਈ = 0;
};


#ਜੇ_ਅੰਤ /* HB_DISPATCH_HH */

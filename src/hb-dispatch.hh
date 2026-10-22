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

#ಅನ್ತರ್ಭಾವ "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned ಪೂರ್ಣಾಙ್ಕ MaxDebugDepth=0>
ಸಂರಚನಾ hb_dispatch_context_t
{
  ಗುಪ್ತ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  ಧ್ರುವ Context* thiz () ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ static_cast<ಧ್ರುವ Context *> (ಏತತ್); }
	Context* thiz ()       { ಪ್ರತ್ಯಾಗಮ static_cast<      Context *> (ಏತತ್); }
  ಸಾರ್ವಜನಿಕ:
  ಧ್ರುವ ವರ್ಣ *get_name () { ಪ್ರತ್ಯಾಗಮ "UNKNOWN"; }
  ಸ್ಥಿರ constexpr unsigned max_debug_depth = MaxDebugDepth;
  ಪ್ರಕಾರನಿರ್ದೇಶ Return return_t;
  template <typename T, typename F>
  ತಾರ್ಕಿಕ may_dispatch (ಧ್ರುವ T *obj HB_UNUSED, ಧ್ರುವ F *format HB_UNUSED) { ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ; }
  template <typename T, typename ...Ts>
  return_t dispatch (ಧ್ರುವ T &obj, Ts&&... ds)
  { ಪ್ರತ್ಯಾಗಮ obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  ಸ್ಥಿರ return_t no_dispatch_return_ಮೌಲ್ಯ () { ಪ್ರತ್ಯಾಗಮ Context::default_return_ಮೌಲ್ಯ (); }
  ಸ್ಥಿರ ತಾರ್ಕಿಕ stop_sublookup_iteration (ಧ್ರುವ return_t r HB_UNUSED) { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

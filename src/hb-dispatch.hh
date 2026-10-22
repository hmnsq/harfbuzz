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

#അന്തര്ഭാവ "hb.hh"

/*
 * Dispatch
 */

template <typename Context, typename Return=hb_empty_t, unsigned പൂർണ്ണസംഖ്യ MaxDebugDepth=0>
ഘടന hb_dispatch_context_t
{
  സ്വകാര്യ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  സ്ഥിരം Context* thiz () സ്ഥിരം { തിരികെ static_cast<സ്ഥിരം Context *> (ഇത്); }
	Context* thiz ()       { തിരികെ static_cast<      Context *> (ഇത്); }
  പൊതു:
  സ്ഥിരം അക്ഷരം *get_name () { തിരികെ "UNKNOWN"; }
  സ്ഥിതം constexpr unsigned max_debug_depth = MaxDebugDepth;
  തരനിർവചനം Return return_t;
  template <typename T, typename F>
  തർക്കം may_dispatch (സ്ഥിരം T *obj HB_UNUSED, സ്ഥിരം F *format HB_UNUSED) { തിരികെ സത്യം; }
  template <typename T, typename ...Ts>
  return_t dispatch (സ്ഥിരം T &obj, Ts&&... ds)
  { തിരികെ obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  സ്ഥിതം return_t no_dispatch_return_മൂല്യം () { തിരികെ Context::default_return_മൂല്യം (); }
  സ്ഥിതം തർക്കം stop_sublookup_iteration (സ്ഥിരം return_t r HB_UNUSED) { തിരികെ അസത്യം; }
  unsigned debug_depth = 0;
};


#endif /* HB_DISPATCH_HH */

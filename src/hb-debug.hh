/*
 * Copyright © 2017  Google, Inc.
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
 * Google Author(s): Behdad Esfahbod
 */

#جے_بنا_تعریف HB_DEBUG_HH
#تعریف_کرو HB_DEBUG_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-algs.hh"


#جے_بنا_تعریف HB_DEBUG
#تعریف_کرو HB_DEBUG 0
#جے_انت


/*
 * Debug output (needs enabling at compile time.)
 */

ساكن مضمن منطقي
_hb_debug (بنانشانی صحيح level,
	   بنانشانی صحيح max_level)
{
  أرجع level < max_level;
}

#تعریف_کرو DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#تعریف_کرو DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ساكن مضمن فراغ
_hb_print_func (ثابت حرف *func)
{
  إذا (func)
  {
    بنانشانی صحيح func_len = لڑی_لمبائی (func);
    /* Skip "static" */
    إذا (0 == لڑی_مقابلہ (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    إذا (0 == لڑی_مقابلہ (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ثابت حرف *space = strchr (func, ' ');
    إذا (space)
      func = space + 1;
    /* Skip parameter list */
    ثابت حرف *paren = strchr (func, '(');
    إذا (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (صحيح) func_len, func);
  }
}

سانچا <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  بنانشانی صحيح level,
		  صحيح level_dir,
		  ثابت حرف *سنیہا,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
سانچا <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  بنانشانی صحيح level,
		  صحيح level_dir,
		  ثابت حرف *سنیہا,
		  va_list ap)
{
  إذا (!_hb_debug (level, max_level))
    أرجع;

  fprintf (stderr, "%-10s", what ? what : "");

  إذا (obj)
    fprintf (stderr, "(%*p) ", (صحيح) (2 * حجم (فراغ *)), obj);
  وإلا
    fprintf (stderr, " %*s  ", (صحيح) (2 * حجم (فراغ *)), "");

  إذا (indented) {
#تعریف_کرو VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#تعریف_کرو VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#تعریف_کرو DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#تعریف_کرو ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#تعریف_کرو LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ساكن ثابت حرف bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + حجم (bars) - 1 - hb_min ((بنانشانی صحيح) حجم (bars) - 1, (بنانشانی صحيح) (حجم (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } وإلا
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  إذا (سنیہا)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, سنیہا, ap);
  }

  fprintf (stderr, "\n");
}
سانچا <> مضمن فراغ HB_PRINTF_FUNC(7, 0)
hb_debug_پیغام_va<0> (ثابت حرف *what HB_بن_ورتیا,
		     ثابت فراغ *obj HB_بن_ورتیا,
		     ثابت حرف *func HB_بن_ورتیا,
		     منطقي indented HB_بن_ورتیا,
		     بنانشانی صحيح level HB_بن_ورتیا,
		     صحيح level_dir HB_بن_ورتیا,
		     ثابت حرف *سنیہا HB_بن_ورتیا,
		     va_list ap HB_بن_ورتیا) {}

سانچا <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیغام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       بنانشانی صحيح level,
	       صحيح level_dir,
	       ثابت حرف *سنیہا,
	       ...) HB_PRINTF_FUNC(7, 8);
سانچا <صحيح max_level> ساكن مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       بنانشانی صحيح level,
	       صحيح level_dir,
	       ثابت حرف *سنیہا,
	       ...)
{
  va_list ap;
  va_شروع (ap, سنیہا);
  hb_debug_پیغام_va<max_level> (what, obj, func, indented, level, level_dir, سنیہا, ap);
  va_سرا (ap);
}
سانچا <> مضمن فراغ
hb_debug_پیغام<0> (ثابت حرف *what HB_بن_ورتیا,
		  ثابت فراغ *obj HB_بن_ورتیا,
		  ثابت حرف *func HB_بن_ورتیا,
		  منطقي indented HB_بن_ورتیا,
		  بنانشانی صحيح level HB_بن_ورتیا,
		  صحيح level_dir HB_بن_ورتیا,
		  ثابت حرف *سنیہا HB_بن_ورتیا,
		  ...) HB_PRINTF_FUNC(7, 8);
سانچا <> مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیغام<0> (ثابت حرف *what HB_بن_ورتیا,
		  ثابت فراغ *obj HB_بن_ورتیا,
		  ثابت حرف *func HB_بن_ورتیا,
		  منطقي indented HB_بن_ورتیا,
		  بنانشانی صحيح level HB_بن_ورتیا,
		  صحيح level_dir HB_بن_ورتیا,
		  ثابت حرف *سنیہا HB_بن_ورتیا,
		  ...) {}

#تعریف_کرو DEBUG_پیغام_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), خالی_سوچک,    سچ, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#تعریف_کرو DEBUG_پیغام(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), خالی_سوچک,    خطأ, 0, 0, __VA_ARGS__)
#تعریف_کرو DEBUG_پیغام_FUNC(WHAT, OBJ, ...)				hb_debug_پیغام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, خطأ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

سانچا <قسم_ناں T>
بنية hb_printer_t {
  ثابت حرف *print (ثابت T&) { أرجع "something"; }
};

سانچا <>
بنية hb_printer_t<منطقي> {
  ثابت حرف *print (منطقي v) { أرجع v ? "true" : "false"; }
};

سانچا <>
بنية hb_printer_t<hb_خالی_تھاواں_t> {
  ثابت حرف *print (hb_خالی_تھاواں_t) { أرجع ""; }
};


/*
 * Trace
 */

سانچا <قسم_ناں T>
ساكن مضمن فراغ _hb_warn_no_return (منطقي returned)
{
  إذا (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
سانچا <>
/*static*/ مضمن فراغ _hb_warn_no_return<hb_خالی_تھاواں_t> (منطقي returned HB_بن_ورتیا) {}
سانچا <>
/*static*/ مضمن فراغ _hb_warn_no_return<فراغ> (منطقي returned HB_بن_ورتیا) {}

سانچا <صحيح max_level, قسم_ناں ret_t>
بنية hb_auto_trace_t
{
  واضح مضمن hb_auto_trace_t (بنانشانی صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *سنیہا,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (خطأ)
  {
    إذا (plevel) ++*plevel;

    va_list ap;
    va_شروع (ap, سنیہا);
    hb_debug_پیغام_va<max_level> (what, obj, func, سچ, plevel ? *plevel : 0, +1, سنیہا, ap);
    va_سرا (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    إذا (!returned) {
      hb_debug_پیغام<max_level> (what, obj, خالی_سوچک, سچ, plevel ? *plevel : 1, -1, " ");
    }
    إذا (plevel) --*plevel;
  }

  سانچا <قسم_ناں T>
  T ret (T&& v,
	 ثابت حرف *func = "",
	 بنانشانی صحيح سطر = 0)
  {
    إذا (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      أرجع std::forward<T> (v);
    }

    hb_debug_پیغام<max_level> (what, obj, func, سچ, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), سطر);
    إذا (plevel) --*plevel;
    plevel = خالی_سوچک;
    returned = سچ;
    أرجع std::forward<T> (v);
  }

  خاص:
  بنانشانی صحيح *plevel;
  ثابت حرف *what;
  ثابت فراغ *obj;
  منطقي returned;
};
سانچا <قسم_ناں ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
بنية hb_auto_trace_t<0, ret_t>
{
  واضح مضمن hb_auto_trace_t (بنانشانی صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *سنیہا,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  سانچا <قسم_ناں T>
  T ret (T&& v,
	 ثابت حرف *func HB_بن_ورتیا = خالی_سوچک,
	 بنانشانی صحيح سطر HB_بن_ورتیا = 0) { أرجع std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
سانچا <قسم_ناں ret_t>
بنية hb_no_trace_t {
  سانچا <قسم_ناں T>
  T ret (T&& v,
	 ثابت حرف *func HB_بن_ورتیا = خالی_سوچک,
	 بنانشانی صحيح سطر HB_بن_ورتیا = 0) { أرجع std::forward<T> (v); }
};

#تعریف_کرو return_trace(RET) أرجع trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#جے_بنا_تعریف HB_DEBUG_ARABIC
#تعریف_کرو HB_DEBUG_ARABIC (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_BLOB
#تعریف_کرو HB_DEBUG_BLOB (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_CORETEXT
#تعریف_کرو HB_DEBUG_CORETEXT (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_DIRECTWRITE
#تعریف_کرو HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_FT
#تعریف_کرو HB_DEBUG_FT (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_JUSTIFY
#تعریف_کرو HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_OBJECT
#تعریف_کرو HB_DEBUG_OBJECT (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_SHAPE_PLAN
#تعریف_کرو HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_UNISCRIBE
#تعریف_کرو HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_WASM
#تعریف_کرو HB_DEBUG_WASM (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_KBTS
#تعریف_کرو HB_DEBUG_KBTS (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_DEPEND
#تعریف_کرو HB_DEBUG_DEPEND (HB_DEBUG+0)
#جے_انت

/*
 * With tracing.
 */

#جے_بنا_تعریف HB_DEBUG_APPLY
#تعریف_کرو HB_DEBUG_APPLY (HB_DEBUG+0)
#جے_انت
#إذا HB_DEBUG_APPLY
#تعریف_کرو TRACE_APPLY(هذا) \
	hb_auto_trace_t<HB_DEBUG_APPLY, منطقي> trace \
	(&c->debug_ڈونگھائی, c->get_name (), هذا, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->بفر->idx, c->بفر->cur().codepoint, (صحيح) c->lookup_اشاریہ)
#وإلا
#تعریف_کرو TRACE_APPLY(هذا) hb_no_trace_t<منطقي> trace
#جے_انت

#جے_بنا_تعریف HB_DEBUG_SANITIZE
#تعریف_کرو HB_DEBUG_SANITIZE (HB_DEBUG+0)
#جے_انت
#إذا HB_DEBUG_SANITIZE
#تعریف_کرو TRACE_SANITIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, منطقي> trace \
	(&c->debug_ڈونگھائی, c->get_name (), هذا, HB_FUNC, \
	 " ")
#وإلا
#تعریف_کرو TRACE_SANITIZE(هذا) hb_no_trace_t<منطقي> trace
#جے_انت

#جے_بنا_تعریف HB_DEBUG_SERIALIZE
#تعریف_کرو HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#جے_انت
#إذا HB_DEBUG_SERIALIZE
#تعریف_کرو TRACE_SERIALIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, منطقي> trace \
	(&c->debug_ڈونگھائی, "SERIALIZE", c, HB_FUNC, \
	 " ")
#وإلا
#تعریف_کرو TRACE_SERIALIZE(هذا) hb_no_trace_t<منطقي> trace
#جے_انت

#جے_بنا_تعریف HB_DEBUG_SUBSET
#تعریف_کرو HB_DEBUG_SUBSET (HB_DEBUG+0)
#جے_انت
#إذا HB_DEBUG_SUBSET
#تعریف_کرو TRACE_SUBSET(هذا) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, منطقي> trace \
  (&c->debug_ڈونگھائی, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#تعریف_کرو TRACE_SUBSET(هذا) hb_no_trace_t<منطقي> trace
#جے_انت

#جے_بنا_تعریف HB_DEBUG_SUBSET_REPACK
#تعریف_کرو HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#جے_انت

#جے_بنا_تعریف HB_DEBUG_PAINT
#تعریف_کرو HB_DEBUG_PAINT (HB_DEBUG+0)
#جے_انت
#إذا HB_DEBUG_PAINT
#تعریف_کرو TRACE_PAINT(هذا) \
  HB_بن_ورتیا hb_auto_trace_t<HB_DEBUG_PAINT, فراغ> trace \
  (&c->debug_ڈونگھائی, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#تعریف_کرو TRACE_PAINT(هذا) HB_بن_ورتیا hb_no_trace_t<فراغ> trace
#جے_انت


#جے_بنا_تعریف HB_DEBUG_DISPATCH
#تعریف_کرو HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#جے_انت
#إذا HB_DEBUG_DISPATCH
#تعریف_کرو TRACE_DISPATCH(هذا, format) \
	hb_auto_trace_t<context_t::max_debug_ڈونگھائی, قسم_ناں context_t::return_t> trace \
	(&c->debug_ڈونگھائی, c->get_name (), هذا, HB_FUNC, \
	 "format %u", (بنانشانی) format)
#وإلا
#تعریف_کرو TRACE_DISPATCH(هذا, format) hb_no_trace_t<قسم_ناں context_t::return_t> trace
#جے_انت


#جے_بنا_تعریف HB_بفر_سنیہا_ہور
#تعریف_کرو HB_بفر_سنیہا_ہور (HB_DEBUG+0)
#جے_انت


#جے_انت /* HB_DEBUG_HH */

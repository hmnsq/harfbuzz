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

#اگر_تعریف_نشده HB_DEBUG_HH
#تعریف_کن HB_DEBUG_HH

#ضمن "hb.hh"
#ضمن "hb-atomic.hh"
#ضمن "hb-algs.hh"


#اگر_تعریف_نشده HB_DEBUG
#تعریف_کن HB_DEBUG 0
#پایان_اگر


/*
 * Debug output (needs enabling at compile time.)
 */

ساكن مضمن منطقي
_hb_debug (بیعلامت صحيح level,
	   بیعلامت صحيح max_level)
{
  أرجع level < max_level;
}

#تعریف_کن DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#تعریف_کن DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ساكن مضمن فراغ
_hb_print_func (ثابت حرف *func)
{
  إذا (func)
  {
    بیعلامت صحيح func_len = طول_رشته (func);
    /* Skip "static" */
    إذا (0 == مقایسه_رشته (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    إذا (0 == مقایسه_رشته (func, "typename ", 9))
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

قالب <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  بیعلامت صحيح level,
		  صحيح level_dir,
		  ثابت حرف *پیام,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
قالب <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیام_va (ثابت حرف *what,
		  ثابت فراغ *obj,
		  ثابت حرف *func,
		  منطقي indented,
		  بیعلامت صحيح level,
		  صحيح level_dir,
		  ثابت حرف *پیام,
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
#تعریف_کن VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#تعریف_کن VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#تعریف_کن DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#تعریف_کن ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#تعریف_کن LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ساكن ثابت حرف bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + حجم (bars) - 1 - hb_min ((بیعلامت صحيح) حجم (bars) - 1, (بیعلامت صحيح) (حجم (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } وإلا
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  إذا (پیام)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, پیام, ap);
  }

  fprintf (stderr, "\n");
}
قالب <> مضمن فراغ HB_PRINTF_FUNC(7, 0)
hb_debug_پیام_va<0> (ثابت حرف *what HB_استفادهنشده,
		     ثابت فراغ *obj HB_استفادهنشده,
		     ثابت حرف *func HB_استفادهنشده,
		     منطقي indented HB_استفادهنشده,
		     بیعلامت صحيح level HB_استفادهنشده,
		     صحيح level_dir HB_استفادهنشده,
		     ثابت حرف *پیام HB_استفادهنشده,
		     va_list ap HB_استفادهنشده) {}

قالب <صحيح max_level> ساكن مضمن فراغ
hb_debug_پیام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       بیعلامت صحيح level,
	       صحيح level_dir,
	       ثابت حرف *پیام,
	       ...) HB_PRINTF_FUNC(7, 8);
قالب <صحيح max_level> ساكن مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیام (ثابت حرف *what,
	       ثابت فراغ *obj,
	       ثابت حرف *func,
	       منطقي indented,
	       بیعلامت صحيح level,
	       صحيح level_dir,
	       ثابت حرف *پیام,
	       ...)
{
  va_list ap;
  va_شروع (ap, پیام);
  hb_debug_پیام_va<max_level> (what, obj, func, indented, level, level_dir, پیام, ap);
  va_انتها (ap);
}
قالب <> مضمن فراغ
hb_debug_پیام<0> (ثابت حرف *what HB_استفادهنشده,
		  ثابت فراغ *obj HB_استفادهنشده,
		  ثابت حرف *func HB_استفادهنشده,
		  منطقي indented HB_استفادهنشده,
		  بیعلامت صحيح level HB_استفادهنشده,
		  صحيح level_dir HB_استفادهنشده,
		  ثابت حرف *پیام HB_استفادهنشده,
		  ...) HB_PRINTF_FUNC(7, 8);
قالب <> مضمن فراغ HB_PRINTF_FUNC(7, 8)
hb_debug_پیام<0> (ثابت حرف *what HB_استفادهنشده,
		  ثابت فراغ *obj HB_استفادهنشده,
		  ثابت حرف *func HB_استفادهنشده,
		  منطقي indented HB_استفادهنشده,
		  بیعلامت صحيح level HB_استفادهنشده,
		  صحيح level_dir HB_استفادهنشده,
		  ثابت حرف *پیام HB_استفادهنشده,
		  ...) {}

#تعریف_کن DEBUG_پیام_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_پیام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), اشاره_تهی,    درست, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#تعریف_کن DEBUG_پیام(WHAT, OBJ, ...)				hb_debug_پیام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), اشاره_تهی,    خطأ, 0, 0, __VA_ARGS__)
#تعریف_کن DEBUG_پیام_FUNC(WHAT, OBJ, ...)				hb_debug_پیام<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, خطأ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

قالب <نام_نوع T>
بنية hb_printer_t {
  ثابت حرف *print (ثابت T&) { أرجع "something"; }
};

قالب <>
بنية hb_printer_t<منطقي> {
  ثابت حرف *print (منطقي v) { أرجع v ? "true" : "false"; }
};

قالب <>
بنية hb_printer_t<hb_خالی_t> {
  ثابت حرف *print (hb_خالی_t) { أرجع ""; }
};


/*
 * Trace
 */

قالب <نام_نوع T>
ساكن مضمن فراغ _hb_warn_no_return (منطقي returned)
{
  إذا (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
قالب <>
/*static*/ مضمن فراغ _hb_warn_no_return<hb_خالی_t> (منطقي returned HB_استفادهنشده) {}
قالب <>
/*static*/ مضمن فراغ _hb_warn_no_return<فراغ> (منطقي returned HB_استفادهنشده) {}

قالب <صحيح max_level, نام_نوع ret_t>
بنية hb_auto_trace_t
{
  صریح مضمن hb_auto_trace_t (بیعلامت صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *پیام,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (خطأ)
  {
    إذا (plevel) ++*plevel;

    va_list ap;
    va_شروع (ap, پیام);
    hb_debug_پیام_va<max_level> (what, obj, func, درست, plevel ? *plevel : 0, +1, پیام, ap);
    va_انتها (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    إذا (!returned) {
      hb_debug_پیام<max_level> (what, obj, اشاره_تهی, درست, plevel ? *plevel : 1, -1, " ");
    }
    إذا (plevel) --*plevel;
  }

  قالب <نام_نوع T>
  T ret (T&& v,
	 ثابت حرف *func = "",
	 بیعلامت صحيح سطر = 0)
  {
    إذا (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      أرجع std::forward<T> (v);
    }

    hb_debug_پیام<max_level> (what, obj, func, درست, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), سطر);
    إذا (plevel) --*plevel;
    plevel = اشاره_تهی;
    returned = درست;
    أرجع std::forward<T> (v);
  }

  خاص:
  بیعلامت صحيح *plevel;
  ثابت حرف *what;
  ثابت فراغ *obj;
  منطقي returned;
};
قالب <نام_نوع ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
بنية hb_auto_trace_t<0, ret_t>
{
  صریح مضمن hb_auto_trace_t (بیعلامت صحيح *plevel_,
				   ثابت حرف *what_,
				   ثابت فراغ *obj_,
				   ثابت حرف *func,
				   ثابت حرف *پیام,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  قالب <نام_نوع T>
  T ret (T&& v,
	 ثابت حرف *func HB_استفادهنشده = اشاره_تهی,
	 بیعلامت صحيح سطر HB_استفادهنشده = 0) { أرجع std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
قالب <نام_نوع ret_t>
بنية hb_no_trace_t {
  قالب <نام_نوع T>
  T ret (T&& v,
	 ثابت حرف *func HB_استفادهنشده = اشاره_تهی,
	 بیعلامت صحيح سطر HB_استفادهنشده = 0) { أرجع std::forward<T> (v); }
};

#تعریف_کن return_trace(RET) أرجع trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#اگر_تعریف_نشده HB_DEBUG_ARABIC
#تعریف_کن HB_DEBUG_ARABIC (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_BLOB
#تعریف_کن HB_DEBUG_BLOB (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_CORETEXT
#تعریف_کن HB_DEBUG_CORETEXT (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_DIRECTWRITE
#تعریف_کن HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_FT
#تعریف_کن HB_DEBUG_FT (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_JUSTIFY
#تعریف_کن HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_OBJECT
#تعریف_کن HB_DEBUG_OBJECT (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_SHAPE_PLAN
#تعریف_کن HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_UNISCRIBE
#تعریف_کن HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_WASM
#تعریف_کن HB_DEBUG_WASM (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_KBTS
#تعریف_کن HB_DEBUG_KBTS (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_DEPEND
#تعریف_کن HB_DEBUG_DEPEND (HB_DEBUG+0)
#پایان_اگر

/*
 * With tracing.
 */

#اگر_تعریف_نشده HB_DEBUG_APPLY
#تعریف_کن HB_DEBUG_APPLY (HB_DEBUG+0)
#پایان_اگر
#إذا HB_DEBUG_APPLY
#تعریف_کن TRACE_APPLY(هذا) \
	hb_auto_trace_t<HB_DEBUG_APPLY, منطقي> trace \
	(&c->debug_عمق, c->get_name (), هذا, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->میانگیر->idx, c->میانگیر->cur().codepoint, (صحيح) c->lookup_شاخص)
#وإلا
#تعریف_کن TRACE_APPLY(هذا) hb_no_trace_t<منطقي> trace
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_SANITIZE
#تعریف_کن HB_DEBUG_SANITIZE (HB_DEBUG+0)
#پایان_اگر
#إذا HB_DEBUG_SANITIZE
#تعریف_کن TRACE_SANITIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, منطقي> trace \
	(&c->debug_عمق, c->get_name (), هذا, HB_FUNC, \
	 " ")
#وإلا
#تعریف_کن TRACE_SANITIZE(هذا) hb_no_trace_t<منطقي> trace
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_SERIALIZE
#تعریف_کن HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#پایان_اگر
#إذا HB_DEBUG_SERIALIZE
#تعریف_کن TRACE_SERIALIZE(هذا) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, منطقي> trace \
	(&c->debug_عمق, "SERIALIZE", c, HB_FUNC, \
	 " ")
#وإلا
#تعریف_کن TRACE_SERIALIZE(هذا) hb_no_trace_t<منطقي> trace
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_SUBSET
#تعریف_کن HB_DEBUG_SUBSET (HB_DEBUG+0)
#پایان_اگر
#إذا HB_DEBUG_SUBSET
#تعریف_کن TRACE_SUBSET(هذا) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, منطقي> trace \
  (&c->debug_عمق, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#تعریف_کن TRACE_SUBSET(هذا) hb_no_trace_t<منطقي> trace
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_SUBSET_REPACK
#تعریف_کن HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#پایان_اگر

#اگر_تعریف_نشده HB_DEBUG_PAINT
#تعریف_کن HB_DEBUG_PAINT (HB_DEBUG+0)
#پایان_اگر
#إذا HB_DEBUG_PAINT
#تعریف_کن TRACE_PAINT(هذا) \
  HB_استفادهنشده hb_auto_trace_t<HB_DEBUG_PAINT, فراغ> trace \
  (&c->debug_عمق, c->get_name (), هذا, HB_FUNC, \
   " ")
#وإلا
#تعریف_کن TRACE_PAINT(هذا) HB_استفادهنشده hb_no_trace_t<فراغ> trace
#پایان_اگر


#اگر_تعریف_نشده HB_DEBUG_DISPATCH
#تعریف_کن HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#پایان_اگر
#إذا HB_DEBUG_DISPATCH
#تعریف_کن TRACE_DISPATCH(هذا, format) \
	hb_auto_trace_t<context_t::max_debug_عمق, نام_نوع context_t::return_t> trace \
	(&c->debug_عمق, c->get_name (), هذا, HB_FUNC, \
	 "format %u", (بیعلامت) format)
#وإلا
#تعریف_کن TRACE_DISPATCH(هذا, format) hb_no_trace_t<نام_نوع context_t::return_t> trace
#پایان_اگر


#اگر_تعریف_نشده HB_میانگیر_پیام_بیشتر
#تعریف_کن HB_میانگیر_پیام_بیشتر (HB_DEBUG+0)
#پایان_اگر


#پایان_اگر /* HB_DEBUG_HH */

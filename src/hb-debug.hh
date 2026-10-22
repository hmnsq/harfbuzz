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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "hb.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-atomic.hh"
#ਅਨ੍ਤਰ੍ਭਾਵ "hb-algs.hh"


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG 0
#ਜੇ_ਅੰਤ


/*
 * Debug output (needs enabling at compile time.)
 */

ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ
_hb_debug (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level,
	   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ max_level)
{
  ਵਾਪਸ level < max_level;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
_hb_print_func (ਅਚਲ ਅੱਖਰ *func)
{
  ਜੇ (func)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ func_len = ਲੜੀ_ਲੰਬਾਈ (func);
    /* Skip "static" */
    ਜੇ (0 == ਲੜੀ_ਤੁਲਨਾ (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    ਜੇ (0 == ਲੜੀ_ਤੁਲਨਾ (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ਅਚਲ ਅੱਖਰ *space = strchr (func, ' ');
    ਜੇ (space)
      func = space + 1;
    /* Skip parameter list */
    ਅਚਲ ਅੱਖਰ *paren = strchr (func, '(');
    ਜੇ (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (ਪੂਰਨ) func_len, func);
  }
}

ਸਾਂਚਾ <ਪੂਰਨ max_level> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_debug_ਸੁਨੇਹਾ_va (ਅਚਲ ਅੱਖਰ *what,
		  ਅਚਲ ਖਾਲੀ *obj,
		  ਅਚਲ ਅੱਖਰ *func,
		  ਤਾਰਕਿਕ indented,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level,
		  ਪੂਰਨ level_dir,
		  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
ਸਾਂਚਾ <ਪੂਰਨ max_level> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_debug_ਸੁਨੇਹਾ_va (ਅਚਲ ਅੱਖਰ *what,
		  ਅਚਲ ਖਾਲੀ *obj,
		  ਅਚਲ ਅੱਖਰ *func,
		  ਤਾਰਕਿਕ indented,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level,
		  ਪੂਰਨ level_dir,
		  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
		  va_list ap)
{
  ਜੇ (!_hb_debug (level, max_level))
    ਵਾਪਸ;

  fprintf (stderr, "%-10s", what ? what : "");

  ਜੇ (obj)
    fprintf (stderr, "(%*p) ", (ਪੂਰਨ) (2 * ਆਕਾਰ (ਖਾਲੀ *)), obj);
  ਨਹੀਂਤਾਂ
    fprintf (stderr, " %*s  ", (ਪੂਰਨ) (2 * ਆਕਾਰ (ਖਾਲੀ *)), "");

  ਜੇ (indented) {
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + ਆਕਾਰ (bars) - 1 - hb_min ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) ਆਕਾਰ (bars) - 1, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ) (ਆਕਾਰ (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } ਨਹੀਂਤਾਂ
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  ਜੇ (ਸੁਨੇਹਾ)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, ਸੁਨੇਹਾ, ap);
  }

  fprintf (stderr, "\n");
}
ਸਾਂਚਾ <> ਸਤਰਅੰਦਰ ਖਾਲੀ HB_PRINTF_FUNC(7, 0)
hb_debug_ਸੁਨੇਹਾ_va<0> (ਅਚਲ ਅੱਖਰ *what HB_ਅਣਵਰਤਿਆ,
		     ਅਚਲ ਖਾਲੀ *obj HB_ਅਣਵਰਤਿਆ,
		     ਅਚਲ ਅੱਖਰ *func HB_ਅਣਵਰਤਿਆ,
		     ਤਾਰਕਿਕ indented HB_ਅਣਵਰਤਿਆ,
		     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level HB_ਅਣਵਰਤਿਆ,
		     ਪੂਰਨ level_dir HB_ਅਣਵਰਤਿਆ,
		     ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ HB_ਅਣਵਰਤਿਆ,
		     va_list ap HB_ਅਣਵਰਤਿਆ) {}

ਸਾਂਚਾ <ਪੂਰਨ max_level> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_debug_ਸੁਨੇਹਾ (ਅਚਲ ਅੱਖਰ *what,
	       ਅਚਲ ਖਾਲੀ *obj,
	       ਅਚਲ ਅੱਖਰ *func,
	       ਤਾਰਕਿਕ indented,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level,
	       ਪੂਰਨ level_dir,
	       ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
	       ...) HB_PRINTF_FUNC(7, 8);
ਸਾਂਚਾ <ਪੂਰਨ max_level> ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ HB_PRINTF_FUNC(7, 8)
hb_debug_ਸੁਨੇਹਾ (ਅਚਲ ਅੱਖਰ *what,
	       ਅਚਲ ਖਾਲੀ *obj,
	       ਅਚਲ ਅੱਖਰ *func,
	       ਤਾਰਕਿਕ indented,
	       ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level,
	       ਪੂਰਨ level_dir,
	       ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
	       ...)
{
  va_list ap;
  va_ਸ਼ੁਰੂ (ap, ਸੁਨੇਹਾ);
  hb_debug_ਸੁਨੇਹਾ_va<max_level> (what, obj, func, indented, level, level_dir, ਸੁਨੇਹਾ, ap);
  va_ਸਿਰਾ (ap);
}
ਸਾਂਚਾ <> ਸਤਰਅੰਦਰ ਖਾਲੀ
hb_debug_ਸੁਨੇਹਾ<0> (ਅਚਲ ਅੱਖਰ *what HB_ਅਣਵਰਤਿਆ,
		  ਅਚਲ ਖਾਲੀ *obj HB_ਅਣਵਰਤਿਆ,
		  ਅਚਲ ਅੱਖਰ *func HB_ਅਣਵਰਤਿਆ,
		  ਤਾਰਕਿਕ indented HB_ਅਣਵਰਤਿਆ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level HB_ਅਣਵਰਤਿਆ,
		  ਪੂਰਨ level_dir HB_ਅਣਵਰਤਿਆ,
		  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ HB_ਅਣਵਰਤਿਆ,
		  ...) HB_PRINTF_FUNC(7, 8);
ਸਾਂਚਾ <> ਸਤਰਅੰਦਰ ਖਾਲੀ HB_PRINTF_FUNC(7, 8)
hb_debug_ਸੁਨੇਹਾ<0> (ਅਚਲ ਅੱਖਰ *what HB_ਅਣਵਰਤਿਆ,
		  ਅਚਲ ਖਾਲੀ *obj HB_ਅਣਵਰਤਿਆ,
		  ਅਚਲ ਅੱਖਰ *func HB_ਅਣਵਰਤਿਆ,
		  ਤਾਰਕਿਕ indented HB_ਅਣਵਰਤਿਆ,
		  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ level HB_ਅਣਵਰਤਿਆ,
		  ਪੂਰਨ level_dir HB_ਅਣਵਰਤਿਆ,
		  ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ HB_ਅਣਵਰਤਿਆ,
		  ...) {}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEBUG_ਸੁਨੇਹਾ_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_ਸੁਨੇਹਾ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), ਖਾਲੀ_ਸੂਚਕ,    ਸੱਚ, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEBUG_ਸੁਨੇਹਾ(WHAT, OBJ, ...)				hb_debug_ਸੁਨੇਹਾ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), ਖਾਲੀ_ਸੂਚਕ,    ਝੂਠ, 0, 0, __VA_ARGS__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DEBUG_ਸੁਨੇਹਾ_FUNC(WHAT, OBJ, ...)				hb_debug_ਸੁਨੇਹਾ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, ਝੂਠ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਬਣਤਰ hb_printer_t {
  ਅਚਲ ਅੱਖਰ *print (ਅਚਲ T&) { ਵਾਪਸ "something"; }
};

ਸਾਂਚਾ <>
ਬਣਤਰ hb_printer_t<ਤਾਰਕਿਕ> {
  ਅਚਲ ਅੱਖਰ *print (ਤਾਰਕਿਕ v) { ਵਾਪਸ v ? "true" : "false"; }
};

ਸਾਂਚਾ <>
ਬਣਤਰ hb_printer_t<hb_ਖਾਲੀ_ਥਾਵਾਂ_t> {
  ਅਚਲ ਅੱਖਰ *print (hb_ਖਾਲੀ_ਥਾਵਾਂ_t) { ਵਾਪਸ ""; }
};


/*
 * Trace
 */

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ _hb_warn_no_return (ਤਾਰਕਿਕ returned)
{
  ਜੇ (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
ਸਾਂਚਾ <>
/*static*/ ਸਤਰਅੰਦਰ ਖਾਲੀ _hb_warn_no_return<hb_ਖਾਲੀ_ਥਾਵਾਂ_t> (ਤਾਰਕਿਕ returned HB_ਅਣਵਰਤਿਆ) {}
ਸਾਂਚਾ <>
/*static*/ ਸਤਰਅੰਦਰ ਖਾਲੀ _hb_warn_no_return<ਖਾਲੀ> (ਤਾਰਕਿਕ returned HB_ਅਣਵਰਤਿਆ) {}

ਸਾਂਚਾ <ਪੂਰਨ max_level, ਕਿਸਮ_ਨਾਂ ret_t>
ਬਣਤਰ hb_auto_trace_t
{
  ਸਪਸ਼ਟ ਸਤਰਅੰਦਰ hb_auto_trace_t (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *plevel_,
				   ਅਚਲ ਅੱਖਰ *what_,
				   ਅਚਲ ਖਾਲੀ *obj_,
				   ਅਚਲ ਅੱਖਰ *func,
				   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (ਝੂਠ)
  {
    ਜੇ (plevel) ++*plevel;

    va_list ap;
    va_ਸ਼ੁਰੂ (ap, ਸੁਨੇਹਾ);
    hb_debug_ਸੁਨੇਹਾ_va<max_level> (what, obj, func, ਸੱਚ, plevel ? *plevel : 0, +1, ਸੁਨੇਹਾ, ap);
    va_ਸਿਰਾ (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    ਜੇ (!returned) {
      hb_debug_ਸੁਨੇਹਾ<max_level> (what, obj, ਖਾਲੀ_ਸੂਚਕ, ਸੱਚ, plevel ? *plevel : 1, -1, " ");
    }
    ਜੇ (plevel) --*plevel;
  }

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  T ret (T&& v,
	 ਅਚਲ ਅੱਖਰ *func = "",
	 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰ = 0)
  {
    ਜੇ (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      ਵਾਪਸ std::forward<T> (v);
    }

    hb_debug_ਸੁਨੇਹਾ<max_level> (what, obj, func, ਸੱਚ, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), ਸਤਰ);
    ਜੇ (plevel) --*plevel;
    plevel = ਖਾਲੀ_ਸੂਚਕ;
    returned = ਸੱਚ;
    ਵਾਪਸ std::forward<T> (v);
  }

  ਨਿਜੀ:
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *plevel;
  ਅਚਲ ਅੱਖਰ *what;
  ਅਚਲ ਖਾਲੀ *obj;
  ਤਾਰਕਿਕ returned;
};
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
ਬਣਤਰ hb_auto_trace_t<0, ret_t>
{
  ਸਪਸ਼ਟ ਸਤਰਅੰਦਰ hb_auto_trace_t (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *plevel_,
				   ਅਚਲ ਅੱਖਰ *what_,
				   ਅਚਲ ਖਾਲੀ *obj_,
				   ਅਚਲ ਅੱਖਰ *func,
				   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  T ret (T&& v,
	 ਅਚਲ ਅੱਖਰ *func HB_ਅਣਵਰਤਿਆ = ਖਾਲੀ_ਸੂਚਕ,
	 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰ HB_ਅਣਵਰਤਿਆ = 0) { ਵਾਪਸ std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ ret_t>
ਬਣਤਰ hb_no_trace_t {
  ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
  T ret (T&& v,
	 ਅਚਲ ਅੱਖਰ *func HB_ਅਣਵਰਤਿਆ = ਖਾਲੀ_ਸੂਚਕ,
	 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸਤਰ HB_ਅਣਵਰਤਿਆ = 0) { ਵਾਪਸ std::forward<T> (v); }
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ return_trace(RET) ਵਾਪਸ trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_ARABIC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_ARABIC (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_BLOB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_BLOB (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_CORETEXT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_CORETEXT (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_DIRECTWRITE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_FT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_FT (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_JUSTIFY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_OBJECT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_OBJECT (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_SHAPE_PLAN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_UNISCRIBE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_WASM
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_WASM (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_KBTS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_KBTS (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_DEPEND
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_DEPEND (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

/*
 * With tracing.
 */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_APPLY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_APPLY (HB_DEBUG+0)
#ਜੇ_ਅੰਤ
#ਜੇ HB_DEBUG_APPLY
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_APPLY(ਇਹ) \
	hb_auto_trace_t<HB_DEBUG_APPLY, ਤਾਰਕਿਕ> trace \
	(&c->debug_ਡੂੰਘਾਈ, c->get_name (), ਇਹ, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->ਬਫਰ->idx, c->ਬਫਰ->cur().codepoint, (ਪੂਰਨ) c->lookup_ਸੂਚਕਾਂਕ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_APPLY(ਇਹ) hb_no_trace_t<ਤਾਰਕਿਕ> trace
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_SANITIZE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_SANITIZE (HB_DEBUG+0)
#ਜੇ_ਅੰਤ
#ਜੇ HB_DEBUG_SANITIZE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_SANITIZE(ਇਹ) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, ਤਾਰਕਿਕ> trace \
	(&c->debug_ਡੂੰਘਾਈ, c->get_name (), ਇਹ, HB_FUNC, \
	 " ")
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_SANITIZE(ਇਹ) hb_no_trace_t<ਤਾਰਕਿਕ> trace
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_SERIALIZE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#ਜੇ_ਅੰਤ
#ਜੇ HB_DEBUG_SERIALIZE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_SERIALIZE(ਇਹ) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, ਤਾਰਕਿਕ> trace \
	(&c->debug_ਡੂੰਘਾਈ, "SERIALIZE", c, HB_FUNC, \
	 " ")
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_SERIALIZE(ਇਹ) hb_no_trace_t<ਤਾਰਕਿਕ> trace
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_SUBSET
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_SUBSET (HB_DEBUG+0)
#ਜੇ_ਅੰਤ
#ਜੇ HB_DEBUG_SUBSET
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_SUBSET(ਇਹ) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, ਤਾਰਕਿਕ> trace \
  (&c->debug_ਡੂੰਘਾਈ, c->get_name (), ਇਹ, HB_FUNC, \
   " ")
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_SUBSET(ਇਹ) hb_no_trace_t<ਤਾਰਕਿਕ> trace
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_SUBSET_REPACK
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_PAINT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_PAINT (HB_DEBUG+0)
#ਜੇ_ਅੰਤ
#ਜੇ HB_DEBUG_PAINT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_PAINT(ਇਹ) \
  HB_ਅਣਵਰਤਿਆ hb_auto_trace_t<HB_DEBUG_PAINT, ਖਾਲੀ> trace \
  (&c->debug_ਡੂੰਘਾਈ, c->get_name (), ਇਹ, HB_FUNC, \
   " ")
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_PAINT(ਇਹ) HB_ਅਣਵਰਤਿਆ hb_no_trace_t<ਖਾਲੀ> trace
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_DEBUG_DISPATCH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#ਜੇ_ਅੰਤ
#ਜੇ HB_DEBUG_DISPATCH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_DISPATCH(ਇਹ, format) \
	hb_auto_trace_t<context_t::max_debug_ਡੂੰਘਾਈ, ਕਿਸਮ_ਨਾਂ context_t::return_t> trace \
	(&c->debug_ਡੂੰਘਾਈ, c->get_name (), ਇਹ, HB_FUNC, \
	 "format %u", (ਬਿਨਾਨਿਸ਼ਾਨੀ) format)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TRACE_DISPATCH(ਇਹ, format) hb_no_trace_t<ਕਿਸਮ_ਨਾਂ context_t::return_t> trace
#ਜੇ_ਅੰਤ


#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ HB_ਬਫਰ_ਸੁਨੇਹਾ_ਹੋਰ (HB_DEBUG+0)
#ਜੇ_ਅੰਤ


#ਜੇ_ਅੰਤ /* HB_DEBUG_HH */

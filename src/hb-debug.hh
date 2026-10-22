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

#ifndef HB_DEBUG_HH
#define HB_DEBUG_HH

#അന്തര്ഭാവ "hb.hh"
#അന്തര്ഭാവ "hb-atomic.hh"
#അന്തര്ഭാവ "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

സ്ഥിതം വരിയിൽ തർക്കം
_hb_debug (unsigned പൂർണ്ണസംഖ്യ level,
	   unsigned പൂർണ്ണസംഖ്യ max_level)
{
  തിരികെ level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

സ്ഥിതം വരിയിൽ ശൂന്യം
_hb_print_func (സ്ഥിരം അക്ഷരം *func)
{
  എങ്കിൽ (func)
  {
    unsigned പൂർണ്ണസംഖ്യ func_len = strlen (func);
    /* Skip "static" */
    എങ്കിൽ (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    എങ്കിൽ (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    സ്ഥിരം അക്ഷരം *space = strchr (func, ' ');
    എങ്കിൽ (space)
      func = space + 1;
    /* Skip parameter list */
    സ്ഥിരം അക്ഷരം *paren = strchr (func, '(');
    എങ്കിൽ (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (പൂർണ്ണസംഖ്യ) func_len, func);
  }
}

template <പൂർണ്ണസംഖ്യ max_level> സ്ഥിതം വരിയിൽ ശൂന്യം
hb_debug_സന്ദേശം_va (സ്ഥിരം അക്ഷരം *what,
		  സ്ഥിരം ശൂന്യം *obj,
		  സ്ഥിരം അക്ഷരം *func,
		  തർക്കം indented,
		  unsigned പൂർണ്ണസംഖ്യ level,
		  പൂർണ്ണസംഖ്യ level_dir,
		  സ്ഥിരം അക്ഷരം *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <പൂർണ്ണസംഖ്യ max_level> സ്ഥിതം വരിയിൽ ശൂന്യം
hb_debug_സന്ദേശം_va (സ്ഥിരം അക്ഷരം *what,
		  സ്ഥിരം ശൂന്യം *obj,
		  സ്ഥിരം അക്ഷരം *func,
		  തർക്കം indented,
		  unsigned പൂർണ്ണസംഖ്യ level,
		  പൂർണ്ണസംഖ്യ level_dir,
		  സ്ഥിരം അക്ഷരം *message,
		  va_list ap)
{
  എങ്കിൽ (!_hb_debug (level, max_level))
    തിരികെ;

  fprintf (stderr, "%-10s", what ? what : "");

  എങ്കിൽ (obj)
    fprintf (stderr, "(%*p) ", (പൂർണ്ണസംഖ്യ) (2 * വലുപ്പം (ശൂന്യം *)), obj);
  അല്ലെങ്കിൽ
    fprintf (stderr, " %*s  ", (പൂർണ്ണസംഖ്യ) (2 * വലുപ്പം (ശൂന്യം *)), "");

  എങ്കിൽ (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    സ്ഥിതം സ്ഥിരം അക്ഷരം bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + വലുപ്പം (bars) - 1 - hb_min ((unsigned പൂർണ്ണസംഖ്യ) വലുപ്പം (bars) - 1, (unsigned പൂർണ്ണസംഖ്യ) (വലുപ്പം (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } അല്ലെങ്കിൽ
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  എങ്കിൽ (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> വരിയിൽ ശൂന്യം HB_PRINTF_FUNC(7, 0)
hb_debug_സന്ദേശം_va<0> (സ്ഥിരം അക്ഷരം *what HB_UNUSED,
		     സ്ഥിരം ശൂന്യം *obj HB_UNUSED,
		     സ്ഥിരം അക്ഷരം *func HB_UNUSED,
		     തർക്കം indented HB_UNUSED,
		     unsigned പൂർണ്ണസംഖ്യ level HB_UNUSED,
		     പൂർണ്ണസംഖ്യ level_dir HB_UNUSED,
		     സ്ഥിരം അക്ഷരം *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <പൂർണ്ണസംഖ്യ max_level> സ്ഥിതം വരിയിൽ ശൂന്യം
hb_debug_സന്ദേശം (സ്ഥിരം അക്ഷരം *what,
	       സ്ഥിരം ശൂന്യം *obj,
	       സ്ഥിരം അക്ഷരം *func,
	       തർക്കം indented,
	       unsigned പൂർണ്ണസംഖ്യ level,
	       പൂർണ്ണസംഖ്യ level_dir,
	       സ്ഥിരം അക്ഷരം *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <പൂർണ്ണസംഖ്യ max_level> സ്ഥിതം വരിയിൽ ശൂന്യം HB_PRINTF_FUNC(7, 8)
hb_debug_സന്ദേശം (സ്ഥിരം അക്ഷരം *what,
	       സ്ഥിരം ശൂന്യം *obj,
	       സ്ഥിരം അക്ഷരം *func,
	       തർക്കം indented,
	       unsigned പൂർണ്ണസംഖ്യ level,
	       പൂർണ്ണസംഖ്യ level_dir,
	       സ്ഥിരം അക്ഷരം *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_സന്ദേശം_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> വരിയിൽ ശൂന്യം
hb_debug_സന്ദേശം<0> (സ്ഥിരം അക്ഷരം *what HB_UNUSED,
		  സ്ഥിരം ശൂന്യം *obj HB_UNUSED,
		  സ്ഥിരം അക്ഷരം *func HB_UNUSED,
		  തർക്കം indented HB_UNUSED,
		  unsigned പൂർണ്ണസംഖ്യ level HB_UNUSED,
		  പൂർണ്ണസംഖ്യ level_dir HB_UNUSED,
		  സ്ഥിരം അക്ഷരം *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> വരിയിൽ ശൂന്യം HB_PRINTF_FUNC(7, 8)
hb_debug_സന്ദേശം<0> (സ്ഥിരം അക്ഷരം *what HB_UNUSED,
		  സ്ഥിരം ശൂന്യം *obj HB_UNUSED,
		  സ്ഥിരം അക്ഷരം *func HB_UNUSED,
		  തർക്കം indented HB_UNUSED,
		  unsigned പൂർണ്ണസംഖ്യ level HB_UNUSED,
		  പൂർണ്ണസംഖ്യ level_dir HB_UNUSED,
		  സ്ഥിരം അക്ഷരം *message HB_UNUSED,
		  ...) {}

#define DEBUG_സന്ദേശം_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_സന്ദേശം<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    സത്യം, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_സന്ദേശം(WHAT, OBJ, ...)				hb_debug_സന്ദേശം<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    അസത്യം, 0, 0, __VA_ARGS__)
#define DEBUG_സന്ദേശം_FUNC(WHAT, OBJ, ...)				hb_debug_സന്ദേശം<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, അസത്യം, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
ഘടന hb_printer_t {
  സ്ഥിരം അക്ഷരം *print (സ്ഥിരം T&) { തിരികെ "something"; }
};

template <>
ഘടന hb_printer_t<തർക്കം> {
  സ്ഥിരം അക്ഷരം *print (തർക്കം v) { തിരികെ v ? "true" : "false"; }
};

template <>
ഘടന hb_printer_t<hb_empty_t> {
  സ്ഥിരം അക്ഷരം *print (hb_empty_t) { തിരികെ ""; }
};


/*
 * Trace
 */

template <typename T>
സ്ഥിതം വരിയിൽ ശൂന്യം _hb_warn_no_return (തർക്കം returned)
{
  എങ്കിൽ (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ വരിയിൽ ശൂന്യം _hb_warn_no_return<hb_empty_t> (തർക്കം returned HB_UNUSED) {}
template <>
/*static*/ വരിയിൽ ശൂന്യം _hb_warn_no_return<ശൂന്യം> (തർക്കം returned HB_UNUSED) {}

template <പൂർണ്ണസംഖ്യ max_level, typename ret_t>
ഘടന hb_auto_trace_t
{
  explicit വരിയിൽ hb_auto_trace_t (unsigned പൂർണ്ണസംഖ്യ *plevel_,
				   സ്ഥിരം അക്ഷരം *what_,
				   സ്ഥിരം ശൂന്യം *obj_,
				   സ്ഥിരം അക്ഷരം *func,
				   സ്ഥിരം അക്ഷരം *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (അസത്യം)
  {
    എങ്കിൽ (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_സന്ദേശം_va<max_level> (what, obj, func, സത്യം, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    എങ്കിൽ (!returned) {
      hb_debug_സന്ദേശം<max_level> (what, obj, nullptr, സത്യം, plevel ? *plevel : 1, -1, " ");
    }
    എങ്കിൽ (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 സ്ഥിരം അക്ഷരം *func = "",
	 unsigned പൂർണ്ണസംഖ്യ വരി = 0)
  {
    എങ്കിൽ (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      തിരികെ std::forward<T> (v);
    }

    hb_debug_സന്ദേശം<max_level> (what, obj, func, സത്യം, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), വരി);
    എങ്കിൽ (plevel) --*plevel;
    plevel = nullptr;
    returned = സത്യം;
    തിരികെ std::forward<T> (v);
  }

  സ്വകാര്യ:
  unsigned പൂർണ്ണസംഖ്യ *plevel;
  സ്ഥിരം അക്ഷരം *what;
  സ്ഥിരം ശൂന്യം *obj;
  തർക്കം returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
ഘടന hb_auto_trace_t<0, ret_t>
{
  explicit വരിയിൽ hb_auto_trace_t (unsigned പൂർണ്ണസംഖ്യ *plevel_,
				   സ്ഥിരം അക്ഷരം *what_,
				   സ്ഥിരം ശൂന്യം *obj_,
				   സ്ഥിരം അക്ഷരം *func,
				   സ്ഥിരം അക്ഷരം *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 സ്ഥിരം അക്ഷരം *func HB_UNUSED = nullptr,
	 unsigned പൂർണ്ണസംഖ്യ വരി HB_UNUSED = 0) { തിരികെ std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
ഘടന hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 സ്ഥിരം അക്ഷരം *func HB_UNUSED = nullptr,
	 unsigned പൂർണ്ണസംഖ്യ വരി HB_UNUSED = 0) { തിരികെ std::forward<T> (v); }
};

#define return_trace(RET) തിരികെ trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#ifndef HB_DEBUG_ARABIC
#define HB_DEBUG_ARABIC (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_BLOB
#define HB_DEBUG_BLOB (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_CORETEXT
#define HB_DEBUG_CORETEXT (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_DIRECTWRITE
#define HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_FT
#define HB_DEBUG_FT (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_JUSTIFY
#define HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_OBJECT
#define HB_DEBUG_OBJECT (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_SHAPE_PLAN
#define HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_UNISCRIBE
#define HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_WASM
#define HB_DEBUG_WASM (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_KBTS
#define HB_DEBUG_KBTS (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_DEPEND
#define HB_DEBUG_DEPEND (HB_DEBUG+0)
#endif

/*
 * With tracing.
 */

#ifndef HB_DEBUG_APPLY
#define HB_DEBUG_APPLY (HB_DEBUG+0)
#endif
#എങ്കിൽ HB_DEBUG_APPLY
#define TRACE_APPLY(ഇത്) \
	hb_auto_trace_t<HB_DEBUG_APPLY, തർക്കം> trace \
	(&c->debug_depth, c->get_name (), ഇത്, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (പൂർണ്ണസംഖ്യ) c->lookup_index)
#അല്ലെങ്കിൽ
#define TRACE_APPLY(ഇത്) hb_no_trace_t<തർക്കം> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#എങ്കിൽ HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(ഇത്) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, തർക്കം> trace \
	(&c->debug_depth, c->get_name (), ഇത്, HB_FUNC, \
	 " ")
#അല്ലെങ്കിൽ
#define TRACE_SANITIZE(ഇത്) hb_no_trace_t<തർക്കം> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#എങ്കിൽ HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(ഇത്) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, തർക്കം> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#അല്ലെങ്കിൽ
#define TRACE_SERIALIZE(ഇത്) hb_no_trace_t<തർക്കം> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#എങ്കിൽ HB_DEBUG_SUBSET
#define TRACE_SUBSET(ഇത്) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, തർക്കം> trace \
  (&c->debug_depth, c->get_name (), ഇത്, HB_FUNC, \
   " ")
#അല്ലെങ്കിൽ
#define TRACE_SUBSET(ഇത്) hb_no_trace_t<തർക്കം> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#എങ്കിൽ HB_DEBUG_PAINT
#define TRACE_PAINT(ഇത്) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, ശൂന്യം> trace \
  (&c->debug_depth, c->get_name (), ഇത്, HB_FUNC, \
   " ")
#അല്ലെങ്കിൽ
#define TRACE_PAINT(ഇത്) HB_UNUSED hb_no_trace_t<ശൂന്യം> trace
#endif


#ifndef HB_DEBUG_DISPATCH
#define HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#endif
#എങ്കിൽ HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(ഇത്, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), ഇത്, HB_FUNC, \
	 "format %u", (unsigned) format)
#അല്ലെങ്കിൽ
#define TRACE_DISPATCH(ഇത്, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */

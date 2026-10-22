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

#ಅನ್ತರ್ಭಾವ "hb.hh"
#ಅನ್ತರ್ಭಾವ "hb-atomic.hh"
#ಅನ್ತರ್ಭಾವ "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ
_hb_debug (unsigned ಪೂರ್ಣಾಙ್ಕ level,
	   unsigned ಪೂರ್ಣಾಙ್ಕ max_level)
{
  ಪ್ರತ್ಯಾಗಮ level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
_hb_print_func (ಧ್ರುವ ವರ್ಣ *func)
{
  ಯದಿ (func)
  {
    unsigned ಪೂರ್ಣಾಙ್ಕ func_len = strlen (func);
    /* Skip "static" */
    ಯದಿ (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    ಯದಿ (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    ಧ್ರುವ ವರ್ಣ *space = strchr (func, ' ');
    ಯದಿ (space)
      func = space + 1;
    /* Skip parameter list */
    ಧ್ರುವ ವರ್ಣ *paren = strchr (func, '(');
    ಯದಿ (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (ಪೂರ್ಣಾಙ್ಕ) func_len, func);
  }
}

template <ಪೂರ್ಣಾಙ್ಕ max_level> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_debug_ಸಂದೇಶ_va (ಧ್ರುವ ವರ್ಣ *what,
		  ಧ್ರುವ ಶೂನ್ಯ *obj,
		  ಧ್ರುವ ವರ್ಣ *func,
		  ತಾರ್ಕಿಕ indented,
		  unsigned ಪೂರ್ಣಾಙ್ಕ level,
		  ಪೂರ್ಣಾಙ್ಕ level_dir,
		  ಧ್ರುವ ವರ್ಣ *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <ಪೂರ್ಣಾಙ್ಕ max_level> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_debug_ಸಂದೇಶ_va (ಧ್ರುವ ವರ್ಣ *what,
		  ಧ್ರುವ ಶೂನ್ಯ *obj,
		  ಧ್ರುವ ವರ್ಣ *func,
		  ತಾರ್ಕಿಕ indented,
		  unsigned ಪೂರ್ಣಾಙ್ಕ level,
		  ಪೂರ್ಣಾಙ್ಕ level_dir,
		  ಧ್ರುವ ವರ್ಣ *message,
		  va_list ap)
{
  ಯದಿ (!_hb_debug (level, max_level))
    ಪ್ರತ್ಯಾಗಮ;

  fprintf (stderr, "%-10s", what ? what : "");

  ಯದಿ (obj)
    fprintf (stderr, "(%*p) ", (ಪೂರ್ಣಾಙ್ಕ) (2 * ಪರಿಮಾಣ (ಶೂನ್ಯ *)), obj);
  ಅನ್ಯಥಾ
    fprintf (stderr, " %*s  ", (ಪೂರ್ಣಾಙ್ಕ) (2 * ಪರಿಮಾಣ (ಶೂನ್ಯ *)), "");

  ಯದಿ (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + ಪರಿಮಾಣ (bars) - 1 - hb_min ((unsigned ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (bars) - 1, (unsigned ಪೂರ್ಣಾಙ್ಕ) (ಪರಿಮಾಣ (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } ಅನ್ಯಥಾ
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  ಯದಿ (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ HB_PRINTF_FUNC(7, 0)
hb_debug_ಸಂದೇಶ_va<0> (ಧ್ರುವ ವರ್ಣ *what HB_UNUSED,
		     ಧ್ರುವ ಶೂನ್ಯ *obj HB_UNUSED,
		     ಧ್ರುವ ವರ್ಣ *func HB_UNUSED,
		     ತಾರ್ಕಿಕ indented HB_UNUSED,
		     unsigned ಪೂರ್ಣಾಙ್ಕ level HB_UNUSED,
		     ಪೂರ್ಣಾಙ್ಕ level_dir HB_UNUSED,
		     ಧ್ರುವ ವರ್ಣ *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <ಪೂರ್ಣಾಙ್ಕ max_level> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_debug_ಸಂದೇಶ (ಧ್ರುವ ವರ್ಣ *what,
	       ಧ್ರುವ ಶೂನ್ಯ *obj,
	       ಧ್ರುವ ವರ್ಣ *func,
	       ತಾರ್ಕಿಕ indented,
	       unsigned ಪೂರ್ಣಾಙ್ಕ level,
	       ಪೂರ್ಣಾಙ್ಕ level_dir,
	       ಧ್ರುವ ವರ್ಣ *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <ಪೂರ್ಣಾಙ್ಕ max_level> ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ HB_PRINTF_FUNC(7, 8)
hb_debug_ಸಂದೇಶ (ಧ್ರುವ ವರ್ಣ *what,
	       ಧ್ರುವ ಶೂನ್ಯ *obj,
	       ಧ್ರುವ ವರ್ಣ *func,
	       ತಾರ್ಕಿಕ indented,
	       unsigned ಪೂರ್ಣಾಙ್ಕ level,
	       ಪೂರ್ಣಾಙ್ಕ level_dir,
	       ಧ್ರುವ ವರ್ಣ *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_ಸಂದೇಶ_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
hb_debug_ಸಂದೇಶ<0> (ಧ್ರುವ ವರ್ಣ *what HB_UNUSED,
		  ಧ್ರುವ ಶೂನ್ಯ *obj HB_UNUSED,
		  ಧ್ರುವ ವರ್ಣ *func HB_UNUSED,
		  ತಾರ್ಕಿಕ indented HB_UNUSED,
		  unsigned ಪೂರ್ಣಾಙ್ಕ level HB_UNUSED,
		  ಪೂರ್ಣಾಙ್ಕ level_dir HB_UNUSED,
		  ಧ್ರುವ ವರ್ಣ *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ HB_PRINTF_FUNC(7, 8)
hb_debug_ಸಂದೇಶ<0> (ಧ್ರುವ ವರ್ಣ *what HB_UNUSED,
		  ಧ್ರುವ ಶೂನ್ಯ *obj HB_UNUSED,
		  ಧ್ರುವ ವರ್ಣ *func HB_UNUSED,
		  ತಾರ್ಕಿಕ indented HB_UNUSED,
		  unsigned ಪೂರ್ಣಾಙ್ಕ level HB_UNUSED,
		  ಪೂರ್ಣಾಙ್ಕ level_dir HB_UNUSED,
		  ಧ್ರುವ ವರ್ಣ *message HB_UNUSED,
		  ...) {}

#define DEBUG_ಸಂದೇಶ_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_ಸಂದೇಶ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    ಸತ್ಯ, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_ಸಂದೇಶ(WHAT, OBJ, ...)				hb_debug_ಸಂದೇಶ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    ಅಸತ್ಯ, 0, 0, __VA_ARGS__)
#define DEBUG_ಸಂದೇಶ_FUNC(WHAT, OBJ, ...)				hb_debug_ಸಂದೇಶ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, ಅಸತ್ಯ, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
ಸಂರಚನಾ hb_printer_t {
  ಧ್ರುವ ವರ್ಣ *print (ಧ್ರುವ T&) { ಪ್ರತ್ಯಾಗಮ "something"; }
};

template <>
ಸಂರಚನಾ hb_printer_t<ತಾರ್ಕಿಕ> {
  ಧ್ರುವ ವರ್ಣ *print (ತಾರ್ಕಿಕ v) { ಪ್ರತ್ಯಾಗಮ v ? "true" : "false"; }
};

template <>
ಸಂರಚನಾ hb_printer_t<hb_empty_t> {
  ಧ್ರುವ ವರ್ಣ *print (hb_empty_t) { ಪ್ರತ್ಯಾಗಮ ""; }
};


/*
 * Trace
 */

template <typename T>
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ _hb_warn_no_return (ತಾರ್ಕಿಕ returned)
{
  ಯದಿ (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ _hb_warn_no_return<hb_empty_t> (ತಾರ್ಕಿಕ returned HB_UNUSED) {}
template <>
/*static*/ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ _hb_warn_no_return<ಶೂನ್ಯ> (ತಾರ್ಕಿಕ returned HB_UNUSED) {}

template <ಪೂರ್ಣಾಙ್ಕ max_level, typename ret_t>
ಸಂರಚನಾ hb_auto_trace_t
{
  explicit ಅನ್ತಃಸ್ಥ hb_auto_trace_t (unsigned ಪೂರ್ಣಾಙ್ಕ *plevel_,
				   ಧ್ರುವ ವರ್ಣ *what_,
				   ಧ್ರುವ ಶೂನ್ಯ *obj_,
				   ಧ್ರುವ ವರ್ಣ *func,
				   ಧ್ರುವ ವರ್ಣ *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (ಅಸತ್ಯ)
  {
    ಯದಿ (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_ಸಂದೇಶ_va<max_level> (what, obj, func, ಸತ್ಯ, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    ಯದಿ (!returned) {
      hb_debug_ಸಂದೇಶ<max_level> (what, obj, nullptr, ಸತ್ಯ, plevel ? *plevel : 1, -1, " ");
    }
    ಯದಿ (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 ಧ್ರುವ ವರ್ಣ *func = "",
	 unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲು = 0)
  {
    ಯದಿ (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      ಪ್ರತ್ಯಾಗಮ std::forward<T> (v);
    }

    hb_debug_ಸಂದೇಶ<max_level> (what, obj, func, ಸತ್ಯ, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), ಸಾಲು);
    ಯದಿ (plevel) --*plevel;
    plevel = nullptr;
    returned = ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ std::forward<T> (v);
  }

  ಗುಪ್ತ:
  unsigned ಪೂರ್ಣಾಙ್ಕ *plevel;
  ಧ್ರುವ ವರ್ಣ *what;
  ಧ್ರುವ ಶೂನ್ಯ *obj;
  ತಾರ್ಕಿಕ returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
ಸಂರಚನಾ hb_auto_trace_t<0, ret_t>
{
  explicit ಅನ್ತಃಸ್ಥ hb_auto_trace_t (unsigned ಪೂರ್ಣಾಙ್ಕ *plevel_,
				   ಧ್ರುವ ವರ್ಣ *what_,
				   ಧ್ರುವ ಶೂನ್ಯ *obj_,
				   ಧ್ರುವ ವರ್ಣ *func,
				   ಧ್ರುವ ವರ್ಣ *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 ಧ್ರುವ ವರ್ಣ *func HB_UNUSED = nullptr,
	 unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲು HB_UNUSED = 0) { ಪ್ರತ್ಯಾಗಮ std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
ಸಂರಚನಾ hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 ಧ್ರುವ ವರ್ಣ *func HB_UNUSED = nullptr,
	 unsigned ಪೂರ್ಣಾಙ್ಕ ಸಾಲು HB_UNUSED = 0) { ಪ್ರತ್ಯಾಗಮ std::forward<T> (v); }
};

#define return_trace(RET) ಪ್ರತ್ಯಾಗಮ trace.ret (RET, HB_FUNC, __LINE__)


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
#ಯದಿ HB_DEBUG_APPLY
#define TRACE_APPLY(ಏತತ್) \
	hb_auto_trace_t<HB_DEBUG_APPLY, ತಾರ್ಕಿಕ> trace \
	(&c->debug_depth, c->get_name (), ಏತತ್, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (ಪೂರ್ಣಾಙ್ಕ) c->lookup_index)
#ಅನ್ಯಥಾ
#define TRACE_APPLY(ಏತತ್) hb_no_trace_t<ತಾರ್ಕಿಕ> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#ಯದಿ HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(ಏತತ್) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, ತಾರ್ಕಿಕ> trace \
	(&c->debug_depth, c->get_name (), ಏತತ್, HB_FUNC, \
	 " ")
#ಅನ್ಯಥಾ
#define TRACE_SANITIZE(ಏತತ್) hb_no_trace_t<ತಾರ್ಕಿಕ> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#ಯದಿ HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(ಏತತ್) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, ತಾರ್ಕಿಕ> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#ಅನ್ಯಥಾ
#define TRACE_SERIALIZE(ಏತತ್) hb_no_trace_t<ತಾರ್ಕಿಕ> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#ಯದಿ HB_DEBUG_SUBSET
#define TRACE_SUBSET(ಏತತ್) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, ತಾರ್ಕಿಕ> trace \
  (&c->debug_depth, c->get_name (), ಏತತ್, HB_FUNC, \
   " ")
#ಅನ್ಯಥಾ
#define TRACE_SUBSET(ಏತತ್) hb_no_trace_t<ತಾರ್ಕಿಕ> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#ಯದಿ HB_DEBUG_PAINT
#define TRACE_PAINT(ಏತತ್) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, ಶೂನ್ಯ> trace \
  (&c->debug_depth, c->get_name (), ಏತತ್, HB_FUNC, \
   " ")
#ಅನ್ಯಥಾ
#define TRACE_PAINT(ಏತತ್) HB_UNUSED hb_no_trace_t<ಶೂನ್ಯ> trace
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
#ಯದಿ HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(ಏತತ್, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), ಏತತ್, HB_FUNC, \
	 "format %u", (unsigned) format)
#ಅನ್ಯಥಾ
#define TRACE_DISPATCH(ಏತತ್, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */

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

#અન્તર્ભાવ "hb.hh"
#અન્તર્ભાવ "hb-atomic.hh"
#અન્તર્ભાવ "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

સ્થિર અંતઃરેખ તાર્કિક
_hb_debug (unsigned પૂર્ણાંક level,
	   unsigned પૂર્ણાંક max_level)
{
  પાછા level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

સ્થિર અંતઃરેખ શૂન્ય
_hb_print_func (અચલ અક્ષર *func)
{
  જો (func)
  {
    unsigned પૂર્ણાંક func_len = strlen (func);
    /* Skip "static" */
    જો (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    જો (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    અચલ અક્ષર *space = strchr (func, ' ');
    જો (space)
      func = space + 1;
    /* Skip parameter list */
    અચલ અક્ષર *paren = strchr (func, '(');
    જો (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (પૂર્ણાંક) func_len, func);
  }
}

template <પૂર્ણાંક max_level> સ્થિર અંતઃરેખ શૂન્ય
hb_debug_સંદેશ_va (અચલ અક્ષર *what,
		  અચલ શૂન્ય *obj,
		  અચલ અક્ષર *func,
		  તાર્કિક indented,
		  unsigned પૂર્ણાંક level,
		  પૂર્ણાંક level_dir,
		  અચલ અક્ષર *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <પૂર્ણાંક max_level> સ્થિર અંતઃરેખ શૂન્ય
hb_debug_સંદેશ_va (અચલ અક્ષર *what,
		  અચલ શૂન્ય *obj,
		  અચલ અક્ષર *func,
		  તાર્કિક indented,
		  unsigned પૂર્ણાંક level,
		  પૂર્ણાંક level_dir,
		  અચલ અક્ષર *message,
		  va_list ap)
{
  જો (!_hb_debug (level, max_level))
    પાછા;

  fprintf (stderr, "%-10s", what ? what : "");

  જો (obj)
    fprintf (stderr, "(%*p) ", (પૂર્ણાંક) (2 * કદ (શૂન્ય *)), obj);
  નહીંતર
    fprintf (stderr, " %*s  ", (પૂર્ણાંક) (2 * કદ (શૂન્ય *)), "");

  જો (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    સ્થિર અચલ અક્ષર bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + કદ (bars) - 1 - hb_min ((unsigned પૂર્ણાંક) કદ (bars) - 1, (unsigned પૂર્ણાંક) (કદ (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } નહીંતર
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  જો (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> અંતઃરેખ શૂન્ય HB_PRINTF_FUNC(7, 0)
hb_debug_સંદેશ_va<0> (અચલ અક્ષર *what HB_UNUSED,
		     અચલ શૂન્ય *obj HB_UNUSED,
		     અચલ અક્ષર *func HB_UNUSED,
		     તાર્કિક indented HB_UNUSED,
		     unsigned પૂર્ણાંક level HB_UNUSED,
		     પૂર્ણાંક level_dir HB_UNUSED,
		     અચલ અક્ષર *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <પૂર્ણાંક max_level> સ્થિર અંતઃરેખ શૂન્ય
hb_debug_સંદેશ (અચલ અક્ષર *what,
	       અચલ શૂન્ય *obj,
	       અચલ અક્ષર *func,
	       તાર્કિક indented,
	       unsigned પૂર્ણાંક level,
	       પૂર્ણાંક level_dir,
	       અચલ અક્ષર *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <પૂર્ણાંક max_level> સ્થિર અંતઃરેખ શૂન્ય HB_PRINTF_FUNC(7, 8)
hb_debug_સંદેશ (અચલ અક્ષર *what,
	       અચલ શૂન્ય *obj,
	       અચલ અક્ષર *func,
	       તાર્કિક indented,
	       unsigned પૂર્ણાંક level,
	       પૂર્ણાંક level_dir,
	       અચલ અક્ષર *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_સંદેશ_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> અંતઃરેખ શૂન્ય
hb_debug_સંદેશ<0> (અચલ અક્ષર *what HB_UNUSED,
		  અચલ શૂન્ય *obj HB_UNUSED,
		  અચલ અક્ષર *func HB_UNUSED,
		  તાર્કિક indented HB_UNUSED,
		  unsigned પૂર્ણાંક level HB_UNUSED,
		  પૂર્ણાંક level_dir HB_UNUSED,
		  અચલ અક્ષર *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> અંતઃરેખ શૂન્ય HB_PRINTF_FUNC(7, 8)
hb_debug_સંદેશ<0> (અચલ અક્ષર *what HB_UNUSED,
		  અચલ શૂન્ય *obj HB_UNUSED,
		  અચલ અક્ષર *func HB_UNUSED,
		  તાર્કિક indented HB_UNUSED,
		  unsigned પૂર્ણાંક level HB_UNUSED,
		  પૂર્ણાંક level_dir HB_UNUSED,
		  અચલ અક્ષર *message HB_UNUSED,
		  ...) {}

#define DEBUG_સંદેશ_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_સંદેશ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    સાચું, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_સંદેશ(WHAT, OBJ, ...)				hb_debug_સંદેશ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    ખોટું, 0, 0, __VA_ARGS__)
#define DEBUG_સંદેશ_FUNC(WHAT, OBJ, ...)				hb_debug_સંદેશ<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, ખોટું, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
સંરચના hb_printer_t {
  અચલ અક્ષર *print (અચલ T&) { પાછા "something"; }
};

template <>
સંરચના hb_printer_t<તાર્કિક> {
  અચલ અક્ષર *print (તાર્કિક v) { પાછા v ? "true" : "false"; }
};

template <>
સંરચના hb_printer_t<hb_empty_t> {
  અચલ અક્ષર *print (hb_empty_t) { પાછા ""; }
};


/*
 * Trace
 */

template <typename T>
સ્થિર અંતઃરેખ શૂન્ય _hb_warn_no_return (તાર્કિક returned)
{
  જો (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ અંતઃરેખ શૂન્ય _hb_warn_no_return<hb_empty_t> (તાર્કિક returned HB_UNUSED) {}
template <>
/*static*/ અંતઃરેખ શૂન્ય _hb_warn_no_return<શૂન્ય> (તાર્કિક returned HB_UNUSED) {}

template <પૂર્ણાંક max_level, typename ret_t>
સંરચના hb_auto_trace_t
{
  explicit અંતઃરેખ hb_auto_trace_t (unsigned પૂર્ણાંક *plevel_,
				   અચલ અક્ષર *what_,
				   અચલ શૂન્ય *obj_,
				   અચલ અક્ષર *func,
				   અચલ અક્ષર *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (ખોટું)
  {
    જો (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_સંદેશ_va<max_level> (what, obj, func, સાચું, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    જો (!returned) {
      hb_debug_સંદેશ<max_level> (what, obj, nullptr, સાચું, plevel ? *plevel : 1, -1, " ");
    }
    જો (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 અચલ અક્ષર *func = "",
	 unsigned પૂર્ણાંક લીટી = 0)
  {
    જો (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      પાછા std::forward<T> (v);
    }

    hb_debug_સંદેશ<max_level> (what, obj, func, સાચું, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), લીટી);
    જો (plevel) --*plevel;
    plevel = nullptr;
    returned = સાચું;
    પાછા std::forward<T> (v);
  }

  ખાનગી:
  unsigned પૂર્ણાંક *plevel;
  અચલ અક્ષર *what;
  અચલ શૂન્ય *obj;
  તાર્કિક returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
સંરચના hb_auto_trace_t<0, ret_t>
{
  explicit અંતઃરેખ hb_auto_trace_t (unsigned પૂર્ણાંક *plevel_,
				   અચલ અક્ષર *what_,
				   અચલ શૂન્ય *obj_,
				   અચલ અક્ષર *func,
				   અચલ અક્ષર *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 અચલ અક્ષર *func HB_UNUSED = nullptr,
	 unsigned પૂર્ણાંક લીટી HB_UNUSED = 0) { પાછા std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
સંરચના hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 અચલ અક્ષર *func HB_UNUSED = nullptr,
	 unsigned પૂર્ણાંક લીટી HB_UNUSED = 0) { પાછા std::forward<T> (v); }
};

#define return_trace(RET) પાછા trace.ret (RET, HB_FUNC, __LINE__)


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
#જો HB_DEBUG_APPLY
#define TRACE_APPLY(આ) \
	hb_auto_trace_t<HB_DEBUG_APPLY, તાર્કિક> trace \
	(&c->debug_depth, c->get_name (), આ, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (પૂર્ણાંક) c->lookup_index)
#નહીંતર
#define TRACE_APPLY(આ) hb_no_trace_t<તાર્કિક> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#જો HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(આ) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, તાર્કિક> trace \
	(&c->debug_depth, c->get_name (), આ, HB_FUNC, \
	 " ")
#નહીંતર
#define TRACE_SANITIZE(આ) hb_no_trace_t<તાર્કિક> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#જો HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(આ) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, તાર્કિક> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#નહીંતર
#define TRACE_SERIALIZE(આ) hb_no_trace_t<તાર્કિક> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#જો HB_DEBUG_SUBSET
#define TRACE_SUBSET(આ) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, તાર્કિક> trace \
  (&c->debug_depth, c->get_name (), આ, HB_FUNC, \
   " ")
#નહીંતર
#define TRACE_SUBSET(આ) hb_no_trace_t<તાર્કિક> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#જો HB_DEBUG_PAINT
#define TRACE_PAINT(આ) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, શૂન્ય> trace \
  (&c->debug_depth, c->get_name (), આ, HB_FUNC, \
   " ")
#નહીંતર
#define TRACE_PAINT(આ) HB_UNUSED hb_no_trace_t<શૂન્ય> trace
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
#જો HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(આ, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), આ, HB_FUNC, \
	 "format %u", (unsigned) format)
#નહીંતર
#define TRACE_DISPATCH(આ, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */

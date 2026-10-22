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

#அந்தர்பாவ "hb.hh"
#அந்தர்பாவ "hb-atomic.hh"
#அந்தர்பாவ "hb-algs.hh"


#ifndef HB_DEBUG
#define HB_DEBUG 0
#endif


/*
 * Debug output (needs enabling at compile time.)
 */

நிலையான உள்வரி தருக்க
_hb_debug (unsigned முழு level,
	   unsigned முழு max_level)
{
  திரும்பு level < max_level;
}

#define DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#define DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

நிலையான உள்வரி வெறுமை
_hb_print_func (மாறிலி எழுத்து *func)
{
  எனில் (func)
  {
    unsigned முழு func_len = strlen (func);
    /* Skip "static" */
    எனில் (0 == strncmp (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    எனில் (0 == strncmp (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    மாறிலி எழுத்து *space = strchr (func, ' ');
    எனில் (space)
      func = space + 1;
    /* Skip parameter list */
    மாறிலி எழுத்து *paren = strchr (func, '(');
    எனில் (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (முழு) func_len, func);
  }
}

template <முழு max_level> நிலையான உள்வரி வெறுமை
hb_debug_செய்தி_va (மாறிலி எழுத்து *what,
		  மாறிலி வெறுமை *obj,
		  மாறிலி எழுத்து *func,
		  தருக்க indented,
		  unsigned முழு level,
		  முழு level_dir,
		  மாறிலி எழுத்து *message,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
template <முழு max_level> நிலையான உள்வரி வெறுமை
hb_debug_செய்தி_va (மாறிலி எழுத்து *what,
		  மாறிலி வெறுமை *obj,
		  மாறிலி எழுத்து *func,
		  தருக்க indented,
		  unsigned முழு level,
		  முழு level_dir,
		  மாறிலி எழுத்து *message,
		  va_list ap)
{
  எனில் (!_hb_debug (level, max_level))
    திரும்பு;

  fprintf (stderr, "%-10s", what ? what : "");

  எனில் (obj)
    fprintf (stderr, "(%*p) ", (முழு) (2 * அளவு (வெறுமை *)), obj);
  இல்லையெனில்
    fprintf (stderr, " %*s  ", (முழு) (2 * அளவு (வெறுமை *)), "");

  எனில் (indented) {
#define VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#define VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#define DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#define ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#define LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    நிலையான மாறிலி எழுத்து bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + அளவு (bars) - 1 - hb_min ((unsigned முழு) அளவு (bars) - 1, (unsigned முழு) (அளவு (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } இல்லையெனில்
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  எனில் (message)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, message, ap);
  }

  fprintf (stderr, "\n");
}
template <> உள்வரி வெறுமை HB_PRINTF_FUNC(7, 0)
hb_debug_செய்தி_va<0> (மாறிலி எழுத்து *what HB_UNUSED,
		     மாறிலி வெறுமை *obj HB_UNUSED,
		     மாறிலி எழுத்து *func HB_UNUSED,
		     தருக்க indented HB_UNUSED,
		     unsigned முழு level HB_UNUSED,
		     முழு level_dir HB_UNUSED,
		     மாறிலி எழுத்து *message HB_UNUSED,
		     va_list ap HB_UNUSED) {}

template <முழு max_level> நிலையான உள்வரி வெறுமை
hb_debug_செய்தி (மாறிலி எழுத்து *what,
	       மாறிலி வெறுமை *obj,
	       மாறிலி எழுத்து *func,
	       தருக்க indented,
	       unsigned முழு level,
	       முழு level_dir,
	       மாறிலி எழுத்து *message,
	       ...) HB_PRINTF_FUNC(7, 8);
template <முழு max_level> நிலையான உள்வரி வெறுமை HB_PRINTF_FUNC(7, 8)
hb_debug_செய்தி (மாறிலி எழுத்து *what,
	       மாறிலி வெறுமை *obj,
	       மாறிலி எழுத்து *func,
	       தருக்க indented,
	       unsigned முழு level,
	       முழு level_dir,
	       மாறிலி எழுத்து *message,
	       ...)
{
  va_list ap;
  va_start (ap, message);
  hb_debug_செய்தி_va<max_level> (what, obj, func, indented, level, level_dir, message, ap);
  va_end (ap);
}
template <> உள்வரி வெறுமை
hb_debug_செய்தி<0> (மாறிலி எழுத்து *what HB_UNUSED,
		  மாறிலி வெறுமை *obj HB_UNUSED,
		  மாறிலி எழுத்து *func HB_UNUSED,
		  தருக்க indented HB_UNUSED,
		  unsigned முழு level HB_UNUSED,
		  முழு level_dir HB_UNUSED,
		  மாறிலி எழுத்து *message HB_UNUSED,
		  ...) HB_PRINTF_FUNC(7, 8);
template <> உள்வரி வெறுமை HB_PRINTF_FUNC(7, 8)
hb_debug_செய்தி<0> (மாறிலி எழுத்து *what HB_UNUSED,
		  மாறிலி வெறுமை *obj HB_UNUSED,
		  மாறிலி எழுத்து *func HB_UNUSED,
		  தருக்க indented HB_UNUSED,
		  unsigned முழு level HB_UNUSED,
		  முழு level_dir HB_UNUSED,
		  மாறிலி எழுத்து *message HB_UNUSED,
		  ...) {}

#define DEBUG_செய்தி_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_செய்தி<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    உண்மை, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#define DEBUG_செய்தி(WHAT, OBJ, ...)				hb_debug_செய்தி<HB_DEBUG_##WHAT> (#WHAT, (OBJ), nullptr,    பொய், 0, 0, __VA_ARGS__)
#define DEBUG_செய்தி_FUNC(WHAT, OBJ, ...)				hb_debug_செய்தி<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, பொய், 0, 0, __VA_ARGS__)


/*
 * Printer
 */

template <typename T>
கட்டமைப்பு hb_printer_t {
  மாறிலி எழுத்து *print (மாறிலி T&) { திரும்பு "something"; }
};

template <>
கட்டமைப்பு hb_printer_t<தருக்க> {
  மாறிலி எழுத்து *print (தருக்க v) { திரும்பு v ? "true" : "false"; }
};

template <>
கட்டமைப்பு hb_printer_t<hb_empty_t> {
  மாறிலி எழுத்து *print (hb_empty_t) { திரும்பு ""; }
};


/*
 * Trace
 */

template <typename T>
நிலையான உள்வரி வெறுமை _hb_warn_no_return (தருக்க returned)
{
  எனில் (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
template <>
/*static*/ உள்வரி வெறுமை _hb_warn_no_return<hb_empty_t> (தருக்க returned HB_UNUSED) {}
template <>
/*static*/ உள்வரி வெறுமை _hb_warn_no_return<வெறுமை> (தருக்க returned HB_UNUSED) {}

template <முழு max_level, typename ret_t>
கட்டமைப்பு hb_auto_trace_t
{
  explicit உள்வரி hb_auto_trace_t (unsigned முழு *plevel_,
				   மாறிலி எழுத்து *what_,
				   மாறிலி வெறுமை *obj_,
				   மாறிலி எழுத்து *func,
				   மாறிலி எழுத்து *message,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (பொய்)
  {
    எனில் (plevel) ++*plevel;

    va_list ap;
    va_start (ap, message);
    hb_debug_செய்தி_va<max_level> (what, obj, func, உண்மை, plevel ? *plevel : 0, +1, message, ap);
    va_end (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    எனில் (!returned) {
      hb_debug_செய்தி<max_level> (what, obj, nullptr, உண்மை, plevel ? *plevel : 1, -1, " ");
    }
    எனில் (plevel) --*plevel;
  }

  template <typename T>
  T ret (T&& v,
	 மாறிலி எழுத்து *func = "",
	 unsigned முழு வரி = 0)
  {
    எனில் (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      திரும்பு std::forward<T> (v);
    }

    hb_debug_செய்தி<max_level> (what, obj, func, உண்மை, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), வரி);
    எனில் (plevel) --*plevel;
    plevel = nullptr;
    returned = உண்மை;
    திரும்பு std::forward<T> (v);
  }

  தனிப்பட்ட:
  unsigned முழு *plevel;
  மாறிலி எழுத்து *what;
  மாறிலி வெறுமை *obj;
  தருக்க returned;
};
template <typename ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
கட்டமைப்பு hb_auto_trace_t<0, ret_t>
{
  explicit உள்வரி hb_auto_trace_t (unsigned முழு *plevel_,
				   மாறிலி எழுத்து *what_,
				   மாறிலி வெறுமை *obj_,
				   மாறிலி எழுத்து *func,
				   மாறிலி எழுத்து *message,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  template <typename T>
  T ret (T&& v,
	 மாறிலி எழுத்து *func HB_UNUSED = nullptr,
	 unsigned முழு வரி HB_UNUSED = 0) { திரும்பு std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
template <typename ret_t>
கட்டமைப்பு hb_no_trace_t {
  template <typename T>
  T ret (T&& v,
	 மாறிலி எழுத்து *func HB_UNUSED = nullptr,
	 unsigned முழு வரி HB_UNUSED = 0) { திரும்பு std::forward<T> (v); }
};

#define return_trace(RET) திரும்பு trace.ret (RET, HB_FUNC, __LINE__)


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
#எனில் HB_DEBUG_APPLY
#define TRACE_APPLY(இது) \
	hb_auto_trace_t<HB_DEBUG_APPLY, தருக்க> trace \
	(&c->debug_depth, c->get_name (), இது, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->buffer->idx, c->buffer->cur().codepoint, (முழு) c->lookup_index)
#இல்லையெனில்
#define TRACE_APPLY(இது) hb_no_trace_t<தருக்க> trace
#endif

#ifndef HB_DEBUG_SANITIZE
#define HB_DEBUG_SANITIZE (HB_DEBUG+0)
#endif
#எனில் HB_DEBUG_SANITIZE
#define TRACE_SANITIZE(இது) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, தருக்க> trace \
	(&c->debug_depth, c->get_name (), இது, HB_FUNC, \
	 " ")
#இல்லையெனில்
#define TRACE_SANITIZE(இது) hb_no_trace_t<தருக்க> trace
#endif

#ifndef HB_DEBUG_SERIALIZE
#define HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#endif
#எனில் HB_DEBUG_SERIALIZE
#define TRACE_SERIALIZE(இது) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, தருக்க> trace \
	(&c->debug_depth, "SERIALIZE", c, HB_FUNC, \
	 " ")
#இல்லையெனில்
#define TRACE_SERIALIZE(இது) hb_no_trace_t<தருக்க> trace
#endif

#ifndef HB_DEBUG_SUBSET
#define HB_DEBUG_SUBSET (HB_DEBUG+0)
#endif
#எனில் HB_DEBUG_SUBSET
#define TRACE_SUBSET(இது) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, தருக்க> trace \
  (&c->debug_depth, c->get_name (), இது, HB_FUNC, \
   " ")
#இல்லையெனில்
#define TRACE_SUBSET(இது) hb_no_trace_t<தருக்க> trace
#endif

#ifndef HB_DEBUG_SUBSET_REPACK
#define HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#endif

#ifndef HB_DEBUG_PAINT
#define HB_DEBUG_PAINT (HB_DEBUG+0)
#endif
#எனில் HB_DEBUG_PAINT
#define TRACE_PAINT(இது) \
  HB_UNUSED hb_auto_trace_t<HB_DEBUG_PAINT, வெறுமை> trace \
  (&c->debug_depth, c->get_name (), இது, HB_FUNC, \
   " ")
#இல்லையெனில்
#define TRACE_PAINT(இது) HB_UNUSED hb_no_trace_t<வெறுமை> trace
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
#எனில் HB_DEBUG_DISPATCH
#define TRACE_DISPATCH(இது, format) \
	hb_auto_trace_t<context_t::max_debug_depth, typename context_t::return_t> trace \
	(&c->debug_depth, c->get_name (), இது, HB_FUNC, \
	 "format %u", (unsigned) format)
#இல்லையெனில்
#define TRACE_DISPATCH(இது, format) hb_no_trace_t<typename context_t::return_t> trace
#endif


#ifndef HB_BUFFER_MESSAGE_MORE
#define HB_BUFFER_MESSAGE_MORE (HB_DEBUG+0)
#endif


#endif /* HB_DEBUG_HH */

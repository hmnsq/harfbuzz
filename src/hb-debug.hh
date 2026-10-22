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

#నిర్వచితం_కాకపోతే HB_DEBUG_HH
#నిర్వచించు HB_DEBUG_HH

#అన్తర్భావ "hb.hh"
#అన్తర్భావ "hb-atomic.hh"
#అన్తర్భావ "hb-algs.hh"


#నిర్వచితం_కాకపోతే HB_DEBUG
#నిర్వచించు HB_DEBUG 0
#అయితే_ముగింపు


/*
 * Debug output (needs enabling at compile time.)
 */

స్థిర అంతఃవరుస తార్కిక
_hb_debug (గుర్తులేని పూర్ణాంకం level,
	   గుర్తులేని పూర్ణాంకం max_level)
{
  తిరిగి level < max_level;
}

#నిర్వచించు DEBUG_LEVEL_ENABLED(WHAT, LEVEL) (_hb_debug ((LEVEL), HB_DEBUG_##WHAT))
#నిర్వచించు DEBUG_ENABLED(WHAT) (DEBUG_LEVEL_ENABLED (WHAT, 0))

స్థిర అంతఃవరుస శూన్య
_hb_print_func (స్థిరరాశి అక్షరం *func)
{
  ఒకవేళ (func)
  {
    గుర్తులేని పూర్ణాంకం func_len = అక్షరశ్రేణి_పొడవు (func);
    /* Skip "static" */
    ఒకవేళ (0 == అక్షరశ్రేణి_పోలిక (func, "static ", 7))
      func += 7;
    /* Skip "typename" */
    ఒకవేళ (0 == అక్షరశ్రేణి_పోలిక (func, "typename ", 9))
      func += 9;
    /* Skip return type */
    స్థిరరాశి అక్షరం *space = strchr (func, ' ');
    ఒకవేళ (space)
      func = space + 1;
    /* Skip parameter list */
    స్థిరరాశి అక్షరం *paren = strchr (func, '(');
    ఒకవేళ (paren)
      func_len = paren - func;
    fprintf (stderr, "%.*s", (పూర్ణాంకం) func_len, func);
  }
}

మూస <పూర్ణాంకం max_level> స్థిర అంతఃవరుస శూన్య
hb_debug_సందేశం_va (స్థిరరాశి అక్షరం *what,
		  స్థిరరాశి శూన్య *obj,
		  స్థిరరాశి అక్షరం *func,
		  తార్కిక indented,
		  గుర్తులేని పూర్ణాంకం level,
		  పూర్ణాంకం level_dir,
		  స్థిరరాశి అక్షరం *సందేశం,
		  va_list ap) HB_PRINTF_FUNC(7, 0);
మూస <పూర్ణాంకం max_level> స్థిర అంతఃవరుస శూన్య
hb_debug_సందేశం_va (స్థిరరాశి అక్షరం *what,
		  స్థిరరాశి శూన్య *obj,
		  స్థిరరాశి అక్షరం *func,
		  తార్కిక indented,
		  గుర్తులేని పూర్ణాంకం level,
		  పూర్ణాంకం level_dir,
		  స్థిరరాశి అక్షరం *సందేశం,
		  va_list ap)
{
  ఒకవేళ (!_hb_debug (level, max_level))
    తిరిగి;

  fprintf (stderr, "%-10s", what ? what : "");

  ఒకవేళ (obj)
    fprintf (stderr, "(%*p) ", (పూర్ణాంకం) (2 * పరిమాణం (శూన్య *)), obj);
  లేకపోతే
    fprintf (stderr, " %*s  ", (పూర్ణాంకం) (2 * పరిమాణం (శూన్య *)), "");

  ఒకవేళ (indented) {
#నిర్వచించు VBAR	"\342\224\202"	/* U+2502 BOX DRAWINGS LIGHT VERTICAL */
#నిర్వచించు VRBAR	"\342\224\234"	/* U+251C BOX DRAWINGS LIGHT VERTICAL AND RIGHT */
#నిర్వచించు DLBAR	"\342\225\256"	/* U+256E BOX DRAWINGS LIGHT ARC DOWN AND LEFT */
#నిర్వచించు ULBAR	"\342\225\257"	/* U+256F BOX DRAWINGS LIGHT ARC UP AND LEFT */
#నిర్వచించు LBAR	"\342\225\264"	/* U+2574 BOX DRAWINGS LIGHT LEFT */
    స్థిర స్థిరరాశి అక్షరం bars[] =
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR
      VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR VBAR;
    fprintf (stderr, "%2u %s" VRBAR "%s",
	     level,
	     bars + పరిమాణం (bars) - 1 - hb_min ((గుర్తులేని పూర్ణాంకం) పరిమాణం (bars) - 1, (గుర్తులేని పూర్ణాంకం) (పరిమాణం (VBAR) - 1) * level),
	     level_dir ? (level_dir > 0 ? DLBAR : ULBAR) : LBAR);
  } లేకపోతే
    fprintf (stderr, "   " VRBAR LBAR);

  _hb_print_func (func);

  ఒకవేళ (సందేశం)
  {
    fprintf (stderr, ": ");
    vfprintf (stderr, సందేశం, ap);
  }

  fprintf (stderr, "\n");
}
మూస <> అంతఃవరుస శూన్య HB_PRINTF_FUNC(7, 0)
hb_debug_సందేశం_va<0> (స్థిరరాశి అక్షరం *what HB_వాడని,
		     స్థిరరాశి శూన్య *obj HB_వాడని,
		     స్థిరరాశి అక్షరం *func HB_వాడని,
		     తార్కిక indented HB_వాడని,
		     గుర్తులేని పూర్ణాంకం level HB_వాడని,
		     పూర్ణాంకం level_dir HB_వాడని,
		     స్థిరరాశి అక్షరం *సందేశం HB_వాడని,
		     va_list ap HB_వాడని) {}

మూస <పూర్ణాంకం max_level> స్థిర అంతఃవరుస శూన్య
hb_debug_సందేశం (స్థిరరాశి అక్షరం *what,
	       స్థిరరాశి శూన్య *obj,
	       స్థిరరాశి అక్షరం *func,
	       తార్కిక indented,
	       గుర్తులేని పూర్ణాంకం level,
	       పూర్ణాంకం level_dir,
	       స్థిరరాశి అక్షరం *సందేశం,
	       ...) HB_PRINTF_FUNC(7, 8);
మూస <పూర్ణాంకం max_level> స్థిర అంతఃవరుస శూన్య HB_PRINTF_FUNC(7, 8)
hb_debug_సందేశం (స్థిరరాశి అక్షరం *what,
	       స్థిరరాశి శూన్య *obj,
	       స్థిరరాశి అక్షరం *func,
	       తార్కిక indented,
	       గుర్తులేని పూర్ణాంకం level,
	       పూర్ణాంకం level_dir,
	       స్థిరరాశి అక్షరం *సందేశం,
	       ...)
{
  va_list ap;
  va_ప్రారంభం (ap, సందేశం);
  hb_debug_సందేశం_va<max_level> (what, obj, func, indented, level, level_dir, సందేశం, ap);
  va_చివర (ap);
}
మూస <> అంతఃవరుస శూన్య
hb_debug_సందేశం<0> (స్థిరరాశి అక్షరం *what HB_వాడని,
		  స్థిరరాశి శూన్య *obj HB_వాడని,
		  స్థిరరాశి అక్షరం *func HB_వాడని,
		  తార్కిక indented HB_వాడని,
		  గుర్తులేని పూర్ణాంకం level HB_వాడని,
		  పూర్ణాంకం level_dir HB_వాడని,
		  స్థిరరాశి అక్షరం *సందేశం HB_వాడని,
		  ...) HB_PRINTF_FUNC(7, 8);
మూస <> అంతఃవరుస శూన్య HB_PRINTF_FUNC(7, 8)
hb_debug_సందేశం<0> (స్థిరరాశి అక్షరం *what HB_వాడని,
		  స్థిరరాశి శూన్య *obj HB_వాడని,
		  స్థిరరాశి అక్షరం *func HB_వాడని,
		  తార్కిక indented HB_వాడని,
		  గుర్తులేని పూర్ణాంకం level HB_వాడని,
		  పూర్ణాంకం level_dir HB_వాడని,
		  స్థిరరాశి అక్షరం *సందేశం HB_వాడని,
		  ...) {}

#నిర్వచించు DEBUG_సందేశం_LEVEL(WHAT, OBJ, LEVEL, LEVEL_DIR, ...)	hb_debug_సందేశం<HB_DEBUG_##WHAT> (#WHAT, (OBJ), శూన్యసూచిక,    నిజం, (LEVEL), (LEVEL_DIR), __VA_ARGS__)
#నిర్వచించు DEBUG_సందేశం(WHAT, OBJ, ...)				hb_debug_సందేశం<HB_DEBUG_##WHAT> (#WHAT, (OBJ), శూన్యసూచిక,    అబద్ధం, 0, 0, __VA_ARGS__)
#నిర్వచించు DEBUG_సందేశం_FUNC(WHAT, OBJ, ...)				hb_debug_సందేశం<HB_DEBUG_##WHAT> (#WHAT, (OBJ), HB_FUNC, అబద్ధం, 0, 0, __VA_ARGS__)


/*
 * Printer
 */

మూస <రకం_పేరు T>
నిర్మాణం hb_printer_t {
  స్థిరరాశి అక్షరం *print (స్థిరరాశి T&) { తిరిగి "something"; }
};

మూస <>
నిర్మాణం hb_printer_t<తార్కిక> {
  స్థిరరాశి అక్షరం *print (తార్కిక v) { తిరిగి v ? "true" : "false"; }
};

మూస <>
నిర్మాణం hb_printer_t<hb_ఖాళీలు_t> {
  స్థిరరాశి అక్షరం *print (hb_ఖాళీలు_t) { తిరిగి ""; }
};


/*
 * Trace
 */

మూస <రకం_పేరు T>
స్థిర అంతఃవరుస శూన్య _hb_warn_no_return (తార్కిక returned)
{
  ఒకవేళ (unlikely (!returned)) {
    fprintf (stderr, "OUCH, returned with no call to return_trace().  This is a bug, please report.\n");
  }
}
మూస <>
/*static*/ అంతఃవరుస శూన్య _hb_warn_no_return<hb_ఖాళీలు_t> (తార్కిక returned HB_వాడని) {}
మూస <>
/*static*/ అంతఃవరుస శూన్య _hb_warn_no_return<శూన్య> (తార్కిక returned HB_వాడని) {}

మూస <పూర్ణాంకం max_level, రకం_పేరు ret_t>
నిర్మాణం hb_auto_trace_t
{
  స్పష్టమైన అంతఃవరుస hb_auto_trace_t (గుర్తులేని పూర్ణాంకం *plevel_,
				   స్థిరరాశి అక్షరం *what_,
				   స్థిరరాశి శూన్య *obj_,
				   స్థిరరాశి అక్షరం *func,
				   స్థిరరాశి అక్షరం *సందేశం,
				   ...) HB_PRINTF_FUNC(6, 7)
				   : plevel (plevel_), what (what_), obj (obj_), returned (అబద్ధం)
  {
    ఒకవేళ (plevel) ++*plevel;

    va_list ap;
    va_ప్రారంభం (ap, సందేశం);
    hb_debug_సందేశం_va<max_level> (what, obj, func, నిజం, plevel ? *plevel : 0, +1, సందేశం, ap);
    va_చివర (ap);
  }
  ~hb_auto_trace_t ()
  {
    _hb_warn_no_return<ret_t> (returned);
    ఒకవేళ (!returned) {
      hb_debug_సందేశం<max_level> (what, obj, శూన్యసూచిక, నిజం, plevel ? *plevel : 1, -1, " ");
    }
    ఒకవేళ (plevel) --*plevel;
  }

  మూస <రకం_పేరు T>
  T ret (T&& v,
	 స్థిరరాశి అక్షరం *func = "",
	 గుర్తులేని పూర్ణాంకం పంక్తి = 0)
  {
    ఒకవేళ (unlikely (returned)) {
      fprintf (stderr, "OUCH, double calls to return_trace().  This is a bug, please report.\n");
      తిరిగి std::forward<T> (v);
    }

    hb_debug_సందేశం<max_level> (what, obj, func, నిజం, plevel ? *plevel : 1, -1,
			      "return %s (line %u)",
			      hb_printer_t<hb_decay<decltype (v)>>().print (v), పంక్తి);
    ఒకవేళ (plevel) --*plevel;
    plevel = శూన్యసూచిక;
    returned = నిజం;
    తిరిగి std::forward<T> (v);
  }

  అంతరంగ:
  గుర్తులేని పూర్ణాంకం *plevel;
  స్థిరరాశి అక్షరం *what;
  స్థిరరాశి శూన్య *obj;
  తార్కిక returned;
};
మూస <రకం_పేరు ret_t> /* Make sure we don't use hb_auto_trace_t when not tracing. */
నిర్మాణం hb_auto_trace_t<0, ret_t>
{
  స్పష్టమైన అంతఃవరుస hb_auto_trace_t (గుర్తులేని పూర్ణాంకం *plevel_,
				   స్థిరరాశి అక్షరం *what_,
				   స్థిరరాశి శూన్య *obj_,
				   స్థిరరాశి అక్షరం *func,
				   స్థిరరాశి అక్షరం *సందేశం,
				   ...) HB_PRINTF_FUNC(6, 7) {}

  మూస <రకం_పేరు T>
  T ret (T&& v,
	 స్థిరరాశి అక్షరం *func HB_వాడని = శూన్యసూచిక,
	 గుర్తులేని పూర్ణాంకం పంక్తి HB_వాడని = 0) { తిరిగి std::forward<T> (v); }
};

/* For disabled tracing; optimize out everything.
 * https://github.com/harfbuzz/harfbuzz/pull/605 */
మూస <రకం_పేరు ret_t>
నిర్మాణం hb_no_trace_t {
  మూస <రకం_పేరు T>
  T ret (T&& v,
	 స్థిరరాశి అక్షరం *func HB_వాడని = శూన్యసూచిక,
	 గుర్తులేని పూర్ణాంకం పంక్తి HB_వాడని = 0) { తిరిగి std::forward<T> (v); }
};

#నిర్వచించు return_trace(RET) తిరిగి trace.ret (RET, HB_FUNC, __LINE__)


/*
 * Instances.
 */

#నిర్వచితం_కాకపోతే HB_DEBUG_ARABIC
#నిర్వచించు HB_DEBUG_ARABIC (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_BLOB
#నిర్వచించు HB_DEBUG_BLOB (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_CORETEXT
#నిర్వచించు HB_DEBUG_CORETEXT (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_DIRECTWRITE
#నిర్వచించు HB_DEBUG_DIRECTWRITE (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_FT
#నిర్వచించు HB_DEBUG_FT (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_JUSTIFY
#నిర్వచించు HB_DEBUG_JUSTIFY (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_OBJECT
#నిర్వచించు HB_DEBUG_OBJECT (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_SHAPE_PLAN
#నిర్వచించు HB_DEBUG_SHAPE_PLAN (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_UNISCRIBE
#నిర్వచించు HB_DEBUG_UNISCRIBE (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_WASM
#నిర్వచించు HB_DEBUG_WASM (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_KBTS
#నిర్వచించు HB_DEBUG_KBTS (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_DEPEND
#నిర్వచించు HB_DEBUG_DEPEND (HB_DEBUG+0)
#అయితే_ముగింపు

/*
 * With tracing.
 */

#నిర్వచితం_కాకపోతే HB_DEBUG_APPLY
#నిర్వచించు HB_DEBUG_APPLY (HB_DEBUG+0)
#అయితే_ముగింపు
#ఒకవేళ HB_DEBUG_APPLY
#నిర్వచించు TRACE_APPLY(ఇది) \
	hb_auto_trace_t<HB_DEBUG_APPLY, తార్కిక> trace \
	(&c->debug_లోతు, c->get_name (), ఇది, HB_FUNC, \
	 "idx %u gid %u lookup %d", \
	 c->బఫరు->idx, c->బఫరు->cur().codepoint, (పూర్ణాంకం) c->lookup_సూచీ)
#లేకపోతే
#నిర్వచించు TRACE_APPLY(ఇది) hb_no_trace_t<తార్కిక> trace
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_SANITIZE
#నిర్వచించు HB_DEBUG_SANITIZE (HB_DEBUG+0)
#అయితే_ముగింపు
#ఒకవేళ HB_DEBUG_SANITIZE
#నిర్వచించు TRACE_SANITIZE(ఇది) \
	hb_auto_trace_t<HB_DEBUG_SANITIZE, తార్కిక> trace \
	(&c->debug_లోతు, c->get_name (), ఇది, HB_FUNC, \
	 " ")
#లేకపోతే
#నిర్వచించు TRACE_SANITIZE(ఇది) hb_no_trace_t<తార్కిక> trace
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_SERIALIZE
#నిర్వచించు HB_DEBUG_SERIALIZE (HB_DEBUG+0)
#అయితే_ముగింపు
#ఒకవేళ HB_DEBUG_SERIALIZE
#నిర్వచించు TRACE_SERIALIZE(ఇది) \
	hb_auto_trace_t<HB_DEBUG_SERIALIZE, తార్కిక> trace \
	(&c->debug_లోతు, "SERIALIZE", c, HB_FUNC, \
	 " ")
#లేకపోతే
#నిర్వచించు TRACE_SERIALIZE(ఇది) hb_no_trace_t<తార్కిక> trace
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_SUBSET
#నిర్వచించు HB_DEBUG_SUBSET (HB_DEBUG+0)
#అయితే_ముగింపు
#ఒకవేళ HB_DEBUG_SUBSET
#నిర్వచించు TRACE_SUBSET(ఇది) \
  hb_auto_trace_t<HB_DEBUG_SUBSET, తార్కిక> trace \
  (&c->debug_లోతు, c->get_name (), ఇది, HB_FUNC, \
   " ")
#లేకపోతే
#నిర్వచించు TRACE_SUBSET(ఇది) hb_no_trace_t<తార్కిక> trace
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_SUBSET_REPACK
#నిర్వచించు HB_DEBUG_SUBSET_REPACK (HB_DEBUG+0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HB_DEBUG_PAINT
#నిర్వచించు HB_DEBUG_PAINT (HB_DEBUG+0)
#అయితే_ముగింపు
#ఒకవేళ HB_DEBUG_PAINT
#నిర్వచించు TRACE_PAINT(ఇది) \
  HB_వాడని hb_auto_trace_t<HB_DEBUG_PAINT, శూన్య> trace \
  (&c->debug_లోతు, c->get_name (), ఇది, HB_FUNC, \
   " ")
#లేకపోతే
#నిర్వచించు TRACE_PAINT(ఇది) HB_వాడని hb_no_trace_t<శూన్య> trace
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_DEBUG_DISPATCH
#నిర్వచించు HB_DEBUG_DISPATCH ( \
	HB_DEBUG_APPLY + \
	HB_DEBUG_SANITIZE + \
	HB_DEBUG_SERIALIZE + \
	HB_DEBUG_SUBSET + \
	HB_DEBUG_PAINT + \
	0)
#అయితే_ముగింపు
#ఒకవేళ HB_DEBUG_DISPATCH
#నిర్వచించు TRACE_DISPATCH(ఇది, format) \
	hb_auto_trace_t<context_t::max_debug_లోతు, రకం_పేరు context_t::return_t> trace \
	(&c->debug_లోతు, c->get_name (), ఇది, HB_FUNC, \
	 "format %u", (గుర్తులేని) format)
#లేకపోతే
#నిర్వచించు TRACE_DISPATCH(ఇది, format) hb_no_trace_t<రకం_పేరు context_t::return_t> trace
#అయితే_ముగింపు


#నిర్వచితం_కాకపోతే HB_బఫరు_సందేశం_మరిన్ని
#నిర్వచించు HB_బఫరు_సందేశం_మరిన్ని (HB_DEBUG+0)
#అయితే_ముగింపు


#అయితే_ముగింపు /* HB_DEBUG_HH */

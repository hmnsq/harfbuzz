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

#నిర్వచితం_కాకపోతే HB_DISPATCH_HH
#నిర్వచించు HB_DISPATCH_HH

#అన్తర్భావ "hb.hh"

/*
 * Dispatch
 */

మూస <రకం_పేరు Context, రకం_పేరు Return=hb_ఖాళీలు_t, గుర్తులేని పూర్ణాంకం Max_Debug_లోతు=0>
నిర్మాణం hb_dispatch_context_t
{
  అంతరంగ:
  /* https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern */
  స్థిరరాశి Context* thiz () స్థిరరాశి { తిరిగి static_cast<స్థిరరాశి Context *> (ఇది); }
	Context* thiz ()       { తిరిగి static_cast<      Context *> (ఇది); }
  బహిరంగ:
  స్థిరరాశి అక్షరం *get_name () { తిరిగి "UNKNOWN"; }
  స్థిర constexpr గుర్తులేని max_debug_లోతు = Max_Debug_లోతు;
  రకనిర్వచనం Return return_t;
  మూస <రకం_పేరు T, రకం_పేరు F>
  తార్కిక may_dispatch (స్థిరరాశి T *obj HB_వాడని, స్థిరరాశి F *format HB_వాడని) { తిరిగి నిజం; }
  మూస <రకం_పేరు T, రకం_పేరు ...Ts>
  return_t dispatch (స్థిరరాశి T &obj, Ts&&... ds)
  { తిరిగి obj.dispatch (thiz (), std::forward<Ts> (ds)...); }
  స్థిర return_t no_dispatch_return_విలువ () { తిరిగి Context::default_return_విలువ (); }
  స్థిర తార్కిక stop_sublookup_iteration (స్థిరరాశి return_t r HB_వాడని) { తిరిగి అబద్ధం; }
  గుర్తులేని debug_లోతు = 0;
};


#అయితే_ముగింపు /* HB_DISPATCH_HH */

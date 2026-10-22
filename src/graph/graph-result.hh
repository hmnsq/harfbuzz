/*
 * Copyright © 2026  Google, Inc.
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
 * Google Author(s): Garret Rieger
 */

#నిర్వచితం_కాకపోతే లేఖాచిత్రం_లేఖాచిత్రం_ఫలితం_HH
#నిర్వచించు లేఖాచిత్రం_లేఖాచిత్రం_ఫలితం_HH

#అన్తర్భావ "../hb-result.hh"

నామాకాశ లేఖాచిత్రం {

గణన లేఖాచిత్రం_error_t {
  కేటాయింపు_FAILURE,
  హద్దు_EXCEEDED,
  INVALID_వాదం,
  చక్రం_DETECTED,
  SANITIZE_FAILURE,
  బహిర్గతం_యొక్క_BOUNDS,
  ORPHANED_NODES,
  అధికప్రవాహం_RESOLUTION_FAILED,
  UNKNOWN,
};

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం* to_string(లేఖాచిత్రం_error_t e) {
  ఎంపిక (e) {
    స్థితి కేటాయింపు_FAILURE: తిరిగి "Memory Allocation Failed";
    స్థితి హద్దు_EXCEEDED: తిరిగి "Limit Exceeded";
    స్థితి INVALID_వాదం: తిరిగి "Invalid Argument";
    స్థితి చక్రం_DETECTED: తిరిగి "Cycle in Graph";
    స్థితి SANITIZE_FAILURE: తిరిగి "Failed Sanitization";
    స్థితి బహిర్గతం_యొక్క_BOUNDS: తిరిగి "Out of Bounds";
    స్థితి ORPHANED_NODES: తిరిగి "Graph is not fully connected";
    స్థితి అధికప్రవాహం_RESOLUTION_FAILED: తిరిగి "Overflows are not able to be resolved";
    స్థితి UNKNOWN:
    అప్రమేయ:
      తిరిగి "Unknown Error";
  }
}

మూస<రకం_పేరు T>
ఉపయోగించి లేఖాచిత్రం_ఫలితం_t = hb_ఫలితం_t<T, లేఖాచిత్రం_error_t>;

} // namespace graph

#అయితే_ముగింపు /* GRAPH_GRAPH_RESULT_HH */

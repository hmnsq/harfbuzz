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

#ifndef GRAPH_GRAPH_RESULT_HH
#define GRAPH_GRAPH_RESULT_HH

#ଅନ୍ତର୍ଭାଵ "../hb-result.hh"

ନାମାକାଶ graph {

ଗଣନା graph_error_t {
  ALLOCATION_FAILURE,
  LIMIT_EXCEEDED,
  INVALID_ARGUMENT,
  CYCLE_DETECTED,
  SANITIZE_FAILURE,
  OUT_OF_BOUNDS,
  ORPHANED_NODES,
  OVERFLOW_RESOLUTION_FAILED,
  UNKNOWN,
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର* to_string(graph_error_t e) {
  ବାଛ (e) {
    ସ୍ଥିତି ALLOCATION_FAILURE: ଫେରନ୍ତୁ "Memory Allocation Failed";
    ସ୍ଥିତି LIMIT_EXCEEDED: ଫେରନ୍ତୁ "Limit Exceeded";
    ସ୍ଥିତି INVALID_ARGUMENT: ଫେରନ୍ତୁ "Invalid Argument";
    ସ୍ଥିତି CYCLE_DETECTED: ଫେରନ୍ତୁ "Cycle in Graph";
    ସ୍ଥିତି SANITIZE_FAILURE: ଫେରନ୍ତୁ "Failed Sanitization";
    ସ୍ଥିତି OUT_OF_BOUNDS: ଫେରନ୍ତୁ "Out of Bounds";
    ସ୍ଥିତି ORPHANED_NODES: ଫେରନ୍ତୁ "Graph is not fully connected";
    ସ୍ଥିତି OVERFLOW_RESOLUTION_FAILED: ଫେରନ୍ତୁ "Overflows are not able to be resolved";
    ସ୍ଥିତି UNKNOWN:
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

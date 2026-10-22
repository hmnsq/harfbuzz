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

#અન્તર્ભાવ "../hb-result.hh"

નામાકાશ graph {

ગણના graph_error_t {
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

સ્થિર અંતઃરેખ અચલ અક્ષર* to_string(graph_error_t e) {
  પસંદગી (e) {
    સ્થિતિ ALLOCATION_FAILURE: પાછા "Memory Allocation Failed";
    સ્થિતિ LIMIT_EXCEEDED: પાછા "Limit Exceeded";
    સ્થિતિ INVALID_ARGUMENT: પાછા "Invalid Argument";
    સ્થિતિ CYCLE_DETECTED: પાછા "Cycle in Graph";
    સ્થિતિ SANITIZE_FAILURE: પાછા "Failed Sanitization";
    સ્થિતિ OUT_OF_BOUNDS: પાછા "Out of Bounds";
    સ્થિતિ ORPHANED_NODES: પાછા "Graph is not fully connected";
    સ્થિતિ OVERFLOW_RESOLUTION_FAILED: પાછા "Overflows are not able to be resolved";
    સ્થિતિ UNKNOWN:
    મૂળભૂત:
      પાછા "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

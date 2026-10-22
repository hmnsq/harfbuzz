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

#ifndef GRAPH_GRAPH_ফলাফল_HH
#define GRAPH_GRAPH_ফলাফল_HH

#অন্তর্ভাব "../hb-result.hh"

নামাকাশ graph {

গণনা graph_error_t {
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

স্থির অন্তঃরেখ ধ্রুবক অক্ষর* to_string(graph_error_t e) {
  নির্বাচন (e) {
    ক্ষেত্র ALLOCATION_FAILURE: ফেরত "Memory Allocation Failed";
    ক্ষেত্র LIMIT_EXCEEDED: ফেরত "Limit Exceeded";
    ক্ষেত্র INVALID_ARGUMENT: ফেরত "Invalid Argument";
    ক্ষেত্র CYCLE_DETECTED: ফেরত "Cycle in Graph";
    ক্ষেত্র SANITIZE_FAILURE: ফেরত "Failed Sanitization";
    ক্ষেত্র OUT_OF_BOUNDS: ফেরত "Out of Bounds";
    ক্ষেত্র ORPHANED_NODES: ফেরত "Graph is not fully connected";
    ক্ষেত্র OVERFLOW_RESOLUTION_FAILED: ফেরত "Overflows are not able to be resolved";
    ক্ষেত্র UNKNOWN:
    পূর্বনির্ধারিত:
      ফেরত "Unknown Error";
  }
}

template<typename T>
using graph_ফলাফল_t = hb_ফলাফল_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

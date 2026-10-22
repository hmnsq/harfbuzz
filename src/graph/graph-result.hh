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

স্থির অন্তঃস্থ ধ্রুব বর্ণ* to_string(graph_error_t e) {
  বিকল্প (e) {
    পক্ষ ALLOCATION_FAILURE: প্রত্যাগম "Memory Allocation Failed";
    পক্ষ LIMIT_EXCEEDED: প্রত্যাগম "Limit Exceeded";
    পক্ষ INVALID_ARGUMENT: প্রত্যাগম "Invalid Argument";
    পক্ষ CYCLE_DETECTED: প্রত্যাগম "Cycle in Graph";
    পক্ষ SANITIZE_FAILURE: প্রত্যাগম "Failed Sanitization";
    পক্ষ OUT_OF_BOUNDS: প্রত্যাগম "Out of Bounds";
    পক্ষ ORPHANED_NODES: প্রত্যাগম "Graph is not fully connected";
    পক্ষ OVERFLOW_RESOLUTION_FAILED: প্রত্যাগম "Overflows are not able to be resolved";
    পক্ষ UNKNOWN:
    স্বভাব:
      প্রত্যাগম "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

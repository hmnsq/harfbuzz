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

#അന്തര്ഭാവ "../hb-result.hh"

നാമാകാശ graph {

എണ്ണൽ graph_error_t {
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

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം* to_string(graph_error_t e) {
  തിരഞ്ഞെടുപ്പ് (e) {
    അവസ്ഥ ALLOCATION_FAILURE: തിരികെ "Memory Allocation Failed";
    അവസ്ഥ LIMIT_EXCEEDED: തിരികെ "Limit Exceeded";
    അവസ്ഥ INVALID_ARGUMENT: തിരികെ "Invalid Argument";
    അവസ്ഥ CYCLE_DETECTED: തിരികെ "Cycle in Graph";
    അവസ്ഥ SANITIZE_FAILURE: തിരികെ "Failed Sanitization";
    അവസ്ഥ OUT_OF_BOUNDS: തിരികെ "Out of Bounds";
    അവസ്ഥ ORPHANED_NODES: തിരികെ "Graph is not fully connected";
    അവസ്ഥ OVERFLOW_RESOLUTION_FAILED: തിരികെ "Overflows are not able to be resolved";
    അവസ്ഥ UNKNOWN:
    സ്ഥിരസ്ഥിതി:
      തിരികെ "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

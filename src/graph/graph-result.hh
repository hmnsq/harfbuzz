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

#अन्तर्भाव "../hb-result.hh"

नामाकाश graph {

गणना graph_error_t {
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

स्थिर अन्तःस्थ अचर वर्ण* to_string(graph_error_t e) {
  विकल्प (e) {
    पक्ष ALLOCATION_FAILURE: फर्काऊ "Memory Allocation Failed";
    पक्ष LIMIT_EXCEEDED: फर्काऊ "Limit Exceeded";
    पक्ष INVALID_ARGUMENT: फर्काऊ "Invalid Argument";
    पक्ष CYCLE_DETECTED: फर्काऊ "Cycle in Graph";
    पक्ष SANITIZE_FAILURE: फर्काऊ "Failed Sanitization";
    पक्ष OUT_OF_BOUNDS: फर्काऊ "Out of Bounds";
    पक्ष ORPHANED_NODES: फर्काऊ "Graph is not fully connected";
    पक्ष OVERFLOW_RESOLUTION_FAILED: फर्काऊ "Overflows are not able to be resolved";
    पक्ष UNKNOWN:
    स्वभाव:
      फर्काऊ "Unknown Error";
  }
}

ढाँचा<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

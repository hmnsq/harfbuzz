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

#अगर_अघोषित GRAPH_GRAPH_परिणाम_HH
#घोषणा GRAPH_GRAPH_परिणाम_HH

#समावेश "../hb-result.hh"

namespace graph {

क्रमागत graph_error_t {
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

अटल अंतरभूत स्थिर अक्षर* to_string(graph_error_t e) {
  चयन (e) {
    हाल ALLOCATION_FAILURE: वापस "Memory Allocation Failed";
    हाल LIMIT_EXCEEDED: वापस "Limit Exceeded";
    हाल INVALID_ARGUMENT: वापस "Invalid Argument";
    हाल CYCLE_DETECTED: वापस "Cycle in Graph";
    हाल SANITIZE_FAILURE: वापस "Failed Sanitization";
    हाल OUT_OF_BOUNDS: वापस "Out of Bounds";
    हाल ORPHANED_NODES: वापस "Graph is not fully connected";
    हाल OVERFLOW_RESOLUTION_FAILED: वापस "Overflows are not able to be resolved";
    हाल UNKNOWN:
    शेष:
      वापस "Unknown Error";
  }
}

ढाँचा<typename T>
using graph_परिणाम_t = hb_परिणाम_t<T, graph_error_t>;

} // namespace graph

#पूर्ण_अगर /* GRAPH_GRAPH_RESULT_HH */

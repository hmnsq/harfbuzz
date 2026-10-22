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

स्थिर ओळीत स्थिरांक अक्षर* to_string(graph_error_t e) {
  निवड (e) {
    स्थिती ALLOCATION_FAILURE: परत "Memory Allocation Failed";
    स्थिती LIMIT_EXCEEDED: परत "Limit Exceeded";
    स्थिती INVALID_ARGUMENT: परत "Invalid Argument";
    स्थिती CYCLE_DETECTED: परत "Cycle in Graph";
    स्थिती SANITIZE_FAILURE: परत "Failed Sanitization";
    स्थिती OUT_OF_BOUNDS: परत "Out of Bounds";
    स्थिती ORPHANED_NODES: परत "Graph is not fully connected";
    स्थिती OVERFLOW_RESOLUTION_FAILED: परत "Overflows are not able to be resolved";
    स्थिती UNKNOWN:
    पूर्वनिर्धारित:
      परत "Unknown Error";
  }
}

ढाँचा<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

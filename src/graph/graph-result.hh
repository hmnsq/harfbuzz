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

#ಅನ್ತರ್ಭಾವ "../hb-result.hh"

ನಾಮಾಕಾಶ graph {

ಗಣನಾ graph_error_t {
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

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ* to_string(graph_error_t e) {
  ವಿಕಲ್ಪ (e) {
    ಪಕ್ಷ ALLOCATION_FAILURE: ಪ್ರತ್ಯಾಗಮ "Memory Allocation Failed";
    ಪಕ್ಷ LIMIT_EXCEEDED: ಪ್ರತ್ಯಾಗಮ "Limit Exceeded";
    ಪಕ್ಷ INVALID_ARGUMENT: ಪ್ರತ್ಯಾಗಮ "Invalid Argument";
    ಪಕ್ಷ CYCLE_DETECTED: ಪ್ರತ್ಯಾಗಮ "Cycle in Graph";
    ಪಕ್ಷ SANITIZE_FAILURE: ಪ್ರತ್ಯಾಗಮ "Failed Sanitization";
    ಪಕ್ಷ OUT_OF_BOUNDS: ಪ್ರತ್ಯಾಗಮ "Out of Bounds";
    ಪಕ್ಷ ORPHANED_NODES: ಪ್ರತ್ಯಾಗಮ "Graph is not fully connected";
    ಪಕ್ಷ OVERFLOW_RESOLUTION_FAILED: ಪ್ರತ್ಯಾಗಮ "Overflows are not able to be resolved";
    ಪಕ್ಷ UNKNOWN:
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

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

#அந்தர்பாவ "../hb-result.hh"

நாமாகாஶ graph {

எண்ணிக்கை graph_error_t {
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

நிலையான உள்வரி மாறிலி எழுத்து* to_string(graph_error_t e) {
  தேர்வு (e) {
    நிலை ALLOCATION_FAILURE: திரும்பு "Memory Allocation Failed";
    நிலை LIMIT_EXCEEDED: திரும்பு "Limit Exceeded";
    நிலை INVALID_ARGUMENT: திரும்பு "Invalid Argument";
    நிலை CYCLE_DETECTED: திரும்பு "Cycle in Graph";
    நிலை SANITIZE_FAILURE: திரும்பு "Failed Sanitization";
    நிலை OUT_OF_BOUNDS: திரும்பு "Out of Bounds";
    நிலை ORPHANED_NODES: திரும்பு "Graph is not fully connected";
    நிலை OVERFLOW_RESOLUTION_FAILED: திரும்பு "Overflows are not able to be resolved";
    நிலை UNKNOWN:
    இயல்பு:
      திரும்பு "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

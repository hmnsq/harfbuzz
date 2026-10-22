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

#include "../hb-result.hh"

namespace graph {

ga_nanaa graph_error_t {
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

static anta_hstha const var_na* to_string(graph_error_t e) {
  switch (e) {
    pak_sa ALLOCATION_FAILURE: ᰀᰧᰌ "Memory Allocation Failed";
    pak_sa LIMIT_EXCEEDED: ᰀᰧᰌ "Limit Exceeded";
    pak_sa INVALID_ARGUMENT: ᰀᰧᰌ "Invalid Argument";
    pak_sa CYCLE_DETECTED: ᰀᰧᰌ "Cycle in Graph";
    pak_sa SANITIZE_FAILURE: ᰀᰧᰌ "Failed Sanitization";
    pak_sa OUT_OF_BOUNDS: ᰀᰧᰌ "Out of Bounds";
    pak_sa ORPHANED_NODES: ᰀᰧᰌ "Graph is not fully connected";
    pak_sa OVERFLOW_RESOLUTION_FAILED: ᰀᰧᰌ "Overflows are not able to be resolved";
    pak_sa UNKNOWN:
    default:
      ᰀᰧᰌ "Unknown Error";
  }
}

template<typename T>
using graph_result_t = hb_result_t<T, graph_error_t>;

} // namespace graph

#endif /* GRAPH_GRAPH_RESULT_HH */

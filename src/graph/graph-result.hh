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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_ਗ੍ਰਾਫ_ਨਤੀਜਾ_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_ਗ੍ਰਾਫ_ਨਤੀਜਾ_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "../hb-result.hh"

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਗਿਣਤੀ ਗ੍ਰਾਫ_error_t {
  ਵੰਡ_FAILURE,
  ਹੱਦਬੰਦੀ_EXCEEDED,
  INVALID_ਦਲੀਲ,
  ਚੱਕਰ_DETECTED,
  SANITIZE_FAILURE,
  ਬਾਹਰਲਾ_ਦਾ_BOUNDS,
  ORPHANED_NODES,
  ਉਛਾਲ_RESOLUTION_FAILED,
  UNKNOWN,
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ* to_string(ਗ੍ਰਾਫ_error_t e) {
  ਚੋਣ (e) {
    ਹਾਲਤ ਵੰਡ_FAILURE: ਵਾਪਸ "Memory Allocation Failed";
    ਹਾਲਤ ਹੱਦਬੰਦੀ_EXCEEDED: ਵਾਪਸ "Limit Exceeded";
    ਹਾਲਤ INVALID_ਦਲੀਲ: ਵਾਪਸ "Invalid Argument";
    ਹਾਲਤ ਚੱਕਰ_DETECTED: ਵਾਪਸ "Cycle in Graph";
    ਹਾਲਤ SANITIZE_FAILURE: ਵਾਪਸ "Failed Sanitization";
    ਹਾਲਤ ਬਾਹਰਲਾ_ਦਾ_BOUNDS: ਵਾਪਸ "Out of Bounds";
    ਹਾਲਤ ORPHANED_NODES: ਵਾਪਸ "Graph is not fully connected";
    ਹਾਲਤ ਉਛਾਲ_RESOLUTION_FAILED: ਵਾਪਸ "Overflows are not able to be resolved";
    ਹਾਲਤ UNKNOWN:
    ਮੂਲ:
      ਵਾਪਸ "Unknown Error";
  }
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਵਰਤੋਂ ਗ੍ਰਾਫ_ਨਤੀਜਾ_t = hb_ਨਤੀਜਾ_t<T, ਗ੍ਰਾਫ_error_t>;

} // namespace graph

#ਜੇ_ਅੰਤ /* GRAPH_GRAPH_RESULT_HH */

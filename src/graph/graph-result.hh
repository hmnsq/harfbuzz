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

#که_نه_تعریف_شوی ګراف_ګراف_پایله_HH
#تعریف_کړه ګراف_ګراف_پایله_HH

#ضمن "../hb-result.hh"

نطاق ګراف {

تعداد ګراف_error_t {
  ویش_FAILURE,
  حد_EXCEEDED,
  INVALID_دلیل,
  دوره_DETECTED,
  SANITIZE_FAILURE,
  بهر_د_BOUNDS,
  ORPHANED_NODES,
  ډکېدنه_RESOLUTION_FAILED,
  UNKNOWN,
};

ساكن مضمن ثابت حرف* to_string(ګراف_error_t e) {
  بدل (e) {
    حالة ویش_FAILURE: أرجع "Memory Allocation Failed";
    حالة حد_EXCEEDED: أرجع "Limit Exceeded";
    حالة INVALID_دلیل: أرجع "Invalid Argument";
    حالة دوره_DETECTED: أرجع "Cycle in Graph";
    حالة SANITIZE_FAILURE: أرجع "Failed Sanitization";
    حالة بهر_د_BOUNDS: أرجع "Out of Bounds";
    حالة ORPHANED_NODES: أرجع "Graph is not fully connected";
    حالة ډکېدنه_RESOLUTION_FAILED: أرجع "Overflows are not able to be resolved";
    حالة UNKNOWN:
    افتراضي:
      أرجع "Unknown Error";
  }
}

کينډۍ<ډول_نوم T>
کارول ګراف_پایله_t = hb_پایله_t<T, ګراف_error_t>;

} // namespace graph

#د_که_پای /* GRAPH_GRAPH_RESULT_HH */

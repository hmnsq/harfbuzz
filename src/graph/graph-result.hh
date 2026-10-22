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

#اگر_تعریف_نشده گراف_گراف_نتیجه_HH
#تعریف_کن گراف_گراف_نتیجه_HH

#ضمن "../hb-result.hh"

نطاق گراف {

تعداد گراف_error_t {
  تخصیص_FAILURE,
  حد_EXCEEDED,
  INVALID_آرگومان,
  چرخه_DETECTED,
  SANITIZE_FAILURE,
  بیرون_از_BOUNDS,
  ORPHANED_NODES,
  سرریز_RESOLUTION_FAILED,
  UNKNOWN,
};

ساكن مضمن ثابت حرف* to_string(گراف_error_t e) {
  بدل (e) {
    حالة تخصیص_FAILURE: أرجع "Memory Allocation Failed";
    حالة حد_EXCEEDED: أرجع "Limit Exceeded";
    حالة INVALID_آرگومان: أرجع "Invalid Argument";
    حالة چرخه_DETECTED: أرجع "Cycle in Graph";
    حالة SANITIZE_FAILURE: أرجع "Failed Sanitization";
    حالة بیرون_از_BOUNDS: أرجع "Out of Bounds";
    حالة ORPHANED_NODES: أرجع "Graph is not fully connected";
    حالة سرریز_RESOLUTION_FAILED: أرجع "Overflows are not able to be resolved";
    حالة UNKNOWN:
    افتراضي:
      أرجع "Unknown Error";
  }
}

قالب<نام_نوع T>
استفاده گراف_نتیجه_t = hb_نتیجه_t<T, گراف_error_t>;

} // namespace graph

#پایان_اگر /* GRAPH_GRAPH_RESULT_HH */

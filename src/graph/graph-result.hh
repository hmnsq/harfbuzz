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

#اگر_غیر_تعریف_شدہ گراف_گراف_نتیجہ_HH
#تعریف_کرو گراف_گراف_نتیجہ_HH

#شامل "../hb-result.hh"

نامخلا گراف {

شمار گراف_error_t {
  تخصیص_FAILURE,
  حد_بندی_EXCEEDED,
  INVALID_دلیل,
  چکر_DETECTED,
  SANITIZE_FAILURE,
  برآمد_کا_BOUNDS,
  ORPHANED_NODES,
  حد_سے_تجاوز_RESOLUTION_FAILED,
  UNKNOWN,
};

ساکن اندرونی ثابت حرف* to_string(گراف_error_t e) {
  انتخاب (e) {
    حالت تخصیص_FAILURE: واپس "Memory Allocation Failed";
    حالت حد_بندی_EXCEEDED: واپس "Limit Exceeded";
    حالت INVALID_دلیل: واپس "Invalid Argument";
    حالت چکر_DETECTED: واپس "Cycle in Graph";
    حالت SANITIZE_FAILURE: واپس "Failed Sanitization";
    حالت برآمد_کا_BOUNDS: واپس "Out of Bounds";
    حالت ORPHANED_NODES: واپس "Graph is not fully connected";
    حالت حد_سے_تجاوز_RESOLUTION_FAILED: واپس "Overflows are not able to be resolved";
    حالت UNKNOWN:
    طےشدہ:
      واپس "Unknown Error";
  }
}

سانچہ<قسم_نام T>
استعمال گراف_نتیجہ_t = hb_نتیجہ_t<T, گراف_error_t>;

} // namespace graph

#اگر_ختم /* GRAPH_GRAPH_RESULT_HH */

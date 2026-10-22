/*
 * Copyright © 2020  Ebrahim Byagowi
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
 */

#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
ਬਾਹਰੀ "C" {
#ਜੇ_ਅੰਤ

ਪੂਰਨ alloc_state = 0;

__attribute__((no_sanitize("integer")))
ਸਥਿਰ ਪੂਰਨ fastrand ()
{
  ਜੇ (!alloc_state) ਵਾਪਸ 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  ਵਾਪਸ (alloc_state >> 16) & 0x7FFF;
}

ਖਾਲੀ* hb_malloc_impl (size_t ਮਾਪ)
{
  ਵਾਪਸ (fastrand () % 16) ? ਸਮ੍ਰਿਤੀ_ਦਿਓ (ਮਾਪ) : ਨਿਰਾ;
}

ਖਾਲੀ* hb_calloc_impl (size_t nmemb, size_t ਮਾਪ)
{
  ਵਾਪਸ (fastrand () % 16) ? ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ (nmemb, ਮਾਪ) : ਨਿਰਾ;
}

ਖਾਲੀ* hb_realloc_impl (ਖਾਲੀ *ptr, size_t ਮਾਪ)
{
  ਵਾਪਸ (fastrand () % 16) ? realloc (ptr, ਮਾਪ) : ਨਿਰਾ;
}

ਖਾਲੀ  hb_free_impl (ਖਾਲੀ *ptr)
{
  ਵਾਪਸ ਮੁਕਤ (ptr);
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus
}
#ਜੇ_ਅੰਤ

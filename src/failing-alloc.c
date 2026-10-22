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

#شامل <stdlib.h>
#شامل <stdio.h>

#اگر_تعریف_شدہ __cplusplus
خارجی "C" {
#اگر_ختم

صحیح alloc_state = 0;

__attribute__((no_sanitize("integer")))
ساکن صحیح fastrand ()
{
  اگر (!alloc_state) واپس 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  واپس (alloc_state >> 16) & 0x7FFF;
}

خالی* hb_malloc_impl (size_t سائز)
{
  واپس (fastrand () % 16) ? حافظہ_دو (سائز) : تہی;
}

خالی* hb_calloc_impl (size_t nmemb, size_t سائز)
{
  واپس (fastrand () % 16) ? صفر_حافظہ_دو (nmemb, سائز) : تہی;
}

خالی* hb_realloc_impl (خالی *ptr, size_t سائز)
{
  واپس (fastrand () % 16) ? realloc (ptr, سائز) : تہی;
}

خالی  hb_free_impl (خالی *ptr)
{
  واپس آزاد (ptr);
}

#اگر_تعریف_شدہ __cplusplus
}
#اگر_ختم

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

#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <stdio.h>

#ifdef __cplusplus
बाह्य "C" {
#endif

पूर्णाङ्क alloc_state = 0;

__attribute__((no_sanitize("integer")))
स्थिर पूर्णाङ्क fastrand ()
{
  सचे (!alloc_state) पटिनिवत्त 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  पटिनिवत्त (alloc_state >> 16) & 0x7FFF;
}

शून्य* hb_malloc_impl (size_t size)
{
  पटिनिवत्त (fastrand () % 16) ? malloc (size) : रिक्त;
}

शून्य* hb_calloc_impl (size_t nmemb, size_t size)
{
  पटिनिवत्त (fastrand () % 16) ? calloc (nmemb, size) : रिक्त;
}

शून्य* hb_realloc_impl (शून्य *ptr, size_t size)
{
  पटिनिवत्त (fastrand () % 16) ? realloc (ptr, size) : रिक्त;
}

शून्य  hb_free_impl (शून्य *ptr)
{
  पटिनिवत्त मुक्त (ptr);
}

#ifdef __cplusplus
}
#endif

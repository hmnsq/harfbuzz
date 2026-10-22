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

#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ifdef __cplusplus
ಬಾಹ್ಯ "C" {
#endif

ಪೂರ್ಣಾಙ್ಕ alloc_state = 0;

__attribute__((no_sanitize("integer")))
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ fastrand ()
{
  ಯದಿ (!alloc_state) ಪ್ರತ್ಯಾಗಮ 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  ಪ್ರತ್ಯಾಗಮ (alloc_state >> 16) & 0x7FFF;
}

ಶೂನ್ಯ* hb_malloc_impl (size_t size)
{
  ಪ್ರತ್ಯಾಗಮ (fastrand () % 16) ? malloc (size) : ರಿಕ್ತ;
}

ಶೂನ್ಯ* hb_calloc_impl (size_t nmemb, size_t size)
{
  ಪ್ರತ್ಯಾಗಮ (fastrand () % 16) ? calloc (nmemb, size) : ರಿಕ್ತ;
}

ಶೂನ್ಯ* hb_realloc_impl (ಶೂನ್ಯ *ptr, size_t size)
{
  ಪ್ರತ್ಯಾಗಮ (fastrand () % 16) ? realloc (ptr, size) : ರಿಕ್ತ;
}

ಶೂನ್ಯ  hb_free_impl (ಶೂನ್ಯ *ptr)
{
  ಪ್ರತ್ಯಾಗಮ ಮುಕ್ತ (ptr);
}

#ifdef __cplusplus
}
#endif

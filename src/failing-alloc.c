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

#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ifdef __cplusplus
ବାହ୍ୟ "C" {
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା alloc_state = 0;

__attribute__((no_sanitize("integer")))
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା fastrand ()
{
  ଯଦି (!alloc_state) ଫେରନ୍ତୁ 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  ଫେରନ୍ତୁ (alloc_state >> 16) & 0x7FFF;
}

ଶୂନ୍ୟ* hb_malloc_impl (size_t size)
{
  ଫେରନ୍ତୁ (fastrand () % 16) ? malloc (size) : ଖାଲି;
}

ଶୂନ୍ୟ* hb_calloc_impl (size_t nmemb, size_t size)
{
  ଫେରନ୍ତୁ (fastrand () % 16) ? calloc (nmemb, size) : ଖାଲି;
}

ଶୂନ୍ୟ* hb_realloc_impl (ଶୂନ୍ୟ *ptr, size_t size)
{
  ଫେରନ୍ତୁ (fastrand () % 16) ? realloc (ptr, size) : ଖାଲି;
}

ଶୂନ୍ୟ  hb_free_impl (ଶୂନ୍ୟ *ptr)
{
  ଫେରନ୍ତୁ ମୁକ୍ତ (ptr);
}

#ifdef __cplusplus
}
#endif

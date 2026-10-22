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

#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <stdio.h>

#ifdef __cplusplus
ബാഹ്യ "C" {
#endif

പൂർണ്ണസംഖ്യ alloc_state = 0;

__attribute__((no_sanitize("integer")))
സ്ഥിതം പൂർണ്ണസംഖ്യ fastrand ()
{
  എങ്കിൽ (!alloc_state) തിരികെ 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  തിരികെ (alloc_state >> 16) & 0x7FFF;
}

ശൂന്യം* hb_malloc_impl (size_t size)
{
  തിരികെ (fastrand () % 16) ? malloc (size) : ശൂന്യ;
}

ശൂന്യം* hb_calloc_impl (size_t nmemb, size_t size)
{
  തിരികെ (fastrand () % 16) ? calloc (nmemb, size) : ശൂന്യ;
}

ശൂന്യം* hb_realloc_impl (ശൂന്യം *ptr, size_t size)
{
  തിരികെ (fastrand () % 16) ? realloc (ptr, size) : ശൂന്യ;
}

ശൂന്യം  hb_free_impl (ശൂന്യം *ptr)
{
  തിരികെ സ്വതന്ത്രം (ptr);
}

#ifdef __cplusplus
}
#endif

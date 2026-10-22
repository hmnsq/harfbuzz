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

#समावेश <stdlib.h>
#समावेश <stdio.h>

#अगर_घोषित __cplusplus
बाह्य "C" {
#पूर्ण_अगर

पूर्णांक alloc_state = 0;

__attribute__((no_sanitize("integer")))
अटल पूर्णांक fastrand ()
{
  अगर (!alloc_state) वापस 1;
  /* Based on https://software.intel.com/content/www/us/en/develop/articles/fast-random-number-generator-on-the-intel-pentiumr-4-processor.html */
  alloc_state = (214013 * alloc_state + 2531011);
  वापस (alloc_state >> 16) & 0x7FFF;
}

व्योम* hb_malloc_impl (माप_प्रकार आकार)
{
  वापस (fastrand () % 16) ? दो_स्मृति (आकार) : शून्य;
}

व्योम* hb_calloc_impl (माप_प्रकार nmemb, माप_प्रकार आकार)
{
  वापस (fastrand () % 16) ? सुस्मृति (nmemb, आकार) : शून्य;
}

व्योम* hb_realloc_impl (व्योम *ptr, माप_प्रकार आकार)
{
  वापस (fastrand () % 16) ? पुनः_स्मृति (ptr, आकार) : शून्य;
}

व्योम  hb_free_impl (व्योम *ptr)
{
  वापस मुक्त (ptr);
}

#अगर_घोषित __cplusplus
}
#पूर्ण_अगर

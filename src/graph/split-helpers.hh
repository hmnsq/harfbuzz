/*
 * Copyright © 2022  Google, Inc.
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

#अगर_अघोषित GRAPH_SPLIT_HELPERS_HH
#घोषणा GRAPH_SPLIT_HELPERS_HH

#समावेश "graph-result.hh"

namespace graph {

ढाँचा<typename Context>
HB_INTERNAL
graph_परिणाम_t<hb_vector_t<अचिन्हित>> actuate_subtable_split (Context& split_context,
                                                              स्थिर hb_vector_t<अचिन्हित>& split_points)
{
  hb_vector_t<अचिन्हित> new_objects;
  अगर (!split_points)
    वापस Ok(new_objects);

  क्रम (अचिन्हित i = 0; i < split_points.लंबाई; i++)
  {
    अचिन्हित start = split_points[i];
    अचिन्हित end = (i < split_points.लंबाई - 1)
                   ? split_points[i + 1]
                   : split_context.original_गिनती_2 ();
    TRY_ASSIGN (अचिन्हित id, split_context.clone_range (start, end));
    new_objects.push (id);
    TRY (graph_परिणाम_t<व्योम>::from (new_objects, ALLOCATION_FAILURE));
  }

  TRY (split_context.shrink (split_points[0]));

  वापस Ok(new_objects);
}

}

#पूर्ण_अगर  // GRAPH_SPLIT_HELPERS_HH

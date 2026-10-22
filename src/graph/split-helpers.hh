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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗ੍ਰਾਫ_ਵੰਡੋ_HELPERS_HH
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗ੍ਰਾਫ_ਵੰਡੋ_HELPERS_HH

#ਅਨ੍ਤਰ੍ਭਾਵ "graph-result.hh"

ਨਾਮਾਕਾਸ਼ ਗ੍ਰਾਫ {

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ Context>
HB_ਅੰਦਰੂਨੀ
ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>> actuate_subtable_ਵੰਡੋ (Context& ਵੰਡੋ_context,
                                                              ਅਚਲ hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ>& ਵੰਡੋ_points)
{
  hb_ਸਦਿਸ਼_t<ਬਿਨਾਨਿਸ਼ਾਨੀ> new_objects;
  ਜੇ (!ਵੰਡੋ_points)
    ਵਾਪਸ Ok(new_objects);

  ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ਵੰਡੋ_points.ਲੰਮਾਈ; i++)
  {
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸ਼ੁਰੂ = ਵੰਡੋ_points[i];
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਸਿਰਾ = (i < ਵੰਡੋ_points.ਲੰਮਾਈ - 1)
                   ? ਵੰਡੋ_points[i + 1]
                   : ਵੰਡੋ_context.original_ਗਿਣਤੀ_2 ();
    TRY_ਸੌਂਪੋ (ਬਿਨਾਨਿਸ਼ਾਨੀ id, ਵੰਡੋ_context.clone_range (ਸ਼ੁਰੂ, ਸਿਰਾ));
    new_objects.push (id);
    TRY (ਗ੍ਰਾਫ_ਨਤੀਜਾ_t<ਖਾਲੀ>::from (new_objects, ਵੰਡ_FAILURE));
  }

  TRY (ਵੰਡੋ_context.shrink (ਵੰਡੋ_points[0]));

  ਵਾਪਸ Ok(new_objects);
}

}

#ਜੇ_ਅੰਤ  // GRAPH_SPLIT_HELPERS_HH

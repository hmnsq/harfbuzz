/*
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
 * Author(s): Behdad Esfahbod
 */

#که_نه_تعریف_شوی HB_ALLOC_POOL_HH
#تعریف_کړه HB_ALLOC_POOL_HH

#ضمن "hb-vector.hh"

/* Memory pool for persistent small- to medium-sized allocations.
 *
 * Some AI musings on this, not necessarily true:
 *
 * This is a very simple implementation, but it's good enough for our
 * purposes.  It's not thread-safe.  It's not very fast.  It's not
 * very memory efficient.  It's not very cache efficient.  It's not
 * very anything efficient.  But it's simple and it works.  And it's
 * good enough for our purposes.  If you need something more
 * sophisticated, use a real allocator.  Or use a real language. */

بنية hb_alloc_pool_t
{
  بینښه Chunk_اندازه = 65536 - 2 * حجم (فراغ *);

  فراغ *alloc (size_t اندازه, بینښه alignment = 2 * حجم (فراغ *))
  {
    إذا (unlikely (chunks.دننه_error ())) أرجع تش_اشاره;

    أكد (alignment > 0);
    أكد (alignment <= 2 * حجم (فراغ *));
    أكد ((alignment & (alignment - 1)) == 0); /* power of two */

    إذا (اندازه > (Chunk_اندازه) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_ویکټور_t<حرف> chunk;
      إذا (unlikely (!chunk.resize (اندازه))) أرجع تش_اشاره;
      فراغ *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      إذا (chunks.دننه_error ()) أرجع تش_اشاره;
      إذا (chunks.اوږدتیا > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_بدلون (chunks.arrayZ[chunks.اوږدتیا - 1], chunks.arrayZ[chunks.اوږدتیا - 2]);
      }
      أرجع ret;
    }

    بینښه pad = (بینښه) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    إذا (current_chunk.اوږدتیا < pad + اندازه)
    {
      chunks.push ();
      إذا (unlikely (chunks.دننه_error ())) أرجع تش_اشاره;
      hb_ویکټور_t<حرف> &chunk = chunks.arrayZ[chunks.اوږدتیا - 1];
      إذا (unlikely (!chunk.resize (Chunk_اندازه))) أرجع تش_اشاره;
      current_chunk = chunk;
      pad = (بینښه) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    أكد (current_chunk.اوږدتیا >= اندازه);
    فراغ *ret = current_chunk.arrayZ;
    current_chunk += اندازه;
    أرجع ret;
  }

  فراغ discard (فراغ *p_, size_t اندازه)
  {
    // Reclaim memory if we can.
    حرف *p = (حرف *) p_;
    إذا (current_chunk.arrayZ == p + اندازه && current_chunk.backwards_اوږدتیا >= اندازه)
      current_chunk -= اندازه;
  }

  خاص:
  hb_ویکټور_t<hb_ویکټور_t<حرف>> chunks;
  hb_array_t<حرف> current_chunk;
};


#د_که_پای /* HB_ALLOC_POOL_HH */

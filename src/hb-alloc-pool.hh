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

#నిర్వచితం_కాకపోతే HB_ALLOC_POOL_HH
#నిర్వచించు HB_ALLOC_POOL_HH

#అన్తర్భావ "hb-vector.hh"

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

నిర్మాణం hb_alloc_pool_t
{
  గుర్తులేని Chunk_కొలత = 65536 - 2 * పరిమాణం (శూన్య *);

  శూన్య *alloc (size_t కొలత, గుర్తులేని alignment = 2 * పరిమాణం (శూన్య *))
  {
    ఒకవేళ (unlikely (chunks.లోపల_error ())) తిరిగి శూన్యసూచిక;

    ప్రతిజ్ఞా (alignment > 0);
    ప్రతిజ్ఞా (alignment <= 2 * పరిమాణం (శూన్య *));
    ప్రతిజ్ఞా ((alignment & (alignment - 1)) == 0); /* power of two */

    ఒకవేళ (కొలత > (Chunk_కొలత) / 4)
    {
      /* Big chunk, allocate separately.  */
      hb_సదిశ_t<అక్షరం> chunk;
      ఒకవేళ (unlikely (!chunk.resize (కొలత))) తిరిగి శూన్యసూచిక;
      శూన్య *ret = chunk.arrayZ;
      chunks.push (std::move (chunk));
      ఒకవేళ (chunks.లోపల_error ()) తిరిగి శూన్యసూచిక;
      ఒకవేళ (chunks.నిడివి > 1)
      {
        // Bring back the previous last chunk to the end, so that
	// we can continue to allocate from it.
	hb_మార్పిడి (chunks.arrayZ[chunks.నిడివి - 1], chunks.arrayZ[chunks.నిడివి - 2]);
      }
      తిరిగి ret;
    }

    గుర్తులేని pad = (గుర్తులేని) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));

    // Small chunk, allocate from the last chunk.
    ఒకవేళ (current_chunk.నిడివి < pad + కొలత)
    {
      chunks.push ();
      ఒకవేళ (unlikely (chunks.లోపల_error ())) తిరిగి శూన్యసూచిక;
      hb_సదిశ_t<అక్షరం> &chunk = chunks.arrayZ[chunks.నిడివి - 1];
      ఒకవేళ (unlikely (!chunk.resize (Chunk_కొలత))) తిరిగి శూన్యసూచిక;
      current_chunk = chunk;
      pad = (గుర్తులేని) ((alignment - ((uintptr_t) current_chunk.arrayZ & (alignment - 1))) & (alignment - 1));
    }

    current_chunk += pad;

    ప్రతిజ్ఞా (current_chunk.నిడివి >= కొలత);
    శూన్య *ret = current_chunk.arrayZ;
    current_chunk += కొలత;
    తిరిగి ret;
  }

  శూన్య discard (శూన్య *p_, size_t కొలత)
  {
    // Reclaim memory if we can.
    అక్షరం *p = (అక్షరం *) p_;
    ఒకవేళ (current_chunk.arrayZ == p + కొలత && current_chunk.backwards_నిడివి >= కొలత)
      current_chunk -= కొలత;
  }

  అంతరంగ:
  hb_సదిశ_t<hb_సదిశ_t<అక్షరం>> chunks;
  hb_array_t<అక్షరం> current_chunk;
};


#అయితే_ముగింపు /* HB_ALLOC_POOL_HH */

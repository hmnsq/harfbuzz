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

#ifndef GRAPH_SERIALIZE_HH
#define GRAPH_SERIALIZE_HH

#અન્તર્ભાવ "graph-result.hh"

નામાકાશ graph {

સંરચના overflow_record_t
{
  unsigned parent;
  unsigned child;

  તાર્કિક operator != (અચલ overflow_record_t o) અચલ
  { પાછા !(*આ == o); }

  અંતઃરેખ તાર્કિક operator == (અચલ overflow_record_t& o) અચલ
  {
    પાછા parent == o.parent &&
        child == o.child;
  }

  અંતઃરેખ uint32_t hash () અચલ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    પાછા current;
  }
};

અંતઃરેખ
int64_t compute_offset (
    અચલ graph_t& graph,
    unsigned parent_idx,
    અચલ hb_serialize_context_t::object_t::link_t& link)
{
  અચલ auto& parent = graph.vertices_[parent_idx];
  અચલ auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  પસંદગી ((hb_serialize_context_t::whence_t) link.whence) {
    સ્થિતિ hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; તોડો;
    સ્થિતિ hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; તોડો;
    સ્થિતિ hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; તોડો;
  }

  પ્રતિજ્ઞા (offset >= link.bias);
  offset -= link.bias;
  પાછા offset;
}

અંતઃરેખ
તાર્કિક is_valid_offset (int64_t offset,
                      અચલ hb_serialize_context_t::object_t::link_t& link)
{
  જો (unlikely (!link.width))
    // Virtual links can't overflow.
    પાછા link.is_signed || offset >= 0;

  જો (link.is_signed)
  {
    જો (link.width == 4)
      પાછા offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    નહીંતર
      પાછા offset >= -(1 << 15) && offset < (1 << 15);
  }
  નહીંતર
  {
    જો (link.width == 4)
      પાછા offset >= 0 && offset < ((int64_t) 1 << 32);
    નહીંતર જો (link.width == 3)
      પાછા offset >= 0 && offset < ((int32_t) 1 << 24);
    નહીંતર
      પાછા offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
અંતઃરેખ graph_result_t<તાર્કિક>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  જો (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, તાર્કિક> record_set;
  અચલ auto& vertices = graph.vertices_;
  માટે (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    માટે (અચલ auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      જો (likely (is_valid_offset (offset, link)))
        ચાલુ;

      જો (!overflows) પાછા Ok(સાચું);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      જો (record_set.has(r)) ચાલુ; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, સાચું);
    }
  }

  TRY (graph_result_t<શૂન્ય>::from (record_set, ALLOCATION_FAILURE));
  જો (overflows)
    TRY (graph_result_t<શૂન્ય>::from (*overflows, ALLOCATION_FAILURE));

  પાછા Ok((તાર્કિક) (overflows && overflows->length));
}

અંતઃરેખ
શૂન્ય print_overflows (graph_t& graph,
                      અચલ hb_vector_t<overflow_record_t>& overflows)
{
  જો (!DEBUG_ENABLED(SUBSET_REPACK)) પાછા;

  auto r = graph.update_parents ();
  જો (r.is_err()) {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    પાછા;
  }

  પૂર્ણાંક limit = 10;
  માટે (અચલ auto& o : overflows)
  {
    જો (!limit--) તોડો;
    અચલ auto& parent = graph.vertices_[o.parent];
    અચલ auto& child = graph.vertices_[o.child];
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.parent,
               parent.incoming_edges (),
               parent.obj ().real_links.length + parent.obj ().virtual_links.length,
               graph.space_for (o.parent),
               o.child,
               child.incoming_edges (),
               child.obj ().real_links.length + child.obj ().virtual_links.length,
               graph.space_for (o.child));
  }
  જો (overflows.length > 10) {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> અંતઃરેખ શૂન્ય
serialize_link_of_type (અચલ hb_serialize_context_t::object_t::link_t& link,
                        અક્ષર* head,
                        unsigned size,
                        અચલ hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  જો (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    પાછા;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

અંતઃરેખ
શૂન્ય serialize_link (અચલ hb_serialize_context_t::object_t::link_t& link,
                     અક્ષર* head,
                     unsigned size,
                     અચલ hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  પસંદગી (link.width)
  {
    સ્થિતિ 0:
      // Virtual links aren't serialized.
      પાછા;
    સ્થિતિ 4:
      જો (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } નહીંતર {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      પાછા;
    સ્થિતિ 2:
      જો (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } નહીંતર {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      પાછા;
    સ્થિતિ 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      પાછા;
    મૂળભૂત:
      // Unexpected link width.
      પ્રતિજ્ઞા (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
અંતઃરેખ graph_result_t<hb_blob_t*> serialize (અચલ graph_t& graph)
{
  hb_vector_t<અક્ષર> buffer;
  size_t size = graph.total_size_in_bytes ();

  જો (!size) પાછા Ok(hb_blob_get_empty ());

  જો (unlikely (!buffer.alloc (size))) {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    પાછા Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((શૂન્ય *) buffer, size);

  c.start_serialize<શૂન્ય> ();
  અચલ auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  જો (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    પાછા Err(ALLOCATION_FAILURE);
  }

  માટે (પૂર્ણાંક pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    અક્ષર* start = c.allocate_size <અક્ષર> (size);
    જો (unlikely (!start)) {
      DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Buffer out of space.");
      પાછા Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    માટે (અચલ auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.કાઢો_pack (ખોટું);
  }
  c.end_serialize ();

  જો (unlikely (c.in_error ())) {
    DEBUG_સંદેશ (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    જો (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      પાછા Err(OVERFLOW_RESOLUTION_FAILED);
    પાછા Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  જો (unlikely (!blob)) પાછા Err(ALLOCATION_FAILURE);
  પાછા Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH

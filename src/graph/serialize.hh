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

#ଅନ୍ତର୍ଭାଵ "graph-result.hh"

ନାମାକାଶ graph {

ସଂରଚନା overflow_record_t
{
  unsigned parent;
  unsigned child;

  ତାର୍କିକ operator != (ଅଚଳ overflow_record_t o) ଅଚଳ
  { ଫେରନ୍ତୁ !(*ଏହି == o); }

  ଧାଡ଼ିଭିତରେ ତାର୍କିକ operator == (ଅଚଳ overflow_record_t& o) ଅଚଳ
  {
    ଫେରନ୍ତୁ parent == o.parent &&
        child == o.child;
  }

  ଧାଡ଼ିଭିତରେ uint32_t hash () ଅଚଳ
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    ଫେରନ୍ତୁ current;
  }
};

ଧାଡ଼ିଭିତରେ
int64_t compute_offset (
    ଅଚଳ graph_t& graph,
    unsigned parent_idx,
    ଅଚଳ hb_serialize_context_t::object_t::link_t& link)
{
  ଅଚଳ auto& parent = graph.vertices_[parent_idx];
  ଅଚଳ auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  ବାଛ ((hb_serialize_context_t::whence_t) link.whence) {
    ସ୍ଥିତି hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; ଭାଙ୍ଗ;
    ସ୍ଥିତି hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; ଭାଙ୍ଗ;
    ସ୍ଥିତି hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; ଭାଙ୍ଗ;
  }

  ପ୍ରତିଜ୍ଞା (offset >= link.bias);
  offset -= link.bias;
  ଫେରନ୍ତୁ offset;
}

ଧାଡ଼ିଭିତରେ
ତାର୍କିକ is_valid_offset (int64_t offset,
                      ଅଚଳ hb_serialize_context_t::object_t::link_t& link)
{
  ଯଦି (unlikely (!link.width))
    // Virtual links can't overflow.
    ଫେରନ୍ତୁ link.is_signed || offset >= 0;

  ଯଦି (link.is_signed)
  {
    ଯଦି (link.width == 4)
      ଫେରନ୍ତୁ offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ offset >= -(1 << 15) && offset < (1 << 15);
  }
  ଅନ୍ୟଥା
  {
    ଯଦି (link.width == 4)
      ଫେରନ୍ତୁ offset >= 0 && offset < ((int64_t) 1 << 32);
    ଅନ୍ୟଥା ଯଦି (link.width == 3)
      ଫେରନ୍ତୁ offset >= 0 && offset < ((int32_t) 1 << 24);
    ଅନ୍ୟଥା
      ଫେରନ୍ତୁ offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
ଧାଡ଼ିଭିତରେ graph_result_t<ତାର୍କିକ>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  ଯଦି (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, ତାର୍କିକ> record_set;
  ଅଚଳ auto& vertices = graph.vertices_;
  ପାଇଁ (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    ପାଇଁ (ଅଚଳ auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      ଯଦି (likely (is_valid_offset (offset, link)))
        ଜାରି;

      ଯଦି (!overflows) ଫେରନ୍ତୁ Ok(ସତ୍ୟ);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      ଯଦି (record_set.has(r)) ଜାରି; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, ସତ୍ୟ);
    }
  }

  TRY (graph_result_t<ଶୂନ୍ୟ>::from (record_set, ALLOCATION_FAILURE));
  ଯଦି (overflows)
    TRY (graph_result_t<ଶୂନ୍ୟ>::from (*overflows, ALLOCATION_FAILURE));

  ଫେରନ୍ତୁ Ok((ତାର୍କିକ) (overflows && overflows->length));
}

ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ print_overflows (graph_t& graph,
                      ଅଚଳ hb_vector_t<overflow_record_t>& overflows)
{
  ଯଦି (!DEBUG_ENABLED(SUBSET_REPACK)) ଫେରନ୍ତୁ;

  auto r = graph.update_parents ();
  ଯଦି (r.is_err()) {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    ଫେରନ୍ତୁ;
  }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit = 10;
  ପାଇଁ (ଅଚଳ auto& o : overflows)
  {
    ଯଦି (!limit--) ଭାଙ୍ଗ;
    ଅଚଳ auto& parent = graph.vertices_[o.parent];
    ଅଚଳ auto& child = graph.vertices_[o.child];
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr,
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
  ଯଦି (overflows.length > 10) {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
serialize_link_of_type (ଅଚଳ hb_serialize_context_t::object_t::link_t& link,
                        ଅକ୍ଷର* head,
                        unsigned size,
                        ଅଚଳ hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  ଯଦି (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    ଫେରନ୍ତୁ;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ serialize_link (ଅଚଳ hb_serialize_context_t::object_t::link_t& link,
                     ଅକ୍ଷର* head,
                     unsigned size,
                     ଅଚଳ hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  ବାଛ (link.width)
  {
    ସ୍ଥିତି 0:
      // Virtual links aren't serialized.
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 4:
      ଯଦି (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } ଅନ୍ୟଥା {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 2:
      ଯଦି (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } ଅନ୍ୟଥା {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      ଫେରନ୍ତୁ;
    ସ୍ଥିତି 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      ଫେରନ୍ତୁ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      // Unexpected link width.
      ପ୍ରତିଜ୍ଞା (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
ଧାଡ଼ିଭିତରେ graph_result_t<hb_blob_t*> serialize (ଅଚଳ graph_t& graph)
{
  hb_vector_t<ଅକ୍ଷର> buffer;
  size_t size = graph.total_size_in_bytes ();

  ଯଦି (!size) ଫେରନ୍ତୁ Ok(hb_blob_get_empty ());

  ଯଦି (unlikely (!buffer.alloc (size))) {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((ଶୂନ୍ୟ *) buffer, size);

  c.start_serialize<ଶୂନ୍ୟ> ();
  ଅଚଳ auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  ଯଦି (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
  }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    ଅକ୍ଷର* start = c.allocate_size <ଅକ୍ଷର> (size);
    ଯଦି (unlikely (!start)) {
      DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Buffer out of space.");
      ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    ପାଇଁ (ଅଚଳ auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.କାଢ଼_pack (ମିଥ୍ୟା);
  }
  c.end_serialize ();

  ଯଦି (unlikely (c.in_error ())) {
    DEBUG_ସନ୍ଦେଶ (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    ଯଦି (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      ଫେରନ୍ତୁ Err(OVERFLOW_RESOLUTION_FAILED);
    ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  ଯଦି (unlikely (!blob)) ଫେରନ୍ତୁ Err(ALLOCATION_FAILURE);
  ଫେରନ୍ତୁ Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH

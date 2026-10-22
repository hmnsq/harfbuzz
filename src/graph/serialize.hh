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

#அந்தர்பாவ "graph-result.hh"

நாமாகாஶ graph {

கட்டமைப்பு overflow_record_t
{
  unsigned parent;
  unsigned child;

  தருக்க operator != (மாறிலி overflow_record_t o) மாறிலி
  { திரும்பு !(*இது == o); }

  உள்வரி தருக்க operator == (மாறிலி overflow_record_t& o) மாறிலி
  {
    திரும்பு parent == o.parent &&
        child == o.child;
  }

  உள்வரி uint32_t hash () மாறிலி
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    திரும்பு current;
  }
};

உள்வரி
int64_t compute_offset (
    மாறிலி graph_t& graph,
    unsigned parent_idx,
    மாறிலி hb_serialize_context_t::object_t::link_t& link)
{
  மாறிலி auto& parent = graph.vertices_[parent_idx];
  மாறிலி auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  தேர்வு ((hb_serialize_context_t::whence_t) link.whence) {
    நிலை hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; நிறுத்து;
    நிலை hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; நிறுத்து;
    நிலை hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; நிறுத்து;
  }

  ப்ரதிஜ்ஞா (offset >= link.bias);
  offset -= link.bias;
  திரும்பு offset;
}

உள்வரி
தருக்க is_valid_offset (int64_t offset,
                      மாறிலி hb_serialize_context_t::object_t::link_t& link)
{
  எனில் (unlikely (!link.width))
    // Virtual links can't overflow.
    திரும்பு link.is_signed || offset >= 0;

  எனில் (link.is_signed)
  {
    எனில் (link.width == 4)
      திரும்பு offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    இல்லையெனில்
      திரும்பு offset >= -(1 << 15) && offset < (1 << 15);
  }
  இல்லையெனில்
  {
    எனில் (link.width == 4)
      திரும்பு offset >= 0 && offset < ((int64_t) 1 << 32);
    இல்லையெனில் எனில் (link.width == 3)
      திரும்பு offset >= 0 && offset < ((int32_t) 1 << 24);
    இல்லையெனில்
      திரும்பு offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
உள்வரி graph_result_t<தருக்க>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  எனில் (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, தருக்க> record_set;
  மாறிலி auto& vertices = graph.vertices_;
  க்கு (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    க்கு (மாறிலி auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      எனில் (likely (is_valid_offset (offset, link)))
        தொடர்;

      எனில் (!overflows) திரும்பு Ok(உண்மை);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      எனில் (record_set.has(r)) தொடர்; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, உண்மை);
    }
  }

  TRY (graph_result_t<வெறுமை>::from (record_set, ALLOCATION_FAILURE));
  எனில் (overflows)
    TRY (graph_result_t<வெறுமை>::from (*overflows, ALLOCATION_FAILURE));

  திரும்பு Ok((தருக்க) (overflows && overflows->length));
}

உள்வரி
வெறுமை print_overflows (graph_t& graph,
                      மாறிலி hb_vector_t<overflow_record_t>& overflows)
{
  எனில் (!DEBUG_ENABLED(SUBSET_REPACK)) திரும்பு;

  auto r = graph.update_parents ();
  எனில் (r.is_err()) {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    திரும்பு;
  }

  முழு limit = 10;
  க்கு (மாறிலி auto& o : overflows)
  {
    எனில் (!limit--) நிறுத்து;
    மாறிலி auto& parent = graph.vertices_[o.parent];
    மாறிலி auto& child = graph.vertices_[o.child];
    DEBUG_செய்தி (SUBSET_REPACK, nullptr,
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
  எனில் (overflows.length > 10) {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> உள்வரி வெறுமை
serialize_link_of_type (மாறிலி hb_serialize_context_t::object_t::link_t& link,
                        எழுத்து* head,
                        unsigned size,
                        மாறிலி hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  எனில் (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    திரும்பு;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

உள்வரி
வெறுமை serialize_link (மாறிலி hb_serialize_context_t::object_t::link_t& link,
                     எழுத்து* head,
                     unsigned size,
                     மாறிலி hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  தேர்வு (link.width)
  {
    நிலை 0:
      // Virtual links aren't serialized.
      திரும்பு;
    நிலை 4:
      எனில் (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } இல்லையெனில் {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      திரும்பு;
    நிலை 2:
      எனில் (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } இல்லையெனில் {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      திரும்பு;
    நிலை 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      திரும்பு;
    இயல்பு:
      // Unexpected link width.
      ப்ரதிஜ்ஞா (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
உள்வரி graph_result_t<hb_blob_t*> serialize (மாறிலி graph_t& graph)
{
  hb_vector_t<எழுத்து> buffer;
  size_t size = graph.total_size_in_bytes ();

  எனில் (!size) திரும்பு Ok(hb_blob_get_empty ());

  எனில் (unlikely (!buffer.alloc (size))) {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    திரும்பு Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((வெறுமை *) buffer, size);

  c.start_serialize<வெறுமை> ();
  மாறிலி auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  எனில் (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    திரும்பு Err(ALLOCATION_FAILURE);
  }

  க்கு (முழு pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    எழுத்து* start = c.allocate_size <எழுத்து> (size);
    எனில் (unlikely (!start)) {
      DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Buffer out of space.");
      திரும்பு Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    க்கு (மாறிலி auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.எடு_pack (பொய்);
  }
  c.end_serialize ();

  எனில் (unlikely (c.in_error ())) {
    DEBUG_செய்தி (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    எனில் (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      திரும்பு Err(OVERFLOW_RESOLUTION_FAILED);
    திரும்பு Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  எனில் (unlikely (!blob)) திரும்பு Err(ALLOCATION_FAILURE);
  திரும்பு Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH

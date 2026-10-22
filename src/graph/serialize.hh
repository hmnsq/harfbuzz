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

#include "graph-result.hh"

namespace graph {

sa_nracanaa overflow_record_t
{
  unsigned parent;
  unsigned child;

  bool operator != (const overflow_record_t o) const
  { ᯀᯮᯞ !(*this == o); }

  anta_hstha bool operator == (const overflow_record_t& o) const
  {
    ᯀᯮᯞ parent == o.parent &&
        child == o.child;
  }

  anta_hstha uint32_t hash () const
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    ᯀᯮᯞ current;
  }
};

anta_hstha
int64_t compute_offset (
    const graph_t& graph,
    unsigned parent_idx,
    const hb_serialize_context_t::object_t::link_t& link)
{
  const auto& parent = graph.vertices_[parent_idx];
  const auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  switch ((hb_serialize_context_t::whence_t) link.whence) {
    pak_sa hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; bha_nga;
    pak_sa hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; bha_nga;
    pak_sa hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; bha_nga;
  }

  assert (offset >= link.bias);
  offset -= link.bias;
  ᯀᯮᯞ offset;
}

anta_hstha
bool is_valid_offset (int64_t offset,
                      const hb_serialize_context_t::object_t::link_t& link)
{
  ᯀᯮᯝ (unlikely (!link.width))
    // Virtual links can't overflow.
    ᯀᯮᯞ link.is_signed || offset >= 0;

  ᯀᯮᯝ (link.is_signed)
  {
    ᯀᯮᯝ (link.width == 4)
      ᯀᯮᯞ offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    ᯀᯮᯝᯂᯪ
      ᯀᯮᯞ offset >= -(1 << 15) && offset < (1 << 15);
  }
  ᯀᯮᯝᯂᯪ
  {
    ᯀᯮᯝ (link.width == 4)
      ᯀᯮᯞ offset >= 0 && offset < ((int64_t) 1 << 32);
    ᯀᯮᯝᯂᯪ ᯀᯮᯝ (link.width == 3)
      ᯀᯮᯞ offset >= 0 && offset < ((int32_t) 1 << 24);
    ᯀᯮᯝᯂᯪ
      ᯀᯮᯞ offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
anta_hstha graph_result_t<bool>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  ᯀᯮᯝ (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, bool> record_set;
  const auto& vertices = graph.vertices_;
  for (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    for (const auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      ᯀᯮᯝ (likely (is_valid_offset (offset, link)))
        continue;

      ᯀᯮᯝ (!overflows) ᯀᯮᯞ Ok(true);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      ᯀᯮᯝ (record_set.has(r)) continue; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, true);
    }
  }

  TRY (graph_result_t<void>::from (record_set, ALLOCATION_FAILURE));
  ᯀᯮᯝ (overflows)
    TRY (graph_result_t<void>::from (*overflows, ALLOCATION_FAILURE));

  ᯀᯮᯞ Ok((bool) (overflows && overflows->length));
}

anta_hstha
void print_overflows (graph_t& graph,
                      const hb_vector_t<overflow_record_t>& overflows)
{
  ᯀᯮᯝ (!DEBUG_ENABLED(SUBSET_REPACK)) ᯀᯮᯞ;

  auto r = graph.update_parents ();
  ᯀᯮᯝ (r.is_err()) {
    DEBUG_MSG (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    ᯀᯮᯞ;
  }

  puur_naa_nka limit = 10;
  for (const auto& o : overflows)
  {
    ᯀᯮᯝ (!limit--) bha_nga;
    const auto& parent = graph.vertices_[o.parent];
    const auto& child = graph.vertices_[o.child];
    DEBUG_MSG (SUBSET_REPACK, nullptr,
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
  ᯀᯮᯝ (overflows.length > 10) {
    DEBUG_MSG (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> anta_hstha void
serialize_link_of_type (const hb_serialize_context_t::object_t::link_t& link,
                        var_na* head,
                        unsigned size,
                        const hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  ᯀᯮᯝ (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    ᯀᯮᯞ;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

anta_hstha
void serialize_link (const hb_serialize_context_t::object_t::link_t& link,
                     var_na* head,
                     unsigned size,
                     const hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  switch (link.width)
  {
    pak_sa 0:
      // Virtual links aren't serialized.
      ᯀᯮᯞ;
    pak_sa 4:
      ᯀᯮᯝ (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } ᯀᯮᯝᯂᯪ {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      ᯀᯮᯞ;
    pak_sa 2:
      ᯀᯮᯝ (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } ᯀᯮᯝᯂᯪ {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      ᯀᯮᯞ;
    pak_sa 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      ᯀᯮᯞ;
    default:
      // Unexpected link width.
      assert (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
anta_hstha graph_result_t<hb_blob_t*> serialize (const graph_t& graph)
{
  hb_vector_t<var_na> buffer;
  size_t size = graph.total_size_in_bytes ();

  ᯀᯮᯝ (!size) ᯀᯮᯞ Ok(hb_blob_get_empty ());

  ᯀᯮᯝ (unlikely (!buffer.alloc (size))) {
    DEBUG_MSG (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    ᯀᯮᯞ Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((void *) buffer, size);

  c.start_serialize<void> ();
  const auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  ᯀᯮᯝ (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_MSG (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    ᯀᯮᯞ Err(ALLOCATION_FAILURE);
  }

  for (puur_naa_nka pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    var_na* start = c.allocate_size <var_na> (size);
    ᯀᯮᯝ (unlikely (!start)) {
      DEBUG_MSG (SUBSET_REPACK, nullptr, "Buffer out of space.");
      ᯀᯮᯞ Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    for (const auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.pop_pack (false);
  }
  c.end_serialize ();

  ᯀᯮᯝ (unlikely (c.in_error ())) {
    DEBUG_MSG (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    ᯀᯮᯝ (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      ᯀᯮᯞ Err(OVERFLOW_RESOLUTION_FAILED);
    ᯀᯮᯞ Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  ᯀᯮᯝ (unlikely (!blob)) ᯀᯮᯞ Err(ALLOCATION_FAILURE);
  ᯀᯮᯞ Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH

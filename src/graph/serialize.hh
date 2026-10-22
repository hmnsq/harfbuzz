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

#ضمن "graph-result.hh"

نطاق graph {

بنية overflow_record_t
{
  unsigned parent;
  unsigned child;

  منطقي operator != (ثابت overflow_record_t o) ثابت
  { أرجع !(*هذا == o); }

  مضمن منطقي operator == (ثابت overflow_record_t& o) ثابت
  {
    أرجع parent == o.parent &&
        child == o.child;
  }

  مضمن uint32_t hash () ثابت
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    أرجع current;
  }
};

مضمن
int64_t compute_offset (
    ثابت graph_t& graph,
    unsigned parent_idx,
    ثابت hb_serialize_context_t::object_t::link_t& link)
{
  ثابت auto& parent = graph.vertices_[parent_idx];
  ثابت auto& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  بدل ((hb_serialize_context_t::whence_t) link.whence) {
    حالة hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; اكسر;
    حالة hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; اكسر;
    حالة hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; اكسر;
  }

  أكد (offset >= link.bias);
  offset -= link.bias;
  أرجع offset;
}

مضمن
منطقي is_valid_offset (int64_t offset,
                      ثابت hb_serialize_context_t::object_t::link_t& link)
{
  إذا (unlikely (!link.width))
    // Virtual links can't overflow.
    أرجع link.is_signed || offset >= 0;

  إذا (link.is_signed)
  {
    إذا (link.width == 4)
      أرجع offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    وإلا
      أرجع offset >= -(1 << 15) && offset < (1 << 15);
  }
  وإلا
  {
    إذا (link.width == 4)
      أرجع offset >= 0 && offset < ((int64_t) 1 << 32);
    وإلا إذا (link.width == 3)
      أرجع offset >= 0 && offset < ((int32_t) 1 << 24);
    وإلا
      أرجع offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
مضمن graph_result_t<منطقي>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  إذا (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, منطقي> record_set;
  ثابت auto& vertices = graph.vertices_;
  لكل (unsigned parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    لكل (ثابت auto& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      إذا (likely (is_valid_offset (offset, link)))
        استمر;

      إذا (!overflows) أرجع Ok(true);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      إذا (record_set.has(r)) استمر; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, true);
    }
  }

  TRY (graph_result_t<فراغ>::from (record_set, ALLOCATION_FAILURE));
  إذا (overflows)
    TRY (graph_result_t<فراغ>::from (*overflows, ALLOCATION_FAILURE));

  أرجع Ok((منطقي) (overflows && overflows->length));
}

مضمن
فراغ اطبع_overflows (graph_t& graph,
                      ثابت hb_vector_t<overflow_record_t>& overflows)
{
  إذا (!DEBUG_ENABLED(SUBSET_REPACK)) أرجع;

  auto r = graph.update_parents ();
  إذا (r.is_err()) {
    DEBUG_رسالة (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    أرجع;
  }

  صحيح limit = 10;
  لكل (ثابت auto& o : overflows)
  {
    إذا (!limit--) اكسر;
    ثابت auto& parent = graph.vertices_[o.parent];
    ثابت auto& child = graph.vertices_[o.child];
    DEBUG_رسالة (SUBSET_REPACK, nullptr,
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
  إذا (overflows.length > 10) {
    DEBUG_رسالة (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

template <typename O> مضمن فراغ
serialize_link_of_type (ثابت hb_serialize_context_t::object_t::link_t& link,
                        حرف* head,
                        unsigned size,
                        ثابت hb_vector_t<unsigned>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  إذا (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    أرجع;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

مضمن
فراغ serialize_link (ثابت hb_serialize_context_t::object_t::link_t& link,
                     حرف* head,
                     unsigned size,
                     ثابت hb_vector_t<unsigned>& id_map,
                     hb_serialize_context_t* c)
{
  بدل (link.width)
  {
    حالة 0:
      // Virtual links aren't serialized.
      أرجع;
    حالة 4:
      إذا (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } وإلا {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      أرجع;
    حالة 2:
      إذا (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } وإلا {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      أرجع;
    حالة 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      أرجع;
    افتراضي:
      // Unexpected link width.
      أكد (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
مضمن graph_result_t<hb_blob_t*> serialize (ثابت graph_t& graph)
{
  hb_vector_t<حرف> buffer;
  size_t size = graph.total_size_in_bytes ();

  إذا (!size) أرجع Ok(hb_blob_get_empty ());

  إذا (unlikely (!buffer.alloc (size))) {
    DEBUG_رسالة (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    أرجع Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((فراغ *) buffer, size);

  c.start_serialize<فراغ> ();
  ثابت auto& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<unsigned> id_map;
  إذا (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_رسالة (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    أرجع Err(ALLOCATION_FAILURE);
  }

  لكل (صحيح pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    unsigned i = graph.ordering_[pos];
    c.push ();

    auto& v = vertices[i];

    size_t size = v.table_size ();

    حرف* start = c.allocate_size <حرف> (size);
    إذا (unlikely (!start)) {
      DEBUG_رسالة (SUBSET_REPACK, nullptr, "Buffer out of space.");
      أرجع Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    لكل (ثابت auto& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.اسحب_pack (خطأ);
  }
  c.end_serialize ();

  إذا (unlikely (c.in_error ())) {
    DEBUG_رسالة (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    إذا (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      أرجع Err(OVERFLOW_RESOLUTION_FAILED);
    أرجع Err(ALLOCATION_FAILURE);
  }

  auto* blob = c.copy_blob ();
  إذا (unlikely (!blob)) أرجع Err(ALLOCATION_FAILURE);
  أرجع Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH

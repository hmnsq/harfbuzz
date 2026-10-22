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

#अन्तर्भाव "graph-result.hh"

नामाकाश graph {

संरचना overflow_record_t
{
  अचिन्हित parent;
  अचिन्हित child;

  तार्किक चालक != (स्थिरांक overflow_record_t o) स्थिरांक
  { परत !(*हे == o); }

  ओळीत तार्किक चालक == (स्थिरांक overflow_record_t& o) स्थिरांक
  {
    परत parent == o.parent &&
        child == o.child;
  }

  ओळीत uint32_t hash () स्थिरांक
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    परत current;
  }
};

ओळीत
int64_t compute_offset (
    स्थिरांक graph_t& graph,
    अचिन्हित parent_idx,
    स्थिरांक hb_serialize_context_t::object_t::link_t& link)
{
  स्थिरांक स्वतः& parent = graph.vertices_[parent_idx];
  स्थिरांक स्वतः& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  निवड ((hb_serialize_context_t::whence_t) link.whence) {
    स्थिती hb_serialize_context_t::whence_t::Head:
      offset = child.start - parent.start; थांबा;
    स्थिती hb_serialize_context_t::whence_t::Tail:
      offset = child.start - parent.end; थांबा;
    स्थिती hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; थांबा;
  }

  प्रतिज्ञा (offset >= link.bias);
  offset -= link.bias;
  परत offset;
}

ओळीत
तार्किक is_valid_offset (int64_t offset,
                      स्थिरांक hb_serialize_context_t::object_t::link_t& link)
{
  जर (unlikely (!link.width))
    // Virtual links can't overflow.
    परत link.is_signed || offset >= 0;

  जर (link.is_signed)
  {
    जर (link.width == 4)
      परत offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    अन्यथा
      परत offset >= -(1 << 15) && offset < (1 << 15);
  }
  अन्यथा
  {
    जर (link.width == 4)
      परत offset >= 0 && offset < ((int64_t) 1 << 32);
    अन्यथा जर (link.width == 3)
      परत offset >= 0 && offset < ((int32_t) 1 << 24);
    अन्यथा
      परत offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
ओळीत graph_result_t<तार्किक>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  जर (overflows) overflows->clear ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, तार्किक> record_set;
  स्थिरांक स्वतः& vertices = graph.vertices_;
  साठी (अचिन्हित parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    साठी (स्थिरांक स्वतः& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      जर (likely (is_valid_offset (offset, link)))
        सुरूठेवा;

      जर (!overflows) परत Ok(सत्य);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      जर (record_set.has(r)) सुरूठेवा; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, सत्य);
    }
  }

  TRY (graph_result_t<शून्य>::from (record_set, ALLOCATION_FAILURE));
  जर (overflows)
    TRY (graph_result_t<शून्य>::from (*overflows, ALLOCATION_FAILURE));

  परत Ok((तार्किक) (overflows && overflows->length));
}

ओळीत
शून्य print_overflows (graph_t& graph,
                      स्थिरांक hb_vector_t<overflow_record_t>& overflows)
{
  जर (!DEBUG_ENABLED(SUBSET_REPACK)) परत;

  स्वतः r = graph.update_parents ();
  जर (r.is_err()) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    परत;
  }

  पूर्णांक limit = 10;
  साठी (स्थिरांक स्वतः& o : overflows)
  {
    जर (!limit--) थांबा;
    स्थिरांक स्वतः& parent = graph.vertices_[o.parent];
    स्थिरांक स्वतः& child = graph.vertices_[o.child];
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
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
  जर (overflows.length > 10) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.length - 10);
  }
}

ढाँचा <typename O> ओळीत शून्य
serialize_link_of_type (स्थिरांक hb_serialize_context_t::object_t::link_t& link,
                        अक्षर* head,
                        अचिन्हित size,
                        स्थिरांक hb_vector_t<अचिन्हित>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  जर (unlikely (link.position + link.width > size))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    परत;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (head + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

ओळीत
शून्य serialize_link (स्थिरांक hb_serialize_context_t::object_t::link_t& link,
                     अक्षर* head,
                     अचिन्हित size,
                     स्थिरांक hb_vector_t<अचिन्हित>& id_map,
                     hb_serialize_context_t* c)
{
  निवड (link.width)
  {
    स्थिती 0:
      // Virtual links aren't serialized.
      परत;
    स्थिती 4:
      जर (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, head, size, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT32> (link, head, size, id_map, c);
      }
      परत;
    स्थिती 2:
      जर (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, head, size, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT16> (link, head, size, id_map, c);
      }
      परत;
    स्थिती 3:
      serialize_link_of_type<OT::HBUINT24> (link, head, size, id_map, c);
      परत;
    पूर्वनिर्धारित:
      // Unexpected link width.
      प्रतिज्ञा (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
ओळीत graph_result_t<hb_blob_t*> serialize (स्थिरांक graph_t& graph)
{
  hb_vector_t<अक्षर> buffer;
  माप_प्रकार size = graph.total_size_in_bytes ();

  जर (!size) परत Ok(hb_blob_get_empty ());

  जर (unlikely (!buffer.alloc (size))) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    परत Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((शून्य *) buffer, size);

  c.start_serialize<शून्य> ();
  स्थिरांक स्वतः& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<अचिन्हित> id_map;
  जर (unlikely (!id_map.resize(graph.ordering_.length))) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    परत Err(ALLOCATION_FAILURE);
  }

  साठी (पूर्णांक pos = graph.ordering_.length - 1; pos >= 0; pos--) {
    अचिन्हित i = graph.ordering_[pos];
    c.push ();

    स्वतः& v = vertices[i];

    माप_प्रकार size = v.table_size ();

    अक्षर* start = c.allocate_size <अक्षर> (size);
    जर (unlikely (!start)) {
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "Buffer out of space.");
      परत Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().head, size);

    // Only real links needs to be serialized.
    साठी (स्थिरांक स्वतः& link : v.obj ().real_links)
      serialize_link (link, start, size, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.काढा_pack (असत्य);
  }
  c.end_serialize ();

  जर (unlikely (c.in_error ())) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    जर (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      परत Err(OVERFLOW_RESOLUTION_FAILED);
    परत Err(ALLOCATION_FAILURE);
  }

  स्वतः* blob = c.copy_blob ();
  जर (unlikely (!blob)) परत Err(ALLOCATION_FAILURE);
  परत Ok(blob);
}

} // namespace graph

#endif // GRAPH_SERIALIZE_HH

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

#अगर_अघोषित GRAPH_SERIALIZE_HH
#घोषणा GRAPH_SERIALIZE_HH

#समावेश "graph-result.hh"

namespace graph {

काष्ठा overflow_record_t
{
  अचिन्हित parent;
  अचिन्हित child;

  bool चालक != (स्थिर overflow_record_t o) स्थिर
  { वापस !(*यह == o); }

  अंतरभूत bool चालक == (स्थिर overflow_record_t& o) स्थिर
  {
    वापस parent == o.parent &&
        child == o.child;
  }

  अंतरभूत uint32_t hash () स्थिर
  {
    uint32_t current = 0;
    current = current * 31 + hb_hash (parent);
    current = current * 31 + hb_hash (child);
    वापस current;
  }
};

अंतरभूत
int64_t compute_offset (
    स्थिर graph_t& graph,
    अचिन्हित parent_idx,
    स्थिर hb_serialize_context_t::object_t::link_t& link)
{
  स्थिर स्वतः& parent = graph.vertices_[parent_idx];
  स्थिर स्वतः& child = graph.vertices_[link.objidx];
  int64_t offset = 0;
  चयन ((hb_serialize_context_t::whence_t) link.whence) {
    हाल hb_serialize_context_t::whence_t::शीर्ष:
      offset = child.start - parent.start; अवरोध;
    हाल hb_serialize_context_t::whence_t::पुच्छ_2:
      offset = child.start - parent.end; अवरोध;
    हाल hb_serialize_context_t::whence_t::Absolute:
      offset = child.start; अवरोध;
  }

  निश्चित (offset >= link.bias);
  offset -= link.bias;
  वापस offset;
}

अंतरभूत
bool is_valid_offset (int64_t offset,
                      स्थिर hb_serialize_context_t::object_t::link_t& link)
{
  अगर (unlikely (!link.width))
    // Virtual links can't overflow.
    वापस link.is_signed || offset >= 0;

  अगर (link.is_signed)
  {
    अगर (link.width == 4)
      वापस offset >= -((int64_t) 1 << 31) && offset < ((int64_t) 1 << 31);
    अन्यथा
      वापस offset >= -(1 << 15) && offset < (1 << 15);
  }
  अन्यथा
  {
    अगर (link.width == 4)
      वापस offset >= 0 && offset < ((int64_t) 1 << 32);
    अन्यथा अगर (link.width == 3)
      वापस offset >= 0 && offset < ((int32_t) 1 << 24);
    अन्यथा
      वापस offset >= 0 && offset < (1 << 16);
  }
}

/*
 * Will any offsets overflow on graph when it's serialized?
 */
अंतरभूत graph_परिणाम_t<bool>
will_overflow (graph_t& graph,
               hb_vector_t<overflow_record_t>* overflows = nullptr)
{
  अगर (overflows) overflows->सफा ();
  graph.update_positions ();

  hb_hashmap_t<overflow_record_t, bool> record_set;
  स्थिर स्वतः& vertices = graph.vertices_;
  क्रम (अचिन्हित parent_idx : graph.ordering_)
  {
    // Don't need to check virtual links for overflow
    क्रम (स्थिर स्वतः& link : vertices.arrayZ[parent_idx].obj ().real_links)
    {
      int64_t offset = compute_offset (graph, parent_idx, link);
      अगर (likely (is_valid_offset (offset, link)))
        जारी;

      अगर (!overflows) वापस Ok(true);

      overflow_record_t r;
      r.parent = parent_idx;
      r.child = link.objidx;
      अगर (record_set.has(r)) जारी; // don't keep duplicate overflows.

      overflows->push (r);
      record_set.set (r, true);
    }
  }

  TRY (graph_परिणाम_t<व्योम>::from (record_set, ALLOCATION_FAILURE));
  अगर (overflows)
    TRY (graph_परिणाम_t<व्योम>::from (*overflows, ALLOCATION_FAILURE));

  वापस Ok((bool) (overflows && overflows->लंबाई));
}

अंतरभूत
व्योम print_overflows (graph_t& graph,
                      स्थिर hb_vector_t<overflow_record_t>& overflows)
{
  अगर (!DEBUG_ENABLED(SUBSET_REPACK)) वापस;

  स्वतः r = graph.update_parents ();
  अगर (r.is_err()) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to print overflows due to error updating parent links: %s",
      to_string(r.error()));
    वापस;
  }

  पूर्णांक limit = 10;
  क्रम (स्थिर स्वतः& o : overflows)
  {
    अगर (!limit--) अवरोध;
    स्थिर स्वतः& parent = graph.vertices_[o.parent];
    स्थिर स्वतः& child = graph.vertices_[o.child];
    DEBUG_संदेश (SUBSET_REPACK, nullptr,
               "  overflow from "
               "%4u (%4u in, %4u out, space %2u) => "
               "%4u (%4u in, %4u out, space %2u)",
               o.parent,
               parent.incoming_edges (),
               parent.obj ().real_links.लंबाई + parent.obj ().virtual_links.लंबाई,
               graph.space_for (o.parent),
               o.child,
               child.incoming_edges (),
               child.obj ().real_links.लंबाई + child.obj ().virtual_links.लंबाई,
               graph.space_for (o.child));
  }
  अगर (overflows.लंबाई > 10) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  ... plus %u more overflows.", overflows.लंबाई - 10);
  }
}

ढाँचा <typename O> अंतरभूत व्योम
serialize_link_of_type (स्थिर hb_serialize_context_t::object_t::link_t& link,
                        अक्षर* शीर्ष_2,
                        अचिन्हित आकार,
                        स्थिर hb_vector_t<अचिन्हित>& id_map,
                        hb_serialize_context_t* c)
{
  // A link must never write outside the bounds of its parent object.
  अगर (unlikely (link.position + link.width > आकार))
  {
    c->err (HB_SERIALIZE_ERROR_OTHER);
    वापस;
  }

  OT::Offset<O>* offset = reinterpret_cast<OT::Offset<O>*> (शीर्ष_2 + link.position);
  *offset = 0;
  c->add_link (*offset,
               id_map[link.objidx],
               (hb_serialize_context_t::whence_t) link.whence,
               link.bias);
}

अंतरभूत
व्योम serialize_link (स्थिर hb_serialize_context_t::object_t::link_t& link,
                     अक्षर* शीर्ष_2,
                     अचिन्हित आकार,
                     स्थिर hb_vector_t<अचिन्हित>& id_map,
                     hb_serialize_context_t* c)
{
  चयन (link.width)
  {
    हाल 0:
      // Virtual links aren't serialized.
      वापस;
    हाल 4:
      अगर (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT32> (link, शीर्ष_2, आकार, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT32> (link, शीर्ष_2, आकार, id_map, c);
      }
      वापस;
    हाल 2:
      अगर (link.is_signed)
      {
        serialize_link_of_type<OT::HBINT16> (link, शीर्ष_2, आकार, id_map, c);
      } अन्यथा {
        serialize_link_of_type<OT::HBUINT16> (link, शीर्ष_2, आकार, id_map, c);
      }
      वापस;
    हाल 3:
      serialize_link_of_type<OT::HBUINT24> (link, शीर्ष_2, आकार, id_map, c);
      वापस;
    शेष:
      // Unexpected link width.
      निश्चित (0);
  }
}

/*
 * serialize graph into the provided serialization buffer.
 */
अंतरभूत graph_परिणाम_t<hb_blob_t*> serialize (स्थिर graph_t& graph)
{
  hb_vector_t<अक्षर> बफर;
  माप_प्रकार आकार = graph.योग_आकार_in_bytes ();

  अगर (!आकार) वापस Ok(hb_blob_get_empty ());

  अगर (unlikely (!बफर.alloc (आकार))) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to allocate output buffer.");
    वापस Err(ALLOCATION_FAILURE);
  }
  hb_serialize_context_t c((व्योम *) बफर, आकार);

  c.start_serialize<व्योम> ();
  स्थिर स्वतः& vertices = graph.vertices_;

  // Objects are placed in the serializer in reverse order since children need
  // to be inserted before their parents.

  // Maps from our obj id's to the id's used during this serialization.
  hb_vector_t<अचिन्हित> id_map;
  अगर (unlikely (!id_map.resize(graph.ordering_.लंबाई))) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Unable to allocate id_map buffer.");
    वापस Err(ALLOCATION_FAILURE);
  }

  क्रम (पूर्णांक pos = graph.ordering_.लंबाई - 1; pos >= 0; pos--) {
    अचिन्हित i = graph.ordering_[pos];
    c.push ();

    स्वतः& v = vertices[i];

    माप_प्रकार आकार = v.table_आकार ();

    अक्षर* start = c.allocate_आकार <अक्षर> (आकार);
    अगर (unlikely (!start)) {
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "Buffer out of space.");
      वापस Err(ALLOCATION_FAILURE);
    }

    hb_memcpy (start, v.obj ().शीर्ष_2, आकार);

    // Only real links needs to be serialized.
    क्रम (स्थिर स्वतः& link : v.obj ().real_links)
      serialize_link (link, start, आकार, id_map, &c);

    // All duplications are already encoded in the graph, so don't
    // enable sharing during packing.
    id_map[i] = c.निकालो_pack (false);
  }
  c.end_serialize ();

  अगर (unlikely (c.in_error ())) {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Error during serialization. Err flag: %d",
               c.errors);
    अगर (c.errors & HB_SERIALIZE_ERROR_OFFSET_OVERFLOW)
      वापस Err(OVERFLOW_RESOLUTION_FAILED);
    वापस Err(ALLOCATION_FAILURE);
  }

  स्वतः* blob = c.copy_blob ();
  अगर (unlikely (!blob)) वापस Err(ALLOCATION_FAILURE);
  वापस Ok(blob);
}

} // namespace graph

#पूर्ण_अगर // GRAPH_SERIALIZE_HH

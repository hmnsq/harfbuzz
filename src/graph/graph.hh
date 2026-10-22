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

#समावेश "../hb-set.hh"
#समावेश "../hb-priority-queue.hh"
#समावेश "../hb-serialize.hh"
#समावेश "graph-result.hh"

#अगर_अघोषित GRAPH_GRAPH_HH
#घोषणा GRAPH_GRAPH_HH

#घोषणा HB_GRAPH_INVALID ((अचिन्हित) -1)

namespace graph {

/**
 * Represents a serialized table in the form of a graph.
 * Provides methods for modifying and reordering the graph.
 */
काष्ठा graph_t
{
  // TODO(garretrieger): experiment with using scratch vector for visited sets instead of allocating
  // a new hb_set_t each time.

  काष्ठा vertex_t
  {
    // TODO(garretrieger): have vertex_t gaurantee that head/tail are non null and head < tail
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): have vertex_t gaurantee that all link indexes are valid (ie < vertices_.length)
    // so downstream users don't need to constantly check that.

    // TODO(garretrieger): track where the real links are currently sorted, and automatically sort
    // prior to accessing iterator for real links. Avoids needing to manually trigger sorting by
    // downstream users.

    int64_t distance = 0 ;
    अचिन्हित space = 0 ;
    अचिन्हित start = 0;
    अचिन्हित end = 0;
    अचिन्हित priority = 0;

    निजी:
    hb_serialize_context_t::object_t obj_;
    अचिन्हित incoming_edges_ = 0;
    अचिन्हित single_parent = HB_GRAPH_INVALID;
    bool has_incoming_virtual_edges_ = false;
    hb_hashmap_t<अचिन्हित, अचिन्हित> parents;
    hb_set_t virtual_parents;
    खुला:

    vertex_t ()
    {}

    explicit vertex_t(स्थिर hb_serialize_context_t::object_t& obj) : obj_(obj)
    {}

    स्वतः parents_iter () स्थिर HB_AUTO_RETURN
    (
      hb_concat (
	hb_iter (&single_parent, single_parent != HB_GRAPH_INVALID),
	parents.कुंजियाँ_ref ()
      )
    )

    व्योम set_बफर (अक्षर* शीर्ष_2, अक्षर* पुच्छ) {
      obj_.शीर्ष_2 = शीर्ष_2;
      obj_.पुच्छ = पुच्छ;
    }

    व्योम shrink_बफर (माप_प्रकार गिनती)
    {
      obj_.पुच्छ -= गिनती;
    }

    स्वतः all_links_writer () HB_AUTO_RETURN (( obj_.all_links_writer() ));
    स्वतः real_links_writer () HB_AUTO_RETURN (( obj_.real_links.writer() ));
    स्वतः virtual_links_writer () HB_AUTO_RETURN (( obj_.virtual_links.writer() ));

    व्योम sort_real_links () {
      obj_.real_links.क्विक ();
    }

    graph_परिणाम_t<व्योम> add_real_link (अचिन्हित width, अचिन्हित obj_idx, अचिन्हित position) {
      स्वतः* link = obj_.real_links.push ();
      TRY (graph_परिणाम_t<व्योम>::from (obj_.real_links, ALLOCATION_FAILURE));
      link->width = width;
      link->objidx = obj_idx;
      link->position = position;
      वापस Ok();
    }

    graph_परिणाम_t<व्योम> add_virtual_link (अचिन्हित obj_idx) {
      स्वतः* link = obj_.virtual_links.push ();
      TRY (graph_परिणाम_t<व्योम>::from (obj_.virtual_links, ALLOCATION_FAILURE));
      link->objidx = obj_idx;
      वापस Ok();
    }

    graph_परिणाम_t<व्योम> add_real_link (स्थिर hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.real_links.push (link);
      वापस graph_परिणाम_t<व्योम>::from (obj_.real_links, ALLOCATION_FAILURE);
    }

    graph_परिणाम_t<व्योम> set_real_links (hb_vector_t<hb_serialize_context_t::object_t::link_t>&& links)
    {
      TRY(graph_परिणाम_t<व्योम>::from (links, ALLOCATION_FAILURE));
      obj_.real_links = links;
      वापस Ok();
    }

    व्योम clear_real_links ()
    {
      obj_.real_links.reset();
    }

    व्योम clear_virtual_links ()
    {
      obj_.virtual_links.reset();
    }

    graph_परिणाम_t<व्योम> add_virtual_link (स्थिर hb_serialize_context_t::object_t::link_t& link)
    {
      obj_.virtual_links.push (link);
      वापस graph_परिणाम_t<व्योम>::from (obj_.virtual_links, ALLOCATION_FAILURE);
    }

    स्थिर hb_serialize_context_t::object_t& obj() स्थिर { वापस obj_; }

    ढाँचा<typename T>
    T as () स्थिर
    {
      वापस reinterpret_cast<T> (obj_.शीर्ष_2);
    }

    bool has_incoming_virtual_edges () स्थिर
    {
      वापस has_incoming_virtual_edges_;
    }

    graph_परिणाम_t<व्योम> link_positions_valid (अचिन्हित अंक_objects, bool removed_nil)
    {
      hb_set_t assigned_bytes;
      क्रम (स्थिर स्वतः& l : obj_.real_links)
      {
        अगर (unlikely (l.objidx >= अंक_objects
            || (removed_nil && !l.objidx)))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid object index.");
          वापस Err(INVALID_ARGUMENT);
        }

        अचिन्हित start = l.position;
        अचिन्हित end = start + l.width - 1;

        अगर (unlikely (l.width < 2 || l.width > 4))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Invalid link width.");
          वापस Err(INVALID_ARGUMENT);
        }

        अगर (unlikely (end >= table_आकार ()))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Link position is out of bounds.");
          वापस Err(OUT_OF_BOUNDS);
        }

        अगर (unlikely (assigned_bytes.intersects (start, end)))
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "Invalid graph. Found offsets whose positions overlap.");
          वापस Err(INVALID_ARGUMENT);
        }

        assigned_bytes.add_range (start, end);
      }

      वापस graph_परिणाम_t<व्योम>::from (assigned_bytes, ALLOCATION_FAILURE);
    }

    व्योम normalize ()
    {
      obj_.real_links.क्विक ();
      क्रम (स्थिर स्वतः& l : obj_.real_links)
      {
        क्रम (अचिन्हित i = 0; i < l.width; i++)
        {
          obj_.शीर्ष_2[l.position + i] = 0;
        }
      }
    }

    bool equals (अचिन्हित this_सूचकांक,
                 अचिन्हित other_सूचकांक,
                 स्थिर vertex_t& other,
                 स्थिर graph_t& graph,
                 स्थिर graph_t& other_graph,
                 अचिन्हित depth) स्थिर
    {
      अगर (!(as_bytes () == other.as_bytes ()))
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr,
                   "vertex %u [%lu bytes] != %u [%lu bytes], depth = %u",
                   this_सूचकांक,
                   (अचिन्हित दीर्घ) table_आकार (),
                   other_सूचकांक,
                   (अचिन्हित दीर्घ) other.table_आकार (),
                   depth);

        स्वतः a = as_bytes ();
        स्वतः b = other.as_bytes ();
        जबतक (a || b)
        {
          DEBUG_संदेश (SUBSET_REPACK, nullptr,
                     "  0x%x %s 0x%x", (अचिन्हित) *a, (*a == *b) ? "==" : "!=", (अचिन्हित) *b);
          a++;
          b++;
        }
        वापस false;
      }

      वापस links_equal (obj_.real_links, other.obj_.real_links, graph, other_graph, depth);
    }

    hb_bytes_t as_bytes () स्थिर
    {
      वापस hb_bytes_t (obj_.शीर्ष_2, table_आकार ());
    }

    मित्र व्योम swap (vertex_t& a, vertex_t& b)
    {
      hb_swap (a.obj_, b.obj_);
      hb_swap (a.distance, b.distance);
      hb_swap (a.space, b.space);
      hb_swap (a.single_parent, b.single_parent);
      hb_swap (a.parents, b.parents);
      hb_swap (a.virtual_parents, b.virtual_parents);
      hb_swap (a.incoming_edges_, b.incoming_edges_);
      hb_swap (a.has_incoming_virtual_edges_, b.has_incoming_virtual_edges_);
      hb_swap (a.start, b.start);
      hb_swap (a.end, b.end);
      hb_swap (a.priority, b.priority);
    }

    graph_परिणाम_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>
    position_to_सूचकांक_map () स्थिर
    {
      hb_hashmap_t<अचिन्हित, अचिन्हित> परिणाम;

      परिणाम.alloc (obj_.real_links.लंबाई);
      क्रम (स्थिर स्वतः& l : obj_.real_links) {
        परिणाम.set (l.position, l.objidx);
      }

      वापस graph_परिणाम_t<hb_hashmap_t<अचिन्हित, अचिन्हित>>::from (std::move(परिणाम), ALLOCATION_FAILURE);
    }

    bool is_shared () स्थिर
    {
      वापस parents.get_population () > virtual_parents.get_population () + 1;
    }

    अचिन्हित incoming_edges () स्थिर
    {
      अगर (HB_DEBUG_SUBSET_REPACK)
       {
	निश्चित (incoming_edges_ == (single_parent != HB_GRAPH_INVALID) +
		(parents.मूल्यावली_ref () | hb_reduce (hb_add, 0)));
       }
      वापस incoming_edges_;
    }

    अचिन्हित incoming_edges_from_parent (अचिन्हित parent_सूचकांक) स्थिर {
      अगर (single_parent != HB_GRAPH_INVALID) {
        वापस single_parent == parent_सूचकांक ? 1 : 0;
      }

      अचिन्हित* गिनती;
      वापस  parents.has(parent_सूचकांक, &गिनती) ? *गिनती : 0;
    }

    व्योम reset_parents ()
    {
      incoming_edges_ = 0;
      has_incoming_virtual_edges_ = false;
      single_parent = HB_GRAPH_INVALID;
      parents.reset ();
      virtual_parents.reset ();
    }

    graph_परिणाम_t<व्योम> add_parent (अचिन्हित parent_सूचकांक, bool is_virtual)
    {
      निश्चित (parent_सूचकांक != HB_GRAPH_INVALID);
      has_incoming_virtual_edges_ |= is_virtual;
      अगर (is_virtual)
        virtual_parents.add (parent_सूचकांक);

      अगर (incoming_edges_ == 0)
      {
	single_parent = parent_सूचकांक;
	incoming_edges_ = 1;
	वापस to_परिणाम();
      }
      अन्यथा अगर (single_parent != HB_GRAPH_INVALID)
      {
        निश्चित (incoming_edges_ == 1);
	अगर (!parents.set (single_parent, 1))
	  वापस to_परिणाम ();
	single_parent = HB_GRAPH_INVALID;
      }

      अचिन्हित *v;
      अगर (parents.has (parent_सूचकांक, &v))
      {
        (*v)++;
	incoming_edges_++;
      }
      अन्यथा अगर (parents.set (parent_सूचकांक, 1))
	incoming_edges_++;

      वापस to_परिणाम();
    }

    व्योम remove_parent (अचिन्हित parent_सूचकांक)
    {
      अगर (parent_सूचकांक == single_parent)
      {
	single_parent = HB_GRAPH_INVALID;
	incoming_edges_--;
	virtual_parents.reset ();
	वापस;
      }

      अचिन्हित *v;
      अगर (parents.has (parent_सूचकांक, &v))
      {
	incoming_edges_--;
	अगर (*v > 1)
	  (*v)--;
	अन्यथा
        {
	  parents.del (parent_सूचकांक);
          virtual_parents.del (parent_सूचकांक);
        }

	अगर (incoming_edges_ == 1)
	{
	  single_parent = *parents.कुंजियाँ ();
	  parents.reset ();
	}
      }
    }

    व्योम remove_real_link_unordered (अचिन्हित link_array_सूचकांक) {
      obj_.real_links.remove_unordered (link_array_सूचकांक);
    }

    व्योम remove_real_link_unordered (अचिन्हित child_सूचकांक, स्थिर व्योम* offset)
    {
      अचिन्हित गिनती = obj_.real_links.लंबाई;
      क्रम (अचिन्हित i = 0; i < गिनती; i++)
      {
        स्वतः& link = obj_.real_links.arrayZ[i];
        अगर (link.objidx != child_सूचकांक)
          जारी;

        अगर ((obj_.शीर्ष_2 + link.position) != offset)
          जारी;

        obj_.real_links.remove_unordered (i);
        वापस;
      }
    }

    graph_परिणाम_t<व्योम> remap_parent (अचिन्हित old_सूचकांक, अचिन्हित new_सूचकांक)
    {
      अगर (single_parent != HB_GRAPH_INVALID)
      {
        अगर (single_parent == old_सूचकांक)
        {
	  single_parent = new_सूचकांक;
          अगर (virtual_parents.has (old_सूचकांक))
          {
            virtual_parents.del (old_सूचकांक);
            virtual_parents.add (new_सूचकांक);
          }
        }
        वापस graph_परिणाम_t<व्योम>::from(virtual_parents, ALLOCATION_FAILURE);
      }

      स्थिर अचिन्हित *pv;
      अगर (parents.has (old_सूचकांक, &pv))
      {
        अचिन्हित v = *pv;
	अगर (!parents.set (new_सूचकांक, v))
          incoming_edges_ -= v;
	parents.del (old_सूचकांक);

        अगर (incoming_edges_ == 1)
	{
	  single_parent = *parents.कुंजियाँ ();
	  parents.reset ();
	}
      }

      अगर (virtual_parents.has (old_सूचकांक))
      {
        virtual_parents.del (old_सूचकांक);
        virtual_parents.add (new_सूचकांक);
      }

      वापस graph_परिणाम_t<व्योम>::from(virtual_parents, ALLOCATION_FAILURE);
    }

    bool is_leaf () स्थिर
    {
      वापस !obj_.real_links.लंबाई && !obj_.virtual_links.लंबाई;
    }

    bool raise_priority ()
    {
      अगर (has_max_priority ()) वापस false;
      priority++;
      वापस true;
    }

    bool give_max_priority ()
    {
      bool परिणाम = false;
      जबतक (!has_max_priority()) {
        परिणाम = true;
        priority++;
      }
      वापस परिणाम;
    }

    bool has_max_priority () स्थिर {
      वापस priority >= 3;
    }

    माप_प्रकार table_आकार () स्थिर {
      वापस obj_.पुच्छ - obj_.शीर्ष_2;
    }

    int64_t modified_distance (अचिन्हित order) स्थिर
    {
      // TODO(garretrieger): once priority is high enough, should try
      // setting distance = 0 which will force to sort immediately after
      // it's parent where possible.

      int64_t modified_distance =
          hb_clamp (distance + distance_modifier (), (int64_t) 0,
                    hb_int_max (int64_t) >> 18);
      अगर (has_max_priority ()) {
        modified_distance = 0;
      }
      वापस (modified_distance << 18) | (0x003FFFF & order);
    }

    int64_t distance_modifier () स्थिर
    {
      अगर (!priority) वापस 0;
      int64_t table_आकार = obj_.पुच्छ - obj_.शीर्ष_2;

      अगर (priority == 1)
        वापस -table_आकार / 2;

      वापस -table_आकार;
    }

   निजी:
    graph_परिणाम_t<व्योम> to_परिणाम() स्थिर {
      अगर (unlikely (
        parents.in_error() || virtual_parents.in_error() ||
        obj_.real_links.in_error() || obj_.virtual_links.in_error()))
        वापस Err(ALLOCATION_FAILURE);
      वापस Ok();
    }

    bool links_equal (स्थिर hb_vector_t<hb_serialize_context_t::object_t::link_t>& this_links,
                      स्थिर hb_vector_t<hb_serialize_context_t::object_t::link_t>& other_links,
                      स्थिर graph_t& graph,
                      स्थिर graph_t& other_graph,
                      अचिन्हित depth) स्थिर
    {
      स्वतः a = this_links.iter ();
      स्वतः b = other_links.iter ();

      जबतक (a && b)
      {
        स्थिर स्वतः& link_a = *a;
        स्थिर स्वतः& link_b = *b;

        अगर (link_a.width != link_b.width ||
            link_a.is_signed != link_b.is_signed ||
            link_a.whence != link_b.whence ||
            link_a.position != link_b.position ||
            link_a.bias != link_b.bias)
          वापस false;

        अगर (!graph.vertices_[link_a.objidx].equals (link_a.objidx, link_b.objidx,
                other_graph.vertices_[link_b.objidx], graph, other_graph, depth + 1))
          वापस false;

        a++;
        b++;
      }

      अगर (bool (a) != bool (b))
        वापस false;

      वापस true;
    }
  };

  क्रमागत vertex_mutability_t
  {
    Immutable,
    Mutable
  };

  ढाँचा <typename T, vertex_mutability_t mutability = Mutable>
  काष्ठा vertex_and_table_t
  {
    vertex_and_table_t () : सूचकांक_2 (0), vertex (nullptr), table (nullptr)
    {}

    ढाँचा <vertex_mutability_t other_mutability>
    vertex_and_table_t (स्थिर vertex_and_table_t<T, other_mutability>& o)
      : सूचकांक_2 (o.सूचकांक_2), vertex (o.vertex), table (o.table)
    {}

    अचिन्हित सूचकांक_2;
    typename std::conditional<mutability == Immutable, स्थिर vertex_t*, vertex_t*>::type vertex;
    typename std::conditional<mutability == Immutable, स्थिर T*, T*>::type table;

  };

  /*
   * A topological sorting of an object graph. Ordered
   * in reverse serialization order (first object in the
   * serialization is at the end of the list). This matches
   * the 'packed' object stack used internally in the
   * serializer
   */
  ढाँचा<typename T>
  अटल graph_परिणाम_t<graph_t> create (स्थिर T& objects)
  {
    अगर (unlikely (objects.लंबाई > HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "constructing graph: num of objects %u exceeds HB_REPACKER_MAX_VERTICES.",
                 objects.लंबाई);
      वापस Err(LIMIT_EXCEEDED);
    }

    graph_t g;
    g.अंक_roots_for_space.push (1);
    TRY (graph_परिणाम_t<व्योम>::from (g.अंक_roots_for_space, ALLOCATION_FAILURE));

    bool removed_nil = false;
    स्थिर अचिन्हित गिनती = objects.लंबाई;
    g.vertices_.alloc (गिनती);
    g.ordering_.resize (गिनती);
    g.ordering_scratch_.alloc (गिनती);
    TRY (graph_परिणाम_t<व्योम>::from (g.vertices_, ALLOCATION_FAILURE));
    TRY (graph_परिणाम_t<व्योम>::from (g.ordering_, ALLOCATION_FAILURE));
    TRY (graph_परिणाम_t<व्योम>::from (g.ordering_scratch_, ALLOCATION_FAILURE));

    अचिन्हित order = objects.लंबाई;
    अचिन्हित skip = 0;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      // If this graph came from a serialization buffer object 0 is the
      // nil object. We don't need it for our purposes here so drop it.
      अगर (i == 0 && !objects.arrayZ[i])
      {
        removed_nil = true;
        order--;
        g.ordering_.resize (गिनती - 1);
        skip++;
        जारी;
      }

      hb_serialize_context_t::object_t obj (*objects.arrayZ[i]);
      अगर (unlikely (obj.in_error ())) {
        वापस Err(ALLOCATION_FAILURE);
      }
      vertex_t* v = g.vertices_.push (std::move (obj));

      TRY (v->link_positions_valid (गिनती, removed_nil));

      // To start we set the ordering to match the provided objects
      // list. Note: objects are provided to us in reverse order (ie.
      // the last object is the root).
      अचिन्हित obj_idx = i - skip;
      g.ordering_[--order] = obj_idx;

      अगर (!removed_nil) जारी;
      // Fix indices to account for removed nil object.
      क्रम (स्वतः& l : v->all_links_writer ()) {
        l.objidx--;
      }
    }

    स्वतः r = g.is_fully_connected();
    अगर (r.is_err() && r.error() == ORPHANED_NODES) {
      g.print_orphaned_nodes ();
    }
    TRY(r);

    वापस Ok (std::move (g));
  }

  graph_t (स्थिर graph_t&) = मिटाओ;
  graph_t& चालक = (स्थिर graph_t&) = मिटाओ;
  graph_t (graph_t&&) = शेष;
  graph_t& चालक = (graph_t&&) = शेष;

  ~graph_t ()
  {
    क्रम (अक्षर* b : buffers)
      hb_free (b);
  }

निजी:
  graph_t () = शेष;

खुला:

  bool चालक== (स्थिर graph_t& other) स्थिर
  {
    वापस मूल ().equals (मूल_idx(), other.मूल_idx(), other.मूल (), *यह, other, 0);
  }

  व्योम print () स्थिर {
    क्रम (अचिन्हित id : ordering_)
    {
      स्थिर स्वतः& v = vertices_[id];
      म_लिखो("%u: %u [", id, (अचिन्हित पूर्णांक)v.table_आकार());
      क्रम (स्थिर स्वतः &l : v.obj().real_links) {
        म_लिखो("%u, ", l.objidx);
      }
      क्रम (स्थिर स्वतः &l : v.obj().virtual_links) {
        म_लिखो("v%u, ", l.objidx);
      }
      म_लिखो("]\n");
    }
  }

  // Sorts links of all objects in a consistent manner and zeroes all offsets.
  व्योम normalize ()
  {
    क्रम (स्वतः& v : vertices_.writer ())
      v.normalize ();
  }

  स्थिर vertex_t& मूल () स्थिर
  {
    वापस vertices_[मूल_idx ()];
  }

  अचिन्हित मूल_idx () स्थिर
  {
    // First element of ordering_ is the root.
    // Since the graph is topologically sorted it's safe to
    // assume the first object has no incoming edges.
    वापस ordering_[0];
  }

  स्थिर hb_serialize_context_t::object_t& object (अचिन्हित i) स्थिर
  {
    वापस vertices_[i].obj();
  }

  graph_परिणाम_t<व्योम> add_बफर (अक्षर* बफर)
  {
    buffers.push (बफर);
    वापस graph_परिणाम_t<व्योम>::from (buffers, ALLOCATION_FAILURE);
  }

  /*
   * Adds a 16 bit link from parent_id to child_id
   */
  ढाँचा<typename T>
  graph_परिणाम_t<व्योम> add_link (T* offset,
                                 अचिन्हित parent_id,
                                 अचिन्हित child_id)
  {
    अगर (unlikely (parent_id >= vertices_.लंबाई || child_id >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);
    स्वतः& v = vertices_[parent_id];
    TRY(v.add_real_link(2, child_id, (अक्षर*) offset - (अक्षर*) v.obj().शीर्ष_2));
    वापस vertices_[child_id].add_parent (parent_id, false);
  }

  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node if positions are marked as invalid.
   */
  graph_परिणाम_t<व्योम> sort_shortest_distance_if_needed ()
  {
    अगर (!positions_invalid) वापस Ok();
    वापस sort_shortest_distance ();
  }


  /*
   * Generates a new topological sorting of graph ordered by the shortest
   * distance to each node.
   */
  graph_परिणाम_t<व्योम> sort_shortest_distance ()
  {
    positions_invalid = true;

    अगर (vertices_.लंबाई <= 1) {
      // Graph of 1 or less doesn't need sorting.
      वापस Ok();
    }

    TRY (update_distances ());

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (vertices_.लंबाई);
    TRY (graph_परिणाम_t<व्योम>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<अचिन्हित> &new_ordering = ordering_scratch_;
    new_ordering.resize (vertices_.लंबाई);
    TRY (graph_परिणाम_t<व्योम>::from (new_ordering, ALLOCATION_FAILURE));

    hb_vector_t<अचिन्हित> removed_edges;
    removed_edges.resize (vertices_.लंबाई);
    TRY (graph_परिणाम_t<व्योम>::from (removed_edges, ALLOCATION_FAILURE));

    TRY (update_parents ());

    queue.insert (मूल ().modified_distance (0), मूल_idx ());
    अचिन्हित order = 1;
    अचिन्हित pos = 0;
    जबतक (!queue.in_error () && !queue.is_empty ())
    {
      अचिन्हित अग्र_id = queue.निकालो_minimum().second;

      अगर (unlikely (pos >= new_ordering.लंबाई)) {
        // We are out of ids. Which means we've visited a node more than once.
        // This graph contains a cycle which is not allowed.
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "Invalid graph. Contains cycle.");
        वापस Err(CYCLE_DETECTED);
      }
      new_ordering[pos++] = अग्र_id;
      स्थिर vertex_t& next = vertices_[अग्र_id];

      क्रम (स्थिर स्वतः& link : next.obj().all_links ()) {
        अगर (unlikely (link.objidx >= vertices_.लंबाई))
          वापस Err(OUT_OF_BOUNDS);
        removed_edges[link.objidx]++;
        स्थिर स्वतः& v = vertices_[link.objidx];
        अगर (!(v.incoming_edges () - removed_edges[link.objidx]))
          // Add the order that the links were encountered to the priority.
          // This ensures that ties between priorities objects are broken in a consistent
          // way. More specifically this is set up so that if a set of objects have the same
          // distance they'll be added to the topological order in the order that they are
          // referenced from the parent object.
          queue.insert (v.modified_distance (order++),
                        link.objidx);
      }
    }

    TRY (graph_परिणाम_t<व्योम>::from (queue, ALLOCATION_FAILURE));
    TRY (graph_परिणाम_t<व्योम>::from (new_ordering, ALLOCATION_FAILURE));

    hb_swap (ordering_, new_ordering);

    अगर (unlikely (pos != vertices_.लंबाई)) {
      print_orphaned_nodes ();
      वापस Err(ORPHANED_NODES);
    }

    वापस Ok();
  }

  /*
   * Finds the set of nodes (placed into roots) that should be assigned unique spaces.
   * More specifically this looks for the top most 24 bit or 32 bit links in the graph.
   * Some special casing is done that is specific to the layout of GSUB/GPOS tables.
   */
  graph_परिणाम_t<व्योम> find_space_roots (hb_set_t& visited, hb_set_t& roots)
  {
    अचिन्हित मूल_सूचकांक = मूल_idx ();
    क्रम (अचिन्हित i : ordering_)
    {
      अगर (visited.has (i)) जारी;

      // Only real links can form 32 bit spaces
      क्रम (स्थिर स्वतः& l : vertices_[i].obj().real_links)
      {
        अगर (l.is_signed || l.width < 3)
          जारी;

        अगर (i == मूल_सूचकांक && l.width == 3)
          // Ignore 24bit links from the root node, this skips past the single 24bit
          // pointer to the lookup list.
          जारी;

        अगर (l.width >= 3)
        {
          // A wide offset forms a root, unless there are 32-bit offsets somewhere
          // in its subgraph, then those become the roots instead. This is to make sure
          // that extension subtables beneath a 24-bit lookup and lookups beneath a
          // 32-bit lookup list become the spaces instead of their enclosing lists.
          hb_set_t sub_roots;
          TRY (find_32bit_roots (l.objidx, sub_roots));
          अगर (sub_roots) {
            क्रम (अचिन्हित sub_मूल_idx : sub_roots) {
              roots.add (sub_मूल_idx);
              TRY (find_subgraph (sub_मूल_idx, visited));
            }
            जारी;
          }
        }

        roots.add (l.objidx);
        TRY (find_subgraph (l.objidx, visited));
      }
    }

    TRY(graph_परिणाम_t<व्योम>::from (visited, ALLOCATION_FAILURE));
    TRY(graph_परिणाम_t<व्योम>::from (roots, ALLOCATION_FAILURE));
    वापस Ok();
  }

  ढाँचा <typename T, typename ...Ts>
  graph_परिणाम_t<vertex_and_table_t<T, Immutable>> as_table (अचिन्हित parent, स्थिर व्योम* offset, Ts... ds)
  {
    TRY_ASSIGN (अचिन्हित idx, सूचकांक_for_offset (parent, offset));
    वापस as_table_from_सूचकांक<T, Immutable> (idx, std::forward<Ts>(ds)...);
  }

  ढाँचा <typename T, typename ...Ts>
  graph_परिणाम_t<vertex_and_table_t<T, Mutable>> as_mutable_table (अचिन्हित parent, स्थिर व्योम* offset, Ts... ds)
  {
    TRY_ASSIGN (स्वतः idx, mutable_सूचकांक_for_offset (parent, offset));
    वापस as_table_from_सूचकांक<T, Mutable> (idx, std::forward<Ts>(ds)...);
  }

  ढाँचा <typename T, vertex_mutability_t mutability = Mutable, typename ...Ts>
  graph_परिणाम_t<vertex_and_table_t<T, mutability>> as_table_from_सूचकांक (अचिन्हित सूचकांक_2, Ts... ds)
  {
    अगर (unlikely (सूचकांक_2 >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    vertex_and_table_t<T, mutability> r;
    r.vertex = (typename std::conditional<mutability == Immutable, स्थिर vertex_t*, vertex_t*>::type) &vertices_[सूचकांक_2];
    r.table = r.vertex->ढाँचा as<typename std::conditional<mutability == Immutable, स्थिर T*, T*>::type> ();
    r.सूचकांक_2 = सूचकांक_2;
    अगर (unlikely (!r.table))
      वापस Err(INVALID_ARGUMENT);

    TRY(r.table->sanitize (*(r.vertex), std::forward<Ts>(ds)...));
    वापस r;
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx].
  graph_परिणाम_t<अचिन्हित> सूचकांक_for_offset (अचिन्हित गाँठ_idx, स्थिर व्योम* offset) स्थिर
  {
    अगर (unlikely (गाँठ_idx >= vertices_.लंबाई)) वापस Err(OUT_OF_BOUNDS);
    स्थिर स्वतः& node = object (गाँठ_idx);
    अगर (unlikely (offset < node.शीर्ष_2 || offset >= node.पुच्छ)) वापस Err(OUT_OF_BOUNDS);

    अचिन्हित गिनती = node.real_links.लंबाई;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      // Use direct access for increased performance, this is a hot method.
      स्थिर स्वतः& link = node.real_links.arrayZ[i];
      अगर (offset != node.शीर्ष_2 + link.position)
        जारी;
      वापस Ok(link.objidx);
    }

    वापस Err(INVALID_ARGUMENT);
  }

  // Finds the object id of the object pointed to by the offset at 'offset'
  // within object[node_idx]. Ensures that the returned object is safe to mutate.
  // That is, if the original child object is shared by parents other than node_idx
  // it will be duplicated and the duplicate will be returned instead.
  graph_परिणाम_t<अचिन्हित> mutable_सूचकांक_for_offset (अचिन्हित गाँठ_idx, स्थिर व्योम* offset)
  {
    TRY_ASSIGN (अचिन्हित child_idx, सूचकांक_for_offset (गाँठ_idx, offset));
    अगर (unlikely (child_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    स्वतः& child = vertices_[child_idx];
    क्रम (अचिन्हित p : child.parents_iter ())
    {
      अगर (p != गाँठ_idx) {
        वापस duplicate (गाँठ_idx, child_idx, true);
      }
    }

    वापस Ok(child_idx);
  }


  /*
   * Assign unique space numbers to each connected subgraph of 24 bit and/or 32 bit offset(s).
   * Currently, this is implemented specifically tailored to the structure of a GPOS/GSUB
   * (including with 24bit offsets) table.
   */
  graph_परिणाम_t<bool> assign_spaces ()
  {
    TRY (update_parents ());

    क्रम (स्वतः& vertex : vertices_.writer ())
    {
      vertex.space = 0;
      vertex.priority = 0;
    }
    अंक_roots_for_space.resize (1);
    TRY (graph_परिणाम_t<व्योम>::from (अंक_roots_for_space, ALLOCATION_FAILURE));
    अंक_roots_for_space[0] = 1;
    distance_invalid = true;
    positions_invalid = true;

    hb_set_t visited;
    hb_set_t roots;
    TRY (find_space_roots (visited, roots));

    // Mark everything not in the subgraphs of the roots as visited. This prevents
    // subgraphs from being connected via nodes not in those subgraphs.
    visited.invert ();

    अगर (!roots) वापस Ok(false);

    जबतक (roots)
    {
      uint32_t next = HB_SET_मूल्य_INVALID;
      TRY (graph_परिणाम_t<व्योम>::from (roots, ALLOCATION_FAILURE));
      अगर (!roots.next (&next)) अवरोध;

      hb_set_t connected_roots;
      TRY (find_connected_nodes (next, roots, visited, connected_roots));
      TRY (graph_परिणाम_t<व्योम>::from (connected_roots, ALLOCATION_FAILURE));

      TRY (isolate_subgraph (connected_roots));
      TRY (graph_परिणाम_t<व्योम>::from (connected_roots, ALLOCATION_FAILURE));

      अचिन्हित अग्र_space = यह->अग्र_space ();
      अगर (unlikely (अग्र_space >= HB_REPACKER_MAX_SPACES))
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 अग्र_space);
        वापस Err(LIMIT_EXCEEDED);
      }
      अंक_roots_for_space.push (0);
      TRY(graph_परिणाम_t<व्योम>::from (अंक_roots_for_space, ALLOCATION_FAILURE));

      क्रम (अचिन्हित मूल : connected_roots)
      {
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "Subgraph %u gets space %u", मूल, अग्र_space);
        vertices_[मूल].space = अग्र_space;
        अंक_roots_for_space[अग्र_space] = अंक_roots_for_space[अग्र_space] + 1;
        distance_invalid = true;
        positions_invalid = true;
      }

      // TODO(grieger): special case for GSUB/GPOS use extension promotions to move 16 bit space
      //                into the 32 bit space as needed, instead of using isolation.
    }

    वापस Ok(true);
  }

  /*
   * Isolates the subgraph of nodes reachable from root. Any links to nodes in the subgraph
   * that originate from outside of the subgraph will be removed by duplicating the linked to
   * object.
   *
   * Indices stored in roots will be updated if any of the roots are duplicated to new indices.
   */
  graph_परिणाम_t<bool> isolate_subgraph (hb_set_t& roots)
  {
    TRY(update_parents ());
    hb_map_t subgraph;

    // incoming edges to root_idx should be all 32 bit in length so we don't need to de-dup these
    // set the subgraph incoming edge count to match all of root_idx's incoming edges
    hb_set_t parents;
    क्रम (अचिन्हित मूल_idx : roots)
    {
      अगर (unlikely (मूल_idx >= vertices_.लंबाई))
        वापस Err(OUT_OF_BOUNDS);
      subgraph.set (मूल_idx, wide_parents (मूल_idx, parents));
    }
    क्रम (अचिन्हित मूल_idx : roots)
    {
      TRY (find_subgraph (मूल_idx, subgraph));
    }
    TRY (graph_परिणाम_t<व्योम>::from (subgraph, ALLOCATION_FAILURE));

    hb_map_t सूचकांक_map;
    bool made_changes = false;
    क्रम (स्वतः entry : subgraph.iter ())
    {
      निश्चित (entry.first < vertices_.लंबाई);
      स्थिर स्वतः& node = vertices_[entry.first];
      अचिन्हित subgraph_incoming_edges = entry.second;

      अगर (subgraph_incoming_edges < node.incoming_edges ())
      {
        // Only  de-dup objects with incoming links from outside the subgraph.
        made_changes = true;
        TRY (duplicate_subgraph (entry.first, सूचकांक_map));
      }
    }

    TRY (graph_परिणाम_t<व्योम>::from (सूचकांक_map, ALLOCATION_FAILURE));

    अगर (!made_changes)
      वापस Ok(false);

    स्वतः new_subgraph =
        + subgraph.कुंजियाँ ()
        | hb_map([&] (uint32_t गाँठ_idx) {
          स्थिर uint32_t *v;
          अगर (सूचकांक_map.has (गाँठ_idx, &v)) वापस *v;
          वापस गाँठ_idx;
        })
        ;

    TRY(remap_obj_indices (सूचकांक_map, new_subgraph));
    TRY(remap_obj_indices (सूचकांक_map, parents.iter (), true));

    // Update roots set with new indices as needed.
    क्रम (स्वतः next : roots)
    {
      स्थिर uint32_t *v;
      अगर (सूचकांक_map.has (next, &v))
      {
        roots.del (next);
        roots.add (*v);
      }
    }

    वापस Ok(true);
  }

  // BFS graph traversal starting at start_idx.
  //
  // The visit_edge function will be called once for the root node and then for each traversed edge
  // with the following signature:
  //
  // bool VisitEdgeFunc(unsigned parent, const link_t* link, unsigned child, unsigned depth)
  //
  // Where a return value of false signals that traversal should not continue into child's outgoing
  // edges. parent/child are the vertex indices. depth starts at 0 for the root node.
  // link will be null when called for entering the root node at the start of the traversal.
  //
  // This traversal does not use a visited set internally, it is the responsibility of visit_edge
  // to track and filter visited nodes if required for the particular traversal.
  ढाँचा <typename VisitEdgeFunc>
  graph_परिणाम_t<व्योम> traverse_directed_bfs (अचिन्हित start_idx, VisitEdgeFunc&& visit_edge)
  {
    अगर (unlikely (start_idx >= vertices_.लंबाई))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "traverse_directed_bfs: unexpected start_idx out of bounds.");
      वापस Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a queue for BFS.
    ordering_scratch_.resize (vertices_.लंबाई);
    TRY (graph_परिणाम_t<व्योम>::from (ordering_scratch_, ALLOCATION_FAILURE));

    अगर (!visit_edge (HB_CODEPOINT_INVALID, nullptr, start_idx, 0))
      वापस Ok();

    अचिन्हित शीर्ष_2 = 0;
    अचिन्हित पुच्छ = 0;
    स्वतः& queue = ordering_scratch_;
    queue[पुच्छ++] = start_idx;

    अचिन्हित depth = 0;
    जबतक (शीर्ष_2 < पुच्छ)
    {
      अचिन्हित level_end = पुच्छ;
      जबतक (शीर्ष_2 < level_end)
      {
        अचिन्हित गाँठ_idx = queue[शीर्ष_2++];
        स्थिर स्वतः& v = vertices_[गाँठ_idx];

        अचिन्हित अंक_real = v.obj().real_links.लंबाई;
        अचिन्हित योग_links = अंक_real + v.obj().virtual_links.लंबाई;

        क्रम (अचिन्हित i = 0; i < योग_links; i++)
        {
          // vertices_ may have re-alloc'd inside an op() call, so reassign the v ref.
          स्थिर स्वतः& v = vertices_[गाँठ_idx];
          स्थिर स्वतः& link = (i < अंक_real)
                             ? v.obj().real_links[i]
                             : v.obj().virtual_links[i - अंक_real];
          अचिन्हित child_idx = link.objidx;

          अगर (!visit_edge (गाँठ_idx, &link, child_idx, depth + 1))
            जारी;

          अगर (unlikely (पुच्छ >= queue.लंबाई))
            वापस Err(CYCLE_DETECTED);

          queue[पुच्छ++] = child_idx;
        }
      }
      depth++;
    }
    वापस Ok();
  }

  graph_परिणाम_t<व्योम> find_subgraph (अचिन्हित गाँठ_idx, hb_map_t& subgraph)
  {
    वापस traverse_directed_bfs (गाँठ_idx, [&] (
      अचिन्हित parent,
      स्थिर hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित depth) {
      अगर (depth == 0) वापस true;
      hb_codepoint_t *गिनती;
      अगर (subgraph.has (child, &गिनती))
      {
        (*गिनती)++;
        वापस false;
      }
      subgraph.set (child, 1);
      वापस true;
    });
  }

  graph_परिणाम_t<व्योम> find_subgraph (अचिन्हित गाँठ_idx, hb_set_t& subgraph)
  {
    वापस traverse_directed_bfs (गाँठ_idx, [&] (
      अचिन्हित parent,
      स्थिर hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित depth) {
      अगर (subgraph.has (child)) वापस false;
      subgraph.add (child);
      वापस true;
    });
  }

  graph_परिणाम_t<माप_प्रकार> find_subgraph_आकार (अचिन्हित गाँठ_idx, hb_set_t& subgraph, अचिन्हित max_depth = HB_GRAPH_INVALID)
  {
    माप_प्रकार आकार = 0;
    TRY (traverse_directed_bfs (गाँठ_idx, [&] (
      अचिन्हित parent,
      स्थिर hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित depth) {
      अगर (subgraph.has (child)) वापस false;
      subgraph.add (child);

      स्थिर स्वतः& o = vertices_[child].obj();
      आकार += o.पुच्छ - o.शीर्ष_2;
      वापस depth < max_depth;
    }));
    वापस Ok(आकार);
  }

  /*
   * Finds the topmost children of 32bit offsets in the subgraph starting
   * at node_idx. Found indices are placed into 'found'.
   */
  graph_परिणाम_t<व्योम> find_32bit_roots (अचिन्हित गाँठ_idx, hb_set_t& found)
  {
    // Note: this specifically requires a BFS based traversal to ensure we don't recurse through
    // a node that is accessible via both 32bit and non-32 bit links.
    hb_set_t visited;
    वापस traverse_directed_bfs (गाँठ_idx, [&] (
      अचिन्हित parent,
      स्थिर hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित _) {

      अगर (link && found.has(parent))
        // Don't traverse from something that's already marked as a root.
        वापस false;

      अगर (link && !link->is_signed && link->width == 4)
      {
        found.add (link->objidx);
        visited.add (link->objidx);
        वापस false;
      }

      अगर (visited.has (child)) वापस false;
      visited.add (child);
      वापस true;
    });
  }

  /*
   * Moves the child of old_parent_idx pointed to by old_offset to a new
   * vertex at the new_offset.
   *
   * Returns the id of the child node that was moved.
   */
  ढाँचा<typename O>
  graph_परिणाम_t<अचिन्हित> move_child (अचिन्हित old_parent_idx,
                                       स्थिर O* old_offset,
                                       अचिन्हित new_parent_idx,
                                       स्थिर O* new_offset)
  {
    distance_invalid = true;
    positions_invalid = true;

    अगर (unlikely (old_parent_idx >= vertices_.लंबाई ||
                  new_parent_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    स्वतः& old_v = vertices_[old_parent_idx];
    स्वतः& new_v = vertices_[new_parent_idx];

    TRY_ASSIGN (अचिन्हित child_id, सूचकांक_for_offset (old_parent_idx,
                                                     old_offset));
    अगर (unlikely (child_id >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    TRY(new_v.add_real_link(O::static_आकार, child_id, (स्थिर अक्षर*) new_offset - (स्थिर अक्षर*) new_v.obj().शीर्ष_2));

    स्वतः& child = vertices_[child_id];
    TRY(child.add_parent (new_parent_idx, false));

    old_v.remove_real_link_unordered (child_id, old_offset);
    child.remove_parent (old_parent_idx);

    वापस Ok(child_id);
  }

  /*
   * Moves all outgoing links in old parent that have
   * a link position between [old_post_start, old_pos_end)
   * to the new parent. Links are placed serially in the new
   * parent starting at new_pos_start.
   */
  ढाँचा<typename O>
  graph_परिणाम_t<व्योम> move_children (अचिन्हित old_parent_idx,
                                      अचिन्हित old_pos_start,
                                      अचिन्हित old_pos_end,
                                      अचिन्हित new_parent_idx,
                                      अचिन्हित new_pos_start)
  {
    अगर (unlikely (old_parent_idx >= vertices_.लंबाई ||
                  new_parent_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    distance_invalid = true;
    positions_invalid = true;

    स्वतः& old_v = vertices_[old_parent_idx];
    स्वतः& new_v = vertices_[new_parent_idx];

    hb_vector_t<hb_serialize_context_t::object_t::link_t> old_links;
    क्रम (स्थिर स्वतः& l : old_v.obj().real_links)
    {
      अगर (l.position < old_pos_start || l.position >= old_pos_end)
      {
        old_links.push (l);
        जारी;
      }

      अचिन्हित array_pos = l.position - old_pos_start;

      अचिन्हित child_id = l.objidx;
      अगर (unlikely (child_id >= vertices_.लंबाई))
        वापस Err(OUT_OF_BOUNDS);

      TRY(new_v.add_real_link(O::static_आकार, child_id, new_pos_start + array_pos));

      स्वतः& child = vertices_[child_id];
      TRY(child.add_parent (new_parent_idx, false));
      child.remove_parent (old_parent_idx);
    }

    TRY (graph_परिणाम_t<व्योम>::from (old_links, ALLOCATION_FAILURE));
    वापस old_v.set_real_links(std::move (old_links));
  }

  /*
   * duplicates all nodes in the subgraph reachable from node_idx. Does not re-assign
   * links. index_map is updated with mappings from old id to new id. If a duplication has already
   * been performed for a given index, then it will be skipped.
   */
  graph_परिणाम_t<व्योम> duplicate_subgraph (अचिन्हित गाँठ_idx, hb_map_t& सूचकांक_map)
  {
    graph_परिणाम_t<व्योम> परिणाम = Ok();
    TRY (traverse_directed_bfs (गाँठ_idx, [&] (
      अचिन्हित parent,
      स्थिर hb_serialize_context_t::object_t::link_t* link,
      अचिन्हित child,
      अचिन्हित _) {
      अगर (परिणाम.is_err () || सूचकांक_map.has (child)) वापस false;
      स्वतः r = duplicate (child);
      अगर (unlikely (r.is_err ()))
      {
        परिणाम = Err(r.error ());
        वापस false;
      }
      सूचकांक_map.set (child, *r);
      वापस true;
    }));

    वापस graph_परिणाम_t<व्योम>::from(सूचकांक_map, ALLOCATION_FAILURE);
  }

  /*
   * Creates a copy of node_idx and returns it's new index.
   */
  graph_परिणाम_t<अचिन्हित> duplicate (अचिन्हित गाँठ_idx, bool copy_table = false)
  {
    अगर (unlikely (गाँठ_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    अगर (unlikely (vertices_.लंबाई >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "duplicating node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.लंबाई);
      वापस Err(LIMIT_EXCEEDED);
    }

    positions_invalid = true;
    distance_invalid = true;

    स्वतः* clone = vertices_.push ();
    TRY (graph_परिणाम_t<व्योम>::from (vertices_, ALLOCATION_FAILURE));

    अचिन्हित clone_idx = vertices_.लंबाई - 1;
    ordering_.push (clone_idx);
    TRY (graph_परिणाम_t<व्योम>::from (ordering_, ALLOCATION_FAILURE));

    स्वतः& child = vertices_[गाँठ_idx];

    अचिन्हित table_आकार = child.obj().पुच्छ - child.obj().शीर्ष_2;
    अगर (copy_table && table_आकार)
    {
      अक्षर* बफर = (अक्षर*) hb_malloc (table_आकार);
      अगर (unlikely (!बफर))
        वापस Err(ALLOCATION_FAILURE);
      स्वतः res = add_बफर (बफर);
      अगर (unlikely (res.is_err ()))
      {
        hb_free (बफर);
        वापस Err(res.error ());
      }
      hb_memcpy (बफर, child.obj().शीर्ष_2, table_आकार);
      clone->set_बफर(बफर, बफर + table_आकार);
    }
    अन्यथा
    {
      clone->set_बफर(child.obj().शीर्ष_2, child.obj().पुच्छ);
    }
    clone->distance = child.distance;
    clone->space = child.space;
    clone->reset_parents ();

    क्रम (स्थिर स्वतः& l : child.obj().real_links)
    {
      TRY(clone->add_real_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, false));
    }

    क्रम (स्थिर स्वतः& l : child.obj().virtual_links)
    {
      TRY(clone->add_virtual_link (l));
      TRY(vertices_[l.objidx].add_parent (clone_idx, true));
    }

    वापस Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the link from
   * parent to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parent_idx,
   * duplication isn't possible and this will return an error.
   */
  graph_परिणाम_t<अचिन्हित> duplicate (अचिन्हित parent_idx, अचिन्हित child_idx, bool copy_table = false)
  {
    अगर (unlikely (parent_idx >= vertices_.लंबाई || child_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    स्थिर स्वतः& child = vertices_[child_idx];
    अचिन्हित links_to_child = child.incoming_edges_from_parent (parent_idx);

    अगर (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Not duplicating %u => %u",
                 parent_idx, child_idx);
      वापस Err(INVALID_ARGUMENT);
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Duplicating %u => %u",
               parent_idx, child_idx);

    TRY_ASSIGN (अचिन्हित clone_idx, duplicate (child_idx, copy_table));
    // duplicate shifts the root node idx, so if parent_idx was root update it.
    अगर (parent_idx == clone_idx) parent_idx++;

    स्वतः& parent = vertices_[parent_idx];
    अचिन्हित गिनती = 0;
    अचिन्हित अंक_real = parent.obj().real_links.लंबाई;
    क्रम (स्वतः& l : parent.all_links_writer ())
    {
      गिनती++;
      अगर (l.objidx != child_idx)
        जारी;

      TRY(reassign_link (l, parent_idx, clone_idx, गिनती > अंक_real));
    }

    वापस Ok(clone_idx);
  }

  /*
   * Creates a copy of child and re-assigns the links from
   * parents to the clone. The copy is a shallow copy, objects
   * linked from child are not duplicated.
   *
   * Returns the index of the newly created duplicate.
   *
   * If the child_idx only has incoming edges from parents,
   * duplication isn't possible or duplication fails and this will
   * return an error.
   */
  graph_परिणाम_t<अचिन्हित> duplicate (स्थिर hb_set_t* parents, अचिन्हित child_idx)
  {
    अगर (unlikely (!parents || parents->is_empty ()))
      वापस Err(INVALID_ARGUMENT);

    अगर (unlikely (child_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    TRY (update_parents ());

    स्थिर स्वतः& child = vertices_[child_idx];
    अचिन्हित links_to_child = 0;
    अचिन्हित last_parent = parents->get_max ();
    अचिन्हित first_parent = parents->get_min ();
    क्रम (अचिन्हित parent_idx : *parents) {
      अगर (unlikely (parent_idx >= vertices_.लंबाई))
        वापस Err(OUT_OF_BOUNDS);
      links_to_child += child.incoming_edges_from_parent (parent_idx);
    }

    अगर (unlikely (child.incoming_edges () <= links_to_child || child.has_incoming_virtual_edges ()))
    {
      // Can't duplicate this node, doing so would orphan the original one as all remaining links
      // to child are from parent.
      //
      // We don't allow duplication of nodes with incoming virtual edges because we don't track
      // the number of virtual vs real incoming edges. As a result we can't tell if a node
      // with virtual edges may end up orphaned by duplication (ie. where one copy is only pointed
      // to by virtual edges).
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Not duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);
      वापस Err(INVALID_ARGUMENT);
    }

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Duplicating %u, ..., %u => %u", first_parent, last_parent, child_idx);

    TRY_ASSIGN (अचिन्हित clone_idx, duplicate (child_idx));

    क्रम (अचिन्हित parent_idx : *parents) {
      // duplicate shifts the root node idx, so if parent_idx was root update it.
      अगर (parent_idx == clone_idx) parent_idx++;
      स्वतः& parent = vertices_[parent_idx];
      अचिन्हित गिनती = 0;
      अचिन्हित अंक_real = parent.obj().real_links.लंबाई;
      क्रम (स्वतः& l : parent.all_links_writer ())
      {
        गिनती++;
        अगर (l.objidx != child_idx)
          जारी;

        TRY(reassign_link (l, parent_idx, clone_idx, गिनती > अंक_real));
      }
    }

    वापस Ok(clone_idx);
  }


  /*
   * Adds a new node to the graph, not connected to anything.
   */
  graph_परिणाम_t<अचिन्हित> new_गाँठ (अक्षर* शीर्ष_2, अक्षर* पुच्छ)
  {
    अगर (unlikely (vertices_.लंबाई >= HB_REPACKER_MAX_VERTICES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "creating new node: num of vertices %u exceeds HB_REPACKER_MAX_VERTICES.",
                 vertices_.लंबाई);
      वापस Err(LIMIT_EXCEEDED);
    }

    positions_invalid = true;
    distance_invalid = true;

    स्वतः* clone = vertices_.push ();
    TRY (graph_परिणाम_t<व्योम>::from (vertices_, ALLOCATION_FAILURE));

    अचिन्हित clone_idx = vertices_.लंबाई - 1;
    ordering_.push (clone_idx);
    TRY (graph_परिणाम_t<व्योम>::from (ordering_, ALLOCATION_FAILURE));

    clone->set_बफर (शीर्ष_2, पुच्छ);
    clone->distance = 0;
    clone->space = 0;

    वापस Ok(clone_idx);
  }

  /*
   * Creates a new child node and remap the old child to it.
   *
   * Returns the index of the newly created child.
   *
   */
  graph_परिणाम_t<अचिन्हित> remap_child (अचिन्हित parent_idx, अचिन्हित old_child_idx)
  {
    अगर (unlikely (parent_idx >= vertices_.लंबाई || old_child_idx >= vertices_.लंबाई))
      वापस Err(OUT_OF_BOUNDS);

    TRY_ASSIGN (अचिन्हित new_child_idx, duplicate (old_child_idx));

    स्वतः& parent = vertices_[parent_idx];
    क्रम (स्वतः& l : parent.real_links_writer ())
    {
      अगर (l.objidx != old_child_idx)
        जारी;
      TRY(reassign_link (l, parent_idx, new_child_idx, false));
    }

    क्रम (स्वतः& l : parent.virtual_links_writer ())
    {
      अगर (l.objidx != old_child_idx)
        जारी;
      TRY(reassign_link (l, parent_idx, new_child_idx, true));
    }
    वापस Ok(new_child_idx);
  }

  /*
   * Raises the sorting priority of all children.
   */
  graph_परिणाम_t<bool> raise_childrens_priority (अचिन्हित parent_idx)
  {
    DEBUG_संदेश (SUBSET_REPACK, nullptr, "  Raising priority of all children of %u",
               parent_idx);
    // This operation doesn't change ordering until a sort is run, so no need
    // to invalidate positions. It does not change graph structure so no need
    // to update distances or edge counts.
    अगर (unlikely (parent_idx >= vertices_.लंबाई)) वापस Err(OUT_OF_BOUNDS);
    bool made_change = false;
    क्रम (स्वतः& l : vertices_[parent_idx].all_links_writer ())
      made_change |= vertices_[l.objidx].raise_priority ();
    वापस Ok(made_change);
  }

  graph_परिणाम_t<व्योम> is_fully_connected ()
  {
    TRY(update_parents());

    अगर (unlikely (मूल().incoming_edges ()))
      // Root cannot have parents.
      वापस Err(ORPHANED_NODES);

    क्रम (अचिन्हित i = 0; i < मूल_idx (); i++)
    {
      अगर (unlikely (!vertices_[i].incoming_edges ()))
        वापस Err(ORPHANED_NODES);
    }
    वापस Ok();
  }

#अगर 0
  /*
   * Saves the current graph to a packed binary format which the repacker fuzzer takes
   * as a seed.
   */
  व्योम save_fuzzer_seed (hb_tag_t tag) स्थिर
  {
    खाता* f = ख_खोलो ("./repacker_fuzzer_seed", "w");
    ख_डालो ((व्योम*) &tag, माप (tag), 1, f);

    uint16_t अंक_objects = vertices_.लंबाई;
    ख_डालो ((व्योम*) &अंक_objects, माप (अंक_objects), 1, f);

    क्रम (स्थिर स्वतः& v : vertices_)
    {
      uint16_t blob_आकार = v.table_आकार ();
      ख_डालो ((व्योम*) &blob_आकार, माप (blob_आकार), 1, f);
      ख_डालो ((स्थिर व्योम*) v.obj.शीर्ष_2, blob_आकार, 1, f);
    }

    uint16_t link_गिनती = 0;
    क्रम (स्थिर स्वतः& v : vertices_)
      link_गिनती += v.obj.real_links.लंबाई;

    ख_डालो ((व्योम*) &link_गिनती, माप (link_गिनती), 1, f);

    प्रकार काष्ठा
    {
      uint16_t parent;
      uint16_t child;
      uint16_t position;
      uint8_t width;
    } link_t;

    क्रम (अचिन्हित i = 0; i < vertices_.लंबाई; i++)
    {
      क्रम (स्थिर स्वतः& l : vertices_[i].obj.real_links)
      {
        link_t link {
          (uint16_t) i, (uint16_t) l.objidx,
          (uint16_t) l.position, (uint8_t) l.width
        };
        ख_डालो ((व्योम*) &link, माप (link), 1, f);
      }
    }

    ख_बंद (f);
  }
#पूर्ण_अगर

  अचिन्हित अंक_roots_for_space_2 (अचिन्हित space) स्थिर
  {
    वापस अंक_roots_for_space[space];
  }

  अचिन्हित अग्र_space () स्थिर
  {
    वापस अंक_roots_for_space.लंबाई;
  }

  graph_परिणाम_t<व्योम> move_to_new_space (स्थिर hb_set_t& indices)
  {
    अगर (unlikely (अंक_roots_for_space.लंबाई >= HB_REPACKER_MAX_SPACES))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "move_to_new_space: num of spaces %u exceeds HB_REPACKER_MAX_SPACES.",
                 अंक_roots_for_space.लंबाई);
      वापस Err(LIMIT_EXCEEDED);
    }
    अंक_roots_for_space.push (0);
    TRY (graph_परिणाम_t<व्योम>::from (अंक_roots_for_space, ALLOCATION_FAILURE));
    अचिन्हित new_space = अंक_roots_for_space.लंबाई - 1;

    क्रम (अचिन्हित सूचकांक_2 : indices) {
      अगर (unlikely (सूचकांक_2 >= vertices_.लंबाई))
        वापस Err(OUT_OF_BOUNDS);
      स्वतः& node = vertices_[सूचकांक_2];
      अंक_roots_for_space[node.space] = अंक_roots_for_space[node.space] - 1;
      अंक_roots_for_space[new_space] = अंक_roots_for_space[new_space] + 1;
      node.space = new_space;
      distance_invalid = true;
      positions_invalid = true;
    }
    वापस Ok();
  }

  अचिन्हित space_for (अचिन्हित सूचकांक_2, अचिन्हित* मूल = nullptr) स्थिर
  {
    जबतक (true)
    {
      निश्चित (सूचकांक_2 < vertices_.लंबाई);
      स्थिर स्वतः& node = vertices_[सूचकांक_2];
      अगर (node.space)
      {
        अगर (मूल != nullptr)
          *मूल = सूचकांक_2;
        वापस node.space;
      }

      अगर (!node.incoming_edges ())
      {
        अगर (मूल)
          *मूल = सूचकांक_2;
        वापस 0;
      }

      सूचकांक_2 = *node.parents_iter ();
    }
  }

  माप_प्रकार योग_आकार_in_bytes () स्थिर {
    माप_प्रकार योग_आकार = 0;
    अचिन्हित गिनती = vertices_.लंबाई;
    क्रम (अचिन्हित i = 0; i < गिनती; i++) {
      माप_प्रकार आकार = vertices_.arrayZ[i].table_आकार ();
      योग_आकार += आकार;
    }
    वापस योग_आकार;
  }


 निजी:

  व्योम print_orphaned_nodes ()
  {
    अगर (!DEBUG_ENABLED(SUBSET_REPACK)) वापस;

    DEBUG_संदेश (SUBSET_REPACK, nullptr, "Graph is not fully connected.");

    parents_invalid = true;
    (व्योम) update_parents();

    अगर (मूल().incoming_edges ()) {
      DEBUG_संदेश (SUBSET_REPACK, nullptr, "Root node has incoming edges.");
    }

    क्रम (अचिन्हित i = 0; i < vertices_.लंबाई; i++)
    {
      स्थिर स्वतः& v = vertices_[i];
      अगर (!v.incoming_edges ())
        DEBUG_संदेश (SUBSET_REPACK, nullptr, "Node %u is orphaned.", i);
    }
  }

  /*
   * Returns the numbers of incoming edges that are 24 or 32 bits wide.
   */
  अचिन्हित wide_parents (अचिन्हित गाँठ_idx, hb_set_t& parents) स्थिर
  {
    अचिन्हित गिनती = 0;
    क्रम (अचिन्हित p : vertices_[गाँठ_idx].parents_iter ())
    {
      // Only real links can be wide
      क्रम (स्थिर स्वतः& l : vertices_[p].obj().real_links)
      {
        अगर (l.objidx == गाँठ_idx
            && (l.width == 3 || l.width == 4)
            && !l.is_signed)
        {
          गिनती++;
          parents.add (p);
        }
      }
    }
    वापस गिनती;
  }

 खुला:
  /*
   * Creates a map from objid to # of incoming edges.
   */
  graph_परिणाम_t<व्योम> update_parents ()
  {
    अगर (!parents_invalid) वापस Ok();

    अचिन्हित गिनती = vertices_.लंबाई;

    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      vertices_.arrayZ[i].reset_parents ();

    क्रम (अचिन्हित p = 0; p < गिनती; p++)
    {
      क्रम (स्थिर स्वतः& l : vertices_.arrayZ[p].obj().real_links)
      {
        अगर (unlikely (l.objidx >= गिनती)) वापस Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, false));
      }

      क्रम (स्थिर स्वतः& l : vertices_.arrayZ[p].obj().virtual_links)
      {
        अगर (unlikely (l.objidx >= गिनती)) वापस Err(INVALID_ARGUMENT);
        TRY(vertices_[l.objidx].add_parent (p, true));
      }
    }

    parents_invalid = false;
    वापस Ok();
  }

  /*
   * compute the serialized start and end positions for each vertex.
   */
  व्योम update_positions ()
  {
    अगर (!positions_invalid) वापस;

    अचिन्हित current_pos = 0;
    क्रम (अचिन्हित i : ordering_)
    {
      स्वतः& v = vertices_[i];
      v.start = current_pos;
      current_pos += v.obj().पुच्छ - v.obj().शीर्ष_2;
      v.end = current_pos;
    }

    positions_invalid = false;
  }

  /*
   * Finds the distance to each object in the graph
   * from the initial node.
   */
  graph_परिणाम_t<व्योम> update_distances ()
  {
    अगर (!distance_invalid) वापस Ok();

    // Uses Dijkstra's algorithm to find all of the shortest distances.
    // https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
    //
    // Implementation Note:
    // Since our priority queue doesn't support fast priority decreases
    // we instead just add new entries into the queue when a priority changes.
    // Redundant ones are filtered out later on by the visited set.
    // According to https://www3.cs.stonybrook.edu/~rezaul/papers/TR-07-54.pdf
    // for practical performance this is faster then using a more advanced queue
    // (such as a fibonacci queue) with a fast decrease priority.
    अचिन्हित गिनती = vertices_.लंबाई;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
      vertices_.arrayZ[i].distance = hb_int_max (int64_t);
    vertices_[मूल_idx ()].distance = 0;

    hb_priority_queue_t<int64_t> queue;
    queue.alloc (गिनती);
    queue.insert (0, मूल_idx ());
    TRY (graph_परिणाम_t<व्योम>::from (queue, ALLOCATION_FAILURE));

    hb_vector_t<bool> visited;
    visited.resize (vertices_.लंबाई);
    TRY (graph_परिणाम_t<व्योम>::from (visited, ALLOCATION_FAILURE));

    जबतक (!queue.in_error () && !queue.is_empty ())
    {
      अचिन्हित अग्र_idx = queue.निकालो_minimum ().second;
      अगर (visited[अग्र_idx]) जारी;
      स्थिर स्वतः& next = vertices_[अग्र_idx];
      int64_t अग्र_distance = next.distance;
      visited[अग्र_idx] = true;

      क्रम (स्थिर स्वतः& link : next.obj().all_links ())
      {
        अगर (unlikely (link.objidx >= vertices_.लंबाई))
          वापस Err(OUT_OF_BOUNDS);

        अगर (visited[link.objidx]) जारी;

        स्वतः& child_v = vertices_.arrayZ[link.objidx];
        स्थिर स्वतः& child = child_v.obj ();
        अचिन्हित link_width = link.width ? link.width : 4; // treat virtual offsets as 32 bits wide
        int64_t child_weight = (child.पुच्छ - child.शीर्ष_2) +
                               ((int64_t) 1 << (link_width * 8)) * (child_v.space + 1);
        int64_t child_distance = अग्र_distance + child_weight;

        अगर (child_distance < child_v.distance)
        {
          child_v.distance = child_distance;
          queue.insert (child_distance, link.objidx);
        }
      }
    }

    TRY (graph_परिणाम_t<व्योम>::from (queue, ALLOCATION_FAILURE));
    अगर (unlikely (!queue.is_empty ()))
    {
      print_orphaned_nodes ();
      वापस Err(ORPHANED_NODES);
    }

    distance_invalid = false;
    वापस Ok();
  }

 निजी:
  /*
   * Updates a link in the graph to point to a different object. Corrects the
   * parents vector on the previous and new child nodes.
   */
  graph_परिणाम_t<व्योम> reassign_link (hb_serialize_context_t::object_t::link_t& link,
                      अचिन्हित parent_idx,
                      अचिन्हित new_idx,
                      bool is_virtual)
  {
    अचिन्हित old_idx = link.objidx;
    link.objidx = new_idx;
    vertices_[old_idx].remove_parent (parent_idx);
    वापस vertices_[new_idx].add_parent (parent_idx, is_virtual);
  }

  /*
   * Updates all objidx's in all links using the provided mapping. Corrects incoming edge counts.
   */
  ढाँचा<typename Iterator, hb_requires (hb_is_iterator (Iterator))>
  graph_परिणाम_t<व्योम> remap_obj_indices (स्थिर hb_map_t& id_map,
                                          Iterator subgraph,
                                          bool only_wide = false)
  {
    अगर (!id_map) वापस Ok();
    क्रम (अचिन्हित i : subgraph)
    {
      स्वतः& vertex = vertices_[i];
      अचिन्हित अंक_real = vertex.obj().real_links.लंबाई;
      अचिन्हित गिनती = 0;
      क्रम (स्वतः& link : vertex.all_links_writer ())
      {
        गिनती++;
        स्थिर uint32_t *v;
        अगर (!id_map.has (link.objidx, &v)) जारी;
        अगर (only_wide && (link.is_signed || (link.width != 4 && link.width != 3))) जारी;

        TRY(reassign_link (link, i, *v, गिनती > अंक_real));
      }
    }
    वापस Ok();
  }

  /*
   * Finds all nodes in targets that are reachable from start_idx, nodes in visited will be skipped.
   * For this search the graph is treated as being undirected.
   *
   * Connected targets will be added to connected and removed from targets. All visited nodes
   * will be added to visited.
   */
  graph_परिणाम_t<व्योम> find_connected_nodes (अचिन्हित start_idx,
                                             hb_set_t& targets,
                                             hb_set_t& visited,
                                             hb_set_t& connected)
  {
    TRY (graph_परिणाम_t<व्योम>::from (visited, ALLOCATION_FAILURE));
    अगर (visited.has (start_idx)) वापस Ok();
    अगर (unlikely (start_idx >= vertices_.लंबाई))
    {
      DEBUG_संदेश (SUBSET_REPACK, nullptr,
                 "find_connected_nodes: unexpected start_idx out of bounds.");
      वापस Err(OUT_OF_BOUNDS);
    }

    // For performance we want to avoid allocating extra memory. So use the ordering_scratch_
    // buffer to implement a stack for DFS.
    ordering_scratch_.resize (vertices_.लंबाई);
    TRY (graph_परिणाम_t<व्योम>::from (ordering_scratch_, ALLOCATION_FAILURE));

    स्वतः& ढेर = ordering_scratch_;
    अचिन्हित ढेर_len = 0;

    graph_परिणाम_t<व्योम> res = Ok();
    स्वतः handle_गाँठ = [&] (अचिन्हित गाँठ_idx) {
      visited.add (गाँठ_idx);
      अगर (targets.has (गाँठ_idx)) {
        targets.del (गाँठ_idx);
        connected.add (गाँठ_idx);
      }

      अगर (unlikely (ढेर_len >= ढेर.लंबाई))
      {
        res = Err(CYCLE_DETECTED);
        वापस false;
      }
      ढेर[ढेर_len++] = गाँठ_idx;
      वापस true;
    };

    अगर (!handle_गाँठ (start_idx)) वापस res;

    जबतक (ढेर_len > 0)
    {
      अचिन्हित गाँठ_idx = ढेर[--ढेर_len];
      स्थिर स्वतः& v = vertices_[गाँठ_idx];

      // Graph is treated as undirected so search children and parents of node_idx
      क्रम (स्थिर स्वतः& l : v.obj().all_links ())
      {
        अचिन्हित child_idx = l.objidx;
        अगर (visited.has (child_idx)) जारी;
        अगर (!handle_गाँठ (child_idx)) वापस res;
      }

      क्रम (अचिन्हित parent_idx : v.parents_iter ())
      {
        अगर (visited.has (parent_idx)) जारी;
        अगर (!handle_गाँठ (parent_idx)) वापस res;
      }
    }

    TRY (graph_परिणाम_t<व्योम>::from (visited, ALLOCATION_FAILURE));
    TRY (graph_परिणाम_t<व्योम>::from (connected, ALLOCATION_FAILURE));

    वापस Ok();
  }

 खुला:
  // TODO(garretrieger): make private, will need to move most of offset overflow code into graph.
  hb_vector_t<vertex_t> vertices_;

  // Specifies the current topological ordering of this graph
  //
  // ordering_[pos] = obj index
  //
  // specifies that the 'pos'th spot is filled by the object
  // given by obj index.
  hb_vector_t<अचिन्हित> ordering_;
  hb_vector_t<अचिन्हित> ordering_scratch_;

 निजी:
  bool parents_invalid = true;
  bool distance_invalid = true;
  bool positions_invalid = true;
  hb_vector_t<अचिन्हित> अंक_roots_for_space;
  hb_vector_t<अक्षर*> buffers;
};

}

#पूर्ण_अगर  // GRAPH_GRAPH_HH

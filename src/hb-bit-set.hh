/*
 * Copyright © 2012,2017  Google, Inc.
 * Copyright © 2021 Behdad Esfahbod
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
 * Google Author(s): Behdad Esfahbod
 */

#अगर_अघोषित HB_BIT_SET_HH
#घोषणा HB_BIT_SET_HH

#समावेश "hb.hh"
#समावेश "hb-bit-page.hh"

काष्ठा hb_bit_set_t
{
  hb_bit_set_t () = शेष;
  ~hb_bit_set_t () = शेष;

  hb_bit_set_t (स्थिर hb_bit_set_t& other) : hb_bit_set_t () { set (other, true); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*यह, other); }
  hb_bit_set_t& चालक= (स्थिर hb_bit_set_t& other) { set (other); वापस *यह; }
  hb_bit_set_t& चालक= (hb_bit_set_t&& other)  noexcept { hb_swap (*यह, other); वापस *यह; }
  मित्र व्योम swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    अगर (likely (!a.successful || !b.successful))
      वापस;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  व्योम init ()
  {
    successful = true;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  व्योम fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  काष्ठा page_map_t
  {
    पूर्णांक cmp (स्थिर page_map_t &o) स्थिर { वापस cmp (o.major); }
    पूर्णांक cmp (uint32_t o_major) स्थिर { वापस (पूर्णांक) o_major - (पूर्णांक) major; }

    uint32_t major;
    uint32_t सूचकांक_2;
  };

  bool successful = true; /* Allocations successful */
  mutable अचिन्हित पूर्णांक population = 0;
  mutable hb_atomic_t<अचिन्हित> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  व्योम err () { अगर (successful) successful = false; } /* TODO Remove */
  bool in_error () स्थिर { वापस !successful; }

  bool resize (अचिन्हित पूर्णांक गिनती, bool सफा = true, bool exact_आकार = false)
  {
    अगर (unlikely (!successful)) वापस false;

    अगर (pages.लंबाई < गिनती && (अचिन्हित) pages.allocated < गिनती && गिनती <= 2)
      exact_आकार = true; // Most sets are small and local

    अगर (unlikely (!pages.resize_full (गिनती, सफा, exact_आकार) ||
	!page_map.resize_full (गिनती, सफा, false)))
    {
      pages.resize_full (page_map.लंबाई, सफा, exact_आकार);
      successful = false;
      वापस false;
    }
    वापस true;
  }

  व्योम alloc (अचिन्हित sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = true;
    सफा ();
    वापस *यह;
  }

  व्योम सफा ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    अगर (!pages.लंबाई && !population) वापस;
    pages.सफा ();
    page_map.सफा ();
    population = 0;
  }
  bool is_empty () स्थिर
  {
    अचिन्हित पूर्णांक गिनती = pages.लंबाई;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      अगर (!pages[i].is_empty ())
	वापस false;
    वापस true;
  }
  explicit चालक bool () स्थिर { वापस !is_empty (); }

  uint32_t hash () स्थिर
  {
    uint32_t h = 0;
    क्रम (स्वतः &map : page_map)
    {
      स्वतः &page = pages.arrayZ[map.सूचकांक_2];
      अगर (unlikely (page.is_empty ())) जारी;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    वापस h;
  }

  निजी:
  व्योम dirty () { population = अच_पूर्णांक_उच्च; }
  खुला:

  व्योम add (hb_codepoint_t g)
  {
    अगर (unlikely (!successful)) वापस;
    अगर (unlikely (g == INVALID)) वापस;
    dirty ();
    page_t *page = page_for (g, true); अगर (unlikely (!page)) वापस;
    page->add (g);
  }
  व्योम add_bits (hb_codepoint_t g, uint64_t bits)
  {
    अगर (unlikely (!successful) || unlikely (!bits)) वापस;
    निश्चित (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); अगर (unlikely (!page)) वापस;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    अगर (unlikely (!successful)) वापस true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    अगर (unlikely (a > b || a == INVALID || b == INVALID)) वापस false;
    dirty ();
    अचिन्हित पूर्णांक ma = get_major (a);
    अचिन्हित पूर्णांक mb = get_major (b);
    अगर (ma == mb)
    {
      page_t *page = page_for (a, true); अगर (unlikely (!page)) वापस false;
      page->add_range (a, b);
    }
    अन्यथा
    {
      page_t *page = page_for (a, true); अगर (unlikely (!page)) वापस false;
      page->add_range (a, major_start (ma + 1) - 1);

      क्रम (अचिन्हित पूर्णांक m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); अगर (unlikely (!page)) वापस false;
	page->init1 ();
      }

      page = page_for (b, true); अगर (unlikely (!page)) वापस false;
      page->add_range (major_start (mb), b);
    }
    वापस true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  ढाँचा<typename Type>
  अटल अंतरभूत स्थिर Type& StructAtOffsetUnaligned(स्थिर व्योम *P, अचिन्हित पूर्णांक offset)
  {
#आशय GCC diagnostic push
#आशय GCC diagnostic ignored "-Wcast-align"
    वापस * reinterpret_cast<स्थिर Type*> ((स्थिर अक्षर *) P + offset);
#आशय GCC diagnostic निकालो
  }

  ढाँचा <typename T>
  व्योम set_array (bool v, स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  {
    अगर (unlikely (!successful)) वापस;
    अगर (!गिनती) वापस;
    dirty ();
    hb_codepoint_t g = *array;
    जबतक (गिनती)
    {
      अचिन्हित पूर्णांक m = get_major (g);
      page_t *page = page_for (g, v); अगर (unlikely (v && !page)) वापस;
      अचिन्हित पूर्णांक start = major_start (m);
      अचिन्हित पूर्णांक end = major_start (m + 1);
      करो
      {
        अगर (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	गिनती--;
      }
      जबतक (गिनती && (g = *array, start <= g && g < end));
    }
  }

  ढाँचा <typename T>
  व्योम add_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { set_array (true, array, गिनती, stride); }
  ढाँचा <typename T>
  व्योम add_array (स्थिर hb_array_t<स्थिर T>& arr) { add_array (&arr, arr.len ()); }

  ढाँचा <typename T>
  व्योम del_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { set_array (false, array, गिनती, stride); }
  ढाँचा <typename T>
  व्योम del_array (स्थिर hb_array_t<स्थिर T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  ढाँचा <typename T>
  bool set_sorted_array (bool v, स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  {
    अगर (unlikely (!successful)) वापस true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    अगर (unlikely (!गिनती)) वापस true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    जबतक (गिनती)
    {
      अचिन्हित पूर्णांक m = get_major (g);
      page_t *page = page_for (g, v); अगर (unlikely (v && !page)) वापस false;
      अचिन्हित पूर्णांक end = major_start (m + 1);
      करो
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	अगर (g < last_g) वापस false;
	last_g = g;

        अगर (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	गिनती--;
      }
      जबतक (गिनती && (g = *array, g < end));
    }
    वापस true;
  }

  ढाँचा <typename T>
  bool add_sorted_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { वापस set_sorted_array (true, array, गिनती, stride); }
  ढाँचा <typename T>
  bool add_sorted_array (स्थिर hb_sorted_array_t<स्थिर T>& arr) { वापस add_sorted_array (&arr, arr.len ()); }

  ढाँचा <typename T>
  bool del_sorted_array (स्थिर T *array, अचिन्हित पूर्णांक गिनती, अचिन्हित पूर्णांक stride=माप(T))
  { वापस set_sorted_array (false, array, गिनती, stride); }
  ढाँचा <typename T>
  bool del_sorted_array (स्थिर hb_sorted_array_t<स्थिर T>& arr) { वापस del_sorted_array (&arr, arr.len ()); }

  व्योम del (hb_codepoint_t g)
  {
    अगर (unlikely (!successful)) वापस;
    page_t *page = page_for (g);
    अगर (!page)
      वापस;
    dirty ();
    page->del (g);
  }
  व्योम del_bits (hb_codepoint_t g, uint64_t bits)
  {
    अगर (unlikely (!successful) || unlikely (!bits)) वापस;
    निश्चित (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    अगर (!page)
      वापस;
    dirty ();
    page->del_bits (g, bits);
  }

  निजी:
  व्योम del_pages (पूर्णांक ds, पूर्णांक de)
  {
    अगर (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<अचिन्हित> compact_workspace;
      अगर (unlikely (!allocate_compact_workspace (compact_workspace))) वापस;

      अचिन्हित पूर्णांक write_सूचकांक = 0;
      क्रम (अचिन्हित पूर्णांक i = 0; i < page_map.लंबाई; i++)
      {
	पूर्णांक m = (पूर्णांक) page_map.arrayZ[i].major;
	अगर (m < ds || de < m)
	  page_map.arrayZ[write_सूचकांक++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_सूचकांक);
      resize (write_सूचकांक);
    }
  }


  खुला:
  व्योम del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    अगर (unlikely (!successful)) वापस;
    अगर (unlikely (a > b || a == INVALID)) वापस;
    dirty ();
    अचिन्हित पूर्णांक ma = get_major (a);
    अचिन्हित पूर्णांक mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    पूर्णांक ds = (a == major_start (ma))? (पूर्णांक) ma: (पूर्णांक) (ma + 1);
    पूर्णांक de = (b + 1 == major_start (mb + 1))? (पूर्णांक) mb: ((पूर्णांक) mb - 1);
    अगर (ds > de || (पूर्णांक) ma < ds)
    {
      page_t *page = page_for (a);
      अगर (page)
      {
	अगर (ma == mb)
	  page->del_range (a, b);
	अन्यथा
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    अगर (de < (पूर्णांक) mb && ma != mb)
    {
      page_t *page = page_for (b);
      अगर (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool लो (hb_codepoint_t g) स्थिर
  {
    स्थिर page_t *page = page_for (g);
    अगर (!page)
      वापस false;
    वापस page->लो (g);
  }
  bool may_have (hb_codepoint_t g) स्थिर { वापस लो (g); }

  /* Has interface. */
  bool चालक [] (hb_codepoint_t k) स्थिर { वापस लो (k); }
  bool has (hb_codepoint_t k) स्थिर { वापस (*यह)[k]; }
  /* Predicate. */
  bool चालक () (hb_codepoint_t k) स्थिर { वापस has (k); }

  /* Sink interface. */
  hb_bit_set_t& चालक << (hb_codepoint_t v)
  { add (v); वापस *यह; }
  hb_bit_set_t& चालक << (स्थिर hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); वापस *यह; }

  bool intersects (स्थिर hb_bit_set_t &other) स्थिर
  {
    अचिन्हित पूर्णांक na = pages.लंबाई;
    अचिन्हित पूर्णांक nb = other.pages.लंबाई;

    अचिन्हित पूर्णांक a = 0, b = 0;
    क्रम (; a < na && b < nb; )
    {
      अगर (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	अगर (page_at (a).intersects (other.page_at (b)))
	  वापस true;
	a++;
	b++;
      }
      अन्यथा अगर (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      अन्यथा
	b++;
    }
    वापस false;
  }
  bool may_intersect (स्थिर hb_bit_set_t &other) स्थिर
  { वापस intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) स्थिर
  {
    hb_codepoint_t c = first - 1;
    वापस next (&c) && c <= last;
  }
  व्योम set (स्थिर hb_bit_set_t &other, bool exact_आकार = false)
  {
    अगर (unlikely (!successful)) वापस;
    अचिन्हित पूर्णांक गिनती = other.pages.लंबाई;
    अगर (unlikely (!resize  (गिनती, false, exact_आकार)))
      वापस;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (स्थिर hb_bit_set_t &other) स्थिर
  {
    अगर (has_population () && other.has_population () &&
	population != other.population)
      वापस false;

    अचिन्हित पूर्णांक na = pages.लंबाई;
    अचिन्हित पूर्णांक nb = other.pages.लंबाई;

    अचिन्हित पूर्णांक a = 0, b = 0;
    क्रम (; a < na && b < nb; )
    {
      अगर (page_at (a).is_empty ()) { a++; जारी; }
      अगर (other.page_at (b).is_empty ()) { b++; जारी; }
      अगर (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	वापस false;
      a++;
      b++;
    }
    क्रम (; a < na; a++)
      अगर (!page_at (a).is_empty ()) { वापस false; }
    क्रम (; b < nb; b++)
      अगर (!other.page_at (b).is_empty ()) { वापस false; }

    वापस true;
  }

  bool is_subset (स्थिर hb_bit_set_t &larger_set) स्थिर
  {
    अगर (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      वापस false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    जबतक (spi < page_map.लंबाई && lpi < larger_set.page_map.लंबाई)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      स्वतः sp = page_at (spi);

      अगर (spm < lpm) {
        अगर (!sp.is_empty ())
          वापस false;
        spi++;
        जारी;
      }

      अगर (lpm < spm) {
        lpi++;
        जारी;
      }

      स्वतः lp = larger_set.page_at (lpi);
      अगर (!sp.is_subset (lp))
        वापस false;

      spi++;
      lpi++;
    }

    जबतक (spi < page_map.लंबाई)
      अगर (!page_at (spi++).is_empty ())
        वापस false;

    वापस true;
  }

  निजी:
  bool allocate_compact_workspace (hb_vector_t<अचिन्हित>& workspace)
  {
    अगर (unlikely (!workspace.resize_exact (pages.लंबाई)))
    {
      successful = false;
      वापस false;
    }

    वापस true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  व्योम compact (hb_vector_t<अचिन्हित>& workspace,
                अचिन्हित पूर्णांक लंबाई)
  {
    निश्चित(workspace.लंबाई == pages.लंबाई);
    hb_vector_t<अचिन्हित>& old_सूचकांक_to_page_map_सूचकांक = workspace;

    hb_fill (old_सूचकांक_to_page_map_सूचकांक.writer(), 0xFFFFFFFF);
    क्रम (अचिन्हित i = 0; i < लंबाई; i++)
      old_सूचकांक_to_page_map_सूचकांक[page_map[i].सूचकांक_2] =  i;

    compact_pages (old_सूचकांक_to_page_map_सूचकांक);
  }
  व्योम compact_pages (स्थिर hb_vector_t<अचिन्हित>& old_सूचकांक_to_page_map_सूचकांक)
  {
    अचिन्हित पूर्णांक write_सूचकांक = 0;
    क्रम (अचिन्हित पूर्णांक i = 0; i < pages.लंबाई; i++)
    {
      अगर (old_सूचकांक_to_page_map_सूचकांक[i] == 0xFFFFFFFF) जारी;

      अगर (write_सूचकांक < i)
	pages[write_सूचकांक] = pages[i];

      page_map[old_सूचकांक_to_page_map_सूचकांक[i]].सूचकांक_2 = write_सूचकांक;
      write_सूचकांक++;
    }
  }
  खुला:

  व्योम process_ (hb_bit_page_t::vector_t (*op) (स्थिर hb_bit_page_t::vector_t &, स्थिर hb_bit_page_t::vector_t &),
		 bool passthru_बायाँ, bool passthru_दायाँ,
		 स्थिर hb_bit_set_t &other,
		 hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    अगर (unlikely (!successful)) वापस;

    dirty ();

    अचिन्हित पूर्णांक na = pages.लंबाई;
    अचिन्हित पूर्णांक nb = other.pages.लंबाई;
    अचिन्हित पूर्णांक अग्र_page = na;

    अचिन्हित पूर्णांक गिनती = 0, new_गिनती = 0;
    अचिन्हित पूर्णांक a = 0, b = 0;
    अचिन्हित पूर्णांक write_सूचकांक = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<अचिन्हित> local_workspace;
    hb_vector_t<अचिन्हित> &compact_workspace = workspace ? *workspace : local_workspace;
    अगर (!passthru_बायाँ)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.लंबाई)
				 : allocate_compact_workspace (compact_workspace);
      अगर (unlikely (!allocated))
      {
	successful = false;
	वापस;
      }
    }

    क्रम (; a < na && b < nb; )
    {
      अगर (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	अगर (!passthru_बायाँ)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  अगर (write_सूचकांक < a)
	    page_map.arrayZ[write_सूचकांक] = page_map.arrayZ[a];
	  write_सूचकांक++;
	}

	गिनती++;
	a++;
	b++;
      }
      अन्यथा अगर (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	अगर (passthru_बायाँ)
	  गिनती++;
	a++;
      }
      अन्यथा
      {
	अगर (passthru_दायाँ)
	  गिनती++;
	b++;
      }
    }
    अगर (passthru_बायाँ)
      गिनती += na - a;
    अगर (passthru_दायाँ)
      गिनती += nb - b;

    अगर (!passthru_बायाँ)
    {
      na  = write_सूचकांक;
      अग्र_page = write_सूचकांक;
      compact (compact_workspace, write_सूचकांक);
    }

    अगर (unlikely (!resize (गिनती)))
      वापस;

    new_गिनती = गिनती;

    /* Process in-place backward. */
    a = na;
    b = nb;
    क्रम (; a && b; )
    {
      अगर (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	गिनती--;
	page_map.arrayZ[गिनती] = page_map.arrayZ[a];
	page_at (गिनती).v = op (page_at (a).v, other.page_at (b).v);
	page_at (गिनती).dirty ();
      }
      अन्यथा अगर (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	अगर (passthru_बायाँ)
	{
	  गिनती--;
	  page_map.arrayZ[गिनती] = page_map.arrayZ[a];
	}
      }
      अन्यथा
      {
	b--;
	अगर (passthru_दायाँ)
	{
	  गिनती--;
	  page_map.arrayZ[गिनती].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[गिनती].सूचकांक_2 = अग्र_page++;
	  page_at (गिनती) = other.page_at (b);
	}
      }
    }
    अगर (passthru_बायाँ)
      जबतक (a)
      {
	a--;
	गिनती--;
	page_map.arrayZ[गिनती] = page_map.arrayZ[a];
      }
    अगर (passthru_दायाँ)
      जबतक (b)
      {
	b--;
	गिनती--;
	page_map.arrayZ[गिनती].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[गिनती].सूचकांक_2 = अग्र_page++;
	page_at (गिनती) = other.page_at (b);
      }
    निश्चित (!गिनती);
    resize (new_गिनती);
  }
  ढाँचा <typename Op>
  अटल hb_bit_page_t::vector_t
  op_ (स्थिर hb_bit_page_t::vector_t &a, स्थिर hb_bit_page_t::vector_t &b)
  { वापस Op{} (a, b); }
  ढाँचा <typename Op>
  व्योम process (स्थिर Op& op,
		स्थिर hb_bit_set_t &other,
		hb_vector_t<अचिन्हित> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  व्योम union_ (स्थिर hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  व्योम intersect (स्थिर hb_bit_set_t &other,
		  hb_vector_t<अचिन्हित> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  व्योम subtract (स्थिर hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  व्योम symmetric_difference (स्थिर hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  bool next (hb_codepoint_t *codepoint) स्थिर
  {
    अगर (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      वापस *codepoint != INVALID;
    }

    स्थिर स्वतः* page_map_array = page_map.arrayZ;
    अचिन्हित पूर्णांक major = get_major (*codepoint);
    अचिन्हित पूर्णांक i = last_page_lookup;

    अगर (unlikely (i >= page_map.लंबाई || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      अगर (i >= page_map.लंबाई) {
        *codepoint = INVALID;
        वापस false;
      }
      last_page_lookup = i;
    }

    स्थिर स्वतः* pages_array = pages.arrayZ;
    स्थिर page_map_t &current = page_map_array[i];
    अगर (likely (current.major == major))
    {
      अगर (pages_array[current.सूचकांक_2].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        वापस true;
      }
      i++;
    }

    क्रम (; i < page_map.लंबाई; i++)
    {
      स्थिर page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.सूचकांक_2].get_min ();
      अगर (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	वापस true;
      }
    }
    *codepoint = INVALID;
    वापस false;
  }
  bool अग्र_bits (hb_codepoint_t *codepoint, uint64_t *bits) स्थिर
  {
    static_assert (page_t::ELT_BITS == 64, "");

    अचिन्हित पूर्णांक i = 0;
    अचिन्हित पूर्णांक elt = 0;
    अगर (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      अगर (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        जाओ done;
      base += page_t::ELT_BITS;

      अचिन्हित पूर्णांक major = get_major (base);
      i = last_page_lookup;
      अगर (unlikely (i >= page_map.लंबाई || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	अगर (i >= page_map.लंबाई)
	  जाओ done;
      }
      अगर (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    क्रम (; i < page_map.लंबाई; i++, elt = 0)
    {
      स्थिर page_map_t &map = page_map.arrayZ[i];
      स्थिर page_t &page = pages.arrayZ[map.सूचकांक_2];
      क्रम (; elt < page_t::len (); elt++)
	अगर (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  वापस true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    वापस false;
  }
  bool previous (hb_codepoint_t *codepoint) स्थिर
  {
    अगर (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      वापस *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    अचिन्हित पूर्णांक i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    अगर (i < page_map.लंबाई && page_map.arrayZ[i].major == map.major)
    {
      अगर (pages[page_map.arrayZ[i].सूचकांक_2].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	वापस true;
      }
    }
    i--;
    क्रम (; (पूर्णांक) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].सूचकांक_2].get_max ();
      अगर (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	वापस true;
      }
    }
    *codepoint = INVALID;
    वापस false;
  }
  bool अग्र_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिर
  {
    hb_codepoint_t i;

    i = *last;
    अगर (!next (&i))
    {
      *last = *first = INVALID;
      वापस false;
    }

    *last = *first = i;

    स्थिर स्वतः* page_map_array = page_map.arrayZ;
    स्थिर स्वतः* pages_array = pages.arrayZ;
    अचिन्हित पूर्णांक major = get_major (*last);
    अचिन्हित पूर्णांक p_सूचकांक = last_page_lookup;

    अगर (unlikely (p_सूचकांक >= page_map.लंबाई || page_map_array[p_सूचकांक].major != major))
    {
      page_map.bfind (major, &p_सूचकांक, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      निश्चित(p_सूचकांक < page_map.लंबाई && page_map_array[p_सूचकांक].major == major);
      last_page_lookup = p_सूचकांक;
    }

    अचिन्हित पूर्णांक rem = page_remainder (*last);
    अचिन्हित पूर्णांक elt = rem / page_t::ELT_BITS;
    अचिन्हित पूर्णांक bit = rem & page_t::ELT_MASK;

    स्थिर page_t *page = &pages_array[page_map_array[p_सूचकांक].सूचकांक_2];
    अचिन्हित पूर्णांक run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    अगर (run < page_t::ELT_BITS - bit)
      वापस true;

    elt++;

    जबतक (true)
    {
      क्रम (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        अगर (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          जारी;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        वापस true;
      }

      p_सूचकांक++;


      अगर (p_सूचकांक >= page_map.लंबाई || page_map_array[p_सूचकांक].major != major + 1)
        वापस true;

      स्थिर स्वतः& entry = page_map_array[p_सूचकांक];
      major = entry.major;
      last_page_lookup = p_सूचकांक;
      page = &pages_array[entry.सूचकांक_2];
      elt = 0;
    }
  }

  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) स्थिर
  {
    hb_codepoint_t i;

    i = *first;
    अगर (!previous (&i))
    {
      *last = *first = INVALID;
      वापस false;
    }

    *last = *first = i;

    स्थिर स्वतः* page_map_array = page_map.arrayZ;
    स्थिर स्वतः* pages_array = pages.arrayZ;
    अचिन्हित पूर्णांक major = get_major (*first);
    अचिन्हित पूर्णांक p_सूचकांक = last_page_lookup;

    अगर (unlikely (p_सूचकांक >= page_map.लंबाई || page_map_array[p_सूचकांक].major != major))
    {
      page_map.bfind (major, &p_सूचकांक, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      निश्चित(p_सूचकांक < page_map.लंबाई && page_map_array[p_सूचकांक].major == major);
      last_page_lookup = p_सूचकांक;
    }

    अचिन्हित पूर्णांक rem = page_remainder (*first);
    पूर्णांक elt = rem / page_t::ELT_BITS;
    अचिन्हित पूर्णांक bit = rem & page_t::ELT_MASK;

    स्थिर page_t *page = &pages_array[page_map_array[p_सूचकांक].सूचकांक_2];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    अचिन्हित पूर्णांक run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    अगर (run < bit + 1)
      वापस true;

    elt--;

    जबतक (true)
    {
      क्रम (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        अगर (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          जारी;
        }
        uint64_t inv = ~w;
        अचिन्हित पूर्णांक zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        वापस true;
      }


      अगर (p_सूचकांक == 0 || page_map_array[p_सूचकांक - 1].major != major - 1)
        वापस true;

      p_सूचकांक--;
      स्थिर स्वतः& entry = page_map_array[p_सूचकांक];

      major = entry.major;
      last_page_lookup = p_सूचकांक;
      page = &pages_array[entry.सूचकांक_2];
      elt = page_t::len () - 1;
    }
  }

  अचिन्हित पूर्णांक अग्र_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  अचिन्हित पूर्णांक    आकार) स्थिर
  {
    // By default, start at the first bit of the first page of values.
    अचिन्हित पूर्णांक start_page = 0;
    अचिन्हित पूर्णांक start_page_मूल्य = 0;
    अगर (unlikely (codepoint != INVALID))
    {
      स्थिर स्वतः* page_map_array = page_map.arrayZ;
      अचिन्हित पूर्णांक major = get_major (codepoint);
      अचिन्हित पूर्णांक i = last_page_lookup;
      अगर (unlikely (i >= page_map.लंबाई || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	अगर (i >= page_map.लंबाई)
	  वापस 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      अगर (page_map_array[i].major == major)
      {
        start_page_मूल्य = page_remainder (codepoint + 1);
        अगर (unlikely (start_page_मूल्य == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मूल्य = 0;
        }
      }
    }

    अचिन्हित पूर्णांक initial_आकार = आकार;
    क्रम (अचिन्हित पूर्णांक i = start_page; i < page_map.लंबाई && आकार; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      अचिन्हित पूर्णांक n = pages[page_map.arrayZ[i].सूचकांक_2].ग_लिखो (base, start_page_मूल्य, out, आकार);
      out += n;
      आकार -= n;
      start_page_मूल्य = 0;
    }
    वापस initial_आकार - आकार;
  }

  अचिन्हित पूर्णांक अग्र_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   अचिन्हित पूर्णांक    आकार) स्थिर
  {
    अचिन्हित पूर्णांक initial_आकार = आकार;
    // By default, start at the first bit of the first page of values.
    अचिन्हित पूर्णांक start_page = 0;
    अचिन्हित पूर्णांक start_page_मूल्य = 0;
    अगर (unlikely (codepoint != INVALID))
    {
      स्थिर स्वतः* page_map_array = page_map.arrayZ;
      अचिन्हित पूर्णांक major = get_major (codepoint);
      अचिन्हित पूर्णांक i = last_page_lookup;
      अगर (unlikely (i >= page_map.लंबाई || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        अगर (unlikely (i >= page_map.लंबाई))
        {
          // codepoint is greater than our max element.
          जबतक (++codepoint != INVALID && आकार)
          {
            *out++ = codepoint;
            आकार--;
          }
          वापस initial_आकार - आकार;
        }
      }
      start_page = i;
      अगर (page_map_array[i].major == major)
      {
        start_page_मूल्य = page_remainder (codepoint + 1);
        अगर (unlikely (start_page_मूल्य == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मूल्य = 0;
        }
      }
    }

    hb_codepoint_t अग्र_मूल्य = codepoint + 1;
    क्रम (अचिन्हित पूर्णांक i=start_page; i<page_map.लंबाई && आकार; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      अचिन्हित पूर्णांक n = pages[page_map.arrayZ[i].सूचकांक_2].write_inverted (base, start_page_मूल्य, out, आकार, &अग्र_मूल्य);
      out += n;
      आकार -= n;
      start_page_मूल्य = 0;
    }
    जबतक (अग्र_मूल्य < HB_SET_मूल्य_INVALID && आकार) {
      *out++ = अग्र_मूल्य++;
      आकार--;
    }
    वापस initial_आकार - आकार;
  }

  bool has_population () स्थिर { वापस population != अच_पूर्णांक_उच्च; }
  अचिन्हित पूर्णांक get_population () स्थिर
  {
    अगर (has_population ())
      वापस population;

    अचिन्हित पूर्णांक निकालो = 0;
    अचिन्हित पूर्णांक गिनती = pages.लंबाई;
    क्रम (अचिन्हित पूर्णांक i = 0; i < गिनती; i++)
      निकालो += pages[i].get_population ();

    population = निकालो;
    वापस निकालो;
  }
  bool get_singleton (hb_codepoint_t *codepoint) स्थिर
  {
    अगर (has_population ())
    {
      अगर (population != 1) वापस false;
      *codepoint = get_min ();
      वापस true;
    }

    hb_codepoint_t singleton = INVALID;
    क्रम (स्थिर स्वतः &map : page_map)
    {
      स्थिर स्वतः &page = pages.arrayZ[map.सूचकांक_2];
      क्रम (अचिन्हित i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	अगर (!bits) जारी;
	अगर (singleton != INVALID || (bits & (bits - 1)))
	  वापस false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    अगर (singleton == INVALID)
    {
      population = 0;
      वापस false;
    }
    population = 1;
    *codepoint = singleton;
    वापस true;
  }
  hb_codepoint_t get_min () स्थिर
  {
    अचिन्हित गिनती = pages.लंबाई;
    क्रम (अचिन्हित i = 0; i < गिनती; i++)
    {
      स्थिर स्वतः& map = page_map.arrayZ[i];
      स्थिर स्वतः& page = pages.arrayZ[map.सूचकांक_2];

      अगर (!page.is_empty ())
	वापस map.major * page_t::PAGE_BITS + page.get_min ();
    }
    वापस INVALID;
  }
  hb_codepoint_t get_max () स्थिर
  {
    अचिन्हित गिनती = pages.लंबाई;
    क्रम (चिन्हित i = गिनती - 1; i >= 0; i--)
    {
      स्थिर स्वतः& map = page_map.arrayZ[(अचिन्हित) i];
      स्थिर स्वतः& page = pages.arrayZ[map.सूचकांक_2];

      अगर (!page.is_empty ())
	वापस map.major * page_t::PAGE_BITS + page.get_max ();
    }
    वापस INVALID;
  }

  अटल constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  काष्ठा iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    अटल constexpr bool is_sorted_iterator = true;
    अटल constexpr bool has_fast_len = true;
    iter_t (स्थिर hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    bool init = true) : s (&s_), v (INVALID), l(0)
    {
      अगर (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    प्रकार hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () स्थिर { वापस v; }
    bool __more__ () स्थिर { वापस v != INVALID; }
    व्योम __next__ () { s->next (&v); अगर (l) l--; }
    व्योम __prev__ () { s->previous (&v); }
    अचिन्हित __len__ () स्थिर { वापस l; }
    iter_t end () स्थिर { वापस iter_t (*s, false); }
    bool चालक != (स्थिर iter_t& o) स्थिर
    { वापस v != o.v; }

    रक्षित:
    स्थिर hb_bit_set_t *s;
    hb_codepoint_t v;
    अचिन्हित l;
  };
  iter_t iter () स्थिर { वापस iter_t (*यह); }
  चालक iter_t () स्थिर { वापस iter (); }

  रक्षित:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    अचिन्हित major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    अचिन्हित i = last_page_lookup;
    अगर (likely (i < page_map.लंबाई))
    {
      स्वतः &cached_page = page_map.arrayZ[i];
      अगर (cached_page.major == major)
	वापस &pages.arrayZ[cached_page.सूचकांक_2];
    }

    page_map_t map = {major, pages.लंबाई};
    अगर (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      अगर (!insert)
        वापस nullptr;

      अगर (unlikely (!resize (pages.लंबाई + 1)))
	वापस nullptr;

      pages.arrayZ[map.सूचकांक_2].init0 ();
      स_हटाओ (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.लंबाई - 1 - i) * page_map.item_आकार);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    वापस &pages.arrayZ[page_map.arrayZ[i].सूचकांक_2];
  }
  स्थिर page_t *page_for (hb_codepoint_t g) स्थिर
  {
    अचिन्हित major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    अचिन्हित i = last_page_lookup;
    अगर (likely (i < page_map.लंबाई))
    {
      स्वतः &cached_page = page_map.arrayZ[i];
      अगर (cached_page.major == major)
	वापस &pages.arrayZ[cached_page.सूचकांक_2];
    }

    page_map_t key = {major};
    अगर (!page_map.bfind (key, &i))
      वापस nullptr;

    last_page_lookup = i;
    वापस &pages.arrayZ[page_map.arrayZ[i].सूचकांक_2];
  }
  page_t &page_at (अचिन्हित पूर्णांक i)
  {
    निश्चित (i < page_map.लंबाई);
    वापस pages.arrayZ[page_map.arrayZ[i].सूचकांक_2];
  }
  स्थिर page_t &page_at (अचिन्हित पूर्णांक i) स्थिर
  {
    निश्चित (i < page_map.लंबाई);
    वापस pages.arrayZ[page_map.arrayZ[i].सूचकांक_2];
  }
  अचिन्हित पूर्णांक get_major (hb_codepoint_t g) स्थिर { वापस g >> page_t::PAGE_BITS_LOG_2; }
  अचिन्हित पूर्णांक page_remainder (hb_codepoint_t g) स्थिर { वापस g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (अचिन्हित पूर्णांक major) स्थिर { वापस major << page_t::PAGE_BITS_LOG_2; }
};


#पूर्ण_अगर /* HB_BIT_SET_HH */

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

#ifndef HB_BIT_SET_HH
#define HB_BIT_SET_HH

#अन्तर्भाव "hb.hh"
#अन्तर्भाव "hb-bit-page.hh"

संरचना hb_bit_set_t
{
  hb_bit_set_t () = स्वभाव;
  ~hb_bit_set_t () = स्वभाव;

  hb_bit_set_t (ध्रुव hb_bit_set_t& other) : hb_bit_set_t () { set (other, सच्च); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*एतत्, other); }
  hb_bit_set_t& operator= (ध्रुव hb_bit_set_t& other) { set (other); पटिनिवत्त *एतत्; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*एतत्, other); पटिनिवत्त *एतत्; }
  friend शून्य swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    सचे (likely (!a.successful || !b.successful))
      पटिनिवत्त;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  शून्य init ()
  {
    successful = सच्च;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  शून्य fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  संरचना page_map_t
  {
    पूर्णाङ्क cmp (ध्रुव page_map_t &o) ध्रुव { पटिनिवत्त cmp (o.major); }
    पूर्णाङ्क cmp (uint32_t o_major) ध्रुव { पटिनिवत्त (पूर्णाङ्क) o_major - (पूर्णाङ्क) major; }

    uint32_t major;
    uint32_t index;
  };

  तार्किक successful = सच्च; /* Allocations successful */
  mutable unsigned पूर्णाङ्क population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  शून्य err () { सचे (successful) successful = मिच्छा; } /* TODO Remove */
  तार्किक in_error () ध्रुव { पटिनिवत्त !successful; }

  तार्किक resize (unsigned पूर्णाङ्क गणनम्, तार्किक clear = सच्च, तार्किक exact_size = मिच्छा)
  {
    सचे (unlikely (!successful)) पटिनिवत्त मिच्छा;

    सचे (pages.length < गणनम् && (unsigned) pages.allocated < गणनम् && गणनम् <= 2)
      exact_size = सच्च; // Most sets are small and local

    सचे (unlikely (!pages.resize_full (गणनम्, clear, exact_size) ||
	!page_map.resize_full (गणनम्, clear, मिच्छा)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = मिच्छा;
      पटिनिवत्त मिच्छा;
    }
    पटिनिवत्त सच्च;
  }

  शून्य alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = सच्च;
    clear ();
    पटिनिवत्त *एतत्;
  }

  शून्य clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    सचे (!pages.length && !population) पटिनिवत्त;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  तार्किक is_empty () ध्रुव
  {
    unsigned पूर्णाङ्क गणनम् = pages.length;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      सचे (!pages[i].is_empty ())
	पटिनिवत्त मिच्छा;
    पटिनिवत्त सच्च;
  }
  explicit operator तार्किक () ध्रुव { पटिनिवत्त !is_empty (); }

  uint32_t hash () ध्रुव
  {
    uint32_t h = 0;
    अत्थाय (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      सचे (unlikely (page.is_empty ())) अनुवर्त;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    पटिनिवत्त h;
  }

  गुप्त:
  शून्य dirty () { population = UINT_MAX; }
  सार्वजनिक:

  शून्य add (hb_codepoint_t g)
  {
    सचे (unlikely (!successful)) पटिनिवत्त;
    सचे (unlikely (g == INVALID)) पटिनिवत्त;
    dirty ();
    page_t *page = page_for (g, सच्च); सचे (unlikely (!page)) पटिनिवत्त;
    page->add (g);
  }
  शून्य add_bits (hb_codepoint_t g, uint64_t bits)
  {
    सचे (unlikely (!successful) || unlikely (!bits)) पटिनिवत्त;
    प्रतिज्ञा (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, सच्च); सचे (unlikely (!page)) पटिनिवत्त;
    page->add_bits (g, bits);
  }
  तार्किक add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    सचे (unlikely (!successful)) पटिनिवत्त सच्च; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    सचे (unlikely (a > b || a == INVALID || b == INVALID)) पटिनिवत्त मिच्छा;
    dirty ();
    unsigned पूर्णाङ्क ma = get_major (a);
    unsigned पूर्णाङ्क mb = get_major (b);
    सचे (ma == mb)
    {
      page_t *page = page_for (a, सच्च); सचे (unlikely (!page)) पटिनिवत्त मिच्छा;
      page->add_range (a, b);
    }
    अञ्ञथा
    {
      page_t *page = page_for (a, सच्च); सचे (unlikely (!page)) पटिनिवत्त मिच्छा;
      page->add_range (a, major_start (ma + 1) - 1);

      अत्थाय (unsigned पूर्णाङ्क m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), सच्च); सचे (unlikely (!page)) पटिनिवत्त मिच्छा;
	page->init1 ();
      }

      page = page_for (b, सच्च); सचे (unlikely (!page)) पटिनिवत्त मिच्छा;
      page->add_range (major_start (mb), b);
    }
    पटिनिवत्त सच्च;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  स्थिर अन्तःस्थ ध्रुव Type& StructAtOffsetUnaligned(ध्रुव शून्य *P, unsigned पूर्णाङ्क offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    पटिनिवत्त * reinterpret_cast<ध्रुव Type*> ((ध्रुव वर्ण *) P + offset);
#pragma GCC diagnostic निष्कासय
  }

  template <typename T>
  शून्य set_array (तार्किक v, ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  {
    सचे (unlikely (!successful)) पटिनिवत्त;
    सचे (!गणनम्) पटिनिवत्त;
    dirty ();
    hb_codepoint_t g = *array;
    याव (गणनम्)
    {
      unsigned पूर्णाङ्क m = get_major (g);
      page_t *page = page_for (g, v); सचे (unlikely (v && !page)) पटिनिवत्त;
      unsigned पूर्णाङ्क start = major_start (m);
      unsigned पूर्णाङ्क end = major_start (m + 1);
      करोहि
      {
        सचे (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	गणनम्--;
      }
      याव (गणनम् && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  शून्य add_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { set_array (सच्च, array, गणनम्, stride); }
  template <typename T>
  शून्य add_array (ध्रुव hb_array_t<ध्रुव T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  शून्य del_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { set_array (मिच्छा, array, गणनम्, stride); }
  template <typename T>
  शून्य del_array (ध्रुव hb_array_t<ध्रुव T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  तार्किक set_sorted_array (तार्किक v, ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  {
    सचे (unlikely (!successful)) पटिनिवत्त सच्च; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    सचे (unlikely (!गणनम्)) पटिनिवत्त सच्च;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    याव (गणनम्)
    {
      unsigned पूर्णाङ्क m = get_major (g);
      page_t *page = page_for (g, v); सचे (unlikely (v && !page)) पटिनिवत्त मिच्छा;
      unsigned पूर्णाङ्क end = major_start (m + 1);
      करोहि
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	सचे (g < last_g) पटिनिवत्त मिच्छा;
	last_g = g;

        सचे (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	गणनम्--;
      }
      याव (गणनम् && (g = *array, g < end));
    }
    पटिनिवत्त सच्च;
  }

  template <typename T>
  तार्किक add_sorted_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { पटिनिवत्त set_sorted_array (सच्च, array, गणनम्, stride); }
  template <typename T>
  तार्किक add_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { पटिनिवत्त add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  तार्किक del_sorted_array (ध्रुव T *array, unsigned पूर्णाङ्क गणनम्, unsigned पूर्णाङ्क stride=परिमाण(T))
  { पटिनिवत्त set_sorted_array (मिच्छा, array, गणनम्, stride); }
  template <typename T>
  तार्किक del_sorted_array (ध्रुव hb_sorted_array_t<ध्रुव T>& arr) { पटिनिवत्त del_sorted_array (&arr, arr.len ()); }

  शून्य del (hb_codepoint_t g)
  {
    सचे (unlikely (!successful)) पटिनिवत्त;
    page_t *page = page_for (g);
    सचे (!page)
      पटिनिवत्त;
    dirty ();
    page->del (g);
  }
  शून्य del_bits (hb_codepoint_t g, uint64_t bits)
  {
    सचे (unlikely (!successful) || unlikely (!bits)) पटिनिवत्त;
    प्रतिज्ञा (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    सचे (!page)
      पटिनिवत्त;
    dirty ();
    page->del_bits (g, bits);
  }

  गुप्त:
  शून्य del_pages (पूर्णाङ्क ds, पूर्णाङ्क de)
  {
    सचे (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      सचे (unlikely (!allocate_compact_workspace (compact_workspace))) पटिनिवत्त;

      unsigned पूर्णाङ्क write_index = 0;
      अत्थाय (unsigned पूर्णाङ्क i = 0; i < page_map.length; i++)
      {
	पूर्णाङ्क m = (पूर्णाङ्क) page_map.arrayZ[i].major;
	सचे (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  सार्वजनिक:
  शून्य del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    सचे (unlikely (!successful)) पटिनिवत्त;
    सचे (unlikely (a > b || a == INVALID)) पटिनिवत्त;
    dirty ();
    unsigned पूर्णाङ्क ma = get_major (a);
    unsigned पूर्णाङ्क mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    पूर्णाङ्क ds = (a == major_start (ma))? (पूर्णाङ्क) ma: (पूर्णाङ्क) (ma + 1);
    पूर्णाङ्क de = (b + 1 == major_start (mb + 1))? (पूर्णाङ्क) mb: ((पूर्णाङ्क) mb - 1);
    सचे (ds > de || (पूर्णाङ्क) ma < ds)
    {
      page_t *page = page_for (a);
      सचे (page)
      {
	सचे (ma == mb)
	  page->del_range (a, b);
	अञ्ञथा
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    सचे (de < (पूर्णाङ्क) mb && ma != mb)
    {
      page_t *page = page_for (b);
      सचे (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  तार्किक get (hb_codepoint_t g) ध्रुव
  {
    ध्रुव page_t *page = page_for (g);
    सचे (!page)
      पटिनिवत्त मिच्छा;
    पटिनिवत्त page->get (g);
  }
  तार्किक may_have (hb_codepoint_t g) ध्रुव { पटिनिवत्त get (g); }

  /* Has interface. */
  तार्किक operator [] (hb_codepoint_t k) ध्रुव { पटिनिवत्त get (k); }
  तार्किक has (hb_codepoint_t k) ध्रुव { पटिनिवत्त (*एतत्)[k]; }
  /* Predicate. */
  तार्किक operator () (hb_codepoint_t k) ध्रुव { पटिनिवत्त has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); पटिनिवत्त *एतत्; }
  hb_bit_set_t& operator << (ध्रुव hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); पटिनिवत्त *एतत्; }

  तार्किक intersects (ध्रुव hb_bit_set_t &other) ध्रुव
  {
    unsigned पूर्णाङ्क na = pages.length;
    unsigned पूर्णाङ्क nb = other.pages.length;

    unsigned पूर्णाङ्क a = 0, b = 0;
    अत्थाय (; a < na && b < nb; )
    {
      सचे (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	सचे (page_at (a).intersects (other.page_at (b)))
	  पटिनिवत्त सच्च;
	a++;
	b++;
      }
      अञ्ञथा सचे (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      अञ्ञथा
	b++;
    }
    पटिनिवत्त मिच्छा;
  }
  तार्किक may_intersect (ध्रुव hb_bit_set_t &other) ध्रुव
  { पटिनिवत्त intersects (other); }

  तार्किक intersects (hb_codepoint_t first, hb_codepoint_t last) ध्रुव
  {
    hb_codepoint_t c = first - 1;
    पटिनिवत्त next (&c) && c <= last;
  }
  शून्य set (ध्रुव hb_bit_set_t &other, तार्किक exact_size = मिच्छा)
  {
    सचे (unlikely (!successful)) पटिनिवत्त;
    unsigned पूर्णाङ्क गणनम् = other.pages.length;
    सचे (unlikely (!resize  (गणनम्, मिच्छा, exact_size)))
      पटिनिवत्त;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  तार्किक is_equal (ध्रुव hb_bit_set_t &other) ध्रुव
  {
    सचे (has_population () && other.has_population () &&
	population != other.population)
      पटिनिवत्त मिच्छा;

    unsigned पूर्णाङ्क na = pages.length;
    unsigned पूर्णाङ्क nb = other.pages.length;

    unsigned पूर्णाङ्क a = 0, b = 0;
    अत्थाय (; a < na && b < nb; )
    {
      सचे (page_at (a).is_empty ()) { a++; अनुवर्त; }
      सचे (other.page_at (b).is_empty ()) { b++; अनुवर्त; }
      सचे (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	पटिनिवत्त मिच्छा;
      a++;
      b++;
    }
    अत्थाय (; a < na; a++)
      सचे (!page_at (a).is_empty ()) { पटिनिवत्त मिच्छा; }
    अत्थाय (; b < nb; b++)
      सचे (!other.page_at (b).is_empty ()) { पटिनिवत्त मिच्छा; }

    पटिनिवत्त सच्च;
  }

  तार्किक is_subset (ध्रुव hb_bit_set_t &larger_set) ध्रुव
  {
    सचे (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      पटिनिवत्त मिच्छा;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    याव (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      सचे (spm < lpm) {
        सचे (!sp.is_empty ())
          पटिनिवत्त मिच्छा;
        spi++;
        अनुवर्त;
      }

      सचे (lpm < spm) {
        lpi++;
        अनुवर्त;
      }

      auto lp = larger_set.page_at (lpi);
      सचे (!sp.is_subset (lp))
        पटिनिवत्त मिच्छा;

      spi++;
      lpi++;
    }

    याव (spi < page_map.length)
      सचे (!page_at (spi++).is_empty ())
        पटिनिवत्त मिच्छा;

    पटिनिवत्त सच्च;
  }

  गुप्त:
  तार्किक allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    सचे (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = मिच्छा;
      पटिनिवत्त मिच्छा;
    }

    पटिनिवत्त सच्च;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  शून्य compact (hb_vector_t<unsigned>& workspace,
                unsigned पूर्णाङ्क length)
  {
    प्रतिज्ञा(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    अत्थाय (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  शून्य compact_pages (ध्रुव hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned पूर्णाङ्क write_index = 0;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < pages.length; i++)
    {
      सचे (old_index_to_page_map_index[i] == 0xFFFFFFFF) अनुवर्त;

      सचे (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  सार्वजनिक:

  शून्य process_ (hb_bit_page_t::vector_t (*op) (ध्रुव hb_bit_page_t::vector_t &, ध्रुव hb_bit_page_t::vector_t &),
		 तार्किक passthru_वामः, तार्किक passthru_दक्षिणः,
		 ध्रुव hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    सचे (unlikely (!successful)) पटिनिवत्त;

    dirty ();

    unsigned पूर्णाङ्क na = pages.length;
    unsigned पूर्णाङ्क nb = other.pages.length;
    unsigned पूर्णाङ्क next_page = na;

    unsigned पूर्णाङ्क गणनम् = 0, new_गणनम् = 0;
    unsigned पूर्णाङ्क a = 0, b = 0;
    unsigned पूर्णाङ्क write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    सचे (!passthru_वामः)
    {
      तार्किक allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      सचे (unlikely (!allocated))
      {
	successful = मिच्छा;
	पटिनिवत्त;
      }
    }

    अत्थाय (; a < na && b < nb; )
    {
      सचे (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	सचे (!passthru_वामः)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  सचे (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	गणनम्++;
	a++;
	b++;
      }
      अञ्ञथा सचे (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	सचे (passthru_वामः)
	  गणनम्++;
	a++;
      }
      अञ्ञथा
      {
	सचे (passthru_दक्षिणः)
	  गणनम्++;
	b++;
      }
    }
    सचे (passthru_वामः)
      गणनम् += na - a;
    सचे (passthru_दक्षिणः)
      गणनम् += nb - b;

    सचे (!passthru_वामः)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    सचे (unlikely (!resize (गणनम्)))
      पटिनिवत्त;

    new_गणनम् = गणनम्;

    /* Process in-place backward. */
    a = na;
    b = nb;
    अत्थाय (; a && b; )
    {
      सचे (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	गणनम्--;
	page_map.arrayZ[गणनम्] = page_map.arrayZ[a];
	page_at (गणनम्).v = op (page_at (a).v, other.page_at (b).v);
	page_at (गणनम्).dirty ();
      }
      अञ्ञथा सचे (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	सचे (passthru_वामः)
	{
	  गणनम्--;
	  page_map.arrayZ[गणनम्] = page_map.arrayZ[a];
	}
      }
      अञ्ञथा
      {
	b--;
	सचे (passthru_दक्षिणः)
	{
	  गणनम्--;
	  page_map.arrayZ[गणनम्].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[गणनम्].index = next_page++;
	  page_at (गणनम्) = other.page_at (b);
	}
      }
    }
    सचे (passthru_वामः)
      याव (a)
      {
	a--;
	गणनम्--;
	page_map.arrayZ[गणनम्] = page_map.arrayZ[a];
      }
    सचे (passthru_दक्षिणः)
      याव (b)
      {
	b--;
	गणनम्--;
	page_map.arrayZ[गणनम्].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[गणनम्].index = next_page++;
	page_at (गणनम्) = other.page_at (b);
      }
    प्रतिज्ञा (!गणनम्);
    resize (new_गणनम्);
  }
  template <typename Op>
  स्थिर hb_bit_page_t::vector_t
  op_ (ध्रुव hb_bit_page_t::vector_t &a, ध्रुव hb_bit_page_t::vector_t &b)
  { पटिनिवत्त Op{} (a, b); }
  template <typename Op>
  शून्य process (ध्रुव Op& op,
		ध्रुव hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  शून्य union_ (ध्रुव hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  शून्य intersect (ध्रुव hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  शून्य subtract (ध्रुव hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  शून्य symmetric_difference (ध्रुव hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  तार्किक next (hb_codepoint_t *codepoint) ध्रुव
  {
    सचे (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      पटिनिवत्त *codepoint != INVALID;
    }

    ध्रुव auto* page_map_array = page_map.arrayZ;
    unsigned पूर्णाङ्क major = get_major (*codepoint);
    unsigned पूर्णाङ्क i = last_page_lookup;

    सचे (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      सचे (i >= page_map.length) {
        *codepoint = INVALID;
        पटिनिवत्त मिच्छा;
      }
      last_page_lookup = i;
    }

    ध्रुव auto* pages_array = pages.arrayZ;
    ध्रुव page_map_t &current = page_map_array[i];
    सचे (likely (current.major == major))
    {
      सचे (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        पटिनिवत्त सच्च;
      }
      i++;
    }

    अत्थाय (; i < page_map.length; i++)
    {
      ध्रुव page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      सचे (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	पटिनिवत्त सच्च;
      }
    }
    *codepoint = INVALID;
    पटिनिवत्त मिच्छा;
  }
  तार्किक next_bits (hb_codepoint_t *codepoint, uint64_t *bits) ध्रुव
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned पूर्णाङ्क i = 0;
    unsigned पूर्णाङ्क elt = 0;
    सचे (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      सचे (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        गच्छ done;
      base += page_t::ELT_BITS;

      unsigned पूर्णाङ्क major = get_major (base);
      i = last_page_lookup;
      सचे (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	सचे (i >= page_map.length)
	  गच्छ done;
      }
      सचे (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    अत्थाय (; i < page_map.length; i++, elt = 0)
    {
      ध्रुव page_map_t &map = page_map.arrayZ[i];
      ध्रुव page_t &page = pages.arrayZ[map.index];
      अत्थाय (; elt < page_t::len (); elt++)
	सचे (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  पटिनिवत्त सच्च;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    पटिनिवत्त मिच्छा;
  }
  तार्किक previous (hb_codepoint_t *codepoint) ध्रुव
  {
    सचे (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      पटिनिवत्त *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned पूर्णाङ्क i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    सचे (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      सचे (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	पटिनिवत्त सच्च;
      }
    }
    i--;
    अत्थाय (; (पूर्णाङ्क) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      सचे (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	पटिनिवत्त सच्च;
      }
    }
    *codepoint = INVALID;
    पटिनिवत्त मिच्छा;
  }
  तार्किक next_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    hb_codepoint_t i;

    i = *last;
    सचे (!next (&i))
    {
      *last = *first = INVALID;
      पटिनिवत्त मिच्छा;
    }

    *last = *first = i;

    ध्रुव auto* page_map_array = page_map.arrayZ;
    ध्रुव auto* pages_array = pages.arrayZ;
    unsigned पूर्णाङ्क major = get_major (*last);
    unsigned पूर्णाङ्क p_index = last_page_lookup;

    सचे (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      प्रतिज्ञा(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned पूर्णाङ्क rem = page_remainder (*last);
    unsigned पूर्णाङ्क elt = rem / page_t::ELT_BITS;
    unsigned पूर्णाङ्क bit = rem & page_t::ELT_MASK;

    ध्रुव page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned पूर्णाङ्क run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    सचे (run < page_t::ELT_BITS - bit)
      पटिनिवत्त सच्च;

    elt++;

    याव (सच्च)
    {
      अत्थाय (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        सचे (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          अनुवर्त;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        पटिनिवत्त सच्च;
      }

      p_index++;


      सचे (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        पटिनिवत्त सच्च;

      ध्रुव auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  तार्किक previous_range (hb_codepoint_t *first, hb_codepoint_t *last) ध्रुव
  {
    hb_codepoint_t i;

    i = *first;
    सचे (!previous (&i))
    {
      *last = *first = INVALID;
      पटिनिवत्त मिच्छा;
    }

    *last = *first = i;

    ध्रुव auto* page_map_array = page_map.arrayZ;
    ध्रुव auto* pages_array = pages.arrayZ;
    unsigned पूर्णाङ्क major = get_major (*first);
    unsigned पूर्णाङ्क p_index = last_page_lookup;

    सचे (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      प्रतिज्ञा(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned पूर्णाङ्क rem = page_remainder (*first);
    पूर्णाङ्क elt = rem / page_t::ELT_BITS;
    unsigned पूर्णाङ्क bit = rem & page_t::ELT_MASK;

    ध्रुव page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned पूर्णाङ्क run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    सचे (run < bit + 1)
      पटिनिवत्त सच्च;

    elt--;

    याव (सच्च)
    {
      अत्थाय (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        सचे (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          अनुवर्त;
        }
        uint64_t inv = ~w;
        unsigned पूर्णाङ्क zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        पटिनिवत्त सच्च;
      }


      सचे (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        पटिनिवत्त सच्च;

      p_index--;
      ध्रुव auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned पूर्णाङ्क next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned पूर्णाङ्क    size) ध्रुव
  {
    // By default, start at the first bit of the first page of values.
    unsigned पूर्णाङ्क start_page = 0;
    unsigned पूर्णाङ्क start_page_मानम् = 0;
    सचे (unlikely (codepoint != INVALID))
    {
      ध्रुव auto* page_map_array = page_map.arrayZ;
      unsigned पूर्णाङ्क major = get_major (codepoint);
      unsigned पूर्णाङ्क i = last_page_lookup;
      सचे (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	सचे (i >= page_map.length)
	  पटिनिवत्त 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      सचे (page_map_array[i].major == major)
      {
        start_page_मानम् = page_remainder (codepoint + 1);
        सचे (unlikely (start_page_मानम् == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मानम् = 0;
        }
      }
    }

    unsigned पूर्णाङ्क initial_size = size;
    अत्थाय (unsigned पूर्णाङ्क i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned पूर्णाङ्क n = pages[page_map.arrayZ[i].index].लिख (base, start_page_मानम्, out, size);
      out += n;
      size -= n;
      start_page_मानम् = 0;
    }
    पटिनिवत्त initial_size - size;
  }

  unsigned पूर्णाङ्क next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned पूर्णाङ्क    size) ध्रुव
  {
    unsigned पूर्णाङ्क initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned पूर्णाङ्क start_page = 0;
    unsigned पूर्णाङ्क start_page_मानम् = 0;
    सचे (unlikely (codepoint != INVALID))
    {
      ध्रुव auto* page_map_array = page_map.arrayZ;
      unsigned पूर्णाङ्क major = get_major (codepoint);
      unsigned पूर्णाङ्क i = last_page_lookup;
      सचे (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        सचे (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          याव (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          पटिनिवत्त initial_size - size;
        }
      }
      start_page = i;
      सचे (page_map_array[i].major == major)
      {
        start_page_मानम् = page_remainder (codepoint + 1);
        सचे (unlikely (start_page_मानम् == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_मानम् = 0;
        }
      }
    }

    hb_codepoint_t next_मानम् = codepoint + 1;
    अत्थाय (unsigned पूर्णाङ्क i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned पूर्णाङ्क n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_मानम्, out, size, &next_मानम्);
      out += n;
      size -= n;
      start_page_मानम् = 0;
    }
    याव (next_मानम् < HB_SET_मानम्_INVALID && size) {
      *out++ = next_मानम्++;
      size--;
    }
    पटिनिवत्त initial_size - size;
  }

  तार्किक has_population () ध्रुव { पटिनिवत्त population != UINT_MAX; }
  unsigned पूर्णाङ्क get_population () ध्रुव
  {
    सचे (has_population ())
      पटिनिवत्त population;

    unsigned पूर्णाङ्क निष्कासय = 0;
    unsigned पूर्णाङ्क गणनम् = pages.length;
    अत्थाय (unsigned पूर्णाङ्क i = 0; i < गणनम्; i++)
      निष्कासय += pages[i].get_population ();

    population = निष्कासय;
    पटिनिवत्त निष्कासय;
  }
  तार्किक get_singleton (hb_codepoint_t *codepoint) ध्रुव
  {
    सचे (has_population ())
    {
      सचे (population != 1) पटिनिवत्त मिच्छा;
      *codepoint = get_min ();
      पटिनिवत्त सच्च;
    }

    hb_codepoint_t singleton = INVALID;
    अत्थाय (ध्रुव auto &map : page_map)
    {
      ध्रुव auto &page = pages.arrayZ[map.index];
      अत्थाय (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	सचे (!bits) अनुवर्त;
	सचे (singleton != INVALID || (bits & (bits - 1)))
	  पटिनिवत्त मिच्छा;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    सचे (singleton == INVALID)
    {
      population = 0;
      पटिनिवत्त मिच्छा;
    }
    population = 1;
    *codepoint = singleton;
    पटिनिवत्त सच्च;
  }
  hb_codepoint_t get_min () ध्रुव
  {
    unsigned गणनम् = pages.length;
    अत्थाय (unsigned i = 0; i < गणनम्; i++)
    {
      ध्रुव auto& map = page_map.arrayZ[i];
      ध्रुव auto& page = pages.arrayZ[map.index];

      सचे (!page.is_empty ())
	पटिनिवत्त map.major * page_t::PAGE_BITS + page.get_min ();
    }
    पटिनिवत्त INVALID;
  }
  hb_codepoint_t get_max () ध्रुव
  {
    unsigned गणनम् = pages.length;
    अत्थाय (signed i = गणनम् - 1; i >= 0; i--)
    {
      ध्रुव auto& map = page_map.arrayZ[(unsigned) i];
      ध्रुव auto& page = pages.arrayZ[map.index];

      सचे (!page.is_empty ())
	पटिनिवत्त map.major * page_t::PAGE_BITS + page.get_max ();
    }
    पटिनिवत्त INVALID;
  }

  स्थिर constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  संरचना iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    स्थिर constexpr तार्किक is_sorted_iterator = सच्च;
    स्थिर constexpr तार्किक has_fast_len = सच्च;
    iter_t (ध्रुव hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    तार्किक init = सच्च) : s (&s_), v (INVALID), l(0)
    {
      सचे (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    प्रकारनिर्देश hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () ध्रुव { पटिनिवत्त v; }
    तार्किक __more__ () ध्रुव { पटिनिवत्त v != INVALID; }
    शून्य __next__ () { s->next (&v); सचे (l) l--; }
    शून्य __prev__ () { s->previous (&v); }
    unsigned __len__ () ध्रुव { पटिनिवत्त l; }
    iter_t end () ध्रुव { पटिनिवत्त iter_t (*s, मिच्छा); }
    तार्किक operator != (ध्रुव iter_t& o) ध्रुव
    { पटिनिवत्त v != o.v; }

    रक्षित:
    ध्रुव hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () ध्रुव { पटिनिवत्त iter_t (*एतत्); }
  operator iter_t () ध्रुव { पटिनिवत्त iter (); }

  रक्षित:

  page_t *page_for (hb_codepoint_t g, तार्किक insert = मिच्छा)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    सचे (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      सचे (cached_page.major == major)
	पटिनिवत्त &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    सचे (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      सचे (!insert)
        पटिनिवत्त nullptr;

      सचे (unlikely (!resize (pages.length + 1)))
	पटिनिवत्त nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    पटिनिवत्त &pages.arrayZ[page_map.arrayZ[i].index];
  }
  ध्रुव page_t *page_for (hb_codepoint_t g) ध्रुव
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    सचे (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      सचे (cached_page.major == major)
	पटिनिवत्त &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    सचे (!page_map.bfind (key, &i))
      पटिनिवत्त nullptr;

    last_page_lookup = i;
    पटिनिवत्त &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned पूर्णाङ्क i)
  {
    प्रतिज्ञा (i < page_map.length);
    पटिनिवत्त pages.arrayZ[page_map.arrayZ[i].index];
  }
  ध्रुव page_t &page_at (unsigned पूर्णाङ्क i) ध्रुव
  {
    प्रतिज्ञा (i < page_map.length);
    पटिनिवत्त pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned पूर्णाङ्क get_major (hb_codepoint_t g) ध्रुव { पटिनिवत्त g >> page_t::PAGE_BITS_LOG_2; }
  unsigned पूर्णाङ्क page_remainder (hb_codepoint_t g) ध्रुव { पटिनिवत्त g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned पूर्णाङ्क major) ध्रुव { पटिनिवत्त major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */

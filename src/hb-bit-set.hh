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

#include "hb.hh"
#include "hb-bit-page.hh"

struct hb_bit_set_t
{
  hb_bit_set_t () = default;
  ~hb_bit_set_t () = default;

  hb_bit_set_t (const hb_bit_set_t& other) : hb_bit_set_t () { set (other, true); }
  hb_bit_set_t ( hb_bit_set_t&& other)  noexcept : hb_bit_set_t () { hb_swap (*this, other); }
  hb_bit_set_t& operator= (const hb_bit_set_t& other) { set (other); върни *this; }
  hb_bit_set_t& operator= (hb_bit_set_t&& other)  noexcept { hb_swap (*this, other); върни *this; }
  friend void swap (hb_bit_set_t &a, hb_bit_set_t &b) noexcept
  {
    ако (likely (!a.successful || !b.successful))
      върни;
    hb_swap (a.population, b.population);
    hb_swap (a.last_page_lookup, b.last_page_lookup);
    hb_swap (a.page_map, b.page_map);
    hb_swap (a.pages, b.pages);
  }

  void init ()
  {
    successful = true;
    population = 0;
    last_page_lookup = 0;
    page_map.init ();
    pages.init ();
  }
  void fini ()
  {
    page_map.fini ();
    pages.fini ();
  }

  using page_t = hb_bit_page_t;
  struct page_map_t
  {
    цяло cmp (const page_map_t &o) const { върни cmp (o.major); }
    цяло cmp (uint32_t o_major) const { върни (цяло) o_major - (цяло) major; }

    uint32_t major;
    uint32_t index;
  };

  bool successful = true; /* Allocations successful */
  mutable unsigned цяло population = 0;
  mutable hb_atomic_t<unsigned> last_page_lookup = 0;
  hb_sorted_vector_t<page_map_t> page_map;
  hb_vector_t<page_t> pages;

  void err () { ако (successful) successful = false; } /* TODO Remove */
  bool in_error () const { върни !successful; }

  bool resize (unsigned цяло count, bool clear = true, bool exact_size = false)
  {
    ако (unlikely (!successful)) върни false;

    ако (pages.length < count && (unsigned) pages.allocated < count && count <= 2)
      exact_size = true; // Most sets are small and local

    ако (unlikely (!pages.resize_full (count, clear, exact_size) ||
	!page_map.resize_full (count, clear, false)))
    {
      pages.resize_full (page_map.length, clear, exact_size);
      successful = false;
      върни false;
    }
    върни true;
  }

  void alloc (unsigned sz)
  {
    sz >>= (page_t::PAGE_BITS_LOG_2 - 1);
    pages.alloc (sz);
    page_map.alloc (sz);
  }

  hb_bit_set_t& reset ()
  {
    successful = true;
    clear ();
    върни *this;
  }

  void clear ()
  {
    /* Early-out on already-empty.  Protects the Null singleton
     * (which is zero-initialized) from any writes.  Any non-empty
     * instance is a real heap object with writable storage, so
     * clearing through the vector's always-safe clear() is fine
     * even if we entered error state. */
    ако (!pages.length && !population) върни;
    pages.clear ();
    page_map.clear ();
    population = 0;
  }
  bool is_empty () const
  {
    unsigned цяло count = pages.length;
    за (unsigned цяло i = 0; i < count; i++)
      ако (!pages[i].is_empty ())
	върни false;
    върни true;
  }
  explicit operator bool () const { върни !is_empty (); }

  uint32_t hash () const
  {
    uint32_t h = 0;
    за (auto &map : page_map)
    {
      auto &page = pages.arrayZ[map.index];
      ако (unlikely (page.is_empty ())) continue;
      h = h * 31 + hb_hash (map.major) + hb_hash (page);
    }
    върни h;
  }

  private:
  void dirty () { population = UINT_MAX; }
  public:

  void add (hb_codepoint_t g)
  {
    ако (unlikely (!successful)) върни;
    ако (unlikely (g == INVALID)) върни;
    dirty ();
    page_t *page = page_for (g, true); ако (unlikely (!page)) върни;
    page->add (g);
  }
  void add_bits (hb_codepoint_t g, uint64_t bits)
  {
    ако (unlikely (!successful) || unlikely (!bits)) върни;
    assert (!(g & (page_t::ELT_BITS - 1)));
    dirty ();
    page_t *page = page_for (g, true); ако (unlikely (!page)) върни;
    page->add_bits (g, bits);
  }
  bool add_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ако (unlikely (!successful)) върни true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ако (unlikely (a > b || a == INVALID || b == INVALID)) върни false;
    dirty ();
    unsigned цяло ma = get_major (a);
    unsigned цяло mb = get_major (b);
    ако (ma == mb)
    {
      page_t *page = page_for (a, true); ако (unlikely (!page)) върни false;
      page->add_range (a, b);
    }
    иначе
    {
      page_t *page = page_for (a, true); ако (unlikely (!page)) върни false;
      page->add_range (a, major_start (ma + 1) - 1);

      за (unsigned цяло m = ma + 1; m < mb; m++)
      {
	page = page_for (major_start (m), true); ако (unlikely (!page)) върни false;
	page->init1 ();
      }

      page = page_for (b, true); ако (unlikely (!page)) върни false;
      page->add_range (major_start (mb), b);
    }
    върни true;
  }

  /* Duplicated here from hb-machinery.hh to avoid including it. */
  template<typename Type>
  static inline const Type& StructAtOffsetUnaligned(const void *P, unsigned цяло offset)
  {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
    върни * reinterpret_cast<const Type*> ((const char *) P + offset);
#pragma GCC diagnostic pop
  }

  template <typename T>
  void set_array (bool v, const T *array, unsigned цяло count, unsigned цяло stride=sizeof(T))
  {
    ако (unlikely (!successful)) върни;
    ако (!count) върни;
    dirty ();
    hb_codepoint_t g = *array;
    докато (count)
    {
      unsigned цяло m = get_major (g);
      page_t *page = page_for (g, v); ако (unlikely (v && !page)) върни;
      unsigned цяло start = major_start (m);
      unsigned цяло end = major_start (m + 1);
      do
      {
        ако (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      докато (count && (g = *array, start <= g && g < end));
    }
  }

  template <typename T>
  void add_array (const T *array, unsigned цяло count, unsigned цяло stride=sizeof(T))
  { set_array (true, array, count, stride); }
  template <typename T>
  void add_array (const hb_array_t<const T>& arr) { add_array (&arr, arr.len ()); }

  template <typename T>
  void del_array (const T *array, unsigned цяло count, unsigned цяло stride=sizeof(T))
  { set_array (false, array, count, stride); }
  template <typename T>
  void del_array (const hb_array_t<const T>& arr) { del_array (&arr, arr.len ()); }

  /* Might return false if array looks unsorted.
   * Used for faster rejection of corrupt data. */
  template <typename T>
  bool set_sorted_array (bool v, const T *array, unsigned цяло count, unsigned цяло stride=sizeof(T))
  {
    ако (unlikely (!successful)) върни true; /* https://github.com/harfbuzz/harfbuzz/issues/657 */
    ако (unlikely (!count)) върни true;
    dirty ();
    hb_codepoint_t g = *array;
    hb_codepoint_t last_g = g;
    докато (count)
    {
      unsigned цяло m = get_major (g);
      page_t *page = page_for (g, v); ако (unlikely (v && !page)) върни false;
      unsigned цяло end = major_start (m + 1);
      do
      {
	/* If we try harder we can change the following comparison to <=;
	 * Not sure if it's worth it. */
	ако (g < last_g) върни false;
	last_g = g;

        ако (g != INVALID && (v || page)) /* The v check is to optimize out the page check if v is true. */
	  page->set (g, v);

	array = &StructAtOffsetUnaligned<T> (array, stride);
	count--;
      }
      докато (count && (g = *array, g < end));
    }
    върни true;
  }

  template <typename T>
  bool add_sorted_array (const T *array, unsigned цяло count, unsigned цяло stride=sizeof(T))
  { върни set_sorted_array (true, array, count, stride); }
  template <typename T>
  bool add_sorted_array (const hb_sorted_array_t<const T>& arr) { върни add_sorted_array (&arr, arr.len ()); }

  template <typename T>
  bool del_sorted_array (const T *array, unsigned цяло count, unsigned цяло stride=sizeof(T))
  { върни set_sorted_array (false, array, count, stride); }
  template <typename T>
  bool del_sorted_array (const hb_sorted_array_t<const T>& arr) { върни del_sorted_array (&arr, arr.len ()); }

  void del (hb_codepoint_t g)
  {
    ако (unlikely (!successful)) върни;
    page_t *page = page_for (g);
    ако (!page)
      върни;
    dirty ();
    page->del (g);
  }
  void del_bits (hb_codepoint_t g, uint64_t bits)
  {
    ако (unlikely (!successful) || unlikely (!bits)) върни;
    assert (!(g & (page_t::ELT_BITS - 1)));
    page_t *page = page_for (g);
    ако (!page)
      върни;
    dirty ();
    page->del_bits (g, bits);
  }

  private:
  void del_pages (цяло ds, цяло de)
  {
    ако (ds <= de)
    {
      // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
      // before attempting to rewrite the page map.
      hb_vector_t<unsigned> compact_workspace;
      ако (unlikely (!allocate_compact_workspace (compact_workspace))) върни;

      unsigned цяло write_index = 0;
      за (unsigned цяло i = 0; i < page_map.length; i++)
      {
	цяло m = (цяло) page_map.arrayZ[i].major;
	ако (m < ds || de < m)
	  page_map.arrayZ[write_index++] = page_map.arrayZ[i];
      }
      compact (compact_workspace, write_index);
      resize (write_index);
    }
  }


  public:
  void del_range (hb_codepoint_t a, hb_codepoint_t b)
  {
    ако (unlikely (!successful)) върни;
    ако (unlikely (a > b || a == INVALID)) върни;
    dirty ();
    unsigned цяло ma = get_major (a);
    unsigned цяло mb = get_major (b);
    /* Delete pages from ds through de if ds <= de. */
    цяло ds = (a == major_start (ma))? (цяло) ma: (цяло) (ma + 1);
    цяло de = (b + 1 == major_start (mb + 1))? (цяло) mb: ((цяло) mb - 1);
    ако (ds > de || (цяло) ma < ds)
    {
      page_t *page = page_for (a);
      ако (page)
      {
	ако (ma == mb)
	  page->del_range (a, b);
	иначе
	  page->del_range (a, major_start (ma + 1) - 1);
      }
    }
    ако (de < (цяло) mb && ma != mb)
    {
      page_t *page = page_for (b);
      ако (page)
	page->del_range (major_start (mb), b);
    }
    del_pages (ds, de);
  }

  bool get (hb_codepoint_t g) const
  {
    const page_t *page = page_for (g);
    ако (!page)
      върни false;
    върни page->get (g);
  }
  bool may_have (hb_codepoint_t g) const { върни get (g); }

  /* Has interface. */
  bool operator [] (hb_codepoint_t k) const { върни get (k); }
  bool has (hb_codepoint_t k) const { върни (*this)[k]; }
  /* Predicate. */
  bool operator () (hb_codepoint_t k) const { върни has (k); }

  /* Sink interface. */
  hb_bit_set_t& operator << (hb_codepoint_t v)
  { add (v); върни *this; }
  hb_bit_set_t& operator << (const hb_codepoint_pair_t& range)
  { add_range (range.first, range.second); върни *this; }

  bool intersects (const hb_bit_set_t &other) const
  {
    unsigned цяло na = pages.length;
    unsigned цяло nb = other.pages.length;

    unsigned цяло a = 0, b = 0;
    за (; a < na && b < nb; )
    {
      ако (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ако (page_at (a).intersects (other.page_at (b)))
	  върни true;
	a++;
	b++;
      }
      иначе ако (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
	a++;
      иначе
	b++;
    }
    върни false;
  }
  bool may_intersect (const hb_bit_set_t &other) const
  { върни intersects (other); }

  bool intersects (hb_codepoint_t first, hb_codepoint_t last) const
  {
    hb_codepoint_t c = first - 1;
    върни next (&c) && c <= last;
  }
  void set (const hb_bit_set_t &other, bool exact_size = false)
  {
    ако (unlikely (!successful)) върни;
    unsigned цяло count = other.pages.length;
    ако (unlikely (!resize  (count, false, exact_size)))
      върни;
    population = other.population;

    page_map = other.page_map;
    pages = other.pages;
  }

  bool is_equal (const hb_bit_set_t &other) const
  {
    ако (has_population () && other.has_population () &&
	population != other.population)
      върни false;

    unsigned цяло na = pages.length;
    unsigned цяло nb = other.pages.length;

    unsigned цяло a = 0, b = 0;
    за (; a < na && b < nb; )
    {
      ако (page_at (a).is_empty ()) { a++; continue; }
      ако (other.page_at (b).is_empty ()) { b++; continue; }
      ако (page_map.arrayZ[a].major != other.page_map.arrayZ[b].major ||
	  !page_at (a).is_equal (other.page_at (b)))
	върни false;
      a++;
      b++;
    }
    за (; a < na; a++)
      ако (!page_at (a).is_empty ()) { върни false; }
    за (; b < nb; b++)
      ако (!other.page_at (b).is_empty ()) { върни false; }

    върни true;
  }

  bool is_subset (const hb_bit_set_t &larger_set) const
  {
    ако (has_population () && larger_set.has_population () &&
	population > larger_set.population)
      върни false;

    uint32_t spi = 0;
    uint32_t lpi = 0;
    докато (spi < page_map.length && lpi < larger_set.page_map.length)
    {
      uint32_t spm = page_map.arrayZ[spi].major;
      uint32_t lpm = larger_set.page_map.arrayZ[lpi].major;
      auto sp = page_at (spi);

      ако (spm < lpm) {
        ако (!sp.is_empty ())
          върни false;
        spi++;
        continue;
      }

      ако (lpm < spm) {
        lpi++;
        continue;
      }

      auto lp = larger_set.page_at (lpi);
      ако (!sp.is_subset (lp))
        върни false;

      spi++;
      lpi++;
    }

    докато (spi < page_map.length)
      ако (!page_at (spi++).is_empty ())
        върни false;

    върни true;
  }

  private:
  bool allocate_compact_workspace (hb_vector_t<unsigned>& workspace)
  {
    ако (unlikely (!workspace.resize_exact (pages.length)))
    {
      successful = false;
      върни false;
    }

    върни true;
  }

  /*
   * workspace should be a pre-sized vector allocated to hold at exactly pages.length
   * elements.
   */
  void compact (hb_vector_t<unsigned>& workspace,
                unsigned цяло length)
  {
    assert(workspace.length == pages.length);
    hb_vector_t<unsigned>& old_index_to_page_map_index = workspace;

    hb_fill (old_index_to_page_map_index.writer(), 0xFFFFFFFF);
    за (unsigned i = 0; i < length; i++)
      old_index_to_page_map_index[page_map[i].index] =  i;

    compact_pages (old_index_to_page_map_index);
  }
  void compact_pages (const hb_vector_t<unsigned>& old_index_to_page_map_index)
  {
    unsigned цяло write_index = 0;
    за (unsigned цяло i = 0; i < pages.length; i++)
    {
      ако (old_index_to_page_map_index[i] == 0xFFFFFFFF) continue;

      ако (write_index < i)
	pages[write_index] = pages[i];

      page_map[old_index_to_page_map_index[i]].index = write_index;
      write_index++;
    }
  }
  public:

  void process_ (hb_bit_page_t::vector_t (*op) (const hb_bit_page_t::vector_t &, const hb_bit_page_t::vector_t &),
		 bool passthru_left, bool passthru_right,
		 const hb_bit_set_t &other,
		 hb_vector_t<unsigned> *workspace = nullptr)
  {
    ако (unlikely (!successful)) върни;

    dirty ();

    unsigned цяло na = pages.length;
    unsigned цяло nb = other.pages.length;
    unsigned цяло next_page = na;

    unsigned цяло count = 0, newCount = 0;
    unsigned цяло a = 0, b = 0;
    unsigned цяло write_index = 0;

    // Pre-allocate the workspace that compact() will need so we can bail on allocation failure
    // before attempting to rewrite the page map.
    hb_vector_t<unsigned> local_workspace;
    hb_vector_t<unsigned> &compact_workspace = workspace ? *workspace : local_workspace;
    ако (!passthru_left)
    {
      bool allocated = workspace ? compact_workspace.resize (pages.length)
				 : allocate_compact_workspace (compact_workspace);
      ако (unlikely (!allocated))
      {
	successful = false;
	върни;
      }
    }

    за (; a < na && b < nb; )
    {
      ако (page_map.arrayZ[a].major == other.page_map.arrayZ[b].major)
      {
	ако (!passthru_left)
	{
	  // Move page_map entries that we're keeping from the left side set
	  // to the front of the page_map vector. This isn't necessary if
	  // passthru_left is set since no left side pages will be removed
	  // in that case.
	  ако (write_index < a)
	    page_map.arrayZ[write_index] = page_map.arrayZ[a];
	  write_index++;
	}

	count++;
	a++;
	b++;
      }
      иначе ако (page_map.arrayZ[a].major < other.page_map.arrayZ[b].major)
      {
	ако (passthru_left)
	  count++;
	a++;
      }
      иначе
      {
	ако (passthru_right)
	  count++;
	b++;
      }
    }
    ако (passthru_left)
      count += na - a;
    ако (passthru_right)
      count += nb - b;

    ако (!passthru_left)
    {
      na  = write_index;
      next_page = write_index;
      compact (compact_workspace, write_index);
    }

    ако (unlikely (!resize (count)))
      върни;

    newCount = count;

    /* Process in-place backward. */
    a = na;
    b = nb;
    за (; a && b; )
    {
      ако (page_map.arrayZ[a - 1].major == other.page_map.arrayZ[b - 1].major)
      {
	a--;
	b--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
	page_at (count).v = op (page_at (a).v, other.page_at (b).v);
	page_at (count).dirty ();
      }
      иначе ако (page_map.arrayZ[a - 1].major > other.page_map.arrayZ[b - 1].major)
      {
	a--;
	ако (passthru_left)
	{
	  count--;
	  page_map.arrayZ[count] = page_map.arrayZ[a];
	}
      }
      иначе
      {
	b--;
	ако (passthru_right)
	{
	  count--;
	  page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	  page_map.arrayZ[count].index = next_page++;
	  page_at (count) = other.page_at (b);
	}
      }
    }
    ако (passthru_left)
      докато (a)
      {
	a--;
	count--;
	page_map.arrayZ[count] = page_map.arrayZ[a];
      }
    ако (passthru_right)
      докато (b)
      {
	b--;
	count--;
	page_map.arrayZ[count].major = other.page_map.arrayZ[b].major;
	page_map.arrayZ[count].index = next_page++;
	page_at (count) = other.page_at (b);
      }
    assert (!count);
    resize (newCount);
  }
  template <typename Op>
  static hb_bit_page_t::vector_t
  op_ (const hb_bit_page_t::vector_t &a, const hb_bit_page_t::vector_t &b)
  { върни Op{} (a, b); }
  template <typename Op>
  void process (const Op& op,
		const hb_bit_set_t &other,
		hb_vector_t<unsigned> *workspace = nullptr)
  {
    process_ (op_<Op>, op (1, 0), op (0, 1), other, workspace);
  }

  void union_ (const hb_bit_set_t &other) { process (hb_bitwise_or, other); }
  void intersect (const hb_bit_set_t &other,
		  hb_vector_t<unsigned> *workspace = nullptr)
  { process (hb_bitwise_and, other, workspace); }
  void subtract (const hb_bit_set_t &other) { process (hb_bitwise_gt, other); }
  void symmetric_difference (const hb_bit_set_t &other) { process (hb_bitwise_xor, other); }

  bool next (hb_codepoint_t *codepoint) const
  {
    ако (unlikely (*codepoint == INVALID)) {
      *codepoint = get_min ();
      върни *codepoint != INVALID;
    }

    const auto* page_map_array = page_map.arrayZ;
    unsigned цяло major = get_major (*codepoint);
    unsigned цяло i = last_page_lookup;

    ако (unlikely (i >= page_map.length || page_map_array[i].major != major))
    {
      page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
      ако (i >= page_map.length) {
        *codepoint = INVALID;
        върни false;
      }
      last_page_lookup = i;
    }

    const auto* pages_array = pages.arrayZ;
    const page_map_t &current = page_map_array[i];
    ако (likely (current.major == major))
    {
      ако (pages_array[current.index].next (codepoint))
      {
        *codepoint += current.major * page_t::PAGE_BITS;
        върни true;
      }
      i++;
    }

    за (; i < page_map.length; i++)
    {
      const page_map_t &current = page_map_array[i];
      hb_codepoint_t m = pages_array[current.index].get_min ();
      ако (m != INVALID)
      {
	*codepoint = current.major * page_t::PAGE_BITS + m;
        last_page_lookup = i;
	върни true;
      }
    }
    *codepoint = INVALID;
    върни false;
  }
  bool next_bits (hb_codepoint_t *codepoint, uint64_t *bits) const
  {
    static_assert (page_t::ELT_BITS == 64, "");

    unsigned цяло i = 0;
    unsigned цяло elt = 0;
    ако (likely (*codepoint != INVALID))
    {
      hb_codepoint_t base = *codepoint & ~page_t::ELT_MASK;
      ако (unlikely (base >= INVALID - page_t::ELT_BITS + 1))
        goto done;
      base += page_t::ELT_BITS;

      unsigned цяло major = get_major (base);
      i = last_page_lookup;
      ако (unlikely (i >= page_map.length || page_map.arrayZ[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ако (i >= page_map.length)
	  goto done;
      }
      ако (page_map.arrayZ[i].major == major)
	elt = page_remainder (base) / page_t::ELT_BITS;
    }

    за (; i < page_map.length; i++, elt = 0)
    {
      const page_map_t &map = page_map.arrayZ[i];
      const page_t &page = pages.arrayZ[map.index];
      за (; elt < page_t::len (); elt++)
	ако (page.v[elt])
	{
	  *codepoint = major_start (map.major) + elt * page_t::ELT_BITS;
	  *bits = page.v[elt];
	  last_page_lookup = i;
	  върни true;
	}
    }

  done:
    *codepoint = INVALID;
    *bits = 0;
    върни false;
  }
  bool previous (hb_codepoint_t *codepoint) const
  {
    ако (unlikely (*codepoint == INVALID)) {
      *codepoint = get_max ();
      върни *codepoint != INVALID;
    }

    page_map_t map = {get_major (*codepoint), 0};
    unsigned цяло i;
    page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST);
    ако (i < page_map.length && page_map.arrayZ[i].major == map.major)
    {
      ако (pages[page_map.arrayZ[i].index].previous (codepoint))
      {
	*codepoint += page_map.arrayZ[i].major * page_t::PAGE_BITS;
	върни true;
      }
    }
    i--;
    за (; (цяло) i >= 0; i--)
    {
      hb_codepoint_t m = pages.arrayZ[page_map.arrayZ[i].index].get_max ();
      ако (m != INVALID)
      {
	*codepoint = page_map.arrayZ[i].major * page_t::PAGE_BITS + m;
	върни true;
      }
    }
    *codepoint = INVALID;
    върни false;
  }
  bool next_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *last;
    ако (!next (&i))
    {
      *last = *first = INVALID;
      върни false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned цяло major = get_major (*last);
    unsigned цяло p_index = last_page_lookup;

    ако (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above next() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned цяло rem = page_remainder (*last);
    unsigned цяло elt = rem / page_t::ELT_BITS;
    unsigned цяло bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    unsigned цяло run = hb_ctz (~(page->v[elt] >> bit));
    *last += run - 1;
    ако (run < page_t::ELT_BITS - bit)
      върни true;

    elt++;

    докато (true)
    {
      за (; elt < page_t::len (); elt++)
      {
        uint64_t w = page->v[elt];
        ако (w == (uint64_t) -1)
        {
          *last += page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        *last += hb_ctz (inv);
        върни true;
      }

      p_index++;


      ако (p_index >= page_map.length || page_map_array[p_index].major != major + 1)
        върни true;

      const auto& entry = page_map_array[p_index];
      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = 0;
    }
  }

  bool previous_range (hb_codepoint_t *first, hb_codepoint_t *last) const
  {
    hb_codepoint_t i;

    i = *first;
    ако (!previous (&i))
    {
      *last = *first = INVALID;
      върни false;
    }

    *last = *first = i;

    const auto* page_map_array = page_map.arrayZ;
    const auto* pages_array = pages.arrayZ;
    unsigned цяло major = get_major (*first);
    unsigned цяло p_index = last_page_lookup;

    ако (unlikely (p_index >= page_map.length || page_map_array[p_index].major != major))
    {
      page_map.bfind (major, &p_index, HB_NOT_FOUND_STORE_CLOSEST);
      // The above previous() call gaurantees the page we're looking for exists.
      assert(p_index < page_map.length && page_map_array[p_index].major == major);
      last_page_lookup = p_index;
    }

    unsigned цяло rem = page_remainder (*first);
    цяло elt = rem / page_t::ELT_BITS;
    unsigned цяло bit = rem & page_t::ELT_MASK;

    const page_t *page = &pages_array[page_map_array[p_index].index];
    uint64_t inv = ~(page->v[elt] << (63 - bit));
    unsigned цяло run = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
    *first -= run - 1;
    ако (run < bit + 1)
      върни true;

    elt--;

    докато (true)
    {
      за (; elt >= 0; elt--)
      {
        uint64_t w = page->v[elt];
        ако (w == (uint64_t) -1)
        {
          *first -= page_t::ELT_BITS;
          continue;
        }
        uint64_t inv = ~w;
        unsigned цяло zeros = hb_bit_storage (inv) ? (page_t::ELT_BITS - hb_bit_storage (inv)) : page_t::ELT_BITS;
        *first -= zeros;
        върни true;
      }


      ако (p_index == 0 || page_map_array[p_index - 1].major != major - 1)
        върни true;

      p_index--;
      const auto& entry = page_map_array[p_index];

      major = entry.major;
      last_page_lookup = p_index;
      page = &pages_array[entry.index];
      elt = page_t::len () - 1;
    }
  }

  unsigned цяло next_many (hb_codepoint_t  codepoint,
			  hb_codepoint_t *out,
			  unsigned цяло    size) const
  {
    // By default, start at the first bit of the first page of values.
    unsigned цяло start_page = 0;
    unsigned цяло start_page_value = 0;
    ако (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned цяло major = get_major (codepoint);
      unsigned цяло i = last_page_lookup;
      ако (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
	page_map.bfind (major, &i, HB_NOT_FOUND_STORE_CLOSEST);
	ако (i >= page_map.length)
	  върни 0;  // codepoint is greater than our max element.
      }
      start_page = i;
      ако (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ако (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    unsigned цяло initial_size = size;
    за (unsigned цяло i = start_page; i < page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned цяло n = pages[page_map.arrayZ[i].index].write (base, start_page_value, out, size);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    върни initial_size - size;
  }

  unsigned цяло next_many_inverted (hb_codepoint_t  codepoint,
				   hb_codepoint_t *out,
				   unsigned цяло    size) const
  {
    unsigned цяло initial_size = size;
    // By default, start at the first bit of the first page of values.
    unsigned цяло start_page = 0;
    unsigned цяло start_page_value = 0;
    ако (unlikely (codepoint != INVALID))
    {
      const auto* page_map_array = page_map.arrayZ;
      unsigned цяло major = get_major (codepoint);
      unsigned цяло i = last_page_lookup;
      ако (unlikely (i >= page_map.length || page_map_array[i].major != major))
      {
        page_map.bfind(major, &i, HB_NOT_FOUND_STORE_CLOSEST);
        ако (unlikely (i >= page_map.length))
        {
          // codepoint is greater than our max element.
          докато (++codepoint != INVALID && size)
          {
            *out++ = codepoint;
            size--;
          }
          върни initial_size - size;
        }
      }
      start_page = i;
      ако (page_map_array[i].major == major)
      {
        start_page_value = page_remainder (codepoint + 1);
        ако (unlikely (start_page_value == 0))
        {
          // The export-after value was last in the page. Start on next page.
          start_page++;
          start_page_value = 0;
        }
      }
    }

    hb_codepoint_t next_value = codepoint + 1;
    за (unsigned цяло i=start_page; i<page_map.length && size; i++)
    {
      uint32_t base = major_start (page_map.arrayZ[i].major);
      unsigned цяло n = pages[page_map.arrayZ[i].index].write_inverted (base, start_page_value, out, size, &next_value);
      out += n;
      size -= n;
      start_page_value = 0;
    }
    докато (next_value < HB_SET_VALUE_INVALID && size) {
      *out++ = next_value++;
      size--;
    }
    върни initial_size - size;
  }

  bool has_population () const { върни population != UINT_MAX; }
  unsigned цяло get_population () const
  {
    ако (has_population ())
      върни population;

    unsigned цяло pop = 0;
    unsigned цяло count = pages.length;
    за (unsigned цяло i = 0; i < count; i++)
      pop += pages[i].get_population ();

    population = pop;
    върни pop;
  }
  bool get_singleton (hb_codepoint_t *codepoint) const
  {
    ако (has_population ())
    {
      ако (population != 1) върни false;
      *codepoint = get_min ();
      върни true;
    }

    hb_codepoint_t singleton = INVALID;
    за (const auto &map : page_map)
    {
      const auto &page = pages.arrayZ[map.index];
      за (unsigned i = 0; i < page_t::len (); i++)
      {
	page_t::elt_t bits = page.v[i];
	ако (!bits) continue;
	ако (singleton != INVALID || (bits & (bits - 1)))
	  върни false;
	singleton = map.major * page_t::PAGE_BITS +
		    i * page_t::ELT_BITS + hb_ctz (bits);
      }
    }

    ако (singleton == INVALID)
    {
      population = 0;
      върни false;
    }
    population = 1;
    *codepoint = singleton;
    върни true;
  }
  hb_codepoint_t get_min () const
  {
    unsigned count = pages.length;
    за (unsigned i = 0; i < count; i++)
    {
      const auto& map = page_map.arrayZ[i];
      const auto& page = pages.arrayZ[map.index];

      ако (!page.is_empty ())
	върни map.major * page_t::PAGE_BITS + page.get_min ();
    }
    върни INVALID;
  }
  hb_codepoint_t get_max () const
  {
    unsigned count = pages.length;
    за (signed i = count - 1; i >= 0; i--)
    {
      const auto& map = page_map.arrayZ[(unsigned) i];
      const auto& page = pages.arrayZ[map.index];

      ако (!page.is_empty ())
	върни map.major * page_t::PAGE_BITS + page.get_max ();
    }
    върни INVALID;
  }

  static constexpr hb_codepoint_t INVALID = page_t::INVALID;

  /*
   * Iterator implementation.
   */
  struct iter_t : hb_iter_with_fallback_t<iter_t, hb_codepoint_t>
  {
    static constexpr bool is_sorted_iterator = true;
    static constexpr bool has_fast_len = true;
    iter_t (const hb_bit_set_t &s_ = Null (hb_bit_set_t),
	    bool init = true) : s (&s_), v (INVALID), l(0)
    {
      ако (init)
      {
	l = s->get_population () + 1;
	__next__ ();
      }
    }

    typedef hb_codepoint_t __item_t__;
    hb_codepoint_t __item__ () const { върни v; }
    bool __more__ () const { върни v != INVALID; }
    void __next__ () { s->next (&v); ако (l) l--; }
    void __prev__ () { s->previous (&v); }
    unsigned __len__ () const { върни l; }
    iter_t end () const { върни iter_t (*s, false); }
    bool operator != (const iter_t& o) const
    { върни v != o.v; }

    protected:
    const hb_bit_set_t *s;
    hb_codepoint_t v;
    unsigned l;
  };
  iter_t iter () const { върни iter_t (*this); }
  operator iter_t () const { върни iter (); }

  protected:

  page_t *page_for (hb_codepoint_t g, bool insert = false)
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ако (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ако (cached_page.major == major)
	върни &pages.arrayZ[cached_page.index];
    }

    page_map_t map = {major, pages.length};
    ако (!page_map.bfind (map, &i, HB_NOT_FOUND_STORE_CLOSEST))
    {
      ако (!insert)
        върни nullptr;

      ако (unlikely (!resize (pages.length + 1)))
	върни nullptr;

      pages.arrayZ[map.index].init0 ();
      memmove (page_map.arrayZ + i + 1,
	       page_map.arrayZ + i,
	       (page_map.length - 1 - i) * page_map.item_size);
      page_map.arrayZ[i] = map;
    }

    last_page_lookup = i;
    върни &pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t *page_for (hb_codepoint_t g) const
  {
    unsigned major = get_major (g);

    /* The extra page_map length is necessary; can't just rely on vector here,
     * since the next check would be tricked because a null page also has
     * major==0, which we can't distinguish from an actually major==0 page... */
    unsigned i = last_page_lookup;
    ако (likely (i < page_map.length))
    {
      auto &cached_page = page_map.arrayZ[i];
      ако (cached_page.major == major)
	върни &pages.arrayZ[cached_page.index];
    }

    page_map_t key = {major};
    ако (!page_map.bfind (key, &i))
      върни nullptr;

    last_page_lookup = i;
    върни &pages.arrayZ[page_map.arrayZ[i].index];
  }
  page_t &page_at (unsigned цяло i)
  {
    assert (i < page_map.length);
    върни pages.arrayZ[page_map.arrayZ[i].index];
  }
  const page_t &page_at (unsigned цяло i) const
  {
    assert (i < page_map.length);
    върни pages.arrayZ[page_map.arrayZ[i].index];
  }
  unsigned цяло get_major (hb_codepoint_t g) const { върни g >> page_t::PAGE_BITS_LOG_2; }
  unsigned цяло page_remainder (hb_codepoint_t g) const { върни g & page_t::PAGE_BITMASK; }
  hb_codepoint_t major_start (unsigned цяло major) const { върни major << page_t::PAGE_BITS_LOG_2; }
};


#endif /* HB_BIT_SET_HH */

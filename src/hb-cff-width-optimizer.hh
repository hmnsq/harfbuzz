/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#ifndef HB_CFF_WIDTH_OPTIMIZER_HH
#define HB_CFF_WIDTH_OPTIMIZER_HH

#अन्तर्भाव "hb.hh"

नामाकाश CFF {

/* Calculate byte cost for encoding a width delta */
स्थिर अन्तःस्थ unsigned
width_delta_cost (पूर्णाङ्क delta)
{
  delta = abs (delta);
  यदि (delta <= 107) प्रत्यागम 1;
  यदि (delta <= 1131) प्रत्यागम 2;
  प्रत्यागम 5;
}

/* Cumulative sum forward */
स्थिर शून्य
cumsum_forward (ध्रुव hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  प्रति (unsigned x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
स्थिर शून्य
cummax_forward (ध्रुव hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  प्रति (unsigned x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
स्थिर शून्य
cumsum_backward (ध्रुव hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  प्रति (पूर्णाङ्क x = (पूर्णाङ्क) max_w; x >= (पूर्णाङ्क) min_w; x--)
  {
    v += freq.get ((unsigned) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
स्थिर शून्य
cummax_backward (ध्रुव hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  प्रति (पूर्णाङ्क x = (पूर्णाङ्क) max_w; x >= (पूर्णाङ्क) min_w; x--)
  {
    v = hb_max (v, freq.get ((unsigned) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
स्थिर अन्तःस्थ unsigned
safe_get (ध्रुव hb_vector_t<unsigned> &vec, पूर्णाङ्क x, unsigned min_w, unsigned max_w)
{
  यदि (x < (पूर्णाङ्क) min_w || x > (पूर्णाङ्क) max_w) प्रत्यागम 0;
  प्रत्यागम vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
स्थिर शून्य
optimize_widths (ध्रुव hb_vector_t<unsigned> &width_list,
                 unsigned &default_width,
                 unsigned &nominal_width)
{
  यदि (width_list.length == 0)
  {
    default_width = nominal_width = 0;
    प्रत्यागम;
  }

  /* Build frequency map */
  hb_hashmap_t<unsigned, unsigned> widths;
  unsigned min_w = width_list[0];
  unsigned max_w = width_list[0];

  प्रति (unsigned w : width_list)
  {
    widths.set (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_vector_t<unsigned> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  auto nomnCost = [&] (unsigned x) -> unsigned {
    प्रत्यागम safe_get (cumFrqU, x, min_w, max_w) +
           safe_get (cumFrqU, x - 108, min_w, max_w) +
           safe_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           safe_get (cumFrqD, x, min_w, max_w) +
           safe_get (cumFrqD, x + 108, min_w, max_w) +
           safe_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  auto dfltCost = [&] (unsigned x) -> unsigned {
    unsigned u = hb_max (hb_max (safe_get (cumMaxU, x, min_w, max_w),
                                  safe_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         safe_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    unsigned d = hb_max (hb_max (safe_get (cumMaxD, x, min_w, max_w),
                                  safe_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         safe_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    प्रत्यागम hb_max (u, d);
  };

  /* Find best nominal */
  unsigned best_nominal = min_w;
  unsigned best_cost = nomnCost (min_w) - dfltCost (min_w);

  प्रति (unsigned x = min_w + 1; x <= max_w; x++)
  {
    unsigned cost = nomnCost (x) - dfltCost (x);
    यदि (cost < best_cost)
    {
      best_cost = cost;
      best_nominal = x;
    }
  }

  /* Work back the best default */
  unsigned best_default = best_nominal;
  unsigned best_default_cost = (unsigned) -1;

  /* Check candidates around best_nominal */
  पूर्णाङ्क candidates[] = {
    (पूर्णाङ्क) best_nominal,
    (पूर्णाङ्क) best_nominal - 108,
    (पूर्णाङ्क) best_nominal - 1132,
    (पूर्णाङ्क) best_nominal + 108,
    (पूर्णाङ्क) best_nominal + 1132
  };

  प्रति (पूर्णाङ्क candidate : candidates)
  {
    यदि (candidate < (पूर्णाङ्क) min_w || candidate > (पूर्णाङ्क) max_w)
      अनुवर्त;

    /* Compute actual cost with this default */
    unsigned cost = 0;
    प्रति (auto kv : widths.iter ())
    {
      unsigned w = kv.first;
      unsigned freq = kv.second;

      यदि (w == (unsigned) candidate)
        अनुवर्त;

      cost += freq * width_delta_cost ((पूर्णाङ्क) w - (पूर्णाङ्क) best_nominal);
    }

    यदि (cost < best_default_cost)
    {
      best_default_cost = cost;
      best_default = (unsigned) candidate;
    }
  }

  default_width = best_default;
  nominal_width = best_nominal;
}

} /* namespace CFF */

#endif /* HB_CFF_WIDTH_OPTIMIZER_HH */

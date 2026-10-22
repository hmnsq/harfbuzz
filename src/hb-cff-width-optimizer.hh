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

#ಅನ್ತರ್ಭಾವ "hb.hh"

ನಾಮಾಕಾಶ CFF {

/* Calculate byte cost for encoding a width delta */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned
width_delta_cost (ಪೂರ್ಣಾಙ್ಕ delta)
{
  delta = abs (delta);
  ಯದಿ (delta <= 107) ಪ್ರತ್ಯಾಗಮ 1;
  ಯದಿ (delta <= 1131) ಪ್ರತ್ಯಾಗಮ 2;
  ಪ್ರತ್ಯಾಗಮ 5;
}

/* Cumulative sum forward */
ಸ್ಥಿರ ಶೂನ್ಯ
cumsum_forward (ಧ್ರುವ hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  ಪ್ರತಿ (unsigned x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ಸ್ಥಿರ ಶೂನ್ಯ
cummax_forward (ಧ್ರುವ hb_hashmap_t<unsigned, unsigned> &freq,
                unsigned min_w, unsigned max_w,
                hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  ಪ್ರತಿ (unsigned x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ಸ್ಥಿರ ಶೂನ್ಯ
cumsum_backward (ಧ್ರುವ hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  unsigned v = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = (ಪೂರ್ಣಾಙ್ಕ) max_w; x >= (ಪೂರ್ಣಾಙ್ಕ) min_w; x--)
  {
    v += freq.get ((unsigned) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ಸ್ಥಿರ ಶೂನ್ಯ
cummax_backward (ಧ್ರುವ hb_hashmap_t<unsigned, unsigned> &freq,
                 unsigned min_w, unsigned max_w,
                 hb_vector_t<unsigned> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  unsigned v = 0;
  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = (ಪೂರ್ಣಾಙ್ಕ) max_w; x >= (ಪೂರ್ಣಾಙ್ಕ) min_w; x--)
  {
    v = hb_max (v, freq.get ((unsigned) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ unsigned
safe_get (ಧ್ರುವ hb_vector_t<unsigned> &vec, ಪೂರ್ಣಾಙ್ಕ x, unsigned min_w, unsigned max_w)
{
  ಯದಿ (x < (ಪೂರ್ಣಾಙ್ಕ) min_w || x > (ಪೂರ್ಣಾಙ್ಕ) max_w) ಪ್ರತ್ಯಾಗಮ 0;
  ಪ್ರತ್ಯಾಗಮ vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ಸ್ಥಿರ ಶೂನ್ಯ
optimize_widths (ಧ್ರುವ hb_vector_t<unsigned> &width_list,
                 unsigned &default_width,
                 unsigned &nominal_width)
{
  ಯದಿ (width_list.length == 0)
  {
    default_width = nominal_width = 0;
    ಪ್ರತ್ಯಾಗಮ;
  }

  /* Build frequency map */
  hb_hashmap_t<unsigned, unsigned> widths;
  unsigned min_w = width_list[0];
  unsigned max_w = width_list[0];

  ಪ್ರತಿ (unsigned w : width_list)
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
    ಪ್ರತ್ಯಾಗಮ safe_get (cumFrqU, x, min_w, max_w) +
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
    ಪ್ರತ್ಯಾಗಮ hb_max (u, d);
  };

  /* Find best nominal */
  unsigned best_nominal = min_w;
  unsigned best_cost = nomnCost (min_w) - dfltCost (min_w);

  ಪ್ರತಿ (unsigned x = min_w + 1; x <= max_w; x++)
  {
    unsigned cost = nomnCost (x) - dfltCost (x);
    ಯದಿ (cost < best_cost)
    {
      best_cost = cost;
      best_nominal = x;
    }
  }

  /* Work back the best default */
  unsigned best_default = best_nominal;
  unsigned best_default_cost = (unsigned) -1;

  /* Check candidates around best_nominal */
  ಪೂರ್ಣಾಙ್ಕ candidates[] = {
    (ಪೂರ್ಣಾಙ್ಕ) best_nominal,
    (ಪೂರ್ಣಾಙ್ಕ) best_nominal - 108,
    (ಪೂರ್ಣಾಙ್ಕ) best_nominal - 1132,
    (ಪೂರ್ಣಾಙ್ಕ) best_nominal + 108,
    (ಪೂರ್ಣಾಙ್ಕ) best_nominal + 1132
  };

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ candidate : candidates)
  {
    ಯದಿ (candidate < (ಪೂರ್ಣಾಙ್ಕ) min_w || candidate > (ಪೂರ್ಣಾಙ್ಕ) max_w)
      ಅನುವರ್ತ;

    /* Compute actual cost with this default */
    unsigned cost = 0;
    ಪ್ರತಿ (auto kv : widths.iter ())
    {
      unsigned w = kv.first;
      unsigned freq = kv.second;

      ಯದಿ (w == (unsigned) candidate)
        ಅನುವರ್ತ;

      cost += freq * width_delta_cost ((ಪೂರ್ಣಾಙ್ಕ) w - (ಪೂರ್ಣಾಙ್ಕ) best_nominal);
    }

    ಯದಿ (cost < best_default_cost)
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

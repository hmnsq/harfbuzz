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
स्थिर ओळीत अचिन्हित
width_delta_cost (पूर्णांक delta)
{
  delta = असल (delta);
  जर (delta <= 107) परत 1;
  जर (delta <= 1131) परत 2;
  परत 5;
}

/* Cumulative sum forward */
स्थिर शून्य
cumsum_forward (स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                अचिन्हित min_w, अचिन्हित max_w,
                hb_vector_t<अचिन्हित> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  साठी (अचिन्हित x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
स्थिर शून्य
cummax_forward (स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                अचिन्हित min_w, अचिन्हित max_w,
                hb_vector_t<अचिन्हित> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  साठी (अचिन्हित x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
स्थिर शून्य
cumsum_backward (स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                 अचिन्हित min_w, अचिन्हित max_w,
                 hb_vector_t<अचिन्हित> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  साठी (पूर्णांक x = (पूर्णांक) max_w; x >= (पूर्णांक) min_w; x--)
  {
    v += freq.get ((अचिन्हित) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
स्थिर शून्य
cummax_backward (स्थिरांक hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                 अचिन्हित min_w, अचिन्हित max_w,
                 hb_vector_t<अचिन्हित> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  साठी (पूर्णांक x = (पूर्णांक) max_w; x >= (पूर्णांक) min_w; x--)
  {
    v = hb_max (v, freq.get ((अचिन्हित) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
स्थिर ओळीत अचिन्हित
safe_get (स्थिरांक hb_vector_t<अचिन्हित> &vec, पूर्णांक x, अचिन्हित min_w, अचिन्हित max_w)
{
  जर (x < (पूर्णांक) min_w || x > (पूर्णांक) max_w) परत 0;
  परत vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
स्थिर शून्य
optimize_widths (स्थिरांक hb_vector_t<अचिन्हित> &width_list,
                 अचिन्हित &default_width,
                 अचिन्हित &nominal_width)
{
  जर (width_list.length == 0)
  {
    default_width = nominal_width = 0;
    परत;
  }

  /* Build frequency map */
  hb_hashmap_t<अचिन्हित, अचिन्हित> widths;
  अचिन्हित min_w = width_list[0];
  अचिन्हित max_w = width_list[0];

  साठी (अचिन्हित w : width_list)
  {
    widths.set (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_vector_t<अचिन्हित> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  स्वतः nomnCost = [&] (अचिन्हित x) -> अचिन्हित {
    परत safe_get (cumFrqU, x, min_w, max_w) +
           safe_get (cumFrqU, x - 108, min_w, max_w) +
           safe_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           safe_get (cumFrqD, x, min_w, max_w) +
           safe_get (cumFrqD, x + 108, min_w, max_w) +
           safe_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  स्वतः dfltCost = [&] (अचिन्हित x) -> अचिन्हित {
    अचिन्हित u = hb_max (hb_max (safe_get (cumMaxU, x, min_w, max_w),
                                  safe_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         safe_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    अचिन्हित d = hb_max (hb_max (safe_get (cumMaxD, x, min_w, max_w),
                                  safe_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         safe_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    परत hb_max (u, d);
  };

  /* Find best nominal */
  अचिन्हित best_nominal = min_w;
  अचिन्हित best_cost = nomnCost (min_w) - dfltCost (min_w);

  साठी (अचिन्हित x = min_w + 1; x <= max_w; x++)
  {
    अचिन्हित cost = nomnCost (x) - dfltCost (x);
    जर (cost < best_cost)
    {
      best_cost = cost;
      best_nominal = x;
    }
  }

  /* Work back the best default */
  अचिन्हित best_default = best_nominal;
  अचिन्हित best_default_cost = (अचिन्हित) -1;

  /* Check candidates around best_nominal */
  पूर्णांक candidates[] = {
    (पूर्णांक) best_nominal,
    (पूर्णांक) best_nominal - 108,
    (पूर्णांक) best_nominal - 1132,
    (पूर्णांक) best_nominal + 108,
    (पूर्णांक) best_nominal + 1132
  };

  साठी (पूर्णांक candidate : candidates)
  {
    जर (candidate < (पूर्णांक) min_w || candidate > (पूर्णांक) max_w)
      सुरूठेवा;

    /* Compute actual cost with this default */
    अचिन्हित cost = 0;
    साठी (स्वतः kv : widths.iter ())
    {
      अचिन्हित w = kv.first;
      अचिन्हित freq = kv.second;

      जर (w == (अचिन्हित) candidate)
        सुरूठेवा;

      cost += freq * width_delta_cost ((पूर्णांक) w - (पूर्णांक) best_nominal);
    }

    जर (cost < best_default_cost)
    {
      best_default_cost = cost;
      best_default = (अचिन्हित) candidate;
    }
  }

  default_width = best_default;
  nominal_width = best_nominal;
}

} /* namespace CFF */

#endif /* HB_CFF_WIDTH_OPTIMIZER_HH */

/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#अगर_अघोषित HB_CFF_WIDTH_OPTIMIZER_HH
#घोषणा HB_CFF_WIDTH_OPTIMIZER_HH

#समावेश "hb.hh"

namespace CFF {

/* Calculate byte cost for encoding a width delta */
अटल अंतरभूत अचिन्हित
width_delta_cost (पूर्णांक delta)
{
  delta = असल (delta);
  अगर (delta <= 107) वापस 1;
  अगर (delta <= 1131) वापस 2;
  वापस 5;
}

/* Cumulative sum forward */
अटल व्योम
cumsum_forward (स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                अचिन्हित min_w, अचिन्हित max_w,
                hb_vector_t<अचिन्हित> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  क्रम (अचिन्हित x = min_w; x <= max_w; x++)
  {
    v += freq.लो (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
अटल व्योम
cummax_forward (स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                अचिन्हित min_w, अचिन्हित max_w,
                hb_vector_t<अचिन्हित> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  क्रम (अचिन्हित x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.लो (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
अटल व्योम
cumsum_backward (स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                 अचिन्हित min_w, अचिन्हित max_w,
                 hb_vector_t<अचिन्हित> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  क्रम (पूर्णांक x = (पूर्णांक) max_w; x >= (पूर्णांक) min_w; x--)
  {
    v += freq.लो ((अचिन्हित) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
अटल व्योम
cummax_backward (स्थिर hb_hashmap_t<अचिन्हित, अचिन्हित> &freq,
                 अचिन्हित min_w, अचिन्हित max_w,
                 hb_vector_t<अचिन्हित> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  अचिन्हित v = 0;
  क्रम (पूर्णांक x = (पूर्णांक) max_w; x >= (पूर्णांक) min_w; x--)
  {
    v = hb_max (v, freq.लो ((अचिन्हित) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
अटल अंतरभूत अचिन्हित
safe_get (स्थिर hb_vector_t<अचिन्हित> &vec, पूर्णांक x, अचिन्हित min_w, अचिन्हित max_w)
{
  अगर (x < (पूर्णांक) min_w || x > (पूर्णांक) max_w) वापस 0;
  वापस vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
अटल व्योम
optimize_widths (स्थिर hb_vector_t<अचिन्हित> &width_list,
                 अचिन्हित &default_width,
                 अचिन्हित &nominal_width)
{
  अगर (width_list.लंबाई == 0)
  {
    default_width = nominal_width = 0;
    वापस;
  }

  /* Build frequency map */
  hb_hashmap_t<अचिन्हित, अचिन्हित> widths;
  अचिन्हित min_w = width_list[0];
  अचिन्हित max_w = width_list[0];

  क्रम (अचिन्हित w : width_list)
  {
    widths.set (w, widths.लो (w) + 1);
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
    वापस safe_get (cumFrqU, x, min_w, max_w) +
           safe_get (cumFrqU, x - 108, min_w, max_w) +
           safe_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           safe_get (cumFrqD, x, min_w, max_w) +
           safe_get (cumFrqD, x + 108, min_w, max_w) +
           safe_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.लो (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  स्वतः dfltCost = [&] (अचिन्हित x) -> अचिन्हित {
    अचिन्हित u = hb_max (hb_max (safe_get (cumMaxU, x, min_w, max_w),
                                  safe_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         safe_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    अचिन्हित d = hb_max (hb_max (safe_get (cumMaxD, x, min_w, max_w),
                                  safe_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         safe_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    वापस hb_max (u, d);
  };

  /* Find best nominal */
  अचिन्हित best_nominal = min_w;
  अचिन्हित best_cost = nomnCost (min_w) - dfltCost (min_w);

  क्रम (अचिन्हित x = min_w + 1; x <= max_w; x++)
  {
    अचिन्हित cost = nomnCost (x) - dfltCost (x);
    अगर (cost < best_cost)
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

  क्रम (पूर्णांक candidate : candidates)
  {
    अगर (candidate < (पूर्णांक) min_w || candidate > (पूर्णांक) max_w)
      जारी;

    /* Compute actual cost with this default */
    अचिन्हित cost = 0;
    क्रम (स्वतः kv : widths.iter ())
    {
      अचिन्हित w = kv.first;
      अचिन्हित freq = kv.second;

      अगर (w == (अचिन्हित) candidate)
        जारी;

      cost += freq * width_delta_cost ((पूर्णांक) w - (पूर्णांक) best_nominal);
    }

    अगर (cost < best_default_cost)
    {
      best_default_cost = cost;
      best_default = (अचिन्हित) candidate;
    }
  }

  default_width = best_default;
  nominal_width = best_nominal;
}

} /* namespace CFF */

#पूर्ण_अगर /* HB_CFF_WIDTH_OPTIMIZER_HH */

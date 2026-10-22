/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#که_نه_تعریف_شوی HB_CFF_پلنوالی_OPTIMIZER_HH
#تعریف_کړه HB_CFF_پلنوالی_OPTIMIZER_HH

#ضمن "hb.hh"

نطاق CFF {

/* Calculate byte cost for encoding a width delta */
ساكن مضمن بینښه
پلنوالی_delta_لګښت (صحيح delta)
{
  delta = مطلق_قیمت (delta);
  إذا (delta <= 107) أرجع 1;
  إذا (delta <= 1131) أرجع 2;
  أرجع 5;
}

/* Cumulative sum forward */
ساكن فراغ
cumsum_forward (ثابت hb_hashmap_t<بینښه, بینښه> &freq,
                بینښه min_w, بینښه max_w,
                hb_ویکټور_t<بینښه> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بینښه v = 0;
  لكل (بینښه x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ساكن فراغ
cummax_forward (ثابت hb_hashmap_t<بینښه, بینښه> &freq,
                بینښه min_w, بینښه max_w,
                hb_ویکټور_t<بینښه> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بینښه v = 0;
  لكل (بینښه x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ساكن فراغ
cumsum_backward (ثابت hb_hashmap_t<بینښه, بینښه> &freq,
                 بینښه min_w, بینښه max_w,
                 hb_ویکټور_t<بینښه> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بینښه v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v += freq.get ((بینښه) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ساكن فراغ
cummax_backward (ثابت hb_hashmap_t<بینښه, بینښه> &freq,
                 بینښه min_w, بینښه max_w,
                 hb_ویکټور_t<بینښه> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بینښه v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v = hb_max (v, freq.get ((بینښه) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ساكن مضمن بینښه
خوندي_get (ثابت hb_ویکټور_t<بینښه> &vec, صحيح x, بینښه min_w, بینښه max_w)
{
  إذا (x < (صحيح) min_w || x > (صحيح) max_w) أرجع 0;
  أرجع vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ساكن فراغ
optimize_widths (ثابت hb_ویکټور_t<بینښه> &پلنوالی_list,
                 بینښه &default_پلنوالی,
                 بینښه &nominal_پلنوالی)
{
  إذا (پلنوالی_list.اوږدتیا == 0)
  {
    default_پلنوالی = nominal_پلنوالی = 0;
    أرجع;
  }

  /* Build frequency map */
  hb_hashmap_t<بینښه, بینښه> widths;
  بینښه min_w = پلنوالی_list[0];
  بینښه max_w = پلنوالی_list[0];

  لكل (بینښه w : پلنوالی_list)
  {
    widths.سېټ (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_ویکټور_t<بینښه> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  خپلکاری nomn_لګښت = [&] (بینښه x) -> بینښه {
    أرجع خوندي_get (cumFrqU, x, min_w, max_w) +
           خوندي_get (cumFrqU, x - 108, min_w, max_w) +
           خوندي_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           خوندي_get (cumFrqD, x, min_w, max_w) +
           خوندي_get (cumFrqD, x + 108, min_w, max_w) +
           خوندي_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  خپلکاری dflt_لګښت = [&] (بینښه x) -> بینښه {
    بینښه u = hb_max (hb_max (خوندي_get (cumMaxU, x, min_w, max_w),
                                  خوندي_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         خوندي_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    بینښه d = hb_max (hb_max (خوندي_get (cumMaxD, x, min_w, max_w),
                                  خوندي_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         خوندي_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    أرجع hb_max (u, d);
  };

  /* Find best nominal */
  بینښه غوره_nominal = min_w;
  بینښه غوره_لګښت = nomn_لګښت (min_w) - dflt_لګښت (min_w);

  لكل (بینښه x = min_w + 1; x <= max_w; x++)
  {
    بینښه لګښت = nomn_لګښت (x) - dflt_لګښت (x);
    إذا (لګښت < غوره_لګښت)
    {
      غوره_لګښت = لګښت;
      غوره_nominal = x;
    }
  }

  /* Work back the best default */
  بینښه غوره_default = غوره_nominal;
  بینښه غوره_default_لګښت = (بینښه) -1;

  /* Check candidates around best_nominal */
  صحيح candidates[] = {
    (صحيح) غوره_nominal,
    (صحيح) غوره_nominal - 108,
    (صحيح) غوره_nominal - 1132,
    (صحيح) غوره_nominal + 108,
    (صحيح) غوره_nominal + 1132
  };

  لكل (صحيح candidate : candidates)
  {
    إذا (candidate < (صحيح) min_w || candidate > (صحيح) max_w)
      استمر;

    /* Compute actual cost with this default */
    بینښه لګښت = 0;
    لكل (خپلکاری kv : widths.iter ())
    {
      بینښه w = kv.لومړی;
      بینښه freq = kv.second;

      إذا (w == (بینښه) candidate)
        استمر;

      لګښت += freq * پلنوالی_delta_لګښت ((صحيح) w - (صحيح) غوره_nominal);
    }

    إذا (لګښت < غوره_default_لګښت)
    {
      غوره_default_لګښت = لګښت;
      غوره_default = (بینښه) candidate;
    }
  }

  default_پلنوالی = غوره_default;
  nominal_پلنوالی = غوره_nominal;
}

} /* namespace CFF */

#د_که_پای /* HB_CFF_WIDTH_OPTIMIZER_HH */

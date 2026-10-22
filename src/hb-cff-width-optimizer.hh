/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#جے_بنا_تعریف HB_CFF_چوڑائی_OPTIMIZER_HH
#تعریف_کرو HB_CFF_چوڑائی_OPTIMIZER_HH

#ضمن "hb.hh"

نطاق CFF {

/* Calculate byte cost for encoding a width delta */
ساكن مضمن بنانشانی
چوڑائی_delta_لاگت (صحيح delta)
{
  delta = مطلق_قدر (delta);
  إذا (delta <= 107) أرجع 1;
  إذا (delta <= 1131) أرجع 2;
  أرجع 5;
}

/* Cumulative sum forward */
ساكن فراغ
cumsum_forward (ثابت hb_hashmap_t<بنانشانی, بنانشانی> &freq,
                بنانشانی min_w, بنانشانی max_w,
                hb_سمتیہ_t<بنانشانی> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بنانشانی v = 0;
  لكل (بنانشانی x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
ساكن فراغ
cummax_forward (ثابت hb_hashmap_t<بنانشانی, بنانشانی> &freq,
                بنانشانی min_w, بنانشانی max_w,
                hb_سمتیہ_t<بنانشانی> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بنانشانی v = 0;
  لكل (بنانشانی x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
ساكن فراغ
cumsum_backward (ثابت hb_hashmap_t<بنانشانی, بنانشانی> &freq,
                 بنانشانی min_w, بنانشانی max_w,
                 hb_سمتیہ_t<بنانشانی> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  بنانشانی v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v += freq.get ((بنانشانی) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
ساكن فراغ
cummax_backward (ثابت hb_hashmap_t<بنانشانی, بنانشانی> &freq,
                 بنانشانی min_w, بنانشانی max_w,
                 hb_سمتیہ_t<بنانشانی> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  بنانشانی v = 0;
  لكل (صحيح x = (صحيح) max_w; x >= (صحيح) min_w; x--)
  {
    v = hb_max (v, freq.get ((بنانشانی) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
ساكن مضمن بنانشانی
محفوظ_get (ثابت hb_سمتیہ_t<بنانشانی> &vec, صحيح x, بنانشانی min_w, بنانشانی max_w)
{
  إذا (x < (صحيح) min_w || x > (صحيح) max_w) أرجع 0;
  أرجع vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
ساكن فراغ
optimize_widths (ثابت hb_سمتیہ_t<بنانشانی> &چوڑائی_list,
                 بنانشانی &default_چوڑائی,
                 بنانشانی &nominal_چوڑائی)
{
  إذا (چوڑائی_list.لمائی == 0)
  {
    default_چوڑائی = nominal_چوڑائی = 0;
    أرجع;
  }

  /* Build frequency map */
  hb_hashmap_t<بنانشانی, بنانشانی> widths;
  بنانشانی min_w = چوڑائی_list[0];
  بنانشانی max_w = چوڑائی_list[0];

  لكل (بنانشانی w : چوڑائی_list)
  {
    widths.سیٹ (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_سمتیہ_t<بنانشانی> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  خودکار nomn_لاگت = [&] (بنانشانی x) -> بنانشانی {
    أرجع محفوظ_get (cumFrqU, x, min_w, max_w) +
           محفوظ_get (cumFrqU, x - 108, min_w, max_w) +
           محفوظ_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           محفوظ_get (cumFrqD, x, min_w, max_w) +
           محفوظ_get (cumFrqD, x + 108, min_w, max_w) +
           محفوظ_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  خودکار dflt_لاگت = [&] (بنانشانی x) -> بنانشانی {
    بنانشانی u = hb_max (hb_max (محفوظ_get (cumMaxU, x, min_w, max_w),
                                  محفوظ_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         محفوظ_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    بنانشانی d = hb_max (hb_max (محفوظ_get (cumMaxD, x, min_w, max_w),
                                  محفوظ_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         محفوظ_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    أرجع hb_max (u, d);
  };

  /* Find best nominal */
  بنانشانی بہترین_nominal = min_w;
  بنانشانی بہترین_لاگت = nomn_لاگت (min_w) - dflt_لاگت (min_w);

  لكل (بنانشانی x = min_w + 1; x <= max_w; x++)
  {
    بنانشانی لاگت = nomn_لاگت (x) - dflt_لاگت (x);
    إذا (لاگت < بہترین_لاگت)
    {
      بہترین_لاگت = لاگت;
      بہترین_nominal = x;
    }
  }

  /* Work back the best default */
  بنانشانی بہترین_default = بہترین_nominal;
  بنانشانی بہترین_default_لاگت = (بنانشانی) -1;

  /* Check candidates around best_nominal */
  صحيح candidates[] = {
    (صحيح) بہترین_nominal,
    (صحيح) بہترین_nominal - 108,
    (صحيح) بہترین_nominal - 1132,
    (صحيح) بہترین_nominal + 108,
    (صحيح) بہترین_nominal + 1132
  };

  لكل (صحيح candidate : candidates)
  {
    إذا (candidate < (صحيح) min_w || candidate > (صحيح) max_w)
      استمر;

    /* Compute actual cost with this default */
    بنانشانی لاگت = 0;
    لكل (خودکار kv : widths.iter ())
    {
      بنانشانی w = kv.پہلا;
      بنانشانی freq = kv.second;

      إذا (w == (بنانشانی) candidate)
        استمر;

      لاگت += freq * چوڑائی_delta_لاگت ((صحيح) w - (صحيح) بہترین_nominal);
    }

    إذا (لاگت < بہترین_default_لاگت)
    {
      بہترین_default_لاگت = لاگت;
      بہترین_default = (بنانشانی) candidate;
    }
  }

  default_چوڑائی = بہترین_default;
  nominal_چوڑائی = بہترین_nominal;
}

} /* namespace CFF */

#جے_انت /* HB_CFF_WIDTH_OPTIMIZER_HH */

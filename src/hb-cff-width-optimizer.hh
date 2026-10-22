/*
 * CFF Width Optimizer
 *
 * Determines optimal defaultWidthX and nominalWidthX values
 * to minimize CharString byte cost.
 *
 * Based on fontTools.cffLib.width
 */

#నిర్వచితం_కాకపోతే HB_CFF_వెడల్పు_OPTIMIZER_HH
#నిర్వచించు HB_CFF_వెడల్పు_OPTIMIZER_HH

#అన్తర్భావ "hb.hh"

నామాకాశ CFF {

/* Calculate byte cost for encoding a width delta */
స్థిర అంతఃవరుస గుర్తులేని
వెడల్పు_delta_ఖర్చు (పూర్ణాంకం delta)
{
  delta = నిరపేక్ష (delta);
  ఒకవేళ (delta <= 107) తిరిగి 1;
  ఒకవేళ (delta <= 1131) తిరిగి 2;
  తిరిగి 5;
}

/* Cumulative sum forward */
స్థిర శూన్య
cumsum_forward (స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని> &freq,
                గుర్తులేని min_w, గుర్తులేని max_w,
                hb_సదిశ_t<గుర్తులేని> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  గుర్తులేని v = 0;
  కొరకు (గుర్తులేని x = min_w; x <= max_w; x++)
  {
    v += freq.get (x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max forward */
స్థిర శూన్య
cummax_forward (స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని> &freq,
                గుర్తులేని min_w, గుర్తులేని max_w,
                hb_సదిశ_t<గుర్తులేని> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  గుర్తులేని v = 0;
  కొరకు (గుర్తులేని x = min_w; x <= max_w; x++)
  {
    v = hb_max (v, freq.get (x));
    cummax[x - min_w] = v;
  }
}

/* Cumulative sum backward */
స్థిర శూన్య
cumsum_backward (స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని> &freq,
                 గుర్తులేని min_w, గుర్తులేని max_w,
                 hb_సదిశ_t<గుర్తులేని> &cumsum)
{
  cumsum.resize (max_w - min_w + 1);
  గుర్తులేని v = 0;
  కొరకు (పూర్ణాంకం x = (పూర్ణాంకం) max_w; x >= (పూర్ణాంకం) min_w; x--)
  {
    v += freq.get ((గుర్తులేని) x);
    cumsum[x - min_w] = v;
  }
}

/* Cumulative max backward */
స్థిర శూన్య
cummax_backward (స్థిరరాశి hb_hashmap_t<గుర్తులేని, గుర్తులేని> &freq,
                 గుర్తులేని min_w, గుర్తులేని max_w,
                 hb_సదిశ_t<గుర్తులేని> &cummax)
{
  cummax.resize (max_w - min_w + 1);
  గుర్తులేని v = 0;
  కొరకు (పూర్ణాంకం x = (పూర్ణాంకం) max_w; x >= (పూర్ణాంకం) min_w; x--)
  {
    v = hb_max (v, freq.get ((గుర్తులేని) x));
    cummax[x - min_w] = v;
  }
}

/* Helper to safely get cumulative value with bounds checking */
స్థిర అంతఃవరుస గుర్తులేని
సురక్షిత_get (స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &vec, పూర్ణాంకం x, గుర్తులేని min_w, గుర్తులేని max_w)
{
  ఒకవేళ (x < (పూర్ణాంకం) min_w || x > (పూర్ణాంకం) max_w) తిరిగి 0;
  తిరిగి vec[x - min_w];
}

/* Optimize defaultWidthX and nominalWidthX from a list of widths
 * O(UPEM+numGlyphs) algorithm from fontTools.cffLib.width */
స్థిర శూన్య
optimize_widths (స్థిరరాశి hb_సదిశ_t<గుర్తులేని> &వెడల్పు_list,
                 గుర్తులేని &default_వెడల్పు,
                 గుర్తులేని &nominal_వెడల్పు)
{
  ఒకవేళ (వెడల్పు_list.నిడివి == 0)
  {
    default_వెడల్పు = nominal_వెడల్పు = 0;
    తిరిగి;
  }

  /* Build frequency map */
  hb_hashmap_t<గుర్తులేని, గుర్తులేని> widths;
  గుర్తులేని min_w = వెడల్పు_list[0];
  గుర్తులేని max_w = వెడల్పు_list[0];

  కొరకు (గుర్తులేని w : వెడల్పు_list)
  {
    widths.సమితి (w, widths.get (w) + 1);
    min_w = hb_min (min_w, w);
    max_w = hb_max (max_w, w);
  }

  /* Cumulative sum/max forward/backward */
  hb_సదిశ_t<గుర్తులేని> cumFrqU, cumMaxU, cumFrqD, cumMaxD;
  cumsum_forward (widths, min_w, max_w, cumFrqU);
  cummax_forward (widths, min_w, max_w, cumMaxU);
  cumsum_backward (widths, min_w, max_w, cumFrqD);
  cummax_backward (widths, min_w, max_w, cumMaxD);

  /* Cost per nominal choice, without default consideration */
  స్వయంచాలక nomn_ఖర్చు = [&] (గుర్తులేని x) -> గుర్తులేని {
    తిరిగి సురక్షిత_get (cumFrqU, x, min_w, max_w) +
           సురక్షిత_get (cumFrqU, x - 108, min_w, max_w) +
           సురక్షిత_get (cumFrqU, x - 1132, min_w, max_w) * 3 +
           సురక్షిత_get (cumFrqD, x, min_w, max_w) +
           సురక్షిత_get (cumFrqD, x + 108, min_w, max_w) +
           సురక్షిత_get (cumFrqD, x + 1132, min_w, max_w) * 3 -
           widths.get (x);
  };

  /* Cost-saving per nominal choice, by best default choice */
  స్వయంచాలక dflt_ఖర్చు = [&] (గుర్తులేని x) -> గుర్తులేని {
    గుర్తులేని u = hb_max (hb_max (సురక్షిత_get (cumMaxU, x, min_w, max_w),
                                  సురక్షిత_get (cumMaxU, x - 108, min_w, max_w) * 2),
                         సురక్షిత_get (cumMaxU, x - 1132, min_w, max_w) * 5);
    గుర్తులేని d = hb_max (hb_max (సురక్షిత_get (cumMaxD, x, min_w, max_w),
                                  సురక్షిత_get (cumMaxD, x + 108, min_w, max_w) * 2),
                         సురక్షిత_get (cumMaxD, x + 1132, min_w, max_w) * 5);
    తిరిగి hb_max (u, d);
  };

  /* Find best nominal */
  గుర్తులేని ఉత్తమ_nominal = min_w;
  గుర్తులేని ఉత్తమ_ఖర్చు = nomn_ఖర్చు (min_w) - dflt_ఖర్చు (min_w);

  కొరకు (గుర్తులేని x = min_w + 1; x <= max_w; x++)
  {
    గుర్తులేని ఖర్చు = nomn_ఖర్చు (x) - dflt_ఖర్చు (x);
    ఒకవేళ (ఖర్చు < ఉత్తమ_ఖర్చు)
    {
      ఉత్తమ_ఖర్చు = ఖర్చు;
      ఉత్తమ_nominal = x;
    }
  }

  /* Work back the best default */
  గుర్తులేని ఉత్తమ_default = ఉత్తమ_nominal;
  గుర్తులేని ఉత్తమ_default_ఖర్చు = (గుర్తులేని) -1;

  /* Check candidates around best_nominal */
  పూర్ణాంకం candidates[] = {
    (పూర్ణాంకం) ఉత్తమ_nominal,
    (పూర్ణాంకం) ఉత్తమ_nominal - 108,
    (పూర్ణాంకం) ఉత్తమ_nominal - 1132,
    (పూర్ణాంకం) ఉత్తమ_nominal + 108,
    (పూర్ణాంకం) ఉత్తమ_nominal + 1132
  };

  కొరకు (పూర్ణాంకం candidate : candidates)
  {
    ఒకవేళ (candidate < (పూర్ణాంకం) min_w || candidate > (పూర్ణాంకం) max_w)
      కొనసాగు;

    /* Compute actual cost with this default */
    గుర్తులేని ఖర్చు = 0;
    కొరకు (స్వయంచాలక kv : widths.iter ())
    {
      గుర్తులేని w = kv.మొదటి;
      గుర్తులేని freq = kv.second;

      ఒకవేళ (w == (గుర్తులేని) candidate)
        కొనసాగు;

      ఖర్చు += freq * వెడల్పు_delta_ఖర్చు ((పూర్ణాంకం) w - (పూర్ణాంకం) ఉత్తమ_nominal);
    }

    ఒకవేళ (ఖర్చు < ఉత్తమ_default_ఖర్చు)
    {
      ఉత్తమ_default_ఖర్చు = ఖర్చు;
      ఉత్తమ_default = (గుర్తులేని) candidate;
    }
  }

  default_వెడల్పు = ఉత్తమ_default;
  nominal_వెడల్పు = ఉత్తమ_nominal;
}

} /* namespace CFF */

#అయితే_ముగింపు /* HB_CFF_WIDTH_OPTIMIZER_HH */

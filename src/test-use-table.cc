#అన్తర్భావ "hb-ot-shaper-use-table.hh"

పూర్ణాంకం ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ఒకవేళ (argc != 2)
  {
    కొరకు (గుర్తులేని u = 0; u < 0x10FFFFu; u++)
      రాయి ("U+%04X %d\n", u, hb_use_get_category (u));
    తిరిగి 0;
  }

  hb_codepoint_t u;
  అక్షరశ్రేణి_చదువు (argv[1], "%x", &u);

  రాయి ("%d\n", hb_use_get_category (u));

  తిరిగి 0;
}

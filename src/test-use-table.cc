#include "hb-ot-shaper-use-table.hh"

පූර්ණ_සංඛ්‍යා main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  නම් (argc != 2)
  {
    සඳහා (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ආපසු 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ආපසු 0;
}

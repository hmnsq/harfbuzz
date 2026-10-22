#include "hb-ot-shaper-use-table.hh"

ကိန်းပြည့် main (ကိန်းပြည့် argc, var_na **argv)
{
  အကယ်၍ (argc != 2)
  {
    အတွက် (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ပြန်ပေး 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ပြန်ပေး 0;
}

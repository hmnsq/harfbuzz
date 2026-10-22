#include "hb-ot-shaper-use-table.hh"

ចំនួនគត់ main (ចំនួនគត់ argc, var_na **argv)
{
  ប្រសិនបើ (argc != 2)
  {
    សម្រាប់ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ត្រឡប់ 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ត្រឡប់ 0;
}

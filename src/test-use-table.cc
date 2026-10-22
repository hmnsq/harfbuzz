#include "hb-ot-shaper-use-table.hh"

ཧྲིལ གཙོ (ཧྲིལ argc, var_na **argv)
{
  གལ་ཏེ (argc != 2)
  {
    ཆེད་དུ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ཕྱིར 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ཕྱིར 0;
}

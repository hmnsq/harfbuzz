#include "hb-ot-shaper-use-table.hh"

số_nguyên main (số_nguyên argc, char **argv)
{
  nếu (argc != 2)
  {
    cho (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    trả_về 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  trả_về 0;
}

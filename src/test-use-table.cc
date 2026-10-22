#include "hb-ot-shaper-use-table.hh"

ຈຳນວນເຕັມ main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  ຖ້າ (argc != 2)
  {
    ສຳລັບ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ສົ່ງຄືນ 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ສົ່ງຄືນ 0;
}

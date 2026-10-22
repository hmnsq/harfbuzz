#include "hb-ot-shaper-use-table.hh"

puur_naa_nka main (puur_naa_nka argc, var_na **argv)
{
  ꦪꦼꦤ꧀ (argc != 2)
  {
    ꦏꦁꦒꦺꦴ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ꦮꦭꦶ 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ꦮꦭꦶ 0;
}

#ಅನ್ತರ್ಭಾವ "hb-ot-shaper-use-table.hh"

ಪೂರ್ಣಾಙ್ಕ main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಯದಿ (argc != 2)
  {
    ಪ್ರತಿ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ಪ್ರತ್ಯಾಗಮ 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ಪ್ರತ್ಯಾಗಮ 0;
}

#ଅନ୍ତର୍ଭାଵ "hb-ot-shaper-use-table.hh"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଯଦି (argc != 2)
  {
    ପାଇଁ (unsigned u = 0; u < 0x10FFFFu; u++)
      printf ("U+%04X %d\n", u, hb_use_get_category (u));
    ଫେରନ୍ତୁ 0;
  }

  hb_codepoint_t u;
  sscanf (argv[1], "%x", &u);

  printf ("%d\n", hb_use_get_category (u));

  ଫେରନ୍ତୁ 0;
}

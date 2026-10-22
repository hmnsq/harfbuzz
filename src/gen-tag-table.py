#!/usr/bin/env python3

"""Generator of the mapping from OpenType tags to BCP 47 tags and vice
versa.

It creates a ``const LangTag[]``, matching the tags from the OpenType
languages system tag list to the language subtags of the BCP 47 language
subtag registry, with some manual adjustments. The mappings are
supplemented with macrolanguages' sublanguages and retired codes'
replacements, according to BCP 47 and some manual additions where BCP 47
omits a retired code entirely.

Also generated is a function, ``hb_ot_ambiguous_tag_to_language``,
intended for use by ``hb_ot_tag_to_language``. It maps OpenType tags
back to BCP 47 tags. Ambiguous OpenType tags (those that correspond to
multiple BCP 47 tags) are listed here, except when the alphabetically
first BCP 47 tag happens to be the chosen disambiguated tag. In that
case, the fallback behavior will choose the right tag anyway.

usage: ./gen-tag-table.py [--rust] languagetags language-subtag-registry

Input files:
* https://docs.microsoft.com/en-us/typography/opentype/spec/languagetags
* https://www.iana.org/assignments/language-subtag-registry/language-subtag-registry
"""

استورد collections
استورد html
من html.parser استورد HTMLParser
استورد itertools
استورد os
استورد re
استورد sys
استورد unicodedata

إذا لمبائی (sys.argv) > 1 و sys.argv[1] == '--rust':
	ہٹاؤ sys.argv[1]
	output_rust = سچ
وإلا:
	output_rust = خطأ

إذا لمبائی (sys.argv) != 3:
	sys.exit (__doc__)

تعریف expect (condition, سنیہا=کجھ_نئیں):
	إذا ليس condition:
		إذا سنیہا اے کجھ_نئیں:
			ارفع AssertionError
		ارفع AssertionError (سنیہا)

تعریف اكتب (s):
	sys.stdout.flush ()
	sys.stdout.بفر.اكتب (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = سیٹ (" -—–.,;:’'ʼ()[]/!=>_+")

تعریف sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	إذا ليس s:
		أرجع ''
	لكل ch في s:
		cat = unicodedata.category (ch)
		إذا ليس (cat.startswith (('L', 'M', 'N', 'Z')) أو ch في _ALLOWED_COMMENT_PUNCT):
			ارفع قدر_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	أرجع s

تعریف sanitize_سرخی (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	إذا ليس re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		ارفع قدر_Error ("Invalid header line %r: contains disallowed characters" % s)
	أرجع s

تعریف sanitize_bcp47_ٹیگ (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (s, str):
		ارفع TypeError ("Expected string, got %s" % type (s).__name__)
	إذا ليس re.fullmatch (r'[a-zA-Z0-9-]+', s):
		ارفع قدر_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	أرجع s

تعریف sanitize_ٹیگ (ٹیگ_2):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	إذا ليس isinstance (ٹیگ_2, str):
		ارفع TypeError ("Expected string for tag, got %s" % type (ٹیگ_2).__name__)
	إذا ٹیگ_2 == DEFAULT_LANGUAGE_SYSTEM:
		أرجع ''
	إذا ليس re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', ٹیگ_2):
		ارفع قدر_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % ٹیگ_2)
	أرجع ٹیگ_2

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_منزل_1 = {
	'aar': 'aa',
	'abk': 'ab',
	'afr': 'af',
	'aka': 'ak',
	'amh': 'am',
	'ara': 'ar',
	'arg': 'an',
	'asm': 'as',
	'ava': 'av',
	'ave': 'ae',
	'aym': 'ay',
	'aze': 'az',
	'bak': 'ba',
	'bam': 'bm',
	'bel': 'be',
	'ben': 'bn',
	'bis': 'bi',
	'bod': 'bo',
	'bos': 'bs',
	'bre': 'br',
	'bul': 'bg',
	'cat': 'ca',
	'ces': 'cs',
	'cha': 'ch',
	'che': 'ce',
	'chu': 'cu',
	'chv': 'cv',
	'cor': 'kw',
	'cos': 'co',
	'cre': 'cr',
	'cym': 'cy',
	'dan': 'da',
	'deu': 'de',
	'div': 'dv',
	'dzo': 'dz',
	'ell': 'el',
	'eng': 'en',
	'epo': 'eo',
	'est': 'et',
	'eus': 'eu',
	'ewe': 'ee',
	'fao': 'fo',
	'fas': 'fa',
	'fij': 'fj',
	'fin': 'fi',
	'fra': 'fr',
	'fry': 'fy',
	'ful': 'ff',
	'gla': 'gd',
	'gle': 'ga',
	'glg': 'gl',
	'glv': 'gv',
	'grn': 'gn',
	'guj': 'gu',
	'hat': 'ht',
	'hau': 'ha',
	'hbs': 'sh',
	'heb': 'he',
	'her': 'hz',
	'hin': 'hi',
	'hmo': 'ho',
	'hrv': 'hr',
	'hun': 'hu',
	'hye': 'hy',
	'ibo': 'ig',
	'ido': 'io',
	'iii': 'ii',
	'iku': 'iu',
	'ile': 'ie',
	'ina': 'ia',
	'ind': 'id',
	'ipk': 'ik',
	'isl': 'is',
	'ita': 'it',
	'jav': 'jv',
	'jpn': 'ja',
	'kal': 'kl',
	'kan': 'kn',
	'kas': 'ks',
	'kat': 'ka',
	'kau': 'kr',
	'kaz': 'kk',
	'khm': 'km',
	'kik': 'ki',
	'kin': 'rw',
	'kir': 'ky',
	'kom': 'kv',
	'kon': 'kg',
	'kor': 'ko',
	'kua': 'kj',
	'kur': 'ku',
	'lao': 'lo',
	'lat': 'la',
	'lav': 'lv',
	'lim': 'li',
	'lin': 'ln',
	'lit': 'lt',
	'ltz': 'lb',
	'lub': 'lu',
	'lug': 'lg',
	'mah': 'mh',
	'mal': 'ml',
	'mar': 'mr',
	'mkd': 'mk',
	'mlg': 'mg',
	'mlt': 'mt',
	'mol': 'mo',
	'mon': 'mn',
	'mri': 'mi',
	'msa': 'ms',
	'mya': 'my',
	'nau': 'na',
	'nav': 'nv',
	'nbl': 'nr',
	'nde': 'nd',
	'ndo': 'ng',
	'nep': 'ne',
	'nld': 'nl',
	'nno': 'nn',
	'nob': 'nb',
	'nor': 'no',
	'nya': 'ny',
	'oci': 'oc',
	'oji': 'oj',
	'ori': 'or',
	'orm': 'om',
	'oss': 'os',
	'pan': 'pa',
	'pli': 'pi',
	'pol': 'pl',
	'por': 'pt',
	'pus': 'ps',
	'que': 'qu',
	'roh': 'rm',
	'ron': 'ro',
	'run': 'rn',
	'rus': 'ru',
	'sag': 'sg',
	'san': 'sa',
	'sin': 'si',
	'slk': 'sk',
	'slv': 'sl',
	'sme': 'se',
	'smo': 'sm',
	'sna': 'sn',
	'snd': 'sd',
	'som': 'so',
	'sot': 'st',
	'spa': 'es',
	'sqi': 'sq',
	'srd': 'sc',
	'srp': 'sr',
	'ssw': 'ss',
	'sun': 'su',
	'swa': 'sw',
	'swe': 'sv',
	'tah': 'ty',
	'tam': 'ta',
	'tat': 'tt',
	'tel': 'te',
	'tgk': 'tg',
	'tgl': 'tl',
	'tha': 'th',
	'tir': 'ti',
	'ton': 'to',
	'tsn': 'tn',
	'tso': 'ts',
	'tuk': 'tk',
	'tur': 'tr',
	'twi': 'tw',
	'uig': 'ug',
	'ukr': 'uk',
	'urd': 'ur',
	'uzb': 'uz',
	'ven': 've',
	'vie': 'vi',
	'vol': 'vo',
	'wln': 'wa',
	'wol': 'wo',
	'xho': 'xh',
	'yid': 'yi',
	'yor': 'yo',
	'zha': 'za',
	'zho': 'zh',
	'zul': 'zu',
}

صنف Language_ٹیگ (object):
	"""A BCP 47 language tag.

	Attributes:
		subtags (List[str]): The list of subtags in this tag.
		grandfathered (bool): Whether this tag is grandfathered. If
			``true``, the entire lowercased tag is the ``language``
			and the other subtag fields are empty.
		language (str): The language subtag.
		script (str): The script subtag.
		region (str): The region subtag.
		variant (str): The variant subtag.

	Args:
		tag (str): A BCP 47 language tag.

	"""
	تعریف __init__ (self, ٹیگ_2):
		عام bcp_47
		self.subtags = ٹیگ_2.ہیٹھلا ().ونڈو ('-')
		self.grandfathered = ٹیگ_2.ہیٹھلا () في bcp_47.grandfathered
		إذا self.grandfathered:
			self.language = ٹیگ_2.ہیٹھلا ()
			self.script = ''
			self.region = ''
			self.variant = ''
		وإلا:
			self.language = self.subtags[0]
			self.script = self.لبھو_پہلا (مجهول s: لمبائی (s) == 4 و s[0] > '9', self.subtags)
			self.region = self.لبھو_پہلا (مجهول s: لمبائی (s) == 2 و s[0] > '9' أو لمبائی (s) == 3 و s[0] <= '9', self.subtags[1:])
			self.variant = self.لبھو_پہلا (مجهول s: لمبائی (s) > 4 أو لمبائی (s) == 4 و s[0] <= '9', self.subtags)

	تعریف __str__(self):
		أرجع '-'.join(self.subtags)

	تعریف __repr__ (self):
		أرجع 'LanguageTag(%r)' % str(self)

	@staticmethod
	تعریف لبھو_پہلا (function, sequence):
		حاول:
			أرجع اگلا (iter (filter (function, sequence)))
		استثناء StopIteration:
			أرجع کجھ_نئیں

	تعریف is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		أرجع ليس (لمبائی (self.subtags) == 1
			أو self.grandfathered
			و لمبائی (self.subtags[1]) != 3
			و ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	تعریف get_گروہ (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		أرجع ('und'
			إذا (self.language == 'und'
				أو self.variant في bcp_47.prefixes و لمبائی (bcp_47.prefixes[self.variant]) == 1)
			وإلا self.language[0])

صنف OpenTypeRegistryParser (HTMLParser):
	"""A parser for the OpenType language system tag registry.

	Attributes:
		header (str): The "last updated" line of the registry.
		names (Mapping[str, str]): A map of language system tags to the
			names they are given in the registry.
		ranks (DefaultDict[str, int]): A map of language system tags to
			numbers. If a single BCP 47 tag corresponds to multiple
			OpenType tags, the tags are ordered in increasing order by
			rank. The rank is based on the number of BCP 47 tags
			associated with a tag, though it may be manually modified.
		to_bcp_47 (DefaultDict[str, AbstractSet[str]]): A map of
			OpenType language system tags to sets of BCP 47 tags.
		from_bcp_47 (DefaultDict[str, AbstractSet[str]]): ``to_bcp_47``
			inverted. Its values start as unsorted sets;
			``sort_languages`` converts them to sorted lists.
		from_bcp_47_uninherited (Optional[Dict[str, AbstractSet[str]]]):
			A copy of ``from_bcp_47``. It starts as ``None`` and is
			populated at the beginning of the first call to
			``inherit_from_macrolanguages``.

	"""
	تعریف __init__ (self):
		HTMLParser.__init__ (self)
		self.سرخی = ''
		self.ناں = {}
		self.ranks = collections.defaultdict (صحيح)
		self.منزل_bcp_47 = collections.defaultdict (سیٹ)
		self.from_bcp_47 = collections.defaultdict (سیٹ)
		self.from_bcp_47_uninherited = کجھ_نئیں
		# Whether the parser is in a <td> element
		self._td = خطأ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = خطأ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	تعریف handle_starttag (self, ٹیگ_2, attrs):
		إذا ٹیگ_2 == 'a':
			إذا self._current_tr و ليس self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = سچ
		وإلاإذا ٹیگ_2 == 'br':
			self._disengaged = سچ
		وإلاإذا ٹیگ_2 == 'meta':
			لكل attr, قدر_2 في attrs:
				إذا attr == 'name' و قدر_2 == 'updated_at':
					self.سرخی = self.get_starttag_متن ()
					اكسر
		وإلاإذا ٹیگ_2 == 'td':
			self._td = سچ
			self._current_tr.append ('')
		وإلاإذا ٹیگ_2 == 'tr':
			self._disengaged = خطأ
			self._current_tr = []

	تعریف handle_endtag (self, ٹیگ_2):
		إذا ٹیگ_2 == 'td':
			self._td = خطأ
			self._disengaged = خطأ
		وإلاإذا ٹیگ_2 == 'tr' و self._current_tr:
			expect (2 <= لمبائی (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			ٹیگ_2 = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			إذا لمبائی (ٹیگ_2) > 4:
				expect (ٹیگ_2.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % ٹیگ_2)
				name += ' (deprecated)'
				ٹیگ_2 = ٹیگ_2.ونڈو (' ')[0]
				rank = 1
			self.ناں[ٹیگ_2] = re.sub (' languages$', '', name)
			إذا ليس self._current_tr[2]:
				أرجع
			iso_codes = self._current_tr[2].strip ()
			self.منزل_bcp_47[ٹیگ_2].update (ISO_639_3_منزل_1.get (کوڈ, کوڈ) لكل کوڈ في iso_codes.replace (' ', '').ونڈو (','))
			rank += 2 * لمبائی (self.منزل_bcp_47[ٹیگ_2])
			self.ranks[ٹیگ_2] = rank

	تعریف handle_انکڑے (self, انکڑے):
		إذا self._td و ليس self._disengaged:
			self._current_tr[-1] += انکڑے

	تعریف handle_charref (self, name):
		self.handle_انکڑے (html.unescape ('&#%s;' % name))

	تعریف handle_entityref (self, name):
		self.handle_انکڑے (html.unescape ('&%s;' % name))

	تعریف parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		نال open (filename, encoding='utf-8') كـ f:
			self.feed (f.اقرأ ())
		expect (self.سرخی)
		لكل ٹیگ_2, iso_codes في self.منزل_bcp_47.اشیاء ():
			لكل iso_کوڈ في iso_codes:
				self.from_bcp_47[iso_کوڈ].جوڑو (ٹیگ_2)

	تعریف جوڑو_language (self, bcp_47_ٹیگ, ot_ٹیگ):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		عام bcp_47
		self.منزل_bcp_47[ot_ٹیگ].جوڑو (bcp_47_ٹیگ)
		self.from_bcp_47[bcp_47_ٹیگ].جوڑو (ot_ٹیگ)
		إذا bcp_47_ٹیگ.ہیٹھلا () ليس في bcp_47.grandfathered:
			حاول:
				[macrolanguage, suffix] = bcp_47_ٹیگ.ونڈو ('-', 1)
				إذا macrolanguage في bcp_47.macrolanguages:
					s = سیٹ ()
					لكل language في bcp_47.macrolanguages[macrolanguage]:
						إذا language.ہیٹھلا () ليس في bcp_47.grandfathered:
							s.جوڑو ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			استثناء قدر_Error:
				لنگھو

	@staticmethod
	تعریف _remove_language (ٹیگ_1, dict_1, dict_2):
		لكل ٹیگ_2_2 في dict_1.نکالو (ٹیگ_1):
			dict_2[ٹیگ_2_2].remove (ٹیگ_1)
			إذا ليس dict_2[ٹیگ_2_2]:
				ہٹاؤ dict_2[ٹیگ_2_2]

	تعریف remove_language_ot (self, ot_ٹیگ):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_ٹیگ, self.منزل_bcp_47, self.from_bcp_47)

	تعریف remove_language_bcp_47 (self, bcp_47_ٹیگ):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_ٹیگ, self.from_bcp_47, self.منزل_bcp_47)

	تعریف inherit_from_macrolanguages (self):
		"""Copy mappings from macrolanguages to individual languages.

		If a BCP 47 tag for an individual mapping has no OpenType
		mapping but its macrolanguage does, the mapping is copied to
		the individual language. For example, als (Tosk Albanian) has no
		explicit mapping, so it inherits from sq (Albanian) the mapping
		to SQI.

		However, if an OpenType tag maps to a BCP 47 macrolanguage and
		some but not all of its individual languages, the mapping is not
		inherited from the macrolanguage to the missing individual
		languages. For example, INUK (Nunavik Inuktitut) is mapped to
		ike (Eastern Canadian Inuktitut) and iu (Inuktitut) but not to
		ikt (Inuinnaqtun, which is an individual language of iu), so
		this method does not add a mapping from ikt to INUK.

		If a BCP 47 tag for a macrolanguage has no OpenType mapping but
		some of its individual languages do, their mappings are copied
		to the macrolanguage.
		"""
		عام bcp_47
		پہلا_ویلا = self.from_bcp_47_uninherited اے کجھ_نئیں
		إذا پہلا_ویلا:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		لكل macrolanguage, languages في dict (bcp_47.macrolanguages).اشیاء ():
			ot_macrolanguages = {
				ot_macrolanguage لكل ot_macrolanguage في self.from_bcp_47_uninherited.get (macrolanguage, سیٹ ())
			}
			blocked_ot_macrolanguages = سیٹ ()
			إذا 'retired code' ليس في bcp_47.scopes.get (macrolanguage, ''):
				لكل ot_macrolanguage في ot_macrolanguages:
					گیڑا_trip_macrolanguages = {
						l لكل l في self.منزل_bcp_47[ot_macrolanguage]
						إذا 'retired code' ليس في bcp_47.scopes.get (l, '')
					}
					گیڑا_trip_languages = {
						l لكل l في languages
						إذا 'retired code' ليس في bcp_47.scopes.get (l, '')
					}
					intersection = گیڑا_trip_macrolanguages & گیڑا_trip_languages
					إذا intersection و intersection != گیڑا_trip_languages:
						blocked_ot_macrolanguages.جوڑو (ot_macrolanguage)
			إذا ot_macrolanguages:
				لكل ot_macrolanguage في ot_macrolanguages:
					إذا ot_macrolanguage ليس في blocked_ot_macrolanguages:
						لكل language في languages:
							self.جوڑو_language (language, ot_macrolanguage)
							إذا ليس blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			وإلاإذا پہلا_ویلا:
				لكل language في languages:
					إذا language في self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					وإلا:
						ot_macrolanguages.clear ()
					إذا ليس ot_macrolanguages:
						اكسر
				لكل ot_macrolanguage في ot_macrolanguages:
					self.جوڑو_language (macrolanguage, ot_macrolanguage)

	تعریف ترتیب_دیو_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		لكل language, ٹیگ_سوچی في self.from_bcp_47.اشیاء ():
			self.from_bcp_47[language] = sorted (ٹیگ_سوچی,
					کلید_2=مجهول t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

صنف BCP47Parser (object):
	"""A parser for the BCP 47 subtag registry.

	Attributes:
		header (str): The "File-Date" line of the registry.
		names (Mapping[str, str]): A map of subtags to the names they
			are given in the registry. Each value is a
			``'\\n'``-separated list of names.
		scopes (Mapping[str, str]): A map of language subtags to strings
			suffixed to language names, including suffixes to explain
			language scopes.
		macrolanguages (DefaultDict[str, AbstractSet[str]]): A map of
			language subtags to the sets of language subtags which
			inherit from them. See
			``OpenTypeRegistryParser.inherit_from_macrolanguages``.
		prefixes (DefaultDict[str, AbstractSet[str]]): A map of variant
			subtags to their prefixes.
		grandfathered (AbstractSet[str]): The set of grandfathered tags,
			normalized to lowercase.

	"""
	تعریف __init__ (self):
		self.سرخی = ''
		self.ناں = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (سیٹ)
		self.prefixes = collections.defaultdict (سیٹ)
		self.grandfathered = سیٹ ()

	تعریف parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		نال open (filename, encoding='utf-8') كـ f:
			subtag_type = کجھ_نئیں
			subtag = کجھ_نئیں
			deprecated = خطأ
			has_preferred_قدر = خطأ
			سطر_بفر = ''
			لكل سطر في itertools.chain (f, ['']):
				سطر = سطر.rstrip ()
				إذا سطر.startswith (' '):
					سطر_بفر += سطر[1:]
					استمر
				سطر, سطر_بفر = سطر_بفر, سطر
				إذا سطر.startswith ('Type: '):
					subtag_type = سطر.ونڈو (' ')[1]
					deprecated = خطأ
					has_preferred_قدر = خطأ
				وإلاإذا سطر.startswith ('Subtag: ') أو سطر.startswith ('Tag: '):
					subtag = سطر.ونڈو (' ')[1]
					إذا subtag_type == 'grandfathered':
						self.grandfathered.جوڑو (subtag.ہیٹھلا ())
				وإلاإذا سطر.startswith ('Description: '):
					description = سطر.ونڈو (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					إذا subtag في self.ناں:
						self.ناں[subtag] += '\n' + description
					وإلا:
						self.ناں[subtag] = description
				وإلاإذا subtag_type == 'language' أو subtag_type == 'grandfathered':
					إذا سطر.startswith ('Scope: '):
						scope = سطر.ونڈو (' ')[1]
						إذا scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						وإلاإذا scope == 'collection':
							scope = ' [collection]'
						وإلا:
							استمر
						self.scopes[subtag] = scope
					وإلاإذا سطر.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = سچ
					وإلاإذا deprecated و سطر.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						لكل language في سطر.replace (',', '').ونڈو (' ')[2:]:
							self.جوڑو_macrolanguage (subtag, language)
					وإلاإذا سطر.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_قدر = سچ
						macrolanguage = سطر.ونڈو (' ')[1]
						self.جوڑو_macrolanguage (macrolanguage, subtag)
					وإلاإذا ليس has_preferred_قدر و سطر.startswith ('Macrolanguage: '):
						self.جوڑو_macrolanguage (سطر.ونڈو (' ')[1], subtag)
				وإلاإذا subtag_type == 'variant':
					إذا سطر.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					وإلاإذا سطر.startswith ('Prefix: '):
						self.prefixes[subtag].جوڑو (سطر.ونڈو (' ')[1])
				وإلاإذا سطر.startswith ('File-Date: '):
					self.سرخی = سطر
		expect (self.سرخی)

	تعریف جوڑو_macrolanguage (self, macrolanguage, language):
		عام ot
		إذا language ليس في ot.from_bcp_47:
			لكل l في self.macrolanguages.get (language, سیٹ ()):
				self.جوڑو_macrolanguage (macrolanguage, l)
		إذا macrolanguage ليس في ot.from_bcp_47:
			لكل ls في list (self.macrolanguages.اقدار ()):
				إذا macrolanguage في ls:
					ls.جوڑو (language)
					أرجع
		self.macrolanguages[macrolanguage].جوڑو (language)

	تعریف remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		لكل macrolanguage, languages في self.macrolanguages.اشیاء ():
			لكل language في languages:
				inverted[language].append (macrolanguage)
		لكل language, macrolanguages في inverted.اشیاء ():
			إذا لمبائی (macrolanguages) > 1:
				macrolanguages.ترتیب_دیو (کلید_2=مجهول ml: لمبائی (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.نکالو ()
				لكل macrolanguage في macrolanguages:
					self.جوڑو_macrolanguage (biggest_macrolanguage, macrolanguage)

	تعریف _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		أرجع self.ناں[subtag].ونڈو ('\n')[0] + self.scopes.get (subtag, '')

	تعریف get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		إذا lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		إذا lt.region:
			name += '; ' + self._get_name_piece (lt.region.اتلا ())
		إذا lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		أرجع name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.جوڑو_language ('ary', 'MOR')

ot.جوڑو_language ('ath', 'ATH')

ot.جوڑو_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.جوڑو_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.جوڑو_language ('el-polyton', 'PGR')

bcp_47.ناں['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.جوڑو_language ('und-fonipa', 'IPPH')

ot.جوڑو_language ('und-fonnapa', 'APPH')

ot.جوڑو_language ('und-fonupa', 'UPPH')

ot.جوڑو_language ('ga-Latg', 'IRT')

ot.جوڑو_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.جوڑو_language ('und-Geok', 'KGE')

ot.جوڑو_language ('kht', 'KHN')
ot.ناں['KHN'] = ot.ناں['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.ناں['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.ناں['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.جوڑو_language ('mnw-TH', 'MONT')

ot.جوڑو_language ('no', 'NOR')

ot.جوڑو_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.جوڑو_language ('qu', 'QUZ')
ot.جوڑو_language ('qub', 'QWH')
ot.جوڑو_language ('qud', 'QVI')
ot.جوڑو_language ('qug', 'QVI')
ot.جوڑو_language ('qul', 'QUH')
ot.جوڑو_language ('qup', 'QVI')
ot.جوڑو_language ('qur', 'QWH')
ot.جوڑو_language ('qus', 'QUH')
ot.جوڑو_language ('quw', 'QVI')
ot.جوڑو_language ('qux', 'QWH')
ot.جوڑو_language ('qva', 'QWH')
ot.جوڑو_language ('qvh', 'QWH')
ot.جوڑو_language ('qvj', 'QVI')
ot.جوڑو_language ('qvl', 'QWH')
ot.جوڑو_language ('qvm', 'QWH')
ot.جوڑو_language ('qvn', 'QWH')
ot.جوڑو_language ('qvo', 'QVI')
ot.جوڑو_language ('qvp', 'QWH')
ot.جوڑو_language ('qvw', 'QWH')
ot.جوڑو_language ('qvz', 'QVI')
ot.جوڑو_language ('qwa', 'QWH')
ot.جوڑو_language ('qws', 'QWH')
ot.جوڑو_language ('qxa', 'QWH')
ot.جوڑو_language ('qxc', 'QWH')
ot.جوڑو_language ('qxh', 'QWH')
ot.جوڑو_language ('qxl', 'QVI')
ot.جوڑو_language ('qxn', 'QWH')
ot.جوڑو_language ('qxo', 'QWH')
ot.جوڑو_language ('qxr', 'QVI')
ot.جوڑو_language ('qxt', 'QWH')
ot.جوڑو_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].جوڑو ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.جوڑو_language ('und-Syre', 'SYRE')
ot.جوڑو_language ('und-Syrj', 'SYRJ')
ot.جوڑو_language ('und-Syrn', 'SYRN')

bcp_47.ناں['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.جوڑو_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.جوڑو_language ('zh-Hant-MO', 'ZHH')
ot.جوڑو_language ('zh-Hant-MO', 'ZHTM')
ot.جوڑو_language ('zh-Hant-HK', 'ZHH')
ot.جوڑو_language ('zh-Hans', 'ZHS')
ot.جوڑو_language ('zh-Hant', 'ZHT')
ot.جوڑو_language ('zh-HK', 'ZHH')
ot.جوڑو_language ('zh-MO', 'ZHH')
ot.جوڑو_language ('zh-MO', 'ZHTM')
ot.جوڑو_language ('zh-TW', 'ZHT')
ot.جوڑو_language ('lzh', 'ZHT')
ot.جوڑو_language ('lzh-Hans', 'ZHS')
ot.جوڑو_language ('yue', 'ZHH')
ot.جوڑو_language ('yue-Hans', 'ZHS')

تعریف rank_delta (bcp_47, ot):
	"""Return a delta to apply to a BCP 47 tag's rank.

	Most OpenType tags have a constant rank, but a few have ranks that
	depend on the BCP 47 tag.

	Args:
		bcp_47 (str): A BCP 47 tag.
		ot (str): An OpenType tag to.

	Returns:
		A number to add to ``ot``'s rank when sorting ``bcp_47``'s
		OpenType equivalents.
	"""
	إذا bcp_47 == 'ak' و ot == 'AKA':
		أرجع -1
	إذا bcp_47 == 'tw' و ot == 'TWI':
		أرجع -1
	أرجع 0

disambiguation = {
	'ALT': 'alt',
	'ARK': 'rki',
	'ATH': 'ath',
	'BHI': 'bhb',
	'BLN': 'bjt',
	'BTI': 'beb',
	'CCHN': 'cco',
	'CMR': 'swb',
	'CPP': 'crp',
	'CRR': 'crx',
	'DUJ': 'dwu',
	'ECR': 'crj',
	'HAL': 'cfm',
	'HND': 'hnd',
	'HYE': 'hyw',
	'KIS': 'kqs',
	'KUI': 'uki',
	'LRC': 'bqi',
	'NDB': 'nd',
	'NIS': 'njz',
	'PLG': 'pce',
	'PRO': 'pro',
	'QIN': 'bgr',
	'QUH': 'quh',
	'QVI': 'qvi',
	'QWH': 'qwh',
	'SIG': 'stv',
	'SRB': 'sr',
	'SXT': 'xnj',
	'ZHH': 'zh-HK',
	'ZHS': 'zh-Hans',
	'ZHT': 'zh-Hant',
	'ZHTM': 'zh-MO',
}

ot.inherit_from_macrolanguages ()
bcp_47.remove_extra_macrolanguages ()
ot.inherit_from_macrolanguages ()
ot.ناں[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.اقدار ()) + 1
لكل tricky_ot_ٹیگ في filter (مجهول ٹیگ_2: re.میل ('[A-Z]{3}$', ٹیگ_2), ot.ناں):
	possible_bcp_47_ٹیگ = tricky_ot_ٹیگ.ہیٹھلا ()
	إذا possible_bcp_47_ٹیگ في bcp_47.ناں و ليس ot.from_bcp_47[possible_bcp_47_ٹیگ]:
		ot.جوڑو_language (possible_bcp_47_ٹیگ, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_ٹیگ] = سیٹ ()
ot.ترتیب_دیو_languages ()

إذا output_rust:
	تعریف hb_ٹیگ_rust (ٹیگ_2):
		إذا ٹیگ_2 == DEFAULT_LANGUAGE_SYSTEM:
			أرجع 'Tag::new(&[0; 4])'
		sanitize_ٹیگ (ٹیگ_2)
		أرجع 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % ٹیگ_2)[:4])

	تعریف get_variant_سیٹ_rust (name):
		أرجع سیٹ (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				لكل n في re.ونڈو ('[\n(),]', name) إذا n)

	تعریف language_name_intersection_rust (a, b):
		أرجع get_variant_سیٹ_rust (a).intersection (get_variant_سیٹ_rust (b))

	تعریف get_matching_language_name_rust (intersection, candidates):
		أرجع اگلا (iter (c لكل c في candidates إذا ليس intersection.isdisjoint (get_variant_سیٹ_rust (c))))

	چھاپو ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	چھاپو ()
	چھاپو ('use read_fonts::types::Tag;')
	چھاپو ()
	چھاپو ('pub(crate) struct LangTag {')
	چھاپو ('    pub language: [u8; 4],')
	چھاپو ('    pub tag: Tag,')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('const fn lang(language: &[u8]) -> [u8; 4] {')
	چھاپو ('    let mut bytes = [0; 4];')
	چھاپو ('    let mut i = 0;')
	چھاپو ()
	چھاپو ('    while i < language.len() {')
	چھاپو ('        bytes[i] = language[i];')
	چھاپو ('        i += 1;')
	چھاپو ('    }')
	چھاپو ()
	چھاپو ('    bytes')
	چھاپو ('}')
	چھاپو ()
	چھاپو ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	چھاپو ()
	چھاپو ('#[rustfmt::skip]')
	چھاپو ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	لكل language, ٹیگ_سوچی في sorted (ot.from_bcp_47.اشیاء ()):
		إذا language == '' أو '-' في language:
			استمر
		commented_باہرلا = لمبائی (language) == 3 و لمبائی (ٹیگ_سوچی) == 1 و language == ٹیگ_سوچی[0].ہیٹھلا ()
		لكل ٹیگ_2 في ٹیگ_سوچی:
			چھاپو ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' إذا commented_باہرلا وإلا '    ', sanitize_bcp47_ٹیگ (language), hb_ٹیگ_rust (ٹیگ_2)), سرا='')
			چھاپو (' // ', سرا='')
			bcp_47_name = bcp_47.ناں.get (language, '')
			bcp_47_name_candidates = bcp_47_name.ونڈو ('\n')
			ot_name = ot.ناں[ٹیگ_2]
			scope = bcp_47.scopes.get (language, '')
			إذا ٹیگ_2 == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			وإلا:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				إذا ليس intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				وإلا:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.ناں[language] = name
					comment = '%s%s' % (name إذا لمبائی (name) > لمبائی (ot_name) وإلا ot_name, scope)
			چھاپو (sanitize_comment (comment))

	چھاپو ('];')
	چھاپو ()
	چھاپو ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	چھاپو ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	تعریف print_subtag_matches_rust (subtag, new_سطر):
		إذا subtag:
			إذا new_سطر:
				چھاپو (' && ', سرا='')
			چھاپو ('subtag_matches(language, "-%s")' % sanitize_bcp47_ٹیگ (subtag), سرا='')

	complex_ٹیگ_سوچی = collections.defaultdict (list)
	لكل initial, گروہ في itertools.groupby ((lt_ٹیگ_سوچی لكل lt_ٹیگ_سوچی في [
				(Language_ٹیگ (language), ٹیگ_سوچی)
				لكل language, ٹیگ_سوچی في sorted (ot.from_bcp_47.اشیاء (),
					کلید_2=مجهول i: (-لمبائی (i[0]), i[0]))
			] إذا lt_ٹیگ_سوچی[0].is_complex ()),
			کلید_2=مجهول lt_ٹیگ_سوچی: lt_ٹیگ_سوچی[0].get_گروہ ()):
		complex_ٹیگ_سوچی[initial] += گروہ

	لكل initial, اشیاء في sorted (complex_ٹیگ_سوچی.اشیاء ()):
		إذا initial != 'und':
			استمر
		لكل lt, ٹیگ_سوچی في اشیاء:
			إذا ليس ٹیگ_سوچی:
				استمر
			إذا lt.variant في bcp_47.prefixes:
				expect (اگلا (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			چھاپو ('    if ', سرا='')
			print_subtag_matches_rust (lt.script, خطأ)
			print_subtag_matches_rust (lt.region, خطأ)
			print_subtag_matches_rust (lt.variant, خطأ)
			چھاپو (' {')
			چھاپو ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			إذا لمبائی (ٹیگ_سوچی) == 1:
				چھاپو ('        tags.push(%s); // %s' % (hb_ٹیگ_rust (ٹیگ_سوچی[0]), sanitize_comment (ot.ناں[ٹیگ_سوچی[0]])))
			وإلا:
				چھاپو ('        let possible_tags = &[')
				لكل ٹیگ_2 في ٹیگ_سوچی:
					چھاپو ('            %s, // %s' % (hb_ٹیگ_rust (ٹیگ_2), sanitize_comment (ot.ناں[ٹیگ_2])))
				چھاپو ('        ];')
				چھاپو ('        tags.extend_from_slice(possible_tags);')
			چھاپو ('        return true;')
			چھاپو ('    }')

	چھاپو ('    match language.as_bytes()[0] {')
	لكل initial, اشیاء في sorted (complex_ٹیگ_سوچی.اشیاء ()):
		إذا initial == 'und':
			استمر
		چھاپو ("        b'%s' => {" % sanitize_bcp47_ٹیگ (initial))
		لكل lt, ٹیگ_سوچی في اشیاء:
			إذا ليس ٹیگ_سوچی:
				استمر
			چھاپو ('            if ', سرا='')
			script = lt.script
			region = lt.region
			إذا lt.grandfathered:
				چھاپو ('&language[1..] == "%s"' % sanitize_bcp47_ٹیگ (lt.language[1:]), سرا='')
			وإلا:
				string_literal = lt.language[1:] + '-'
				إذا script:
					string_literal += script
					script = کجھ_نئیں
					إذا region:
						string_literal += '-' + region
						region = کجھ_نئیں
				إذا string_literal[-1] == '-':
					چھاپو ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_ٹیگ (string_literal), لمبائی (string_literal)), سرا='')
				وإلا:
					چھاپو ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_ٹیگ (string_literal), سرا='')
			print_subtag_matches_rust (script, سچ)
			print_subtag_matches_rust (region, سچ)
			print_subtag_matches_rust (lt.variant, سچ)
			چھاپو (' {')
			چھاپو ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			إذا لمبائی (ٹیگ_سوچی) == 1:
				چھاپو ('                tags.push(%s); // %s' % (hb_ٹیگ_rust (ٹیگ_سوچی[0]), sanitize_comment (ot.ناں[ٹیگ_سوچی[0]])))
			وإلا:
				چھاپو ('                let possible_tags = &[')
				لكل ٹیگ_2 في ٹیگ_سوچی:
					چھاپو ('                    %s, // %s' % (hb_ٹیگ_rust (ٹیگ_2), sanitize_comment (ot.ناں[ٹیگ_2])))
				چھاپو ('                ];')
				چھاپو ('                tags.extend_from_slice(possible_tags);')
			چھاپو ('                return true;')
			چھاپو ('            }')
		چھاپو ('        }')
	چھاپو ('        _ => {}')
	چھاپو ('    }')
	چھاپو ('    false')
	چھاپو ('}')
	sys.exit (0)

چھاپو ('/* == Start of generated table == */')
چھاپو ('/*')
چھاپو (' * The following table is generated by running:')
چھاپو (' *')
چھاپو (' *   %s languagetags language-subtag-registry' % os.رستہ.basename (sys.argv[0]))
چھاپو (' *')
چھاپو (' * on files with these headers:')
چھاپو (' *')
چھاپو (' * %s' % sanitize_سرخی (ot.سرخی))
چھاپو (' * %s' % sanitize_سرخی (bcp_47.سرخی))
چھاپو (' */')
چھاپو ()
چھاپو ('#ifndef HB_OT_TAG_TABLE_HH')
چھاپو ('#define HB_OT_TAG_TABLE_HH')
چھاپو ()

تعریف hb_ٹیگ (ٹیگ_2):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	إذا ٹیگ_2 == DEFAULT_LANGUAGE_SYSTEM:
		أرجع 'HB_TAG_NONE\t       '
	sanitize_ٹیگ (ٹیگ_2)
	أرجع "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % ٹیگ_2)[:4])

تعریف get_variant_سیٹ (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	أرجع سیٹ (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			لكل n في re.ونڈو ('[\n(),]', name) إذا n)

تعریف language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	أرجع get_variant_سیٹ (a).intersection (get_variant_سیٹ (b))

تعریف get_matching_language_name (intersection, candidates):
	أرجع اگلا (iter (c لكل c في candidates إذا ليس intersection.isdisjoint (get_variant_سیٹ (c))))

تعریف same_ٹیگ (bcp_47_ٹیگ, ot_ٹیگ_سوچی):
	أرجع لمبائی (bcp_47_ٹیگ) == 3 و لمبائی (ot_ٹیگ_سوچی) == 1 و bcp_47_ٹیگ == ot_ٹیگ_سوچی[0].ہیٹھلا ()

تعریف format_language_comment (language, ٹیگ_2):
	bcp_47_name = bcp_47.ناں.get (language, '')
	bcp_47_name_candidates = bcp_47_name.ونڈو ('\n')
	ot_name = ot.ناں[ٹیگ_2]
	scope = bcp_47.scopes.get (language, '')
	إذا ٹیگ_2 == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	وإلا:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		إذا ليس intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		وإلا:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.ناں[language] = name
			comment = '%s%s' % (name إذا لمبائی (name) > لمبائی (ot_name) وإلا ot_name, scope)
	أرجع sanitize_comment (comment)

چھاپو ('static const LangTag ot_languages2[] = {')
لكل language, ٹیگ_سوچی في sorted (ot.from_bcp_47.اشیاء ()):
	إذا language == '' أو '-' في language أو لمبائی (language) != 2:
		استمر
	لكل ٹیگ_2 في ٹیگ_سوچی:
		چھاپو ('  {%s,\t%s},\t/* %s */' % (
			hb_ٹیگ (language),
			hb_ٹیگ (ٹیگ_2),
			format_language_comment (language, ٹیگ_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_اقدار = []
لكل language, ٹیگ_سوچی في sorted (ot.from_bcp_47.اشیاء ()):
	إذا language == '' أو '-' في language أو لمبائی (language) != 3:
		استمر
	إذا same_ٹیگ (language, ٹیگ_سوچی):
		استمر
	إذا ٹیگ_سوچی == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, ٹیگ_سوچی[0]))
		استمر
	إذا لمبائی (ٹیگ_سوچی) == 1:
		languages3_singles.append ((language, ٹیگ_سوچی[0]))
		استمر
	وتھ = لمبائی (languages3_multi_اقدار)
	languages3_multi_اقدار.extend ((language, ٹیگ_2) لكل ٹیگ_2 في ٹیگ_سوچی)
	languages3_multi_ranges.append ((language, وتھ, لمبائی (ٹیگ_سوچی), ٹیگ_سوچی[0]))

چھاپو ('static const hb_tag_t ot_languages3_blocked[] = {')
لكل language, ٹیگ_2 في languages3_blocked:
	چھاپو ('  %s,\t/* %s */' % (hb_ٹیگ (language), format_language_comment (language, ٹیگ_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('static const LangTag ot_languages3[] = {')
لكل language, ٹیگ_2 في languages3_singles:
	چھاپو ('  {%s,\t%s},\t/* %s */' % (
		hb_ٹیگ (language),
		hb_ٹیگ (ٹیگ_2),
		format_language_comment (language, ٹیگ_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('static const hb_tag_t ot_languages3_multi_values[] = {')
لكل language, ٹیگ_2 في languages3_multi_اقدار:
	چھاپو ('  %s,\t/* %s */' % (hb_ٹیگ (ٹیگ_2), format_language_comment (language, ٹیگ_2)))
چھاپو ('};')
چھاپو ()

چھاپو ('static const LangTagRange ot_languages3_multi[] = {')
لكل language, وتھ, گنتی, پہلا_ٹیگ في languages3_multi_ranges:
	چھاپو ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_ٹیگ (language),
		صحيح(وتھ),
		صحيح(گنتی),
		format_language_comment (language, پہلا_ٹیگ)))
چھاپو ('};')
چھاپو ('#endif')
چھاپو ()

چھاپو ('/**')
چھاپو (' * hb_ot_tags_from_complex_language:')
چھاپو (' * @lang_str: a BCP 47 language tag to convert.')
چھاپو (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
چھاپو (' * conversion.')
چھاپو (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
چھاپو (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
چھاپو (' * @tags: array of size at least @language_count to store the language tag')
چھاپو (' * results')
چھاپو (' *')
چھاپو (' * Converts a multi-subtag BCP 47 language tag to language tags.')
چھاپو (' *')
چھاپو (' * Return value: Whether any language systems were retrieved.')
چھاپو (' **/')
چھاپو ('static inline bool')
چھاپو ('hb_ot_tags_from_complex_language (const char   *lang_str,')
چھاپو ('\t\t\t\t  const char   *limit,')
چھاپو ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
چھاپو ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
چھاپو ('{')

تعریف print_subtag_matches (subtag, string, new_سطر):
	إذا subtag:
		إذا new_سطر:
			چھاپو ()
			چھاپو ('\t&& ', سرا='')
		چھاپو ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_ٹیگ (subtag), 1 + لمبائی (subtag)), سرا='')

complex_ٹیگ_سوچی = collections.defaultdict (list)
لكل initial, گروہ في itertools.groupby ((lt_ٹیگ_سوچی لكل lt_ٹیگ_سوچی في [
			(Language_ٹیگ (language), ٹیگ_سوچی)
			لكل language, ٹیگ_سوچی في sorted (ot.from_bcp_47.اشیاء (),
				کلید_2=مجهول i: (-لمبائی (i[0]), i[0]))
		] إذا lt_ٹیگ_سوچی[0].is_complex ()),
		کلید_2=مجهول lt_ٹیگ_سوچی: lt_ٹیگ_سوچی[0].get_گروہ ()):
	complex_ٹیگ_سوچی[initial] += گروہ

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
لكل initial, اشیاء في sorted (complex_ٹیگ_سوچی.اشیاء ()):
	إذا initial != 'und':
		استمر
	لكل lt, ٹیگ_سوچی في اشیاء:
		إذا ليس ٹیگ_سوچی:
			استمر
		subtag_len = 0
		subtag_len += 1 + لمبائی (lt.script) إذا lt.script اے ليس کجھ_نئیں وإلا 0
		subtag_len += 1 + لمبائی (lt.region) إذا lt.region اے ليس کجھ_نئیں وإلا 0
		subtag_len += 1 + لمبائی (lt.variant) إذا lt.variant اے ليس کجھ_نئیں وإلا 0
		min_subtag_len = min(subtag_len, min_subtag_len)

چھاپو ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
چھاپو ('  {')
چھاپو ("    const char *p = strchr (lang_str, '-');")
چھاپو ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
لكل initial, اشیاء في sorted (complex_ٹیگ_سوچی.اشیاء ()):
	إذا initial != 'und':
		استمر
	لكل lt, ٹیگ_سوچی في اشیاء:
		إذا ليس ٹیگ_سوچی:
			استمر
		إذا lt.variant في bcp_47.prefixes:
			expect (اگلا (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		چھاپو ('    if (', سرا='')
		print_subtag_matches (lt.script, 'p', خطأ)
		print_subtag_matches (lt.region, 'p', خطأ)
		print_subtag_matches (lt.variant, 'p', خطأ)
		چھاپو (')')
		چھاپو ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		چھاپو ()
		إذا لمبائی (ٹیگ_سوچی) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_ٹیگ (ٹیگ_سوچی[0]), sanitize_comment (ot.ناں[ٹیگ_سوچی[0]])))
			چھاپو ()
			چھاپو ('      *count = 1;')
		وإلا:
			چھاپو ('    hb_tag_t possible_tags[] = {')
			لكل ٹیگ_2 في ٹیگ_سوچی:
				اكتب ('      %s,  /* %s */' % (hb_ٹیگ (ٹیگ_2), sanitize_comment (ot.ناں[ٹیگ_2])))
				چھاپو ()
			چھاپو ('      };')
			چھاپو ('      for (i = 0; i < %s && i < *count; i++)' % لمبائی (ٹیگ_سوچی))
			چھاپو ('\ttags[i] = possible_tags[i];')
			چھاپو ('      *count = i;')
		چھاپو ('      return true;')
		چھاپو ('    }')
چھاپو ('  }')
چھاپو ('out:')

چھاپو ('  switch (lang_str[0])')
چھاپو ('  {')
لكل initial, اشیاء في sorted (complex_ٹیگ_سوچی.اشیاء ()):
	إذا initial == 'und':
		استمر
	چھاپو ("  case '%s':" % sanitize_bcp47_ٹیگ (initial))
	لكل lt, ٹیگ_سوچی في اشیاء:
		إذا ليس ٹیگ_سوچی:
			استمر
		چھاپو ('    if (', سرا='')
		script = lt.script
		region = lt.region
		إذا lt.grandfathered:
			چھاپو ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_ٹیگ (lt.language[1:]), سرا='')
		وإلا:
			string_literal = lt.language[1:] + '-'
			إذا script:
				string_literal += script
				script = کجھ_نئیں
				إذا region:
					string_literal += '-' + region
					region = کجھ_نئیں
			إذا string_literal[-1] == '-':
				چھاپو ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_ٹیگ (string_literal), لمبائی (string_literal)), سرا='')
			وإلا:
				چھاپو ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_ٹیگ (string_literal), لمبائی (string_literal)), سرا='')
		print_subtag_matches (script, 'lang_str', سچ)
		print_subtag_matches (region, 'lang_str', سچ)
		print_subtag_matches (lt.variant, 'lang_str', سچ)
		چھاپو (')')
		چھاپو ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		چھاپو ()
		إذا لمبائی (ٹیگ_سوچی) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_ٹیگ (ٹیگ_سوچی[0]), sanitize_comment (ot.ناں[ٹیگ_سوچی[0]])))
			چھاپو ()
			چھاپو ('      *count = 1;')
		وإلا:
			چھاپو ('      unsigned int i;')
			چھاپو ('      hb_tag_t possible_tags[] = {')
			لكل ٹیگ_2 في ٹیگ_سوچی:
				اكتب ('\t%s,  /* %s */' % (hb_ٹیگ (ٹیگ_2), sanitize_comment (ot.ناں[ٹیگ_2])))
				چھاپو ()
			چھاپو ('      };')
			چھاپو ('      for (i = 0; i < %s && i < *count; i++)' % لمبائی (ٹیگ_سوچی))
			چھاپو ('\ttags[i] = possible_tags[i];')
			چھاپو ('      *count = i;')
		چھاپو ('      return true;')
		چھاپو ('    }')
	چھاپو ('    break;')

چھاپو ('  }')
چھاپو ('  return false;')
چھاپو ('}')
چھاپو ()
چھاپو ('/**')
چھاپو (' * hb_ot_ambiguous_tag_to_language')
چھاپو (' * @tag: A language tag.')
چھاپو (' *')
چھاپو (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
چھاپو (' * many language tags) and the best tag is not the first (sorted alphabetically,')
چھاپو (' * with two-letter tags having priority over all three-letter tags), or if the')
چھاپو (' * best tag consists of multiple subtags, or if the best tag does not appear in')
چھاپو (' * #ot_languages2 or #ot_languages3.')
چھاپو (' *')
چھاپو (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
چھاپو (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
چھاپو (' **/')
چھاپو ('static inline hb_language_t')
چھاپو ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
چھاپو ('{')
چھاپو ('  switch (tag)')
چھاپو ('  {')

تعریف verify_disambiguation_dict ():
	"""Verify and normalize ``disambiguation``.

	``disambiguation`` is a map of ambiguous OpenType language system
	tags to the particular BCP 47 tags they correspond to. This function
	checks that all its keys really are ambiguous and that each key's
	value is valid for that key. It checks that no ambiguous tag is
	missing, except when it can figure out which BCP 47 tag is the best
	by itself.

	It modifies ``disambiguation`` to remove keys whose values are the
	same as those that the fallback would return anyway, and to add
	ambiguous keys whose disambiguations it determined automatically.

	Raises:
		AssertionError: Verification failed.
	"""
	عام bcp_47
	عام disambiguation
	عام ot
	لكل ot_ٹیگ, bcp_47_ٹیگ_سوچی في ot.منزل_bcp_47.اشیاء ():
		إذا ot_ٹیگ == DEFAULT_LANGUAGE_SYSTEM:
			primary_ٹیگ_سوچی = []
		وإلا:
			primary_ٹیگ_سوچی = list (t لكل t في bcp_47_ٹیگ_سوچی إذا t ليس في bcp_47.grandfathered و ot.from_bcp_47.get (t)[0] == ot_ٹیگ)
		إذا لمبائی (primary_ٹیگ_سوچی) == 1:
			expect (ot_ٹیگ ليس في disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_ٹیگ)
			إذا '-' في primary_ٹیگ_سوچی[0]:
				disambiguation[ot_ٹیگ] = primary_ٹیگ_سوچی[0]
			وإلا:
				پہلا_ٹیگ = sorted ((t لكل t في bcp_47_ٹیگ_سوچی إذا t ليس في bcp_47.grandfathered و ot_ٹیگ في ot.from_bcp_47.get (t)),
						کلید_2=مجهول t: (لمبائی (t), t))[0]
				إذا primary_ٹیگ_سوچی[0] != پہلا_ٹیگ:
					disambiguation[ot_ٹیگ] = primary_ٹیگ_سوچی[0]
		وإلاإذا لمبائی (primary_ٹیگ_سوچی) == 0:
			expect (ot_ٹیگ ليس في disambiguation, 'There is no possible valid disambiguation for %s' % ot_ٹیگ)
		وإلا:
			original_languages = [t لكل t في primary_ٹیگ_سوچی إذا t في ot.from_bcp_47_uninherited و 'retired code' ليس في bcp_47.scopes.get (t, '')]
			إذا لمبائی (original_languages) == 1:
				macrolanguages = original_languages
			وإلا:
				macrolanguages = [t لكل t في primary_ٹیگ_سوچی إذا bcp_47.scopes.get (t) == ' [macrolanguage]']
			إذا لمبائی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٹیگ_سوچی إذا bcp_47.scopes.get (t) == ' [collection]')
			إذا لمبائی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٹیگ_سوچی إذا 'retired code' ليس في bcp_47.scopes.get (t, ''))
			إذا لمبائی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٹیگ_سوچی إذا t.ہیٹھلا () == ISO_639_3_منزل_1.get (ot_ٹیگ.ہیٹھلا (), ot_ٹیگ.ہیٹھلا ()))
			إذا لمبائی (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_ٹیگ_سوچی إذا '-' ليس في t)
			إذا لمبائی (macrolanguages) != 1:
				expect (ot_ٹیگ في disambiguation, 'ambiguous OT tag: %s %s' % (ot_ٹیگ, sorted (primary_ٹیگ_سوچی)))
				expect (disambiguation[ot_ٹیگ] في bcp_47_ٹیگ_سوچی,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_ٹیگ], ot_ٹیگ))
			وإلاإذا ot_ٹیگ ليس في disambiguation:
				disambiguation[ot_ٹیگ] = macrolanguages[0]
			إذا '-' ليس في disambiguation[ot_ٹیگ]:
				different_bcp_47_ٹیگ_سوچی = sorted ((t لكل t في bcp_47_ٹیگ_سوچی إذا ليس same_ٹیگ (t, ot.from_bcp_47.get (t))),
						کلید_2=مجهول t: (لمبائی (t), t))
				إذا different_bcp_47_ٹیگ_سوچی و disambiguation[ot_ٹیگ] == different_bcp_47_ٹیگ_سوچی[0]:
					ہٹاؤ disambiguation[ot_ٹیگ]
	لكل ot_ٹیگ في disambiguation.کلیدیں ():
		expect (ot_ٹیگ في ot.منزل_bcp_47, 'unknown OT tag: %s' % ot_ٹیگ)

verify_disambiguation_dict ()
لكل ot_ٹیگ, bcp_47_ٹیگ في sorted (disambiguation.اشیاء ()):
	اكتب ('  case %s:  /* %s */' % (hb_ٹیگ (ot_ٹیگ), sanitize_comment (ot.ناں[ot_ٹیگ])))
	چھاپو ()
	اكتب ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_ٹیگ (bcp_47_ٹیگ), sanitize_comment (bcp_47.get_name (Language_ٹیگ (bcp_47_ٹیگ)))))
	چھاپو ()

چھاپو ('  default:')
چھاپو ('    return HB_LANGUAGE_INVALID;')
چھاپو ('  }')
چھاپو ('}')

چھاپو ()
چھاپو ('#endif /* HB_OT_TAG_TABLE_HH */')
چھاپو ()
چھاپو ('/* == End of generated table == */')

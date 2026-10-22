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

আনযন collections
আনযন html
তঃ html.parser আনযন HTMLParser
আনযন itertools
আনযন os
আনযন re
আনযন sys
আনযন unicodedata

যদি len (sys.argv) > 1 চ sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = সত্য
অন্যথা:
	output_rust = অসত্য

যদি len (sys.argv) != 3:
	sys.exit (__doc__)

def expect (condition, message=None):
	যদি ন condition:
		যদি message is None:
			উত্থাপয AssertionError
		উত্থাপয AssertionError (message)

def লিখ (s):
	sys.stdout.flush ()
	sys.stdout.buffer.লিখ (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = set (" -—–.,;:’'ʼ()[]/!=>_+")

def sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	যদি ন isinstance (s, str):
		উত্থাপয TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	যদি ন s:
		প্রত্যাগম ''
	প্রতি ch অন্তঃ s:
		cat = unicodedata.category (ch)
		যদি ন (cat.startswith (('L', 'M', 'N', 'Z')) বা ch অন্তঃ _ALLOWED_COMMENT_PUNCT):
			উত্থাপয মান_Error ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	প্রত্যাগম s

def sanitize_header (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	যদি ন isinstance (s, str):
		উত্থাপয TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	যদি ন re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		উত্থাপয মান_Error ("Invalid header line %r: contains disallowed characters" % s)
	প্রত্যাগম s

def sanitize_bcp47_tag (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	যদি ন isinstance (s, str):
		উত্থাপয TypeError ("Expected string, got %s" % type (s).__name__)
	যদি ন re.fullmatch (r'[a-zA-Z0-9-]+', s):
		উত্থাপয মান_Error ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	প্রত্যাগম s

def sanitize_tag (tag):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	যদি ন isinstance (tag, str):
		উত্থাপয TypeError ("Expected string for tag, got %s" % type (tag).__name__)
	যদি tag == DEFAULT_LANGUAGE_SYSTEM:
		প্রত্যাগম ''
	যদি ন re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', tag):
		উত্থাপয মান_Error ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % tag)
	প্রত্যাগম tag

DEFAULT_LANGUAGE_SYSTEM = ''

# from https://www-01.sil.org/iso639-3/iso-639-3.tab
ISO_639_3_TO_1 = {
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

বর্গ LanguageTag (object):
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
	def __init__ (self, tag):
		বৈশ্বিক bcp_47
		self.subtags = tag.lower ().split ('-')
		self.grandfathered = tag.lower () অন্তঃ bcp_47.grandfathered
		যদি self.grandfathered:
			self.language = tag.lower ()
			self.script = ''
			self.region = ''
			self.variant = ''
		অন্যথা:
			self.language = self.subtags[0]
			self.script = self._find_first (অনামন্ s: len (s) == 4 চ s[0] > '9', self.subtags)
			self.region = self._find_first (অনামন্ s: len (s) == 2 চ s[0] > '9' বা len (s) == 3 চ s[0] <= '9', self.subtags[1:])
			self.variant = self._find_first (অনামন্ s: len (s) > 4 বা len (s) == 4 চ s[0] <= '9', self.subtags)

	def __str__(self):
		প্রত্যাগম '-'.join(self.subtags)

	def __repr__ (self):
		প্রত্যাগম 'LanguageTag(%r)' % str(self)

	@staticmethod
	def _find_first (function, sequence):
		যত্ন:
			প্রত্যাগম next (iter (filter (function, sequence)))
		অপবাদ StopIteration:
			প্রত্যাগম None

	def is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		প্রত্যাগম ন (len (self.subtags) == 1
			বা self.grandfathered
			চ len (self.subtags[1]) != 3
			চ ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	def get_group (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		প্রত্যাগম ('und'
			যদি (self.language == 'und'
				বা self.variant অন্তঃ bcp_47.prefixes চ len (bcp_47.prefixes[self.variant]) == 1)
			অন্যথা self.language[0])

বর্গ OpenTypeRegistryParser (HTMLParser):
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
	def __init__ (self):
		HTMLParser.__init__ (self)
		self.header = ''
		self.names = {}
		self.ranks = collections.defaultdict (পূর্ণাঙ্ক)
		self.to_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47_uninherited = None
		# Whether the parser is in a <td> element
		self._td = অসত্য
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = অসত্য
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	def handle_starttag (self, tag, attrs):
		যদি tag == 'a':
			যদি self._current_tr চ ন self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = সত্য
		নোচেদ্যদি tag == 'br':
			self._disengaged = সত্য
		নোচেদ্যদি tag == 'meta':
			প্রতি attr, মান_2 অন্তঃ attrs:
				যদি attr == 'name' চ মান_2 == 'updated_at':
					self.header = self.get_starttag_text ()
					ভঙ্গ
		নোচেদ্যদি tag == 'td':
			self._td = সত্য
			self._current_tr.append ('')
		নোচেদ্যদি tag == 'tr':
			self._disengaged = অসত্য
			self._current_tr = []

	def handle_endtag (self, tag):
		যদি tag == 'td':
			self._td = অসত্য
			self._disengaged = অসত্য
		নোচেদ্যদি tag == 'tr' চ self._current_tr:
			expect (2 <= len (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			tag = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			যদি len (tag) > 4:
				expect (tag.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % tag)
				name += ' (deprecated)'
				tag = tag.split (' ')[0]
				rank = 1
			self.names[tag] = re.sub (' languages$', '', name)
			যদি ন self._current_tr[2]:
				প্রত্যাগম
			iso_codes = self._current_tr[2].strip ()
			self.to_bcp_47[tag].update (ISO_639_3_TO_1.get (code, code) প্রতি code অন্তঃ iso_codes.replace (' ', '').split (','))
			rank += 2 * len (self.to_bcp_47[tag])
			self.ranks[tag] = rank

	def handle_data (self, data):
		যদি self._td চ ন self._disengaged:
			self._current_tr[-1] += data

	def handle_charref (self, name):
		self.handle_data (html.unescape ('&#%s;' % name))

	def handle_entityref (self, name):
		self.handle_data (html.unescape ('&%s;' % name))

	def parse (self, filename):
		"""Parse the OpenType language system tag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		with open (filename, encoding='utf-8') ইব f:
			self.feed (f.পঠ ())
		expect (self.header)
		প্রতি tag, iso_codes অন্তঃ self.to_bcp_47.বস্তুসমূহ ():
			প্রতি iso_code অন্তঃ iso_codes:
				self.from_bcp_47[iso_code].add (tag)

	def add_language (self, bcp_47_tag, ot_tag):
		"""Add a language as if it were in the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag. If the tag is more than just
				a language subtag, and if the language subtag is a
				macrolanguage, then new languages are added corresponding
				to the macrolanguages' individual languages with the
				remainder of the tag appended.
			ot_tag (str): An OpenType language system tag.
		"""
		বৈশ্বিক bcp_47
		self.to_bcp_47[ot_tag].add (bcp_47_tag)
		self.from_bcp_47[bcp_47_tag].add (ot_tag)
		যদি bcp_47_tag.lower () ন অন্তঃ bcp_47.grandfathered:
			যত্ন:
				[macrolanguage, suffix] = bcp_47_tag.split ('-', 1)
				যদি macrolanguage অন্তঃ bcp_47.macrolanguages:
					s = set ()
					প্রতি language অন্তঃ bcp_47.macrolanguages[macrolanguage]:
						যদি language.lower () ন অন্তঃ bcp_47.grandfathered:
							s.add ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			অপবাদ মান_Error:
				pass

	@staticmethod
	def _remove_language (tag_1, dict_1, dict_2):
		প্রতি tag_2 অন্তঃ dict_1.বের_করো (tag_1):
			dict_2[tag_2].remove (tag_1)
			যদি ন dict_2[tag_2]:
				del dict_2[tag_2]

	def remove_language_ot (self, ot_tag):
		"""Remove an OpenType tag from the registry.

		Args:
			ot_tag (str): An OpenType tag.
		"""
		self._remove_language (ot_tag, self.to_bcp_47, self.from_bcp_47)

	def remove_language_bcp_47 (self, bcp_47_tag):
		"""Remove a BCP 47 tag from the registry.

		Args:
			bcp_47_tag (str): A BCP 47 tag.
		"""
		self._remove_language (bcp_47_tag, self.from_bcp_47, self.to_bcp_47)

	def inherit_from_macrolanguages (self):
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
		বৈশ্বিক bcp_47
		first_time = self.from_bcp_47_uninherited is None
		যদি first_time:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		প্রতি macrolanguage, languages অন্তঃ dict (bcp_47.macrolanguages).বস্তুসমূহ ():
			ot_macrolanguages = {
				ot_macrolanguage প্রতি ot_macrolanguage অন্তঃ self.from_bcp_47_uninherited.get (macrolanguage, set ())
			}
			blocked_ot_macrolanguages = set ()
			যদি 'retired code' ন অন্তঃ bcp_47.scopes.get (macrolanguage, ''):
				প্রতি ot_macrolanguage অন্তঃ ot_macrolanguages:
					round_trip_macrolanguages = {
						l প্রতি l অন্তঃ self.to_bcp_47[ot_macrolanguage]
						যদি 'retired code' ন অন্তঃ bcp_47.scopes.get (l, '')
					}
					round_trip_languages = {
						l প্রতি l অন্তঃ languages
						যদি 'retired code' ন অন্তঃ bcp_47.scopes.get (l, '')
					}
					intersection = round_trip_macrolanguages & round_trip_languages
					যদি intersection চ intersection != round_trip_languages:
						blocked_ot_macrolanguages.add (ot_macrolanguage)
			যদি ot_macrolanguages:
				প্রতি ot_macrolanguage অন্তঃ ot_macrolanguages:
					যদি ot_macrolanguage ন অন্তঃ blocked_ot_macrolanguages:
						প্রতি language অন্তঃ languages:
							self.add_language (language, ot_macrolanguage)
							যদি ন blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			নোচেদ্যদি first_time:
				প্রতি language অন্তঃ languages:
					যদি language অন্তঃ self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					অন্যথা:
						ot_macrolanguages.clear ()
					যদি ন ot_macrolanguages:
						ভঙ্গ
				প্রতি ot_macrolanguage অন্তঃ ot_macrolanguages:
					self.add_language (macrolanguage, ot_macrolanguage)

	def sort_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		প্রতি language, tags অন্তঃ self.from_bcp_47.বস্তুসমূহ ():
			self.from_bcp_47[language] = sorted (tags,
					চাবি_2=অনামন্ t: (self.ranks[t] + rank_delta (language, t), t))

ot = OpenTypeRegistryParser ()

বর্গ BCP47Parser (object):
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
	def __init__ (self):
		self.header = ''
		self.names = {}
		self.scopes = {}
		self.macrolanguages = collections.defaultdict (set)
		self.prefixes = collections.defaultdict (set)
		self.grandfathered = set ()

	def parse (self, filename):
		"""Parse the BCP 47 subtag registry.

		Args:
			filename (str): The file name of the registry.
		"""
		with open (filename, encoding='utf-8') ইব f:
			subtag_type = None
			subtag = None
			deprecated = অসত্য
			has_preferred_মান = অসত্য
			সারি_buffer = ''
			প্রতি সারি অন্তঃ itertools.chain (f, ['']):
				সারি = সারি.rstrip ()
				যদি সারি.startswith (' '):
					সারি_buffer += সারি[1:]
					অনুবর্ত
				সারি, সারি_buffer = সারি_buffer, সারি
				যদি সারি.startswith ('Type: '):
					subtag_type = সারি.split (' ')[1]
					deprecated = অসত্য
					has_preferred_মান = অসত্য
				নোচেদ্যদি সারি.startswith ('Subtag: ') বা সারি.startswith ('Tag: '):
					subtag = সারি.split (' ')[1]
					যদি subtag_type == 'grandfathered':
						self.grandfathered.add (subtag.lower ())
				নোচেদ্যদি সারি.startswith ('Description: '):
					description = সারি.split (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					যদি subtag অন্তঃ self.names:
						self.names[subtag] += '\n' + description
					অন্যথা:
						self.names[subtag] = description
				নোচেদ্যদি subtag_type == 'language' বা subtag_type == 'grandfathered':
					যদি সারি.startswith ('Scope: '):
						scope = সারি.split (' ')[1]
						যদি scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						নোচেদ্যদি scope == 'collection':
							scope = ' [collection]'
						অন্যথা:
							অনুবর্ত
						self.scopes[subtag] = scope
					নোচেদ্যদি সারি.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = সত্য
					নোচেদ্যদি deprecated চ সারি.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						প্রতি language অন্তঃ সারি.replace (',', '').split (' ')[2:]:
							self._add_macrolanguage (subtag, language)
					নোচেদ্যদি সারি.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_মান = সত্য
						macrolanguage = সারি.split (' ')[1]
						self._add_macrolanguage (macrolanguage, subtag)
					নোচেদ্যদি ন has_preferred_মান চ সারি.startswith ('Macrolanguage: '):
						self._add_macrolanguage (সারি.split (' ')[1], subtag)
				নোচেদ্যদি subtag_type == 'variant':
					যদি সারি.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					নোচেদ্যদি সারি.startswith ('Prefix: '):
						self.prefixes[subtag].add (সারি.split (' ')[1])
				নোচেদ্যদি সারি.startswith ('File-Date: '):
					self.header = সারি
		expect (self.header)

	def _add_macrolanguage (self, macrolanguage, language):
		বৈশ্বিক ot
		যদি language ন অন্তঃ ot.from_bcp_47:
			প্রতি l অন্তঃ self.macrolanguages.get (language, set ()):
				self._add_macrolanguage (macrolanguage, l)
		যদি macrolanguage ন অন্তঃ ot.from_bcp_47:
			প্রতি ls অন্তঃ list (self.macrolanguages.মানসমূহ ()):
				যদি macrolanguage অন্তঃ ls:
					ls.add (language)
					প্রত্যাগম
		self.macrolanguages[macrolanguage].add (language)

	def remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		প্রতি macrolanguage, languages অন্তঃ self.macrolanguages.বস্তুসমূহ ():
			প্রতি language অন্তঃ languages:
				inverted[language].append (macrolanguage)
		প্রতি language, macrolanguages অন্তঃ inverted.বস্তুসমূহ ():
			যদি len (macrolanguages) > 1:
				macrolanguages.sort (চাবি_2=অনামন্ ml: len (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.বের_করো ()
				প্রতি macrolanguage অন্তঃ macrolanguages:
					self._add_macrolanguage (biggest_macrolanguage, macrolanguage)

	def _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		প্রত্যাগম self.names[subtag].split ('\n')[0] + self.scopes.get (subtag, '')

	def get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		যদি lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		যদি lt.region:
			name += '; ' + self._get_name_piece (lt.region.upper ())
		যদি lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		প্রত্যাগম name

bcp_47 = BCP47Parser ()

ot.parse (sys.argv[1])
bcp_47.parse (sys.argv[2])

ot.add_language ('ary', 'MOR')

ot.add_language ('ath', 'ATH')

ot.add_language ('bai', 'BML')

ot.ranks['BAL'] = ot.ranks['KAR'] + 1

ot.add_language ('ber', 'BBR')

ot.remove_language_ot ('PGR')
ot.add_language ('el-polyton', 'PGR')

bcp_47.names['flm'] = 'Falam Chin'
bcp_47.scopes['flm'] = ' (retired code)'
bcp_47.macrolanguages['flm'] = {'cfm'}

ot.ranks['FNE'] = ot.ranks['TNE'] + 1

ot.add_language ('und-fonipa', 'IPPH')

ot.add_language ('und-fonnapa', 'APPH')

ot.add_language ('und-fonupa', 'UPPH')

ot.add_language ('ga-Latg', 'IRT')

ot.add_language ('hy-arevmda', 'HYE')

ot.remove_language_ot ('KGE')
ot.add_language ('und-Geok', 'KGE')

ot.add_language ('kht', 'KHN')
ot.names['KHN'] = ot.names['KHT'] + ' (Microsoft fonts)'
ot.ranks['KHN'] = ot.ranks['KHT'] + 1

ot.ranks['LCR'] = ot.ranks['MCR'] + 1

ot.names['MAL'] = 'Malayalam Traditional'
ot.ranks['MLR'] += 1

bcp_47.names['mhv'] = 'Arakanese'
bcp_47.scopes['mhv'] = ' (retired code)'

ot.add_language ('mnw-TH', 'MONT')

ot.add_language ('no', 'NOR')

ot.add_language ('oc-provenc', 'PRO')

ot.remove_language_ot ('QUZ')
ot.add_language ('qu', 'QUZ')
ot.add_language ('qub', 'QWH')
ot.add_language ('qud', 'QVI')
ot.add_language ('qug', 'QVI')
ot.add_language ('qul', 'QUH')
ot.add_language ('qup', 'QVI')
ot.add_language ('qur', 'QWH')
ot.add_language ('qus', 'QUH')
ot.add_language ('quw', 'QVI')
ot.add_language ('qux', 'QWH')
ot.add_language ('qva', 'QWH')
ot.add_language ('qvh', 'QWH')
ot.add_language ('qvj', 'QVI')
ot.add_language ('qvl', 'QWH')
ot.add_language ('qvm', 'QWH')
ot.add_language ('qvn', 'QWH')
ot.add_language ('qvo', 'QVI')
ot.add_language ('qvp', 'QWH')
ot.add_language ('qvw', 'QWH')
ot.add_language ('qvz', 'QVI')
ot.add_language ('qwa', 'QWH')
ot.add_language ('qws', 'QWH')
ot.add_language ('qxa', 'QWH')
ot.add_language ('qxc', 'QWH')
ot.add_language ('qxh', 'QWH')
ot.add_language ('qxl', 'QVI')
ot.add_language ('qxn', 'QWH')
ot.add_language ('qxo', 'QWH')
ot.add_language ('qxr', 'QVI')
ot.add_language ('qxt', 'QWH')
ot.add_language ('qxw', 'QWH')

bcp_47.macrolanguages['ro-MD'].add ('mo')

ot.remove_language_ot ('SYRE')
ot.remove_language_ot ('SYRJ')
ot.remove_language_ot ('SYRN')
ot.add_language ('und-Syre', 'SYRE')
ot.add_language ('und-Syrj', 'SYRJ')
ot.add_language ('und-Syrn', 'SYRN')

bcp_47.names['xst'] = "Silt'e"
bcp_47.scopes['xst'] = ' (retired code)'
bcp_47.macrolanguages['xst'] = {'stv', 'wle'}

ot.add_language ('xwo', 'TOD')

ot.remove_language_ot ('ZHH')
ot.remove_language_ot ('ZHP')
ot.remove_language_ot ('ZHT')
ot.remove_language_ot ('ZHTM')
bcp_47.macrolanguages['zh'].remove ('lzh')
bcp_47.macrolanguages['zh'].remove ('yue')
ot.add_language ('zh-Hant-MO', 'ZHH')
ot.add_language ('zh-Hant-MO', 'ZHTM')
ot.add_language ('zh-Hant-HK', 'ZHH')
ot.add_language ('zh-Hans', 'ZHS')
ot.add_language ('zh-Hant', 'ZHT')
ot.add_language ('zh-HK', 'ZHH')
ot.add_language ('zh-MO', 'ZHH')
ot.add_language ('zh-MO', 'ZHTM')
ot.add_language ('zh-TW', 'ZHT')
ot.add_language ('lzh', 'ZHT')
ot.add_language ('lzh-Hans', 'ZHS')
ot.add_language ('yue', 'ZHH')
ot.add_language ('yue-Hans', 'ZHS')

def rank_delta (bcp_47, ot):
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
	যদি bcp_47 == 'ak' চ ot == 'AKA':
		প্রত্যাগম -1
	যদি bcp_47 == 'tw' চ ot == 'TWI':
		প্রত্যাগম -1
	প্রত্যাগম 0

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
ot.names[DEFAULT_LANGUAGE_SYSTEM] = ''
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.মানসমূহ ()) + 1
প্রতি tricky_ot_tag অন্তঃ filter (অনামন্ tag: re.match ('[A-Z]{3}$', tag), ot.names):
	possible_bcp_47_tag = tricky_ot_tag.lower ()
	যদি possible_bcp_47_tag অন্তঃ bcp_47.names চ ন ot.from_bcp_47[possible_bcp_47_tag]:
		ot.add_language (possible_bcp_47_tag, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_tag] = set ()
ot.sort_languages ()

যদি output_rust:
	def hb_tag_rust (tag):
		যদি tag == DEFAULT_LANGUAGE_SYSTEM:
			প্রত্যাগম 'Tag::new(&[0; 4])'
		sanitize_tag (tag)
		প্রত্যাগম 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % tag)[:4])

	def get_variant_set_rust (name):
		প্রত্যাগম set (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				প্রতি n অন্তঃ re.split ('[\n(),]', name) যদি n)

	def language_name_intersection_rust (a, b):
		প্রত্যাগম get_variant_set_rust (a).intersection (get_variant_set_rust (b))

	def get_matching_language_name_rust (intersection, candidates):
		প্রত্যাগম next (iter (c প্রতি c অন্তঃ candidates যদি ন intersection.isdisjoint (get_variant_set_rust (c))))

	print ('// WARNING: this file was generated by ../../harfbuzz/src/gen-tag-table.py --rust')
	print ()
	print ('use read_fonts::types::Tag;')
	print ()
	print ('pub(crate) struct LangTag {')
	print ('    pub language: [u8; 4],')
	print ('    pub tag: Tag,')
	print ('}')
	print ()
	print ('const fn lang(language: &[u8]) -> [u8; 4] {')
	print ('    let mut bytes = [0; 4];')
	print ('    let mut i = 0;')
	print ()
	print ('    while i < language.len() {')
	print ('        bytes[i] = language[i];')
	print ('        i += 1;')
	print ('    }')
	print ()
	print ('    bytes')
	print ('}')
	print ()
	print ('use super::tag::{lang_matches, strncmp, subtag_matches};')
	print ()
	print ('#[rustfmt::skip]')
	print ('pub(crate) static OPEN_TYPE_LANGUAGES: &[LangTag] = &[')

	প্রতি language, tags অন্তঃ sorted (ot.from_bcp_47.বস্তুসমূহ ()):
		যদি language == '' বা '-' অন্তঃ language:
			অনুবর্ত
		commented_out = len (language) == 3 চ len (tags) == 1 চ language == tags[0].lower ()
		প্রতি tag অন্তঃ tags:
			print ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' যদি commented_out অন্যথা '    ', sanitize_bcp47_tag (language), hb_tag_rust (tag)), end='')
			print (' // ', end='')
			bcp_47_name = bcp_47.names.get (language, '')
			bcp_47_name_candidates = bcp_47_name.split ('\n')
			ot_name = ot.names[tag]
			scope = bcp_47.scopes.get (language, '')
			যদি tag == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			অন্যথা:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				যদি ন intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				অন্যথা:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.names[language] = name
					comment = '%s%s' % (name যদি len (name) > len (ot_name) অন্যথা ot_name, scope)
			print (sanitize_comment (comment))

	print ('];')
	print ()
	print ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	print ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	def print_subtag_matches_rust (subtag, new_সারি):
		যদি subtag:
			যদি new_সারি:
				print (' && ', end='')
			print ('subtag_matches(language, "-%s")' % sanitize_bcp47_tag (subtag), end='')

	complex_tags = collections.defaultdict (list)
	প্রতি initial, group অন্তঃ itertools.groupby ((lt_tags প্রতি lt_tags অন্তঃ [
				(LanguageTag (language), tags)
				প্রতি language, tags অন্তঃ sorted (ot.from_bcp_47.বস্তুসমূহ (),
					চাবি_2=অনামন্ i: (-len (i[0]), i[0]))
			] যদি lt_tags[0].is_complex ()),
			চাবি_2=অনামন্ lt_tags: lt_tags[0].get_group ()):
		complex_tags[initial] += group

	প্রতি initial, বস্তুসমূহ অন্তঃ sorted (complex_tags.বস্তুসমূহ ()):
		যদি initial != 'und':
			অনুবর্ত
		প্রতি lt, tags অন্তঃ বস্তুসমূহ:
			যদি ন tags:
				অনুবর্ত
			যদি lt.variant অন্তঃ bcp_47.prefixes:
				expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			print ('    if ', end='')
			print_subtag_matches_rust (lt.script, অসত্য)
			print_subtag_matches_rust (lt.region, অসত্য)
			print_subtag_matches_rust (lt.variant, অসত্য)
			print (' {')
			print ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			যদি len (tags) == 1:
				print ('        tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			অন্যথা:
				print ('        let possible_tags = &[')
				প্রতি tag অন্তঃ tags:
					print ('            %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('        ];')
				print ('        tags.extend_from_slice(possible_tags);')
			print ('        return true;')
			print ('    }')

	print ('    match language.as_bytes()[0] {')
	প্রতি initial, বস্তুসমূহ অন্তঃ sorted (complex_tags.বস্তুসমূহ ()):
		যদি initial == 'und':
			অনুবর্ত
		print ("        b'%s' => {" % sanitize_bcp47_tag (initial))
		প্রতি lt, tags অন্তঃ বস্তুসমূহ:
			যদি ন tags:
				অনুবর্ত
			print ('            if ', end='')
			script = lt.script
			region = lt.region
			যদি lt.grandfathered:
				print ('&language[1..] == "%s"' % sanitize_bcp47_tag (lt.language[1:]), end='')
			অন্যথা:
				string_literal = lt.language[1:] + '-'
				যদি script:
					string_literal += script
					script = None
					যদি region:
						string_literal += '-' + region
						region = None
				যদি string_literal[-1] == '-':
					print ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
				অন্যথা:
					print ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_tag (string_literal), end='')
			print_subtag_matches_rust (script, সত্য)
			print_subtag_matches_rust (region, সত্য)
			print_subtag_matches_rust (lt.variant, সত্য)
			print (' {')
			print ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			যদি len (tags) == 1:
				print ('                tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			অন্যথা:
				print ('                let possible_tags = &[')
				প্রতি tag অন্তঃ tags:
					print ('                    %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('                ];')
				print ('                tags.extend_from_slice(possible_tags);')
			print ('                return true;')
			print ('            }')
		print ('        }')
	print ('        _ => {}')
	print ('    }')
	print ('    false')
	print ('}')
	sys.exit (0)

print ('/* == Start of generated table == */')
print ('/*')
print (' * The following table is generated by running:')
print (' *')
print (' *   %s languagetags language-subtag-registry' % os.path.basename (sys.argv[0]))
print (' *')
print (' * on files with these headers:')
print (' *')
print (' * %s' % sanitize_header (ot.header))
print (' * %s' % sanitize_header (bcp_47.header))
print (' */')
print ()
print ('#ifndef HB_OT_TAG_TABLE_HH')
print ('#define HB_OT_TAG_TABLE_HH')
print ()

def hb_tag (tag):
	"""Convert a tag to ``HB_TAG`` form.

	Args:
		tag (str): An OpenType tag.

	Returns:
		A snippet of C++ representing ``tag``.
	"""
	যদি tag == DEFAULT_LANGUAGE_SYSTEM:
		প্রত্যাগম 'HB_TAG_NONE\t       '
	sanitize_tag (tag)
	প্রত্যাগম "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % tag)[:4])

def get_variant_set (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	প্রত্যাগম set (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			প্রতি n অন্তঃ re.split ('[\n(),]', name) যদি n)

def language_name_intersection (a, b):
	"""Return the names in common between two language names.

	Args:
		a (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.
		b (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		The normalized language names shared by ``a`` and ``b``.
	"""
	প্রত্যাগম get_variant_set (a).intersection (get_variant_set (b))

def get_matching_language_name (intersection, candidates):
	প্রত্যাগম next (iter (c প্রতি c অন্তঃ candidates যদি ন intersection.isdisjoint (get_variant_set (c))))

def same_tag (bcp_47_tag, ot_tags):
	প্রত্যাগম len (bcp_47_tag) == 3 চ len (ot_tags) == 1 চ bcp_47_tag == ot_tags[0].lower ()

def format_language_comment (language, tag):
	bcp_47_name = bcp_47.names.get (language, '')
	bcp_47_name_candidates = bcp_47_name.split ('\n')
	ot_name = ot.names[tag]
	scope = bcp_47.scopes.get (language, '')
	যদি tag == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	অন্যথা:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		যদি ন intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		অন্যথা:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.names[language] = name
			comment = '%s%s' % (name যদি len (name) > len (ot_name) অন্যথা ot_name, scope)
	প্রত্যাগম sanitize_comment (comment)

print ('static const LangTag ot_languages2[] = {')
প্রতি language, tags অন্তঃ sorted (ot.from_bcp_47.বস্তুসমূহ ()):
	যদি language == '' বা '-' অন্তঃ language বা len (language) != 2:
		অনুবর্ত
	প্রতি tag অন্তঃ tags:
		print ('  {%s,\t%s},\t/* %s */' % (
			hb_tag (language),
			hb_tag (tag),
			format_language_comment (language, tag)))
print ('};')
print ()

print ('#ifndef HB_NO_LANGUAGE_LONG')

languages3_blocked = []
languages3_singles = []
languages3_multi_ranges = []
languages3_multi_মানসমূহ = []
প্রতি language, tags অন্তঃ sorted (ot.from_bcp_47.বস্তুসমূহ ()):
	যদি language == '' বা '-' অন্তঃ language বা len (language) != 3:
		অনুবর্ত
	যদি same_tag (language, tags):
		অনুবর্ত
	যদি tags == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, tags[0]))
		অনুবর্ত
	যদি len (tags) == 1:
		languages3_singles.append ((language, tags[0]))
		অনুবর্ত
	offset = len (languages3_multi_মানসমূহ)
	languages3_multi_মানসমূহ.extend ((language, tag) প্রতি tag অন্তঃ tags)
	languages3_multi_ranges.append ((language, offset, len (tags), tags[0]))

print ('static const hb_tag_t ot_languages3_blocked[] = {')
প্রতি language, tag অন্তঃ languages3_blocked:
	print ('  %s,\t/* %s */' % (hb_tag (language), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTag ot_languages3[] = {')
প্রতি language, tag অন্তঃ languages3_singles:
	print ('  {%s,\t%s},\t/* %s */' % (
		hb_tag (language),
		hb_tag (tag),
		format_language_comment (language, tag)))
print ('};')
print ()

print ('static const hb_tag_t ot_languages3_multi_values[] = {')
প্রতি language, tag অন্তঃ languages3_multi_মানসমূহ:
	print ('  %s,\t/* %s */' % (hb_tag (tag), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTagRange ot_languages3_multi[] = {')
প্রতি language, offset, গুনতি, first_tag অন্তঃ languages3_multi_ranges:
	print ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_tag (language),
		পূর্ণাঙ্ক(offset),
		পূর্ণাঙ্ক(গুনতি),
		format_language_comment (language, first_tag)))
print ('};')
print ('#endif')
print ()

print ('/**')
print (' * hb_ot_tags_from_complex_language:')
print (' * @lang_str: a BCP 47 language tag to convert.')
print (' * @limit: a pointer to the end of the substring of @lang_str to consider for')
print (' * conversion.')
print (' * @count: maximum number of language tags to retrieve (IN) and actual number of')
print (' * language tags retrieved (OUT). If no tags are retrieved, it is not modified.')
print (' * @tags: array of size at least @language_count to store the language tag')
print (' * results')
print (' *')
print (' * Converts a multi-subtag BCP 47 language tag to language tags.')
print (' *')
print (' * Return value: Whether any language systems were retrieved.')
print (' **/')
print ('static inline bool')
print ('hb_ot_tags_from_complex_language (const char   *lang_str,')
print ('\t\t\t\t  const char   *limit,')
print ('\t\t\t\t  unsigned int *count /* IN/OUT */,')
print ('\t\t\t\t  hb_tag_t     *tags /* OUT */)')
print ('{')

def print_subtag_matches (subtag, string, new_সারি):
	যদি subtag:
		যদি new_সারি:
			print ()
			print ('\t&& ', end='')
		print ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_tag (subtag), 1 + len (subtag)), end='')

complex_tags = collections.defaultdict (list)
প্রতি initial, group অন্তঃ itertools.groupby ((lt_tags প্রতি lt_tags অন্তঃ [
			(LanguageTag (language), tags)
			প্রতি language, tags অন্তঃ sorted (ot.from_bcp_47.বস্তুসমূহ (),
				চাবি_2=অনামন্ i: (-len (i[0]), i[0]))
		] যদি lt_tags[0].is_complex ()),
		চাবি_2=অনামন্ lt_tags: lt_tags[0].get_group ()):
	complex_tags[initial] += group

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
প্রতি initial, বস্তুসমূহ অন্তঃ sorted (complex_tags.বস্তুসমূহ ()):
	যদি initial != 'und':
		অনুবর্ত
	প্রতি lt, tags অন্তঃ বস্তুসমূহ:
		যদি ন tags:
			অনুবর্ত
		subtag_len = 0
		subtag_len += 1 + len (lt.script) যদি lt.script is ন None অন্যথা 0
		subtag_len += 1 + len (lt.region) যদি lt.region is ন None অন্যথা 0
		subtag_len += 1 + len (lt.variant) যদি lt.variant is ন None অন্যথা 0
		min_subtag_len = min(subtag_len, min_subtag_len)

print ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
print ('  {')
print ("    const char *p = strchr (lang_str, '-');")
print ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
প্রতি initial, বস্তুসমূহ অন্তঃ sorted (complex_tags.বস্তুসমূহ ()):
	যদি initial != 'und':
		অনুবর্ত
	প্রতি lt, tags অন্তঃ বস্তুসমূহ:
		যদি ন tags:
			অনুবর্ত
		যদি lt.variant অন্তঃ bcp_47.prefixes:
			expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		print ('    if (', end='')
		print_subtag_matches (lt.script, 'p', অসত্য)
		print_subtag_matches (lt.region, 'p', অসত্য)
		print_subtag_matches (lt.variant, 'p', অসত্য)
		print (')')
		print ('    {')
		লিখ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		যদি len (tags) == 1:
			লিখ ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		অন্যথা:
			print ('    hb_tag_t possible_tags[] = {')
			প্রতি tag অন্তঃ tags:
				লিখ ('      %s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
				print ()
			print ('      };')
			print ('      for (i = 0; i < %s && i < *count; i++)' % len (tags))
			print ('\ttags[i] = possible_tags[i];')
			print ('      *count = i;')
		print ('      return true;')
		print ('    }')
print ('  }')
print ('out:')

print ('  switch (lang_str[0])')
print ('  {')
প্রতি initial, বস্তুসমূহ অন্তঃ sorted (complex_tags.বস্তুসমূহ ()):
	যদি initial == 'und':
		অনুবর্ত
	print ("  case '%s':" % sanitize_bcp47_tag (initial))
	প্রতি lt, tags অন্তঃ বস্তুসমূহ:
		যদি ন tags:
			অনুবর্ত
		print ('    if (', end='')
		script = lt.script
		region = lt.region
		যদি lt.grandfathered:
			print ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_tag (lt.language[1:]), end='')
		অন্যথা:
			string_literal = lt.language[1:] + '-'
			যদি script:
				string_literal += script
				script = None
				যদি region:
					string_literal += '-' + region
					region = None
			যদি string_literal[-1] == '-':
				print ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
			অন্যথা:
				print ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
		print_subtag_matches (script, 'lang_str', সত্য)
		print_subtag_matches (region, 'lang_str', সত্য)
		print_subtag_matches (lt.variant, 'lang_str', সত্য)
		print (')')
		print ('    {')
		লিখ ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		যদি len (tags) == 1:
			লিখ ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		অন্যথা:
			print ('      unsigned int i;')
			print ('      hb_tag_t possible_tags[] = {')
			প্রতি tag অন্তঃ tags:
				লিখ ('\t%s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
				print ()
			print ('      };')
			print ('      for (i = 0; i < %s && i < *count; i++)' % len (tags))
			print ('\ttags[i] = possible_tags[i];')
			print ('      *count = i;')
		print ('      return true;')
		print ('    }')
	print ('    break;')

print ('  }')
print ('  return false;')
print ('}')
print ()
print ('/**')
print (' * hb_ot_ambiguous_tag_to_language')
print (' * @tag: A language tag.')
print (' *')
print (' * Converts @tag to a BCP 47 language tag if it is ambiguous (it corresponds to')
print (' * many language tags) and the best tag is not the first (sorted alphabetically,')
print (' * with two-letter tags having priority over all three-letter tags), or if the')
print (' * best tag consists of multiple subtags, or if the best tag does not appear in')
print (' * #ot_languages2 or #ot_languages3.')
print (' *')
print (' * Return value: The #hb_language_t corresponding to the BCP 47 language tag,')
print (' * or #HB_LANGUAGE_INVALID if @tag is not ambiguous.')
print (' **/')
print ('static inline hb_language_t')
print ('hb_ot_ambiguous_tag_to_language (hb_tag_t tag)')
print ('{')
print ('  switch (tag)')
print ('  {')

def verify_disambiguation_dict ():
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
	বৈশ্বিক bcp_47
	বৈশ্বিক disambiguation
	বৈশ্বিক ot
	প্রতি ot_tag, bcp_47_tags অন্তঃ ot.to_bcp_47.বস্তুসমূহ ():
		যদি ot_tag == DEFAULT_LANGUAGE_SYSTEM:
			primary_tags = []
		অন্যথা:
			primary_tags = list (t প্রতি t অন্তঃ bcp_47_tags যদি t ন অন্তঃ bcp_47.grandfathered চ ot.from_bcp_47.get (t)[0] == ot_tag)
		যদি len (primary_tags) == 1:
			expect (ot_tag ন অন্তঃ disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_tag)
			যদি '-' অন্তঃ primary_tags[0]:
				disambiguation[ot_tag] = primary_tags[0]
			অন্যথা:
				first_tag = sorted ((t প্রতি t অন্তঃ bcp_47_tags যদি t ন অন্তঃ bcp_47.grandfathered চ ot_tag অন্তঃ ot.from_bcp_47.get (t)),
						চাবি_2=অনামন্ t: (len (t), t))[0]
				যদি primary_tags[0] != first_tag:
					disambiguation[ot_tag] = primary_tags[0]
		নোচেদ্যদি len (primary_tags) == 0:
			expect (ot_tag ন অন্তঃ disambiguation, 'There is no possible valid disambiguation for %s' % ot_tag)
		অন্যথা:
			original_languages = [t প্রতি t অন্তঃ primary_tags যদি t অন্তঃ ot.from_bcp_47_uninherited চ 'retired code' ন অন্তঃ bcp_47.scopes.get (t, '')]
			যদি len (original_languages) == 1:
				macrolanguages = original_languages
			অন্যথা:
				macrolanguages = [t প্রতি t অন্তঃ primary_tags যদি bcp_47.scopes.get (t) == ' [macrolanguage]']
			যদি len (macrolanguages) != 1:
				macrolanguages = list (t প্রতি t অন্তঃ primary_tags যদি bcp_47.scopes.get (t) == ' [collection]')
			যদি len (macrolanguages) != 1:
				macrolanguages = list (t প্রতি t অন্তঃ primary_tags যদি 'retired code' ন অন্তঃ bcp_47.scopes.get (t, ''))
			যদি len (macrolanguages) != 1:
				macrolanguages = list (t প্রতি t অন্তঃ primary_tags যদি t.lower () == ISO_639_3_TO_1.get (ot_tag.lower (), ot_tag.lower ()))
			যদি len (macrolanguages) != 1:
				macrolanguages = list (t প্রতি t অন্তঃ primary_tags যদি '-' ন অন্তঃ t)
			যদি len (macrolanguages) != 1:
				expect (ot_tag অন্তঃ disambiguation, 'ambiguous OT tag: %s %s' % (ot_tag, sorted (primary_tags)))
				expect (disambiguation[ot_tag] অন্তঃ bcp_47_tags,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_tag], ot_tag))
			নোচেদ্যদি ot_tag ন অন্তঃ disambiguation:
				disambiguation[ot_tag] = macrolanguages[0]
			যদি '-' ন অন্তঃ disambiguation[ot_tag]:
				different_bcp_47_tags = sorted ((t প্রতি t অন্তঃ bcp_47_tags যদি ন same_tag (t, ot.from_bcp_47.get (t))),
						চাবি_2=অনামন্ t: (len (t), t))
				যদি different_bcp_47_tags চ disambiguation[ot_tag] == different_bcp_47_tags[0]:
					del disambiguation[ot_tag]
	প্রতি ot_tag অন্তঃ disambiguation.চাবিগুলি ():
		expect (ot_tag অন্তঃ ot.to_bcp_47, 'unknown OT tag: %s' % ot_tag)

verify_disambiguation_dict ()
প্রতি ot_tag, bcp_47_tag অন্তঃ sorted (disambiguation.বস্তুসমূহ ()):
	লিখ ('  case %s:  /* %s */' % (hb_tag (ot_tag), sanitize_comment (ot.names[ot_tag])))
	print ()
	লিখ ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_tag (bcp_47_tag), sanitize_comment (bcp_47.get_name (LanguageTag (bcp_47_tag)))))
	print ()

print ('  default:')
print ('    return HB_LANGUAGE_INVALID;')
print ('  }')
print ('}')

print ()
print ('#endif /* HB_OT_TAG_TABLE_HH */')
print ()
print ('/* == End of generated table == */')

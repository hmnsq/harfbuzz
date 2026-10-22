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

ܐܢ len (sys.argv) > 1 ܘ sys.argv[1] == '--rust':
	del sys.argv[1]
	output_rust = ܫܪܪܐ
ܐܚܪܢܐ:
	output_rust = ܕܓܠܐ

ܐܢ len (sys.argv) != 3:
	sys.exit (__doc__)

def expect (condition, message=None):
	ܐܢ ܠܐ condition:
		ܐܢ message is None:
			ارفع AssertionError
		ارفع AssertionError (message)

def اكتب (s):
	sys.stdout.flush ()
	sys.stdout.buffer.اكتب (s.encode ('utf-8'))

_ALLOWED_COMMENT_PUNCT = set (" -—–.,;:’'ʼ()[]/!=>_+")

def sanitize_comment (s):
	"""Validate that a comment contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ܐܢ ܠܐ isinstance (s, str):
		ارفع TypeError ("Expected string for comment, got %s" % type (s).__name__)
	s = s.strip ()
	ܐܢ ܠܐ s:
		ܦܢܐ ''
	لكل ch في s:
		cat = unicodedata.category (ch)
		ܐܢ ܠܐ (cat.startswith (('L', 'M', 'N', 'Z')) ܐܘ ch في _ALLOWED_COMMENT_PUNCT):
			ارفع ValueError ("Disallowed character %r (U+%04X) in comment: %r" % (ch, ord (ch), s))
	ܦܢܐ s

def sanitize_header (s):
	"""Validate that a header string contains only safe characters and return the stripped string.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ܐܢ ܠܐ isinstance (s, str):
		ارفع TypeError ("Expected string for header, got %s" % type (s).__name__)
	s = s.strip ()
	ܐܢ ܠܐ re.fullmatch (r'[a-zA-Z0-9 <>=/:\-_."]{1,200}', s):
		ارفع ValueError ("Invalid header line %r: contains disallowed characters" % s)
	ܦܢܐ s

def sanitize_bcp47_tag (s):
	"""Validate that a BCP-47 tag or subtag contains only alphanumeric characters and hyphens.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ܐܢ ܠܐ isinstance (s, str):
		ارفع TypeError ("Expected string, got %s" % type (s).__name__)
	ܐܢ ܠܐ re.fullmatch (r'[a-zA-Z0-9-]+', s):
		ارفع ValueError ("Invalid BCP-47 tag or subtag literal %r: must contain only alphanumeric characters and hyphens" % s)
	ܦܢܐ s

def sanitize_tag (tag):
	"""Validate that an OpenType tag contains only 1-4 alphanumeric characters or spaces.

	Raises ValueError if any character outside the allowlist is present.
	"""
	ܐܢ ܠܐ isinstance (tag, str):
		ارفع TypeError ("Expected string for tag, got %s" % type (tag).__name__)
	ܐܢ tag == DEFAULT_LANGUAGE_SYSTEM:
		ܦܢܐ ''
	ܐܢ ܠܐ re.fullmatch (r'[a-zA-Z0-9 ]{1,4}', tag):
		ارفع ValueError ("Invalid tag %r: must contain only 1-4 alphanumeric characters or spaces" % tag)
	ܦܢܐ tag

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

صنف LanguageTag (object):
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
		عام bcp_47
		self.subtags = tag.lower ().split ('-')
		self.grandfathered = tag.lower () في bcp_47.grandfathered
		ܐܢ self.grandfathered:
			self.language = tag.lower ()
			self.script = ''
			self.region = ''
			self.variant = ''
		ܐܚܪܢܐ:
			self.language = self.subtags[0]
			self.script = self._find_first (مجهول s: len (s) == 4 ܘ s[0] > '9', self.subtags)
			self.region = self._find_first (مجهول s: len (s) == 2 ܘ s[0] > '9' ܐܘ len (s) == 3 ܘ s[0] <= '9', self.subtags[1:])
			self.variant = self._find_first (مجهول s: len (s) > 4 ܐܘ len (s) == 4 ܘ s[0] <= '9', self.subtags)

	def __str__(self):
		ܦܢܐ '-'.join(self.subtags)

	def __repr__ (self):
		ܦܢܐ 'LanguageTag(%r)' % str(self)

	@staticmethod
	def _find_first (function, sequence):
		حاول:
			ܦܢܐ next (iter (filter (function, sequence)))
		استثناء StopIteration:
			ܦܢܐ None

	def is_complex (self):
		"""Return whether this tag is too complex to represent as a
		``LangTag`` in the generated code.

		Complex tags need to be handled in
		``hb_ot_tags_from_complex_language``.

		Returns:
			Whether this tag is complex.
		"""
		ܦܢܐ ܠܐ (len (self.subtags) == 1
			ܐܘ self.grandfathered
			ܘ len (self.subtags[1]) != 3
			ܘ ot.from_bcp_47[self.subtags[0]] == ot.from_bcp_47[self.language])

	def get_group (self):
		"""Return the group into which this tag should be categorized in
		``hb_ot_tags_from_complex_language``.

		The group is the first letter of the tag, or ``'und'`` if this tag
		should not be matched in a ``switch`` statement in the generated
		code.

		Returns:
			This tag's group.
		"""
		ܦܢܐ ('und'
			ܐܢ (self.language == 'und'
				ܐܘ self.variant في bcp_47.prefixes ܘ len (bcp_47.prefixes[self.variant]) == 1)
			ܐܚܪܢܐ self.language[0])

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
	def __init__ (self):
		HTMLParser.__init__ (self)
		self.header = ''
		self.names = {}
		self.ranks = collections.defaultdict (صحيح)
		self.to_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47 = collections.defaultdict (set)
		self.from_bcp_47_uninherited = None
		# Whether the parser is in a <td> element
		self._td = ܕܓܠܐ
		# Whether the parser ignores the rest of the current <td> element
		self._disengaged = ܕܓܠܐ
		# The text of the <td> elements of the current <tr> element.
		self._current_tr = []

	def handle_starttag (self, tag, attrs):
		ܐܢ tag == 'a':
			ܐܢ self._current_tr ܘ ܠܐ self._disengaged:
				self._current_tr[-1] = ''
				self._disengaged = ܫܪܪܐ
		وإلاإذا tag == 'br':
			self._disengaged = ܫܪܪܐ
		وإلاإذا tag == 'meta':
			لكل attr, value في attrs:
				ܐܢ attr == 'name' ܘ value == 'updated_at':
					self.header = self.get_starttag_text ()
					اكسر
		وإلاإذا tag == 'td':
			self._td = ܫܪܪܐ
			self._current_tr.append ('')
		وإلاإذا tag == 'tr':
			self._disengaged = ܕܓܠܐ
			self._current_tr = []

	def handle_endtag (self, tag):
		ܐܢ tag == 'td':
			self._td = ܕܓܠܐ
			self._disengaged = ܕܓܠܐ
		وإلاإذا tag == 'tr' ܘ self._current_tr:
			expect (2 <= len (self._current_tr) <= 3)
			name = self._current_tr[0].strip ()
			tag = self._current_tr[1].strip ("\t\n\v\f\r '")
			rank = 0
			ܐܢ len (tag) > 4:
				expect (tag.endswith (' (deprecated)'), 'ill-formed OpenType tag: %s' % tag)
				name += ' (deprecated)'
				tag = tag.split (' ')[0]
				rank = 1
			self.names[tag] = re.sub (' languages$', '', name)
			ܐܢ ܠܐ self._current_tr[2]:
				ܦܢܐ
			iso_codes = self._current_tr[2].strip ()
			self.to_bcp_47[tag].update (ISO_639_3_TO_1.get (code, code) لكل code في iso_codes.replace (' ', '').split (','))
			rank += 2 * len (self.to_bcp_47[tag])
			self.ranks[tag] = rank

	def handle_data (self, data):
		ܐܢ self._td ܘ ܠܐ self._disengaged:
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
		with open (filename, encoding='utf-8') كـ f:
			self.feed (f.اقرأ ())
		expect (self.header)
		لكل tag, iso_codes في self.to_bcp_47.items ():
			لكل iso_code في iso_codes:
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
		عام bcp_47
		self.to_bcp_47[ot_tag].add (bcp_47_tag)
		self.from_bcp_47[bcp_47_tag].add (ot_tag)
		ܐܢ bcp_47_tag.lower () ܠܐ في bcp_47.grandfathered:
			حاول:
				[macrolanguage, suffix] = bcp_47_tag.split ('-', 1)
				ܐܢ macrolanguage في bcp_47.macrolanguages:
					s = set ()
					لكل language في bcp_47.macrolanguages[macrolanguage]:
						ܐܢ language.lower () ܠܐ في bcp_47.grandfathered:
							s.add ('%s-%s' % (language, suffix))
					bcp_47.macrolanguages['%s-%s' % (macrolanguage, suffix)] = s
			استثناء ValueError:
				pass

	@staticmethod
	def _remove_language (tag_1, dict_1, dict_2):
		لكل tag_2 في dict_1.pop (tag_1):
			dict_2[tag_2].remove (tag_1)
			ܐܢ ܠܐ dict_2[tag_2]:
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
		عام bcp_47
		first_time = self.from_bcp_47_uninherited is None
		ܐܢ first_time:
			self.from_bcp_47_uninherited = dict (self.from_bcp_47)
		لكل macrolanguage, languages في dict (bcp_47.macrolanguages).items ():
			ot_macrolanguages = {
				ot_macrolanguage لكل ot_macrolanguage في self.from_bcp_47_uninherited.get (macrolanguage, set ())
			}
			blocked_ot_macrolanguages = set ()
			ܐܢ 'retired code' ܠܐ في bcp_47.scopes.get (macrolanguage, ''):
				لكل ot_macrolanguage في ot_macrolanguages:
					round_trip_macrolanguages = {
						l لكل l في self.to_bcp_47[ot_macrolanguage]
						ܐܢ 'retired code' ܠܐ في bcp_47.scopes.get (l, '')
					}
					round_trip_languages = {
						l لكل l في languages
						ܐܢ 'retired code' ܠܐ في bcp_47.scopes.get (l, '')
					}
					intersection = round_trip_macrolanguages & round_trip_languages
					ܐܢ intersection ܘ intersection != round_trip_languages:
						blocked_ot_macrolanguages.add (ot_macrolanguage)
			ܐܢ ot_macrolanguages:
				لكل ot_macrolanguage في ot_macrolanguages:
					ܐܢ ot_macrolanguage ܠܐ في blocked_ot_macrolanguages:
						لكل language في languages:
							self.add_language (language, ot_macrolanguage)
							ܐܢ ܠܐ blocked_ot_macrolanguages:
								self.ranks[ot_macrolanguage] += 1
			وإلاإذا first_time:
				لكل language في languages:
					ܐܢ language في self.from_bcp_47_uninherited:
						ot_macrolanguages |= self.from_bcp_47_uninherited[language]
					ܐܚܪܢܐ:
						ot_macrolanguages.clear ()
					ܐܢ ܠܐ ot_macrolanguages:
						اكسر
				لكل ot_macrolanguage في ot_macrolanguages:
					self.add_language (macrolanguage, ot_macrolanguage)

	def sort_languages (self):
		"""Sort the values of ``from_bcp_47`` in ascending rank order."""
		لكل language, tags في self.from_bcp_47.items ():
			self.from_bcp_47[language] = sorted (tags,
					key=مجهول t: (self.ranks[t] + rank_delta (language, t), t))

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
		with open (filename, encoding='utf-8') كـ f:
			subtag_type = None
			subtag = None
			deprecated = ܕܓܠܐ
			has_preferred_value = ܕܓܠܐ
			line_buffer = ''
			لكل line في itertools.chain (f, ['']):
				line = line.rstrip ()
				ܐܢ line.startswith (' '):
					line_buffer += line[1:]
					استمر
				line, line_buffer = line_buffer, line
				ܐܢ line.startswith ('Type: '):
					subtag_type = line.split (' ')[1]
					deprecated = ܕܓܠܐ
					has_preferred_value = ܕܓܠܐ
				وإلاإذا line.startswith ('Subtag: ') ܐܘ line.startswith ('Tag: '):
					subtag = line.split (' ')[1]
					ܐܢ subtag_type == 'grandfathered':
						self.grandfathered.add (subtag.lower ())
				وإلاإذا line.startswith ('Description: '):
					description = line.split (' ', 1)[1].replace (' (individual language)', '')
					description = re.sub (r' (\(family\)|\((individual |macro)language\)|languages)$', '',
							description)
					ܐܢ subtag في self.names:
						self.names[subtag] += '\n' + description
					ܐܚܪܢܐ:
						self.names[subtag] = description
				وإلاإذا subtag_type == 'language' ܐܘ subtag_type == 'grandfathered':
					ܐܢ line.startswith ('Scope: '):
						scope = line.split (' ')[1]
						ܐܢ scope == 'macrolanguage':
							scope = ' [macrolanguage]'
						وإلاإذا scope == 'collection':
							scope = ' [collection]'
						ܐܚܪܢܐ:
							استمر
						self.scopes[subtag] = scope
					وإلاإذا line.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
						deprecated = ܫܪܪܐ
					وإلاإذا deprecated ܘ line.startswith ('Comments: see '):
						# If a subtag is split into multiple replacement subtags,
						# it essentially represents a macrolanguage.
						لكل language في line.replace (',', '').split (' ')[2:]:
							self._add_macrolanguage (subtag, language)
					وإلاإذا line.startswith ('Preferred-Value: '):
						# If a subtag is deprecated in favor of a single replacement subtag,
						# it is either a dialect or synonym of the preferred subtag. Either
						# way, it is close enough to the truth to consider the replacement
						# the macrolanguage of the deprecated language.
						has_preferred_value = ܫܪܪܐ
						macrolanguage = line.split (' ')[1]
						self._add_macrolanguage (macrolanguage, subtag)
					وإلاإذا ܠܐ has_preferred_value ܘ line.startswith ('Macrolanguage: '):
						self._add_macrolanguage (line.split (' ')[1], subtag)
				وإلاإذا subtag_type == 'variant':
					ܐܢ line.startswith ('Deprecated: '):
						self.scopes[subtag] = ' (retired code)' + self.scopes.get (subtag, '')
					وإلاإذا line.startswith ('Prefix: '):
						self.prefixes[subtag].add (line.split (' ')[1])
				وإلاإذا line.startswith ('File-Date: '):
					self.header = line
		expect (self.header)

	def _add_macrolanguage (self, macrolanguage, language):
		عام ot
		ܐܢ language ܠܐ في ot.from_bcp_47:
			لكل l في self.macrolanguages.get (language, set ()):
				self._add_macrolanguage (macrolanguage, l)
		ܐܢ macrolanguage ܠܐ في ot.from_bcp_47:
			لكل ls في list (self.macrolanguages.values ()):
				ܐܢ macrolanguage في ls:
					ls.add (language)
					ܦܢܐ
		self.macrolanguages[macrolanguage].add (language)

	def remove_extra_macrolanguages (self):
		"""Make every language have at most one macrolanguage."""
		inverted = collections.defaultdict (list)
		لكل macrolanguage, languages في self.macrolanguages.items ():
			لكل language في languages:
				inverted[language].append (macrolanguage)
		لكل language, macrolanguages في inverted.items ():
			ܐܢ len (macrolanguages) > 1:
				macrolanguages.sort (key=مجهول ml: len (self.macrolanguages[ml]))
				biggest_macrolanguage = macrolanguages.pop ()
				لكل macrolanguage في macrolanguages:
					self._add_macrolanguage (biggest_macrolanguage, macrolanguage)

	def _get_name_piece (self, subtag):
		"""Return the first name of a subtag plus its scope suffix.

		Args:
			subtag (str): A BCP 47 subtag.

		Returns:
			The name form of ``subtag``.
		"""
		ܦܢܐ self.names[subtag].split ('\n')[0] + self.scopes.get (subtag, '')

	def get_name (self, lt):
		"""Return the names of the subtags in a language tag.

		Args:
			lt (LanguageTag): A BCP 47 language tag.

		Returns:
			The name form of ``lt``.
		"""
		name = self._get_name_piece (lt.language)
		ܐܢ lt.script:
			name += '; ' + self._get_name_piece (lt.script.title ())
		ܐܢ lt.region:
			name += '; ' + self._get_name_piece (lt.region.upper ())
		ܐܢ lt.variant:
			name += '; ' + self._get_name_piece (lt.variant)
		ܦܢܐ name

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
	ܐܢ bcp_47 == 'ak' ܘ ot == 'AKA':
		ܦܢܐ -1
	ܐܢ bcp_47 == 'tw' ܘ ot == 'TWI':
		ܦܢܐ -1
	ܦܢܐ 0

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
ot.ranks[DEFAULT_LANGUAGE_SYSTEM] = max (ot.ranks.values ()) + 1
لكل tricky_ot_tag في filter (مجهول tag: re.match ('[A-Z]{3}$', tag), ot.names):
	possible_bcp_47_tag = tricky_ot_tag.lower ()
	ܐܢ possible_bcp_47_tag في bcp_47.names ܘ ܠܐ ot.from_bcp_47[possible_bcp_47_tag]:
		ot.add_language (possible_bcp_47_tag, DEFAULT_LANGUAGE_SYSTEM)
		bcp_47.macrolanguages[possible_bcp_47_tag] = set ()
ot.sort_languages ()

ܐܢ output_rust:
	def hb_tag_rust (tag):
		ܐܢ tag == DEFAULT_LANGUAGE_SYSTEM:
			ܦܢܐ 'Tag::new(&[0; 4])'
		sanitize_tag (tag)
		ܦܢܐ 'Tag::new(b"%s%s%s%s")' % tuple (('%-4s' % tag)[:4])

	def get_variant_set_rust (name):
		ܦܢܐ set (unicodedata.normalize (
					'NFD',
					n.replace ('\u02BC', "'").replace ('\u2019', "'"),
				)
				.encode ('ASCII', 'ignore')
				.strip ()
				لكل n في re.split ('[\n(),]', name) ܐܢ n)

	def language_name_intersection_rust (a, b):
		ܦܢܐ get_variant_set_rust (a).intersection (get_variant_set_rust (b))

	def get_matching_language_name_rust (intersection, candidates):
		ܦܢܐ next (iter (c لكل c في candidates ܐܢ ܠܐ intersection.isdisjoint (get_variant_set_rust (c))))

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

	لكل language, tags في sorted (ot.from_bcp_47.items ()):
		ܐܢ language == '' ܐܘ '-' في language:
			استمر
		commented_out = len (language) == 3 ܘ len (tags) == 1 ܘ language == tags[0].lower ()
		لكل tag في tags:
			print ('%sLangTag { language: lang(b"%s"), \ttag: %s },' % ('//  ' ܐܢ commented_out ܐܚܪܢܐ '    ', sanitize_bcp47_tag (language), hb_tag_rust (tag)), end='')
			print (' // ', end='')
			bcp_47_name = bcp_47.names.get (language, '')
			bcp_47_name_candidates = bcp_47_name.split ('\n')
			ot_name = ot.names[tag]
			scope = bcp_47.scopes.get (language, '')
			ܐܢ tag == DEFAULT_LANGUAGE_SYSTEM:
				comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
			ܐܚܪܢܐ:
				intersection = language_name_intersection_rust (bcp_47_name, ot_name)
				ܐܢ ܠܐ intersection:
					comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
				ܐܚܪܢܐ:
					name = get_matching_language_name_rust (intersection, bcp_47_name_candidates)
					bcp_47.names[language] = name
					comment = '%s%s' % (name ܐܢ len (name) > len (ot_name) ܐܚܪܢܐ ot_name, scope)
			print (sanitize_comment (comment))

	print ('];')
	print ()
	print ('/// Converts a multi-subtag BCP 47 language tag to language tags.')
	print ('pub fn tags_from_complex_language(language: &str, tags: &mut smallvec::SmallVec<[Tag; 3]>) -> bool {')

	def print_subtag_matches_rust (subtag, new_line):
		ܐܢ subtag:
			ܐܢ new_line:
				print (' && ', end='')
			print ('subtag_matches(language, "-%s")' % sanitize_bcp47_tag (subtag), end='')

	complex_tags = collections.defaultdict (list)
	لكل initial, group في itertools.groupby ((lt_tags لكل lt_tags في [
				(LanguageTag (language), tags)
				لكل language, tags في sorted (ot.from_bcp_47.items (),
					key=مجهول i: (-len (i[0]), i[0]))
			] ܐܢ lt_tags[0].is_complex ()),
			key=مجهول lt_tags: lt_tags[0].get_group ()):
		complex_tags[initial] += group

	لكل initial, items في sorted (complex_tags.items ()):
		ܐܢ initial != 'und':
			استمر
		لكل lt, tags في items:
			ܐܢ ܠܐ tags:
				استمر
			ܐܢ lt.variant في bcp_47.prefixes:
				expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
						'%s is not a valid prefix of %s' % (lt.language, lt.variant))
			print ('    if ', end='')
			print_subtag_matches_rust (lt.script, ܕܓܠܐ)
			print_subtag_matches_rust (lt.region, ܕܓܠܐ)
			print_subtag_matches_rust (lt.variant, ܕܓܠܐ)
			print (' {')
			print ('        // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ܐܢ len (tags) == 1:
				print ('        tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			ܐܚܪܢܐ:
				print ('        let possible_tags = &[')
				لكل tag في tags:
					print ('            %s, // %s' % (hb_tag_rust (tag), sanitize_comment (ot.names[tag])))
				print ('        ];')
				print ('        tags.extend_from_slice(possible_tags);')
			print ('        return true;')
			print ('    }')

	print ('    match language.as_bytes()[0] {')
	لكل initial, items في sorted (complex_tags.items ()):
		ܐܢ initial == 'und':
			استمر
		print ("        b'%s' => {" % sanitize_bcp47_tag (initial))
		لكل lt, tags في items:
			ܐܢ ܠܐ tags:
				استمر
			print ('            if ', end='')
			script = lt.script
			region = lt.region
			ܐܢ lt.grandfathered:
				print ('&language[1..] == "%s"' % sanitize_bcp47_tag (lt.language[1:]), end='')
			ܐܚܪܢܐ:
				string_literal = lt.language[1:] + '-'
				ܐܢ script:
					string_literal += script
					script = None
					ܐܢ region:
						string_literal += '-' + region
						region = None
				ܐܢ string_literal[-1] == '-':
					print ('strncmp(&language[1..], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
				ܐܚܪܢܐ:
					print ('lang_matches(&language[1..], "%s")' % sanitize_bcp47_tag (string_literal), end='')
			print_subtag_matches_rust (script, ܫܪܪܐ)
			print_subtag_matches_rust (region, ܫܪܪܐ)
			print_subtag_matches_rust (lt.variant, ܫܪܪܐ)
			print (' {')
			print ('                // %s' % sanitize_comment (bcp_47.get_name (lt)))
			ܐܢ len (tags) == 1:
				print ('                tags.push(%s); // %s' % (hb_tag_rust (tags[0]), sanitize_comment (ot.names[tags[0]])))
			ܐܚܪܢܐ:
				print ('                let possible_tags = &[')
				لكل tag في tags:
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
	ܐܢ tag == DEFAULT_LANGUAGE_SYSTEM:
		ܦܢܐ 'HB_TAG_NONE\t       '
	sanitize_tag (tag)
	ܦܢܐ "HB_TAG('%s','%s','%s','%s')" % tuple (('%-4s' % tag)[:4])

def get_variant_set (name):
	"""Return a set of variant language names from a name.

	Args:
		name (str): A list of language names from the BCP 47 registry,
			joined on ``'\\n'``.

	Returns:
		A set of normalized language names.
	"""
	ܦܢܐ set (unicodedata.normalize (
				'NFD',
				n.replace ('\u02BC', "'").replace ('\u2019', "'"),
			)
			.encode ('ASCII', 'ignore')
			.strip ()
			لكل n في re.split ('[\n(),]', name) ܐܢ n)

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
	ܦܢܐ get_variant_set (a).intersection (get_variant_set (b))

def get_matching_language_name (intersection, candidates):
	ܦܢܐ next (iter (c لكل c في candidates ܐܢ ܠܐ intersection.isdisjoint (get_variant_set (c))))

def same_tag (bcp_47_tag, ot_tags):
	ܦܢܐ len (bcp_47_tag) == 3 ܘ len (ot_tags) == 1 ܘ bcp_47_tag == ot_tags[0].lower ()

def format_language_comment (language, tag):
	bcp_47_name = bcp_47.names.get (language, '')
	bcp_47_name_candidates = bcp_47_name.split ('\n')
	ot_name = ot.names[tag]
	scope = bcp_47.scopes.get (language, '')
	ܐܢ tag == DEFAULT_LANGUAGE_SYSTEM:
		comment = f'{bcp_47_name_candidates[0]}{scope} != {ot.names[language.upper ()]}'
	ܐܚܪܢܐ:
		intersection = language_name_intersection (bcp_47_name, ot_name)
		ܐܢ ܠܐ intersection:
			comment = '%s%s -> %s' % (bcp_47_name_candidates[0], scope, ot_name)
		ܐܚܪܢܐ:
			name = get_matching_language_name (intersection, bcp_47_name_candidates)
			bcp_47.names[language] = name
			comment = '%s%s' % (name ܐܢ len (name) > len (ot_name) ܐܚܪܢܐ ot_name, scope)
	ܦܢܐ sanitize_comment (comment)

print ('static const LangTag ot_languages2[] = {')
لكل language, tags في sorted (ot.from_bcp_47.items ()):
	ܐܢ language == '' ܐܘ '-' في language ܐܘ len (language) != 2:
		استمر
	لكل tag في tags:
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
languages3_multi_values = []
لكل language, tags في sorted (ot.from_bcp_47.items ()):
	ܐܢ language == '' ܐܘ '-' في language ܐܘ len (language) != 3:
		استمر
	ܐܢ same_tag (language, tags):
		استمر
	ܐܢ tags == [DEFAULT_LANGUAGE_SYSTEM]:
		languages3_blocked.append ((language, tags[0]))
		استمر
	ܐܢ len (tags) == 1:
		languages3_singles.append ((language, tags[0]))
		استمر
	offset = len (languages3_multi_values)
	languages3_multi_values.extend ((language, tag) لكل tag في tags)
	languages3_multi_ranges.append ((language, offset, len (tags), tags[0]))

print ('static const hb_tag_t ot_languages3_blocked[] = {')
لكل language, tag في languages3_blocked:
	print ('  %s,\t/* %s */' % (hb_tag (language), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTag ot_languages3[] = {')
لكل language, tag في languages3_singles:
	print ('  {%s,\t%s},\t/* %s */' % (
		hb_tag (language),
		hb_tag (tag),
		format_language_comment (language, tag)))
print ('};')
print ()

print ('static const hb_tag_t ot_languages3_multi_values[] = {')
لكل language, tag في languages3_multi_values:
	print ('  %s,\t/* %s */' % (hb_tag (tag), format_language_comment (language, tag)))
print ('};')
print ()

print ('static const LangTagRange ot_languages3_multi[] = {')
لكل language, offset, count, first_tag في languages3_multi_ranges:
	print ('  {%s,\t%u,\t%u},\t/* %s */' % (
		hb_tag (language),
		صحيح(offset),
		صحيح(count),
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

def print_subtag_matches (subtag, string, new_line):
	ܐܢ subtag:
		ܐܢ new_line:
			print ()
			print ('\t&& ', end='')
		print ('subtag_matches (%s, limit, "-%s", %i)' % (string, sanitize_bcp47_tag (subtag), 1 + len (subtag)), end='')

complex_tags = collections.defaultdict (list)
لكل initial, group في itertools.groupby ((lt_tags لكل lt_tags في [
			(LanguageTag (language), tags)
			لكل language, tags في sorted (ot.from_bcp_47.items (),
				key=مجهول i: (-len (i[0]), i[0]))
		] ܐܢ lt_tags[0].is_complex ()),
		key=مجهول lt_tags: lt_tags[0].get_group ()):
	complex_tags[initial] += group

# Calculate the min length of the subtags outside the switch
min_subtag_len = 100
لكل initial, items في sorted (complex_tags.items ()):
	ܐܢ initial != 'und':
		استمر
	لكل lt, tags في items:
		ܐܢ ܠܐ tags:
			استمر
		subtag_len = 0
		subtag_len += 1 + len (lt.script) ܐܢ lt.script is ܠܐ None ܐܚܪܢܐ 0
		subtag_len += 1 + len (lt.region) ܐܢ lt.region is ܠܐ None ܐܚܪܢܐ 0
		subtag_len += 1 + len (lt.variant) ܐܢ lt.variant is ܠܐ None ܐܚܪܢܐ 0
		min_subtag_len = min(subtag_len, min_subtag_len)

print ('  if (limit - lang_str >= %d)' % (min_subtag_len + 2))
print ('  {')
print ("    const char *p = strchr (lang_str, '-');")
print ("    if (!p || p >= limit || limit - p < %i) goto out;" % min_subtag_len)
لكل initial, items في sorted (complex_tags.items ()):
	ܐܢ initial != 'und':
		استمر
	لكل lt, tags في items:
		ܐܢ ܠܐ tags:
			استمر
		ܐܢ lt.variant في bcp_47.prefixes:
			expect (next (iter (bcp_47.prefixes[lt.variant])) == lt.language,
					'%s is not a valid prefix of %s' % (lt.language, lt.variant))
		print ('    if (', end='')
		print_subtag_matches (lt.script, 'p', ܕܓܠܐ)
		print_subtag_matches (lt.region, 'p', ܕܓܠܐ)
		print_subtag_matches (lt.variant, 'p', ܕܓܠܐ)
		print (')')
		print ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		ܐܢ len (tags) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		ܐܚܪܢܐ:
			print ('    hb_tag_t possible_tags[] = {')
			لكل tag في tags:
				اكتب ('      %s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
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
لكل initial, items في sorted (complex_tags.items ()):
	ܐܢ initial == 'und':
		استمر
	print ("  case '%s':" % sanitize_bcp47_tag (initial))
	لكل lt, tags في items:
		ܐܢ ܠܐ tags:
			استمر
		print ('    if (', end='')
		script = lt.script
		region = lt.region
		ܐܢ lt.grandfathered:
			print ('0 == strcmp (&lang_str[1], "%s")' % sanitize_bcp47_tag (lt.language[1:]), end='')
		ܐܚܪܢܐ:
			string_literal = lt.language[1:] + '-'
			ܐܢ script:
				string_literal += script
				script = None
				ܐܢ region:
					string_literal += '-' + region
					region = None
			ܐܢ string_literal[-1] == '-':
				print ('0 == strncmp (&lang_str[1], "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
			ܐܚܪܢܐ:
				print ('lang_matches (&lang_str[1], limit, "%s", %i)' % (sanitize_bcp47_tag (string_literal), len (string_literal)), end='')
		print_subtag_matches (script, 'lang_str', ܫܪܪܐ)
		print_subtag_matches (region, 'lang_str', ܫܪܪܐ)
		print_subtag_matches (lt.variant, 'lang_str', ܫܪܪܐ)
		print (')')
		print ('    {')
		اكتب ('      /* %s */' % sanitize_comment (bcp_47.get_name (lt)))
		print ()
		ܐܢ len (tags) == 1:
			اكتب ('      tags[0] = %s;  /* %s */' % (hb_tag (tags[0]), sanitize_comment (ot.names[tags[0]])))
			print ()
			print ('      *count = 1;')
		ܐܚܪܢܐ:
			print ('      unsigned int i;')
			print ('      hb_tag_t possible_tags[] = {')
			لكل tag في tags:
				اكتب ('\t%s,  /* %s */' % (hb_tag (tag), sanitize_comment (ot.names[tag])))
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
	عام bcp_47
	عام disambiguation
	عام ot
	لكل ot_tag, bcp_47_tags في ot.to_bcp_47.items ():
		ܐܢ ot_tag == DEFAULT_LANGUAGE_SYSTEM:
			primary_tags = []
		ܐܚܪܢܐ:
			primary_tags = list (t لكل t في bcp_47_tags ܐܢ t ܠܐ في bcp_47.grandfathered ܘ ot.from_bcp_47.get (t)[0] == ot_tag)
		ܐܢ len (primary_tags) == 1:
			expect (ot_tag ܠܐ في disambiguation, 'unnecessary disambiguation for OT tag: %s' % ot_tag)
			ܐܢ '-' في primary_tags[0]:
				disambiguation[ot_tag] = primary_tags[0]
			ܐܚܪܢܐ:
				first_tag = sorted ((t لكل t في bcp_47_tags ܐܢ t ܠܐ في bcp_47.grandfathered ܘ ot_tag في ot.from_bcp_47.get (t)),
						key=مجهول t: (len (t), t))[0]
				ܐܢ primary_tags[0] != first_tag:
					disambiguation[ot_tag] = primary_tags[0]
		وإلاإذا len (primary_tags) == 0:
			expect (ot_tag ܠܐ في disambiguation, 'There is no possible valid disambiguation for %s' % ot_tag)
		ܐܚܪܢܐ:
			original_languages = [t لكل t في primary_tags ܐܢ t في ot.from_bcp_47_uninherited ܘ 'retired code' ܠܐ في bcp_47.scopes.get (t, '')]
			ܐܢ len (original_languages) == 1:
				macrolanguages = original_languages
			ܐܚܪܢܐ:
				macrolanguages = [t لكل t في primary_tags ܐܢ bcp_47.scopes.get (t) == ' [macrolanguage]']
			ܐܢ len (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_tags ܐܢ bcp_47.scopes.get (t) == ' [collection]')
			ܐܢ len (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_tags ܐܢ 'retired code' ܠܐ في bcp_47.scopes.get (t, ''))
			ܐܢ len (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_tags ܐܢ t.lower () == ISO_639_3_TO_1.get (ot_tag.lower (), ot_tag.lower ()))
			ܐܢ len (macrolanguages) != 1:
				macrolanguages = list (t لكل t في primary_tags ܐܢ '-' ܠܐ في t)
			ܐܢ len (macrolanguages) != 1:
				expect (ot_tag في disambiguation, 'ambiguous OT tag: %s %s' % (ot_tag, sorted (primary_tags)))
				expect (disambiguation[ot_tag] في bcp_47_tags,
						'%s is not a valid disambiguation for %s' % (disambiguation[ot_tag], ot_tag))
			وإلاإذا ot_tag ܠܐ في disambiguation:
				disambiguation[ot_tag] = macrolanguages[0]
			ܐܢ '-' ܠܐ في disambiguation[ot_tag]:
				different_bcp_47_tags = sorted ((t لكل t في bcp_47_tags ܐܢ ܠܐ same_tag (t, ot.from_bcp_47.get (t))),
						key=مجهول t: (len (t), t))
				ܐܢ different_bcp_47_tags ܘ disambiguation[ot_tag] == different_bcp_47_tags[0]:
					del disambiguation[ot_tag]
	لكل ot_tag في disambiguation.keys ():
		expect (ot_tag في ot.to_bcp_47, 'unknown OT tag: %s' % ot_tag)

verify_disambiguation_dict ()
لكل ot_tag, bcp_47_tag في sorted (disambiguation.items ()):
	اكتب ('  case %s:  /* %s */' % (hb_tag (ot_tag), sanitize_comment (ot.names[ot_tag])))
	print ()
	اكتب ('    return hb_language_from_string (\"%s\", -1);  /* %s */' % (sanitize_bcp47_tag (bcp_47_tag), sanitize_comment (bcp_47.get_name (LanguageTag (bcp_47_tag)))))
	print ()

print ('  default:')
print ('    return HB_LANGUAGE_INVALID;')
print ('  }')
print ('}')

print ()
print ('#endif /* HB_OT_TAG_TABLE_HH */')
print ()
print ('/* == End of generated table == */')

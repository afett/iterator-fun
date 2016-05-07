This is no production code. Don't use it, it's broken.

Thank you Markus and Alex. There goes my weekend!

	std::map<int, bool> foo;
	BOOST_FOREACH(bool x, foo | boost::adaptors::map_values) {
		...
	}

WTF, How does this work?

C++ is for designing frameworks... and it's not good at this. As a bonus
you have to put up with all the extra complexity when you actually want
to "get shit done (TM)". Can't believe we're actually using this kind
of crap.

Btw. "Real C++ programmers" will tell you that this is:
1. due to my lack of knowledge
2. not that bad
3. all fixed in C++NN (with NN >= 11)

*YOU* are full of bullshit.
   -- Linus Torvalds (Subject: Re: [RFC] Convert builin-mailinfo.c to use The Better String Library.)

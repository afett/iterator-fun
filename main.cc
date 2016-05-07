#include <iostream>
#include <iterator>
#include <map>
#include <boost/foreach.hpp>
#include <algorithm>

template <typename Iterator, typename Tag> struct iterator_adapter;

// modern C++ no way to hide...
// just like calling a funktion dont_call_me_XXXX()
namespace detail {
struct value_iterator_tag {};
}

template <typename AdapterIterator, typename BaseIterator>
class iterator_adapter_base {
public:
	bool operator==(iterator_adapter_base const& o) const
	{
		return base_iter_ == o.base_iter_;
	}

	bool operator!=(iterator_adapter_base const& o) const
	{
		return base_iter_ != o.base_iter_;
	}

	bool operator<(iterator_adapter_base const& o) const
	{
		return base_iter_ < o.base_iter_;
	}

	bool operator<=(iterator_adapter_base const& o) const
	{
		return base_iter_ <= o.base_iter_;
	}

	bool operator>=(iterator_adapter_base const& o) const
	{
		return base_iter_ >= o.base_iter_;
	}

	AdapterIterator & operator++()
	{
		++base_iter_;
		return *static_cast<AdapterIterator*>(this);
	}

	AdapterIterator operator++(int)
	{
		AdapterIterator res(base_iter_);
		++base_iter_;
		return res;
	}

	AdapterIterator & operator--()
	{
		--base_iter_;
		return *this;
	}

	AdapterIterator operator--(int)
	{
		AdapterIterator res(base_iter_);
		--base_iter_;
		return res;
	}

protected:
	iterator_adapter_base()
	:
		base_iter_()
	{ }

	iterator_adapter_base(BaseIterator const& base_iter)
	:
		base_iter_(base_iter)
	{ }

	BaseIterator base_iter_;
};

template <typename Iterator>
// yeah looks sophisticated, doesn't it? Sophisticate my arse!
class iterator_adapter<Iterator, detail::value_iterator_tag> : public iterator_adapter_base<iterator_adapter<Iterator, detail::value_iterator_tag>, Iterator > {
public:
	// STL is just broken...
	typedef typename std::iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename std::iterator_traits<Iterator>::value_type::second_type value_type;
	typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
	// uhrg yes well you see it's a const iterator, but there is this std::pair...
	typedef typename std::iterator_traits<Iterator>::value_type::second_type const* pointer;
	typedef typename std::iterator_traits<Iterator>::value_type::second_type const& reference;

	iterator_adapter() { }

	iterator_adapter(Iterator const& base_iter)
	:
		// well we don't know what this means, could be anything... so please be explicit.
		iterator_adapter_base<iterator_adapter<Iterator, detail::value_iterator_tag>, Iterator >(base_iter)
	{ }

	iterator_adapter(iterator_adapter const& o)
	:
		// oh, and mark the space at the end. C++1X finally did away with this HOORAYY! (sryly?)
		iterator_adapter_base<iterator_adapter<Iterator, detail::value_iterator_tag>, Iterator >(o.base_iter_)
	{ }

	iterator_adapter & operator=(iterator_adapter const& o)
	{
		// again... what is base_iter_? ahh this one.
		this->base_iter_ = o.base_iter_;
		return *this;
	}

	reference operator*() const
	{
		return this->base_iter_->second;
	}

	pointer operator->() const
	{
		// looks beautiful doesn't it?
		return &(this->base_iter_->second);
	}
};

template <typename Iterator>
iterator_adapter<Iterator, detail::value_iterator_tag> make_value_iterator(Iterator const& it)
{
	return iterator_adapter<Iterator, detail::value_iterator_tag>(it);
}

detail::value_iterator_tag value_adapter;

template <typename ContainerT, typename Tag>
struct range_adaptor;

template <typename ContainerT, typename Tag>
struct range_adaptor {
	range_adaptor(ContainerT & container)
	:
		begin_(container.begin()),
		end_(container.end())
	{ }

	range_adaptor(ContainerT const& container)
	:
		cbegin_(container.begin()),
		cend_(container.end())
	{ }

	// needed
	range_adaptor(range_adaptor const& o)
	{ }

	// I just *love* the typename shit.
	typedef iterator_adapter<typename ContainerT::iterator, Tag> iterator;
	typedef iterator_adapter<typename ContainerT::const_iterator, Tag> const_iterator;

	iterator begin()
	{
		return begin_;
	}

	iterator end()
	{
		return end_;
	}

	const_iterator begin() const
	{
		return cbegin_;
	}

	const_iterator end() const
	{
		return cend_;
	}

private:
	range_adaptor() {}
	range_adaptor & operator=(range_adaptor const&) {}

	iterator begin_;
	iterator end_;
	// meh. giving up.
	const_iterator cbegin_;
	const_iterator cend_;
};

// Yay let's overload some operators. 'Cause you know *frameworks*
template <typename ContainerT, typename Tag>
range_adaptor<ContainerT, Tag> operator|(ContainerT const& container, Tag)
{
	return range_adaptor<ContainerT, Tag>(container);
}

template <typename ContainerT, typename Tag>
range_adaptor<ContainerT, Tag> operator|(ContainerT & container, Tag)
{
	return range_adaptor<ContainerT, Tag>(container);
}

void foo(bool const&)
{
}

int main()
{
	typedef std::map<int, bool> map_type;
	map_type m;

	// now any decent language has had this since 1995.
	BOOST_FOREACH(bool x, m | value_adapter) {
		(void) x;
	}

	BOOST_FOREACH(bool const& x, m | value_adapter) {
		(void) x;
	}

#if 0
	// ahhrgg...
	BOOST_FOREACH(bool & x, m | value_adapter) {
		(void) x;
	}
#endif
	std::for_each(make_value_iterator(m.begin()), make_value_iterator(m.end()), foo);

	return 0;
}

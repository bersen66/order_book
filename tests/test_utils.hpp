#pragma once

#include <ostream>
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <list>

template<
		typename Key,
		typename Value,
		typename Compare,
		template<typename, typename, typename> class Map
>
std::ostream& PrintMap(std::ostream& os, const Map<Key, Value, Compare>& m)
{
	os << "{";
	bool first = true;
	for (const auto& kv: m)
	{
		if (!first)
		{
			os << ", ";
		}
		first = false;
		os << kv.first << ": " << kv.second;
	}
	return os << "}";
}

template<class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& s)
{
	os << "{";
	bool first = true;
	for (const auto& x: s)
	{
		if (!first)
		{
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "}";
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::list<T>& l)
{
	os << "[";
	bool first = true;
	for (const auto& x: l)
	{
		if (!first)
		{
			os << ", ";
		}
		first = false;
		os << x;
	}
	return os << "]";
}

template<class K, class V, class C = std::less<K>>
std::ostream& operator<<(std::ostream& os, const std::map<K, V, C>& m)
{
	return PrintMap(os, m);
}

template<class K, class V>
std::ostream& operator<<(std::ostream& os, const std::unordered_map<K, V>& m)
{
	return PrintMap(os, m);
}

#ifndef COLLECTOR_H
#define COLLECTOR_H


#include <string>
#include <vector>
#include <cassert>

#include <fstream>


class Collector
{
	std::vector<T>    _items;
	std::vector<bool> _removed_signs;
	size_t            _removed_count = 0;

	bool    invariant() const
	{
		return _items.size() == _removed_signs.size() && _removed_count <= _items.size();
	}

public:
	size_t getSize() const { return _items.size(); }

	T    getItem(size_t index) const
	{
		assert(index < _items.size());

		return _items[index];
	}

	bool isRemoved(size_t index) const
	{
		assert(index < _removed_signs.size());

		return _removed_signs[index];
	}

	void addItem(T item)
	{
		_items.emplace_back(item);
		_removed_signs.emplace_back(false);
	}

	void removeItem(size_t index)
	{
		assert(index < _items.size());
		assert(index < _removed_signs.size());

		if (!_removed_signs[index])
		{
			_removed_signs[index] = true;
			_removed_count++;
		}
	}

	void updateItem(size_t index, const T & item)
	{
		assert(index < _items.size());

		_items[index] = item;
	}

	void clean()
	{
		_items.clear();
		_removed_signs.clear();
	}

	bool loadCollection(const std::string file_name)
	{
		std::ifstream ifs(file_name, std::ios_base::binary);

		if (!ifs)
			return false;

		size_t count = readNumber<size_t>(ifs);

		_items.reserve(count);

		for (size_t i = 0; i < count; ++i)
			addItem(T::read(ifs));

		assert(invariant());

		return ifs.good();
	}

	bool saveCollection(const std::string file_name) const
	{
		assert(invariant());

		std::ofstream ofs(file_name, std::ios_base::binary);

		if (!ofs)
			return false;

		assert(_items.size() >= _removed_count);
		size_t count = _items.size() - _removed_count;

		writeNumber(ofs, count);

		for (size_t i = 0; i < count; ++i)
			if (!_removed_signs[i])
				T::write(_items[i], ofs);

		return ofs.good();
	}

};


#endif
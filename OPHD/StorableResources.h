#pragma once

#include <algorithm>
#include <array>
#include <numeric>
#include <vector>


struct StorableResources
{
	constexpr StorableResources& operator+=(const StorableResources& other)
	{
		for (size_t i = 0; i < resources.size(); ++i)
		{
			resources[i] += other.resources[i];
		}

		return *this;
	}

	constexpr StorableResources& operator-=(const StorableResources& other)
	{
		for (size_t i = 0; i < resources.size(); ++i)
		{
			resources[i] -= other.resources[i];
		}

		return *this;
	}

	constexpr bool operator<=(const StorableResources& other) const
	{
		for (size_t i = 0; i < resources.size(); ++i)
		{
			if (!(resources[i] <= other.resources[i]))
			{
				return false;
			}
		}
		return true;
	}

	constexpr bool operator<(const StorableResources& other) const
	{
		for (size_t i = 0; i < resources.size(); ++i)
		{
			if (!(resources[i] < other.resources[i]))
			{
				return false;
			}
		}
		return true;
	}

	constexpr bool operator>=(const StorableResources& other) const
	{
		return other <= *this;
	}

	constexpr bool operator>(const StorableResources& other) const
	{
		return other < *this;
	}

	constexpr StorableResources cap(int max) const
	{
		StorableResources out;
		for (std::size_t i = 0; i < resources.size(); ++i)
		{
			out.resources[i] = std::clamp(resources[i], 0, max);
		}

		return out;
	}

	constexpr bool isEmpty() const
	{
		for (size_t i = 0; i < resources.size(); ++i)
		{
			if (resources[i] > 0)
			{
				return false;
			}
		}
		return true;
	}


	// Sum of all resource types
	int total()
	{
		return std::accumulate(resources.begin(), resources.end(), 0);
	}


	std::vector<std::size_t> getIndicesWithStock()
	{
		std::vector<std::size_t> indicesWithStock;

		for (std::size_t i = 0; i < resources.size(); ++i)
		{
			if (resources[i] > 0)
			{
				indicesWithStock.push_back(i);
			}
		}

		return indicesWithStock;
	}


	std::array<int, 4> resources{};
};


constexpr StorableResources operator+(StorableResources lhs, const StorableResources& rhs)
{
	return lhs += rhs;
}

constexpr StorableResources operator-(StorableResources lhs, const StorableResources& rhs)
{
	return lhs -= rhs;
}

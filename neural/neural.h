#pragma once

#include <type_traits>
#include <cassert>
#include <utility>
#include <array>
#include <iostream>

using float_type = float;

template <typename Type>
struct real_type
{
	typedef std::remove_reference_t<std::remove_cv_t<Type>> type;
};

template <typename Type>
using real_type_t = typename real_type<Type>::type;

template <template <typename, typename> class Trait, typename Type, typename Type2>
constexpr bool are_all()
{
	return Trait<real_type_t<Type>, real_type_t<Type2>>::value;
}

template <template <typename, typename> class Trait, typename Type, typename Type2, typename Type3, typename... Args>
constexpr bool are_all()
{
	return Trait<real_type_t<Type>, real_type_t<Type2>>::value ? are_all<Trait, Type, Type3, Args...>() : false;
}

namespace nmake_array
{
	template <std::size_t iter, typename Type, typename... Args>
	constexpr std::array<Type, iter + sizeof...(Args)> impl(std::array<Type, iter + sizeof...(Args)>& arr, Args&&... args)
	{
		return arr;
	}

	template <std::size_t iter, typename Type, typename Type2, typename... Args>
	constexpr std::array<Type, iter + 1 + sizeof...(Args)> impl(std::array<Type, iter + 1 + sizeof...(Args)>& arr, Type2&& obj, Args&&... args)
	{
		arr[iter] = static_cast<Type>(obj);
		return (iter < 1 + sizeof...(Args)) ? impl<iter + 1, Type>(arr, std::forward<Args>(args)...) : arr;
	}
}

template <typename Type, typename... Args, typename = std::enable_if_t<are_all<std::is_same, Type, Args...>()>>
constexpr std::array<Type, sizeof...(Args)> make_array(Args&&... args)
{
	return std::array<Type, sizeof...(Args)>{ {std::forward<Args>(args)...} };
}

template <typename Type, typename... Args>
typename std::enable_if_t<!are_all<std::is_same, Type, Args...>(), std::array<Type, sizeof...(Args)>> make_array(Args&&... args)
{
	static_assert(are_all<std::is_convertible, Type, Args...>(), "All values must be convertible to Type.");
	std::array<Type, sizeof...(Args)> arr;
	return nmake_array::impl<0, Type>(arr, std::forward<Args>(args)...);
}

template <typename Type, typename... Args>
constexpr typename std::enable_if_t<!are_all<std::is_same, Type, Args...>(), std::array<Type, sizeof...(Args)>> make_array(std::array<Type, sizeof...(Args)>& arr, Args&&... args)
{
	static_assert(are_all<std::is_convertible, Type, Args...>(), "All values must be convertible to Type.");
	return nmake_array::impl<0, Type>(arr, std::forward<Args>(args)...);
}

template <typename Type>
struct DefaultThresoldPolicy
{
	static bool is_valid_thresold(Type&& thresold) { return true; }
};

template <typename Type>
struct DefaultWeightPolicy
{
	static bool is_valid_weight(Type&& weight) { return true; }
};

template <std::size_t n_entries = 1, typename Type = float_type, typename ThresoldPolicy = DefaultThresoldPolicy<Type>, typename WeightPolicy = DefaultWeightPolicy<Type>, typename = std::enable_if_t<(n_entries > 0)>>
class Neural
{
public:
	template <typename... Args, typename = std::enable_if_t<are_all<std::is_same, Type, Args...>()>>
	constexpr Neural(Type&& thresold, Args&&... args) : m_thresold(thresold), m_weights{ std::move(make_array<Type>(std::forward<Args>(args)...)) }
	{
		static_assert(are_all<std::is_convertible, Type, Args...>(), "Weights must be convertible to a floating type.");
		static_assert(sizeof...(args) == n_entries, "Too much/few weights entered.");
	}

	template <typename... Args>
	constexpr Neural(typename std::enable_if_t<!are_all<std::is_same, Type, Args...>(), Type> thresold, Args&&... args) : m_thresold(thresold)
	{
		static_assert(are_all<std::is_convertible, Type, Args...>(), "Weights must be convertible to a floating type.");
		static_assert(sizeof...(args) == n_entries, "Too much/few weights entered.");
		make_array(m_weights, std::forward<Args>(args)...);
	}

	template <typename... Args>
	bool compute(Args&&... args) const
	{
		static_assert(are_all<std::is_convertible, Type, Args...>(), "Entries must be convertible to a floating type.");
		static_assert(sizeof...(args) == n_entries, "Too much/few entries entered.");

		std::array<Type, n_entries> entries = std::move(make_array<Type>(std::forward<Args>(args)...));

		Type sum = 0;
		for (std::size_t i = 0; i < n_entries; ++i)
		{
			sum += entries[i] * m_weights[i];
		}
		return sum > m_thresold;
	}

	void modify_thresold(Type&& thresold)
	{
		assert(is_valid_thresold(std::forward<Type>(thresold)) && "Invalid thresold.");
		m_thresold = std::forward<Type>(thresold);
	}

	void modify_weight(std::size_t index, Type&& weight)
	{
		assert(index < m_weights.size() && index >= 0 && "Index out of range.");
		assert(is_valid_weight(std::forward<Type>(weight)) && "Invalid weight.");

		m_weights[index] = std::forward<Type>(weight);
	}

	template <typename... Args>
	void modify_weights(Args&&... args)
	{
		static_assert(are_all<std::is_convertible, Type, Args...>(), "Entries must be convertible to a floating type.");
		static_assert(sizeof...(args) == n_entries, "Too much/few entries entered.");
		assert(are_valid_weights(std::forward<Args>(args)...) && "At least one weight is invalid.");

		m_weights = std::move(make_array<Type>(std::forward<Args>(args)...));
	}

private:
	inline bool is_valid_thresold(Type&& thresold) const { return ThresoldPolicy::is_valid_thresold(std::forward<Type>(thresold)); }
	inline bool is_valid_weight(Type&& weight) const { return WeightPolicy::is_valid_weight(std::forward<Type>(weight)); }
	
	template <typename Type2>
	bool are_valid_weights(Type2&& weight)
	{
		return is_valid_weight(static_cast<Type>(std::forward<Type2>(weight))); // grand mystere : static_cast necessaire sur msvc15 uniquement
	}

	template <typename... Args>
	bool are_valid_weights(Type&& weight, Args&&... args)
	{
		return is_valid_weight(std::forward<Type>(weight)) ? are_valid_weights(std::forward<Args>(args)...) : false;
	}

	Type m_thresold;
	std::array<Type, n_entries> m_weights;
};

#pragma once

#include <map>
#include <functional>
#include <memory>

namespace a_game_engine
{
	class ResourceLoader
	{
	public:
		template <typename ResourceType, typename PathT>
		static ResourceType& defaultLoad(
			std::map<PathT, std::unique_ptr<ResourceType>>& resources,
			const PathT& path,
			std::function<std::unique_ptr<ResourceType>(const PathT& p)> resReader,
			std::function<ResourceType& ()> getDefault)
		{
			auto it = resources.find(path);
			if (it != resources.end()) // found
			{
				if (it->second == nullptr) // but failed
					return getDefault();
				return *it->second;
			}

			auto res = resReader(path);
			if (res)
			{
				auto& newRes = resources[path];
				newRes = std::move(res);
				return *newRes.get();
			}
			resources[path] = nullptr;
			return getDefault();
		}
	};
}

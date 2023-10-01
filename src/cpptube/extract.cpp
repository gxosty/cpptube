#include <cpptube/extract.hpp>
#include <cpptube/helpers.hpp>

namespace cpptube::extract
{
	std::string video_id(const std::string& url)
	{
		return cpptube::helpers::regex_search("(?:v=|\\/)([0-9A-Za-z_-]{11}).*", url, 1);
	}
}
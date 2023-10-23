#include <cpptube/itags.hpp>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <unordered_map>
#include <string>
#include <vector>

namespace cpptube::itags
{
	std::unordered_map<int, std::pair<std::string, std::string>> PROGRESSIVE_VIDEO = {
		{5, {"240p", "64kbps"}},
		{6, {"270p", "64kbps"}},
		{13, {"144p", ""}},
		{17, {"144p", "24kbps"}},
		{18, {"360p", "96kbps"}},
		{22, {"720p", "192kbps"}},
		{34, {"360p", "128kbps"}},
		{35, {"480p", "128kbps"}},
		{36, {"240p", ""}},
		{37, {"1080p", "192kbps"}},
		{38, {"3072p", "192kbps"}},
		{43, {"360p", "128kbps"}},
		{44, {"480p", "128kbps"}},
		{45, {"720p", "192kbps"}},
		{46, {"1080p", "192kbps"}},
		{59, {"480p", "128kbps"}},
		{78, {"480p", "128kbps"}},
		{82, {"360p", "128kbps"}},
		{83, {"480p", "128kbps"}},
		{84, {"720p", "192kbps"}},
		{85, {"1080p", "192kbps"}},
		{91, {"144p", "48kbps"}},
		{92, {"240p", "48kbps"}},
		{93, {"360p", "128kbps"}},
		{94, {"480p", "128kbps"}},
		{95, {"720p", "256kbps"}},
		{96, {"1080p", "256kbps"}},
		{100, {"360p", "128kbps"}},
		{101, {"480p", "192kbps"}},
		{102, {"720p", "192kbps"}},
		{132, {"240p", "48kbps"}},
		{151, {"720p", "24kbps"}},
		{300, {"720p", "128kbps"}},
		{301, {"1080p", "128kbps"}},
	};

	std::unordered_map<int, std::pair<std::string, std::string>> DASH_VIDEO = {
		{133, {"240p", ""}},  // MP4
		{134, {"360p", ""}},  // MP4
		{135, {"480p", ""}},  // MP4
		{136, {"720p", ""}},  // MP4
		{137, {"1080p", ""}},  // MP4
		{138, {"2160p", ""}},  // MP4
		{160, {"144p", ""}},  // MP4
		{167, {"360p", ""}},  // WEBM
		{168, {"480p", ""}},  // WEBM
		{169, {"720p", ""}},  // WEBM
		{170, {"1080p", ""}},  // WEBM
		{212, {"480p", ""}},  // MP4
		{218, {"480p", ""}},  // WEBM
		{219, {"480p", ""}},  // WEBM
		{242, {"240p", ""}},  // WEBM
		{243, {"360p", ""}},  // WEBM
		{244, {"480p", ""}},  // WEBM
		{245, {"480p", ""}},  // WEBM
		{246, {"480p", ""}},  // WEBM
		{247, {"720p", ""}},  // WEBM
		{248, {"1080p", ""}},  // WEBM
		{264, {"1440p", ""}},  // MP4
		{266, {"2160p", ""}},  // MP4
		{271, {"1440p", ""}},  // WEBM
		{272, {"4320p", ""}},  // WEBM
		{278, {"144p", ""}},  // WEBM
		{298, {"720p", ""}},  // MP4
		{299, {"1080p", ""}},  // MP4
		{302, {"720p", ""}},  // WEBM
		{303, {"1080p", ""}},  // WEBM
		{308, {"1440p", ""}},  // WEBM
		{313, {"2160p", ""}},  // WEBM
		{315, {"2160p", ""}},  // WEBM
		{330, {"144p", ""}},  // WEBM
		{331, {"240p", ""}},  // WEBM
		{332, {"360p", ""}},  // WEBM
		{333, {"480p", ""}},  // WEBM
		{334, {"720p", ""}},  // WEBM
		{335, {"1080p", ""}},  // WEBM
		{336, {"1440p", ""}},  // WEBM
		{337, {"2160p", ""}},  // WEBM
		{394, {"144p", ""}},  // MP4
		{395, {"240p", ""}},  // MP4
		{396, {"360p", ""}},  // MP4
		{397, {"480p", ""}},  // MP4
		{398, {"720p", ""}},  // MP4
		{399, {"1080p", ""}},  // MP4
		{400, {"1440p", ""}},  // MP4
		{401, {"2160p", ""}},  // MP4
		{402, {"4320p", ""}},  // MP4
		{571, {"4320p", ""}},  // MP4
		{694, {"144p", ""}},  // MP4
		{695, {"240p", ""}},  // MP4
		{696, {"360p", ""}},  // MP4
		{697, {"480p", ""}},  // MP4
		{698, {"720p", ""}},  // MP4
		{699, {"1080p", ""}},  // MP4
		{700, {"1440p", ""}},  // MP4
		{701, {"2160p", ""}},  // MP4
		{702, {"4320p", ""}},  // MP4
	};

	std::unordered_map<int, std::pair<std::string, std::string>> DASH_AUDIO = {
		{139, {"", "48kbps"}},  // MP4
		{140, {"", "128kbps"}},  // MP4
		{141, {"", "256kbps"}},  // MP4
		{171, {"", "128kbps"}},  // WEBM
		{172, {"", "256kbps"}},  // WEBM
		{249, {"", "50kbps"}},  // WEBM
		{250, {"", "70kbps"}},  // WEBM
		{251, {"", "160kbps"}},  // WEBM
		{256, {"", "192kbps"}},  // MP4
		{258, {"", "384kbps"}},  // MP4
		{325, {"", ""}},  // MP4
		{328, {"", ""}},  // MP4
	};

	std::vector<int> HDR = {330, 331, 332, 333, 334, 335, 336, 337};

	std::vector<int> _3D = {82, 83, 84, 85, 100, 101, 102};

	std::vector<int> LIVE = {91, 92, 93, 94, 95, 96, 132, 151};

	nlohmann::json get_format_profile(const int& itag)
	{
		std::string res = "";
		std::string bitrate = "";
		bool is_dash = false;

		if (PROGRESSIVE_VIDEO.find(itag) != PROGRESSIVE_VIDEO.end())
		{
			res = PROGRESSIVE_VIDEO[itag].first;
			bitrate = PROGRESSIVE_VIDEO[itag].second;
		}
		else if (DASH_VIDEO.find(itag) != DASH_VIDEO.end())
		{
			res = DASH_VIDEO[itag].first;
			bitrate = DASH_VIDEO[itag].second;
			is_dash = true;
		}
		else if (DASH_AUDIO.find(itag) != DASH_AUDIO.end())
		{
			res = DASH_AUDIO[itag].first;
			bitrate = DASH_AUDIO[itag].second;
			is_dash = true;
		}

		return {
			{"resolution", res},
			{"abr", bitrate},
			{"is_live", std::find(LIVE.begin(), LIVE.end(), itag) != LIVE.end()},
			{"is_3d", std::find(_3D.begin(), _3D.end(), itag) != _3D.end()},
			{"is_hdr", std::find(HDR.begin(), HDR.end(), itag) != HDR.end()},
			{"is_dash", is_dash},
		};
	}
}
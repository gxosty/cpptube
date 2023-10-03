#include <cpptube/extract.hpp>
#include <cpptube/helpers.hpp>

namespace cpptube::extract
{
	std::string video_id(const std::string& url)
	{
		return cpptube::helpers::regex_search("(?:v=|\\/)([0-9A-Za-z_-]{11}).*", url, 1);
	}

	std::string js_url(const std::string* watch_html)
	{
		return "";
	}

	nlohmann::json apply_descrambler(nlohmann::json* stream_data)
	{
		nlohmann::json formats;
		if (stream_data->contains("formats"))
		{
			auto& stream_data__formats = stream_data->at("formats");
			formats.insert(stream_data__formats.begin(), stream_data__formats.end());
		}
		if (stream_data->contains("adaptiveFormats"))
		{
			auto& stream_data__adaptiveFormats = stream_data->at("adaptiveFormats");
			formats.insert(stream_data__adaptiveFormats.begin(), stream_data__adaptiveFormats.end());
		}

		for (auto& data : formats.items())
		{
			if (!data.value().contains("url"))
			{
				if (data.value().contains("signatureCipher"))
				{
					nlohmann::json cipher_url = cpptube::helpers::parse_qs(data.value()["signatureCipher"]);
					data.value()["url"] = cipher_url["url"][0].get<std::string>();
					data.value()["s"] = cipher_url["s"][0].get<std::string>();
				}
			}
			data.value()["is_otf"] = (bool)(data.value()["type"].get<std::string>() == "FORMAT_STREAM_TYPE_OTF");
		}
		return formats;
	}

	void apply_signature(nlohmann::json* stream_manifest, nlohmann::json* vid_info, std::string* js)
	{

	}
}